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
* Description:  Generate code for DATA initialization.
*
****************************************************************************/


#include "ftnstd.h"
#include "fcodes.h"
#include "parmtype.h"
#include "progsw.h"
#include "global.h"
#include "bglobal.h"
#include "fcgbls.h"
#include "datastmt.h"
#include "objutil.h"
#include "tdreloc.h"
#include "genobj.h"

extern  sym_id          STConst(ftn_type *,int,int);
extern  label_id        NextLabel();
extern  void            GBranch(label_id);
extern  void            GLabel(label_id);
extern  void            GenLabRef(label_id);
extern  int             ParmType(int,int);
extern  void            EmitOp(unsigned_16);
extern  void            FreeLabel(label_id);
extern  void            OpnRef(itnode *);
extern  void            BackPatch(obj_ptr);
extern  int             TypeSize(int);


extern  label_id        GDataProlog() {
//=====================================

// Start data statement code.

    label_id    data;
    targ_addr   link;

    if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
        data = NextLabel();
        GBranch( data );
    }
    if( PrevDChain.offset == NULL ) {
#if _TARGET == _8086
        DataChain.seg = CurrSeg;
#endif
        DataChain.offset = ObjPtr;
    } else {
#if _TARGET == _8086
        link.seg = CurrSeg;
#endif
        link.offset = ObjPtr;
        RelocAddr( &PrevDChain, &link );
    }
#if _TARGET == _8086
    PrevDChain.seg = CurrSeg;
#endif
    PrevDChain.offset = ObjPtr;
    OutIIPtr();                         // link to next data statement
    GenLabRef( TBLabel );
    OutObjPtr( 0 );                     // offset_const_list
    OutInt( ISNNumber );
    return( data );
}


extern  void    GDataEpilog( label_id end_data ) {
//================================================

// Finish off data statement code.

    if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
        GLabel( end_data );
        FreeLabel( end_data );
    }
}


extern  void    GBegDList() {
//===========================

// Finish off data statement code.

    BackPatch( PrevDChain.offset + offsetof( data_prol, const_ptr ) );
}


extern  void    GDataItem( itnode *rpt ) {
//========================================

// Put out the codes for one data item.

    obj_ptr     addr;
    int         typ;
    intstar4    one;

    if( rpt == NULL ) {
        one = 1;
        addr = LinkSym( STConst( (ftn_type *)&one, TY_INTEGER, TypeSize( TY_INTEGER ) ) );
    } else {
        addr = LinkSym( rpt->sym_ptr );
    }
    typ = CITNode->typ;
    if( typ == TY_HEX ) {
        typ = PT_NOTYPE;
    } else {
        typ = ParmType( typ, CITNode->size );
    }
    OutDataPtr( addr );
    OutU16( typ );
    OpnRef( CITNode );
}


extern  void    GEndDSet() {
//==========================

// Terminate set of variables.
// (i.e. DATA I,J,K/1,2,3/, M/3/ - i,j,k is a set and m is a set)

    OutInt( 0 );
}


extern  void    GEndDList() {
//===========================

// Terminate list of data.

}


extern  void    GEndVarSet() {
//============================

// Terminate list of variable.

    EmitOp( RT_END_DATA_VARS );
}
