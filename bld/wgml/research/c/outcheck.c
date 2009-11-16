/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
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
* Description:  Tests the text file output functions for wgml.
*               In addition to main(), these global functions are implemented:
*                   print_banner()
*                   print_usage()
*               as well as this #define macro:
*                   TEXT_START
*               and these enums:
*                   oc_types
*                   tp_types
*               and these structs:
*                   tp_bx_block
*                   test_element
*                   test_page
*                   text_phrase
*                   tp_bx_element
*                   tp_fig_element
*                   tp_text_element
*               and these variables (the text arrays are not listed):
*                   oc_h_start
*                   oc_v_start
*                   oc_font_number
*                   oc_tab_char
*                   oc_test_element_pool
*                   oc_text_chars_pool
*                   oc_text_line_pool
*                   oc_test_page
*               and these local functions:
*                   emulate_fill_text_lines()
*                   emulate_input_source()
*                   emulate_layout_page()
*                   emulate_output_page()
*                   emulate_wgml()
*
* Notes:        The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
*               This program uses/tests the binary device library to output
*               a text file. As such, all structs and field names refer to
*               those in "copfiles.h", not the research code.
****************************************************************************/
 
#define __STDC_WANT_LIB_EXT1__ 1
#include <conio.h>
#include <process.h>
#include <stdbool.h>    // Required, but not included, by wgml.h.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "banner.h"
#include "common.h"
#include "copfiles.h"
#include "findfile.h"
#include "gvars.h"
#include "research.h"
#include "wgml.h"

/* This is used with the text_chars struct to set the initial value of length. */

#define TEXT_START 32

/* Enum definitions. */

typedef enum {
    oc_text,
    oc_bx,
    oc_fig,
} oc_types;

typedef enum {
    tp_text,
    tp_bx,
    tp_fig,
    tp_dbox,
    tp_hline,
    tp_vline,
    tp_redundant_aa
} tp_types;

/* Struct definitions. */

struct test_element; // Forward declaration.

typedef struct {
            uint32_t            v_position;
            uint32_t            depth;
    struct  test_element    *   first;
} tp_bx_element;

typedef struct {
            uint32_t            v_position;
            uint32_t            depth;
    struct  test_element    *   first;
} tp_fig_element;

typedef struct {
    uint32_t            count;
    uint32_t            v_position;
    uint32_t            depth;
    text_line       *   first;
} tp_text_element;

typedef union {
    tp_bx_element       tp_bx;
    tp_fig_element      tp_fig;
    tp_text_element     tp_text;
} tp_element;

typedef struct test_element {
    struct  test_element    *   next;
            uint32_t            v_position;
            tp_types            type;
            tp_element          element;
} test_element;

typedef struct {
    uint32_t            cur_depth;
    uint32_t            max_depth;
    uint32_t            page_bottom;
    uint32_t            page_left;
    uint32_t            page_right;
    uint32_t            page_top;
    test_element    *   first;
} test_page;

typedef struct {
    uint8_t             font_number;
    bool                line_break;
    char            *   text;
} text_phrase;
 
/* Local variables. */
 
/* These are used by more than one function. */

//static  char                tab_char = ' ';
static  test_element    *   oc_test_element_pool    = NULL;
static  test_page           oc_test_page;
static  text_chars      *   oc_text_chars_pool      = NULL;
static  text_line       *   oc_text_line_pool       = NULL;
static  uint8_t             oc_font_number;
static  uint32_t            oc_h_start;
static  uint32_t            oc_v_start;
 
/* Load the usage text array. */
 
static  char    const   *   usage_text[] = {
"Usage:  outcheck defined-name",
"'defined-name' is the defined name for the device to use",
"'defined-name' must be enclosed in quotes if it contains spaces",
NULL
};
 
/* Load the document text arrays. */
 
static  text_phrase         title[] = {
    { 3, false, "OUTCHECK~ Document" },
    { 0, false, NULL}
};
 
static  text_phrase         para1[] = {
    {0, false, "Parts of this document were copied and modified from the Wiki. " },
    {0, false, "The reason for this was to make it less self-referential. " },
    {0, false, "Ironically, we start with some rather self-referential tests " },
    {0, false, "of font switching: the title was in available font 3; most of " },
    {0, false, "the text is in available font 0; here is a phrase in " },
    {1, false, "available font 1" },
    {0, false, " and here is a phrase in " },
    {2, false, "available font 2" },
    {0, false, ", neither including spaces before or behind (the usual " },
    {0, false, "situation). The rarer cases have the highlighted phrase include" },
    {1, false, " the space before" },
    {0, false, " or " },
    {2, false, "the space behind " },
    {0, false, "(which can affect layout if the space widths vary by font) " },
    {0, false, "and, very rarely, perhaps, to start in the mid" },
    {3, false, "dle" },
    {0, false, " of a word." },
    {0, false, NULL }
};
 
#if 0 // Needed until the boxes are ready for testing
 
static  text_phrase         para2[] = {
    {0, false, "This box was created using .bx (and several other control " },
    {0, false, "words) and illustrates one of the many possibilites available: " },
    {0, false, NULL }
};
 
static  text_phrase         bx_box[] = {
    {0, false, "%binary(3)>the appropriate graphic appears" },
    {0, true, "%binary1(4)>the appropriate graphic appears" },
    {0, true, "%binary2(5)>the appropriate graphic appears" },
    {0, true, "%binary4(6)>the appropriate graphic appears" },
    {0, false, NULL }
};
 
static  text_phrase         para3[] = {
    {0, false, "This box was created using :FIG., which is in some ways less " },
    {0, false, "flexible than .bx but which has its advantages: " },
    {0, false, NULL }
};
 
