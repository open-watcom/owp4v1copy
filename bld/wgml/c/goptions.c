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
* Description:  wgml cmdline option processing
*
****************************************************************************/


#include "wgml.h"
#include "gvars.h"

#include <io.h>
#include <fcntl.h>


static char             *optParm;
static unsigned         optValue;
static char             *optScanPtr;

static char             fileattr[32];

typedef struct option
{
    char                *option;        // the option
    short               optionLenM1;    // length of option - 1
    short               minLength;      // minimum abbreviation
    int                 value;          // sometimes value to set option to
    void                (*function)( struct option *optentry );
    int                 parmcount;      // expected number of parms
} option, *Poption;




/***************************************************************************/
/*  Format error in cmdline                                                */
/***************************************************************************/

static char *badCmdLine( char * msg, char *str, char n )
{
    char        *p;
    char        buffer[128];

    p = buffer;
    for( ; p < buffer + sizeof( buffer ) - 1; ) {
        if( *str == '\0' ) break;
        *p++ = *str++;
        if( *str == '-' ) break;
        if( *str == SwitchChar ) break;

        if( *str == n ) break;         // for additional stop char '(' or ' '
    }
    *p = '\0';
    GBanner();
    OutMsg( msg, buffer );
    ErrCount++;
    return( str );
}



/***************************************************************************/
/*  ignore option consuming option parms if neccessary                     */
/***************************************************************************/

static void ignOption( option *opt )
{

    OutMsg( "WNG_IGN_OPTION %s\n", opt->option );
    WngCount++;
    if( opt->parmcount > 0 ) {
        char    *p = optScanPtr;
        int     k;
        char    c;

        while( *p == ' ' || *p == '\t' ) ++p;
        for( k = 0; k < opt->parmcount; k++ ) {
            for( ;; ) {
                c = *p++;
                if( c == ' ' ) break;
                if( c == '-' ) break;
                if( c == '(' ) break;
                if( c == SwitchChar ) break;
                if( c == '\0' )  break;
            }
            if( c == '\0' ) break;
        }
        optScanPtr = p;
    }
    return;
}


/***************************************************************************/
/*  ( altext xxx                                                           */
/***************************************************************************/

static void set_altext( option *opt )
{
    char        *pw;
    char        *p = optParm;
    int         len;

    len = optScanPtr - p;
    if( len > 0 ) {
        pw = AltExt;
        if( *p != '.' ) {
            *pw++ = '.';                // make extension start with .
        }
        while( len > 0 ) {
             len--;
             *pw++ = *p++;
        }
        *pw = '\0';
    }

};

/***************************************************************************/
/*  ( del x     set GML delimiter                                          */
/***************************************************************************/

static void set_delim( option *opt )
{
    if( optScanPtr - optParm != 1 ) {   // not length 1
        OutMsg( "ERR_INVALID_OPTION_VALUE %s %s\n", opt->option, optParm );
        ErrCount++;
        GMLChar = ';';                  // set default
    } else {
        GMLChar = *optParm;
    }
}

/***************************************************************************/
/*  ( output filename   or (T:1234)filename                                */
/***************************************************************************/

static void set_outfile( option *opt )
{
    int len;

    if( optScanPtr - optParm < 1 ) {    //
        OutMsg( "ERR_INVALID_OPTION_VALUE %s\n", opt->option );
        ErrCount++;
        OutFile = NULL;
    } else {
        len = optScanPtr - optParm;
        OutFile = GMemAlloc( len + 1 );
        memcpy( OutFile, optParm, len );
        *(OutFile + len) = '\0';
        splitAttrFile( OutFile );
        if( fileattr[0] ) {
            OutMsg( "ERR_FILEATTR_IGNORED %s %s\n", fileattr, OutFile );
            WngCount++;
            OutFileAttr = GMemAlloc( 1 + strlen( fileattr ) );
            strcpy( OutFileAttr, fileattr );
        } else {
            OutFileAttr = NULL;
        }
    }
}

/***************************************************************************/
/*  ( Bind even odd                                                        */
/***************************************************************************/

static void set_bind( option *opt )
{
    ignOption ( opt );
}

static void set_quiet( option *opt )    { GlobalFlags.quiet = opt->value; };

#if 0
static void set_wscript( option *opt )  { GlobalFlags.wscript = opt->value; };
#endif


/* struct option and logic adapted from bld\cc\c\coptions.c */
/* not all of the following is used             */
/* '=' indicates optional '=' */
/* '#' indicates a decimal numeric value */
/* '$' indicates identifier */
/* '@' indicates filename */
/* '*' indicates additional characters will be scanned by option routine */
/* if a capital letter appears in the option, then input must match exactly */
/* otherwise all input characters are changed to lower case before matching */


