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
* Description:  wmake input analysis
*
****************************************************************************/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#ifdef __OS2__
   #include <stdio.h>
#endif
#ifdef __WATCOMC__
#include <process.h>
#endif

#include "massert.h"
#include "mtypes.h"
#include "mtarget.h"
#include "msysdep.h"
#include "macros.h"
#include "make.h"
#include "mcache.h"
#include "mmemory.h"
#include "mexec.h"
#include "mmisc.h"
#include "mparse.h"
#include "mrcmsg.h"
#include "msg.h"
#include "msuffix.h"
#include "mupdate.h"
#include "mvecstr.h"
#include "mautodep.h"
#if defined( __WINDOWS__ )
#define STRICT
#include <windows.h>
#endif

STATIC TLIST    *mustTargs;     /* targets we must update                   */
STATIC TLIST   *firstTargFound; /* first targets we ever found              */
STATIC NODE     *filesToDo;     /* pointers into argv to -f files           */

#pragma on (check_stack);
STATIC void doBuiltIns( const char *makeopts )
/*********************************************
 * perform the builtin commands
 */
{
    TLIST *list;
    char buf[ 2048 ];
    char *cpy;

    if( !Glob.overide ) {
        DoingBuiltIn = TRUE;
        FmtStr( buf, "%F", BuiltIns );
        cpy = MallocSafe( 2048 + strlen( makeopts ) );
        FmtStr( cpy, buf, makeopts );
        InsString( cpy, FALSE );
        list = Parse();
        FreeTList( list );
        strcpy(cpy, "MAKE=" );
        if( _cmdname( cpy + sizeof "MAKE=" - 1 ) == NULL )
            strcat( cpy, "wmake" );
        InsString( cpy, FALSE );
        list = Parse();
        FreeTList( list );
        if( Glob.microsoft | Glob.unix ) {
            const char *suffices = MSSuffixList;
            const char *builtins = MSBuiltIn; 
            FmtStr( cpy, "%%MAKEFLAGS=$(%%MAKEFLAGS) %F", makeopts );
            InsString( cpy, FALSE );
            list = Parse();
            FreeTList( list );
            if( Glob.unix ) {
                suffices = UNIXSuffixList;
                builtins = UNIXBuiltIn;
            }
            FmtStr( buf, "%F", builtins );
            FmtStr( cpy, buf, makeopts );
            InsString( cpy, FALSE );
            list = Parse();
            FreeTList( list );
            FmtStr( buf, "%F", suffices );
        } else {
            FmtStr( buf, "%F", SuffixList );
        }
        FmtStr( cpy, buf, makeopts );
        InsString( cpy, FALSE );
        list = Parse();
        FreeTList( list );
        FreeSafe( cpy );
        DoingBuiltIn = FALSE;
    }
}
#pragma off(check_stack);

static void setFirstTarget( TLIST *potential_first )
{
    if( firstTargFound != NULL || potential_first == NULL ) {
        return;
    }
    /*  Note all first targets must not have attribute explicit */

    firstTargFound = potential_first;
}

void Header( void )
/*****************/
{
    if( Glob.noheader ) {
        return;
    }
    if( !Glob.headerout ) {
        Glob.headerout = TRUE;  /* so we don't print more than once */
        PrtMsg( INF| BANNER );
    }
}


STATIC void handleMacroDefn( char *buf )
/*********************************************
 * Can't use Parse() at this point because we need readonly macros, so we
 * simply use LexToken().
 * This statement is not true for microsoft option.  In microsoft, all the
 * macros even the one on the command line can be overwritten
 */
{
    char    *p;
    int     pos;

    assert( buf != NULL );

    buf = StrDupSafe( buf );    /* we need our own copy */

    p = strpbrk( buf, "#=" );
    assert( p != NULL );
    *p = '=';                   /* lex doesn't recognize '#' */

    InsString( buf, FALSE );     /* put arg into stream */
    while( LexToken( LEX_PARSER ) != STRM_END ) {
        /* NOP - eat all the characters */
    }

    if( Glob.microsoft ) {
        Glob.macreadonly = FALSE;
        /* Insert twice because in nmake declaring a macro in the command line */
        /* is equivalent to declaring one as is and one that is all upper case */
        pos = 0;
        while( buf[ pos ] != NULLCHAR &&
               buf[ pos ] != '=' ) {
            buf[ pos ] = (char) toupper( buf[ pos ] );
            ++pos;
        }

        InsString( buf, TRUE );     /* put arg into stream */
        while( LexToken( LEX_PARSER ) != STRM_END ) {
            /* NOP - eat all the characters */
        }
        Glob.macreadonly = TRUE;
    }
    FreeSafe( buf );
}


