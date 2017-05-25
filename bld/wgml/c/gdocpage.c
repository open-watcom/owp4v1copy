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
* Description:  Page-oriented output
*
*               do_ban_column_out       output text from one or more ban_columns
*               do_doc_panes_out        output text from one or more doc_panes
*               do_el_list_out          output text from an array of element lists
*               do_page_out             actually output t_page
*               full_page_out           output all full pages
*               insert_col_bot          insert doc_element into t_page.cols->bot_fig
*               insert_col_fn           insert doc_element into t_page.cols->footnote
*               insert_col_main         insert doc_element into t_page.cols->main
*               insert_col_width        insert doc_element into t_page.col_width
*               insert_page_width       insert doc_element into t_page.page_width
*               last_page_out           force output of all remaining pages
*               reset_bot_ban           reset t_page.bottom_banner and related externs
*               reset_t_page            reset t_page and related externs
*               reset_top_ban           reset t_page.top_banner and related externs
*               set_skip_vars           merge the various skips into the externs
*               set_v_positions         set vertical positions in an element list
*               split_element           splits an element at given depth
*               text_page_out           output all pages where main is full
*               update_t_page           update t_page from n_page
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

static  uint32_t        bottom_depth;   // used in setting banners
static  uint32_t        top_depth;      // used in setting banners

/***************************************************************************/
/*  does the actual output to the device                                   */
/***************************************************************************/
 
static void do_el_list_out( doc_element * array, unsigned char count )
{
    doc_element *   cur_el;
    doc_element *   save;
    int             i;
    text_line   *   cur_line;

    /*  The array should have the doc_elements in linked-list form in the   */
    /*  same order as the columns they came from were linked together.      */
    /*  The columns should no longer point to these doc_element lists.      */

    for( i = 0; i < count; i++ ) {
        cur_el = &array[i];
        while( cur_el != NULL ) {
            if( i == 0 ) {      // restrict output to first column, for now
                switch( cur_el->type ) {
                case el_binc :
                    if( GlobalFlags.lastpass ) {
                        ob_binclude( &cur_el->element.binc );
                    }
                    break;
                case el_dbox :  // should only be found if DBOX block exists
                    if( GlobalFlags.lastpass ) {
                        fb_dbox( &cur_el->element.dbox );
                    }
                    break;
                case el_graph :
                    if( GlobalFlags.lastpass ) {
                        if( ProcFlags.ps_device ) {   // only available to PS device
                            ob_graphic( &cur_el->element.graph );
                        }
                    }
                    break;
                case el_hline :  // should only be found if HLINE block exists
                    if( GlobalFlags.lastpass ) {
                        fb_hline( &cur_el->element.hline );
                    }
                    break;
                case el_text :
                    if( GlobalFlags.lastpass ) {
                        ProcFlags.force_op = cur_el->element.text.force_op;
                        for( cur_line = cur_el->element.text.first; cur_line != NULL; cur_line = cur_line ->next ) {
                            fb_output_textline( cur_line );
                        }
                        ProcFlags.force_op = false;
                    }
                    break;
                case el_vline :  // should only be found if VLINE block exists
                    if( GlobalFlags.lastpass ) {
                        fb_vline( &cur_el->element.vline );
                    }
                    break;
                default :
                    internal_err( __FILE__, __LINE__ );
                }
            }
            save = cur_el->next;
            cur_el->next = NULL;            // clear only current element
            add_doc_el_to_pool( cur_el ); 
            cur_el = save;
        }
    }    

    return;
}

/***************************************************************************/
/*  set the vertical positions in a linked list of elements                */
/***************************************************************************/

static void set_v_positions( doc_element * list, uint32_t v_start )
{
    bool            use_spacing;
    bool            at_top;
    doc_element *   cur_el;
    text_line   *   cur_line;
    uint32_t        cur_spacing;
    uint32_t        old_v_start;

    at_top = !ProcFlags.page_started;
    g_cur_v_start = v_start;

    for( cur_el = list; cur_el != NULL; cur_el = cur_el->next ) {
        use_spacing = false;
        if( at_top ) {
            if( cur_el->blank_lines > 0 ) {
                cur_spacing = cur_el->blank_lines + cur_el->subs_skip;
            } else {
                cur_spacing = cur_el->top_skip;
            }
        } else {
            cur_spacing = cur_el->blank_lines + cur_el->subs_skip;
            use_spacing = (cur_spacing == 0);       // see el_text below
        }

        switch( cur_el->type ) {
        case el_binc :
            cur_el->element.binc.at_top = at_top && (t_page.top_banner == NULL);
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_spacing;
            } else {
                g_cur_v_start += cur_spacing;
            }
            cur_el->element.binc.y_address = g_cur_v_start;
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_el->depth;
            } else {
                g_cur_v_start += cur_el->depth;
            }
            break;
        case el_dbox :
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_spacing;
            } else {
                g_cur_v_start += cur_spacing;
            }
            cur_el->element.dbox.v_start = g_cur_v_start;
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_el->depth;
            } else {
                g_cur_v_start += cur_el->depth;
            }
            break;
        case el_graph :
            cur_el->element.graph.at_top = at_top && (t_page.top_banner == NULL);
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_spacing;
            } else {
                g_cur_v_start += cur_spacing;
            }
            cur_el->element.graph.y_address = g_cur_v_start;
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_el->depth;
            } else {
                g_cur_v_start += cur_el->depth;
            }
            break;
        case el_hline :
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_spacing;
            } else {
                g_cur_v_start += cur_spacing;
            }
            cur_el->element.hline.v_start = g_cur_v_start;
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_el->depth;
            } else {
                g_cur_v_start += cur_el->depth;
            }
            break;
        case el_text :
            for( cur_line = cur_el->element.text.first; cur_line != NULL;
                                                cur_line = cur_line->next ) {
                if( use_spacing ) {
                    cur_spacing += cur_line->spacing + cur_line->line_height;
                } else {
                    cur_spacing += cur_line->line_height;
                    use_spacing = true;         // use between lines
                }
                if( !at_top ) {                 // not first element
                    if( cur_el->element.text.overprint ) {  // overprint
                        if( use_spacing ) {
                            cur_spacing -= cur_line->spacing + cur_line->line_height;
                        } else {
                            cur_spacing -= cur_line->line_height;
                        }
                        cur_el->element.text.overprint = false;
                    }
                } else {

                    /********************************************************/
                    /*  Possible future complexities:                       */
                    /*    it is not clear how forced overprint & minimum    */
                    /*      height interact                                 */
                    /*    it applies to the very first line on the top,     */
                    /*      even if that is a banner, and so does not       */
                    /*      automatically apply if Proc_flags.page_started  */
                    /*      is true, as that happens at the start of each   */
                    /*      section of the page to suppress any subs_skip   */
                    /********************************************************/

                    if( cur_el->element.text.overprint && cur_el->element.text.force_op ) {
                        if( use_spacing ) {
                            cur_spacing -= cur_line->spacing + cur_line->line_height;
                        } else {
                            cur_spacing -= cur_line->line_height;
                        }
                    } else if( (t_page.top_banner == NULL) &&
                                (t_page.panes->page_width == NULL) ) {    // minimum height
                        if( cur_spacing < wgml_fonts[g_curr_font].line_height ) {
                            cur_spacing = wgml_fonts[g_curr_font].line_height;
                        }
                    }
                    at_top = false;
                }

                /****************************************************/
                /* Apply cur_spacing to the vertical position       */
                /* Then fix an overprint line at the top of the     */
                /* page, at least for character devices like TASA.  */
                /* g_cur_v_start cannot be above the y_start which  */
                /* the device specified for the overprint line, but */
                /* must be restored for the remaining lines.        */
                /****************************************************/

                if( bin_driver->y_positive == 0x00 ) {
                    g_cur_v_start -= cur_spacing;
                    old_v_start = g_cur_v_start;
                    if( g_cur_v_start > bin_device->y_start ) {
                        g_cur_v_start = bin_device->y_start;
                    }
                } else {
                    g_cur_v_start += cur_spacing;
                    old_v_start = g_cur_v_start;
                    if( g_cur_v_start < bin_device->y_start ) {
                        g_cur_v_start = bin_device->y_start;
                    }
                }

                cur_line->y_address = g_cur_v_start;
                cur_spacing = 0;
                g_cur_v_start = old_v_start;
            }
            break;
        case el_vline :
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_spacing;
            } else {
                g_cur_v_start += cur_spacing;
            }
            cur_el->element.vline.v_start = g_cur_v_start;
            if( bin_driver->y_positive == 0x00 ) {
                g_cur_v_start -= cur_el->depth;
            } else {
                g_cur_v_start += cur_el->depth;
            }
            break;
        default :
            internal_err( __FILE__, __LINE__ );
        }
    }

    return;
}


