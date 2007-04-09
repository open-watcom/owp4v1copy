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
*                   check_directory()
*                   parse_cop_file()
*                   print_banner()
*                   print_usage()
*                   verify_device()
*                   verify_driver()
*                   verify_font()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* wstd.h must preceed cfdev.h,cfdrv.h,cffon.h to avoid conflict with */
/* stdbool.h, which those headers include */

#include "wstd.h"
#include "banner.h"
#include "cfdev.h"
#include "cfdir.h"
#include "cfdrv.h"
#include "cffon.h"
#include "cfheader.h"
#include "cfparse.h"
#include "common.h"
#include "research.h"

/*  Return values for the verify_ local functions */

#define OPEN_ERROR 0
#define BAD_HEADER 1
#define READ_ERROR 2
#define BAD_MATCH  3
#define GOOD_MATCH 4

/*  Local variables */

/*  Load the usage text array */

static  char const *    usage_text[] = {
#include "cfpusage.h"
NULL
};

/*  Local function declarations */

static void check_directory( FILE *, uint32_t);
static int  verify_device( char *, char * );
static int  verify_driver( char *, char * );
static int  verify_font( char *, char * );

/*  Function definitions */

/*  Function parse_cop_file().
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
 *      tgt_path contains the file passed on the command line
 *
 *  Returns:
 *      FAILURE if the directory provided cannot be opened
 *      SUCCESS if the directory can be opened (and so was checked)
 */

int parse_cop_file( void )
{
    FILE *      current_file    = NULL;
    uint16_t    entry_count;
    char        designator[4];
    char        file_type;
    int         retval;    

    /* Open the file */

    fopen_s( &current_file, tgt_path, "rb" );
    if( current_file == NULL ) return( FAILURE );

    /* Process the file */

    retval = parse_header( current_file, &file_type );
    if(retval == FAILURE)
    {
        printf_s( "%s is not a valid .COP file\n", tgt_path );
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
        if( is_dev_file( current_file ) ) {
            printf_s( "%s is a device file\n", tgt_path );
            break;
        }
        fseek( current_file, -3, SEEK_CUR ); /* Reset file to designator */
        if( is_drv_file( current_file ) ) {
            printf_s( "%s is a driver file\n", tgt_path );
            break;
        }
        fseek( current_file, -3, SEEK_CUR ); /* Reset file to designator */
        if( is_fon_file( current_file ) ) {
            printf_s( "%s is a font file\n", tgt_path );
            break;
        }
        fseek( current_file, -3, SEEK_CUR ); /* Reset file to designator */
        fread( &designator, 3, 1, current_file );
        if( ferror( current_file ) || feof( current_file ) ) {
            puts("Incorrect file type: file error on attempt to get designator");
            break;
        }
        designator[3] = NULLCHAR;
        printf_s("Incorrect file type: %s\n", designator);
        break;
    case( 0x04 ): 
    /* This is a version 4.1 directory file */
        retval = get_entry_count( current_file, &entry_count );
        if(retval == FAILURE) return( FAILURE );
        check_directory( current_file, entry_count );
        break;
    default:
        printf_s( "Unknown file type: %i\n", tgt_path, retval );
    }
    fclose( current_file );
    current_file = NULL;

    return( SUCCESS );
}

/*  Function print_banner().
 *  Print the banner to the screen
 */

