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
* Description:  DOS implementation of directory functions.
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <direct.h>
#include <dos.h>
#include "strdup.h"
#include "liballoc.h"
#include "tinyio.h"
#include "seterrno.h"
#include "msdos.h"
#include "_direct.h"

#define SEEK_ATTRIB (TIO_HIDDEN | TIO_SYSTEM | TIO_SUBDIRECTORY)


/*
 * Copies a dirent struct to an LFN findfirst struct
 */
#ifdef __WATCOM_LFN__
static void copy_dirent_lfn( struct find_t *lfnblock, DIR_TYPE *dosblock )
{
    memcpy( lfnblock, dosblock, sizeof( *lfnblock ) );
}

/*
 * Copies an LFN findfirst struct to a dirent struct
 */
static void copy_lfn_dirent( DIR_TYPE *dosblock, struct find_t *lfnblock )
{
    memcpy( dosblock, lfnblock, sizeof( *lfnblock ) );
}
#endif //__WATCOM_LFN__

static int is_directory( const CHAR_TYPE *name )
/**********************************************/
{
    UINT_WC_TYPE    curr_ch;
    UINT_WC_TYPE    prev_ch;

    curr_ch = NULLCHAR;
    for(;;) {
        prev_ch = curr_ch;
#if defined( __WIDECHAR__ ) || defined( __UNIX__ )
        curr_ch = *name;
#else
        curr_ch = _mbsnextc( name );
#endif
        if( curr_ch == NULLCHAR )
            break;
        if( prev_ch == '*' )
            break;
        if( prev_ch == '?' )
            break;
#if defined( __WIDECHAR__ ) || defined( __UNIX__ )
        ++name;
#else
        name = _mbsinc( name );
#endif
    }
    if( curr_ch == NULLCHAR ) {
        if( prev_ch == '\\' || prev_ch == '/' || prev_ch == '.' ) {
            return( 1 );
        }
    }
    return( 0 );
}


#ifdef __WIDECHAR__
static void filenameToWide( DIR_TYPE *dir )
/*****************************************/
{
    wchar_t             wcs[ _MAX_PATH ];

    /* convert string */
    mbstowcs( wcs, (char*)dir->d_name, sizeof( wcs ) / sizeof( wchar_t ) );
    /* copy string */
    wcscpy( dir->d_name, wcs );
}
#endif


