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
*                   display_device()
*                   display_driver()
*                   parse_cop_file()
*                   print_banner()
*                   print_usage()
*                   verify_device()
*                   verify_driver()
*                   verify_font()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdint.h>
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
static void display_device( cop_device *);
static void display_driver( cop_driver *);
static int  verify_device( char *, char * );
static int  verify_driver( char *, char * );
static int  verify_font( char *, char * );

/*  Function definitions */

/*  Function parse_cop_file().
 *  Verify that the file provided to the program is a .COP file and parse it
 *  if it is.
 *  This version only actually parses directory and device files.
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
    cop_device *    current_device  = NULL;
    cop_driver *    current_driver  = NULL;
    FILE       *    current_file    = NULL;
    char            designator[4];
    uint16_t        entry_count;
    char            file_type;
    int             retval;    

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
            current_device = parse_device( current_file );
            if( current_device ) display_device( current_device );
            break;
        }
        fseek( current_file, -3, SEEK_CUR ); /* Reset file to designator */
        if( is_drv_file( current_file ) ) {
            printf_s( "%s is a driver file\n", tgt_path );
            current_driver = parse_driver( current_file );
            if( current_driver ) display_driver( current_driver );
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
    int                 bad_file_count  = 0;  /* counts files with errors */
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
    int                 mismatch_count  = 0;  /* counts files with wrong designator */
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

/*  Function display_device().
 *  Displays the contents of a cop_device instance.
 *
 *  Parameter:
 *      in_device is a pointer to the cop_device instance.
 */

void display_device( cop_device * in_device)
{
    char        font_character[2];
    int         i;
    int         j;
    char        translation[2];

    printf_s( "Allocated size:            %i\n", in_device->allocated_size );
    printf_s( "Bytes used:                %i\n", in_device->next_offset );
    if( in_device->driver_name == NULL ) puts( "Driver Name:");
    else printf_s( "Driver Name:               %s\n", in_device->driver_name );
    if( in_device->output_name == NULL ) puts( "Output File Name:" );
    else printf_s( "Output File Name:          %s\n", in_device->output_name );
    if( in_device->output_extension == NULL ) puts( "Output File Extension:" );
    else printf_s( "Output File Extension:     %s\n", in_device->output_extension );
    printf_s( "Page Width:                %i\n", in_device->page_width );
    printf_s( "Page Depth:                %i\n", in_device->page_depth );
    printf_s( "Horizontal Base Units:     %i\n", in_device->horizontal_base_units );
    printf_s( "Vertical Base Units:       %i\n", in_device->vertical_base_units );
    printf_s( "Page Start X Value:        %i\n", in_device->x_start );
    printf_s( "Page Start Y Value:        %i\n", in_device->y_start );
    printf_s( "Page Offset X Value:       %i\n", in_device->x_offset );
    printf_s( "Page Offset Y Value:       %i\n", in_device->y_offset );
    if( in_device->box.font_name == NULL ) printf_s( "Box Font Number:           %i\n", in_device->box.font_number);
    else printf_s( "Box Font Name:             %s\n", in_device->box.font_name );
    printf_s( "Horizontal line character: %c\n", in_device->box.horizontal_line );
    printf_s( "Vertical line character:   %c\n", in_device->box.vertical_line );
    printf_s( "Top left character:        %c\n", in_device->box.top_left );
    printf_s( "Top right character:       %c\n", in_device->box.top_right );
    printf_s( "Bottom left character:     %c\n", in_device->box.bottom_left );
    printf_s( "Bottom right character:    %c\n", in_device->box.bottom_right );
    printf_s( "Top join character:        %c\n", in_device->box.top_join );
    printf_s( "Bottom join character:     %c\n", in_device->box.bottom_join );
    printf_s( "Left join character:       %c\n", in_device->box.left_join );
    printf_s( "Right join character:      %c\n", in_device->box.right_join );
    printf_s( "Inside join character:     %c\n", in_device->box.inside_join );
    if( in_device->underscore.font_name == NULL ) printf_s( "Underscore Font Number:    %i\n", in_device->underscore.font_number);
    else printf_s( "Underscore Font Name:      %s\n", in_device->underscore.font_name );
    printf_s( "Underscore character:      %c\n", in_device->underscore.underscore_char );
    if( in_device->intrans == NULL) {
        puts( "No Intrans Table");
    } else {
        puts( "Intrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_device->intrans->table[i] != i ) {
                display_char( font_character, (char) i );
                display_char( translation, in_device->intrans->table[i] );
                printf_s( "%c%c %c%c\n", font_character[0], font_character[1], translation[0], translation[1] );
            }
        }
    }
    if( in_device->outtrans == NULL) {
        puts( "No Outtrans Table");
    } else {
        puts( "Outtrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_device->outtrans->table[i] != NULL ) {
                display_char( font_character, (char) i );
                printf_s( "%c%c ", font_character[0], font_character[1] );
                for( j = 0; j < in_device->outtrans->table[i]->count; j++ ) {
                    display_char( translation, in_device->outtrans->table[i]->data[j] );
                    printf_s( "%c%c ", translation[0], translation[1] );
                }
                puts( "" );
            }
        }
    }
    printf_s( "Number of Default Fonts: %i\n", in_device->defaultfonts.count );
    for( i = 0; i < in_device->defaultfonts.count; i++ ) {
        printf_s( "  Default Font Number  %i:\n", i );
        if( in_device->defaultfonts.font[i].font_name == NULL ) puts( "    Font Name:");
        else printf_s( "    Font Name:         %s\n", in_device->defaultfonts.font[i].font_name );
        if( in_device->defaultfonts.font[i].font_style == NULL ) puts( "    FontStyle:");
        else printf_s( "    Font Style:        %s\n", in_device->defaultfonts.font[i].font_style );
        printf_s( "    Font Height:       %i\n", in_device->defaultfonts.font[i].font_height );
        printf_s( "    Font Space:        %i\n", in_device->defaultfonts.font[i].font_space );
    }
    if( in_device->pauses.startpause == NULL ) puts( "No START Pause" );
    else {
        puts( "START Pause:" );
        display_hex_block( in_device->pauses.startpause, in_device->pauses.startpause_count );
    }
    if( in_device->pauses.documentpause == NULL ) puts( "No DOCUMENT Pause" );
    else {
        puts( "DOCUMENT Pause:" );
        display_hex_block( in_device->pauses.documentpause, in_device->pauses.documentpause_count );
    }
    if( in_device->pauses.docpagepause == NULL ) puts( "No DOCUMENT_PAGE Pause" );
    else {
        puts( "DOCUMENT_PAGE Pause:" );
        display_hex_block( in_device->pauses.docpagepause, in_device->pauses.docpagepause_count );
    }
    if( in_device->pauses.devpagepause == NULL ) puts( "No DEVICE_PAGE Pause" );
    else {
        puts( "DEVICE_PAGE Pause:" );
        display_hex_block( in_device->pauses.devpagepause, in_device->pauses.devpagepause_count );
    }
    printf_s( "Number of Device Fonts: %i\n", in_device->devicefonts.count );
    for( i = 0; i < in_device->devicefonts.count; i++ ) {
        printf_s( "  Device Font Index:   %i:\n", i );
        if( in_device->devicefonts.font[i].font_name == NULL ) puts( "    Font Name:");
        else printf_s( "    Font Name:         %s\n", in_device->devicefonts.font[i].font_name );
        if( in_device->devicefonts.font[i].font_switch == NULL ) puts( "    Font Switch:");
        else printf_s( "    Font Switch:       %s\n", in_device->devicefonts.font[i].font_switch );
        printf_s( "    Resident Font:     %i\n", in_device->devicefonts.font[i].resident );
        if( in_device->devicefonts.font[i].fontpause == NULL ) puts( "    No Font Pause" );
        else {
            puts( "    Font Pause:" );
            display_hex_block( in_device->devicefonts.font[i].fontpause, in_device->devicefonts.font[i].fontpause_count );
        }
    }

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
    char            designator[4];
    char            file_name[_MAX_PATH];
    char            type;
    cop_device *    current_device = NULL;
    FILE *          device_file = NULL;
    
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
        fclose( device_file );
        return( BAD_HEADER );
    }
    
    /* Perform the test and parse the file if appropriate */
    if( is_dev_file( device_file ) ) {
        current_device = parse_device( device_file );
        if( current_device ) display_device( current_device );
        fclose( device_file );
        return( GOOD_MATCH );
    }

    /* Report the mismatch */
    fseek( device_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, device_file );
    if( ferror( device_file ) || feof( device_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        fclose( device_file );
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a device file: %s\n", tgt_path, designator );
    fclose( device_file );
    return( BAD_MATCH );
}

/*  Function display_driver().
 *  Displays the contents of a cop_driver instance.
 *
 *  Parameter:
 *      in_driver is a pointer to the cop_driver instance.
 */

void display_driver( cop_driver * in_driver )
{
    int i;
    int j;
    
    printf_s( "Allocated size:            %i\n", in_driver->allocated_size );
    printf_s( "Bytes used:                %i\n", in_driver->next_offset );
    if( in_driver->rec_spec == NULL ) puts( "Record Specification:");
    else printf_s( "Record Specification:      %s\n", in_driver->rec_spec );
    printf_s( "Fill Character:            %c\n", in_driver->fill_char );
    puts( "Page Address Flags:");
    printf_s( "  x_positive:              %i\n", in_driver->x_positive );
    printf_s( "  y_positive:              %i\n", in_driver->y_positive );
    puts( "Start :INIT Block:" );
    if( in_driver->init.start_initblock != NULL ) {
        for( i = 0; i < in_driver->init.start_initblock->count; i++ ) {
            if( in_driver->init.start_initblock->codetext[i].is_fontvalue ) puts( ":FONTVALUE Block:");
            else puts( ":VALUE Block:");
            display_hex_block( in_driver->init.start_initblock->codetext[i].text, in_driver->init.start_initblock->codetext[i].count );
        }
    }
    puts( "Document :INIT Block:" );
    if( in_driver->init.document_initblock != NULL ) {
        for( i = 0; i < in_driver->init.document_initblock->count; i++ ) {
            if( in_driver->init.document_initblock->codetext[i].is_fontvalue ) puts( ":FONTVALUE Block:");
            else puts( ":VALUE Block:");
            display_hex_block( in_driver->init.document_initblock->codetext[i].text, in_driver->init.document_initblock->codetext[i].count );
        }
    }
    puts( "End :FINISH Block:" );
    if( in_driver->finish.end_finishblock != NULL ) {
        display_hex_block( in_driver->finish.end_finishblock->text, in_driver->finish.end_finishblock->count );
    }
    puts( "Document :FINISH Block:" );
    if( in_driver->finish.document_finishblock != NULL ) {
        display_hex_block( in_driver->finish.document_finishblock->text, in_driver->finish.document_finishblock->count );
    }
    if( in_driver->newline.newlineblock == NULL ) puts( ":NEWLINE Block:");
    else {
        puts( ":NEWLINE Block(s):" );
        for( i = 0; i < in_driver->newline.count; i++ ) {
            printf_s( "  Advance: %i\n", in_driver->newline.newlineblock[i].advance );
            if( in_driver->newline.newlineblock[i].text != NULL ) {
                display_hex_block( in_driver->newline.newlineblock[i].text, in_driver->newline.newlineblock[i].count );
            }
        }
    }
    puts( ":NEWPAGE Block:" );
    if( in_driver->newpage.text != NULL ) {
        display_hex_block( in_driver->newpage.text, in_driver->newpage.count );
    }
    puts( ":HTAB Block:" );
    if( in_driver->htab.text != NULL ) {
        display_hex_block( in_driver->htab.text, in_driver->htab.count );
    }
    if( in_driver->fontswitch.fontswitchblock == NULL ) puts( ":FONTSWITCH Block:");
    else {
        puts( ":FONTSWITCH Block(s):" );    
        for( i = 0; i < in_driver->fontswitch.count; i++ ) {
            printf_s( "  Type: %s\n", in_driver->fontswitch.fontswitchblock[i].type );
            if( in_driver->fontswitch.fontswitchblock[i].startvalue != NULL ) {
                puts( "  :STARTVALUE Block:");
                display_hex_block( in_driver->fontswitch.fontswitchblock[i].startvalue->text, in_driver->fontswitch.fontswitchblock[i].startvalue->count );
            }
            if( in_driver->fontswitch.fontswitchblock[i].endvalue != NULL ) {
                puts( "  :ENDVALUE Block:");
                display_hex_block( in_driver->fontswitch.fontswitchblock[i].endvalue->text, in_driver->fontswitch.fontswitchblock[i].endvalue->count );
            }
        }
    }
    if( in_driver->fontstyle.fontstyle == NULL ) puts( ":FONTSTYLE Block:");
    else {
        puts( ":FONTSTYLE Block(s):" );    
        for( i = 0; i < in_driver->fontstyle.count; i++ ) {
            printf_s( "  Type: %s\n", in_driver->fontstyle.fontstyle[i].type );
            if( in_driver->fontstyle.fontstyle[i].startvalue == NULL ) {
                puts( "  No :STARTVALUE Block");
            } else {
                puts( "  :STARTVALUE Block:");
                display_hex_block( in_driver->fontstyle.fontstyle[i].startvalue->text, in_driver->fontstyle.fontstyle[i].startvalue->count );
            }
            if( in_driver->fontstyle.fontstyle[i].endvalue == NULL ) {
                puts( "  No :ENDVALUE Block");
            } else {
                puts( "  :ENDVALUE Block:");
                display_hex_block( in_driver->fontstyle.fontstyle[i].endvalue->text, in_driver->fontstyle.fontstyle[i].endvalue->count );
            }
            if(in_driver->fontstyle.fontstyle[i].lineprocs == NULL ) {
                puts( "  No :LINEPROC Blocks");
            } else {
                puts( "  :LINEPROC Block(s):");
                for( j = 0; j < in_driver->fontstyle.fontstyle[i].passes; j++ ) { 
                    printf_s( "  Pass: %i\n", j+1 );
                    if( in_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue == NULL ) {
                        puts( "  No :STARTVALUE Block");
                    } else {
                        puts( "  :STARTVALUE Block:");
                        display_hex_block( in_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue->text, in_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue->count );
                    }
                    if( in_driver->fontstyle.fontstyle[i].lineprocs[j].firstword == NULL ) {
                        puts( "  No :FIRSTWORD Block");
                    } else {
                        puts( "  :FIRSTWORD Block:");
                        display_hex_block( in_driver->fontstyle.fontstyle[i].lineprocs[j].firstword->text, in_driver->fontstyle.fontstyle[i].lineprocs[j].firstword->count );
                    }
                    if( in_driver->fontstyle.fontstyle[i].lineprocs[j].startword == NULL ) {
                        puts( "  No :STARTWORD Block");
                    } else {
                        puts( "  :STARTWORD Block");
                        display_hex_block( in_driver->fontstyle.fontstyle[i].lineprocs[j].startword->text, in_driver->fontstyle.fontstyle[i].lineprocs[j].startword->count );
                    }
                    if( in_driver->fontstyle.fontstyle[i].lineprocs[j].endword == NULL ) {
                        puts( "  No :ENDWORD Block");
                    } else {
                        puts( "  :ENDWORD Block:");
                        display_hex_block( in_driver->fontstyle.fontstyle[i].lineprocs[j].endword->text, in_driver->fontstyle.fontstyle[i].lineprocs[j].endword->count );
                    }
                    if( in_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue == NULL ) {
                        puts( "  No :ENDVALUE Block");
                    } else {
                        puts( "  :ENDVALUE Block:");
                        display_hex_block( in_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue->text, in_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue->count );
                    }
                }
            }
        }
    }
    if( in_driver->absoluteaddress.text == NULL ) {
        puts( "No :ABSOLUTEADDRESS Block" );
    } else {
    puts( ":ABSOLUTEADDRESS Block:" );
        display_hex_block( in_driver->absoluteaddress.text, in_driver->absoluteaddress.count );
    }
    if( in_driver->hline.text == NULL ) {
        puts( "No :HLINE Block" );
    } else {
    puts( ":HLINE Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->hline.thickness );
        display_hex_block( in_driver->hline.text, in_driver->hline.count );
    }
    if( in_driver->vline.text == NULL ) {
        puts( "No :VLINE Block" );
    } else {
    puts( ":VLINE Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->vline.thickness );
        display_hex_block( in_driver->vline.text, in_driver->vline.count );
    }
    if( in_driver->dbox.text == NULL ) {
        puts( "No :DBOX Block" );
    } else {
    puts( ":DBOX Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->dbox.thickness );
        display_hex_block( in_driver->dbox.text, in_driver->dbox.count );
    }

    return;
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
    char            designator[4];
    char            file_name[_MAX_PATH];
    char            type;
    cop_driver *    current_driver = NULL;
    FILE *          driver_file = NULL;

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
        fclose( driver_file );
        return( BAD_HEADER );
    }
    
    /* Perform the test and parse the file if appropriate */
    if( is_drv_file( driver_file ) ) {
        current_driver = parse_driver( driver_file );
        if( current_driver ) display_driver( current_driver );
        fclose( driver_file );
        return( GOOD_MATCH );
    }

    /* Report the mismatch */
    fseek( driver_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, driver_file );
    if( ferror( driver_file ) || feof( driver_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        fclose( driver_file );
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a driver file: %s\n", tgt_path, designator );
    fclose( driver_file );
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
        fclose( font_file );
        return( BAD_HEADER );
    }
    
    /* Perform the test */
    if( is_fon_file( font_file ) ) {
        fclose( font_file );
        return( GOOD_MATCH );
    }
    
    /* Report the mismatch */
    fseek( font_file, -3, SEEK_CUR ); /* Reset file to designator */
    fread( &designator, 3, 1, font_file );
    if( ferror( font_file ) || feof( font_file ) ) {
        puts("Incorrect file type: file error on attempt to get designator");
        fclose( font_file );
        return( READ_ERROR );
    }
    designator[3] = NULLCHAR;
    printf_s( "%s has incorrect designator for a font file: %s\n", tgt_path, designator );
    fclose( font_file );
    return( BAD_MATCH );
}

