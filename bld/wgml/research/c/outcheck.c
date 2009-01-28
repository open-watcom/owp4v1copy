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
*               and this local function:
*                   emulate_wgml()
*
* Notes:        The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
*               This program uses/tests the binary device library to output
*               a text file. As such, all structs and field names refer to
*               those in "copfiles.h", not the research code.
****************************************************************************/

#include <process.h>
#include <stdio.h>
#include <stdlib.h>

#include "banner.h"
#include "common.h"
#include "copfiles.h"
#include "findfile.h"
#include "research.h"

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
 */

static void emulate_wgml( void )
{
    /* START processing.*/

    fb_start();

    /* DOCUMENT processing.*/

    /* First page. */

    /* :NEWPAGE block. */

    /* Second page. */

    /* :FINISH block. */

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
    size_t  cmdlen          = 0;
    char *  cmdline         = NULL;
    int     retval;

null_buffer();
start_heapcheck( "main" );

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
    master_fname = "plain";
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

    if( out_file != NULL ) {
        mem_free( out_file );
        out_file = NULL;
    }
    if( out_file_attr != NULL ) {
        mem_free( out_file_attr );
        out_file_attr = NULL;
    }

end_heapcheck( "main" );

    return( EXIT_SUCCESS );
}