STATIC void handleTarg( const char *buf )
/***************************************/
{
    assert( buf != NULL );

    /* if it is not a valid target name, it won't be able to be
       made by Update() */
    WildTList( &mustTargs, buf, FALSE, TRUE );
}


STATIC void checkCtrl( const char *p )
/************************************/
{
    assert( p != NULL );

    if( p )
        while( *p ) {          /* scan for control characters */
            if( !isprint( *p ) ) {
                PrtMsg( FTL| CTRL_CHAR_IN_CMD, *p );
            }
            ++p;
        }
}


STATIC char *procFlags( char const * const *argv, const char **log_name )
/************************************************************************
 * process the flags, macro=defn, and targets from the command line
 * writes targets to mustTargs, files to fileToDo, and defines macros
 * it is important to have the ms switch first to have correct functionality
 * of some features in microsoft compatability
 */
{
    char    select;     /* - or swchar (*argv)[0]                   */
    char    option;     /* the option (*argv)[1]                    */
    const char *p;      /* working pointer to *argv                 */
    NODE    *new;       /* for adding a new file                    */
    int      options[256 + 1] = { 0 };

    if( ( p = argv[1] ) != NULL ) {
        if( strcmp( p, "?" ) == 0 
        || ( p[0] == '-' || p[0] == Glob.swchar ) && strcmp( p+1, "?" ) == 0 ) {
            Usage();
        }
    }

    Glob.macreadonly = TRUE;

    while( *++argv != NULL ) {
        checkCtrl( p = *argv );
        select = p[0], option = (char) toupper( p[1] );
        if( select == '-' || select == Glob.swchar ) {
            if( option != NULLCHAR && p[2] == NULLCHAR ) {
                switch( option ) {
                case '?':   Usage();                break;
                case 'A':   Glob.all       = TRUE;  break;
                case 'B':   Glob.block     = TRUE;  break;
                case 'C':   Glob.nocheck   = TRUE;  break;
                case 'D':   Glob.debug     = TRUE;  break;
                case 'E':   Glob.erase     = TRUE;  break;
                case 'H':   Glob.noheader  = TRUE;  break;
                case 'I':   Glob.ignore    = TRUE;  break;
                case 'J':   Glob.rcs_make  = TRUE;  break;
                case 'K':   Glob.cont      = TRUE;  break;
                case 'M':   Glob.nomakeinit= TRUE;  break;
                case 'N':   Glob.noexec    = TRUE;  break;
                case 'O':   Glob.optimize  = TRUE;  break;
                case 'P':   Glob.print     = TRUE;  break;
                case 'Q':   Glob.query     = TRUE;  break;
                case 'R':   Glob.overide   = TRUE;  break;
                case 'S':   Glob.silent    = TRUE;  break;
                case 'T':   Glob.touch     = TRUE;  break;
                case 'U':   Glob.unix      = TRUE;  break;
                case 'V':   Glob.verbose   = TRUE;  break;
                case 'W':   Glob.auto_depends = TRUE;break;
#ifdef CACHE_STATS
                case 'X':   Glob.cachestat = TRUE;  break;
#endif
                case 'Y':   Glob.show_offenders = TRUE; break;
                case 'Z':   Glob.hold      = TRUE;  break;
                    /* these options require a filename */
                case 'F':
                case 'L':
                    if( ( p = *++argv ) == NULL ) {
                        PrtMsg( ERR| INVALID_FILE_OPTION, select, option );
                        Usage();
                    }
                    checkCtrl( p );
                    if( option == 'F' ) {
                        if ( ( p[0] == '-' ) && ( p[1] == NULLCHAR ) ) {
                            // stdin
                        } else if ( ( p[0] == '-' ) || ( p[0] == Glob.swchar ) ) {
                            PrtMsg( ERR| INVALID_FILE_OPTION, select, option );
                            Usage();
                        }
                        new = MallocSafe( sizeof( *new ) );
                        new->name = (char *) p;
                        new->next = filesToDo;
                        filesToDo = new;
                    } else
                        *log_name = p;
                    break;
                default:
                    PrtMsg( ERR| INVALID_OPTION, select, option );
                    Usage();
                    break;
                }
                options[ ( option | 0x20 ) + 1 ] = TRUE;
                continue;
            }
            if( toupper( p[3]) == NULLCHAR ) {
                if( option == 'M'  && toupper( p[2] ) == 'S' ) {
                    Glob.microsoft = TRUE;
                    Glob.nocheck   = TRUE;
                    options[ ( option | 0x20 ) + 1 ] = TRUE;
                    continue;
                }
                if( option == 'S'  && toupper( p[2] ) == 'N' ) {
                    Glob.silentno  = TRUE;
                    options[ ( option | 0x20 ) + 1 ] = TRUE;
                    continue;
                }
            }
        }
        if( strpbrk( p, "=#" ) != NULL ) {     /* is macro=defn */
            handleMacroDefn( (char*) p );
        } else {                /* is a target */
            handleTarg( p );
        }
    } // while( *++argv != NULL )

    if( Glob.microsoft && Glob.unix ) {
        PrtMsg( ERR| INCOMPATIBLE__OPTIONS, select, option );
        Usage();
    }

    Glob.macreadonly = FALSE;

    {
        // 120 allows for 30 options.
        size_t const optsize = 120 + ( ( *log_name ) ? strlen( *log_name ) + 1: 0 ) + 1;
        char * const makeopts = MallocSafe( optsize );
        unsigned opt_index;
        char default_option[] = " -?";

        makeopts[0] = 0;
        opt_index = 'a' - 1;
        while( ++opt_index <= 'z' ) {
            if( options[ opt_index + 1 ] ) {
                switch( opt_index ) {
                case 'f':
                case 'n':
                    break;
                case 'l':
                    strcat( makeopts, ( *makeopts ) ? " -l " : "-l " );
                    strcat( makeopts, *log_name );
                    break;
                case 'm':
                    if( Glob.nomakeinit ) {
                        strcat( makeopts, ( *makeopts ) ? " -m" : "-m" );
                    }
                    if( Glob.microsoft ) {
                        strcat( makeopts, ( *makeopts ) ? " -ms" : "-ms" );
                    }
                    break;
                case 's':
                    if( Glob.silentno ) {
                        strcat( makeopts, ( *makeopts ) ? " -sn" : "-sn" );
                    } else {
                        strcat( makeopts, ( *makeopts ) ? " -s" : "-s" );
                    }
                    break;
                default:
                    default_option[2] = (char) opt_index;
                    strcat( makeopts, default_option + (int) ( *makeopts == '\0' ) );
                }
            }

        }
        return( makeopts );
    }
}


