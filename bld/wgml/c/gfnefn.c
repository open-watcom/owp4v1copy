/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  tags FN and eFN
*
* Note: FNREF is defined in gxxref.c
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

static  bool            concat_save;            // for ProcFlags.concat
static  char            id[ID_LEN];             // FIG attribute used by eFIG
static  group_type      sav_group_type;         // save prior group type
static  ju_enum         justify_save;           // for ProcFlags.justify

/***************************************************************************/
/*      :FN  [id=’id-name’].                                               */
/*           <paragraph elements>                                          */
/*           <basic document elements>                                     */
/* The footnote tag causes a note to be placed at the bottom of the page.  */
/* The footnote text is preceded by a footnote number which is generated   */
/* by the WATCOM Script/GML processor. Footnotes may be used where a basic */
/* document element is permitted to appear, with the exception of a        */
/* figure, footnote, or example. The :efn tag terminates a footnote.       */
/***************************************************************************/

void gml_fn( const gmltag * entry )
{
    bool            id_seen = false;
    char        *   p;
    ref_entry   *   cur_ref;

    start_doc_sect();
    scr_process_break();
    scan_err = false;

    if( cur_group_type == gt_fn ) {        // nested :FIG tag not supported
        g_err_tag_x_in_y( "FN", "FN" );
        scan_start = scan_stop + 1;
        return;
    }

    /******************************************************************/
    /*  test for FIG within :ADDRESS, :FN, .fb, .fk, :XMP             */
    /******************************************************************/

    if( cur_group_type == gt_address ) {
        g_err_tag_x_in_y( "FN", "ADDRESS" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fig ) {
        g_err_tag_x_in_y( "FN", "FIG" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fb ) {
        g_err_tag_x_in_y( "FN", "FB" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fk ) {
        g_err_tag_x_in_y( "FN", "FK" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_xmp ) {
        g_err_tag_x_in_y( "FN", "XMP" );
        scan_start = scan_stop + 1;
        return;
    }

    init_nest_cb();
    nest_cb->p_stack = copy_to_nest_stack();
    /// this may not be the best way to handle line_indent!!!///
    nest_cb->left_indent = conv_hor_unit( &layout_work.fn.line_indent );
    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;
    nest_cb->font = layout_work.fn.font;
    nest_cb->c_tag = t_FN;

    g_curr_font = nest_cb->font;

    g_cur_left += nest_cb->left_indent;
    g_cur_left += wgml_fonts[g_curr_font].spc_width;    // TBD, space for VLINE?
    g_page_right += nest_cb->right_indent;

    g_cur_h_start = g_cur_left;
    ProcFlags.keep_left_margin = true;  // keep special indent

    spacing = layout_work.fn.spacing;

/// pre_lines on first fn only, after that 0 -- perhaps ///

    set_skip_vars( NULL, &layout_work.fn.pre_lines, NULL, spacing, g_curr_font );

    sav_group_type = cur_group_type;
    cur_group_type = gt_fn;
    cur_doc_el_group = alloc_doc_el_group( gt_fn );
    cur_doc_el_group->prev = t_doc_el_group;
    t_doc_el_group = cur_doc_el_group;
    cur_doc_el_group = NULL;

    concat_save = ProcFlags.concat;
    ProcFlags.concat = false;
    justify_save = ProcFlags.justify;
    ProcFlags.justify = ju_off;         // TBD

    fn_count++;                         // get current FN number

    p = scan_start;
    if( *p == '.' ) p++;                // possible tag end
    for( ;; ) {
        while( *p == ' ' ) {            // over WS to attribute
            p++;
        }
        if( *p == '\0' ) {              // end of line: get new line
            if( !(input_cbs->fmflags & II_eof) ) {
                if( get_line( true ) ) {// next line for missing attribute
 
                    process_line();
                    scan_start = buff2;
                    scan_stop  = buff2 + buff2_lg;
                    if( (*scan_start == SCR_char) ||    // cw found: end-of-tag
                        (*scan_start == GML_char) ) {   // tag found: end-of-tag
                        ProcFlags.tag_end_found = true; 
                        break;          
                    } else {
                        p = scan_start; // new line is part of current tag
                        continue;
                    }
                }
            }
        }
        if( !strnicmp( "id", p, 2 ) ) {
            p += 2;
            p = get_refid_value( p, id );
            if( val_start == NULL ) {
                break;
            }
            id_seen = true;             // valid id attribute found
            if( ProcFlags.tag_end_found ) {
                break;
            }
        } else {    // no match = end-of-tag in wgml 4.0
            ProcFlags.tag_end_found = true;
            break;
        }
    }

    /* Only create the entry on the first pass */

    if( id_seen ) {
        cur_ref = find_refid( fn_ref_dict, id );
        if( pass == 1 ) {
            if( !cur_ref ) {                    // new entry
                fn_re = mem_alloc( sizeof( ref_entry ) );
                init_ref_entry( fn_re, id );
                fn_re->flags = rf_fx;                   // mark as FN
                fn_re->number = fn_count;               // add number of this FN
                add_ref_entry( &fn_ref_dict, fn_re );
            } else {
                dup_id_err( cur_ref->id, "footnote" );
            }
        } else {
            if( (page + 1) != cur_ref->pageno ) {       // page number changed
                cur_ref->pageno = page;
                init_fwd_ref( fig_fwd_refs, id );
            }
        }
    }

    if( *p ) {
        process_text( p, g_curr_font); // if text follows
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*      :eFN.                                                              */
/* This tag signals the end of a footnote. A corresponding :fn tag must be */
/* previously specified for each :efn tag.                                 */
/***************************************************************************/

void gml_efn( const gmltag * entry )
{
    char    *   p;
    tag_cb  *   wk;

    scr_process_break();
    rs_loc = 0;

    if( cur_group_type != gt_fn ) {         // no preceding :FN tag
        g_err_tag_prec( "FN" );
        scan_start = scan_stop + 1;
        return;
    }

    ProcFlags.concat = concat_save;
    ProcFlags.justify = justify_save;
    g_cur_left = nest_cb->lm;
    g_page_right = nest_cb->rm;

    wk = nest_cb;
    nest_cb = nest_cb->prev;
    add_tag_cb_to_pool( wk );

    g_curr_font = nest_cb->font;

    /* Place the accumulated lines on the proper page */

    cur_group_type = sav_group_type;
    if( t_doc_el_group != NULL) {
        cur_doc_el_group = t_doc_el_group;      // detach current element group
        t_doc_el_group = t_doc_el_group->prev;  // processed doc_elements go to next group, if any
        cur_doc_el_group->prev = NULL;

        if( cur_doc_el_group->first != NULL ) {
            cur_doc_el_group->depth += (cur_doc_el_group->first->blank_lines +
                                cur_doc_el_group->first->subs_skip);
        }

        if( (cur_doc_el_group->depth + t_page.cur_depth) > t_page.max_depth ) {

            /*  the block won't fit on this page */

            if( cur_doc_el_group->depth  <= t_page.max_depth ) {

                /*  the block will be on the next page */

                do_page_out();
                reset_t_page();
            }
        }

        while( cur_doc_el_group->first != NULL ) {
            insert_col_main( cur_doc_el_group->first );
            cur_doc_el_group->first = cur_doc_el_group->first->next;
        }

        add_doc_el_group_to_pool( cur_doc_el_group );
        cur_doc_el_group = NULL;
    }

    g_cur_h_start = g_cur_left;

    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // possible tag end
    if( *p ) {
        process_text( p, g_curr_font);  // if text follows
    }
    if( pass > 1 ) {                    // not on first pass
        fn_re = fn_re->next;            // get to next FN
    }
    scan_start = scan_stop + 1;
    return;
}

