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
// RELOPS       : Generate code for relational expressions
//

#include "ftnstd.h"
#include "opr.h"
#include "opn.h"
#include "fcodes.h"
#include "global.h"

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  bool            ChkUDef(itnode *);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  int             GetStkEntries(int);
extern  int             Map2BaseType(int);


extern  void    RelOp( int typ1, int typ2, int opr ) {
//====================================================

// Generate code for a relational operator.

    uint        need_stk;
    int         temp;

    typ1 = Map2BaseType( typ1 );
    typ2 = Map2BaseType( typ2 );
    opr = CITNode->link->opr - FIRST_RELOP;
    if( ( CITNode->opn & OPN_WHERE ) != OPN_SAFE ) {
        // operands are in the correct order
        PushOpn( CITNode->link );
        PushOpn( CITNode );
    } else {
        if( ( CITNode->link->opn & OPN_WHERE ) != OPN_SAFE ) {
            // operands are in the reverse order
            temp = typ1;
            typ1 = typ2;
            typ2 = temp;
            PushOpn( CITNode->link );
            opr += NUM_RELS;
        }
    }  // else operands are already on the stack in the correct order
    if( ResultType == TY_CHAR ) {
        if( ChkUDef( CITNode ) ) {
            EmitOp( UCHAR_CMP );
        } else {
            EmitOp( CHAR_CMP );
        }
    } else {
        need_stk = GetStkEntries( typ1 ) + GetStkEntries( typ2 );
        ReqStkDepth( need_stk );
        EmitOp( CMPOPS + NUM_TYPES * ( typ2 - TY_INTEGER )
                       + typ1 - TY_INTEGER );
        ModStkDepth( -need_stk );
    }
    EmitOp( RELOPS + opr );
}
