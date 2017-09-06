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
* Description:  implements control words FB and FK
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

typedef enum {
    fbk_none,   // initial value
    fbk_begin,  // FB BEGIN/FK BEGIN
    fbk_dump,   // FB DUMP/FK DUMP
    fbk_end     // FB END/FK END
} fbk_cmd;

typedef struct {
    doc_element *   text_el;    // for t_element
    text_line   *   last_line;  // for t_el_last
    text_line   *   text_line;  // for t_line
} print_vars;

static  fbk_cmd         cur_cmd;        // current command
static  group_type      sav_group_type; // save prior group type
static  int32_t         add_space;      // additional space
static  print_vars      sav_state = { NULL, NULL, NULL };   // save/reset values on entry
static  uint32_t        dump_cnt;       // number of blocks to dump

static char * get_params( const char * scw_name ) {
    char            *   p;
    char            *   pa;
    size_t              len;
    su                  fbk_su;

    dump_cnt = 0;
    add_space = 0;
    p = scan_start;

    while( *p && (*p == ' ') ) {
        p++;
    }
    pa = p;
    while( *p && (*p != ' ') ) {
        p++;
    }
    len = p - pa;

    /* Identify the first operand */

    cur_cmd = fbk_none;
    if( (len == 3) && !memicmp( pa , "end", len ) ) {
        cur_cmd = fbk_end;
    } else if( (len == 4) && !memicmp( pa , "dump", len ) ) {
        cur_cmd = fbk_dump;
    } else if( (len == 5) && !memicmp( pa , "begin", len ) ) {
        cur_cmd = fbk_begin;
    }

    if( cur_cmd == fbk_none ) {
        p = pa;                             // reset, invalid input
    } else {
        while( *p && (*p == ' ') ) {        // find next argument, if any
            p++;
        }
        if( *p ) {
            if( cur_cmd == fbk_dump ) {       // dump takes an integer >= 0
    //        dump_cnt
            } else if( cur_cmd == fbk_end ) {        // end take a vertical space unit
                pa = p;
                if( !cw_val_to_su( &p, &fbk_su ) ) {
                    add_space = conv_vert_unit( &fbk_su, spacing, g_curr_font );
                    if( add_space <= 0 ) {
//***                    xx_line_err( err_inv_box_pos, pa );
                    } else {
                        if( fbk_su.su_relative ) {
//***                        cur_col += prev_col;
                        }
                    }
                } else {
                    xx_line_err( err_spc_not_valid, pa );
                }
            } else {                                // begin does not allow another operand
                xx_val_line_warn( wng_too_many_ops, scw_name, p );
            }
        }
    }

    return( p );
}

/**************************************************************************/
/* FLOATING BLOCK allows the user to create an "in-storage" block of text */
/* that will not be printed until the user chooses to print it.           */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |       | <BEGIN|END <0|w>>                                |      */
/*      |  .FB  |                                                  |      */
/*      |       | <DUMP <n>>                                       |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* <BEGIN|END <0|w>>: When the ".FB BEGIN" control word is encountered,   */
/*    the values of all relevant print control variables are saved and    */
/*    SCRIPT prepares to accept Floating Block text. When the ".FB END"   */
/*    control word is encountered, a break is caused for text within the  */
/*    block and the former values are restored. An additional "w"         */
/*    vertical units may be used to logically alter the number of lines   */
/*    in the text block. If not specified, the default is zero. More      */
/*    than one Floating Block may be created before being output.         */
/* <DUMP <n>>: causes the first "n" Floating Blocks to be output to the   */
/*    text area. If "n" is not specified, then all existing Floating      */
/*    Blocks are output.                                                  */
/*                                                                        */
/* This control word causes a break only with the "DUMP" operand. A       */
/* first operand must be provided as none will be assumed.                */
/*                                                                        */
/* NOTES                                                                  */
/* (1) See the discussion under the .CC control word for the list of      */
/*     control words that may not be used within the BEGIN/END range of a */
/*     .FB text block.                                                    */
/* (2) The &SYSFBC System Set Symbol can be examined after the ".FB END"  */
/*     control word to determine the count of lines within all            */
/*     outstanding text blocks. The &SYSFBF System Set Symbol can be      */
/*     used to determine the count of lines within the first outstanding  */
/*     block.                                                             */
/*                                                                        */
/* NOTE: FB BEGIN does not, in fact, take <0|w>; only FB END does         */
/*                                                                        */
/**************************************************************************/

/************************************************************************/
/* Implements control word FB                                           */
/************************************************************************/

