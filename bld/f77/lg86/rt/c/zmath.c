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
// ZMATH        : math support routines
//

#include "ftnstd.h"
#include "rtenv.h"
#include "ifenv.h"

single          AINT(single);
double          DINT(double);
extended        QINT(extended);
double          PowRR(double,double);


#if _8087 == _OFF

intstar4        Z_RtoI( single val ) {
//====================================

// Convert REAL*4 to INTEGER*4.

    return( val );
}


intstar4        Z_DtoI( double val ) {
//====================================

// Convert REAL*8 to INTEGER*4.

    return( val );
}


intstar4        Z_EtoI( extended val ) {
//=========================================

// Convert REAL*16 to INTEGER*4.

    return( val );
}


single  Z_ItoR( intstar4 val ) {
//==============================

// Convert INTEGER*4 to REAL*4.

    return( val );
}


single  Z_DtoR( double val ) {
//============================

// Convert REAL*8 to REAL*4.

    return( val );
}


single  Z_EtoR( extended val ) {
//==================================

// Convert REAL*16 to REAL*4.

    return( val );
}


double  Z_ItoD( intstar4 val ) {
//==============================

// Convert INTEGER*4 to REAL*8.

    return( val );
}


double  Z_RtoD( single val ) {
//============================

// Convert REAL*4 to REAL*8.

    return( val );
}

double  Z_EtoD( extended val ) {
//============================

// Convert REAL*16 to REAL*8.

    return( val );
}

extended        Z_ItoE( intstar4 val ) {
//==============================

// Convert INTEGER*4 to REAL*16.

    return( val );
}


extended        Z_RtoE( single val ) {
//============================

// Convert REAL*4 to REAL*16.

    return( val );
}

extended        Z_DtoE( double val ) {
//=========================================

// Convert REAL*8 to REAL*16.

    return( val );
}

#endif


unsigned_32     Z_U4Div( unsigned_32 arg1, unsigned_32 arg2 ) {
//=============================================================

    return( arg1 / arg2 );
}


signed_32       Z_I4Div( signed_32 arg1, signed_32 arg2 ) {
//=========================================================

    return( arg1 / arg2 );
}


#if _8087 == _OFF

single          Z_F4Add( single arg1, single arg2 ) {
//===================================================

    return( arg1 + arg2 );
}


single          Z_F4Sub( single arg1, single arg2 ) {
//===================================================

    return( arg1 - arg2 );
}


single          Z_F4Mul( single arg1, single arg2 ) {
//===================================================

    return( arg1 * arg2 );
}


single          Z_F4Div( single arg1, single arg2 ) {
//===================================================

    return( arg1 / arg2 );
}


single          Z_F4Trunc( single arg ) {
//=======================================

    return( AINT( arg ) );
}


double          Z_F8Add( double arg1, double arg2 ) {
//===================================================

    return( arg1 + arg2 );
}


double          Z_F8Sub( double arg1, double arg2 ) {
//===================================================

    return( arg1 - arg2 );
}


double          Z_F8Mul( double arg1, double arg2 ) {
//===================================================

    return( arg1 * arg2 );
}


double          Z_F8Div( double arg1, double arg2 ) {
//===================================================

    return( arg1 / arg2 );
}


double          Z_F8Trunc( double arg ) {
//=======================================

    return( DINT( arg ) );
}


int             Z_F8Cmp( double arg1, double arg2 ) {
//===================================================

    if( arg1 < arg2 ) {
        return( -1 );
    } else if( arg1 == arg2 ) {
        return( 0 );
    } else {
        return( 1 );
    }
}


double          Z_F8Pow( double arg1, double arg2 ) {
//===================================================

    return( PowRR( arg1, arg2 ) );
}


extended                Z_F16Add( extended arg1, extended arg2 ) {
//================================================================

    return( arg1 + arg2 );
}


extended                Z_F16Sub( extended arg1, extended arg2 ) {
//================================================================

    return( arg1 - arg2 );
}


extended                Z_F16Mul( extended arg1, extended arg2 ) {
//================================================================

    return( arg1 * arg2 );
}


extended                Z_F16Div( extended arg1, extended arg2 ) {
//================================================================

    return( arg1 / arg2 );
}


extended                Z_F16Trunc( extended arg ) {
//===================================================

    return( QINT( arg ) );
}


int             Z_F16Cmp( extended arg1, extended arg2 ) {
//========================================================

    if( arg1 < arg2 ) {
        return( -1 );
    } else if( arg1 == arg2 ) {
        return( 0 );
    } else {
        return( 1 );
    }
}


extended        Z_F16Pow( extended arg1, extended arg2 ) {
//=========================================================

    return( PowRR( arg1, arg2 ) );
}

#endif
