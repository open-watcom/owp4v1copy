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
* Description:  Routines to modify registry and configuration files.
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#if defined( __DOS__ ) || defined( __WINDOWS__ )
    #include <dos.h>
#endif
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#if defined( __WINDOWS__ ) || defined( __NT__ )
    #include <windows.h>
#endif
#if defined( __OS2__ )
    #define INCL_DOSMISC
    #include <os2.h>
#endif
#include "gui.h"
#include "guiutil.h"
#include "setup.h"
#include "setupinf.h"
#include "genvbl.h"
#include "gendlg.h"
#include "utils.h"

#if defined( __UNIX__ )
    #define SETENV          "export "
    #define SETENV_LEN      7
    #define BATCHEXT        ".sh"
    #define PATH_SEP        ":"
    #define PATH_SEP_CHAR   ':'
    #define PATH_VAR_AFTER  "export PATH=$PATH:%s\n"
    #define PATH_VAR_BEFORE "export PATH=%s:$PATH\n"
    #define PATH_VAR_SET    "export PATH=%s\n"
    #define SET_VAR_AFTER   SETENV "%s=$%s:%s\n"
    #define SET_VAR_BEFORE  SETENV "%s=%s:$%s\n"
    #define SET_VAR_SET     SETENV "%s=%s\n"
#else
    #define SETENV          "SET "
    #define SETENV_LEN      4
    #define PATH_SEP        ";"
    #define PATH_SEP_CHAR   ';'
    #define PATH_VAR_AFTER  "PATH %%PATH%%;%s\n"
    #define PATH_VAR_BEFORE "PATH %s;%%PATH%%\n"
    #define PATH_VAR_SET    "PATH %s\n"
    #define SET_VAR_AFTER   SETENV "%s=%%%s%%;%s\n"
    #define SET_VAR_BEFORE  SETENV "%s=%s;%%%s%%\n"
    #define SET_VAR_SET     SETENV "%s=%s\n"
    #if defined( __OS2__ )
        #define BATCHEXT    ".cmd"
    #else
        #define BATCHEXT    ".bat"
    #endif
#endif

extern int              GetOptionVarValue( vhandle var_handle, bool is_minimal );
extern bool             CheckForceDLLInstall( char *name );

extern vhandle          UnInstall;

static char             new_val[MAXENVVAR + 1];

#if defined( __NT__ )
static struct reg_location {
    HKEY    key;
    bool    key_is_open;
    bool    modify;
} RegLocation[NUM_REG_LOCATIONS];
#endif

#if !defined( __UNIX__ )

#if defined( _M_IX86 )

#if !defined( __OS2__ )
static char     *WinDotCom = NULL;
#endif
static char     OrigAutoExec[] = "?:\\AUTOEXEC.BAT";
static char     OrigConfig[] = "?:\\CONFIG.SYS";
#endif

static bool GetOldConfigFileDir( char *newauto, const char *drive_path )
/**********************************************************************/
{
    char        drive[_MAX_DRIVE];

    if( VarGetIntVal( UnInstall ) ) {
        _splitpath( drive_path, drive, NULL, NULL, NULL );
        if( drive[0] == '\0' ) {
            _fullpath( newauto, drive_path, _MAX_PATH );
            _splitpath( newauto, drive, NULL, NULL, NULL );
        }
        _makepath( newauto, drive, NULL, NULL, NULL );
    } else {
        strcpy( newauto, GetVariableStrVal( "DstDir" ) );
    }

    return( TRUE );
}

static void NoDupPaths( char *new_value, char *old_value, char delim )
/********************************************************************/
{
    char        *semi;
    char        *dup;
    char        *p;
    char        *look;
    int         len;
    char        *new_curr;

    new_curr = new_value;
    for( ;; ) {
        semi = strchr( new_curr, delim );
        if( semi != NULL ) {
            *semi = '\0';
        }
        look = old_value;
        len = strlen( new_curr );
        while( (dup = stristr( look, new_curr )) != NULL ) {
            if( dup[len] == delim ) {
                if( dup == old_value || dup[-1] == delim ) {
                    memmove( dup, dup + len + 1, strlen( dup + len + 1 ) + 1 );
                } else {
                    look = dup + len;
                }
            } else if( dup[len] == '\0' || dup[len] == '\n' ) {
                if( dup == old_value ) {
                    dup[0] = dup[len];
                    dup[1] = '\0';
                } else if( dup[-1] == delim ) {
                    memmove( dup - 1, dup + len, strlen( dup + len ) + 1 );
                } else {
                    look = dup + len;
                }
            } else {
                look = dup + len;
            }
        }
        if( semi == NULL )
            break;
        *semi = delim;
        new_curr = semi + 1;
    }
    p = strchr( old_value, '\n' );
    if( p != NULL && p[1] == '\0' )
        *p = '\0';
    if( *old_value != '\0' ) {
        len = strlen( old_value );
        if( old_value[len - 1] == delim ) {
            old_value[len - 1] = '\0';
        }
    }
}

#if defined( _M_IX86 )

static short GetBootDrive( void )
{
#ifdef __OS2__
    UCHAR           DataBuf[10];
    APIRET          rc;

    rc = DosQuerySysInfo( QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, DataBuf, sizeof( DataBuf ) );
    if( rc != 0 ) {
        printf("DosQuerySysInfo error: return code = %ld ",rc);
    }
    return( *(unsigned long *)DataBuf );
#elif defined( __DOS__ )
    union REGS  r;

    r.w.ax = 0x3305;
    intdos( &r, &r );
    return( r.h.dl );
#else
    return( 0 );
#endif
}

static char *StrNDup( char *str, size_t len )
/*******************************************/
{
    char                *new;

    new = GUIMemAlloc( len + 1 );
    if( new != NULL ) {
        memcpy( new, str, len );
        new[len] = '\0';
    }
    return( new );
}

static void cmdline_modify_var( char *buf, const char *env_var, char *env_val, append_mode append )
/*************************************************************************************************/
{
#if !( defined( __OS2__ ) || defined( __UNIX__ ) )
    if( GetVariableIntVal( "IsOS2DosBox" ) == 1 || stricmp( env_var, "PATH" ) != 0 ) {
#endif
        if( VarGetIntVal( UnInstall ) ) {
            sprintf( buf, SET_VAR_SET, env_var, env_val );
        } else if( append == AM_AFTER ) {
            sprintf( buf, SETENV "%s=%s" PATH_SEP "%s\n", env_var, env_val, new_val );
        } else if( append == AM_BEFORE ) {
            sprintf( buf, SETENV "%s=%s" PATH_SEP "%s\n", env_var, new_val, env_val );
        } else {
            sprintf( buf, SET_VAR_SET, env_var, new_val );
        }
#if !( defined( __OS2__ ) || defined( __UNIX__ ) )
    } else {    // handle PATH specially
        if( VarGetIntVal( UnInstall ) ) {
            sprintf( buf, "PATH %s" PATH_SEP "\n", env_val );
        } else if( append == AM_AFTER ) {
            sprintf( buf, "PATH %s" PATH_SEP "%s" PATH_SEP "\n", env_val, new_val );
        } else if( append == AM_BEFORE ) {
            sprintf( buf, "PATH %s" PATH_SEP "%s" PATH_SEP "\n", new_val, env_val );
        } else {
            sprintf( buf, "PATH %s" PATH_SEP "\n", new_val );
        }
    }
#endif
}

