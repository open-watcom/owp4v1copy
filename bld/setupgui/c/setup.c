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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <unistd.h>
#include "gui.h"
#include "setup.h"
#include "setupinf.h"
#include "gendlg.h"
#include "genvbl.h"
#include "utils.h"

extern void InitIO( void );          /* fns. from wpack */
extern void EvalRegNumber();
extern void SetupTextTable( void );
extern void SaveState(void);
extern int CountDisks( bool * );
extern void DoSpawn( when_time );
extern void SetupTitle();
extern void DeleteObsoleteFiles();
extern void ResetDiskInfo(void);
extern void EspeciallyUglyLittleKanjiiKludge();
extern void StampEvalFiles();
extern void SelfRegisterDynamo();
extern void MakeEmbedded();

#if defined( WINNT ) && defined( WSQL )
    extern bool NIDSetup( void );
#endif
#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
    extern bool MakePackageDir();
    extern int MSBackOffice;
#endif
#if defined( WINNT ) && defined( WSQL )
    extern bool JustDoNIDSetup;
#endif
#ifdef PATCH
    int IsPatch = 0;
#endif
extern int SkipDialogs;
extern bool CancelSetup;
extern vhandle UnInstall;
typedef enum {
    Stack_Push,
    Stack_Pop,
    Stack_IsEmpty } DIR_PARAM_STACK_OPS;


static bool SetupOperations()
/***************************/
{
    bool                ok;
    bool                uninstall;

    ok = ok;
    #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
        if( GetVariableIntVal( "MakePackage" ) == 1 ) {

            if( MakePackageDir() ) {
                return( TRUE );
            } else {
                return( FALSE );
            }
        }
    #endif
    if( GetVariableIntVal( "MakeDisks" ) == 1 ) {
        if( MakeDisks() ) {
            return( TRUE );
        } else {
            return( FALSE );
        }
    }

    // are we doing an UnInstall?
    uninstall = VarGetIntVal( UnInstall );

    if( GetVariableIntVal( "IsUpgrade" ) == 1 ) {
        if( !CheckUpgrade() ) {
            return( FALSE );
        }
    }

    DoSpawn( WHEN_BEFORE );
    #ifdef PATCH
        if( GetVariableIntVal( "Patch" ) == 1 ) {
            IsPatch = 1;
            if( !PatchFiles() ) {
                return( FALSE );
            }
        }
    #endif
    // Copy the files
    if( GetVariableIntVal( "DoCopyFiles" ) == 1 ) {
        if( !CopyAllFiles() ) {
            return( FALSE );
        }
    }
    #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) )
        if( GetVariableIntVal( "DoEmbedded" ) == 1 ) {
            MakeEmbedded();
        }
    #endif
    DoSpawn( WHEN_AFTER );

    // Modify AUTOEXEC.BAT and CONFIG.SYS
    if( GetVariableIntVal( "DoModifyAuto" ) == 1 ) {
        if( !ModifyStartup( uninstall ) ) {
            return( FALSE );
        }
    }
    #if defined( WSQL ) && !defined( _UI )
    // Modify Registry and INI file settings
    if( GetVariableIntVal( "DoProfile" ) == 1 ) {
        WriteProfileStrings( uninstall );  // will write to the win.ini file.
    }
    #endif
    // Create program group (folder)
    if( GetVariableIntVal( "DoCreateIcons" ) == 1 ||
        GetVariableIntVal( "DoCreateHelpIcons" ) == 1 ) {
        if( !ModifyEnvironment( uninstall ) ) {
            return( FALSE );
        }
    }
    DoSpawn( WHEN_END );
    #ifdef WSQL
        #if defined( __WINDOWS__ ) || defined( __NT__ )
        // Convert ODBC data sources
        if( !uninstall && GetVariableIntVal( "DoODBCConvert" ) == 1 ) {
            if( !UpdateODBC() ) {
                return( FALSE );
            }
            #if defined( __NT__ )
            if( !RegUpdateODBC() ) {
                return( FALSE );
            }
            #endif
        }
        #endif

        // Apply the license
        if( !uninstall && GetVariableIntVal( "DoLicense" ) == 1 ) {
            StatusLines( STAT_APPLYLICENSE, "" );
            ok = ApplyLicense();
            StatusLines( STAT_BLANK, "" );
            StatusAmount( 0, 1 );
            if( !ok ) {
                return( FALSE );
            }
        }

        // Create evaluation version
        if( !uninstall && GetVariableIntVal( "IsEval" ) == 1 ) {
            StampEvalFiles();
        }

        #if defined( WINNT )
            // Self-register any dll
            if( !uninstall && GetVariableIntVal( "SelfRegisterDynamo" ) == 1 ) {
                SelfRegisterDynamo();
            }

            if( !uninstall && GetVariableIntVal( "DoDynamoConfig" ) ) {
                if( !NIDSetup() ) {
                    return( FALSE );
                }
            }
        #endif
    #endif

    return( TRUE );
}

