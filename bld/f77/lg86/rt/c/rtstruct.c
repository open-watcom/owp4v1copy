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
// RTSTRUCT     : Run-time structure support
//

#include "ftnstd.h"
#include "rundat.h"
#include "rtenv.h"
#include "parmtype.h"
#include "pgmacc.h"
#include "structio.h"
#include "rcb.h"

extern  const byte __FAR        SizeVars[];

#if _TARGET == _80386
#define ADDBASE1( __offs )      ((obj_ptr)(__offs) + (_base1))
#define BASE1                   , obj_ptr _base1
#define ADDBASE2( __offs )      ((obj_ptr)(__offs) + (_base2))
#define BASE2                   , obj_ptr _base2
#define SEG
#else
#define ADDBASE1( __offs )      (__offs)
#define BASE1
#define SEG                     , unsigned_16 _seg
#define ADDBASE2( __offs )      (__offs)
#define BASE2
#endif


void    StructIOInit( sio_cntrl_blk PGM *cb SEG BASE1, rcb r ) {
//==============================================================

    sdefn PGM   *sd;

    cb->origin = r.origin;
    cb->cur_sdefn = ADDBASE1( r.defn );
    sd = SDEFN_REF_PGM( _seg, ADDBASE1( r.defn ) );
    cb->cur_offs = PGM_SDEFN_REF( NEXT_SDEFN( sd, sizeof( sdefn_header ) ) );
    sd->hd.back_link = NULL;
    sd->hd.elts_left = 0;
}


int     StructIOItem( sio_cntrl_blk PGM *cb, bool do_read SEG BASE1 ) {
//=====================================================================

//  Do the work necessary to output/input one item in a structure.

    byte        info;
    sdefn PGM   *sd_offs;
    sdefn PGM   *sd;
    sdefn PGM   *nested_sd;
    uint        amt;
    uint        size;
    lg_adv PGM  *adv_ptr;
    byte PGM    *bptr;

    sd = SDEFN_REF_PGM( _seg, cb->cur_sdefn );
    sd_offs = SDEFN_REF_PGM( _seg, cb->cur_offs );
    for(;;) {
        for(;;) {
            // find next item to output
            info = sd_offs->info;
            if( info != PT_NOTYPE ) break;
            // at the end of a structure, so pop the stack and try again
            if( sd->hd.back_link == NULL ) {
                return( PT_NOTYPE );            // empty stack, done i/o
            }
            sd_offs = SDEFN_REF_PGM( _seg, sd->hd.back_offs );// finish the pop
            sd = SDEFN_REF_PGM( _seg, sd->hd.back_link );
        }
        switch( info & SDEFN_TYPE ) {
        case PT_STRUCT:
            if( info & SDEFN_UNION ) {  // really is a union
                nested_sd = SDEFN_REF_PGM( _seg, ADDBASE1( sd_offs->un.map ));
                amt = sizeof( sdefn_union );
            } else {
                nested_sd = SDEFN_REF_PGM( _seg,ADDBASE1(sd_offs->st.sd_ref));
                amt = sizeof( sdefn_struct );
            }
            break;
        case PT_CHAR:
            size = sd_offs->ch.size;
            IORslt.string.len = size;
            IORslt.string.strptr = cb->origin;
            amt = sizeof( sdefn_char );
            cb->origin += size;
            break;
        default:
            if( do_read ) {
                IORslt.pgm_ptr = cb->origin;
            } else {
                switch( info & SDEFN_TYPE ) {
                case PT_LOG_1:
                    IORslt.logstar4 = *(logstar1 PGM *)cb->origin;
                    break;
                case PT_LOG_4:
                    IORslt.logstar4 = *(logstar4 PGM *)cb->origin;
                    break;
                case PT_INT_1:
                    IORslt.intstar4 = *(intstar1 PGM *)cb->origin;
                    break;
                case PT_INT_2:
                    IORslt.intstar4 = *(intstar2 PGM *)cb->origin;
                    break;
                case PT_INT_4:
                    IORslt.intstar4 = *(intstar4 PGM *)cb->origin;
                    break;
                case PT_REAL_4:
                    IORslt.single = *(single PGM *)cb->origin;
                    break;
                case PT_REAL_8:
                    IORslt.dble = *(double PGM *)cb->origin;
                    break;
                case PT_CPLX_8:
                    IORslt.complex = *(complex PGM *)cb->origin;
                    break;
                case PT_CPLX_16:
                    IORslt.dcomplex = *(dcomplex PGM *)cb->origin;
                    break;
                }
            }
            size = SizeVars[ info & SDEFN_TYPE ];
            amt = sizeof( sdefn_simple );
            cb->origin += size;
            break;
        }
        if( info & SDEFN_ARRAY ) {
            // either we are processing an array, or we are just entering one
            bptr = (byte PGM *)NEXT_SDEFN( sd_offs, amt );
            // adv is beyond the field name
            bptr += *bptr + 1;
            adv_ptr = (lg_adv PGM *)bptr;
            if( sd->hd.elts_left == 0 ) {
                // we are just entering an array
                sd->hd.elts_left = adv_ptr->num_elts;
            }
            // decrement number of elements left
            --sd->hd.elts_left;
            if( sd->hd.elts_left == 0 ) {
                // last element in array, so advance sd_offs
                amt = ADV_BASE_SIZE;
                amt += (adv_ptr->num_ss & ADV_SUBSCRS) * ADV_DIM_SIZE;
                sd_offs = NEXT_SDEFN( (sdefn PGM *)adv_ptr, amt );
            }
        } else {
            sd_offs = NEXT_SDEFN( sd_offs, amt );
        }
        if( ( info & SDEFN_TYPE ) != PT_STRUCT ) break;
        // 'recurse' into the structure
        nested_sd->hd.back_link = PGM_SDEFN_REF( sd );
        nested_sd->hd.back_offs = PGM_SDEFN_REF( sd_offs );
        sd = nested_sd;
        sd_offs = NEXT_SDEFN( sd, sizeof( sdefn_header ) );
    }
    // return with a simple type
    cb->cur_offs = PGM_SDEFN_REF( sd_offs );
    cb->cur_sdefn = PGM_SDEFN_REF( sd );
    return( info & SDEFN_TYPE );
}


