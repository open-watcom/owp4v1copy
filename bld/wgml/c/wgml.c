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
* Description:  WGML top level driver module and file I/O.
*               not yet functional
*   some logic / ideas adopted from Watcom Script 3.2 IBM S/360 Assembler
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "banner.h"

#ifdef  TRMEM

/***************************************************************************/
/*  memory tracking use project code from bld\trmem                        */
/***************************************************************************/

    #include "trmem.h"

    _trmem_hdl  handle;

    static void prt( int * fhandle, const char * buff, size_t len )
    /*************************************************************/
    {
        size_t i;

        fhandle = fhandle;
        for( i = 0; i < len; ++i ) {
//          fputc( *buff++, stderr );
            fputc( *buff++, stdout );
        }
    }

#endif


#if defined( __UNIX__ )
    #define IS_PATH_SEP( ch ) ((ch) == '/')
#else
    #define IS_PATH_SEP( ch ) ((ch) == '/' || (ch) == '\\')
#endif

#define CRLF            "\n"



/***************************************************************************/
/*  Program end                                                            */
/***************************************************************************/

void my_exit( int rc )
{
    exit( rc );
}


/***************************************************************************/
/*  Output Banner if wanted and not yet done                               */
/***************************************************************************/

void g_banner( void )
{
    if( !(GlobalFlags.bannerprinted || GlobalFlags.quiet) ) {
        out_msg( banner1w( "WGML Script/GML", _WGML_VERSION_ ) CRLF );
        out_msg( banner2a() CRLF );
        out_msg( banner3 CRLF );
        out_msg( banner3a CRLF );
#ifdef  TRMEM
        out_msg( CRLF "Compiled with TRMEM memory tracker (trmem)" CRLF );
#endif
        GlobalFlags.bannerprinted = TRUE;
    }
}

/***************************************************************************/
/*  Usage info TBD                                                         */
/***************************************************************************/

static void usage( void )
{
    g_banner();

    out_msg( CRLF "Usage: wgml [options] srcfile [options]" CRLF );
    out_msg( "Options:" CRLF );
    out_msg( "-q\t\tQuiet, don't show product info." CRLF );
    out_msg( "-r\t\tResearch, no formatting, only count GML/SCR keywords" CRLF );
    out_msg( "\t\tand follow .im, .ap, :include tags." CRLF );
    out_msg( "\tother options to be done / documented." CRLF );
    my_exit( 4 );
}


char *get_filename_full_path( char *buff, char const *name, size_t max )
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

void *mem_alloc( size_t size )
{
    void    *p;

    #ifdef TRMEM
        p = _trmem_alloc( size, _trmem_guess_who(), handle );
    #else
        p = malloc( size );
    #endif
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL" );
        err_count++;
        g_suicide();
    }
    return( p );
}

/***************************************************************************/
/*  Re-allocate some storage                                               */
/***************************************************************************/

void *mem_realloc( void *p, size_t size )
{
    #ifdef TRMEM
        p = _trmem_realloc( p, size, _trmem_guess_who(), handle );
    #else
        p = realloc( p, size );
    #endif
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL" );
        err_count++;
        g_suicide();
    }
    return( p );
}

/***************************************************************************/
/*  Free storage                                                           */
/***************************************************************************/

void mem_free( void *p )
{
    #ifdef TRMEM
        _trmem_free( p, _trmem_guess_who(), handle );
    #else
        free( p );
    #endif
    p = NULL;
}


/***************************************************************************/
/*  Try to close an opened include file                                    */
/***************************************************************************/

static bool free_inc_fp( void )
{
    filecb      *cb;
    int         rc;

    cb = file_cbs;
    while( cb != NULL ) {
        if( cb->flags & FF_open ) {
            rc = fgetpos( cb->fp, &cb->pos );
            if( rc != 0 ) {
                out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                err_count++;
                g_suicide();
            }
            rc = fclose( cb->fp );
            if( rc != 0 ) {
                out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                err_count++;
                g_suicide();
            }
            cb->flags &= ~FF_open;
            return TRUE;
        }
        cb = cb->prev;
    }
    return FALSE;                       // nothing to close
}


static void reopen_inc_fp( filecb *cb )
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
            if( !free_inc_fp() ) break; // try closing an include file
        }
        if( erc == 0 ) {
            rc = fsetpos( cb->fp, &cb->pos );
            if( rc != 0 ) {
                out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                err_count++;
                g_suicide();
            }
            cb->flags |= FF_open;
        } else {
            out_msg( "ERR_FILE_IO %d %s\n", erc2, cb->filename );
            err_count++;
            g_suicide();
        }
    }
}

