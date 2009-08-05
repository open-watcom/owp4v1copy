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
*               and this struct:
*                   text_phrase
*               and these variables (the text arrays are ignored):
*                   cur_h_start
*                   cur_v_start
*                   max_line_height
*                   page_bottom
*                   page_left
*                   page_right
*                   page_top
*                   text_chars_pool
*               and these local functions:
*                   emulate_text_output()
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

/* Struct definition. */
 
typedef struct {
    uint8_t     font_number;
    char    *   text;
} text_phrase;
 
/* Local variables. */
 
/* These are used by more than one function. */
 
static  text_chars      *   text_chars_pool = NULL;
static  uint32_t            cur_h_start;
static  uint32_t            cur_v_start;
static  uint32_t            max_line_height;
static  uint32_t            page_bottom;
static  uint32_t            page_left;
static  uint32_t            page_right;
static  uint32_t            page_top;
 
/* Load the usage text array. */
 
static  char    const   *   usage_text[] = {
"Usage:  outcheck defined-name",
"'defined-name' is the defined name for the device to use",
"'defined-name' must be enclosed in quotes if it contains spaces",
NULL
};
 
/* Load the document text arrays. */
 
static  text_phrase         title[] = {
    { 3, "OUTCHECK~ Document" },
    { 0, NULL}
};
 
static  text_phrase         page1_para1[] = {
    {0, "This document is copied and modified from the Wiki. The reason for" },
    {0, "this is to make it less boring." },
    {0, NULL }
};
 
static  text_phrase         page1_para2[] = {
    {0, "Those function sequences involving literal parameters which are" },
    {0, "discussed here behave as expected in all compiled function blocks." },
    {0, "The number of possible function sequences in this category is" },
    {0, "infinite; these examples were tested:" },
    {0, NULL }
};
 
#if 0 // Needed until the boxes are ready for testing
 
static  text_phrase         page1_box[] = {
    {0, "%binary(3)              the appropriate graphic appears" },
    {0, "%binary1(4)             the appropriate graphic appears" },
    {0, "%binary2(5)             the appropriate graphic appears" },
    {0, "%binary4(6)             the appropriate graphic appears" },
    {0, "%image('image test')    the string \"image test\" appears" },
    {0, "%image(%decimal(53))    the digits \"53\" appear" },
    {0, "%image(%hex(53))        the digits \"35\" appear" },
    {0, "%image(%lower('SUZY'))  the string \"suzy\" appears" },
    {0, "%text('text test')      the string \"text test\" appears" },
    {0, "%text(%decimal(53))     the digits \"53\" appear" },
    {0, "%text(%hex(53))         the digits \"35\" appear" },
    {0, "%text(%lower('SUZY'))   the string \"suzy\" appears" },
    {0, NULL }
};
 
#endif // #if 0
 
static  text_phrase         page2_para1[] = {
    {0, "An interesting phenomenon became apparent during these tests: the" },
    {0, "XP VDM window (at least) behaves as if the null characters generated" },
    {0, "by %binary2(5) and %binary4(6) were CR+LF characters. Both the" },
    {0, "binary file and a file containing the redirected screen output were" },
    {0, "examined, and neither showed actual additional CR+LF characters," },
    {0, "extra compiled %recordbreak() functions, or blank lines in the output" },
    {0, "file, so this pretty much has to be something the VDM is doing." },
    {0, NULL }
};
 
static  text_phrase         page2_para2[] = {
    {0, "All other function sequences discussed which use literal parameters" },
    {0, "output function sequences in which the parameters are encoded using" },
    {0, "parameter blocks." },
    {0, NULL }
};
 
static  text_phrase         page2_para3[] = {
    {0, "These functions are presumed to work uniformly:" },
    {0, NULL }
};
 
#if 0 // Needed until the boxes are ready for testing
 
static  text_phrase         page2_box[] = {
    {0, "%font_number()" },
    {0, "%pages()" },
    {0, "%tab_width()" },
    {0, "%x_address()" },
    {0, "%x_size()" },
    {0, "%y_address()" },
    {0, "%y_size()" },
    {0, NULL }
};
 
#endif // #if 0
 
/* Local function definitions. */