void scr_fb( void )
{
    char    *   p;

    p = get_params( "FB" );

    switch( cur_cmd) {
    case fbk_begin : 
        g_keep_nest( "FB" );                // catch nesting errors
        sav_state.text_el = t_element;
        t_element = NULL;
        sav_state.last_line = t_el_last;
        t_el_last = NULL;
        sav_state.text_line = t_line;
        t_line = NULL;
        sav_group_type = cur_group_type;
        cur_group_type = gt_fb;
        cur_doc_el_group = alloc_doc_el_group( gt_fb );
        cur_doc_el_group->next = t_doc_el_group;
        t_doc_el_group = cur_doc_el_group;
        cur_doc_el_group = NULL;
        break;
    case fbk_end :
        if( cur_group_type == gt_fb ) {
            if( t_doc_el_group != NULL) {
                scr_process_break();
                cur_group_type = sav_group_type;
                cur_doc_el_group = t_doc_el_group;
                t_doc_el_group = t_doc_el_group->next;
                cur_doc_el_group->next = NULL;
                if( block_queue == NULL ) {
                    block_queue = cur_doc_el_group;
                    block_queue_end = block_queue;
                } else {
                    block_queue_end->next = cur_doc_el_group;
                    block_queue_end = block_queue_end->next;
                }
            }
        } else {
            xx_line_err( err_no_fb_begin, p );
        }
        t_element = sav_state.text_el;
        t_el_last = sav_state.last_line;
        t_line = sav_state.text_line;
        break;
    case fbk_dump :
        g_keep_nest( "FB" );                // catch nesting errors
        break;
    case fbk_none :
        break;
    default:
        internal_err( __FILE__, __LINE__ );
        break;
    }        

    scan_restart = scan_stop + 1;
    return;
}


/**************************************************************************/
/* FLOATING KEEP enables the user to create a block of text that will     */
/* print together, either immediately or at the top of the next column    */
/* (or page, if in single-column mode).                                   */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |       | <BEGIN|END <0|w>>                                |      */
/*      |  .FK  |                                                  |      */
/*      |       | <DUMP <n>>                                       |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* <BEGIN|END <0|w>>: These operands are equivalent to those for the .CC  */
/*    control word, with the exception that if the block will not fit in  */
/*    the current column, then it is queued for output into the text area */
/*    at the top of the next column, and the remainder of the current     */
/*    column is filled with the output produced by the text and control   */
/*    words that come after the ".FK END". Text blocks created by .FK     */
/*    may therefore be "floated" to a position later than where they were */
/*    defined in the input. The "END" operand may be further qualified    */
/*    by "w" vertical space units which defaults to zero. The text block  */
/*    will be "floated" if it does not fit or if there will be less than  */
/*    "w" space units left in the current column after the text block is  */
/*    printed.                                                            */
/* DUMP <n>: forces the immediate output of all (or the first "n")        */
/*    Floating Keep blocks that have not yet been output. Each            */
/*    outstanding block of text will print at the top of a column if      */
/*    there is insufficient room left in the current column.              */
/*                                                                        */
/* This control word causes a break. An operand must be provided as none  */
/* will be assumed. See the description under .CC for the list of         */
/* control words that may not be used within the BEGIN/END range. The     */
/* &SYSFKC System Set Symbol can be examined after the ".FK END" control  */
/* word to determine the count of lines in all Floating Keep blocks       */
/* currently waiting to be output.                                        */
/**************************************************************************/

/************************************************************************/
/* Implements control word FK                                           */
/************************************************************************/

void scr_fk( void )
{
    char    *   p;

    p = get_params( "FK" );

    switch( cur_cmd) {
    case fbk_begin : 
        g_keep_nest( "FK" );                // catch nesting errors
        sav_state.text_el = t_element;
        t_element = NULL;
        sav_state.last_line = t_el_last;
        t_el_last = NULL;
        sav_state.text_line = t_line;
        t_line = NULL;
        cur_group_type = gt_fk;
        cur_doc_el_group = alloc_doc_el_group( gt_fk );
        cur_doc_el_group->next = t_doc_el_group;
        t_doc_el_group = cur_doc_el_group;
        cur_doc_el_group = NULL;
        break;
    case fbk_end :
        if( cur_group_type == gt_fk ) {
            if( t_doc_el_group != NULL) {
                scr_process_break();
                cur_group_type = sav_group_type;
                cur_doc_el_group = t_doc_el_group;
                t_doc_el_group = t_doc_el_group->next;
                cur_doc_el_group->next = NULL;
                if( keep_queue == NULL ) {
                    keep_queue = cur_doc_el_group;
                    keep_queue_end = keep_queue;
                } else {
                    keep_queue_end->next = cur_doc_el_group;
                    keep_queue_end = keep_queue_end->next;
                }
            }
        } else {
            xx_line_err( err_no_fk_begin, p );
        }
        t_element = sav_state.text_el;
        t_el_last = sav_state.last_line;
        t_line = sav_state.text_line;
        break;
    case fbk_dump :
        g_keep_nest( "FK" );                // catch nesting errors
        break;
    case fbk_none :
        break;
    default:
        internal_err( __FILE__, __LINE__ );
        break;
    }        

    scan_restart = scan_stop + 1;
    return;
}

