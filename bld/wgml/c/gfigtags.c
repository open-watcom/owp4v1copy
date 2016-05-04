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
* Description:  tags FIG, eFIG, FIGCAP, FIGDESC, and FIGREF
*
* Note: FIGLIST is defined in gdocsect.c
*       FIGREF is defined in gxxref.c
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

static  bf_place        place;                  // FIG attribute used by eFIG
static  bool            concat_save;            // for ProcFlags.concat
static  bool            figcap_done;            // FIGCAP done for current FIG
static  char            id[ID_LEN];             // FIG attribute used by eFIG
static  def_frame       frame;                  // FIG attribute used by eFIG
static  group_type      sav_group_type;         // save prior group type
static  ju_enum         justify_save;           // for ProcFlags.justify
static  uint32_t        depth           = 0;    // FIG attribute used by eFIG
static  uint32_t        width           = 0;    // FIG attribute used by eFIG
static  uint32_t        figdesc_skip    = 0;    // FIGDESC pre_lines if no text


/***************************************************************************/
/*      :FIG [depth=’vert-space-unit’]                                     */
/*           [frame=box                                                    */
/*                  rule                                                   */
/*                  none                                                   */
/*                  ’character string’]                                    */
/*           [id=’id-name’]                                                */
/*           [place=top                                                    */
/*                  bottom                                                 */
/*                  inline]                                                */
/*           [width=page                                                   */
/*                  column                                                 */
/*                  ’hor-space-unit’].                                     */
/*           <paragraph elements>                                          */
/*           <basic document elements>                                     */
/* This tag signals the start of a figure. Each line of source text        */
/* following the figure tag is placed in the output document without       */
/* normal text processing. Spacing between words is preserved, and the     */
/* input text is not right justified. Input source lines which do not fit  */
/* on a line in the output document are split into two lines on a          */
/* character, rather than a word basis. A figure may be used where a basic */
/* document element is permitted, except within a figure, footnote, or     */
/* example. If the figure does not fit on the current page or column, it   */
/* is forced to the next one. If the current column is empty, the figure   */
/* will be split into two parts.                                           */
/***************************************************************************/

