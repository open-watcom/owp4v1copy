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

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "linuxcomm.h"
#include "trpimp.h"
#include "trperr.h"
#include "squish87.h"
#include "mad.h"
#include "madregs.h"

// TODO: Need signals and execve() in runtime library to make this work!!

static struct {
    unsigned long       in;
    unsigned long       out;
    unsigned long       err;
} StdPos;

#define MAX_WP          32

//struct _watch_struct    WatchPoints[ MAX_WP ];
static short    watchCount = 0;
static pid_t    pid;

static unsigned WriteBuffer( char *data, addr_off offv, unsigned size )
{
    int count;

    /* Write the process memory 32-bits at a time. Kind of silly that
     * Linux does not have an extended ptrace call to read and write
     * blocks of data from the debuggee process, but this is what we
     * need to do for now.
     */
    for (count = size; count >= 4; count -= 4) {
        if (sys_ptrace(PTRACE_POKETEXT, pid, offv, data) != 0)
            return size - count;
        data += 4;
        offv += 4;
        }

    /* Now handle last partial write if neccesary */
    if (count) {
        u_long  val;
        switch (count) {
            case 1:
                val = (u_long)(*((unsigned_8*)data));
                break;
            case 2:
                val = (u_long)(*((unsigned_16*)data));
                break;
            case 3:
                val = ((u_long)(*((unsigned_8*)(data+0))) << 0) |
                      ((u_long)(*((unsigned_8*)(data+1))) << 8) |
                      ((u_long)(*((unsigned_8*)(data+2))) << 16);
                break;
            }
        if (sys_ptrace(PTRACE_POKETEXT, pid, offv, &val) != 0)
            return size - count;
        }
    return size;
}

static unsigned ReadBuffer( char *data, addr_off offv, unsigned size )
{
    int count;

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

    // TODO: This appears to a segment and offset address in the
    //       process disk image address space to a real segment/offset
    //       space in virtual memory. We need to figure out what segment
    //       this address corresponds to within the process and
    //       convert the address appropriately (using FlatCS/FlatDS as necessary).
    //
    // TODO: See the NT implementation for ideas!
    acc = GetInPtr(0);
    ret = GetOutPtr(0);
    ret->lo_bound = 0;
    ret->hi_bound = ~(addr48_off)0;
    ret->out_addr.offset = acc->in_addr.offset;
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
            amount = ReadBuffer( buf, offv, size );
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
    len = ReadBuffer(GetOutPtr(0),acc->mem_addr.offset,acc->len);
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
    ret->len = WriteBuffer( GetInPtr(sizeof(*acc)), acc->mem_addr.offset, len );
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
    // TODO: Read the CPU state for debuggee!
}

static void ReadFPU( struct x86_fpu *r )
{
    // TODO: Read the FPU state for debuggee!
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
    // TODO: Write the CPU state for the debuggee!
}

static void WriteFPU( struct x86_fpu *r )
{
    // TODO: Write the FPU state for the debuggee!
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

unsigned ReqProg_load()
{
    // TODO: Load the debuggee program!
    return( 0 );
}

unsigned ReqProg_kill()
{
    prog_kill_ret   *ret;

    // TODO: Kill the debuggee process!
    ret = GetOutPtr( 0 );
    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqSet_break()
{
//    long            opcode;
    set_break_req   *acc;
    set_break_ret   *ret;

    // TODO: Set a breakpoint
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
//    __qnx_debug_brk( ProcInfo.proc, ProcInfo.pid, _DEBUG_BRK_SET, &opcode,
//                     acc->break_addr.offset, acc->break_addr.segment );
//    ret->old = opcode;
    return( sizeof( *ret ) );
}

unsigned ReqClear_break()
{
    long            opcode;
    clear_break_req *acc;

    // TODO: Clear a breakpoint
    acc = GetInPtr( 0 );
    opcode = acc->old;
//    __qnx_debug_brk( ProcInfo.proc, ProcInfo.pid, _DEBUG_BRK_CLR, &opcode,
//                acc->break_addr.offset, acc->break_addr.segment );
    return( 0 );
}

unsigned ReqSet_watch()
{
    unsigned        size;
    set_watch_req   *acc;
    set_watch_ret   *ret;

    // TODO: Figure out how to add a watch point
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    for( size = acc->size; size != 0; --size ) {
//        WatchPoints[ watchCount ].seg = acc->watch_addr.segment;
//        WatchPoints[ watchCount ].off = acc->watch_addr.offset;
        ++acc->watch_addr.offset;
        ++watchCount;
    }
    ret->err = 0;
    ret->multiplier = 1000;
    return( sizeof( *ret ) );
}

unsigned ReqClear_watch()
{
    watchCount = 0; /* assume all are cleared at the same time */
    return( 0 );
}

static unsigned ProgRun( int step )
{
    // TODO: Figure out how to run and step the debuggee!!
    return( 0 );
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
    redirect_stdin_ret  *ret;

    // TODO: Figure out how to redirect standard input for debuggee!
    ret = GetOutPtr( 0 );
    ret->err = 1;
    return( sizeof( *ret ) );
}

unsigned ReqRedirect_stdout( void  )
{
    redirect_stdout_ret *ret;

    // TODO: Figure out how to redirect standard output for debuggee!
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

trap_version TRAPENTRY TrapInit( char *parm, char *err, int remote )
{
    trap_version ver;

    parm = parm;
    remote = remote;
    StdPos.in  = lseek( 0, 0, SEEK_CUR );
    StdPos.out = lseek( 1, 0, SEEK_CUR );
    StdPos.err = lseek( 2, 0, SEEK_CUR );
    err[0] = '\0'; /* all ok */
    ver.major = TRAP_MAJOR_VERSION;
    ver.minor = TRAP_MINOR_VERSION;
    ver.remote = FALSE;
    return( ver );
}

void TRAPENTRY TrapFini()
{
}

