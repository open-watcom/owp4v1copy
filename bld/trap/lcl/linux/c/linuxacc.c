/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Main Linux specific debugger trap file OS access module.
*
****************************************************************************/

/* started by Kendall Bennett
   continued by Bart Oldeman -- thanks to Andi Kleen's stepper.c example
   5.5.2004 added XMM registers support JM

   still to do:
   * complete watchpoints
   * combine global into a struct (like the QNX trap file) to make it a little
     clearer what is global and what not.
   * implement thread support
   * implement corefile post-mortem support
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include "trpimp.h"
#include "trperr.h"
#include "mad.h"
#include "madregs.h"
#include "exeelf.h"
#include "linuxcomm.h"

u_short                 flatCS;
u_short                 flatDS;
pid_t                   pid;
long                    orig_eax;
long                    last_eip;

static pid_t            OrigPGrp;
static int              attached;
static int              last_sig;
static int              at_end;
static struct r_debug   rdebug;         /* Copy of debuggee's r_debug (if present) */
static struct r_debug   *dbg_rdebug;    /* Ptr to r_debug in debuggee's space */
static int              have_rdebug;    /* Flag indicating valid r_debug */
static Elf32_Dyn        *dbg_dyn;       /* VA of debuggee's dynamic section (if present) */
static bp_t             old_ld_bp;

#ifdef DEBUG_OUT
void Out( const char *str )
{
    write( 1, (char *)str, strlen( str ) );
}

void OutNum( unsigned long i )
{
    char numbuff[16];
    char *ptr;

    ptr = numbuff+10;
    *--ptr = '\0';
    do {
        *--ptr = ( i % 16 ) + '0';
        if( *ptr > '9' )
        *ptr += 'A' - '9' - 1;
        i /= 16;
    } while( i != 0 );
    Out( ptr );
}
#endif

unsigned WriteMem( void *ptr, addr_off offv, unsigned size )
{
    char    *data = ptr;
    int     count;

    /* Write the process memory 32-bits at a time. Kind of silly that
     * Linux does not have an extended ptrace call to read and write
     * blocks of data from the debuggee process, but this is what we
     * need to do for now.
     */
    for( count = size; count >= 4; count -= 4 ) {
        if( ptrace( PTRACE_POKETEXT, pid, (void *)offv, (void *)(*(unsigned_32*)data) ) != 0 )
            return( size - count );
        data += 4;
        offv += 4;
    }

    /* Now handle last partial write if neccesary. Note that we first
     * must read the full 32-bit value, then just change the section
     * we want to update.
     */
    if( count ) {
        u_long  val;

        errno = 0;
        if( (val = ptrace( PTRACE_PEEKTEXT, pid, (void *)offv, &val )) == -1 ) {
            if( errno )
                return( size - count );
        }
#if DEBUG_WRITEMEM
        Out( "writemem:" );
        OutNum( val );
        Out( "\n" );
#endif
        switch( count ) {
        case 1:
            val &= 0xFFFFFF00;
            val |= (u_long)(*((unsigned_8*)data));
            break;
        case 2:
            val &= 0xFFFF0000;
            val |= (u_long)(*((unsigned_16*)data));
            break;
        case 3:
            val &= 0xFF000000;
            val |= ((u_long)(*((unsigned_8*)(data+0))) << 0) |
                   ((u_long)(*((unsigned_8*)(data+1))) << 8) |
                   ((u_long)(*((unsigned_8*)(data+2))) << 16);
            break;
        }
#if DEBUG_WRITEMEM
        Out( "writemem:" );
        OutNum( val );
        Out( "\n" );
#endif
        if( ptrace( PTRACE_POKETEXT, pid, (void *)offv, (void *)val ) != 0 )
            return( size - count );
    }

    return( size );
}

