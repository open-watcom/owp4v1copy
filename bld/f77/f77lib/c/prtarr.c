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
// PRTARR :     array output routine
//

#include "ftnstd.h"
#include "parmtype.h"
#include "rtenv.h"

extern  void    OutLOG1(logstar1);
extern  void    OutLOG4(logstar4);
extern  void    OutINT1(intstar1);
extern  void    OutINT2(intstar2);
extern  void    OutINT4(intstar4);
extern  void    OutREAL(single);
extern  void    OutDBLE(double);
extern  void    OutXTND(extended);
extern  void    OutCPLX(single,single);
extern  void    OutDBCX(double,double);
extern  void    OutXTCX(extended,extended);
extern  void    OutCHAR(string *);


void    PrtArr( char *arr, long int nelts, int typ ) {
//====================================================

// Print the array pointed to by 'arr' having 'nelts' elements and
// type 'typ'.

    long int    i;
    single      r1;
    single      r2;
    double      d1;
    double      d2;
    extended    e1;
    extended    e2;

    for( i = 1; i <= nelts; i++ ) {
        switch( typ ) {
        case PT_LOG_1 :
            OutLOG1( *(logstar1 *)arr );
            arr += sizeof( logstar1 );
            break;
        case PT_LOG_4 :
            OutLOG4( *(logstar4 *)arr );
            arr += sizeof( logstar4 );
            break;
        case PT_INT_1 :
            OutINT1( *(intstar1 *)arr );
            arr += sizeof( intstar1 );
            break;
        case PT_INT_2 :
            OutINT2( *(intstar2 *)arr );
            arr += sizeof( intstar2 );
            break;
        case PT_INT_4 :
            OutINT4( *(intstar4 *)arr );
            arr += sizeof( intstar4 );
            break;
        case PT_REAL_4 :
            OutREAL( *(single *)arr );
            arr += sizeof( single );
            break;
        case PT_REAL_8 :
            OutDBLE( *(double *)arr );
            arr += sizeof( double );
            break;
        case PT_REAL_16 :
            OutXTND( *(extended *)arr );
            arr += sizeof( extended );
            break;
        case PT_CPLX_8 :
            r1 = *(single *)arr;
            arr += sizeof( single );
            r2 = *(single *)arr;
            OutCPLX( r1, r2 );
            arr += sizeof( single );
            break;
        case PT_CPLX_16 :
            d1 = *(double *)arr;
            arr += sizeof( double );
            d2 = *(double *)arr;
            OutDBCX( d1, d2 );
            arr += sizeof( double );
            break;
        case PT_CPLX_32 :
            e1 = *(extended *)arr;
            arr += sizeof( extended );
            e2 = *(extended *)arr;
            OutXTCX( e1, e2 );
            arr += sizeof( extended );
            break;
        }
    }
}


void    PrtChArr( char *arr, long int nelts, int size ) {
//=======================================================

// Print the array pointed to by 'arr' having 'nelts' elements and
// element size 'size'.

    long int    i;
    string      scb;

    scb.len = size;
    scb.strptr = arr;
    for( i = 1; i <= nelts; i++ ) {
        OutCHAR( &scb );
        scb.strptr += size;
    }
}
