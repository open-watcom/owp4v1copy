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
*               and these local functions:
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

/* Local variables. */

/* Load the usage text array. */

static  char const *    usage_text[] = {
"Usage:  outcheck defined-name",
"'defined-name' is the defined name for the device to use",
"'defined-name' must be enclosed in quotes if it contains spaces",
NULL
};

/* Load the document text arrays. */
#if 0
static  char const *    page1_para1[] = {
"This is not a very interesting document. The reason for this is that it is",
"very self-referential. For example, this is the third sentence in the first",
"paragraph of the first page.",
NULL
};

static  char const *    page1_para2[] = {
"And here we have the second paragraph of the first page. This is, of course,",
"it second sentence. It is amazing how uninteresting this is, isn't it? One might",
"even say it was boring.",
NULL
};

static  char const *    page1_box[] = {
"For variety, here we emulate some,",
"boxed text. Actually doing this in",
"wgml will be interesting.",
NULL
};

static  char const *    page2_para1[] = {
"In fact, this is so boring that I am going to make the second page very short.",
"I hope that you have found this demonstration convincing.",
NULL
};
#endif
/* Local function definition. */

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
    record_buffer   *   translated;
    uint32_t            cur_h_len;
    uint32_t            cur_h_start;
    uint32_t            cur_v_len;
    uint32_t            cur_v_start;
    uint32_t            max_char_width;
    uint32_t            max_line_height;
    uint32_t            net_page_height;
    uint32_t            net_page_width;

    /* Set the variables. */

    translated = (record_buffer *) mem_alloc( sizeof( record_buffer ) );
    translated->current = 0;
    translated->length = 80;
    translated->text = (uint8_t *) mem_alloc( translated->length );

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

    /* Test the input translation. */

    out_msg( "The escape character is: '%c'\n", in_esc );
    out_msg( "The in_trans flag is: %i\n", ProcFlags.in_trans );
    out_msg( "Setting the input escape character to '~'\n" );
    cop_ti_table( "set ~", 5 );
    out_msg( "The escape character is: '%c'\n", in_esc );
    out_msg( "The in_trans flag is: %i\n", ProcFlags.in_trans );
    out_msg( "Clearing the input escape character\n" );
    cop_ti_table( "set", 3 );
    out_msg( "The escape character is: '%c'\n", in_esc );
    out_msg( "The in_trans flag is: %i\n", ProcFlags.in_trans );

    out_msg( "Using characters to set the translations\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));
    cop_ti_table( "i j t u", 7 );
    out_msg( ".ti table set to translate 'i' to 'j' and 't' to 'u'\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));
    cop_ti_table( "i", 1 );
    out_msg( ".ti table reset to translate 'i' to 'i'\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));
    cop_ti_table( "", 0 );
    out_msg( ".ti table reset\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));

    out_msg( "Using hexadecimals to set the translations\n" );
    cop_ti_table( "69 6A 74 75", 11 );
    out_msg( ".ti table set to translate 'i' to 'j' and 't' to 'u'\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));
    cop_ti_table( "69", 2 );
    out_msg( ".ti table reset to translate 'i' to 'i'\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));
    cop_ti_table( "", 0 );
    out_msg( ".ti table reset\n" );
    out_msg( "Original char: 'i'; translated char: '%c'\n", cop_in_trans( 'i', 0 ));
    out_msg( "Original char: 't'; translated char: '%c'\n", cop_in_trans( 't', 0 ));

    out_msg( "Output translation test\n" );
    out_msg( "Original text: 'Mary had a little lamb.'\n");
    cop_out_trans( "Mary had a little lamb.", \
                        strlen( "Mary had a little lamb." ), translated, 0 );
    /* This is only safe because we know the length of the test string is
     * less than the 80 bytes in translated->text. If done in general, the
     * possibility of having to mem_realloc() translated->text would have
     * to be taken into account.
     */

    translated->text[translated->current] = '\0';
    out_msg( "Translated text: '%s'\n", translated->text );


    out_msg( ".tr table set to translate space to '~'\n" );
    cop_tr_table( "20 ~", 4 );
    out_msg( "Original text: 'Mary had a little lamb.'\n");
    cop_out_trans( "Mary had a little lamb.", \
                        strlen( "Mary had a little lamb." ), translated, 0 );
    
    /* This is only safe because we know the length of the test string is
     * less than the 80 bytes in translated->text. If done in general, the
     * possibility of having to mem_realloc() translated->text would have
     * to be taken into account.
     */

    translated->text[translated->current] = '\0';
    out_msg( "Translated text: '%s'\n", translated->text );

    out_msg( ".tr table cleared\n" );
    cop_tr_table( "", 0 );
    out_msg( "Original text: 'Mary had a little lamb.'\n");
    cop_out_trans( "Mary had a little lamb.", \
                        strlen( "Mary had a little lamb." ), translated, 0 );

    /* This is only safe because we know the length of the test string is
     * less than the 80 bytes in translated->text. If done in general, the
     * possibility of having to mem_realloc() translated->text would have
     * to be taken into account.
     */

    translated->text[translated->current] = '\0';
    out_msg( "Translated text: '%s'\n", translated->text );


    /* First pass processing. */
    /* START processing.*/

    fb_start();

    /* DOCUMENT processing.*/

    fb_document();

    /* Last pass processing. */

// May need third page to test fb_new_section().

    /* First page. */

    /* Margin/indent setup. */

    /* One-inch margin on line 1. */

    cur_h_start = bin_device->horizontal_base_units;
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start = net_page_height - (1 * max_line_height);
    } else {
        cur_v_start = 1 * max_line_height;
    }

    fb_position( cur_h_start, cur_v_start );

    /* One-half inch paragraph indent. */

    cur_h_start += bin_device->horizontal_base_units / 2;

    fb_position( cur_h_start, cur_v_start );

    /* First box. */

    cur_h_len = bin_device->horizontal_base_units;
    cur_h_start = bin_device->horizontal_base_units;
    cur_v_len = 2 * max_char_width;
    if( bin_driver->y_positive == 0x00 ) {
        cur_v_start = net_page_height - (1 * max_line_height);
    } else {
        cur_v_start = 1 * max_line_height;
    }

    out_msg( "cur_h_len = %i\n", cur_h_len );
    out_msg( "cur_h_start = %i\n", cur_h_start );
    out_msg( "cur_v_len = %i\n", cur_v_len );
    out_msg( "cur_v_start = %i\n", cur_v_start );
    out_msg( "max_char_width = %i\n", max_char_width );
    out_msg( "max_line_height = %i\n", max_line_height );
    out_msg( "net_page_height = %i\n", net_page_height );
    out_msg( "net_page_width = %i\n", net_page_width );

    if( bin_driver->absoluteaddress.text != NULL ) {
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

    /* Second box. */

    if( bin_driver->absoluteaddress.text != NULL ) {
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

    if( translated != NULL ) {
        if( translated->text != NULL ) {
            mem_free( translated->text );
            translated->text = NULL;
        }
        mem_free( translated );
        translated = NULL;
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
    int         k;
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

    out_msg( "out_file name: %s\n", out_file);
    out_msg( "out_file format: %s\n", out_file_attr);
    
    /* Release the memory allocated. */

    cop_teardown();
    ff_teardown();

    mem_free(tgt_path);
    tgt_path = NULL;

    /* These are from free_some_mem(), which is static in wgml.c. */

    if( token_buf != NULL ) {
        mem_free( token_buf );
    }
    if( alt_ext != NULL ) {
        mem_free( alt_ext );
    }
    if( def_ext != NULL ) {
        mem_free( def_ext );
    }
    if( master_fname != NULL ) {
        mem_free( master_fname );
    }
    if( master_fname_attr != NULL ) {
        mem_free( master_fname_attr );
    }
    if( dev_name != NULL ) {
        mem_free( dev_name );
    }
    if( out_file != NULL ) {
        mem_free( out_file );
    }
    if( out_file_attr != NULL ) {
        mem_free( out_file_attr );
    }
    if( global_dict != NULL ) {
        free_dict( &global_dict );
    }
    if( macro_dict != NULL ) {
        free_macro_dict( &macro_dict );
    }
    if( tag_dict != NULL ) {
        free_tag_dict( &tag_dict );
    }
    if( buff2 != NULL ) {
        mem_free( buff2 );
    }

    for( k = 0; k < max_buflist; k++ ) {
        if( buflist[k].buf == NULL )  break;
        mem_free( buflist[k].buf );
    }
    {
        text_chars  *wk;
        text_chars  *w = text_list.next;

        while( w != NULL ) {
           wk = w->next;
           mem_free( w );
           w = wk;
        }

        w =  words.first;
        while( w != NULL ) {
           wk = w->next;
           mem_free( w );
           w = wk;
        }
    }

end_heapcheck( "main" );

    return( EXIT_SUCCESS );
}

