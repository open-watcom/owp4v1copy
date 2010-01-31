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
*                   oc_construct_type
*                   oc_element_type
*                   oc_position
*               and these structs:
*                   oc_bx_element
*                   oc_column
*                   oc_element
*                   oc_fig_element
*                   oc_page
*                   oc_text_element
*                   text_phrase
*               and these variables (the text arrays are not listed):
*                   oc_break
*                   oc_cur_page
*                   oc_cur_post_skip
*                   oc_document_start
*                   oc_element_pool
*                   oc_font_number
*                   oc_indent
*                   oc_is_paragraph
*                   oc_line_height_zero
*                   oc_max_depth
*                   oc_new_element
*                   oc_new_page
*                   oc_new_section
*                   oc_next_column
*                   oc_old_post_skip
*                   oc_page_left
*                   oc_page_right
*                   oc_page_top
*                   oc_pre_skip
*                   oc_pre_top_skip
*                   oc_script
*                   oc_tab_char
*                   oc_text_chars_pool
*                   oc_text_line_pool
*               and these local functions:
*                   emulate_input_source()
*                   emulate_layout_page()
*                   emulate_wgml()
*                   oc_add_text_chars_to_pool()
*                   oc_add_text_line_to_pool()
*                   oc_alloc_oc_element()
*                   oc_alloc_text_chars()
*                   oc_alloc_text_line()
*                   oc_intrans()
*                   oc_output_page()
*                   oc_process_line_full()
*                   oc_process_text()
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
    oc_bx_box,
    oc_fig_box,
    oc_paragraph,
    oc_title_text,
} oc_construct_type;

typedef enum {
    oc_bx,
    oc_dbox,
    oc_fig,
    oc_hline,
    oc_redundant_aa,
    oc_text,
    oc_vline,
} oc_element_type;

typedef enum {
    oc_center,
    oc_left,
    oc_none,
    oc_right,
} oc_position;

/* Struct definitions. */

struct oc_element; // Forward declaration.

typedef struct {
            uint32_t            depth;
    struct  oc_element    *   first;
} oc_bx_element;

typedef struct {
            uint32_t            depth;
    struct  oc_element    *   first;
} oc_fig_element;

typedef struct {
    uint32_t            count;
    uint32_t            depth;
    text_line       *   first;
} oc_text_element;

typedef struct oc_element {
    struct  oc_element      *   next;
            uint32_t            indent;
            uint32_t            net_skip;
            oc_position         line_position;
            oc_element_type     type;
    union {
            oc_bx_element       oc_bx;
            oc_fig_element      oc_fig;
            oc_text_element     oc_text;
    } element;
} oc_element;

typedef struct {
    uint32_t            cur_depth;
    oc_element      *   first;
    oc_element      *   last;
} oc_column;

typedef struct {
    bool                new_section;
    oc_column           column;
} oc_page;

typedef struct {
    uint8_t             font_number;
    bool                line_break;
    char            *   text;
} text_phrase;
 
/* Local variables. */
 
/* These are used by more than one function. */

static  bool                oc_break                = false;
static  bool                oc_document_start       = false;
static  bool                oc_is_paragraph         = false;
static  bool                oc_new_page             = false;
static  bool                oc_new_section          = false;
static  bool                oc_script               = false;
//static  char                oc_tab_char = ' ';
static  oc_element      *   oc_element_pool         = NULL;
static  oc_element      *   oc_new_element          = NULL;
static  oc_column           oc_next_column          = { 0, NULL, NULL };
static  oc_page             oc_cur_page             = { false, { 0, NULL, NULL } };
static  text_chars      *   oc_text_chars_pool      = NULL;
static  text_line       *   oc_text_line_pool       = NULL;
static  uint8_t             oc_font_number          = 0;
static  uint32_t            oc_cur_post_skip        = 0;
static  uint32_t            oc_indent               = 0;
static  uint32_t            oc_line_height_zero     = 0;
static  uint32_t            oc_max_depth            = 0;
static  uint32_t            oc_old_post_skip        = 0;
static  uint32_t            oc_page_left            = 0;
static  uint32_t            oc_page_right           = 0;
static  uint32_t            oc_page_top             = 0;
static  uint32_t            oc_pre_skip             = 0;
static  uint32_t            oc_pre_top_skip         = 0;
 
/* Load the usage text array. */
 
static  char    const   *   usage_text[] = {
"Usage:  outcheck defined-name",
"'defined-name' is the defined name for the device to use",
"'defined-name' must be enclosed in quotes if it contains spaces",
NULL
};
 
/* Load the document text arrays. */
 
static  text_phrase         title[] = {
    { 3, true, "OUTCHECK~ Document" },
    { 0, false, NULL}
};

static  text_phrase         para_font[] = {
    {0, true, "Parts of this document were copied and modified from the Wiki. " },
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
    {0, false, " of a word. " },
    {0, false, NULL }
};
 
static  text_phrase         para_stop[] = {
    {0, true, "Now we get to the test of the stops. Research suggests that " },
    {0, false, "there are four: '.', '!', '?' and '.', at least when the " },
    {0, false, "criterion is that wgml 4.0 puts two spaces after them when " },
    {0, false, "they are used with wscript/noscript in effect! This is surely " },
    {0, false, "worth examining, is it not? It is a bit surprising, however, " },
    {0, false, "to find that neither ',' nor ';' is a stop; still, what " },
    {0, false, "matters here is what wgml 4.0 does. And putting the stop before " },
    {0, false, "a ')' appears to cancel the effect. (Thus, this will be " },
    {0, false, "not be followed by two spaces.) (Nor will this example!) (And " },
    {0, false, "this won't either, will it?) (This one is, of course, rather " },
    {0, false, "unlikely:) in practice. " },
    {0, false, NULL }
};
 
#if 0 // Needed until the boxes are ready for testing
 
static  text_phrase         para2[] = {
    {0, true, "This box was created using .bx (and several other control " },
    {0, false, "words) and illustrates one of the many possibilites available: " },
    {0, false, NULL }
};
 
static  text_phrase         bx_box[] = {
    {0, true, "%binary(3)>the appropriate graphic appears" },
    {0, true, "%binary1(4)>the appropriate graphic appears" },
    {0, true, "%binary2(5)>the appropriate graphic appears" },
    {0, true, "%binary4(6)>the appropriate graphic appears" },
    {0, false, NULL }
};
 
static  text_phrase         para3[] = {
    {0, true, "This box was created using :FIG., which is in some ways less " },
    {0, false, "flexible than .bx but which has its advantages: " },
    {0, false, NULL }
};

static  text_phrase         fig_box[] = {
    {0, true, "%x_address() returns the current horizontal print position" },
    {0, true, "%y_address() returns the current vertical print position" },
    {0, false, NULL }
};
 
#endif // #if 0
 
/* Local function definitions. */