unsigned ReadMem( void *ptr, addr_off offv, unsigned size )
{
    char    *data = ptr;
    int     count;

    /* Read the process memory 32-bits at a time */
    for( count = size; count >= 4; count -= 4 ) {
        u_long  val;
	
	errno = 0;
        if( (val = ptrace( PTRACE_PEEKTEXT, pid, (void *)offv, &val )) == -1 ) {
            if( errno ) {
                return( size - count );
            }
        }
	*(u_long *)data = val;
        data += 4;
        offv += 4;
        }

    /* Now handle last partial read if neccesary */
    if( count ) {
        u_long  val;

        errno = 0;
        if( (val = ptrace( PTRACE_PEEKTEXT, pid, (void *)offv, &val )) == -1 ) {
            if( errno ) {
                return( size - count );
            }
        }
        CONV_LE_32( val );
        switch( count ) {
        case 1:
            *((unsigned_8*)data) = (unsigned_8)val;
            break;
        case 2:
            *((unsigned_16*)data) = (unsigned_16)val;
            break;
        case 3:
            *((unsigned_8*)(data+0)) = (unsigned_8)(val >> 0);
            *((unsigned_8*)(data+1)) = (unsigned_8)(val >> 8);
            *((unsigned_8*)(data+2)) = (unsigned_8)(val >> 16);
            break;
        }
    count = 0;
    }
    return( size - count );
}

Elf32_Dyn *GetDebuggeeDynSection( const char *exe_name )
{
    Elf32_Dyn   *result;
    Elf32_Ehdr  ehdr;
    Elf32_Phdr  phdr;
    int         fd;
    size_t      i;

    result = NULL;
    fd = open( exe_name, O_RDONLY );
    if( fd < 0 )
        return( result );
    /* Obtain the address of the dynamic section from the program
     * header. If anything unexpected happens, give up
     */
    if( read( fd, &ehdr, sizeof( ehdr ) ) >= sizeof( ehdr ) &&
        memcmp( ehdr.e_ident, ELF_SIGNATURE, 4 ) == 0 &&
        ehdr.e_phoff != 0 &&
        ehdr.e_phentsize >= sizeof( phdr ) &&
        lseek( fd, ehdr.e_phoff, SEEK_SET ) == ehdr.e_phoff ) {
        for( i = 0; i < ehdr.e_phnum; i++ ) {
            if( read( fd, &phdr, sizeof phdr ) < sizeof( phdr ) )
                break;
            if( phdr.p_type == PT_DYNAMIC ) {
                result = (Elf32_Dyn *)phdr.p_vaddr;
                break;
            }
            if( lseek( fd, ehdr.e_phentsize - sizeof( phdr ), SEEK_CUR ) < 0 )
                break;
        }
    }
    close( fd );
    Out( "DynSection at: " );
    OutNum( (size_t)result );
    Out( "\n" );
    return( result );
}

/* Copy dynamic linker rendezvous structure from debuggee's address space
 * to memory provided by caller. Note that it is perfectly valid for this
 * function to fail - that will happen if the debuggee is statically linked.
 */
int Get_ld_info( struct r_debug *debug_ptr, struct r_debug **dbg_rdebug_ptr )
{
    Elf32_Dyn       loc_dyn;
    struct r_debug  *rdebug = NULL;
    unsigned        read_len;

    if( dbg_dyn == NULL ) {
        Out( "Get_ld_info: dynamic section not available\n" );
        return( FALSE );
    }
    read_len = sizeof( loc_dyn );
    if( ReadMem( &loc_dyn, (addr_off)dbg_dyn, read_len ) != read_len ) {
        Out( "Get_ld_info: failed to copy first dynamic entry\n" );
        return( FALSE );
    }
    while( loc_dyn.d_tag != DT_NULL ) {
        if( loc_dyn.d_tag == DT_DEBUG ) {
            rdebug = (struct r_debug *)loc_dyn.d_un.d_ptr;
            break;
        }
        dbg_dyn++;
        if( ReadMem( &loc_dyn, (addr_off)dbg_dyn, read_len ) != read_len ) {
            Out( "Get_ld_info: failed to copy dynamic entry\n" );
            return( FALSE );
        }
    }
    if( rdebug == NULL ) {
        Out( "Get_ld_info: failed to find DT_DEBUG entry\n" );
        return( FALSE );
    }
    read_len = sizeof( *debug_ptr );
    if( ReadMem( debug_ptr, (addr_off)rdebug, read_len ) != read_len ) {
        Out( "Get_ld_info: failed to copy r_debug struct\n" );
        return( FALSE );
    }
    *dbg_rdebug_ptr = rdebug;
    Out( "Get_ld_info: dynamic linker rendezvous structure found\n" );
    return( TRUE );
}

