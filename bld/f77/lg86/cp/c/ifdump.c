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
// IFDUMP       : Intrinsic function processing
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "opn.h"
#include "wf77aux.h"

extern  void            ProcList(itnode *);
extern  void            SetOpn(itnode *,int);
extern  void            PushOpn(itnode *);
extern  void            OpnRef(itnode *);
extern  int             TypeSize(int);
extern  void            GCnvTo(uint,uint);
extern  void            AdvanceITPtr(void);
extern  void            EmitOp(uint);
extern  void            GArg(void);
extern  int             AdjCallList(itnode *,aux_info *);
extern  int             GParm(pass_by *);


void    IFDump() {
//================

// Dump intrinsic functions.

}


static  void    MakeComplex( int arg_type, int if_type ) {
//========================================================

// Common routines for above 2 routines.

    itnode      *cit;

    cit = CITNode;
    AdvanceITPtr();
    PushOpn( CITNode );                       // push the imaginary part
    GCnvTo( arg_type, TypeSize( arg_type ) ); // convert it to correct type
    CITNode = cit;
    PushOpn( CITNode );                       // push the real part
    GCnvTo( arg_type, TypeSize( arg_type ) ); // convert it to correct type
    AdvanceITPtr();
    cit->typ = if_type;
    cit->size = TypeSize( if_type );
}


void    GMakeCplx() {
//===================

// Convert 2 arguments (       a1  ,       a2   )
// into complex number ( real( a1 ), real( a2 ) )

    MakeComplex( TY_REAL, TY_COMPLEX );
}


void    GMakeDCplx() {
//====================

// Convert 2 arguments (       a1  ,       a2   )
// into double complex number ( double( a1 ), double( a2 ) )

    MakeComplex( TY_DOUBLE, TY_DCOMPLEX );
}


void    GMakeXCplx() {
//====================

// Convert 2 arguments (       a1  ,       a2   )
// into double complex number ( double( a1 ), double( a2 ) )

    MakeComplex( TY_EXTENDED, TY_XCOMPLEX );
}


void    GLoc( void ) {
//====================

    GArg();
    AdjCallList( CITNode, NULL );
    EmitOp( LOCATION_OF );
    GParm( NULL );
    SetOpn( CITNode, OPN_SAFE );
}


void    GVolatile() {
//===================

    PushOpn( CITNode );
}
