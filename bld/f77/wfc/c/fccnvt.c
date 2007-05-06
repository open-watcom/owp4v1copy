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
// FCCNVT    : conversion F-Code processor
//

#include "ftnstd.h"
#include "wf77defs.h"
#include "wf77cg.h"
#include "emitobj.h"
#include "fctypes.h"
#include "cgswitch.h"
#define  BY_CLI
#include "cgprotos.h"


extern  void            XPush(cg_name);
extern  cg_name         XPopValue(cg_type);
extern  void            XPopCmplx(cg_cmplx *,cg_type);
extern  cg_name         SCBPointer(cg_name);
extern  cg_name         XPop(void);


void    FCIChar( void ) {
//=================

    XPush( CGUnary( O_CONVERT,
                    CGUnary( O_POINTS, SCBPointer( XPop() ), T_UINT_1 ),
                    T_UINT_4 ) );
}


void    FCConvert( void ) {
//===================

// Perform type conversion.

    unsigned_16 typ_info;
    cg_type     from;
    cg_type     to;
    cg_cmplx    z;

    typ_info = GetU16();
    from = GetType1( typ_info );
    to = GetType2( typ_info );
    if( from == T_COMPLEX ) {
        XPopCmplx( &z, T_COMPLEX );
        if( to == T_DCOMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_DOUBLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_DOUBLE ) );
        } else if( to == T_COMPLEX ) {
            XPush( z.imagpart );
        } else if( to == T_XCOMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_LONGDOUBLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_LONGDOUBLE ) );
        } else {
            CGTrash( z.imagpart );
            if( to != T_SINGLE ) {
                z.realpart = CGUnary( O_CONVERT, z.realpart, to );
            }
        }
        XPush( z.realpart );
    } else if( from == T_DCOMPLEX ) {
        XPopCmplx( &z, T_DCOMPLEX );
        if( to == T_COMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_SINGLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_SINGLE ) );
        } else if( to == T_DCOMPLEX ) {
            XPush( z.imagpart );
        } else if( to == T_XCOMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_LONGDOUBLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_LONGDOUBLE ) );
        } else {
            CGTrash( z.imagpart );
            if( to != T_DOUBLE ) {
                z.realpart = CGUnary( O_CONVERT, z.realpart, to );
            }
        }
        XPush( z.realpart );
    } else if( from == T_XCOMPLEX ) {
        XPopCmplx( &z, T_XCOMPLEX );
        if( to == T_COMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_SINGLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_SINGLE ) );
        } else if( to == T_DCOMPLEX ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, T_DOUBLE );
            XPush( CGUnary( O_CONVERT, z.imagpart, T_DOUBLE ) );
        } else if( to == T_XCOMPLEX ) {
            XPush( z.imagpart );
        } else {
            CGTrash( z.imagpart );
            if( to != T_LONGDOUBLE ) {
                z.realpart = CGUnary( O_CONVERT, z.realpart, to );
            }
        }
        XPush( z.realpart );
    } else { // converting a scalar
        z.realpart = XPopValue( from );
        if( to == T_COMPLEX ) {
            to = T_SINGLE;
            XPush( CGInteger( 0, to ) );
        } else if( to == T_DCOMPLEX ) {
            to = T_DOUBLE;
            XPush( CGInteger( 0, to ) );
        } else if( to == T_XCOMPLEX ) {
            to = T_LONGDOUBLE;
            XPush( CGInteger( 0, to ) );
        }
        if( to != from ) {
            z.realpart = CGUnary( O_CONVERT, z.realpart, to );
        }
        XPush( z.realpart );
    }
}


void            FCMakeComplex( void ) {
//===============================

// Make a COMPLEX number from 2 scalars.

    MakeComplex( T_SINGLE );
}


void            FCMakeDComplex( void ) {
//================================

// Make a DCOMPLEX number from 2 scalars.

    MakeComplex( T_DOUBLE );
}


void            FCMakeXComplex( void ) {
//================================

// Make a XCOMPLEX number from 2 scalars.

    MakeComplex( T_LONGDOUBLE );
}


static  void    MakeComplex( cg_type to ) {
//=========================================

// Make a COMPLEX/COMPLEX*16/COMPLEX*20 number from 2 scalars.

    cg_type     arg_type;
    cg_name     real_part;
    cg_name     imag_part;

    arg_type = GetType( GetU16() );
    real_part = XPopValue( arg_type );
    imag_part = XPopValue( arg_type );
    if( arg_type != to ) {
        real_part = CGUnary( O_CONVERT, real_part, to );
        imag_part = CGUnary( O_CONVERT, imag_part, to );
    }
    XPush( imag_part );
    XPush( real_part );
}
