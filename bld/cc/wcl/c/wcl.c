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
* Description:  C/C++ Compile and Link utility.
*
****************************************************************************/


#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#ifndef __UNIX__
#include <direct.h>
#else
#include <dirent.h>
#endif
#include <process.h>
#include <malloc.h>
#include <conio.h>

#include "clcommon.h"
#include "banner.h"
#undef  _BANEXTRA
#define _BANEXTRA _BANEXSHORT

#ifdef WCLAXP
  #define WCLNAME     "wclaxp"          /* Name of Compile and Link Utility*/
  #define CC          "wccaxp"          /* Open Watcom C compiler          */
  #define CCXX        "wppaxp"          /* Open Watcom C++ compiler        */
  #define WCLENV      "WCLAXP"          /* name of environment variable    */
  #define STACKSIZE   "8192"            /* default stack size              */
  #define _NAME_      "C/C++ Alpha AXP "
#elif defined(WCLPPC)
  #define WCLNAME     "wclppc"          /* Name of Compile and Link Utility*/
  #define CC          "wccppc"          /* Open Watcom C compiler          */
  #define CCXX        "wppppc"          /* Open Watcom C++ compiler        */
  #define WCLENV      "WCLPPC"          /* name of environment variable    */
  #define STACKSIZE   "8192"            /* default stack size              */
  #define _NAME_      "C/C++ PowerPC "
#elif defined(WCL386)
  #define WCLNAME     "wcl386"          /* Name of Compile and Link Utility*/
  #define CC          "wcc386"          /* Open Watcom C compiler          */
  #define CCXX        "wpp386"          /* Open Watcom C++ compiler        */
  #define WCLENV      "WCL386"          /* name of environment variable    */
  #define STACKSIZE   "8192"            /* default stack size              */
  #define _NAME_      "C/C++32 "
#else
  #define WCLNAME     "wcl"             /* Name of Compile and Link Utility*/
  #define CC          "wcc"             /* Open Watcom C compiler          */
  #define CCXX        "wpp"             /* Open Watcom C++ compiler        */
  #define WCLENV      "WCL"             /* name of environment variable    */
  #define STACKSIZE   "4096"            /* default stack size              */
  #define _NAME_      "C/C++16 "
#endif
#ifdef __UNIX__
#define PATH_SEP_STR "/"
#else
#define PATH_SEP_STR "\\"
#endif
#define LINK        "wlink"             /* Open Watcom linker              */
#define TEMPFILE    "@__WCL__.LNK"      /* temporary linker directive file */
static  char    *Cmd;               /* command line parameters            */
static  char    *Word;              /* one parameter                      */
static  char    *SystemName;        /* system to link for                 */
static  char    Files[MAX_CMD];     /* list of filenames from Cmd         */
        char    Libs[MAX_CMD];      /* list of libraires from Cmd         */
static  char    CC_Opts[MAX_CMD];   /* list of compiler options from Cmd  */
static  char    CC_Path[_MAX_PATH]; /* path name for wcc.exe              */
static  char    PathBuffer[_MAX_PATH];/* buffer for path name of tool     */
        FILE    *Fp;                /* file pointer for Temp_Link         */
static  char    *Link_Name;         /* Temp_Link copy if /fd specified    */
static  char    *Temp_Link;         /* temporary linker directive file    */
                                    /* Temp_Link concurrent usages clash  */
        struct  list *Obj_List;     /* linked list of object filenames    */
static  struct directives *Directive_List; /* linked list of directives   */
        char    Exe_Name[_MAX_PATH];/* name of executable                 */
        char    *Map_Name;          /* name of map file                   */
        char    *Obj_Name;          /* object file name pattern           */
static  char    *StackSize;         /* size of stack                      */
static  char    DebugFlag;          /* debug info wanted                  */
static  char    Conventions;        /* 'r' for -3r or 's' for -3s         */
static  char    *CmdCopy;           /* just a copy of the pointer         */
static  size_t  CmdLen;             /* and Cmd's length so no overflow    */
static  char    Switch_Chars[4];    /* valid switch characters            */

