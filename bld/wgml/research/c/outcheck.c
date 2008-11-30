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
* Description:  Tests the device library load function for wgml.
*               In addition to main(), these global functions are implemented:
*                   print_banner()
*                   print_usage()
* Notes:        The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
*               This program uses/tests the production code for parsing the
*               binary device library. As such, all structs and field names
*               refer to those in "copfiles.h", not the research code.
*
****************************************************************************/

//#define __STDC_WANT_LIB_EXT1__ 1
//#include <process.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "banner.h"
//#include "common.h"
//#include "copfiles.h"
//#include "dfinterp.h"
//#include "research.h"

/*  Local variables. */

/*  Load the usage text array. */

static  char const *    usage_text[] = {
"Usage:  outcheck defined-name",
"'defined-name' is the defined name for the device to use",
"'defined-name' must be enclosed in quotes if it contains spaces",
NULL
};

/* Local function definitions. */

/* Function print_banner().
 * Print the banner to the screen.
 */

void print_banner( void )
{
    puts( banner1w( "Document Output Test Program", \
                                                        _RESEARCH_VERSION_ ) );
    puts( banner2( "1983" ) );
    puts( banner3 );
    puts( banner3a );
}

/* Function print_usage().
 * Print the usage information to the screen.
 */

void print_usage( void )
{
    char const * *  list;

    list = usage_text;
    while( *list ) {
        puts( *list );
        ++list;
    }
}

/* Function main().
 * ***needs to be redone***
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
    get_env_vars();
    
    /* Parse the command line: allocates and sets tgt_path. */

    retval = parse_cmdline( cmdline );
    if( retval == FAILURE ) {
        free( cmdline );
        return( EXIT_FAILURE );
    }

    /* Free the memory held by cmdline and reset it. */

    free( cmdline );
    cmdline = NULL;

    /* Adjust tgt_path if necessary; see the Wiki. */

    if( !strcmp( tgt_path, "''" ) ) tgt_path[0] = '\0';

    /* Parse the alleged .COP file. */

    retval = parse_defined_name();

    /* Respond to failure. */

    if( retval == FAILURE ) {
      print_usage();
      return( EXIT_FAILURE );
    }

    return( EXIT_SUCCESS );
}

