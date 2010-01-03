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
* Description:  WGML helper functions for widow processing
****************************************************************************/
#include    "wgml.h"
//#include    "findfile.h"
#include    "gvars.h"

extern  void    document_top_banner( void );


/***************************************************************************/
/*  buffer output line                                                     */
/***************************************************************************/

void    add_line_to_buf_lines( text_line * * b_lines, text_line * a_line )
{
    text_line   *   tline;
    text_line   *   wline;

    wline = *b_lines;
    tline = mem_alloc( sizeof( *tline ) );

    if( wline == NULL ) {
        *b_lines = tline;
         wline  = tline;
    } else {
        while( wline->next != NULL ) {
            wline = wline->next;
        }
        wline->next = tline;
    }
    tline->next        = NULL;
    tline->first       = a_line->first;
    tline->y_address   = a_line->y_address;
    tline->line_height = a_line->line_height;
}


/***************************************************************************/
/*  output buffered lines                                                  */
/*  either on current page or next                                         */
/***************************************************************************/

void    out_buf_lines( text_line * * b_lines, bool newpage )
{
    text_line   *   tline;
    text_line   *   wline;
    int32_t         delta;

    if( *b_lines == NULL ) {
        return;
    }

    if( newpage ) {                  // not enough space on page  put on next
        finish_page();
        document_new_page();
        document_top_banner();

        tline = *b_lines;
        delta = g_page_top - tline->y_address;  // vertical adjust value

        while( tline != NULL ) {        // set y_addr(s) on new page
            tline->y_address += delta;
            if( tline->next == NULL ) {
                wline = tline;          // remember last line
            }
            tline = tline->next;
        }
    }

    if( GlobalFlags.lastpass ) {        // now really output the lines
        tline = *b_lines;
        while( tline != NULL ) {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( tline );
            }
            fb_output_textline( tline );
            tline = tline->next;
        }
    }
    if( newpage ) {                     // correction for vertical position
        if( bin_driver->y_positive == 0x00 ) {
            g_cur_v_start = wline->y_address - wline->line_height;
        } else {
            g_cur_v_start = wline->y_address + wline->line_height;
        }
    }
    wline = *b_lines;
    while( wline != NULL ) {            // free / reuse memory
        tline = wline;
        wline = wline->next;
        add_text_chars_to_pool( tline );
        mem_free( tline );
    }
    *b_lines = NULL;
}