/***************************************************************************/
/*  Compose full path / filename and try to open for reading               */
/***************************************************************************/

int try_open( char *prefix, char *separator, char *filename, char *suffix )
{
    int         i;
    FILE        *fp;
    char        buf[ FILENAME_MAX ];
    errno_t     erc;

    i = 0;
    while( (buf[i] = *prefix++) )    ++i;
    while( (buf[i] = *separator++) ) ++i;
    while( (buf[i] = *filename++) )  ++i;
    while( (buf[i] = *suffix++) )    ++i;
    filename = &buf[0];                 // point to the full name

    try_file_name = NULL;
    try_fp = NULL;

    for( ;; ) {
        erc = fopen_s( &fp, filename, "rb" );
        if( erc == 0 ) break;
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
        if( !free_inc_fp() ) break;     // try closing an include file
    }
    if( fp == NULL ) return( 0 );

    try_file_name = mem_alloc( i + 2 );
    strcpy_s( try_file_name, i + 2, buf );
    try_fp = fp;
    return( 1 );
}


/***************************************************************************/
/*  Search for filename in curdir, and along environment vars              */
/***************************************************************************/

int search_file_in_dirs( char *filename, char *defext, char *altext, DIRSEQ sequence )
{
    char        buff[ FILENAME_MAX ];
    char        try[ FILENAME_MAX ];
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
        if( try_open( "", "", filename, "" ) != 0 ) return( 1 );
        if( *ext == '\0' ) {
            if( try_open( "", "", filename, defext ) != 0 ) return( 1 );
            if( *altext != '\0' ) {
                if( try_open( "", "", filename, altext ) != 0 ) return( 1 );
            }
            if( strcmp(defext, GML_EXT )) { // one more try with .gml
                if( try_open( "", "", filename, GML_EXT ) != 0 ) return( 1 );
            }
        } else {
            if( *altext != '\0' ) {
                _makepath( try, drive, dir, filename, altext );
                if( try_open( "", "", try, "" ) != 0 ) return( 1 );
            }
        }
        return( 0 );
    }
    /* no absolute path specified, try curr dir */
    if( try_open( "", "", filename, "" ) != 0 ) return( 1 );
    if( *ext == '\0' ) {
        if( try_open( "", "", filename, defext ) != 0 ) return( 1 );
        if( *altext != '\0' ) {
            if( try_open( "", "", filename, altext ) != 0 ) return( 1 );
        }
        if( strcmp(defext, GML_EXT )) { // one more try with .gml
            if( try_open( "", "", filename, GML_EXT ) != 0 ) return( 1 );
        }
    } else {
        if( *altext != '\0' ) {
            _makepath( try, drive, dir, name, altext );
            if( try_open( "", "", try, "" ) != 0 ) return( 1 );
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
    } else {
        searchdirs[ 0 ] = NULL;
        searchdirs[ 1 ] = NULL;
        searchdirs[ 2 ] = NULL;
    }
    for( k = 0; k < 3; k++ ) {
        p = searchdirs[ k ];
        if( p == NULL ) break;

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

#undef  SEP_LEN

            if( try_open( try, PATH_SEP, filename, "" ) != 0 ) return( 1 );

            if( *ext == '\0' ) {
                if( try_open( try, PATH_SEP, filename, defext ) != 0 ) return( 1 );
                if( *altext != '\0' ) {
                    if( try_open( try, PATH_SEP, filename, altext ) != 0 ) return( 1 );
                }
                if( strcmp(defext, GML_EXT )) { // one more try with .gml
                    if( try_open( try, PATH_SEP, filename, GML_EXT ) != 0 ) return( 1 );
                }
            } else {
                if( *altext != '\0' ) {
                    _makepath( try, drive, dir, filename, altext );
                    if( try_open( "", "", try, "" ) != 0 ) return( 1 );
                }
            }
            if( *p == INCLUDE_SEP || *p == ';' ) ++p;
        } while( *p != '\0' );
    }
    return( 0 );
}


/***************************************************************************/
/*  Set the extension of the Master input file as default extension        */
/***************************************************************************/

void set_default_extension( const char *masterfname )
{
    char        buff[ FILENAME_MAX ];
    char        *ext;

    _splitpath2( masterfname, buff, NULL, NULL, NULL, &ext );
    if( strlen( ext ) > 0) {
        if( strlen( ext ) > strlen( def_ext ) ) {
            mem_free( def_ext);
            def_ext = mem_alloc( 1+ strlen( ext ) );
        }
        strcpy_s( def_ext, 1 + strlen( ext ), ext );
    }
}