static  text_phrase         fig_box[] = {
    {0, false, "%x_address() returns the current horizontal print position" },
    {0, true, "%y_address() returns the current vertical print position" },
    {0, false, NULL }
};
 
#endif // #if 0
 
/* Local function definitions. */

/* Function emulate_fill_text_lines()
 * This function produces text_line instances from a single input buffer. This
 * may be as simple as adding a few words to an existing text_line or finishing
 * the current text_line and starting another to adding as many as are needed
 * to reach the end of the buffer to the list.
 *
 * Parameters:
 *      input_text contains a pointer to the input buffer.
 *      tl_list contains a pointer to the linked list of text_lines to be used.
 *
 * Globals Used:
 *      oc_h_start contains the horizontal location of the first character.
 *      oc_font contains the font number for input_text.
 *
 * Globals Changed:
 *      oc_h_start contains the horizontal location of the next character after
 *          the last character displayed.
 *
 * Notes:
 *      This function is intended to provide some idea of how input text 
 *          which has been finalized except for input translation and tabbing
 *          might be converted to a linked list of text_line instances. 
 *      The first text_chars of the first text_line will be positioned at the
 *          location given by oc_h_start on entry. The first text_chars of
 *          subsequent text_lines will be positioned at the location given by
 *          page_left. This may or may not be acceptable in production code.
 */
 
