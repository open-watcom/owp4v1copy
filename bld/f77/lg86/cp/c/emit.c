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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


//
// EMIT         : Emit information to object memory
//

#include "ftnstd.h"
#include "fcodes.h"
#include "objutil.h"
#include "fcenv.h"

#if _TARGET == _8086
//    #pragma aux (lg_sym) FC_TABLE;
//    extern    unsigned_16     __FAR FC_TABLE[];
//    #define MAP_FCODE( __v )  FC_TABLE[ __v ]
    #define MAP_FCODE( __v )    ( ( __v ) * 2 )
#elif _TARGET == _80386
    #define MAP_FCODE( __v )    ( ( __v ) * 4 )
#endif

extern  obj_ptr         ObjPtr;

static  unsigned_16             PrevOp;
static  obj_ptr                 PrevLoc;


static  const unsigned_16 __FAR PeepHole[] = {
//  num  prev_op         current_op      replacement_op for prev_op
    7,  PUSH_INT4,
                        POP_INT4,       MOVE_LL_I4,
                        II_ADD,         ADD_L_I4,
                        II_SUB,         SUB_L_I4,
                        II_MUL,         MUL_L_I4,
                        II_DIV,         DIV_L_I4,
                        RT_1_SUBSCR,    PH_1_SUBSCR,
                        RT_OUT_INT4,    SIMPLE_OUT_I4,
    7,  UPSH_INT4,
                        POP_INT4,       MOVE_ULL_I4,
                        II_ADD,         ADD_UL_I4,
                        II_SUB,         SUB_UL_I4,
                        II_MUL,         MUL_UL_I4,
                        II_DIV,         DIV_UL_I4,
                        RT_U1_SUBSCR,   PH_U1_SUBSCR,
                        RT_P1_SUBSCR,   PH_P1_SUBSCR,
    6,  PUSH_REAL8,
                        POP_REAL8,      MOVE_LL_R8,
                        DD_ADD,         ADD_L_R8,
                        DD_SUB,         SUB_L_R8,
                        DD_MUL,         MUL_L_R8,
                        DD_DIV,         DIV_L_R8,
                        RT_OUT_DBLE,    SIMPLE_OUT_R8,
    5,  UPSH_REAL8,
                        POP_REAL8,      MOVE_ULL_R8,
                        DD_ADD,         ADD_UL_R8,
                        DD_SUB,         SUB_UL_R8,
                        DD_MUL,         MUL_UL_R8,
                        DD_DIV,         DIV_UL_R8,
    5,  PUSH_IND_REAL8,
                        POP_REAL8,      MOVE_IL_R8,
                        DD_ADD,         ADD_I_R8,
                        DD_SUB,         SUB_I_R8,
                        DD_MUL,         MUL_I_R8,
                        DD_DIV,         DIV_I_R8,
    6,  PUSH_REAL4,
                        POP_REAL4,      MOVE_LL_R4,
                        RR_ADD,         ADD_L_R4,
                        RR_SUB,         SUB_L_R4,
                        RR_MUL,         MUL_L_R4,
                        RR_DIV,         DIV_L_R4,
                        RT_OUT_REAL,    SIMPLE_OUT_R4,
    5,  UPSH_REAL4,
                        POP_REAL4,      MOVE_ULL_R4,
                        RR_ADD,         ADD_UL_R4,
                        RR_SUB,         SUB_UL_R4,
                        RR_MUL,         MUL_UL_R4,
                        RR_DIV,         DIV_UL_R4,
    2,  EQ2LOGIC,
                        BC_TRUE,        BC_EQ,
                        BC_FALSE,       BC_NE,
    2,  NE2LOGIC,
                        BC_TRUE,        BC_NE,
                        BC_FALSE,       BC_EQ,
    2,  GT2LOGIC,
                        BC_TRUE,        BC_GT,
                        BC_FALSE,       BC_LE,
    2,  GE2LOGIC,
                        BC_TRUE,        BC_GE,
                        BC_FALSE,       BC_LT,
    2,  LT2LOGIC,
                        BC_TRUE,        BC_LT,
                        BC_FALSE,       BC_GE,
    2,  LE2LOGIC,
                        BC_TRUE,        BC_LE,
                        BC_FALSE,       BC_GT,
    3,  PUSH_ADDR,
                        RT_INP_INT4,    SIMPLE_INP_I4,
                        RT_INP_REAL,    SIMPLE_INP_R4,
                        RT_INP_DBLE,    SIMPLE_INP_R8,
    0 };


void    InitPeepHole() {
//======================

// initialize the peephole optimizer

    PrevOp = 0xffff;
}


void    EmitOp( unsigned_16 value ) {
//===================================

// Emit the address of the specified opcode routine.

    const unsigned_16 __FAR     *ph;
    unsigned_16                 num;

    ph = PeepHole;
    for(;;) {
        num = *ph++;
        if( num == 0 ) break;
        if( *ph++ == PrevOp ) break;
        ph += num * 2;
    }
    if( num == 0 ) {
        PrevLoc = ObjPtr;
        OutU16( MAP_FCODE( value ) );
    } else {
        for(;;) {
            if( *ph == value ) break;
            ph += 2;
            --num;
            if( num == 0 ) break;
        }
        if( num == 0 ) {
            PrevLoc = ObjPtr;
            OutU16( MAP_FCODE( value ) );
        } else {
            ++ph;
            value = *ph;
            PutU16( PrevLoc, MAP_FCODE( value ) );
        }
    }
    PrevOp = value;
}


void    EmitNulls( inttarg count ) {
//==================================

// Emit specified number of NULL bytes.

    while( count != 0 ) {
        OutByte( 0 );
        --count;
    }
}
