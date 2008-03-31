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
* Description:  Compile and link utility for Open Watcom FORTRAN.
*
****************************************************************************/


#include "ftnstd.h"
#include "ftextfun.h"
#include "errcod.h"
#include "optflags.h"
#include "cioconst.h"
#include "banner.h"
#include "errrtns.h"

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifndef __UNIX__
  #include <direct.h>
#endif
#include <process.h>
#include <malloc.h>

/* forward declarations */
static  void    Usage( void );
static  int     Parse( void );
static  void    FindPath( char *name, char *buf );
static  int     CompLink( void );
static  void    MakeName( char *name, char *ext );
static  void    Fputnl( char *text, FILE *fptr );
static  int     IsOption( char *cmd, int cmd_len, char *opt );
static  void    AddName( char *name, FILE *link_fp );


#if _CPU == 386
  #define _CmpName        "wfc386"        // compiler name
  #define _CmpExeName     "wfc386.exe"    // compiler executable file name
  #define _LnkTmpFile     "@_WFL386_.LNK" // temporary linker directive file
  #define _WCLEnv         "WFL386"        // "wcl" environment variable
#elif _CPU == 8086
  #define _CmpName        "wfc"           // copmiler name
  #define _CmpExeName     "wfc.exe"       // compiler exe file name
  #define _LnkTmpFile     "@__WFL__.LNK"  // temporary linker directive file
  #define _WCLEnv         "WFL"           // "wcl" environment variable
#elif _CPU == _AXP
  #define _CmpName        "wfcaxp"         // copmiler name
  #define _CmpExeName     "wfcaxp.exe"     // compiler exe file name
  #define _LnkTmpFile     "@_WFLAXP_.LNK"// temporary linker directive file
  #define _WCLEnv         "WFLAXP"         // "wcl" environment variable
#elif _CPU == _PPC
  #define _CmpName        "wfcppc"         // copmiler name
  #define _CmpExeName     "wfcppc.exe"     // compiler exe file name
  #define _LnkTmpFile     "@_WFLPPC_.LNK"// temporary linker directive file
  #define _WCLEnv         "WFLPPC"         // "wcl" environment variable
#else
  #error Unknown OS/CPU
#endif

#if defined( __QNX__ )
  #define _ObjExtn        ".o"          // object file extension
#else
  #define _ObjExtn        ".obj"        // object file extension
#endif

#define _LnkName        "wlink"         // linker name
#define _LnkExeName     "wlink.exe"     // linker executable file name

#define _CVPName        "cvpack"        // Codeview pack name
#define _CVPExeName     "cvpack.exe"    // Codeview pack executable file name

#define NULL_STR        ""
#define NULLCHAR        '\0'
#define ATTR_MASK       _A_HIDDEN + _A_SYSTEM + _A_VOLID + _A_SUBDIR
                                        // mask for illegal file types
#define TRUE            1
#define FALSE           0

typedef struct list {
    char        *filename;
    struct list *next;
} list;
#if defined( __OS2__ ) || defined( __NT__ )
#define MAX_CMD 10240
#else
#define MAX_CMD 130
#endif

static  char    *Cmd;                   // command line parameters
static  char    *Word;                  // one parameter
static  char    Files[MAX_CMD];         // list of filenames from Cmd
static  char    Libs[MAX_CMD];          // list of libraries from Cmd
static  char    CmpOpts[MAX_CMD];       // list of compiler options from Cmd
static  char    CmpPath[_MAX_PATH];     // path for compiler executable file
static  char    LinkPath[_MAX_PATH];    // path for linker executable file
static  FILE    *Fp;                    // file pointer for TempFile
static  char    *LinkName;              // name for TempFile if /fd specified
static  char    *TempFile;              // temporary linker directive file
static  list    *ObjList;               // linked list of object filenames
static  char    SwitchChars[3];         // valid switch characters
static  char    ExeName[_MAX_PATH];     // name of executable
static  char    *ObjName;               // object file name pattern
static  char    *SystemName;            // system name
static  int     DebugFlag;              // debugging flag

static char *DebugOptions[] = {
    "",
    "debug dwarf\n",
    "debug dwarf\n",
    "debug watcom all\n",
    "debug codeview\n",
    "debug dwarf\n"
};

