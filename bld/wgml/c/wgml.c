/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  wgml top level driver module and file I/O.
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "banner.h"

/* #include "gscan.h" */



#ifndef _MAX_PATH
    #define _MAX_PATH   (PATH_MAX + 1)
#endif
#ifndef _MAX_PATH2
    #define _MAX_PATH2  (PATH_MAX + 4)
#endif

#if defined( __UNIX__ )
    #define IS_PATH_SEP( ch ) ((ch) == '/')
#else
    #define IS_PATH_SEP( ch ) ((ch) == '/' || (ch) == '\\')
#endif

#define MAX_INC_DEPTH   255
#define CRLF            "\n"


// local function prototypes
void    *GMemAlloc( unsigned size );
void    GMemFree( void *p );



/***************************************************************************/
/*  Program end                                                            */
/***************************************************************************/

void MyExit( int rc )
{
    exit( rc );
}


/***************************************************************************/
/*  Output Banner if wanted and not yet done                               */
/***************************************************************************/

void GBanner( void )
{
    if( !(GlobalFlags.bannerprinted || GlobalFlags.quiet) ) {
        OutMsg( banner1w( "WGML Script/GML", _WGML_VERSION_ ) CRLF );
        OutMsg( banner2a() CRLF );
        OutMsg( banner3 CRLF );
        OutMsg( banner3a CRLF );
        GlobalFlags.bannerprinted = TRUE;
    }
}

/***************************************************************************/
/*  Usage info TBD                                                         */
/***************************************************************************/

static void usage( void )
{
    GBanner();

    OutMsg( "Usage: wgml [options] srcfile" CRLF );
    OutMsg( "Options:" CRLF );
    OutMsg( "-q\t\tQuiet, don't show product info." CRLF );
    OutMsg( "\tothers to be done / documented." CRLF );
    MyExit( 4 );
}


char *GetFilenameFullPath( char *buff, char const *name, size_t max )
/********************************************************************/
{
    char        *p;

    p = _fullpath( buff, name, max );
    if( p == NULL )
        p = (char *)name;

#ifdef __UNIX__
    if( (p[0] == '/' && p[1] == '/') && (name[0] != '/' || name[1] != '/') ) {
        //
        // if the _fullpath result has a node number and
        // the user didn't specify one, strip the node number
        // off before returning
        //
        p += 2;
        while( *(p++) != '/' ) ;
    }
#endif
    return( p );
}


/***************************************************************************/
/*  Allocate some storage                                                  */
/***************************************************************************/

void *GMemAlloc( unsigned size )
{
    void    *p;

    p = malloc( size );
    if( p == NULL ) {
        OutMsg( "ERR_NOMEM_AVAIL" );
        ErrCount++;
        GSuicide();
    }
    return( p );
}

/***************************************************************************/
/*  Free storage                                                           */
/***************************************************************************/

void GMemFree( void *p )
{
    free( p );
}


/***************************************************************************/
/*  Try to close an opened include file                                    */
/***************************************************************************/

static bool freeIncFP( void )
{
    FILECB      *cb;
    int         rc;

    cb = FileCbs;
    while( cb != NULL ) {
        if( cb->flags & FF_open ) {
            rc = fgetpos( cb->fp, &cb->pos );
            if( rc != 0 ) {
                OutMsg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                ErrCount++;
                GSuicide();
            }
            rc = fclose( cb->fp );
            if( rc != 0 ) {
                OutMsg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                ErrCount++;
                GSuicide();
            }
            cb->flags &= ~FF_open;
            return TRUE;
        }
        cb = cb->prev;
    }
    return FALSE;                       // nothing to close
}


static void reopenIncFP( FILECB *cb )
{
    int         rc;
    errno_t     erc;
    errno_t     erc2;

    if( ! cb->flags & FF_open ) {
        for( ;; ) {
            erc = fopen_s( &cb->fp, cb->filename, "rb" );
            if( erc == 0 ) break;
            erc2 = errno;
            if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
            if( !freeIncFP() ) break;       // try closing an include file
        }
        if( erc == 0 ) {
            rc = fsetpos( cb->fp, &cb->pos );
            if( rc != 0 ) {
                OutMsg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                ErrCount++;
                GSuicide();
            }
            cb->flags |= FF_open;
        } else {
            OutMsg( "ERR_FILE_IO %d %s\n", erc2, cb->filename );
            ErrCount++;
            GSuicide();
        }
    }
}

/***************************************************************************/
/*  Compose full path / filename and try to open for reading               */
/***************************************************************************/

