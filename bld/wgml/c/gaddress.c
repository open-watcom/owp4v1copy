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
* Description:  WGML tags :ADDRESS and :eADDRESS  and :ALINE
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"

extern  void set_v_start( int8_t spacing );


static  bool            first_aline;    // special for first :ALINE
static  text_line   *   adr_lines = NULL;   // collect :ALINE


/***************************************************************************/
/*  error msgs for missing or duplicate :ADDRESS :eADDRESS tags            */
/***************************************************************************/

static void g_err_tag( char * tag )
{
    char            tagn[8];

    sprintf_s( tagn, 8, "%c%s", GML_char, tag );
    g_err( ERR_TAG_EXPECTED, tagn );
    file_mac_info();
    err_count++;
    return;
}

static void g_err_tag_prec( char * tag )
{
    char            tagn[8];

    sprintf_s( tagn, 8, "%c%s", GML_char, tag );
    g_err( ERR_TAG_PRECEDING, tagn );
    file_mac_info();
    err_count++;
    return;
}

/***************************************************************************/
/*  calc aline position   ( vertical )                                     */
/***************************************************************************/

void    calc_aline_pos( int8_t font, int8_t spacing, bool first, bool onemore )
{

    if( first ) {                       // first aline of current :ADDRESS
        if( !ProcFlags.page_started ) {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start = g_page_top
                        - conv_vert_unit( &layout_work.address.pre_skip, 0 );
            } else {
                g_cur_v_start = g_page_top
                        + conv_vert_unit( &layout_work.address.pre_skip, 0 );
            }
        } else {
            if( bin_driver->y_positive == 0 ) {
                g_cur_v_start -=
                    conv_vert_unit( &layout_work.address.pre_skip, 0 );
            } else {
                g_cur_v_start += onemore +
                    conv_vert_unit( &layout_work.address.pre_skip, 0 );
            }
        }
    } else {
        if( bin_driver->y_positive == 0 ) {
            g_cur_v_start -= onemore * g_max_line_height +
                             conv_vert_unit( &layout_work.aline.skip, 0 );
        } else {
            g_cur_v_start += onemore * g_max_line_height +
                             conv_vert_unit( &layout_work.aline.skip, 0 );
        }
    }
    return;
}

/***************************************************************************/
/*  :ADDRESS                                                               */
/***************************************************************************/

extern  void    gml_address( const gmltag * entry )
{
    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    if( ProcFlags.address_active ) {    // nested :ADDRESS tag
        g_err_tag( "eADDRESS" );
    }
    ProcFlags.address_active = true;
    first_aline = true;
    adr_lines = NULL;

/*                         TBD
    pre_top_skip = 0;
    post_top_skip = 0;
    post_skip = 0;
 */

    spacing = layout_work.titlep.spacing;
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  :eADDRESS                                                              */
/***************************************************************************/

extern  void    gml_eaddress( const gmltag * entry )
{
    text_line   *   tline;
    bool            page_needed;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    if( !ProcFlags.address_active ) {   // no preceding :ADDRESS tag
        g_err_tag_prec( "ADDRESS" );
    }
    page_needed = false;
    tline = adr_lines;

    /***********************************************************************/
    /*  check if all aline lines fit on page                               */
    /***********************************************************************/

    while( tline != NULL ) {
        if( bin_driver->y_positive == 0 ) {
            if( tline->y_address < g_page_bottom ) {
                page_needed = true;
                break;
            }
        } else {
            if( tline->y_address > g_page_bottom ) {
                page_needed = true;
                break;
            }
        }
        tline = tline->next;
    }
    if( page_needed ) {
        finish_page();
        document_new_page();

        first_aline = true;
        tline = adr_lines;
        while( tline != NULL ) {        // calc y_addr on new page
            calc_aline_pos( tline->first->font_number, spacing, first_aline,
                            page_needed );
            first_aline = false;
            tline->y_address = g_cur_v_start;
            tline = tline->next;
        }
    }

    if( GlobalFlags.lastpass ) {
        tline = adr_lines;
        while( tline != NULL ) {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( tline );
            }
            fb_output_textline( tline );
            tline = tline->next;
        }
    }
    while( adr_lines != NULL ) {
        tline = adr_lines;
        adr_lines = adr_lines->next;
        add_text_chars_to_pool( tline );
        mem_free( tline );
    }
    ProcFlags.address_active = false;
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
    uint32_t        curr_x;

    h_left = g_page_left + conv_hor_unit( &layout_work.address.left_adjust );
    h_right = g_page_right - conv_hor_unit( &layout_work.address.right_adjust );

    if( *p ) {
        curr_t = alloc_text_chars( p, strlen( p ), g_curr_font_num );
    } else {
        curr_t = alloc_text_chars( "aline", 5, g_curr_font_num );   // dummy
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
    curr_x = h_left;
    if( layout_work.address.page_position == pos_center ) {
        if( h_left + curr_t->width < h_right ) {
            curr_x = h_left + (h_right - h_left - curr_t->width) / 2;
        }
    } else if( layout_work.address.page_position == pos_right ) {
        curr_x = h_right - curr_t->width;
    }
    curr_t->x_address = curr_x;
    p_line->ju_x_start = curr_x;

    return;
}


/***************************************************************************/
/*  add address line to adresslines                                        */
/***************************************************************************/

static void add_aline( text_line * ad_line )
{
    text_line   *   p_line;

    ProcFlags.page_started = true;

    p_line = adr_lines;
    if( first_aline ) {
        p_line = ad_line;
        adr_lines = ad_line;
    } else {
        while( p_line->next != NULL ) {
            p_line = p_line->next;
        }
        p_line->next = ad_line;
    }

    set_v_start( spacing );
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
    int8_t          font;
    int8_t          spacing;
    int8_t          font_save;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    if( !ProcFlags.address_active ) {   // no preceding :ADDRESS tag
        g_err_tag_prec( "ADDRESS" );
    }
    p = scan_start;
    if( *p == '.' ) p++;                // over '.'

    ad_line = mem_alloc( sizeof( text_line ) );
    ad_line->first = NULL;
    ad_line->next  = NULL;

    if( first_aline ) {
        prepare_doc_sect( doc_sect_titlep );// if not already done
    }

    spacing = layout_work.titlep.spacing;

    font = layout_work.address.font;

    if( font >= wgml_font_cnt ) font = 0;
    font_save = g_curr_font_num;
    g_curr_font_num = font;

    calc_aline_pos( font, spacing, first_aline, false );
    ad_line->y_address = g_cur_v_start;
    ad_line->line_height = wgml_fonts[g_curr_font_num].line_height;

    prep_aline( ad_line, p );

    add_aline( ad_line );

    ProcFlags.page_started = true;
    first_aline = false;
    scan_start = scan_stop + 1;
}
