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
   continued by Bart Oldeman -- thanks to Andi Kleen's stepper.c example */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <process.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "trpimp.h"
#include "trperr.h"
#include "squish87.h"
#include "mad.h"
#include "madregs.h"
#include "dbg386.h"
#include "linuxcomm.h"

static pid_t            OrigPGrp;

static watch_point  wpList[ MAX_WP ];
static int          wpCount = 0;
static pid_t        pid;
static u_short      flatCS;
static u_short      flatDS;
static int          ptrace_sig;

#if 0
void Out( char *str )
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
        if (*ptr > '9') *ptr += 'A' - '9' - 1;
        i /= 16;
    } while( i != 0 );
    Out( ptr );
}
#else
#define Out(x)
#define OutNum(x)
#endif

static unsigned WriteMem( void *ptr, addr_off offv, unsigned size )
{
    char    *data = ptr;
    int     count;

    /* Write the process memory 32-bits at a time. Kind of silly that
     * Linux does not have an extended ptrace call to read and write
     * blocks of data from the debuggee process, but this is what we
     * need to do for now.
     */
    for (count = size; count >= 4; count -= 4) {
        if (sys_ptrace(PTRACE_POKETEXT, pid, offv,
                       (void *)(*(unsigned_32*)data)) != 0)
            return size - count;
        data += 4;
        offv += 4;
        }

    /* Now handle last partial write if neccesary. Note that we first
     * must read the full 32-bit value, then just change the section
     * we want to update.
     */
    if (count) {
        u_long  val;
        if (sys_ptrace(PTRACE_PEEKTEXT, pid, offv, &val) != 0)
            return size - count;
        Out("readmem:"); 
        OutNum( val );
        Out(" ");
        switch (count) {
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
        Out("writemem:");
        OutNum( val );
        Out(" ");
        if (sys_ptrace(PTRACE_POKETEXT, pid, offv, (void *)val) != 0)
            return size - count;
        }
        
    return size;
}

static unsigned ReadMem( void *ptr, addr_off offv, unsigned size )
{
    char    *data = ptr;
    int     count;

    /* Read the process memory 32-bits at a time */
    for (count = size; count >= 4; count -= 4) {
        if (sys_ptrace(PTRACE_PEEKTEXT, pid, offv, data) != 0)
            return size - count;
        data += 4;
        offv += 4;
        }

    /* Now handle last partial read if neccesary */
    if (count) {
        u_long  val;
        if (sys_ptrace(PTRACE_PEEKTEXT, pid, offv, &val) != 0)
            return size - count;
        switch (count) {
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
        }
    return size - count;
}

unsigned ReqGet_sys_config()
{
    get_sys_config_ret  *ret;

    ret = GetOutPtr(0);
    ret->sys.os = OS_LINUX;

    // TODO: Detect OS version!
    ret->sys.osmajor = 1;
    ret->sys.osminor = 0;

    // TODO: Detect if we have an FPU emulator installed (X86_EMU)
    ret->sys.fpu = X86_387;

    // TODO: Detect the installed processor
    ret->sys.cpu = X86_586;
    ret->sys.huge_shift = 3;
    ret->sys.mad = MAD_X86;
    return( sizeof( *ret ) );
}

unsigned ReqMap_addr()
{
    map_addr_req    *acc;
    map_addr_ret    *ret;
    unsigned long    val;

    // TODO: This appears to a segment and offset address in the
    //       process disk image address space to a real segment/offset
    //       space in virtual memory. We need to figure out what segment
    //       this address corresponds to within the process and
    //       convert the address appropriately (using FlatCS/FlatDS as necessary).
    //
    // TODO: See the NT implementation for ideas!
    //
    // Note: Info about the process address space is stored in the user register
    //       for GDB, so we can use that to find out what we need to convert these
    //       values in here...
    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    ret->lo_bound = 0;
    ret->hi_bound = ~(addr48_off)0;
    sys_ptrace(PTRACE_PEEKUSER, pid, offsetof(user_struct,start_code), &val);
    ret->out_addr.offset = acc->in_addr.offset + val + 0x8048100;
    OutNum(acc->in_addr.segment);
    OutNum(acc->handle);
    if ( acc->in_addr.segment == MAP_FLAT_DATA_SELECTOR ||
         acc->in_addr.segment == flatDS ) {
        sys_ptrace(PTRACE_PEEKUSER, pid, offsetof(user_struct,u_tsize), &val);
        ret->out_addr.offset += val;
        ret->out_addr.segment = flatDS;
    } else {
        ret->out_addr.segment = flatCS;
    }
    return( sizeof( *ret ) );
}

unsigned ReqChecksum_mem()
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

    acc = GetInPtr(0);
    ret = GetOutPtr(0);
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

unsigned ReqRead_mem()
{
    read_mem_req    *acc;
    unsigned        len;

    acc = GetInPtr(0);
    len = ReadMem(GetOutPtr(0),acc->mem_addr.offset,acc->len);
    return( acc->len );
}

unsigned ReqWrite_mem()
{
    write_mem_req   *acc;
    write_mem_ret   *ret;
    unsigned        len;

    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    len = GetTotalSize() - sizeof(*acc);
    ret->len = WriteMem( GetInPtr(sizeof(*acc)), acc->mem_addr.offset, len );
    return( sizeof( *ret ) );
}

unsigned ReqRead_io()
{
    read_io_req *acc;
    void        *ret;
    unsigned    len;

    /* Perform I/O on the target machine on behalf of the debugger.
     * Since there are no kernel API's in Linux to do this, we just
     * enable IOPL and use regular I/O. We will bail if we can't get
     * IOPL=3, so the debugger trap file will need to be run as root
     * before it can be used for I/O access.
     */
    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    if (iopl(3) == 0) {
        len = acc->len;
        switch( len ) {
        case 1:
            *((unsigned_8*)ret) = inpb( acc->IO_offset );
            break;
        case 2:
            *((unsigned_16*)ret) = inpw( acc->IO_offset );
            break;
        case 4:
            *((unsigned_32*)ret) = inpd( acc->IO_offset );
            break;
        }
    } else {
        len = 0;
    }
    return( len );
}

unsigned ReqWrite_io()
{
    write_io_req    *acc;
    write_io_ret    *ret;
    void            *data;
    unsigned        len;

    /* Perform I/O on the target machine on behalf of the debugger.
     * Since there are no kernel API's in Linux to do this, we just
     * enable IOPL and use regular I/O. We will bail if we can't get
     * IOPL=3, so the debugger trap file will need to be run as root
     * before it can be used for I/O access.
     */
    acc = GetInPtr(0);
    data = GetInPtr( sizeof( *acc ) );
    len = GetTotalSize() - sizeof( *acc );
    ret = GetOutPtr(0);
    if (iopl(3) == 0) {
        ret->len = len;
        switch( len ) {
        case 1:
            outpb( acc->IO_offset, *((unsigned_8*)data) );
            break;
        case 2:
            outpw( acc->IO_offset, *((unsigned_16*)data) );
            break;
        case 4:
            outpd( acc->IO_offset, *((unsigned_32*)data) );
            break;
        }
    } else {
        ret->len = 0;
    }
    return( sizeof( *ret ) );
}

static void ReadCPU( struct x86_cpu *r )
{
    user_regs_struct    regs;

    memset( r, 0, sizeof( *r ) );
    if (sys_ptrace(PTRACE_GETREGS, pid, 0, &regs) == 0) {
        r->eax = regs.eax;
        r->ebx = regs.ebx;
        r->ecx = regs.ecx;
        r->edx = regs.edx;
        r->esi = regs.esi;
        r->edi = regs.edi;
        r->ebp = regs.ebp;
        r->esp = regs.esp;
        r->eip = regs.eip;
        r->efl = regs.eflags;
        r->cs = regs.cs;
        r->ds = regs.ds;
        r->ss = regs.ss;
        r->es = regs.es;
        r->fs = regs.fs;
        r->gs = regs.gs;
    }
}

static void ReadFPU( struct x86_fpu *r )
{
    user_i387_struct    regs;

    memset( r, 0, sizeof( *r ) );
    if (sys_ptrace(PTRACE_GETFPREGS, pid, 0, &regs) == 0) {
        r->cw = regs.cwd;
        r->sw = regs.swd;
        r->tag = regs.twd;
        r->ip_err.p.offset = regs.fip;
        r->ip_err.p.segment = regs.fcs;
        r->op_err.p.offset = regs.foo;
        r->op_err.p.segment = regs.fos;
        memcpy(r->reg,regs.st_space,sizeof(r->reg));
    }
}

unsigned ReqRead_cpu()
{
    ReadCPU( GetOutPtr( 0 ) );
    return( sizeof( struct x86_cpu ) );
}

unsigned ReqRead_fpu()
{
    ReadFPU( GetOutPtr( 0 ) );
    return( sizeof( struct x86_fpu ) );
}

unsigned ReqRead_regs( void )
{
    mad_registers   *mr;

    mr = GetOutPtr( 0 );
    ReadCPU( &mr->x86.cpu );
    ReadFPU( &mr->x86.fpu );
    return( sizeof( mr->x86 ) );
}

static void WriteCPU( struct x86_cpu *r )
{
    user_regs_struct    regs;

    regs.eax = r->eax;
    regs.ebx = r->ebx;
    regs.ecx = r->ecx;
    regs.edx = r->edx;
    regs.esi = r->esi;
    regs.edi = r->edi;
    regs.ebp = r->ebp;
    regs.esp = r->esp;
    regs.eip = r->eip;
    regs.eflags = r->efl;
    regs.cs = r->cs;
    regs.ds = r->ds;
    regs.ss = r->ss;
    regs.es = r->es;
    regs.fs = r->fs;
    regs.gs = r->gs;
    sys_ptrace(PTRACE_SETREGS, pid, 0, &regs);
}

static void WriteFPU( struct x86_fpu *r )
{
    user_i387_struct    regs;

    regs.cwd = r->cw;
    regs.swd = r->sw;
    regs.twd = r->tag;
    regs.fip = r->ip_err.p.offset;
    regs.fcs = r->ip_err.p.segment;
    regs.foo = r->op_err.p.offset;
    regs.fos = r->op_err.p.segment;
    sys_ptrace(PTRACE_SETFPREGS, pid, 0, &regs);
}

unsigned ReqWrite_cpu()
{
    WriteCPU( GetInPtr( sizeof( write_cpu_req ) ) );
    return( 0 );
}

unsigned ReqWrite_fpu()
{
    WriteFPU( GetInPtr( sizeof( write_fpu_req ) ) );
    return( 0 );
}

unsigned ReqWrite_regs( void )
{
    mad_registers   *mr;

    mr = GetInPtr( sizeof( write_regs_req ) );
    WriteCPU( &mr->x86.cpu );
    WriteFPU( &mr->x86.fpu );
    return( 0 );
}

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
    if ( pidd == 0 || sys_ptrace( PTRACE_ATTACH, pidd, 0, 0 ) == -1 )
        return( 0 );
    return( pidd );
}

unsigned ReqProg_load()
{
    // TODO: Get the FlatCS and FlatDS register values from the process context
    //       of the debuggee!!
    char                        **args;
    char                        *parms;
    char                        *parm_start;
    int                         i;
    char                        exe_name[255];
    char                        *name;
    pid_t                       save_pgrp;
    prog_load_req               *acc;
    prog_load_ret               *ret;
    unsigned                    len;
    int                         status;

    flatDS = DS();
    flatCS = CS();

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

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
        args = __alloca( i * sizeof( *args ) );
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
        args = __alloca( (i+2) * sizeof( *args ) );
        args[ SplitParms( parms, &args[1], len ) + 1 ] = NULL;
    }
    args[0] = parm_start;
    pid = RunningProc( args[0], &name );
    if( pid == 0 ) {
        args[0] = name;
        if( FindFilePath( TRUE, args[0], exe_name ) == 0 ) {
            exe_name[0] = '\0';
        }
        save_pgrp = getpgrp();
        setpgid( 0, OrigPGrp );
        pid = fork();
        if ( pid == -1 )
            return( 0 );
        if ( pid == 0 ) {
            if ((long)sys_ptrace( PTRACE_TRACEME, 0, 0, 0 ) < 0) {
                exit( 1 );
            }
            /* stopping before exec appears to be necessary ... */
            if ( kill( getpid(), SIGSTOP ) < 0 ) {
                exit( 1 );
            }
            execve( exe_name, (const char **)args, (const char **)environ );
        }
        setpgid( 0, save_pgrp );
    }
    ret->flags = 0;
    if( pid != -1 && pid != 0 ) {
        int status;
        
        ret->task_id = pid;
        ret->flags |= LD_FLAG_IS_PROT | LD_FLAG_IS_32;

        /* wait until it hits _start (upon execve) */
        if ( waitpid( pid, &status, WUNTRACED ) < 0 )
            goto fail;
        if ( sys_ptrace( PTRACE_CONT, pid, 0, 0 ) )
            goto fail;
        /* wait until it hits SIGSTOP */
        if ( waitpid ( pid, &status, WUNTRACED ) < 0 )
            goto fail;
        ptrace_sig = 0;
        errno = 0;
    }
    ret->err = errno;
    if( ret->err != 0 ) {
        pid = 0;
    }
    return( sizeof( *ret ) );
fail:
    if ( pid != 0 && pid != -1 ) {
        kill( pid, SIGKILL );
        waitpid( pid, &status, 0 );
    }
    
}