int TryOpen( char *prefix, char *separator, char *filename, char *suffix )
{
    int         i;
    FILE        *fp;
    char        buf[ _MAX_PATH2 ];
    errno_t     erc;

    i = 0;
    while( (buf[i] = *prefix++) )    ++i;
    while( (buf[i] = *separator++) ) ++i;
    while( (buf[i] = *filename++) )  ++i;
    while( (buf[i] = *suffix++) )    ++i;
    filename = &buf[0];                 // point to the full name

    TryFileName = NULL;
    Tryfp = NULL;

    for( ;; ) {
        erc = fopen_s( &fp, filename, "rb" );
        if( erc == 0 ) break;
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
        if( !freeIncFP() ) break;       // try closing an include file
    }
    if( fp == NULL ) return( 0 );

    TryFileName = GMemAlloc( i + 2 );
    strcpy_s( TryFileName, i + 2, buf );
    Tryfp = fp;
    return( 1 );
}


/***************************************************************************/
/*  Search for filename in curdir, and along environment vars              */
/***************************************************************************/

int  SearchFileinDirs( char * filename, char * defext, char * altext, DIRSEQ sequence )
{
    char        buff[_MAX_PATH2];
    char        try[_MAX_PATH];
    char        *drive;
    char        *dir;
    char        *name;
    char        *ext;
    char        *p;
    char        *searchdirs[ 3 ];
    int         i;
    int         k;

    _splitpath2( filename, buff, &drive, &dir, &name, &ext );
    if( drive[0] != '\0' || IS_PATH_SEP(dir[0]) ) {
        /* Drive or path from root specified */
        if( TryOpen( "", "", filename, "" ) != 0 ) return( 1 );
        if( *ext == '\0' ) {
            if( TryOpen( "", "", filename, defext ) != 0 ) return( 1 );
            if( *altext != '\0' ) {
                if( TryOpen( "", "", filename, altext ) != 0 ) return( 1 );
            }
        } else {
            if( *altext != '\0' ) {
                _makepath( try, drive, dir, filename, altext );
                if( TryOpen( "", "", try, "" ) != 0 ) return( 1 );
            }
        }
        return( 0 );
    }
    /* no absolute path specified, try curr dir */
    if( TryOpen( "", "", filename, "" ) != 0 ) return( 1 );
    if( *ext == '\0' ) {
        if( TryOpen( "", "", filename, defext ) != 0 ) return( 1 );
        if( *altext != '\0' ) {
            if( TryOpen( "", "", filename, altext ) != 0 ) return( 1 );
        }
    } else {
        if( *altext != '\0' ) {
            _makepath( try, drive, dir, name, altext );
            if( TryOpen( "", "", try, "" ) != 0 ) return( 1 );
        }
    }

    /* now try the dirs in specified sequence */
    if( sequence == DS_cur_lib_inc_path ) {
        searchdirs[ 0 ] = GMLlibs;
        searchdirs[ 1 ] = GMLincs;
        searchdirs[ 2 ] = Pathes;
    } else if( sequence == DS_cur_inc_lib_path ) {
        searchdirs[ 0 ] = GMLincs;
        searchdirs[ 1 ] = GMLlibs;
        searchdirs[ 2 ] = Pathes;
    } else if( sequence == DS_cur_lib_path ) {
        searchdirs[ 0 ] = GMLlibs;
        searchdirs[ 1 ] = Pathes;
        searchdirs[ 2 ] = NULL;
    }
    for( k = 0; k < 3; k++ ) {
        p = searchdirs[ k ];
        if( p == NULL ) continue;

        do {
            i = 0;
            while( *p == ' ' ) ++p;
            for( ; ; ) {
                if( *p == INCLUDE_SEP || *p == ';' ) break;
                if( *p == '\0' ) break;
                if( i < sizeof( try ) - 2 ) {
                    try[ i++ ] = *p;
                }
                ++p;
            }
            while( i != 0 ) {
                if( try[ i-1 ] != ' ' ) break;
                --i;
            }
#define SEP_LEN (sizeof( PATH_SEP ) - 1)
            try[ i ] = '\0';
            if( i >= SEP_LEN && strcmp( &try[ i - SEP_LEN ], PATH_SEP ) == 0 ) {
                try[ i - SEP_LEN ] = '\0';
            }
            if( TryOpen( try, PATH_SEP, filename, "" ) != 0 ) return( 1 );


            if( *ext == '\0' ) {
                if( TryOpen( try, PATH_SEP, filename, defext ) != 0 ) return( 1 );
                if( *altext != '\0' ) {
                    if( TryOpen( try, PATH_SEP, filename, altext ) != 0 ) return( 1 );
                }
            } else {
                if( *altext != '\0' ) {
                    _makepath( try, drive, dir, filename, altext );
                    if( TryOpen( "", "", try, "" ) != 0 ) return( 1 );
                }
            }
            if( *p == INCLUDE_SEP || *p == ';' ) ++p;
        } while( *p != '\0' );
    }
    return( 0 );
}

