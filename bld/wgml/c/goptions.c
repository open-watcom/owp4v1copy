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

typedef struct  option
{
    char            *option;            // the option
    short           optionLenM1;        // length of option - 1
    short           minLength;          // minimum abbreviation
    long            value;              // sometimes value to set option to
    void            (*function)( struct option *optentry );
    int             parmcount;          // expected number of parms
} option;


typedef struct cmd_tok {
    struct cmd_tok  *nxt;
    size_t          toklen;
    int             bol;
    char            token[1];           // variable length

} cmd_tok;

static unsigned     level;              // include level 0 = cmdline
static char         *buffers[ MAX_NESTING ];
static cmd_tok      *cmd_tokens[ MAX_NESTING ];
static char         *file_names[ MAX_NESTING ];
static cmd_tok      *sav_tokens[ MAX_NESTING ];
static char         *save[ MAX_NESTING ];
static char         *opt_parm;
static char         *opt_scan_ptr;
static long         opt_value;
static cmd_tok      *tokennext;


/***************************************************************************/
/*  free storage for tokens at specified include level                     */
/***************************************************************************/

static  void    free_tokens( int lvl )
{
    cmd_tok         *tok;
    cmd_tok         *wk;

    tok = cmd_tokens[ lvl ];
    cmd_tokens[ lvl ] = NULL;

    while( tok != NULL ) {
        wk = tok->nxt;
        mem_free( tok );
        tok = wk;
    }
}

/***************************************************************************/
/*  split a line into blank delimited words                                */
/***************************************************************************/

static  int     split_tokens( char *str )
{
    cmd_tok         *tok;
    cmd_tok         *new;
    int             cnt;
    char            *tokstart;
    char            *tokend;
    int             linestart;
    size_t          tokl;

    linestart = 1;                      // assume start of line
    cnt = 0;                            // found tokens

    tok = cmd_tokens[ level ];
    if( tok != NULL ) {
        while( tok->nxt != NULL ) {
            tok = tok->nxt;
        }
    }

    for( ;; ) {
        while( *str == ' ' )  str++;    // skip leading blanks
        if( *str == '\0' ) {
            break;
        }
        tokstart = str;
        while( *str ) {
            if( *str == ' ' || *str == '\n' ) {
                break;
            }
            str++;
        }
        cnt++;
        tokend = str;
        tokl = tokend - tokstart;
        if( *str == '\n' ) {
            linestart =  1;
            str++;
        }
        if( tokl == 0 ) {
            continue;
        }

        new = mem_alloc( sizeof( *new ) + tokl );
        new->nxt = NULL;
        new->bol = linestart;
        linestart = 0;
        new->toklen = tokl;
        strncpy_s(new->token, new->toklen + 1, tokstart, tokl );

        if( tok == NULL ) {
            cmd_tokens[ level ] = new;
        } else {
            tok->nxt = new;
        }
        tok = new;
    }
    return( cnt );
}



/***************************************************************************/
/*  Format error in cmdline                                                */
/***************************************************************************/

static  char    *bad_cmd_line( char * msg, char *str, char n )
{
    char        *p;
    char        buffer[128];

    p = buffer;
    split_tokens( p );
    for( ; p < buffer + sizeof( buffer ) - 1; ) {
        if( *str == '\0' ) break;
        if( *str == '\n' ) break;
        *p++ = *str++;
        if( *str == '-' ) break;
        if( *str == switch_char ) break;

        if( *str == n ) break;         // for additional stop char '(' or ' '
    }
    *p = '\0';
    g_banner();
    out_msg( msg, buffer );
    err_count++;
    return( str );
}


/***************************************************************************/
/*  read an option file into memory                                        */
/***************************************************************************/

char *read_indirect_file( const char *filename )
{
    char        *buf;
    char        *str;
    int         handle;
    int         len;
    char        ch;

    buf = NULL;
    handle = open( filename, O_RDONLY | O_BINARY );
    if( handle != -1 ) {
        len = filelength( handle );
        buf = mem_alloc( len + 1 );
        read( handle, buf, len );
        buf[ len ] = '\0';
        close( handle );
        // zip through characters changing \r into ' '
        str = buf;
        while( *str ) {
            ch = *str;
            if( ch == '\r' ) {
                *str = ' ';
            } else if( ch == 0x1A ) {   // if end of file
                *str = '\0';            // - mark end of str
                break;
            }
            ++str;
        }
    }
    return( buf );
}