#define MAX_DIAGS 20

#ifdef __WINDOWS__
bool CheckForSetup32( int argc, char **argv )
{
    DWORD       version = GetVersion();
    int         winver;
    char        *buff;
    int         mem_needed;
    int         i;
    char        new_exe[_MAX_PATH];
    char        drive[_MAX_DRIVE];
    char        path[_MAX_PATH];
    char        name[_MAX_FNAME];
    char        ext[_MAX_EXT];
    char        *os;

    winver = LOBYTE( LOWORD( version ) ) * 100 + HIBYTE( LOWORD( version ) );
    os = getenv( "OS" );
    if( winver >= 390 || ( os != NULL && stricmp( os, "Windows_NT" ) == 0 ) ) {
        _splitpath( argv[0], drive, path, name, ext );
        _makepath( new_exe, drive, path, "SETUP32", ext );
        mem_needed = strlen( new_exe );
        for( i = 1; i < argc; i++ ) {
            mem_needed += strlen( argv[ i ] ); // command line arguments
        }
        mem_needed += i; //spaces between arguments + terminating null
        buff = malloc( mem_needed );
        if( buff == NULL ) {
            return( FALSE );
        }
        strcpy( buff, new_exe );
        if( access( buff, F_OK ) == 0 ) {
            for( i = 1; i < argc; i++ ) {
                strcat( buff, " " );
                strcat( buff, argv[ i ] );
            }
            WinExec( buff, SW_SHOW );
            return( TRUE );
        }
    }
    return( FALSE );
}
#endif

#if defined( __NT__ )
static bool CheckWin95Uninstall( int argc, char **argv )
{
// The Windows 95 version of setup gets installed as the
// uninstall utility. So that it can erase itself, the setup
// program gets copied to the Windows directory, and run from
// there. The version in the Windows directory gets erased by
// the WININIT program.

    int                 len;
    char                buff[ 2 * _MAX_PATH ];
    char                drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ];
    char                name[ _MAX_FNAME ];

    if( argc > 1 && stricmp( argv[ 1 ], "-u" ) == 0 ) {
        // copy setup program to unsetup.exe in system directory
        GetWindowsDirectory( buff, _MAX_PATH );
        strcat( buff, "\\UnSetup.exe" );
        if( DoCopyFile( argv[ 0 ], buff, FALSE ) == CFE_NOERROR ) {
            // add entry to wininit.ini to erase unsetup.exe
            WritePrivateProfileString( "rename", "NUL", buff, "wininit.ini" );
            // setup.inf should be in same directory as setup.exe
            len = strlen( buff );
            buff[ len ] = ' ';
            buff[ len + 1 ] = '\"';
            _splitpath( argv[ 0 ], drive, dir, name, NULL );
            _makepath( &buff[ len + 2 ], drive, dir, name, "inf" );
            strcat( buff, "\"" );
            // execute unsetup
            WinExec( buff, SW_SHOW );
            return( TRUE );
        }
    }
    return( FALSE );
}
#endif

bool CheckValidDisketteDrive( char *dst_str )
/***********************************/
{
#if defined( UNIX )
    return( TRUE );
#else
    if( !IsDiskette( dst_str[0] ) ) return( FALSE );
    if( dst_str[1] == '\0' ) return( TRUE );
    if( dst_str[1] != ':' ) return( FALSE );
    if( dst_str[2] == '\0' ) return( TRUE );
    if( dst_str[2] != '\\' && dst_str[2] != '/' ) return( FALSE );
    if( dst_str[3] == '\0' ) return( TRUE );
    return( FALSE );
#endif
}

bool DirParamStack( char                **inf_name,
                    char                **tmp_path,
                    DIR_PARAM_STACK_OPS function)
/*************************************************/
{
    // Not really a stack; stores only one "node"

    static char *       old_inf_name =                  NULL;
    static char *       old_tmp_path =                  NULL;

    if( function == Stack_Push ) {
        // Push values on "stack"
        old_inf_name = *inf_name;
        old_tmp_path = *tmp_path;

        *inf_name = GUIAlloc( _MAX_PATH );
        if( *inf_name == NULL ) {
            return FALSE;
        }
        *tmp_path = GUIAlloc( _MAX_PATH );
        if( *tmp_path == NULL ) {
            GUIFree( *inf_name );
            return FALSE;
        }
        return TRUE;
    } else if( function == Stack_Pop ) {
        // Pop
        GUIFree( *inf_name );
        GUIFree( *tmp_path );
        *inf_name = old_inf_name;
        *tmp_path = old_tmp_path;
        old_inf_name = NULL;
        old_tmp_path = NULL;
        return TRUE;
    } else {
        // IsEmpty
        return( old_inf_name == NULL );
    }
}

