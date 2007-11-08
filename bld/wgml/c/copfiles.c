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
* Description:  Implements the functions used by wgml as such to
*               parse and interpret the information from .COP files:
*                   get_cop_file()
*                   get_cop_device()
*                   get_cop_driver()
*                   get_cop_font()
*                   parse_header()
*
*               and these items which might have a more general application:
*                   directory_list
*                   gml_lib_dirs
*                   gml_inc_dirs
*                   path_dirs
*                   initialize_directory_list()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>

#include "copdir.h"
#include "copfiles.h"
#include "gvars.h"

/* These items might more properly be globals, used by all of wgml and gendev */

/* The count contains the number of directories */

typedef struct {
    uint16_t count;
    char * * directories;
} directory_list;

/* These should be globals, or part of a wgml/gendev file search module */

static  directory_list  gml_lib_dirs;
static  directory_list  gml_inc_dirs;
static  directory_list  path_dirs;

/* Function initialize_directory_list()
 * Initializes in_directory_list from in_path_list.
 *
 * Parameters:
 *      in_path_list is a string containing the contents of a path list.
 *      in_directory_list is a pointer to the directory list to initialize.
 *
 * Modified Parameter:
 *      *in_directory_list is modified as shown below.
 *
 * Notes:
 *      in_directory_list should be in its as-created state.
 *      If in_path_list is NULL, *in_directory_list will not be changed.
 *      If *in_directory_list->directories is not NULL on exit, it is a 
 *          single block of memory and can be freed with one statement.
 */
 
static void initialize_directory_list( char const * in_path_list, \
                                        directory_list * in_list )
{
    char *          current;
    char * *        array_base;
    directory_list  local_list;
    int             i;
    int             j;
    int             k;
    uint16_t        byte_count;
    uint16_t        path_count;

    /* If in_path_list is NULL, return at once. */

    if( in_path_list == NULL ) return;

    /* Determine the number of paths and the total length needed. */

    byte_count = 0;
    path_count = 0; 
    for( i = 0; i < strlen( in_path_list ); i++ ) {
        if( in_path_list[ i ] == '"' ) {
            for( j = i + 1; j < strlen( in_path_list ); j++ ) {
                if( in_path_list[ j ] == '"' ) break;
                byte_count++;
            }    
            i = j;
            if( in_path_list[ i ] == '\0' ) break;
            continue;
        }
        if( in_path_list[ i ] == INCLUDE_SEP ) {
            if( in_path_list[ i - 1 ] != INCLUDE_SEP ) {
                path_count++;
                if( in_path_list[ i - 1 ] == '"' ) {
                    if( in_path_list[ i - 2 ] != PATH_SEP[ 0 ] ) byte_count++;
                }
                if( in_path_list[ i - 1 ] != PATH_SEP[ 0 ] ) byte_count++;
                continue;
            }
        }
        byte_count++;
    }
    if( in_path_list[ i - 1 ] != INCLUDE_SEP ) {
        path_count++;
        if( in_path_list[ i - 1 ] == '"' ) {
            if( in_path_list[ i - 2 ] != PATH_SEP[ 0 ] ) byte_count++;
        }
        if( in_path_list[ i - 1 ] != PATH_SEP[ 0 ] ) byte_count++;
    }

    /* Initialize local_list. */

    local_list.count = path_count;
    local_list.directories = (char * *) malloc( \
        (path_count * sizeof( char * )) + byte_count + path_count );

    array_base = local_list.directories;
    current = (char *) array_base + (path_count * sizeof( char * ));

    k = 0;
    array_base[ 0 ] = current;
    for( i = 0; i < strlen( in_path_list ); i++ ) {
        if( in_path_list[ i ] == '"' ) {
            for( j = i + 1; j < strlen( in_path_list ); j++ ) {
                if( in_path_list[ j ] == '"' ) break;
                *current++ = in_path_list[ j ];
            }    
            i = j;
            if( in_path_list[ i ] == '\0' ) break;
            continue;
        }
        if( in_path_list[ i ] == INCLUDE_SEP ) {
            if( in_path_list[ i - 1 ] != INCLUDE_SEP ) {
                if( in_path_list[ i - 1 ] != PATH_SEP[ 0 ] ) {
                    if( in_path_list[ i - 1 ] == '"' ) {
                        if( in_path_list[ i - 2 ] != PATH_SEP[ 0 ] ) \
                            *current++ = PATH_SEP[0];
                    } else *current++ = PATH_SEP[0];
                }
                *current++ = '\0';
                if( ++k < path_count ) array_base[ k ] = current;
            }
            continue;
        }
        *current++ = in_path_list[ i ];
    }
    if( in_path_list[ i - 1 ] != INCLUDE_SEP ) {
        if( in_path_list[ i - 1 ] == '"' ) {
            if( in_path_list[ i - 2 ] != PATH_SEP[ 0 ] ) \
                *current++ = PATH_SEP[0];
        }
        if( in_path_list[ i - 1 ] != PATH_SEP[ 0 ] ) \
            *current++ = PATH_SEP[0];
        *current++ = '\0';
    }

    /* Check for over-length directory names and remove any found. */

    byte_count = 0;
    path_count = local_list.count;

    for( i = 0; i < local_list.count; i++ ){
        if( strlen( local_list.directories[ i ] ) > _MAX_PATH ) {
            path_count--;
            out_msg( "Directory path is too long (limit is %i):\n  %s\n", \
                _MAX_PATH, local_list.directories[ i ] );
            local_list.directories[ i ] = NULL;
        } else {
            byte_count += strnlen_s( local_list.directories[ i ], _MAX_PATH );
        }
    }

    /* If local_list has no entries at all, clear in_list and return. */

    if( path_count == 0 ) {
        in_list->count = 0;
        mem_free( in_list->directories );
        in_list->directories = NULL;
        return;
    }

    if( path_count < local_list.count) {

        in_list->count = path_count;
        in_list->directories = (char * *) malloc( \
            (path_count * sizeof( char * )) + byte_count + path_count );

        array_base = in_list->directories;
        current = (char *) (array_base + path_count * sizeof( char * ));
 
        byte_count += path_count;
        j = 0;
        for( i = 0; i < local_list.count; i++ ) {
            if( local_list.directories[ i ] != NULL ) {
                array_base[ j ] = current;
                strcpy_s( current, byte_count, local_list.directories[ i ] );
                byte_count -= strlen( local_list.directories[ i ] ) - 1;
                j++;
            }
        }
    } else {
        in_list->count = local_list.count;
        in_list->directories = local_list.directories;
    }

    return;
}

