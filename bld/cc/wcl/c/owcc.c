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
* Description:  POSIX style compiler driver.
*
****************************************************************************/


/* FIXME
 *  if linking is done, remove objects afterwards?
 *  unrecognized options should warn, possibly error
 *  should owcc output a warning message if -b names unknown target?
 *  -S should remove .o files
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "getopt.h"
#include <process.h>
#include <malloc.h>
#include <conio.h>

#include "clcommon.h"
#include "banner.h"
#undef  _BANEXTRA
#define _BANEXTRA _BANEXSHORT

#if defined(__UNIX__)
#define strfcmp strcmp
#else
#define strfcmp stricmp
#endif

#define DIS         "wdis"
#if defined( _M_I86 )
#define CC          "wcc"             /* Open Watcom C compiler (16-bit)   */
#define CCXX        "wpp"             /* Open Watcom C++ compiler (16-bit) */
#else
#define CC          "wcc386"          /* Open Watcom C compiler (32-bit)   */
#define CCXX        "wpp386"          /* Open Watcom C++ compiler (32-bit) */
#endif
#define WCLENV      "OWCC"
#define _NAME_      "C/C++ "

#ifdef __UNIX__
#define PATH_SEP_STR "/"
#else
#define PATH_SEP_STR "/\\"
#endif
#define OUTPUTFILE  "a.out"
#define LINK        "wlink"             /* Open Watcom linker              */
#define TEMPFILE    "@__wcl__.lnk"      /* temporary linker directive file */

char *OptEnvVar = WCLENV;           /* Data interface for GetOpt()        */

static  char    *Word;              /* one parameter                      */
static  char    *SystemName;        /* system to link for                 */
static  char    Files[MAX_CMD];     /* list of filenames from Cmd         */
static  char    CC_Opts[MAX_CMD];   /* list of compiler options from Cmd  */
static  char    target_CC[20] = CC;    /* name of the wcc variant to use     */
static  char    target_CCXX[20] = CCXX; /* name of the wpp variant to use     */
static  char    PathBuffer[_MAX_PATH];/* buffer for path name of tool     */
static  char    *Link_Name;         /* Temp_Link copy if /fd specified    */
static  char    *Temp_Link;         /* temporary linker directive file    */
                                    /* Temp_Link concurrent usages clash  */
static  struct directives *Directive_List; /* linked list of directives   */
static  char    *StackSize;         /* size of stack                      */
static  int     DebugFlag;          /* debug info wanted                  */
static  char    CPU_Class;          /* [0..6]86, 'm'ips or 'a'xp          */
static  char    Conventions[2];     /* 'r' for -3r or 's' for -3s         */
static  char    *O_Name;            /* name of -o option                  */

static  char    preprocess_only;    /* flag: -E option used?              */
static  char    cpp_want_lines;     /* flag: want #lines output?          */
static  char    cpp_keep_comments;  /* flag: keep comments in output?     */
static  char    cpp_encrypt_names;  /* flag: encrypt C++ names?           */
static  char    *cpp_linewrap;      /* line length for cpp output         */



/*
 *  Static function prototypes
 */

static void MakeName( char *, char * );

#undef pick
#define pick(code,english)      english

extern const char *WclMsgs[] = {
#include "wclmsg.h"
};

static const char *EnglishHelp[] = {
#include "owcchelp.gh"
    NULL
};

typedef struct {
    char    *LongName;  /* if ending in ':', copy rest to OW option */
    char    *WatcomName;
} option_mapping;