/***************************************************************************/
/*  ignore option consuming option parms if neccessary                     */
/***************************************************************************/

static void ign_option( option *opt )
{

    out_msg( "WNG_IGN_OPTION %s\n", opt->option );
    wng_count++;
    if( opt->parmcount > 0 ) {
        char    *p = opt_scan_ptr;
        int     k;

        while( *p == ' ' || *p == '\t' ) ++p;
        for( k = 0; k < opt->parmcount; k++ ) {
            if( tokennext == NULL )  break;
            if( tokennext->bol ) break;
            if( tokennext->token[ 0 ] == '(' ) break;
            tokennext = tokennext->nxt;
        }
    }
    return;
}


/***************************************************************************/
/*  ( altext xxx    set alternate extension                                */
/***************************************************************************/

static void set_altext( option *opt )
{
    char        *pw;
    char        *p;
    int         len;

    if( tokennext != NULL ) {
        len = tokennext->toklen;
        p = tokennext->token;
        if( alt_ext ) {
            mem_free( alt_ext );
        }
        alt_ext = mem_alloc( len + 2 );
        pw = alt_ext;
        if( *p != '.' ) {
            *pw++ = '.';                // make extension start with .
        }
        while( len > 0 ) {
             len--;
             *pw++ = *p++;
        }
        *pw = '\0';
        tokennext = tokennext->nxt;
    }
}

/***************************************************************************/
/*  ( Bind odd [even]   1 or 2 Horizontal Space values                     */
/***************************************************************************/

static void set_bind( option *opt )
{
    bool        scanerr;
    char        *p;
    su          bindwork;

    if( tokennext == NULL || tokennext->bol || tokennext->token[ 0 ] == '(' ) {
        out_msg( "ERR_MISSING_OPTION_VALUE %.*s\n", opt->optionLenM1+1,
                opt->option );
        err_count++;

    } else {
        p = tokennext->token;
        scanerr = to_internal_SU( &p, &bindwork );
        if( scanerr ) {
            out_msg( "ERR_INVALID_OPTION_VALUE %.*s %s\n", opt->optionLenM1+1,
                    opt->option, tokennext->token );
            err_count++;
            tokennext = tokennext->nxt;
        } else {
            memcpy_s( &bind_odd, sizeof( bind_odd), &bindwork, sizeof( bindwork ) );

            out_msg( "\tbind odd  value %li '%s' %li %li \n", bind_odd.su_conv,
                    bind_odd.su_txt, bind_odd.su_whole, bind_odd.su_dec );

            tokennext = tokennext->nxt; // check for optional bind even val
            if( tokennext == NULL || tokennext->bol ||
                tokennext->token[ 0 ] == '(' ) {

                memcpy_s( &bind_even, sizeof( bind_even), &bind_odd,
                          sizeof( bind_odd ) );  // use bind_odd
            } else {
                p = tokennext->token;
                scanerr = to_internal_SU( &p, &bindwork );
                if( scanerr ) {
                    out_msg( "ERR_INVALID_OPTION_VALUE %.*s %s\n",
                            opt->optionLenM1+1, opt->option, tokennext->token );
                    err_count++;
                } else {
                    memcpy( &bind_even, &bindwork, sizeof( bindwork ) );
                    out_msg( "\tbind even value %li '%s' %li %li \n",
                            bind_even.su_conv, bind_even.su_txt, bind_even.su_whole,
                            bind_even.su_dec );
                }
                tokennext = tokennext->nxt;
            }
        }
    }
}

/***************************************************************************/
/*  ( delim x     set GML delimiter                                        */
/***************************************************************************/

static void set_delim( option *opt )
{
    if( tokennext == NULL || tokennext->toklen != 1 ) {       // not length 1
        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s %s\n", opt->option,
                tokennext == NULL ? " " : tokennext->token );
        err_count++;
        GML_char = GML_CHAR_DEFAULT;    // set default :
    } else {
        GML_char = tokennext->token[0]; // new delimiter
    }
}

