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
// LOGOPS       : Generate code for logical operations
//

#include <ftnstd.h>
#include <opr.h>
#include <opn.h>
#include <fcodes.h>
#include <global.h>

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  void            SetOpn(itnode *,int);


#pragma off (unreferenced);
extern  void    LogOp( int typ1, int typ2, int op ) {
#pragma on (unreferenced);
//===================================================

// Generate code for a relational operator.

    PushOpn( CITNode->link );
    if( typ1 == -1 ) {    // if unary
        SetOpn( CITNode, OPN_SAFE );
    } else {
        PushOpn( CITNode );
    }
    if( _IsTypeInteger( typ2 ) ) {
        EmitOp( BITOPS + op );
    } else {
        EmitOp( LOGOPS + op );
    }
}