/* Map of options which don't need special treatment */
option_mapping mappings[] = {
    { "fpmath=287", "fp2" },
    { "fpmath=387", "fp3" },
    { "fptune=586", "fp5" },
    { "fptune=686", "fp6" },
    { "fno-short-enum", "ei" },
    { "fshort-enum", "em" },
    { "fsigned-char", "j" },
    { "fpack-struct=:", "zp" },
    { "ffar-data-threshold=:", "zt" },
    { "frtti", "xr" },
    { "fmessage-full-path", "ef" },
    { "femit-names", "en" },
    { "fbrowser", "db" },
    { "fhook-epilogue", "ee" },
    { "fhook-prologue=:", "ep" },
    { "fhook-prologue", "ep" },
    { "fwrite-def", "v" },
    { "fwrite-def-without-typedefs", "zg" },
    { "fno-stack-check", "s" },
    { "fgrow-stack", "sg" },
    { "fstack-probe", "st" },
    { "fno-writable-strings", "zc" },
    { "fnostdlib", "zl" },
    { "ffunction-sections", "zm" },
    { "fno-strict-aliasing", "oa" },
    { "fguess-branch-probability", "ob" },
    { "fno-optimize-sibling-calls", "oc" },
    { "finline-functions", "oe" },
    { "finline-limit=:", "oe=" },
    { "fno-omit-frame-pointer", "of" },
    { "fno-omit-leaf-frame-pointer", "of+" },
    { "frerun-optimizer", "oh" },
    { "finline-intrinsics-max", "oi+" },
    { "finline-intrinsics", "oi" },
    { "finline-fp-rounding", "zri" },
    { "fomit-fp-rounting", "zro" },
    { "fschedule-prologue", "ok" },
    { "floop-optimize", "ol" },
    { "funroll-loops", "ol+" },
    { "finline-math", "om" },
    { "funsafe-math-optimizations", "on" },
    { "ffloat-store", "op" },
    { "fschedule-insns", "or" },
    { "fkeep-duplicates", "ou" },
    { "fno-eh", "xd" },
    { "feh-direct", "xst" },
    { "feh-table", "xss" },
    { "feh", "xs" },
    { "ftabstob=:", "t=" },
    /* { "mcmodel=:", "m" }, --- handled explicitly */
    { "mabi=cdecl", "ecc" },
    { "mabi=stdcall", "ecd" },
    { "mabi=fastcall", "ecf" },
    { "mabi=pascal", "ecp" },
    { "mabi=fortran", "ecr" },
    { "mabi=syscall", "ecs" },
    { "mabi=watcall", "ecw" },
    { "mwindows", "bg" },
    { "mconsole", "bc" },
    { "mthreads", "bm" },
    { "mrtdll", "br" },
    { "mdefault-windowing", "bw" },
    { "msoft-float", "fpc" },
    { "w", "w0" },
    { "Wlevel:", "w" },
    { "Wall", "w4" },
    { "Wextra", "wx" },
    { "Werror", "we" },
    { "Wn:", "wce=" },
    { "Wno-n:", "wcd=" },
    { "Woverlay", "wo" },
    { "Wpadded", "zpw" },
    { "Wc,-:", "" },
    { "Wstop-after-errors=:", "e" },
    { "ansi", "za" },
    { "std=c99", "za99" },
    { "std=c89", "za" },
    { "std=ow", "ze" },
    { "O0", "od" },
    { "O1", "oil" },
    { "O2", "onatx" },
    { "O3", "onatxl+" },
    { "Os", "os" },
    { "Ot", "ot" },
    { "O", "oil" },
    { "H", "fti" },
    { "fignore-line-directives", "pil" },
    { "fvoid-ptr-arithmetic", "zev" },
    { "shared", "bd" },
};

/* Others to be checked:
    { "-tp=<name>                      (C) set #pragma on <name>",

OW options that might be useful to add:
    -ft / -fx  non-8.3 include search options

*/

typedef enum tool_type {
    TYPE_ASM,
    TYPE_C,
    TYPE_CPP,
    TYPE_LINK,
    TYPE_PACK,
    TYPE_DIS,
    TYPE_MAX
} tool_type;

static struct {
    char *name;
    char *exename;
    char *path;
} tools[TYPE_MAX] = {
    { "wasm",   "wasm" EXE_EXT,     NULL },
    { CC,       CC EXE_EXT,         NULL },
    { CCXX,     CCXX EXE_EXT,       NULL },
    { LINK,     LINK EXE_EXT,       NULL },
    { "cvpack", "cvpack" EXE_EXT,   NULL },
    { DIS,      DIS EXE_EXT,        NULL },
};

void print_banner( void )
{
    static int  done;

    if( done )
        return;
    puts( banner1w( _NAME_ "Compiler Driver Program", _WCL_VERSION_ ) );
    puts( banner2( "1988" ) );
    puts( banner3 );
    puts( banner3a );
    done = 1;
}

static char *xlate_fname( char *name )
{
#ifndef __UNIX__
    /* On non-POSIX hosts, pathnames must be translated to format
     * expected by other tools.
     */
    char    *run = name;

    while( *run ) {
        if( *run == '/' )
            *run = '\\';
        run++;
    }
#endif
    return( name );
}

/* The following 'f' functions are designed to handle filenames */
static char *strfcat( char *target, const char *source )
{
    return( xlate_fname( strcat( target, source ) ) );
}

static char *strfdup( const char *source )
{
    return( xlate_fname( strdup( source ) ) );
}

void addccopt( int option, char *opt )
{
    char    op[4];

    op[0] = ' ';
    op[1] = '-';
    op[2] = option;
    op[3] = '\0';
    strcat( CC_Opts, op );
    if( opt != NULL ) {
        strcat( CC_Opts, opt );
    }
}