static void emulate_fill_text_lines( char * input_text, text_line * tl_list )
{
    static  size_t          increment   = 0;
    static  size_t          spaces      = 0;
    static  text_chars  *   cur_chars   = NULL;
    static  text_chars  *   next_chars  = NULL;
    static  text_chars  *   old_chars   = NULL;
    static  text_line   *   the_line    = NULL;

            char            the_char;
            int             i;
            int             j;
            size_t          count;
            size_t          space_width;
            uint32_t        cur_height;

    /* This seems reasonable, but may turn out to be wrong in wgml. */

    if( input_text == NULL) return;
    if( tl_list == NULL) return;

    /* This depends on tl_list containing no text_char instances when, and
     * only when, a break has occurred. In other words, the calling function
     * must extract all of the text_line instances when a break occurs, and
     * resume processing with a text_line instance with no text_char instances.
     */

    cur_height = 0;
    if( tl_list->first == NULL ) {

        /* If there was a break, tl_list should consist of exactly one
         * text_line instance. 
         */

        if( tl_list->next != NULL ) {
            out_msg( "Empty text_line has non-NULL next pointer\n" );
            err_count++;
            g_suicide();
        }

        /* If there was a break, initialize everything. */

        the_line = tl_list;
        the_line->line_height = 0;

        cur_chars = NULL;
        if( oc_text_chars_pool == NULL ) {
            next_chars = (text_chars *) mem_alloc( sizeof( text_chars ) + \
                                                                TEXT_START );
            next_chars->length = TEXT_START;
        } else {
            next_chars = oc_text_chars_pool;
            oc_text_chars_pool = oc_text_chars_pool->next;
        }
        next_chars->next = NULL;
        next_chars->font_number = oc_font_number;
        next_chars->x_address = oc_h_start;
        next_chars->width = 0;
        next_chars->count = 0;
        old_chars = NULL;

    } else {

        /* If there was no break, continue on with the prior state. */    

        /* If there were no spaces found at the end of the last cur_phrase,
         * then next_chars is certainly not an empty text_chars.
         */

        if( spaces != 0 ) {
            if( next_chars->count == 0 ) {
                if( next_chars->font_number != oc_font_number ) {

                    /* next_chars is empty and the new text_phrase has a new
                     * font_number: add next_chars to the_line.
                     */

                    the_line = tl_list;
                    if( the_line->first == NULL ) {
                        the_line->first = next_chars;
                    } else {
                        cur_chars->next = next_chars;
                    }
                    old_chars = cur_chars;
                    cur_chars = next_chars;

                    /* Adjust the line_height, if appropriate. */

                    cur_height = wgml_fonts[cur_chars->font_number].line_height;
                    if( the_line->line_height < cur_height ) \
                                            the_line->line_height = cur_height;

                    /* Update oc_h_start and get the next text_chars instance. */

                    oc_h_start += increment;
                    if( oc_text_chars_pool == NULL ) {
                        next_chars = (text_chars *) mem_alloc( \
                                        sizeof( text_chars ) + TEXT_START );
                        next_chars->length = TEXT_START;
                    } else {
                        next_chars = oc_text_chars_pool;
                        oc_text_chars_pool = oc_text_chars_pool->next;
                    }
                    next_chars->next = NULL;
                    next_chars->font_number = oc_font_number;
                    next_chars->x_address = oc_h_start;
                    next_chars->width = 0;
                    next_chars->count = 0;
                }
            }
        }
    }

    /* Now process input_text. Note that the for-loop indexes are very
     * useful, albeit not strictly necessary with a null-terminated string.
     */

    spaces = 0;
    count = strlen( input_text );
    for( i = 0; i < count; i++ ) {

        /* Ensure that next_chars points to a text_chars instance. */

        if( next_chars == NULL ) {
            if( oc_text_chars_pool == NULL ) {
                next_chars = (text_chars *) mem_alloc( sizeof( text_chars ) + \
                                                                TEXT_START );
                next_chars->length = TEXT_START;
            } else {
                next_chars = oc_text_chars_pool;
                oc_text_chars_pool = oc_text_chars_pool->next;
            }
            next_chars->next = NULL;
            next_chars->font_number = oc_font_number;
            next_chars->x_address = oc_h_start;
            next_chars->width = 0;
            next_chars->count = 0;
        }

        /* Count any spaces. */

        for( j = i; j < count; j++ ) {
            if( !isspace( input_text[j] ) ) break;
            spaces++;
        }
        i = j;

        /* Compute space_width and adjust x_address and oc_h_start. */

        if( spaces == 0 ) {
            space_width = 0;
        } else {
            space_width = spaces * wgml_fonts[oc_font_number].spc_width;
            oc_h_start += space_width;
            next_chars->x_address = oc_h_start;
        }

        /* The remaining code in the loop has this effect:
         *   When the end of the text_phrase has not been reached, then
         * the next text_chars inserted into the_line will start at a
         * position which includes the value of space_width.
         *   When the end of the text_phrase has been reached, an empty
         * text_chars, which includes the value of space_width, is
         * inserted into the_line.
         */

        /* This loop processes a single text_chars. */

        for( j = i; j < count; j++ ) {

            if( isspace( input_text[j] ) ) break;

            the_char = input_text[j];
 
            if( ProcFlags.in_trans ) {
                if( in_esc == the_char ) {
                    j++;
                    the_char = cop_in_trans( input_text[j], oc_font_number );
                }
            }
            if( next_chars->count >= next_chars->length ) {
                next_chars->length *= 2;
                next_chars = (text_chars *) mem_realloc( next_chars, \
                                sizeof( text_chars ) + next_chars->length );
            }
            next_chars->text[next_chars->count] = the_char;
            next_chars->count++;
        }

        /* Ensure that, when the top of the loop is reached, i will have
         * the correct value after it is incremented.
         */

        i = j - 1;

        if( next_chars->count == 0 ) {
            next_chars->width = 0;
        } else {
            next_chars->width = cop_text_width( next_chars->text, \
                                next_chars->count, next_chars->font_number );
        }
        increment = next_chars->width;

        /* Update text_line. */

        if( (oc_h_start + increment) > oc_test_page.page_right ) {

            /* The line is full: next_chars will start the next line. */

            /* If cur_chars is an empty text_chars, strip it out. Otherwise,
             * set it to NULL so it will not be altered below, as it is still
             * the last text_chars in the current line.
             */

            if( cur_chars->count == 0 ) {
                old_chars->next = NULL;
            } else {
                cur_chars = NULL;
                old_chars = NULL;
            }

            /* The text_line is finished; add another to the list. */

            if( oc_text_line_pool == NULL ) {
                the_line->next = (text_line *) mem_alloc( sizeof( text_line ) );
            } else {
                the_line->next = oc_text_line_pool;
                oc_text_line_pool = oc_text_line_pool->next;
            }
            the_line = the_line->next;
            the_line->next = NULL;
            the_line->line_height = 0;
            the_line->y_address = 0;
            the_line->first = NULL;

            /* Reset oc_h_start. */

            oc_h_start = oc_test_page.page_left;

            /* If cur_chars is not NULL and does not have the same font
             * as next_chars, use it as the first text_chars in the_line.
             * Otherwise, return the text_chars to the pool, as it will
             * not be used.
             */

            if( cur_chars != NULL ) {
                if( cur_chars->font_number != next_chars->font_number ) {
                    the_line->first = cur_chars;
                    cur_chars->x_address = oc_h_start;

                    /* Adjust the line_height, if appropriate. */

                    cur_height = wgml_fonts[cur_chars->font_number].line_height;
                    if( the_line->line_height < cur_height ) \
                                            the_line->line_height = cur_height;
                } else {
                    cur_chars->next = oc_text_chars_pool;
                    oc_text_chars_pool = cur_chars;
                    cur_chars = NULL;
                    old_chars = NULL;
                }

            }

            /* Now add next_chars. */

            if( the_line->first == NULL ) {
                the_line->first = next_chars;
            } else {
                next_chars->next = next_chars;
            }
            old_chars = cur_chars;
            cur_chars = next_chars;
            cur_chars->x_address = oc_h_start;

            /* Adjust the line_height, if appropriate. */

            cur_height = wgml_fonts[cur_chars->font_number].line_height;
            if( the_line->line_height < cur_height ) \
                                            the_line->line_height = cur_height;
        } else {

            /* If next_chars is empty, exit the loop. */

            if( next_chars->count == 0 ) break;

            /* Ensure that the_line is not NULL. */

            if( the_line == NULL ) {
                if( oc_text_line_pool == NULL ) {
                    the_line = (text_line *) mem_alloc( sizeof( text_line ) );
                } else {
                    the_line = oc_text_line_pool;
                    oc_text_line_pool = oc_text_line_pool->next;
                }
                the_line->next = NULL;
                the_line->line_height = 0;
                the_line->y_address = 0;
                the_line->first = NULL;
            }

            /* The text_chars belongs to this text_line. */

            if( the_line->first == NULL ) {
                the_line->first = next_chars;
            } else {
                cur_chars->next = next_chars;
            }
            old_chars = cur_chars;
            cur_chars = next_chars;

            /* Adjust the line_height, if appropriate. */

            cur_height = wgml_fonts[cur_chars->font_number].line_height;
            if( the_line->line_height < cur_height ) \
                                            the_line->line_height = cur_height;
        }

        /* Update oc_h_start and get the next text_chars instance. */

        oc_h_start += increment;
        next_chars = NULL;
#if 0
        if( oc_text_chars_pool == NULL ) {
            next_chars = (text_chars *) mem_alloc( sizeof( text_chars ) + \
                                                                TEXT_START );
            next_chars->length = TEXT_START;
        } else {
            next_chars = oc_text_chars_pool;
            oc_text_chars_pool = oc_text_chars_pool->next;
        }
        next_chars->next = NULL;
        next_chars->font_number = oc_font_number;
        next_chars->x_address = oc_h_start;
        next_chars->width = 0;
        next_chars->count = 0;
#endif
        /* This ensures that spaces will be 0 if the loop is exited 
         * because the current phrase ended without a space character.
         * Genuinely-empty text_chars trigger a break above this and
         * so will be processed properly.
         */

        spaces = 0;
    }

    return;
}