/***************************************************************************/
/*  ( output      filename or (T:1234)filename                             */
/***************************************************************************/

static void set_outfile( option *opt )
{
    int     len;
    char    attrwork[ MAX_FILE_ATTR ];

    if( tokennext == NULL || tokennext->bol ) {
        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s\n", opt->option );
        err_count++;
        out_file = NULL;
        out_file_attr = NULL;
    } else {
        len = tokennext->toklen;
        out_file = mem_alloc( len + 1 );

        memcpy_s( out_file, len + 1, tokennext->token, len );
        *(out_file + len) = '\0';

        split_attr_file( out_file, attrwork, sizeof( attrwork ) );
        if( attrwork[ 0 ] ) {
            out_msg( "WNG_FILEATTR_IGNORED (%s) %s\n", attrwork, out_file );
            wng_count++;
            len = 1 + strlen( attrwork );
            out_file_attr = mem_alloc( len );
            strcpy_s( out_file_attr, len, attrwork );
        } else {
            out_file_attr = NULL;
        }
        tokennext = tokennext->nxt;
    }
}


/***************************************************************************/
/*  convert string to integer                                              */
/***************************************************************************/
static  long    get_num_value( char *p )
{
    char    c;
    int     j;
    long    value;

    value = 0;
    j = 0;
    for( ;; ) {
        c = p[j];
        if( c < '0' || c > '9' ) break;
        value = value * 10 + c - '0';
        ++j;
    }
    return( value );
}



/***************************************************************************/
/*  ( passes n                                                             */
/***************************************************************************/

static void set_passes( option *opt )
{
    char    *p;

    if( tokennext == NULL || tokennext->bol ||
        tokennext->token[ 0 ] == '(' ) {

        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s\n", opt->option );
        err_count++;
        passes = opt->value;            // set default value
    } else {
        p = tokennext->token;
        opt_value = get_num_value( p );

        if( opt_value < 1 || opt_value > MAX_PASSES ) {
            out_msg( "ERR_PASSES_VALUE_OUT_OF_RANGE %d\n", opt_value );
            err_count++;
            passes = opt->value;        // set default value
        } else {
            passes = opt_value;
        }
        tokennext = tokennext->nxt;
    }
}

/***************************************************************************/
/*  ( from n   start printing at pageno n                                  */
/***************************************************************************/

static void set_from( option *opt )
{
    char    *p;

    if( tokennext == NULL || tokennext->bol ||
        tokennext->token[ 0 ] == '(' ) {

        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s\n", opt->option );
        err_count++;
        print_from = opt->value;        // set default value
    } else {
        p = tokennext->token;
        opt_value = get_num_value( p );
        if( opt_value < 1 || opt_value >= LONG_MAX ) {
            out_msg( "ERR_The_from_option_value_must_be_greater_than_zero\n" );
            err_count++;
            print_from = opt->value;    // set default value
        } else {
            print_from = opt_value;
        }
        tokennext = tokennext->nxt;
    }
}

/***************************************************************************/
/*  ( to n     stop  printing at pageno n                                  */
/***************************************************************************/

static void set_to( option *opt )
{
    char    *p;

    if( tokennext == NULL || tokennext->bol ||
        tokennext->token[ 0 ] == '(' ) {

        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s\n", opt->option );
        err_count++;
        print_to = opt->value;          // set default value
    } else {
        p = tokennext->token;
        opt_value = get_num_value( p );
        if( opt_value < 1 || opt_value >= LONG_MAX ) {
            out_msg( "ERR_The_to_option_value_must_be_greater_than_zero\n" );
            err_count++;
            print_to = opt->value;      // set default value
        } else {
            print_to = opt_value;
        }
        tokennext = tokennext->nxt;
    }
}

/***************************************************************************/
/*  ( inclist   or   ( noinclist                                           */
/***************************************************************************/

static void set_inclist( option *opt )
{
    GlobalFlags.inclist = opt->value;
}


/***************************************************************************/
/*  ( file xxx    command option file                                      */
/***************************************************************************/