static int FileExtension( char *p, char *ext )
{
    char        *dot;

    dot = NULL;
    while( *p != '\0' ) {
        if( *p == '.' )
            dot = p;
        ++p;
    }
    if( dot != NULL ) {
        if( strfcmp( dot, ext ) == 0 ) {
            return( 1 );                /* indicate file extension matches */
        }
    }
    return( 0 );                        /* indicate no match */
}

static  void AddDirective( char *directive )
/******************************************/
{
    struct directives   *p;
    struct directives   *p2;

    p = MemAlloc( sizeof( struct directives ) );
    p->next = NULL;
    p->directive = MemAlloc( strlen( directive ) + 1 );
    strcpy( p->directive, directive );
    if( Directive_List == NULL ) {
        Directive_List = p;
    } else {
        p2 = Directive_List;
        while( p2->next != NULL ) {
            p2 = p2->next;
        }
        p2->next = p;
    }
}

static  int  ConsultSpecsFile( const char *target )
/*************************************************/
{
    FILE    *specs;
    char    line[MAX_CMD];
    char    start_line[MAX_CMD] = "system begin ";
    int     in_target = FALSE;
    char    *p, *blank;

    FindPath( "specs.owc", PathBuffer );
    specs = fopen( PathBuffer, "r" );
    if( specs == NULL ) {
        fprintf( stderr, "Could not open specs file '%s' for reading!\n",
                 PathBuffer );
        exit( EXIT_FAILURE );
    }

    /* search for a block whose first line is "system begin <target>" ... */
    strcat( start_line, target );
    while( fgets( line, MAX_CMD, specs ) ) {
        p = strchr( line, '\n' );
        if( p != NULL ) {
            *p = '\0';
        }
        if( !stricmp( line, start_line ) ) {
            in_target = TRUE;
        } else if( !stricmp( line, "end" ) ) {
            in_target = FALSE;
        } else if( in_target ) {
            for( p = line; isspace( (unsigned char)*p ); p++ )
                ; /* do nothing else */
            if( strncmp ( p, "wcc", 3 ) ) {
                /* wrong format --> don't use this line */
                continue;
            }
            blank = strchr( p, ' ' );
            if( blank == NULL ) {
                blank = strchr( p, '\t' );
            }
            if( blank != NULL ) {
                *blank = '\0';
            }
            strcpy( target_CC, p );

            /* this is a little nasty: transform 'wcc386' into 'wpp386', in-place */
            p[1] = p[2] = 'p';
            strcpy( target_CCXX, p );
            if( blank != NULL ) {
                /* if there are further options, copy them */
                *blank = ' ';
                strcat( CC_Opts, blank );
            }
            fclose( specs );
            return( 1 );
        }
    }
    fclose( specs );
    return( 0 );
}