/* Function emulate_input_source()
 * This function prepares a tp_text_element instance containing the text
 * from a text_phrase array.
 *
 * Parameters:
 *      in_text points to the text_phrase array to be processed.
 *      out_text points to the tp_text_element instance to use.
 *
 * Global Changed:
 *      oc_font_number is set to the font_number of the last text_phrase
 *          processed.
 *
 * Notes:
 *      This function is intended to provide some idea of how function
 *          emulate_fill_text_lines() might be used to capture text for use
 *          in page layout.
 *      This function only recognizes two types of input record ending
 *          events: breaking and non-breaking. Unfortunately, the methods
 *          used to recognize these events bear no relation to the methods
 *          wgml will use.
 *      The fields out_text->depth and out_text->count must have the correct
 *          values for any existing content of out_text. Given that as a
 *          prerequisite, these fields will be updated to reflect the new 
 *          content added to out_text.
 */
 
static void emulate_input_source( text_phrase * in_text, \
                                  tp_text_element * out_text )
{
    text_line   *   cur_lines   = NULL;
    text_line   *   next_line   = NULL;
    text_line   *   temp_line   = NULL;
    text_phrase *   cur_phrase  = NULL;

    /* Set up variables/fields. */

    if( oc_text_line_pool == NULL ) {
        cur_lines = (text_line *) mem_alloc( sizeof( text_line ) );
    } else {
        cur_lines = oc_text_line_pool;
        oc_text_line_pool = oc_text_line_pool->next;
    }
    cur_lines->next = NULL;
    cur_lines->line_height = 0;
    cur_lines->y_address = 0;
    cur_lines->first = NULL;
    next_line = out_text->first;
    if( next_line != NULL ) \
                while( next_line->next != NULL) next_line = next_line->next;

    /* Process all the text_phrases in in_text. */

    cur_phrase = in_text;

    /* A NULL text pointer indicates a break. */

    while( cur_phrase->text != NULL ) {

        /* Convert the current buffer. */

        if( cur_phrase->font_number >= wgml_font_cnt ) cur_phrase->font_number = 0;
        oc_font_number = cur_phrase->font_number;

        emulate_fill_text_lines( cur_phrase->text, cur_lines);

        /* Capture the whole lines, leaving the last (presumably partial)
         * line in cur_lines for the next pass through the loop.
         */

        if( cur_lines != NULL ) {
            while( cur_lines->next != NULL ) {
                temp_line = cur_lines;
                cur_lines = cur_lines->next;
                temp_line->next = NULL;
                if( next_line == NULL ) {
                    out_text->first = temp_line;
                    next_line = out_text->first;
                } else {
                    next_line->next = temp_line;
                    next_line = next_line->next;
                }
                out_text->count++;
                out_text->depth += next_line->line_height;
            }
        }

        /* Go to the next text_phrase. This emulates a non-break input record
         * terminator.
         */
 
        cur_phrase++;
    }

    /* Capture any remaining lines in cur_lines. */

    while( cur_lines != NULL ) {
        temp_line = cur_lines;
        cur_lines = cur_lines->next;
        temp_line->next = NULL;
        if( next_line == NULL ) {
            out_text->first = temp_line;
            next_line = out_text->first;
        } else {
            next_line->next = temp_line;
        }
        out_text->count++;
        out_text->depth += next_line->line_height;
        next_line = next_line->next;
        if( next_line != NULL ) next_line->next = NULL;
    }

    return;
}

/* Function emulate_output_page()
 * This function sends the current page to the output device. It also returns
 * the text_line and text_chars instances to their respective pools after they
 * have been sent to the output device.
 *
 * Parameters:
 *
 * Globals Used:
 *      oc_v_start, which must be set to the position of the first line.
 *      oc_test_page is sent to the output device.
 *
 * Globals Changed:
 *      oc_v_start will be set to the position of the last item printed. 
 *      oc_test_page be set correctly for the next page.
 *
 * Notes:
 *      This function is intended to provide some idea of how a page might
 *          be output in wgml; the actual code would probably need to be more
 *          complicated, since very little of wgml's capabilities are modeled
 *          here.
 */
 