static void cmdline_create_var( char *buf, const char *env_var, append_mode append )
/**********************************************************************************/
{
#if !( defined( __OS2__ ) || defined( __UNIX__ ) )
    if( GetVariableIntVal( "IsOS2DosBox" ) == 1 || stricmp( env_var, "PATH" ) != 0 ) {
#endif
        sprintf( buf, SET_VAR_SET, env_var, new_val );
#if !( defined( __OS2__ ) || defined( __UNIX__ ) )
    } else {    // handle PATH specially
        if( append == AM_AFTER ) {
            sprintf( buf, PATH_VAR_AFTER, new_val );
        } else if( append == AM_BEFORE ) {
            sprintf( buf, PATH_VAR_BEFORE, new_val );
        } else {
            sprintf( buf, PATH_VAR_SET, new_val );
        }
    }
#endif
}

static void CheckEnvironmentLine( char *buf, int num, bool *Found )
/*****************************************************************/
{
    int                 i;
    append_mode         append;
    char                *p;
    char                *q;
    unsigned            len;
    char                *env_var;
    const char          *new_var;
    char                env_val[MAXENVVAR];
    bool                uninstall;
    bool                found;

    uninstall = VarGetIntVal( UnInstall );

    do {
        found = FALSE;
        if( memicmp( buf, SETENV, SETENV_LEN ) == 0 ) {
            p = strchr( buf, '=' );
            if( p == NULL )
                return;
            q = buf + SETENV_LEN;
            while( isspace( *q ) )
                ++q;
            len = p - q;
            while( len > 0 && isspace( q[len - 1] ) )
                --len;
            env_var = StrNDup( q, len );
            ++p;
            while( isspace( *p ) )
                ++p;
            strcpy( env_val, p );
        } else if( memicmp( buf, "PATH ", 5 ) == 0 || memicmp( buf, "PATH=", 5 ) == 0 ) {
            env_var = StrNDup( "PATH", 4 );
            strcpy( env_val, buf + 5 );
        } else {
            return;     // not an environment variable setting
        }

        for( i = 0; i < num; ++i ) {
            if( Found[i] || !uninstall && !SimCheckEnvironmentCondition( i ) )
                continue;

            append = SimGetEnvironmentStrings( i, &new_var, new_val );
            if( stricmp( env_var, new_var ) == 0 ) {
                // found an environment variable, replace its value
                NoDupPaths( new_val, env_val, PATH_SEP_CHAR );
                cmdline_modify_var( buf, env_var, env_val, append );
                Found[i] = TRUE;
                found = TRUE;
                break;
            }
        }
        GUIMemFree( env_var );
    } while( found );
}

static void FinishEnvironmentLines( FILE *fp, char *buf, int num, bool *Found )
/*****************************************************************************/
{
    int                 i;
    append_mode         append;
    const char          *new_var;

    if( VarGetIntVal( UnInstall ) )
        return;

    for( i = 0; i < num; ++i ) {
        if( Found[i] || !SimCheckEnvironmentCondition( i ) )
            continue;

        append = SimGetEnvironmentStrings( i, &new_var, new_val );
        cmdline_create_var( buf, new_var, append );
        Found[i] = TRUE;
        CheckEnvironmentLine( buf, num, Found );
        if( buf[0] != '\0' ) {
            fputs( buf, fp );
        }
    }
}


static bool ModFile( char *orig, char *new,
                     void (*func)( char *, int, bool * ),
                     void (*finish)( FILE *, char *, int, bool * ),
                     size_t num, size_t num_env )
/*****************************************************************/
{
    FILE                *fp1, *fp2;
    char                *line;
    bool                *Found;
    bool                *FoundEnv;
    char                envbuf[MAXENVVAR + 1];

    fp1 = fopen( orig, "rt" );
    if( fp1 == NULL ) {
        MsgBox( NULL, "IDS_ERROR_OPENING", GUI_OK, orig );
        return( FALSE );
    }
    fp2 = fopen( new, "wt" );
    if( fp2 == NULL ) {
        MsgBox( NULL, "IDS_ERROR_OPENING", GUI_OK, new );
        fclose( fp1 );
        return( FALSE );
    }
    // allocate array to remember variables
    if( num ) {
        Found = GUIMemAlloc( num * sizeof( bool ) );
        if( Found == NULL ) {
            return( FALSE );
        }
        memset( Found, 0, num * sizeof( bool ) );
    }
    if( num_env ) {
        FoundEnv = GUIMemAlloc( num_env * sizeof( bool ) );
        if( FoundEnv == NULL ) {
            GUIMemFree( Found );
            return( FALSE );
        }
        memset( FoundEnv, 0, num_env * sizeof( bool ) );
    }
    while( fgets( envbuf, MAXENVVAR, fp1 ) != NULL ) {
        if( strchr( envbuf, '\n' ) == 0 ) {
            strcat( envbuf, "\n" );
        }
        for( line = envbuf; isspace( *line ); ++line );
        // don't process empty lines but keep them in new file
        if( line[0] != '\0' ) {
            func( line, num, Found );
            if( num_env ) {
                CheckEnvironmentLine( line, num_env, FoundEnv );
            }
        }
        if( fputs( envbuf, fp2 ) < 0 ) {
            MsgBox( NULL, "IDS_ERROR_WRITING", GUI_OK, new );
            return( FALSE );
        }
    }
    // handle any remaining variables
    finish( fp2, envbuf, num, Found );
    if( num ) {
        GUIMemFree( Found );
    }
    if( num_env ) {
        FinishEnvironmentLines( fp2, envbuf, num_env, FoundEnv );
        GUIMemFree( FoundEnv );
    }
    fclose( fp1 );
    if( fclose( fp2 ) != 0 ) {
        MsgBox( NULL, "IDS_ERROR_CLOSING", GUI_OK, new );
        return( FALSE );
    }
    return( TRUE );
}


#ifndef __OS2__