/*options in format of WGML i.e.,   ( option         */

static option GML_old_Options[] =
{   /* optionname                   Processing routine
                 length -1                          Parmcount
                           min Abbrev
                              default value         */
    { "altextension$", 11, 6, 0,    set_altext,     1 },
    { "bind",          3,  1, 0,    set_bind,       2 },
    { "cpinch",        5,  3, 10,   ignOption,      1 },
    { "delim$",        4,  3, 0,    set_delim,      1 },
    { "device",        5,  3, 0,    ignOption,      1 },
    { "description",   10, 4, 0,    ignOption,      1 },
    { "duplex",        5,  3, 0,    ignOption,      0 },
/*  { "file",          3,  4, 0,    set_OPTFile,    1 }, */
/*    file is handled directly in ProcOptions() ) */
    { "font",          3,  4, 0,    ignOption,      5 },
    { "fontfamily",    9,  5, 0,    ignOption,      0 },
    { "format",        5,  4, 0,    ignOption,      1 },
    { "from",          3,  4, 1,    ignOption,      1 },
    { "inclist",       6,  4, 1,    ignOption,      0 },
    { "index",         4,  4, 0,    ignOption,      0 },
    { "layout",        5,  3, 0,    ignOption,      1 },
    { "linemode",      7,  4, 0,    ignOption,      0 },
    { "llength",       6,  2, 130,  ignOption,      1 },
    { "logfile",       6,  3, 0,    ignOption,      1 },
    { "lpinch",        5,  3, 6,    ignOption,      1 },
    { "mailmerge",     8,  4, 0,    ignOption,      1 },
    { "noduplex",      7,  5, 0,    ignOption,      0 },
    { "noinclist",     8,  6, 0,    ignOption,      0 },
    { "noindex",       6,  5, 0,    ignOption,      0 },
    { "nopause",       6,  3, 0,    ignOption,      0 },
    { "noquiet",       6,  3, 0,    ignOption,      0 },
    { "noscript",      7,  5, 0,    ignOption,      0 },
    { "nostatistics",  11, 6, 0,    ignOption,      0 },
    { "nowait",        5,  6, 0,    ignOption,      0 },
    { "nowarning",     8,  6, 0,    ignOption,      0 },
    { "output$",       5,  3, 0,    set_outfile,    1 },
    { "passes",        5,  4, 0,    ignOption,      1 },
    { "pause",         4,  5, 1,    ignOption,      0 },
    { "process",       6,  4, 0,    ignOption,      1 },
    { "quiet",         4,  5, 1,    ignOption,      0 },
    { "resetscreen",   10, 5, 1,    ignOption,      0 },
    { "script",        5,  3, 1,    ignOption,      0 },
    { "setsymbol",     8,  3, 0,    ignOption,      2 },
    { "statistics",    9,  4, 1,    ignOption,      0 },
    { "terse",         4,  5, 1,    ignOption,      0 },
    { "to",            1,  2, 0,    ignOption,      1 },
    { "valueset",      7,  6, 0,    ignOption,      1 },
    { "verbose",       6,  4, 1,    ignOption,      0 },
    { "wait",          3,  4, 1,    ignOption,      0 },
    { "warning",       6,  4, 1,    ignOption,      0 },
    { "wscript",       6,  4, 1,    ignOption,      0 },
    { NULL, 0, 0, 0, ignOption, 0 }             /* end marker*/
};

/* options in new format   -o                        */
static option GML_new_Options[] =
{
    { "q",            0,  1, 1,     set_quiet,      0 },
    { NULL, 0, 0, 0, ignOption, 0 }             /* end marker*/
};


/* uses global TokenBuf */
char *CollectEnvOrFileName( char *str )
{
    char        *env;
    char        ch;

    while( *str == ' ' || *str == '\t' ) ++str;
    env = TokenBuf;
    for( ;; ) {
        ch = *str;
        if( ch == '\0' ) break;
        ++str;
        if( ch == ' ' ) break;
        if( ch == '\t' ) break;
//      if( ch == '(' ) break;
        if( ch == '-' ) break;
        if( ch == SwitchChar ) break;
        *env++ = ch;
    }
    *env = '\0';
    return( str );
}


/***************************************************************************/
/*  split (t:200)Filename                                                  */
/***************************************************************************/