/***************************************************************************/
/*  Add info about file  to LIFO list                                      */
/***************************************************************************/

static  void    addFileCbEntry( void )
{
    FILECB      *new;

    new = GMemAlloc( sizeof( FILECB ) + strlen( TryFileName ) );
    new->prev  = NULL;
    new->lineno = 0;
    new->flags = FF_open;
    strcpy( new->filename, TryFileName );
    new->fp = Tryfp;
    new->filebuf = GMemAlloc( BufSize );
    new->currchar = '\n';               // for reading

    if( FileCbs == NULL ) {
        new->level = 1;
    } else {
        new->prev = FileCbs;
        new->level = FileCbs->level + 1;
    }
    FileCbs = new;
}

/***************************************************************************/
/*  remove info about file  from LIFO list                                 */
/***************************************************************************/

static  void    delFileCbEntry( FILECB *cb )
{
    FILECB      *wk;

    GMemFree( cb->filebuf );
    wk = FileCbs;
    if( wk == cb ) {
        FileCbs = wk->prev;
    }
    GMemFree( cb );
}


/***************************************************************************/
/*  get line from current input source                                     */
/***************************************************************************/
static  void    getLine( void )
{
    FILECB      *cb;
    char        *p;

    cb = FileCbs;
    if( ! cb->flags & FF_open ) {
        reopenIncFP( cb );
    }
    if( cb->currchar == '\n' ) {
        p = fgets( cb->filebuf, cb->buflen, cb->fp );
        if( p != NULL ) {
            cb->lineno++;
            cb->currchar = *p;
            cb->scanPtr = p;
            cb->flags |= FF_startofline;
        } else {
            if( feof( cb->fp ) ) {
                cb->flags |= FF_eof;
                cb->flags &= ~FF_startofline;
                cb->currchar = '\0';
                cb->scanPtr = cb->filebuf;
                *(cb->filebuf) = '\0';
            } else {
                OutMsg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                ErrCount++;
                GSuicide();
            }
        }
#if 0
    } else {
        cb->flags &= ~FF_startofline;
        if( cb->currchar = CWSepChar ) {
            cb->currchar = *(++cb->scanPtr);
        } else {
            OutMsg( "ERR_NOT_LLstart \n" );
            ErrCount++;
            GSuicide();
        }
#endif
    }
}

/***************************************************************************/
/*  process the input file                                                 */
/***************************************************************************/

static  void    procGML( char *filename )
{
    FILECB      *cb;

    strcpy( TokenBuf, filename );
    if( SearchFileinDirs( TokenBuf, GML_EXT, AltExt, DS_cur_inc_lib_path ) ) {
        addFileCbEntry();

        getLine();
        cb = FileCbs;
        OutMsg( "Processing GML File ( still dummy ) \n" );


/* fill in processing */
/* fill in processing */
/* fill in processing */
/* fill in processing */
/* fill in processing */
/* fill in processing */
/* fill in processing */
/* fill in processing */


        delFileCbEntry( FileCbs );
    } else {
        OutMsg( "ERR_MASTER_INPUT_FILE_NOT_FOUND %s\n", filename );
        ErrCount++;
        return;
    }
}


/***************************************************************************/
/*  main                                                                   */
/***************************************************************************/

int main( int argc, char *argv[] )
{
    char        cmdline[ BUF_SIZE ];
    jmp_buf     env;

    Environment = &env;
    if( setjmp( env ) ) {               // if fatal error has occurred
        MyExit( 16 );
    }

    InitGlobalVars();

    BufSize = BUF_SIZE;
    Buffer = GMemAlloc( BufSize );
    TokenBuf = GMemAlloc( BufSize );

    GetEnvVars();

    getcmd( cmdline );

    OutMsg( "cmdline=%s\n", cmdline );

    ProcOptions( cmdline );
    GBanner();

    if( MasterFName != NULL ) {         // filename specified
        procGML( MasterFName );
    } else {
        OutMsg( "ERR_MISSING_MAINFILENAME\n");
        ErrCount++;
        usage();
    }

    return( ErrCount ? 8 : WngCount ? 4 : 0 );
}

