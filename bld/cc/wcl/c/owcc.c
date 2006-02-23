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
#define CC          "wcc386"          /* Open Watcom C compiler          */
#define CCXX        "wpp386"          /* Open Watcom C++ compiler        */
#define WCLENV      "OWCC"
#define _NAME_      "C/C++32 "

#ifdef __UNIX__
#define PATH_SEP_STR "/"
#else
#define PATH_SEP_STR "/\\"
#endif
#define OUTPUTFILE  "a.out"
#define LINK        "wlink"             /* Open Watcom linker              */
#define TEMPFILE    "@__WCL__.LNK"      /* temporary linker directive file */

char *OptEnvVar = WCLENV;           /* Data interface for GetOpt()        */

static  char    *Cmd;               /* command line parameters            */
static  char    *Word;              /* one parameter                      */
static  char    *SystemName;        /* system to link for                 */
static  char    Files[MAX_CMD];     /* list of filenames from Cmd         */
static  char    CC_Opts[MAX_CMD];   /* list of compiler options from Cmd  */
static  char    target_CC[20] = CC;    /* name of the wcc variant to use     */
static  char    target_CCXX[20] = CCXX; /* name of the wpp variant to use     */
static  char    CC_Path[_MAX_PATH]; /* path name for wcc.exe              */
static  char    PathBuffer[_MAX_PATH];/* buffer for path name of tool     */
static  char    *Link_Name;         /* Temp_Link copy if /fd specified    */
static  char    *Temp_Link;         /* temporary linker directive file    */
                                    /* Temp_Link concurrent usages clash  */
static  struct directives *Directive_List; /* linked list of directives   */
static  char    *StackSize;         /* size of stack                      */
static  char    DebugFlag;          /* debug info wanted                  */
static  char    Conventions;        /* 'r' for -3r or 's' for -3s         */
static  char    *O_Name;            /* name of -o option                  */


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

void print_banner( void )
{
    static int  done;

    if( done ) return;
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
     * expected buy other tools.
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
    if( opt )
        strcat( CC_Opts, opt );
}

