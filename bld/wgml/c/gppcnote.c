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
#include    "wgml.h"
//#include    "findfile.h"
#include    "gvars.h"

extern void test_page_full( void );
extern void shift_spaces( void );

/***************************************************************************/
/*  :P.perhaps paragraph elements                                          */
/***************************************************************************/
static  void    proc_p_pc( p_lay_tag * p_pc )
{
    char        *   p;
    uint32_t        skippre;
    uint32_t        skippost;

    scan_err = false;
    p = scan_start;

    prepare_doc_sect( ProcFlags.doc_sect ); // if not already done

    scr_process_break();

    if( ProcFlags.test_widow ) {
        out_buf_lines( &buf_lines, false );  // lines are no widows
        buf_lines_cnt = 0;
    }

    g_cur_left = g_page_left + g_indent;// left start
                                        // possibly indent first line
    g_cur_h_start = g_cur_left + conv_hor_unit( &(p_pc->line_indent) );

    g_cur_threshold = layout_work.widow.threshold; // standard threshold

    if( ProcFlags.page_started ) {      // TBD
        skippre = conv_vert_unit( &(p_pc->pre_skip), spacing );
        if( post_skip != NULL ) {
            skippost = conv_vert_unit( post_skip, spacing );
        } else {
            skippost = 0;
        }
        if( skippost > skippre ) {
            skippre = skippost;         // take maximum skip amount
        }
        skippost = calc_skip_value();
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
            if( post_skip != NULL ) {
                g_cur_v_start -= conv_vert_unit( post_skip, spacing );
            }
        } else {
            if( post_skip != NULL ) {
                g_cur_v_start += conv_vert_unit( post_skip, spacing );
            }
        }
    }
    post_skip = NULL;

    ProcFlags.test_widow = true;        // prevent possible widows
    post_space = 0;
    post_space_save = 0;
    p_char = NULL;

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

void    start_line_with_string( char * text, uint8_t font_num )
{
    text_chars          *   n_char;     // new text char
    size_t                  count;

    count = strlen( text );
    if( count == 0 ) {
        return;
    }

    if( !ProcFlags.prep_section ) {
        prepare_doc_sect( ProcFlags.doc_sect );
    }

    test_page_full();

    n_char = alloc_text_chars( text, count, font_num );

    shift_spaces();

    n_char->x_address = g_cur_h_start;
    ju_x_start = g_cur_h_start;
    pre_space = 0;
    input_cbs->fmflags &= ~II_sol;      // no longer start of line

    n_char->width = cop_text_width( n_char->text, n_char->count, font_num );
    /***********************************************************/
    /*  Test if word hits right margin                         */
    /***********************************************************/

    if( n_char->x_address + n_char->width > g_page_right ) {
        pre_space = 0;
        process_line_full( &t_line, ProcFlags.concat );
        p_char = NULL;
        if( !ProcFlags.page_started ) {
            document_new_page();// page full, start new one
            document_top_banner();
        }
        set_h_start();
        n_char->x_address = g_cur_h_start;
    }

    if( t_line.first == NULL ) {        // first element in output line
        pre_space = 0;
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
        p_char->next = n_char;
    }
    p_char = n_char;

    g_cur_h_start = n_char->x_address + n_char->width;
    ProcFlags.page_started = true;
    post_space = 0;
    post_space_save = 0;
}

/***************************************************************************/
/*  :NOTE.perhaps paragraph elements                                       */
/***************************************************************************/
extern  void    gml_note( const gmltag * entry )
{
    char        *   p;
    uint32_t        skippre;
    uint32_t        skippost;

    scan_err = false;
    p = scan_start;

    prepare_doc_sect( ProcFlags.doc_sect ); // if not already done

    scr_process_break();

    if( ProcFlags.test_widow ) {
        out_buf_lines( &buf_lines, false );  // lines are no widows
        buf_lines_cnt = 0;
    }

    if( ProcFlags.page_started ) {
        skippre = conv_vert_unit( &(layout_work.note.pre_skip), spacing );
        if( skippre > 0 ) {
            skippre -= wgml_fonts[layout_work.note.font].line_height; // TBD
        }
        if( post_skip != NULL ) {
            skippost = conv_vert_unit( post_skip, spacing );
        } else {
            skippost = 0;
        }
        if( skippost > skippre ) {
            skippre = skippost;         // take maximum skip amount
        }

        skippost = calc_skip_value();
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
            if( post_skip != NULL ) {
                g_cur_v_start -= conv_vert_unit( post_skip, spacing );
            }
        } else {
            if( post_skip != NULL ) {
                g_cur_v_start += conv_vert_unit( post_skip, spacing );
            }
        }
    }
    post_skip = NULL;

    ProcFlags.test_widow = true;        // prevent possible widows
    post_space = 0;
    post_space_save = 0;
    pre_space = 0;
    p_char = NULL;

    g_cur_left = g_page_left + conv_hor_unit( &layout_work.note.left_indent );
    g_cur_h_start = g_cur_left;

    start_line_with_string( layout_work.note.string, layout_work.note.font );
    g_cur_left = g_page_left + p_char->width;
    if( p_char != NULL ) {
        g_cur_left += p_char->width;
    }

    g_cur_h_start = g_cur_left;
    ju_x_start = g_cur_h_start;

    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, g_curr_font_num ); // if text follows
    }

    post_skip = &(layout_work.note.post_skip);

    scan_start = scan_stop + 1;
    return;
}
