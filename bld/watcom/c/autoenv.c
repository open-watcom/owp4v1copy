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
* Description:  Set up environment variables for Open Watcom tools.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <env.h>
#include <process.h>
#include "autoenv.h"

#ifdef __UNIX__
    #define DIR_SEP_CHAR   '/'
    #define DIR_SEP_STR    "/"
    #define PATH_SEP_CHAR  ':'
    #define PATH_SEP_STR   ":"
#else
    #define DIR_SEP_CHAR   '\\'
    #define DIR_SEP_STR    "\\"
    #define PATH_SEP_CHAR  ';'
    #define PATH_SEP_STR   ";"
#endif

#if defined( __LINUX__ )
    #define PRIMARY_PATH    "binl"
#elif defined( __OS2__ )
    #define PRIMARY_PATH    "binp"
    #define SECONDARY_PATH  "binw"
#elif defined( __NT__ )
    #define PRIMARY_PATH    "binnt"
    #define SECONDARY_PATH  "binw"
#else
    #define PRIMARY_PATH    "binw"
#endif

#ifdef DEBUG_AUTOENV
    #define dbg_puts( a )   puts( a )
#else
    #define dbg_puts( a )
#endif

#if defined( __OS2__ )
/* Set up BeginLIBPATH if required; this is not an environment variable
 * and requires OS/2 specific API calls.
 */

#define INCL_DOSMISC
#define INCL_DOSMODULEMGR
#define INCL_DOSERRORS
#define INCL_ORDINALS
#include <os2.h>

#define HAVE_SETUP_OS_ENV

static int setup_os_env( const char *watcom )
{
    char        *buf;
    char        old_blpath[1024] = "";
    int         old_blpath_len;
    HMODULE     hmod;
    APIRET      rc;
    APIRET      (APIENTRY *fnDosQueryExtLIBPATH)( PSZ, ULONG );
    APIRET      (APIENTRY *fnDosSetExtLIBPATH)( PSZ, ULONG );

    /* Older versions of OS/2 did not support BEGIN/ENDLIBPATH. Dynamically
     * query the API entrypoints to prevent load failures.
     */
    if( DosLoadModule( old_blpath, sizeof( old_blpath ),
        "DOSCALLS", &hmod ) != NO_ERROR ) {
        dbg_puts( "autoenv: DosLoadModule failed for DOSCALLS" );
        return( -8 );
    }
    if( DosQueryProcAddr( hmod, ORD_DOS32QUERYEXTLIBPATH,
        NULL, (PFN *)&fnDosQueryExtLIBPATH ) != NO_ERROR ) {
        dbg_puts( "autoenv: DosQueryProcAddr failed for DosQueryExtLIBPATH" );
        return( -8 );
    }
    if( DosQueryProcAddr( hmod, ORD_DOS32SETEXTLIBPATH,
        NULL, (PFN *)&fnDosSetExtLIBPATH ) != NO_ERROR ) {
        dbg_puts( "autoenv: DosQueryProcAddr failed for DosSetExtLIBPATH" );
        return( -8 );
    }

    /* Query existing BEGINLIBPATH */
    if( fnDosQueryExtLIBPATH( old_blpath, BEGIN_LIBPATH ) == NO_ERROR ) {
        if( strstr( old_blpath, watcom ) != NULL ) {
            /* Already have an entry in BEGINLIBPATH */
            dbg_puts( "autoenv: Found valid entry in BEGINLIBPATH" );
            return( 0 );
        }
        old_blpath_len = strlen( old_blpath );
    } else {
        old_blpath_len = 0;
    }
    buf = malloc( strlen( watcom ) + 16 + old_blpath_len );
    if( buf == NULL ) {
        dbg_puts( "autoenv: Memory allocation failed" );
        return( -8 );
    }
    strcpy( buf, watcom );
    strcat( buf, "\\binp\\dll;" );
    strcat( buf, old_blpath );
    rc = fnDosSetExtLIBPATH( buf, BEGIN_LIBPATH );
    if( rc == NO_ERROR ) {
        dbg_puts( "autoenv: DosSetExtLIBPATH succeeded:" );
        dbg_puts( buf );
        free( buf );
        return( 0 );
    } else {
        dbg_puts( "autoenv: DosSetExtLIBPATH failed" );
        free( buf );
        return( -8 );
    }
}

