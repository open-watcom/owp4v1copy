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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#define __OS2__
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "liballoc.h"
#include "tinyio.h"
#include "seterrno.h"
#include "msdos.h"
#include "_direct.h"
#include "strdup.h"

#define SEEK_ATTRIB (TIO_HIDDEN | TIO_SYSTEM | TIO_SUBDIRECTORY)

static  int     is_directory( const char *name )
/**********************************************/
{
    if( name[0] == '\0' )
        return( 0 );
    while( name[1] != '\0' ){
        if( name[0] == '*' || name[0] == '?' ) {
            return( 0 );
        }
        ++name;
    }
    if( name[0] == '\\' || name[0] == '/' || name[0] == '.' ){
        return( 1 );
    }
    return( 0 );
}

static tiny_ret_t __find_close( DIR_TYPE *dirp )
/**********************************************/
{
    return( TinyFindCloseDTA( dirp->d_dta ) );
}

static tiny_ret_t __find_first( const char *name, unsigned attr, DIR_TYPE *dirp )
/*******************************************************************************/
{
    return( TinyFindFirstDTA( name, attr, dirp->d_dta ) );
}

_WCRTLINK DIR_TYPE *_opendir( const char *dirname, unsigned attr, DIR_TYPE *dirp )
/********************************************************************************/
{
    DIR_TYPE    tmp;
    int         i;
    tiny_ret_t  rc;
    char        pathname[_MAX_PATH+6];
    const char  *p;
    int         flag_opendir = ( dirp == NULL );
    int         opened;

    tmp.d_attr = _A_SUBDIR;
    opened = 0;
    if( !is_directory( dirname ) ) {
        rc = __find_first( dirname, attr, &tmp );
        if( TINY_ERROR( rc ) ) {
            __set_errno_dos( TINY_INFO( rc ) );
            return( NULL );
        }
        opened = 1;
    }
    if( tmp.d_attr & _A_SUBDIR ) {                  /* 05-apr-91 */
        p = dirname;
        for( i = 0; i < _MAX_PATH; i++ ) {
            pathname[i] = *p;
            if( *p == '\0' ) {
                if( i != 0  &&  pathname[i-1] != '\\' && pathname[i-1] != '/' ){
                    pathname[i++] = '\\';
                }
                strcpy( &pathname[i], "*.*" );
                if( opened ) {
                    __find_close( &tmp );
                }
                rc = __find_first( pathname, attr, &tmp );
                if( TINY_ERROR( rc ) ) {
                    __set_errno_dos( TINY_INFO( rc ) );
                    return( NULL );
                }
                opened = 1;
                break;
            }
            if( *p == '*' )
                break;
            if( *p == '?' )
                break;
            ++p;
        }
    }
    if( flag_opendir ) {
        dirp = lib_malloc( sizeof( DIR_TYPE ) );
        if( dirp == NULL ) {
            __find_close( &tmp );
            __set_errno_dos( E_nomem );
            return( NULL );
        }
        tmp.d_openpath = __F_NAME(__clib_strdup,__clib_wcsdup)( dirname );
    } else {
        __find_close( dirp );
        tmp.d_openpath = dirp->d_openpath;
    }
    tmp.d_first = _DIR_ISFIRST;
    *dirp = tmp;
    return( dirp );
}


_WCRTLINK DIR_TYPE *opendir( const char *dirname )
{
    return( _opendir( dirname, SEEK_ATTRIB, NULL ) );
}


_WCRTLINK DIR_TYPE *readdir( DIR_TYPE *dirp )
{
    tiny_ret_t rc;

    if( dirp == NULL || dirp->d_first >= _DIR_INVALID )
        return( NULL );
    if( dirp->d_first == _DIR_ISFIRST ) {
        dirp->d_first = _DIR_NOTFIRST;
    } else {
        rc = TinyFindNextDTA( dirp->d_dta );
        if( TINY_ERROR( rc ) ) {
            if( TINY_INFO( rc ) != E_nomore ) {
                __set_errno_dos( TINY_INFO( rc ) );
            }
            return( NULL );
        }
    }
    return( dirp );
}


_WCRTLINK int closedir( DIR_TYPE *dirp )
{
    if( dirp == NULL || dirp->d_first == _DIR_CLOSED ) {
        return( __set_errno_dos( E_ihandle ) );
    }
    __find_close( dirp );
    dirp->d_first = _DIR_CLOSED;
    if( dirp->d_openpath != NULL )
        free( dirp->d_openpath );
    lib_free( dirp );
    return( 0 );
}


_WCRTLINK void __F_NAME(rewinddir,_wrewinddir)( DIR_TYPE *dirp )
{
    if( dirp == NULL || dirp->d_openpath == NULL )
        return;
    if( __F_NAME(_opendir,_w_opendir)( dirp->d_openpath, SEEK_ATTRIB, dirp ) == NULL ) {
        dirp->d_first = _DIR_INVALID;    /* so reads won't work any more */
    }
}