static void CheckAutoLine( char *buf, int num, bool *Found )
/**********************************************************/
{
    int                 i;
    append_mode         append;
    char                *p;
    char                *q;
    unsigned            len;
    char                *env_var;
    char                env_val[MAXENVVAR];
    bool                uninstall;
    bool                found;
    char                fname[_MAX_FNAME];
    char                fext[_MAX_EXT];
    const char          *new_var;

    uninstall = VarGetIntVal( UnInstall );

    do {
        found = FALSE;
        if( memicmp( buf, SETENV, SETENV_LEN ) == 0 ) {
            p = strchr( buf, '=' );
            if( p == NULL )
                return;
            q = buf + SETENV_LEN;
            while( isspace( *q ) )
                ++q;
            len = p - q;
            while( len > 0 && isspace( q[len - 1] ) )
                --len;
            env_var = StrNDup( q, len );
            ++p;
            while( isspace( *p ) )
                ++p;
            strcpy( env_val, p );
        } else if( memicmp( buf, "PATH ", 5 ) == 0 || memicmp( buf, "PATH=", 5 ) == 0 ) {
            env_var = StrNDup( "PATH", 4 );
            strcpy( env_val, buf + 5 );
        } else {
            if( !uninstall ) {
                p = strchr( buf, ' ' );
                if( p != NULL )
                    *p = '\0';
                q = strchr( buf, '\n' );
                if( q != NULL )
                    *q = '\0';
                _splitpath( buf, NULL, NULL, fname, fext );
                if( p != NULL )
                    *p = ' ';
                if( q != NULL )
                    *q = '\n';
                if( stricmp( fname, "win" ) == 0 &&
                    (stricmp( fext, ".com" ) == 0 || fext[0] == '\0') ) {
                    GUIStrDup( buf, &WinDotCom );
                    buf[0] = '\0';
                }
            }
            return;     // not an environment variable setting
        }

        for( i = 0; i < num; ++i ) {
            if( Found[i] || !uninstall && !SimCheckAutoExecCondition( i ) )
                continue;

            append = SimGetAutoExecStrings( i, &new_var, new_val );
            if( stricmp( env_var, new_var ) == 0 ) {
                // found an environment variable, replace its value
                NoDupPaths( new_val, env_val, PATH_SEP_CHAR );
                cmdline_modify_var( buf, env_var, env_val, append );
                Found[i] = TRUE;
                found = TRUE;
                break;
            }
        }
        GUIMemFree( env_var );
    } while( found );
}

static void FinishAutoLines( FILE *fp, char *buf, int num, bool *Found )
/**********************************************************************/
{
    int                 i;
    append_mode         append;
    const char          *new_var;

    if( VarGetIntVal( UnInstall ) )
        return;

    for( i = 0; i < num; ++i ) {
        if( Found[i] || !SimCheckAutoExecCondition( i ) )
            continue;

        append = SimGetAutoExecStrings( i, &new_var, new_val );
        cmdline_create_var( buf, new_var, append );
        Found[i] = TRUE;
        CheckAutoLine( buf, num, Found );
        if( buf[0] != '\0' ) {
            fputs( buf, fp );
        }
    }
    if( WinDotCom != NULL ) {
        fputs( WinDotCom, fp );
        GUIMemFree( WinDotCom );
        WinDotCom = NULL;
    }
}


static bool ModAuto( char *orig, char *new )
/******************************************/
{
    int         num_auto;
    int         num_env;

    num_auto = SimNumAutoExec();
    num_env = SimNumEnvironment();
    return( ModFile( orig, new, CheckAutoLine, FinishAutoLines, num_auto, num_env ) );
}

#endif

static void CheckConfigLine( char *buf, int num, bool *Found )
/************************************************************/
{
    int                 i;
    append_mode         append;
    char                *p, *q;
    char                *cfg_var;
    char                cfg_val[MAXENVVAR];
    unsigned            len;
    bool                uninstall;
    bool                found;
    bool                have_set;
    const char          *new_var;

    uninstall = VarGetIntVal( UnInstall );

    do {
        found = FALSE;
        have_set = FALSE;
        if( memicmp( buf, SETENV, SETENV_LEN ) == 0 ) {
            have_set = TRUE;
            p = strchr( buf, '=' );
            if( p == NULL )
                return;
            q = buf + SETENV_LEN;
            while( isspace( *q ) )
                ++q;
            len = p - q;
            while( len > 0 && isspace( q[len - 1] ) )
                --len;
            cfg_var = StrNDup( q, len );
            ++p;
            while( isspace( *p ) )
                ++p;
            strcpy( cfg_val, p );
        } else {
            p = strchr( buf, '=' );
            if( p == NULL )
                return;
            q = p - 1;  // trim blanks
            while( q > buf && *q == ' ' )
                --q;
            cfg_var = StrNDup( buf, q - buf + 1 );
            ++p;
            while( *p == ' ' )
                ++p;
            strcpy( cfg_val, p );
        }

        for( i = 0; i < num; ++i ) {
            if( Found[i] || !uninstall && !SimCheckConfigCondition( i ) )
                continue;

            append = SimGetConfigStrings( i, &new_var, new_val );
            if( stricmp( cfg_var, new_var ) == 0 ) {
                // found an environment variable, replace its value
                if( isdigit( cfg_val[0] ) ) { // for files=20, buffers=30 etc
                    if( !uninstall && atoi( new_val ) > atoi( cfg_val ) ) {
                        sprintf( buf, "%s=%s\n", cfg_var, new_val );
                    }
                } else if( stricmp( cfg_var, "run" ) == 0 ) {
                    // if already there, just mark it as found
                    if( memicmp( cfg_val, new_val, strlen( new_val ) ) == 0 ) { // nyi - do splitpath
                        sprintf( buf, "%sRUN=%s\n", uninstall ? "REM " : "", new_val );
                    } else {
                        continue;
                    }
                } else {
                    NoDupPaths( new_val, cfg_val, PATH_SEP_CHAR );
                    if( uninstall ) {
                        sprintf( buf, "%s%s=%s\n", have_set ? SETENV : "", cfg_var, cfg_val );
                    } else if( append == AM_AFTER ) {
                        sprintf( buf, "%s%s=%s" PATH_SEP "%s\n", have_set ? SETENV : "", cfg_var, cfg_val, new_val );
                    } else if( append == AM_BEFORE ) {
                        sprintf( buf, "%s%s=%s" PATH_SEP "%s\n", have_set ? SETENV : "", cfg_var, new_val, cfg_val );
                    } else {
                        sprintf( buf, "%s%s=%s\n", have_set ? SETENV : "", cfg_var, new_val );
                    }
                }
                Found[i] = TRUE;
                found = TRUE;
                break;
            }
        }
        GUIMemFree( cfg_var );
    } while( found );
}