void gml_fig( const gmltag * entry )
{
    bool            id_seen = false;
    char        *   p;
    char        *   pa;
    ref_entry   *   cur_ref = NULL;
    su              cur_su;

    start_doc_sect();
    scr_process_break();
    scan_err = false;

    g_keep_nest( "Figure" );            // catch nesting errors

    p = scan_start;
    depth = 0;                          // default value: depth will be depth of box contents
    frame.type = layout_work.fig.default_frame.type;
    if( frame.type == char_frame ) {
        strcpy_s( frame.string, str_size, layout_work.fig.default_frame.string );
    }
    width = g_net_page_width;           // this is rm - lm, so may not be entirely correct with more than one column

    if( *p == '.' ) {
        /* already at tag end */
    } else {
        for( ;; ) {
            pa = get_att_start( p );
            p = att_start;
            if( ProcFlags.reprocess_line ) {
                break;
            }
            if( !strnicmp( "depth", p, 5 ) ) {
                p += 5;
                p = get_att_value( p );
                if( val_start == NULL ) {
                    break;
                }
                if( att_val_to_su( &cur_su, true ) ) {
                    return;
                }
                depth = conv_vert_unit( &cur_su, spacing );
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else if( !strnicmp( "frame", p, 5 ) ) {
                p += 5;
                p = get_att_value( p );
                if( val_start == NULL ) {
                    break;
                }
                if( !strnicmp( "none", val_start, 4 ) ) {
                    frame.type = none;
                } else if( !strnicmp( "box", val_start, 3 ) ) {
                    frame.type = box_frame;
                } else if( !strnicmp( "rule", val_start, 4 ) ) {
                    frame.type = rule_frame;
                } else {
                    frame.type = char_frame;
                }
                if( frame.type == char_frame ) {
                    memcpy_s( frame.string, str_size, val_start, val_len );
                    if( val_len < str_size ) {
                        frame.string[val_len] = '\0';
                    } else {
                        frame.string[str_size - 1] = '\0';
                    }
                    if( strnlen_s( frame.string, str_size ) == 0 ) {
                        frame.type = none;      // treat null string as "none"
                    }
                } else {                        // blank any existing frame.string value
                    frame.string[0] = '\0';
                }
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else if( !strnicmp( "id", p, 2 ) ) {
                p += 2;
                p = get_refid_value( p, id );
                if( val_start == NULL ) {
                    break;
                }
                id_seen = true;             // valid id attribute found
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else if( !strnicmp( "place", p, 5 ) ) {
                p += 5;
                p = get_att_value( p );
                if( val_start == NULL ) {
                    break;
                }
                if( !strnicmp( "bottom", val_start, 5 ) ) {
                    place = bottom_place;
                } else if( !strnicmp( "inline", val_start, 6 ) ) {
                    place = inline_place;
                } else if( !strnicmp( "top", val_start, 3 ) ) {
                    place = top_place;
                } else {
                    xx_line_err( err_inv_att_val, val_start );
                    scan_start = scan_stop + 1;
                    return;
                }
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else if( !strnicmp( "width", p, 5 ) ) {
                p += 5;
                p = get_att_value( p );
                if( val_start == NULL ) {
                    break;
                }
                if( !strnicmp( "page", val_start, 4 ) ) {
                    // default value is the correct value to use
                } else if( !strnicmp( "column", val_start, 6 ) ) {
                    // default value is the correct value to use
                } else {    // value actually specifies the width
                    pa = val_start;
                    if( att_val_to_su( &cur_su, true ) ) {
                        return;
                    }
                    width = conv_hor_unit( &cur_su );
                    if( width == 0 ) {
                        xx_line_err( err_inv_width_fig, pa );
                        scan_start = scan_stop + 1;
                        return;
                    }
                }
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else {    // no match = end-of-tag in wgml 4.0
                p = pa; // restore spaces before text
                break;
            }
        }
    }

// Cannot fit the figure in the adjusted left and right margins
// Cannot fit the figure with a frame in the adjusted left and right margins
                /* there should be a check somewhere for width > page width */

    init_nest_cb();
    nest_cb->p_stack = copy_to_nest_stack();
    nest_cb->left_indent = conv_hor_unit( &layout_work.fig.left_adjust );
    nest_cb->right_indent = -1 * conv_hor_unit( &layout_work.fig.right_adjust );
    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;
    nest_cb->font = layout_work.fig.font;
    nest_cb->c_tag = t_FIG;

    g_curr_font = nest_cb->font;

    g_cur_left += nest_cb->left_indent;
    g_page_right += nest_cb->right_indent;
/// space for VLINE may depend on whether there /is/ a VLINE!
    if( frame.type == box_frame ) {
        g_cur_left += wgml_fonts[g_curr_font].spc_width;    // TBD, space for VLINE?
        g_page_right += wgml_fonts[g_curr_font].spc_width;  // TBD, space for VLINE?
    }

    g_cur_h_start = g_cur_left;
    ProcFlags.keep_left_margin = true;  // keep special indent

    spacing = layout_work.fig.spacing;

    set_skip_vars( NULL, &layout_work.fig.pre_skip, &layout_work.fig.post_skip,
                                                            spacing, g_curr_font );

    nest_cb->post_skip = g_post_skip;   // shift post_skip to follow eXMP
    g_post_skip = 0;

    sav_group_type = cur_group_type;
    cur_group_type = gt_fig;
    cur_doc_el_group = alloc_doc_el_group( gt_fig );
    cur_doc_el_group->prev = t_doc_el_group;
    t_doc_el_group = cur_doc_el_group;
    cur_doc_el_group = NULL;

    concat_save = ProcFlags.concat;
    ProcFlags.concat = false;
    justify_save = ProcFlags.justify;
    ProcFlags.justify = ju_off;         // TBD

    /* Only create the entry on the first pass */

    if( pass == 1 ) {                   // add this FIG to fig_list
        fig_entry = init_ffh_entry( fig_list );
        fig_entry->flags = ffh_fig;     // mark as FIG
        if( fig_list == NULL ) {        // first entry
            fig_list = fig_entry;
        }
        if( id_seen ) {                 // add this entry to fig_ref_dict
            cur_ref = find_refid( fig_ref_dict, id );
            if( cur_ref == NULL ) {             // new entry
                cur_ref = (ref_entry *) mem_alloc( sizeof( ref_entry ) ) ;
                init_ref_entry( cur_ref, id );
                cur_ref->flags = rf_ffh;
                cur_ref->entry = fig_entry;
                add_ref_entry( &fig_ref_dict, cur_ref );
            } else {                // duplicate id
                dup_id_err( cur_ref->id, "figure" );
            }
        }
    }

    if( !ProcFlags.reprocess_line && *p ) {
        if( *p == '.' ) p++;                // possible tag end
        if( *p ) {
            process_text( p, g_curr_font); // if text follows
        }
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*      :eFIG.                                                             */
/* This tag signals the end of a figure. A corresponding :fig tag must be  */
/* previously specified for each :efig tag.                                */
/***************************************************************************/

void gml_efig( const gmltag * entry )
{
    char    *   p;
    tag_cb  *   wk;

    scr_process_break();
    rs_loc = 0;

    if( cur_group_type != gt_fig ) {       // no preceding :FIG tag
        g_err_tag_prec( "FIG" );
        scan_start = scan_stop + 1;
        return;
    }

    ProcFlags.concat = concat_save;
    ProcFlags.justify = justify_save;
    g_cur_left = nest_cb->lm;
    g_page_right = nest_cb->rm;

    g_post_skip = nest_cb->post_skip;   // shift post_skip to follow eXMP
    g_post_skip += figdesc_skip;        // FIGDESC pre_lines if no FIGDESC text
    figdesc_skip = 0;                   // cleanup for next FIG
    ProcFlags.skips_valid = false;      // activate post_skip for next element

    wk = nest_cb;
    nest_cb = nest_cb->prev;
    add_tag_cb_to_pool( wk );

    g_curr_font = nest_cb->font;

    if( (strlen( id ) > 0) && !figcap_done ) {  // FIG id requires FIGCAP
        xx_err( err_fig_id_cap );
    }

    /* Place the accumulated lines on the proper page */

    cur_group_type = sav_group_type;
    if( t_doc_el_group != NULL) {
        cur_doc_el_group = t_doc_el_group;      // detach current element group
        t_doc_el_group = t_doc_el_group->prev;  // processed doc_elements go to next group, if any
        cur_doc_el_group->prev = NULL;

        if( (cur_doc_el_group->depth + t_page.cur_depth) > t_page.max_depth ) {

            /* the block won't fit on this page */

            if( cur_doc_el_group->depth <= t_page.max_depth ) {

                /* the block will be on the next page */

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

    if( pass > 1 ) {                                // not on pass 1
        if( (page + 1) != fig_entry->pageno ) {        // page number changed
            fig_entry->pageno = page + 1;
            fig_fwd_refs = init_fwd_ref( fig_fwd_refs, id );
        }
    }

    g_cur_h_start = g_cur_left;

    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // possible tag end
    if( *p ) {
        process_text( p, g_curr_font);  // if text follows
    }
    if( pass > 1 ) {                    // not on first pass
        fig_entry = fig_entry->next;    // get to next FIG
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*      :FIGCAP.<text line>                                                */
/* The figure caption tag is used within a figure to specify the caption   */
/* for the figure. The figure caption tag must be specified if the figure  */
/* has an identifier name associated with it. Layout defined text followed */
/* by the figure number and a delimiter is inserted before the caption     */
/* text (the default text and delimiter is "Figure" and a period). The     */
/* figure caption follows the main text of the figure.                     */
/***************************************************************************/

void gml_figcap( const gmltag * entry )
{
    char            buffer[11];
    char        *   prefix;
    char        *   p;
    size_t          count;
    size_t          current;

    start_doc_sect();
    scr_process_break();
    rs_loc = figcap_tag;

    scan_err = false;
    p = scan_start;

    g_curr_font = layout_work.figcap.string_font;
    set_skip_vars( &layout_work.figcap.pre_lines, NULL, NULL, spacing, g_curr_font );

    input_cbs->fmflags &= ~II_eol;      // prefix is never EOL
    if( pass == 1 ) {                   // only on the first pass

        /* Only FIGs with captions are numbered */

        fig_count++;
        fig_entry->number = fig_count;

        /* Now build, save, and output the prefix */

        count = strlen(&layout_work.figcap.string);
        ultoa( fig_entry->number, &buffer, 10 );
        count += strlen(&buffer);
        count ++;                       // for the delimiter character
        prefix = (char *) mem_alloc( count + 1);
        strcpy_s( prefix, count, &layout_work.figcap.string);
        current = strlen( prefix );
        strcat_s( &prefix[current], count - current, &buffer);
        current = strlen( prefix );
        prefix[current] = layout_work.figcap.delim;
        prefix[current + 1] = '\0';
        fig_entry->prefix = prefix;
        process_text( prefix, g_curr_font );
        prefix = NULL;
    } else {                            // use existing prefix
        process_text( fig_entry->prefix, g_curr_font );
    }

    /* Output the caption text, if any */

    g_cur_left += (t_line->last->width + wgml_fonts[g_curr_font].spc_width );
    g_cur_h_start = g_cur_left;
    g_curr_font = layout_work.figcap.font;
    if( *p == '.' ) p++;                    // possible tag end
    if( *p == ' ' ) p++;                    // skip preceding spaces
    if( *p ) {
        ProcFlags.concat = true;            // even if was false on entry
        ProcFlags.stop_xspc = true;         // suppress 2nd space after stops
        post_space = 0;                     // g_curr_left should be enough
        input_cbs->fmflags &= ~II_sol;      // prefix was SOL, so this is not
        if( pass == 1 ) {                   // only on first pass
            current = strlen( p );
            fig_entry->text = (char *) mem_alloc( current + 1);
            strcpy_s(fig_entry->text, current + 1, p );
        }
        process_text( p, g_curr_font );     // if text follows
    }
    fig_entry->flags |= ffh_figcap;         // mark as FIGCAP present, with or without text
    figcap_done = true;

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*      :FIGDESC.<paragraph elements>                                      */
/*               <basic document elements>                                 */
/* This tag signals the start of the description for a figure. The tag is  */
/* placed after the optional :figcap tag within a figure. The GML          */
/* processor automatically adds a colon(:) following the caption when a    */
/* figure description is present.                                          */
/***************************************************************************/

void gml_figdesc( const gmltag * entry )
{
    char    *   p;

    start_doc_sect();
    rs_loc = 0;

    scan_err = false;
    p = scan_start;

    if( figcap_done ) {                         // FIGCAP was present
        ProcFlags.ct = true;                    // emulate CT
        input_cbs->fmflags &= ~II_eol;          // ":" is never EOL
        process_text( ":", g_curr_font);        // uses FIGCAP font
        g_curr_font = layout_work.figdesc.font; // change to FIGDESC font
    } else {                                    // FIGCAP not present
        scr_process_break();                    
        g_curr_font = layout_work.figdesc.font;
        set_skip_vars( &layout_work.figdesc.pre_lines, NULL, NULL, spacing, g_curr_font );
    }

    nest_cb->font = g_curr_font;        // support font changes inside description

    if( *p == '.' ) p++;                // possible tag end
    if( *p ) {
        ProcFlags.concat = true;        // even if was false on entry
        process_text( p, g_curr_font);  // if text follows
    } else {
        if( !figcap_done ) {            // if no FIGCAP was present
            figdesc_skip = g_subs_skip; // and no text
        }
    }
    scan_start = scan_stop + 1;
    return;
}