/***************************************************************************/
/*  output the ban_column(s)                                               */
/***************************************************************************/

static void do_ban_column_out( ban_column * a_column, uint32_t v_start )
{
    ban_column  *   cur_col;
    doc_element **  cur_el;
    int             i;
    unsigned char   col_count;

    col_count = 0;
    for( cur_col = a_column; cur_col != NULL; cur_col = cur_col->next ) {
        col_count++;
    }
    cur_el = (doc_element **)mem_alloc( col_count * sizeof( doc_element * ) );

    cur_col = a_column;
    for( i = 0; i < col_count; i++ ) {
        cur_el[i] = NULL;
        if( cur_col->first != NULL ) {
            set_v_positions( cur_col->first, v_start );
            cur_el[i] = cur_col->first;
            cur_col->first = NULL;
        }
        cur_col = cur_col->next;
    }

    do_el_list_out( *cur_el, col_count );
    mem_free( cur_el );

    return;
}


/***************************************************************************/
/*  output the doc_pane(s)                                                 */
/*                                                                         */
/*  Convert the linked list of panes into an array of doc_elements, such   */
/*  that each column contains all the doc_elements in the corresponding    */
/*  column of each pane, and the first column contains the doc_element,    */
/*  if any, in each pane's page_width field as if it were, in fact, in the */
/*  first column of that pane                                              */
/***************************************************************************/
 
static void do_doc_panes_out( void )
{
    doc_element *       cur_el[MAX_COL];
    doc_element *       last_el[MAX_COL];
    doc_pane    *       cur_pane;
    int                 i;
    uint32_t            col_count;

    for( i = 0; i < MAX_COL; i++ ) {
        last_el[i] = NULL;
        cur_el[i] = NULL;
    }

    col_count = 0;
    cur_pane = t_page.panes;
    while( cur_pane != NULL ) {
        if( col_count < cur_pane->col_count ) { // maximum number of colums used
            col_count = cur_pane->col_count;
        }
        if( cur_pane->page_width != NULL ) {
            set_v_positions( cur_pane->page_width, cur_pane->page_width_top );
            cur_el[0] = cur_pane->page_width;
            last_el[0] = cur_pane->page_width;
            if( (cur_pane == t_page.panes) && (t_page.post_skip > 0)
                    && (cur_pane->cols != NULL) && (cur_pane->cols->main != NULL) ) {
                cur_pane->cols->main->top_skip = t_page.post_skip;
                t_page.post_skip = 0;
            }
            cur_pane->page_width = NULL;
            ProcFlags.page_started = true;
        } 
        for( i = 0; i < MAX_COL; i++ ) {
            if( cur_pane->cols[i].col_width != NULL ) {
                set_v_positions( cur_pane->cols[i].col_width, cur_pane->col_width_top );
                cur_el[i] = cur_pane->cols[i].col_width;
                last_el[i] = cur_pane->cols[i].col_width;
                while( last_el[i]->next != NULL ) {    // find last element
                    last_el[i] = last_el[i]->next;
                }
                cur_pane->cols[i].col_width = NULL;
                if( (cur_pane == t_page.panes) && (cur_pane->cols[i].post_skip > 0)
                        && (cur_pane->cols[i].main != NULL) ) {
                    cur_pane->cols[i].main->top_skip = cur_pane->cols[i].post_skip;
                    cur_pane->cols[i].post_skip = 0;
                }
                ProcFlags.page_started = true;
            }
            if( cur_pane->cols[i].main != NULL ) {
                set_v_positions( cur_pane->cols[i].main, cur_pane->cols[i].main_top );
                if( cur_el[i] == NULL ) {
                    cur_el[i] = cur_pane->cols[i].main;
                    last_el[i] = cur_pane->cols[i].main;
                } else {
                    last_el[i]->next = cur_pane->cols[i].main;
                }
                while( last_el[i]->next != NULL ) {    // find last element
                    last_el[i] = last_el[i]->next;
                }
                cur_pane->cols[i].main = NULL;
            }
            ProcFlags.page_started = true;
            if( cur_pane->cols[i].bot_fig != NULL ) {
                set_v_positions( cur_pane->cols[i].bot_fig, cur_pane->cols[i].fig_top );
                if( cur_el[i] == NULL ) {
                    cur_el[i] = cur_pane->cols[i].bot_fig;
                    last_el[i] = cur_pane->cols[i].bot_fig;
                } else {
                    last_el[i]->next = cur_pane->cols[i].bot_fig;
                }
                while( last_el[i]->next != NULL ) {    // find last element
                    last_el[i] = last_el[i]->next;
                }
                cur_pane->cols[i].bot_fig = NULL;
            }
            if( cur_pane->cols[i].footnote != NULL ) {
                set_v_positions( cur_pane->cols[i].footnote, cur_pane->cols[i].fn_top );
                if( cur_el[i] == NULL ) {
                    cur_el[i] = cur_pane->cols[i].footnote;
                } else {
                    last_el[i]->next = cur_pane->cols[i].footnote;
                }
                while( last_el[i]->next != NULL ) {    // find last element
                    last_el[i] = last_el[i]->next;
                }
                cur_pane->cols[i].footnote = NULL;
            }
        }
        cur_pane = cur_pane->next;
    }

    do_el_list_out( *cur_el, col_count );

    return;
}


