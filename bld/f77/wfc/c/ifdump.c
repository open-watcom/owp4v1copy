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
// IFDUMP    : intrinsic function processing
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "emitobj.h"
#include "types.h"
#include "utility.h"


void    GMakeCplx() {
//===================

// Convert 2 arguments (       a1  ,       a2   )
// into complex number ( real( a1 ), real( a2 ) )

    EmitOp( MAKE_COMPLEX );
    MakeComplex( TY_COMPLEX );
}


void    GMakeDCplx() {
//====================

// Convert 2 arguments (       a1  ,       a2   )
// into double complex number ( double( a1 ), double( a2 ) )

    EmitOp( MAKE_DCOMPLEX );
    MakeComplex( TY_DCOMPLEX );
}


void    GMakeXCplx() {
//====================

// Convert 2 arguments (       a1  ,       a2   )
// into double complex number ( double( a1 ), double( a2 ) )

    EmitOp( MAKE_XCOMPLEX );
    MakeComplex( TY_XCOMPLEX );
}


static  void    MakeComplex( TYPE if_type ) {
//==========================================

// Common routines for above 2 routines.

    GenType( CITNode );
    CITNode->typ = if_type;
    CITNode->size = TypeSize( if_type );
    AdvanceITPtr();
}


static  void    G2Math( uint ifn ) {
//==================================

    EmitOp( ifn );
    GenTypes( CITNode, CITNode->link );
    AdvanceITPtr();
}


void    GModulus() {
//==================

    G2Math( MODULUS );
}


void    GSign() {
//===============

    G2Math( SIGN );
}


void    GCharLen() {
//==================

    EmitOp( CHAR_LEN );
    AdvanceITPtr();
}


void    GImag() {
//===============

    GMath( IMAG );
}


void    GConjg() {
//================

    GMath( CONJG );
}


void    GDProd() {
//================

    EmitOp( DPROD );
    AdvanceITPtr();
}


void    GXProd() {
//================

    EmitOp( XPROD );
    AdvanceITPtr();
}


void    GMin( TYPE func_type ) {
//=============================

    EmitOp( MIN );
    GenTypes( CITNode, CITNode->link );
    DumpType( func_type, TypeSize( func_type ) );
    AdvanceITPtr();
}


void    GMax( TYPE func_type ) {
//=============================

    EmitOp( MAX );
    GenTypes( CITNode, CITNode->link );
    DumpType( func_type, TypeSize( func_type ) );
    AdvanceITPtr();
}


static  void    GBitFunction( unsigned_16 fn ) {
//==============================================

    EmitOp( fn );
    GenTypes( CITNode, CITNode->link );
    AdvanceITPtr();
}


void    GBitTest() {
//==================

    GBitFunction( BIT_TEST );
}


void    GBitSet() {
//=================

    GBitFunction( BIT_SET );
}


void    GBitClear() {
//===================

    GBitFunction( BIT_CLEAR );
}


void    GBitOr() {
//================

    GBitFunction( BIT_OR );
}


void    GBitAnd() {
//=================

    GBitFunction( BIT_AND );
}


void    GBitNot() {
//=================

    EmitOp( BIT_NOT );
    GenType( CITNode );
    AdvanceITPtr();
}


void    GBitExclOr() {
//====================

    GBitFunction( BIT_EXCL_OR );
}


void    GBitChange() {
//====================

    GBitFunction( BIT_CHANGE );
}


void    GBitLShift() {
//====================

    GBitFunction( LEFT_SHIFT );
}


void    GBitRShift() {
//====================

    GBitFunction( RIGHT_SHIFT );
}


void            GMod() {
//======================

    GMath( MATH_MOD );
}


void            GAbs() {
//======================

    GMath( MATH_ABS );
}


void            GASin() {
//=======================

    GMath( MATH_ASIN );
}


void            GACos() {
//=======================

    GMath( MATH_ACOS );
}


void            GATan() {
//=======================

    GMath( MATH_ATAN );
}


void            GATan2() {
//========================

    GMath( MATH_ATAN2 );
}


void            GLog() {
//======================

    GMath( MATH_LOG );
}


void            GLog10() {
//========================

    GMath( MATH_LOG10 );
}


void            GCos() {
//======================

    GMath( MATH_COS );
}


void            GSin() {
//======================

    GMath( MATH_SIN );
}


void            GTan() {
//======================

    GMath( MATH_TAN );
}


void            GSinh() {
//=======================

    GMath( MATH_SINH );
}


void            GCosh() {
//=======================

    GMath( MATH_COSH );
}


void            GTanh() {
//=======================

    GMath( MATH_TANH );
}


void            GSqrt() {
//=======================

    GMath( MATH_SQRT );
}


void            GExp() {
//======================

    GMath( MATH_EXP );
}


static  void    GMath( uint ifn ) {
//=================================

    EmitOp( ifn );
    GenType( CITNode );
    AdvanceITPtr();
}


void    GLoc() {
//==============

    PushOpn( CITNode );
    EmitOp( LOC );
    GenType( CITNode );
    AdvanceITPtr();
}


void    GVolatile() {
//===================

    PushOpn( CITNode );
    EmitOp( VOLATILE );
    AdvanceITPtr();
}
