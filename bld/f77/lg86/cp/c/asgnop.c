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
// ASGNOP       : Generate code for assignment
//

#include "ftnstd.h"
#include "fcodes.h"
#include "global.h"
#include "astype.h"

extern  void            GConvert(int,int);
extern  void            PushOpn(itnode *);
extern  void            PopOpn(void);
extern  void            AsgnChar(void);
extern  void            EmitOp(unsigned_16);


#pragma off (unreferenced);
extern  void    AsgnOp( int typ1, int typ2, int opr ) {
#pragma on (unreferenced);
//=====================================================

// Generate code to perform an assign operation with the necessary
// conversions.

    switch( ResultType ) {
    case TY_CHAR:
        AsgnChar();
        break;
    case TY_STRUCTURE:
        PushOpn( CITNode->link );
        PushOpn( CITNode );
        EmitOp( ASGN_STRUCT );
        break;
    default:
        PushOpn( CITNode->link );
        GConvert( typ2, typ1 );
        PopOpn();
        break;
    }
}
