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
* Description: WGML implement page layout setup
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*   The page geometry and margins are set up here to match the wgml 4.0   */
/*  behaviour. Some values are guesswork and some are hardcoded, if no     */
/*  formula is found for computing the value                               */
/*  The values used are from the device and the layout :page and :default  */
/*                                                                         */
/*  The system variables &SYSxxx show where the value is used              */
/*                                                                         */
/*                                      incomplete              TBD        */
/*                                                                         */
/***************************************************************************/
void    init_page_geometry( void )
{
    int         k;
#if 0                                   // activate for multi column
    uint32_t    offset;
#endif

    g_resh = bin_device->horizontal_base_units; // hor resolution  &sysresh
    g_resv = bin_device->vertical_base_units;   // vert resolution &sysresv

    g_max_char_width = 0;
    g_max_line_height = 0;

    for( k = 0; k < wgml_font_cnt; k++ ) {
        if( g_max_char_width < wgml_fonts[k].default_width ) \
            g_max_char_width = wgml_fonts[k].default_width;
        if( g_max_line_height < wgml_fonts[k].line_height ) \
            g_max_line_height = wgml_fonts[k].line_height;
    }
    if( layout_work.defaults.font >= wgml_font_cnt ) {
        layout_work.defaults.font = 0;  // make the default font num valid
    }
    g_curr_font_num = layout_work.defaults.font;


    tm = conv_vert_unit( &layout_work.page.top_margin, 1 );// top margin &systm
#if 0
    if( !strnicmp( bin_device->driver_name, "psdrv", 5 ) && tm == 0 ) {
        tm = g_resv;        // hack to get the same value as wgml 4       TBD
    }
#endif
    bm = tm;                            // bottom margin &sysbm
    hm = wgml_fonts[g_curr_font_num].line_height;   // heading margin &syshm
    fm = wgml_fonts[g_curr_font_num].line_height;   // footing margin &sysfm


    lm = conv_hor_unit( &layout_work.page.left_margin )
         - bin_device->x_offset;        // left margin &syspagelm

    rm = conv_hor_unit( &layout_work.page.right_margin )
         - bin_device->x_offset;        // right margin &syspagerm

    g_page_left = max( lm + bin_device->x_offset, bin_device->x_start );
    g_page_left_org = g_page_left;

    g_page_right = min( rm + bin_device->x_offset, bin_device->page_width );

    g_net_page_width = rm - lm;
    g_page_right_org = g_page_right;

    if( bin_driver->y_positive == 0 ) {
        g_page_depth = conv_vert_unit( &layout_work.page.depth, 1 )
                       - bin_device->y_offset;  // &syspaged
        if( tm == 0 ) {                 // without top margin 1 line down
            g_page_top = bin_device->y_start
                         - g_max_line_height;   // start of text area
        } else {
            g_page_top = min( bin_device->page_depth - tm,
                              bin_device->y_start );// start of text area
        }

        g_page_bottom = max( bin_device->y_start - g_page_depth,
                             bin_device->y_offset );// end of text area

        g_net_page_height = g_page_top - g_page_bottom;

        lcmax = 1 + (g_net_page_height + bin_device->y_offset)
                 / wgml_fonts[g_curr_font_num].line_height;   // usable no of lines

        g_ll = g_net_page_width * CPI
               / bin_device->horizontal_base_units; // &sysll

    } else {
        g_page_depth = conv_vert_unit( &layout_work.page.depth, 1 )// &syspaged
                       - bin_device->y_offset + 1;// make the wgml 4 value TBD

        g_page_top = max( tm, bin_device->y_start );

        if( bin_device->y_offset > 0) {
            g_page_bottom = min( g_page_top + g_page_depth, bin_device->y_offset );
        } else {
            g_page_bottom = g_page_top + g_page_depth;
        }

        g_net_page_height = g_page_bottom - g_page_top;
        lcmax = g_net_page_height;

        g_ll = g_net_page_width;        // line length
    }
    g_page_bottom_org = g_page_bottom; // save bottom for possible bot banner calculation
    g_page_top_org = g_page_top;// save top for possible bot banner calculation

    g_cd = layout_work.defaults.columns;// no of columns   &syscd
    g_gutter = conv_hor_unit( &layout_work.defaults.gutter );   // &sysgutter

#if 0                                   // activate for multi column
    if( g_cd > 1 ) {                    // multi column layout
        if( g_cd > 9 ) {
                                        // no more than 9 columns
            g_cd = 9;                   // this limit is found in script_tso.txt
                                        // for .cd control word
        }
        g_cl = (g_net_page_width - (g_cd - 1) * g_gutter )
                / (g_cd - 1);           // column length
        offset = g_page_left;
        for( k = 0; k < g_cd; ++k ) {
            g_offset[k] = offset;       // start of each column
            offset += g_cl + g_gutter;
        }
        for( ; k < 9; ++k ) {
            g_offset[k] = 0;            // dummy start of undefined columns
        }
    } else {
        g_cl = g_ll;
    }
#else
    g_cl = g_ll;                        // column length &syscl
                // This is what wgml 4 does, even if in multi column mode TBD
#endif

//  if( GlobalFlags.firstpass && GlobalFlags.research ) {  // show values TBD
    if( GlobalFlags.firstpass                         ) {
        out_msg( "\ntm:%d lm:%d rm:%d depth:%d\n\n", tm, lm, rm,
                 g_page_depth );

        out_msg( "dev:%s page_w:%d page_d:%d hor_u:%d ver_u:%d x_s:%d y_s:%d"
                 " x_o:%d y_o:%d\n\n",
                 bin_device->driver_name,
                 bin_device->page_width,
                 bin_device->page_depth,
                 bin_device->horizontal_base_units,
                 bin_device->vertical_base_units,
                 bin_device->x_start,
                 bin_device->y_start,
                 bin_device->x_offset,
                 bin_device->y_offset
               );
        out_msg( "default font number:%d font_count:%d\n", g_curr_font_num, wgml_font_cnt );
        for( k = 0; k < wgml_font_cnt; ++k ) {
            out_msg( "font:%d def_width:%d dv:%d em:%d font_h:%d font_s:%d"
                     " line_h:%d line_s:%d spc_w:%d\n",
                     k,
                     wgml_fonts[k].default_width,
                     wgml_fonts[k].dv_base,
                     wgml_fonts[k].em_base,
                     wgml_fonts[k].font_height,
                     wgml_fonts[k].font_space,
                     wgml_fonts[k].line_height,
                     wgml_fonts[k].line_space,
                     wgml_fonts[k].spc_width
                   );
        }

        out_msg( "\npage top:%d bottom:%d left:%d right:%d lines:%d\n",
                  g_page_top, g_page_bottom, g_page_left, g_page_right, lcmax );
        out_msg(
           "page net heigth:%d width:%d line height:%d char width:%d\n\n",
                  g_net_page_height, g_net_page_width, g_max_line_height,
                  g_max_char_width );
    }
    spacing = layout_work.defaults.spacing;
}