static void FinishConfigLines( FILE *fp, char *buf, int num, bool *Found )
/************************************************************************/
{
    int                 i;
    append_mode         append;
    const char          *new_var;

    if( VarGetIntVal( UnInstall ) )
        return;

    for( i = 0; i < num; ++i ) {
        if( Found[i] || !SimCheckConfigCondition( i ) )
            continue;
        append = SimGetConfigStrings( i, &new_var, new_val );
#ifndef __OS2__
        if( GetVariableIntVal( "IsOS2DosBox" ) == 1 ) {
#endif
            if( stricmp( new_var, "LIBPATH" ) == 0 ) {
                sprintf( buf, "%s=%s\n", new_var, new_val );
            } else if( stricmp( new_var, "RUN"  ) == 0 ) {
                sprintf( buf, "%s=%s\n", new_var, new_val );
            } else {
                sprintf( buf, SET_VAR_SET, new_var, new_val );
            }
#ifndef __OS2__
        } else {
            sprintf( buf, "%s=%s\n", new_var, new_val );
        }
#endif
        Found[i] = TRUE;
        CheckConfigLine( buf, num, Found );
        if( buf[0] != '\0' ) {
            fputs( buf, fp );
        }
    }
}

static bool ModConfig( char *orig, char *new )
/********************************************/
{
    int         num_cfg;
    int         num_env;

    num_cfg = SimNumConfig();
#ifdef __OS2__
    num_env = SimNumEnvironment();
#else
    num_env = 0;
#endif
    if( num_cfg == 0 && num_env == 0 ) {
         return( TRUE );
    }
    return( ModFile( orig, new, CheckConfigLine, FinishConfigLines, num_cfg, num_env ) );
}

static void ReplaceExt( char *filename, char *new_ext )
/*****************************************************/
{
    char                drive[_MAX_DRIVE];
    char                dir[_MAX_DIR];
    char                fname[_MAX_FNAME];

    _splitpath( filename, drive, dir, fname, NULL );
    _makepath( filename, drive, dir, fname, new_ext );
}


static void BackupName( char *filename )
/**************************************/
{
    char        num_buf[5];
    int         num;

    for( num = 0; num < 999; num++ ) {
        sprintf( num_buf, "%3.3d", num );
        ReplaceExt( filename, num_buf );
        if( access( filename, F_OK ) != 0 ) {
            break;
        }
    }
}

extern bool ModifyAutoExec( void )
/********************************/
{
    int                 num_auto;
    int                 num_cfg;
    int                 num_env;
    int                 boot_drive;
    int                 mod_type;
#ifndef __OS2__
    char                newauto[_MAX_PATH];
#endif
    char                newcfg[_MAX_PATH];
    FILE                *fp;

    num_auto = SimNumAutoExec();
    num_cfg = SimNumConfig();
    num_env = SimNumEnvironment();
    if( num_auto == 0 && num_cfg == 0 && num_env == 0 ) {
        return( TRUE );
    }
#ifdef __OS2__
    SetVariableByName( "AutoText", GetVariableStrVal( "IDS_MODIFY_CONFIG" ) );
#else
    SetVariableByName( "AutoText", GetVariableStrVal( "IDS_MODIFY_AUTOEXEC" ) );
#endif
    if( DoDialog( "Modify" ) == DLG_CAN ) {
        return( FALSE );
    }
    if( GetVariableIntVal( "ModNow" ) == 1 ) {
        mod_type = MOD_IN_PLACE;
    } else {
        mod_type = MOD_LATER;
    }

    if( GetVariableIntVal( "IsWin95" ) != 0 ) {
        boot_drive = 0;
    } else {
        boot_drive = GetBootDrive();
    }
    if( boot_drive == 0 ) {
#ifdef __NT__
        if( GetDriveType( "C:\\" ) == DRIVE_FIXED ) {
            OrigAutoExec[0] = 'C';   // assume C if it is a hard drive
        } else {
            // otherwise guess it is the same as the windows system directory
            const char  *sys_drv;
            sys_drv = GetVariableStrVal( "WinSystemDir" );
            boot_drive = toupper( sys_drv[0] ) - 'A' + 1;
            OrigAutoExec[0] = 'A' + boot_drive - 1;
        }
#else
        OrigAutoExec[0] = 'C';   // assume C
#endif
    } else {
        OrigAutoExec[0] = 'A' + boot_drive - 1;
    }
    OrigConfig[0] = OrigAutoExec[0];

    SetVariableByName( "FileToFind", "CONFIG.SYS" );
    while( access( OrigConfig, F_OK ) != 0 ) {
        SetVariableByName( "CfgDir", OrigConfig );
        if( DoDialog( "LocCfg" ) == DLG_CAN ) {
            MsgBox( NULL, "IDS_CANTFINDCONFIGSYS", GUI_OK );
            return( FALSE );
        }
        if( GetVariableIntVal( "CfgFileCreate" ) != 0 ) {
            fp = fopen( OrigConfig, "wt" );
            if( fp == NULL ) {
                MsgBox( NULL, "IDS_CANTCREATEFILE", GUI_OK, OrigConfig );
                return( FALSE );
            }
            fclose( fp );
        }
        strcpy( newcfg, GetVariableStrVal("CfgDir") );
        OrigConfig[0] = newcfg[0];
        OrigAutoExec[0] = OrigConfig[0];
    }
#ifndef __OS2__
    SetVariableByName( "FileToFind", "AUTOEXEC.BAT" );
    while( access( OrigAutoExec, F_OK ) != 0 ) {
        SetVariableByName( "CfgDir", OrigAutoExec );
        if( DoDialog( "LocCfg" ) == DLG_CAN ) {
            MsgBox( NULL, "IDS_CANTFINDAUTOEXEC", GUI_OK );
            return( FALSE );
        }
        if( GetVariableIntVal( "CfgFileCreate" ) != 0 ) {
            fp = fopen( OrigAutoExec, "wt" );
            if( fp == NULL ) {
                MsgBox( NULL, "IDS_CANTCREATEFILE", GUI_OK, OrigAutoExec );
                return( FALSE );
            }
            fclose( fp );
        }
        strcpy( newcfg, GetVariableStrVal("CfgDir") );
        OrigAutoExec[0] = newcfg[0];
    }
#endif

    if( mod_type == MOD_IN_PLACE ) {
        // copy current files to AUTOEXEC.BAK and CONFIG.BAK

#ifndef __OS2__
        strcpy( newauto, OrigAutoExec );
        BackupName( newauto );
#endif
        strcpy( newcfg, OrigConfig );
        BackupName( newcfg );

#ifdef __OS2__
        MsgBox( NULL, "IDS_COPYCONFIGSYS", GUI_OK, newcfg );
#else
        MsgBox( NULL, "IDS_COPYAUTOEXEC", GUI_OK, newauto, newcfg );
#endif

#ifndef __OS2__
        if( DoCopyFile( OrigAutoExec, newauto, FALSE ) != CFE_NOERROR ) {
            MsgBox( NULL, "IDS_ERRORBACKAUTO", GUI_OK );
        } else {
            if( !ModAuto( newauto, OrigAutoExec ) ) {
                return( FALSE );
            }
        }
        if( DoCopyFile( OrigConfig, newcfg, FALSE ) != CFE_NOERROR ) {
            MsgBox( NULL, "IDS_ERRORBACKCONFIG", GUI_OK );
        } else {
            if( !ModConfig( newcfg, OrigConfig ) ) {
                return( FALSE );
            }
        }
#else
        if( DoCopyFile( OrigConfig, newcfg, FALSE ) != CFE_NOERROR ) {
            MsgBox( NULL, "IDS_ERRORBACKCONFIG", GUI_OK );
        } else {
            if( !ModConfig( newcfg, OrigConfig ) ) {
                return( FALSE );
            }
            MsgBox( NULL, "IDS_OS2CONFIGSYS", GUI_OK );
        }
#endif
        // indicate config files were modified if and only if we got this far
        ConfigModified = TRUE;
    } else {
        // place modifications in AUTOEXEC.NEW and CONFIG.NEW
#ifndef __OS2__
        GetOldConfigFileDir( newauto, OrigAutoExec );
        strcat( newauto, &OrigAutoExec[2] );
#if defined(__NT__)
        ReplaceExt( newauto, "W95" );
#else
        ReplaceExt( newauto, "DOS" );
#endif
#endif
        GetOldConfigFileDir( newcfg, OrigConfig );
        strcat( newcfg, &OrigConfig[2] );
#if defined( __OS2__ )
        ReplaceExt( newcfg, "OS2" );
#elif defined( __NT__ )
        ReplaceExt( newcfg, "W95" );
#else
        ReplaceExt( newcfg, "DOS" );
#endif

#ifdef __OS2__
        MsgBox( NULL, "IDS_NEWCONFIGSYS", GUI_OK, newcfg );
        if( !ModConfig( OrigConfig, newcfg ) ) {
            return( FALSE );
        }
#else
        MsgBox( NULL, "IDS_NEWAUTOEXEC", GUI_OK, newauto, newcfg );
        if( !ModAuto( OrigAutoExec, newauto ) ) {
            return( FALSE );
        }
        if( !ModConfig( OrigConfig, newcfg ) ) {
            return( FALSE );
        }
#endif
    }
    return( TRUE );
}