#endif

/* Check that the WATCOM environment variable exists and if not, try to
 * heuristically determine what it should be and set it. Then check if
 * the PATH contains the appropriate paths and if not, add them depending
 * on the host platform.
 */
int watcom_setup_env( void )
{
    char        *watcom;
    char        *path;
    char        *newpath;
    char        buf[PATH_MAX];

    watcom = getenv( "WATCOM" );
    /* If WATCOM env var isn't set, try to construct it */
    if( watcom == NULL ) {
        int     len;
        char    *p;
        char    *path_sep = NULL;
        char    *prev_path_sep = NULL;

        dbg_puts( "autoenv: WATCOM environment variable not found" );
        if( _cmdname( buf ) == NULL ) {
            return( -1 );
        }
        /* Set WATCOM to a directory one level up from the executable */
        len = strlen( buf );
        p = buf;
        while( *p != '\0' ) {
            if( *p == DIR_SEP_CHAR ) {
                if( path_sep != NULL ) {
                    prev_path_sep = path_sep;
                }
                path_sep = p;
            }
            ++p;
        }
        if( prev_path_sep == NULL ) {
            return( -1 );
        }
        /* Cut off path at next-to-last directory separator */
        *prev_path_sep = '\0';
        if( setenv( "WATCOM", buf, 0 ) != 0 ) {
            return( -1 );
        }
        watcom = getenv( "WATCOM" );
        dbg_puts( "autoenv: WATCOM environment successfully set:" );
        dbg_puts( watcom );
    }
    /* At this point, WATCOM is set; construct what we want in PATH */
    if( sizeof( buf ) < (strlen( watcom ) + 16) ) {
        return( -2 );
    }
    strcpy( buf, watcom );
    strcat( buf, DIR_SEP_STR );
    strcat( buf, PRIMARY_PATH );

    /* Verify PATH */
    path = getenv( "PATH" );
    if( path == NULL ) {
        /* The easy case - PATH isn't set at all */
        if( setenv( "PATH", buf, 0 ) != 0 ) {
            return( -2 );
        }
    } else {
        if( strstr( path, buf ) == NULL ) {
            /* Our path is not present */
            newpath = malloc( strlen( path ) + strlen( buf ) * 2 + 16 );
            if( newpath == NULL ) {
                return( -2 );
            }
            strcpy( newpath, buf );
            strcat( newpath, PATH_SEP_STR );
#if defined( SECONDARY_PATH )
            /* Add second path component on platforms that require it */
            strcpy( buf, watcom );
            strcat( buf, DIR_SEP_STR );
            strcat( buf, SECONDARY_PATH );
            strcat( newpath, buf );
            strcat( newpath, PATH_SEP_STR );
#endif
            strcat( newpath, path );
            if( setenv( "PATH", newpath, 1 ) != 0 ) {
                free( newpath );
                return( -2 );
            }
            free( newpath );
        }
    }

    /* Finally set up platform specific stuff if applicable */
#ifdef HAVE_SETUP_OS_ENV
    return( setup_os_env( watcom ) );
#else
    return( 0 );
#endif
}

#ifdef AUTOENV_TEST

void main( void )
{
    char        *watcom;
    char        *path;

    if( watcom_setup_env() != 0 ) {
        printf( "watcom_setup_env() failed!\n" );
    }

    watcom = getenv( "WATCOM" );
    path = getenv( "PATH" );
    if( watcom == NULL ) {
        printf( "WATCOM env var not found\n" );
    } else {
        printf( "WATCOM=%s\n", watcom );
    }
    if( path == NULL ) {
        printf( "PATH env var not found\n" );
    } else {
        printf( "PATH=%s\n", path );
    }
}

#endif