STATIC const char *procLogName( const char * const *argv )
/*****************************************************************
 Find log file name
 */
{
    const char *p;      /* working pointer to *argv                 */

    while( *++argv != NULL ) {
        p = *argv;
        if( ( ( p[0] == '-' ) || ( p[0] == Glob.swchar ) ) && ( toupper( p[1] ) == 'L' ) && ( p[2] == NULLCHAR ) ) {
            return ( ( p = *++argv ) == NULL || ( p[0] == '-' ) || ( p[0] == Glob.swchar ) ) ? NULL : p;
        }
    }
    return( NULL );
}

STATIC void parseFiles( void )
/*****************************
 * Parse()s each of the files in filesToDo
 * post:    filesToDo == NULL
 */
{
    char    *p;
    NODE    *cur;
    NODE    *newhead;
    RET_T   ret;

    Glob.preproc = TRUE;            /* turn on preprocessor */

                                    /* process makeinit */
    if( !Glob.nomakeinit ) {
        if ( Glob.microsoft ) {
            ret = InsFile( TOOLSINI_NAME, TRUE );
        } else {
            ret = InsFile( MAKEINIT_NAME, TRUE );
        }
        if( ret == RET_SUCCESS ) {
            setFirstTarget( Parse() );
            if( firstTargFound != NULL ) {
                PrtMsg( WRN| MAKEINIT_HAS_TARGET );
            }
        }
    }

    if( filesToDo == NULL ) {
        ret = InsFile( MAKEFILE_NAME, FALSE );
        if( ret == RET_SUCCESS ) {
            setFirstTarget( Parse() );
        }
    } else {
        newhead = NULL;     /* reverse order of files stacked by procFlags */
        while( filesToDo != NULL ) {
            cur = filesToDo;
            filesToDo = cur->next;

            cur->next = newhead;
            newhead = cur;
        }

        while( newhead != NULL ) {
            cur = newhead;
            newhead = cur->next;
            p = cur->name;
            FreeSafe( cur );
            if( p[0] == '-' && p[1] == NULLCHAR ) { /* handle -f - */
                InsOpenFile( STDIN );
                ret = RET_SUCCESS;
            } else {
                ret = InsFile( p, FALSE );
            }
            if( ret == RET_SUCCESS ) {
                setFirstTarget( Parse() );
            } else {
                PrtMsg( ERR| UNABLE_TO_INCLUDE, p );
            }
        }
    }

    if( !Glob.nomakeinit ) {
        if( ! Glob.microsoft ) {
            ret = InsFile( MAKEFINI_NAME, TRUE );
            if( ret == RET_SUCCESS ) {
                setFirstTarget( Parse() );
            }
        }
    }

    Glob.preproc = FALSE;           /* turn off preprocessor */
}


STATIC void print( void )
/***********************/
{
    PrintMacros();
    PrintSuffixes();
    PrintTargets();
}

