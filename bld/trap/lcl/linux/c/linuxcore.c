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
* Description:  Linux core file debug support.
*
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "trpimp.h"
#include "exeelf.h"
#include "linuxcomm.h"
#include "squish87.h"
#include "mad.h"
#include "madregs.h"
#include "orl.h"

#define NO_FILE         (-1)

enum {
    MH_NONE,
    MH_DEBUGGEE,
    MH_SLIB,
    MH_PROC
};

typedef struct buff_entry       *buff_list;
struct buff_entry {
    buff_list   next;
    char        buff[1];
};

struct {
    unsigned                    loaded                  : 1;
    unsigned                    ignore_timestamp        : 1;
    unsigned                    read_gdts               : 1;
    unsigned                    dbg32                   : 1;
    unsigned                    mapping_shared          : 1;
    int                         fd;
    orl_handle                  orl;
    buff_list                   buf_list;
//    struct      _dumper_hdr     hdr;
} core_info;

#if 0
static void Out( char *str )
{
    write( 1, str, strlen( str ) );
}

static void OutNum( unsigned i )
{
    char numbuff[10];
    char *ptr;

    ptr = numbuff+10;
    *--ptr = '\0';
    do {
        *--ptr = ( i % 10 ) + '0';
        i /= 10;
    } while( i != 0 );
    Out( ptr );
}
#endif


unsigned ReqGet_sys_config( void )
{
    get_sys_config_ret  *ret;

    ret = GetOutPtr(0);
    ret->sys.mad = MAD_X86;
    ret->sys.os  = OS_LINUX;
    ret->sys.osmajor = 1;
    ret->sys.osminor = 0;
    ret->sys.fpu = X86_387;
    ret->sys.cpu = X86_686;
    ret->sys.huge_shift = 3;
    return( sizeof( *ret ) );
}

unsigned ReqMap_addr( void )
{
    map_addr_req        *acc;
    map_addr_ret        *ret;
    unsigned            index;
    unsigned            seg;

    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    ret->lo_bound = 0;
    ret->hi_bound = ~(addr48_off)9;

    seg = acc->in_addr.segment;
    switch( seg ) {
    case MAP_FLAT_CODE_SELECTOR:
        seg = 0x04;
        break;
    case MAP_FLAT_DATA_SELECTOR:
        seg = 0x0c;
        break;
    }

    ret->out_addr.offset = acc->in_addr.offset;
    switch( acc->handle ) {
    case MH_DEBUGGEE:
        if( acc->in_addr.segment == MAP_FLAT_DATA_SELECTOR ) {
            index = 0;
        } else {
            index = seg >> 3;
        }
#if 0
        if( core_info.loaded ) {
            ret->out_addr.offset += core_info.segs[ index ].mem_off;
        }
#endif
        break;
    case MH_SLIB:
#if 0
        slib = GetSLibTable( core_info.dbg32 );
        seg += slib.segment - 4;
#endif
        break;
    case MH_PROC:
        seg += 0xE0 - 4;
        break;
    }
    ret->out_addr.segment = 0;
    return( sizeof( *ret ) );
}


unsigned ReqChecksum_mem( void )
{
    checksum_mem_ret    *ret;

    ret = GetOutPtr(0);
    ret->result = 0;
    return( sizeof( *ret ) );
}

unsigned ReqRead_mem( void )
{
    read_mem_req        *acc;
    void                *ret;
//    unsigned            i;
    unsigned            len;

    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    if( !core_info.loaded ) {
        return( 0 );
    }
    len = acc->len;
#if 0
    for( i = 0; i < core_info.hdr.numsegs; ++i ) {
        if( core_info.segs[i].real_seg == acc->mem_addr.segment ) {
            if( acc->mem_addr.offset >= core_info.segs[i].seg_len ) {
                len = 0;
            } else if( acc->mem_addr.offset+len > core_info.segs[i].seg_len ) {
                len = core_info.segs[i].seg_len - acc->mem_addr.offset;
            }
            if( len != 0 ) {
                lseek( core_info.fd, core_info.segs[i].file_off + acc->mem_addr.offset,
                         SEEK_SET );
                len = read( core_info.fd, ret, len );
                if( len == -1 ) len = 0;
            }
            return( len );
        }
    }
#endif
    return( 0 );
}


