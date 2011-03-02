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
* Description:  WGML tags :DOCNUM processing
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"
 
/***************************************************************************/
/*  calc docnum position  ( vertical )                                     */
/***************************************************************************/
 
static  void    calc_docnum_pos( int8_t font, int8_t d_spacing )
{
 
    if( bin_driver->y_positive == 0 ) {
        g_cur_v_start -=
            conv_vert_unit( &layout_work.docnum.pre_skip, d_spacing );
    } else {
        g_cur_v_start +=
            conv_vert_unit( &layout_work.docnum.pre_skip, d_spacing );
    }
    return;
}
 
/***************************************************************************/
/*  prepare docnum line                                                    */
/***************************************************************************/
 
static void prep_docnum_line( text_line * p_line, char * p )
{
    text_chars  *   curr_t;
    uint32_t        h_left;
    uint32_t        h_right;
 
    h_left = g_page_left + conv_hor_unit( &layout_work.docnum.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.docnum.right_adjust );
 
    if( *p ) {
        curr_t = alloc_text_chars( layout_work.docnum.string, 1 + strlen( p ) +
                                   strlen( layout_work.docnum.string ),
                                   g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( layout_work.docnum.string,
                                   1 + strlen( layout_work.docnum.string ),
                                   g_curr_font_num );
    }
    if( *p  ) {
        strcat_s( curr_t->text, curr_t->length, p );
    }
    curr_t->count = strlen( curr_t->text );
    curr_t->count = len_to_trail_space( curr_t->text, curr_t->count );
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
    p_line->last = curr_t;
    p_line->line_height = wgml_fonts[g_curr_font_num].line_height;
    curr_t->x_address = h_left;
    if( layout_work.docnum.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            curr_t->x_address += (h_right - h_left - curr_t->width) / 2;
        }
    } else if( layout_work.docnum.page_position == pos_right ) {
        curr_t->x_address = h_right - curr_t->width;
    }
    ju_x_start = curr_t->x_address;
 
    return;
}
 
/***************************************************************************/
/*  :docnum.docnum   tag                                                       */
/***************************************************************************/
 
void    gml_docnum( const gmltag * entry )
{
    char        *   p;
    text_line       p_line;
    int8_t          d_spacing;
    int8_t          font_save;
    int32_t         rc;
    symsub      *   docnumval;
 
 
    if( !((ProcFlags.doc_sect == doc_sect_titlep) ||
          (ProcFlags.doc_sect_nxt == doc_sect_titlep)) ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    p = scan_start;
    if( *p && *p != '.' ) {
        out_msg( "gdocnum.c TBD\n" );
    }
    p++;                                // over . to docnum
    rc = find_symvar( &sys_dict, "$docnum", no_subscript, &docnumval );
    if( *p ) {                          // docnum specified
        strcpy_s( docnumval->value, 60, p );
    } else {
        *(docnumval->value) = 0;
    }
 
    start_doc_sect();                   // if not already done
 
    p_line.first = NULL;
    p_line.next  = NULL;
    p_line.last  = NULL;
    p_line.line_height = wgml_fonts[layout_work.docnum.font].line_height;
 
    d_spacing = layout_work.titlep.spacing;
 
    font_save = g_curr_font_num;
    g_curr_font_num = layout_work.docnum.font;
 
    calc_docnum_pos( g_curr_font_num, d_spacing );
    p_line.y_address = g_cur_v_start;
 
    prep_docnum_line( &p_line, docnumval->value );
 
    ProcFlags.page_started = true;
 
    process_line_full( &p_line, false );
 
    g_curr_font_num = font_save;
 
    if( p_line.first != NULL) {
        add_text_chars_to_pool( &p_line );
    }
    ProcFlags.page_started = true;
 
    scan_start = scan_stop + 1;
}
