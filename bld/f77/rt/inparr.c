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
// INPARR :     array input routine
//

#include "ftnstd.h"
#include "parmtype.h"
#include "rtenv.h"

extern  void    InpLOG1(logstar1 *);
extern  void    InpLOG4(logstar4 *);
extern  void    InpINT1(intstar1 *);
extern  void    InpINT2(intstar2 *);
extern  void    InpINT4(intstar4 *);
extern  void    InpREAL(single *);
extern  void    InpDBLE(double *);
extern  void    InpXTND(extended *);
extern  void    InpCPLX(complex *);
extern  void    InpDBCX(dcomplex *);
extern  void    InpXTCX(xcomplex *);
extern  void    InpCHAR(string *);


void    InpArr( char *arr, long int nelts, int typ ) {
//====================================================

// Input the array pointed to by 'arr' having 'nelts' elements and
// type 'typ'.

    long int    i;

    for( i = 1; i <= nelts; i++ ) {
       switch( typ ) {
       case PT_LOG_1 :
           InpLOG1( (logstar1 *)arr );
           arr += sizeof( logstar1 );
           break;
       case PT_LOG_4 :
           InpLOG4( (logstar4 *)arr );
           arr += sizeof( logstar4 );
           break;
       case PT_INT_1 :
           InpINT1( (intstar1 *)arr );
           arr += sizeof( intstar1 );
           break;
       case PT_INT_2 :
           InpINT2( (intstar2 *)arr );
           arr += sizeof( intstar2 );
           break;
       case PT_INT_4 :
           InpINT4( (intstar4 *)arr );
           arr += sizeof( intstar4 );
           break;
       case PT_REAL_4 :
           InpREAL( (single *)arr );
           arr += sizeof( single );
           break;
       case PT_REAL_8 :
           InpDBLE( (double *)arr );
           arr += sizeof( double );
           break;
       case PT_REAL_16 :
           InpXTND( (extended *)arr );
           arr += sizeof( extended );
           break;
       case PT_CPLX_8 :
           InpCPLX( (complex *)arr );
           arr += sizeof( complex );
           break;
       case PT_CPLX_16 :
           InpDBCX( (dcomplex *)arr );
           arr += sizeof( dcomplex );
           break;
       case PT_CPLX_32 :
           InpXTCX( (xcomplex *)arr );
           arr += sizeof( xcomplex );
           break;
       }
    }
}


void    InpChArr( char *arr, long int nelts, int size ) {
//=======================================================

// Input the array pointed to by 'arr' having 'nelts' elements and
// element size 'size'.

    long int    i;
    string      scb;

    scb.len = size;
    scb.strptr = arr;
    for( i = 1; i <= nelts; i++ ) {
        InpCHAR( &scb );
        scb.strptr += size;
    }
}
