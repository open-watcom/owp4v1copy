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
// GDO       : code generation for DO for(;;) {s
//

#include "ftnstd.h"
#include "fcodes.h"
#include "parmtype.h"
#include "global.h"
#include "doloop.h"

extern  void            EatDoParm(void);
extern  void            AdvanceITPtr(void);
extern  int             ParmType(int,int);
extern  bool            RecComma(void);
extern  bool            ReqComma(void);
extern  void            GCnvTo(uint,uint);
extern  void            GConvert(uint,uint);
extern  void            EmitOp(unsigned_16);
extern  sym_id          StaticAlloc(int,int);
extern  void            PushAddr(itnode *);
extern  void            PushConst(int);
extern  void            GenTmpRef(sym_id);
extern  void            GenLabRef(label_id);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  int             GetStkEntries(int);
extern  uint            TypeSize(uint);

static  const byte __FAR        DoCBSizes[] = {     // DO control block sizes
       sizeof( do_intstar4 ),   // PT_INT_1
       sizeof( do_intstar4 ),   // PT_INT_2
       sizeof( do_intstar4 ),   // PT_INT_4
       sizeof( do_single ),     // PT_REAL_4
       sizeof( do_double ),     // PT_REAL_8
       sizeof( do_extended )    // PT_REAL_16
    };


extern  void    GDoInit( int do_type ) {
//======================================

// Initialize a DO or implied-DO.
// Process "DO I==e1,e2,e3" where e1, e2 and e3 are numeric expressions.

    do_entry    *do_pointer;
    itnode      *do_var;

    do_pointer = CSHead->cs_info.do_parms;
    do_pointer->do_type = ParmType( do_type, CITNode->size ) - PT_INT_1;
    do_var = CITNode;                    // save pointer to do variable
    AdvanceITPtr();                      // bump past the "="
    DoExpr( do_type );                   // process e1
    if( ReqComma() ) {
        DoExpr( do_type );               // process e2
        if( RecComma() ) {
            DoExpr( do_type );           // process e3
        } else {
            PushConst( 1 );              // indicate unit incrementation
            GConvert( TY_INTEGER, do_type );
        }
    }
    ReqStkDepth( 3 * GetStkEntries( do_type ) );
    PushAddr( do_var );
    EmitOp( do_pointer->do_type + I1DO_INIT );
    do_pointer->do_cb = StaticAlloc( DoCBSizes[ do_pointer->do_type ],
                                     TY_NO_TYPE );
    GenTmpRef( do_pointer->do_cb );
    GenLabRef( CSHead->bottom );
    ModStkDepth( -3 * GetStkEntries( do_type ) );
}


extern  void    DoExpr( int typ ) {
//=================================

// Evaluate a DO expression (e1, e2 or e3 ).

    EatDoParm();
    if( !AError ) {
        GCnvTo( typ, TypeSize( typ ) );
    }
    AdvanceITPtr();
}


extern  void    GDoEnd() {
//========================

// Terminate a DO or an implied-DO.

    do_entry    *do_pointer;

    do_pointer = CSHead->cs_info.do_parms;
    EmitOp( do_pointer->do_type + I1DO_LOOP );
    GenTmpRef( do_pointer->do_cb );
}
