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
* Description: implement .ct (continued text) script control word
*                    no real processing           experimental       TBD
*
*  comments are from script-tso.txt
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"


/**************************************************************************/
/* CONTINUED TEXT causes the first character  of the next text input line */
/* to be placed onto the output line immediately following the last char- */
/* acter already on the output line, with no intervening blank.           */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |  .CT  |    <line>                                        |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* When concatenation is in effect,  each new input text record is joined */
/* to previous  text with an intervening  blank.   Text that occurs  in a */
/* "line" operand with  this control word will be joined  to any previous */
/* text with no intervening blank.   If  no previous text exists then the */
/* "line" operand  will be processed  as normal text.    If concatenation */
/* (.CO)  is not in effect then there  will never be any previous text to */
/* be immediately  continued.   This  can be  contrasted with  the Define */
/* Character "continue" character (.DC CONT)  that  is used to continue a */
/* text line with text that follows.                                      */
/*    This control word does not cause a break.  If it is used without an */
/* operand  line,  the  continuation  of text  is  disabled  even if  the */
/* previous text was to be continued.                                     */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) This is an example of con                                          */
/*     .ct tinued text.                                                   */
/*     produces:  This is an example of continued text.                   */
/*                                                                        */
/**************************************************************************/

void    scr_ct( void )
{
    char        *   p;

    p = scan_start;
    while( *p && *p != ' ' ) {          // over cw
        p++;
    }
    if( *p ) {
        p++;                            // over space
        if( *p ) {
            split_input( scan_start, p );   // line operand
        }
    }

    scan_restart = scan_stop + 1;
    return;
}

#if 0
/***************************************************************************/
/*  insert incomplete line if any into t_element                           */
/*  insert t_element into t_page and reset it to NULL                      */
/***************************************************************************/

void  scr_process_break( void )
{
    if( t_line != NULL ) {
        if( t_line->first != NULL ) {
            process_line_full( t_line, false );
            t_line = NULL;
            set_h_start();
        }
    }
    if( t_element != NULL ) {
        insert_col_main( t_element );
        t_element = NULL;
        t_el_last = NULL;
    } else if( ProcFlags.empty_doc_el ) {   // empty element needed?

        t_element = alloc_doc_el( el_text );
        t_element->depth = wgml_fonts[g_curr_font_num].line_height + g_spacing;
        t_element->blank_lines = g_blank_lines;
        g_blank_lines = 0;
        t_element->subs_skip = g_subs_skip;
        t_element->top_skip = g_top_skip;
        t_element->element.text.overprint = ProcFlags.overprint;
        ProcFlags.overprint = false;
        t_element->element.text.first = alloc_text_line();
        t_element->element.text.first->line_height =
                                        wgml_fonts[g_curr_font_num].line_height;
        t_element->element.text.first->first = NULL;
        t_element->element.text.spacing = g_spacing;
        insert_col_main( t_element );

        t_element = NULL;
        t_el_last = NULL;

    } else if( g_blank_lines > 0 ) {              // blank lines at end of section?
        t_element = alloc_doc_el( el_text );
        t_element->depth = wgml_fonts[g_curr_font_num].line_height + g_spacing;
        t_element->blank_lines = g_blank_lines;
        g_blank_lines = 0;
        t_element->subs_skip = g_subs_skip;
        t_element->top_skip = g_top_skip;
        t_element->element.text.overprint = ProcFlags.overprint;
        ProcFlags.overprint = false;
        t_element->element.text.first = alloc_text_line();
        t_element->element.text.first->line_height =
                                        wgml_fonts[g_curr_font_num].line_height;
        t_element->element.text.first->first = NULL;
        t_element->element.text.spacing = g_spacing;
        insert_col_main( t_element );

        t_element = NULL;
        t_el_last = NULL;

    }
    ProcFlags.empty_doc_el = false;

    return;
}
#endif