static  int  Parse( int argc, char **argv )
/*****************************************/
{
    char        *p;
    int         wcc_option;
    int         c;
    int         i;

    Flags.map_wanted   = 0;
    Flags.two_case     = 1;
    Flags.tiny_model   = 0;
    Flags.be_quiet     = 1;
    Flags.no_link      = 0;
    Flags.do_link      = 0;
    Flags.do_cvpack    = 0;
    Flags.link_for_dos = 0;
    Flags.link_for_os2 = 0;
    Flags.windows      = 0;
#if defined( _M_I86 )
    Flags.is32bit      = 0;
#else
    Flags.is32bit      = 1;
#endif
    Flags.math_8087    = 1;
    Flags.keep_exename = 0;
    Flags.want_errfile = 0;
    Flags.strip_all    = 0;
    DebugFlag          = 1;
    StackSize = NULL;
    strcpy( Conventions, "r" );
    preprocess_only    = 0;
    cpp_want_lines     = 1; /* NB: wcc and wcl default to 0 here */
    cpp_keep_comments  = 0;
    cpp_encrypt_names  = 0;
    cpp_linewrap       = NULL;

    AltOptChar = '-'; /* Suppress '/' as option herald */
    while( (c = GetOpt( &argc, argv,
                        "b:Cc::D:Ef:g::"
                        "HI:i::k:L:l:M::m:"
                        "O::o:P::QSs::U:vW::wx:yz::",
                        EnglishHelp )) != -1 ) {

        char    *Word = "";
        int     found_mapping = FALSE;

        for( i = 0; i < sizeof( mappings ) / sizeof( mappings[0] ); i++ ) {
            option_mapping  *m    = mappings + i;
            char            *tail = strchr( m->LongName, ':' );

            if( c != m->LongName[0] )
                continue;
            if( OptArg == NULL ) {
                if( m->LongName[1] == '\0' ) {
                    strcat( CC_Opts, " -" );
                    strcat( CC_Opts, m->WatcomName );
                    found_mapping = TRUE;
                    break;
                }
                /* non-existant argument can't match other cases */
                continue;
            }
            if( tail != NULL ) {
                if( !strncmp( OptArg, m->LongName + 1,
                              tail - m->LongName - 1 ) ) {
                    strcat( CC_Opts, " -" );
                    strcat( CC_Opts, m->WatcomName );
                    strcat( CC_Opts, OptArg + ( tail - m->LongName - 1) );
                    found_mapping = TRUE;
                    break;
                }
            } else if( !strcmp( OptArg, m->LongName + 1 ) ) {
                strcat( CC_Opts, " -" );
                strcat( CC_Opts, m->WatcomName );
                found_mapping = TRUE;
                break;
            }
        }
        if( found_mapping )
            continue;

        if( OptArg != NULL ) {
            Word = malloc( strlen( OptArg ) + 6 );
            strcpy( Word, OptArg );
        }

        wcc_option = 1;

        switch( c ) {
        case 'f':
            if( !strcmp( Word, "syntax-only" ) ) {
                c = 'z';
                strcpy( Word, "s" );
                Flags.no_link = 1;
                break;
            }
            if( !strncmp( Word, "cpp-wrap=", 9 ) ) {
                if( cpp_linewrap != NULL )
                    free( cpp_linewrap );
                Word[7] = 'w';
                cpp_linewrap = strdup( Word + 7 );
                wcc_option = 0;
                break;
            }
            if( !strcmp( Word, "mangle-cpp" ) ) {
                cpp_encrypt_names = 1;
                wcc_option = 0;
                break;
            }
            switch( Word[0] ) {
            case 'd':           /* name of linker directive file */
                Link_Name = "__owcc__.lnk";
                if( Word[1] == '='  ||  Word[1] == '#' ) {
                    MakeName( Word, ".lnk" );    /* add extension */
                    Link_Name = strfdup( Word + 2 );
                }
                wcc_option = 0;
                break;
            case 'm':           /* name of map file */
                Flags.map_wanted = TRUE;
                if( Word[1] == '='  ||  Word[1] == '#' ) {
                    Map_Name = strfdup( Word + 2 );
                }
                wcc_option = 0;
                break;
            case 'o':           /* name of object file */
                /* parse off argument, so we get right filename
                   in linker command file */
                p = &Word[1];
                if( Word[1] == '='  ||  Word[1] == '#' ) {
                    ++p;
                }
                free( Obj_Name );
                Obj_Name = strfdup( p );        /* 08-mar-90 */
                break;
            case 'r':           /* name of error report file */
                Flags.want_errfile = TRUE;
                break;
            }
            /* avoid passing on unknown options */
            wcc_option = 0;
            break;

        case 'k':               /* stack size option */
            if( Word[0] != '\0' ) {
                StackSize = strdup( Word );
            }
            wcc_option = 0;
            break;

        /* compiler options that affect the linker */
        case 'c':           /* compile only */
            Flags.no_link = TRUE;
            wcc_option = 0;
            break;
        case 'x':           /* change source language */
            if( strcmp( Word, "c" ) == 0 ) {
                Flags.force_c = TRUE;
            } else if( strcmp( Word, "c++" ) == 0 ) {
                Flags.force_c_plus = TRUE;
            } else {
                Flags.no_link = TRUE;
            }
            wcc_option = 0;
            break;

        case 'm':
            if( ( !strncmp( "cmodel=", Word, 7 ) )
                && ( Word[8] == '\0' ) ) {
                if( Word[7] == 't' ) {      /* tiny model */
                    Word[0] = 's';              /* change to small */
                    Flags.tiny_model = TRUE;
                } else {
                    Word[0] = Word[7];
                }
                Word[1] = '\0';
                break;
            }
            if( !strncmp("regparm=", Word, 8 ) ) {
                if( !strcmp( Word + 8, "0" ) ) {
                    Conventions[0] =  's';
                } else {
                    Conventions[0] = 'r';
                }
                wcc_option = 0;
                break;
            }
            if( !strncmp("tune=i", Word, 6 ) ) {
                switch( Word[6] ) {
                case '0':
                case '1':
                case '2':
                    CPU_Class = Word[6];
                    Conventions[0] = '\0';
                    break;
                case '3':
                case '4':
                case '5':
                case '6':
                    CPU_Class = Word[6];
                    break;
                default:
                    /* Unknown CPU type --- disable generation of this
                     * option */
                    CPU_Class = '\0';
                }
                wcc_option = 0;
                break;
            }
            wcc_option = 0;     /* dont' pass on unknown options */
            break;

        case 'z':                   /* 12-jan-89 */
            switch( tolower( Word[0] ) ) {
            case 's':
                Flags.no_link = TRUE;
                break;
            case 'q':
                Flags.be_quiet = TRUE;
                break;
            case 'w':
                Flags.windows = TRUE;
            }
            break;
        case 'E':
            preprocess_only = 1;
            wcc_option = 0;
            break;
        case 'P':
            cpp_want_lines = 0;
            wcc_option = 0;
            break;
        case 'C':
            cpp_keep_comments = 1;
            wcc_option = 0;
            break;
        case 'o':
            O_Name = strfdup( OptArg );
            wcc_option = 0;
            break;
        case 'g':
            if( OptArg == NULL ) {
                Word = "2";
            } else if( !isdigit( OptArg[0] ) ) {
                c = 'h';
                if( strcmp( Word, "w" ) == 0 ) {
                    DebugFlag = 3;
                } else if( strcmp( Word, "c" ) == 0 ) { /* 02-mar-91 */
                    Flags.do_cvpack = 1;
                    DebugFlag = 4;
                } else if( strcmp( Word, "d" ) == 0 ) {
                    DebugFlag = 5;
                }
                break;
            }
            c = 'd';
        parse_d:
            if( DebugFlag == 0 ) {  /* not set by -h yet */
                if( strcmp( Word, "1" ) == 0 ) {
                    DebugFlag = 1;
                } else if( strcmp( Word, "1+" ) == 0 ) { /* 02-mar-91 */
                    DebugFlag = 2;
                } else if( strcmp( Word, "2" ) == 0 ) {
                    DebugFlag = 2;
                } else if( strcmp( Word, "2i" ) == 0 ) {
                    DebugFlag = 2;
                } else if( strcmp( Word, "2s" ) == 0 ) {
                    DebugFlag = 2;
                } else if( strcmp( Word, "3" ) == 0 ) {
                    DebugFlag = 2;
                } else if( strcmp( Word, "3i" ) == 0 ) {
                    DebugFlag = 2;
                } else if( strcmp( Word, "3s" ) == 0 ) {
                    DebugFlag = 2;
                }
            }
            break;
        case 'S':
            Flags.do_disas = TRUE;
            Flags.no_link = TRUE;
            if( DebugFlag == 0 ) {
                c = 'd';
                Word = "1";
                goto parse_d;
            }
            wcc_option = 0;
            break;
        case 's':
            if( OptArg != NULL ) {
                /* leave -shared to mapping table */
                wcc_option = 0;
                break;
            }
            Flags.strip_all = 1;
            DebugFlag = 0;
            wcc_option = 0;
            break;
        case 'v':
            Flags.be_quiet = 0;
            wcc_option = 0;
            break;
        case 'W':
            if( OptArg != NULL && strncmp( OptArg, "l,", 2 ) == 0 ) {
                AddDirective( OptArg + 2 );
                wcc_option = 0;
            }
            /* other cases handled by table */
            break;
        case 'I':
            xlate_fname( Word );
            break;
        case 'b':
            Flags.link_for_sys = TRUE;
            SystemName = strdup( Word );
            /* if Word found in specs.owc, add options from there: */
            if( ConsultSpecsFile( Word ) ) {
                /* all set */
                wcc_option = 0;
            } else {
                /* not found --- default to bt=<system> */
                strcpy( Word, "t=" );
                strcat( Word, SystemName );
            }
            break;
        case 'l':
            strcat( Libs, Libs[0] != '\0' ? ",lib" : " lib" );
            strcat( Libs, OptArg );
            strcat( Libs, ".a" );
            wcc_option = 0;
            break;
        case 'L':
            xlate_fname( Word );
            fputs( "libpath ", Fp );
            Fputnl( Word, Fp );
            wcc_option = 0;
            break;
        case 'i':       /* -include <file> --> -fi=<file> */
            if( OptArg == NULL ) {
                wcc_option = 0;
                break;
            }
            if( !strcmp( OptArg, "nclude" ) ) {
                c = 'f';
                Word = realloc( Word, strlen( argv[OptInd] ) + 6 );
                if( OptInd >= argc - 1 ) {
                    PrintMsg( "Argument of -include missing\n", OptArg );
                    return( 1 );
                }
                strcpy( Word, "i=" );
                strfcat( Word, argv[OptInd] );
                argv[OptInd++][0] = '\0';
                break;
            }
            /* avoid passing un unknown options */
            wcc_option = 0;
            break;

        case 'M':               /* autodepend information for Unix makes */
            if( OptArg == NULL ) {
                wcc_option = 0;
                break;
            }
            c = 'a';
            if( !strcmp( OptArg, "D" ) ||
                !strcmp( OptArg, "MD" ) ) {
                /* NB: only -MMD really matches OW's behaviour, but
                 * for now, let's accept -MD to mean the same */
                /* translate to -adt=.o */
                strcpy( Word, "dt=.o" );
            } else if( !strcmp( OptArg, "F" ) ) {
                Word = realloc( Word, strlen( argv[OptInd] ) + 6 );
                if( OptInd >= argc - 1 ) {
                    PrintMsg( "Argument of -MF missing\n", OptArg );
                    return( 1 );
                }
                strcpy( Word, "d=" );
                strfcat( Word, argv[OptInd] );
                argv[OptInd++][0] = '\0';
            } else if( !strcmp( OptArg, "T") ) {
                Word = realloc( Word, strlen( argv[OptInd] ) + 6 );
                if( OptInd >= argc - 1 ) {
                    PrintMsg( "Argument of -M%s missing\n", OptArg );
                    return( 1 );
                }
                strcpy( Word, "dt=" );
                strcat( Word, argv[OptInd] );
                argv[OptInd++][0] = '\0';
            } else {
                /* avoid passing on incompatible options */
                wcc_option = 0;
            }
            break;
        }

        /* don't add linker-specific options */
        /* to compiler command line:     */
        if( wcc_option ) {
            addccopt( c, Word );
        }
        if( OptArg != NULL ) {
            free( Word );
            Word = NULL;
        }
    }

    if( preprocess_only ) {
        Flags.no_link = TRUE;
        if( O_Name == NULL ) {
            free( Obj_Name );           /* preprocess to stdout by default */
            Obj_Name = NULL;
        }
        strcat( CC_Opts, " -p" );
        if( cpp_encrypt_names )
            strcat( CC_Opts, "e" );
        if( cpp_want_lines )
            strcat( CC_Opts, "l" );
        if( cpp_keep_comments )
            strcat( CC_Opts, "c" );
        if( cpp_linewrap != NULL ) {
            strcat( CC_Opts, cpp_linewrap );
        }
    }
    if( CPU_Class )
        addccopt( CPU_Class, Conventions );
    if( Flags.be_quiet )
        addccopt( 'z', "q" );
    if( O_Name != NULL ) {
        if( Flags.no_link && !Flags.do_disas ) {
            free( Obj_Name );
            Obj_Name = O_Name;
        } else {
            strcpy( Exe_Name, O_Name );
            Flags.keep_exename = 1;
        }
        O_Name = NULL;
    }
    if( Obj_Name != NULL ) {
        strcat( CC_Opts, " -fo=" );
        strcat( CC_Opts, Obj_Name );
    }
    if( !Flags.want_errfile ) {
        strcat( CC_Opts, " -fr" );
    }
    for( i = 1; i < argc ; i++ ) {
        Word = argv[i];
        if( Word == NULL || Word[0] == '\0' )
            /* HBB 20060217: argument was used up */
            continue;
        if( FileExtension( Word, ".lib" ) || FileExtension( Word, ".a" ) ) {
            strcat( Libs, Libs[0] != '\0' ? "," : " " );
            strfcat( Libs, Word );
        } else {
            strfcat( Files, Word );
            strcat( Files, " " );
        }
    }
    return( 0 );
}

