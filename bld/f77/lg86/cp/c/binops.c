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
// BINOPS       : Generate code for binary operations
//

#include "ftnstd.h"
#include "optr.h"
#include "opn.h"
#include "fcodes.h"
#include "global.h"

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  void            SetOpn(itnode *,int);
extern  void            ReqStkDepth(uint);
extern  void            ModStkDepth(uint);
extern  uint            GetStkEntries(uint);
extern  void            ReqStkTemps(uint,uint,uint);
extern  int             Map2BaseType(int);

static  const bool __FAR Commute[] =
                            { TRUE, FALSE, TRUE, FALSE, FALSE };
//                              +     -      *     /      **


extern  void    BinOp( int typ1, int typ2, int op ) {
//===================================================

// Generate code to perform a binary operation.

    int         temp;
    uint        need_stk;
    uint        opr;

    typ1 = Map2BaseType( typ1 );
    typ2 = Map2BaseType( typ2 );
    need_stk = GetStkEntries( typ2 );
    typ2 -= TY_INTEGER;              // make type origin 0
    opr = op - OPTR_ADD;             // make operator origin 0
    if( typ1 != -1 ) {               // binary operator
        need_stk += GetStkEntries( typ1 );
        typ1 -= TY_INTEGER;          // make type origin 0
        if( ( CITNode->opn & OPN_WHERE ) != OPN_SAFE ) {
            // operands are on the stack in the correct order
            PushOpn( CITNode->link );
            PushOpn( CITNode );
            ReqStkDepth( need_stk );
        } else {
            if( ( CITNode->link->opn & OPN_WHERE ) != OPN_SAFE ) {
                // operands are on stack in reverse order
                PushOpn( CITNode->link );
                ReqStkDepth( need_stk );
                if( Commute[ opr ] ) {
                    // operator is commutative, so just flip types around
                    temp = typ1;
                    typ1 = typ2;
                    typ2 = temp;
                } else {
                    // have to reverse operands on stack
                    EmitOp( FLIPOPS + typ1 * NUM_TYPES + typ2 );
                }
            } else {
                ReqStkDepth( need_stk );
            }
        }
        ReqStkTemps( op, typ1, typ2 );
        EmitOp( BINOPS + TABLE_SIZE * opr + NUM_TYPES * typ2 + typ1 );
    } else {
        PushOpn( CITNode->link );
        if( opr != 0 ) { // unary plus is a NOP
            ReqStkDepth( need_stk );
            EmitOp( UNIOPS + typ2 );
        }
        SetOpn( CITNode, OPN_SAFE );
    }
    ModStkDepth( GetStkEntries( ResultType ) - need_stk );
}


extern  void    ExpOp( int typ1, int typ2, int opr ) {
//====================================================

// Generate code to perform exponentiation.

    BinOp( typ1, typ2, opr );
}
