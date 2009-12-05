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
* Description:  WGML tags :DATE processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"


/***************************************************************************/
/*  calc date position   ( vertical )                                      */
/***************************************************************************/

static  void    calc_date_pos( int8_t font, int8_t spacing )
{

/***************************************************************************/
/*  pre_skip is always used, even at top of page, in contrary to the docs  */
/***************************************************************************/

    if( bin_driver->y_positive == 0 ) {
        g_cur_v_start -= conv_vert_unit( &layout_work.date.pre_skip, spacing );
    } else {
        g_cur_v_start += conv_vert_unit( &layout_work.date.pre_skip, spacing );
    }
    return;
}

/***************************************************************************/
/*  prepare date line                                                      */
/***************************************************************************/

static void prep_date_line( text_line * p_line, char * p )
{
    text_chars  *   curr_t;
    uint32_t        h_left;
    uint32_t        h_right;
    uint32_t        curr_x;
    symsub      *   subdate;
    int             rc;

    h_left = g_page_left + conv_hor_unit( &layout_work.date.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.date.right_adjust );

    if( *p ) {
        curr_t = alloc_text_chars( p, strlen( p ), g_curr_font_num );
    } else {
        rc = find_symvar( &global_dict, "date", no_subscript, &subdate );
        curr_t = alloc_text_chars( subdate->value, strlen( subdate->value ),
                                   g_curr_font_num );
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
    if( layout_work.date.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            h_left += (h_right - h_left - curr_t->width) / 2;
            curr_x = h_left;
        }
    } else if( layout_work.date.page_position == pos_right ) {
        h_left = h_right - curr_t->width;
        curr_x = h_left;
    }
    if( curr_x == 0 ) {
        curr_x = h_left;
    }
    curr_t->x_address = curr_x;
    p_line->ju_x_start = curr_x;

    return;
}

/***************************************************************************/
/*  :DATE.date   tag                                                       */
/***************************************************************************/

void    gml_date( const gmltag * entry )
{
    char        *   p;
    text_line       p_line;
    int8_t          font;
    int8_t          spacing;
    int8_t          font_save;
    uint32_t        y_save;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    p = scan_start;
    if( *p && *p != '.' ) {
        out_msg( "gdate.c TBD\n" );
    }
    p++;                                           // over . to optional date
    if( *p ) {                                              // date specified
        add_symvar( &global_dict, "date", p, no_subscript, 0 );
    }

    prepare_doc_sect( doc_sect_titlep );// if not already done

    p_line.first = NULL;
    p_line.next  = NULL;
    p_line.line_height = g_max_line_height;

    spacing = layout_work.titlep.spacing;

    font = layout_work.title.font;

    if( font >= wgml_font_cnt ) font = 0;
    font_save = g_curr_font_num;

    calc_date_pos( font, spacing );
    p_line.y_address = g_cur_v_start;

    prep_date_line( &p_line, p );

    ProcFlags.page_started = true;
    y_save = g_cur_v_start;
    process_line_full( &p_line, false );
    g_curr_font_num = font_save;
//  g_cur_v_start = y_save;             // TBD

    if( p_line.first != NULL) {
        add_text_chars_to_pool( &p_line );
        p_line.first = NULL;
    }
    ProcFlags.page_started = true;



    scan_start = scan_stop + 1;
}