/* Function emulate_text_output()
 * This function does the final preparation of the text and outputs it.
 *
 * Parameters:
 *      text contains the text to be output
 *
 * Globals Used:
 *      cur_h_start contains the horizontal location of the first character.
 *      cur_v_start contains the vertical location of the first character.
 *
 * Globals Changed:
 *      cur_h_start contains the horizontal location of the next character after
 *          the last character displayed.
 *      cur_v_start contains the vertical location of the last text_line
 *          output.
 *
 * Notes:
 *      This function is intended to provide some idea of what might be
 *          done between the point where the text is finalized except for
 *          input translation and the point where it is sent to be output.
 *      This function is the sort of thing that might, with a bit of
 *          enhancement, be used to output the text used with a :P. tag. The
 *          "enhancement" would include processing the various tags specifying
 *          the desired font for a given phrase, rather than getting the
 *          information from the text_phrase.
 *      The first text_line will be output starting at the location given by
 *          cur_h_start on entry. Subsequent lines will be output at the
 *          location given by page_left. This may or may not be acceptable
 *          in production code, depending on the tag being implemented.
 */
 
void emulate_text_output( text_phrase * text )
{
    char            the_char;
    int             i;
    int             j;
    size_t          count;
    size_t          increment;
    text_chars  *   cur_chars;
    text_chars  *   next_chars;
    text_chars  *   pool_ptr;
    text_chars  *   save_chars;
    text_line       the_line;
    text_phrase *   cur_phrase;
    uint8_t         font_number;

    /* This looks right, but may turn out to be wrong in wgml. */
 
    if( text == NULL) return;
 
    /* Set up the_line and for the outer loop. */

    the_line.y_address = cur_v_start;
    the_line.first = NULL;
    cur_chars = NULL;
    if( text_chars_pool == NULL ) {
        next_chars = (text_chars *) mem_alloc( sizeof( text_chars ) + \
                                                                TEXT_START );
        next_chars->length = TEXT_START;
    } else {
        next_chars = text_chars_pool;
        text_chars_pool = text_chars_pool->next;
    }
    cur_phrase = text;
    font_number = cur_phrase->font_number;
    if( font_number > wgml_font_cnt ) font_number = 0;
    next_chars->next = NULL;
    next_chars->font_number = font_number;
    next_chars->x_address = cur_h_start;
    next_chars->width = 0;
    next_chars->count = 0;
 
    /* The outer loop processes all the text_phrases in text. */
 
    while( cur_phrase->text != NULL ) {
        count = strlen( cur_phrase->text );
 
        /* This loop processes a single text_phrase. */
 
        for( i = 0; i < count; i++ ) {
            if( isspace( cur_phrase->text[i] ) ) continue;
 
            /* This loop processes a single text_chars. */
 
            for( j = i; j < count; j++ ) {

                the_char = cur_phrase->text[j];
                if( isspace( the_char ) ) break;
 
                if( ProcFlags.in_trans ) {
                    if( in_esc == the_char ) {
                        j++;
                        the_char = cop_in_trans( cur_phrase->text[j], \
                                                    cur_phrase->font_number );
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
            i = j;

            /* Compute the width and initialize increment. */
 
            next_chars->width = cop_text_width( next_chars->text, \
                                next_chars->count, next_chars->font_number );
            increment = next_chars->width;
 
            /* Allow one space between text_chars instances. */
 
            increment += wgml_fonts[next_chars->font_number].spc_width;
 
            /* Finalize the the text_line. */
 
            if( (cur_h_start + increment) > page_right ) {
 
                /* The text_line is ready for output and reinitialization. */
 
                save_chars = next_chars;
                fb_output_textline( &the_line );
 
                if( bin_driver->y_positive == 0x00 ) {
                    cur_v_start -= max_line_height;
                } else {
                    cur_v_start += max_line_height;
                }
                the_line.y_address = cur_v_start;
 
                if( text_chars_pool == NULL ) {
                    text_chars_pool = the_line.first;
                } else {
                    pool_ptr = text_chars_pool;
                    while( pool_ptr->next != NULL) pool_ptr = pool_ptr->next;
                    pool_ptr->next = the_line.first;
                }
                the_line.first = save_chars;
                cur_chars = save_chars;
                cur_h_start = page_left;
                cur_chars->x_address = cur_h_start;

            } else {
 
                /* The text_chars belongs to this text_line. */
 
                if( the_line.first == NULL ) {
                    the_line.first = next_chars;
                } else {
                    cur_chars->next = next_chars;
                }
                cur_chars = next_chars;
            }
 
            /* Update cur_h_start and get the next text_chars instance. */

            cur_h_start += increment;
            if( text_chars_pool == NULL ) {
                next_chars = (text_chars *) mem_alloc( sizeof( text_chars ) + \
                                                                TEXT_START );
                next_chars->length = TEXT_START;
            } else {
                next_chars = text_chars_pool;
                text_chars_pool = text_chars_pool->next;
            }
            next_chars->next = NULL;
            next_chars->font_number = font_number;
            next_chars->x_address = cur_h_start;
            next_chars->width = 0;
            next_chars->count = 0;
        }

        /* Go to the next text_phrase. */
 
        cur_phrase++;
        font_number = cur_phrase->font_number;
        if( font_number > wgml_font_cnt ) font_number = 0;
    }

    /* Output the final text_line and return the text_chars to the pool.*/
 
    fb_output_textline( &the_line );
 
    if( text_chars_pool == NULL ) {
        text_chars_pool = the_line.first;
    } else {
        pool_ptr = text_chars_pool;
        while( pool_ptr->next != NULL) pool_ptr = pool_ptr->next;
        pool_ptr->next = the_line.first;
    }
    the_line.first = NULL;
 
    if( next_chars != NULL ) {
        next_chars->next = text_chars_pool;
        text_chars_pool = next_chars;
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
    text_chars      *   current;
    text_chars      *   pool_ptr;
    uint32_t            cur_h_len;
    uint32_t            cur_v_len;
    uint32_t            max_char_width;
    uint32_t            net_page_height;
    uint32_t            net_page_width;
 
    /* Set the file-level globals. */
 
    /* Initialize the text_chars pool. */

    current = (text_chars *) mem_alloc( sizeof( text_chars ) + TEXT_START );
    current->next = NULL;
    current->font_number = 0;
    current->x_address = 0;
    current->width = 0;
    current->count = 0;
    current->length = TEXT_START;
    text_chars_pool = current;
 
    /* Now bring the number of instances to 20. */
 
    pool_ptr = text_chars_pool;
    for( i = 0; i < 19; i++ ) {
        current = (text_chars *) mem_alloc( sizeof( text_chars ) + TEXT_START );
        current->next = NULL;
        current->font_number = 0;
        current->x_address = 0;
        current->width = 0;
        current->count = 0;
        current->length = TEXT_START;
        pool_ptr->next = current;
        pool_ptr = current;
    }
 
    /* These would normally be set per the :LAYOUT. Here, half-inch
     * top, bottom and right margins and a one-inch left margin are
     * applied to an 8-1/2" x 11" page.
     */
 
    if( bin_driver->y_positive == 0 ) {
        page_top = 10 * bin_device->vertical_base_units;
        page_bottom = bin_device->vertical_base_units / 2;
    } else {
        page_top = bin_device->vertical_base_units / 2;
        page_bottom = 10 * bin_device->vertical_base_units;
    }
 
    page_left = bin_device->horizontal_base_units;
    page_right = (8 * bin_device->horizontal_base_units) - \
                                    (bin_device->horizontal_base_units / 2);
 
    /* Set the variables. */
 
    max_char_width = 0;
    max_line_height = 0;
 
    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( max_char_width < wgml_fonts[i].default_width ) \
            max_char_width = wgml_fonts[i].default_width;
        if( max_line_height < wgml_fonts[i].line_height ) \
            max_line_height = wgml_fonts[i].line_height;
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
 
        page_width = page_right - page_left;
        title_width = cop_text_width( title[0].text, sizeof( title[0].text ), \
                                                        title[0].font_number );
                                                        
        cur_h_start = page_left + ((page_width - title_width) / 2);
    }
 
    /* This places the title part-way down the page, something
     * which would normally be set per the :LAYOUT. The computation is: 14
     * line gap from the top of the page, plus one line for the line the
     * text is to appear on. 
     */
     
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start = page_top - (15 * max_line_height);
    } else {
        cur_v_start = page_top + (15 * max_line_height);
    }
 
    fb_position( cur_h_start, cur_v_start );
 
    /* Output the Title. */
 
    emulate_text_output( title );
 
    /* First page. */
 
    /* Margin/indent setup. */
 
    /* One-inch margin on line 1. */
 
    cur_h_start = page_left;
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start = page_top - max_line_height;
    } else {
        cur_v_start = page_top + max_line_height;
    }
 
    fb_new_section( cur_v_start );
 
    /* One-half inch paragraph indent. */
 
    cur_h_start += bin_device->horizontal_base_units / 2;
 
    /* Output the first paragraph. */
 
    emulate_text_output( page1_para1 );

    /* A blank line is placed between paragraphs. */

    /* Output the second paragraph. */

    cur_h_start = page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start -= 2 * max_line_height;
    } else {
        cur_v_start += 2 * max_line_height;
    }
    emulate_text_output( page1_para2 );

    /* First box. */
 
    cur_h_len = bin_device->horizontal_base_units;
    cur_h_start = bin_device->horizontal_base_units;
    cur_v_len = 2 * max_char_width;
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start -= 2 * max_line_height;
    } else {
        cur_v_start += 2 * max_line_height;
    }
 
    if( has_aa_block ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->hline.text != NULL ) {
                fb_hline( cur_h_start, cur_v_start, cur_h_len );
                fb_hline( cur_h_start, cur_v_start - cur_v_len, cur_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
                cur_v_start -= cur_v_len;
                fb_vline( cur_h_start, cur_v_start, cur_v_len );
                fb_vline( cur_h_start + cur_h_len, cur_v_start, cur_v_len );
                cur_v_start += cur_v_len;
            }
        } else {
            if( bin_driver->hline.text != NULL ) {
                fb_hline( cur_h_start, cur_v_start, cur_h_len );
                fb_hline( cur_h_start, cur_v_start + cur_v_len, cur_h_len );
            }
            if( bin_driver->vline.text != NULL ) {
                cur_v_start += cur_v_len;
                fb_vline( cur_h_start, cur_v_start, cur_v_len );
                fb_vline( cur_h_start + cur_h_len, cur_v_start, cur_v_len );
                cur_v_start -= cur_v_len;
            }
        }
    }
 
    /* :NEWPAGE block. */
 
    fb_document_page();
 
    /* Second page. */
 
    /* Output the first paragraph. */

    cur_h_start = page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start = net_page_height - max_line_height;
    } else {
        cur_v_start = max_line_height;
    }
    emulate_text_output( page2_para1 );
 
    /* Output the second paragraph. */

    cur_h_start = page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start -= 2 * max_line_height;
    } else {
        cur_v_start += 2 * max_line_height;
    }
    emulate_text_output( page2_para2 );
 
    /* Output the third paragraph. */

    cur_h_start = page_left + (bin_device->horizontal_base_units / 2);
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start -= 2 * max_line_height;
    } else {
        cur_v_start += 2 * max_line_height;
    }
    emulate_text_output( page2_para3 );
 
    /* Second box. */
 
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start -= 2 * max_line_height;
    } else {
        cur_v_start += 2 * max_line_height;
    }

    cur_h_start = bin_device->horizontal_base_units;
    if( has_aa_block != NULL ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( bin_driver->dbox.text != NULL ) {
                fb_dbox( cur_h_start, cur_v_start - cur_v_len, cur_h_len, \
                                                                cur_v_len );
            }
        } else {
            if( bin_driver->dbox.text != NULL ) {
                fb_dbox( cur_h_start, cur_v_start + cur_v_len, cur_h_len, \
                                                                cur_v_len );
            }
        }
    }
 
    /* :FINISH block. */
 
    fb_finish();
 
    /* Free allocated memory. */
 
    if( text_chars_pool != NULL ) {
        while( text_chars_pool->next != NULL ) {
            current = text_chars_pool;
            text_chars_pool = text_chars_pool->next;
            mem_free( current );
        }
        current = text_chars_pool;
        text_chars_pool = text_chars_pool->next;
        mem_free( current );
        current = NULL;
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
 
    return( EXIT_SUCCESS );
}
 
