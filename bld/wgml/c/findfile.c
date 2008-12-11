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
* Description:  Implements the global function used by wgml to find and
*               open files:
*                   ff_setup()
*                   ff_teardown()
*                   free_inc_fp()
*                   get_cop_file()
*                   search_file_in_dirs()
*               plus these local items:
*                   cur_dir
*                   cur_dir_list
*                   directory_list
*                   gml_lib_dirs
*                   gml_inc_dirs
*                   initialize_directory_list()
*                   path_dirs

*                   get_member_name()
*                   IS_PATH_SEP()
*                   try_open()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent which is related to the binary device library.
*               This should help in most such cases.
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "copdir.h"
#include "copfiles.h"
#include "wgml.h"
#include "gvars.h"

/*************Valid Stuff Below This Line!*******************/

/* Local #define macro. */

#if defined( __UNIX__ )
    #define IS_PATH_SEP( ch ) ((ch) == '/')
#else
    #define IS_PATH_SEP( ch ) ((ch) == '/' || (ch) == '\\')
#endif

/* Local struct. */

/* The count contains the number of directories. */

typedef struct {
    uint16_t count;
    char * * directories;
} directory_list;

/* Local data. */

static  size_t          env_var_length  = 0;
static  char    *       env_var_buffer  = NULL;
static  char    *       cur_dir = "";
static  directory_list  cur_dir_list;
static  directory_list  gml_lib_dirs;
static  directory_list  gml_inc_dirs;
static  directory_list  path_dirs;

/* Define the global variables. */

#define global
#include "findfile.h"

/*************Not Necessarily Valid Stuff Below This Line!*******************/

/* Local macros. */

/* Local function definitions. */

/* Function get_member_name().
 * Searches the given directory for file wgmlst.cop. If the file is found,
 * searches wgmlst.cop for the defined name. If the defined name is found,
 * returns the corresponding member name.
 *
 * Parameters:
 *      in_dir points to the directory to search.
 *      in_name points to the defined name to match.
 *
 * Returns:
 *      on success, the corresponding member name.
 *      on failure, a NULL pointer.
 */