#endif   // _M_IX86

#endif   // !__UNIX__

static char *ReplaceVarsInplace( char *buff, bool dorealloc )
/***********************************************************/
//  Replace occurrences of %variable% in src with the destination directory,
//  and place the result in dst.
{
    char                *p, *quest;
    char                *e;
    char                varname[128];
    const char          *varval;
    size_t              varlen;
    char                *colon;
    char                *newbuff;
    size_t              bufflen;

    p = buff;
    bufflen = strlen( buff );
    while( *p != '\0' ) {
        if( *p++ != '%' ) continue;
        if( *p == '%' ) {
            memmove( p - 1, p, strlen( p ) + 1 );
            continue;
        }
        e = strchr( p, '%' );
        if( e == NULL ) break;
        memcpy( varname, p, e - p );
        varname[e - p] = '\0';
        for( ;; ) {     // loop for multiple '?' operators
            quest = strchr( varname, '?' );
            if( quest != NULL ) {
                *quest++ = '\0';
            }
            if( stricmp( varname, "root" ) == 0 ) { // kludge?
                varval = GetVariableStrVal( "DstDir" );
            } else if( varname[0] == '@' ) {
                varval = getenv( &varname[1] );
            } else {
                varval = GetVariableStrVal( varname );
            }
            if( quest == NULL ) {
                break;  // no '?' operator
            } else {
                colon = strchr( quest, ':' );
                if( GetOptionVarValue( GetVariableByName( varname ), FALSE ) ) {
                    *colon = '\0';
                    varval = GetVariableStrVal( quest );
                    break;
                } else {
                    strcpy( varname, colon + 1 );
                    continue;
                }
            }
        }
        --p;
        if( varval != NULL ) {
            varlen = strlen( varval );
            if( dorealloc ) {
                if( varlen > e - p ) {
                    newbuff = GUIMemRealloc( buff, bufflen + varlen - (e - p) + 1 );
                    p = newbuff + (p - buff);
                    e = newbuff + (e - buff);
                    buff = newbuff;
                }
            }
            memmove( p + varlen, e + 1, strlen( e + 1 ) + 1 );
            memcpy( p, varval, varlen );
        } else {
            memmove( p, e + 1, strlen( e + 1 ) + 1 );
        }
    }
    return( buff );
}

void ReplaceVars( char *dst, const char *src )
/********************************************/
//  Replace occurrences of %variable% in src with the destination directory,
//  and place the result in dst.
{
    strcpy( dst, src );
    ReplaceVarsInplace( dst, FALSE );
}


//***************************************************************************


//***************************************************************************

/* place additional paths to search in here
   NOTE:  trailing \ is necessary as is final NULL
*/

static char *AdditionalPaths[] = { "drive:\\directory\\",
                                   /* insert paths here */
                                   NULL };

static void secondarysearch( char *filename, char *buffer )
/*********************************************************/
{
    char                drive[_MAX_DRIVE];
    char                dir[_MAX_DIR];
    char                ext[_MAX_EXT];
    char                path[_MAX_PATH];
    char                name[_MAX_PATH];
    unsigned int        counter;

    strcpy( buffer, "" );
    _splitpath( filename, NULL, NULL, name, ext );
    for( counter = 0; AdditionalPaths[counter]; counter++ ) {
        _splitpath( AdditionalPaths[counter],
                    drive,
                    dir,
                    NULL,
                    NULL );
        _makepath( path, drive, dir, name, ext );
        if( access( path, F_OK ) == 0 ) {
            strcpy( buffer, path );
            break;
        }
    }
}

