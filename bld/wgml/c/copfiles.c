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
* Description:  Implements the functions used by wgml to parse .COP files:
*                   get_cop_device()
*                   get_cop_driver()
*                   get_cop_font()
*                   parse_header()
*
*               Also these functions for integration with wgml:
*                   cop_setup()
*                   cop_teardown()
*                   get_systime()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <setjmp.h> // Required (but not included) by gvars.h.
#include <stdlib.h>

#include "copdev.h"
#include "copdrv.h"
#include "copfon.h"
#include "findfile.h"
#include "gtype.h" // Required (but not included) by gvars.h.
#include "gvars.h"

/* Extern function definitions */

/* Function get_cop_device().
 * Converts the defined name of a :DEVICE block into a cop_device struct
 * containing the information in that :DEVICE block.
 *
 * Parameter:
 *      in_name points to the defined name of the device.
 *
 * Globals Used:
 *      try_file_name contains the name of the device file, if found.
 *      try_fp contains the FILE * for the device file, if found.
 *
 * Return:
 *      on success, a cop_device instance containing the data.
 *      on failure, a NULL pointer.
 */

cop_device * get_cop_device( char const * in_name )
{
    cop_device      *   out_device  = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the device %s was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :DEVICE block. */
    
    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:
    
        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_device );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_dev_file( try_fp ) ) {
            out_msg( "Device library problem: file given for device %s does not" \
                            " encode a device:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_device = parse_device( try_fp );
        if( out_device == NULL ) \
            out_msg( "Device library problem: file given for device %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_device );
}

/* Function get_cop_driver().
 * Converts the defined name of a :DRIVER block into a cop_driver struct
 * containing the information in that :DRIVER block.
 *
 * Parameter:
 *      in_name points to the defined name of the device.
 *
 * Returns:
 *      on success, a cop_driver instance containing the data.
 *      on failure, a NULL pointer.
 */

cop_driver * get_cop_driver( char const * in_name )
{
    cop_driver      *   out_driver  = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the driver %s was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :DRIVER block. */
    
    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:
    
        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_driver );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_drv_file( try_fp ) ) {
            out_msg( "Device library problem: file given for driver %s does not" \
                            " encode a driver:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_driver = parse_driver( try_fp );
        if( out_driver == NULL ) \
            out_msg( "Device library problem: file given for driver %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_driver );
}

/* Function get_cop_font().
 * Converts the defined name of a :FONT block into a cop_font struct
 * containing the information in that :FONT block.
 *
 * Parameter:
 *      in_name points to the defined name of the font.
 *
 * Returns:
 *      on success, a cop_font instance containing the data.
 *      on failure, a NULL pointer.
 */

cop_font * get_cop_font( char const * in_name )
{
    cop_font        *   out_font    = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the font %s was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :FONT block. */
    
    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:
    
        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_font );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_fon_file( try_fp ) ) {
            out_msg( "Device library problem: file given for font %s does not" \
                            " encode a font:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_font = parse_font( try_fp );
        if( out_font == NULL ) \
            out_msg( "Device library problem: file given for font %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_font );
}

/* Function parse_header().
 * Determine if the current position of the input stream points to the
 * start of a valid same-endian version 4.1 binary device file and, if 
 * it does, advance the stream to the first byte following the header.
 *
 * Parameter:
 *      in_file points the input stream.
 *
 * Returns:
 *      dir_v4_1_se if the file is a same-endian version 4.1 directory file.
 *      se_v4_1_not_dir if the file is a same-endian version 4.1 device,
 *          driver, or font file.
 *      not_se_v4_1 if the file is not same-endian and/or not version 4.1.
 *      not_bin_dev if the file is not a binary device file at all.
 *      file_error if an error occurred while reading the file.
 */

cop_file_type parse_header( FILE * in_file )
{
    char        count;
    char        text_version[0x0b];
    uint16_t    version;

    /* Get the count and ensure it is 0x02. */

    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    if( count != 0x02 ) return( not_bin_dev );

    /* Get the version. */

    fread( &version, 2, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    /* Check for a same_endian version 4.1 header.
    *  Note: checking 0x0c00 would, presumably, identify a different-endian
    *  version 4.1 header, if that ever becomes necessary.
    */
        
    if( version != 0x000c ) return( not_se_v4_1 );

    /* Get the text_version_length and ensure it is 0x0b. */

    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    if( count != 0x0b ) return( not_bin_dev );

    /* Verify the text_version. */

    fread( &text_version, 0x0b, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    text_version[0x0b] = '\0';
    if( strcmp( text_version, "V4.1 PC/DOS" ) ) return( not_bin_dev );

    /* Get the type. */

    count = fgetc( in_file );

    /* If there is no more data, this is not a valid .COP file. */
    
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );
    
    /* Valid header, more data exists, determine the file type. */

    if( count == 0x03 ) {
        return( se_v4_1_not_dir );
    }
    if( count == 0x04 ) {
        return( dir_v4_1_se ); 
    }

    /* Invalid file type: this cannot be a valid .COP file. */

    return( not_bin_dev );
}

/* For integration with wgml. */


static void free_opt_fonts( void )
{
    opt_font    *   current;
    opt_font    *   old;

    if( opt_fonts == NULL) return;

    if( opt_fonts->name != NULL) {
        mem_free( opt_fonts->name );
        opt_fonts->name = NULL;
    }

    if( opt_fonts->style != NULL) {
        mem_free( opt_fonts->style );
        opt_fonts->style = NULL;
    }

    current = opt_fonts->nxt;
    while( current != NULL) {
        old = current;

        if( current->name != NULL) {
            mem_free( current->name );
            current->name = NULL;
        }

        if( current->style != NULL) {
            mem_free( current->style );
            current->style = NULL;
        }

        current = current->nxt;
        mem_free( old );
    }

    mem_free( opt_fonts );
    opt_fonts = NULL;

    return;
}

extern void cop_setup( void )
{
    free_opt_fonts();    
    return;
}

extern void cop_teardown( void )
{
    return;
}

extern void get_systime( void ) {}


