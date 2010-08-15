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
* Description:  WGML tags :P, :PC and :NOTE processing
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include    "wgml.h"
#include    "gvars.h"

/***************************************************************************/
/*  :P.perhaps paragraph elements                                          */
/***************************************************************************/
static  void    proc_p_pc( p_lay_tag * p_pc )
{
    char        *   p;
    int32_t         skippre;
    int32_t         skippost;

    scan_err = false;
    p = scan_start;

    ProcFlags.keep_left_margin = true;  //    special Note indent
    prepare_doc_sect( ProcFlags.doc_sect ); // if not already done

    scr_process_break();

    if( ProcFlags.test_widow ) {
        out_buf_lines( &buf_lines, false ); // lines are no widows
        buf_lines_cnt = 0;
    }

//  g_cur_left = g_page_left + g_indent;// left start    TBD
    g_cur_left += g_indent;             // left start
                                        // possibly indent first line
    g_cur_h_start = g_cur_left + conv_hor_unit( &(p_pc->line_indent) );

    g_cur_threshold = layout_work.widow.threshold; // standard threshold

    if( post_skip != NULL ) {
        skippost = conv_vert_unit( post_skip, spacing );
        post_skip = NULL;
    } else {
        skippost = 0;
    }

    if( ProcFlags.page_started ) {
        skippre = conv_vert_unit( &(p_pc->pre_skip), spacing );
        if( skippost > skippre ) {
            skippre = skippost;         // take maximum skip amount
        }
        skippost = calc_skip_value();   // pending .sk value?
        if( skippost > skippre ) {
            skippre = skippost;         // take maximum skip amount
        }
        if( bin_driver->y_positive == 0x00 ) {
            if( skippre < g_cur_v_start ) {
                g_cur_v_start -= skippre;
            } else {
                g_cur_v_start = g_page_bottom - 1;  // force new page
            }
        } else {
            g_cur_v_start += skippre;
        }
    } else {
        if( bin_driver->y_positive == 0x00 ) {
            if( skippost > 0 ) {
                g_cur_v_start -= skippost;
            }
        } else {
            if( skippost > 0 ) {
                g_cur_v_start += skippost;
            }
        }
    }

    ProcFlags.test_widow = true;        // prevent possible widows
    post_space = 0;

    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, g_curr_font_num );
    }

    post_skip = &(p_pc->post_skip);

    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*  :P.perhaps paragraph elements                                          */
/***************************************************************************/
extern  void    gml_p( const gmltag * entry )
{
    proc_p_pc( &layout_work.p );
}

/***************************************************************************/
/*  :PC.perhaps paragraph elements                                         */
/***************************************************************************/
extern  void    gml_pc( const gmltag * entry )
{
    proc_p_pc( &layout_work.pc );
}

/***************************************************************************/
/* special for :NOTE tag                                                   */
/* trailing spaces are stripped                                            */
/* influencing the left margin for the NOTE paragraph                      */
/***************************************************************************/

void    start_line_with_string( char * text, uint8_t font_num )
{
    text_chars          *   n_char;     // new text char
    size_t                  count;

    count = strlen( text );
    if( count == 0 ) {
        return;
    }
    post_space = 0;
    while( *(text + count - 1) == ' ' ) {   // strip trailing spaces
        post_space++;
        if( --count == 0 ) {
            break;
        }
    }

    test_page_full();

    n_char = alloc_text_chars( text, count, font_num );

    n_char->x_address = g_cur_h_start;
    ju_x_start = g_cur_h_start;
    input_cbs->fmflags &= ~II_sol;      // no longer start of line

    n_char->width = cop_text_width( n_char->text, n_char->count, font_num );
    /***********************************************************/
    /*  Test if word hits right margin                         */
    /***********************************************************/

    if( n_char->x_address + n_char->width > g_page_right ) {
        process_line_full( &t_line, ProcFlags.concat );
        if( !ProcFlags.page_started ) {
            document_new_page();// page full, start new one
            document_top_banner();
        }
        set_h_start();
        n_char->x_address = g_cur_h_start;
    }

    if( t_line.first == NULL ) {        // first element in output line
        calc_skip();
        test_page_full();
        if( !ProcFlags.top_ban_proc ) {
            document_new_page();
            document_top_banner();
        }
        t_line.first = n_char;
        t_line.y_address = g_cur_v_start;
        t_line.line_height = wgml_fonts[font_num].line_height;
        ju_x_start = n_char->x_address;
        ProcFlags.line_started = true;
    } else {
        t_line.last->next = n_char;
        n_char->prev = t_line.last;
    }
    t_line.last  = n_char;

    g_cur_h_start = n_char->x_address + n_char->width;
    ProcFlags.page_started = true;
    post_space = post_space * wgml_fonts[layout_work.defaults.font].spc_width;
}

/***************************************************************************/
/*  :NOTE.perhaps paragraph elements                                       */
/***************************************************************************/
extern  void    gml_note( const gmltag * entry )
{
    char        *   p;
    int32_t        skippre;
    int32_t        skippost;
    int32_t        skipsk;

    scan_err = false;
    p = scan_start;

    prepare_doc_sect( ProcFlags.doc_sect ); // if not already done

    scr_process_break();

    if( ProcFlags.test_widow ) {
        out_buf_lines( &buf_lines, false );  // lines are no widows
        buf_lines_cnt = 0;
    }

    if( post_skip != NULL ) {
        skippost = conv_vert_unit( post_skip, spacing );
    } else {
        skippost = 0;
    }
    skipsk = calc_skip_value();   // pending .sk value?
    g_curr_font_num = layout_work.note.font;
    if( ProcFlags.page_started ) {
        skippre = conv_vert_unit( &(layout_work.note.pre_skip), spacing );
        if( skippost > skippre ) {
            skippre = skippost;         // take maximum skip amount
        }
        if( skipsk > skippre ) {
            skippre = skipsk;           // take maximum skip amount
        }
        if( bin_driver->y_positive == 0x00 ) {
            if( skippre < g_cur_v_start ) {
                g_cur_v_start -= skippre;
            } else {
                g_cur_v_start = g_page_bottom - 1;  // force new page
            }
        } else {
            g_cur_v_start += skippre;
        }
    } else {
        if( bin_driver->y_positive == 0x00 ) {
            if( skippost > 0 ) {
                g_cur_v_start -= skippost;
            }
        } else {
            if( skippost > 0 ) {
                g_cur_v_start += skippost;
            }
        }
    }
    post_skip = NULL;

    ProcFlags.test_widow = true;        // prevent possible widows
    post_space = 0;

//  g_cur_left = g_page_left + conv_hor_unit( &layout_work.note.left_indent );
    g_cur_left += conv_hor_unit( &layout_work.note.left_indent );
    g_cur_h_start = g_cur_left;

    ProcFlags.keep_left_margin = true;  // keep special Note indent
    start_line_with_string( layout_work.note.string, layout_work.note.font );
    if( t_line.last != NULL ) {
        g_cur_left += t_line.last->width + post_space;
    }
    post_space = 0;
    g_cur_h_start = g_cur_left;
    ju_x_start = g_cur_h_start;

    spacing = layout_work.note.spacing;
    g_curr_font_num = layout_work.defaults.font;
    if( *p == '.' ) p++;                // over '.'
    while( *p == ' ' ) p++;             // skip initial spaces
    if( *p ) {
        process_text( p, g_curr_font_num ); // if text follows
    }

    post_skip = &(layout_work.note.post_skip);

    scan_start = scan_stop + 1;
    return;
}
