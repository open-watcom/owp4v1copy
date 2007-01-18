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
* Description:  Implements the utility functions for cfparse():
*                   parse_cop_file()
*                   print_banner()
*                   print_usage()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include "banner.h"
#include "cfdir.h"
#include "cfheader.h"
#include "cfparse.h"
#include "common.h"
#include "research.h"

/* Local variables */

/* Load the usage text array */

static  char const *    usage_text[] = {
#include "cfpusage.h"
NULL
};

/* Local function declaration */

static void check_directory( FILE *, uint32_t);

/*
 *  Verify that the file provided to the program is a .COP file and parse it
 *  if it is.
 *  This version only actually parses directory files.
 *  The actual parsing is done using functions declared in other headers:
 *      cfdir.h for directory files
 *      cfdev.h for device files (planned, not implemented)
 *      cfdrv.h for driver files (planned, not implemented)
 *      cffon.h for font files (planned, not implemented)
 *  The number of each type of directory file record (0x101, 0x201, 0x401)
 *  is displayed.
 *
 *  Global Used:
 *      tgtpath contains the file passed on the command line
 *
 *  Returns:
 *      FAILURE if the directory provided cannot be opened
 *      SUCCESS if the directory can be opened (and so was checked)
 */

int parse_cop_file( void )
{
    FILE *      current_file    = NULL;
    uint16_t    entry_count;
    char        file_type;
    int         retval;    

    /* Open the file */

    fopen_s( &current_file, tgtpath, "rb" );
    if( current_file == NULL ) return( FAILURE );

    /* Process the file */

    retval = parse_header( current_file, &file_type );
    if(retval == FAILURE)
    {
        printf_s( "%s is not a valid .COP file\n", tgtpath );
        return( FAILURE) ;
    }
    switch( file_type ) {
    case( 0x02 ): 
    /* This is a version 3.33 directory file */
        retval = get_entry_count( current_file, &entry_count );
        if(retval == FAILURE) return( FAILURE );
        check_directory( current_file, entry_count );
        break;
    case( 0x03 ):
    /* This type not yet processed */
        printf_s( "%s: device/directory/font files not yet supported\n", tgtpath );
        break;
    case( 0x04 ): 
    /* This is a version 4.1 directory file */
        retval = get_entry_count( current_file, &entry_count );
        if(retval == FAILURE) return( FAILURE );
        check_directory( current_file, entry_count );
        break;
    default:
        printf_s( "Unknown file type: %i\n", tgtpath, retval );
    }
    fclose( current_file );
    current_file = NULL;

    return( SUCCESS );
}

/*
 *  Print the banner to the screen
 */

void print_banner( void )
{
    puts( banner1w( "Script/GML Binary File Parser Program", _RESEARCH_VERSION_ ) );
    puts( banner2( "1983" ) );
    puts( banner3 );
    puts( banner3a );
}

/*
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

/*
 *  Checks the entry types. This function will print a message if an unknown
 *  entry type is found and prints the total number of each known type after
 *  processing the last entry. The check starts with the entry pointed to by
 *  in_file and only checks count entries.
 *
 *  Parameters:
 *      in_file points to the start of the first directory entry to check
 *      count contains the number of entries to check
 */

void check_directory( FILE * in_file, uint32_t count)
{
    int                 dev_file_count  = 0; /* counts entries of type 0x101 */
    int                 drv_file_count  = 0; /* counts entries of type 0x201 */
    compact_entry_t     compact_entry;
    uint16_t            entry_type;
    extended_entry_t    extended_entry;
    int                 fon_file_count  = 0; /* counts entries of type 0x401 */
    int                 i;
    int                 retval;

    /* Tabulate the entry types */

    for( i = 0; i < count; i++) {
        retval = get_entry_type( in_file, &entry_type );
        if( retval == FAILURE ) {
            puts( "File error or EOF: entry counts may not match expected total");
            break;
        }
        switch( entry_type) {
        case 0x0000:
            i--;    /* no actual entry found, reset counter */
            break;    
        case 0x0001:
            for( ;; ) { /* ensure loop is always exited at some point */
                retval = get_entry_type( in_file, &entry_type );
                if( retval == FAILURE ) {
                    puts( "File error or EOF: entry counts may not match expected total");
                    break;
                }
                switch( entry_type) {
                case 0x0000:
                    i--;    /* no actual entry found, reset counter */
                    break;    
                case 0x0001: /* the only case where the loop is not exited */
                    continue;    
                case 0x0101:
                    dev_file_count++;   /* entry is for a device file */
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                            printf_s( "No data for device entry %i of type %i\n", i+1, entry_type );
                    printf_s( "Entry: %i Device Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    break;
                case 0x0201:
                    drv_file_count++;   /* entry is for a driver file */
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                            printf_s( "No data for device entry %i of type %i\n", i+1, entry_type );
                    printf_s( "Entry: %i Driver Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    break;
                case 0x0401:
                    fon_file_count++;   /* entry is for a font file */
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                            printf_s( "No data for device entry %i of type %i\n", i+1, entry_type );
                    printf_s( "Entry: %i Font Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    break;
                default:
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                        printf_s( "No data for unknown entry %i of type %i\n", i+1, entry_type);
                    printf_s( "Entry: %i Unknown Item Type: %i Name: %20s File Name: %s\n", i+1, entry_type, extended_entry.item_name, extended_entry.file_name );
                }
                break; /* exits unless entry_type is 0x0001 */
            }
            break;
        case 0x0101:
            dev_file_count++;   /* entry is for a device file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            printf_s( "Entry: %i Device Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            break;
        case 0x0201:
            drv_file_count++;   /* entry is for a driver file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            printf_s( "Entry: %i Driver Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            break;
        case 0x0401:
            fon_file_count++;   /* entry is for a font file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            printf_s( "Entry: %i Font Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            break;
        default:
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for unknown entry %i of type %i\n", i+1, entry_type);
            printf_s( "Entry: %i Unknown Item Type: %i Name: %20s File Name: %s\n", i+1, entry_type, compact_entry.item_name, compact_entry.file_name );
      }
        if( retval == FAILURE ) {
            puts( "File error or EOF: entry counts may not match expected total");
            break;
        }
    }

    /* Report the number of each type of entry */

    printf_s( "Device entries: %i\n", dev_file_count );
    printf_s( "Driver entries: %i\n", drv_file_count );
    printf_s( "Font entries:   %i\n", fon_file_count );

    return;
}


