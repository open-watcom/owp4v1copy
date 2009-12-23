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
* Description:  LFN-enabled _dos_open / _dos_creat / _dos_creatnew.
*
****************************************************************************/


#define __WATCOM_LFN__

#include "variety.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "seterrno.h"
#include "tinyio.h"


static int CTinyOpen( const char *path, int mode )
{
    char    short_name[128];
    union   REGS      r;
    struct  SREGS     s;

    if( _lfntosfn( ( char * )path, short_name ) == NULL ) {
        return( -2 );
    }

    r.w.ax = ( short_name[0] != '\0' ) ?
        TinyOpen( short_name, mode ) : -1;

    if( TINY_OK( r.w.ax ) ) return ( r.w.ax );

    s.ds   = FP_SEG( path );
    r.w.si = FP_OFF( path );
    r.w.bx = mode;
    r.w.dx = 0;
    r.w.ax = 0x716C;

    intdosx( &r, &r, &s );

    if( r.x.cflag || r.w.ax == 0x7100 ) return ( TinyOpen( path, mode ) );
    return ( ( r.w.ax < 5 ) ? -1 : r.w.ax );
}

static int CTinyCreate( const char *path, int attribs, int style )
{
    union REGS      r;
    struct SREGS    s;

    s.ds   = FP_SEG( path );
    r.w.si = FP_OFF( path );
    r.w.bx = O_WRONLY;
    r.w.cx = attribs;
    r.w.dx = style;
    r.w.ax = 0x716C;

    intdosx( &r, &r, &s );

    if( ( r.x.cflag || r.w.ax == 0x7100 ) && !_islfn( path ) )
        return ( ( style == 0x12 ) ? TinyCreate( path, attribs ) :
              TinyCreateNew( path, attribs ) );
    else if( _islfn( path ) && ( r.x.cflag || r.w.ax == 0x7100 ) )
        return -r.w.ax;
    return ( ( r.w.ax < 5 ) ? CTinyOpen( path, O_WRONLY ) : r.w.ax );
}

_WCRTLINK unsigned _dos_open( const char *path, unsigned mode, int *handle )
{
    *handle = CTinyOpen( path, mode );
    if( TINY_ERROR( *handle ) ) {
        return( __set_errno_dos_reterr( TINY_INFO( *handle ) ) );
    }
    return( 0 );
}

_WCRTLINK unsigned _dos_creat( const char *path, unsigned attr, int *handle)
{
    *handle = CTinyCreate( path, attr, 0x12 );
    if( TINY_ERROR( *handle ) ) {
        return( __set_errno_dos_reterr( TINY_INFO( *handle ) ) );
    }
    return( 0 );
}

_WCRTLINK unsigned _dos_creatnew( const char *path, unsigned attr, int *handle)
{
    *handle = CTinyCreate( path, attr, 0x10 );
    if( TINY_ERROR( *handle ) ) {
        return( __set_errno_dos_reterr( TINY_INFO( *handle ) ) );
    }
    return( 0 );
}
