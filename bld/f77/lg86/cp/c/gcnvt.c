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
// GCNVT        : generate code to perform type conversion
//

#include "ftnstd.h"
#include "fcodes.h"
#include "global.h"

extern  void            PushOpn(itnode *);
extern  void            EmitOp(unsigned_16);
extern  int             TypeSize(uint);
extern  uint            GetStkEntries(uint);
extern  void            ReqStkDepth(uint);
extern  void            ModStkDepth(uint);
extern  int             Map2BaseType(int);


#pragma off (unreferenced);
void            GCnvTo( uint typ, uint size ) {
#pragma on (unreferenced);
//=============================================

// Convert operand to given type.

    PushOpn( CITNode );
    GConvert( CITNode->typ, typ );
    CITNode->typ = typ;
    CITNode->size = TypeSize( typ );
}


void            GILCnvTo( uint typ, uint size ) {
//===============================================

// Convert operand to given type.

    GCnvTo( typ, size );
}


void            GConvert( uint old_typ, uint new_typ ) {
//======================================================

// Convert operand to given type.

    unsigned_16 cnv_op;

    old_typ = Map2BaseType( old_typ );
    new_typ = Map2BaseType( new_typ );
    if( old_typ != new_typ ) {
        cnv_op = CNVOPS + NUM_TYPES * ( new_typ - TY_INTEGER )
                                     + ( old_typ - TY_INTEGER );
        ReqStkDepth( GetStkEntries( old_typ ) );
        EmitOp( cnv_op );
        ModStkDepth( GetStkEntries( new_typ ) -
                     GetStkEntries( old_typ ) );
    }
}