static void emulate_output_page( void )
{
    test_element    *   cur_elem    = NULL;
    test_element    *   next_elem   = NULL;
    test_element    *   save_elem   = NULL;
    text_chars      *   pool_ptr    = NULL;
    text_line       *   cur_line    = NULL;
    text_line       *   last_line   = NULL;
    text_line       *   next_line   = NULL;
    uint32_t            cur_depth;
    uint32_t            line_count;

    /* Set the variables. */

    cur_depth = 0;
    cur_elem = oc_test_page.first;

    while( cur_elem != NULL ) {
        switch( cur_elem->type ) {
        case tp_text:

            if( (cur_depth + cur_elem->element.tp_text.depth) > \
                                                    oc_test_page.max_depth ) {

                /* Determine which line is the last to fit on this page. */

                cur_line = cur_elem->element.tp_text.first;

                /* This seems reasonable -- but is it? */

                if( cur_line == NULL ) {
                    out_msg( "No text_lines found in tp_text_element\n" );
                    cur_elem = cur_elem->next;
                    continue;
                }

                /* This would be a good place to check for "widows". The
                 * thought is that line_count and cur_depth can adjusted as
                 * needed to move lines to the next page to prevent a "widow".
                 */

                last_line = NULL;
                line_count = 0;
                while( cur_line != NULL ) {
                    if( (cur_depth + cur_line->line_height) > \
                                                oc_test_page.max_depth ) break;
                    line_count++;
                    cur_depth += cur_line->line_height;
                    last_line = cur_line;
                    cur_line = cur_line->next;
                }

                if( cur_line == NULL ) {
                    
                    /* If cur_line is NULL, then all lines will fit. While
                     * not an error as such, it should still never happen.
                     */

                    out_msg( "All text_lines fit, unexpectedly. \n" );
                } else {

                    /* Split this element to fit part of it onto the page. */

                    /* Initialize a new text_element. */

                    if( oc_test_element_pool == NULL ) {
                        save_elem = (test_element *) \
                                        mem_alloc( sizeof( test_element ) );
                    } else {
                        save_elem = oc_test_element_pool;
                        oc_test_element_pool = oc_test_element_pool->next;
                    }
                    save_elem->next = cur_elem->next;
                    save_elem->v_position = 0;
                    save_elem->type = tp_text;
                    save_elem->element.tp_text.count = 0;
                    save_elem->element.tp_text.v_position = 0;
                    save_elem->element.tp_text.depth = 0;
                    save_elem->element.tp_text.first = cur_line;

                    cur_elem->next = NULL;
                    last_line->next = NULL;

                    /* Update cur_elem with to reflect its new contents. */

                    cur_elem->element.tp_text.count = line_count;
                    cur_elem->element.tp_text.depth = cur_depth;

                    /* Update save_elem to reflect its contents. cur_line
                     * still heads the linked list of text_line instances
                     * in save_elem.
                     */

                    while( cur_line != NULL) {
                        save_elem->element.tp_text.count++;
                        save_elem->element.tp_text.depth += cur_line->line_height;
                        cur_line = cur_line->next;
                    }
                }
            }
            cur_depth += cur_elem->element.tp_text.depth;

            /* Now all lines in cur_elem will fit on the current page.*/

            cur_line = cur_elem->element.tp_text.first;
            while( cur_line != NULL ) {

                if( bin_driver->y_positive == 0x00 ) {
                    oc_v_start -= cur_line->line_height;
                } else {
                    oc_v_start += cur_line->line_height;
                }
                cur_line->y_address = oc_v_start;

                fb_output_textline( cur_line );

                /* Return the text_chars instances to the text_chars pool. */

                if( oc_text_chars_pool == NULL ) {
                    oc_text_chars_pool = cur_line->first;
                } else {
                    pool_ptr = oc_text_chars_pool;
                    while( pool_ptr->next != NULL) pool_ptr = pool_ptr->next;
                    pool_ptr->next = cur_line->first;
                }
                cur_line->first = NULL;

                /* Return the cur_line to oc_text_line_pool. */

                next_line = cur_line->next;
                if( oc_text_line_pool == NULL ) {
                    oc_text_line_pool = cur_line;
                    cur_line->next = NULL;
                } else {
                    cur_line->next = oc_text_line_pool;
                    oc_text_line_pool = cur_line;
                }
                cur_line = next_line;
                cur_elem->element.tp_text.first = cur_line;
            }
            break;
        case tp_bx:
            /* Implementation: to be done. */
            break;
        case tp_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect test_element\n" );
            err_count++;
            g_suicide();
        }

        /* Return the cur_elem to oc_test_element_pool. */

        next_elem = cur_elem->next;
        if( oc_test_element_pool == NULL ) {
            oc_test_element_pool = cur_elem;
            cur_elem->next = NULL;
        } else {
            cur_elem->next = oc_test_element_pool;
            oc_test_element_pool = cur_elem;
        }
        cur_elem = next_elem;
        oc_test_page.first = cur_elem;
    }

    /* Reconfigure oc_test_page as the next page. */

    oc_test_page.cur_depth = 0;
    oc_test_page.first = save_elem;
    oc_v_start = oc_test_page.page_top;
    cur_elem = oc_test_page.first;
    while( cur_elem != NULL ) {
        switch( cur_elem->type ) {
        case tp_text:
            oc_test_page.cur_depth += cur_elem->element.tp_text.depth;
            break;
        case tp_bx:
            oc_test_page.cur_depth += cur_elem->element.tp_bx.depth;
            break;
        case tp_fig:
            oc_test_page.cur_depth  += cur_elem->element.tp_fig.depth;
            break;
        default:
            out_msg( "outcheck internal error: incorrect test_element\n" );
            err_count++;
            g_suicide();
        }
        cur_elem = cur_elem->next;
    }       

    return;
}

/* Function emulate_layout_page()
 * This function obtains and adds page elements to the current document page.
 * If that page is ready for output, this function will also output it and
 * do initialize a new page.
 *
 * Parameters:
 *      el_type encodes the element type to be added to the page.
 *      input_text points to the text_phrase array to be processed.
 *
 * Globals Used:
 *      oc_test_page is the page being used.
 *
 * Globals Changed:
 *      oc_test_page will have the new element added and may also have been
 *          sent to the output device, leaving it with those elements
 *          carried over to the next page to be output.
 *
 * Notes:
 *      This function is intended to provide some idea of how a page might
 *          be set up by wgml; however, the actual code would probably need
 *          to be quite different, since it would be driven by control words
 *          and tags. 
 */
 