static void VersionStr( int fp, char *ver, int verlen, char *verbuf )
/*******************************************************************/
{
    #define BUFF_SIZE   2048
    static char         Buffer[BUFF_SIZE];
    int                 len, size;
    char                *p;

    verbuf[0] = '\0';
    for( ;; ) {
        len = read( fp, Buffer, BUFF_SIZE );
        if( len < BUFF_SIZE ) {
            size = len;
            memset( Buffer + size, 0, BUFF_SIZE - size );
        } else {
            size = BUFF_SIZE - 256;
        }
        for( p = Buffer; p < Buffer + size; ++p ) {
            if( memcmp( p, ver, verlen ) == 0 ) {
                p += verlen;
                strcpy( verbuf, p );
                return;
            }
        }
        if( len < BUFF_SIZE ) break;    // eof
        if( lseek( fp, -256L, SEEK_CUR ) == -1 ) break;
    }
}


static void CheckVersion( char *path, char *drive, char *dir )
/************************************************************/
{
    int                 fp, len, hours;
    char                am_pm, buf[100];
    int                 check;
    struct stat         statbuf;
    struct tm           *timeptr;

    fp = open( path, O_RDONLY + O_BINARY, 0 );
    if( fp == -1 ) {
        return;     // shouldn't happen
    }

    // concat date and time to end of path
    check = fstat( fp, &statbuf );
    if( check == -1 ) {
        return;         // shouldn't happen
    }

    timeptr = gmtime( &(statbuf.st_mtime) );
    hours   = timeptr->tm_hour;
    if( hours <= 11 ) {
        am_pm = 'a';
        if( hours == 0 ) {
            hours += 12;
        }
    } else {
        am_pm = 'p';
        if( hours != 12 ) {
            hours -= 12;
        }
    }
    _splitpath( path, drive, dir, NULL, NULL );
    _makepath( path, drive, dir, NULL, NULL );
    len = strlen( path );
    sprintf( path + len, "  (%.2d-%.2d-%.4d %.2d:%.2d%cm)  ",
             timeptr->tm_mon + 1, timeptr->tm_mday, timeptr->tm_year + 1900,
             hours, timeptr->tm_min, am_pm );

    // also concat version number if it exists
    VersionStr( fp, "VeRsIoN=", 8, buf );
    if( buf[0] != '\0' ) {
        // Novell DLL
        strcat( path, buf );
    } else {
        lseek( fp, 0, SEEK_SET );
        VersionStr( fp, "FileVersion", 12, buf ); // includes '\0'
        if( buf[0] != '\0' ) {
            // Windows DLL
            strcat( path, buf );
        }
    }
    close( fp );
}

extern gui_message_return CheckInstallDLL( char *name, vhandle var_handle )
/*************************************************************************/
{
    const char          *dst;
    unsigned            dst_len;
    char                drive[_MAX_DRIVE];
    char                dir[_MAX_DIR];
    char                fname[_MAX_FNAME];
    char                ext[_MAX_EXT];
    char                unpacked_as[_MAX_PATH];
    char                dll_name[_MAX_FNAME + _MAX_EXT];
    char                path1[_MAX_PATH + 100];
    char                path2[_MAX_PATH + 100];
#if defined( __WINDOWS__ )
    OFSTRUCT            ofPrev;
    #define prev_path   ofPrev.szPathName
#else
    char                prev_path[_MAX_PATH];
#endif

    _splitpath( name, drive, dir, fname, ext );
    _makepath( dll_name, NULL, NULL, fname, ext );
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
    _makepath( unpacked_as, drive, dir, fname, "._D_" );
#else
    _makepath( unpacked_as, drive, dir, fname, ext );
#endif
#if defined( __WINDOWS__ )
    if( OpenFile( dll_name, &ofPrev, OF_EXIST ) == -1 ) {
        secondarysearch( dll_name, prev_path );
        if( prev_path[0] == '\0' ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
            if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
                rename( unpacked_as, name );
            } else {
                remove( unpacked_as );
                if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                    return( GUI_RET_CANCEL );
                }
            }
#endif
            return( GUI_RET_OK );     // did not previously exist
        }
    }
#else
    _searchenv( dll_name, "PATH", prev_path );
    if( prev_path[0] == '\0' ) {
        secondarysearch( dll_name, prev_path );
        if( prev_path[0] == '\0' ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
            if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
                rename( unpacked_as, name );
            } else {
                remove( unpacked_as );
                if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                    return( GUI_RET_CANCEL );
                }
            }
#endif
            return( GUI_RET_OK );     // did not previously exist
        }
    }
#endif

    _splitpath( name, drive, dir, NULL, NULL );
    _makepath( path1, drive, dir, NULL, NULL );
    strupr( path1 );
    _splitpath( prev_path, drive, dir, NULL, NULL );
    _makepath( path2, drive, dir, NULL, NULL );
    strupr( path2 );
    dst = GetVariableStrVal( "DstDir" );
    dst_len = strlen( dst );
    if( memicmp( path1, dst, dst_len ) == 0 && memicmp( path2, dst, dst_len ) == 0 ) {
        /* both files are going into the main installation sub-tree */
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
            rename( unpacked_as, name );
        } else {
            remove( unpacked_as );
            if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                return( GUI_RET_CANCEL );
            }
        }
#endif
        return( GUI_RET_OK );
    }
    if( stricmp( path1, path2 ) == 0 ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        /* both files are going into the same directory */
        struct stat         new, old;

        stat( prev_path, &old );
        stat( unpacked_as, &new );
        if( new.st_mtime < old.st_mtime ) {
            remove( unpacked_as );
        } else {
            if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
                rename( unpacked_as, name );
            } else {
                remove( unpacked_as );
                if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                    return( GUI_RET_CANCEL );
                }
            }
        }
        return( GUI_RET_OK );
#else
        /* there is only one file & it's been zapped */
        return( GUI_RET_OK );
#endif
    }
    if( CheckForceDLLInstall( dll_name ) ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
            rename( unpacked_as, name );
        } else {
            remove( unpacked_as );
            if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                return( GUI_RET_CANCEL );
            }
        }
#endif
        return( GUI_RET_OK );
    }

    strupr( dll_name );
    strcpy( path1, unpacked_as );
    strcpy( path2, prev_path );
    CheckVersion( path1, drive, dir );
    CheckVersion( path2, drive, dir );
    SetVariableByName( "FileDesc", dll_name );
    SetVariableByName( "DLLDir", path1 );
    SetVariableByName( "OtherDLLDir", path2 );

    // don't display the dialog if the user selected the "Skip dialog" option
    if( GetVariableIntVal( "DLL_Skip_Dialog" ) == 0 ) {
        if( DoDialog( "DLLInstall" ) == DLG_CAN ) {
            remove( unpacked_as );
            return( GUI_RET_CANCEL );
        }
    }

    if( GetVariableIntVal( "DLL_Delete_Old" ) == 1 ) {
        remove( prev_path );
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
            rename( unpacked_as, name );
        } else {
            remove( unpacked_as );
            if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                return( GUI_RET_CANCEL );
            }
        }