void print_banner( void )
{
    puts( banner1w( "Script/GML Binary File Parser Program", _RESEARCH_VERSION_ ) );
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

/*  Function check_directory().
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
    int                 bad_file_count = 0;  /* counts files with errors */
    compact_entry_t     compact_entry;
    int                 dev_file_count  = 0; /* counts entries of type 0x101 */
    char                dir[_MAX_DIR];
    char                drive[_MAX_DRIVE];
    int                 drv_file_count  = 0; /* counts entries of type 0x201 */
    uint16_t            entry_type;
    char                ext[_MAX_EXT];
    extended_entry_t    extended_entry;
    char                file_path[_MAX_PATH];
    char                fname[_MAX_FNAME];
    int                 fon_file_count  = 0; /* counts entries of type 0x401 */
    int                 i;
    int                 mismatch_count = 0;  /* counts files with wrong designator */
    int                 retval;

    /* Split tgt_path */

    _splitpath( tgt_path, drive, dir, fname, ext );

    /* Construct file_path */

    strcpy_s( file_path, sizeof( file_path ), drive );
    strcat_s( file_path, sizeof( file_path ), dir );

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
                    else printf_s( "Entry: %i Device Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    retval = verify_device( &file_path, & extended_entry.file_name );
                    if( retval == BAD_MATCH ) mismatch_count++;
                    else if( retval != GOOD_MATCH) bad_file_count++;                    
                    break;
                case 0x0201:
                    drv_file_count++;   /* entry is for a driver file */
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                            printf_s( "No data for device entry %i of type %i\n", i+1, entry_type );
                    else printf_s( "Entry: %i Driver Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    retval = verify_driver( &file_path, & extended_entry.file_name );
                    if( retval == BAD_MATCH ) mismatch_count++;
                    else if( retval != GOOD_MATCH) bad_file_count++;                    
                    break;
                case 0x0401:
                    fon_file_count++;   /* entry is for a font file */
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                            printf_s( "No data for device entry %i of type %i\n", i+1, entry_type );
                    else printf_s( "Entry: %i Font Name: %s File Name: %s\n", i+1, extended_entry.item_name, extended_entry.file_name );
                    retval = verify_font( &file_path, & extended_entry.file_name );
                    if( retval == BAD_MATCH ) mismatch_count++;
                    else if( retval != GOOD_MATCH) bad_file_count++;                    
                    break;
                default:
                    retval = get_extended_entry( in_file, &extended_entry );
                    if( retval == FAILURE ) \
                        printf_s( "No data for unknown entry %i of type %i\n", i+1, entry_type);
                    else printf_s( "Entry: %i Unknown Item Type: %i Name: %20s File Name: %s\n", i+1, entry_type, extended_entry.item_name, extended_entry.file_name );
                }
                break; /* exits unless entry_type is 0x0001 */
            }
            break;
        case 0x0101:
            dev_file_count++;   /* entry is for a device file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            else printf_s( "Entry: %i Device Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            retval = verify_device( &file_path, & compact_entry.file_name );
            if( retval == BAD_MATCH ) mismatch_count++;
            else if( retval != GOOD_MATCH) bad_file_count++;                    
            break;
        case 0x0201:
            drv_file_count++;   /* entry is for a driver file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            else printf_s( "Entry: %i Driver Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            retval = verify_driver( &file_path, & compact_entry.file_name );
            if( retval == BAD_MATCH ) mismatch_count++;
            else if( retval != GOOD_MATCH) bad_file_count++;                    
            break;
        case 0x0401:
            fon_file_count++;   /* entry is for a font file */
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for entry %i of type %i\n", i+1, entry_type);
            else printf_s( "Entry: %i Font Name: %s File Name: %s\n", i+1, compact_entry.item_name, compact_entry.file_name );
            retval = verify_font( &file_path, & compact_entry.file_name );
            if( retval == BAD_MATCH ) mismatch_count++;
            else if( retval != GOOD_MATCH) bad_file_count++;                    
            break;
        default:
            retval = get_compact_entry( in_file, &compact_entry );
            if( retval == FAILURE ) \
                printf_s( "No data for unknown entry %i of type %i\n", i+1, entry_type);
            else printf_s( "Entry: %i Unknown Item Type: %i Name: %20s File Name: %s\n", i+1, entry_type, compact_entry.item_name, compact_entry.file_name );
      }
    }

    /* Report the number of each type of entry */

    printf_s( "Entries requested: %i\n", count );
    printf_s( "Entries found:     %i\n", dev_file_count + drv_file_count + fon_file_count );
    printf_s( "   of which %i could not be opened or, if opened, read\n", bad_file_count++);
    printf_s( "   and of which %i had a different designator (DEV, DRV, FON) than was expected\n", mismatch_count);
    puts( "By type:");
    printf_s( "Device entries:    %i\n", dev_file_count );
    printf_s( "Driver entries:    %i\n", drv_file_count );
    printf_s( "Font entries:      %i\n", fon_file_count );

    return;
}

/*  Function verify_device().
 *  Verifies that the file is a device file.
 *
 *  Parameter:
 *      in_path contains the path from the command-line parameter
 *      in_file contains the file name from the directory file entry
 *
 *  Returns:
 *      OPEN_ERROR if the file cannot be opened (implies file does not exist)
 *      BAD_HEADER if the file does not start with a valid .COP file header
 *      READ_ERROR if the file cannot be read (implies is not a valid .COP file)
 *      BAD_MATCH  if the bytes that should contain "DEV" contains something else
 *      GOOD_MATCH if the bytes that should contain "DEV" do contain "DEV"
 */
 