/***************************************************************************/
/*  update t_page from the elements in n_page                              */
/*  the various n_page fields may or may not be empty on return            */
/*  NOTE: the order used is:                                               */
/*    page_width                                                           */
/*    then in each column:                                                 */
/*      col_width                                                          */
/*      col_bot                                                            */
/*      footnote                                                           */
/*      main                                                               */
/*    and this appears to match what wgml 4.0 does                         */
/***************************************************************************/

static void update_t_page( void )
{
    bool                splittable;
    doc_element     *   cur_el;
    doc_el_group    *   cur_group;
    uint32_t            old_max_depth;
    uint32_t            depth;

    reset_t_page();
    old_max_depth = t_page.max_depth;   // save original value for testing

    /***********************************************************************/
    /*  The first block in n_page.page_width is processed                  */
    /*  Remaining blocks will be processed at the rate of one per page     */
    /*  until none are left in n_page.page_width                           */
    /*  Note: t_page.page_width will always be NULL at this point          */
    /***********************************************************************/

    if( n_page.page_width != NULL ) {           // at most one item can be placed
        cur_group = n_page.page_width;
        t_page.panes->page_width = cur_group->first;
        t_page.post_skip = cur_group->post_skip;
        if( bin_driver->y_positive == 0 ) {
            t_page.panes_top -= cur_group->depth;
        } else {
            t_page.panes_top += cur_group->depth;
        }
        t_page.max_depth -= cur_group->depth;
        n_page.page_width = n_page.page_width->next;
        cur_group->next = NULL;
        cur_group->first = NULL;
        add_doc_el_group_to_pool( cur_group );
    }

    /***********************************************************************/
    /*  The first block in n_page.col_width is processed                   */
    /*  Remaining blocks will be processed at the rate of one per page     */
    /*  until none are left in n_page.col_width                            */
    /*  Note: t_page.cols and so t_page.cols->col_width will always be     */
    /*        NULL at this point                                           */
    /***********************************************************************/

    if( n_page.col_width != NULL ) {                // at most one item can be placed
        cur_group = n_page.col_width;
        t_page.cur_col->col_width = cur_group->first;
        t_page.cur_col->post_skip = cur_group->post_skip;
        if( bin_driver->y_positive == 0 ) {
            t_page.cur_col->main_top -= cur_group->depth;
        } else {
            t_page.cur_col->main_top += cur_group->depth;
        }
        t_page.max_depth -= cur_group->depth;
        n_page.col_width = n_page.col_width->next;
        cur_group->next = NULL;
        cur_group->first = NULL;
        add_doc_el_group_to_pool( cur_group );
    }

    /***********************************************************************/
    /*  The first block in n_page.col_bot is processed                     */
    /*  Remaining blocks will be processed at the rate of one per page     */
    /*  until none are left in n_page.col_bot                              */
    /*  Note: t_page.cols will only be NULL at this point if no top column */
    /*        width FIG has been placed on the page                        */
    /*        t_page.cols->bot_fig will always be NULL at this point       */
    /***********************************************************************/

    if( n_page.col_bot != NULL ) {              // at most one item can be placed
        cur_group = n_page.col_bot;
        t_page.cur_col->bot_fig = cur_group->first;
        if( bin_driver->y_positive == 0) {
            t_page.cur_col->fig_top += cur_group->depth;
        } else {
            t_page.cur_col->fig_top -= cur_group->depth; 
        }
        t_page.max_depth -= cur_group->depth;
        n_page.col_bot = n_page.col_bot->next;
        cur_group->next = NULL;
        cur_group->first = NULL;
        add_doc_el_group_to_pool( cur_group );
    }

    /***********************************************************************/
    /*  As many blocks from n_page.col_fn as will fit are processed        */
    /*  Until a block that fits is found, each block is dropped and a      */
    /*    warning is issued. Once such a block is found, all blocks are    */
    /*    processed until one that will not fit is found                   */
    /*  That block and all remaining blocks will be processed on future    */
    /*    until none are left in n_page.col_fn                             */
    /*  Note: t_page.cols will only be NULL at this point if no top column */
    /*        width or bottom FIG has been placed on the page              */
    /***********************************************************************/

    if( n_page.col_fn != NULL ) {
        while( n_page.col_fn != NULL ) {
            cur_group = n_page.col_fn;
            if( t_page.cur_depth + cur_group->depth > t_page.max_depth ) {
                while( (cur_group->first != NULL) &&
                        (t_page.cur_depth + cur_group->first->depth <= t_page.max_depth) ) {
                    cur_el = cur_group->first; 
                    if( t_page.cur_col->footnote == NULL ) {
                        t_page.cur_col->footnote = cur_el;
                    } else {
                        t_page.last_col_fn->next = cur_el;
                    }
                    t_page.last_col_fn = cur_el;
                    cur_group->first = cur_group->first->next;
                    cur_el->next = NULL;
                    if( bin_driver->y_positive == 0) {
                        t_page.cur_col->fig_top += cur_el->depth;
                        t_page.cur_col->fn_top += cur_el->depth; 
                    } else {
                        t_page.cur_col->fig_top -= cur_el->depth; 
                        t_page.cur_col->fn_top -= cur_el->depth; 
                    }
                    t_page.max_depth -= cur_el->depth;
                    cur_group->depth -= cur_el->depth;
                }
            } else {
                cur_el = cur_group->first;          // here, cur_el is the last element
                while( cur_el->next != NULL ) {
                    cur_el = cur_el->next;
                }
                if( t_page.cur_col->footnote == NULL ) {
                    t_page.cur_col->footnote = cur_group->first;
                } else {
                    t_page.last_col_fn->next = cur_group->first;
                }
                t_page.last_col_fn = cur_el;
                if( bin_driver->y_positive == 0) {
                    t_page.cur_col->fig_top += cur_group->depth;
                    t_page.cur_col->fn_top += cur_group->depth;
                } else {
                    t_page.cur_col->fig_top -= cur_group->depth; 
                    t_page.cur_col->fn_top -= cur_group->depth; 
                }
                t_page.max_depth -= cur_group->depth;
            }
            n_page.col_fn = n_page.col_fn->next;
            cur_group->next = NULL;
            cur_group->first = NULL;
            add_doc_el_group_to_pool( cur_group );
        }
    }

    /***********************************************************************/
    /*  As many blocks from n_page.col_main as will fit are processed      */
    /*  A page is not full unless there is at least one element left in    */
    /*    at least one of the sections of n_page                           */
    /*  The original value of max_depth is used to prevent looping if the  */
    /*    current element is too large to fit on any page                  */
    /*  Note: a page which is not full will only be emitted if it is the   */
    /*    last page in a section (or the document)                         */
    /*  Note: t_page.cols will only be NULL at this point if neither       */
    /*    footnotes nor a buttom FIG has been placed on the page           */
    /***********************************************************************/

    while( n_page.col_main != NULL ) {
        cur_el = n_page.col_main;
        n_page.col_main = n_page.col_main->next;
        if( n_page.col_main == NULL ) {
            n_page.last_col_main = NULL;
        }

        /****************************************************************/
        /*  this section identifies skips and blank lines that finish   */
        /*  the current page and then exits the loop after adjusting    */
        /*  the element field values as needed                          */
        /****************************************************************/
        
        if( cur_el->blank_lines > 0 ) {
            if( (t_page.cur_depth + cur_el->blank_lines) >= t_page.max_depth ) {
                cur_el->blank_lines -= (t_page.max_depth - t_page.cur_depth);
                break;
            } else if( !ProcFlags.page_started && ((t_page.cur_depth +
                        cur_el->blank_lines + cur_el->top_skip) >=
                        t_page.max_depth) ) {
                cur_el->top_skip -= (t_page.max_depth - t_page.cur_depth);
                cur_el->top_skip += cur_el->blank_lines;
                cur_el->blank_lines = 0;
                break;
            } else if( (t_page.cur_depth + cur_el->blank_lines +
                         cur_el->subs_skip) >= t_page.max_depth ) {
                cur_el->blank_lines = 0;
                break;
            }
        }

        if( !ProcFlags.page_started ) {
            if( cur_el->blank_lines > 0 ) {
                depth = cur_el->blank_lines + cur_el->subs_skip;
            } else {
                depth = cur_el->top_skip;
            }
            ProcFlags.page_started = true;
        } else {
            depth = cur_el->blank_lines + cur_el->subs_skip;
        }

        if( depth >= t_page.max_depth ) {    // skip fills page
            break;
        }

        /****************************************************************/
        /*  Does the first line minimum apply here? If so, it needs to  */
        /*  be implemented. Note that cur_el->depth does not reflect it */
        /*  because there is no way to tell if it will apply when the   */
        /*  cur_el->depth is computed.                                  */
        /****************************************************************/

        if( (depth + cur_el->depth) > t_page.max_depth ) {    // cur_el will fill the page
            splittable = split_element( cur_el, t_page.max_depth -
                                                t_page.cur_depth - depth );
            if( splittable ) {
                if( cur_el->next != NULL ) {    // cur_el was split
                    n_page.col_main = cur_el->next;
                    if( n_page.last_col_main == NULL ) {
                        n_page.last_col_main = n_page.col_main;
                    }
                    cur_el->next = NULL;
                }
                if( t_page.cur_col->main == NULL ) {
                    t_page.cur_col->main = cur_el;
                    t_page.last_col_main = t_page.cur_col->main;
                } else {
                    t_page.last_col_main->next = cur_el;
                    t_page.last_col_main = t_page.last_col_main->next;
                }
                t_page.last_col_main->next = NULL;
                t_page.cur_depth += cur_el->depth;
            } else {
                if( cur_el->depth > old_max_depth ) {   // cur_el will not fit on any page
                    xx_err( err_text_line_too_deep );
                    g_suicide();                        // prevents looping
                }
                n_page.col_main = cur_el->next;
                if( n_page.last_col_main == NULL ) {
                    n_page.last_col_main = n_page.col_main;
                }
                cur_el->next = NULL;
            }
        } else {                                    // cur_el fits as-is
            if( t_page.cur_col->main == NULL ) {
                t_page.cur_col->main = cur_el;
                t_page.last_col_main = t_page.cur_col->main;
            } else {
                t_page.last_col_main->next = cur_el;
                t_page.last_col_main = t_page.last_col_main->next;
            }
            t_page.last_col_main->next = NULL;
            if( (cur_el->type == el_text) && cur_el->element.text.overprint
                                          && cur_el->element.text.force_op ) {
                /* do nothing, adjusts for top-of-page overprint */
            } else {
                t_page.cur_depth += cur_el->depth;
            }
        }
    }
    return;
}

