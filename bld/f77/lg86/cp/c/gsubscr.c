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
// GSUBSCR      : Generate subscript code
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "opn.h"
#include "objutil.h"
#include "tdreloc.h"

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  void            OpnRef(itnode *);
extern  bool            ChkUDef(itnode *);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  uint            GetStkEntries(uint);
extern  void            SetOpn(itnode *,int);


#pragma off (unreferenced);
extern  void    GBegSubScr( itnode *array_node ) {
#pragma on (unreferenced);
//================================================

// Start a subscript operation.

}


extern  void    GSubScr() {
//=========================

// Generate one subscript.

    PushOpn( CITNode );
}


extern  void    GEndSubScr( itnode *itptr ) {
//===========================================

// Finish off a subscripting operation.

    uint        num_ss;
    uint        depth;
    bool        is_field;

    is_field = ( itptr->opn & OPN_FLD ) && ( itptr->value.st.tmp_id == NULL );
    if( is_field ) {
        num_ss = _DimCount( itptr->sym_ptr->fd.dim_ext->dim_flags );
    } else {
        num_ss = _DimCount( itptr->sym_ptr->ns.si.va.dim_ext->dim_flags );
    }
    depth = GetStkEntries( TY_INTEGER );
    ReqStkDepth( num_ss * depth );
    if( is_field ) {
        if( num_ss == 1 ) {
            EmitOp( FIELD_SUBSCR_1 );
        } else {
            EmitOp( FIELD_SUBSCR );
        }
        OutDataPtr( LinkReloc( &itptr->sym_ptr->fd.dim_ext->l.reloc_chain ) );
        OutInt( itptr->value.intstar4 );
    } else {
        if( num_ss == 1 ) {
            if( ChkUDef( itptr ) == FALSE ) {
                EmitOp( RT_1_SUBSCR );
            } else if( itptr->flags & SY_SUB_PARM ) {
                EmitOp( RT_P1_SUBSCR );
            } else {
                EmitOp( RT_U1_SUBSCR );
            }
        } else {
            if( ChkUDef( itptr ) == FALSE ) {
                EmitOp( RT_SUBSCRIPT );
            } else if( itptr->flags & SY_SUB_PARM ) {
                EmitOp( RT_PSUBSCRIPT );
            } else {
                EmitOp( RT_USUBSCRIPT );
            }
        }
        OpnRef( itptr );
    }
    ModStkDepth( -( num_ss * depth ) );
    if( itptr->value.st.tmp_id != NULL ) {
        // an offset has been calculated, so let's add it to the result
        EmitOp( SUBSCR_FIELD_OP );
        SymRef( itptr->value.st.tmp_id );
        // this ptr isn't used by SUBSCR_FIELD_OP... it's here for
        // undefined value reporting only
        OpnRef( itptr );
    }
    switch( itptr->typ ) {
    case TY_CHAR:
        if( itptr->value.st.tmp_id != NULL ) {
            // an offset has been calculated, use it for result
            EmitOp( FIELD_CHAR_SUBSCR );
            SymRef( itptr->value.st.tmp_id );
        } else if( !( itptr->opn & OPN_FLD ) ) {
            // We don't have to do field subscripting here since a GFieldSCB
            // will be done.
            EmitOp( RT_CHAR_SUBSCR );
            // indicate SCB is on the stack so that A(3)(2:) for A*(*)
            // can get the length when performing substring operation
        }
        SetOpn( itptr, OPN_SAFE );
        break;
    case TY_STRUCTURE:
        if( !is_field ) {
            EmitOp( STRUCT_SUBSCR );
            SymRef( itptr->sym_ptr->ns.xt.record );
            // indicate that RCB is on the stack
            SetOpn( itptr, OPN_SAFE );
        }
        break;
    }
}