#endif
    } else if( GetVariableIntVal( "DLL_Keep_Both" ) == 1 ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        if( access( name, F_OK ) != 0 || remove( name ) == 0 ) {
            rename( unpacked_as, name );
        } else {
            remove( unpacked_as );
            if( MsgBox( NULL, "IDS_CANTREPLACE", GUI_YES_NO, name ) == GUI_RET_NO ) {
                return( GUI_RET_CANCEL );
            }
        }
#endif
    } else if( GetVariableIntVal( "DLL_Replace_Old" ) == 1 ) {
        DoCopyFile( unpacked_as, prev_path, FALSE );
        SetVariableByHandle( var_handle, prev_path );
        remove( unpacked_as );
    } else if( GetVariableIntVal( "DLL_Dont_Install" ) == 1 ) {
        SetVariableByHandle( var_handle, prev_path );
        remove( unpacked_as );
    } else if( GetVariableIntVal( "DLL_Abort_Install" ) == 1 ) {
        SetVariableByHandle( var_handle, prev_path );
        remove( unpacked_as );
        return( GUI_RET_CANCEL );
    }
    return( GUI_RET_OK );
}


//***************************************************************************

#if defined( __NT__ )

static bool ModEnv( int num_env )
/*******************************/
{
    BOOL                err = FALSE;
    int                 i, j, k, rc;
    append_mode         append;
    DWORD               oldvar_len, oldval_len;
    DWORD               type, old_type;
    bool                uninstall;
    char                old_var[MAXBUF];
    char                old_val[MAXENVVAR];
    char                envbuf[MAXENVVAR + 1];
    const char          *new_var;

    uninstall = VarGetIntVal( UnInstall );

    for( i = 0; i < num_env; i ++ ) {
        if( !uninstall && !SimCheckEnvironmentCondition( i ) )
            continue;

        append = SimGetEnvironmentStrings( i, &new_var, new_val );
        for( j = CURRENT_USER; j < NUM_REG_LOCATIONS; j++ ) {
            if( RegLocation[j].key_is_open == FALSE || RegLocation[j].modify == FALSE ) {
                continue;
            }
            // Look for current definition
            // we need to do this all the time, since if we are uninstalling
            // we want to get rid of existing settings
            k = 0;
            do {
                oldvar_len = MAXENVVAR;
                oldval_len = MAXENVVAR;
                rc = RegEnumValue( RegLocation[j].key, k, old_var, &oldvar_len,
                                   NULL, NULL, (LPBYTE)old_val, &oldval_len );
                if( rc != 0 )
                    break;
                ++k;
            } while( stricmp( old_var, new_var ) != 0 );
            if( rc == ERROR_NO_MORE_ITEMS ) {
                // No existing value so add it
                if( uninstall ) {
                    rc = 0;
                } else {
                    rc = RegSetValueEx( RegLocation[j].key, new_var, 0, REG_SZ, (LPCBYTE)new_val,
                                        strlen( new_val ) + 1 );
                }
            } else if( rc == 0 ) {
                NoDupPaths( new_val, old_val, PATH_SEP_CHAR );
                if( uninstall ) {
                    if( append == AM_OVERWRITE ) {
//                        envbuf[0] = '\0';
                        sprintf( envbuf, "%s", old_val );
                    } else {
                        sprintf( envbuf, "%s", old_val );
                    }
                } else if( append == AM_AFTER ) {
                    sprintf( envbuf, "%s;%s", old_val, new_val );
                } else if( append == AM_BEFORE ) {
                    sprintf( envbuf, "%s;%s", new_val, old_val );
                } else {
                    sprintf( envbuf, "%s", new_val );
                }
                if( *envbuf == '\0' ) {
                    rc = RegDeleteValue( RegLocation[j].key, new_var );
                } else {
                    rc = RegQueryValueEx( RegLocation[j].key, new_var, NULL, &old_type, NULL, NULL );
                    if( rc == 0 ) {
                        type = old_type;
                    } else {
                        type = REG_SZ;
                    }
                    rc = RegSetValueEx( RegLocation[j].key, new_var, 0, type, (LPCBYTE)envbuf,
                                        strlen( envbuf ) + 1 );
                }
            }
            if( rc != 0 ) {
                err = TRUE;
            }
        }
    }
    return( !err );
}