/* Like strcpy() but source string is in debuggee's address space. Not
 * very efficient, use sparingly!
 */
char *dbg_strcpy( char *s1, const char *s2 )
{
    char    *dst = s1;
    char    c;

    do {
        if( ReadMem( &c, (addr48_off)s2, 1 ) != 1 ) {
            Out( "dbg_strcpy: failed at " );
            OutNum( (addr48_off)s2 );
            Out( "\n" );
            return( NULL );
        }
        *dst++ = c;
        ++s2;
    } while( c );

    return( s1 );
}

/* Copy a link map struct from debuggee address space to memory
 * provided by caller.
 */
int GetLinkMap( struct link_map *dbg_lmap, struct link_map *local_lmap )
{
    unsigned    read_len;

    read_len = sizeof( *local_lmap );
    if( ReadMem( local_lmap, (addr_off)dbg_lmap, read_len ) != read_len ) {
        Out( "GetLinkMap: failed to copy link_map struct at " );
        OutNum( (addr48_off)dbg_lmap );
        Out( "\n" );
        return( FALSE );
    }
    return( TRUE );
}

unsigned ReqChecksum_mem( void )
{
    char                buf[256];
    addr_off            offv;
    u_short             length;
    u_short             size;
    int                 i;
    u_short             amount;
    u_long              sum;
    checksum_mem_req    *acc;
    checksum_mem_ret    *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    sum = 0;
    if( pid != 0 ) {
        length = acc->len;
        offv = acc->in_addr.offset;
        for( ;; ) {
            if( length == 0 )
                break;
            size = (length > sizeof( buf )) ? sizeof( buf ) : length;
            amount = ReadMem( buf, offv, size );
            for( i = amount; i != 0; --i )
                sum += buf[ i - 1 ];
            offv += amount;
            length -= amount;
            if( amount != size ) break;
        }
    }
    ret->result = sum;
    return( sizeof( *ret ) );
}

unsigned ReqRead_mem( void )
{
    read_mem_req    *acc;
    unsigned        len;

    acc = GetInPtr( 0 );
    CONV_LE_32( acc->mem_addr.offset );
    CONV_LE_16( acc->mem_addr.segment );
    CONV_LE_16( acc->len );
    len = ReadMem( GetOutPtr( 0 ), acc->mem_addr.offset, acc->len );
    return( len );
}

unsigned ReqWrite_mem( void )
{
    write_mem_req   *acc;
    write_mem_ret   *ret;
    unsigned        len;

    acc = GetInPtr( 0 );
    CONV_LE_32( acc->mem_addr.offset );
    CONV_LE_16( acc->mem_addr.segment );
    ret = GetOutPtr( 0 );
    len = GetTotalSize() - sizeof( *acc );
    ret->len = WriteMem( GetInPtr( sizeof( *acc ) ), acc->mem_addr.offset, len );
    CONV_LE_16( ret->len );
    return( sizeof( *ret ) );
}

#if defined( MD_x86 )
static int GetFlatSegs( u_short *cs, u_short *ds )
{
    user_regs_struct    regs;

    if( ptrace( PTRACE_GETREGS, pid, NULL, &regs ) == 0 ) {
        *cs = regs.cs;
    *ds = regs.ds;
    return( TRUE );
    }
    return( FALSE );
}
#endif

