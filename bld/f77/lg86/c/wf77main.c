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


//
// WF77MAIN  : WATFOR-77 entry point
//

#include <string.h>
#include <malloc.h>
#include <process.h>

#include "ftnstd.h"

extern  int             MainLine(char *);

extern  void            (* __ErrorInit)(char *);
extern  void            (* __ErrorFini)(void);

#define MAX_CMD         127


int     main( int argc, char *argv[] ) {
//======================================

// WATFOR-77 entry point.

    int         ret_code;
    char        *cmd_buff;
    char        *p;
    size_t      len;

    argc = argc; argv = argv;
#if defined( __INCL_ERRMSGS__ )
    {
        extern  void    __InitError(void);

        __InitError();
        __ErrorInit( NULL );
    }
#else
    {
        extern  void    __InitResource(void);

        __InitResource();
        __ErrorInit( argv[0] );
    }
#endif
#if defined( M_I86MM ) || defined( M_I86SM )
    _heapgrow();
#endif
    p = getenv( "WATFOR" );
    if( p != NULL ) {
        len = strlen( p ) + ( MAX_CMD + 2 );
    } else {
        len = MAX_CMD;
    }
    cmd_buff = malloc( len );
    if( p != NULL ) {
        len -= MAX_CMD + 2;
        memcpy( cmd_buff, p, len );
        p = cmd_buff + len;
        *p++ = ' ';                     // put a space after environ value
    } else {
        p = cmd_buff;
    }
    getcmd( p );
    ret_code = MainLine( cmd_buff );
    __ErrorFini();
    return( ret_code );
}