static void set_OPTFile( option *opt )
{
    int     len;
    char    attrwork[ MAX_FILE_ATTR ];
    char    *str;


    if( tokennext == NULL || tokennext->bol
        /* || tokennext->token[ 0 ] == '('  allow (t:123)file.opt construct */
                                         ) {
        out_msg( "ERR_INVALID_MISSING_OPTION_VALUE %s\n", opt->option );
        err_count++;
    } else {
        len = tokennext->toklen;

        str = tokennext->token;

        out_msg( "INF_RECOGNIZED_OPTION_FILE %s\n", str );
        strcpy_s( token_buf, buf_size, str );
        if( try_file_name != NULL ) {
            mem_free( try_file_name );
        }
        split_attr_file( token_buf, attrwork, sizeof( attrwork ) );
        if( attrwork[0]  ) {
            out_msg( "WNG_FILEATTR_IGNORED (%s) %s\n", attrwork, token_buf );
            wng_count++;
        }
        if( level < MAX_NESTING ) {
            sav_tokens[ level ] = tokennext->nxt;

            buffers[ level + 1 ] = NULL;
            file_names[ level + 1] = NULL;
            if( search_file_in_dirs( token_buf, OPT_EXT, "",
                                  DS_cur_lib_inc_path ) ) {
                bool  skip = FALSE;

                fclose( try_fp );
                if( level > 0 ) {
                    int     k;

                    for( k = level; k > 0; k-- ) {
                        if( stricmp( try_file_name, file_names[ k ]) == 0 ) {
                            out_msg( "ERR_The_option_file_'%s'_is_recursively_included\n",
                                    try_file_name );
                            err_count++;
                            skip = TRUE;
                            break;
                        }
                    }
                }
                if( !skip ) {
                    file_names[ ++level ] = try_file_name;

                    str = read_indirect_file( try_file_name );
                    split_tokens( str );
                    mem_free( str );
                    try_file_name = NULL;// free will be done via file_names[ level ]
                    tokennext = cmd_tokens[ level ];
                    return;
                }
            } else {
                out_msg( "ERR_OPTION_FILE_NOT_FOUND %s %s\n", attrwork,
                        token_buf );
                err_count++;
            }
            if( str == NULL )  {
                if( try_file_name != NULL ) mem_free( try_file_name );
                if( file_names[ level ] != NULL ) mem_free( file_names[ level ] );
                str = save[--level];
                tokennext = sav_tokens[ level ];
            }
        } else {                        // max nesting level exceeded
            if( try_file_name != NULL ) mem_free( try_file_name );
            out_msg( "ERR_MAX_NESTING_OPTION_FILE %s\n", token_buf );
            err_count++;
        }
        tokennext = tokennext->nxt;
    }
}

/***************************************************************************/
/*  Processing routines for 'new format' options                           */
/***************************************************************************/

static void set_quiet( option *opt )    { GlobalFlags.quiet = opt->value; };

static void set_research( option *opt ) { GlobalFlags.research = opt->value; };

#if 0     // always set wscript option, don't allow to disable
static void set_wscript( option *opt )  { GlobalFlags.wscript = opt->value; };
#endif


/* struct option and logic adapted from bld\cc\c\coptions.c */

/*options in format of WGML i.e.,   ( option xxx     */