unsigned ReqWrite_mem( void )
{
    write_mem_ret       *ret;

    ret = GetOutPtr(0);
    ret->len = 0;
    return( sizeof( *ret ) );
}


unsigned ReqRead_io( void )
{
    return( 0 );
}


unsigned ReqWrite_io( void )
{
    write_io_ret        *ret;

    ret = GetOutPtr(0);
    ret->len = 0;
    return( sizeof( *ret ) );
}

static void ReadCPU( struct x86_cpu *r )
{
    memset( r, 0, sizeof( *r ) );
    if( core_info.loaded ) {
#if 0
        r->eax = core_info.hdr.reg.ax;
        r->ebx = core_info.hdr.reg.bx;
        r->ecx = core_info.hdr.reg.cx;
        r->edx = core_info.hdr.reg.dx;
        r->esi = core_info.hdr.reg.si;
        r->edi = core_info.hdr.reg.di;
        r->ebp = core_info.hdr.reg.bp;
        r->esp = core_info.hdr.reg.sp;
        r->eip = core_info.hdr.reg.ip;
        r->efl = core_info.hdr.reg.fl;
        r->cs = core_info.hdr.reg.cs;
        r->ds = core_info.hdr.reg.ds;
        r->ss = core_info.hdr.reg.ss;
        r->es = core_info.hdr.reg.es;
        r->fs = core_info.hdr.reg.fs;
        r->gs = core_info.hdr.reg.gs;
#endif
    }
}

static void ReadFPU( struct x86_fpu *r )
{
    memset( r, 0, sizeof( *r ) );
    if( core_info.loaded ) {
#if 0
        memcpy( r, core_info.hdr.x87, sizeof( core_info.hdr.x87 ) );
        if( !core_info.fpu32 ) FPUExpand( r );
#endif
    }
}

unsigned ReqRead_cpu( void )
{
    ReadCPU( GetOutPtr( 0 ) );
    return( sizeof( struct x86_cpu ) );
}

unsigned ReqRead_fpu( void )
{
    ReadFPU( GetOutPtr( 0 ) );
    return( sizeof( struct x86_fpu ) );
}

unsigned ReqRead_regs( void )
{
    mad_registers       *mr;

    mr = GetOutPtr( 0 );

    ReadCPU( &mr->x86.cpu );
    ReadFPU( &mr->x86.fpu );
    return( sizeof( mr->x86 ) );
}

unsigned ReqWrite_cpu( void )
{
    return( 0 );
}

unsigned ReqWrite_fpu( void )
{
    return( 0 );
}

unsigned ReqWrite_regs( void )
{
    return( 0 );
}

static bool LoadPmdHeader( char *name )
{
#if 0
    struct stat     tmp;
    char            result[256];

    if( TryOnePath( ":/usr/dumps", &tmp, name, result ) == 0 ) return( FALSE );
    core_info.fd = open( result, O_RDONLY );
    if( core_info.fd < 0 ) return( FALSE );
    if( read( core_info.fd, &core_info.hdr, sizeof( core_info.hdr ) )
            != sizeof( core_info.hdr ) ) {
        close( core_info.fd );
        core_info.fd = NO_FILE;
        errno = ENOEXEC;
        return( FALSE );
    }
    if( core_info.hdr.signature != DUMP_SIGNATURE
     || core_info.hdr.version != DUMP_VERSION
     || core_info.hdr.errnum != 0 ) {
        close( core_info.fd );
        core_info.fd = NO_FILE;
        errno = ENOEXEC;
        return( FALSE );
    }
#endif
    return( TRUE );
}

