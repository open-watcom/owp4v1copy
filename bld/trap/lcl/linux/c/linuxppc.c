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
#include <asm/ptrace.h>
#include "trpimp.h"
#include "trperr.h"
#include "mad.h"
#include "madregs.h"
#include "exeelf.h"
#include "linuxcomm.h"


static void ReadCPU( struct ppc_mad_registers *r )
{
    struct pt_regs      regs;
    int                 i;

    memset( r, 0, sizeof( *r ) );
    memset( &regs, 0, sizeof( regs ) );
    /* Read GPRs */
    for( i = 0; i < 32; i++ ) {
        regs.gpr[i] = ptrace( PTRACE_PEEKUSER, pid, i * REGSIZE, 0 );
    }
    /* Read SPRs */
    regs.nip  = ptrace( PTRACE_PEEKUSER, pid, PT_NIP * REGSIZE, 0 );
    regs.msr  = ptrace( PTRACE_PEEKUSER, pid, PT_MSR * REGSIZE, 0 );
    regs.ctr  = ptrace( PTRACE_PEEKUSER, pid, PT_CTR * REGSIZE, 0 );
    regs.link = ptrace( PTRACE_PEEKUSER, pid, PT_LNK * REGSIZE, 0 );
    regs.xer  = ptrace( PTRACE_PEEKUSER, pid, PT_XER * REGSIZE, 0 );
    regs.ccr  = ptrace( PTRACE_PEEKUSER, pid, PT_CCR * REGSIZE, 0 );
    regs.mq   = ptrace( PTRACE_PEEKUSER, pid, PT_MQ  * REGSIZE, 0 );
    /* Copy to MAD structure */
    r->r0.u._32[0]  = regs.gpr[0];
    r->r1.u._32[0]  = regs.gpr[1];
    r->r2.u._32[0]  = regs.gpr[2];
    r->r3.u._32[0]  = regs.gpr[3];
    r->r4.u._32[0]  = regs.gpr[4];
    r->r5.u._32[0]  = regs.gpr[5];
    r->r6.u._32[0]  = regs.gpr[6];
    r->r7.u._32[0]  = regs.gpr[7];
    r->r8.u._32[0]  = regs.gpr[8];
    r->r9.u._32[0]  = regs.gpr[9];
    r->r10.u._32[0] = regs.gpr[10];
    r->r11.u._32[0] = regs.gpr[11];
    r->r12.u._32[0] = regs.gpr[12];
    r->r13.u._32[0] = regs.gpr[13];
    r->r14.u._32[0] = regs.gpr[14];
    r->r15.u._32[0] = regs.gpr[15];
    r->r16.u._32[0] = regs.gpr[16];
    r->r17.u._32[0] = regs.gpr[17];
    r->r18.u._32[0] = regs.gpr[18];
    r->r19.u._32[0] = regs.gpr[19];
    r->r20.u._32[0] = regs.gpr[20];
    r->r20.u._32[0] = regs.gpr[21];
    r->r21.u._32[0] = regs.gpr[22];
    r->r22.u._32[0] = regs.gpr[23];
    r->r23.u._32[0] = regs.gpr[24];
    r->r24.u._32[0] = regs.gpr[25];
    r->r25.u._32[0] = regs.gpr[26];
    r->r26.u._32[0] = regs.gpr[26];
    r->r27.u._32[0] = regs.gpr[27];
    r->r28.u._32[0] = regs.gpr[28];
    r->r29.u._32[0] = regs.gpr[29];
    r->r30.u._32[0] = regs.gpr[30];
    r->r31.u._32[0] = regs.gpr[31];
    r->lr.u._32[0]  = regs.link;
    r->ctr.u._32[0] = regs.ctr;
    r->iar.u._32[0] = regs.nip;
    r->msr.u._32[0] = regs.msr;
    r->cr  = regs.ccr;
    r->xer = regs.xer;
    last_eip = regs.nip;
#if 0
        orig_eax = regs.orig_eax;
#endif
}

unsigned ReqRead_cpu( void )
{
//    ReadCPU( GetOutPtr( 0 ) );
//    return( sizeof( mr->ppc ) );
    return( 0 );
}

unsigned ReqRead_fpu( void )
{
    return( 0 );
}

unsigned ReqRead_regs( void )
{
    mad_registers   *mr;

    mr = GetOutPtr( 0 );
    ReadCPU( &mr->ppc );
    return( sizeof( mr->ppc ) );
}

static void WriteCPU( struct ppc_mad_registers *r )
{
#if 0
    /* the kernel uses an extra register orig_eax
       If orig_eax >= 0 then it will check eax for
       certain values to see if it needs to restart a
       system call.
       If it restarts a system call then it will set
       eax=orig_eax and eip-=2.
       If orig_eax < 0 then eax is used as is.
    */

    regs.eax = r->eax;
    if( regs.eip != last_eip ) {
        /* eip is actually changed! This means that
           the orig_eax value does not make sense;
           set it to -1 */
        orig_eax = -1;
        last_eip = regs.eip;
    }
    regs.orig_eax = orig_eax;
#endif
    ptrace( PTRACE_POKEUSER, pid, PT_NIP * REGSIZE, (void *)(r->iar.u._32[0]) );
    ptrace( PTRACE_POKEUSER, pid, PT_MSR * REGSIZE, (void *)(r->msr.u._32[0]) );
    ptrace( PTRACE_POKEUSER, pid, PT_CTR * REGSIZE, (void *)(r->ctr.u._32[0]) );
    ptrace( PTRACE_POKEUSER, pid, PT_LNK * REGSIZE, (void *)(r->lr.u._32[0]) );
    ptrace( PTRACE_POKEUSER, pid, PT_CCR * REGSIZE, (void *)r->cr );
    ptrace( PTRACE_POKEUSER, pid, PT_XER * REGSIZE, (void *)r->xer );
    ptrace( PTRACE_POKEUSER, pid, 0 * REGSIZE, (void *)(r->r0.u._32[0]) );
    ptrace( PTRACE_POKEUSER, pid, 1 * REGSIZE, (void *)(r->r1.u._32[0]) );
}

unsigned ReqWrite_cpu( void )
{
//    WriteCPU( GetInPtr( sizeof( write_cpu_req ) ) );
    return( 0 );
}

unsigned ReqWrite_fpu()
{
    return( 0 );
}

unsigned ReqWrite_regs( void )
{
    mad_registers   *mr;

    mr = GetInPtr( sizeof( write_regs_req ) );
    WriteCPU( &mr->ppc );
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