static char * get_member_name( char const * in_dir, char const * in_name )
{
    char    *       member_name     = NULL;
    char            filename_buffer[FILENAME_MAX];
    cop_file_type   file_type;
    directory_entry current_entry;
    entry_found     entry_status;
    FILE    *       directory_file  = NULL;
    uint16_t        entry_type;

    /* See if in_dir contains a wgmlst.cop file. */

    if( (strnlen_s( filename_buffer, FILENAME_MAX  ) + strlen( "wgmlst.cop" ) \
                                                     + 1) > FILENAME_MAX ) {
        out_msg( "Directory path is too long to be used with 'wgmlst.cop' " \
                "(limit is %i):\n  %s\n", FILENAME_MAX - strlen( "wgmlst.cop" ), \
                                                                        in_dir );
        return( member_name );
    }

    strcpy_s( filename_buffer, FILENAME_MAX, in_dir );
    strcat_s( filename_buffer, FILENAME_MAX, "wgmlst.cop" );

    /* Not finding wgmlst.cop is not an error. */

    fopen_s( &directory_file, &filename_buffer, "rb" );
    if( directory_file == NULL ) return( member_name );

    /* Now see if in_name is found in it. */

    file_type = parse_header( directory_file );

    switch( file_type ) {
    case not_bin_dev:

        /* The file was not a binary device (.COP) file. */

        out_msg( "This directory contains a file named 'wgmlst.cop',\n" \
                    "but that file is not a directory file:\n  %s\n", in_dir );
        break;

    case not_se_v4_1:

        /* The file was not a same-endian version 4.1 file. */

        out_msg( "This directory contains a binary device directory,\n" \
          "but it is either not version 4.1 or not same-endian:\n  %s\n", in_dir );
        break;

    case se_v4_1_not_dir:

        /* The file was a same-endian version 4.1 file, but not a directory file. */

        out_msg( "This directory may contain a binary device directory,\n" \
            "but the file 'wgmlst.cop' is not a directory file:\n  %s\n", in_dir );
        break;

    case dir_v4_1_se:

        /* The file was a same-endian version 4.1 directory file. */

        /* Skip the number of entries. */

        fseek( directory_file, sizeof( uint32_t ), SEEK_CUR );
        if( ferror( directory_file ) || feof( directory_file ) ) break;

        for( ;; ) {

            /* Get the entry_type. This is either the type or the metatype,
             * depending on whether this is a CompactDirEntry or an
             * ExtendedDirEntry.
             */

            fread( &entry_type, sizeof( entry_type ), 1, directory_file );

            /* Exit the loop when the final entry has been processed. */

            if( feof( directory_file ) || ferror( directory_file ) ) break;

            switch( entry_type) {
            case 0x0000:

                /* This should only happen when the end-of-file padding is
                 * reached, but continue in case there is more data.
                 */

                continue;

            case 0x0001:

            /* This will be an ExtendedDirEntry. */

                for( ;; ) {

                    /* Get the entry_type. This is always the type, since the
                     * metatype has already been read.
                     */

                    fread( &entry_type, sizeof( entry_type ), 1, directory_file );

                    /* Exit the loop when the final entry has been processed. */

                    if( feof( directory_file ) || ferror( directory_file ) ) \
                                                                            break;

                    switch( entry_type ) {
                    case 0x0000:

                        /* This should only happen when the end-of-file padding is
                         * reached, but continue in case there is more data.
                         */

                        continue;

                    case 0x0001:

                        /* This should never actually occur; however, continue
                         * in case there is more data.
                         */

                        continue;

                    case 0x0101:
                    case 0x0201:
                    case 0x0401:

                        /* For any type, check the defined name. */

                        entry_status = get_extended_entry( directory_file, \
                                                                &current_entry );
                        switch( entry_status ) {
                        case valid_entry:

                            /* Return the member name, if found. */

                            if( !strcmp( in_name, current_entry.defined_name ) \
                                                                            ) {
                                fclose( directory_file );
                                directory_file = NULL;
                                member_name = (char *) mem_alloc( strnlen_s( \
                            current_entry.member_name, FILENAME_MAX ) + 1 );
                                strcpy_s( member_name, strnlen_s( \
        current_entry.member_name, FILENAME_MAX ) + 1, current_entry.member_name );
                                return( member_name );
                            }

                            break;

                        case not_valid_entry:

                            /* The entry was incomplete. */

                            out_msg("This directory file may be corrupt:\n" \
                                                    "  %s\n", &filename_buffer );

                            break;

                        default:

                            /* The entry_status is an unknown value. */

                            out_msg("wgml internal error\n");
                        }
                        break;

                    default:

                        /* The entry_type is an unknown value. */

                        out_msg("wgml internal error\n");
                    }
                    break;
                }
                break;

            case 0x0101:
            case 0x0201:
            case 0x0401:

                /* For any type, check the defined name. */

                entry_status = get_compact_entry( directory_file, &current_entry );
                switch( entry_status ) {

                case valid_entry:

                    /* Return the member name, if found. */

                    if( !strcmp( in_name, current_entry.defined_name) ) {
                        fclose( directory_file );
                        directory_file = NULL;
                        member_name = (char *) mem_alloc( strnlen_s( \
                                current_entry.member_name, FILENAME_MAX ) + 1 );
                        strcpy_s( member_name, strnlen_s( \
        current_entry.member_name, FILENAME_MAX ) + 1, current_entry.member_name );
                        return( member_name );
                    }

                    break;

                case not_valid_entry:

                    /* The entry was incomplete. */

                    out_msg("This directory file may be corrupt:\n  %s\n", \
                                                            &filename_buffer );

                    break;

                default:

                    /* The entry_status is an unknown value. */

                    out_msg("wgml internal error\n");
                }
                break;

            default:

                /* The entry_type is an unknown value. */

                out_msg("wgml internal error\n");
          }
        }

        break;

    case file_error:

        /* A file error or premature end-of-file occurred. */

        out_msg( "This directory may contain a binary device directory,\n" \
            "but a file error occurred when the file 'wgmlst.cop' was being " \
                                                        "read:\n  %s\n", in_dir );
        break;

    default:

        /* The file_type is an unknown value. */

        out_msg("wgml internal error\n");
    }

    fclose( directory_file );
    directory_file = NULL;

    return( member_name );
}

