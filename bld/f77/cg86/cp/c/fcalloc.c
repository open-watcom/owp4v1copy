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
// FCALLOC :    Generate call to dynamically allocate/deallocate array
//

#include "ftnstd.h"
#include "fcodes.h"
#include "symbol.h"
#include "wf77cg.h"
#include "wf77defs.h"
#include "falloc.h"

//=================== Back End Code Generation Routines ====================

extern  cg_name         CGInteger(signed_32,cg_type);
extern  cg_name         CGCall(call_handle);
extern  cg_name         CGUnary(cg_op,cg_name,cg_type);
extern  cg_name         CGBinary(cg_op,cg_name,cg_name,cg_type);
extern  cg_name         CGFlow(cg_op,cg_name,cg_name);
extern  cg_name         CGBackName(back_handle,cg_type);
extern  cg_name         CGFEName(sym_handle,cg_type);
extern  cg_name         CGCompare(cg_op,cg_name,cg_name,cg_type);
extern  void            CGControl(cg_op,cg_name,label_handle);
extern  void            CGAddParm(call_handle,cg_name,cg_type);
extern  void            CGDone(cg_name);
extern  void            CGDone(cg_name);
extern  label_handle    BENewLabel(void);
extern  void            BEFiniLabel(label_handle);
extern  unsigned long   BETypeLength(cg_type);

//=========================================================================

extern  call_handle     InitCall(int);
extern  void            *GetPtr(void);
extern  unsigned_16     GetU16(void);
extern  cg_name         XPop(void);
extern  cg_name         XPopValue(cg_type);
extern  cg_name         StructRef(cg_name,int);
extern  void            FCodeSequence(void);
extern  void            SymPush(sym_id);
extern  sym_id          SymPop(void);
extern  void            XPush(cg_name);
extern  cg_name         SymAddr(sym_id);
extern  cg_type         ArrayPtrType(sym_id);
extern  cg_name         GetAdv(sym_id);


#if _TARGET == _8086 || _TARGET == _80386
  #define FLAG_PARM_TYPE        T_UINT_2
#else
  #define FLAG_PARM_TYPE        T_UINT_4
#endif

static cg_name  getFlags( sym_id sym ) {
//======================================

    int                 tlen;
    cg_name             fl;
    cg_type             typ;

    if( sym->ns.flags & SY_SUBSCRIPTED ) {
        typ = ArrayPtrType( sym );
        tlen = BETypeLength( typ );
    } else {
        tlen = BETypeLength( T_CHAR );
        typ = T_CHAR;
    }
    fl = StructRef( CGFEName( sym, typ ), tlen );
    return( CGUnary( O_POINTS, fl, T_UINT_2 ) );
}


void            FCAllocate() {
//============================

    call_handle         handle;
    sym_id              arr;
    act_dim_list        *dim;
    uint                num;
    unsigned_16         alloc_flags;
    cg_name             expr_stat;
    cg_name             expr_loc;
    cg_name             fl;
    label_handle        label;

    num = 0;
    SymPush( NULL );
    for(;;) {
        arr = GetPtr();
        if( arr == NULL ) break;
        // check if array is already allocated before filling in ADV
        label = BENewLabel();
        fl = getFlags( arr );
        fl = CGBinary( O_AND, fl, CGInteger( ALLOC_MEM, T_UINT_2 ), T_UINT_2 );
        CGControl( O_IF_TRUE, CGCompare( O_NE, fl,
                                CGInteger( 0, T_UINT_2 ), T_UINT_2 ), label );
        FCodeSequence(); // fill in the ADV, SCB or RCB
        CGControl( O_LABEL, NULL, label );
        BEFiniLabel( label );
        SymPush( arr );
        ++num;
    }
    alloc_flags = GetU16();
    if( alloc_flags & ALLOC_NONE ) {
        expr_loc = CGInteger( 0, T_POINTER );
    } else {
        FCodeSequence();
        if( alloc_flags & ALLOC_LOC ) {
            expr_loc = XPopValue( T_INT_4 );
            if( alloc_flags & ALLOC_STAT ) {
                FCodeSequence();
                expr_stat = XPop();
            }
        } else {
            expr_stat = XPop();
        }
    }
    handle = InitCall( RT_ALLOCATE );
    for(;;) {
        arr = SymPop();
        if( arr == NULL ) break;
        if( arr->ns.flags & SY_SUBSCRIPTED ) {
            dim = arr->ns.si.va.dim_ext;
            CGAddParm( handle, CGInteger( _SymSize( arr ), T_INT_4 ),
                       T_INT_4 );
            CGAddParm( handle, CGInteger( _DimCount( dim->dim_flags ),
                                          T_INTEGER ),
                       T_INTEGER );
            CGAddParm( handle, GetAdv( arr ), T_LOCAL_POINTER );
        }
        CGAddParm( handle, CGFEName( arr, T_POINTER ), T_POINTER );
        CGAddParm( handle, getFlags( arr ), FLAG_PARM_TYPE );
    }
    if( alloc_flags & ALLOC_NONE ) {
        CGAddParm( handle, expr_loc, T_POINTER );
    } else {
        if( alloc_flags & ALLOC_LOC ) {
            CGAddParm( handle, expr_loc, T_INT_4 );
        }
        if( alloc_flags & ALLOC_STAT ) {
            CGAddParm( handle, expr_stat, T_POINTER );
        }
    }
    CGAddParm( handle, CGInteger( num, T_INTEGER ), T_INTEGER );
    CGAddParm( handle, CGInteger( alloc_flags, T_UINT_2 ), FLAG_PARM_TYPE );
    CGDone( CGCall( handle ) );
}


void            FCDeAllocate() {
//==============================

    call_handle         handle;
    sym_id              arr;
    uint                num;

    num = 0;
    handle = InitCall( RT_DEALLOCATE );
    for(;;) {
        arr = GetPtr();
        if( arr == NULL ) break;
        CGAddParm( handle, CGFEName( arr, T_POINTER ), T_POINTER );
        CGAddParm( handle, getFlags( arr ), FLAG_PARM_TYPE );
        ++num;
    }
    CGAddParm( handle, CGInteger( num, T_INTEGER ), T_INTEGER );
    if( GetU16() & ALLOC_STAT ) {
        FCodeSequence();
        CGAddParm( handle, XPop(), T_POINTER );
    } else {
        CGAddParm( handle, CGInteger( 0, T_POINTER ), T_POINTER );
    }
    CGDone( CGCall( handle ) );
}


void    FCAllocated() {
//=====================

// Generate code for ALLOCATED intrinsic function.

    uint                type;
    cg_name             fl;

    type = GetU16();
    fl = XPop();

    if( type & ALLOC_STRING ) {
        fl = CGUnary( O_POINTS, fl, T_POINTER );
    }
    XPush( CGCompare( O_NE, fl, CGInteger( 0, T_POINTER ), T_POINTER ) );
}