/* Function oc_add_text_chars_to_pool().
 * Returns the text_chars instances of in_line to the pool.
 *
 * Parameter:
 *      in_line points to the text_line to process.
 *
 * Notes:
 *      This function is currently a lightly-adapted version of function
 *          add_text_chars_to_pool() in gproctxt.c.
 *      The original version of this algorithm was based on only one
 *          text_line existing at a time. In that case, most of the
 *          text_chars instances would be in the text_line, so appending the
 *          text_chars from the text_line to the end of the pool made sense.
 *          But now multiple text_lines are in use, and it is more efficient
 *          to prepend each text_line's set of text_chars instances to the
 *          pool. 
 *      This version requires the calling function to set in_line->first to
 *          NULL. It is not entirely clear that this is either good or
 *          necessary.
 */

void oc_add_text_chars_to_pool( text_line * in_line )
{
    text_chars      *   cur_chars;

    if( in_line->first == NULL ) return;

    cur_chars = in_line->first;
    while( cur_chars->next != NULL ) {
        cur_chars = cur_chars->next;
    }
    cur_chars->next = oc_text_chars_pool;

    oc_text_chars_pool = in_line->first;

    return;
}

/* Function oc_add_text_line_to_pool().
 * Returns the text_line instances of in_line to the pool. This function also
 * returns the text_chars instances in each text_line to the pool.
 *
 * Parameter:
 *      in_line points to the linked list of text_line instances to process.
 *
 * Note:
 *      This version requires the calling function to set whatever was
 *          pointing to these text_lines to NULL. This was done in case
 *          something other than a text oc_element is being used. It is
 *          not clear that this is necessary.
 */

void oc_add_text_line_to_pool( text_line * in_line )
{
    text_line      *   cur_line;

    if( in_line == NULL ) return;

    cur_line = in_line;
    while( cur_line->next != NULL ) {
        oc_add_text_chars_to_pool( cur_line );
        cur_line->first = NULL;
        cur_line = cur_line->next;
    }
    oc_add_text_chars_to_pool( cur_line );
    cur_line->first = NULL;

    cur_line->next = oc_text_line_pool;
    oc_text_line_pool = in_line;

    return;
}

/* Function oc_alloc_oc_element().
 * Gets a oc_element from oc_element_pool or allocates a new one; in
 * either case, initializes it. This function has no counterpart in wgml at
 * present.
 *
 * Parameter:
 *      el_type contains the type of oc_element to create.
 *
 * Returns:
 *      A pointer to the text_chars instance.
 */

static oc_element * oc_alloc_oc_element( oc_element_type el_type )
{
    oc_element    *   retval  = NULL;

    if( oc_element_pool == NULL ) {
        retval = (oc_element *) mem_alloc( sizeof( oc_element ) );
    } else {
        retval = oc_element_pool;
        oc_element_pool = oc_element_pool->next;
    }
    retval->next = NULL;
    retval->indent = oc_indent;
    retval->net_skip = 0;
    retval->line_position = oc_none;
    retval->type = el_type;

    switch( el_type ) {
    case oc_text:
        retval->element.oc_text.count = 0;
        retval->element.oc_text.depth = 0;
        retval->element.oc_text.first = NULL;
        break;
    case oc_bx:
    case oc_fig:
    case oc_dbox:
    case oc_hline:
    case oc_vline:
    case oc_redundant_aa:
        break;
    default:
        out_msg( "Unknown oc_element type\n" );
        err_count++;
        g_suicide();
    }
    return( retval );
}

/* Function oc_alloc_text_chars().
 * Gets a text_chars from oc_text_chars_pool or allocates a new one; in either
 * case, initializes it. The parameters used are the same as seen in the wgml
 * version in gproctxt.c, although the details are different. In wgml, the
 * function is called from several different locations, which presumably
 * explains the parameters.
 *
 * Parameters:
 *      in_text points to the text to be controlled by the new instance
 *      count contains the number of characters in in_text
 *      font_number contains the font number for the text_chars
 *
 * Returns:
 *      A pointer to the text_chars instance.
 */

static text_chars * oc_alloc_text_chars(  char * in_text, size_t count,
                                           uint8_t font_number )
{
    uint32_t        size    = TEXT_START;
    text_chars  *   retval  = NULL;

    if( oc_text_chars_pool == NULL ) {
        if( count > TEXT_START ) {
            size = (( count / TEXT_START ) + 1) * TEXT_START;
        }
        retval = (text_chars *) mem_alloc( sizeof( text_chars ) + size );
        retval->length = size;
    } else {
        retval  = oc_text_chars_pool;
        oc_text_chars_pool = oc_text_chars_pool->next;
        if( count > retval->length ) {
            size = (( count / TEXT_START ) + 1) * TEXT_START;
            retval = (text_chars *) mem_realloc( retval, \
                                                sizeof( text_chars ) + size );
            retval->length = size;
        }
    }
    retval->next = NULL;
    retval->font_number = font_number;
    retval->x_address = 0;
    retval->width = 0;

    if( in_text != NULL ) {
        memcpy_s(retval->text, count, in_text, count );
        retval->count = count;
    } else {
        retval->count = 0;
        retval->text[0] = '\0';
    }

    return( retval);
}

/* Function oc_alloc_text_line().
 * Gets a text_line from oc_text_line_pool or allocates a new one; in either
 * case, initializes it.
 *
 * Returns:
 *      A pointer to the text_line instance.
 */

static text_line * oc_alloc_text_line( void )
{
    text_line   *   retval  = NULL;

    if( oc_text_line_pool == NULL ) {
        retval = (text_line *) mem_alloc( sizeof( text_line ) );
    } else {
        retval = oc_text_line_pool;
        oc_text_line_pool = oc_text_line_pool->next;
    }
    retval->next = NULL;
    retval->line_height = 0;
    retval->y_address = 0;
    retval->first = NULL;

    return( retval);
}

/* Function oc_intrans()
 * Applies input translation, if specified, to the text.
 *
 * Parameters:
 *      data contains the text to process.
 *      len contains the number of characters in text.
 *      font contains the font number to use.
 *
 * Parameters Modified:
 *      text will contain the text with input translation applied.
 *      count will contain the number of characters in the translated text.
 *
 * Note:
 *      The function body is an unaltered copy of intrans() in gproctxt.c.
 */

static void oc_intrans( char * data, uint16_t * len, uint8_t font )
{
    char    *   ps;                     // source ptr
    char    *   pt;                     // target ptr
    uint32_t    k;

    if( !ProcFlags.in_trans ) {
        return;                         // input translation not active
    }
    k = *len;
    ps = data;
    pt = data;
    for( k = 0; k <= *len; k++ ) {
        if( *ps == in_esc ) {           // translate needed
            ps++;                       // skip escape char
            k++;                        // and count
            *pt = cop_in_trans( *ps, font );   // translate
            ps++;
            pt++;
        } else {
            *pt++ = *ps++;              // else copy byte
        }
    }

    if( pt < ps ) {                     // something translated
        *len -= (ps - pt);              // new length
        *pt = ' ';
    }

    return;
}