static option GML_old_Options[] =
{   /* optionname                      Processing routine
                 length -1                             Parmcount
                           min Abbrev
                              default value         */
    { "altextension",  11, 6, 0,       set_altext,     1 },
    { "bind",          3,  1, 0,       set_bind,       1 },
    { "cpinch",        5,  3, 10,      ign_option,     1 },
    { "delim",         4,  3, 0,       set_delim,      1 },
    { "device",        5,  3, 0,       ign_option,     1 },
    { "description",   10, 4, 0,       ign_option,     1 },
    { "duplex",        5,  3, 0,       ign_option,     0 },
    { "file",          3,  4, 0,       set_OPTFile,    1 },
    { "font",          3,  4, 0,       ign_option,     5 },
    { "fontfamily",    9,  5, 0,       ign_option,     0 },
    { "format",        5,  4, 0,       ign_option,     1 },
    { "from",          3,  4, 1,       set_from,       1 },
    { "inclist",       6,  4, 1,       set_inclist,    0 },
    { "index",         4,  4, 0,       ign_option,     0 },
    { "layout",        5,  3, 0,       ign_option,     1 },
    { "linemode",      7,  4, 0,       ign_option,     0 },
    { "llength",       6,  2, 130,     ign_option,     1 },
    { "logfile",       6,  3, 0,       ign_option,     1 },
    { "lpinch",        5,  3, 6,       ign_option,     1 },
    { "mailmerge",     8,  4, 0,       ign_option,     1 },
    { "noduplex",      7,  5, 0,       ign_option,     0 },
    { "noinclist",     8,  6, 0,       set_inclist,    0 },
    { "noindex",       6,  5, 0,       ign_option,     0 },
    { "nopause",       6,  3, 0,       ign_option,     0 },
    { "noquiet",       6,  3, 0,       ign_option,     0 },
    { "noscript",      7,  5, 0,       ign_option,     0 },
    { "nostatistics",  11, 6, 0,       ign_option,     0 },
    { "nowait",        5,  6, 0,       ign_option,     0 },
    { "nowarning",     8,  6, 0,       ign_option,     0 },
    { "output",        5,  3, 0,       set_outfile,    1 },
    { "passes",        5,  4, 1,       set_passes,     1 },
    { "pause",         4,  5, 1,       ign_option,     0 },
    { "process",       6,  4, 0,       ign_option,     1 },
    { "quiet",         4,  5, 1,       ign_option,     0 },
    { "resetscreen",   10, 5, 1,       ign_option,     0 },
    { "script",        5,  3, 1,       ign_option,     0 },
    { "setsymbol",     8,  3, 0,       ign_option,     2 },
    { "statistics",    9,  4, 1,       ign_option,     0 },
    { "terse",         4,  5, 1,       ign_option,     0 },
    { "to",            1,  2, INT_MAX, set_to,         1 },
    { "valueset",      7,  6, 0,       ign_option,     1 },
    { "verbose",       6,  4, 1,       ign_option,     0 },
    { "wait",          3,  4, 1,       ign_option,     0 },
    { "warning",       6,  4, 1,       ign_option,     0 },
    { "wscript",       6,  4, 1,       ign_option,     0 },
    { NULL, 0, 0, 0, ign_option, 0 }              /* end marker*/
};

/* options in 'new' format   -o                        */
static option GML_new_Options[] =
{
    { "q",            0,  1, 1,        set_quiet,      0 },
    { "r",            0,  1, 1,        set_research,   0 },
    { NULL, 0, 0, 0, ign_option, 0 }              /* end marker*/
};


/***************************************************************************/
/*  split (t:200)Filename    into t:200 and Filename                       */
/***************************************************************************/

void split_attr_file( char *filename , char *attr, size_t attrlen )
{
    char        *fn;
    char        *p;
    size_t      k;

    fn = filename;
    p = attr;
    if( *fn == '(' ) {                  // attribute infront of filename
        k = 0;
        while( ++k < attrlen ) {
            *p++ = *++fn;               // isolate attribute
            if( *fn == ')' ) {
                break;
            }
        }
        *--p = '\0';                    // terminate attr

        p = filename;
        while( *fn != '\0' ) {          // shift filename
            *p++ = *++fn;
        }
    } else {
        *p = '\0';                      // no attr
    }
}


/***************************************************************************/
/*  test for delimiter                                                     */
/***************************************************************************/

static int option_delimiter( char c )
{
    if( c == ' ' || c == '-' || c == '\0' || c == '\t' || c == '(' ||
        c == switch_char || c == '\n' ) {
        return( 1 );
    }
    return( 0 );
}


/***************************************************************************/
/*                                                                         */
/***************************************************************************/

static void strip_quotes( char *fname )
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


/***************************************************************************/
/*  process 'new' option -x  /x                                            */
/***************************************************************************/

