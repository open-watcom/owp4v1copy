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
// GSUBLIST     : Generate subscripting code
//

#include "ftnstd.h"
#include "global.h"
#include "fcgbls.h"
#include "fcodes.h"
#include "progsw.h"
#include "objutil.h"
#include "tdreloc.h"

extern  void            OutPtr(void *);
extern  void            EmitOp(unsigned_16);
extern  void            AddConst(itnode *);
extern  void            PushOpn(itnode *);
extern  void            DecStkDepth(int);
extern  void            PushConst(int);
extern  label_id        NextLabel(void);
extern  void            GLabel(label_id);
extern  void            GenLabRef(label_id);
extern  void            FreeLabel(label_id);


void            GWarp( sym_id sym ) {
//===================================

// Generate warp to code to fill in ADV.

    EmitOp( GO_WARP );
    GenLabRef( sym->ns.si.va.dim_ext->l.init_label );
}


warp_label              GBegSList() {
//===================================

// Generate code to start ADV initialization.

    label_id    lab;

    lab = NextLabel();
    GLabel( lab );
    return( lab );
}


void            GSLoBound( int dim_no, sym_id sym_ptr ) {
//=======================================================

// Generat  code to fill in ADV subscript element (lo bound).

    SubScrFill( S_LOW_BOUNDS, dim_no, sym_ptr );
}


void            GForceHiBound( int dim_no, sym_id sym_ptr ) {
//===========================================================

// Generate code to fill in ADV subscript element (hi bound).
// The hi bound is constant and the low bound is not.
// We have to force the filling in of the high bound so that the number of
// of elements gets computed.
//
// Scenario:   SUBROUTINE SAM( A, J )
//             DIMENSION A(J:3)
//
// GInitADV() fills in the lo bound and & of elements in the dimension at
// compile-time. The lo bound is unknown so the ADV does not contain the
// correct information. The lo bound gets filled in at run-time but
// since the hi bound is not dumped into the ADV at compile time we
// must fill it in at run-time and compute the correct number of elements
// in the dimension.

    AddConst( CITNode );
    SubScrFill( S_HIGH_BOUNDS, dim_no, sym_ptr );
}


void            GSHiBound( int dim_no, sym_id sym_ptr ) {
//=======================================================

// Generate code to fill in ADV subscript element (hi bound).

    SubScrFill( S_HIGH_BOUNDS, dim_no, sym_ptr );
}


void            GSHiBoundLo1( int dim_no, sym_id sym_ptr ) {
//==========================================================

// Generate code to fill in ADV subscript element (hi bound) with
// an assumed lo bound of 1.

    PushOpn( CITNode );         // put high bound on stack
    PushConst( 1 );
    SubScrFill( S_LOW_BOUNDS, dim_no, sym_ptr );
    SubScrFill( S_HIGH_BOUNDS, dim_no, sym_ptr );
}


void            GEndSList( sym_id sym_ptr ) {
//===========================================

// Finish off ADV initialization.

    if( sym_ptr->ns.si.va.dim_ext->dim_flags & ( DIM_PVD | DIM_ASSUMED ) ) {
        EmitOp( RT_ADV_ASSUME );
    } else if( sym_ptr->ns.si.va.dim_ext->dim_flags & DIM_VARIABLE ) {
        EmitOp( RT_ADV_VAR );
    } else {
        EmitOp( RT_ADV_CONST );
    }
    SymRef( sym_ptr );
    EmitOp( END_WARP );
}


void    DumpADV( act_dim_list *dim_ptr, uint size, bool field_ref,
                                                            bool cp_reloc ) {
//===========================================================================

// Initialize an array dope vector.

    uint                subs_no;
    uint                num_elts;
    uint                chk;
    uint                shift;
    intstar4            lo;
    intstar4            hi;
    intstar4            *bounds;

    subs_no = _DimCount( dim_ptr->dim_flags );
    if( cp_reloc ) {
        OutIIPtr();                     // offs/seg of array
    } else {
        OutRTPtr();
    }
    OutRTPtr();                         // link for chaining ADVs together
    OutInt( size );  // element size
    OutU32( dim_ptr->num_elts ); // # elements
    shift = 0;
    chk = 1;
    for(;;) {
        if( chk == 0 ) {
            shift = 0x80;
            break;
        }
        if( chk == size ) break;
        ++shift;
        chk <<= 1;
    }
                                                // number of subscripts
    OutU16( ( shift << 8 ) | subs_no | ( field_ref ? ADV_FIELD : 0 ) );
    bounds = &dim_ptr->subs_1_lo + subs_no * 2;
    for(;;) {
        --bounds;
        hi = *bounds;
        --bounds;
        lo = *bounds;
        OutU32( lo );
        num_elts = hi-lo + 1;
        OutInt( num_elts );      // # of elements in this dimension
        if( --subs_no == 0 ) break;
    }
}


void            GInitAdv( sym_id sym_ptr, bool cp_reloc ) {
//=========================================================

    uint        size;

    if( sym_ptr->ns.typ == TY_STRUCTURE ) {
        size = sym_ptr->ns.xt.record->size;
    } else {
        size = sym_ptr->ns.xt.size;
    }
    DumpADV( sym_ptr->ns.si.va.dim_ext, size, FALSE, cp_reloc );
}


void            SubScrFill( int base_fill, int dim_no, sym_id sym_ptr ) {
//=======================================================================

// Generate code to fill in ADV subscript element.

    PushOpn( CITNode );
    EmitOp( base_fill + dim_no - 1 );
    SymRef( sym_ptr );
    DecStkDepth( TY_INTEGER );
}


void            FreeWarpLabel( warp_label warp ) {
//================================================

    FreeLabel( warp );
}