/***************************************************************************/
/*  actually output t_page to the device                                   */
/***************************************************************************/

void do_page_out( void )
{
    doc_column  *   first_col;
    doc_element *   work_el;
    font_number     save_prev;
    uint32_t        curr_height;
    uint32_t        prev_height;
    uint32_t        sav_hs;
    uint32_t        sav_hl;

    /* Set up for the new page */

    if( apage && GlobalFlags.lastpass ) {   // don't do before first page
       fb_document_page();                  // NEWPAGE is interpreted here
    }
    apage++;
    page++;

    /* Get the banner text into the proper sections */

    if( ProcFlags.keep_left_margin ) {
        sav_hs = g_cur_h_start;
        sav_hl = g_cur_left;
    }
    g_cur_h_start = g_page_left_org;
    g_cur_left    = g_page_left_org;

    if( (t_page.top_banner != NULL) && (t_page.top_banner->region != NULL) ) {
        save_prev = g_prev_font;            // will be reset for top banner
        out_ban_top();

        /********************************************************************/
        /* Adjust height of first element, if hline with ban_adjust == true */
        /* Note: since this applies only to hline, page_width must be empty */
        /*       and the hline must be the first or second element in main, */
        /*       specifically in main.main                                  */
        /* The first text element on the page is also adjusted              */
        /* Note: in the OW docs, this will always be just after the top     */
        /*       box line                                                   */
        /********************************************************************/

        first_col = &t_page.panes->cols[0];
        if( (t_page.panes->page_width == NULL) && (first_col->main != NULL) ) {
            work_el = NULL;
            if( (first_col->main->type == el_hline) &&
                    (first_col->main->element.hline.ban_adjust) ) {
                work_el = first_col->main;
            } else if( (first_col->main->type == el_text) &&
                    (first_col->main->next != NULL) &&
                    (first_col->main->next->type == el_hline) &&
                    (first_col->main->next->element.hline.ban_adjust) ) {
                work_el = first_col->main->next;
            }
            if( work_el != NULL ) {                     // top line is hline
                curr_height = wgml_fonts[g_curr_font].line_height;
                prev_height = wgml_fonts[g_prev_font].line_height;
                if( prev_height < curr_height ) {
                    work_el->subs_skip += (curr_height - prev_height) / 2;
                    work_el->top_skip += (curr_height - prev_height) / 2;
                } else if( prev_height > curr_height ) {
                    work_el->subs_skip -= (prev_height - curr_height) / 2;
                    work_el->top_skip -= (prev_height - curr_height) / 2;
                }
                while( (work_el != NULL) && (work_el->type != el_text) ) {
                    work_el = work_el->next;
                }
                if( work_el != NULL ) {             // element is text
                    if( prev_height < curr_height ) {
                        work_el->subs_skip -= (curr_height - prev_height) / 2;
                    } else if( prev_height > curr_height ) {
                        work_el->subs_skip += (prev_height - curr_height) / 2;
                    }
                }
                work_el = first_col->main;
                while( (work_el != NULL) && (work_el->type != el_vline) ) {
                    work_el = work_el->next;
                }
                while( (work_el != NULL) && (work_el->type == el_vline)  ) {
                    if( prev_height < curr_height ) {
                        work_el->element.vline.v_len -= (curr_height - prev_height) / 2;
                    } else if( prev_height > curr_height ) {
                        work_el->element.vline.v_len += (prev_height - curr_height) / 2;
                    }
                    work_el = work_el->next;
                }
            }
        }
        g_prev_font = save_prev;            // restore old g_prev_font value
    }

    if( ProcFlags.keep_left_margin ) {
        g_cur_h_start = sav_hs;
        g_cur_left = sav_hl;
    }

    if( (t_page.bottom_banner != NULL) && (t_page.bottom_banner->region != NULL) ) {
        out_ban_bot();
    }

    /****************************************************************/
    /* Finish drawing any open box.                                 */
    /* Note: this may need to be done for other control words and   */
    /* tags that accumulate doc_elements                            */
    /****************************************************************/

    if( ProcFlags.in_bx_box ) {
        eop_bx_box();
    }

    /* Output the page section by section */

    ProcFlags.page_started = false;
    if( t_page.top_ban != NULL ) {
        do_ban_column_out( t_page.top_ban, t_page.page_top );
        add_ban_col_to_pool( t_page.top_ban );
        t_page.top_ban = NULL;
    }

    if( t_page.panes != NULL ) {
        do_doc_panes_out();
        ProcFlags.page_started = true;
    }

    if( t_page.bot_ban != NULL ) {
        do_ban_column_out( t_page.bot_ban, t_page.bot_ban_top );
        add_ban_col_to_pool( t_page.bot_ban );
        t_page.bot_ban = NULL;
    }

    /************************************************************/
    /* If this page used banners associated with an Hn heading, */
    /* reset the banners for the next page                      */
    /************************************************************/

    if( ProcFlags.heading_banner ) {
        set_section_banners( ProcFlags.doc_sect );
        ProcFlags.heading_banner = false;
    }

    ProcFlags.page_started = false;             // reset after output done
    ProcFlags.tophead_done = false;             // reset after output done
    return;
}