extern bool DoMainLoop( dlg_state * state )
/*****************************************/
{
    char                buff[20];
    char                *diag_list[MAX_DIAGS+1];
    char                *diags;
    char                *dstdir;
    int                 dstlen;
    bool                got_disk_sizes = FALSE;
    int                 i;
    char                newdst[_MAX_PATH];
    char                *next;
    bool                ret = FALSE;

    // initialize decompression facility
    EspeciallyUglyLittleKanjiiKludge();
    SetupTitle();
    SetupTextTable();
//    InitIO();
    // display initial dialog
    diags = GetVariableStrVal( "DialogOrder" );
    if( stricmp( diags, "" ) == 0 ) diags = "Welcome";
    i = 0;
    for( ;; ) {
        diag_list[i] = diags;
        next = strchr( diags, ',' );
        if( next == NULL ) break;
        *next = '\0';
        diags = next + 1;
        ++i;
    }
    diag_list[i+1] = NULL;
    /* process installation dialogs */

    #if defined( WINNT ) && defined( WSQL )
        if( JustDoNIDSetup ) {
            if( !GetVariableIntVal( "HasDynamo" ) ) {
                JustDoNIDSetup = FALSE;
            } else {
                return( NIDSetup() );
            }
        }
    #endif

    i = 0;
    for( ;; ) {
        if( i < 0 ) break;
        if( diag_list[i] == NULL ) {
            if( GetVariableIntVal( "MakeDisks" ) == 1 ) {
                char *dst_str = GetVariableStrVal( "MakeDiskDrive" );
                if( !CheckValidDisketteDrive( dst_str ) ) {
                    MsgBox( NULL, "IDS_INVALID_DISKETTE_SPEC", GUI_OK, dst_str );
                    #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
                        // If an error occured, do not proceed further with the install
                        if( MSBackOffice ) {
                            break;
                        }
                    #endif
                    i = 0;
                }
            } else if( GetVariableIntVal( "DoCopyFiles" ) == 1 ) {
                #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
                    if( GetVariableIntVal( "MakePackage" ) != 1  // don't check drive if making a package
                        && !CheckDrive( TRUE ) ) {
                            // If an error occured, do not proceed further with the install
                            if( MSBackOffice ) {
                                break;
                            }
                        i = 0;
                    }
                #else
                if( !CheckDrive( TRUE ) ) {
                    i = 0;
                }
                #endif
            }
            if( GetVariableByName( "SetupPath" ) != NO_VAR ) {
                ret = TRUE;
                break;
            }
            if( diag_list[i] == NULL ) {
                    StatusShow( TRUE );
                    ret = SetupOperations();
                    StatusShow( FALSE );
                    if( ret ) DoDialog( "Finished" );
                    break;
            }
        }
        if( stricmp( diag_list[i], "GetDiskSizesHere" ) == 0 ) {
            if( *state == DLG_NEXT ) {

                dstdir = GetVariableStrVal( "DstDir" );
                dstlen = strlen( dstdir );
                if( dstlen != 0 &&
                  ( dstdir[dstlen-1] == '\\' || dstdir[dstlen-1] == '/' ) ) {
                    strcpy( newdst, dstdir );
                    if( dstlen == 3 && dstdir[1] == ':' ) {
                        newdst[dstlen] = '.';
                        newdst[dstlen+1] = '\0';
                    } else {
                        newdst[dstlen-1] = '\0';
                    }
                    SetVariableByName( "DstDir", newdst );
                }
                SimSetNeedGetDiskSizes();
                ResetDiskInfo();
                got_disk_sizes = TRUE;

                #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
                    //Don't delete when making a package
                    DeleteObsoleteFiles();
                #endif
            }
        #if defined( WSQL )
        } else if( stricmp( diag_list[i], "EvalRegNumber" ) == 0 ) {
            if( *state == DLG_NEXT ) {
                EvalRegNumber();
            }
        #endif
        } else {
            *state = DoDialog( diag_list[i] );
            GUIWndDirty( NULL );
            StatusCancelled();
        }
        if( *state == DLG_CAN ) {
            if( MsgBox( NULL, "IDS_QUERYABORT", GUI_YES_NO ) == GUI_RET_YES ) {
                CancelSetup = TRUE;
                break;
            }
        } else if( *state == DLG_DONE ) {
            CancelSetup = TRUE;
            break;
        }
        if( GetVariableIntVal( "MakeDisks" ) != 0 ) {
            #if defined( WSQL )
                // Kludge - set FullInstall=true when making disks so that all
                //          conditions are true, and all files get copied to disk
                SetVariableByName( "FullInstall", "1" );
            #endif
            itoa( CountDisks( NULL ), buff, 10 );
            SetVariableByName( "NumDisksNeeded", buff );
        #if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
        } else if( got_disk_sizes
               && GetVariableIntVal( "MakePackage" ) != 1 ) { // no need to check drv w/Bkoff
        #else
        } else if( got_disk_sizes ) {
        #endif
            if( !CheckDrive( FALSE ) ) {
                break;
            }
        }
        if( *state == DLG_SAME ) {
            /* nothing */
        } else if( *state == DLG_NEXT ) {
            if( SkipDialogs ) {
                ++i;
            } else {
                for( ;; ) {
                    ++i;
                    if( diag_list[i] == NULL ) break;
                    if( CheckDialog( diag_list[i] ) ) break;
                }
            }
        } else if( *state == DLG_PREV ) {
            for( ;; ) {
                --i;
                if( i < 0 ) break;
                if( CheckDialog( diag_list[i] ) ) break;
            }
        } else if( *state == DLG_START ) {
            i = 0;
        }
    } /* for */

    return ret;
}