int verify_device( char * in_path, char * in_name)
{
    char    designator[4];
    char    file_name[_MAX_PATH];
    char    type;
    FILE *  device_file = NULL;
    
    /* Build the file name */
    strcpy_s( file_name, sizeof( file_name ), in_path );
    strcat_s( file_name, sizeof( file_name ), in_name );
    strcat_s( file_name, sizeof( file_name ), ".COP" );

    /* Open the file */
    fopen_s( &device_file, file_name, "rb" );
    if( device_file == NULL ) {
        printf_s( "Could not open device file %s\n", file_name );
        return( OPEN_ERROR );
    }

    /* Skip the header */

    if( parse_header( device_file, &type ) == FAILURE ) {
        printf_s( "%s is not a .COP file (bad header)\n", file_name );
        return( BAD_HEADER );
    }
    
    /* Perform the test */
    if( is_dev_file( device_file ) ) return( GOOD_MATCH );

    /* Report the mismatch */
    fseek( device_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, device_file );
    if( ferror( device_file ) || feof( device_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a device file: %s\n", tgt_path, designator );
    return( BAD_MATCH );
}

/*  Function verify_driver().
 *  Verifies that the file is a driver file.
 *
 *  Parameter:
 *      in_path contains the path from the command-line parameter
 *      in_file contains the file name from the directory file entry
 *
 *  Returns:
 *      OPEN_ERROR if the file cannot be opened (implies file does not exist)
 *      BAD_HEADER if the file does not start with a valid .COP file header
 *      READ_ERROR if the file cannot be read (implies is not a valid .COP file)
 *      BAD_MATCH  if the bytes that should contain "DRV" contains something else
 *      GOOD_MATCH if the bytes that should contain "DRV" do contain "DRV"
 */
 
int verify_driver( char * in_path, char * in_name )
{
    char    designator[4];
    char    file_name[_MAX_PATH];
    char    type;
    FILE *  driver_file = NULL;

    /* Build the file name */
    strcpy_s( file_name, sizeof( file_name ), in_path );
    strcat_s( file_name, sizeof( file_name ), in_name );
    strcat_s( file_name, sizeof( file_name ), ".COP" );

    /* Open the file */
    fopen_s( &driver_file, file_name, "rb" );
    if( driver_file == NULL ) {
        printf_s( "Could not open driver file %s\n", file_name );
        return( OPEN_ERROR );
    }

    /* Skip the header */

    if( parse_header( driver_file, &type ) == FAILURE ) {
        printf_s( "%s is not a .COP file (bad header)\n", file_name );
        return( BAD_HEADER );
    }
    
    /* Perform the test */
    if( is_drv_file( driver_file ) ) return( GOOD_MATCH );

    /* Report the mismatch */
    fseek( driver_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, driver_file );
    if( ferror( driver_file ) || feof( driver_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a driver file: %s\n", tgt_path, designator );
    return( BAD_MATCH );
}

/*  Function verify_font().
 *  Verifies that the file is a font file.
 *
 *  Parameter:
 *      in_path contains the path from the command-line parameter
 *      in_file contains the file name from the directory file entry
 *
 *  Returns:
 *      OPEN_ERROR if the file cannot be opened (implies file does not exist)
 *      BAD_HEADER if the file does not start with a valid .COP file header
 *      READ_ERROR if the file cannot be read (implies is not a valid .COP file)
 *      BAD_MATCH  if the bytes that should contain "FON" contains something else
 *      GOOD_MATCH if the bytes that should contain "FON" do contain "FON"
 */
 
int verify_font( char * in_path, char * in_name )
{
    char    designator[4];
    char    file_name[_MAX_PATH];
    char    type;
    FILE *  font_file = NULL;
    
    /* Build the file name */
    strcpy_s( file_name, sizeof( file_name ), in_path );
    strcat_s( file_name, sizeof( file_name ), in_name );
    strcat_s( file_name, sizeof( file_name ), ".COP" );

    /* Open the file */
    fopen_s( &font_file, file_name, "rb" );
    if( font_file == NULL ) {
        printf_s( "Could not open font file %s\n", file_name );
        return( OPEN_ERROR );
    }

    /* Skip the header */

    if( parse_header( font_file, &type ) == FAILURE ) {
        printf_s( "%s is not a .COP file (bad header)\n", file_name );
        return( BAD_HEADER );
    }
    
    /* Perform the test */
    if( is_fon_file( font_file ) ) return( GOOD_MATCH );

    /* Report the mismatch */
    fseek( font_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, font_file );
    if( ferror( font_file ) || feof( font_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a font file: %s\n", tgt_path, designator );
    return( BAD_MATCH );
}


