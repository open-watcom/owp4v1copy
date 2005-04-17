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
// RELOPS       : relational operators
//

#include "ftnstd.h"
#include "opr.h"
#include "opn.h"
#include "fcodes.h"
#include "global.h"

extern  void            EmitOp(unsigned_16);
extern  void            OutPtr(void *);
extern  void            PushOpn(itnode *);
extern  bool            TypeCmplx(int);
extern  void            GenTypes(itnode *,itnode *);
extern  void            DumpType(uint,uint);
extern  void            SetOpn(itnode *,byte);
extern  bool            OptimalChSize(uint);
extern  uint            MapTypes(uint,uint);


void    RelOp( int typ1, int typ2, int opr ) {
//============================================

// Generate code for a relational operator.

    bool        flip;
    bool        associative;
    bool        char_1_cmp;
    int         i;
    int         j;

    // must check for "flip" before we call "CharLength" since they may
    // call "PushOpn"
    flip = FALSE;
    if( ( ( CITNode->opn & OPN_WHERE ) == OPN_SAFE ) &&
        ( ( CITNode->link->opn & OPN_WHERE ) != OPN_SAFE ) ) {
        flip = TRUE;
    }
    // must do "CITNode->link" first to get operands in the right order
    i = CharLength( CITNode->link );
    j = CharLength( CITNode );
    opr = CITNode->link->opr;
    if( ( opr == OPR_EQ ) || ( opr == OPR_NE ) ) {
        char_1_cmp = OptimalChSize( i ) && OptimalChSize( j ) && ( i == j );
        associative = TRUE;
    } else {
        char_1_cmp = (i == 1 ) && ( j == 1 );
        associative = FALSE;
    }
    PushOpn( CITNode->link );
    PushOpn( CITNode );
    opr -= FIRST_RELOP;
    if( TypeCmplx( typ1 ) && TypeCmplx( typ2 ) ) {
        opr += CC_CMP_EQ;
    } else if( TypeCmplx( typ1 ) ) {
        if( flip ) {
            opr += XC_CMP_EQ;
        } else {
            opr += CX_CMP_EQ;
        }
    } else if( TypeCmplx( typ2 ) ) {
        if( flip ) {
            opr += CX_CMP_EQ;
        } else {
            opr += XC_CMP_EQ;
        }
    } else {
        if( flip && !associative ) {
            EmitOp( FLIP );
        }
        if( typ1 == TY_CHAR ) {
            if( char_1_cmp ) {
                opr += CHAR_1_RELOPS;
            } else {
                opr += CHAR_RELOPS;
            }
        } else {
            opr += RELOPS;
        }
    }
    EmitOp( opr );
    if( char_1_cmp ) {
        if( associative ) {
            DumpType( MapTypes( TY_INTEGER, i ), i );
        } else {
            // Assert: comparing CHARACTER*1 with LT, LE, GT, or GE
            // Consider: CHARACTER A/'a'/
            //           IF( A .lt. CHAR(159) ) PRINT *, 'OK'
            // we must generate an unsigned comparison
            DumpType( MapTypes( TY_LOGICAL, i ), i );
        }
        if( flip && associative ) {
            GenChar1Op( CITNode->link );
            GenChar1Op( CITNode );
        } else {
            GenChar1Op( CITNode );
            GenChar1Op( CITNode->link );
        }
    } else if( typ1 != TY_CHAR ) {
        if( flip && associative ) {
            GenTypes( CITNode->link, CITNode );
        } else {
            GenTypes( CITNode, CITNode->link );
        }
    }
}


static  int     CharLength( itnode *op ) {
//========================================

    byte        opn;

    if( op->typ != TY_CHAR ) return( 0 );
    opn = op->opn;
    if( opn & OPN_SS1 ) return( op->value.st.ss_size );
    if( ( opn & OPN_WHERE ) == OPN_SAFE ) return( 0 );
    if( ( opn & OPN_WHAT ) == OPN_NNL ) {
        return( op->sym_ptr->ns.xt.size );
    }
    if( ( opn & OPN_WHAT ) == OPN_CON ) {
        return( op->sym_ptr->lt.length );
    }
    return( 0 );
}


void    GenChar1Op( itnode *op ) {
//================================

    if( ( ( op->opn & OPN_WHAT ) == OPN_CON ) ) {
        OutPtr( op->sym_ptr );
        SetOpn( op, OPN_SAFE );
    } else {
        OutPtr( NULL );
    }
    op->opn &= ~OPN_SS1;
}