/***************************************************************************/
/*  output all full pages                                                  */
/*  t_page will not be empty but n_page will be empty on return            */
/*  Note: may need to be expanded to consider doc_elements accumulated by  */
/*  the various tags and control words that accumulate doc_elements        */
/***************************************************************************/

void full_page_out( void )
{
    while( (n_page.page_width != NULL) || (n_page.col_width != NULL)
            || (n_page.col_main != NULL) || (n_page.col_bot != NULL)
            || (n_page.col_fn != NULL) ) {
        do_page_out();
        update_t_page();
    }
    return;
}


/****************************************************************************/
/*  insert a doc_element_group into t_page.cols->bot_fig                    */
/****************************************************************************/

void insert_col_bot( doc_el_group * a_group )
{
    uint32_t    depth;

    /****************************************************************/
    /*  if t_page.cols->bot_fig is empty and if it fits, place      */
    /*  cur_doc_el_group in t_page.cols->bot_fig                    */
    /*  otherwise, append cur_doc_el_groupto n_page.last_col_bot    */
    /*  NOTE: FIG/eFIG blocks must fit on an empty page or, rather, */
    /*        must be split so as to fit before they are submitted  */
    /****************************************************************/

    depth = a_group->depth;

    if( (t_page.cur_col->bot_fig == NULL) &&
        ((t_page.cur_depth + depth) <= t_page.max_depth) ) {    // the figure will fit

        t_page.cur_col->bot_fig = a_group->first;
        a_group->first = NULL;
        add_doc_el_group_to_pool( a_group );
        if( bin_driver->y_positive == 0) {
            t_page.cur_col->fig_top += depth; 
        } else {
            t_page.cur_col->fig_top -= depth; 
        }
        t_page.max_depth -= depth;
    } else {
        if( n_page.col_bot == NULL ) {
            n_page.col_bot = a_group;
        } else {
            n_page.last_col_bot->next = a_group;
        }
        n_page.last_col_bot = a_group;
    }

    return;
}


/***************************************************************************/
/*  insert a doc_element into t_page.cols->footnote                        */
/***************************************************************************/

