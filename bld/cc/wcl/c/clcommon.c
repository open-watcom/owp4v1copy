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
* Description:  Common stuff for wcl and owcc.
*
****************************************************************************/


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#ifndef __UNIX__
#include <direct.h>
#else
#include <dirent.h>
#endif

#include "cmdlhelp.h"
#include "clcommon.h"

#ifndef __UNIX__
#define ATTR_MASK   _A_HIDDEN + _A_SYSTEM + _A_VOLID + _A_SUBDIR
#endif

struct  flags   Flags;
FILE    *Fp;                /* file pointer for Temp_Link         */
char    Libs[MAX_CMD];      /* list of libraires from Cmd         */
char    *Map_Name;          /* name of map file                   */
struct  list *Obj_List;     /* linked list of object filenames    */
char    *Obj_Name;          /* object file name pattern           */
char    Exe_Name[_MAX_PATH];/* name of executable                 */

extern char *DebugOptions[] = {
    "",
    "debug dwarf\n",
    "debug dwarf\n",
    "debug watcom all\n",
    "debug codeview\n",
    "debug dwarf\n",
};

void PrintMsg( const char *fmt, ... )
/***********************************/
{
    char        c;
    int         i;
    char        *p;
    unsigned    value;
    va_list     args;
    int         len;
    char        buf[128];

    value = value;
    va_start( args, fmt );
    len = 0;
    for( ;; ) {
        c = *fmt++;
        if( c == '\0' )
            break;
        if( c == '%' ) {
            c = *fmt++;
            if( c == 's' ) {
                p = va_arg( args, char * );
                for( ;; ) {
                    c = *p++;
                    if( c == '\0' )
                        break;
                    putchar(c);
                }
            } else if( c == 'd' ) {
                i = va_arg( args, int );
                itoa( i, buf, 10 );
                for( len = 0; buf[len] != '\0'; ++len ) {
                    putchar(buf[len]);
                }
            }
        } else {
            putchar(c);
        }
    }
}

void  Fputnl( char *text, FILE *fptr )
/************************************/
{
    fputs( text, fptr );
    fputs( "\n", fptr );
}

void BuildLinkFile( void )
/************************/
{
    char    quoted[_MAX_PATH];

    fputs( "name ", Fp );
    BuildQuotedFName( quoted, sizeof( quoted ), "", Exe_Name, "'" );
    Fputnl( quoted, Fp );
    if( Flags.keep_exename ) {
        Fputnl( "option noextension", Fp );
    }
    if( Flags.map_wanted ) {
        if( Map_Name == NULL ) {
            Fputnl( "option map", Fp );
        } else {
            fputs( "option map=", Fp );
            BuildQuotedFName( quoted, sizeof( quoted ), "", Map_Name, "'" );
            Fputnl( quoted, Fp );
        }
    }
    if( Libs[0] != '\0' ) {
        fputs( "library ", Fp );
        Fputnl( Libs, Fp );
    }
    if( Flags.two_case ) {
        Fputnl( "option caseexact", Fp );
    }
    fclose( Fp );       /* close Temp_Link */
}


void  *MemAlloc( int size )
/*************************/
{
    void        *ptr;

    if( (ptr = malloc( size )) == NULL ) {
        PrintMsg( WclMsgs[OUT_OF_MEMORY] );
        exit( 1 );
    }
    return( ptr );
}