/* Extern function definition. */

/* Function get_cop_file().
 * Converts the defined name into the fully-qualified name of the file encoding
 * the corresponding :DEVICE, :DRIVER, or :FONT block.
 *
 * Parameter:
 *      in_name points to the defined name to convert.
 *
 * Globals used:
 *      GMLlibs points to the content of the environment variable GML_LIB.
 *      GMLincs points to the content of the environment variable GML_INC.
 *      Pathes points to the content of the environment variable PATH.
 *
 * Returns:
 *      on success, the fully-qualified name of the file to search for.
 *      on failure, a NULL pointer.
 */

extern char * get_cop_file( char const * in_name )
{
    char        *   current_path    = NULL;
    char        *   file_name       = NULL;
    char        *   member_name     = NULL;
    int             i;
    size_t          buffer_length;

    /* Process the directories in gml_lib_dirs. */

    if( gml_lib_dirs.count > 0 ) {
        for( i = 0; i < gml_lib_dirs.count; i++ ) {

            /* See if the current directory contains the desired library. */

            member_name = get_member_name( gml_lib_dirs.directories[i], in_name );
            if( member_name != NULL ) {
                current_path = gml_lib_dirs.directories[i];
                break;
            }
        }
    }

    /* Process the directories in gml_lib_dirs. */

    if( member_name == NULL ) {

        if( gml_lib_dirs.count > 0  ) {
            for( i = 0; i < gml_lib_dirs.count; i++ ) {

                /* See if the current directory contains the desired library. */

                member_name = get_member_name( gml_lib_dirs.directories[i], in_name );
                if( member_name != NULL ) {
                    current_path = gml_lib_dirs.directories[i];
                    break;
                }
            }
        }
    }
    
    /* Process the directories in path_dirs. */

    if( member_name == NULL ) {
        if( path_dirs.count > 0 ) {
            for( i = 0; i < path_dirs.count; i++ ) {

                /* See if the current directory contains the desired library. */

                member_name = get_member_name( path_dirs.directories[i], in_name );
                if( member_name != NULL ) {
                    current_path = path_dirs.directories[i];
                    break;
                }
            }
        }
    }

    /* Now combine the member name and the path into name_buffer,
     * adding ".cop" if the member name has no extension.
     */

    if( member_name != NULL ) {
        if( memchr( member_name, '.', strnlen_s( member_name, FILENAME_MAX ) ) == \
                                                                        NULL ) {
            buffer_length = strnlen_s( current_path, FILENAME_MAX ) + \
                    strnlen_s( member_name, FILENAME_MAX ) + strlen( ".cop" ) + 1;
            if( buffer_length <= FILENAME_MAX ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                strcat_s( file_name, buffer_length, ".cop" );
                return( file_name );
            }
        } else {
            buffer_length = strnlen_s( current_path, FILENAME_MAX ) + \
                                        strnlen_s( member_name, FILENAME_MAX ) + 1;
            if( buffer_length <= FILENAME_MAX ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                return( file_name );
            }
        }

        /* If we get here, then the file name is too long. */

        out_msg( "File name for %s is too long (limit is %i):\n  %s%s\n", \
                                in_name, FILENAME_MAX, current_path, member_name );
        mem_free( file_name );
        file_name = NULL;
        mem_free( member_name );
        member_name = NULL;
        return( file_name );
    }

    return( file_name );
}

/*************Valid Stuff Below This Line!*******************/

/* Local function definitions. */