static int useCPlusPlus( char *p )
/********************************/
{
    return(
        strfcmp( p, ".cp" ) == 0 ||
        strfcmp( p, ".cpp" ) == 0 ||
#ifdef __UNIX__
        strcmp( p, ".c++" ) == 0 ||
        strcmp( p, ".C" ) == 0 ||
#endif
        strfcmp( p, ".cxx" ) == 0 ||
        strfcmp( p, ".cc" )  == 0 ||
        strfcmp( p, ".hpp" ) == 0 ||
        strfcmp( p, ".hxx" ) == 0 );
}


static char *FindToolPath( tool_type utl )
/****************************************/
{
    if( tools[utl].path == NULL ) {
        FindPath( tools[utl].exename, PathBuffer );
        tools[utl].path = MemAlloc( strlen( PathBuffer ) + 1 );
        strcpy( tools[utl].path, PathBuffer );
    }
    return( tools[utl].path );
}

static int tool_exec( tool_type utl, char *p1, char *p2 )
/*******************************************************/
{
    int     rc;

    FindToolPath( utl );
    if( !Flags.be_quiet ) {
        if( utl == TYPE_DIS ) {
            PrintMsg( "\t%s -s -a %s %s\n", tools[utl].name, p1, p2 );
        } else if( p2 == NULL ) {
            PrintMsg( "\t%s %s\n", tools[utl].name, p1 );
        } else {
            PrintMsg( "\t%s %s %s\n", tools[utl].name, p1, p2 );
        }
    }
    fflush( NULL );
    if( utl == TYPE_DIS ) {
        rc = spawnlp( P_WAIT, tools[utl].path, tools[utl].name, "-s", "-a", p1, p2, NULL );
    } else if( p2 == NULL ) {
        rc = spawnlp( P_WAIT, tools[utl].path, tools[utl].name, p1, NULL );
    } else {
        rc = spawnlp( P_WAIT, tools[utl].path, tools[utl].name, p1, p2, NULL );
    }
    if( rc != 0 ) {
        if( (rc == -1) || (rc == 255) ) {
            PrintMsg( WclMsgs[UNABLE_TO_INVOKE_EXE], tools[utl].path );
        } else {
            if( utl == TYPE_LINK ) {
                PrintMsg( WclMsgs[LINKER_RETURNED_A_BAD_STATUS] );
            } else if( utl == TYPE_PACK ) {
                PrintMsg( WclMsgs[CVPACK_RETURNED_A_BAD_STATUS] );
            } else {
                PrintMsg( WclMsgs[COMPILER_RETURNED_A_BAD_STATUS], p1 );
            }
        }
    }
    return( rc );
}

