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
* Description:  Linux debugger trap file x86 specific functions.
*
****************************************************************************/


#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include "trpimp.h"
#include "trperr.h"
#include "mad.h"
#include "madregs.h"
#include "exeelf.h"
#include "linuxcomm.h"

#if 0
static void ReadCPU( struct x86_cpu *r )
{
    user_regs_struct    regs;

    memset( r, 0, sizeof( *r ) );
    if( ptrace( PTRACE_GETREGS, pid, NULL, &regs ) == 0 ) {
        last_eip = regs.eip;
        orig_eax = regs.orig_eax;
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
    if( ptrace( PTRACE_GETFPREGS, pid, NULL, &regs ) == 0 ) {
        r->cw = regs.cwd;
        r->sw = regs.swd;
        r->tag = regs.twd;
        r->ip_err.p.offset = regs.fip;
        r->ip_err.p.segment = regs.fcs;
        r->op_err.p.offset = regs.foo;
        r->op_err.p.segment = regs.fos;
        memcpy( r->reg, regs.st_space, sizeof( r->reg ) );
    }
}
#endif

unsigned ReqRead_cpu( void )
{
//    ReadCPU( GetOutPtr( 0 ) );
//    return( sizeof( struct x86_cpu ) );
    return( 0 );
}

unsigned ReqRead_fpu( void )
{
//    ReadFPU( GetOutPtr( 0 ) );
//    return( sizeof( struct x86_fpu ) );
    return( 0 );
}

unsigned ReqRead_regs( void )
{
    mad_registers   *mr;

    mr = GetOutPtr( 0 );
//    ReadCPU( &mr->x86.cpu );
    return( sizeof( mr->ppc ) );
}

#if 0
static void WriteCPU( struct x86_cpu *r )
{
    user_regs_struct    regs;

    /* the kernel uses an extra register orig_eax
       If orig_eax >= 0 then it will check eax for
       certain values to see if it needs to restart a
       system call.
       If it restarts a system call then it will set
       eax=orig_eax and eip-=2.
       If orig_eax < 0 then eax is used as is.
    */

    regs.eax = r->eax;
    regs.ebx = r->ebx;
    regs.ecx = r->ecx;
    regs.edx = r->edx;
    regs.esi = r->esi;
    regs.edi = r->edi;
    regs.ebp = r->ebp;
    regs.esp = r->esp;
    regs.eip = r->eip;
    if( regs.eip != last_eip ) {
        /* eip is actually changed! This means that
           the orig_eax value does not make sense;
           set it to -1 */
        orig_eax = -1;
        last_eip = regs.eip;
    }
    regs.orig_eax = orig_eax;
    regs.eflags = r->efl;
    regs.cs = r->cs;
    regs.ds = r->ds;
    regs.ss = r->ss;
    regs.es = r->es;
    regs.fs = r->fs;
    regs.gs = r->gs;
    ptrace( PTRACE_SETREGS, pid, NULL, &regs );
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
    memcpy( regs.st_space, r->reg, sizeof( r->reg ) );
    ptrace( PTRACE_SETFPREGS, pid, NULL, &regs );
}
#endif

unsigned ReqWrite_cpu( void )
{
//    WriteCPU( GetInPtr( sizeof( write_cpu_req ) ) );
    return( 0 );
}

unsigned ReqWrite_fpu()
{
//    WriteFPU( GetInPtr( sizeof( write_fpu_req ) ) );
    return( 0 );
}

unsigned ReqWrite_regs( void )
{
    mad_registers   *mr;

    mr = GetInPtr( sizeof( write_regs_req ) );
//    WriteCPU( &mr->x86.cpu );
    return( 0 );
}

unsigned ReqSet_watch( void )
{
    set_watch_req   *acc;
    set_watch_ret   *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->multiplier = 100000;
    ret->err = 1;
    return( sizeof( *ret ) );
}

unsigned ReqClear_watch( void )
{
    clear_watch_req *acc;

    acc = GetInPtr( 0 );
    return( 0 );
}

unsigned ReqRead_io( void )
{
    read_io_req *acc;
    void        *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    return( 0 );
}

unsigned ReqWrite_io( void )
{
    write_io_req    *acc;
    write_io_ret    *ret;
    void            *data;

    acc = GetInPtr( 0 );
    data = GetInPtr( sizeof( *acc ) );
    ret = GetOutPtr( 0 );
    ret->len = 0;
    return( sizeof( *ret ) );
}

unsigned ReqGet_sys_config( void )
{
    get_sys_config_ret  *ret;

    ret = GetOutPtr( 0 );
    ret->sys.os = OS_LINUX;

    // TODO: Detect OS version (kernel version?)!
    ret->sys.osmajor = 1;
    ret->sys.osminor = 0;

    ret->sys.cpu = PPC_603;
    ret->sys.fpu = 0;
    ret->sys.huge_shift = 3;
    ret->sys.mad = MAD_PPC;
    CONV_LE_16( ret->sys.mad );
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
    *data = 0;
    return( sizeof( *ret ) + sizeof( *data ) );
}
