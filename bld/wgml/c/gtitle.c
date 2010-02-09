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
* Description:  WGML tags :TITLE processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"


/***************************************************************************/
/*  calc title position   ( vertical )                                     */
/***************************************************************************/

void    calc_title_pos( int8_t font, int8_t t_spacing, bool first )
{
    int32_t height  = wgml_fonts[font].line_height;

    if( first ) {
        if( !ProcFlags.page_started ) {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start = g_page_top
                    - conv_vert_unit( &layout_work.title.pre_top_skip, t_spacing );
            } else {
                g_cur_v_start = g_page_top
                    + conv_vert_unit( &layout_work.title.pre_top_skip, t_spacing );
            }
        } else {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start -=
                    conv_vert_unit( &layout_work.title.pre_top_skip, t_spacing );
            } else {
                g_cur_v_start +=
                    conv_vert_unit( &layout_work.title.pre_top_skip, t_spacing );
            }
        }
    } else {
        if( bin_driver->y_positive == 0 ) {
            g_cur_v_start -= conv_vert_unit( &layout_work.title.skip, t_spacing );
        } else {
            g_cur_v_start += conv_vert_unit( &layout_work.title.skip, t_spacing );
        }
    }
    if( ProcFlags.page_started ) {
        if( bin_driver->y_positive == 0 ) {
            if( g_cur_v_start - height < g_page_bottom ) {
                finish_page();
                document_new_page();
                calc_title_pos( font, t_spacing, true );// 1 recursion
            }
        } else {
            if( g_cur_v_start + height > g_page_bottom ) {
                finish_page();
                document_new_page();
                calc_title_pos( font, t_spacing, true );// 1 recursion
            }
        }
    }
    return;
}

/***************************************************************************/
/*  prepare title line                                                     */
/***************************************************************************/

static void prep_title_line( text_line * p_line, char * p )
{
    text_chars  *   curr_t;
    uint32_t        h_left;
    uint32_t        h_right;
    uint32_t        curr_x;

    h_left = g_page_left + conv_hor_unit( &layout_work.title.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.title.right_adjust );

    if( *p ) {
        curr_t = alloc_text_chars( p, strlen( p ), g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( "title", 6, g_curr_font_num );
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
    p_line->last  = curr_t;
    curr_x = h_left;
    if( layout_work.title.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            curr_x += (h_right - h_left - curr_t->width) / 2;
        }
    } else if( layout_work.title.page_position == pos_right ) {
        curr_x = h_right - curr_t->width;
    }
    curr_t->x_address = curr_x;
    ju_x_start = curr_x;

    return;
}

/***************************************************************************/
/*  :TITLE tag                                                             */
/***************************************************************************/

void    gml_title( const gmltag * entry )
{
    char        *   p;
    text_line       p_line;
    int8_t          t_spacing;
    int8_t          font_save;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    p = scan_start;
    if( *p && *p != '.' ) p++;

    while( *p == ' ' ) {                // over WS to attribute
        p++;
    }
    if( *p &&
        ! (strnicmp( "stitle ", p, 7 ) &&   // look for stitle
           strnicmp( "stitle=", p, 7 )) ) {
        char        quote;
        char    *   valstart;

        p += 6;
        while( *p == ' ' ) {
            p++;
        }
        if( *p == '=' ) {
            p++;
            while( *p == ' ' ) {
                p++;
            }
        }
        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = ' ';
        }
        valstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';
        if( !ProcFlags.title_tag_seen ) {// first stitle goes into dictionary
            add_symvar( &global_dict, "$stitle", valstart, no_subscript, 0 );
        }
        p++;
    } else {
        if( !ProcFlags.title_tag_seen ) {
            add_symvar( &global_dict, "$stitle", "", no_subscript, 0 );// set nullstring
        }
    }

    if( *p == '.' ) p++;                // over '.'
    if( !ProcFlags.title_tag_seen ) {
        if( *p ) {                      // first title goes into dictionary
            add_symvar( &global_dict, "$title", p, no_subscript, 0 );
        }
    }

    prepare_doc_sect( doc_sect_titlep );// if not already done

    p_line.first = NULL;
    p_line.next  = NULL;
    p_line.last  = NULL;

    t_spacing = layout_work.titlep.spacing;

    font_save = g_curr_font_num;
    g_curr_font_num = layout_work.title.font;

    p_line.line_height = wgml_fonts[g_curr_font_num].line_height;

    calc_title_pos( g_curr_font_num, t_spacing, !ProcFlags.title_tag_seen );
    p_line.y_address = g_cur_v_start;

    prep_title_line( &p_line, p );

    ProcFlags.page_started = true;
    process_line_full( &p_line, false );
    g_curr_font_num = font_save;

    if( p_line.first != NULL) {
        add_text_chars_to_pool( &p_line );
    }
    ProcFlags.page_started = true;

    ProcFlags.title_tag_seen = true;
    scan_start = scan_stop + 1;
}
