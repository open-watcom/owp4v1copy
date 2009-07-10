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
* Description: WGML implement prototype support for default layout
*                   incomplete to be redesigned when :LAYOUT tag is
*                              processed
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

static    uint32_t    max_char_width;
static    uint32_t    max_line_height;
static    uint32_t    net_page_height;
static    uint32_t    net_page_width;


/***************************************************************************/
/*  init_def_lay                                                           */
/***************************************************************************/

void    init_def_lay( void )
{
    int         i;

    /* These would normally be set per the :LAYOUT. Here, half-inch
     * top, bottom and right margins and a one-inch left margin are
     * applied to an 8-1/2" x 11" page.
                test with one-inch right margin

     * A4 is 210 mm x 297 mm   8.26772 x 11.69291

     */

    if( bin_driver->y_positive == 0 ) {
        g_page_top = 10 * bin_device->vertical_base_units;
        g_page_bottom = bin_device->vertical_base_units / 2;
    } else {
        g_page_top = bin_device->vertical_base_units / 2;
        g_page_bottom = 10 * bin_device->vertical_base_units;
    }

    g_page_left = bin_device->horizontal_base_units;
    g_page_right = (8 * bin_device->horizontal_base_units) - \
//                                    (bin_device->horizontal_base_units / 2);
                                     bin_device->horizontal_base_units;
    max_char_width = 0;
    max_line_height = 0;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( max_char_width < wgml_fonts[i].default_width ) \
            max_char_width = wgml_fonts[i].default_width;
        if( max_line_height < wgml_fonts[i].line_height ) \
            max_line_height = wgml_fonts[i].line_height;
    }

    net_page_height = bin_device->page_depth;
    net_page_width = bin_device->page_width;

//    set single column mode

    g_cl = g_page_right - g_page_left;  // column length
    g_ll = g_cl;                        // line length
    g_cd = 1;                           // no of columns
}


void  init_def_margins( void )
{
    char        buf[BUF_SIZE];

    g_cur_h_start = g_page_left;
    if( bin_driver->y_positive == 0x00 ) {
        g_cur_v_start = g_page_top - (15 * max_line_height);
    } else {
        g_cur_v_start = g_page_top + (1 * max_line_height);
    }


    if( GlobalFlags.firstpass && GlobalFlags.research ) {
        out_msg( "\ndev:%s page_w:%d page_d:%d hor_u:%d ver_u:%d x_s:%d y_s:%d"
                 " x_o:%d y_o:%d\n",
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
        out_msg( "\nfont:0 def_width:%d dv:%d em:%d font_h:%d font_s:%d"
                 " line_h:%d line_s:%d spc_w:%d\n\n",
                 wgml_fonts[0].default_width,
                 wgml_fonts[0].dv_base,
                 wgml_fonts[0].em_base,
                 wgml_fonts[0].font_height,
                 wgml_fonts[0].font_space,
                 wgml_fonts[0].line_height,
                 wgml_fonts[0].line_space,
                 wgml_fonts[0].spc_width
               );

        snprintf( buf, buf_size,
                  "page top:%d bottom:%d left:%d right:%d'\n",
                  g_page_top, g_page_bottom, g_page_left, g_page_right );
        out_msg( buf );
        snprintf( buf, buf_size,
           "page net heigth:%d width:%d line height:%d char width:%d'\n\n",
                  net_page_height, net_page_width, max_line_height,
                  max_char_width );
        out_msg( buf );
    }
}