static tool_type SrcName( char *name )
/************************************/
{
    char        *p;
    tool_type   utl;

    p = strrchr( name, '.' );
    if( p == NULL || strpbrk( p, PATH_SEP_STR ) )
        p = name + strlen( name );
    if( strfcmp( p, ".asm" ) == 0 || stricmp( p, ".s" ) == 0 ) {
        utl = TYPE_ASM;
    } else {
        utl = TYPE_C;               // assume C compiler
        if( !Flags.force_c ) {
            if( Flags.force_c_plus || useCPlusPlus( p ) ) {
                utl = TYPE_CPP;     // use C++ compiler
            }
        }
    }
    return( utl );
}

static  int  CompLink( void )
/***************************/
{
    int         rc;
    char        *p;
    char        *end;
    char        *file;
    char        *path;
    struct directives *d_list;
    char        errors_found;
    tool_type   utl;
    int         i;

    if( Flags.be_quiet ) {
        Fputnl( "option quiet", Fp );
    }

    fputs( DebugOptions[DebugFlag], Fp );
    if( StackSize != NULL ) {
        fputs( "option stack=", Fp );
        Fputnl( StackSize, Fp );
    }
    if( Flags.link_for_sys ) {
        fputs( "system ", Fp );
        Fputnl( SystemName, Fp );
    } else if( Flags.is32bit ) {
  #if defined(__OS2__)
        Fputnl( "system os2v2", Fp );
  #elif defined(__NT__)
        Fputnl( "system nt", Fp );
  #elif defined(__LINUX__)
        Fputnl( "system linux", Fp );
        if( !Flags.strip_all )
            Fputnl( "option exportall", Fp );
  #else
        Fputnl( "system dos4g", Fp );
  #endif
    } else {
        if( Flags.windows ) {                   /* 15-mar-90 */
            Fputnl( "system windows", Fp );
        } else if( Flags.tiny_model ) {
            Fputnl( "system com", Fp );
        } else {
#if defined(__OS2__)
            Fputnl( "system os2", Fp );
#else
            Fputnl( "system dos", Fp );
#endif
        }
    }
    for( d_list = Directive_List; d_list; d_list = d_list->next ) {
        Fputnl( d_list->directive, Fp );
    }
    Word = MemAlloc( MAX_CMD );
    errors_found = 0;                   /* 21-jan-92 */
    p = Files;
    while( *p != '\0' ) {
        if( *p == '"' ) {
            end = strpbrk( ++p, "\"" ); /* get quoted filespec */
        } else {
            end = strpbrk( p, " " );    /* get filespec */
        }
        if( end != NULL ) {
            *(end++) = 0;
            if( *end == ' ' ) {
                end++;
            }
        }
        strcpy( Word, p );
        utl = SrcName( Word );      /* if no extension, assume .c */
        file = GetName( Word );         /* get first matching filename */
        path = MakePath( Word );        /* isolate path portion of filespec */
        while( file != NULL ) {         /* while more filenames: */
            strcpy( Word, path );
            strcat( Word, file );
            if( !FileExtension( Word, OBJ_EXT ) &&  // if not .obj or .o, compile
                !FileExtension( Word, OBJ_EXT_SECONDARY ) ) {
                rc = tool_exec( utl, Word, CC_Opts );
                if( rc != 0 ) {
                    errors_found = 1;
                }
                p = strrchr( file, '.' );
                if( p != NULL )  {
                    *p = '\0';
                }
                strcpy( Word, file );
            }
            AddName( Word, Fp );
            if( Obj_List != NULL && Flags.do_disas ) {
                char    *sfile;
                char    *ofile;

                ofile = malloc( strlen( file ) + 6 );
                strcpy( ofile, file );

                if( Exe_Name[0] != '\0' ) {     /* have "-S -o output.name" */
                    sfile = Exe_Name;
                } else {
                    if( FileExtension( Word, OBJ_EXT ) ||
                        FileExtension( Word, OBJ_EXT_SECONDARY ) ) {
                        p = strrchr( file, '.' );
                        if( p != NULL )  {
                            *p = '\0';
                        }
                        strcpy( Word, file );
                    } else {            /* wdis needs extension */
                        strcat( ofile, Obj_Name );
                    }
                    sfile = Word;
                    strcat( Word, ".s" );
                }
                memmove( sfile + 3, sfile, strlen( sfile ) + 1 );
                sfile[0] = '-';
                sfile[1] = 'l';
                sfile[2] = '=';
                rc = tool_exec( TYPE_DIS, ofile, sfile );
                free( ofile );
            }
            if( Exe_Name[0] == '\0' ) {
#ifdef __UNIX__
                strcpy( Exe_Name, OUTPUTFILE );
                Flags.keep_exename = 1;
#else
                p = strrchr( Word, '.' );
                if( p != NULL ) {
                    *p = '\0';
                }
                strcpy( Exe_Name, Word );
#endif
            }
            file = GetName( NULL );     /* get next filename */
        }
        p = end;        /* get next filespec */
    }
    if( errors_found ) {
        rc = 1;
    } else {
        rc = 0;
        BuildLinkFile();
        if(( Obj_List != NULL || Flags.do_link ) && Flags.no_link == FALSE ) {
            rc = tool_exec( TYPE_LINK, Temp_Link, NULL );
            if( rc == 0 && Flags.do_cvpack ) {
                rc = tool_exec( TYPE_PACK, Exe_Name, NULL );
            }
            if( rc != 0 ) {
                rc = 2;     /* return 2 to show Temp_File already closed */
            }
        }
    }
    if( Word != NULL ) {
        free( Word );
        Word = NULL;
    }
    for( i = 0; i < TYPE_MAX; ++i ) {
        if( tools[i].path != NULL ) {
            free( tools[i].path );
            tools[i].path = NULL;
        }
    }
    return( rc );
}