void insert_col_fn( doc_el_group * a_group )
{
    doc_element *   cur_el;
    uint32_t        depth;
    
    /****************************************************************/
    /*  if it fits, put the footnote on the current page            */
    /*  if it does not fit, and it is the firt, split it            */
    /*  otherwise, put the footnote on n_page.last_col_fn           */
    /****************************************************************/

    depth = a_group->depth;
    if( (t_page.cur_depth + depth) > t_page.max_depth ) {
        while( (a_group->first != NULL) &&
                (t_page.cur_depth + a_group->first->depth <= (t_page.max_depth)) ) {
            cur_el = a_group->first; 
            if( t_page.cur_col->footnote == NULL ) {
                t_page.cur_col->footnote = cur_el;
            } else {
                t_page.last_col_fn->next = cur_el;
            }
            t_page.last_col_fn = cur_el;
            a_group->first = a_group->first->next;
            cur_el->next = NULL;
            if( bin_driver->y_positive == 0) {
                t_page.cur_col->fig_top += cur_el->depth;
                t_page.cur_col->fn_top += cur_el->depth; 
            } else {
                t_page.cur_col->fig_top -= cur_el->depth; 
                t_page.cur_col->fn_top -= cur_el->depth; 
            }
            t_page.max_depth -= cur_el->depth;
            a_group->depth -= cur_el->depth;
        }
        if( a_group->first != NULL ) {          // more footnote text remains
            if( n_page.col_fn == NULL ) {
                n_page.col_fn = a_group;
            } else {
                n_page.last_col_fn->next = a_group;
            }
            n_page.last_col_fn = a_group;
        }
    } else {
        cur_el = a_group->first;            // here, cur_el is the last element
        while( cur_el->next != NULL ) {
            cur_el = cur_el->next;
        }
        if( t_page.cur_col->footnote == NULL ) {
            t_page.cur_col->footnote = a_group->first;
        } else {
            t_page.last_col_fn->next = a_group->first;
        }
        t_page.last_col_fn = cur_el;
        a_group->first = NULL;
        add_doc_el_group_to_pool( a_group );
        if( bin_driver->y_positive == 0) {
            t_page.cur_col->fig_top += depth;
            t_page.cur_col->fn_top += depth;
        } else {
            t_page.cur_col->fig_top -= depth;
            t_page.cur_col->fn_top -= depth;
        }
        t_page.max_depth -= depth;
    }

    return;
}


/***************************************************************************/
/*  insert a doc_element into t_page.cols->main                            */
/*  may need update to work with multi-column pages: this would be a good  */
/*  place to add the next column to t_page.cols                            */
/***************************************************************************/

void insert_col_main( doc_element * a_element )
{
    bool            page_full;
    bool            splittable;
    uint32_t        cur_skip;
    uint32_t        depth;

    /****************************************************************/
    /*  alternate procesing: accumulate elements for later          */
    /*  submission                                                  */
    /*  this is the basis for all block processing                  */
    /****************************************************************/

    if( t_doc_el_group != NULL ) {
        if( t_doc_el_group->first == NULL ) {
            t_doc_el_group->first = a_element;
            t_doc_el_group->last = t_doc_el_group->first;
            t_doc_el_group->depth = (a_element->blank_lines + a_element->subs_skip +
                                     a_element->depth);
        } else {
            t_doc_el_group->last->next = a_element;
            t_doc_el_group->last = t_doc_el_group->last->next;
            t_doc_el_group->depth += (a_element->blank_lines + a_element->subs_skip +
                                      a_element->depth);
            if( a_element->type == el_text ) {  // subsequent text elements only
                a_element->depth += g_spacing;
                t_doc_el_group->depth += g_spacing;
            }
        }
        return;
    }

    /****************************************************************/
    /*  test version until things get a bit more clear              */
    /*  the theory here is that only one processing step should be  */
    /*      here, the rest being done in update_t_page()            */
    /****************************************************************/

    page_full = false;

    /****************************************************************/
    /*  this section sets page_full to "true" if any of the skips   */
    /*  or blank_lines finishes the page, alone or in various       */
    /*  combinations                                                */
    /*  element field values are adjusted as needed                 */
    /****************************************************************/
        
    if( a_element->blank_lines > 0 ) {
        if( (t_page.cur_depth + a_element->blank_lines) >= t_page.max_depth ) {
            a_element->blank_lines -= (t_page.max_depth - t_page.cur_depth);
            page_full = true;
        } else if( !ProcFlags.page_started && ((t_page.cur_depth +
                    a_element->blank_lines + a_element->top_skip) >=
                    t_page.max_depth) ) {
            a_element->top_skip -= (t_page.max_depth - t_page.cur_depth);
            a_element->top_skip += a_element->blank_lines;
            a_element->blank_lines = 0;
            page_full = true;
        } else if( (t_page.cur_depth + a_element->blank_lines +
                     a_element->subs_skip) >= t_page.max_depth ) {
            a_element->blank_lines = 0;
            page_full = true;
        }
    } else if( !ProcFlags.page_started ) {
        if( a_element->top_skip >= t_page.max_depth ) {
            a_element->top_skip -= t_page.max_depth;
            page_full = true;
        }
    }

    if( !page_full ) {

        /****************************************************************/
        /*  this test is done separately because an element may fail to */
        /*  set page_full to "true" above but the skip actually used    */
        /*  may still fill the page                                     */
        /****************************************************************/

        if( !ProcFlags.page_started ) {
            if( a_element->blank_lines > 0 ) {
                cur_skip = a_element->blank_lines + a_element->subs_skip;
            } else {
                cur_skip = a_element->top_skip;
            }
        } else {
            cur_skip = a_element->blank_lines + a_element->subs_skip;
        }

        if( (cur_skip + t_page.cur_depth) > t_page.max_depth ) {
            page_full = true;
        }
    }

    if( !page_full ) {

        /****************************************************************/
        /*  at least part of the element should fit on the page         */
        /*  does the first line minimum apply here? If so, it needs to  */
        /*  be implemented. Note that cur_el->depth does not reflect it */
        /*  because there is no way to tell if it will apply when the   */
        /*  is computed.                                                */
        /*  overprint lines pose a problem:                             */
        /*    anywhere but at the top of the page, they do not count as */
        /*      part of the page depth                                  */
        /*    if the page is full (t_page.cur_depth == t_page.max_depth)*/
        /*      they do count ... and start a new page                  */
        /****************************************************************/

        if( (a_element->type == el_text) && a_element->element.text.overprint && 
                                        (t_page.cur_depth != t_page.max_depth) ) {
            depth = cur_skip;
        } else {
            depth = cur_skip + a_element->depth;
        }
        if( (depth + t_page.cur_depth) > t_page.max_depth ) {   // a_element fills the page
            splittable = split_element( a_element, t_page.max_depth -
                                        t_page.cur_depth - cur_skip );
            if( a_element->next != NULL ) { // a_element was split
                if( t_page.cur_col->main == NULL ) {
                    t_page.cur_col->main = a_element;
                    t_page.last_col_main = t_page.cur_col->main;
                } else {
                    t_page.last_col_main->next = a_element;
                    t_page.last_col_main = t_page.last_col_main->next;
                }
                t_page.cur_depth += depth;
                a_element = a_element->next;
                t_page.last_col_main->next = NULL;
            }
            page_full = true;
        } else {    // the entire element fits on the current page
            if( t_page.cur_col->main == NULL ) {
                t_page.cur_col->main = a_element;
                t_page.last_col_main = t_page.cur_col->main;
            } else {
                t_page.last_col_main->next = a_element;
                t_page.last_col_main = t_page.last_col_main->next;
            }
            t_page.cur_depth += depth;
        }
        ProcFlags.page_started = true;
    }

    if( page_full ) {

        /****************************************************************/
        /*  if page_full is true then a_element goes to n_page and      */
        /*  t_page must be output                                       */
        /****************************************************************/

        if( n_page.col_main == NULL ) {
            n_page.col_main = a_element;
            n_page.last_col_main = n_page.col_main;
        } else {
            n_page.last_col_main->next = a_element;
            n_page.last_col_main = n_page.last_col_main->next;
        }
        text_page_out();
    } 

    return;
}