bool    StructDefnCmp( sdefn PGM *sd1 BASE1, sdefn PGM *sd2 BASE2 ) {
//===================================================================

// Compare two structure definitions to determine if they define
// 'identical' structures.

    byte        info;
    uint        amt;
    sdefn PGM   *tmp1;
    sdefn PGM   *tmp2;
    lg_adv PGM  *adv1;
    lg_adv PGM  *adv2;

    sd1 = (sdefn PGM *)ADDBASE1( sd1 );
    sd2 = (sdefn PGM *)ADDBASE2( sd2 );
    // check if same definition
    if( sd1 == sd2 ) return( TRUE );
    // compare the structure sizes
    if( sd1->hd.total_size != sd2->hd.total_size ) return( FALSE );
    sd1 = NEXT_SDEFN( sd1, sizeof( sdefn_header ) );
    sd2 = NEXT_SDEFN( sd2, sizeof( sdefn_header ) );
    // compare field by field
    for(;;) {
        info = sd1->info;
        if( info != sd2->info ) return( FALSE );        // unequal info's
        if( info == PT_NOTYPE ) break;
        switch( info & SDEFN_TYPE ) {
        case PT_CHAR:
            if( sd1->ch.size != sd2->ch.size ) return( FALSE );
            sd1 = NEXT_SDEFN( sd1, sizeof( sdefn_char ) );
            sd2 = NEXT_SDEFN( sd2, sizeof( sdefn_char ) );
            break;
        case PT_STRUCT:
            if( info & SDEFN_UNION ) {
                tmp1 = SDEFN_REF_PGM( FP_SEG(sd1), ADDBASE1( sd1->un.map ) );
                tmp2 = SDEFN_REF_PGM( FP_SEG(sd2), ADDBASE2( sd2->un.map ) );
                if( tmp1->hd.total_size != tmp2->hd.total_size ) return(FALSE);
                sd1 = NEXT_SDEFN( sd1, sizeof( sdefn_union ) );
                sd2 = NEXT_SDEFN( sd2, sizeof( sdefn_union ) );
            } else {
                tmp1 = SDEFN_REF_PGM( FP_SEG(sd1), sd1->st.sd_ref );
                tmp2 = SDEFN_REF_PGM( FP_SEG(sd2), sd2->st.sd_ref );
#if _TARGET == _80386
                if( StructDefnCmp( tmp1, _base1, tmp2, _base2 ) == FALSE )
#else
                if( StructDefnCmp( tmp1, tmp2 ) == FALSE )
#endif
                                                            {
                    return( FALSE );
                }
                sd1 = NEXT_SDEFN( sd1, sizeof( sdefn_struct ) );
                sd2 = NEXT_SDEFN( sd2, sizeof( sdefn_struct ) );
            }
            break;
        default:
            sd1 = NEXT_SDEFN( sd1, sizeof( sdefn_simple ) );
            sd2 = NEXT_SDEFN( sd2, sizeof( sdefn_simple ) );
            break;
        }
        if( info & SDEFN_ARRAY ) {
            sd1 = NEXT_SDEFN( sd1, sd1->info + 1 ); // skip field name
            sd2 = NEXT_SDEFN( sd2, sd2->info + 1 );
            adv1 = (lg_adv PGM *)sd1;
            adv2 = (lg_adv PGM *)sd2;
            if( adv1->num_elts != adv2->num_elts ) return( FALSE );
            amt = ADV_BASE_SIZE + ( adv1->num_ss & ADV_SUBSCRS )*ADV_DIM_SIZE;
            sd1 = NEXT_SDEFN( sd1, amt );
            amt = ADV_BASE_SIZE + ( adv2->num_ss & ADV_SUBSCRS )*ADV_DIM_SIZE;
            sd2 = NEXT_SDEFN( sd2, amt );
        }
    }
    return( TRUE );
}
