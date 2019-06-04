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


#include "wgml.h"


typedef enum {
    fbk_none,   // initial value
    fbk_begin,  // FB BEGIN/FK BEGIN
    fbk_dump,   // FB DUMP/FK DUMP
    fbk_end     // FB END/FK END
} fbk_cmd;

/**************************************************************************/
/* In addition to items shown, the following are not saved/restored by    */
/* wgml 4.0 but it might make sense to do so: concatenation state,        */
/* justification state, and the current IN indents, both left and right   */
/**************************************************************************/

typedef struct {
    doc_element *   text_el;    // for t_element
    text_line   *   last_line;  // for t_el_last
    text_line   *   text_line;  // for t_line
    uint32_t        cur_width;  // for t_page.cur_width
    uint32_t        subs_skip;  // for g_subs_skip
} print_vars;

static  fbk_cmd         cur_cmd;        // current command
static  group_type      sav_group_type; // save prior group type
static  print_vars      sav_state;      // save/reset values on entry
static  uint32_t        sav_post_space; // save/restore post_space for line after FB/FK END

/**************************************************************************/
/* save the "printing variables", as the TSO calls them                   */
/**************************************************************************/

static void save_state( void )
{
    sav_state.text_el = t_element;
    t_element = NULL;
    sav_state.last_line = t_el_last;
    t_el_last = NULL;
    sav_state.text_line = t_line;
    t_line = NULL;
    sav_state.cur_width = t_page.cur_width;
    t_page.cur_width = t_page.cur_left;
    sav_state.subs_skip = g_subs_skip;
    g_subs_skip = 0;

    return;
}

/**************************************************************************/
/* restore the "printing variables", as the TSO calls them                */
/**************************************************************************/

static void restore_state( void )
{
    t_element = sav_state.text_el;
    t_el_last = sav_state.last_line;
    t_line = sav_state.text_line;
    t_page.cur_width = sav_state.cur_width;
    g_subs_skip = sav_state.subs_skip;

    return;
}

/**************************************************************************/
/* get the parameters for FB and FK                                       */
/**************************************************************************/