STATIC void ignoreNoCommands( const TLIST *targ )
{
    TLIST const *current;

    current = targ;
    // set targets to be OK if there are no commands to update it
    while( current != NULL ) {
        current->target->allow_nocmd = TRUE;
        current = current->next;
    }
}

STATIC RET_T doMusts( void )
/**************************/
{
    RET_T   ret;

    if( firstTargFound == NULL && mustTargs == NULL ) {
        PrtMsg( FTL| NO_TARGETS_SPECIFIED );
    }

    UpdateInit();

    if( mustTargs == NULL ) {
        ignoreNoCommands( firstTargFound );
        ret = MakeList( firstTargFound );
    } else {
        ignoreNoCommands( mustTargs );
        ret = MakeList( mustTargs );
    }

    UpdateFini();
    return( ret );
}


STATIC void globInit( void )
/**************************/
{
    Glob.swchar = (char) SwitchChar();
}


STATIC void init( char const * const *argv )
/******************************************/
{
    char    *makeopts;
    char const *log_name;

    LogInit( NULL );
    globInit();
    MemInit();          /* memory handlers          */
    if( !MsgInit() ) exit( EXIT_FAILURE );
    VecInit();          /* vector strings           */
    CacheInit();        /* directory cacheing       */
    MacroInit();        /* initialize macros        */
    TargetInit();       /* target must come before  */
    SuffixInit();       /* suffix                   */
    LexInit();
    ExecInit();
    AutoDepInit();

#ifdef __NT__
#if __WATCOMC__ >= 1100
    _fileinfo = 0;      /* C Library Kludge -------------------------- */
#endif
#endif
#ifdef __OS2__
    _grow_handles( 100 ); /* Some OS/2 versions allow only 20 files open by default */
#endif
    filesToDo = NULL;
    mustTargs = NULL;
    log_name = NULL;
    LogFini();
    LogInit( procLogName( argv ) );
    makeopts = procFlags( argv, &log_name );
    ParseInit();
    doBuiltIns( makeopts );
    FreeSafe( makeopts );
}


int ExitSafe( int rc )
/********************/
{
    static BOOLEAN busy = FALSE;    /* recursion protection */

    if( !busy ) {
        busy = TRUE;
        if( rc == EXIT_ERROR || rc == EXIT_FATAL ) {
            PrtMsg( ERR| MAKE_ABORT );
        }
#ifndef NDEBUG
        while( filesToDo != NULL ) {
            NODE * const cur = filesToDo;
            filesToDo = cur->next;
            FreeSafe( cur );
        }
        if( mustTargs != NULL ) {
            FreeTList( mustTargs );
        }
        if ( firstTargFound != NULL ) {
            FreeTList( firstTargFound );
        }
#endif

        AutoDepFini();
        ExecFini();
        LexFini();
        SuffixFini();       /* suffix must come before target   */
        TargetFini();
        MacroFini();
        CacheFini();
        VecFini();
#ifndef NDEBUG
        PutEnvFini();
        DLLFini();
#endif
        MemFini();
        MsgFini();
        LogFini();
    }

    return( rc );
}

#ifndef __WATCOMC__
char **_argv;
#else
#pragma off(unreferenced);
#endif
#if !defined( __WINDOWS__ )
int main( int argc, char * const *argv )
#else
int wmake_main( int argc, char * const *argv )
#endif
#ifdef __WATCOMC__
#pragma on (unreferenced);
#endif
/*********************************************/
{
    assert( argv[argc] == NULL );       /* part of ANSI standard */
#ifndef __WATCOMC__
    _argv = argv;
#endif
    InitSignals();
    InitHardErr();
    init( (const char **) argv );        /* initialize, process cmdline */
    Header();
    parseFiles();
    if( Glob.print ) {
        print();
        return( ExitSafe( EXIT_OK ) );
    }
    if( Glob.erroryet ) {
        return( ExitSafe( EXIT_ERROR ) );
    }
    if( doMusts() != RET_SUCCESS ) {
        return( ExitSafe( EXIT_ERROR ) );
    }
    ParseFini();
    return( ExitSafe( EXIT_OK ) );
}

#if 0
#ifdef __NT__
// remove when mktime is faster
char *getenv( const char *name )
    {
        register char **envp;
        register char *p;
        register int len;

        envp = environ;
        if( envp != NULL  &&  name != NULL ) {
            len = strlen( name );
            for( ; p = *envp; ++envp ) {
                if( strnicmp( p, name, len ) == 0 ) {
                    if( p[ len ] == '=' ) return( &p[ len+1 ] );
                }
            }
            // after search in case user sets TZ
            if( ! Glob.disable_TZ_kludge && strnicmp( name, "TZ", len ) == 0 ) {
                return( "EST5DST" );
            }
        }
        return( NULL );                 /* not found */
    }
#endif
#endif