/* Local enum */

typedef enum {
    dir_v4_1_le,        // The file is a little-endian version 4.1 directory file.
    le_v4_1_not_dir,    // The file is a little-endian version 4.1 device, driver, or font file.
    not_le_v4_1,        // The file is not little-endian and/or not version 4.1.
    not_bin_dev,        // The file is not a binary device file at all.
    file_error          // An error occurred while reading the file.
} cop_file_type;

/* Local function definitions */

/* Function parse_header().
 * Determine if the current position of the input stream points to the
 * start of a valid little-endian version 4.1 binary device file and, if 
 * it does, advance the stream to the first byte following the header.
 *
 * Parameter:
 *      in_file points the input stream
 *
 * Returns:
 *      dir_v4_1_le if the file is a little-endian version 4.1 directory file.
 *      le_v4_1_not_dir if the file is a little-endian version 4.1 device,
 *          driver, or font file.
 *      not_le_v4_1 if the file is not little-endian and/or not version 4.1.
 *      not_bin_dev if the file is not a binary device file at all.
 *      file_error if an error occurred while reading the file.
 */

static cop_file_type parse_header( FILE * in_file )
{
    char        count;
    uint16_t    version;

    /* Get the "magic number", which is the version length byte. */

    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    /* If the "magic number" is not equal to 0x02,
     * then this is not a binary device file.
     */

    if( count != 0x02 ) return( not_bin_dev );

    /* Get the version number. */

    fread( &version, 2, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    /* Check for a little_endian version 4.1 header.
    *  Note: checking 0x0c00 would, presumably, identify a big-endian
    *  version 4.1 header, if that ever becomes necessary.
    */
        
    if( version != 0x000c ) return( not_le_v4_1 );

    /* Skip the version text */
    
    /* Get the version length byte */

    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    fseek( in_file, count, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );

    /* Get the file type byte */

    count = fgetc( in_file );

    /* If there is no more data, this is not a valid .COP file */
    
    if( ferror( in_file ) || feof( in_file ) ) return( file_error );
    
    /* Valid header, more data exists, determine the file type */

    if( count == 0x03 ) {
        return( le_v4_1_not_dir );
    } else {
        return( dir_v4_1_le ); 
    }
}

/* Function get_member_name().
 * Searches the given directory for file wgmlst.cop. If the file is found,
 * searches wgmlst.cop for the defined name. If the defined name is found,
 * returns the corresponding member name.
 *
 * Parameters:
 *      in_dir:  the directory to search
 *      in_name: the defined name to match
 *
 * Returns:
 *      on success, the corresponding member name
 *      on failure, a NULL string
 */

static char * get_member_name( char const * in_dir, char const * in_name )
{
    char    *       member_name     = NULL;
    char            filename_buffer[ _MAX_PATH ];
    cop_file_type   file_type;    
    directory_entry current_entry;
    entry_found     entry_status;
    FILE    *       directory_file  = NULL;
    uint16_t        entry_type;

    /* See if in_dir contains a wgmlst.cop file */

    if( (strnlen_s( filename_buffer, _MAX_PATH  ) + strlen( "wgmlst.cop" ) + 1) > _MAX_PATH ) {
        out_msg( "Directory path is too long to be used with 'wgmlst.cop'" \
            "(limit is %i):\n  %s\n", _MAX_PATH - strlen( "wgmlst.cop" ), in_dir );
        return( member_name );
    }

    strcpy_s( filename_buffer, _MAX_PATH, in_dir );
    strcat_s( filename_buffer, _MAX_PATH, "wgmlst.cop" );

    /* Not finding wgmlst.cop is not an error. */

    fopen_s( &directory_file, &filename_buffer, "rb" );
    if( directory_file == NULL ) return( member_name );

    /* Now see in in_name is found in it */

    file_type = parse_header( directory_file );

    switch( file_type ) {
    case not_bin_dev:

        /* The file was not a binary device (.COP) file. */

        out_msg( "This directory contains a file named 'wgmlst.cop', \n" \
            "but that file is not a directory file:\n  %s\n", in_dir );
        break;

    case not_le_v4_1:

        /* The file was not a little-endian version 4.1 file. */

        out_msg( "This directory contains a binary device directory, \n" \
            "but it is either not version 4.1 or not little-endian:\n  %s\n", in_dir );
        break;

    case le_v4_1_not_dir:

        /* The file was a little-endian version 4.1 file, but not a directory file. */

        out_msg( "This directory may contain a binary device directory, \n" \
            "but the file 'wgmlst.cop' is not a directory file:\n  %s\n", in_dir );
        break;

    case dir_v4_1_le:
    
        /* The file was a little-endian version 4.1 directory file */

        /* Skip the two-byte count. */

        fseek( directory_file, sizeof( uint16_t ), SEEK_CUR );
        if( ferror( directory_file ) || feof( directory_file ) ) return( member_name );

        for( ;; ) {

            /* Catch the final entry and exit. */
        
            fread( &entry_type, sizeof( entry_type ), 1, directory_file );
            if( feof( directory_file ) || ferror( directory_file ) ) break;

            switch( entry_type) {

            case 0x0000:

            /* Skip this value if it ever occurs. */

                continue;    

            case 0x0001:

            /* This will be an "extended" entry. */

                for( ;; ) {

                    /* Catch the final entry and exit. */

                    fread( &entry_type, sizeof( entry_type ), 1, directory_file );
                    if( feof( directory_file ) || ferror( directory_file ) ) break;
                
                    switch( entry_type) {

                    case 0x0000:

                        /* If it ever occurs, it should indicate a "compact" entry. */

                        break;    

                    case 0x0001:
                 
                        /* This is the only case where the loop is not exited. */

                        continue;    

                    case 0x0101:
                    case 0x0201:
                    case 0x0401:

                        /* For any type, check the defined name */

                        entry_status = get_extended_entry( directory_file, \
                            &current_entry );
                        switch( entry_status ) {

                        case valid_entry:

                            /* Return the member name, if found. */
       
                            if( !strcmp( in_name, current_entry.defined_name ) ) {
                                fclose( directory_file );
                                directory_file = NULL;
                                member_name = (char *) mem_alloc( strnlen_s( \
                                    current_entry.member_name, _MAX_PATH ) + 1 );
                                strcpy_s( member_name, strnlen_s( \
                                    current_entry.member_name, _MAX_PATH ) + 1, \
                                    current_entry.member_name );
                                return( member_name );
                            }

                            break;

                        case not_valid_entry:

                            /* The entry was incomplete */

                            out_msg("This directory file may be corrupt: %n  %s", \
                                &filename_buffer );

                            break;
                        default:

                            /* The entry status is an unknown value */

                            out_msg("wgml internal error");
                        }
                        break;

                    default:

                        /* The entry type is an unknown value */

                        out_msg("wgml internal error");
                    }
                    break;
                }
                break;

            case 0x0101:
            case 0x0201:
            case 0x0401:

                /* For any type, check the defined name */

                entry_status = get_compact_entry( directory_file, &current_entry );
                switch( entry_status ) {

                case valid_entry:

                    /* Return the member name, if found. */
       
                    if( !strcmp( in_name, current_entry.defined_name) ) {
                        fclose( directory_file );
                        directory_file = NULL;
                        member_name = (char *) mem_alloc( strnlen_s( \
                            current_entry.member_name, _MAX_PATH ) + 1 );
                        strcpy_s( member_name, strnlen_s( \
                            current_entry.member_name, _MAX_PATH ) + 1, \
                            current_entry.member_name );    
                        return( member_name );
                    }

                    break;

                case not_valid_entry:

                    /* The entry was incomplete */

                    out_msg("This directory file may be corrupt: %n  %s", &filename_buffer );

                    break;

                default:

                    /* The entry status is an unknown value */

                    out_msg("wgml internal error");
                }
                break;

            default:

                /* The entry type is an unknown value */

                out_msg("wgml internal error");
          }
        }

        break;

    case file_error:

        out_msg( "This directory may contain a binary device directory, \n" \
            "but a file error occurred when the file 'wgmlst.cop' was being "  \
            "read:\n  %s\n", in_dir );
        break;

    default:

    /* parse_header() returned an unknown value */

        out_msg("wgml internal error");
    }

    fclose( directory_file );
    directory_file = NULL;

    return( member_name );
}

/* Function get_cop_file().
 * Converts the defined name into the fully-qualified name of the file encoding
 * the corresponding :DEVICE, :DRIVER, or :FONT block.
 *
 * Parameter:
 *      in_name: the defined name to convert
 *
 * Globals used:
 *      GMLlibs: the content of the environment variable GML_LIB
 *      GMLincs: the content of the environment variable GML_INC
 *      Pathes:  the content of the environment variable PATH
 *
 * Returns:
 *      on success, the fully-qualified name of the file to search for
 *      on failure, a NULL pointer
 */

static char * get_cop_file( char const * in_name )
{
    char        *   current_path    = NULL;
    char        *   file_name       = NULL;
    char        *   member_name     = NULL;
    int             i;    
    size_t          buffer_length;

    /* Process the directories in GMLlibs. */

    if( GMLlibs !=NULL ) {
        if( gml_lib_dirs.directories == NULL) initialize_directory_list( \
            GMLlibs, &gml_lib_dirs );
        for( i = 0; i < gml_lib_dirs.count; i++ ) {

            /* See if the current directory contains the desired library. */

            member_name = get_member_name( gml_lib_dirs.directories[ i ], in_name );
            if( member_name != NULL ) {
                current_path = gml_lib_dirs.directories[ i ];
                break;
            }
        }
    }
    
    /* Process the directories in GMLincs. */

    if( member_name == NULL ) {
        if( GMLincs !=NULL ) {
            if( gml_inc_dirs.directories == NULL) initialize_directory_list( \
                GMLincs, &gml_inc_dirs );
            for( i = 0; i < gml_inc_dirs.count; i++ ) {

                /* See if the current directory contains the desired library. */

                member_name = get_member_name( gml_inc_dirs.directories[ i ], in_name );
                if( member_name != NULL ) {
                    current_path = gml_inc_dirs.directories[ i ];
                    break;
                }
            }
        }
    }

    /* Process the directories in Pathes. */

    if( member_name == NULL ) {
        if( Pathes !=NULL ) {
            if( path_dirs.directories == NULL) initialize_directory_list( \
                Pathes, &path_dirs );
            for( i = 0; i < path_dirs.count; i++ ) {

                /* See if the current directory contains the desired library. */

                member_name = get_member_name( path_dirs.directories[ i ], in_name );
                if( member_name != NULL ) {
                    current_path = path_dirs.directories[ i ];
                    break;
                }
            }
        }
    }
        
    /* Now combine the member name and the path into name_buffer,
     * adding ".cop" if the member name has no extension.
     */

    if( member_name != NULL ) {
        if( memchr( member_name, '.', strnlen_s( member_name, _MAX_PATH ) ) == \
            NULL ) {
            buffer_length = strnlen_s( gml_lib_dirs.directories[ i ], _MAX_PATH ) \
                + strnlen_s( member_name, _MAX_PATH ) + strlen( ".cop" ) + 1;
            if( buffer_length <= _MAX_PATH ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                strcat_s( file_name, buffer_length, ".cop" );
                return( file_name );
            }
        } else {
            buffer_length = strnlen_s( gml_lib_dirs.directories[ i ], _MAX_PATH ) \
                + strnlen_s( member_name, _MAX_PATH ) + 1;
            if( buffer_length <= _MAX_PATH ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                return( file_name );
            }
        }
            
        /* If we get here, then the file name is too long. */
                
        out_msg( "File name for %s is too long (limit is %i):\n  %s%s\n", in_name, \
            _MAX_PATH, current_path, member_name );
        mem_free( file_name );
        file_name = NULL;
        mem_free( member_name );
        member_name = NULL;
        return( file_name );
    }

    return( file_name );
}

/* Global function definitions */

/* Function get_cop_device().
 * Converts the defined name of a :DEVICE block into a cop_device struct
 * containing the information in that :DEVICE block.
 *
 * Parameter:
 *      in_name: the defined name of the device
 *
 * Return:
 *      on success, a cop_device instance containing the data
 *      on failure, a NULL pointer
 */

cop_device * get_cop_device( char const * in_name )
{
    char        *    file_name   = NULL;
    cop_device  *   out_device  = NULL;

    file_name = get_cop_file( in_name );
    // do all processing, including error messages, requiring file_name
    mem_free( file_name );
    file_name = NULL;

    return( out_device );
}

/* Function get_cop_driver().
 * Converts the defined name of a :DRIVER block into a cop_driver struct
 * containing the information in that :DRIVER block.
 *
 * Parameter:
 *      in_name: the defined name of the device
 *
 * Returns:
 *      on success, a cop_driver instance containing the data
 *      on failure, a NULL pointer
 */

cop_driver * get_cop_driver( char const * in_name )
{
    char        *    file_name   = NULL;
    cop_driver  *   out_driver  = NULL;

    file_name = get_cop_file( in_name );
    // do all processing, including error messages, requiring file_name
    mem_free( file_name );
    file_name = NULL;

    return( out_driver );
}

/* Function get_cop_font().
 * Converts the defined name of a :FONT block into a cop_font struct
 * containing the information in that :FONT block.
 *
 * Parameter:
 *      in_name: the defined name of the font
 *
 * Returns:
 *      on success, a cop_font instance containing the data
 *      on failure, a NULL pointer
 */

cop_font * get_cop_font( char const * in_name )
{
    char        *    file_name   = NULL;
    cop_font    *   out_font    = NULL;

    file_name = get_cop_file( in_name );
    // do all processing, including error messages, requiring file_name
    mem_free( file_name );
    file_name = NULL;

    return( out_font );
}