static  struct flags {
    unsigned quiet        : 1;  // compile quietly
    unsigned no_link      : 1;  // compile only, no link step
    unsigned link_for_sys : 1;  // system specified
#if _CPU == 386 || _CPU == _AXP || _CPU == _PPC
    unsigned default_win  : 1;  // OS/2 default windowed application
#else
    unsigned windows      : 1;  // Windows application
    unsigned link_for_dos : 1;  // produce DOS executable
    unsigned link_for_os2 : 1;  // produce OS/2 executable
#endif
    unsigned do_cvpack    : 1;  // do codeview cvpack
} Flags;

#if _CPU == 386
    #define     _NAME_  "F77/32 "
#elif _CPU == 8086
    #define     _NAME_  "F77/16 "
#elif _CPU == _AXP
    #define     _NAME_  "F77 Alpha AXP "
#elif _CPU == _PPC
    #define     _NAME_  "F77 PowerPC "
#else
    #error Unknown System
#endif


static  void    wfl_exit( int rc ) {
//==================================

    __ErrorFini();
    exit( rc );
}


static  void    PrintMsg( uint msg, ... ) {
//=========================================

    va_list     args;
    char        buff[LIST_BUFF_SIZE+1];

    va_start( args, msg );
    __BldErrMsg( msg, buff, args );
    va_end( args );
    puts( &buff[1] ); // skip leading blank
}


static  void    PrtBanner( void ) {
//===========================

    puts( banner1w( _NAME_ "Compile and Link Utility", _WFL_VERSION_ ) );
    puts( banner2( "1990" ) );
    puts( banner3 );
    puts( banner3a );
}


static  char    *SkipSpaces( char *ptr ) {
//========================================

    while( *ptr == ' ' ) ptr++;
    return( ptr );
}


static  void    *MemAlloc( int size ) {
//=====================================

    void            *ptr;

    ptr = malloc( size );
    if( ptr == NULL ) {
        PrintMsg( CL_OUT_OF_MEMORY );
        wfl_exit( 1 );
    }
    return( ptr );
}


