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
* Description: implement .br (break)  script control word
*                    and related routines                            TBD
*
*  comments are from script-tso.txt
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"


/**************************************************************************/
/* BREAK forces  the current partially-full output  line (if any)   to be */
/* printed without  justification (if on),  and  a new output line  to be */
/* begun.                                                                 */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |  .BR  |    <line>                                        |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* The optional  "line" operand starts one  blank after the  control word */
/* and may be text or another control word.                               */
/*                                                                        */
/* NOTES                                                                  */
/* (1) Many other control words also cause  a Break.   No Break is neces- */
/*     sary  when one  of these  is  present.   The  description of  each */
/*     control word  indicates whether  it causes  a break,   and summary */
/*     lists are contained in other components of this document.          */
/* (2) A blank or tab in column one of  an input line has the effect of a */
/*     Break immediately before the line.    See Leading Blank (.LB)  and */
/*     Leading Tab (.LT) to control this automatic Break.                 */
/* (3) If Concatenate NO is in effect, all lines appear to be followed by */
/*     a Break.                                                           */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) This is a text line                                                */
/*     .br                                                                */
/*     followed by a .BR control word.                                    */
/*     produces:                                                          */
/*     This is a text line                                                */
/*     followed by a .BR control word.                                    */
/*                                                                        */
/*     Without the Break, it would print as:                              */
/*     This is a text line followed by a .BR control word.                */
/**************************************************************************/

void    scr_br( void )
{
    char        *   p;

    p = scan_start;
    while( *p && *p != ' ' ) {          // over cw
        p++;
    }
    if( *p ) {
        p++;                            // over space
        if( *p ) {
            split_input( scan_start, p, false );// line operand
        }
    }
    scr_process_break();                // break processing

    scan_restart = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  insert incomplete line if any into t_element                           */
/*  insert t_element into t_page and reset it to NULL                      */
/***************************************************************************/

void  scr_process_break( void )
{
    text_chars  *   marker  = NULL;

    if( t_line != NULL ) {
        if( t_line->first != NULL ) {

            /* Insert a marker if CO OFF and post_space > 0 */

            if( !ProcFlags.concat && (post_space > 0) ) {
                marker = process_word( NULL, 0, g_curr_font );
                marker->type = norm;
                g_cur_h_start += post_space;
                post_space = 0;
                marker->x_address = g_cur_h_start;
                t_line->last->next = marker;
                marker->prev = t_line->last;
                if( t_line->line_height < wgml_fonts[g_curr_font].line_height ) {
                    t_line->line_height = wgml_fonts[g_curr_font].line_height;
                }
                t_line->last = marker;
                marker = NULL;
            }

#if 0
            /* First line of paragraph: minimum line height */

            if( ProcFlags.para_starting ) {
                if( t_line->line_height < wgml_fonts[g_curr_font].line_height ) {
                    t_line->line_height = wgml_fonts[g_curr_font].line_height;
                }
            }
#endif

            /* The last line is not justified, but is right-aligned or centered */

            process_line_full( t_line, ((ProcFlags.justify != ju_off) &&
                (ProcFlags.justify != ju_on) && (ProcFlags.justify != ju_half)) );
            t_line = NULL;
        }
    }
    if( t_element != NULL ) {
        insert_col_main( t_element );
        t_element = NULL;
        t_el_last = NULL;
    } else if( ProcFlags.para_starting ) {      // LP, P or PC : no text before break
        t_element = alloc_doc_el( el_text );
        if( g_line_indent > 0 ) {
            t_element->depth = wgml_fonts[g_curr_font].line_height + g_spacing;
        } else {
            t_element->depth = 0;
        }
        t_element->blank_lines = g_blank_lines;
        g_blank_lines = 0;
        t_element->subs_skip = g_subs_skip;
        t_element->top_skip = g_top_skip;
        t_element->element.text.overprint = ProcFlags.overprint;
        ProcFlags.overprint = false;
        t_element->element.text.first = alloc_text_line();
        if( g_line_indent > 0 ) {
            t_element->element.text.first->line_height =
                                                wgml_fonts[g_curr_font].line_height;
        } else {
            t_element->element.text.first->line_height = 0;
        }
        t_element->element.text.first->first = NULL;
        t_element->element.text.spacing = g_spacing;
        ProcFlags.skips_valid = false;
        insert_col_main( t_element );
        t_element = NULL;
        t_el_last = NULL;
    } else if( blank_lines > 0 ) {              // blank lines into own doc_element
        set_skip_vars( NULL, NULL, NULL, 1, g_curr_font );
        t_element = alloc_doc_el( el_text );
        t_element->depth = 0;
        t_element->blank_lines = g_blank_lines;
        g_blank_lines = 0;
        t_element->subs_skip = g_subs_skip;
        t_element->top_skip = g_top_skip;
        t_element->element.text.overprint = ProcFlags.overprint;
        ProcFlags.overprint = false;
        t_element->element.text.first = alloc_text_line();
        t_element->element.text.first->line_height = 0;
        t_element->element.text.first->first = NULL;
        t_element->element.text.spacing = g_spacing;
        ProcFlags.skips_valid = false;
        insert_col_main( t_element );
        t_element = NULL;
        t_el_last = NULL;

    } else if( g_blank_lines > 0 ) {            // blank lines at end of section?
        t_element = alloc_doc_el( el_text );
        t_element->depth = wgml_fonts[g_curr_font].line_height + g_spacing;
        t_element->blank_lines = g_blank_lines;
        g_blank_lines = 0;
        t_element->subs_skip = g_subs_skip;
        t_element->top_skip = g_top_skip;
        t_element->element.text.overprint = ProcFlags.overprint;
        ProcFlags.overprint = false;
        t_element->element.text.first = alloc_text_line();
        t_element->element.text.first->line_height =
                                        wgml_fonts[g_curr_font].line_height;
        t_element->element.text.first->first = NULL;
        t_element->element.text.spacing = g_spacing;
        ProcFlags.skips_valid = false;
        insert_col_main( t_element );
        t_element = NULL;
        t_el_last = NULL;

    }
    set_h_start();      // to stop paragraph indent from being used after a break
    ProcFlags.para_starting = false;
    c_stop = NULL;

    return;
}

