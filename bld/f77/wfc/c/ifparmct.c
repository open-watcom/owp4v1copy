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
// IFPARMCT  : intrinsic function argument count table
//

#include "ftnstd.h"
#include "errcod.h"
#include "ifargs.h"

extern  void            Error(int,...);
extern  void            Extension(int,...);

extern  char            *IFNames[];
extern  const byte __FAR    IFArgCt[];


void    IFChkExtension( uint func ) {
//===================================

    if( IFArgCt[ func ] & IF_EXTENSION ) {
        Extension( LI_IF_NOT_STANDARD, IFNames[ func ] );
    }
}


void    IFCntPrms( uint func, byte actual_cnt ) {
//===============================================

    int         need;

    need = IFArgCt[ func ] & IF_COUNT_MASK;
    if( need == TWO_OR_MORE ) {
        if( actual_cnt >= 2 ) return;
    } else if( need == ONE_OR_TWO ) {
        if( actual_cnt <= 2 ) return;
    } else {
        if( need == actual_cnt ) return;
    }
    Error( AR_BAD_COUNT, IFNames[ func ] );
}


bool    IFGenInLine( uint func ) {
//================================

    return( ( IFArgCt[ func ] & IF_IN_LINE ) != 0 );
}
