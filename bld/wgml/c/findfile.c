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
*                   ff_teardown()
*                   get_cop_file()
*                   get_env_vars()
*                   GML_get_env()
*                   search_file_in_dirs()
*               plus these local items:
*                   directory_list
*                   get_member_name()
*                   gml_lib_dirs
*                   gml_inc_dirs
*                   initialize_directory_list()
*                   IS_PATH_SEP()
*                   my_free_inc_fp()
*                   path_dirs
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

/* Define the global variables. */

#define global
#include "findfile.h"

/* These items are from gvars.h. */

static char            *Pathes;        // content of PATH Envvar
static char            *GMLlibs;       // content of GMMLIB Envvar
static char            *GMLincs;       // content of GMLINC Envvar

/* Local macros. */

/* environment variable names */

#define GMLLIB          "GMLLIB"
#define GMLINC          "GMLINC"

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

static  directory_list  gml_lib_dirs;
static  directory_list  gml_inc_dirs;
static  directory_list  path_dirs;

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
        if( strlen( local_list.directories[i] ) > _MAX_PATH ) {
            path_count--;
            out_msg( "Directory path is too long (limit is %i):\n  %s\n", \
                                        _MAX_PATH, local_list.directories[i] );
            local_list.directories[i] = NULL;
        } else {
            byte_count += strnlen_s( local_list.directories[i], _MAX_PATH );
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
    char            filename_buffer[_MAX_PATH];
    cop_file_type   file_type;    
    directory_entry current_entry;
    entry_found     entry_status;
    FILE    *       directory_file  = NULL;
    uint16_t        entry_type;

    /* See if in_dir contains a wgmlst.cop file. */

    if( (strnlen_s( filename_buffer, _MAX_PATH  ) + strlen( "wgmlst.cop" ) \
                                                            + 1) > _MAX_PATH ) {
        out_msg( "Directory path is too long to be used with 'wgmlst.cop' " \
                "(limit is %i):\n  %s\n", _MAX_PATH - strlen( "wgmlst.cop" ), \
                                                                        in_dir );
        return( member_name );
    }

    strcpy_s( filename_buffer, _MAX_PATH, in_dir );
    strcat_s( filename_buffer, _MAX_PATH, "wgmlst.cop" );

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
                                    current_entry.member_name, _MAX_PATH ) + 1 );
                                strcpy_s( member_name, strnlen_s( \
        current_entry.member_name, _MAX_PATH ) + 1, current_entry.member_name );
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
                                    current_entry.member_name, _MAX_PATH ) + 1 );
                        strcpy_s( member_name, strnlen_s( \
        current_entry.member_name, _MAX_PATH ) + 1, current_entry.member_name );    
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

/***************************************************************************/
/*  Try to close an opened include file                                    */
/***************************************************************************/

static  bool    my_free_inc_fp( void )
{
    inputcb *   ip;
    filecb  *   cb;
    int         rc;

    ip = input_cbs;
    while( ip != NULL ) {              // as long as input stack is not empty
        if( (cb = ip->s.f) != NULL ) {  // if file (not macro)
            if( (cb->flags & FF_open) ) {   // and file is open
                rc = fgetpos( cb->fp, &cb->pos );
                if( rc != 0 ) {
                    out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                    err_count++;
                    g_suicide();
                }
                rc = fclose( cb->fp );
                if( rc != 0 ) {
                    out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                    err_count++;
                    g_suicide();
                }
                cb->flags &= ~FF_open;
                return( true );
            }
        }
        ip = ip->prev;                  // next higher input level
    }
    return( false );                    // nothing to close
}

/***************************************************************************/
/*  Compose full path / filename and try to open for reading               */
/***************************************************************************/