void  AddName( char *name, FILE *link_fp )
/****************************************/
{
    struct list *curr_name, *last_name, *new_name;
    char        path  [_MAX_PATH];
    char        quoted[_MAX_PATH];
    char        buff1 [_MAX_PATH2];
    char        buff2 [_MAX_PATH2];
    char        *fname;

    curr_name = Obj_List;
    while( curr_name != NULL ) {
        if( strcmp( name, curr_name->filename ) == 0 )
            return;
        last_name = curr_name;
        curr_name = curr_name->next;
    }
    new_name = MemAlloc( sizeof( struct list ) );
    if( Obj_List == NULL ) {
        Obj_List = new_name;
    } else {
        last_name->next = new_name;
    }
    new_name->filename = strdup( name );
    new_name->next = NULL;
    fputs( "file ", link_fp );
    if( Obj_Name != NULL ) {
        char        *drive;
        char        *drive2;
        char        *dir;
        char        *dir2;
        char        *extension;
        char        *ext2;

        /* construct full name of object file from Obj_Name information */
        _splitpath2( Obj_Name, buff1, &drive, &dir, &fname, &extension );
        if( extension[0] == '\0' )
            extension = OBJ_EXT;

        if( fname[0] == '\0' || fname[0] == '*' ) {
            /* there's no usable basename in the -fo= pattern, but there drive and directory
             * and extension should still be applied.
             * OTOH the input name may have its own, explicitly given
             * drive, directory and extension, so let those take precedence */
            _splitpath2( name, buff2, &drive2, &dir2, &fname, &ext2 );
            if( ext2[0] != '\0' )
                extension = ext2;
            if( drive2[0] != '\0' )
                drive = drive2;
            if( dir2[0] != '\0' ) {
                dir = dir2;
            }
        }
        _makepath( path, drive, dir, fname, extension );
        name = path;
    }
    BuildQuotedFName( quoted, sizeof( quoted ), "", name, "'" );
    Fputnl( quoted, link_fp );
}


char  *MakePath( char *path )
/***************************/
{
    char        *p;

    p = strrchr( path ,PATH_SEP );
    if( p != NULL ) {
        p[1] = '\0';
    } else {
#ifdef __UNIX__
        *path = '\0';
#else
        p = strchr( path, ':' );
        if( p != NULL ) {
            p[1] = '\0';
        } else {
            *path = '\0';
        }
#endif
    }
    return( strdup( path ) );
}

char  *GetName( char *path )
/**************************/
{
#ifndef __UNIX__
    static      DIR     *dirp;
    struct      dirent  *direntp;

    if( path != NULL ) {                /* if given a filespec to open,  */
        if( *path == '\0' ) {       /*   but filespec is empty, then */
            closedir( dirp );           /*   close directory and return  */
            return( NULL );             /*   (for clean up after error)  */
        }
        dirp = opendir( path );         /* try to find matching filenames */
        if( dirp == NULL ) {
            PrintMsg( WclMsgs[UNABLE_TO_OPEN], path );
            return( NULL );
        }
    }

    while( ( direntp = readdir( dirp ) ) != NULL ) {
        if( (direntp->d_attr & ATTR_MASK) == 0 ) {    /* valid file? */
            return( direntp->d_name );
        }
    }
    closedir( dirp );
    return( NULL );
#else
    char    *name;

    if( path == NULL )
        return( NULL );
    name = strrchr(path, '/');
    if( name == NULL ) {
        name = path;
    } else {
        name++;
    }
    return( strdup(name) );
#endif
}

void FindPath( char *name, char *buf )
/************************************/
{
    _searchenv( name, "PATH", buf );
    if( buf[0] == '\0' ) {
        PrintMsg( WclMsgs[UNABLE_TO_FIND], name );
        exit( 1 );
    }
}

int iswsOrOpt( char ch, char opt, char *Switch_Chars )
{
    if( isblank( ch ) )
        return( 1 );

    if( opt == '-'  ||  opt == Switch_Chars[1] ) {
        /* if we are processing a switch, stop at a '-' */
        if( ch == '-' )
            return( 1 );
#ifndef __UNIX__
        if( ch == Switch_Chars[1] ) {
            return( 1 );
        }
#endif
    }
    return( 0 );
}

char *FindNextWSOrOpt( char *str, char opt, char *Switch_Chars )
/***********************************
 * Finds next free white space character, allowing doublequotes to
 * be used to specify strings with white spaces.
 */
{
    char    string_open = 0;

    while( *str != '\0' ) {
        if( *str == '\\' ) {
            str++;
            if( *str != '\0' ) {
                if( !string_open && iswsOrOpt( *str, opt, Switch_Chars ) ) {
                    break;
                }
                str++;
            }
        } else {
            if( *str == '\"' ) {
                string_open = !string_open;
                str++;
            } else {
                if( string_open ) {
                    str++;
                } else {
                    if( iswsOrOpt( *str, opt, Switch_Chars ) )
                        break;
                    str++;
                }
            }
        }
    }
    return( str );
}