static cmd_tok  *process_option( option *op_table, cmd_tok *tok )
{
    int         i;
    int         j;
    char        *opt;
    char        c;
    char        *option_start;
    char        *p;


    option_start = tok->token;
    p = option_start;
    c = tolower( *p );
    if( option_delimiter( c ) ) {
        p++;
        c = tolower( *p );
    }
    tokennext = tok->nxt;
    for( i = 0; ; i++ ) {
        opt = op_table[i].option;
        j = 1;
        if( opt == NULL ) break;        // not found

        if( c == *opt ) {               // match for first char

            opt_value = op_table[i].value;
            j = 1;
            for(;;) {
                ++opt;
                if( *opt == '\0' ) {
                    if( p - option_start == 1 ) {
                                        // make sure end of option
                        if( !option_delimiter( p[j] ) ) break;
                    }
                    opt_scan_ptr = p + j;
                    out_msg( "INF_RECOGNIZED n1 %s\n", option_start );
                    op_table[i].function( &op_table[ i ]);
                    return( tokennext );
                }
                c = tolower( p[j] );
                if( *opt != c ) {
                    if( *opt < 'A' || *opt > 'Z' ) break;
                    if( *opt != p[j] ) break;
                }
                ++j;
            }
        }
    }
    p = bad_cmd_line( "Invalid option %s\n", option_start, ' ' );
    return( tokennext );
}


/***************************************************************************/
/*  process 'old' option ( xxx                                             */
/***************************************************************************/

static cmd_tok  *process_option_old( option *op_table, cmd_tok *tok )
{
    int         i;
    int         j;
    int         len;
    char        *opt;
    char        c;
    char        *p;
    char        *option_start;

    p = tok->token;
    option_start = p;
    len = tok->toklen;
    tokennext = tok->nxt;
    c = tolower( *p );
    if(  c == '(' ) {
        if( len == 1 ) {
            return( tokennext );        // skip single (
        }
        p++;
        c = tolower( *p );
    }
    for( i = 0; ; i++ ) {
        opt = op_table[i].option;
        j = 1;
        if( opt == NULL ) break;
        if( c != *opt )  continue;
        if( len < op_table[ i ].minLength ) {
            continue;                   // cannot be this option
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
        opt_value = op_table[i].value;
        j = len;
        opt += op_table[i].optionLenM1;
        for(;;) {
            ++opt;
            if( *opt == '\0' || *opt == '*' ) {
                if( *opt == '\0' ) {
                    if( p - option_start == 1 ) {
                                        // make sure end of option
                        if( !option_delimiter( p[j] ) ) break;
                    }
                }
                opt_scan_ptr = p + j;
                out_msg( "INF_RECOGNIZED 1 %s\n", option_start );
                op_table[i].function( &op_table[ i ]);
                return( tokennext );
            }
            if( *opt == '#' ) {         // collect a number
                while( p[ j ] == ' ' ) {// skip blanks
                    ++j;
                }
                if( p[j] >= '0' && p[j] <= '9' ) {
                    opt_value = 0;
                    for(;;) {
                        c = p[j];
                        if( c < '0' || c > '9' ) break;
                        opt_value = opt_value * 10 + c - '0';
                        ++j;
                    }
                    opt_scan_ptr = p + j;
                }
                out_msg( "INF_RECOGNIZED num %s\n", option_start );
            } else if( *opt == '$' ) {  // collect an identifer
                if( p[ j ] == ' ' ) j++;// skip 1 blank

                opt_parm = &p[j];
                for(;;) {
                    c = p[j];
                    if( c == '-' ) break;
                    if( c == '(' ) break;
                    if( c == ' ' ) break;
                    if( c == switch_char ) break;
                    if( c == '\n' ) {
                        p[j] = ' ';
                        break;
                    }
                    if( c == '\0' ) break;
                    ++j;
                }
                opt_scan_ptr = p + j;
                out_msg( "INF_RECOGNIZED id %s\n", option_start );
            } else if( *opt == '@' ) {  // collect a filename
                opt_parm = &p[j];
                c = p[j];
                if( c == '"' ){         // "filename"
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
                        if( c == ' ' ) break;
                        if( c == '\t' ) break;
                        if( c == switch_char ) break;
                        if( c == '\n' ) {
                            p[j] = ' ';
                            break;
                        }
                        if( c == '\0' ) break;
                        ++j;
                    }
                }
                out_msg( "INF_RECOGNIZED fn %s\n", option_start );
            } else if( *opt == '=' ) {  // collect an optional '='
                if( p[j] == '=' || p[j] == '#' ) ++j;
            } else {
                c = tolower( p[j] );
                if( *opt != c ) {
                    if( *opt < 'A' || *opt > 'Z' ) break;
                    if( *opt != p[j] ) break;
                }
                ++j;
                opt_scan_ptr = p + j;
            }
        }
        out_msg( "INF_RECOGNIZED 5 %s\n", option_start );
    }
    p = bad_cmd_line( "ERR_INVALID_OPTION %s\n", option_start, '(' );
    return( tokennext );
}