static  void  MakeName( char *name, char *ext )
/*********************************************/
{
    /* If the last '.' is before the last path seperator character */
    if( strrchr( name, '.' ) <= strpbrk( name, PATH_SEP_STR ) ) {
        strcat( name, ext );
    }
}

static void ExitHandler( void )
/*****************************/
{
    if( Fp != NULL )
        fclose( Fp );
    remove( Temp_Link + 1 );
}

int main( int argc, char **argv )
/*******************************/
{
    int     rc;

    Temp_Link = TEMPFILE;

    if( argc <= 1 ) {
        /* no arguments: just tell the user who I am */
        puts( "Usage: owcc [-?] [options] file ..." );
        exit( EXIT_SUCCESS );
    }

    errno = 0; /* Standard C does not require fopen failure to set errno */
    if( (Fp = fopen( Temp_Link + 1, "w" )) == NULL ) {
        /* Message before banner decision as '@' option uses Fp in Parse() */
        PrintMsg( WclMsgs[UNABLE_TO_OPEN_TEMPORARY_FILE], Temp_Link + 1,
            strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    Map_Name = NULL;
    Obj_Name = strdup( ".o" );
    Directive_List = NULL;

    /* destruct the temp. linker script in case of -? or parse errors */
    atexit( ExitHandler );
    rc = Parse( argc, argv );
    if( rc == 0 ) {
        if( !Flags.be_quiet ) {
            print_banner();
        }
        rc = CompLink();
    }
    if( rc == 1 ) {
        fclose( Fp );
    }
    if( Link_Name != NULL ) {
        if( strfcmp( Link_Name, Temp_Link + 1) != 0 ) {
            remove( Link_Name );
            rename( Temp_Link + 1, Link_Name );
        }
    } else {
        remove( Temp_Link + 1 );
    }
    return( rc == 0 ? 0 : 1 );
}