static void splitAttrFile( char *filename )
{
    char        *fn;
    int         k;

    fn = filename;
    k = 0;
    if( *fn == '(' ) {                  // attribute infront of filename
        do {
            fileattr[k] = *fn++;
            k++;
            if( fileattr[ k - 1 ] == ')' ) {
                break;
            }
        } while( k < sizeof( fileattr ) && fileattr[ k - 1 ] != '\0' );
        fileattr[ k ] = '\0';

        do {
            *filename++ = *fn;
        } while( *fn++ != '\0' );
    } else {
        fileattr[ 0 ] = '\0';
    }
}

char *ReadIndirectFile( const char *filename )
{
    char        *env;
    char        *str;
    int         handle;
    int         len;
    char        ch;

    env = NULL;
    handle = open( filename, O_RDONLY | O_BINARY );
    if( handle != -1 ) {
        len = filelength( handle );
        env = GMemAlloc( len + 1 );
        read( handle, env, len );
        env[len] = '\0';
        close( handle );
        // zip through characters changing \r, \n etc into ' '
        str = env;
        while( *str ) {
            ch = *str;
            if( ch == '\r' || ch == '\n' ) {
                *str = ' ';
            }
            if( ch == 0x1A ) {          // if end of file
                *str = '\0';            // - mark end of str
                break;
            }
            ++str;
        }
    }
    return( env );
}

/* test for delimiter */
static int optionDelimiter( char c )
{
    if( c == ' ' || c == '-' || c == '\0' || c == '\t' || c == '('
         || c == SwitchChar ) {
         return( 1 );
    }
    return( 0 );
}


static void stripQuotes( char *fname )
{
    char    *s;
    char    *d;

    if( *fname == '"' ) {
    /* string will shrink so we can reduce in place */
        d = fname;
        for( s = d + 1; *s && *s != '"'; ++s ) {
        /* collapse double backslashes, only then look for escaped quotes */
             if( s[0] == '\\' && s[1] == '\\' ) {
                 ++s;
             } else if( s[0] == '\\' && s[1] == '"' ) {
                 ++s;
             }
             *d++ = *s;
        }
        *d = '\0';
    }
}


static char *processOption( option *op_table, char *p, char  *option_start )
{
    int         i;
    int         j;
    char        *opt;
    char        c;

    for( i = 0; ; i++ ) {
        opt = op_table[i].option;
        j = 1;
        if( opt == NULL ) break;
        c = tolower( *p );

        if( c == *opt ) {

            optValue = op_table[i].value;
            j = 1;
            for(;;) {
                ++opt;
                if( *opt == '\0' || *opt == '*' ) {
                    if( *opt == '\0' ) {
                        if( p - option_start == 1 ) {
                            // make sure end of option
                            if( !optionDelimiter( p[j] ) ) break;
                        }
                    }
                    optScanPtr = p + j;
                    OutMsg( "INF_RECOGNIZED n1 %s\n", option_start );
                    op_table[i].function( &op_table[ i ]);
                    return( optScanPtr );
                }
                if( *opt == '#' ) {             // collect a number
                    if( p[j] >= '0' && p[j] <= '9' ) {
                        optValue = 0;
                        for(;;) {
                            c = p[j];
                            if( c < '0' || c > '9' ) break;
                            optValue = optValue * 10 + c - '0';
                            ++j;
                        }
                    }
                } else if( *opt == '$' ) {      // collect an identifer
                    optParm = &p[j];
                    for(;;) {
                        c = p[j];
                        if( c == '\0' ) break;
                        if( c == '-' ) break;
                        if( c == ' ' ) break;
                        if( c == SwitchChar ) break;
                        ++j;
                    }
                } else if( *opt == '@' ) {      // collect a filename
                    optParm = &p[j];
                    c = p[j];
                    if( c == '"' ){ // "filename"
                        for(;;){
                            c = p[++j];
                            if( c == '"' ){
                                ++j;
                                break;
                            }
                            if( c == '\0' )break;
                            if( c == '\\' ){
                                ++j;
                            }
                        }
                    }else{
                        for(;;) {
                            c = p[j];
                            if( c == '\0' ) break;
                            if( c == ' ' ) break;
                            if( c == '\t' ) break;
                            if( c == SwitchChar ) break;
                            ++j;
                        }
                    }
                } else if( *opt == '=' ) {      // collect an optional '='
                    if( p[j] == '=' || p[j] == '#' ) ++j;
                } else {
                    c = tolower( p[j] );
                    if( *opt != c ) {
                        if( *opt < 'A' || *opt > 'Z' ) break;
                        if( *opt != p[j] ) break;
                    }
                    ++j;
                }
            }
        }
    }
    p = badCmdLine( "Invalid option %s\n", option_start, ' ' );
    return( p );
}