struct  flags   Flags;

/*
 *  Static function prototypes
 */

int     Parse( void );
int     CompLink( void );
char    *SkipSpaces( char * );
void    Fputnl( char *, FILE * );
void    *MemAlloc( int );
void    MakeName( char *, char * );
void    Usage( void );
#if defined( __UNIX__ )
  #define _dos_switch_char() '-'
#elif defined( __OS2__ ) || defined( __NT__ )
  #define _dos_switch_char() '/'
#else
  extern  int     _dos_switch_char();
#endif
#ifdef __UNIX__
  #define EXE_EXT ""
#else
  #define EXE_EXT ".exe"
#endif


#undef pick
#define pick(code,english)      english

extern const char *WclMsgs[] = {
#include "wclmsg.h"
};

static const char *EnglishHelp[] = {
#include "wclhelp.h"
NULL
};


void print_banner( void )
{
    static int done;

    if( done ) return;
    puts( banner1w( _NAME_ "Compile and Link Utility",_WCL_VERSION_) );
    puts( banner2( "1988" ) );
    puts( banner3 );
    puts( banner3a );
    done = 1;
}

void  main()
/**********/
{
    int         rc;
    char        *wcl_env;
    char        *p;
    char        *q;

//      determine language to use and set WclMsgs

    CC_Opts[0] = '\0';

    Switch_Chars[0] = '-';
    Switch_Chars[1] = _dos_switch_char();
    Switch_Chars[2] = '\0';

    Word = MemAlloc( MAX_CMD );
    Cmd = MemAlloc( MAX_CMD*2 ); /*enough for cmd line & wcl variable*/

    /* add wcl environment variable to Cmd             */
    /* unless /y is specified in either Cmd or wcl */

    wcl_env = getenv( WCLENV );
    if( wcl_env != NULL ) {
        strcpy( Cmd, wcl_env );
        strcat( Cmd, " " );
        p = Cmd + strlen( Cmd );
        getcmd( p );
        q = Cmd;
        while( ( q = strpbrk( q, Switch_Chars ) ) != NULL ) {
            if( tolower( *(++q) ) == 'y' ) {
                getcmd( Cmd );
                p = Cmd;
                break;
            }
        }
    } else {
        getcmd( Cmd );
        p = Cmd;
    }
    p = SkipSpaces( p );
    if( *p == NULLCHAR || strncmp( p, "? ", 2 ) == NULL ) {
        Usage();
        exit( 1 );
    }

    CmdCopy = Cmd;
    if(CmdCopy)
        CmdLen = strlen( CmdCopy );

    Temp_Link = TEMPFILE;
    errno = 0; /* Standard C does not require fopen failure to set errno */
    if( ( Fp = fopen( &Temp_Link[ 1 ], "w" ) ) == NULL ) {
        /* Message before banner decision as '@' option uses Fp in Parse() */
        PrintMsg( WclMsgs[ UNABLE_TO_OPEN_TEMPORARY_FILE ], Temp_Link+1,
            strerror( errno ) );
        exit( 1 );
    }
    Map_Name = NULL;
    Obj_Name = NULL;
    Directive_List = NULL;
    rc = Parse();
    if( rc == 0 ) {
        if( ! Flags.be_quiet ) {
            print_banner();
        }
        rc = CompLink();
    }
    if( rc == 1 )  fclose( Fp );
    if( Link_Name != NULL ) {
        if( stricmp( Link_Name, &Temp_Link[ 1 ] ) != 0 ) {
            remove( Link_Name );
            rename( &Temp_Link[ 1 ], Link_Name );
        }
    } else {
        remove( &Temp_Link[ 1 ] );
    }
    exit( rc == 0 ? 0 : 1 );
}

