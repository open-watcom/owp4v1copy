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
// BINOPS :     Generate F-Code for binary operations
//

#include "ftnstd.h"
#include "optr.h"
#include "opn.h"
#include "fcodes.h"
#include "global.h"

extern  void            EmitOp(unsigned_16);
extern  void            OutU16(unsigned_16);
extern  void            SetOpn(itnode *,int);
extern  void            GenType(itnode *);
extern  void            GenTypes(itnode *,itnode *);
extern  void            PushOpn(itnode *);
extern  bool            TypeCmplx(int);
extern  int             ParmType(int,int);
extern  intstar4        ITIntValue(itnode *);


static  bool    UnaryMul( int typ1, int typ2 ) {
//==============================================

    if( typ1 > TY_EXTENDED ) return( FALSE );
    if( !_IsTypeInteger( typ2 ) ) return( FALSE );
    if( CITNode->link->opn != OPN_CON ) return( FALSE );
    if( ITIntValue( CITNode->link ) < 0 ) return( FALSE );
    if( ITIntValue( CITNode->link ) > 8 ) return( FALSE );
    return( TRUE );
}


void            ExpOp( int typ1, int typ2, int opr ) {
//====================================================

// Generate code to perform exponentiation.

    if( UnaryMul( typ1, typ2 ) ) {
        PushOpn( CITNode );
        EmitOp( UNARY_MUL );
        GenType( CITNode );
        OutU16( ITIntValue( CITNode->link ) );
        SetOpn( CITNode, OPN_SAFE );
    } else {
        BinOp( typ1, typ2, opr );
    }
}


void            BinOp( int typ1, int typ2, int opr ) {
//====================================================

// Generate code to perform a binary operation.

    if( typ1 != -1 ) {                        // binary operator
        Binary( typ1, typ2, opr );
    } else {                                  // unary operator.
        Unary( typ2, opr );
    }
}


static  void    Unary( int typ, uint opr ) {
//==========================================

// Generate code for unary plus or unary minus.

    PushOpn( CITNode->link );
    if( opr == OPTR_SUB ) {             // unary minus
        if( TypeCmplx( typ ) ) {
            EmitOp( CUMINUS );
        } else {
            EmitOp( UMINUS );
        }
        GenType( CITNode->link );
    } else if( ( _IsTypeInteger( CITNode->link->typ ) ) &&
               ( CITNode->link->size < sizeof( intstar4 ) ) ) {
        // convert INTEGER*1 or INTEGER*2 to INTEGER*4
        EmitOp( CONVERT );
        OutU16( ( ParmType( CITNode->link->typ, CITNode->link->size ) << 8 ) |
                  ParmType( TY_INTEGER, sizeof( intstar4 ) ) );
    }
    SetOpn( CITNode, OPN_SAFE );
}


static  void    Binary( int typ1, int typ2, uint opr ) {
//======================================================

// Generate code for binary operations.

    bool        flip;
    bool        associative;

    associative = FALSE;
    if( ( opr == OPTR_ADD ) || ( opr == OPTR_MUL ) ) {
        associative = TRUE;
    }
    flip = FALSE;
    if( ( ( CITNode->opn & OPN_WHERE ) == OPN_SAFE ) &&
        ( ( CITNode->link->opn & OPN_WHERE ) != OPN_SAFE ) ) {
        flip = TRUE;
    }
    opr = BINOPS + ( opr - OPTR_ADD );
    PushOpn( CITNode->link );
    PushOpn( CITNode );
    if( TypeCmplx( typ1 ) && TypeCmplx( typ2 ) ) {
        opr += CMPLX_OPS;
        if( flip && !associative ) {
            EmitOp( CMPLX_FLIP );
        }
    } else if( TypeCmplx( typ1 ) ) {
        if( flip ) {
            if( associative ) {
                opr += XC_MIXED;
            } else {
                EmitOp( XC_FLIP );
                opr += CX_MIXED;
            }
        } else {
            opr += CX_MIXED;
        }
    } else if( TypeCmplx( typ2 ) ) {
        if( flip ) {
            if( associative ) {
                opr += CX_MIXED;
            } else {
                EmitOp( CX_FLIP );
                opr += XC_MIXED;
            }
        } else {
            opr += XC_MIXED;
        }
    } else {
        if( flip && !associative ) {
            EmitOp( FLIP );
        }
    }
    EmitOp( opr );
    if( flip && associative ) {
        GenTypes( CITNode->link, CITNode );
    } else {
        GenTypes( CITNode, CITNode->link );
    }
}
