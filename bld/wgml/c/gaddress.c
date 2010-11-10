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
* Description:  WGML tags :ADDRESS and :eADDRESS and :ALINE
*                    with helper functions
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static  bool            first_aline;    // special for first :ALINE
static  text_line   *   adr_lines = NULL;   // collect :ALINEs
static  int8_t          a_spacing;      // spacing between adr lines
static  int8_t          font_save;      // save for font

/***************************************************************************/
/*  calc aline position   ( vertical )                                     */
/***************************************************************************/

static void calc_aline_pos( int8_t font, int8_t line_spc, bool first )
{

    if( first ) {                       // first aline of current :ADDRESS
        if( !ProcFlags.page_started ) {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start = g_page_top - wgml_fonts[font].line_height
                        - conv_vert_unit( &layout_work.address.pre_skip, line_spc );
            } else {
                g_cur_v_start = g_page_top + wgml_fonts[font].line_height
                        + conv_vert_unit( &layout_work.address.pre_skip, line_spc );
            }
        } else {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start -= wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.address.pre_skip, line_spc );
            } else {
                g_cur_v_start += wgml_fonts[font].line_height +
                    conv_vert_unit( &layout_work.address.pre_skip, line_spc );
            }
        }
    } else {
        if( bin_driver->y_positive == 0 ) {
            g_cur_v_start -= wgml_fonts[font].line_height +
                             conv_vert_unit( &layout_work.aline.skip, line_spc );
        } else {
            g_cur_v_start += wgml_fonts[font].line_height +
                             conv_vert_unit( &layout_work.aline.skip, line_spc );
        }
    }
    return;
}

/***************************************************************************/
/*  output collected :ALINEs                                               */
/***************************************************************************/

static  void    output_addresslines( bool newpage )
{
    text_line   *   tline;

    if( adr_lines == NULL ) {
        return;                         // no stored lines
    }
    if( newpage ) {                  // not enough space on page  put on next
        finish_page();
        document_new_page();

        first_aline = true;
        tline = adr_lines;
        while( tline != NULL ) {        // recalc y_addr on new page
            calc_aline_pos( tline->first->font_number, a_spacing, first_aline );
            first_aline = false;
            tline->y_address = g_cur_v_start;
            tline = tline->next;
        }
    }
    ProcFlags.page_started = true;
    if( GlobalFlags.lastpass ) {        // now really output the lines
        tline = adr_lines;
        while( tline != NULL ) {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( tline );
            }
            fb_output_textline( tline );
            tline = tline->next;
        }
    }
    while( adr_lines != NULL ) {        // reuse memory
        tline = adr_lines;
        adr_lines = adr_lines->next;
        add_text_chars_to_pool( tline );
        add_text_line_to_pool( tline );
    }
}


/***************************************************************************/
/*  :ADDRESS                                                               */
/***************************************************************************/

extern  void    gml_address( const gmltag * entry )
{
    if( !((ProcFlags.doc_sect == doc_sect_titlep) ||
          (ProcFlags.doc_sect_nxt == doc_sect_titlep)) ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
        scan_start = scan_stop + 1;
        return;
    }
    ProcFlags.address_active = true;
    first_aline = true;
    adr_lines = NULL;
    font_save = g_curr_font_num;
    g_curr_font_num = layout_work.address.font;
    rs_loc = address_tag;

    init_nest_cb();
    nest_cb->p_stack = copy_to_nest_stack();
    nest_cb->c_tag = t_ADDRESS;

//    spacing = layout_work.titlep.spacing;
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  :eADDRESS                                                              */
/***************************************************************************/

extern  void    gml_eaddress( const gmltag * entry )
{
    tag_cb  *   wk;

    if( !ProcFlags.address_active ) {   // no preceding :ADDRESS tag
        g_err_tag_prec( "ADDRESS" );
        scan_start = scan_stop + 1;
        return;
    }

    output_addresslines( false );

    g_curr_font_num = font_save;
    ProcFlags.address_active = false;
    rs_loc = titlep_tag;
    wk = nest_cb;
    nest_cb = nest_cb->prev;
    add_tag_cb_to_pool( wk );

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  prepare address line for output                                        */
/***************************************************************************/

static void prep_aline( text_line * p_line, char * p )
{
    text_chars  *   curr_t;
    uint32_t        h_left;
    uint32_t        h_right;

    h_left = g_page_left + conv_hor_unit( &layout_work.address.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.address.right_adjust );

    if( *p ) {
        curr_t = alloc_text_chars( p, strlen( p ), g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( "aline", 5, g_curr_font_num );   // dummy
    }
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
    curr_t->x_address = h_left;
    if( layout_work.address.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            curr_t->x_address = h_left + (h_right - h_left - curr_t->width) / 2;
        }
    } else if( layout_work.address.page_position == pos_right ) {
        curr_t->x_address = h_right - curr_t->width;
    }

    return;
}


/***************************************************************************/
/*  add address line to adresslines                                        */
/***************************************************************************/

static void add_aline( text_line * ad_line )
{
    text_line   *   p_line;

    ProcFlags.page_started = true;

    if( adr_lines == NULL ) {
        adr_lines = ad_line;
        ju_x_start = ad_line->first->x_address;
    } else {
        p_line = adr_lines;
        while( p_line->next != NULL ) {
            p_line = p_line->next;
        }
        p_line->next = ad_line;
    }

    set_h_start();
    return;
}

/***************************************************************************/
/*  :ALINE tag                                                             */
/***************************************************************************/

void    gml_aline( const gmltag * entry )
{
    char        *   p;
    text_line   *   ad_line;

    if( !((ProcFlags.doc_sect == doc_sect_titlep) ||
          (ProcFlags.doc_sect_nxt == doc_sect_titlep)) ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    if( !ProcFlags.address_active ) {   // no preceding :ADDRESS tag
        g_err_tag_prec( "ADDRESS" );
    }
    p = scan_start;
    if( *p == '.' ) p++;                // over '.'

    ad_line = alloc_text_line();

    if( first_aline ) {
        start_doc_sect();               // if not already done
        a_spacing = layout_work.titlep.spacing;
        g_curr_font_num = layout_work.address.font;
    }

    calc_aline_pos( g_curr_font_num, a_spacing, first_aline );

    if( bin_driver->y_positive == 0 ) {
        if( g_cur_v_start < g_page_bottom ) {
            output_addresslines( true );
            calc_aline_pos( g_curr_font_num, a_spacing, first_aline );
        }
    } else {
        if( g_cur_v_start > g_page_bottom ) {
            output_addresslines( true );
            calc_aline_pos( g_curr_font_num, a_spacing, first_aline );
        }
    }
    ad_line->y_address = g_cur_v_start;
    ad_line->line_height = wgml_fonts[g_curr_font_num].line_height;

    prep_aline( ad_line, p );

    add_aline( ad_line );

    first_aline = false;
    scan_start = scan_stop + 1;
}