extern bool ModifyConfiguration( void )
/*************************************/
{
    size_t              num_env;
    int                 mod_type;
    char                changes[_MAX_PATH];
    FILE                *fp;
    int                 i, j;
    bool                bRet;
    const char          *env_var;
    const char          *new_var;
    char                buf[MAXENVVAR + 1];
    char                envbuf[MAXENVVAR + 1];
    bool                *found;
    append_mode         append;
    int                 rc;

    num_env = SimNumEnvironment();
    if( num_env == 0 ) {
        return( TRUE );
    }

    rc = RegOpenKey( HKEY_CURRENT_USER, "Environment", &RegLocation[CURRENT_USER].key );
    if( rc == 0 ) {
        RegLocation[CURRENT_USER].key_is_open = TRUE;
    } else {
        RegLocation[CURRENT_USER].key_is_open = FALSE;
    }

    rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
              "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
              0, KEY_ALL_ACCESS, &RegLocation[LOCAL_MACHINE].key );
    if( rc == 0 ) {
        RegLocation[LOCAL_MACHINE].key_is_open = TRUE;
    } else {
        RegLocation[LOCAL_MACHINE].key_is_open = FALSE;
    }

    if( RegLocation[LOCAL_MACHINE].key_is_open && !VarGetIntVal( UnInstall ) ) {
        if( DoDialog( "ModifyEnvironment" ) == DLG_CAN ) {
            return( FALSE );
        }
    } else {
        // Note we use the same dialog as for AUTOEXEC changes
        // We set the Variable AUTOTEXT to contain the proper wording
        SetVariableByName( "AutoText", GetVariableStrVal( "IDS_MODIFY_ENVIRONMENT" ) );
        if( DoDialog( "Modify" ) == DLG_CAN ) {
            return( FALSE );
        }
    }
    if( GetVariableIntVal( "ModLater" ) == 1 ) {
        mod_type = MOD_LATER;
    } else {
        mod_type = MOD_IN_PLACE;
        if( VarGetIntVal( UnInstall ) ) { //Clean up everywhere
            RegLocation[LOCAL_MACHINE].modify = TRUE;
            RegLocation[CURRENT_USER].modify  = TRUE;
        } else if( GetVariableIntVal( "ModMachine" ) == 1 ) {
            RegLocation[LOCAL_MACHINE].modify = TRUE;
            RegLocation[CURRENT_USER].modify  = FALSE;
        } else { // ModNow == 1 or ModUser == 1
            RegLocation[LOCAL_MACHINE].modify = FALSE;
            RegLocation[CURRENT_USER].modify  = TRUE;
        }
    }

    if( mod_type == MOD_IN_PLACE ) {
        bRet = ModEnv( num_env );
        // indicate config files were modified if and only if we got this far
        ConfigModified = TRUE;
    } else {  // handle MOD_LATER case
        found = GUIMemAlloc( num_env * sizeof( bool ) );
        memset( found, FALSE, num_env * sizeof( bool ) );
        GetOldConfigFileDir( changes, GetVariableStrVal( "DstDir" ) );
        strcat( changes, "\\CHANGES.ENV" );
        MsgBox( NULL, "IDS_CHANGES", GUI_OK, changes );
        fp = fopen( changes, "wt" );
        if( fp != NULL ) {
            fprintf( fp, "%s\n\n", GetVariableStrVal( "IDS_ENV_CHANGES" ) );
            for( i = 0; i < num_env; i ++ ) {
                if( found[i] || !SimCheckEnvironmentCondition( i ) )
                    continue;

                append = SimGetEnvironmentStrings( i, &new_var, new_val );
                if( append == AM_AFTER ) {
                    fprintf( fp, GetVariableStrVal( "IDS_ADD_TO_VAR" ), new_var );
                } else if( append == AM_BEFORE ) {
                    fprintf( fp, GetVariableStrVal( "IDS_ADD_TO_VAR" ), new_var );
                } else {
                    fprintf( fp, GetVariableStrVal( "IDS_SET_VAR_TO" ), new_var );
                }
                sprintf( envbuf, "%s", new_val );
                for( j = i + 1; j < num_env; ++j ) {
                    if( !SimCheckEnvironmentCondition( j ) )
                        continue;
                    append = SimGetEnvironmentStrings( j, &env_var, new_val );
                    if( stricmp( env_var, new_var ) == 0 ) {
                        found[j] = TRUE;
                        if( append == AM_AFTER ) {
                            strcpy( buf, envbuf );
                            sprintf( envbuf, "%s;%s", buf, new_val );
                        } else if( append == AM_BEFORE ) {
                            strcpy( buf, envbuf );
                            sprintf( envbuf, "%s;%s", new_val, buf );
                        } else {
                            sprintf( envbuf, "%s", new_val );
                        }
                    }
                }
                fprintf( fp, "\n    %s\n", envbuf );
            }
            fclose( fp );
        }
        GUIMemFree( found );
        bRet = TRUE;
    }

    if( RegLocation[CURRENT_USER].key_is_open ) {
        RegCloseKey( RegLocation[CURRENT_USER].key );
    }
    if( RegLocation[LOCAL_MACHINE].key_is_open ) {
        RegCloseKey( RegLocation[LOCAL_MACHINE].key );
    }

    return( bRet );
}

extern bool ModifyRegAssoc( bool uninstall )
/******************************************/
{
    HKEY    hkey;
    char    buf[256];
    char    ext[16];
    char    keyname[256];
    char    program[256];
    char    description[256];
    int     num;
    int     i;

    if( !uninstall ) {
        if( DoDialog( "ModifyAssociations" ) == DLG_CAN ) {
            return( FALSE );
        }
        if( GetVariableIntVal( "NoModEnv" ) == 1 ) {
            return( TRUE );
        }
        num = SimNumAssociations();
        for( i = 0; i < num; i++ ) {
            SimGetAssociationExt( i, ext );
            SimGetAssociationKeyName( i, keyname );
            SimGetAssociationProgram( i, program );
            SimGetAssociationDescription( i, description );
            sprintf( buf, ".%s", ext );
            RegCreateKey( HKEY_CLASSES_ROOT, buf, &hkey );
            RegSetValue( hkey, NULL, REG_SZ, keyname, strlen( keyname ) );
            RegCloseKey( hkey );
            RegCreateKey( HKEY_CLASSES_ROOT, keyname, &hkey );
            RegSetValue( hkey, NULL, REG_SZ, description, strlen( description ) );
            if( SimGetAssociationNoOpen( i ) != 1 ) {
                sprintf( buf, "%s %%1", program );
                ReplaceVarsInplace( buf, FALSE );
                RegSetValue( hkey, "shell\\open\\command", REG_SZ, buf, strlen( buf ) );
            }
            sprintf( buf, "%s,%d", program, SimGetAssociationIconIndex( i ) );
            ReplaceVarsInplace( buf, FALSE );
            RegSetValue( hkey, "DefaultIcon", REG_SZ, buf, strlen( buf ) );
            RegCloseKey( hkey );
        }
    }

    return( TRUE );
}

#endif

extern bool GenerateBatchFile( bool uninstall )
/*********************************************/
{
    size_t              num_env;
    char                batch_file[_MAX_PATH];
    FILE                *fp;
    int                 i;
    const char          *new_var;
    append_mode         append;
    char                drive[_MAX_DRIVE];
    char                dir[_MAX_DIR];
    char                fname[_MAX_FNAME];
    char                ext[_MAX_EXT];

    num_env = SimNumEnvironment();
    strcpy( batch_file, GetVariableStrVal( "BatchFileName" ) );
    ReplaceVarsInplace( batch_file, FALSE );
    _splitpath( batch_file, drive, dir, fname, ext );
    if( *ext == '\0' ) {
        strcpy( ext, BATCHEXT );
    }
    _makepath( batch_file, drive, dir, fname, ext );
    if( uninstall ) {
        remove( batch_file );
    } else {
        fp = fopen( batch_file, "wt" );
        if( fp != NULL ) {
#ifdef __UNIX__
            fprintf( fp, "#!/bin/sh\n" );
#else
            fprintf( fp, "@echo off\n" );
#endif
            fprintf( fp, "echo %s\n", GetVariableStrVal( "BatchFileCaption" ) );
            for( i = 0; i < num_env; i++ ) {
                if( !SimCheckEnvironmentCondition( i ) ) {
                    continue;
                }
                append = SimGetEnvironmentStrings( i, &new_var, new_val );
                if( append == AM_AFTER ) {
                    fprintf( fp, SET_VAR_AFTER, new_var, new_var, new_val );
                } else if( append == AM_BEFORE ) {
                    fprintf( fp, SET_VAR_BEFORE, new_var, new_val, new_var );
                } else {
                    fprintf( fp, SET_VAR_SET, new_var, new_val );
                }
            }
            fclose( fp );
        }
    }
    return( TRUE );
}
