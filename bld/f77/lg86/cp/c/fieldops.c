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
* Description:  Field selection operator generation routines.
*
****************************************************************************/


#include "ftnstd.h"
#include "global.h"
#include "opn.h"
#include "fcodes.h"
#include "stmtsw.h"
#include "objutil.h"
#include "tdreloc.h"
#include "rcb.h"

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  sym_id          TmpAlloc(int);
extern  void            SymRef(sym_id);
extern  void            SetOpn(itnode *,int);
extern  void            OpnRef(itnode *);
extern  void            PushAddr(itnode *);


void    FieldOp( int typ1, int typ2, int op ) {
//=============================================

// Generate code for a field selection operator.
    byte        what2;
    itnode      *link;

    typ1 = typ1;
    typ2 = typ2;
    op = op;
    PushOpn( CITNode->link );
    if( CITNode->opn & OPN_FLD ) {
        // sub-field reference
        PushOpn( CITNode );
        EmitOp( II_ADD );
    } else {
        link = CITNode->link;
        what2 = link->opn & OPN_WHAT;
        if( what2 == OPN_ARR ) {
            // the result of the field op is an array, we have to create
            // a temporary ADV for it
            act_dim_list        *dim_ext;

            // get the struct addr
            PushAddr( CITNode );
            EmitOp( FIELD_OP_ADV );
            dim_ext = link->sym_ptr->fd.dim_ext;
            CITNode->sym_ptr = TmpAlloc( ADV_BASE_SIZE +
                        ADV_DIM_SIZE * _DimCount( dim_ext->dim_flags ) );
            OpnRef( CITNode );
            OutDataPtr( LinkReloc( &dim_ext->l.reloc_chain ) );
            SetOpn( CITNode, OPN_TMP );
        } else {
            switch( typ2 ) {
            case TY_CHAR:
                CITNode->value.st.tmp_id = TmpAlloc( sizeof( string ) );
                switch( what2 ) {
                //case OPN_ARR: handled above
                case OPN_NWL:
                case OPN_ASS:
                    // SCB already on stack -- copy it to tmp
                    EmitOp( FIELD_OP_UPDATE_SCB );
                    break;
                default:
                    // we must create an SCB
                    EmitOp( FIELD_OP_CHAR );
                    OutInt( link->size );
                    break;
                }
                SymRef( CITNode->value.st.tmp_id );
                break;
            case TY_STRUCTURE:
                CITNode->value.st.tmp_id = TmpAlloc( sizeof( rcb ) );
                switch( what2 ) {
                //case OPN_ARR: handled above
                case OPN_NWL:
                case OPN_ASS:
                    // RCB already on stack -- just update the offs/seg field
                    EmitOp( FIELD_OP_UPDATE_RCB );
                    break;
                default:
                    // we must create an RCB
                    EmitOp( FIELD_OP_STRUCT );
                    SymRef( link->sym_ptr->fd.xt.sym_record );
                    break;
                }
                SymRef( CITNode->value.st.tmp_id );
                break;
            default:
                // store the offset that we've calculated so far
                CITNode->value.st.tmp_id = TmpAlloc( sizeof( intstar4 ) );
                EmitOp( POP_INT4 );
                SymRef( CITNode->value.st.tmp_id );
                break;
            }
        }
    }
}


void    GFieldSCB( inttarg size ) {
//=================================

// Setup an SCB for a character field.

    EmitOp( FIELD_SCB );
    OutInt( size );
}