static int SplitParms( char *p, char *args[], unsigned len )
{
    int     i;
    char    endc;

    i = 0;
    if( len == 1 ) goto done;
    for( ;; ) {
        for( ;; ) {
            if( len == 0 ) goto done;
            if( *p != ' ' && *p != '\t' ) break;
            ++p;
            --len;
        }
        if( len == 0 ) goto done;
        if( *p == '"' ) {
            --len;
            ++p;
            endc = '"';
        } else {
            endc = ' ';
        }
        if( args != NULL ) args[i] = p;
        ++i;
        for( ;; ) {
            if( len == 0 ) goto done;
            if( *p == endc
                || *p == '\0'
                || (endc == ' ' && *p == '\t' ) ) {
                if( args != NULL ) {
                    *p = '\0';  //NYI: not a good idea, should make a copy
                }
                ++p;
                --len;
                if( len == 0 ) goto done;
                break;
            }
            ++p;
            --len;
        }
    }
done:
    return( i );
}

static pid_t RunningProc( char *name, char **name_ret )
{
    pid_t       pidd;
    char        ch;
    char        *start;

    start = name;

    for( ;; ) {
        ch = *name;
        if( ch != ' ' && ch != '\t' ) break;
        ++name;
    }
    if( name_ret != NULL ) *name_ret = name;
    pidd = 0;
    for( ;; ) {
        if( *name < '0' || *name > '9' ) break;
        pidd = (pidd*10) + (*name - '0');
        ++name;
    }
    if( *name != '\0') return( 0 );
    return( pidd );
}

static int GetExeNameFromPid( pid_t pid, char *buffer, int max_len )
{
    char        procfile[24];
    int         len;

    sprintf( procfile, "/proc/%d/exe", pid );
    len = readlink( procfile, buffer, max_len );
    if( len < 0 )
        len = 0;
    buffer[len] = '\0';
    return( len );
}