static void emulate_layout_page( text_phrase * input_text, oc_types el_type )
{
    test_element    *   cur_elem    = NULL;
    test_element    *   next_elem   = NULL;

    next_elem = oc_test_page.first;
    if( next_elem != NULL) {
        while( next_elem->next != NULL) next_elem = next_elem->next;
    }

    switch( el_type ) {
    case oc_text:

        /* Initialize a new text_element. */

        if( oc_test_element_pool == NULL ) {
            cur_elem = (test_element *) mem_alloc( sizeof( test_element ) );
        } else {
            cur_elem = oc_test_element_pool;
            oc_test_element_pool = oc_test_element_pool->next;
        }
        cur_elem->next = NULL;
        cur_elem->v_position = 0;
        cur_elem->type = tp_text;
        cur_elem->element.tp_text.count = 0;
        cur_elem->element.tp_text.v_position = 0;
        cur_elem->element.tp_text.depth = 0;
        cur_elem->element.tp_text.first = NULL;

        /* Put input_text into cur_elem and add it to oc_test_page. */

        emulate_input_source( input_text, &cur_elem->element.tp_text );
        oc_test_page.cur_depth += cur_elem->element.tp_text.depth;
        if( oc_test_page.first == NULL ) {
            oc_test_page.first = cur_elem;
            next_elem = oc_test_page.first;
        } else {
            next_elem->next = cur_elem;
            next_elem = next_elem->next;
        }
        next_elem->next = NULL;

        break;
    case oc_bx:
        /* Implementation: to be done. */
        break;
    case oc_fig:
        /* Implementation: to be done. */
        break;
    default:
        out_msg( "outcheck internal error: incorrect element type\n" );
        err_count++;
        g_suicide();
    }

    while( oc_test_page.cur_depth > oc_test_page.max_depth ) {
        emulate_output_page();
        fb_document_page();
    }

    return;
}

/* Function emulate_wgml().
 * This function loads the binary device library using the same code as wgml,
 * and then produces an text image output file using the device specified on
 * the command line. No parsing is done: the text is entirely static and the
 * assignment of words to lines etc is done manually; only the actual output
 * is being tested. 
 *
 * Notes:
 *      This code only allows for the :PAGEADDRESS block y_positive to have
 *          different values in different drivers. No known drivers have any
 *          value for x_positive other than "1" ("yes"). 
 */
 
static void emulate_wgml( void )
{
    int                 i;
    test_element    *   te_pool_ptr = NULL;
    text_chars      *   tc_pool_ptr = NULL;
    text_line       *   tl_pool_ptr = NULL;
    uint32_t            oc_h_len;
    uint32_t            oc_v_len;
    uint32_t            line_height_zero;
    uint32_t            max_char_width;
    uint32_t            net_page_height;
    uint32_t            net_page_width;

    /* Set the file-level globals. */

    /* Initialize the test_element pool to hold ten instances. These are
     * tp_text_elements because that is the test_element used most often.
     */

    /* Initialize the text_chars pool to hold 20 instances. */

    oc_text_chars_pool = \
                (text_chars *) mem_alloc( sizeof( text_chars ) + TEXT_START );
    oc_text_chars_pool->next = NULL;
    oc_text_chars_pool->font_number = 0;
    oc_text_chars_pool->x_address = 0;
    oc_text_chars_pool->width = 0;
    oc_text_chars_pool->count = 0;
    oc_text_chars_pool->length = TEXT_START;
    tc_pool_ptr = oc_text_chars_pool;
    for( i = 0; i < 19; i++ ) {
        tc_pool_ptr->next = \
                (text_chars *) mem_alloc( sizeof( text_chars ) + TEXT_START );
        tc_pool_ptr = tc_pool_ptr->next;
        tc_pool_ptr->next = NULL;
        tc_pool_ptr->font_number = 0;
        tc_pool_ptr->x_address = 0;
        tc_pool_ptr->width = 0;
        tc_pool_ptr->count = 0;
        tc_pool_ptr->length = TEXT_START;
    }

    /* Initialize the text_line pool to hold ten instances. */

    oc_text_line_pool = (text_line *) mem_alloc( sizeof( text_line ) );
    oc_text_line_pool->next = NULL;
    oc_text_line_pool->line_height = 0;
    oc_text_line_pool->y_address = 0;
    oc_text_line_pool->first = 0;
    tl_pool_ptr = oc_text_line_pool;
    for( i = 0; i < 9; i++ ) {
        tl_pool_ptr->next = (text_line *) mem_alloc( sizeof( text_line ) );
        tl_pool_ptr = tl_pool_ptr->next;
        tl_pool_ptr->next = NULL;
        tl_pool_ptr->line_height = 0;
        tl_pool_ptr->y_address = 0;
        tl_pool_ptr->first = NULL;
    }

    oc_test_element_pool = (test_element *) mem_alloc( sizeof( test_element ) );
    oc_test_element_pool->next = NULL;
    oc_test_element_pool->v_position = 0;
    oc_test_element_pool->type = tp_text;
    oc_test_element_pool->element.tp_text.count = 0;
    oc_test_element_pool->element.tp_text.v_position = 0;
    oc_test_element_pool->element.tp_text.depth = 0;
    oc_test_element_pool->element.tp_text.first = NULL;
    te_pool_ptr = oc_test_element_pool;
    for( i = 0; i < 9; i++ ) {
        te_pool_ptr->next = (test_element *) \
                                        mem_alloc( sizeof( test_element ) );
        te_pool_ptr = te_pool_ptr->next;
        te_pool_ptr->next = NULL;
        te_pool_ptr->v_position = 0;
        te_pool_ptr->type = tp_text;
        te_pool_ptr->element.tp_text.count = 0;
        te_pool_ptr->element.tp_text.v_position = 0;
        te_pool_ptr->element.tp_text.depth = 0;
        te_pool_ptr->element.tp_text.first = NULL;
    }

    /* Set up oc_test_page. */

    oc_test_page.cur_depth = 0;
    oc_test_page.first = 0;

    /* These would normally be set per the :LAYOUT. Here, half-inch
     * top, bottom and right margins and a one-inch left margin are
     * applied to an 8-1/2" x 11" page.
     */

    if( bin_driver->y_positive == 0 ) {
        oc_test_page.page_top = 10 * bin_device->vertical_base_units;
        oc_test_page.page_bottom = bin_device->vertical_base_units / 2;
        oc_test_page.max_depth = oc_test_page.page_top - oc_test_page.page_bottom;
    } else {
        oc_test_page.page_top = bin_device->vertical_base_units / 2;
        oc_test_page.page_bottom = 10 * bin_device->vertical_base_units;
        oc_test_page.max_depth = oc_test_page.page_bottom - oc_test_page.page_top;
    }

    oc_test_page.page_left = bin_device->horizontal_base_units;
    oc_test_page.page_right = (8 * bin_device->horizontal_base_units) - \
                                    (bin_device->horizontal_base_units / 2);

    /* Set the variables. */

    line_height_zero = wgml_fonts[0].line_height;
    max_char_width = 0;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( max_char_width < wgml_fonts[i].default_width ) \
            max_char_width = wgml_fonts[i].default_width;
    }

    net_page_height = bin_device->page_depth;
    net_page_width = bin_device->page_width;

    /* The OUTCHECK Test Document. */

    /* Allow input translation tests. */

    cop_ti_table( "set ~", 5 );

    /* First pass processing. */
    /* START processing.*/

    fb_start();

    /* DOCUMENT processing.*/

    fb_document();

    /* Last pass processing. */

    /* Title page. */ 

    /* This centers the title horizontally. The horizontal title position
     * would normally be set per the :LAYOUT.
     */

    {
        uint32_t    page_width;
        uint32_t    title_width;

        page_width = oc_test_page.page_right - oc_test_page.page_left;
        title_width = cop_text_width( title[0].text, sizeof( title[0].text ), \
                                                        title[0].font_number );
                                                       
        oc_h_start = oc_test_page.page_left + ((page_width - title_width) / 2);
    }

    /* This places the title part-way down the page, something
     * which would normally be set per the :LAYOUT. The computation is: 14
     * line gap from the top of the page, plus one line for the line the
     * text is to appear on. 
     */
    
    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start = oc_test_page.page_top - (15 * line_height_zero);
    } else {
        oc_v_start = oc_test_page.page_top + (15 * line_height_zero);
    }

    fb_position( oc_h_start, oc_v_start );

    /* Output the Title. */

    emulate_layout_page( title, oc_text );

    /* Force the final page out. */

    emulate_output_page();

    /* Document Section. */

    /* Margin/indent setup. */

    /* One-inch margin on line 1. */

    oc_h_start = oc_test_page.page_left;
    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start = oc_test_page.page_top - line_height_zero;
    } else {
        oc_v_start = oc_test_page.page_top + line_height_zero;
    }

    /* Do the new section processing. */

    fb_new_section( oc_v_start );

    /* One-half inch paragraph indent. */

    oc_h_start += bin_device->horizontal_base_units / 2;

    /* Output the first paragraph. */

    emulate_layout_page( para1, oc_text );

    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );
    oc_h_start = oc_test_page.page_left + bin_device->horizontal_base_units / 2;
    emulate_layout_page( para1, oc_text );

