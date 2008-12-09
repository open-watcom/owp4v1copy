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
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>

#include "copdev.h"
#include "copdrv.h"
#include "copfon.h"
#include "findfile.h"

/* Extern function definitions */

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

/* Function get_cop_device().
 * Converts the defined name of a :DEVICE block into a cop_device struct
 * containing the information in that :DEVICE block.
 *
 * Parameter:
 *      in_name points to the defined name of the device.
 *
 * Return:
 *      on success, a cop_device instance containing the data.
 *      on failure, a NULL pointer.
 */

cop_device * get_cop_device( char const * in_name )
{
    char            *   file_name   = NULL;
    cop_device      *   out_device  = NULL;
    cop_file_type       file_type;
    FILE            *   device_file = NULL;

    /* Acquire the file, if it exists. */

    file_name = get_cop_file( in_name );
    if( file_name == NULL ) {
        out_msg( "No entry for the device %s was found.\n", in_name );
        return( out_device );
    }

    fopen_s( &device_file, file_name, "rb" );
    if( device_file == NULL ) {
        out_msg( "The file for the device %s was not found:\n  %s\n", in_name, \
                                                                    file_name );
        mem_free( file_name );
        file_name = NULL;
        return( out_device );
    }

    /* Determine if the file encodes a :DEVICE block. */
    
    file_type = parse_header( device_file );

    switch( file_type ) {
    case not_bin_dev:

        /* The file was not a binary device (.COP) file. */

        out_msg( "This file is not a binary device file:\n  %s\n", file_name );
        break;

    case not_se_v4_1:

        /* The file was not a same-endian version 4.1 file. */

        out_msg( "This file is a binary device file,\nbut it is either not " \
                        "version 4.1 or not same-endian:\n  %s\n", file_name );
        break;

    case se_v4_1_not_dir:

        /* The file was a same-endian version 4.1 file, but not a directory file. */

        if( !is_dev_file( device_file ) ) {
            out_msg( "This file is supposed to be for the device %s, but it " \
                        "does not encode a device:\n  %s\n", in_name, file_name );
            break;
        }

        out_device = parse_device( device_file );
        if( out_device == NULL ) \
            out_msg( "The file for the device %s appears to be corrupted:\n" \
                                                "  %s\n", in_name, file_name );

        break;

    case dir_v4_1_se:
    
        /* The file was a same-endian version 4.1 directory file. */

        out_msg( "This file is a directory file for a binary device library:" \
                                                        "\n  %s\n", file_name );
        break;

    case file_error:

        /* A file error or premature end-of-file occurred */

        out_msg( "This directory may contain a binary device directory,\nbut a " \
        "file error occurred when this file was being read:\n  %s\n", file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
    }

    fclose( device_file );
    device_file = NULL;
    mem_free( file_name );
    file_name = NULL;

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
    char            *   file_name   = NULL;
    cop_driver      *   out_driver  = NULL;
    cop_file_type       file_type;
    FILE            *   driver_file = NULL;

    /* Acquire the file, if it exists. */

    file_name = get_cop_file( in_name );
    if( file_name == NULL ) {
        out_msg( "No entry for the driver %s was found.\n", in_name );
        return( out_driver );
    }

    fopen_s( &driver_file, file_name, "rb" );
    if( driver_file == NULL ) {
        out_msg( "The file for the driver %s was not found:\n  %s\n", in_name, \
                                                                    file_name );
        mem_free( file_name );
        file_name = NULL;
        return( out_driver );
    }

    /* Determine if the file encodes a :DRIVER block. */
    
    file_type = parse_header( driver_file );

    switch( file_type ) {
    case not_bin_dev:

        /* The file was not a binary device (.COP) file. */

        out_msg( "This file is not a binary device file:\n  %s\n", file_name );
        break;

    case not_se_v4_1:

        /* The file was not a same-endian version 4.1 file. */

        out_msg( "This file is a binary device file,\nbut it is either not " \
                            "version 4.1 or not same-endian:\n  %s\n", file_name );
        break;

    case se_v4_1_not_dir:

        /* The file was a same-endian version 4.1 file,
         * but not a directory file.
         */

        if( !is_drv_file( driver_file ) ) {
            out_msg( "This file is supposed to be for the driver %s, but it " \
                        "does not encode a driver:\n  %s\n", in_name, file_name );
            break;
        }

        out_driver = parse_driver( driver_file );
        if( out_driver == NULL ) \
            out_msg( "The file for the driver %s appears to be corrupted:\n" \
                                                "  %s\n", in_name, file_name );

        break;

    case dir_v4_1_se:
    
        /* The file was a same-endian version 4.1 directory file. */

        out_msg( "This file is a directory file for a binary device library:" \
                                                        "\n  %s\n", file_name );
        break;

    case file_error:

        /* A file error or premature end-of-file occurred */

        out_msg( "This directory may contain a binary device directory,\nbut a " \
        "file error occurred when this file was being read:\n  %s\n", file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
    }

    fclose( driver_file );
    driver_file = NULL;
    mem_free( file_name );
    file_name = NULL;

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
    char            *   file_name   = NULL;
    cop_font        *   out_font    = NULL;
    cop_file_type       file_type;
    FILE            *   font_file   = NULL;

    /* Acquire the file, if it exists. */

    file_name = get_cop_file( in_name );
    if( file_name == NULL ) {
        out_msg( "No entry for the font %s was found.\n", in_name );
        return( out_font );
    }

    fopen_s( &font_file, file_name, "rb" );
    if( font_file == NULL ) {
        out_msg( "The file for the font %s was not found:\n  %s\n", in_name, \
            file_name );
        mem_free( file_name );
        file_name = NULL;
        return( out_font );
    }

    /* Determine if the file encodes a :FONT block. */
    
    file_type = parse_header( font_file );

    switch( file_type ) {
    case not_bin_dev:

        /* The file was not a binary device (.COP) file. */

        out_msg( "This file is not a binary device file:\n  %s\n", file_name );
        break;

    case not_se_v4_1:

        /* The file was not a same-endian version 4.1 file. */

        out_msg( "This file is a binary device file,\nbut it is either not " \
                        "version 4.1 or not same-endian:\n  %s\n", file_name );
        break;

    case se_v4_1_not_dir:

        /* The file was a same-endian version 4.1 file,
         * but not a directory file.
         */

        if( !is_fon_file( font_file ) ) {
            out_msg( "This file is supposed to be for the font %s, but it " \
                        "does not encode a font:\n  %s\n", in_name, file_name );
            break;
        }

        out_font = parse_font( font_file );
        if( out_font == NULL ) \
            out_msg( "The file for the font %s appears to be corrupted:\n" \
                                                "  %s\n", in_name, file_name );

        break;

    case dir_v4_1_se:
    
        /* The file was a same-endian version 4.1 directory file. */

        out_msg( "This file is a directory file for a binary device library:" \
                                                        "\n  %s\n", file_name );
        break;

    case file_error:

        /* A file error or premature end-of-file occurred */

        out_msg( "This directory may contain a binary device directory,\nbut a " \
        "file error occurred when this file was being read:\n  %s\n", file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
    }

    fclose( font_file );
    font_file = NULL;
    mem_free( file_name );
    file_name = NULL;

    return( out_font );
}

