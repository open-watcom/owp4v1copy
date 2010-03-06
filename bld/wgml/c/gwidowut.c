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
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"


/***************************************************************************/
/*  buffer output line                                                     */
/***************************************************************************/

void    add_line_to_buf_lines( text_line * * buf_lines, text_line * a_line )
{
    text_line   *   tline;
    text_line   *   wline;

    tline = alloc_text_line();

    tline->first       = a_line->first;
    tline->last        = a_line->last;
    tline->y_address   = a_line->y_address;
    tline->line_height = a_line->line_height;

    wline = *buf_lines;
    if( wline == NULL ) {
        *buf_lines = tline;
    } else {
        while( wline->next != NULL ) {
            wline = wline->next;
        }
        wline->next = tline;
    }
}


/***************************************************************************/
/*  output buffered lines                                                  */
/*  either on current page or next  (as requested)                         */
/***************************************************************************/

void    out_buf_lines( text_line * * b_lines, bool newpage )
{
    text_line   *   tline;
    text_line   *   wline;
    int32_t         delta;

    if( *b_lines == NULL ) {
        return;
    }

    if( newpage ) {                   // not enough space on page put on next
        finish_page();
        document_new_page();
        document_top_banner();

        tline = *b_lines;
        delta = g_page_top - tline->y_address - tline->line_height;// vertical adjust value

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
        if( input_cbs->fmflags & II_research ) {
            out_msg( "\noutput buffered lines\n" );
        }
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
            g_cur_v_start = wline->y_address;
        } else {
            g_cur_v_start = wline->y_address;
        }
    }
    wline = *b_lines;
    while( wline != NULL ) {            // reuse memory
        tline = wline;
        wline = wline->next;
        add_text_chars_to_pool( tline );
        add_text_line_to_pool( tline );
    }
    *b_lines = NULL;                    // no buffered lines
}


/***************************************************************************/
/*  widow_check  returns true if widow lines are output on new page        */
/***************************************************************************/

bool    widow_check( void )
{
    bool newpage = false;

    if( buf_lines != NULL ) {           // lines buffered
        if( buf_lines_cnt <= g_cur_threshold ) {
            newpage = true;             // prevent widow, new page
        }
        out_buf_lines( &buf_lines, newpage );
        buf_lines_cnt = 0;
        ProcFlags.page_started |= newpage;  // page no longer empty
    }
    return( newpage );
}

