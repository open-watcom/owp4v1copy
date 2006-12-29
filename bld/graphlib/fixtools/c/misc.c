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
* Description:  Miscellaneous, wildcard functions.
*
****************************************************************************/


#if defined( __UNIX__ )
  #include <dirent.h>
#else
  #include <direct.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <assert.h>
#include "misc.h"

#define NULLCHAR '\0'

#define WILD_METAS "*?"

#ifdef __UNIX__
  #define IGNORE_MASK        ( 0 )
#else
  #define IGNORE_MASK        ( _A_VOLID )
#endif

typedef struct PathGroup {
    char    *drive;
    char    *dir;
    char    *fname;
    char    *ext;
    char    buffer[PATH_MAX + 4];
} PGROUP;

extern char *FixName( char *name )
{
#if defined( __DOS__ )
/*********************************
 * Down case all filenames, converting fwd-slash to back-slash
 */
    char    *ptr;
    char    hold;

    assert( name != NULL );

    ptr = name;
    hold = *ptr;
    for( ;; ) {
        if( hold == NULLCHAR ) {
            break;
        }
        if( hold == '/' ) {
            *ptr = '\\';
        } else if( ( hold -= 'A' ) < 26 ) {     /* SIDE EFFECT!!! */
            *ptr = hold + 'a';
        }
        hold = *++ptr;
        if( hold == NULLCHAR ) {
            break;
        }
        if( hold == '/' ) {
            *ptr = '\\';
        } else if( ( hold -= 'A' ) < 26 ) {     /* SIDE EFFECT!!! */
            *ptr = hold + 'a';
        }
        hold = *++ptr;
    }

    return( name );
#elif defined( __OS2__ ) || defined( __NT__ )
/*********************************
 * convert fwd-slash to back-slash
 */
    char    *ptr;
    char    hold;

    assert( name != NULL );

    ptr = name;
    hold = *ptr;
    for( ;; ) {
        if( hold == NULLCHAR ) {
            break;
        }
        if( hold == '/' ) {
            *ptr = '\\';
        }
        hold = *++ptr;
        if( hold == NULLCHAR ) {
            break;
        }
        if( hold == '/' ) {
            *ptr = '\\';
        }
        hold = *++ptr;
    }

    return( name );
#else
    return( name );
#endif
}


extern int FNameCmp( const char *a, const char *b )
/*************************************************/
{
#if defined( __OS2__ ) || defined( __NT__ )
    return( stricmp( a, b ) );
#else
    return( strcmp( a, b ) );
#endif
}


static int FNameCmpChr( char a, char b )
/**************************************/
{
#if defined( __OS2__ ) || defined( __NT__ )
    return( tolower( a ) - tolower( b ) );
#else
    return( a - b );
#endif
}


#ifdef USE_FAR
extern int _fFNameCmp( const char FAR *a, const char FAR *b )
/***********************************************************/
{
#if defined( __OS2__ ) || defined( __NT__ )
    return( _fstricmp( a, b ) );
#else
    return( _fstrcmp( a, b ) );
#endif
}
#endif


#define IS_WILDCARD_CHAR( x ) ((*x == '*') || (*x == '?'))