/* Function oc_output_page()
 * This function sends the current page to the output device. It also returns
 * the text_line and text_chars instances to their respective pools after they
 * have been sent to the output device.
 *
 * Parameters:
 *
 * Globals Used:
 *      oc_cur_page.column is sent to the output device.
 *
 * Globals Modified:
 *      oc_cur_page.column will be set correctly for the next page.
 *
 * Notes:
 *      This function is intended to provide some idea of how a page might
 *          be output in wgml; the actual code would probably need to be more
 *          complicated.
 *      This function currently computes the actual vertical position, which
 *          makes sense since it is only when a page is finished that this
 *          can be done without having to redo it if one or more lines must be
 *          moved to the next page. However, that also results in a lot of
 *          other processing being done here that could be done earlier if 
 *          each text_line is output as soon as it is finalized.
 */
 
static void oc_output_page( void )
{
    oc_element      *   cur_elem    = NULL;
    oc_element      *   next_elem   = NULL;
    text_line       *   cur_line    = NULL;
    uint32_t            cur_h_address;
    uint32_t            cur_v_address;
    uint32_t            start_depth;
    uint32_t            start_width;

    /* An empty document section can result in this function being invoked
     * when oc_cur_page.column has no contents. This is not an error; however,
     * starting a new document page would be inappropriate.
     */

    if( oc_cur_page.column.first == NULL ) return;

    cur_elem = oc_cur_page.column.first;
    switch( cur_elem->type ) {
    case oc_text:
        if( cur_elem->element.oc_text.first == NULL ) return;
        cur_line = cur_elem->element.oc_text.first;
        start_depth = (cur_elem->net_skip + 1) * cur_line->line_height;
        break;
    case oc_bx:
        /* Implementation: to be done. */
        break;
    case oc_fig:
        /* Implementation: to be done. */
        break;
    default:
        out_msg( "outcheck internal error: incorrect oc_element\n" );
        err_count++;
        g_suicide();
    }
    oc_new_page = true;

    /* Assign the vertical positions to the various elements. While this could
     * be done in the output loop at present, if more than one column is
     * present on a page both will have to have their vertical positions
     * computed because the text will be printed using text_lines which
     * extend across the entire width of the page and which are formed
     * ad-hoc from all columns taken together.
     */

    while( cur_elem != NULL ) {
        while( cur_line != NULL ) {
            if( oc_document_start ) {

                /* NOTE: This may need adjusting when element types other than
                 * text are in use.
                 */

                /* cur_line is the first line in the document. The vertical
                 * position is the same as top-of-page: pre_skip is ignored.
                 * However, fb_position() must be called, possibly twice, to
                 * finish initializing devfuncs.c.
                 * NOTE: the actual computation of start_depth is a guess at
                 * this point.
                 */

                cur_h_address = oc_page_left;
                if( bin_driver->y_positive == 0x00 ) {
                    cur_v_address = oc_page_top - start_depth;
                } else {
                    cur_v_address = oc_page_top + start_depth;
                }

                /* Set the position. */

                fb_position( cur_h_address, cur_v_address );

                /* If this is a :P. and the horizontal address of the first
                 * text_chars is not oc_page_left (that is, the left margin), so
                 * that an indent exists, call fb_position() to set the indent.
                 * NOTE: Non-:P. paragraphs might have indents, if a control
                 * word exists that produces one. This may result in broadening
                 * the meaning of oc_is_paragraph to include non-:P. paragraphs.
                 */

                start_width = cur_line->first->x_address;
                if( oc_is_paragraph && (start_width > oc_page_left) ) {
                    cur_h_address = start_width;
                    fb_position( cur_h_address, cur_v_address );
                }

            } else {

                if( oc_new_page ) {

                    /* cur_line is the first line on a page (which may or may
                     * not be the first page of a new section, but which is
                     * not the first page in the document).
                     * NOTE: This computation may need tweaking.
                     */

                    if( bin_driver->y_positive == 0x00 ) {
                        cur_v_address = oc_page_top - start_depth;
                    } else {
                        cur_v_address = oc_page_top + start_depth;
                    }

                    /* New section/new page processing. */

                    if( oc_cur_page.new_section ) {
                        oc_cur_page.new_section = false;
                        fb_new_section( cur_v_address );
                    } else {
                        fb_document_page();
                    }

                } else {

                    /* cur_line is not the first line on the page. */

                    if( bin_driver->y_positive == 0x00 ) {
                        cur_v_address -= start_depth;
                    } else {
                        cur_v_address += start_depth;
                    }

                }
            }
            oc_document_start = false;
            oc_new_page = false;
            cur_line->y_address = cur_v_address;
            cur_line = cur_line->next;
            if ( cur_line != NULL ) {
                start_depth = cur_line->line_height;
            }
        }

        cur_elem = cur_elem->next;
        if( cur_elem != NULL ) {
            switch( cur_elem->type ) {
            case oc_text:
                cur_line = cur_elem->element.oc_text.first;
                start_depth = (cur_elem->net_skip + 1) * cur_line->line_height;
                break;
            case oc_bx:
                /* Implementation: to be done. */
                break;
            case oc_fig:
                /* Implementation: to be done. */
                break;
            default:
                out_msg( "outcheck internal error: incorrect oc_element\n" );
                err_count++;
                g_suicide();
            }
        }
    }

    /* Now output the page. This is correct for a page with one column. */

    cur_elem = oc_cur_page.column.first;
    while( cur_elem != NULL ) {
        switch( cur_elem->type ) {
        case oc_text:

            cur_line = cur_elem->element.oc_text.first;
            while( cur_line != NULL ) {
                fb_output_textline( cur_line );
                cur_line = cur_line->next;
            }

            /* Return the text_lines to the pool. */

            oc_add_text_line_to_pool( cur_elem->element.oc_text.first );
            cur_elem->element.oc_text.first = NULL;

            break;
        case oc_bx:
            /* Implementation: to be done. */
            break;
        case oc_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect oc_element\n" );
            err_count++;
            g_suicide();
        }

        /* Return the cur_elem to oc_element_pool. */

        next_elem = cur_elem->next;
        if( oc_element_pool == NULL ) {
            oc_element_pool = cur_elem;
            cur_elem->next = NULL;
        } else {
            cur_elem->next = oc_element_pool;
            oc_element_pool = cur_elem;
        }
        cur_elem = next_elem;
        oc_cur_page.column.first = cur_elem;
    }

    /* Reconfigure oc_cur_page.column as the next page. This must be done
     * every time a page is output, and is best done here.
     */

    oc_cur_page.column.cur_depth = oc_next_column.cur_depth;
    oc_cur_page.column.first = oc_next_column.first;
    oc_cur_page.column.last = oc_next_column.last;
    oc_next_column.cur_depth = 0;
    oc_next_column.first = NULL;
    oc_next_column.last = NULL;

    return;
}

/* Function oc_process_line_full().
 * This function processes lines that are full, either because they do not have
 * enough room for the next word in the input text or because they are followed
 * by a break. This versions will, when implemented, add them to a static
 * global text oc_element and perform other actions.
 *
 * Parameters:
 *      in_line points to the text_line to be processed.
 *      justify provides the ability to prohibit justification even if
 *          justification is on.
 *
 * Note:
 *      Function process_line_full() in gproctxt.c was used as a starting-
 *          point for this function, but this is not a copy.
 */