/***************************************************************************/
/*  get Document master input file name                                    */
/***************************************************************************/

static cmd_tok  *process_master_filename( cmd_tok *tok )
{
    char        attrwork[ MAX_FILE_ATTR ];
    char        *p;
    char        *str;
    int         len;

    len = tok->toklen;
    p = (char *) mem_alloc( len + 1 );
    memcpy_s( p, len + 1, tok->token, len );
    p[ len ] = '\0';
    out_msg( "INF_RECOGNIZED_DOCUMENT_SOURCE_FILE %s\n", p );
    strip_quotes( p );
    if( master_fname != NULL ) {         // more than one master file ?
        g_banner();
        str = bad_cmd_line(
                "ERR_Document_source_file_specified_more_than_once %s\n",
                tok->token, ' ' );
        mem_free( p );
    } else {
        split_attr_file( p , attrwork, sizeof( attrwork ) );
        if( attrwork[0]  ) {
            out_msg( "WNG_FILEATTR_IGNORED (%s)\n", attrwork );
            wng_count++;
            master_fname_attr = mem_alloc( 1 + strlen( attrwork ) );
            strcpy( master_fname_attr, attrwork );
        } else {
            master_fname_attr = NULL;
        }
        master_fname = p;
    }
    return( tok->nxt );
}


/***************************************************************************/
/*  process command line and option files                                  */
/***************************************************************************/

void proc_options( char *string )
{
    int         tokcount;
    cmd_tok     *tok;
    int         sol;                    // start of line switch
    char        c;

    level = 0;                     // option file include level: 0 == cmdline
    buffers[ 0 ] = NULL;
    cmd_tokens[ 0 ] = NULL;
    tokcount = split_tokens( string );
    out_msg( "INF_Tokencount_cmdline %d\n", tokcount );

    tok = cmd_tokens[ level ];
    for( ; ; ) {
        while( tok != NULL ) {
            sol = tok->bol;
            c = tok->token[ 0 ];

            if( c == '-' || c == switch_char ) {

            /***************************************************************/
            /*  process 'new' options -x or /x                             */
            /***************************************************************/
                tok = process_option( GML_new_Options, tok );

                if( tok != NULL && tok->bol == 0 ) {

                    /*******************************************************/
                    /*  allow master filename if following 'new' option    */
                    /*******************************************************/
                    tok->bol = sol;
                }
            } else {
                if( c == '(' || !sol ) {

                    /*******************************************************/
                    /*  process 'old' options  ( xxx                       */
                    /*******************************************************/
                    tok = process_option_old( GML_old_Options, tok );
                } else {

                    /*******************************************************/
                    /*  collect Document source file name                  */
                    /*******************************************************/
                    tok = process_master_filename( tok );
                }
            }
        }
        if( buffers[ level ] != NULL ) {
            mem_free( buffers[ level ] );
            buffers[ level ] = NULL;
        }
        if( cmd_tokens[ level ] != NULL ) {
            free_tokens( level );
        }
        if( file_names[ level ] != NULL ) {
            mem_free( file_names[ level ] );
        }
        if( level == 0 ) break;
        tok = sav_tokens[ --level ];
    }
    if( print_to < print_from  ) {
        g_banner();
        err_count++;
        out_msg( "ERR_MASTERFILE_LINE_RANGE_INVALID %ld - %ld\n", print_from,
                print_to );
    }
    return;
}
