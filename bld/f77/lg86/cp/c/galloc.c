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
// GALLOC :     Generate call to dynamically allocate/deallocate array
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "falloc.h"
#include "opn.h"
#include "objutil.h"
#include "tdreloc.h"

extern  void            EmitOp(uint);
extern  void            PushOpn(itnode *);
extern  void            OpnRef(itnode *);
extern  void            SetOpn(itnode *,int);
extern  void            DimArray(sym_id);
extern  void            LoadSCB(sym_id);

static  uint            NumberOfArgs;
static  unsigned_16     alloc_Flags;


void    GBegAllocate() {
//======================

    NumberOfArgs = 0;
    alloc_Flags = ALLOC_NONE;
}


void    GAllocate( sym_id sym ) {
//===============================

    DimArray( sym );
    EmitOp( PUSH_ADDR_STACK );
    SymRef( sym );
    EmitOp( PUSH_INLINE ); // emit type of allocation
    OutInt( 0 );
    ++NumberOfArgs;
}


void    GAllocateString( sym_id sym ) {
//=====================================

    LoadSCB( sym );
    EmitOp( PUSH_ADDR_STACK );
    SymRef( sym );
    EmitOp( PUSH_INLINE ); // emit type of allocation
    OutInt( ALLOC_STRING );
    ++NumberOfArgs;
}


void    GAllocEOL( void ) {
//=========================

}


void    GEndAllocate() {
//======================

    EmitOp( RT_ALLOCATE );
    OutU16( ( alloc_Flags << 8 ) | NumberOfArgs );
}


void    GBegDeAllocate() {
//========================

    NumberOfArgs = 0;
    alloc_Flags = ALLOC_NONE;
}


void    GDeAllocate( sym_id sym ) {
//=================================

    EmitOp( PUSH_ADDR_STACK );
    SymRef( sym );
    EmitOp( PUSH_INLINE ); // emit type of allocation
    OutInt( 0 );
    ++NumberOfArgs;
}


void    GDeAllocateString( sym_id sym ) {
//=======================================

    EmitOp( PUSH_ADDR_STACK );
    SymRef( sym );
    EmitOp( PUSH_INLINE ); // emit type of allocation
    OutInt( ALLOC_STRING );
    ++NumberOfArgs;
}


void    GEndDeAllocate() {
//========================

    EmitOp( RT_DEALLOCATE );
    OutU16( ( alloc_Flags << 8 ) | NumberOfArgs );
}


void    GAllocLoc() {
//===================

    alloc_Flags &= ~ALLOC_NONE;
    alloc_Flags |= ALLOC_LOC;
    PushOpn( CITNode );
}


void    GAllocStat() {
//====================

    alloc_Flags &= ~ALLOC_NONE;
    alloc_Flags |= ALLOC_STAT;
    EmitOp( PUSH_ADDR_STACK );
    OpnRef( CITNode );
    SetOpn( CITNode, OPN_SAFE );
}


void    GAllocated() {
//====================

// Generate code for ALLOCATED intrinsic function.

    EmitOp( ALLOCATED );
    OpnRef( CITNode );
    SetOpn( CITNode, OPN_SAFE );
}


void GSCBLength( sym_id sym ) {
//=============================

    PushOpn( CITNode );         // push length of SCB
    EmitOp( SET_SCB_LEN );
    SymRef( sym );
}
