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
// GDATA     : generate code for DATA initialization
//

#include "ftnstd.h"
#include "fcodes.h"
#include "parmtype.h"
#include "global.h"
#include "fcgbls.h"

extern  sym_id          STConst(void *,int,int);
extern  void            EmitOp(unsigned_16);
extern  void            OutPtr(pointer);
extern  void            OutU16(unsigned_16);
extern  obj_ptr         ObjTell(void);
extern  obj_ptr         ObjSeek(obj_ptr);
extern  unsigned_16     ObjOffset(obj_ptr);
extern  int             TypeSize(int);


label_id        GDataProlog() {
//=============================

// Start off data statement code.

    EmitOp( START_DATA_STMT );
    DtConstList = ObjTell();
    OutU16( 0 );
    return( NULL );
}


void    GDataEpilog( label_id label ) {
//=====================================

// Finish off data statement code.

    label = label;
}


void    GBegDList() {
//===================

// Start list of data.

    unsigned_16 const_offset;
    obj_ptr     curr_obj;

    const_offset = ObjOffset( DtConstList );
    curr_obj = ObjSeek( DtConstList );
    OutU16( const_offset );
    ObjSeek( curr_obj );
}


void    GDataItem( itnode *rpt ) {
//================================

// Generate a data item.

    sym_id      data;
    int         typ;
    intstar4    one;

    if( rpt == NULL ) {
        one = 1;
        data = STConst( &one, TY_INTEGER, TypeSize( TY_INTEGER ) );
    } else {
        data = rpt->sym_ptr;
    }
    typ = CITNode->typ;
    if( typ == TY_HEX ) {
        typ = PT_NOTYPE;
    } else {
        typ = ParmType( typ, CITNode->size );
    }
    OutPtr( data );
    OutU16( typ );
    OutPtr( CITNode->sym_ptr );
}


void    GEndDSet() {
//==================

// Terminate set of constants (i.e. Data i,j,k/1,2,3/,m/3/ - 1,2,3 is a set
// and 3 is a set).

    OutPtr( NULL );
}


void    GEndDList() {
//===================

// Terminate list of data.

}


void    GEndVarSet() {
//====================

// Terminate set of variables (i.e. Data i,j,k/1,2,3/,m/3/ - i,j,k is a set
// and m is a set).

    EmitOp( END_VAR_SET );
}
