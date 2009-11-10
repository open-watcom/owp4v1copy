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
* Description: WGML implement processing for title page output
*                                             incomplete            TBD
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*  calc title position   ( vertical )                                     */
/***************************************************************************/

extern  void    calc_title_pos( int8_t font, int8_t spacing )
{
    if( bin_driver->y_positive == 0 ) {
        g_cur_v_start = g_page_top
                        - conv_vert_unit( &layout_work.title.pre_top_skip );
    } else {
        g_cur_v_start = g_page_top
                        + conv_vert_unit( &layout_work.title.pre_top_skip );
    }
    return;
}


/***************************************************************************/
/*  prepare 1 or more text_chars with region content                       */
/***************************************************************************/

static void prep_title_line( text_line * p_line )
{
    text_chars  *   curr_t;
    symsub      *   symsubval;
    int             rc;
    uint32_t        h_left;
    uint32_t        h_right;
    uint32_t        curr_x;

    h_left = g_page_left + conv_hor_unit( &layout_work.title.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.title.right_adjust );

    rc = find_symvar( &global_dict, "$title", no_subscript, &symsubval );
    if( rc == 2 ) {
        curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                   g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( "title", 6, g_curr_font_num );
    }
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
    curr_x = 0;
    if( layout_work.title.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            h_left += (h_right - h_left - curr_t->width) / 2;
            curr_x = h_left;
        }
    } else if( layout_work.title.page_position == pos_right ) {
        h_left = h_right - curr_t->width;
        curr_x = h_left;
    }
    if( curr_x == 0 ) {
        curr_x = h_left;
    }
    curr_t->x_address = curr_x;

    return;
}

/***************************************************************************/
/*  output title page                                                      */
/*  only the first title line is output so far                        TBD  */
/***************************************************************************/

void    titlep_output( void )
{
    text_line   p_line;
    int8_t      font;
    int8_t      spacing;
    int8_t      font_save;

    p_line.first = NULL;
    p_line.next  = NULL;
    p_line.line_height = g_max_line_height;

    spacing = layout_work.titlep.spacing;

    font = layout_work.title.font;

    if( font >= wgml_font_cnt ) font = 0;
    font_save = g_curr_font_num;
    g_curr_font_num = font;

    if( ProcFlags.title_tag_seen ) {    // title was specified

        prepare_doc_sect( doc_sect_titlep );

        calc_title_pos( font, spacing );
        p_line.y_address = g_cur_v_start;

        prep_title_line( &p_line );

        ProcFlags.page_started = true;

        process_line_full( &p_line, false );

        if( p_line.first != NULL) {
            add_text_chars_to_pool( &p_line );
            p_line.first = NULL;
        }
        ProcFlags.page_started = true;
        if( GlobalFlags.lastpass ) {
            finish_page();
        }

    }
    g_curr_font_num = font_save;
}