/***************************************************************************/
/*  add info about file  to LIFO list                                      */
/***************************************************************************/

static  void    add_file_cb_entry( void )
{
    filecb      *new;
    size_t      fnlen;

    fnlen = strlen( try_file_name );
    new = mem_alloc( sizeof( filecb ) + fnlen );// count for terminating \0
                                                 // is in filecb structure
    new->prev    = NULL;
    new->lineno  = 0;
    new->linemin = line_from;
    new->linemax = line_to;
    strcpy_s( new->filename, fnlen + 1, try_file_name );
    mem_free( try_file_name );

    if( try_fp ) {
        new->flags = FF_open;
        new->fp    = try_fp;
    } else {
        new->flags = FF_clear;
        new->fp    = NULL;
    }

    new->filebuf = mem_alloc( buf_size );
    new->buflen = buf_size -1;

    if( file_cbs != NULL ) {
        new->prev = file_cbs;
    }
    file_cbs = new;
}


/***************************************************************************/
/*  remove info about file  from LIFO list                                 */
/***************************************************************************/

static  void    del_file_cb_entry( void )
{
    filecb      *wk;

    wk = file_cbs;
    if( wk == NULL ) {
        return;
    }
    if( wk->flags & FF_open ) {         // close file if neccessary
       fclose( wk->fp );
    }
    if( wk->filebuf ) {
        mem_free( wk->filebuf );
    }
    file_cbs = file_cbs->prev;
    mem_free( wk );
}


/***************************************************************************/
/*  get line from current input ( file )                                   */
/*  skipping lines before the first one to process if neccessary           */
/***************************************************************************/
static  void    get_line( void )
{
    filecb      *cb;
    char        *p;

    cb = file_cbs;
    if( ! cb->flags & FF_open ) {
        reopen_inc_fp( cb );
    }
    do {
        p = fgets( cb->filebuf, cb->buflen, cb->fp );
        if( p != NULL ) {
            cb->lineno++;
            cb->scanPtr = p;
            cb->flags |= FF_startofline;
        } else {
            if( feof( cb->fp ) || cb->lineno > cb->linemax ) {
                cb->flags |= FF_eof;
                cb->flags &= ~FF_startofline;
                cb->scanPtr = cb->filebuf;
                *(cb->filebuf) = '\0';
                break;
            } else {
                out_msg( "ERR_FILE_IO %d %s\n", errno, cb->filename );
                err_count++;
                g_suicide();
            }
        }
    } while( cb->lineno < cb->linemin );
    cb->usedlen = strlen( cb->filebuf );
}


/***************************************************************************/
/*  process the input file                                                 */
/*      if research mode flag set, minimal processing                      */
/***************************************************************************/

static  void    proc_GML( char *filename )
{
    filecb      *cb;
    char        attrwork[ 32 ];

    ProcFlags.newLevel = TRUE;
    strcpy_s( token_buf, buf_size, master_fname );

    for( ; ; ) {
        if( ProcFlags.newLevel ) {

        /*******************************************************************/
        /*  start a new include file level                                 */
        /*******************************************************************/
            ProcFlags.newLevel = FALSE;

        /*******************************************************************/
        /*  split off attribute  (f:xxxx)                                  */
        /*******************************************************************/
            split_attr_file( token_buf, attrwork, sizeof( attrwork ) );

            if( attrwork[0] ) {
                out_msg( "WNG_FILEATTR_IGNORED (%s) %s\n", attrwork, token_buf );
                wng_count++;
            }
            if( search_file_in_dirs( token_buf, def_ext, alt_ext,
                                     DS_cur_inc_lib_path ) ) {

                if( inc_level >= MAX_INC_DEPTH ) {
                    out_msg( "ERR_MAX_INPUT_NESTING %s\n", token_buf );
                    err_count++;
                    continue;           // terminate this inc level
                }
            } else {
                out_msg( "ERR_INPUT_FILE_NOT_FOUND %s\n", token_buf );
                err_count++;
                continue;               // terminate this inc level
            }
            inc_level++;                // start new level
            add_file_cb_entry();
            cb = file_cbs;
            cb->linemin = line_from;
            cb->linemax = line_to;
            if( attrwork[0] ) {
                strcpy_s( cb->fileattr, sizeof( cb->fileattr ), attrwork );
            } else {
                cb->fileattr[ 0 ] = '\0';
            }
            if( GlobalFlags.inclist ) {
                out_msg( "\nCurrent file is '%s'\n", cb->filename );
            }
        }
        if( inc_level == 0 ) {
            break;                 // we are done (master document not found)
        }

        while( !(cb->flags & FF_eof) ) {

            get_line();

            if( cb->flags & (FF_eof | FF_err) ) {
                break;
            }
            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                printf( "\n%s", cb->scanPtr );
            }


            scan_line();

            if( ProcFlags.newLevel ) {  // imbed and friends found
                break;                  // start new file
            }

        }
        if( ProcFlags.newLevel ) {
            continue;
        }

        del_file_cb_entry();               // one level finished
        cb = file_cbs;
        inc_level--;
        if( inc_level == 0 ) {
            break;                      // we are done with master document
        }
    }
}