unsigned ReqProg_load( void )
{
    char                        **args;
    char                        *parms;
    char                        *parm_start;
    int                         i;
    char                        exe_name[PATH_MAX];
    char                        *name;
    pid_t                       save_pgrp;
    prog_load_req               *acc;
    prog_load_ret               *ret;
    unsigned                    len;
    int                         status;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

    last_sig = -1;
    have_rdebug = FALSE;
    dbg_dyn = NULL;
    at_end = FALSE;
    parms = (char *)GetInPtr( sizeof( *acc ) );
    parm_start = parms;
    len = GetTotalSize() - sizeof( *acc );
    if( acc->true_argv ) {
        i = 1;
        for( ;; ) {
            if( len == 0 ) break;
            if( *parms == '\0' ) {
                i++;
            }
            ++parms;
            --len;
        }
        args = alloca( i * sizeof( *args ) );
        parms = parm_start;
        len = GetTotalSize() - sizeof( *acc );
        i = 1;
        for( ;; ) {
            if( len == 0 ) break;
            if( *parms == '\0' ) {
                args[ i++ ] = parms + 1;
            }
            ++parms;
            --len;
        }
        args[ i-1 ] = NULL;
    } else {
        while( *parms != '\0' ) {
            ++parms;
            --len;
        }
        ++parms;
        --len;
        i = SplitParms( parms, NULL, len );
        args = alloca( (i + 2)  * sizeof( *args ) );
        args[ SplitParms( parms, &args[1], len ) + 1 ] = NULL;
    }
    args[0] = parm_start;
    attached = TRUE;
    pid = RunningProc( args[0], &name );
    if( pid == 0 || ptrace( PTRACE_ATTACH, pid, NULL, NULL ) == -1 ) {
        attached = FALSE;
        args[0] = name;
        if( FindFilePath( TRUE, args[0], exe_name ) == 0 ) {
            exe_name[0] = '\0';
        }
        save_pgrp = getpgrp();
        setpgid( 0, OrigPGrp );
        pid = fork();
        if( pid == -1 )
            return( 0 );
        if( pid == 0 ) {
            if( (long)ptrace( PTRACE_TRACEME, 0, NULL, NULL ) < 0 ) {
                exit( 1 );
            }
            execve( exe_name, (const char **)args, (const char **)dbg_environ );
            exit( 1 ); /* failsafe */
        }
        setpgid( 0, save_pgrp );
    } else if( pid ) {
        GetExeNameFromPid( pid, exe_name, PATH_MAX );
    }
    ret->flags = 0;
    ret->mod_handle = 0;
    if( (pid != -1) && (pid != 0) ) {
        int status;

        ret->task_id = pid;
        ret->flags |= LD_FLAG_IS_PROT | LD_FLAG_IS_32;
        /* wait until it hits _start (upon execve) or
           gives us a SIGSTOP (if attached) */
        if( waitpid ( pid, &status, 0 ) < 0 )
            goto fail;
        if( !WIFSTOPPED( status ) )
            goto fail;
        if( attached ) {
            ret->flags |= LD_FLAG_IS_STARTED;
            if( WSTOPSIG( status ) != SIGSTOP )
                goto fail;
        } else {
            if( WSTOPSIG( status ) != SIGTRAP )
                goto fail;
        }

#if defined( MD_x86 )
        if( !GetFlatSegs( &flatCS, &flatDS ) )
            goto fail;
#endif

        dbg_dyn = GetDebuggeeDynSection( exe_name );
        AddProcess();
        errno = 0;
    }
    ret->err = errno;
    if( ret->err != 0 ) {
        pid = 0;
    }
    CONV_LE_32( ret->err );
    CONV_LE_32( ret->task_id );
    CONV_LE_32( ret->mod_handle );
    return( sizeof( *ret ) );
fail:
    if( pid != 0 && pid != -1 ) {
        if( attached ) {
            ptrace( PTRACE_DETACH, pid, NULL, NULL );
            attached = FALSE;
        } else {
            ptrace( PTRACE_KILL, pid, NULL, NULL );
            waitpid( pid, &status, 0 );
        }
    }
    pid = 0;
    CONV_LE_32( ret->err );
    CONV_LE_32( ret->task_id );
    CONV_LE_32( ret->mod_handle );
    return( 0 );
}

unsigned ReqProg_kill( void )
{
    prog_kill_ret   *ret;

    if( pid != 0 && !at_end ) {
        if( attached ) {
            ptrace( PTRACE_DETACH, pid, NULL, NULL );
            attached = FALSE;
        } else {
            int status;
    
            ptrace( PTRACE_KILL, pid, NULL, NULL );
            waitpid( pid, &status, 0 );
        }
    }
    DelProcess();
    at_end = FALSE;
    pid = 0;
    ret = GetOutPtr( 0 );
    ret->err = 0;
    CONV_LE_32( ret->err );
    return( sizeof( *ret ) );
}

unsigned ReqSet_break( void )
{
    set_break_req   *acc;
    set_break_ret   *ret;
    bp_t            opcode;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    CONV_LE_32( acc->break_addr.offset );
    CONV_LE_16( acc->break_addr.segment );
    ReadMem( &opcode, acc->break_addr.offset, sizeof( opcode ) );
    ret->old = opcode;
    opcode = BRK_POINT;
    WriteMem( &opcode, acc->break_addr.offset, sizeof( opcode ) );
    Out( "ReqSet_break at " );
    OutNum( acc->break_addr.offset );
    Out( " (was " );
    OutNum( ret->old );
    Out( ")\n" );
    return( sizeof( *ret ) );
}