static void oc_process_line_full( text_line * in_line, bool justify )
{
    static  text_line   *   line_ptr    = NULL;

            oc_element  *   cur_elem    = NULL;
            oc_element  *   save_elem   = NULL;
            text_chars  *   cur_chars   = NULL;
            text_line   *   cur_line    = NULL;
            text_line   *   last_line   = NULL;
            uint32_t        cur_depth   = 0;
            uint32_t        line_count  = 0;
            uint32_t        offset      = 0;
            uint32_t        title_width = 0;

    /* This should be correct. */

    if( in_line == NULL ) return;

    /* As should this. */

    if( in_line->first == NULL ) return;

    cur_elem = oc_cur_page.column.last;

    /* oc_process_text() aligns text_lines as if oc_left were in effect.
     * If oc_left is not in effect, adjustments must be made.
     */

    switch( cur_elem->line_position ) {
    case oc_center:
        switch( cur_elem->type ) {
        case oc_text:

            /* The computation shown produces the same result as wgml 4.0, but
             * not the result described in the Reference Manual. 
             */

            /* Compute the total width of the line, that is, the width of
             * the text that will be output (internal spacing included).
             */

            cur_chars = in_line->first;
            if( cur_chars->next != NULL) {
                while( cur_chars->next != NULL) {
                    cur_chars = cur_chars->next;
                }
            }

            title_width = cur_chars->x_address + cur_chars->width;
            title_width -= in_line->first->x_address;

            offset = (oc_page_right - title_width) / 2;
            offset -= oc_page_left;

            /* Update each text_chars instance. */

            cur_chars = in_line->first;
            while( cur_chars != NULL) {
                cur_chars->x_address += offset;
                cur_chars = cur_chars->next;
            }

            break;
        case oc_bx:
            /* Implementation: to be done. */
            break;
        case oc_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect oc_element_type\n" );
            err_count++;
            g_suicide();
        }
        break;
    case oc_left:
        /* Nothing to do? see below. */
        break;
    case oc_none:

        /* Do nothing; the element does not have the relevent attribute. */

        break;
    case oc_right:
        /* Not implemented yet. */
        break;
    default:
        out_msg( "outcheck internal error: incorrect oc_position\n" );
        err_count++;
        g_suicide();
    }

    /* Note: it is not known how the various tags which have a page_position
     * :LAYOUT attribute interact with justification. It might be as easy as
     * setting justify to "false", or it might require adjustment of the
     * variables controlling how justification is done. 
     */

    /* outcheck.c does not attempt justification; however, it would occur
     * inside the following if() statement if parameter justify is true
     * and it is otherwise called for.
     */

    if( justify ) {}

    /* Add in_line to the current element. */

    if( line_ptr == NULL ) {
        switch( cur_elem->type ) {
        case oc_text:
            cur_elem->element.oc_text.first = in_line;
            break;
        case oc_bx:
            /* Implementation: to be done. */
            break;
        case oc_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect oc_element\n" );
            err_count++;
            g_suicide();
        }
        line_ptr = in_line;
    } else {
        line_ptr->next = in_line;
        line_ptr = line_ptr->next;
    }
    cur_elem->element.oc_text.count++;
    cur_elem->element.oc_text.depth += in_line->line_height;

    /* Break processing. */

    cur_depth = oc_cur_page.column.cur_depth;
    if( oc_break ) {    

        /* Full page processing. */

        line_ptr = NULL;
        cur_depth += cur_elem->net_skip * oc_line_height_zero;
        if( (cur_depth + cur_elem->element.oc_text.depth) > oc_max_depth ) {

            /* Determine which line is the last to fit on this page. */

            cur_line = cur_elem->element.oc_text.first;

            /* cur_line cannot be NULL at this point because we cannot get here
             * unless in_line was non-NULL and was added to oc_cur_page.column.last.
             */

            /* This would be a good place to check for "widows". The
             * thought is that line_count and cur_depth can adjusted as
             * needed to move lines to the next page to prevent a "widow".
             */

            line_count = 0;
            while( cur_line != NULL ) {
                if( (cur_depth + cur_line->line_height) >
                                                oc_max_depth ) break;
                line_count++;
                cur_depth += cur_line->line_height;
                last_line = cur_line;
                cur_line = cur_line->next;
            }

            /* cur_line, as noted, should not be NULL. However, if the entire
             * element must be moved to next_column intact, then last_line
             * will be NULL.
             */

            if( cur_line == NULL ) {
                    
                /* If cur_line is NULL, then all lines will fit. While
                 * not an error as such, it should still never happen.
                 */

                out_msg( "All text_lines fit, unexpectedly. \n" );
            } else {

                if( last_line == NULL ) {

                    /* Transfer cur_elem intact to oc_next_column.
                     * First find the element before cur_elem and have
                     * save_elem point to it.
                     */

                    save_elem = oc_cur_page.column.first;

                    if( save_elem == cur_elem ) {

                        /* This is most unlikely -- at least with text-only. */

                        out_msg( "internal error: page finishing.\n" );
                        err_count++;
                        g_suicide();
                    }

                    while( save_elem->next != cur_elem ) {
                        save_elem = save_elem->next;
                    }
                    save_elem->next = NULL;

                    /* Add cur_elem to oc_next_column and set variables. */

                    if( oc_next_column.first == NULL ) {
                        oc_next_column.first = cur_elem;
                        oc_next_column.last = oc_next_column.first;
                    } else {
                        oc_next_column.last->next = cur_elem;
                        oc_next_column.last = oc_next_column.last->next;
                    }

                    if( oc_next_column.first == oc_next_column.last ) {

                        /* Finalize net_skip for cur_elem.
                         * Since cur_elem is the first element on the page,
                         * only oc_pre_top_skip is used.
                         * oc_old_post_skip is updated for possible use with the
                         * next element, but is not used in finalizing net_skip
                         * because it refers to an element which occurred at the
                         * end of the prior page.
                         * NOTE: this only works for now. When :FIG is done, and
                         * cur_elem may not be the first element on the new
                         * page, some means of recovering the appropriate
                         * post_skip will need to be found.
                         */

                        switch( cur_elem->type ) {
                        case oc_text:
                            cur_elem->net_skip = oc_pre_top_skip;
                            oc_old_post_skip = oc_cur_post_skip;
                            break;
                        case oc_bx:
                            /* Implementation: to be done. */
                            break;
                        case oc_fig:
                            /* Implementation: to be done. */
                            break;
                        default:
                            out_msg( "outcheck internal error: incorrect oc_element\n" );
                            err_count++;
                            g_suicide();
                        }
                    }

                } else {

                    /* Split cur_elem to fit part of it onto the page. */

                    /* Add a new text element to oc_next_column and set
                     * variables.
                     */

                    if( oc_next_column.first == NULL ) {
                        oc_next_column.first = oc_alloc_oc_element( oc_text );
                        oc_next_column.last = oc_next_column.first;
                    } else {
                        oc_next_column.last->next = oc_alloc_oc_element( oc_text );
                        oc_next_column.last = oc_next_column.last->next;
                    }
                    save_elem = oc_next_column.last;
                    cur_elem->next = NULL;
                    last_line->next = NULL;

                    if( oc_next_column.first == oc_next_column.last ) {

                        /* Finalize net_skip for save_elem.
                         * Since save_elem is the first element on the page,
                         * only oc_pre_top_skip is used.
                         * oc_old_post_skip is updated for possible use with the
                         * next element, but is not used in finalizing net_skip
                         * because it refers to an element which occurred at the
                         * end of the prior page.
                         * NOTE: this only works for now. When :FIG is done, and
                         * save_elem may not be the first element on the new
                         * page, some means of recovering the appropriate
                         * post_skip will need to be found.
                         */

                        switch( save_elem->type ) {
                        case oc_text:
                            save_elem->net_skip = oc_pre_top_skip;
                            oc_old_post_skip = oc_cur_post_skip;
                            break;
                        case oc_bx:
                            /* Implementation: to be done. */
                            break;
                        case oc_fig:
                            /* Implementation: to be done. */
                            break;
                        default:
                            out_msg( "outcheck internal error: incorrect oc_element\n" );
                            err_count++;
                            g_suicide();
                        }
                    }

                    /* Update oc_cur_elem to reflect its new contents. */

                    cur_elem->element.oc_text.count = line_count;
                    cur_elem->element.oc_text.depth = cur_depth;

                    /* Update save_elem to reflect its contents. cur_line
                     * still heads the linked list of text_line instances
                     * in save_elem.
                     */

                    save_elem->element.oc_text.first = cur_line;
                    while( cur_line != NULL) {
                        save_elem->element.oc_text.count++;
                        save_elem->element.oc_text.depth += cur_line->line_height;
                        cur_line = cur_line->next;
                    }
                    oc_next_column.cur_depth += save_elem->element.oc_text.depth;
                }

                /* Finalize the current page. */

                oc_cur_page.column.cur_depth += cur_elem->element.oc_text.depth;
            }

            /* Print the current page. This also reinitializes it with the
             * contents of oc_next_column.
             */

            oc_output_page();

        } else {
            oc_cur_page.column.cur_depth += cur_elem->net_skip * oc_line_height_zero;
            oc_cur_page.column.cur_depth += cur_elem->element.oc_text.depth;
        }
    }

    return;
}

