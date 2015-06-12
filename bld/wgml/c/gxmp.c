/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML tags :XMP and :eXMP and helper functions
*               depth attribute is not supported   TBD
*
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static  bool        concat_save;            // for ProcFlags.concat
static  bool        first_xline;            // special for first xmp LINE
static  font_number font_save;              // save for font
static  group_type  sav_group_type;         // save prior group type
static  ju_enum     justify_save;           // for ProcFlags.justify

/***************************************************************************/
/*      :XMP [depth='vert-space-unit'].                                    */
/*           <paragraph elements>                                          */
/*           <basic document elements>                                     */
/* This tag signals the start of an example.  Each line of source text     */
/* following the example tag is placed in the output document without      */
/* normal text processing.  Spacing between words is preserved, and the    */
/* input text is not right justified.  Input source lines which do not fit */
/* on a line in the output document are split into two lines on a          */
/* character, rather than a word basis.  An example may be used where a    */
/* basic document element is permitted to appear, except within a figure,  */
/* footnote, or example.  A corresponding :exmp tag must be specified for  */
/* each :xmp tag.  If the example does not fit on the current page or      */
/* column, it is forced to the next one.  If the current column is empty,  */
/* the example will be split into two parts.  The depth attribute accepts  */
/* vertical space units as possible values.  The amount of specified       */
/* vertical space is created in the output before any source input text is */
/* processed.  The value of the depth attribute is linked to the current   */
/* font                                                                    */
/*                                                                         */
/***************************************************************************/

void gml_xmp( const gmltag * entry )
{
    char    *   p;

    start_doc_sect();
    scr_process_break();
    scan_err = false;
    p = scan_start;
    if( *p == '.' ) {
        /* already at tag end */
    } else {
        p++;
        while( *p == ' ' ) {
            p++;
        }

        if( !strnicmp( "depth=", p, 6 ) ) {
            p += 6;
           /***************************************************************/
           /*  Although unsupported, scan depth='xxx'                     */
           /***************************************************************/
           g_warn( wng_unsupp_att, "depth" );
           wng_count++;
           file_mac_info();

           while( *p && *p != '.' ) {   // ignore all up to tag end
               p++;
           }
        }
    }
    if( cur_group_type == gt_xmp ) {        // nested :XMP tag not supported
        g_err_tag_nest( "eXMP" );
        scan_start = scan_stop + 1;
        return;
    }

    /******************************************************************/
    /*  test for XMP within  :ADDRESS, :FIG , :FN                     */
    /******************************************************************/

    if( cur_group_type == gt_address ) {
        g_err_tag_x_in_y( "XMP", "ADDRESS" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fig ) {
        g_err_tag_x_in_y( "XMP", "FIG" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fn ) {
        g_err_tag_x_in_y( "XMP", "FN" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fb ) {
        g_err_tag_x_in_y( "XMP", "FB" );
        scan_start = scan_stop + 1;
        return;
    } else if( cur_group_type == gt_fk ) {
        g_err_tag_x_in_y( "XMP", "FK" );
        scan_start = scan_stop + 1;
        return;
    }

    first_xline = true;
    font_save = g_curr_font;
    g_curr_font = layout_work.xmp.font;

    init_nest_cb();
    nest_cb->p_stack = copy_to_nest_stack();
    nest_cb->left_indent = conv_hor_unit( &layout_work.xmp.left_indent );
    nest_cb->right_indent = -1 * conv_hor_unit( &layout_work.xmp.right_indent );
    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;
    nest_cb->c_tag = t_XMP;

    g_cur_left += nest_cb->left_indent;
    g_page_right += nest_cb->right_indent;

    g_cur_h_start = g_cur_left;
    ProcFlags.keep_left_margin = true;  // keep special indent

    spacing = layout_work.xmp.spacing;

    set_skip_vars( NULL, &layout_work.xmp.pre_skip, NULL, spacing,
                       g_curr_font );

    sav_group_type = cur_group_type;
    cur_group_type = gt_xmp;
    cur_doc_el_group = alloc_doc_el_group( gt_xmp );
    cur_doc_el_group->prev = t_doc_el_group;
    t_doc_el_group = cur_doc_el_group;
    cur_doc_el_group = NULL;

    concat_save = ProcFlags.concat;
    ProcFlags.concat = false;
    justify_save = ProcFlags.justify;
    ProcFlags.justify = ju_off;         // TBD

    if( *p == '.' ) p++;                // possible tag end
    if( *p ) {
        process_text( p, g_curr_font); // if text follows
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/* Format:  :eXMP.                                                         */
/*                                                                         */
/* This tag signals the end of an example. A corresponding :xmp tag must   */
/* be previously specified for each :exmp tag.                             */
/*                                                                         */
/***************************************************************************/

void gml_exmp( const gmltag * entry )
{
    char    *   p;
    tag_cb  *   wk;

    scr_process_break();
    if( cur_group_type != gt_xmp ) {       // no preceding :XMP tag
        g_err_tag_prec( "XMP" );
        scan_start = scan_stop + 1;
        return;
    }
    g_curr_font = font_save;
    ProcFlags.concat = concat_save;
    ProcFlags.justify = justify_save;
    g_cur_left = nest_cb->lm;
    g_page_right = nest_cb->rm;

    wk = nest_cb;
    nest_cb = nest_cb->prev;
    add_tag_cb_to_pool( wk );

    /* Place the accumulated xlines on the proper page */

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
    if( *p == '.' ) p++;            // over '.'
    if( *p ) {
        gml_pc( NULL );
    }

    scan_start = scan_stop + 1;
    return;
}