/***************************************************************************/
/*  printStats show statistics at program end                              */
/***************************************************************************/

static  void    print_stats( void )
{
    out_msg( "Statistics:\n" );
    out_msg( "  Error count: %6ld\n", err_count );
    out_msg( "Warning count: %6ld\n", wng_count );
    out_msg( "   Returncode: %6d\n",  err_count ? 8 : wng_count ? 4 : 0 );

}

/***************************************************************************/
/*  initPass                                                               */
/***************************************************************************/
static  void    init_pass( void )
{

    GlobalFlags.firstpass = pass > 1      ? FALSE : TRUE;
    GlobalFlags.lastpass  = pass < passes ? FALSE : TRUE;

    line_from   = 1;                  // processing line range Masterdocument
    line_to     = ULONG_MAX - 1;
}

/***************************************************************************/
/*  main WGML                                                              */
/***************************************************************************/

int main( int argc, char *argv[] )
{
    char       *cmdline;
    int         cmdlen;
    jmp_buf     env;

    environment = &env;
    if( setjmp( env ) ) {               // if fatal error has occurred
        my_exit( 16 );
    }
    #ifdef TRMEM

        handle = _trmem_open( &malloc, &free, &realloc, NULL, NULL, &prt,
                              _TRMEM_ALLOC_SIZE_0 | _TRMEM_REALLOC_SIZE_0 |
                              _TRMEM_REALLOC_NULL | _TRMEM_FREE_NULL |
                              _TRMEM_OUT_OF_MEMORY | _TRMEM_CLOSE_CHECK_FREE );
    #endif

    init_global_vars();

    token_buf = mem_alloc( buf_size );
    get_env_vars();

    cmdlen = _bgetcmd( NULL, 0 ) + 1;
    cmdline = mem_alloc( cmdlen );
    _bgetcmd( cmdline, cmdlen );

    out_msg( "cmdline=%s\n", cmdline );

    proc_options( cmdline );
    g_banner();

    if( master_fname != NULL ) {        // filename specified
        set_default_extension( master_fname );// make this extension first choice

        for( pass = 1; pass <= passes; pass++ ) {

            init_pass();

            out_msg( "\nStarting pass %d of %d ( %s mode ) \n", pass, passes,
                     GlobalFlags.research ? "research" : "normal" );

            proc_GML( master_fname );

            #ifdef TRMEM
                _trmem_prt_list( handle );// show allocated memory at pass end

                out_msg( "\n  End of pass %d of %d ( %s mode ) \n", pass, passes,
                     GlobalFlags.research ? "research" : "normal" );
            #endif
        }
    } else {
        out_msg( "ERR_MISSING_MAINFILENAME\n");
        err_count++;
        usage();
    }
    if( GlobalFlags.research ) {
        print_GML_tags_research();
        free_GML_tags_research();

        print_SCR_tags_research();
        free_SCR_tags_research();
    }

    mem_free( cmdline );
    if( token_buf != NULL ) {
        mem_free( token_buf );
    }
    if( alt_ext != NULL ) {
        mem_free( alt_ext );
    }
    if( def_ext != NULL ) {
        mem_free( def_ext );
    }
    if( master_fname != NULL ) {
        mem_free( master_fname );
    }
    if( master_fname_attr != NULL ) {
        mem_free( master_fname_attr );
    }
    if( out_file != NULL ) {
        mem_free( out_file );
    }
    if( out_file_attr != NULL ) {
        mem_free( out_file_attr );
    }
    if( GMLlibs != NULL ) {
        mem_free( GMLlibs );
    }
    if( GMLincs != NULL) {
        mem_free( GMLincs );
    }
    if( Pathes != NULL ) {
        mem_free( Pathes );
    }

    #ifdef TRMEM
        _trmem_prt_list( handle );
    #endif

    print_stats();

    #ifdef TRMEM
        _trmem_prt_list( handle );
        _trmem_close( handle );
    #endif

    my_exit( err_count ? 8 : wng_count ? 4 : 0 );
    return( 0 );                   // never reached, but makes compiler happy
}

