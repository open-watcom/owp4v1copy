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
* Description: implement .co concatenate control
*                        .fo format control
*                        .ju justification control
*                                             incomplete TBD
*  comments are from script-tso.txt
****************************************************************************/


#include "wgml.h"


/****************************************************************************/
/* FORMAT combines the effect of Concatenate and Justify.                   */
/*                                                                          */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커        */
/*      |       |                                                  |        */
/*      |       |    <ON|OFF                                       |        */
/*      |  .FO  |     Left|Right|Centre|                           |        */
/*      |       |     INSIDE|OUTSIDE|HALF>                         |        */
/*      |       |                                                  |        */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸        */
/*                                                                          */
/* This control word creates a break.  The "OFF" operand is equivalent to   */
/* the control words Concatenate OFF and Justify OFF.  An omitted operand   */
/* is treated as "ON",  which is equivalent to Concatenate ON and Justify   */
/* ON.  The other possible operands specify Concatenate ON and the appro-   */
/* priate mode of Justify.  See the .CO and .JU descriptions for details.   */
/****************************************************************************/

/***************************************************************************/
/* CONCATENATE  enables  or cancels  the  formation  of output  lines  by  */
/* concatenating input lines and truncating  at the nearest word boundary  */
/* to fit on the output line.                                              */
/*                                                                         */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커       */
/*      |       |                                                  |       */
/*      |  .CO  |    <ON|OFF>                                      |       */
/*      |       |                                                  |       */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸       */
/*                                                                         */
/*                                                                         */
/* ON:  Output lines will be created by adding words from the input lines  */
/*    until a word is found which will  not fit on the output line.   The  */
/*    resulting output line will therefore have as many words as possible  */
/*    without  exceeding the  line-length  restriction (or  column-length  */
/*    restriction, if in multiple-column mode).                            */
/* OFF:  Suppresses  the concatenation action,   which results in  a "one  */
/*    line in,  one  line out" effect that  may exceed the length  of the  */
/*    output line,   depending on  the lengths of  the input  lines.   If  */
/*    justification (.JU)  is  still enabled,  extra blanks  will then be  */
/*    added between words to extend the line to the right margin.          */
/*                                                                         */
/* This control word  causes a break.   An omitted operand  is treated as  */
/* "ON".   Concatenation is "ON" at the  start of SCRIPT processing,  and  */
/* can only be suppressed  by the .CO or .FO control  words with an "OFF"  */
/* operand.   Multiple  blanks that occur  together within an  input line  */
/* will  all be  retained  in the  concatenated  output  line unless  the  */
/* concatenation  process "breaks"  the  input line  at  that point  when  */
/* adding words to the output line.                                        */
/***************************************************************************/

/****************************************************************************/
/* JUSTIFY causes output lines to be padded with inter-word blanks to the   */
/* right margin.                                                            */
/*                                                                          */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커        */
/*      |       |                                                  |        */
/*      |       |    <ON|HALF|OFF|                                 |        */
/*      |  .JU  |     Left|Right|Centre|                           |        */
/*      |       |     INSIDE|OUTSIDE>                              |        */
/*      |       |                                                  |        */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸        */
/*                                                                          */
/*                                                                          */
/* OFF:  suppresses the  justification process.   If Concatenate  is OFF,   */
/*    input lines  will be  copied to  output lines  on a  "one for  one"   */
/*    basis;  if  Concatenate is  ON,  the  output line  will retain  its   */
/*    "ragged right" style from the point in the concatenation process at   */
/*    which SCRIPT decides it cannot get any more on the output line.       */
/* ON:  On output devices that support "proportional spacing", the inter-   */
/*    word blanks will be equal between all words on the output line.  On   */
/*    other devices,  the following justification algorithm is used until   */
/*    no blanks remain  to be distributed between the  words:   one extra   */
/*    blank is first placed after each period, exclamation mark, or ques-   */
/*    tion mark in the  output line;  then an extra blank  will be placed   */
/*    after any colon  or semi-colon,  and then after any  comma or right   */
/*    parenthesis;  if more blanks must still  be added they are distrib-   */
/*    uted at equidistant places between words.                             */
/* HALF:  produces an effect between  "ragged right" and "full justifica-   */
/*    tion" by only  distributing half of the blanks that  it would under   */
/*    "ON".                                                                 */
/* <Left|Right|Centre|Center>:   causes  the  output  line  to  be  left-   */
/*    justified, right-justified or centered.                               */
/* <INSIDE|OUTSIDE>:   INSIDE  causes  odd-numbered  pages  to  be  left-   */
/*    justified and  even-numbered pages to be  right-justified;  OUTSIDE   */
/*    does the opposite.                                                    */
/*                                                                          */
/* This control word causes a break.   ON  is in effect until a ".JU OFF"   */
/* or ".FO OFF" is encountered.   An omitted  operand is treated as "ON".   */
/* The  Format control  word combines  the functions  of Concatenate  and   */
/* Justify.                                                                 */
/****************************************************************************/