/***************************************************************************/
/*  insert a doc_element into t_page.cols->col_width                       */
/***************************************************************************/

void insert_col_width( doc_el_group * a_group )
{
    uint32_t    depth;

    /****************************************************************/
    /*  if t_page.cols->col_width is empty and if it fits, place    */
    /*    cur_doc_el_group in t_page.cols->col_width                */
    /*  otherwise, append cur_doc_el_group to n_page.last_col_width */
    /*  only FIG/eFIG blocks with place set to top and width no     */
    /*    greater than "column" come here                           */
    /*  NOTE: FIG/eFIG blocks must fit on an empty page or, rather, */
    /*        must be split so as to fit before they are submitted  */
    /****************************************************************/

    depth = a_group->depth;
    if( depth <= t_page.max_depth ) {
        if( t_page.cur_col->col_width == NULL ) {          // must be empty
            t_page.cur_col->col_width = a_group->first;
            t_page.cur_col->post_skip = a_group->post_skip;
            a_group->first = NULL;
            t_page.max_depth -= depth;
            ProcFlags.page_started = true;
            if( bin_driver->y_positive == 0 ) {
                t_page.cur_col->main_top -= depth;
            } else {
                t_page.cur_col->main_top += depth;
            }
            a_group->first = NULL;
            add_doc_el_group_to_pool( a_group );
        } else {        // save on n_page as a block, not as doc_elements
            if( n_page.col_width == NULL ) {
                n_page.col_width = a_group;
            } else {
                n_page.last_col_width->next = a_group;
            }
            n_page.last_col_width = a_group;
        }        
    } else {
        if( n_page.col_width == NULL ) {
            n_page.col_width = a_group;
        } else {
            n_page.last_col_width->next = a_group;
        }
        n_page.last_col_width = a_group;
    }

    return;
}


/***************************************************************************/
/*  insert a doc_element into t_page.page_width                            */
/***************************************************************************/

void insert_page_width( doc_el_group * a_group )
{
    uint32_t    depth;

    /****************************************************************/
    /*  headings with page_eject set to any value except ej_no or   */
    /*  which will not fit on the current page and FIG/eFIG blocks  */
    /*  with place set to top and width greater than "column" come  */
    /*  here                                                        */
    /*  headings must be sized before submission as they cannot be  */
    /*  split or delayed                                            */
    /*  NOTE: FIG/eFIG blocks must fit on an empty page or, rather, */
    /*        must be split so as to fit before they are submitted  */
    /****************************************************************/

    depth = a_group->depth;
    if( depth <= t_page.max_depth ) {
        if( t_page.panes->page_width == NULL ) {       // must be empty
            t_page.panes->page_width = a_group->first;
            t_page.post_skip = a_group->post_skip;
            a_group->first = NULL;
            t_page.max_depth -= depth;
            ProcFlags.page_started = true;
            if( bin_driver->y_positive == 0 ) {
                t_page.panes_top -= depth;
                t_page.cur_col->main_top -= depth;
            } else {
                t_page.panes_top += depth;
                t_page.cur_col->main_top += depth;
            }
            a_group->first = NULL;
            add_doc_el_group_to_pool( a_group );
        } else {        // save on n_page as a block, not as doc_elements
            if( n_page.page_width == NULL ) {
                n_page.page_width = a_group;
            } else {
                n_page.last_page_width->next = a_group;
            }
            n_page.last_page_width = a_group;
        }        
    } else {
        internal_err( __FILE__, __LINE__ );
    }

    return;
}


/***************************************************************************/
/*  force output of the current page, even if not full                     */
/*  t_page and n_page will both be empty on return                         */
/*  Note: may need to be expanded to consider doc_elements accumulated by  */
/*  the various tags and control words that accumulate doc_elements        */
/***************************************************************************/
 
void last_page_out( void )
{
    full_page_out();

    /* Emit last page only if it has content, not just banners */

    if( (t_page.panes->page_width != NULL) || (t_page.panes->cols[0].main != NULL) ) {
        do_page_out();
        reset_t_page();
    }

    return;
}


/***************************************************************************/
/*  set the bottom banner and adjust the max depth                         */
/***************************************************************************/

void reset_bot_ban( void )
{
    uint32_t    old_depth;

    if( t_page.bottom_banner != NULL ) {
        old_depth = t_page.bottom_banner->ban_depth;
    } else {
        old_depth = 0;
    }
    
    t_page.bottom_banner = sect_ban_bot[!(page & 1)];

    if( t_page.bottom_banner != NULL ) {
        bottom_depth = t_page.bottom_banner->ban_depth;
        if( bin_driver->y_positive == 0x00 ) {
            t_page.bot_ban_top = g_page_bottom_org + bottom_depth;
        } else {
            t_page.bot_ban_top = g_page_bottom_org - bottom_depth;
        }
    } else {
        bottom_depth = 0;
        t_page.bot_ban_top = g_page_bottom_org;
    }

    if( old_depth < bottom_depth ) {
        max_depth -= (bottom_depth - old_depth);
    } else if( bottom_depth < old_depth ) {
        max_depth += (old_depth - bottom_depth);
    }
}


/***************************************************************************/
/*  set the top banner and adjust the max depth                            */
/***************************************************************************/

