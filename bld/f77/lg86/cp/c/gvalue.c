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
// GVALUE       : Generate code to generate value for address
//

#include "ftnstd.h"
#include "fcodes.h"
#include "symflgs.h"

extern  void            EmitOp(unsigned_16);
extern  int             ParmType(int,int);
extern  void            IncStkDepth(int);
extern  void            DecStkDepth(int);
extern  int             GetStkEntries(int);
extern  void            ReqStkDepth(int);


extern  void    GValue( int typ, int size ) {
//===========================================

// Pointer to value is on the stack. Replace it with the actual value.

    IncStkDepth( typ );
    EmitOp( VALOPS + ParmType( typ, size ) - 1 );
}


extern  void    GUValue( int typ, int size ) {
//============================================

// Pointer to value is on the stack. Replace it with the actual value.
// used after array subscripting

    IncStkDepth( typ );
    EmitOp( UVALOPS + ParmType( typ, size ) - 1 );
}


extern  void    GUFValue( int typ, int size ) {
//=============================================

// Pointer to value is on the stack. Replace it with the actual value.
// used after field offset calculation

    IncStkDepth( typ );
    EmitOp( UVALFOPS + ParmType( typ, size ) - 1 );
}


extern  void    GStore( int typ, int size ) {
//===========================================

// Pointer to value is on the stack. Replace it with the actual value.

    ReqStkDepth( GetStkEntries( typ ) );
    EmitOp( STOROPS + ParmType( typ, size ) - 1 );
    DecStkDepth( typ );
}