/* Function initialize_directory_list().
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
 *      If in_path_list is NULL, *in_directory_list will be cleared.
 *      If *in_directory_list->directories is not NULL on exit, it is a 
 *          single block of memory and can be freed with one mem_free().
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

    if( in_path_list == NULL ) {
        in_list->count = 0;
        mem_free( in_list->directories );
        in_list->directories = NULL;
        return;
    }

    /* Note: the two for loops which parse the path names were tested and
     * tweaked. Please do not change them without thoroughly testing them.
     * They are intended to correctly handle all directory lists, even if
     * spaces and quotation marks are involved.
     */

    /* Determine the number of paths and the total length needed. */

    byte_count = 0;
    path_count = 0; 
    for( i = 0; i < strlen( in_path_list ); i++ ) {
        if( in_path_list[i] == '"' ) {
            for( j = i + 1; j < strlen( in_path_list ); j++ ) {
                if( in_path_list[j] == '"' ) break;
                byte_count++;
            }    
            i = j;
            if( in_path_list[i] == '\0' ) {
                if( in_path_list[i - 1] == INCLUDE_SEP ) path_count++;
                if( in_path_list[i - 1] != PATH_SEP[0] ) byte_count++;
                break;
            }
            continue;
        }
        if( in_path_list[i] == INCLUDE_SEP ) {
            if( in_path_list[i - 1] != INCLUDE_SEP ) {
                path_count++;
                if( in_path_list[i - 1] == '"' ) {
                    if( in_path_list[i - 2] != PATH_SEP[0] ) byte_count++;
                }
                if( in_path_list[i - 1] != PATH_SEP[0] ) byte_count++;
                continue;
            }
        }
        byte_count++;
    }
    if( in_path_list[i - 1] != INCLUDE_SEP ) {
        path_count++;
        if( in_path_list[i - 1] == '"' ) {
            if( in_path_list[i - 2] != PATH_SEP[0] ) byte_count++;
        }
        if( in_path_list[i - 1] != PATH_SEP[0] ) byte_count++;
    }

    /* Initialize local_list. */

    local_list.count = path_count;
    local_list.directories = (char * *) mem_alloc( \
                    (path_count * sizeof( char * )) + byte_count + path_count );

    array_base = local_list.directories;
    current = (char *) array_base + (path_count * sizeof( char * ));

    k = 0;
    array_base[0] = current;
    for( i = 0; i < strlen( in_path_list ); i++ ) {
        if( in_path_list[i] == '"' ) {
            for( j = i + 1; j < strlen( in_path_list ); j++ ) {
                if( in_path_list[j] == '"' ) break;
                *current++ = in_path_list[j];
            }    
            i = j;
            if( in_path_list[i] == '\0' ) {
                if( in_path_list[i - 1] == INCLUDE_SEP ) {
                    if( ++k < path_count ) array_base[k] = current;
                }
                if( in_path_list[i - 1] != PATH_SEP[0] ) *current++ = PATH_SEP[0];
                *current++ = '\0';
                break;
            }
            continue;
        }
        if( in_path_list[i] == INCLUDE_SEP ) {
            if( in_path_list[i - 1] != INCLUDE_SEP ) {
                if( in_path_list[i - 1] != PATH_SEP[0] ) {
                    if( in_path_list[i - 1] == '"' ) {
                        if( in_path_list[i - 2] != PATH_SEP[0] ) \
                                                        *current++ = PATH_SEP[0];
                    } else *current++ = PATH_SEP[0];
                }
                *current++ = '\0';
                if( ++k < path_count ) array_base[k] = current;
            }
            continue;
        }
        *current++ = in_path_list[i];
    }
    if( in_path_list[i - 1] != INCLUDE_SEP ) {
        if( in_path_list[i - 1] == '"' ) {
            if( in_path_list[i - 2] != PATH_SEP[0] ) *current++ = PATH_SEP[0];
        }
        if( in_path_list[i - 1] != PATH_SEP[0] ) *current++ = PATH_SEP[0];
        *current++ = '\0';
    }

    /* Check for over-length directory names and remove any found. */

    byte_count = 0;
    path_count = local_list.count;

    for( i = 0; i < local_list.count; i++ ){
        if( strlen( local_list.directories[i] ) > FILENAME_MAX ) {
            path_count--;
            out_msg( "Directory path is too long (limit is %i):\n  %s\n", \
                                        FILENAME_MAX, local_list.directories[i] );
            local_list.directories[i] = NULL;
        } else {
            byte_count += strnlen_s( local_list.directories[i], FILENAME_MAX );
        }
    }

    /* If local_list has no entries at all, free local_list.directories and
     * clear in_list and return.
     */

    if( path_count == 0 ) {
        mem_free( local_list.directories );
        local_list.directories = NULL;
        in_list->count = 0;
        mem_free( in_list->directories );
        in_list->directories = NULL;
        return;
    }

    /* If some paths were eliminated for length, then initialize in_list with
     * the remaining paths (only) and free local_list.directories. Otherwise,
     * local_list.directories becomes in_list->directories.
     */

    if( path_count < local_list.count) {

        in_list->count = path_count;
        in_list->directories = (char * *) mem_alloc( \
                    (path_count * sizeof( char * )) + byte_count + path_count );

        array_base = in_list->directories;
        current = (char *) (array_base + path_count * sizeof( char * ));
 
        byte_count += path_count;
        j = 0;
        for( i = 0; i < local_list.count; i++ ) {
            if( local_list.directories[i] != NULL ) {
                array_base[j] = current;
                strcpy_s( current, byte_count, local_list.directories[i] );
                byte_count -= (strlen( local_list.directories[i] ) + 1);
                current += (strlen( local_list.directories[i] ) + 1);
                j++;
            }
        }
        mem_free( local_list.directories );
        local_list.directories = NULL;
    } else {
        in_list->count = local_list.count;
        in_list->directories = local_list.directories;
    }

    return;
}