extern void GUImain( void )
/*************************/
{
    int                 argc = 0;
    char                **argv = NULL;
    char *              dir;
    char *              drive;
    char *              inf_name;
    char *              tmp_path;
    char *              new_inf;
    char                current_dir[ _MAX_PATH ];
    bool                ret = FALSE;
    dlg_state           state;

    GUIGetArgs( &argv, &argc );
    #if defined( __NT__ )
        if( CheckWin95Uninstall( argc, argv ) ) return;
    #endif
    #ifdef __WINDOWS__
        if( CheckForSetup32( argc, argv ) ) return;
    #endif

    // initialize paths and env. vbls.

    if( !GetDirParams( argc, argv, &inf_name, &tmp_path ) ) return;
    if( !SetupInit() ) return;
    InitIO();
    InitGlobalVarList();
    strcpy( current_dir, tmp_path );
    while( InitInfo( inf_name, tmp_path ) ) {

        ret = DoMainLoop( &state );

        if( state == DLG_DONE ) break;
//        if( CancelSetup == TRUE || !ret ) break;
        if( CancelSetup == TRUE ) break;
//        if( !ret ) break;

        // look for another SETUP.INF
        if( GetVariableByName( "SetupPath" ) == NO_VAR ) {
            if( DirParamStack( &inf_name, &tmp_path, Stack_IsEmpty ) == FALSE ) {  // "IsEmpty"?
                DirParamStack( &inf_name, &tmp_path, Stack_Pop ); // "Pop"
                CloseDownMessage( ret );
                CancelSetup = FALSE;
                ret = TRUE;
            } else {
                CloseDownMessage( ret );
                break;
            }
        } else {
            if( GetVariableIntVal( "IsMultiInstall" ) ) {
                // push current script on stack
                DirParamStack( &inf_name, &tmp_path, Stack_Push ); // "Push"
            }
            new_inf = GUIAlloc( _MAX_PATH );
            drive = GUIAlloc( _MAX_DRIVE );
            dir = GUIAlloc( _MAX_PATH );
            if( new_inf == NULL || drive == NULL || dir == NULL ) {
                GUIFree( new_inf );
                GUIFree( drive );
                GUIFree( dir );
                break;
            }
            // construct new path relative to previous
            ReplaceVars( new_inf, GetVariableStrVal( "SetupPath" ) );
            _splitpath( current_dir, drive, dir, NULL, NULL );
            _makepath( inf_name, drive, dir, new_inf, NULL );

            _splitpath( inf_name, drive, dir, NULL, NULL );
            _makepath( tmp_path, drive, dir, NULL, NULL );
//          strcpy( current_dir, tmp_path );

            GUIFree( new_inf );
            GUIFree( drive );
            GUIFree( dir );
        } /* if */

        FreeGlobalVarList( FALSE );
        FreeDefaultDialogs();
        FreeAllStructs();
        ConfigModified = FALSE;
    } /* while */

    FreeGlobalVarList( TRUE );
    FreeDefaultDialogs();
    FreeAllStructs();
    FreeDirParams( &inf_name, &tmp_path );
    CloseDownProgram();
}