unsigned ReqProg_kill()
{
    prog_kill_ret   *ret;

    if ( pid != 0 ) {
        int status;
        kill( pid, SIGKILL );
        waitpid( pid, &status, 0 );
    }
    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqSet_break( void )
{
    set_break_req   *acc;
    set_break_ret   *ret;
    u_char          opcode;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ReadMem( &opcode, acc->break_addr.offset, sizeof(opcode) );
    ret->old = opcode;
    opcode = BRK_POINT;
    WriteMem( &opcode, acc->break_addr.offset, sizeof(opcode) );
    return( sizeof( *ret ) );
}

unsigned ReqClear_break( void )
{
    clear_break_req *acc;
    u_char          opcode;

    acc = GetInPtr( 0 );
    opcode = acc->old;
    WriteMem( &opcode, acc->break_addr.offset, sizeof(opcode) );
    return( 0 );
}

u_long GetDR6( void )
{
    u_long  val;
    sys_ptrace(PTRACE_PEEKUSER, pid, O_DEBUGREG(6), &val);
    return val;
}

static void SetDR6( u_long val )
{
    sys_ptrace(PTRACE_POKEUSER, pid, O_DEBUGREG(6), (void *)val);
}

static void SetDR7( u_long val )
{
    sys_ptrace(PTRACE_POKEUSER, pid, O_DEBUGREG(7), (void *)val);
}

static u_long SetDRn( int i, u_long linear, long type )
{
    sys_ptrace(PTRACE_POKEUSER, pid, O_DEBUGREG(i), (void *)linear);
    return( ( type << DR7_RWLSHIFT(i) )
//        | ( DR7_GEMASK << DR7_GLSHIFT(i) ) | DR7_GE
          | ( DR7_LEMASK << DR7_GLSHIFT(i) ) | DR7_LE );
}

void ClearDebugRegs( void )
{
    int i;

    for( i = 0; i < 4; i++)
        SetDRn( i, 0, 0 );
    SetDR6( 0 );
    SetDR7( 0 );
}

int SetDebugRegs( void )
{
    int         needed,i,dr;
    u_long      dr7;
    watch_point *wp;

    needed = 0;
    for( i = 0; i < wpCount; i++)
        needed += wpList[i].dregs;
    if( needed > 4 )
        return( FALSE );
    dr  = 0;
    dr7 = 0;
    for( i = 0, wp = wpList; i < wpCount; i++, wp++ ) {
        dr7 |= SetDRn( dr, wp->linear, DRLen( wp->len ) | DR7_BWR );
        dr++;
        if( wp->dregs == 2 ) {
            dr7 |= SetDRn( dr, wp->linear+wp->len, DRLen( wp->len ) | DR7_BWR );
            dr++;
        }
    }
    SetDR7( dr7 );
    return( TRUE );
}

int CheckWatchPoints( void )
{
    u_long  value;
    int     i;

    for( i = 0; i < wpCount; i++ ) {
        ReadMem( &value, wpList[i].loc.offset, sizeof( value ) );
        if( value != wpList[i].value ) {
            return( TRUE );
        }
    }
    return( FALSE );
}

unsigned ReqSet_watch( void )
{
    set_watch_req   *acc;
    set_watch_ret   *ret;
    u_long          value;
    watch_point     *curr;
    u_long          linear;
    unsigned        i,needed;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->multiplier = 100000;
    ret->err = 1;
    if( wpCount < MAX_WP ) {
        ret->err = 0;
        curr = wpList + wpCount;
        curr->loc.segment = acc->watch_addr.segment;
        curr->loc.offset = acc->watch_addr.offset;
        ReadMem( &value, acc->watch_addr.offset, sizeof(dword) );
        curr->value = value;
        curr->len = acc->size;
        wpCount++;
        curr->linear = linear = acc->watch_addr.offset;
        curr->linear &= ~(curr->len-1);
        curr->dregs = (linear & (curr->len-1) ) ? 2 : 1;
        needed = 0;
        for( i = 0; i < wpCount; ++i ) {
            needed += wpList[ i ].dregs;
        }
        if( needed <= 4 ) ret->multiplier |= USING_DEBUG_REG;
    }
    return( sizeof( *ret ) );
}

unsigned ReqClear_watch( void )
{
    clear_watch_req *acc;
    watch_point     *dst;
    watch_point     *src;
    int             i;

    acc = GetInPtr( 0 );
    dst = src = wpList;
    for( i = 0; i < wpCount; i++ ) {
        if( src->loc.segment != acc->watch_addr.segment
                || src->loc.offset != acc->watch_addr.offset ) {
            dst->loc.offset = src->loc.offset;
            dst->loc.segment = src->loc.segment;
            dst->value = src->value;
            dst++;
        }
        src++;
    }
    wpCount--;
    return( 0 );
}

static unsigned ProgRun( int step )
{
    user_regs_struct    regs;
    int                 status;
    prog_go_ret         *ret;
    
    if ( pid == 0 )
        return 0;
    ret = GetOutPtr( 0 );
    if ( step ) {
        sys_ptrace( PTRACE_SINGLESTEP, pid, 0, 0 ); //(void *)ptrace_sig );
    } else {
        sys_ptrace( PTRACE_CONT, pid, 0, 0 ); //(void *)ptrace_sig );
    }
    waitpid ( pid, &status, WUNTRACED );
    if ( WIFSTOPPED( status ) ) {
        switch( ( ptrace_sig = WSTOPSIG( status ) ) ) {
        case SIGSEGV:
        case SIGILL:
            ret->conditions = COND_EXCEPTION;
            break;
        case SIGTRAP:
            if ( !step ) {
                sys_ptrace(PTRACE_GETREGS, pid, 0, &regs);
                Out("decrease eip(sigtrap)" );
                regs.eip--;
                sys_ptrace(PTRACE_SETREGS, pid, 0, &regs);
                ret->conditions = COND_BREAK;
            } else {
                ret->conditions = COND_TRACE;
            }
            Out("sigtrap");
            break;
        }
    } else if ( WIFEXITED( status ) ) {
        ret->conditions = COND_TERMINATE;
    }
    if (sys_ptrace(PTRACE_GETREGS, pid, 0, &regs) == 0) {
        Out( " eip " );
        OutNum( regs.eip );
        Out( " " );
        ret->program_counter.offset = regs.eip;
        ret->program_counter.segment = regs.cs;
        ret->stack_pointer.offset = regs.esp;
        ret->stack_pointer.segment = regs.ss;
        ret->conditions |= COND_CONFIG;
    }
    return( sizeof( *ret ) );
}

unsigned ReqProg_step()
{
    return( ProgRun( TRUE ) );
}

unsigned ReqProg_go()
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

unsigned ReqFile_string_to_fullpath()
{
    file_string_to_fullpath_req *acc;
    file_string_to_fullpath_ret *ret;
    int                         exe;
    int                         len;
    char                        *name;
    char                        *fullname;

    acc = GetInPtr( 0 );
    name = GetInPtr( sizeof( *acc ) );
    ret = GetOutPtr( 0 );
    fullname = GetOutPtr( sizeof( *ret ) );
    exe = ( acc->file_type == TF_TYPE_EXE ) ? TRUE : FALSE;
    // TODO: Need to find out how to get the name of the
    //       running process first...
    len = FindFilePath( exe, name, fullname );
    if( len == 0 ) {
        ret->err = ENOENT;      /* File not found */
    } else {
        ret->err = 0;
    }
    return( sizeof( *ret ) + len + 1 );
}

unsigned ReqGet_message_text()
{
    get_message_text_ret    *ret;
    char                    *err_txt;

    // TODO: Eventually need to get OS exception error information here
    //       so we can display for the user in the debugger!
    ret = GetOutPtr( 0 );
    err_txt = GetOutPtr( sizeof(*ret) );
    err_txt[0] = '\0';
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

unsigned ReqMachine_data()
{
    machine_data_req    *acc;
    machine_data_ret    *ret;
    unsigned_8          *data;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    data = GetOutPtr( sizeof( *ret ) );
    ret->cache_start = 0;
    ret->cache_end = ~(addr_off)0;
    *data = X86AC_BIG;
    return( sizeof( *ret ) + sizeof( *data ) );
}

unsigned ReqGet_lib_name( void )
{
    get_lib_name_ret    *ret;
    char                *name;

    // No shared libraries at this time
    ret = GetOutPtr(0);
    name = GetOutPtr( sizeof( *ret ) );
    ret->handle = 0;
    name[0] = '\0';
    return( sizeof( *ret ) );
}

static int last_sig;

static void sighand(int s)
{
    last_sig = s;
}

/* Usable sigaction wrapper */
static int setsignal(int sig, void (*hand)(int), int flags, sigset_t *mask)
{
    struct sigaction sa = {0};

    sa.sa_handler = hand;
    if (mask) 
        sa.sa_mask = *mask;
    sa.sa_flags = flags;
    return sigaction(sig, &sa, NULL); 
}

trap_version TRAPENTRY TrapInit( char *parm, char *err, int remote )
{
    trap_version ver;

    parm = parm;
    remote = remote;
    err[0] = '\0'; /* all ok */
    ver.major = TRAP_MAJOR_VERSION;
    ver.minor = TRAP_MINOR_VERSION;
    ver.remote = FALSE;

    OrigPGrp = getpgrp();

    setsignal(SIGTRAP, sighand, 0, NULL); 
    setsignal(SIGCHLD, sighand, 0, NULL); 
    setsignal(SIGSEGV, sighand, 0, NULL); 
    setsignal(SIGILL, sighand, 0, NULL); 
    
    return( ver );
}

void TRAPENTRY TrapFini()
{
}