static char *processOptionOld( option *op_table, char *p, char *option_start )
{
    int         i;
    int         j;
    int         len;
    char        *opt;
    char        c;
    char        *pp;

    pp = p;
    for( len = 1; ; len++ ) {
        pp++;
        if( optionDelimiter( *pp ) ) {
            break;
        }
    }

    for( i = 0; ; i++ ) {
        opt = op_table[i].option;
        j = 1;
        if( opt == NULL ) break;

        if( len < op_table[ i ].minLength ) {
            continue;  /* cannot be this option */
        }
        if( strnicmp( opt, p, len ) ) {
            continue;
        }
/* '=' indicates optional '=' */
/* '#' indicates a decimal numeric value */
/* '$' indicates identifier */
/* '@' indicates filename */
/* '*' indicates additional characters will be scanned by option routine */
/* if a capital letter appears in the option, then input must match exactly */
/* otherwise all input characters are changed to lower case before matching */
        {
            optValue = op_table[i].value;
            j = len;
            opt += op_table[i].optionLenM1;
            for(;;) {
                ++opt;
                if( *opt == '\0' || *opt == '*' ) {
                    if( *opt == '\0' ) {
                        if( p - option_start == 1 ) {
                            // make sure end of option
                            if( !optionDelimiter( p[j] ) ) break;
                        }
                    }
                    optScanPtr = p + j;
                    OutMsg( "INF_RECOGNIZED 1 %s\n", option_start );
                    op_table[i].function( &op_table[ i ]);
                    return( optScanPtr );
                }
                if( *opt == '#' ) {             // collect a number
                    if( p[j] >= '0' && p[j] <= '9' ) {
                        optValue = 0;
                        for(;;) {
                            c = p[j];
                            if( c < '0' || c > '9' ) break;
                            optValue = optValue * 10 + c - '0';
                            ++j;
                        }
                                      optScanPtr = p + j;
                    }
                    OutMsg( "INF_RECOGNIZED 2 %s\n", option_start );
                } else if( *opt == '$' ) {      // collect an identifer
                    if( p[ j ] == ' ' ) j++; /* skip 1 blank */
                    optParm = &p[j];
                    for(;;) {
                        c = p[j];
                        if( c == '\0' ) break;
                        if( c == '-' ) break;
                        if( c == '(' ) break;
                        if( c == ' ' ) break;
                        if( c == SwitchChar ) break;
                        ++j;
                    }
                                      optScanPtr = p + j;
                    OutMsg( "INF_RECOGNIZED 3 %s\n", option_start );
                } else if( *opt == '@' ) {      // collect a filename
                    optParm = &p[j];
                    c = p[j];
                    if( c == '"' ){ // "filename"
                        for(;;){
                            c = p[++j];
                            if( c == '"' ){
                                ++j;
                                break;
                            }
                            if( c == '\0' )break;
                            if( c == '\\' ){
                                ++j;
                            }
                        }
                    }else{
                        for(;;) {
                            c = p[j];
                            if( c == '\0' ) break;
                            if( c == ' ' ) break;
                            if( c == '\t' ) break;
                            if( c == SwitchChar ) break;
                            ++j;
                        }
                    }

                    OutMsg( "INF_RECOGNIZED 4 %s\n", option_start );
                } else if( *opt == '=' ) {      // collect an optional '='
                    if( p[j] == '=' || p[j] == '#' ) ++j;
                } else {
                    c = tolower( p[j] );
                    if( *opt != c ) {
                        if( *opt < 'A' || *opt > 'Z' ) break;
                        if( *opt != p[j] ) break;
                    }
                    ++j;
                                      optScanPtr = p + j;
                }
            }
            OutMsg( "INF_RECOGNIZED 5 %s\n", option_start );
        }
    }
    p = badCmdLine( "Invalid option %s\n", option_start, '(' );
    return( p );
}

#define MAX_NESTING 32

