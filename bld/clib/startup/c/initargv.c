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
* Description:  Functions to set up argc and argv parameters of main(), etc.
*
****************************************************************************/


#ifdef __NETWARE__
    void __Init_Argv( void ) { }
    void __Fini_Argv( void ) { }
#else
#include "dll.h"        // needs to be first
#include "widechar.h"
#include <stdio.h>
#include <string.h>
#include "liballoc.h"

// Why is _WCRTLINK not used consistently, below? W.Briscoe 2003-12-15
          extern int                    __historical_splitparms;
_WCRTLINK extern CHAR_TYPE  *__F_NAME(  _LpPgmName, _LpwPgmName );
_WCRTLINK extern CHAR_TYPE  *__F_NAME(  _LpCmdLine, _LpwCmdLine );
          extern int         __F_NAME(  _argc,      _wargc );   /* argument count  */
_WCRTLINK extern int         __F_NAME(  ___Argc,    ___wArgc ); /* argument count */
          extern CHAR_TYPE **__F_NAME(  _argv,      _wargv );   /* argument vector */
_WCRTLINK extern CHAR_TYPE **__F_NAME(  ___Argv,    ___wArgv ); /* argument vector */

static           CHAR_TYPE  *__F_NAME(  __CmdLine,  __wCmdLine ); /* cmdline buffer */

static unsigned _SplitParms( int historical, CHAR_TYPE *p, CHAR_TYPE **argv, CHAR_TYPE **endptr );

void __F_NAME( __Init_Argv, __wInit_Argv )( void )
{
    CHAR_TYPE          *endptr;         // ptr to end of command line
                                        // argument count
    unsigned            argc = _SplitParms( __historical_splitparms, __F_NAME(_LpCmdLine,_LpwCmdLine), NULL, &endptr ) + 1;
                                        // length of command line
    unsigned const      len = (unsigned) ( endptr - __F_NAME(_LpCmdLine,_LpwCmdLine) ) + 1;
                                        // offset of argv in storage
    unsigned const      argv_offset = __ALIGN_SIZE( len * sizeof(CHAR_TYPE));
                                        // amount to allocate - round up for argv pointer alignment
    unsigned const      size = __ALIGN_SIZE( argv_offset + ( argc+1 ) * sizeof(CHAR_TYPE *) );
    CHAR_TYPE          *cmdline;        // copy of command line
    CHAR_TYPE         **argv;           // argument vector

    #if defined(__REAL_MODE__) && defined(__BIG_DATA__)
        #if defined(__OS2_286__)
            if( _osmode == DOS_MODE ) {
                cmdline = lib_nmalloc( size );
                if( (void _WCI86NEAR *) cmdline == NULL ) {
                    cmdline = lib_malloc( size );
                }
            } else {
                cmdline = lib_malloc( size );
            }
        #else
            cmdline = lib_nmalloc( size );
            if( (void _WCI86NEAR *) cmdline == NULL ) {
                cmdline = lib_malloc( size );
            }
        #endif
    #else
        cmdline = lib_malloc( size );
    #endif

    if( cmdline ) {
        memcpy( cmdline, __F_NAME(_LpCmdLine,_LpwCmdLine), len * sizeof(CHAR_TYPE) );
        argv = (void *) ( ( ( char*) cmdline ) + argv_offset );
        argv[0] = __F_NAME(  _LpPgmName, _LpwPgmName );

        _SplitParms( __historical_splitparms, cmdline, argv + 1, &endptr );

        argv[argc] = NULL;
    } else {
        argv = NULL;
        argc = 0;
    }
    __F_NAME( ___Argc, ___wArgc ) = __F_NAME( __argc, __wargc ) = __F_NAME( _argc, _wargc ) = argc;
    __F_NAME( ___Argv, ___wArgv ) = __F_NAME( __argv, __wargv ) = __F_NAME( _argv, _wargv ) = argv;
    __F_NAME(  __CmdLine,  __wCmdLine ) = cmdline;
}


static unsigned _SplitParms( int historical, CHAR_TYPE *p, CHAR_TYPE **argv, CHAR_TYPE **endptr )
{
    register unsigned   argc;
    register CHAR_TYPE *start;
    register CHAR_TYPE *new;
    enum QUOTE_STATE {
        QUOTE_NONE,             /* no " active in current parm */
        QUOTE_DELIMITER,        /* " was first char and must be last */
        QUOTE_STARTED           /* " was seen, look for a match */
    };
    register enum QUOTE_STATE state;

    argc = 0;
    for( ; ; ) {
        while( *p == ' ' || *p == '\t' ) {
            ++p; /* skip over blanks or tabs */
        }
        if( *p == '\0' ) break;
        /* we are at the start of a parm */
        state = QUOTE_NONE;
        if( *p == '\"' ) {
            p++;
            state = QUOTE_DELIMITER;
        }
        new = start = p;
        // Incrementing p here costs marginal 20 bytes
        for( ; ; ) {
            if( *p == '\"' ) {
                if( !historical ) {
                    p++;
                    // Bizarrely, ternary operator here costs marginal 4 bytes.
#if 1
                    if( state == QUOTE_NONE ) {
                        state = QUOTE_STARTED;
                    } else {
                        state = QUOTE_NONE;
                    }
#else
                    state = ( state == QUOTE_NONE ) ? QUOTE_STARTED : QUOTE_NONE;
#endif
                    continue;
                }
                if( state == QUOTE_DELIMITER ) {
                    break;
                }
            }
            if( ( ( *p == ' ' || *p == '\t' ) && state == QUOTE_NONE ) || *p == '\0' ) {
                break;
            }
            if( *p == '\\' ) {
                if( !historical ) {
                    if( p[1] == '\"' ) {
                        ++p;
                        if( p[-2] == '\\' ) {
                            continue;
                        }
                    }
                } else {
                    if( p[1] == '\"' || p[1] == '\\' && state == QUOTE_DELIMITER ) {
                        ++p;
                    }
                }
            }
            if( argv ) {
                *(new++) = *p;
            }
            ++p;
        }
        if( argv ) {
            argv[ argc ] = start;
            ++argc;

            /*
              The *new = '\0' is req'd in case there was a \" to "
              translation. It must be after the *p check against
              '\0' because new and p could point to the same char
              in which case the scan would be terminated too soon.
            */

            if( *p == '\0' ) {
                *new = '\0';
                break;
            }
            *new = '\0';
        } else {
            ++argc;
            if( *p == '\0' ) {
                break;
            }
        }
        ++p;
    }
    *endptr = p;
    return( argc );
}

void __F_NAME(__Fini_Argv,__wFini_Argv)( void )
{
    if( __F_NAME(__CmdLine,__wCmdLine) != NULL ) {
        lib_free( __F_NAME(__CmdLine,__wCmdLine) );
    }
}
#endif
