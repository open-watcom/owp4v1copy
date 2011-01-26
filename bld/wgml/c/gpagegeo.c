/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*   The page geometry and margins are set up here to match the wgml 4     */
/*  behaviour. Some values are guesswork and some are hardcoded, if no     */
/*  formula is found for computing the value                               */
/*  The values used are from the device and the layout :page and :default  */
/*                                                                         */
/*  The system variables &SYSxxx show where the value is stored in wgml4   */
/*                                                                         */
/*                                      incomplete              TBD        */
/*                                                                         */
/***************************************************************************/
void    init_page_geometry( void )
{
    int         k;
    uint32_t    page_depth_org;
    uint32_t    net_top_margin;
    uint32_t    net_y_start;
    uint32_t    top_margin;
    uint32_t    y_start_correction;
#if 0                                   // activate for multi column TBD
    uint32_t    offset;
#endif


    g_resh = bin_device->horizontal_base_units; // hor resolution  &sysresh
    g_resv = bin_device->vertical_base_units;   // vert resolution &sysresv

    spacing = layout_work.defaults.spacing;

    g_cur_threshold = layout_work.widow.threshold;

    g_max_char_width = 0;
    g_max_line_height = 0;
    for( k = 0; k < wgml_font_cnt; k++ ) {
        if( g_max_char_width < wgml_fonts[k].default_width ) \
            g_max_char_width = wgml_fonts[k].default_width;
        if( g_max_line_height < wgml_fonts[k].line_height ) \
            g_max_line_height = wgml_fonts[k].line_height;
    }
    g_curr_font_num = layout_work.defaults.font;
//  g_line_skip =  spacing * wgml_fonts[g_curr_font_num].line_height;// ??? TBD

    lm = conv_hor_unit( &layout_work.page.left_margin )
         - bin_device->x_offset;        // left margin &syspagelm

    rm = conv_hor_unit( &layout_work.page.right_margin )
         - bin_device->x_offset;        // right margin &syspagerm

    g_page_left_org = max( lm + bin_device->x_offset, bin_device->x_start );
    g_page_left = g_page_left_org;
    g_cur_left = g_page_left;           // set initial value

    g_page_right_org = min( rm + bin_device->x_offset, bin_device->page_width );
    g_page_right = g_page_right_org;

    g_net_page_width = rm - lm;
    g_ll = g_net_page_width * CPI / bin_device->horizontal_base_units; // &sysll

    top_margin = conv_vert_unit_rdd( &layout_work.page.top_margin, 1 );

    page_depth_org = conv_vert_unit_rdd( &layout_work.page.depth, 1 );
    if( bin_device->y_offset > page_depth_org ) {
        xx_err( err_page_depth_too_small ); // candidate Severe Error
        g_suicide();                        // no recovery possible
    } else {
        g_page_depth = page_depth_org - bin_device->y_offset;  // &syspaged
    }
    if( bin_device->y_offset < top_margin ) {
        net_top_margin = top_margin - bin_device->y_offset;
    } else {
        net_top_margin = 0;
    }
    if( bin_driver->y_positive == 0 ) {
        g_page_top = bin_device->y_start - net_top_margin;
        if( g_page_depth > bin_device->y_start ) {
            /* see Wiki for discussion, wgml 4.0 differs here */
            xx_err( err_page_depth_too_big );   // candidate Severe Error
            g_suicide();                        // no recovery possible
        } else {
            g_page_bottom = bin_device->y_start - g_page_depth;// end of text area
        }
        g_net_page_height = g_page_top - g_page_bottom;

        lcmax = 1 + (g_net_page_height + bin_device->y_offset)
                 / wgml_fonts[g_curr_font_num].line_height;   // usable no of lines
    } else {
        net_y_start = max( bin_device->y_start, net_top_margin );
        if( bin_device->y_start > net_top_margin ) {
            y_start_correction = min( bin_device->y_start - net_top_margin, wgml_fonts[g_curr_font_num].line_height );
        } else {
            y_start_correction = 0;
        }
        g_page_top = net_y_start - y_start_correction;
        g_page_bottom = g_page_top + g_page_depth;

        g_net_page_height = g_page_bottom - g_page_top;
        lcmax = g_net_page_height;
    }

    g_page_bottom_org = g_page_bottom;// save for possible bot banner calculation
    g_page_top_org = g_page_top;// save top for possible bot banner calculation

    g_cd = layout_work.defaults.columns;// no of columns   &syscd
    g_gutter = conv_hor_unit( &layout_work.defaults.gutter );   // &sysgutter

#if 0                                   // activate for multi column TBD
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
        out_msg( "\ntm:%d lm:%d rm:%d top margin:%d depth:%d\n\n", tm, lm, rm,
                 top_margin, g_page_depth );

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
        out_msg( "default font number:%d font_count:%d\n", g_curr_font_num,
                 wgml_font_cnt );
        for( k = 0; k < wgml_font_cnt; ++k ) {
            out_msg( "font:%d def_width:%d em:%d font_h:%d font_s:%d"
                     " line_h:%d line_s:%d spc_w:%d\n",
                     k,
                     wgml_fonts[k].default_width,
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
    g_indent = 0;
    g_indentr = 0;

    init_nest_cb();
}


/***************************************************************************/
/*  Layout end processing / document start processing                      */
/*  will be called either before the first non LAYOUT tag is processed,    */
/*  or when the first line without tags is found, i.e. produces output     */
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

        set_banners();                  // prepare banners for selection
    }
}