#if 0 // restore when boxing is done again and blank-line-insertion is handled
    /* A blank line is placed between paragraphs. */

    /* Output the second paragraph. */

    oc_h_start = oc_test_page.page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start -= 2 * line_height_zero;
    } else {
        oc_v_start += 2 * line_height_zero;
    }
    emulate_layout_page( para2, oc_text );
#endif
    /* First box. */

    oc_h_len = bin_device->horizontal_base_units;
    oc_h_start = bin_device->horizontal_base_units;
    oc_v_len = 2 * max_char_width;
#if 0 // move to within the bx_element including the above three lines
    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start -= 2 * line_height_zero;
    } else {
        oc_v_start += 2 * line_height_zero;
    }

    if( has_aa_block ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->hline.text != NULL ) {
                fb_hline( oc_h_start, oc_v_start, oc_h_len );
                fb_hline( oc_h_start, oc_v_start - oc_v_len, oc_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
                oc_v_start -= oc_v_len;
                fb_vline( oc_h_start, oc_v_start, oc_v_len );
                fb_vline( oc_h_start + oc_h_len, oc_v_start, oc_v_len );
                oc_v_start += oc_v_len;
            }
        } else {
            if( bin_driver->hline.text != NULL ) {
                fb_hline( oc_h_start, oc_v_start, oc_h_len );
                fb_hline( oc_h_start, oc_v_start + oc_v_len, oc_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
                oc_v_start += oc_v_len;
                fb_vline( oc_h_start, oc_v_start, oc_v_len );
                fb_vline( oc_h_start + oc_h_len, oc_v_start, oc_v_len );
                oc_v_start -= oc_v_len;
            }
        }
    }
#endif
    /* Output the third paragraph. */

#if 0 // restore when boxing is done again
    oc_h_start = oc_test_page.page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start = net_page_height - line_height_zero;
    } else {
        oc_v_start = line_height_zero;
    }
    emulate_layout_page( para3, oc_text );

    /* Second box. */

    if( bin_driver->y_positive == 0x00 ) {
        oc_v_start -= 2 * line_height_zero;
    } else {
        oc_v_start += 2 * line_height_zero;
    }

    oc_h_start = bin_device->horizontal_base_units;
    if( has_aa_block != NULL ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->dbox.text != NULL ) {
                fb_dbox( oc_h_start, oc_v_start - oc_v_len, oc_h_len, \
                                                                oc_v_len );
            }
        } else {
            if( bin_driver->dbox.text != NULL ) {
                fb_dbox( oc_h_start, oc_v_start + oc_v_len, oc_h_len, \
                                                                oc_v_len );
            }
        }
    }