_WCRTLINK DIR_TYPE *__F_NAME(_opendir,_w_opendir)( const CHAR_TYPE *name, unsigned attr )
{
#ifdef __WATCOM_LFN__
    struct find_t   lfntemp;
#endif
    DIR_TYPE        tmp;
    DIR_TYPE        *parent;
    int             i;
    tiny_ret_t      rc;
    auto CHAR_TYPE  pathname[ _MAX_PATH + 6 ];
    const CHAR_TYPE *dirnameStart = name;
    UINT_WC_TYPE    curr_ch;
    UINT_WC_TYPE    prev_ch;

    /*** Convert a wide char string to a multibyte string ***/
#ifdef __WIDECHAR__
    char            mbcsName[ MB_CUR_MAX * _MAX_PATH ];

    if( wcstombs( mbcsName, name, sizeof( mbcsName ) ) == (size_t) - 1 )
        return( NULL );
#endif

    parent = &tmp;
#ifdef __WATCOM_LFN__
    if( !is_directory( name ) ) {
        rc = _dos_findfirst( __F_NAME(name,mbcsName), attr, &lfntemp );
        if( rc != 0 ) {
            __set_errno_dos( rc );
            return( NULL );
        }
#else
    TinySetDTA( &(tmp.d_dta) );
    if( !is_directory( name ) ) {
        rc = TinyFindFirst( __F_NAME(name,mbcsName), attr );
        if( TINY_ERROR( rc ) ) {
            __set_errno_dos( TINY_INFO( rc ) );
            return( NULL );
        }
#endif
    } else {
        parent->d_attr = _A_SUBDIR;
    }
    if( parent->d_attr & _A_SUBDIR ) {
        prev_ch = NULLCHAR;
        for( i = 0; i < _MAX_PATH; i++ ) {
            pathname[i] = *name;
#if defined( __WIDECHAR__ ) || defined( __UNIX__ )
            curr_ch = *name;
#else
            curr_ch = _mbsnextc( name );
            if( curr_ch > 256 ) {
                ++i;
                ++name;
                pathname[i] = *name;        /* copy second byte */
            }
#endif
            if( curr_ch == NULLCHAR ) {
                if( i != 0  &&  prev_ch != '\\' && prev_ch != '/' ) {
                    pathname[i++] = '\\';
                }
                __F_NAME(strcpy,wcscpy)( &pathname[i], STRING( "*.*" ) );
#ifdef __WIDECHAR__
                if( wcstombs( mbcsName, pathname, sizeof( mbcsName ) ) == (size_t) - 1 )
                    return( NULL );
#endif
#ifdef __WATCOM_LFN__
                rc = _dos_findfirst( __F_NAME(pathname,mbcsName), attr, &lfntemp );
                if( rc != 0 ) {
                    __set_errno_dos( rc );
                    return( NULL );
                }
#else
                rc = TinyFindFirst( __F_NAME(pathname,mbcsName), attr );
                if( TINY_ERROR( rc ) ) {
                    __set_errno_dos( TINY_INFO( rc ) );
                    return( NULL );
                }
#endif
                break;
            }
            if( *name == '*' )
                break;
            if( *name == '?' )
                break;
            ++name;
            prev_ch = curr_ch;
        }

    }
    parent = lib_malloc( sizeof( *parent ) );
    if( parent == NULL ) {
        return( parent );
    }
#ifdef __WATCOM_LFN__
    copy_lfn_dirent( parent, &lfntemp );
    parent->d_first = _DIR_ISFIRST;
#else
    *parent = tmp;
    parent->d_first = _DIR_ISFIRST;
#endif

    if( parent != NULL )
        parent->d_openpath = __F_NAME(__clib_strdup,__clib_wcsdup)( dirnameStart );
    return( parent );
}


_WCRTLINK DIR_TYPE *__F_NAME(opendir,_wopendir)( const CHAR_TYPE *name )
{
    return( __F_NAME(_opendir,_w_opendir)( name, SEEK_ATTRIB ) );
}


_WCRTLINK DIR_TYPE *__F_NAME(readdir,_wreaddir)( DIR_TYPE *parent )
{
    tiny_ret_t      rc;
#ifdef __WATCOM_LFN__
    struct find_t   lfntemp;
#endif

    if( parent == NULL ) {
        return( NULL );
    }
    if( parent->d_first == _DIR_CLOSED )
        return( NULL );
    if( parent->d_first == _DIR_ISFIRST ) {
        parent->d_first = _DIR_NOTFIRST;
#ifdef __WIDECHAR__
        filenameToWide( parent );
#endif
        return( parent );
    }
#ifdef __WATCOM_LFN__
    copy_dirent_lfn( &lfntemp, parent );
    rc = _dos_findnext( &lfntemp );
    if( rc != 0 ) {
        if( rc != E_nomore )
            __set_errno_dos( rc );
        return( NULL );
    }
    copy_lfn_dirent( parent, &lfntemp );
#else
    TinySetDTA( &(parent->d_dta) );
    rc = TinyFindNext();
    if( TINY_ERROR( rc ) ) {
        if( TINY_INFO( rc ) != E_nomore )
            __set_errno_dos( TINY_INFO( rc ) );
        return( NULL );
    }
#endif

#ifdef __WIDECHAR__
    filenameToWide( parent );
#endif
    return( parent );
}


_WCRTLINK int __F_NAME(closedir,_wclosedir)( DIR_TYPE *dirp )
{
#ifdef __WATCOM_LFN__
    struct find_t   lfntemp;

    copy_dirent_lfn( &lfntemp, dirp );
    if( _dos_findclose( &lfntemp ) != 0 ) {
        return( 1 );
    }
#else
    if( dirp == NULL || dirp->d_first > _DIR_MAX_FOR_CLOSE_OK ) {
        return( 1 );
    }
#endif
    dirp->d_first = _DIR_CLOSED;
    if( dirp->d_openpath != NULL )
        free( dirp->d_openpath );
    lib_free( dirp );
    return( 0 );
}


_WCRTLINK void __F_NAME(rewinddir,_wrewinddir)( DIR_TYPE *dirp )
{
    CHAR_TYPE       *openpath;
    DIR_TYPE        *newDirp;

    /*** Get the name of the directory before closing it ***/
    if( dirp->d_openpath == NULL )
        return;     /* can't continue if NULL */
    openpath = __F_NAME(__clib_strdup,__clib_wcsdup)( dirp->d_openpath ); /* store path */
    if( openpath == NULL ) {
        dirp->d_first = _DIR_CLOSED;    /* so reads won't work any more */
        return;
    }

    /*** Reopen the directory ***/
    __F_NAME(closedir,_wclosedir)( dirp );              /* close directory */
    newDirp = __F_NAME(opendir,_wopendir)( openpath );  /* open it again */
    if( newDirp == NULL ) {
        dirp->d_first = _DIR_CLOSED;    /* so reads won't work any more */
        return;
    }

    /*** Clean up and go home ***/
    free( openpath );                               /* don't need this any more */
    memcpy( dirp, newDirp, sizeof( DIR_TYPE ) );    /* copy into user buffer */
}