/***************************************************************************/
/*  Layout end processing / document start processing                      */
/*  will be called either before a non LAYOUT tag is processed, or when the*/
/*  first line without tags is found                                       */
/***************************************************************************/

void    do_layout_end_processing( void )
{

    /***********************************************************************/
    /*  init_page_geometry must be called before fb_document(), as the     */
    /*  sysvars &SYSPAGERM and &SYSPAGED are used in document :init        */
    /*  for device PS                                                      */
    /***********************************************************************/
    init_page_geometry();

    /* fb_document() needs to be done on the first pass only, but
     * also needs to be done immediately after the :ELAYOUT. tag.
     * This means that it may need to be relocated when layout
     * processing is implemented.
     */

    if( GlobalFlags.firstpass == 1) {
        out_msg( "Formatting document\n" );

        fb_document();                 // DOCUMENT :PAUSE & :INIT processing.
        ProcFlags.fb_document_done = true;
    }

    set_section_banners( doc_sect_body );   // set body default banners

}


/***************************************************************************/
/*  set page position per section                                   TBD    */
/***************************************************************************/

void    set_page_position( doc_section ds )
{
    page++;
    lc = lcmax;
    line = 0;
    g_cur_h_start = g_page_left_org + bin_device->x_offset;
    if( bin_driver->y_positive == 0x00 ) {
        g_cur_v_start = g_page_top - (1 * g_max_line_height);
    } else {
        g_cur_v_start = g_page_top + (0 * g_max_line_height);
    }
    switch( ds ) {
    case   doc_sect_none:               // treat as body
    case   doc_sect_body:
        break;
    default:
        out_msg( "set_page_position unimplemented doc_section\n" );
        break;
    }
}