unsigned ReqClear_break( void )
{
    clear_break_req *acc;
    bp_t            opcode;

    acc = GetInPtr( 0 );
    CONV_LE_32( acc->break_addr.offset );
    CONV_LE_16( acc->break_addr.segment );
    opcode = acc->old;
    WriteMem( &opcode, acc->break_addr.offset, sizeof( opcode ) );
    Out( "ReqClear_break at " );
    OutNum( acc->break_addr.offset );
    Out( " (setting to " );
    OutNum( opcode );
    Out( ")\n" );
    return( 0 );
}

typedef void (*sighandler_t)(int);
static sighandler_t setsig( int sig, sighandler_t handler )
{
    struct sigaction sa, old_sa;

    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    sigaction( sig, &sa, &old_sa );
    return old_sa.sa_handler;
}

static unsigned ProgRun( int step )
{
    static int          ptrace_sig = 0;
    static int          ld_state = 0;
    user_regs_struct    regs;
    int                 status;
    prog_go_ret         *ret;
    void                (*old)(int);
    int                 debug_continue;
    int                 inside_signal = FALSE;

    if( pid == 0 )
        return( 0 );
    ret = GetOutPtr( 0 );

    if( at_end ) {
        ptrace_sig = 0;
        ret->conditions = COND_TERMINATE;
        goto end;
    }

    /* we only want child-generated SIGINTs now */
    do {
        old = setsig( SIGINT, SIG_IGN );
        if( step && !inside_signal ) {
            ptrace( PTRACE_SINGLESTEP, pid, NULL, (void *)ptrace_sig );
        } else {
            ptrace( PTRACE_CONT, pid, NULL, (void *)ptrace_sig );
        }
        waitpid( pid, &status, 0 );
        setsig( SIGINT, old );
        
#if defined( MD_x86 )
        ptrace( PTRACE_GETREGS, pid, NULL, &regs );
#elif defined( MD_ppc )
        regs.eip = ptrace( PTRACE_PEEKUSER, pid, REGSIZE * PT_NIP, NULL );
        regs.esp = ptrace( PTRACE_PEEKUSER, pid, REGSIZE * PT_R1, NULL );
#endif
        Out( " eip " );
        OutNum( regs.eip );
        Out( "\n" );
        
        inside_signal = FALSE;                
        debug_continue = FALSE;
        if( WIFSTOPPED( status ) ) {
            switch( ( ptrace_sig = WSTOPSIG( status ) ) ) {
            case SIGSEGV:
            case SIGILL:
            case SIGFPE:
            case SIGABRT:
            case SIGBUS:
            case SIGQUIT:
            case SIGSYS:
                last_sig = ptrace_sig;
                ret->conditions = COND_EXCEPTION;
                ptrace_sig = 0;
                break;
            case SIGINT:
                ret->conditions = COND_USER;
                ptrace_sig = 0;
                break;
            case SIGTRAP:
                ret->conditions = step ? COND_TRACE : COND_BREAK;
                Out( "sigtrap\n" );
                ptrace_sig = 0;
                break;
            default:
                /* For signals that we do not wish to handle, we need
                 * to continue the debuggee until we get a signal
                 * that we need to handle
                 */
                Out( "Unknown signal " );
                OutNum( ptrace_sig );
                Out( "\n" );
                debug_continue = TRUE;
//                inside_signal = TRUE;
                // continue debugger
                // if we come back to original EIP, single or continue
                break;
            }
        } else if( WIFEXITED( status ) ) {
            Out( "WIFEXITED\n" );
            at_end = TRUE;
            ret->conditions = COND_TERMINATE;
            ptrace_sig = 0;
            goto end;
        }
    } while( debug_continue );

    if( ret->conditions == COND_BREAK ) {
#if defined( MD_x86 )
        if( regs.eip == rdebug.r_brk + sizeof( old_ld_bp ) ) {
#elif defined( MD_ppc )
        if( regs.eip == rdebug.r_brk ) {
#endif
            int         psig = 0;
            void        (*oldsig)(int);
            bp_t        opcode = BRK_POINT;
    
            /* The dynamic linker breakpoint was hit, meaning that
             * libraries are being loaded or unloaded. This gets a bit
             * tricky because we must restore the original code that was
             * at the breakpoint and execute it, but we still want to
             * keep the breakpoint.
             */
            WriteMem( &old_ld_bp, rdebug.r_brk, sizeof( old_ld_bp ) );
            ReadMem( &rdebug, (addr48_off)dbg_rdebug, sizeof( rdebug ) );
            Out( "ld breakpoint hit, state is " );
            switch( rdebug.r_state ) {
            case RT_ADD:
                Out( "RT_ADD\n" );
                ld_state = RT_ADD;
                AddOneLib( rdebug.r_map );
                break;
            case RT_DELETE:
                Out( "RT_DELETE\n" );
                ld_state = RT_DELETE;
                break;
            case RT_CONSISTENT:
                Out( "RT_CONSISTENT\n" );
                if( ld_state == RT_DELETE )
                    DelOneLib( rdebug.r_map );
                ld_state = RT_CONSISTENT;
                break;
            default:
                Out( "error!\n" );
                break;
            }
            regs.orig_eax = -1;
#if defined( MD_x86 )
            regs.eip--;
            ptrace( PTRACE_SETREGS, pid, NULL, &regs );
#endif
            oldsig = setsig( SIGINT, SIG_IGN );
            ptrace( PTRACE_SINGLESTEP, pid, NULL, (void *)psig );
            waitpid( pid, &status, 0 );
            setsig( SIGINT, oldsig );
            WriteMem( &opcode, rdebug.r_brk, sizeof( old_ld_bp ) );
            ret->conditions = COND_LIBRARIES;
        } else {
#if defined( MD_x86 )
            Out( "decrease eip(sigtrap)\n" );
            regs.orig_eax = -1;
            regs.eip--;
            ptrace( PTRACE_SETREGS, pid, NULL, &regs );
#endif
        }
    }
    orig_eax = regs.orig_eax;
    last_eip = regs.eip;
    ret->program_counter.offset = regs.eip;
    ret->program_counter.segment = regs.cs;
    ret->stack_pointer.offset = regs.esp;
    ret->stack_pointer.segment = regs.ss;
    ret->conditions |= COND_CONFIG;

    /* If debuggee has dynamic section, try getting the r_debug struct
     * every time the debuggee stops. The r_debug data may not be available
     * immediately after the debuggee process loads.
     */
    if( !have_rdebug && (dbg_dyn != NULL) ) {
        if( Get_ld_info( &rdebug, &dbg_rdebug ) ) {
            bp_t        opcode;
    
            AddInitialLibs( rdebug.r_map );
            have_rdebug = TRUE;
            ret->conditions |= COND_LIBRARIES;
    
            /* Set a breakpoint in dynamic linker. That way we can be
             * informed on dynamic library load/unload events.
             */
            ReadMem( &old_ld_bp, rdebug.r_brk, sizeof( old_ld_bp ) );
            Out( "Setting ld breakpoint at " );
            OutNum( rdebug.r_brk );
            Out( " old opcode was " );
            OutNum( old_ld_bp );
            Out( "\n" );
            opcode = BRK_POINT;
            WriteMem( &opcode, rdebug.r_brk, sizeof( opcode ) );
        }
    }
 end:
    CONV_LE_32( ret->stack_pointer.offset );
    CONV_LE_16( ret->stack_pointer.segment );
    CONV_LE_32( ret->program_counter.offset );
    CONV_LE_16( ret->program_counter.segment );
    CONV_LE_16( ret->conditions );
    return( sizeof( *ret ) );
}

unsigned ReqProg_step( void )
{
    return( ProgRun( TRUE ) );
}

unsigned ReqProg_go( void )
{
    return( ProgRun( FALSE ) );
}

unsigned ReqRedirect_stdin( void  )
{
    redirect_stdin_ret *ret;

    ret = GetOutPtr( 0 );
    ret->err = 1;
    return( sizeof( *ret ) );
}

unsigned ReqRedirect_stdout( void  )
{
    redirect_stdout_ret *ret;

    ret = GetOutPtr( 0 );
    ret->err = 1;
    return( sizeof( *ret ) );
}

unsigned ReqFile_string_to_fullpath( void )
{
    file_string_to_fullpath_req *acc;
    file_string_to_fullpath_ret *ret;
    int                         exe;
    int                         len;
    char                        *name;
    char                        *fullname;
    pid_t                       pidd;

    pidd = 0;
    acc = GetInPtr( 0 );
    name = GetInPtr( sizeof( *acc ) );
    ret = GetOutPtr( 0 );
    fullname = GetOutPtr( sizeof( *ret ) );
    exe = ( acc->file_type == TF_TYPE_EXE ) ? TRUE : FALSE;
    if( exe ) {
        pidd = RunningProc( name, &name );
    }
    if( pidd != 0 ) {
        len = GetExeNameFromPid( pidd, fullname, PATH_MAX );
    } else {
        len = FindFilePath( exe, name, fullname );
    }
    if( len == 0 ) {
        ret->err = ENOENT;      /* File not found */
    } else {
        ret->err = 0;
    }
    CONV_LE_32( ret->err );
    return( sizeof( *ret ) + len + 1 );
}

unsigned ReqGet_message_text( void )
{
    get_message_text_ret    *ret;
    char                    *err_txt;
    static const char *const ExceptionMsgs[] = {
        "",
        TRP_QNX_hangup,
        TRP_QNX_user_interrupt,
        TRP_QNX_quit,
        TRP_EXC_illegal_instruction,
        TRP_QNX_trap,
        TRP_QNX_abort,
        TRP_QNX_bus_error,
        TRP_QNX_floating_point_error,
        TRP_QNX_process_killed,
        TRP_QNX_user_signal_1,
        TRP_EXC_access_violation "(SIGSEGV)",
        TRP_QNX_user_signal_2,
        TRP_QNX_broken_pipe,
        TRP_QNX_alarm,
        TRP_QNX_process_termination,
        TRP_EXC_floating_point_stack_check,
        TRP_QNX_child_stopped,
        TRP_QNX_process_continued,
        TRP_QNX_process_stopped,
        "", /* sigtstp */
        "", /* sigttin */
        "", /* sigttou */
        TRP_QNX_urgent,
        "", /* sigxcpu */
        "", /* sigxfsz */
        "", /* sigvtalarm */
        "", /* sigprof */
        TRP_QNX_winch,
        TRP_QNX_poll,
        TRP_QNX_power_fail,
        TRP_QNX_sys,
        ""
    };

    ret = GetOutPtr( 0 );
    err_txt = GetOutPtr( sizeof( *ret ) );
    if( last_sig == -1 ) {
        err_txt[0] = '\0';
    } else if( last_sig > (sizeof( ExceptionMsgs ) / sizeof( char * ) - 1) ) {
        strcpy( err_txt, TRP_EXC_unknown );
    } else {
        strcpy( err_txt, ExceptionMsgs[ last_sig ] );
        last_sig = -1;
        ret->flags = MSG_NEWLINE | MSG_ERROR;
    }
    return( sizeof( *ret ) + strlen( err_txt ) + 1 );
}

unsigned ReqAddr_info( void )
{
    addr_info_req   *acc;
    addr_info_ret   *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->is_32 = TRUE;
    return( sizeof( *ret ) );
}

trap_version TRAPENTRY TrapInit( char *parm, char *err, bool remote )
{
    trap_version ver;

    parm = parm;
    remote = remote;
    err[0] = '\0'; /* all ok */
    ver.major = TRAP_MAJOR_VERSION;
    ver.minor = TRAP_MINOR_VERSION;
    ver.remote = FALSE;
    OrigPGrp = getpgrp();

    return( ver );
}

void TRAPENTRY TrapFini()
{
}
