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
* Description:  WGML tags :AUTHOR processing
*
****************************************************************************/
#include    "wgml.h"
//nclude    "findfile.h"
#include    "gvars.h"


/***************************************************************************/
/*  calc author position   ( vertical )                                    */
/***************************************************************************/

static  void    calc_author_pos( int8_t font, int8_t line_spc, bool first )
{

    if( !ProcFlags.page_started ) {
        g_cur_v_start = g_page_top;
    }
    if( first ) {
        if( bin_driver->y_positive == 0 ) {
            g_cur_v_start -= wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.author.pre_skip, line_spc );
        } else {
            g_cur_v_start += wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.author.pre_skip, line_spc );
        }
    } else {
        if( bin_driver->y_positive == 0 ) {
            g_cur_v_start -= wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.author.skip, line_spc );
        } else {
            g_cur_v_start += wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.author.skip, line_spc );
        }
    }
    if( bin_driver->y_positive == 0 ) {
        if( g_cur_v_start < g_page_bottom && ProcFlags.page_started ) {
            finish_page();
            document_new_page();
            calc_author_pos( font, line_spc,
                    !ProcFlags.author_tag_seen & first );   // 1 recursion
        }
    } else {
        if( g_cur_v_start > g_page_bottom && ProcFlags.page_started ) {
            finish_page();
            document_new_page();
            calc_author_pos( font, line_spc,
                    !ProcFlags.author_tag_seen & first );   // 1 recursion
        }
    }
    return;
}

/***************************************************************************/
/*  prepare author line                                                    */
/***************************************************************************/

static void prep_author_line( text_line * p_line, char * p )
{
    text_chars  *   curr_t;
    uint32_t        h_left;
    uint32_t        h_right;
    uint32_t        curr_x;

    h_left = g_page_left + conv_hor_unit( &layout_work.author.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.author.right_adjust );

    if( *p ) {
        curr_t = alloc_text_chars( p, strlen( p ), g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( "author", 7, g_curr_font_num );
    }
    intrans( curr_t->text, &curr_t->count, g_curr_font_num );
    curr_t->width = cop_text_width( curr_t->text, curr_t->count,
                                    g_curr_font_num );
    while( curr_t->width > (h_right - h_left) ) {   // too long for line
        if( curr_t->count < 2) {        // sanity check
            break;
        }
        curr_t->count -= 1;             // truncate text
        curr_t->width = cop_text_width( curr_t->text, curr_t->count,
                                        g_curr_font_num );
    }
    p_line->first = curr_t;
    curr_x = h_left;
    if( layout_work.author.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            curr_x = h_left + (h_right - h_left - curr_t->width) / 2;
        }
    } else if( layout_work.author.page_position == pos_right ) {
        curr_x = h_right - curr_t->width;
    }
    curr_t->x_address = curr_x;
    ju_x_start = curr_x;

    return;
}

/***************************************************************************/
/*  :author tag                                                             */
/***************************************************************************/

void    gml_author( const gmltag * entry )
{
    char        *   p;
    text_line       p_line;
    int8_t          spacing;
    int8_t          font_save;
    uint32_t        y_save;
    int32_t         rc;
    symsub      *   authorval;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    p = scan_start;
    if( *p && *p != '.' ) {
        out_msg( "gauthor.c TBD\n" );
    }
    p++;                                // over . to author name
    if( !ProcFlags.author_tag_seen ) {
        rc = find_symvar( &sys_dict, "$author", no_subscript, &authorval );
        if( *p ) {                      // author specified
            strcpy_s( authorval->value, 60, p );
        } else {
            *(authorval->value) = 0;
        }
    }

    prepare_doc_sect( doc_sect_titlep );// if not already done

    p_line.first = NULL;
    p_line.next  = NULL;

    spacing = layout_work.titlep.spacing;

    font_save = g_curr_font_num;
    g_curr_font_num = layout_work.author.font;

    calc_author_pos( g_curr_font_num, spacing, !ProcFlags.author_tag_seen );
    p_line.y_address = g_cur_v_start;
    p_line.line_height = wgml_fonts[g_curr_font_num].line_height;

    prep_author_line( &p_line, p );

    ProcFlags.page_started = true;
    y_save = g_cur_v_start;
    process_line_full( &p_line, false );
    g_curr_font_num = font_save;
    g_cur_v_start = y_save;             // TBD

    if( p_line.first != NULL) {
        add_text_chars_to_pool( &p_line );
        p_line.first = NULL;
    }
    ProcFlags.page_started = true;

    ProcFlags.author_tag_seen = true;
    scan_start = scan_stop + 1;
}