/***************************************************************************/
/*  Compose full path / filename and try to open for reading               */
/***************************************************************************/

static int try_open( char * prefix, char * filename, char * suffix )
{
    int         i;
    FILE    *   fp;
    char        buf[ FILENAME_MAX ];
    errno_t     erc;

    i = 0;
    while( (buf[i] = *prefix++) )    ++i;
    while( (buf[i] = *filename++) )  ++i;
    while( (buf[i] = *suffix++) )    ++i;
    filename = &buf[0];                 // point to the full name

    try_file_name = NULL;
    try_fp = NULL;

    for( ;; ) {
        erc = fopen_s( &fp, filename, "rb" );
        if( erc == 0 ) break;
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
        if( !free_resources( errno ) ) break;     // try closing an include file
    }
    if( fp == NULL ) return( 0 );

    try_file_name = mem_alloc( i + 2 );
    strcpy_s( try_file_name, i + 2, buf );
    try_fp = fp;
    return( 1 );
}

/* Extern function definitions. */

/* Function ff_setup().
 * Initializes the directory lists.
 */
 
void ff_setup( void )
{
    errno_t rc;
    size_t  gmlinc_length;
    size_t  gmllib_length;
    size_t  max_length;
    size_t  path_length;
    
    /* Initialize the globals. */

    try_file_name = NULL;
    try_fp = NULL;

    /* This directory list encodes the current directory. */

    cur_dir_list.count = 1;
    cur_dir_list.directories = &cur_dir;

    /* Initialize the directory lists. */
    
    gml_inc_dirs.count = 0;
    gml_inc_dirs.directories = NULL;
    gml_lib_dirs.count = 0;
    gml_lib_dirs.directories = NULL;
    path_dirs.count = 0;
    path_dirs.directories = NULL;

    /* Get the lengths of the environment variables. */

    getenv_s( &gmlinc_length, NULL, 0, "GMLINC" );
    getenv_s( &gmllib_length, NULL, 0, "GMLLIB" );
    getenv_s( &path_length, NULL, 0, "PATH" );

    /* Set max_length to the largest of the three. */

    max_length = path_length;
    if( gmlinc_length > max_length ) max_length = gmlinc_length;    
    if( gmllib_length > max_length ) max_length = gmllib_length;    

    /* Allocate the buffer, allowing for a terminating '\0'. */

    max_length++;
    env_var_buffer = mem_alloc( max_length * sizeof( char ) );

    /* Initialize the directory list for GMLINC. */

    rc = getenv_s( &env_var_length, env_var_buffer, max_length, "GMLINC" );

    if( rc == 0 ) env_var_buffer[env_var_length] = '\0';
    else env_var_length = 0;
    
    if( env_var_length > 0 ) initialize_directory_list( env_var_buffer, \
                                                        &gml_inc_dirs );

    /* Initialize the directory list for GMLLIB. */
    
    rc = getenv_s( &env_var_length, env_var_buffer, max_length, "GMLLIB" );

    if( rc == 0 ) env_var_buffer[env_var_length] = '\0';
    else env_var_length = 0;

    if( env_var_length > 0 ) initialize_directory_list( env_var_buffer, \
                                                        &gml_lib_dirs );
    /* Initialize the directory list for PATH. */
    
    rc = getenv_s( &env_var_length, env_var_buffer, max_length, "PATH" );

    if( rc == 0 ) env_var_buffer[env_var_length] = '\0';
    else env_var_length = 0;

    if( env_var_length > 0 ) initialize_directory_list( env_var_buffer, \
                                                        &path_dirs );

    /* Free the environment variable buffer. */
    
    mem_free( env_var_buffer );
    env_var_buffer = NULL;

    return;
}