static char *ScanFName( char *end, int len )
{
    int quoted;

    if( *Word == '=' ) {
        quoted = ( *(Word + 1) == '"' );
    } else {
        quoted = ( *Word == '"' );
    }
    quoted ^= ( *(end - 1) == '"' );
    for(;;) {   /* 15-jan-89: Allow '-' in filenames */
        if( *end == '\0' ) break;
        if( !quoted ) {
            if( *end == ' '  ) break;
            if( *end == '\t'  ) break;                  /* 16-mar-91 */
        } else if( *end == '"'  ) {
            quoted = 0;
        }
        Word[ len ] = *end;
        ++len;
        ++end;
    }
    Word[ len ] = NULLCHAR;
    return( end );
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
        if( stricmp( dot, ext ) == 0 ) {
            return( 1 );                // indicate file extension matches
        }
    }
    return( 0 );                        // indicate no match
}

static void AddDirective( int len )
/*********************************/
{
    struct directives *p;
    struct directives *p2;

    p = MemAlloc( sizeof(struct directives) );
    p->next = NULL;
    p->directive = MemAlloc( len + 1 );
    strcpy( p->directive, Word );
    if( Directive_List == NULL ) {
        Directive_List = p;
    } else {
        p2 = Directive_List;
        while( p2->next != NULL )  p2 = p2->next;
        p2->next = p;
    }
}