/* Function oc_process_text().
 * This function produces text_line instances from a single input buffer. This
 * may be as simple as adding a few words to an existing text_line or finishing
 * the current text_line, calling oc_process_line_full(), and starting another.
 *
 * Parameters:
 *      input_text contains a pointer to the input buffer.
 *      font_number contains the font number to be used.
 *
 * Globals Used:
 *      oc_break indicates whether or not a break occurred before the current
 *          buffer.
 *
 * Globals Changed:
 *      oc_break will be "false" on exit.
 *
 * Notes:
 *      This function is intended to explore methods for converting input text 
 *          which has been finalized except for input translation and tabbing
 *          to a text_line instance. It is intended to mimic features of
 *          parse_text() in gproctxt.c. This version works as if "script" were
 *          given on the command line.
 *      The first text_chars of the first text_line in each oc_element will be
 *          positioned at the location given by oc_page_left plus oc_indent.
 *          The first text_chars of subsequent text_lines in the same
 *          oc_element will be positioned at the location given by page_left.
 */

static void oc_process_text( char * input_text, uint8_t font_number )
{
    static  size_t          increment       = 0;
    static  size_t          spaces          = 0;
    static  uint8_t         old_font        = 0;
    static  uint32_t        cur_h_address   = 0;
    static  text_chars  *   cur_chars       = NULL;
    static  text_chars  *   pre_chars       = NULL;
    static  text_line   *   the_line        = NULL;

            char            ch;
            char        *   token_start     = 0;
            size_t          count           = 0;
            size_t          space_width     = 0;
            text_chars  *   next_chars      = NULL;
            uint32_t        cur_height      = 0;

    /* These invariants should hold on entry:
     * If oc_new_section is true:
     *      It is the start of new document section.
     *      cur_chars should be NULL.
     *      pre_chars should be NULL.
     *      spaces should be 0.
     *      When we start counting tabs, tabs should be 0.
     *      increment should be 0.
     * otherwise, this is the start of a new phrase:
     *      the_line->first points to a linked list of text_chars instances.
     *      cur_chars points to the last text_chars instance in the_line.
     *      pre_chars is null if cur_chars points to the first (and only)
     *          text_chars instance in the_line; otherwise, it points to
     *          the text_chars instance such that pre_chars->next points to
     *          the same text_chars instance as cur_chars.
     *      spaces contains the number of spaces (if any) before this phrase.
     *      tabs, when counted, will do the same for tab characters.
     *      increment should contain the width of next_chars.
     * If input_text is NULL, then oc_break should be true.
     */

    /* Make sure we have a text_line. */

    if( the_line == NULL ) the_line = oc_alloc_text_line();

    /* Detect the start of a new section. */

    if( oc_new_section == true ) {

        /* If there is no input_text, do nothing: this section will be done
         * when input_text is not NULL.
         */

        if( input_text == NULL ) return;

        /* Ensure proper starting values for the new section. */

        cur_chars = NULL;
        pre_chars = NULL;
        cur_h_address = oc_page_left + oc_indent;
        spaces = 0; // when tabs are counted, set tabs to 0 also
        increment = 0;

        /* Enable new section processing in oc_output_page(). */

        oc_cur_page.new_section = true;
        oc_new_section = false;

        /* Finalize net_skip for oc_new_element.
         * Since this is the start of a page, only oc_pre_top_skip is used.
         * oc_old_post_skip is updated for possible use with the next element,
         * but is not used in finalizing net_skip because it refers to an
         * element which occurred at the end of the prior page.
         */

        switch( oc_new_element->type ) {
        case oc_text:
            oc_new_element->net_skip += oc_pre_top_skip;
            oc_old_post_skip = oc_cur_post_skip;
            break;
        case oc_bx:
            /* Implementation: to be done. */
            break;
        case oc_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect oc_element\n" );
            err_count++;
            g_suicide();
        }

        /* Attach oc_new_element to oc_cur_page.column. */

        if( oc_cur_page.column.first == NULL ) {
            oc_cur_page.column.first = oc_new_element;
            oc_cur_page.column.last = oc_cur_page.column.first;
        } else {
            oc_cur_page.column.last->next = oc_new_element;
            oc_cur_page.column.last = oc_cur_page.column.last->next;
        }
        oc_new_element = NULL;

        /* Since the_line is empty, it follows that break processing is not
         * appropriate, even if oc_break is true.
         */

        oc_break = false;
    }

    /* Break processing. */

    if( oc_break ) {

        /* Submit the_line with justification suppressed. */

        oc_process_line_full( the_line, false );

        /* Allocate a new text_line. */

        the_line = oc_alloc_text_line();

        /* If there is no input_text, this was the last line on the last page
         * of a section (or document). There is, then, no point in continuing.
         */

        if( input_text == NULL ) return;

        /* Reset variables for use with the new line. If the above line is
         * triggered, they will be reset when the function is called next
         * as a new section will be starting.
         */

        cur_chars = NULL;
        pre_chars = NULL;
        cur_h_address = oc_page_left + oc_cur_page.column.last->indent;
        spaces = 0; // when tabs are counted, set tabs to 0 also
        increment = 0;

        /* Set the line_height to its initial value. */

        the_line->line_height = oc_line_height_zero;

        /* Finalize net_skip for oc_new_element.
         * Since this is not the start of a page, oc_old_post_skip, oc_pre_skip,
         * and oc_pre_top_skip are used.
         * oc_old_post_skip is updated for possible use with the next element.
         * NOTE: this element's net_skip may be reset later if it turns out to be
         * the first element on a new page.
         */

        switch( oc_new_element->type ) {
        case oc_text:
            oc_new_element->net_skip += oc_old_post_skip;
            oc_new_element->net_skip += oc_pre_skip;
            oc_new_element->net_skip += oc_pre_top_skip;
            oc_old_post_skip = oc_cur_post_skip;
            break;
        case oc_bx:
            /* Implementation: to be done. */
            break;
        case oc_fig:
            /* Implementation: to be done. */
            break;
        default:
            out_msg( "outcheck internal error: incorrect oc_element\n" );
            err_count++;
            g_suicide();
        }

        /* Attach oc_new_element to oc_cur_page.column. */

        if( oc_cur_page.column.first == NULL ) {
            oc_cur_page.column.first = oc_new_element;
            oc_cur_page.column.last = oc_cur_page.column.first;
        } else {
            oc_cur_page.column.last->next = oc_new_element;
            oc_cur_page.column.last = oc_cur_page.column.last->next;
        }
        oc_new_element = NULL;

        /* Break processing is done. */

        oc_break = false;
    }

    /* This should not happen, but it is innocuous. */

    if( input_text == NULL ) return;

    /* At this point, we should be ready to add text_chars to the_line
     * based on the contents of input_text.
     */

    cur_height = 0;

    /* Now process input_text. */

    while( *input_text != '\0' ) {

        /* These invariants should hold at the top of this loop:
         * the_line should never be NULL.
         * If the_line->first is NULL and next_chars is NULL:
         *      It is the start of a new phrase and a new line.
         *      cur_chars should be NULL.
         *      pre_chars should be NULL.
         *      spaces should be 0.
         *      When we start counting tabs, tabs should be 0.
         *      increment should be 0.
         * If the_line->first is NULL and next_chars is not NULL:
         *      this is the start of a new line, but not of a new phrase.
         *      cur_chars should be NULL.
         *      pre_chars should be NULL.
         *      spaces may be 0 or may be > 0.
         *      When we start counting tabs, tabs may be 0 or may be > 0.
         *      increment should be 0.
         * If the_line->first is not NULL but next_chars is NULL:
         *      It is the start of a new phrase, but not of a new line.
         *      cur_chars points to the last text_chars instance in the_line.
         *      pre_chars is null if cur_chars points to the first (and only)
         *          text_chars instance in the_line; otherwise, it points to
         *          the text_chars instance such that pre_chars->next points to
         *          the same text_chars instance as cur_chars.
         *      spaces may be 0 or may be > 0.
         *      When we start counting tabs, tabs may be 0 or may be > 0.
         *      increment should be 0.
         * If the_line->first is not NULL and next_chars is not NULL:
         *      This cannot happen.
         */

        if( oc_script ) {

            /* If "script" was specified, then an empty text_chars will be
             * needed if next_chars is NULL (that is, this is the start of a
             * new phrase) and spaces (and/or, eventually,  tabs) is not zero,
             * and if the font used by the previous phrase differs from that
             * used by the current phrase.
             */

            if( (next_chars == NULL) && (spaces != 0) ) {
                if( old_font != font_number ) {
                    if( the_line->first == NULL ) {

                        /* An empty text_chars at the start of the line is
                         * used for the spacing that establishes the left
                         * margin plus any indent. For now, it uses
                         * oc_page_left.
                         */

                        the_line->first = \
                                    oc_alloc_text_chars( NULL, 0, old_font );
                        cur_chars = the_line->first;
                        pre_chars = NULL;
                        cur_h_address = oc_page_left + \
                                                oc_cur_page.column.last->indent;
                    } else {

                        /* An empty text_chars in the middle of a line is used
                         * for the spacing from the end of the last text_chars
                         * to the start of the new phrase.
                         */

                        pre_chars = cur_chars;
                        cur_chars->next = \
                                    oc_alloc_text_chars( NULL, 0, old_font );
                        cur_chars = cur_chars->next;
                    }
                    cur_chars->x_address = cur_h_address;

                    /* Reset spaces (and, eventually, tabs), and adjust the
                     * line_height if necessary.
                     */

                    spaces = 0;
                    cur_height = wgml_fonts[old_font].line_height;
                    if( the_line->line_height < cur_height ) {
                        the_line->line_height = cur_height;
                    }
                }
            }
        }
        old_font = font_number;

        /* If next_chars is not NULL, add it to the_line.
         */

        if( next_chars != NULL ) {
            if( the_line->first == NULL ) {
                the_line->first = next_chars;
            } else {
                pre_chars = cur_chars;
                cur_chars->next = next_chars;
            }
            cur_chars = next_chars;
            next_chars = NULL;

            /* Adjust the line_height, if appropriate. */

            cur_height = wgml_fonts[font_number].line_height;
            if( the_line->line_height < cur_height ) {
                the_line->line_height = cur_height;
            }

            /* Update cur_h_address to the width of the token added. */

            cur_h_address += cur_chars->width;

            /* Reset spaces (and, eventually, tabs). */

            spaces = 0;
        }

        /* Count any spaces before the next token. */

        while( *input_text != '\0' ) {
            if( !isspace( *input_text ) ) break;
            spaces++;
            input_text++;
        }

        /* Compute space_width and adjust cur_h_address. */

        if( spaces == 0 ) {
            space_width = 0;
        } else {

            if( !oc_script ) { 

                /* If "script" was not specified, then either "wscript" was
                 * specified, "noscript" was specified, or nothing was specified
                 * (which is the same as "noscript") and the space count must be
                 * reduced to "1" ("2" after a stop or half stop) at this point.
                 * NOTE: the list of stops is per actual test with wgml 4.0, but
                 * will be tested further. There is no indication (so far) that
                 * a stop followed by a ")" is also a stop. Note that the text,
                 * at this point, is not being justified; justification may
                 * change things a bit.
                 */

                ch = cur_chars->text[cur_chars->count - 1];
                if( ch == '.' || ch == '!' || ch == '?' || ch == ':' ) {
                    spaces = 2;
                } else {
                    spaces = 1;
                }
            }

            space_width = spaces * wgml_fonts[font_number].spc_width;
            cur_h_address += space_width;

            /* Reset spaces (and, eventually, tabs). */

            spaces = 0;
        }

        /* If the end of the phrase is next, then the phrase ended with
         * spaces and/or tabs. Phrase processing is done.
         */

        if( *input_text == '\0' ) break;

        /* At this point, next_chars will be NULL. It is time to get the
         * next token.
         */

        token_start = input_text;
        while( *input_text != '\0' ) {
            if( isspace( *input_text ) ) {
                if( ProcFlags.in_trans ) {
                    if( *(input_text - 1) != in_esc ) {
                        break;
                    }
                }
            }
            input_text++;
        }
        count = input_text - token_start;

        /* Initialize next_chars to contain the token. */

        next_chars = oc_alloc_text_chars( token_start, count, font_number );
        next_chars->x_address = cur_h_address;
        oc_intrans( next_chars->text, &next_chars->count, font_number );

        if( next_chars->count == 0 ) {
            next_chars->width = 0;
        } else {
            next_chars->width = cop_text_width( next_chars->text, \
                                next_chars->count, next_chars->font_number );
        }
        increment = next_chars->width;

        /* Update text_line. */

        if( (cur_h_address + increment) > oc_page_right ) {

            /* Detach cur_chars, if it is empty.
             * Note that cur_chars is now the only pointer to this text_chars.
             */

            if( cur_chars->count == 0 ) {
                pre_chars->next = NULL;
            }

            /* Process the full text_line and get a new one. */

            oc_process_line_full( the_line, true );
            the_line = oc_alloc_text_line();

            /* Reset cur_h_address. */

            cur_h_address = oc_page_left;

            /* At this point, next_chars->x_address is set to the proper
             * value for the end of the prior text_line. This resets it
             * to the proper value for the start of the current line.
             */

            next_chars->x_address = cur_h_address;

            /* Attach cur_chars to the_line if it was empty, using the same
             * horizontal position as just given to next_chars.
             */

            if( cur_chars->count == 0 ) {
                cur_chars->x_address = cur_h_address;
                the_line->first = cur_chars;
                pre_chars = NULL;
            }
        }
    }

    /* At this point, if next_chars is not NULL, then these invariants hold: 
     *      the_line is not NULL.
     *      next_chars is guaranteed to fit into the_line.
     */

    if( next_chars != NULL ) {
        if( the_line->first == NULL ) {
            the_line->first = next_chars;
        } else {
            cur_chars->next = next_chars;
        }
        cur_chars = next_chars;
        next_chars = NULL;
        spaces = 0;

        /* Update cur_h_address to the width of the token added. */

        cur_h_address += increment;

        /* Adjust the line_height, if appropriate. */

        cur_height = wgml_fonts[font_number].line_height;
        if( the_line->line_height < cur_height ) {
            the_line->line_height = cur_height;
        }
    }

    return;
}