unsigned ReqProg_load( void )
{
    prog_load_req       *acc;
    prog_load_ret       *ret;
    char                *argv;

    core_info.dbg32 = FALSE;
    core_info.mapping_shared = FALSE;
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    argv = GetInPtr( sizeof( *acc ) );
    ret->mod_handle = MH_DEBUGGEE;

    if( argv[0] == '\0' ) {
        ret->task_id = 0;
        ret->err = ENOENT;
        return( sizeof( *ret ) );
    }
    errno = 0;
    LoadPmdHeader( argv );
    ret->flags = LD_FLAG_IS_STARTED | LD_FLAG_IS_PROT | LD_FLAG_IS_32;
    ret->task_id = 123; //core_info.hdr.psdata.pid;
    ret->err = errno;
    if( errno == 0 ) {
        core_info.loaded = TRUE;
    } else {
        close( core_info.fd );
        core_info.fd = NO_FILE;
    }
    return( sizeof( *ret ) );
}

unsigned ReqProg_kill( void )
{
    prog_kill_ret       *ret;

    if( core_info.loaded ) {
        core_info.loaded = FALSE;
        close( core_info.fd );
        core_info.fd = NO_FILE;
    }
    core_info.mapping_shared = FALSE;
    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqSet_break( void )
{
    set_break_ret       *ret;

    ret = GetOutPtr( 0 );
    ret->old = 0;
    return( sizeof( *ret ) );
}

unsigned ReqClear_break( void )
{
    return( 0 );
}

unsigned ReqSet_watch( void )
{
    set_watch_ret       *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    ret->multiplier = USING_DEBUG_REG | 1;
    return( sizeof( *ret ) );
}

unsigned ReqClear_watch( void )
{
    return( 0 );
}


unsigned ReqProg_go( void )
{
    prog_go_ret *ret;

    ret = GetOutPtr( 0 );
    ret->conditions = COND_TERMINATE;
    return( sizeof( *ret ) );
}

unsigned ReqProg_step( void )
{
    return( ReqProg_go() );
}


unsigned ReqGet_message_text( void )
{
    get_message_text_ret        *ret;
    char                        *err_txt;

    ret = GetOutPtr( 0 );
    err_txt = GetOutPtr( sizeof(*ret) );
    err_txt[0] = '\0';
    ret->flags = MSG_NEWLINE | MSG_ERROR;
    return( sizeof( *ret ) + 1 );
}

unsigned ReqRedirect_stdin( void )
{
    redirect_stdin_ret  *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqRedirect_stdout( void )
{
    return( ReqRedirect_stdin() );
}

unsigned ReqFile_string_to_fullpath( void )
{
    struct  stat                chk;
    unsigned_16                 len;
    char                        *name;
    char                        *fullname;
    unsigned                    save_handle;
    file_string_to_fullpath_req *acc;
    file_string_to_fullpath_ret *ret;

    acc = GetInPtr( 0 );
    name = GetInPtr( sizeof( *acc ) );
    ret = GetOutPtr( 0 );
    fullname = GetOutPtr( sizeof( *ret ) );
    fullname[0] = '\0';
    len = 0;
    if( acc->file_type != TF_TYPE_EXE ) {
        len = FindFilePath( FALSE, name, fullname );
    } else if( core_info.mapping_shared ) {
        len = FindFilePath( TRUE, name, fullname );
    } else {
        save_handle = core_info.fd;
        if( LoadPmdHeader( name ) ) {
            name = "/foo/bar"; //core_info.hdr.psdata.un.proc.name;
            if( stat( name, &chk ) != 0 ) {
                /* try it without the node number */
                name += 2;
                while( *name != '/' ) ++name;
                if( stat( name, &chk ) != 0 ) {
                    chk.st_mtime = 0;
                }
            }
#if 0
            if( core_info.ignore_timestamp || chk.st_mtime == core_info.hdr.cmdtime ) {
                len = StrCopy( name, fullname ) - fullname;
            }
#endif
            close( core_info.fd );
        }
        core_info.fd = save_handle;
    }
    if( len == 0 ) {
        ret->err = ENOENT;      /* File not found */
    } else {
        ret->err = 0;
    }
    return( sizeof( *ret ) + len + 1 );
}

unsigned ReqAddr_info( void )
{
    addr_info_req       *acc;
    addr_info_ret       *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->is_32 = TRUE;
    return( sizeof( *ret ) );
}

unsigned ReqMachine_data( void )
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
    get_lib_name_req    *acc;
    get_lib_name_ret    *ret;
    char                *name;

    acc = GetInPtr(0);
    ret = GetOutPtr( 0 );
    name = GetOutPtr( sizeof( *ret ) );
    switch( acc->handle ) {
    case MH_NONE:
    case MH_DEBUGGEE:
        ret->handle = MH_SLIB;
        if( core_info.dbg32 ) {
            strcpy( name, "/boot/sys/Slib32" );
        } else {
            strcpy( name, "/boot/sys/Slib16" );
        }
        break;
    case MH_SLIB:
        ret->handle = MH_PROC;
#if 0
        if( core_info.hdr.osdata.sflags & _PSF_32BIT ) {
            strcpy( name, "/boot/sys/Proc32" );
        } else {
            strcpy( name, "/boot/sys/Proc16" );
        }
#endif
        break;
    default:
        ret->handle = MH_NONE;
        name[0] = '\0';
        break;
    }
#if 0
    if( core_info.read_gdts ) {
        core_info.mapping_shared = TRUE;
    } else {
        name[0] = '\0';
        ret->handle = MH_NONE;
    }
#endif
    return( sizeof( *ret ) + 1 + strlen( name ) );
}

#if 0
unsigned ReqThread_get_next( void )
{
    thread_get_next_req *req;
    thread_get_next_ret *ret;

    req = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    if( req->thread == 0 ) {
        ret->thread = core_info.hdr.psdata.pid;
        ret->state = THREAD_THAWED;
    } else {
        ret->thread = 0;
    }
    return( sizeof( *ret ) );
}

unsigned ReqThread_set( void )
{
    thread_set_ret      *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    ret->old_thread = core_info.hdr.psdata.pid;
    return( sizeof( *ret ) );
}

unsigned ReqThread_freeze( void )
{
    thread_freeze_ret   *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqThread_thaw( void )
{
    thread_thaw_ret     *ret;

    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqThread_get_extra( void )
{
    char                    *ret;

    ret = GetOutPtr( 0 );
    ret[0] = '\0';
    return( strlen( ret ) + 1 );
}
#endif

static void * orl_read( void *hdl, unsigned int len )
/***************************************************/
{
    buff_list   ptr;

    ptr = malloc( sizeof( *core_info.buf_list ) + len - 1 );
    ptr->next = core_info.buf_list;
    core_info.buf_list = ptr;
    if( read( (int)hdl, ptr->buff, len ) != len ) {
        free( ptr );
        return( NULL );
    }
    return( ptr->buff );
}

static long orl_seek( void *hdl, long pos, int where )
/****************************************************/
{
    return( lseek( (int)hdl, pos, where ) );
}

static void free_orl_buffers( void )
/**********************************/
{
    buff_list   next;

    while( core_info.buf_list ) {
        next = core_info.buf_list->next;
        free( core_info.buf_list );
        core_info.buf_list = next;
    }
}

static orl_funcs orl_imports = {
    orl_read,
    orl_seek,
    malloc,
    free
};

trap_version TRAPENTRY TrapInit( char *parm, char *err, bool remote )
{
    trap_version ver;

    remote = remote;
    core_info.fd = NO_FILE;
    if( parm != NULL ) {
        while( *parm != '\0' ) {
            switch( *parm ) {
            case 'I':
            case 'i':
                core_info.ignore_timestamp = TRUE;
                break;
            }
            ++parm;
        }
    }
    core_info.orl = ORLInit( &orl_imports );
    err[0] = '\0'; /* all ok */
    ver.major = TRAP_MAJOR_VERSION;
    ver.minor = TRAP_MINOR_VERSION;
    ver.remote = FALSE;
    return( ver );
}

void TRAPENTRY TrapFini( void )
{
    free_orl_buffers();
    ORLFini( core_info.orl );
}