static  int  Parse( void )
/************************/
{
    char        opt;
    char        *end;
    FILE        *atfp;
    char        buffer[_MAX_PATH];
    int         len;
    char        *p;
    int         wcc_option;

    Flags.map_wanted   = 0;
    Flags.two_case     = 0;
    Flags.tiny_model   = 0;
    Flags.be_quiet     = 0;
    Flags.no_link      = 0;
    Flags.do_link      = 0;
    Flags.do_cvpack    = 0;
    Flags.link_for_dos = 0;
    Flags.link_for_os2 = 0;
    Flags.windows      = 0;
    Flags.math_8087    = 1;
    DebugFlag          = 0;
    StackSize = NULL;
    Conventions = 'r';

    /* Cmd will always begin with at least one */
    /* non-space character if we get this far  */

    for(;;) {
        Cmd = SkipSpaces( Cmd );
        if( *Cmd == NULLCHAR ) break;
        opt = *Cmd;
        if( opt == '-'  ||  opt == Switch_Chars[1] ) {
            Cmd++;
        } else if( opt != '@' ) {
            opt = ' ';
        }

        end = Cmd;
        if( *Cmd == '"' ) {
            for(;;) {
                ++end;
                if( *end == '\0' ) break;
                if( *end == '"'  ) break;
            }
        } else {
            for(;;) {
                if( *end == '\0' ) break;
                if( *end == ' '  ) break;
                if( *end == '\t'  ) break;              /* 16-mar-91 */
                if( opt == '-'  ||  opt == Switch_Chars[1] ) {
                    /* if we are processing a switch, stop at a '-' */
                    if( *end == '-' ) break;
#ifndef __UNIX__
                    if( *end == Switch_Chars[1] ) break;
#endif
                }
                ++end;
            }
        }
        len = end - Cmd;
        if( len != 0 ) {

            if( opt == ' ' ) {          /* if filename, add to list */
                strncpy( Word, Cmd, len );
                Word[ len ] = NULLCHAR;
                end = ScanFName( end, len );
                if( FileExtension( Word, ".lib" ) ) {
                    strcat( Libs, Libs[0] != '\0' ? "," : " " );
                    strcat( Libs, Word );
                } else {
                    strcat( Files, Word );
                    strcat( Files, " " );
                }
            } else {                    /* otherwise, do option */
                --len;
                strncpy( Word, Cmd + 1, len );
                Word[ len ] = NULLCHAR;
                wcc_option = 1;         /* assume its a wcc option */
                switch( tolower( *Cmd ) ) {

                case 'b':               /* possibly -bcl */
                    if( ('c' == tolower( Word[0] )) && ('l' == tolower( Word[1] )) && ('=' == tolower( Word[2] )) ){
                        char *  temp_cmd = strdup( Cmd + 1 + len);   /* copy from bcl option */
                        char *  sys_name = strdup( &Word[3] );
                        if( (NULL == sys_name) || (NULL == temp_cmd)){
                            PrintMsg( WclMsgs[ OUT_OF_MEMORY ] );
                            exit( 1 );
                        }

                        /* Sanity check */
                        if( (CmdLen + ( 2 * strlen(sys_name) ) + 8) > ( MAX_CMD*2 ) ){
                            PrintMsg( WclMsgs[ OUT_OF_MEMORY ] );
                            exit( 1 );
                        }

                        sprintf(Cmd, "bt=%s -l=%s", sys_name, sys_name);
                        strcat(Cmd, temp_cmd);
                        end = Cmd - 1;
                        wcc_option = 0;
                    }
                    break;

                case 'f':               /* files option */
                    end = ScanFName( end, len );
                    switch( tolower( Word[0] ) ) {
                    case 'd':           /* name of linker directive file */
                        Link_Name = "__WCL__.LNK";
                        if( Word[1] == '='  ||  Word[1] == '#' ) {
                            MakeName( Word, ".lnk" );    /* add extension */
                            Link_Name = strdup( Word + 2 );
                        }
                        wcc_option = 0;
                        break;
                    case 'e':           /* name of exe file */
                        if( Word[1] == '='  ||  Word[1] == '#' ) {
                            strcpy( Exe_Name, Word + 2 );
                        }
                        wcc_option = 0;
                        break;
                    case 'i':           /* name of forced include file */
                        break;
                    case 'm':           /* name of map file */
                        Flags.map_wanted = TRUE;
                        if( Word[1] == '='  ||  Word[1] == '#' ) {
                            Map_Name = strdup( Word + 2 );
                        }
                        wcc_option = 0;
                        break;
                    case 'o':           /* name of object file */
                        /* parse off argument, so we get right filename
                            in linker command file */
                        p = &Word[1];
                        if( Word[1] == '='  ||  Word[1] == '#' ) ++p;
                        Obj_Name = strdup( p );         /* 08-mar-90 */
                        break;
#if defined(WCLI86) || defined(WCL386)
                    case 'p':           /* floating-point option */
                        if( tolower( Word[1] ) == 'c' ) {
                            Flags.math_8087 = 0;
                        }
                        break;
#endif
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
                case 'l':               /* link target option */
                    switch( (Word[1] << 8) | tolower( Word[0] ) ) {
                    case 'p':
                        Flags.link_for_dos = 0;
                        Flags.link_for_os2 = TRUE;
                        break;
                    case 'r':
                        Flags.link_for_dos = TRUE;
                        Flags.link_for_os2 = 0;
                        break;
                    default:                    /* 10-jun-91 */
                        Flags.link_for_sys = TRUE;
                        p = &Word[0];
                        if( Word[0] == '='  ||  Word[0] == '#' ) ++p;
                        SystemName = strdup( p );
                        break;
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
                        end = ScanFName( end, len );
                        MakeName( Word, ".lnk" );
                        if( ( atfp = fopen( Word, "r" ) ) == NULL ) {
                            PrintMsg( WclMsgs[UNABLE_TO_OPEN_DIRECTIVE_FILE], Word );
                            return( 1 );
                        }
                        while( fgets( buffer, sizeof(buffer), atfp ) != NULL ) {
                            if( strnicmp( buffer, "file ", 5 ) == 0 ) {

                                /* look for names separated by ','s */

                                p = strchr( buffer, '\n' );
                                if( p )  *p = NULLCHAR;
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
#ifdef WCL386
                case '3':
                case '4':
                case '5':                           /* 22-sep-92 */
                    Conventions = tolower( Word[0] );
                    break;
#endif
                case 'd':
                    if( DebugFlag == 0 ){ /* not set by -h yet */
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
                    if( strcmp( Word, "w" ) == 0 ) {
                        DebugFlag = 3;
                    } else if( strcmp( Word, "c" ) == 0 ) { /* 02-mar-91 */
                        Flags.do_cvpack = 1;
                        DebugFlag = 4;
                    } else if( strcmp( Word, "d" ) == 0 ) {
                        DebugFlag = 5;
                    }
                    break;
                case 'i':           /* include file path */
                    end = ScanFName( end, len );
                    break;
                case 'c':           /* compile only */
                    if( stricmp( Word, "c" ) == 0 ) {
                        Flags.force_c = TRUE;
                    } else if( stricmp( Word, "c++" ) == 0 ) {
                        Flags.force_c_plus = TRUE;
                    } else {
                        Flags.no_link = TRUE;
                    }
                    /* fall through */
                case 'y':
                    wcc_option = 0;
                    break;
#if defined(WCLI86) || defined(WCL386)
                case 'm':           /* memory model */
                    if( Cmd[1] == 't' || Cmd[1] == 'T' ) { /* tiny model*/
                        Word[0] = 's';              /* change to small */
                        Flags.tiny_model = TRUE;
                    }
                    break;
#endif
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
#ifdef WCLI86
                    case 'w':
                        Flags.windows = TRUE;
#endif
                    }
                    break;
                case '"':                           /* 17-dec-91 */
                    wcc_option = 0;
                    if( *end == '"'  )  ++end;
                    AddDirective( len );
                    break;
                }

                /* don't add linker-specific options */
                /* to compiler command line:     */

                if( wcc_option ) {
                    len = strlen( CC_Opts );
                    CC_Opts[ len++ ] = ' ';
                    CC_Opts[ len++ ] = opt;
                    CC_Opts[ len++ ] = *Cmd;    /* keep original case */
                    CC_Opts[ len   ] = NULLCHAR;
                    strcat( CC_Opts, Word );
                }
            }
            Cmd = end;
        }
    }

    return( 0 );
}

static int useCPlusPlus( char *p )
{
    return
        stricmp( p, ".cpp" ) == 0 ||
        stricmp( p, ".cxx" ) == 0 ||
        stricmp( p, ".cc" )  == 0 ||
        stricmp( p, ".hpp" ) == 0 ||
        stricmp( p, ".hxx" ) == 0 ;
}

static char *SrcName( char *name )
/********************************/
{
    char        *cc_name;
    char        *exename;
    char        *p;
    char        buffer[_MAX_PATH2];
    char        *ext;

    _splitpath2( name, buffer, NULL, NULL, NULL, &ext );
    p = &ext[0];
    if( ext[0] == '\0' ) {
        p = strchr( name, '\0' );
        strcpy( p, ".cxx" );
        if( access( name, F_OK ) != 0 ) {
            strcpy( p, ".cpp" );
            if( access( name, F_OK ) != 0 ) {
                strcpy( p, ".cc" );
                if( access( name, F_OK ) != 0 ) {
                    strcpy( p, ".asm" );
                    if( access( name, F_OK ) != 0 ) {
                        strcpy( p, ".c" );
                    }
                }
            }
        }
    }
    if( stricmp( p, ".asm" ) == 0 ) {
        exename = "wasm" EXE_EXT;
        cc_name = "wasm";
    } else {
        exename = CC EXE_EXT;            // assume C compiler
        cc_name = CC;
        if( ! Flags.force_c ) {
            if( Flags.force_c_plus || useCPlusPlus( p ) ) {
                exename = CCXX EXE_EXT;  // use C++ compiler
                cc_name = CCXX;
            }
        }
    }
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
#if defined(WCL386) || defined(WCLI86)
    Fputnl( "option dosseg", Fp );
#endif
    if( StackSize != NULL ) {
        fputs( "option stack=", Fp );
        Fputnl( StackSize, Fp );
    }
    if( Flags.link_for_sys ) {                  /* 10-jun-91 */
        fputs( "system ", Fp );
        Fputnl( SystemName, Fp );
    } else {
#if defined(WCLAXP)
        Fputnl( "system ntaxp", Fp );
#elif defined(WCLPPC)
        Fputnl( "system ntppc", Fp );
#elif defined(WCL386)
  #if defined(__OS2__)
        Fputnl( "system os2v2", Fp );           /* 04-feb-92 */
  #elif defined(__NT__)
        Fputnl( "system nt", Fp );
  #elif defined(__LINUX__)
        Fputnl( "system linux", Fp );
  #else
        Fputnl( "system dos4g", Fp );
  #endif
#else
        if( Flags.windows ) {                   /* 15-mar-90 */
            Fputnl( "system windows", Fp );
        } else if( Flags.tiny_model ) {
            Fputnl( "system com", Fp );
        } else if( Flags.link_for_dos ) {
            Fputnl( "system dos", Fp );
        } else if( Flags.link_for_os2 ) {
            Fputnl( "system os2", Fp );
        } else {
#if defined(__OS2__)
            Fputnl( "system os2", Fp );
#else
            Fputnl( "system dos", Fp );
#endif
        }
#endif
    }
    for( d_list = Directive_List; d_list; d_list = d_list->next ) {
        Fputnl( d_list->directive, Fp );
    }

    errors_found = 0;                   /* 21-jan-92 */
    p = Files;
    while( *p != '\0' ) {
        if( *p == '"' ) {
            end = strpbrk(++p, "\"");        /* get quoted filespec */
        } else {
            end = strpbrk(p, " ");         /* get filespec */
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
                if( p != NULL )  *p = NULLCHAR;
                strcpy( Word, file );
            }
            AddName( Word, Fp );
            if( Exe_Name[0] == '\0' ) {
                p = strrchr( Word, '.' );
                if( p != NULL )  *p = NULLCHAR;
                strcpy( Exe_Name, Word );
            }
            file = GetName( NULL );     /* get next filename */
        }
        p = end;        /* get next filespec */
    }
    if( errors_found )  return( 1 );            /* 21-jan-92 */
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
            return( 2 );        /* return 2 to show Temp_File already closed */
        }
        if( Flags.do_cvpack ){
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


static  char  *SkipSpaces( char *ptr )
/************************************/
{
    while( *ptr == ' ' || *ptr == '\t' )  ptr++;    /* 16-mar-91 */
    return( ptr );
}


static  void  MakeName( char *name, char *ext )
/*********************************************/
{
    /* If the last '.' is before the last path seperator character */
    if( strrchr( name, '.' ) <= strpbrk( name, PATH_SEP_STR ) ) {
        strcat( name, ext );
    }
}


static  void  Usage( void )
/*************************/
{
    char const  **list;
    char const  *p;
    int         lines_printed;
    unsigned int i, n;
    auto        char buf[82];
#ifndef __UNIX__
    int const   paging = isatty( fileno( stdout ) );
    int const   height = 25; /* Number of lines assumed on screen */
#endif

    print_banner();
    lines_printed = 4;
    list = EnglishHelp;
    while( *list ) {
        memset( buf, ' ', 80 );
        if( **list == '[' ) {                   /* title to be centered */
            i = strlen( *list );
            strcpy( &buf[38-i/2], *list );
            ++list;
            for( n = 0; list[n]; ++n ) {        /* count number in list */
                if( *list[n] == '[' ) break;
            }
            n = (n+1) / 2;                      /* half way through list */
#ifndef __UNIX__
            if( paging && lines_printed != 0 && lines_printed + n > height ) {
                fputs( WclMsgs[ PRESS_ANY_KEY_TO_CONTINUE ], stdout );
                fflush( stdout );
                getch();
                puts( "" );
                lines_printed = 0;
            }
#endif
            puts( buf );
            lines_printed++;
            for(;;) {
                memset( buf, ' ', 80 );
                p = *list;
                if( p == NULL ) break;
                for( i = 0; *p; )  buf[i++] = *p++;
                p = list[n];
                if( p != NULL  &&  *p != '[' ) {
                    for( i = 38; *p; )  buf[i++] = *p++;
                }
                buf[i] = '\0';
                puts( buf );
                lines_printed++;
                p = list[n];
                if( p == NULL ) break;
                if( *p == '[' ) break;
                list[n] = NULL; /* indicate already printed */
                ++list;
            }
            list = &list[n];
        } else {
            puts( *list );
            lines_printed++;
            ++list;
        }
    }
}