static char * get_params( const char * scw_name ) {
    char            *   p;
    char            *   pa;
    size_t              len;
    su                  fbk_su;

    p = scan_start;
    SkipSpaces( p );

    if( *p ) {                  // at least one potential operand
        pa = p;
        SkipNonSpaces( p );
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
            p = pa;                                 // reset, invalid input
        } else {
            SkipSpaces( p );                        // find next argument, if any
            if( *p ) {
                if( cur_cmd == fbk_begin ) {        // begin does not allow another operand
                    xx_val_line_warn( wng_too_many_ops, scw_name, p );
                } else {                            // both <n> and <0|w> are treated as space
                    pa = p;                         // values and ignored by wgml 4.0
                    if( !cw_val_to_su( &p, &fbk_su ) ) {
                        if( fbk_su.su_relative ) {
                            xx_line_err( err_spc_not_valid, pa );
                        }
                    }
                }
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
/* NOTE: FB BEGIN does not, in fact, take a space value; only FB END does */
/* NOTE: even after FB END, a space value has no effect                   */
/* NOTE: FB DUMP ignores any count it is given: all pending blocks are    */
/*       output without regard to any value following DUMP                */
/*                                                                        */
/**************************************************************************/

/************************************************************************/
/* Implements control word FB                                           */
/************************************************************************/

void scr_fb( void )
{
    char    *   p;
    doc_element *   cur_el;
    doc_element *   sav_el;

    p = get_params( "FB" );

    switch( cur_cmd) {
    case fbk_begin :
        g_keep_nest( "FB" );                // catch nesting errors
        ProcFlags.in_fb_fk_block = true;
        save_state();
        sav_group_type = cur_group_type;
        cur_group_type = gt_fb;
        cur_doc_el_group = alloc_doc_el_group( gt_fb );
        cur_doc_el_group->next = t_doc_el_group;
        t_doc_el_group = cur_doc_el_group;
        cur_doc_el_group = NULL;
        break;
    case fbk_end :
        if( cur_group_type == gt_fb ) {
            sav_post_space = post_space;
            scr_process_break();
            post_space = sav_post_space;
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
            restore_state();
            ProcFlags.in_fb_fk_block = false;
        } else {
            xx_line_err( err_no_fb_begin, p );
        }
        break;
    case fbk_dump :
        g_keep_nest( "FB" );                // catch nesting errors

        /* Dump all pending blocks*/

        while( block_queue != NULL ) {
            cur_doc_el_group = block_queue;
            block_queue = block_queue->next;
            cur_doc_el_group->next = NULL;
            cur_el = cur_doc_el_group->first;
            while( cur_el != NULL ) {
                sav_el = cur_el->next;
                cur_el->next = NULL;
                insert_col_main( cur_el );
                cur_el = sav_el;
            }
            cur_doc_el_group->first = NULL;
            add_doc_el_group_to_pool( cur_doc_el_group );
        }
        block_queue_end = NULL;
        break;
    case fbk_none :
        xx_val_line_err( err_no_operand, "FB", p - 1 );
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
/*                                                                        */
/* NOTE: FK BEGIN does not, in fact, take a space value; only FK END does */
/* NOTE: even after FK END, a space value has no effect                   */
/* NOTE: FK DUMP ignores any count it is given: all pending blocks are    */
/*       output without regard to any value following DUMP                */
/**************************************************************************/

/************************************************************************/
/* Implements control word FK                                           */
/************************************************************************/

void scr_fk( void )
{
    char        *   p;
    doc_element *   cur_el;
    doc_element *   sav_el;

    p = get_params( "FK" );

    switch( cur_cmd) {
    case fbk_begin :
        g_keep_nest( "FK" );                // catch nesting errors
        ProcFlags.in_fb_fk_block = true;
        save_state();
        sav_group_type = cur_group_type;
        cur_group_type = gt_fk;
        cur_doc_el_group = alloc_doc_el_group( gt_fk );
        cur_doc_el_group->next = t_doc_el_group;
        t_doc_el_group = cur_doc_el_group;
        cur_doc_el_group = NULL;
        break;
    case fbk_end :
        if( cur_group_type == gt_fk ) {
            sav_post_space = post_space;
            scr_process_break();
            post_space = sav_post_space;
            cur_group_type = sav_group_type;
            cur_doc_el_group = t_doc_el_group;
            t_doc_el_group = t_doc_el_group->next;
            cur_doc_el_group->next = NULL;
            if( (n_page.fk_queue != NULL) ||
                    (t_page.cur_depth + cur_doc_el_group->depth > t_page.max_depth) ) {
                if( n_page.fk_queue == NULL ) {
                    n_page.fk_queue = cur_doc_el_group;
                    n_page.last_fk_queue = n_page.fk_queue;
                } else {
                    n_page.last_fk_queue->next = cur_doc_el_group;
                    n_page.last_fk_queue = n_page.last_fk_queue->next;
                }
            } else {
                cur_el = cur_doc_el_group->first;
                while( cur_el != NULL ) {
                    sav_el = cur_el->next;
                    cur_el->next = NULL;
                        insert_col_main( cur_el );
                    cur_el = sav_el;
                }
                cur_doc_el_group->first = NULL;
                cur_doc_el_group->next = NULL;
                add_doc_el_group_to_pool( cur_doc_el_group );
            }
            restore_state();
            ProcFlags.in_fb_fk_block = false;
        } else {
            xx_line_err( err_no_fk_begin, p );
        }
        break;
    case fbk_dump :
        g_keep_nest( "FK" );                // catch nesting errors
        if( n_page.fk_queue != NULL ) {
            scr_process_break();
            next_column();
        }
        break;
    case fbk_none :
        xx_val_line_err( err_no_operand, "FK", p - 1 );
        break;
    default:
        internal_err( __FILE__, __LINE__ );
        break;
    }

    scan_restart = scan_stop + 1;
    return;
}

/**************************************************************************/
/* Output all remaining FB blocks (for use at end-of-file)                */
/**************************************************************************/

void fb_blocks_out( void )
{
    doc_element *   cur_el;
    doc_element *   sav_el;

    if( block_queue != NULL ) {
        next_column();
        while( block_queue != NULL ) {
            cur_doc_el_group = block_queue;
            block_queue = block_queue->next;
            cur_el = cur_doc_el_group->first;
            while( cur_el != NULL ) {
                sav_el = cur_el->next;
                cur_el->next = NULL;
                insert_col_main( cur_el );
                cur_el = sav_el;
            }
            cur_doc_el_group->first = NULL;
            cur_doc_el_group->next = NULL;
            add_doc_el_group_to_pool( cur_doc_el_group );
        }
        block_queue_end = NULL;
        last_page_out();
    }

    return;
}