static int __fnmatch( char *pattern, char *string )
/**************************************************
 * OS specific compare function FNameCmpChr
 * must be used for file names
 */
{
    char    *p;
    int     len;
    int     star_char;
    int     i;

    /*
     * check pattern section with wildcard characters
     */
    star_char = 0;
    while( IS_WILDCARD_CHAR( pattern ) ) {
        if( *pattern == '?' ) {
            if( *string == 0 ) {
                return( 0 );
            }
            string++;
        } else {
            star_char = 1;
        }
        pattern++;
    }
    if( *pattern == 0 ) {
        if( (*string == 0) || star_char ) {
            return( 1 );
        } else {
            return( 0 );
        }
    }
    /*
     * check pattern section with exact match
     * ( all characters except wildcards )
     */
    p = pattern;
    len = 0;
    do {
        if( star_char ) {
            if( string[len] == 0 ) {
                return( 0 );
            }
            len++;
        } else {
            if( FNameCmpChr( *pattern, *string ) != 0 ) {
                return( 0 );
            }
            string++;
        }
        pattern++;
    } while( *pattern && !IS_WILDCARD_CHAR( pattern ) );
    if( star_char == 0 ) {
        /*
         * match is OK, try next pattern section
         */
        return( __fnmatch( pattern, string ) );
    } else {
        /*
         * star pattern section, try locate exact match
         */
        while( *string ) {
            if( FNameCmpChr( *p, *string ) == 0 ) {
                for( i = 1; i < len; i++ ) {
                    if( FNameCmpChr( *(p + i), *(string + i) ) != 0 ) {
                        break;
                    }
                }
                if( i == len ) {
                    /*
                     * if rest doesn't match, find next occurence
                     */
                    if( __fnmatch( pattern, string + len ) ) {
                        return( 1 );
                    }
                }
            }
            string++;
        }
        return( 0 );
    }
}

/*
 * THIS FUNCTION IS NOT RE-ENTRANT!
 *
 * It returns a pointer to a character string, after doing wildcard
 * substitutions.  It returns NULL when there are no more substitutions
 * possible.
 *
 * DoWildCard behaves similarly to strtok.  You first pass it a pointer
 * to a substitution string.  It checks if the string contains wildcards,
 * and if not it simply returns this string.  If the string contains
 * wildcards, it attempts an opendir with the string.  If that fails it
 * returns the string.
 *
 * If the opendir succeeds, or you pass DoWildCard a NULL pointer, it reads
 * the next normal file from the directory, and returns the filename.
 *
 * If there are no more files in the directory, or no directory is open,
 * DoWildCard returns null.
 *
 */

static DIR  *parent = NULL;  /* we need this across invocations */
static char *path = NULL;
static char *pattern = NULL;

extern char *DoWildCard( char *base )
/***********************************************/
{
    PGROUP          pg;
    struct dirent   *entry;

    if( base != NULL ) {
        if( path != NULL ) {        /* clean up from previous invocation */
            free( path );
            path = NULL;            /* 1-jun-90 AFS */
        }
        if( pattern != NULL ) {
            free( pattern );
            pattern = NULL;
        }
        if( parent != NULL ) {
            closedir( parent );
            parent = NULL;          /* 1-jun-90 AFS */
        }

        if( strpbrk( base, WILD_METAS ) == NULL ) {
            return( base );
        }
        // create directory name and pattern
        path = malloc( _MAX_PATH );
        pattern = malloc( _MAX_PATH );
        strcpy( path, base );
        FixName( path );

        _splitpath2( path, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
        _makepath( path, pg.drive, pg.dir, ".", NULL );
        // create file name pattern
        _makepath( pattern, NULL, NULL, pg.fname, pg.ext );

        parent = opendir( path );
        if( parent == NULL ) {
            free( path );
            path = NULL;
            free( pattern );
            pattern = NULL;
            return( base );
        }
    }

    if( parent == NULL ) {
        return( NULL );
    }

    assert( path != NULL && parent != NULL );

    entry = readdir( parent );
    while( entry != NULL ) {
#ifndef __UNIX__
        if( ( entry->d_attr & IGNORE_MASK ) == 0 ) {
#endif
            if( __fnmatch( pattern, entry->d_name ) ) {
                break;
            }
#ifndef __UNIX__
        }
#endif
        entry = readdir( parent );
    }
    if( entry == NULL ) {
        closedir( parent );
        parent = NULL;
        free( path );
        path = NULL;                    /* 1-jun-90 AFS */
        free( pattern );
        pattern = NULL;
        return( base );
    }

    _splitpath2( path, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
    _makepath( path, pg.drive, pg.dir, entry->d_name, NULL );

    return( path );
}


extern void DoWildCardClose( void )
/*********************************/
{
    if( path != NULL ) {
        free( path );
        path = NULL;
    }
    if( pattern != NULL ) {
        free( pattern );
        pattern = NULL;
    }
    if( parent != NULL ) {
        closedir( parent );
        parent = NULL;
    }
}