/* Function emulate_input_source()
 * This function passes known text buffers to oc_process_text() after setting
 * the font number and break flag. 
 *
 * Parameters:
 *      in_text points to the text_phrase array to be processed.
 *
 * Global Changed:
 *      oc_break is set to the correct value.
 *      oc_font_number is set to the font_number of the last text_phrase
 *          processed.
 *
 * Notes:
 *      This function is intended to emulate the last stage of text
 *          processing in our wgml. 
 */
 
static void emulate_input_source( text_phrase * in_text )
{
    text_phrase *   cur_phrase  = NULL;

    /* Process all the text_phrases in in_text. */

    cur_phrase = in_text;
    while( cur_phrase->text != NULL ) {
        if( cur_phrase->font_number >= wgml_font_cnt ) \
                                                cur_phrase->font_number = 0;
        oc_font_number = cur_phrase->font_number;
        oc_break = cur_phrase->line_break;
        oc_process_text( cur_phrase->text, oc_font_number );
        cur_phrase++;
    }
    return;
}

/* Function emulate_layout_page()
 * This function emulates "the rest of wgml", setting various flags and
 * globals depending on the current element type and sending any text buffers
 * to emulate_input_source() to handle the break flag and the font number and
 * get each text buffer processed. 
 *
 * Parameters:
 *      construct encodes the type of construct to be emulated.
 *      input_text points to the text_phrase array to be processed.
 */
 
