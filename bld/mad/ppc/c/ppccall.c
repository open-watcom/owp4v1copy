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
* Description:  PowerPC call stack unwinding.
*
****************************************************************************/


#include <stddef.h>
#include <string.h>
#include "ppc.h"
#include "madregs.h"

mad_string              DIGENTRY MICallStackGrowsUp( void )
{
    return( MS_FAIL );
}

const mad_string        *DIGENTRY MICallTypeList( void )
{
    static const mad_string list[] = { MSTR_NIL };

    return( list );
}

mad_status      DIGENTRY MICallBuildFrame( mad_string call, address ret, address rtn, mad_registers const *in, mad_registers *out )
{
    call = call;
    out->ppc = in->ppc;
    //NYI: 64 bit
    out->ppc.lr.u._32[0] = ret.mach.offset;
    out->ppc.iar.u._32[0] = rtn.mach.offset;
    return( MS_OK );
}

const mad_reg_info      *DIGENTRY MICallReturnReg( mad_string call, address rtn )
{
    return( &RegList[IDX_r3].info );
}

const mad_reg_info      **DIGENTRY MICallParmRegList( mad_string call, address rtn )
{
    static const mad_reg_info *list[] = {
        &RegList[IDX_r3].info, &RegList[IDX_r4].info, &RegList[IDX_r5].info,
        &RegList[IDX_r6].info, &RegList[IDX_r7].info, &RegList[IDX_r8].info,
        &RegList[IDX_r9].info, &RegList[IDX_r10].info,
        NULL };

    return( list );
}

static addr_off GetAnOffset( address *a )
{
    addr_off    off;

    MCReadMem( *a, sizeof( off ), &off );
    return( off );
}

static mad_status HeuristicTraceBack( address const *start,
                                address *execution,
                                address *frame,
                                address *stack,
                                mad_registers const *mr )
{
    mad_disasm_data     dd;
    mad_status          ms;
    address             where;

    if( execution->mach.offset == start->mach.offset ) {
        /* return address is in link register */
        if( mr == NULL ) return( MS_FAIL );
        execution->mach.offset = mr->ppc.lr.u._32[0];
        return( MS_OK );
    }
    /* have to parse the prologue */
    ms = DisasmOne( &dd, (address *)start, 0 );
    if( ms != MS_OK ) return( ms );
    if( dd.ins.type != DI_PPC_stwu ) return( MS_FAIL );
    where = *stack;
    stack->mach.offset -= dd.ins.op[1].value;
    for( ;; ) {
        if( execution->mach.offset == start->mach.offset ) break;
        ms = DisasmOne( &dd, (address *)start, 0 );
        if( ms != MS_OK ) return( ms );
        // TODO: figure out proper instruction types
        if( dd.ins.type != DI_PPC_stw && dd.ins.type != DI_PPC_stwu ) break;
        // TODO: redo this code for PowerPC
#if 0
        if( TRANS_REG( dd.ins.op[0].base ) == AR_ra ) {
            where.mach.offset += dd.ins.op[1].value;
            execution->mach.offset = GetAnOffset( &where );
            return( MS_OK );
        }
#else
        return( MS_FAIL );
#endif
    }
    if( mr == NULL ) return( MS_FAIL );
    execution->mach.offset = mr->ppc.lr.u._32[0];
    return( MS_OK );
}

static mad_status SymbolicTraceBack( address const *start,
                        address *execution,
                        address *frame,
                        address *stack,
                        mad_registers const *mr,
                        long bp_disp )
{
    address             where;

    if( execution->mach.offset == start->mach.offset ) {
        /* return address is in link register */
        if( mr == NULL ) return( MS_FAIL );
        execution->mach.offset = mr->ppc.lr.u._32[0];
        return( MS_OK );
    }
    where = *frame;
    frame->mach.offset = GetAnOffset( &where );
    where.mach.offset += bp_disp;
    execution->mach.offset = GetAnOffset( &where );
    *stack = where;
    stack->mach.offset += sizeof( unsigned_64 );
    return( MS_OK );
}

unsigned        DIGENTRY MICallUpStackSize( void )
{
    return( sizeof( mad_call_up_data ) );
}

mad_status      DIGENTRY MICallUpStackInit( mad_call_up_data *cud, const mad_registers *mr )
{
    cud = cud;
    mr = mr;
    return( MS_OK );
}

mad_status      DIGENTRY MICallUpStackLevel( mad_call_up_data *cud,
                                address const *start,
                                unsigned rtn_characteristics,
                                long return_disp,
                                mad_registers const *in,
                                address *execution,
                                address *frame,
                                address *stack,
                                mad_registers **out )
{
    address             prev_sp_value;
    mad_status          ms;

    cud = cud;
    rtn_characteristics = rtn_characteristics;
    *out = NULL;
    prev_sp_value = *stack;
    switch( return_disp ) {
    case 0:
    case -1:
        ms = HeuristicTraceBack( start, execution, frame, stack, in );
        break;
    default:
        ms = SymbolicTraceBack( start, execution, frame, stack, in, return_disp );
        break;
    }
    if( ms != MS_OK ) return( ms );
    if( stack->mach.offset < prev_sp_value.mach.offset ) return( MS_FAIL );
    return( MS_OK );
}