static int FileExtension( char *p, char *ext )
{
    char        *dot;

    dot = NULL;
    while( *p != '\0' ) {
        if( *p == '.' )  dot = p;
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

static  int  ConsultSpecsFile( const char *system )
/*************************************************/
{
    FILE    *specs = fopen( "specs.owc", "r" );
    char    line[MAX_CMD];
    char    start_line[MAX_CMD] = "system begin ";
    int     in_system = FALSE;
    char    *p, *blank;

    if( !specs ) {
        fputs( "Could not find specs file\n", stderr );
    }
    strcat( start_line, system );

    while( fgets( line, MAX_CMD, specs ) ) {
        p = strchr( line, '\n' );
        if( p ) {
            *p = '\0';
        }
        if( ! stricmp( line, start_line ) ) {
            in_system = TRUE;
        } else if( !stricmp( line, "end" ) ) {
            in_system = FALSE;
        } else if( in_system ) {
            for( p = line; isspace( *p ); p++ )
                ; /* do nothing else */
            if( strnicmp ( p, "wcc", 3 ) ) {
                /* wrong format --> don't use this line */
                continue;
            }
            blank = strchr( p, ' ' );
            if( ! blank ) {
                blank = strchr( p, '\t' );
            }
            if( blank ) {
                *blank = '\0';
            }
            strcpy( target_CC, p );
            /* nasty: transform 'wcc386' into 'wpp386' */
            *p = tolower( *p );
            p[1] = p[2] = 'p';
            strcpy( target_CCXX, p );
            if( blank ) {
                /* if there are options, copy them */
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
    FILE        *atfp;
    char        buffer[_MAX_PATH];
    char        *p;
    int         wcc_option;
    int         c;
    int         i;

    Flags.map_wanted   = 0;
    Flags.two_case     = 0;
    Flags.tiny_model   = 0;
    Flags.be_quiet     = 1;
    Flags.no_link      = 0;
    Flags.do_link      = 0;
    Flags.do_cvpack    = 0;
    Flags.link_for_dos = 0;
    Flags.link_for_os2 = 0;
    Flags.windows      = 0;
    Flags.is32bit      = 1;
    Flags.math_8087    = 1;
    Flags.keep_exename = 0;
    Flags.want_errfile = 0;
    Flags.strip_all    = 0;
    DebugFlag          = 1;
    StackSize = NULL;
    Conventions = 'r';

    AltOptChar = '-'; /* Suppress '/' as option herald */
    while( (c = GetOpt( &argc, argv,
                        "0123::456a::b:c::D:d:Ee:f:Gg::h:I:i:jk:L:l:M:m:N:n:"
                        "O::o:P::p::Qr::Sst:U:vW:w:Xx::yz:",
                        EnglishHelp )) != -1 ) {

        char    *Word = "";

        if( OptArg ) {
            Word = malloc( strlen( OptArg ) + 6 );
            strcpy( Word, OptArg );
        }

        wcc_option = 1;

        switch( c ) {
        case 'f':               /* files option */
            switch( Word[0] ) {
            case 'd':           /* name of linker directive file */
                Link_Name = "__WCL__.LNK";
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
            case 'p':           /* floating-point option */
                /* FIXME: should be mapped to -mfp... */
                if( Word[1] == 'c' ) {
                    Flags.math_8087 = 0;
                }
                break;
            default:
                break;
            }
            break;
        case 'k':               /* stack size option */
            if( Word[0] != '\0' ) {
                StackSize = strdup( Word );
            }
            wcc_option = 0;
            break;
        case 'x':
            if( Word[0] == '\0' ) {
                Flags.two_case = TRUE;
                wcc_option = 0;
            }
            break;
        case '@':
            if( Word[0] != '\0' ) {
                MakeName( Word, ".lnk" );
                errno = 0;
                if( ( atfp = fopen( Word, "r" ) ) == NULL ) {
                    PrintMsg( WclMsgs[UNABLE_TO_OPEN_DIRECTIVE_FILE], Word, strerror( errno ) );
                    return( 1 );
                }
                while( fgets( buffer, sizeof( buffer ), atfp ) != NULL ) {
                    if( strnicmp( buffer, "file ", 5 ) == 0 ) {

                        /* look for names separated by ','s */
                        p = strchr( buffer, '\n' );
                        if( p ) {
                            *p = NULLCHAR;
                        }
                        AddName( &buffer[5], Fp );
                        Flags.do_link = TRUE;
                    } else {
                        fputs( buffer, Fp );
                    }
                }
                fclose( atfp );
            }
            wcc_option = 0;
            break;

            /* compiler options that affect the linker */
        case '3':
        case '4':
        case '5':                           /* 22-sep-92 */
            Conventions = tolower( Word[0] );
            break;
        case 'd':
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
        case 'h':
        parse_h:
            if( strcmp( Word, "w" ) == 0 ) {
                DebugFlag = 3;
            } else if( strcmp( Word, "c" ) == 0 ) { /* 02-mar-91 */
                Flags.do_cvpack = 1;
                DebugFlag = 4;
            } else if( strcmp( Word, "d" ) == 0 ) {
                DebugFlag = 5;
            }
            break;
        case 'c':           /* compile only */
            if( strcmp( Word, "c" ) == 0 ) {
                Flags.force_c = TRUE;
            } else if( strcmp( Word, "c++" ) == 0 ) {
                Flags.force_c_plus = TRUE;
            } else {
                Flags.no_link = TRUE;
            }
            wcc_option = 0;
            break;
        case 'm':           /* memory model */
            if( tolower( Cmd[1] ) == 't' ) {    /* tiny model*/
                Word[0] = 's';                  /* change to small */
                Flags.tiny_model = TRUE;
            }
            break;
        case 'p':
            Flags.no_link = TRUE;
            break;      /* this is a preprocessor option */
        case 'z':                   /* 12-jan-89 */
            switch( tolower( Cmd[1] ) ) {
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
            Flags.no_link = TRUE;
            free( Obj_Name );           /* preprocess to stdout by default */
            Obj_Name = NULL;
            c = 'p';
            Word = "l";
            break;
        case 'O':
            if( !OptArg )
                Word = "il";
            else if( isdigit( OptArg[0] ) ) {
                int     d = OptArg[0] - '0';

                if( d == 0 )
                    OptArg = "d";
                else if( d == 1 )
                    OptArg = "il";
                else if( d == 2 )
                    OptArg = "natx";
                else if( d == 3 )
                    OptArg = "natxl+";
                else
                    break;
                strcpy( Word, OptArg );
            } else
                break;
            break;
        case 'o':
            O_Name = strfdup( OptArg );
            wcc_option = 0;
            break;
        case 'g':
            if( !OptArg )
                Word = "2";
            else if( !isdigit( OptArg[0] ) ) {
                c = 'h';
                goto parse_h;
            }
            c = 'd';
            goto parse_d;
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
            Flags.strip_all = 1;
            DebugFlag = 0;
            wcc_option = 0;
            break;
        case 'v':
            Flags.be_quiet = 0;
            wcc_option = 0;
            break;
        case 'W':
            if( OptArg ) {
                if( strcmp( OptArg, "all" ) == 0 ) {
                    c = 'w';
                    strcpy( Word, "x" );
                }
                else if( strncmp( OptArg, "l,", 2 ) == 0 ) {
                    AddDirective( OptArg + 2 );
                    wcc_option = 0;
                }
                else if( strncmp( OptArg, "c,", 2 ) == 0 &&
                         strlen( OptArg ) >= 4 ) {
                    c = OptArg[3];
                    strcpy( Word, OptArg + 4 );
                }
            }
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
        case 'M':               /* autodepend for Unix makes */
            if( !strcmp( OptArg, "D" ) ||
                !strcmp( OptArg, "MD" ) ) {
                /* translate to -adt=.o */
                /* NB: only -MMD really matches OW's behaviour, but
                 * accept -MD to mean the same */
                c = 'a';
                strcpy( Word, "dt=.o" );
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
        if( OptArg )
            free( Word );
    }
    if( Flags.be_quiet )
        addccopt( 'z', "q" );
    if( O_Name ) {
        if( Flags.no_link && !Flags.do_disas ) {
            free( Obj_Name );
            Obj_Name = O_Name;
        } else {
            strcpy( Exe_Name, O_Name );
            Flags.keep_exename = 1;
        }
        O_Name = NULL;
    }
    if ( Obj_Name ) {
        strcat( CC_Opts, " -fo=" );
        strcat( CC_Opts, Obj_Name );
    }
    if ( ! Flags.want_errfile ) {
        strcat( CC_Opts, " -fr" );
    }
    for( i = 1; i < argc ; i++ ) {
        Word = argv[i];
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


static char *SrcName( char *name )
/********************************/
{
    char        *cc_name;
    char        exename[20];  /* enough to hold even "wasm_mips.exe" */
    char        *p;

    p = strrchr( name, '.' );
    if( p == NULL || strpbrk( p, PATH_SEP_STR ) )
        p = name + strlen( name );
    if( strfcmp( p, ".asm" ) == 0 || stricmp( p, ".s" ) == 0 ) {
        cc_name = "wasm";
    } else {
        cc_name = target_CC;          /* assume C compiler */
        if( !Flags.force_c ) {
            if( Flags.force_c_plus || useCPlusPlus( p ) ) {
                cc_name = target_CCXX;/* use C++ compiler */
            }
        }
        strcpy( exename, cc_name );
    }
    strcat( exename, EXE_EXT );
    FindPath( exename, CC_Path );
    return( cc_name );
}

static  int  CompLink( void )
/***************************/
{
    int         rc;
    char        *p;
    char        *end;
    char        *file;
    char        *path;
    char        *cc_name;
    struct directives *d_list;
    char        errors_found;

    if( Flags.be_quiet ) {
        Fputnl( "option quiet", Fp );
    }

    fputs( DebugOptions[ DebugFlag ], Fp );
    Fputnl( "option dosseg", Fp );
    if( StackSize != NULL ) {
        fputs( "option stack=", Fp );
        Fputnl( StackSize, Fp );
    }
    if( Flags.link_for_sys ) {                  /* 10-jun-91 */
        fputs( "system ", Fp );
        Fputnl( SystemName, Fp );
    } else if( Flags.is32bit ) {
  #if defined(__OS2__)
        Fputnl( "system os2v2", Fp );           /* 04-feb-92 */
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
            if( *end == ' ' ) end++;
        }
        strcpy( Word, p );
        cc_name = SrcName( Word );      /* if no extension, assume .c */

        file = GetName( Word );         /* get first matching filename */
        path = MakePath( Word );        /* isolate path portion of filespec */
        while( file != NULL ) {         /* while more filenames: */
            strcpy( Word, path );
            strcat( Word, file );
            if( !FileExtension( Word, OBJ_EXT ) &&  // if not .obj or .o, compile
                !FileExtension( Word, OBJ_EXT_SECONDARY ) ) {
                if( !Flags.be_quiet ) {
                    PrintMsg( "       %s %s %s\n", cc_name, Word, CC_Opts );
                    fflush( stdout );
                }
                rc = spawnlp( P_WAIT, CC_Path, cc_name, Word, CC_Opts, NULL );
                if( rc != 0 ) {
                    if( rc == -1  ||  rc == 255 ) {
                        PrintMsg( WclMsgs[ UNABLE_TO_INVOKE_EXE ], CC_Path );
                    } else {
                        PrintMsg( WclMsgs[ COMPILER_RETURNED_A_BAD_STATUS ],
                                    Word );
                    }
                    errors_found = 1;           /* 21-jan-92 */
                }
                p = strrchr( file, '.' );
                if( p != NULL )  {
                    *p = NULLCHAR;
                }
                strcpy( Word, file );
            }
            AddName( Word, Fp );
            if( Obj_List != NULL && Flags.do_disas ) {
                char    *sfile;
                char    *ofile;

                ofile = malloc( strlen( file ) + 6 );
                strcpy( ofile, file );

                if( Exe_Name[0] )       /* have "-S -o output.name" */
                    sfile = Exe_Name;
                else {
                    if( FileExtension( Word, OBJ_EXT ) ||
                        FileExtension( Word, OBJ_EXT_SECONDARY) ) {
                        p = strrchr( file, '.' );
                        if( p != NULL )  {
                            *p = NULLCHAR;
                        }
                        strcpy( Word, file );
                    } else {            /* wdis needs extension */
                        strcat(ofile, Obj_Name);
                    }
                    sfile = Word;
                    strcat( Word, ".s" );
                }
                memmove( sfile + 3, sfile, strlen( sfile ) + 1 );
                sfile[0] = '-';
                sfile[1] = 'l';
                sfile[2] = '=';
                if( !Flags.be_quiet ) {
                    PrintMsg( "       %s -s -a %s %s\n", DIS, ofile, sfile );
                    fflush( stdout );
                }
                rc = spawnlp( P_WAIT, DIS, "-s", "-a", ofile, sfile, NULL );
                if( rc != 0 ) {
                    if( rc == -1  ||  rc == 255 ) {
                        PrintMsg( WclMsgs[ UNABLE_TO_INVOKE_EXE ], CC_Path );
                    } else {
                        PrintMsg( WclMsgs[ COMPILER_RETURNED_A_BAD_STATUS ],
                                    Word );
                    }
                }
                free( ofile );
            }
            if( Exe_Name[0] == NULLCHAR ) {
#ifdef __UNIX__
                strcpy( Exe_Name, OUTPUTFILE );
                Flags.keep_exename = 1;
#else
                p = strrchr( Word, '.' );
                if( p != NULL ) {
                    *p = NULLCHAR;
                }
                strcpy( Exe_Name, Word );
#endif
            }
            file = GetName( NULL );     /* get next filename */
        }
        p = end;        /* get next filespec */
    }
    if( errors_found ) {
        puts( "" );
        return( 1 );            /* 21-jan-92 */
    }
    BuildLinkFile();

    if( ( Obj_List != NULL || Flags.do_link )  &&  Flags.no_link == FALSE ) {
        FindPath( "wlink" EXE_EXT, PathBuffer );
        if( !Flags.be_quiet ) {
            puts( "" );
        }
        fflush( stdout );
        rc = spawnlp( P_WAIT, PathBuffer, LINK, Temp_Link, NULL );
        if( rc != 0 ) {
            if( rc == -1  ||  rc == 255 ) {
                PrintMsg( WclMsgs[ UNABLE_TO_INVOKE_EXE ], PathBuffer );
            } else {
                PrintMsg( WclMsgs[ LINKER_RETURNED_A_BAD_STATUS ] );
            }
            puts( "" );
            return( 2 );        /* return 2 to show Temp_File already closed */
        }
        if( Flags.do_cvpack ) {
            FindPath( "cvpack" EXE_EXT, PathBuffer );
            rc = spawnlp( P_WAIT, PathBuffer, "cvpack", Exe_Name, NULL );
            if( rc != 0 ) {
                if( rc == -1  ||  rc == 255 ) {
                    PrintMsg( WclMsgs[ UNABLE_TO_INVOKE_EXE ], PathBuffer );
                } else {
                    PrintMsg( WclMsgs[ CVPACK_RETURNED_A_BAD_STATUS ] );
                }
                return( 2 );  /* return 2 to show Temp_File already closed */
            }
        }
    }
    return( 0 );
}


static  void  MakeName( char *name, char *ext )
/*********************************************/
{
    /* If the last '.' is before the last path seperator character */
    if( strrchr( name, '.' ) <= strpbrk( name, PATH_SEP_STR ) ) {
        strcat( name, ext );
    }
}

int   main( int argc, char **argv )
{
    int     rc;

    Temp_Link = TEMPFILE;

    if( argc <= 1 ) {
        /* no arguments: just tell the user who I am */
        puts( "Usage: owcc [-?] [options] file ..." );
        return( EXIT_SUCCESS );
    }

    errno = 0; /* Standard C does not require fopen failure to set errno */
    if( ( Fp = fopen( &Temp_Link[ 1 ], "w" ) ) == NULL ) {
        /* Message before banner decision as '@' option uses Fp in Parse() */
        PrintMsg( WclMsgs[ UNABLE_TO_OPEN_TEMPORARY_FILE ], Temp_Link + 1,
            strerror( errno ) );
        exit( 1 );
    }
    Map_Name = NULL;
    Obj_Name = strdup( ".o" );
    Directive_List = NULL;

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
        if( strfcmp( Link_Name, &Temp_Link[ 1 ] ) != 0 ) {
            remove( Link_Name );
            rename( &Temp_Link[ 1 ], Link_Name );
        }
    } else {
        remove( &Temp_Link[ 1 ] );
    }
    return( rc == 0 ? 0 : 1 );
}