static void emulate_layout_page( text_phrase * input_text, \
                                   oc_construct_type construct )
{
    switch( construct ) {
    case oc_title_text:

        /* Set the globals. This emulates a pre_top_skip of 15. */

        oc_pre_top_skip = 15;
        oc_pre_skip = 0;
        oc_old_post_skip = oc_cur_post_skip;
        oc_cur_post_skip = 0;

        /* Initalize oc_new_element. */

        oc_new_element = oc_alloc_oc_element( oc_text );

        /* Emulate setting up the element per the :LAYOUT :TITLEP :TITLE info. */

        oc_new_element->line_position = oc_center;

        /* Output the Title. */

        oc_is_paragraph = false;
        emulate_input_source( input_text );

        break;

    case oc_paragraph:

        /* Set the globals. This emulates a pre_skip of 1 and an indent of
         * '0.5i'.
         */

        oc_pre_top_skip = 0;
        oc_pre_skip = 1;
        oc_old_post_skip = oc_cur_post_skip;
        oc_cur_post_skip = 0;
        oc_indent = bin_device->horizontal_base_units / 2;

        /* Initalize oc_new_element. */

        oc_new_element = oc_alloc_oc_element( oc_text );

        /* Emulate setting up the element per the :LAYOUT :P :TITLE info. */

        oc_new_element->line_position = oc_none;

        /* Put input_text into cur_elem. */

        oc_is_paragraph = true;
        emulate_input_source( input_text );
        break;
    case oc_bx_box:
        /* Implementation: to be done. */
        break;
    case oc_fig_box:
        /* Implementation: to be done. */
        break;
    default:
        out_msg( "outcheck internal error: incorrect element type\n" );
        err_count++;
        g_suicide();
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
    oc_element      *   te_pool_ptr = NULL;
    text_chars      *   tc_pool_ptr = NULL;
    text_line       *   tl_pool_ptr = NULL;
    uint32_t            oc_h_len;
    uint32_t            oc_v_len;
    uint32_t            max_char_width;

    /* Set the file-level globals. */

    /* Initialize the oc_element pool to hold ten instances. These are
     * oc_text_elements because that is the oc_element used most often.
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

    oc_element_pool = (oc_element *) mem_alloc( sizeof( oc_element ) );
    oc_element_pool->next = NULL;
    oc_element_pool->type = oc_text;
    oc_element_pool->element.oc_text.count = 0;
    oc_element_pool->element.oc_text.depth = 0;
    oc_element_pool->element.oc_text.first = NULL;
    te_pool_ptr = oc_element_pool;
    for( i = 0; i < 9; i++ ) {
        te_pool_ptr->next = (oc_element *) \
                                        mem_alloc( sizeof( oc_element ) );
        te_pool_ptr = te_pool_ptr->next;
        te_pool_ptr->next = NULL;
        te_pool_ptr->type = oc_text;
        te_pool_ptr->element.oc_text.count = 0;
        te_pool_ptr->element.oc_text.depth = 0;
        te_pool_ptr->element.oc_text.first = NULL;
    }

    /* Set up oc_cur_page.column. */

    oc_cur_page.column.cur_depth = 0;
    oc_cur_page.column.first = 0;

    /* These values are currently being tuned to match wgml 4.0.
     * The constants used with oc_max_depth match the values used by
     * wgml 4.0 for a page depth of 9.66i converted to vertical base
     * units and rounded up if the result is not an integer. The use of
     * y_offset shown comes from our wgml code. 
     */

    if( bin_driver->y_positive == 0 ) {
        oc_page_top = bin_device->y_start;
        oc_max_depth = 9660 - bin_device->y_offset;
    } else {
        oc_page_top = 0;
        oc_max_depth = 58;
    }

    /* The "missing" value, oc_page_bottom, was dropped because no use was
     * found for it in computing the page layout. The corresponding variable
     * may, of course, be useful in our wgml.
     */

    /* The values used set up a left margin of '1i' and a right margin of
     * '7i', expressed in horizontal base units.
     */

    oc_page_left = bin_device->horizontal_base_units;
    oc_page_right = 7 * bin_device->horizontal_base_units;

    /* This is used for the line height when the actual height is either not
     * known or not well defined. Not every use in this test program will be
     * appropriate in our wgml, at least, not when it has to process the OW
     * docs, which do have different line heights for different fonts.
     */

    oc_line_height_zero = wgml_fonts[0].line_height;

    /* Set the variables. */

    max_char_width = 0;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( max_char_width < wgml_fonts[i].default_width ) \
            max_char_width = wgml_fonts[i].default_width;
    }

    /* The OUTCHECK Test Document. */

    /* Start in "script" mode. */

    oc_script = true;

    /* Allow input translation tests. */

    cop_ti_table( "set ~", 5 );

    /* First pass processing. */
    /* START processing.*/

    fb_start();

    /* DOCUMENT processing.*/

    fb_document();

    /* Only one document pass is emulated. */

    oc_document_start = true;
    oc_new_section = true;

    /* Title page. */ 

    emulate_layout_page( title, oc_title_text );

    /* The title page is done. Finish the page and force it out. */

    oc_break = true;
    oc_process_text( NULL, oc_font_number );
    oc_output_page();

    /* Document Section. */

    oc_new_section = true;

    /* Output the text paragraphs. */

    emulate_layout_page( para_font, oc_paragraph );
    emulate_layout_page( para_stop, oc_paragraph );

    /* Now switch to "wscript/noscript" mode. */

    oc_script = false;
    emulate_layout_page( para_font, oc_paragraph );
    emulate_layout_page( para_stop, oc_paragraph );

#if 0 // restore when boxing is done again and blank-line-insertion is handled
    /* A blank line is placed between paragraphs. */

    /* Output the second paragraph. */

//    oc_h_start = oc_page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
//        oc_v_start -= 2 * line_height_zero;
    } else {
//        oc_v_start += 2 * line_height_zero;
    }
    emulate_layout_page( para2, oc_paragraph );