void    main( int argc, char *argv[] ) {
//======================================

    int         rc;
    char        *wcl_env;
    char        *p;
    char        *q;

    argc = argc;

    __InitResource();
    __ErrorInit( argv[0] );

    CmpOpts[0] = '\0';

    SwitchChars[0] = '/';
    SwitchChars[1] = '-';
    SwitchChars[2] = '\0';

    Word = MemAlloc( MAX_CMD );
    Cmd = MemAlloc( 2*MAX_CMD ); // for "wfl" environment variable and command line

    // add "wcl" environment variable to "Cmd" unless "/y" is specified
    // in "Cmd" or the "wcl" environment string

    wcl_env = getenv( _WCLEnv );
    if( wcl_env != NULL ) {
        strcpy( Cmd, wcl_env );
        strcat( Cmd, " " );
        p = Cmd + strlen( Cmd );
        getcmd( p );
        q = Cmd;
        for(;;) {
            q = strpbrk( q, SwitchChars );
            if( q == NULL ) break;
            ++q;
            if( tolower( *q ) == 'y' ) {
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
    if( ( *p == NULLCHAR ) || ( strncmp( p, "? ", 2 ) == NULL ) ) {
        Usage();
        wfl_exit( 1 );
    }
    TempFile = _LnkTmpFile;
    Fp = fopen( &TempFile[1], "w" );
    if( Fp == NULL ) {
        PrintMsg( CL_ERROR_OPENING_TMP_FILE );
        wfl_exit( 1 );
    }
    ObjName = NULL;
    rc = Parse();
    FindPath( _CmpExeName, CmpPath );
    if( rc == 0 ) {
        if( !Flags.quiet ) {
            PrtBanner();
        }
        rc = CompLink();
    }
    if( rc == 1 ) fclose( Fp );
    if( LinkName != NULL ) {
        if( stricmp( LinkName, &TempFile[1] ) != 0 ) {
            remove( LinkName );
            rename( &TempFile[1], LinkName );
        }
    } else {
        remove( &TempFile[1] );
    }
    wfl_exit( rc == 0 ? 0 : 1 );
}


static  char    *ScanFName( char *end, int len ) {
//================================================

    for(;;) {
        if( *end == '\0' ) break;
        if( *end == ' '  ) break;
        if( *end == SwitchChars[0] ) break;
        if( *end == SwitchChars[1] ) break;
        Word[len] = *end;
        ++len;
        ++end;
    }
    Word[len] = NULLCHAR;
    return( end );
}


static  int     Parse( void ) {
//=============================

    char        opt;
    char        *end;
    int         len;
    int         cmp_option;
    char        in_quotes;

    Flags.no_link = 0;
    Flags.link_for_sys = 0;
    Flags.quiet        = 0;
#if _CPU == 386 || _CPU == _AXP || _CPU == _PPC
    Flags.default_win  = 0;
#else
    Flags.windows      = 0;
    Flags.link_for_dos = 0;
    Flags.link_for_os2 = 0;
#endif
    Flags.do_cvpack    = 0;

    DebugFlag = 0;

    // "Cmd" will always begin with at least one
    // non-space character if we get this far

    do {
        opt = *Cmd;
        if( ( opt == SwitchChars[0] ) || ( opt == SwitchChars[1] ) ) {
            Cmd++;
        } else {
            opt = ' ';
        }
        in_quotes = FALSE;
        end = Cmd;
        for(;;) {
            if( *end == '\0' ) break;
            if( *end == '"' ) {
                if( in_quotes ) break;
                in_quotes = TRUE;
            }
            if( !in_quotes ) {
                if( *end == ' '  ) break;
                if( *end == SwitchChars[0] ) break;
                if( *end == SwitchChars[1] ) break;
            }
            ++end;
        }
        len = end - Cmd;
        if( len != 0 ) {
            if( opt == ' ' ) {  // if filename, add to list
                strncpy( Word, Cmd, len );
                Word[len] = NULLCHAR;
                strlwr( Word );
                if( strstr( Word, ".lib" ) != NULL ) {
                    strcat( Libs, Libs[0] != '\0' ? "," : " " );
                    strcat( Libs, Word );
                } else {
                    strcat( Files, Word );
                    strcat( Files, " " );
                }
            } else {            // otherwise, do option
                --len;
                strncpy( Word, Cmd + 1, len );
                Word[len] = NULLCHAR;
                cmp_option = 1; // assume its a compiler option
                switch( tolower( *Cmd ) ) {
                case 'f':       // files option
                    end = ScanFName( end, len );
                    switch( tolower( Word[0] ) ) {
                    case 'd':   // name of linker directive file
                        if( Word[1] == '\0' ) {
                            LinkName = _LnkTmpFile;
                            ++LinkName;     // skip over '@'
                            cmp_option = 0;
                        } else if( (Word[1] == '=') || (Word[1] == '#') ) {
                            MakeName( Word, ".lnk" );    // add extension
                            LinkName = strdup( Word + 2 );
                            cmp_option = 0;
                        }
                        break;
                    case 'e':   // name of exe file
                        if( ( Word[1] == '=' ) || ( Word[1] == '#' ) ) {
                            fputs( "name ", Fp );
                            Fputnl( Word + 2, Fp );
                            strcpy( ExeName, Word + 2 );
                            cmp_option = 0;
                        }
                        break;
                    case 'm':   // name of map file
                        if( Word[1] == '\0' ) {
                            fputs( "option map\n", Fp );
                            cmp_option = 0;
                        } else if( (Word[1] == '=') || (Word[1] == '#') ) {
                            fputs( "option map=", Fp );
                            Fputnl( Word + 2, Fp );
                            cmp_option = 0;
                        }
                        break;
                    case 'i':
                        if( ( Word[1] == '=' ) || ( Word[1] == '#' ) ) {
                            fputs( "@", Fp );
                            Fputnl( Word + 2, Fp );
                            cmp_option = 0;
                        }
                        break;
                    case 'o':   // name of object file
                        // parse off argument, so we get right filename
                        // in linker command file
                        if( ( Word[1] == '=' ) || ( Word[1] == '#' ) ) {
                            ObjName = strdup( &Word[2] );
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                case 'k':       // stack size option
                    if( ( Word[0] == '=' ) || ( Word[0] == '#' ) ) {
                        fputs( "option stack=", Fp );
                        Fputnl( Word + 1, Fp );
                        cmp_option = 0;
                    }
                    break;
                case 'c':       // compile only
                    if( Word[0] == '\0' ) {
                        Flags.no_link = 1;
                        cmp_option = 0;
                    }
                    break;
                case 'y':
                    if( Word[0] == '\0' ) {
                        cmp_option = 0;
                    }
                    break;
                case 'p':
                    // ignore the /p option - we now only
                    // have a protect-mode compiler
                    if( Word[0] == '\0' ) {
                        cmp_option = 0;
                    }
                    break;
                case 'l':
                    if( ( Word[0] == '=' ) || ( Word[0] == '#' ) ) {
                        Flags.link_for_sys = 1;
                        SystemName = strdup( &Word[1] );
                        cmp_option = 0;
#if _CPU == 8086
                    } else if( stricmp( Word, "r" ) == 0 ) {
                        Flags.link_for_dos = 1;
                        Flags.link_for_os2 = 0;
                        cmp_option = 0;
                    } else if( stricmp( Word, "p" ) == 0 ) {
                        Flags.link_for_os2 = 1;
                        Flags.link_for_dos = 0;
                        cmp_option = 0;
#endif
                    }
                    break;
                case '"':
                    Fputnl( &Word[0], Fp );
                    ++end;      // skip over closing quote
                    cmp_option = 0;
                    break;

                // compiler options that affect the linker

#if _CPU == 386 || _CPU == _AXP || _CPU == _PPC
                case 'b':
                    if( stricmp( Word, "w" ) ) {
                        Flags.default_win = 1;
                    }
                    break;
#endif

                case 'q':
                    if( IsOption( Cmd, len + sizeof(char), "Quiet" ) ) {
                        Flags.quiet = 1;
                    }
                    break;
                case 'd':
                    if( DebugFlag == 0 ) { // not set by -h yet
                        if( strcmp( Word, "1" ) == 0 ) {
                            DebugFlag = 1;
                        } else if( strcmp( Word, "2" ) == 0 ) {
                            DebugFlag = 2;
                        }
                    }
                    break;
                case 'h':
                    if( strcmp( Word, "w" ) == 0 ) {
                        DebugFlag = 3;
                    } else if( strcmp( Word, "c" ) == 0 ) {
                        Flags.do_cvpack = 1;
                        DebugFlag = 4;
                    } else if( strcmp( Word, "d" ) == 0 ) {
                        DebugFlag = 5;
                    }
                    break;
                case 's':
                    if( IsOption( Cmd, len + sizeof( char ), "SYntax" ) ) {
                        Flags.no_link = 1;
                    }
                    break;
#if _CPU == 8086
                case 'w':
                    if( IsOption( Cmd, len + sizeof( char ), "WIndows" ) ) {
                        Flags.windows = 1;
                    }
                    break;
#endif
                default:
                    break;
                }
                // don't add linker-specific options to compiler command line
                if( cmp_option != 0 ) {
                    len = strlen( CmpOpts );
                    CmpOpts[len++] = ' ';
                    CmpOpts[len++] = opt;
                    CmpOpts[len++] = *Cmd;      // keep original case
                    CmpOpts[len] = NULLCHAR;
                    strcat( CmpOpts, Word );
                }
            }
            Cmd = end;
        }
        Cmd = SkipSpaces( Cmd );
    } while( *Cmd != NULLCHAR );
    return( 0 );
}


static int     IsOption( char *cmd, int cmd_len, char *opt ) {
//=====================================================

    int         len;

    len = 0;
    for(;;) {
        if( len == cmd_len ) break;
        if( toupper( *cmd ) != toupper( *opt ) ) return( 0 );
        ++cmd;
        ++opt;
        ++len;
    }
    if( *opt == NULLCHAR ) return( 1 );
    if( isupper( *opt ) ) return( 0 );
    return( 1 );
}


static  char    *MakePath( char *path ) {
//=======================================

    char    *p;

    p = strrchr( path ,'\\' );
    if( p != NULL ) {
        p[1] = NULLCHAR;
    } else {
        p = strchr( path, ':' );
        if( p != NULL ) {
            p[1] = NULLCHAR;
        } else {
            *path = NULLCHAR;
        }
    }
    return( strdup( path ) );
}


static  char    *GetName( char *path ) {
//======================================

#ifndef __UNIX__
    static      DIR     *dirp;
    struct      dirent  *direntp;

    if( path != NULL ) {                /* if given a filespec to open,  */
        if( *path == NULLCHAR ) {       /*   but filespec is empty, then */
            closedir( dirp );           /*   close directory and return  */
            return( NULL );             /*   (for clean up after error)  */
        }
        dirp = opendir( path );         /* try to find matching filenames */
        if( dirp == NULL ) {
            PrintMsg( CL_UNABLE_TO_OPEN, path );
            return( NULL );
        }
    }

    while( ( direntp = readdir( dirp ) ) != NULL ) {
        if( ( direntp->d_attr & ATTR_MASK ) == 0 ) {    /* valid file? */
            return( direntp->d_name );
        }
    }
    closedir( dirp );
    return( NULL );
#else
    char *name;
    if ( path == NULL )
            return NULL;
    name = strrchr(path, '/');
    if ( name == NULL )
        name = path;
    else
        name++;
    return ( strdup(name) );
#endif
}


static  int     CompLink( void ) {
//================================

    int         rc;
    char        *p;
    char        *file;
    char        *path;
    int         comp_err;

    if( Flags.quiet ) {
        Fputnl( "option quiet", Fp );
    }
    fputs( DebugOptions[ DebugFlag ], Fp );
    if( Flags.link_for_sys ) {
        fputs( "system ", Fp );
        Fputnl( SystemName, Fp );
    } else {
#if defined( __QNX__ )
        Fputnl( "system qnx", Fp );
#elif _CPU == 386
    #if defined( __OS2__ )
        Fputnl( "system os2v2", Fp );
    #elif defined( __NT__ )
        Fputnl( "system nt", Fp );
    #else
        Fputnl( "system dos4g", Fp );
    #endif
#elif _CPU == 8086
        if( Flags.windows ) {
            Fputnl( "system windows", Fp );
        } else if( Flags.link_for_dos ) {
            Fputnl( "system dos", Fp );
        } else if( Flags.link_for_os2 ) {
            Fputnl( "system os2", Fp );
        } else {
    #if defined( __OS2__ )
            Fputnl( "system os2", Fp );
    #else
            Fputnl( "system dos", Fp );
    #endif
        }
#elif _CPU == _AXP
        Fputnl( "system ntaxp", Fp );
#else
    #error Unknown System
#endif

    }

    comp_err = FALSE;
    p = strtok( Files, " " );   // get first filespec
    while( p != NULL ) {
        strcpy( Word, p );
        MakeName( Word, ".for" );   // if no extension, assume ".for"

        file = GetName( Word );     // get first matching filename
        path = MakePath( Word );    // isolate path portion of filespec
        while( file != NULL ) {     // while more filenames:
            strcpy( Word, path );
            strcat( Word, file );
            strlwr( Word );
            if( strstr( Word, _ObjExtn ) == NULL ) {  // if not object, compile
                if( !Flags.quiet ) {
                    fputs( "        " _CmpName " ", stdout );
                    fputs( Word, stdout );
                    fputs( " ", stdout );
                    puts( CmpOpts );
                    fflush( stdout );
                }
                rc = spawnlp( P_WAIT, CmpPath, _CmpName, Word, CmpOpts, NULL );
                if( rc != 0 ) {
                    comp_err = TRUE;
                    if( ( rc == -1 ) || ( rc == 255 ) ) {
                        PrintMsg( CL_UNABLE_TO_INVOKE_COMPILER );
                        GetName( NULL_STR );
                        return( 1 );
                    } else {
                        PrintMsg( CL_BAD_COMPILE, Word );
                    }
                }
                strcpy( Word, file );
                strlwr( Word );
            }
            p = strrchr( Word, '.' );
            if( p != NULL ) *p = NULLCHAR;

            if( ExeName[0] == '\0' ) {
                fputs( "name ", Fp );
                Fputnl( Word, Fp );
                strcpy( ExeName, Word );
            }
            AddName( Word, Fp );

            file = GetName( NULL );     // get next filename
        }
        p = strtok( NULL, " " );    // get next filespec
    }
    if( Libs[0] != '\0' ) {
        fputs( "library", Fp );
        Fputnl( Libs, Fp );
    }
    fclose( Fp );   // close TempFile

    if( ( ObjList != NULL ) && !Flags.no_link && !comp_err ) {
        FindPath( _LnkExeName, LinkPath );
        if( !Flags.quiet ) {
            puts( NULL_STR );
        }
        fflush( stdout );
        rc = spawnlp( P_WAIT, LinkPath, _LnkName, TempFile, NULL );
        if( rc != 0 ) {
            if( ( rc == -1 ) || ( rc == 255 ) ) {
                PrintMsg( CL_UNABLE_TO_INVOKE_LINKER );
            } else {
                PrintMsg( CL_BAD_LINK );
            }
            return( 2 );    // return 2 to show Temp_File already closed
        }
        if( Flags.do_cvpack ){
            FindPath( _CVPExeName, LinkPath );
            rc = spawnlp( P_WAIT, LinkPath, _CVPName, ExeName, NULL );
            if( rc != 0 ) {
                if( rc == -1  ||  rc == 255 ) {
                    PrintMsg( CL_UNABLE_TO_INVOKE_CVPACK );
                } else {
                    PrintMsg( CL_BAD_CVPACK );
                }
                return( 2 );  /* return 2 to show Temp_File already closed */
            }
        }
    }
    return( 0 );
}


static  void    Fputnl( char *text, FILE *fptr ) {
//================================================

    fputs( text, fptr );
    fputs( "\n", fptr );
}


static  void    MakeName( char *name, char *ext ) {
//=================================================

    if( strrchr( name, '.' ) <= strstr( name, "\\" ) ) {
        strcat( name, ext );
    }
}


static  void    AddName( char *name, FILE *link_fp ) {
//====================================================

    list        *curr_name;
    list        *last_name;
    list        *new_name;
    char        path[_MAX_PATH];
    char        buff1[_MAX_PATH2];
    char        buff2[_MAX_PATH2];
    char        *drive;
    char        *dir;
    char        *fname;
    char        *ext1;
    char        *ext2;
    char        *extension;

    curr_name = ObjList;
    while( curr_name != NULL ) {
        if( stricmp( name, curr_name->filename ) == 0 )  return;
        last_name = curr_name;
        curr_name = curr_name->next;
    }
    new_name = MemAlloc( sizeof( struct list ) );
    if( ObjList == NULL ) {
        ObjList = new_name;
    } else {
        last_name->next = new_name;
    }
    new_name->filename = strdup( name );
    new_name->next = NULL;
    fputs( "file ", link_fp );
    if( ObjName != NULL ) {
        // construct full name of object file from ObjName information
        _splitpath2( ObjName, buff1, &drive, &dir, &fname, &ext1 );
        extension = ext1;
        if( ext1[0] == '\0' ) extension = _ObjExtn;
        if( ( fname[0] == '\0' ) ||
            ( ( fname[0] == '*' ) && ( fname[1] == '\0' ) ) ) {
            _splitpath2( name, buff2, NULL, NULL, &fname, &ext2 );
            if( ext2[0] != '\0' ) extension = ext2;
        }
        _makepath( path, drive, dir, fname, extension );
        name = path;
    }
    Fputnl( name, link_fp );
}


static  void    FindPath( char *name, char *buf ) {
//=================================================

    _searchenv( name, "PATH", buf );
    if( buf[0] == '\0' ) {
        PrintMsg( CL_UNABLE_TO_FIND, name );
        wfl_exit( 1 );
    }
}


void    TOutNL( char *msg ) {
//===========================

    puts( msg );
}


void    TOut( char *msg ) {
//===========================

    fputs( msg, stdout );
}


#define opt( name, bit, flags, actionstr, actionneg, desc ) name, desc, flags

#include "cpopt.h"
#include "optinfo.h"


static  void    Usage( void ) {
//=============================

    char        buff[LIST_BUFF_SIZE+1];

    PrtBanner();
    puts( "" );
    MsgBuffer( CL_USAGE_LINE, buff );
    puts( buff );
    puts( "" );
    ShowOptions( buff );
}
