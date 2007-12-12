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
* Description:  Checks files in a gendev source directory for
*               unknown device functions.
*               In addition to main(), these local functions are defined:
*                   check_directory()
*                   print_banner()
*                   print_usage()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banner.h"
#include "common.h"
#include "lhdirect.h"
#include "research.h"


/* Local variables */
/* Load the usage text array */

static  char const *    usage_text[] = {
#include "ffusage.h"
NULL
};

/*  Function check_directory().
 *  Perform the check of the directory provided to the program.
 *  Only files, not subdirectories, are checked.
 *  The length of all files is checked to see if it is a multiple of 16.
 *  Function parse_header() is used to process the header of each file.
 *  The number of files of types 0x02, 0x03 and 0x04 is displayed.
 *  Any file types other than 0x02, 0x03 and 0x04 are displayed.
 *
 *  Global Used:
 *      tgt_path contains the directory passed on the command line
 *
 *  Returns:
 *      FAILURE if the directory provided cannot be opened
 *      SUCCESS if the directory can be opened (and so was checked)
 */

static int check_directory( void )
{
    char *              extension       = NULL;
    DIR  *              current_dir     = NULL;
    struct  dirent  *   dir_entry       = NULL;
    FILE *              current_file    = NULL;

    current_dir = opendir( tgt_path );
    if( current_dir == NULL ) return( FAILURE );
    chdir( tgt_path );
    for(;;) {
        dir_entry = readdir( current_dir );
        if( dir_entry == NULL ) break;

        /* Open the file */

        fopen_s( &current_file, dir_entry->d_name, "rb" );
        if( current_file == NULL ) continue;

        /* Ensure that the extension is ".PCD" */

        extension = strrchr( dir_entry->d_name, '.' );
        if( extension == NULL) continue;
        if( stricmp( extension + 1, "pcd" ) ) continue;

        /* Process the file */

/* Here is where the work will be done! */
        puts(dir_entry->d_name);

        fclose( current_file );
        current_file = NULL;
    }
    closedir( current_dir );
   
    return( SUCCESS );
}

/*  Function print_banner().
 *  Print the banner to the screen
 */

void print_banner( void )
{
    puts( banner1w( "Device Function Research Program", _RESEARCH_VERSION_ ) );
    puts( banner2( "1983" ) );
    puts( banner3 );
    puts( banner3a );
}

/*  Function print_usage().
 *  Print the usage information to the screen
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

/*  Function main().
 *  Given a valid directory path, check all .PCD files in that directory
 *  for device functions not already known to exist.
 *  The actual checks are performed in the function check_directory(); main()
 *  is concerned with overall program architecture, not details.
 *
 *  Returns:
 *      EXIT_FAILURE or EXIT_SUCCESS, as appropriate
 */

int main()
{
    /* Declare automatic variables */

    size_t  cmdlen  = 0;
    char *  cmdline = NULL;
    int     retval;

    /* Display the banner */

    print_banner();

    /* Display the usage information if the command line is empty */

    cmdlen = _bgetcmd( NULL, 0 );
    if( cmdlen == 0 ) {
        print_usage();
        return( EXIT_FAILURE );
    }

    /* Get the command line */

    cmdlen++; /* Include space for the terminating null character */
    cmdline = malloc( cmdlen );
    if( cmdline == NULL ) {
        return( EXIT_FAILURE );
    }

    cmdlen = _bgetcmd( cmdline, cmdlen );

    /* Initialize the globals */

    initialize_globals();
    res_initialize_globals();
    
    /* Parse the command line: allocates and sets tgt_path */

    retval = parse_cmdline( cmdline );
    if( retval == FAILURE ) {
        free( cmdline );
        return( EXIT_FAILURE );
    }

    /* Free the memory held by cmdline and reset it */

    free( cmdline );
    cmdline = NULL;

    /* Check all files in current directory */

    retval = check_directory();

    /* Free the memory held by tgt_path and reset it */

    free( tgt_path );
    tgt_path = NULL;

    /* Done */

    if( retval == FAILURE) {
      print_usage();
      return( EXIT_FAILURE );
    }
    
    return( EXIT_SUCCESS );
}