/***************************************************************************/
/*  process .ju setting, .co too if both set                               */
/*                                                                         */
/*  NOTE: the implementation is correct for FO; CO actually behaves a bit  */
/*        differently                                                      */
/*        this implementation does not use doc_el_group; this may need to  */
/*        be changed in the future                                         */
/***************************************************************************/

static void process_fo_ju( bool both , char *cwcurr )
{
    char        *   pa;
    char        *   p;
    int             len;

    p = scan_start;
    SkipSpaces( p );                    // next word start
    pa = p;
    SkipNonSpaces( p );                 // end of word
    len = p - pa;
    switch( len ) {
    case 0 :                            // omitted means ON
        if( both ) {
            ProcFlags.concat = true;
        }
        ProcFlags.justify = ju_on;
        scan_restart = p;
        break;
    case 2 :                            // only ON valid
        if( !strnicmp( "ON", pa, 2 ) ) {
            if( both ) {
                ProcFlags.concat = true;
                g_post_skip = 0;
            }
            ProcFlags.justify = ju_on;
            scan_restart = pa + len;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    case 3 :                            // only OFF valid
        if( !strnicmp( "OFF", pa, 3 ) ) {
            if( both ) {
                ProcFlags.concat = false;
                g_post_skip = 0;
            }
            ProcFlags.justify = ju_off;
            scan_restart = pa + len;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    case 4 :                            // Left or half valid
        if( !strnicmp( "LEFT", pa, 4 ) ) {
            if( both ) {
                ProcFlags.concat = true;
            }

            /***************************************************************/
            /*  .ju left is treated as .ju off by wgml4.0                  */
            /*  system variable &SYSJU is set to OFF                       */
            /***************************************************************/
            ProcFlags.justify = ju_off; // left is like off for wgml 4.0
            scan_restart = pa + len;
        } else {
            if( !strnicmp( "HALF", pa, 4 ) ) {
                if( both ) {
                    ProcFlags.concat = true;
                }
                ProcFlags.justify = ju_half;
                scan_restart = pa + len;
            } else {
                xx_opt_err( cwcurr, pa );
            }
        }
        break;
    case 5 :                            // only Right valid
        if( !strnicmp( "RIGHT", pa, 5 ) ) {
            if( both ) {
                ProcFlags.concat = true;
            }
            ProcFlags.justify = ju_right;
            scan_restart = pa + len;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    case 6 :                            // center or inside valid
        if( !strnicmp( "CENTER", pa, 6 ) || !strnicmp( "CENTRE", pa, 6 ) ) {
            if( both ) {
                ProcFlags.concat = true;
            }
            ProcFlags.justify = ju_centre;
            scan_restart = pa + len;
        } else {
            if( !strnicmp( "INSIDE", pa, 6 ) ) {
                if( both ) {
                    ProcFlags.concat = true;
                }
                ProcFlags.justify = ju_inside;
                scan_restart = pa + len;
            } else {
                xx_opt_err( cwcurr, pa );
            }
        }
        break;
    case 7 :                            // only outside valid
        if( !strnicmp( "OUTSIDE", pa, 7 ) ) {
            if( both ) {
                ProcFlags.concat = true;
            }
            ProcFlags.justify = ju_outside;
            scan_restart = pa + len;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    default:
        xx_opt_err( cwcurr, pa );
        break;
    }
    return;
}

/***************************************************************************/
/*  scr_co    implement .co concatenate control word                       */
/*                                                                         */
/*  NOTE: the implementation is correct for CO; FO actually behaves a bit  */
/*        differently                                                      */
/*        each text line is assumed to be in its own doc_element           */
/*        until the block is split, there is only one doc_el_group to be   */
/*        processed                                                        */           
/*        further work may be needed                                       */
/***************************************************************************/

void    scr_co( void )
{
    char                cwcurr[4 ];
    char            *   pa;
    char            *   p;
    doc_element     *   cur_el;
    doc_el_group    *   cur_group;  // current group from n_page, not cur_doc_el_group
    doc_el_group    *   last_group; // group to which doc_elements are to be added
    int                 len;

    cwcurr[0] = SCR_char;
    cwcurr[1] = 'c';
    cwcurr[2] = 'o';
    cwcurr[3] = '\0';

    p = scan_start;
    SkipSpaces( p );                    // next word start
    pa = p;
    SkipNonSpaces( p );                 // end of word
    len = p - pa;
    switch( len ) {
    case 0 :                            // omitted means ON
        ProcFlags.concat = true;
        scan_restart = pa;
        break;
    case 2 :                            // only ON valid
        if( !strnicmp( "ON", pa, 2 ) ) {

            /* must have text and it must have been started by CO OFF */

            if( (t_doc_el_group != NULL) && (t_doc_el_group->owner == gt_co) ) {
                cur_doc_el_group = t_doc_el_group;      // detach current element group
                t_doc_el_group = t_doc_el_group->next;  // processed doc_elements go to next group, if any
                cur_doc_el_group->next = NULL;

                if( ((cur_doc_el_group->depth + t_page.cur_depth) <= t_page.max_depth) ) {

                    /* the entire block will fit on the current page */

                    while( cur_doc_el_group->first != NULL ) {
                        cur_el = cur_doc_el_group->first;
                        cur_doc_el_group->first = cur_doc_el_group->first->next;
                        cur_el->next = NULL;
                        insert_col_main( cur_el );
                    }
                    add_doc_el_group_to_pool( cur_doc_el_group );
                    cur_doc_el_group = NULL;
                } else {

                    /* divide block into sub-blocks at each SK */

                    last_group = cur_doc_el_group;              // initialize last_group
                    cur_el = last_group->first->next;           // detach doc_elements after first
                    last_group->first->next = NULL;
                    last_group->last = last_group->first;
                    last_group->depth = last_group->last->blank_lines +
                                        last_group->last->subs_skip + last_group->last->depth;

                    while( cur_el != NULL ) {
                        if( cur_el->sk_val ) {                  // new sub-block
                            last_group->next = alloc_doc_el_group( gt_co );
                            last_group = last_group->next;
                            last_group->first = cur_el;
                            last_group->last = last_group->first;
                        } else {                                // add to current sub-block
                            last_group->last->next = cur_el;
                            last_group->last = last_group->last->next;
                        }
                        cur_el = cur_el->next;                  // detach from list
                        last_group->last->next = NULL;                                
                        last_group->depth += last_group->last->blank_lines +
                                        last_group->last->subs_skip + last_group->last->depth;
                    }

                    /* output sub-blocks */

                    while( (cur_doc_el_group != NULL) && (cur_doc_el_group->first != NULL) ) {

                        /****************************************************************/
                        /* The text starts at the top of the next column under these    */
                        /* conditions:                                                  */
                        /* 1. An SK with -1 or an argument greater than 0 preceded the  */
                        /*    text in the block.                                        */
                        /* 2. The block will not fit in the current column              */
                        /* 3. The block will fit in a column with no other text         */
                        /****************************************************************/

                        if( cur_doc_el_group->first->sk_val
                            && ((cur_doc_el_group->depth + t_page.cur_depth) > t_page.max_depth)
                            && (cur_doc_el_group->depth <= t_page.max_depth) ) {

                            next_column();
                        }

                        /* Now output the text */

                        while( cur_doc_el_group->first != NULL ) {
                            cur_el = cur_doc_el_group->first;
                            cur_doc_el_group->first = cur_doc_el_group->first->next;
                            cur_el->next = NULL;
                            insert_col_main( cur_el );
                        }

                        cur_group = cur_doc_el_group;
                        cur_doc_el_group = cur_doc_el_group->next;
                        cur_group->next = NULL;
                        add_doc_el_group_to_pool( cur_group );
                        cur_group = NULL;
                    }
                }
            }
            scan_restart = pa + len;
            ProcFlags.concat = true;
            ProcFlags.sk_co = false;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    case 3 :                            // only OFF valid
        if( !strnicmp( "OFF", pa, 3 ) ) {
            ProcFlags.concat = false;
            ProcFlags.sk_co = false;
            cur_doc_el_group = alloc_doc_el_group( gt_co );
            cur_doc_el_group->next = t_doc_el_group;
            t_doc_el_group = cur_doc_el_group;
            scan_restart = pa + len;
        } else {
            xx_opt_err( cwcurr, pa );
        }
        break;
    default:
        xx_opt_err( cwcurr, pa );
        break;
    }
    return;
}


/***************************************************************************/
/*  scr_fo    implement .fo format control                                 */
/***************************************************************************/

void    scr_fo( void )
{
    char            cwcurr[4];

    cwcurr[0] = SCR_char;
    cwcurr[1] = 'f';
    cwcurr[2] = 'o';
    cwcurr[3] = '\0';

    process_fo_ju( true, cwcurr );      // .ju and .co processing
}


/***************************************************************************/
/*  scr_ju    implement .ju justify control                                */
/***************************************************************************/

void    scr_ju( void )
{
    char            cwcurr[4];

    cwcurr[0] = SCR_char;
    cwcurr[1] = 'j';
    cwcurr[2] = 'u';
    cwcurr[3] = '\0';

    process_fo_ju( false, cwcurr );     // only .ju processing

}