void ProcOptions( char *str )
{
    unsigned    level;
    char        *save[ MAX_NESTING ];
    char        *buffers[ MAX_NESTING ];
    char        *fileNames[ MAX_NESTING ];
    int         len;
    const int   fileOptLen = 7;
    const char  fileOpt[] = "( file ";

    if( str != NULL ) {
        level = 0;
        buffers[0] = NULL;
        for( ; ; ) {
            while( *str == ' ' || *str == '\t' ) ++str;
            len = 0;
            if( *str == '@' ) {
                len = 1;

                /***********************************************************/
                /*  accept @filename and  ( file filename                  */
                /*  for included option file                               */
                /*  filename can start with Attributes (T:200)filename     */
                /***********************************************************/

            } else if( !strnicmp( str, fileOpt, fileOptLen ) ) {
                len = fileOptLen;
            }
            if( len > 0 ) {
                if( level < MAX_NESTING ) {
                    save[level] = CollectEnvOrFileName( str + len );
                    ++level;
                    buffers[ level ] = NULL;
                    fileNames[ level ] = NULL;
                    TryFileName = NULL;
                    str = NULL;
                    splitAttrFile( TokenBuf );
                    if( fileattr[0]  ) {
                        OutMsg( "ERR_FILEATTR_IGNORED %s %s\n", fileattr,
                                TokenBuf );
                        WngCount++;
                    }
                    if( SearchFileinDirs( TokenBuf, OPT_EXT, "",
                                          DS_cur_lib_inc_path ) ) {
                        bool  skip = FALSE;
                        fclose( Tryfp );
                        if( level > 1 ) {
                            int     k;
                            for( k = level - 1; k > 0; k-- ) {
                                if( stricmp( TryFileName, fileNames[ k ]) == 0 ) {
                                    OutMsg( "ERR_RECURSIVE_CMD_FILE %s\n",
                                            TryFileName );
                                    ErrCount++;
                                    skip = TRUE;
                                }
                            }
                        }
                        if( !skip ) {
                            fileNames[ level ] = TryFileName;
                            str = ReadIndirectFile( TryFileName );
                            buffers[level] = str;
                        }
                    } else {
                        OutMsg( "ERR_OPTION_FILE_NOTFOUND %s %s\n", fileattr,
                                TokenBuf );
                        ErrCount++;
                    }
                    if( str != NULL )  continue;
                    if( TryFileName != NULL ) GMemFree( TryFileName );
                    str = save[--level];
                } else {                         // max nseting leve exceeded
                    str = CollectEnvOrFileName( str + len );
                    splitAttrFile( TokenBuf );
                    if( fileattr[0]  ) {
                        OutMsg( "ERR_FILEATTR_IGNORED %s %s\n", fileattr,
                                TokenBuf );
                        WngCount++;
                    }
                    if( SearchFileinDirs( TokenBuf, OPT_EXT, AltExt,
                                          DS_cur_lib_inc_path ) ) {
                        fclose( Tryfp );
                    }
                    if( TryFileName != NULL ) GMemFree( TryFileName );
                    OutMsg( "ERR_MAX_NESTING_OPTION_FILE %s\n",
                            TokenBuf );
                    ErrCount++;
                }
            }
            while( *str == ' ' || *str == '\t' ) ++str;
            if( *str == '\0' ) {
                if( level == 0 ) break;
                if( buffers[level] != NULL ) {
                    GMemFree( buffers[level] );
                    buffers[level] = NULL;
                }
                str = save[--level];
                continue;
            }
            if( *str == '-' || *str == SwitchChar ) {

                str = processOption( GML_new_Options, str + 1, str );

            } else {
                if( (*str == '(') && (*(str +1) == ' ') ) {
                    char    *optptr = str;

                    str += 2;
                    while( *str == ' ' || *str == '\t' ) ++str;

                    str = processOptionOld( GML_old_Options, str, optptr );

                } else {  /* collect  file name */
                    char    *beg;
                    char    *p;
                    int     len;

                    beg = str;
                    if( *str == '"' ){
                        for(;;){
                            ++str;
                            if( *str == '"' ){
                                ++str;
                                break;
                            }
                            if( *str == '\0' ) break;
                            if( *str == '\\' ){
                                ++str;
                            }
                        }
                    } else {
                        for(;;) {
                            if( *str == '\0' ) break;
                            if( *str == ' '  ) break;
                            if( *str == '\t'  ) break;
                            if( *str == SwitchChar ) break;
                            ++str;
                        }
                    }
                    len = str - beg;
                    p = (char *) GMemAlloc( len + 1 );
                    memcpy( p, beg, len );
                    p[ len ] = '\0';
                    stripQuotes( p );
                    splitAttrFile( p );
                    if( fileattr[0]  ) {
                        OutMsg( "ERR_FILEATTR_IGNORED %s %s\n", fileattr, p );
                        WngCount++;
                        MasterFNameAttr = GMemAlloc( 1 + strlen( fileattr ) );
                        strcpy( MasterFNameAttr, fileattr );
                    } else {
                        MasterFNameAttr = NULL;
                    }
                    if( MasterFName != NULL ) {
                        /* more than one master file ? */
                        GBanner();
                        str = badCmdLine( "ERR_SOURCE_FILE_SPECIFIED_TWICE %s\n",
                                          beg, ' ' );
                        GMemFree( MasterFName );
                    }
                    MasterFName = p;
                }
            }
        }
    }
}