#endif
    /* First box. */

    oc_h_len = bin_device->horizontal_base_units;
//    oc_h_start = bin_device->horizontal_base_units;
    oc_v_len = 2 * max_char_width;
#if 0 // move to within the bx_element including the above three lines
    if( bin_driver->y_positive == 0x00 ) {
//        oc_v_start -= 2 * line_height_zero;
    } else {
//        oc_v_start += 2 * line_height_zero;
    }

    if( has_aa_block ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->hline.text != NULL ) {
//                fb_hline( oc_h_start, oc_v_start, oc_h_len );
//                fb_hline( oc_h_start, oc_v_start - oc_v_len, oc_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
//                oc_v_start -= oc_v_len;
//                fb_vline( oc_h_start, oc_v_start, oc_v_len );
//                fb_vline( oc_h_start + oc_h_len, oc_v_start, oc_v_len );
//                oc_v_start += oc_v_len;
            }
        } else {
            if( bin_driver->hline.text != NULL ) {
//                fb_hline( oc_h_start, oc_v_start, oc_h_len );
//                fb_hline( oc_h_start, oc_v_start + oc_v_len, oc_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
//                oc_v_start += oc_v_len;
//                fb_vline( oc_h_start, oc_v_start, oc_v_len );
//                fb_vline( oc_h_start + oc_h_len, oc_v_start, oc_v_len );
//                oc_v_start -= oc_v_len;
            }
        }
    }
#endif
    /* Output the third paragraph. */

#if 0 // restore when boxing is done again
//    oc_h_start = oc_page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
//        oc_v_start = oc_page_top - line_height_zero;
    } else {
//        oc_v_start = line_height_zero;
    }
    emulate_layout_page( para3, oc_text );

    /* Second box. */

    if( bin_driver->y_positive == 0x00 ) {
//        oc_v_start -= 2 * line_height_zero;
    } else {
//        oc_v_start += 2 * line_height_zero;
    }

//    oc_h_start = bin_device->horizontal_base_units;
    if( has_aa_block != NULL ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->dbox.text != NULL ) {
//                fb_dbox( oc_h_start, oc_v_start - oc_v_len, oc_h_len, \
                                                                oc_v_len );
            }
        } else {
            if( bin_driver->dbox.text != NULL ) {
//                fb_dbox( oc_h_start, oc_v_start + oc_v_len, oc_h_len, \
                                                                oc_v_len );
            }
        }
    }
#endif

    /* The document is processed. Finish the page and force it out. */

    oc_break = true;
    oc_process_text( NULL, oc_font_number );
    oc_output_page();

    /* :FINISH block. */

    fb_finish();

    /* Free allocated memory. */

    /* Free the memory held by the oc_element pool. */

    if( oc_element_pool != NULL ) {
        while( oc_element_pool->next != NULL ) {
            te_pool_ptr = oc_element_pool;
            oc_element_pool = oc_element_pool->next;
            switch( te_pool_ptr->type ) {
            case oc_text:

                if( te_pool_ptr->element.oc_text.first != NULL ) {

                    /* Return any text_line instances to the text_line pool. */

                    out_msg( "oc_text_element in pool not empty!\n" );
                    oc_add_text_line_to_pool( te_pool_ptr->element.oc_text.first );
                    te_pool_ptr->element.oc_text.first = NULL;
                }
            case oc_bx:
                /* Implementation: to be done. */
                break;
            case oc_fig:
                /* Implementation: to be done. */
                break;
            default:
                out_msg( "outcheck internal error: incorrect oc_element\n" );
                err_count++;
                g_suicide();
            }
            mem_free( te_pool_ptr );
        }
        mem_free( oc_element_pool );
        oc_element_pool = NULL;
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
                oc_add_text_chars_to_pool( tl_pool_ptr );
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