void reset_top_ban( void )
{
    uint32_t    old_depth;

    if( t_page.top_banner != NULL ) {
        old_depth = t_page.top_banner->ban_depth;
    } else {
        old_depth = 0;
    }

    t_page.top_banner = sect_ban_top[!(page & 1)];

    if( t_page.top_banner != NULL ) {
        top_depth = t_page.top_banner->ban_depth;
        if( bin_driver->y_positive == 0x00 ) {
            t_page.panes_top = t_page.page_top - top_depth;
        } else {
            t_page.panes_top = t_page.page_top + top_depth;
        }
    } else {
        top_depth = 0;
        t_page.panes_top = t_page.page_top;
    }
    t_page.panes->page_width_top = t_page.panes_top;

    if( old_depth < top_depth ) {
        t_page.max_depth -= (top_depth - old_depth);
    } else if( top_depth < old_depth ) {
        t_page.max_depth += (old_depth - top_depth);
    }
}


/***************************************************************************/
/* reset t_page and related externs                                        */
/***************************************************************************/

void reset_t_page( void )
{
    doc_pane    *   cur_pane;
    doc_pane    *   sav_pane;
    int             i;

    reset_top_ban();
    reset_bot_ban();

    if( t_page.top_banner != NULL ) {
        top_depth = t_page.top_banner->ban_depth;
    } else {
        top_depth = 0;
    }

    if( t_page.bottom_banner != NULL ) {
        bottom_depth = t_page.bottom_banner->ban_depth;
    } else {
        bottom_depth = 0;
    }

    t_page.max_depth = g_page_depth - top_depth - bottom_depth;
    t_page.cur_depth = 0;
    t_page.last_col_main = NULL;
    t_page.last_col_fn = NULL;
    max_depth = t_page.max_depth - t_page.cur_depth;

    if( t_page.panes->next != NULL ) {  // keep only the first pane
        cur_pane = t_page.panes->next;
        while( cur_pane != NULL ) {
            sav_pane = cur_pane->next;
            cur_pane->next = NULL;
            mem_free( cur_pane );
            cur_pane = sav_pane;
        }
    }
    t_page.panes->col_width_top = t_page.panes_top;
    for( i = 0; i < t_page.panes->col_count; i++ ) {
        t_page.panes->cols[i].main_top = t_page.panes_top;
        t_page.panes->cols[i].fig_top = t_page.bot_ban_top;
        t_page.panes->cols[i].fn_top = t_page.bot_ban_top;
    }
    ProcFlags.page_started = false;
}


/***************************************************************************/
/*  merge the various skips and set the externs appropriately              */
/***************************************************************************/

void set_skip_vars( su * pre_skip, su * pre_top_skip, su * post_skip,
                    uint32_t spacing, font_number font )
{
    int32_t skiptop;
    int32_t skippost;
    int32_t skippre;
    int32_t skipsk;

    skipsk = calc_skip_value();     // pending .sk value?
    if( pre_skip != NULL ) {
        skippre = conv_vert_unit( pre_skip, spacing, font );
    } else {
        skippre = 0;
    }
    if( pre_top_skip != NULL ) {
        skiptop = conv_vert_unit( pre_top_skip, spacing, font );
    } else {
        skiptop = 0;
    }

    if( g_subs_skip > skipsk ) {    // provisional: merge with existing value
        skipsk = g_subs_skip;       // not yet in Wiki per server failure
    }

    if( g_post_skip > skipsk ) {    // merge sk-skip & post-skip per Wiki
        skippost = g_post_skip;
    } else {
        skippost = skipsk;
    }

    if( skippre > skiptop ) {   // merge pre-skip & pre-top-skip per Wiki
        if( skippre > (2 * skiptop) ) {
            skippre -= skiptop;
        } else {
            skippre = skiptop;
        }
    } else {
        skippre = skiptop;
    }

    if( skippost > skippre ) {  // final merger per Wiki
        skippre = skippost;
    }    

    g_subs_skip = skippre;
    g_top_skip = skiptop;

    if( post_skip != NULL ) {
        g_post_skip = conv_vert_unit( post_skip, spacing, font );
    } else {
        g_post_skip = 0;
    }
    g_blank_lines = blank_lines * wgml_fonts[font].line_height;
    blank_lines = 0;
    g_spacing = (spacing - 1) * wgml_fonts[font].line_height;
    ProcFlags.skips_valid = true;

    return;
}


/***************************************************************************/
/*  split one doc_element into two                                         */
/*  the first doc_element returned will fit in the depth specified         */
/*  the return value will be false if the doc_element could not be split   */
/***************************************************************************/

bool split_element( doc_element * a_element, uint32_t req_depth )
{
    bool            splittable;
    doc_element *   split_el;
    text_line   *   cur_line;
    text_line   *   last;
    uint32_t        count;
    uint32_t        cur_depth;

    count = 0;
    cur_depth = 0;
    last = NULL;
    splittable = true;

    switch( a_element->type ) {
    // add code for other element types as appropriate
    case el_binc :
    case el_dbox :
    case el_graph :
    case el_hline :
    case el_vline :
        splittable = false;     
        break;
    case el_text :
        for( cur_line = a_element->element.text.first; cur_line != NULL;
                                cur_line = cur_line->next ) {
            if( (cur_depth + cur_line->line_height + cur_line->spacing) > req_depth ) {
                break;
            }
            count++;
            cur_depth += cur_line->line_height + cur_line->spacing;
            last = cur_line;
        }

        if( cur_line != NULL ) {    // at least one more line
            if( count < g_cur_threshold ) {
                splittable = false;     // widow criteria failed
                a_element->blank_lines = 0;
                break;
            }
        }

        if( last == NULL ) {        // all lines fit; unlikely, but seen
            break;
        }

        /*  if we get here, a_element is splittable, cur_line is    */
        /*  the first line of the new element, and last is the last */
        /*  line that can be left in the original element           */

        split_el = alloc_doc_el( el_text ); // most defaults are correct

        split_el->depth = a_element->depth - cur_depth;
        split_el->element.text.first = cur_line;
        last->next = NULL;
        a_element->depth = cur_depth;
        if( a_element->next == NULL ) {
            a_element->next = split_el;
        } else {
            split_el->next = a_element->next;
            a_element->next = split_el;
        }
        
        break;
    default :
        internal_err( __FILE__, __LINE__ );
    }
    return( splittable );
}


/***************************************************************************/
/*  output all pages in which t_page.cols->main is full                    */
/*  t_page will not be empty but n_page.col_main will be empty on return   */
/*  Note: may need to be expanded to consider doc_elements accumulated by  */
/*  the various tags and control words that accumulate doc_elements        */
/***************************************************************************/

void text_page_out( void )
{
    while( n_page.col_main != NULL ) {
        do_page_out();
        update_t_page();
    }
    return;
}