int try_open( char * prefix, char * separator, char * filename, char * suffix )
{
    int         i;
    FILE    *   fp;
    char        buf[ FILENAME_MAX ];
    errno_t     erc;

    i = 0;
    while( (buf[i] = *prefix++) )    ++i;
    while( (buf[i] = *separator++) ) ++i;
    while( (buf[i] = *filename++) )  ++i;
    while( (buf[i] = *suffix++) )    ++i;
    filename = &buf[0];                 // point to the full name

    try_file_name = NULL;
    try_fp = NULL;

    for( ;; ) {
        erc = fopen_s( &fp, filename, "rb" );
        if( erc == 0 ) break;
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
        if( !my_free_inc_fp() ) break;     // try closing an include file
    }
    if( fp == NULL ) return( 0 );

    try_file_name = mem_alloc( i + 2 );
    strcpy_s( try_file_name, i + 2, buf );
    try_fp = fp;
    return( 1 );
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

    /* Process the directories in GMLlibs. */

    if( GMLlibs !=NULL ) {
        if( gml_lib_dirs.directories == NULL) initialize_directory_list( \
                                                        GMLlibs, &gml_lib_dirs );
        for( i = 0; i < gml_lib_dirs.count; i++ ) {

            /* See if the current directory contains the desired library. */

            member_name = get_member_name( gml_lib_dirs.directories[i], in_name );
            if( member_name != NULL ) {
                current_path = gml_lib_dirs.directories[i];
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

                member_name = get_member_name( gml_inc_dirs.directories[i], \
                                                                        in_name );
                if( member_name != NULL ) {
                    current_path = gml_inc_dirs.directories[i];
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
        if( memchr( member_name, '.', strnlen_s( member_name, _MAX_PATH ) ) == \
                                                                        NULL ) {
            buffer_length = strnlen_s( current_path, _MAX_PATH ) + \
                    strnlen_s( member_name, _MAX_PATH ) + strlen( ".cop" ) + 1;
            if( buffer_length <= _MAX_PATH ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                strcat_s( file_name, buffer_length, ".cop" );
                return( file_name );
            }
        } else {
            buffer_length = strnlen_s( current_path, _MAX_PATH ) + \
                                        strnlen_s( member_name, _MAX_PATH ) + 1;
            if( buffer_length <= _MAX_PATH ) {
                file_name = mem_alloc( buffer_length );
                strcpy_s( file_name, buffer_length, current_path );
                strcat_s( file_name, buffer_length, member_name );
                return( file_name );
            }
        }
            
        /* If we get here, then the file name is too long. */
                
        out_msg( "File name for %s is too long (limit is %i):\n  %s%s\n", \
                                in_name, _MAX_PATH, current_path, member_name );
        mem_free( file_name );
        file_name = NULL;
        mem_free( member_name );
        member_name = NULL;
        return( file_name );
    }

    return( file_name );
}

/***************************************************************************/
/*  Search for filename in curdir, and along environment vars              */
/***************************************************************************/

int search_file_in_dirs( char * filename, char * defext, char * altext,
                          DIRSEQ sequence )
{
    char        buff[ FILENAME_MAX ];
    char        try[ FILENAME_MAX ];
    char    *   drive;
    char    *   dir;
    char    *   name;
    char    *   ext;
    char    *   p;
    char    *   searchdirs[ 3 ];
    int         i;
    int         k;

    _splitpath2( filename, buff, &drive, &dir, &name, &ext );
    if( drive[0] != '\0' || IS_PATH_SEP(dir[0]) ) {
        /* Drive or path from root specified */
        if( try_open( "", "", filename, "" ) != 0 ) return( 1 );
        if( *ext == '\0' ) {
            if( try_open( "", "", filename, defext ) != 0 ) return( 1 );
            if( *altext != '\0' ) {
                if( try_open( "", "", filename, altext ) != 0 ) return( 1 );
            }
            if( strcmp(defext, GML_EXT )) { // one more try with .gml
                if( try_open( "", "", filename, GML_EXT ) != 0 ) return( 1 );
            }
        } else {
            if( *altext != '\0' ) {
                _makepath( try, drive, dir, filename, altext );
                if( try_open( "", "", try, "" ) != 0 ) return( 1 );
            }
        }
        return( 0 );
    }
    /* no absolute path specified, try curr dir */
    if( try_open( "", "", filename, "" ) != 0 ) return( 1 );
    if( *ext == '\0' ) {
        if( try_open( "", "", filename, defext ) != 0 ) return( 1 );
        if( *altext != '\0' ) {
            if( try_open( "", "", filename, altext ) != 0 ) return( 1 );
        }
        if( strcmp(defext, GML_EXT )) { // one more try with .gml
            if( try_open( "", "", filename, GML_EXT ) != 0 ) return( 1 );
        }
    } else {
        if( *altext != '\0' ) {
            _makepath( try, drive, dir, name, altext );
            if( try_open( "", "", try, "" ) != 0 ) return( 1 );
        }
    }

    /* now try the dirs in specified sequence */
    if( sequence == DS_cur_lib_inc_path ) {
        searchdirs[ 0 ] = GMLlibs;
        searchdirs[ 1 ] = GMLincs;
        searchdirs[ 2 ] = Pathes;
    } else if( sequence == DS_cur_inc_lib_path ) {
        searchdirs[ 0 ] = GMLincs;
        searchdirs[ 1 ] = GMLlibs;
        searchdirs[ 2 ] = Pathes;
    } else if( sequence == DS_cur_lib_path ) {
        searchdirs[ 0 ] = GMLlibs;
        searchdirs[ 1 ] = Pathes;
        searchdirs[ 2 ] = NULL;
    } else {
        searchdirs[ 0 ] = NULL;
        searchdirs[ 1 ] = NULL;
        searchdirs[ 2 ] = NULL;
    }
    for( k = 0; k < 3; k++ ) {
        p = searchdirs[ k ];
        if( p == NULL ) break;

        do {
            i = 0;
            while( *p == ' ' ) ++p;
            for( ; ; ) {
                if( *p == INCLUDE_SEP || *p == ';' ) break;
                if( *p == '\0' ) break;
                if( i < sizeof( try ) - 2 ) {
                    try[ i++ ] = *p;
                }
                ++p;
            }
            while( i != 0 ) {
                if( try[ i-1 ] != ' ' ) break;
                --i;
            }

#define SEP_LEN (sizeof( PATH_SEP ) - 1)

            try[ i ] = '\0';
            if( i >= SEP_LEN && strcmp( &try[ i - SEP_LEN ], PATH_SEP ) == 0 ) {
                try[ i - SEP_LEN ] = '\0';
            }

#undef  SEP_LEN

            if( try_open( try, PATH_SEP, filename, "" ) != 0 ) return( 1 );

            if( *ext == '\0' ) {
                if( try_open( try, PATH_SEP, filename, defext ) != 0 ) return( 1 );
                if( *altext != '\0' ) {
                    if( try_open( try, PATH_SEP, filename, altext ) != 0 ) return( 1 );
                }
                if( strcmp(defext, GML_EXT )) { // one more try with .gml
                    if( try_open( try, PATH_SEP, filename, GML_EXT ) != 0 ) return( 1 );
                }
            } else {
                if( *altext != '\0' ) {
                    _makepath( try, drive, dir, filename, altext );
                    if( try_open( "", "", try, "" ) != 0 ) return( 1 );
                }
            }
            if( *p == INCLUDE_SEP || *p == ';' ) ++p;
        } while( *p != '\0' );
    }
    return( 0 );
}

/***************************************************************************/
/*  get the wgml environment variables                                     */
/***************************************************************************/

void get_env_vars( void )
{
    Pathes  = GML_get_env( "PATH" );
    GMLlibs = GML_get_env( GMLLIB );
    GMLincs = GML_get_env( GMLINC );
}

/***************************************************************************/
/*  Read an environment variable and return content in allocated buffer    */
/***************************************************************************/

char * GML_get_env( char *name )
{
    errno_t     rc;
    size_t      len;
    char    *   value;
    size_t      maxsize;

    maxsize = 128;
    value = mem_alloc( maxsize );
    rc = getenv_s( &len, value, maxsize, name );
    if( rc ) {
        mem_free( value );
        value = NULL;
        if( len ) {   /*  we need more space */
            maxsize = len + 1;
            value = mem_alloc( maxsize );
            rc = getenv_s( &len, value, maxsize, name );
        }
    }
    if( len == 0 ) {
        if( value != NULL ) {
            mem_free( value );
        }
        value = NULL;
    }
    return( value );
}

/* Function ff_teardown().
 * Releases the memory allocated by (currently) GML_get_env() and (eventually)
 * ff_setup(). This version was done to keep wgml memory-leak free after the
 * separation of the material in findfunc and before its integration into a
 * single, coherent module.
 */

void ff_teardown( void )
{
    if( GMLlibs != NULL ) {
        mem_free( GMLlibs );
    }
    if( GMLincs != NULL) {
        mem_free( GMLincs );
    }
    if( Pathes != NULL ) {
        mem_free( Pathes );
    }
    return;
}