#endif
    /* Force the final page out. */

    emulate_output_page();

    /* :FINISH block. */

    fb_finish();

    /* Free allocated memory. */

    /* Free the memory held by the test_element pool. */

    if( oc_test_element_pool != NULL ) {
        while( oc_test_element_pool->next != NULL ) {
            te_pool_ptr = oc_test_element_pool;
            oc_test_element_pool = oc_test_element_pool->next;
            switch( te_pool_ptr->type ) {
            case tp_text:

                if( te_pool_ptr->element.tp_text.first != NULL ) {

                    /* Return any text_line instances to the text_line pool. */

                    out_msg( "tp_text_element in pool not empty!\n" );
                    if( oc_text_line_pool == NULL ) {
                        oc_text_line_pool = te_pool_ptr->element.tp_text.first;
                    } else {
                        tl_pool_ptr = te_pool_ptr->element.tp_text.first;
                        while( tl_pool_ptr->next != NULL ) {
                            tl_pool_ptr = tl_pool_ptr->next;
                        }
                        tl_pool_ptr->next = oc_text_line_pool;
                        oc_text_line_pool = te_pool_ptr->element.tp_text.first;
                    }
                    te_pool_ptr->element.tp_text.first = NULL;
                }
            case tp_bx:
                /* Implementation: to be done. */
                break;
            case tp_fig:
                /* Implementation: to be done. */
                break;
            default:
                out_msg( "outcheck internal error: incorrect test_element\n" );
                err_count++;
                g_suicide();
            }
            mem_free( te_pool_ptr );
        }
        mem_free( oc_test_element_pool );
        oc_test_element_pool = NULL;
        te_pool_ptr = NULL;
    }

    /* Free the memory held by the text_line pool. */

    if( oc_text_line_pool != NULL ) {
        while( oc_text_line_pool->next != NULL ) {
            tl_pool_ptr = oc_text_line_pool;
            oc_text_line_pool = oc_text_line_pool->next;
            if( tl_pool_ptr->first != NULL ) {

                /* Return any text_chars instances to the text_chars pool. */

                out_msg( "text_line in pool not empty!\n" );
                if( oc_text_chars_pool == NULL ) {
                    oc_text_chars_pool = tl_pool_ptr->first;
                } else {
                    tc_pool_ptr = tl_pool_ptr->first;
                    while( tc_pool_ptr->next != NULL ) {
                        tc_pool_ptr = tc_pool_ptr->next;
                    }
                    tc_pool_ptr->next = oc_text_chars_pool;
                    oc_text_chars_pool = tl_pool_ptr->first;
                }
                tl_pool_ptr->first = NULL;
            }
            mem_free( tl_pool_ptr );
        }
        mem_free( oc_text_line_pool );
        oc_text_line_pool = NULL;
        tl_pool_ptr = NULL;
    }

    /* Free the memory held by the text_chars pool. */

    if( oc_text_chars_pool != NULL ) {
        while( oc_text_chars_pool->next != NULL ) {
            tc_pool_ptr = oc_text_chars_pool;
            oc_text_chars_pool = oc_text_chars_pool->next;
            mem_free( tc_pool_ptr );
        }
        mem_free( oc_text_chars_pool );
        oc_text_chars_pool = NULL;
        tc_pool_ptr = NULL;
    }

    return;
}

/* Global function definitions. */

/* Function print_banner().
 * Print the banner to the screen.
 */

extern void print_banner( void )
{
    puts( banner1w( "Document Output Test Program", _RESEARCH_VERSION_ ) );
    puts( banner2( "1983" ) );
    puts( banner3 );
    puts( banner3a );
}

/* Function print_usage().
 * Print the usage information to the screen.
 */

extern void print_usage( void )
{
    char const * *  list;

    list = usage_text;
    while( *list ) {
        puts( *list );
        ++list;
    }
}

/* Function main().
 * Given a valid defined name, verify that it is a valid .COP file and parse 
 * it if it is.
 *
 * The actual parsing is performed in the function parse_defined_name(); main()
 * is concerned with overall program architecture, not details.
 *
 * Returns:
 *      EXIT_FAILURE or EXIT_SUCCESS, as appropriate.
 */

int main()
{
    char    *   cmdline = NULL;
    int         retval;
    jmp_buf     env;
    size_t      cmdlen  = 0;

null_buffer();
start_heapcheck( "main" );

    /* For compatibility with wgml modules. */

    environment = &env;
    if( setjmp( env ) ) {               // if fatal error has occurred
        my_exit( 16 );
    }

    /* Display the banner. */

    print_banner();

    /* Display the usage information if the command line is empty. */

    cmdlen = _bgetcmd( NULL, 0 );
    if( cmdlen == 0 ) {
        print_usage();
        return( EXIT_FAILURE );
    }

    /* Include space for the terminating null character. */

    cmdlen++;

    /* Get the command line. */

    cmdline = malloc( cmdlen );
    if( cmdline == NULL ) {
        return( EXIT_FAILURE );
    }

    cmdlen = _bgetcmd( cmdline, cmdlen );

    /* Initialize the globals. */

    initialize_globals();
    res_initialize_globals();
    init_global_vars();         // wgml globals

    /* Parse the command line: allocates and sets tgt_path. */

    retval = parse_cmdline( cmdline );
    if( retval == FAILURE ) {
        free( cmdline );
        return( EXIT_FAILURE );
    }

    /* Free the memory held by cmdline and reset it. */

    free( cmdline );
    cmdline = NULL;

    dev_name = tgt_path;
    master_fname = "outcheck";
    out_file = NULL;
    out_file_attr = NULL;

    /* Initialize the binary device library. */

    ff_setup();
    cop_setup();

    /* Create the output file. */

    emulate_wgml();

    /* Release the memory allocated. */

    cop_teardown();
    ff_teardown();

    mem_free(tgt_path);
    tgt_path = NULL;

    free_some_mem();            // wgml globals

end_heapcheck( "main" );
//display_heap( "pre" );
//display_heap( "post" );
    return( EXIT_SUCCESS );
}