/* Function ff_teardown().
 * Releases the memory allocated by ff_setup().
 */

void ff_teardown( void )
{

    if( try_file_name != NULL ) {
        mem_free( try_file_name );
    }

    if( try_fp != NULL) {
        fclose( try_fp );
    }

    if( gml_inc_dirs.directories != NULL ) {
        mem_free( gml_inc_dirs.directories );
    }

    if( gml_lib_dirs.directories != NULL) {
        mem_free( gml_lib_dirs.directories );
    }

    if( path_dirs.directories != NULL ) {
        mem_free( path_dirs.directories );
    }

    return;
}

/* Function search_file_in_dirs().
 * Searches for filename in curdir and the directories given in the
 * environment variables, as appropriate to the value of sequence.
 *
 * Parameters:
 *      filename points to the name of the desired file, or to the desired
 *          defined name if dirseq is ds_bin_lib.
 *      defext points to the first (or only) extension to use.
 *      altext points to the second extension to use, if appropriate.
 *      dirseq indicates the type of file sought.
 *
 * Returns:
 *      0 if the file is not found.
 *      1 if the file is found.
 *
 * Note:
 *      if the file is found, then try_file() will have set try_file_name
 *      and try_fp to the name as found and FILE * of the file.
 */

int search_file_in_dirs( char * filename, char * defext, char * altext,
                          dirseq sequence )
{
    char                buff[FILENAME_MAX];
    char                alternate_file[FILENAME_MAX];
    char                default_file[FILENAME_MAX];
    char                primary_file[FILENAME_MAX];
    char                try[FILENAME_MAX];
    char            *   fn_dir;
    char            *   fn_drive;
    char            *   fn_ext;
    char            *   fn_name;
    char            *   dir_ptr;
    directory_list  *   list_ptr;
    directory_list  *   searchdirs[4];
    int         i;
    int         j;

    /* For ds_bin_lib, "filename" contains a defined name. */

    if( sequence != ds_bin_lib ) { 
        _splitpath2( filename, buff, &fn_drive, &fn_dir, &fn_name, &fn_ext );
        if( fn_drive[0] != '\0' || IS_PATH_SEP(fn_dir[0]) ) {

            /* Drive or path from root specified. */

            if( try_open( "", filename, "" ) != 0 ) return( 1 );
            if( *fn_ext == '\0' ) {
                if( try_open( "", filename, defext ) != 0 ) return( 1 );
                if( *altext != '\0' ) {
                    if( try_open( "", filename, altext ) != 0 ) return( 1 );
                }
                if( strcmp(defext, GML_EXT )) { // one more try with .gml
                    if( try_open( "", filename, GML_EXT ) != 0 ) return( 1 );
                }
            } else {
                if( *altext != '\0' ) {
                    _makepath( try, fn_drive, fn_dir, filename, altext );
                    if( try_open( "", try, "" ) != 0 ) return( 1 );
                }
            }
            return( 0 );
        }
    }

    /* Set up the file names and the dirs for the specified sequence. */

    switch( sequence ) {
    case ds_opt_file:
        if( *fn_ext == '\0' ) {
            strcpy_s( primary_file, FILENAME_MAX, fn_name );
            strcat_s( primary_file, FILENAME_MAX, ".opt" );
        }
        searchdirs[0] = &cur_dir_list;
        searchdirs[1] = &gml_lib_dirs;
        searchdirs[2] = &gml_inc_dirs;
        searchdirs[3] = &path_dirs;
        break;
    case ds_doc_spec:
        if( *fn_ext == '\0' ) {
            strcpy_s( primary_file, FILENAME_MAX, fn_name );
            strcat_s( primary_file, FILENAME_MAX, ".gml" );
        }
        if( *altext != '\0' ) {
            strcpy_s( alternate_file, FILENAME_MAX, fn_name );
            strcat_s( alternate_file, FILENAME_MAX, altext );
        }
        if( strcmp( defext, GML_EXT )) {
            strcpy_s( default_file, FILENAME_MAX, fn_name );
            strcat_s( default_file, FILENAME_MAX, GML_EXT );
        }
        searchdirs[0] = &cur_dir_list;
        searchdirs[1] = &gml_inc_dirs;
        searchdirs[2] = &gml_lib_dirs;
        searchdirs[3] = &path_dirs;
        break;
    case ds_bin_lib:
        searchdirs[0] = &gml_lib_dirs;
        searchdirs[1] = &gml_inc_dirs;
        searchdirs[2] = &path_dirs;
        searchdirs[3] = NULL;
        break;
    case ds_lib_src: 
        if( *fn_ext == '\0' ) {
            strcpy_s( primary_file, FILENAME_MAX, fn_name );
            strcat_s( primary_file, FILENAME_MAX, ".pcd" );
        }
        if( *altext == '\0' ) {
            strcpy_s( alternate_file, FILENAME_MAX, fn_name );
            strcat_s( alternate_file, FILENAME_MAX, ".fon" );
        } else {
            strcpy_s( alternate_file, FILENAME_MAX, fn_name );
            strcat_s( alternate_file, FILENAME_MAX, altext );
        }
        searchdirs[0] = &cur_dir_list;
        searchdirs[1] = &gml_inc_dirs;
        searchdirs[2] = NULL;
        searchdirs[3] = NULL;
        break;
    default:
        out_msg( "findfile internal error\n" );
        my_exit( 8 );
    }

    /* For ds_bin_lib, "filename" contains a defined name. */

    if( sequence == ds_bin_lib ) { 

/* Not yet ready for ds_bin_lib */

    } else {

    /* The other sequences all provide a file name to search for. */

        for( i = 0; i < 4; i++ ) {
            list_ptr = searchdirs[i];
            if( list_ptr == NULL ) break;

            for( j = 0; j < list_ptr->count; j++ ) {
                dir_ptr = list_ptr->directories[j];

                if( try_open( dir_ptr, primary_file, "" ) != 0 ) return( 1 );

                if( alternate_file != NULL ) {
                    if( try_open( dir_ptr, alternate_file, "" ) != 0 ) return( 1 );
                }

                if( default_file != NULL ) {
                    if( try_open( dir_ptr, default_file, "" ) != 0 ) return( 1 );
                }
            }
        }
    }

    return( 0 );
}

