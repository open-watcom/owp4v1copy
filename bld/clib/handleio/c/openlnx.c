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
* Description:  Linux implementation of open() and sopen()
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include "linuxsys.h"
#ifdef __WIDECHAR__
    #include "mbwcconv.h"
#endif


/* open() and sopen() are identical under Linux, since the extra share
 * flags passed to sopen() are simply ignored. We implement sopen() because
 * Watcom based programs may use it, along with the fact that the runtime
 * library internally always uses it.
 */

_WCRTLINK int __F_NAME(open,_wopen)( const CHAR_TYPE *name, int oflag, ... )
{
    int         mode;
    va_list     args;
#ifdef __WIDECHAR__
    char        mbName[MB_CUR_MAX*_MAX_PATH];   /* single-byte char */
#endif

    va_start( args, oflag );
    mode = va_arg( args, int );
    va_end( args );
    #ifdef __WIDECHAR__
        __filename_from_wide( mbName, name );
        return sys_open( mbName, oflag, mode );
    #else
        return sys_open( name, oflag, mode );
    #endif
}


_WCRTLINK int __F_NAME(sopen,_wsopen)( const CHAR_TYPE *name, int oflag, int shflag, ... )
{
    int         mode;
    va_list     args;
#ifdef __WIDECHAR__
    char        mbName[MB_CUR_MAX*_MAX_PATH];   /* single-byte char */
#endif

    va_start( args, shflag );
    mode = va_arg( args, int );
    va_end( args );
    #ifdef __WIDECHAR__
        __filename_from_wide( mbName, name );
        return sys_open( mbName, oflag, mode );
    #else
        return sys_open( name, oflag, mode );
    #endif
}

