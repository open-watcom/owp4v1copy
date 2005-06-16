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
* Description:  Setup utility functions.
*
****************************************************************************/


#if defined( __UNIX__ )
    #include <utime.h>
#else
    #include <dos.h>
    #include <direct.h>
    #include <sys/utime.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <ctype.h>
#include <setjmp.h>
#include <limits.h>
#include <process.h>
#include "gui.h"
#include "text.h"
#include "setupinf.h"
#include "setup.h"
#include "genvbl.h"
#include "gendlg.h"
#include "guiutil.h"
#include "utils.h"
#include "disksize.h"
#include "wpack.h"
#if defined( __WINDOWS__ ) || defined( __NT__ )
    #include "wpi.h"
#endif

#include "errno.h"

typedef struct def_var {
    char                *variable;
    char                *value;
    struct def_var      *link;
} DEF_VAR;

extern char     *NextToken( char *buf, char delim );
extern void     BumpStatus( long );

extern int      ReadInternal( char * );

bool            ConfigModified = FALSE;
static enum { SRC_UNKNOWN, SRC_CD, SRC_DISK } SrcInstState;

extern bool     CancelSetup;
extern vhandle  UnInstall;
extern vhandle  PreviousInstall;
int             SkipDialogs;
char            *VariablesFile;
DEF_VAR         *ExtraVariables;
int             Invisible;

#ifdef PATCH
extern int      InitIO( void );
extern int      Decode( arccmd *);

static int      DecodeError;
extern int      IsPatch;
#endif

extern bool ModifyEnvironment( bool uninstall )
/*********************************************/
{
    bool                ret;

    ret = TRUE;
#ifdef _UI
    uninstall = uninstall;
#else
    ret = CreatePMInfo( uninstall );
    if( !ret ) {                   // create folder and icons
        gui_message_return  gui_ret;
        if( VarGetIntVal( UnInstall ) != 0 ) {
        gui_ret = MsgBox( NULL, "IDS_PMREMNOGOOD", GUI_YES_NO );
        } else {
        gui_ret = MsgBox( NULL, "IDS_PMADDNOGOOD", GUI_YES_NO );
        }
        if( gui_ret == GUI_RET_YES ) {
        ret = TRUE;
        }
    }
#endif
    return( ret );
}

extern bool ModifyStartup( bool uninstall )
/*****************************************/
{
    bool                ret;

#if !defined( _UI )
    WriteProfileStrings( uninstall );  // will write to the win.ini file.
#endif

//    if( !uninstall ) {
#if defined( __NT__ )
#ifndef __AXP__
    if( GetVariableIntVal( "IsWin95" ) != 0 ) {
        ret = ModifyAutoExec();
    } else {
#endif
        ret = ModifyConfiguration();
#ifndef __AXP__
    }
#endif
#else
    ret = ModifyAutoExec();
#endif
//    }
    return( ret );
}

typedef struct {
    unsigned long       free_space;
    unsigned            cluster_size;
    unsigned            use_target_for_tmp_file : 1;
    unsigned            fixed : 1;
    unsigned            diskette : 1;
} drive_info;

static drive_info       Drives[32];

#if defined( __DOS__ )

int __far critical_error_handler( unsigned deverr,
                  unsigned errcode,
                  unsigned far *devhdr )
{
    deverr=deverr;errcode=errcode;devhdr=devhdr;
    return( _HARDERR_FAIL );
}

#endif

typedef __far (HANDLER)( unsigned deverr,
                  unsigned errcode,
                  unsigned far *devhdr );
static void NoHardErrors()
{
#if defined( __OS2__ )
    DosError( FERR_DISABLEHARDERR );
#elif defined( __DOS__ )
    _harderr( (HANDLER *)critical_error_handler );
#elif defined( __WINDOWS__ ) || defined( __NT__ )
    SetErrorMode( SEM_FAILCRITICALERRORS );
#endif
}

#ifdef __NT__
bool NTSpawnWait( char *cmd, DWORD *exit_code, HANDLE in, HANDLE out, HANDLE err )
{
    STARTUPINFO             start;
    PROCESS_INFORMATION      info;

    memset( &start, 0, sizeof( start ) );
    start.cb = sizeof( start );
    start.hStdInput = in;
    start.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    start.hStdError = GetStdHandle( STD_ERROR_HANDLE );
    if( in || out || err ) {
        start.dwFlags = STARTF_USESTDHANDLES;
    }
    if( cmd[ 0 ] == '-' ) {
        cmd++;
        start.wShowWindow = FALSE;
        start.dwFlags |= STARTF_USESHOWWINDOW;
    }
    if( !CreateProcess( NULL, cmd, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP+CREATE_NEW_CONSOLE,
            NULL, NULL, &start, &info ) ) {
        return( FALSE );
    } else {
        WaitForSingleObject( info.hProcess, INFINITE );
        while( !GetExitCodeProcess( info.hProcess, exit_code ) ||
            *exit_code == STILL_ACTIVE ) {
            if( StatusCancelled() )
                return( FALSE );
        }
    }
    CloseHandle( info.hProcess );
    CloseHandle( info.hThread );
    return( TRUE );
}
#endif

#ifdef __WINDOWS__
#include "toolhelp.h"
static bool WinSpawnWait( char *cmd )
{
    struct {
        WORD        segEnv;
        LPCSTR      lpzCmdLine;
        UINT FAR *lpShow;
        UINT FAR *lpReserved;
    } parm;
    UINT        show[2];
    HINSTANCE   inst;
    HINSTANCE   toolhelp;
    BOOL WINAPI (*taskfirst)(TASKENTRY FAR *);
    BOOL WINAPI (*tasknext)(TASKENTRY FAR *);
    TASKENTRY   task;
    BOOL        stillthere;
    BOOL        ok;
    char        buff[_MAX_PATH];

    show[0] = 2;
    show[1] = SW_SHOW;
    memset( &parm, 0, sizeof( parm ) );
    parm.lpzCmdLine = "";
    parm.lpShow = show;
    inst = LoadModule( cmd, &parm );
    if( inst < HINSTANCE_ERROR ) return( FALSE );
    ReplaceVars( buff, "%ToolHelp%" );
    if( buff[0] == '\0' ) return( FALSE );
    toolhelp = LoadModule( buff, &parm );
    if( toolhelp < HINSTANCE_ERROR ) return( FALSE );
    taskfirst = (BOOL WINAPI(*)(TASKENTRY FAR*))GetProcAddress( toolhelp, "TaskFirst" );
    if( taskfirst == NULL ) return( FALSE );
    tasknext = (BOOL WINAPI(*)(TASKENTRY FAR*))GetProcAddress( toolhelp, "TaskNext" );
    if( tasknext == NULL ) return( FALSE );
    task.dwSize = sizeof( task );
    do {
        stillthere = FALSE;
        ok = taskfirst( &task );
        while( ok ) {
            if( task.hInst == inst )
                stillthere = TRUE;
            ok = tasknext( &task );
            StatusCancelled(); // yield
        }
    } while( stillthere );
    return( TRUE );
}
#endif

#ifdef __OS2__
static bool OS2SpawnWait( char *cmd, int *rc )
{
    RESULTCODES res;
    PID     dummy;
    char    *p;

    p = strchr( cmd, ' ' );
    if( p != NULL )
        *p = '\0';
    if( DosExecPgm( NULL, 0, EXEC_ASYNCRESULT, cmd, NULL, &res, cmd ) != 0 )
        return( FALSE );

    if( p != NULL )
        *p = ' ';
    for( ;; ) {
        *rc = DosWaitChild( DCWA_PROCESS, DCWW_NOWAIT, &res,
                    &dummy, res.codeTerminate );
        if( *rc != ERROR_CHILD_NOT_COMPLETE ) {
            *rc = res.codeResult;
            break;
        }
        StatusCancelled();
    }
    return( TRUE );
}
#endif

static bool DoSpawnCmd( char *cmd )
{
    bool        rc;

#if defined( __WINDOWS__ )
    {
    rc = WinSpawnWait( cmd );
    }
#elif defined( __NT__ )
    {
    DWORD exit_code;
    if( NTSpawnWait( cmd, &exit_code, 0, 0, 0 ) ) {
        rc = exit_code == 0;
    } else {
        rc = FALSE;
    }
    }
#elif defined( __OS2__ )
    {
    int     code;
    OS2SpawnWait( cmd, &code );
    rc = code == 0;
    }
#elif defined( _UI )
    {
    system( cmd );
    rc = TRUE;
    }
#endif
    GUIWndDirty( NULL );
    return( rc );
}


void DoSpawn( when_time when )
{
    char buff[2*_MAX_PATH];
    int         i, num_spawns;

    num_spawns = SimNumSpawns();
    for( i = 0; i < num_spawns; ++i ) {
        if( when != SimWhen( i ) )
            continue;
        if( !SimEvalSpawnCondition( i ) )
            continue;
        SimGetSpawnCommand( buff, i );
        if( buff[0] == '\0' )
            continue;
        DoSpawnCmd( buff );
    }
}


#define TMPFILENAME "_watcom_.tmp"
static void GetTmpFileName( char drive, char *buff )
{
    buff[0] = drive;
    strcpy( buff+1, ":\\" TMPFILENAME );
}

#if defined( WINNT ) || defined( WIN )
static void GetTmpFileNameUNC( char *path, char *buff )
{
    if( path == NULL || buff == NULL ) {
        return;
    }
    if( path[ 0 ] == '\\' && path[ 1 ] == '\\' ) {
        strcpy( buff, path );
        if( buff[ strlen( buff ) - 1 ] != '\\' ) {
            strcat( buff, "\\" );
        }
        strcat( buff, TMPFILENAME );
    } else {
        buff[0] = *path;
        strcpy( buff+1, ":\\" TMPFILENAME );
    }
}
#endif

static void GetTmpFileNameInTarget( unsigned drive, char *buff )
{
    int         i;
    int         max_targets = SimNumTargets();
    int         len;

    for( i = 0; i < max_targets; ++i ) {
        SimTargetDir( i, buff );
        if( tolower( buff[0] ) == tolower( drive ) && buff[1] == ':' ) {
            len = strlen( buff );
            if( len != 0 && buff[len-1] != '\\' ) {
                strcat( buff, "\\" );
            }
            strcat( buff, TMPFILENAME );
            return;
        }
    }
    GetTmpFileName( drive, buff );
}

void ResetDriveInfo()
{
    int         i;

    for( i = 0; i < sizeof( Drives ) / sizeof( Drives[0] ); ++i ) {
        Drives[ i ].cluster_size = 0;
    }
}

static int GetDriveNum( int drive )
{
    drive = toupper( drive ) - 'A' + 1;
    if( drive < 1 || drive > 26 ) drive = 0;
        return( drive );
}

static unsigned GetDriveInfo( int drive, bool removable )
{
    drive_info  *info;
    char        drive_char = drive;

    drive = GetDriveNum( drive );
    info = &Drives[drive];
    if( ( info->cluster_size == 0 || removable /* recheck - could have been replaced */ ) ) {
        memset( info, 0, sizeof( *info ) );
    if( drive == 0 ) return( 0 );
        NoHardErrors();
#if defined( __OS2__ )
    {
        typedef struct {
            BYTE cmdinfo;
            BYTE drive;
        } parm;

        typedef struct {
            BYTE bpb[31];
            short cyl;
            BYTE type;
            short attrs;
        } ret;

        #define         BUF_SIZE 40

        UCHAR           fsinfobuf[BUF_SIZE];
        APIRET          rc;

        ULONG           sectors_per_cluster;      /* sectors per allocation unit */
        ULONG           free_clusters;      /* available units */
        ULONG           bytes_per_sector;       /* bytes per sector */
        parm            p;
        ret             r;
        ULONG           parmLengthInOut;
        ULONG           dataLengthInOut;
        char            dev[3];
        struct {
            FSQBUFFER2  b;
            char        stuff[100];
        } dataBuffer;
        ULONG           dataBufferLen;

        rc = DosQueryFSInfo( (ULONG)drive, FSIL_ALLOC, fsinfobuf, BUF_SIZE );

        if( rc == 0 ) {
            /* This is a bit strange: the respective values are not */
            /* returned in a structure but in an array BLAH! from   */
            /* which one must extract info as below. See OS/2 Manual*/
            /* for clarification.                                   */
            sectors_per_cluster =  *(ULONG *)(fsinfobuf + (1*sizeof(ULONG)));
            free_clusters =  *(ULONG *)(fsinfobuf + (3*sizeof(ULONG)));
            bytes_per_sector  =  *(USHORT *)(fsinfobuf + (4*sizeof(ULONG)));
            info->cluster_size = sectors_per_cluster * bytes_per_sector;
            info->free_space = free_clusters * (ULONG)info->cluster_size;
        } else {
            info->free_space = -1;
        }
        info->fixed = FALSE;
        info->diskette = FALSE;
        p.cmdinfo = 0;
        p.drive = drive - 1;
        parmLengthInOut = sizeof( p );
        dataLengthInOut = sizeof( r );
        if( DosDevIOCtl(-1, 8, 0x63, &p,
            sizeof( p ), &parmLengthInOut, &r,
            sizeof( r ), &dataLengthInOut) == 0 ) {
            dev[0] = drive_char;
            dev[1] = ':';
            dev[2] = '\0';
            dataBufferLen = sizeof( dataBuffer );
            rc = DosQueryFSAttach( dev, 0, FSAIL_QUERYNAME, (PFSQBUFFER2)&dataBuffer, &dataBufferLen );
            if( rc == 0 ) {
                if( dataBuffer.b.iType == FSAT_LOCALDRV ) {
                    switch( r.type ) {
                    case 5: // fixed disk
                        info->fixed = TRUE;
                        break;
                    case 6: // tape
                        break;
                    default: // diskette
                        info->diskette = TRUE;
                        break;
                    }
                }
            } else if( rc == ERROR_NOT_READY ) {
                info->diskette = TRUE; // diskette not in drive
            }
        }
    }
#elif defined(__NT__)
    {
        char        root[4];
        DWORD       sectors_per_cluster;
        DWORD       bytes_per_sector;
        DWORD       free_clusters;
        DWORD       total_clusters;
        UINT        drive_type;

        root[0] = drive_char;
        strcpy( &root[1], ":\\" );
        if( GetDiskFreeSpace( root, &sectors_per_cluster, &bytes_per_sector,
                   &free_clusters, &total_clusters ) ) {
            info->cluster_size = bytes_per_sector * sectors_per_cluster;
            info->free_space = free_clusters * (DWORD)info->cluster_size;
        } else {
            info->free_space = -1;
        }
        drive_type = GetDriveType( root );
        info->diskette = drive_type == DRIVE_REMOVABLE;
        info->fixed = drive_type == DRIVE_FIXED;
    }
#else
    {
        extern long diskette( int );
        extern long remote( int );
        extern long drivemap( int );
        #ifdef __386__
        #pragma aux diskette parm [ bl ] value [ eax ] modify [ edx ] = \
            "mov ax,4408h" \
            "int 21h" \
            "sbb dx,dx" \
            "rol eax,16" \
            "mov ax,dx" \
            "rol eax,16"

        #pragma aux remote parm [ bl ] value [ eax ] modify [ edx ] = \
            "mov ax,4409h" \
            "int 21h" \
            "sbb dx,dx" \
            "shl eax,16" \
            "mov ax,dx"

        #pragma aux drivemap parm [ bl ] value [ eax ] modify [ edx ] = \
            "mov ax,440eh" \
            "int 21h" \
            "sbb dx,dx" \
            "rol eax,16" \
            "mov ax,dx" \
            "rol eax,16"

        #else
        #pragma aux diskette parm [ bl ] value [ dx ax ] = \
            "mov ax,4408h" \
            "int 21h" \
            "sbb dx,dx"

        #pragma aux remote parm [ bl ] value [ dx ax ] = \
            "mov ax,4409h" \
            "int 21h" \
            "sbb ax,ax"

        #pragma aux drivemap parm [ bl ] value [ dx ax ] = \
            "mov ax,440eh" \
            "int 21h" \
            "sbb dx,dx"

        #endif

        struct diskfree_t   FreeSpace;
        long        rc;

        info->diskette = FALSE;
        info->fixed = FALSE;
        info->cluster_size = 0;
        info->free_space = -1;
        rc = drivemap( drive );
        if( rc >= 0 && ( rc & 0xFF ) != drive && ( rc & 0xFF ) != 0 ) {
            return( drive ); // mapped drive not there!
        }
        info->fixed = TRUE;
        rc = remote( drive );
        if( ( rc & 0xFFFF ) != 0 ) {
            info->fixed = 0; // if remote returns error, it's a bad drive
        }
        rc = diskette( drive );
        if( rc >= 0 ) {
            info->diskette = ( rc & 0xFFFF ) == 0;
            if( info->diskette ) info->fixed = 0;
        } else {
            info->fixed = 0;
        }
        if( _dos_getdiskfree( drive, &FreeSpace ) == 0 ) {
            info->cluster_size = FreeSpace.sectors_per_cluster
                        * FreeSpace.bytes_per_sector;
            info->free_space = FreeSpace.avail_clusters * (long)info->cluster_size;
        } else if( removable ) { // diskette not present
            info->cluster_size = 0;
            info->free_space = -1;
        } else {
            /* doesn't work on network drive - assume 4K cluster, max free */
            info->cluster_size = 4096;
            info->free_space = LONG_MAX;
        }
    }
#endif
    if( !removable ) {
        int         io;
        char        path[_MAX_PATH];

        GetTmpFileNameInTarget( drive_char, path );
        io = open( path, O_RDWR+O_CREAT+O_TRUNC, S_IREAD+S_IWRITE );
        if( io == -1 ) {
            GetTmpFileName( drive_char, path );
            io = open( path, O_RDWR+O_CREAT+O_TRUNC, S_IREAD+S_IWRITE );
            info->use_target_for_tmp_file = FALSE;
        } else {
            info->use_target_for_tmp_file = TRUE;
        }
        if( io != -1 ) {
            close( io );
            remove( path );
        } else {
            info->cluster_size = 1;
            info->free_space = -1;
        }
    }
    }
    return( drive );
}

extern unsigned long GetFreeDiskSpace( unsigned drive, bool removable )
/*********************************************************************/
{
    return( Drives[ GetDriveInfo( drive, removable ) ].free_space );
}

void ResetDiskInfo()
/******************/
{
    memset( Drives, 0, sizeof( Drives ) );
}

bool IsFixedDisk( unsigned drive )
/************************************/
{
    if( drive == 0 )
        return( FALSE );
    // don't bang diskette every time!
    if( Drives[ GetDriveNum( drive ) ].diskette )
        return( FALSE );
    return( Drives[ GetDriveInfo( drive, FALSE ) ].fixed != 0 );
}

unsigned GetClusterSize( unsigned drive )
/***************************************/
{
    if( drive == 0 )
        return( 1 );
    return( Drives[ GetDriveInfo( drive, FALSE ) ].cluster_size );
}

#if defined( WINNT ) || defined( WIN )
// The 3 functions: GetRootFromPath(), FreeSpace() and ClusterSize() were originally in
// bkoffice.c, but now they are being used by utils.c and setupinf.c to support UNC naming for
// regular installs.
extern bool GetRootFromPath( char * root, char * path )
/*****************************************************/
{
    char        *index;
    char        drive[ _MAX_PATH ];
    int         i;
    if( path == NULL || root == NULL ) {
    return( FALSE );
    }

    if( isalpha( path[ 0 ] ) != 0 && path[ 1 ] == ':' ) {
        // turn a path like "c:\dir" into "c:\"
        root[ 0 ] = path [ 0 ];
        root[ 1 ] = ':';
        root[ 2 ] = '\\';
        root[ 3 ] = '\0';
        return( TRUE );
    } else if( path[ 0 ] == '\\' && path[ 1 ] == '\\' ) {
        // turn a UNC name like "\\root\share\dir\subdir" into "\\root\share\"
        strcpy( root, path );
        i = 0;
        if( root[ strlen( root ) - 1 ] != '\\' ) {
            strcat( root, "\\" );
        }
        index = root;
        while( *index != '\0' ) {
            if( *index == '\\' ) {
                i++;
            }
            index++;
            if( i == 4 ) { // cut off string at character after 4th backslash
                *index = '\0';
                return( TRUE );
            }
        }
        return( FALSE );  // invalid UNC name such as: "\\missingshare\"
    } else {
        // for relative paths like "\dir" use the current drive.
        if( getcwd( drive, sizeof( drive ) ) == NULL ) {
            return( FALSE );
        }
        _splitpath( drive, drive, NULL, NULL, NULL );
        strcat( drive, "\\" );
        return( TRUE );
    }
}

extern long FreeSpace( char * path )
/**********************************/
{
#ifdef WINNT
    DWORD       sectors_per_cluster;
    DWORD       bytes_per_sector;
    DWORD       avail_clusters;
    DWORD       total_clusters;
    char        root[ _MAX_PATH ];

    if( GetRootFromPath( root, path ) ) {
        if( GetDiskFreeSpace( root, &sectors_per_cluster,
                      &bytes_per_sector, &avail_clusters,
                      &total_clusters ) ) {
            return( sectors_per_cluster * bytes_per_sector * avail_clusters );
        }
    }
#else
    struct diskfree_t info;
    if( isalpha( *path ) != 0 ) {
        if( _getdiskfree( toupper( *path ) - 'A' + 1, &info ) == 0 ) {
            return( (long) info.sectors_per_cluster * info.bytes_per_sector * info.avail_clusters );
        }
    }
#endif
    return 0;
}

extern long ClusterSize( char * path )
/************************************/
{
#ifdef WINNT
    DWORD       sectors_per_cluster;
    DWORD       bytes_per_sector;
    DWORD       avail_clusters;
    DWORD       total_of_clusters;
    char        root[ _MAX_PATH ];

    if( GetRootFromPath( root, path ) ) {
        if( GetDiskFreeSpace( root, &sectors_per_cluster,
                      &bytes_per_sector, &avail_clusters,
                      &total_of_clusters ) ) {
            return( sectors_per_cluster * bytes_per_sector );
        }
    }

#else
    struct diskfree_t info;
    if( isalpha( *path ) != 0 ) {
        if( _getdiskfree( toupper( *path ) - 'A' + 1, &info ) == 0 ) {
            return( (long) info.sectors_per_cluster * info.bytes_per_sector );
        }
    }
#endif
    return 0;
}

bool IsDriveWritable( char *path )
/********************************/
{
    int         io;
    char        tempfile[_MAX_PATH];
    char        root[_MAX_PATH];

    if( path == NULL ) {
        return( FALSE );
    }

    if( !GetRootFromPath( root, path ) ) {
        return( FALSE );
    }

    GetTmpFileNameUNC( root, tempfile );

    io = open( tempfile, O_RDWR+O_CREAT+O_TRUNC, S_IREAD+S_IWRITE );
    if( io == -1 ) {
        return( FALSE );
    } else {
        close( io );
        remove( tempfile );
        return( TRUE );
    }
}
#endif

bool DriveInfoIsAvailable( char *path )
/*************************************/
{
#ifdef WINNT
    DWORD       sectors_per_cluster;
    DWORD       bytes_per_sector;
    DWORD       avail_clusters;
    DWORD       total_clusters;
    char        root[ _MAX_PATH ];

    if( GetRootFromPath( root, path ) ) {
        if( GetDiskFreeSpace( root, &sectors_per_cluster,
                      &bytes_per_sector, &avail_clusters,
                      &total_clusters ) ) {
            return( TRUE );
        }
    }
#else
    struct diskfree_t info;

    if( isalpha( *path ) != 0 ) {
        if( _getdiskfree( toupper( *path ) - 'A' + 1, &info ) == 0 ) {
            return( TRUE );
        }
    }
#endif
    return( FALSE );
}

// ********** Functions for Creating the destination directory tree **********


static void RemoveDstDir( int dir_index, char *dst_dir )
/******************************************************/
{
    int         child;
    int         max_dirs = SimNumDirs();

    SimDirNoSlash( dir_index, dst_dir );
    if( access( dst_dir, F_OK ) != 0 )
        return;
    for( child = 0; child < max_dirs; ++child ) {
        if( SimDirParent( child ) == dir_index ) {
            RemoveDstDir( child, dst_dir );
        }
    }
    if( SimDirParent( dir_index ) == -1 ) {
//        return; // leave root dir (for config.new, etc)
    }
    SimDirNoSlash( dir_index, dst_dir );
    rmdir( dst_dir );
}


void MakeParentDir( char *dir, char *drive, char *path )
{
    char                *parent,*end;
    int                 path_len;

    _splitpath( dir, drive, path, NULL, NULL );
    path_len = strlen( path );
    end = path + path_len - 1;
    if( path_len == 0 )
        return;
    if( *end == '\\' || *end == '/' )
        *end = '\0';
    if( path[0] == '\0' )
        return;
    parent = alloca( strlen( drive ) + path_len + 10 ); // lotsa room
    strcpy( parent, drive );
    strcat( parent, path );
    MakeParentDir( parent, drive, path );
    mkdir( parent );
}


extern bool CreateDstDir( int i, char *dst_dir )
/**********************************************/
// check for directory exitstance.  If dir exists return TRUE.
// Else try and create directory.
{
    bool                ok;
    int                 parent;
    char                drive[_MAX_DRIVE];
    char                path[_MAX_PATH];

    parent = SimDirParent( i );
    if( parent != -1 ) {
        ok = CreateDstDir( parent, dst_dir );
        if( !ok ) return( FALSE );
    }
    SimDirNoSlash( i, dst_dir );
    if( access( dst_dir, F_OK ) == 0 ) return( TRUE );          // check for existance
        MakeParentDir( dst_dir, drive, path );
    if( mkdir( dst_dir ) == 0 ) return( TRUE );
        MsgBox( NULL, "IDS_CANTMAKEDIR", GUI_OK, dst_dir );
    return( FALSE );
}

static void catnum( char *buff, long num )
{

    char        num_buff[128];
    char        ch;

    ch = ' ';
    if( num < 0 ) {
        num = -num;
        ch = '-';
    }
    if( num < 1000 ) {
        sprintf( num_buff, "%c%d bytes", ch, (int)num );
    } else {
        num /= 1000;
        if( num > 1000000 ) {
            sprintf( num_buff, "%c%d,%3.3d,%3.3dK", ch, (int)((num/1000000)%1000), (int)((num/1000)%1000), (int)(num%1000) );
        } else if( num > 1000 ) {
            sprintf( num_buff, "%c%d,%3.3dK", ch, (int)((num/1000)%1000), (int)(num%1000) );
        } else {
            sprintf( num_buff, "%c%dK", ch, (int)num );
        }
    }
    strcat( buff, num_buff );
}

static void ucatnum( char *buff, unsigned long num )
{

    char        num_buff[128];
    char        ch;

    ch = ' ';
    if( num < 1000 ) {
        sprintf( num_buff, "%c%u bytes", ch, (int)num );
    } else {
        num /= 1000;
        if( num > 1000000 ) {
            sprintf( num_buff, "%c%u,%3.3u,%3.3uK", ch, (int)((num/1000000)%1000), (int)((num/1000)%1000), (int)(num%1000) );
        } else if( num > 1000 ) {
            sprintf( num_buff, "%c%u,%3.3uK", ch, (int)((num/1000)%1000), (int)(num%1000) );
        } else {
            sprintf( num_buff, "%c%uK", ch, (int)num );
        }
    }
    strcat( buff, num_buff );
}

static bool MatchEnd( char *path, char *end )
/*******************************************/
{
    int plen, endlen;

    plen = strlen( path );
    endlen = strlen( end );
    if( endlen > plen )
        return( FALSE );
    return( stricmp( path + plen - endlen, end ) == 0 );
}

static bool FindUpgradeFile( char *path )
/***************************************/
{
    DIR                 *d;
    struct dirent       *info;
    char                *path_end;
    int                 upgrades = SimNumUpgrades();
    int                 i;

    StatusAmount( 0,1 );
    StatusLines( STAT_CHECKING, path );
    if( StatusCancelled() )
        return( FALSE );
    d = opendir( path );
    path_end = path+strlen(path);
    if( path_end > path && path_end[-1] == '\\' )
        --path_end;
    *path_end = '\\';
    while( info = readdir( d ) ) {
        strcpy( path_end+1, info->d_name );
        if( info->d_attr & _A_SUBDIR ) {
            if( info->d_name[0] != '.' ) {
                if( FindUpgradeFile( path ) )
                    return( TRUE );
            }
        } else {
            for( i = 0; i < upgrades; ++i ) {
                if( MatchEnd( path, SimGetUpgradeName( i ) ) )
                    return( TRUE );
            }
        }
    }
    *path_end = '\0';
    closedir( d );
    return( FALSE );
}

extern bool CheckUpgrade()
/************************/
{
    char                disk[_MAX_PATH];
    dlg_state           return_state;

    if( GetVariableIntVal( "PreviousInstall" ) ) return( TRUE );
    DoDialog( "UpgradeStart" );
    for( disk[0] = 'c'; disk[0] <= 'z'; disk[0]++ ) {
        if( !IsFixedDisk( disk[0] ) )
            continue;
        strcpy( disk+1, ":\\" );
        StatusCancelled();
        if( FindUpgradeFile( disk ) )
            return( TRUE );
    }
    return_state = DoDialog( "UpgradeNotQualified" );
    return( return_state != DLG_CAN && return_state != DLG_DONE );
}


extern bool CheckDrive( bool issue_message )
/******************************************/
//check if there is enough disk space
{
    bool                ret;
    unsigned long       free_disk_space;
    long                disk_space_needed;
    unsigned long       max_tmp_file;
    int                 max_targs;
    int                 i, j, targ_num;
    char                *disks[ MAX_DRIVES ];
    bool                disk_counted[ MAX_DRIVES ];
    char                disk;
    gui_message_return  reply;
    char                buff[_MAX_PATH];
    char                drive[20];
    struct {
        char            *drive;
        long            needed;
        unsigned long   max_tmp;
        unsigned long   free;
        int             num_files;
    } space[ MAX_DRIVES ];
    char                root[ 2 ][ _MAX_PATH ];


    for( i = 0; i < MAX_DRIVES; i++ ) {
        space[i].drive = GUIMemAlloc( _MAX_PATH );
        if( space[ i ].drive == NULL ) {
            return( FALSE );
        }
        *space[i].drive = 0;
    }
    ret = TRUE;
    if( !SimCalcTargetSpaceNeeded() )
        return( FALSE );
    max_targs = SimNumTargets();
    for( i = 0; i < max_targs; i++ ) {
        // get drive letter for each target (actually the path including the drive letter)
        disks[ i ] = SimGetDriveLetter( i );
        if( disks[ i ][ strlen( disks[ i ] ) - 1 ] != '\\' ) {
            strcat( disks[ i ], "\\" );
        }
        if( disks[ i ] == NULL ) {
            return( FALSE );
        } else {
            disk_counted[ i ] = FALSE;
        }
    }
    // check for enough disk space, combine drives that are the same
    for( i = 0; i < max_targs; i++ ) {
        if( !disk_counted[ i ] ) {
            targ_num = i;
            disk_space_needed = SimTargetSpaceNeeded( i );
            max_tmp_file = SimMaxTmpFile( i );
            for( j = i + 1; j < max_targs; ++j ) {
                if( ( tolower( *disks[ j ] ) == tolower( *disks[ i ] )
                    && isalpha( *disks[ i ] ) )
                    || stricmp( root[ 0 ], root[ 1 ] ) == 0 ) {
                    targ_num = j;
                    disk_space_needed += SimTargetSpaceNeeded( j );
                    if( SimMaxTmpFile( j ) > max_tmp_file ) {
                        max_tmp_file = SimMaxTmpFile( j );
                    }
                    disk_counted[ j ] = TRUE;
                }
            }
            free_disk_space = GetFreeDiskSpace( *disks[i], FALSE );
            if( free_disk_space == (unsigned long)-1 )
                free_disk_space = 0;
            space[i].drive = disks[i];
            space[i].free = free_disk_space;
            space[i].needed = disk_space_needed;
            space[i].max_tmp = max_tmp_file;
            space[i].num_files = SimGetTargNumFiles( targ_num );
            if( disk_space_needed > 0 && free_disk_space < disk_space_needed + max_tmp_file ) {
                for( disk = 'c'; disk <= 'z'; ++disk ) {
                    if( disk == tolower( *disks[ i ] ) )
                        continue;
                    if( !IsFixedDisk( disk ) )
                        continue;
                    if( GetFreeDiskSpace( disk, FALSE ) > max_tmp_file ) {
                        SimSetTargTempDisk( i, disk );
                        for( j = i + 1; j < max_targs; ++j ) {
                            if( tolower( *disks[ j ] ) == *disks[ i ] ) {
                                SimSetTargTempDisk( j, disk );
                            }
                        }
                        break;
                    }
                    if( disk == 'z' && issue_message ) {
                        MsgBox( NULL, "IDS_NOTEMPSPACE", GUI_OK, max_tmp_file/1000 );
                        return( FALSE );
                    }
                }
            }
            if( issue_message ) {
                if( disk_space_needed > 0 && free_disk_space < disk_space_needed ) {
                    reply = MsgBox( NULL, "IDS_NODISKSPACE", GUI_YES_NO,
                            *disks[ i ], free_disk_space/1000, disk_space_needed/1000 );
                    if( reply == GUI_RET_NO ) {
                        return( FALSE );
                    }
                }
            }
        }
    }
    for( i = 0; i < max_targs; ++i ) {
        strcpy( drive, "DriveFreeN" );
        if( *space[i].drive != 0 && SimTargetNeedsUpdate( i ) ) {
            sprintf( buff, GetVariableStrVal( "IDS_DRIVE_SPEC" ), toupper( *(space[i].drive) ) );
            if( space[i].needed < 0 ) {
                catnum( buff, -space[i].needed );
                strcat( buff, GetVariableStrVal( "IDS_DRIVE_FREED" ) );
            } else {
                catnum( buff, space[i].needed );
                strcat( buff, GetVariableStrVal( "IDS_DRIVE_REQUIRED" ) );
                ucatnum( buff, space[i].free );
                strcat( buff, GetVariableStrVal( "IDS_DRIVE_AVAILABLE" ) );
            }
        } else {
            buff[0] = '\0';
        }
        drive[ strlen( drive ) - 1 ] = i + 1 + '0';
        SetVariableByName( drive, buff );
    }
    return( ret );
}

static void SetFileDate( char *dst_path, time_t date )
/****************************************************/
{
    struct utimbuf      timebuf;

    timebuf.modtime = date;
    timebuf.actime = date;
    utime( dst_path, &timebuf );
}

static void SameFileDate( char *src_path, char *dst_path )
/********************************************************/
{
    struct stat         statblk;

    stat( src_path, &statblk );
    SetFileDate( dst_path, statblk.st_mtime );
}

// ********************Functions for Deleting Files***********************

extern bool DoDeleteFile( char *Path )
/************************************/
{
    int         returnvalue;

    returnvalue = unlink( Path );
    if( returnvalue == 0 ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}

// ******************* Functions for Copying Files ***************************

extern COPYFILE_ERROR DoCopyFile( char *src_path, char *dst_path, int append )
/****************************************************************************/
{
    static char         lastchance[1024];
    size_t              buffer_size = 16 * 1024;
    int                 src_files, dst_files;
    int                 bytes_read, bytes_written, style;
    char                *pbuff;

    src_files = open( src_path, O_RDONLY + O_BINARY );
    if( src_files == -1 ) return( CFE_CANTOPENSRC );

    for( ;; ) {
        pbuff = GUIMemAlloc( buffer_size );
        if( pbuff != NULL ) break;
        buffer_size >>= 1;
        if( buffer_size < sizeof(lastchance) ) {
            pbuff = lastchance;
            buffer_size = sizeof(lastchance);
            break;
        }
    }

    if( append ) {
        style = O_RDWR + O_BINARY;
    } else {
        style = O_CREAT + O_TRUNC + O_WRONLY + O_BINARY;
    }
    dst_files = open( dst_path, style, S_IREAD + S_IWRITE );
    if( dst_files == -1 ) {
        close( src_files );
        if( pbuff != lastchance ) GUIMemFree( pbuff );
        dst_files = open( dst_path, O_RDONLY );
        if( dst_files != -1 ) {
            // read only file
            close( dst_files );
            return( CFE_DSTREADONLY );
        }
        return( CFE_CANTOPENDST );
    }
    if( append ) {
        lseek( dst_files, 0, SEEK_END );
    }

    do {
        bytes_read = read( src_files, pbuff, buffer_size );
        bytes_written = write( dst_files, pbuff, bytes_read );
        BumpStatus( bytes_written );
        if( bytes_written != bytes_read || StatusCancelled() ) {
            close( dst_files );
            close( src_files );
            if( bytes_written == bytes_read ) {
                // copy was aborted, delete destination file
                DoDeleteFile( dst_path );
                return( CFE_ABORT );
            }
            // error writing file - probably disk full
            if( pbuff != lastchance ) GUIMemFree( pbuff );
            SetupError( "IDS_WRITEERROR" );
            return( CFE_ERROR );
        }
    } while( bytes_read == buffer_size );

    // Make the destination file have the same time stamp as the source file.
    close( dst_files );

    SameFileDate( src_path, dst_path );
    if( pbuff != lastchance ) GUIMemFree( pbuff );
    close( src_files );
    return( CFE_NOERROR );
}


#define DIR_THRESHOLD   10

typedef struct split_file {
    struct split_file   *next;
    char                *src_path;
    char                *disk_desc;     // a file may span multiple disks
} split_file;

#define OVERHEAD_SIZE 10000 // removing a file is about like copying a small file

static bool CreateDirectoryTree()
{
    long                num_total_install;
    long                num_installed;
    int                 i;
    char                dst_path[ _MAX_PATH ];
    int                 max_dirs = SimNumDirs();

    num_total_install = 0;
    for( i = 0; i < max_dirs; i++ ) {
        if( SimDirUsed( i ) ) {
            ++num_total_install;
        }
    }
    if( num_total_install != 0 ) {
        StatusLines( STAT_CREATEDIRECTORY, "" );
        StatusAmount( 0, num_total_install );
    }
    num_installed = 0;
    for( i = 0; i < max_dirs; i++ ) {
        if( SimDirUsed( i ) ) {
            if( !CreateDstDir( i, dst_path ) ) return( FALSE );
            StatusLines( STAT_SAME, dst_path );
            StatusAmount( ++num_installed, num_total_install );
            if( StatusCancelled() ) return( FALSE );
        }
    }
    if( num_total_install != 0 ) {
        StatusLines( STAT_SAME, "" );
        StatusAmount( num_total_install, num_total_install );
    }
    return( TRUE );
}

static bool RelocateFiles()
/*************************/
{
    int                 filenum;
    int                 subfilenum, max_subfiles;
    long                num_total_install;
    long                num_installed;
    char                dst_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                dir[ _MAX_PATH ];
    char                file_desc[ MAXBUF ];
    int                 max_files = SimNumFiles();

    num_total_install = 0;
    for( filenum = 0; filenum < max_files; filenum++ ) {
        if( SimFileRemove( filenum ) ) continue;
        max_subfiles = SimNumSubFiles( filenum );
        for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
            if( SimSubFileInOldDir( filenum, subfilenum ) ) {
                num_total_install += SimSubFileSize( filenum, subfilenum );
            }
        }
    }

    if( num_total_install != 0 ) {
        StatusLines( STAT_RELOCATING, "" );
        StatusAmount( 0, num_total_install );
    }
    num_installed = 0;
    for( filenum = 0; filenum < max_files; filenum++ ) {
        if( SimFileRemove( filenum ) ) continue;
        max_subfiles = SimNumSubFiles( filenum );
        for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
            if( SimSubFileInOldDir( filenum, subfilenum ) ) {
                SimFileOldDir( filenum, dir );
                SimSubFileName( filenum, subfilenum, file_desc );
                _makepath( src_path, NULL, dir, file_desc, NULL );
                SimFileDir( filenum, dir );
                _makepath( dst_path, NULL, dir, file_desc, NULL );
                StatusLines( STAT_SAME, src_path );
                if( SimSubFileInNewDir( filenum, subfilenum ) ) {
                    remove( dst_path );
                }
                if( DoCopyFile( src_path, dst_path, FALSE ) != CFE_NOERROR ) {
                    return( FALSE );
                }
                SameFileDate( src_path, dst_path );
                remove( src_path );
                num_installed += SimSubFileSize( filenum, subfilenum );
                StatusAmount( num_installed, num_total_install );
            }
        }
    }
    if( num_total_install != 0 ) {
        StatusLines( STAT_RELOCATING, "" );
        StatusAmount( num_total_install, num_total_install );
    }
    return( TRUE );
}


typedef struct file_check {
    struct file_check   *next;
    char                *name;
    vhandle             var_handle;
    bool                is_dll;
} file_check;

static file_check *FileCheck = NULL;
static file_check *FileCheckThisPack = NULL;

static void NewFileToCheck( char *name, bool is_dll )
/***********************************************************/
{
    file_check  *new;

    new = GUIMemAlloc( sizeof( *new ) );
    new->next = FileCheckThisPack;
    FileCheckThisPack = new;
    GUIStrDup( name, &new->name );
    new->var_handle = NO_VAR;
    new->is_dll = is_dll;
}

static void UpdateCheckList( char *name, vhandle var_handle )
/********************************************************/
{
    file_check  *check;

    for( check = FileCheckThisPack; check != NULL; check = check->next ) {
        if( stricmp( name, check->name ) == 0 ) {
            check->var_handle = var_handle;
        }
    }
}

static void TransferCheckList()
/*****************************/
{
    file_check  *check,*next;

    for( check = FileCheckThisPack; check != NULL; check = next ) {
        next = check->next;
        check->next = FileCheck;
        FileCheck = check;
    }
    FileCheckThisPack = NULL;
}

static bool CheckPendingFiles()
/*****************************/
{
    file_check  *curr, *next;
    gui_message_return  ret;

    for( curr = FileCheck; curr != NULL; curr = next ) {
        next = curr->next;
        if( curr->is_dll ) {
            ret = CheckInstallDLL( curr->name, curr->var_handle );
        } else {
            ret = CheckInstallNLM( curr->name, curr->var_handle );
        }
        if( ret == GUI_RET_CANCEL ) return( FALSE );
        GUIMemFree( curr->name );
        GUIMemFree( curr );
    }
    return( TRUE );
}

static void CopySetupInfFile()
/****************************/
{
    char                *p;
    char                dst_path[ _MAX_PATH ];
    char                tmp_path[ _MAX_PATH ];

    // if DoCopyInf variable is set, copy/delete setup.inf
    p = GetVariableStrVal( "DoCopyInf" );
    if( ( p != NULL ) && ( strlen( p ) > 0 ) ) {
        ReplaceVars( tmp_path, p );
        _makepath( dst_path, NULL, tmp_path, "setup.inf", NULL );
        if( VarGetIntVal( UnInstall ) ) {
            remove( dst_path );
        } else {
            p = GetVariableStrVal( "SetupInfFile" );
            DoCopyFile( p, dst_path, FALSE );
        }
    }
}

static bool DoCopyFiles()
/***********************/
{
    int                 filenum, disk_num;
    int                 subfilenum, max_subfiles;
    COPYFILE_ERROR      copy_error;
    char                dst_path[ _MAX_PATH ];
    char                tmp_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                file_name[ _MAX_FNAME + _MAX_EXT ];
    char                file_desc[ MAXBUF ], dir[ _MAX_PATH ], disk_desc[ MAXBUF ];
    char                old_dir[ _MAX_PATH ];
    long                num_total_install;
    long                num_installed;
    split_file          *split = NULL;
    split_file          **owner_split = &split;
    split_file          *junk;
    int                 value;
    vhandle             var_handle;
    gui_message_return  ret = GUI_RET_OK;
    int                 max_files = SimNumFiles();
    int                 len;
    char                *p;


    num_total_install = 0;
    for( filenum = 0; filenum < max_files; filenum++ ) {
        SimFileDir( filenum, dir );
        if( SimFileAdd( filenum ) && !SimFileUpToDate( filenum ) ) {
            num_total_install += SimFileSize( filenum );
            if( SimFileSplit( filenum ) ) {
                num_total_install += SimFileSize( filenum );
            }
            max_subfiles = SimNumSubFiles( filenum );
            for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
                if( SimSubFileReadOnly( filenum, subfilenum ) ) {
                    SimSubFileName( filenum, subfilenum, file_desc );
                    _makepath( tmp_path, NULL, dir, file_desc, NULL );
                    if( !PromptUser( tmp_path,
                             "ReadOnlyFile",
                             "RO_Skip_Dialog",
                             "RO_Replace_Old",
                             &value ) ) return( FALSE );
                    if( value ) {
                        chmod( tmp_path, S_IWRITE );
                    }
                }
                if( SimSubFileNewer( filenum, subfilenum ) ) {
                    SimSubFileName( filenum, subfilenum, file_desc );
//                  _splitpath( file_desc, NULL, NULL, NULL, file_ext );
                    _makepath( tmp_path, NULL, dir, file_desc, NULL );
                    if( !PromptUser( tmp_path,
                            "NewerFile",
                            "Newer_Skip_Dialog",
                            "Newer_Replace_Old",
                            &value ) ) return( FALSE );
                    if( value ) {
                        SetFileDate( tmp_path, SimSubFileDate( filenum, subfilenum )-1 );
                    }
                }
            }
        } else if( SimFileRemove( filenum ) ) {
            max_subfiles = SimNumSubFiles( filenum );
            for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
                if( !SimSubFileExists( filenum, subfilenum ) ) continue;
                if( SimSubFileReadOnly( filenum, subfilenum ) ) {
                    SimSubFileName( filenum, subfilenum, file_desc );
                    _makepath( tmp_path, NULL, dir, file_desc, NULL );
                    if( !PromptUser( tmp_path,
                             "DeleteReadOnlyFile",
                             "RO_Skip_Remove",
                             "RO_Remove_Old",
                             &value ) ) return( FALSE );
                    if( value ) {
                        chmod( tmp_path, S_IWRITE );
                        num_total_install += OVERHEAD_SIZE;
                    }
                } else {
                    num_total_install += OVERHEAD_SIZE;
                }
            }
        }
    }

    num_installed = 0;
    StatusLines( STAT_COPYINGFILE, "" );
    StatusAmount( 0, num_total_install );

    /* remove files first so we don't go over disk space estimate */

    for( filenum = 0; filenum < max_files; filenum++ ) {
        if( SimFileRemove( filenum ) ) {
            SimFileDir( filenum, dir );
            max_subfiles = SimNumSubFiles( filenum );
            for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
                if( !SimSubFileExists( filenum, subfilenum ) ) continue;
                num_installed += OVERHEAD_SIZE;
                SimSubFileName( filenum, subfilenum, file_desc );
                _makepath( tmp_path, NULL, dir, file_desc, NULL );
                StatusLines( STAT_REMOVING, tmp_path );
                remove( tmp_path );
                if( SimSubFileInOldDir( filenum, subfilenum ) ) {
                    SimFileOldDir( filenum, old_dir );
                    _makepath( tmp_path, NULL, old_dir, file_desc, NULL );
                    StatusLines( STAT_REMOVING, tmp_path );
                    remove( tmp_path );
                }
                StatusAmount( num_installed, num_total_install );
                if( StatusCancelled() ) return( FALSE );
            }
        }
    }

    /* now go ahead and add files */

    for( filenum = 0; filenum < max_files; filenum++ ) {
        if( !SimFileAdd( filenum ) || SimFileUpToDate( filenum ) ) {
            while( split != NULL ) {
                junk = split;
                split = split->next;
                GUIMemFree( junk->src_path );
                GUIMemFree( junk );
            }
            owner_split = &split;
            *owner_split = NULL;
            continue;
        }
        SimFileDir( filenum, dir );
        SimGetFileDesc( filenum, file_desc );
        SimGetFileName( filenum, file_name );
        disk_num = SimFileDisk( filenum, disk_desc );

//      _splitpath( file_desc, NULL, NULL, NULL, file_ext );
        _makepath( dst_path, NULL, dir, file_desc, NULL );

        strcpy( src_path, GetVariableStrVal( "SrcDir" ) );
        p = GetVariableStrVal( "DstDir" );
        len = strlen( src_path );
        if( len > 0 && src_path[ len - 1 ] != '\\' ) {
            strcat( src_path, "\\" );
        }
        len = strlen( p );
        if( strncmp( dir, p, len ) == 0 ) {
            if( dir[len] == '\\' ) len++; // if 1st char to concat is a backslash, skip it
            strcat( src_path, dir+len ); // get rid of the dest directory, just keep the subdir
        } else {
            // use the macro as the directory name   eg: cd_drive:\winsys\filename
            SimTargetDirName( SimDirTargNum( SimFileDirNum( filenum ) ), tmp_path );
            len = strlen( GetVariableStrVal( tmp_path ) );
            strcat( src_path, tmp_path );
            strcat( src_path, dir + len );
        }
        p = src_path + strlen(src_path);
        strcat( src_path, file_desc );

        if( StatusCancelled() ) {
            return( FALSE );
        }

        max_subfiles = SimNumSubFiles( filenum );
        for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
            do {
                SimSubFileName( filenum, subfilenum, file_desc );
                var_handle = SimSubFileVar( filenum, subfilenum );
                _makepath( tmp_path, NULL, dir, file_desc, NULL );

                *p='\0'; // nuke name from end of src_path
                strcat( src_path, file_desc );
                StatusLines( STAT_COPYINGFILE, tmp_path );
                UnPackHook( tmp_path );
                copy_error = DoCopyFile( src_path, tmp_path, FALSE );

                switch( copy_error ) {
                case CFE_ABORT:
                case CFE_ERROR:
                    ret = GUI_RET_CANCEL;
                    break;
                case CFE_BAD_CRC:
                    MsgBox( NULL, "IDS_BADCRC", GUI_OK, src_path );
                    ret = GUI_RET_CANCEL;
                    break;
                case CFE_NOERROR:
                case CFE_DSTREADONLY:
                    copy_error = CFE_NOERROR;
                    break;
                case CFE_NOMEMORY:
                    ret = MsgBox( NULL, "IDS_NOMEMORYCOPY", GUI_RETRY_CANCEL );
                    break;
                case CFE_CANTOPENSRC:
                    ret = MsgBox( NULL, "IDS_CANTOPENSRC", GUI_RETRY_CANCEL, src_path  );
                    break;
                case CFE_CANTOPENDST:
                    ret = MsgBox( NULL, "IDS_CANTOPENDST", GUI_RETRY_CANCEL, tmp_path );
                    break;
                }
                if( ret == GUI_RET_CANCEL ) return( FALSE );
            } while( copy_error != CFE_NOERROR );

            SetVariableByHandle( var_handle, tmp_path );
            UpdateCheckList( tmp_path, var_handle );
        }
        TransferCheckList();

        num_installed += SimFileSize( filenum );
        if( num_installed > num_total_install ) num_installed = num_total_install;
        StatusAmount( num_installed, num_total_install );
        if( StatusCancelled() ) return( FALSE );
    }
    if( !CheckPendingFiles() ) return( FALSE );
    StatusAmount( num_total_install, num_total_install );
    return( TRUE );
}


static void RemoveUnusedDirs()
/****************************/
{
    char        dst_path[_MAX_PATH];
    int         i;
    int         max_dirs = SimNumDirs();

    for( i = 0; i < max_dirs; i++ ) {
        if( !SimDirUsed( i ) ) {
            RemoveDstDir( i, dst_path );
        }
    }
}

static void RemoveExtraFiles()
/****************************/
// remove supplemental files
{
    char                *p;
    char                dst_path[ _MAX_PATH ];

    if( VarGetIntVal( UnInstall ) ) {
        // delete saved autoexec's and config's
        p = GetVariableStrVal( "DstDir" );
#if defined( __NT__ )
        // Windows NT
        strcpy( dst_path, p );
        strcat( dst_path, "\\CHANGES.ENV" );
        remove( dst_path );
        // Windows 95
        strcpy( dst_path, p );
        strcat( dst_path, "\\AUTOEXEC.W95" );
        remove( dst_path );
        strcpy( dst_path, p );
        strcat( dst_path, "\\CONFIG.W95" );
        remove( dst_path );
#elif defined( __OS2__ )
        strcpy( dst_path, p );
        strcat( dst_path, "\\CONFIG.OS2" );
        remove( dst_path );
#else
        strcpy( dst_path, p );
        strcat( dst_path, "\\AUTOEXEC.DOS" );
        remove( dst_path );
        strcpy( dst_path, p );
        strcat( dst_path, "\\CONFIG.DOS" );
        remove( dst_path );
#endif
    }
}

extern  void DetermineSrcState( char *src_dir )
/*********************************************/
{
    int         len;

//  if( SrcInstState != SRC_UNKNOWN ) return;

    // if installing from CD or hard disk, add DISK# to source path
    len = strlen( src_dir );
    if( len > 0 && src_dir[ len - 1 ] != '\\' ) {
        src_dir[ len ] = '\\';
        src_dir[ len + 1 ] = '\0';
    }
    strcat( src_dir, "diskimgs\\disk01" );
    if( access( src_dir, F_OK ) == 0 ) {
        SetVariableByName( "SrcIsCD", "1" );
        SrcInstState = SRC_CD;
    } else {
        SetVariableByName( "SrcIsCD", "0" );
        SrcInstState = SRC_DISK;
    }
    src_dir[ len ] = '\0';
}

extern bool CopyAllFiles( void )
/******************************/
{
    FileCheck = NULL;
    FileCheckThisPack = NULL;

    if( !CreateDirectoryTree() ) return( FALSE );
    if( !RelocateFiles() ) return( FALSE );
#if defined( __OS2__ ) && !defined( _UI )
    LabelDirs();    // add labels (long names) to directories
#endif
    if( !DoCopyFiles() ) return( FALSE );
    CopySetupInfFile();
    RemoveExtraFiles();
    RemoveUnusedDirs();
    StatusCancelled(); /* make sure display gets updated */
    return( TRUE );
}


static bool NukePath( char *path, int stat )
/******************************************/
{
    DIR                 *d;
    struct dirent       *info;
    char                *path_end;

    d = opendir( path );
    path_end = path+strlen(path);
    if( path_end > path && path_end[-1] == '\\' ) --path_end;
    *path_end = '\\';
    while( info = readdir( d ) ) {
        strcpy( path_end+1, info->d_name );
        if( info->d_attr & _A_SUBDIR ) {
            if( info->d_name[0] != '.' ) {
            if( !NukePath( path, stat ) ) return( FALSE );
                rmdir( path );
            }
        } else {
            if( info->d_attr & (_A_RDONLY+_A_SYSTEM+_A_HIDDEN) ) {
                chmod( path, S_IWRITE );
            }
            if( remove( path ) != 0 ) {
                return( FALSE );
            }
        }
        StatusLines( stat, path );
    }
    *path_end = '\0';
    closedir( d );
    return( TRUE );
}


// *********************** Miscellaneous Function ****************************

void DeleteObsoleteFiles()
/************************/
{
    int         i, max_deletes;
    int         group;
    dlg_state   state;
    char        buff[_MAX_PATH];
    bool        *found;
    bool        found_any;

    max_deletes = SimNumDeletes();
    group = 0;
    for( i = 0; i < max_deletes; ++i ) {
        if( SimDeleteIsDialog( i ) ) ++group;
    }
    found = GUIMemAlloc( sizeof( bool ) * group );
    memset( found, FALSE, sizeof( bool ) * group );
    found_any = FALSE;
    group = -1;
    for( i = 0; i < max_deletes; ++i ) {
        if( SimDeleteIsDialog( i ) ) {
            ++group;
        } else {
            ReplaceVars( buff, SimDeleteName( i ) );
            if( access( buff, F_OK ) == 0 ) {
                found[group] = TRUE;
                found_any = TRUE;
            }
        }
    }
    group = -1;
    if( found_any ) {
        StatusShow( TRUE );
        StatusLines( STAT_REMOVING, "" );
        StatusAmount( 0, 1 );
        for( i = 0; i < max_deletes; ++i ) {
            if( SimDeleteIsDialog( i ) ) {
                ++group;
            }
            if( found[group] ) {
                ReplaceVars( buff, SimDeleteName( i ) );
                if( SimDeleteIsDialog( i ) ) {
                    state = DoDialog( buff );
                } else if( state == DLG_NEXT ) {
                    if( SimDeleteIsDir( i ) ) {
                        NukePath( buff, STAT_REMOVING );
                        rmdir( buff );
                    } else {
                        StatusLines( STAT_REMOVING, buff );
                        remove( buff );
                    }
                }
            }
        }
        StatusShow( FALSE );
    }
}

extern char *GetInstallName()
{
    static char name[_MAX_FNAME];
    int         argc;
    char        **argv;

    if( name[0] == '\0' ) {
        GUIGetArgs( &argv, &argc );
        _splitpath( argv[0], NULL, NULL, name, NULL );
        strupr( name );
    }
    return( name );
}

char *AddInstallName( char *text, bool dorealloc )
/***********************************************/
{
    size_t              len;
    size_t              inst_len;
    char                *inst_name;
    char                *p;
    int                 offset;

    len = strlen( text ) + 1;
    inst_name = GetInstallName();
    inst_len = strlen( inst_name );
    for( ;; ) {
        // p = strchr( text, '@' ); no good for dbcs!!!
        p = text;
        for( ;; ) {
            if( *p == '\0' ) {
                p = NULL;
                break;
            }
            if( *p == '@' ) break;
            p += GUICharLen( *p );
        }
        if( p == NULL ) break;
        offset = p - text;
        if( dorealloc ) {
            text = GUIMemRealloc( text, len + inst_len );
            p = text + offset;
        }
        memmove( p + inst_len, p+1, len - (p+1 - text) );
        memcpy( p, inst_name, inst_len );
        p += inst_len;
        len += inst_len;
    }
    return( text );
}

extern gui_message_return MsgBox( gui_window *gui, char *messageid, gui_message_type wType, ... )
/***************************************************************************************/
{
    gui_message_return  result;
    char                buff[ 1024 ];
    char                msg[ 1024 ];
    char *              errormessage;
    va_list             arglist;
    int                 i;
    int                 msg_index;

    if( stricmp( messageid, "IDS_NOSETUPINFOFILE" ) == 0 ) {
        // If the message is "can't find the setup.inf file", then
        // don't look up the string, because it is in the file we can't find
        errormessage = "The file %s cannot be found.";
    } else {
        errormessage = GetVariableStrVal( messageid );
    }
    if( errormessage == NULL ) {
        strcpy( buff, GetVariableStrVal( "IDS_UNKNOWNERROR" ) );
    } else {
        va_start( arglist, wType );
        vsprintf( buff, errormessage, arglist );
        va_end( arglist );
    }
    AddInstallName( buff, FALSE );

    if( gui == NULL ) {
//      wType |= GUI_SYSTEMMODAL;
    }

    if( GUIIsGUI() ){
        if( wType & GUI_YES_NO ) {
            wType |= GUI_QUESTION;
        } else {
            wType |= GUI_INFORMATION;
        }
    }

//  following code removed - causes infinite loop (and buff will never be NULL)
//  if( buff == NULL || buff[0] == '\0' ) {
//      SetupError( "IDS_NULLSTRING" );
//  }

    msg_index = 0;
    for( i = 0; i < strlen( buff ); i++ ) {
        if( buff[ i ] == '&' ) {
            continue;              // skip the '& character.
        }
        msg[ msg_index ] = buff [ i ];
        msg_index++;
    }
    msg[ msg_index ] = '\0';

    if( SkipDialogs ) {
        switch( wType ) {
        case GUI_ABORT_RETRY_IGNORE:
            result = GUI_RET_ABORT;
            break;
        case GUI_RETRY_CANCEL:
            result = GUI_RET_CANCEL;
            break;
        case GUI_YES_NO:
        case GUI_YES_NO + GUI_QUESTION:
        case GUI_YES_NO + GUI_INFORMATION:
            result = GUI_RET_NO;
            break;
        case GUI_YES_NO_CANCEL:
        case GUI_YES_NO_CANCEL + GUI_QUESTION:
        case GUI_YES_NO_CANCEL + GUI_INFORMATION:
            result = GUI_RET_CANCEL;
            break;
        default:
            result = GUI_RET_OK;
        }
    } else {
        result = GUIDisplayMessage( gui == NULL ? MainWnd : gui, msg, GetInstallName(), wType );
    }
    return( result );
}

#ifdef PATCH
// *********************** DECODE Functions **********************************

static jmp_buf          PackFailed;

extern int PerformDecode( char *src, char *dst, unsigned_32 internal )
/*********************************************************************/
{

    arccmd              cmd;
    wpackfile           files[ 1 ];
    char                drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ];
    char                destdir[ _MAX_DIR ];

     // get rid of trailing filename if it exists.
    _splitpath( dst, drive, dir, NULL, NULL );
    _makepath( destdir, drive, dir, NULL, NULL );

    cmd.u.path = destdir;
    files[ 0 ].filename = NULL;
    files[ 0 ].packname = NULL;
    cmd.files = &files;
    cmd.arcname = src;
    cmd.flags = PREPEND_PATH | BE_QUIET;
    cmd.internal = internal;
    DecodeError = CFE_NOERROR;
    if( setjmp( PackFailed ) == 0 ) {
        if( !Decode( &cmd ) ) {
            DecodeError = CFE_ERROR;
        }
    } else {
        // we longjmp'd to here
    }
    return( DecodeError );
}

void QueryCancel()
{
    if( StatusCancelled() ) {
        PackExit();
    }
}


// functions required for WPack Decoder

static void PackExitWithCode( int code )
/********************************/
{
    DecodeError = code;
    longjmp( PackFailed, 1 );
}


extern void PackExit( void )
/**************************/
{
    // the WPACK code can't handle this routine returning
    PackExitWithCode( CFE_ERROR );
}

extern void Error( int code, char *msg )
/****************************/
{
    dlg_state           return_state;

    switch( code ) {
    case TXT_INC_CRC:
        PackExitWithCode( CFE_BAD_CRC );
        break;
    case TXT_ARC_NOT_EXIST:
        PackExitWithCode( CFE_CANTOPENSRC );
        break;
    case TXT_NOT_OPEN:
        SetVariableByName( "OpenError", msg );
        return_state = DoDialog( "CantOpen" );
        if( return_state == DLG_CAN || return_state == DLG_DONE ) {
            PackExit();
        }
        break;
    default:
        MsgBox( NULL, "IDS_ERROR", GUI_OK, msg );
        PackExit();
        break;
    }
}
#endif


extern int PromptUser( char *name, char *dlg, char *skip,
            char *replace, int *value )
{
    dlg_state                   return_state;

    SetVariableByName( "FileDesc", name );

    // don't display the dialog if the user selected the "Skip dialog" option
    if( GetVariableIntVal( skip ) == 0 ) {
        for( ;; ) {
            return_state = DoDialog( dlg );
            if( return_state != DLG_DONE && return_state != DLG_CAN ) break;
            if( MsgBox( NULL, "IDS_QUERYABORT", GUI_YES_NO ) == GUI_RET_YES ) {
                CancelSetup = TRUE;
                return( FALSE );
            }
        }
    }

    *value = ( GetVariableIntVal( replace ) == 1 );
    return( TRUE );
}

int UnPackHook( char *name )
/***************************/
{
    char        drive[_MAX_DRIVE];
    char        dir[ _MAX_DIR ];
    char        fname[_MAX_FNAME];
    char        ext[_MAX_EXT];

    _splitpath( name, drive, dir, fname, ext );
    if( stricmp( ext, ".NLM" ) == 0 ) {
        NewFileToCheck( name, FALSE );
#ifndef PATCH
        _makepath( name, drive, dir, fname, "._N_" );
#endif
        return( 1 );
    } else if( stricmp( ext, ".DLL" ) == 0 ) {
        NewFileToCheck( name, TRUE );
#ifdef PATCH
        if( !IsPatch ) {
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
            _makepath( name, drive, dir, fname, "._D_" );
#endif
        }
#endif
        return( 1 );
    }
    return( 0 );
}


#ifdef PATCH
extern int OK_ToReplace( char *name )
/***********************************/
{
    name=name;
    return( FALSE ); // pre-checked.  Anything left is NOT ok
}


extern int OK_ReplaceRDOnly( char *name )
/***************************************/
{
    name=name;
    return( FALSE ); // pre-checked.  Anything left is NOT ok
}


extern void LogUnPacking( char *name )
/************************************/
{
    StatusLines( STAT_COPYINGFILE, name );
}

extern void Log( char *start, ... )
/*********************************/
{
    // shouldn't get called

    start = start;
}
#endif


static void AddDefine( char *def )
/********************************/
// Process command line switch to set a variable
//     -dvariable=value
{
    char                *p;
    DEF_VAR             *var;

    p = strchr( def, '=' );
    if( p != NULL ) {
        *p = '\0';
        ++p;
        var = GUIMemAlloc( sizeof( DEF_VAR ) );
        var->variable = strdup( def );
        var->value = strdup( p );
        var->link = ExtraVariables;
        ExtraVariables = var;
    }
}

static void DefineVars()
/**********************/
// Create variables specified on command line
{
    DEF_VAR             *var;

    for( var = ExtraVariables; var != NULL; var = var->link ) {
        SetVariableByName( var->variable, var->value );
    }
}

extern bool GetDirParams( int                   argc,
                          char **               argv,
                          char **               inf_name,
                          char **               tmp_path )
/********************************************************/
{
    char                buff[ _MAX_PATH ];
    char                dir[ _MAX_DIR ];
    char                drive[ _MAX_DRIVE ];
    int                 i;

    *inf_name = GUIMemAlloc( _MAX_PATH );
    if( *inf_name == NULL ) {
        return FALSE;
    }

    *tmp_path = GUIMemAlloc( _MAX_PATH );
    if( *tmp_path == NULL ) {
        GUIMemFree( *inf_name );
        return FALSE;
    }


    Invisible           = FALSE;
    SkipDialogs         = FALSE;
    VariablesFile       = NULL;
    ExtraVariables      = NULL;
    i                   = 1;

    while( i < argc ) {
        if( argv[ i ][ 0 ] == '-'
            || argv[ i ][ 0 ] == '/' ) {
            switch( argv[ i ][ 1 ] ) {
            case 'f': // Process "script" file to override variables in setup.inf
            case 'F':
                if( argv[ i ][ 2 ] == '='
                    && argv[ i ][ 3 ] != '\0'
                    && access( &argv[ i ][ 3 ], R_OK ) == 0 ) {
                    VariablesFile = strdup( &argv[ i ][ 3 ] );
                }
                break;
            case 'd':
            case 'D':
                AddDefine( &argv[ i ][ 2 ] );
                break;
            case 'i': // No screen output (requires SkipDialogs below as well)
            case 'I':
                Invisible = TRUE;
                // falling through!!!

            case 's': // Skip dialogs
            case 'S':
                SkipDialogs = TRUE;
                break;
            }

            i++;
        } else {
            break;
        }
    }
    if( i < argc ) {
        strcpy( *inf_name, argv[ i ] );
        i++;
    } else {
        _splitpath( argv[ 0 ], drive, dir, NULL, NULL );
        _makepath( buff, drive, dir, "setup", "inf" );
        _fullpath( *inf_name, buff, _MAX_PATH );
    }

    if( i < argc ) {
        strcpy( *tmp_path, argv[ i ] );
    } else {
        _splitpath( *inf_name, drive, dir, NULL, NULL );
        _makepath( *tmp_path, drive, dir, NULL, NULL );
    }

    return TRUE;
}

extern bool FreeDirParams( char **inf_name, char **tmp_path )
/***********************************************************/
{
    if( inf_name == NULL || tmp_path == NULL ) return FALSE;

    GUIMemFree( *inf_name );
    GUIMemFree( *tmp_path );

    *inf_name = NULL;
    *tmp_path = NULL;

    return TRUE;
}

extern void ReadVariablesFile( char * name )
/******************************************/
{
    FILE   *fp;
    char   buf[ 256 ];
    char   *line;
    char   *variable;
    char   *value;

    fp = fopen( VariablesFile, "rt" );
    if( fp == NULL ) {
        return;
    }

    while( fgets( buf, sizeof( buf ), fp ) != NULL ) {
        line = buf;
        while( isspace( *line ) != 0 ) {
            line++;
        }
        if( *line == '#' ) {
            continue;
        }
        while( strlen( line ) > 0
            && isspace( line[ strlen( line ) - 1 ] ) != 0 ) {
            line[ strlen( line ) - 1 ] = '\0';
        }
        variable = strtok( line, " =\t" );
        value = strtok( NULL, "=\t\0" );
        if( value != NULL ) {
            while( isspace( *value ) != 0 ) {
                value++;
            }

            while( strlen( value ) > 0
                && isspace( value[ strlen( value ) - 1 ] ) != 0 ) {
                value[ strlen( value ) - 1 ] = '\0';
            }
            if( variable != NULL ) {
                if( name == NULL
                    || ( name != NULL
                         && stricmp( name, variable ) == 0 ) ) {

                    if( stricmp( value, "true" ) == 0 ) {
                        SetVariableByName( variable, "1" );
                    } else if( stricmp( value, "false" ) == 0 ) {
                        SetVariableByName( variable, "0" );
                    } else {
                        SetVariableByName( variable, value );
                    }
                }
            }
        }
    }
    fclose( fp );
}

extern bool InitInfo( char * inf_name, char * tmp_path )
/******************************************************/
// initialize global vbls. and read setup.inf into memory.
{
    char                dir[ _MAX_DIR ];
    char                drive[ _MAX_DRIVE ];
    int                 ret;
    int                 len;

    SetVariableByName( "SrcDir", tmp_path );
    DetermineSrcState( tmp_path );
    len = strlen( tmp_path );
    if( len > 0 && tmp_path[len-1] == '\\' ) {
        tmp_path[len-1] = '\0';
    }
    SetVariableByName( "SrcDir2", tmp_path );
    _splitpath( inf_name, drive, dir, NULL, NULL );
    _makepath( tmp_path, drive, dir, "diskset", "inf" );

    ret = SimInit( inf_name );
    if( ret == SIM_INIT_NOERROR ) {
        DefineVars();
        if( VariablesFile != NULL ) {
            ReadVariablesFile( NULL );
        }
        return( TRUE );
    }
    if( ret == SIM_INIT_NOMEM ) {
        MsgBox( NULL, "IDS_NOMEMORY", GUI_OK, inf_name );
    } else { // SIM_INIT_NOFILE
        MsgBox( NULL, "IDS_NOSETUPINFOFILE", GUI_OK, inf_name );
    }
    return( FALSE );
}

extern void CloseDownMessage( bool installed_ok )
/***********************************************/
{

    if( installed_ok ) {
        if( VarGetIntVal( UnInstall ) != 0 ) {
            MsgBox( NULL, "IDS_UNSETUPCOMPLETE", GUI_OK );
        } else {
            if( ConfigModified ) {
#if defined( __NT__ )
                MsgBox( NULL, "IDS_NT_REBOOT", GUI_OK );
#elif defined( __OS2__ )
                MsgBox( NULL, "IDS_OS2_REBOOT", GUI_OK );
#elif defined( __DOS__ )
                MsgBox( NULL, "IDS_DOS_REBOOT", GUI_OK );
#elif defined( __WINDOWS__ )
                MsgBox( NULL, "IDS_WINDOWS_REBOOT", GUI_OK );
#endif
            } else {
                MsgBox( NULL, "IDS_COMPLETE", GUI_OK );
            }
        }
    } else if( !CancelSetup ) {
        if( VarGetIntVal( UnInstall ) != 0 ) {
            MsgBox( NULL, "IDS_UNSETUPNOGOOD", GUI_OK );
        } else {
            MsgBox( NULL, "IDS_SETUPNOGOOD", GUI_OK );
        }
    }
}


extern void CloseDownProgram( void )
/**********************************/
{
    StatusFini();
    SetupFini();
}


#ifdef DO_DEBUGGING
extern void CheckHeap( void )
/***************************/
{
    switch( _heapchk() ) {
    case _HEAPOK:
        SetupError( "IDS_HEAPOK" );
        break;
    case _HEAPEND:
        SetupError( "IDS_HEAPEND" );
        break;
    case _HEAPEMPTY:
        SetupError( "IDS_HEAPEMPTY" );
        break;
    case _HEAPBADBEGIN:
        SetupError( "IDS_HEAPDAMAGE" );
        break;
    case _HEAPBADPTR:
        SetupError( "IDS_HEAPBADPOINTER" );
        break;
    case _HEAPBADNODE:
        SetupError( "IDS_HEAPBADNODE" );
        break;
    }
}
#endif

char *stristr( char *str, char *substr )
{
    int str_len, substr_len;

    str_len = strlen( str );
    substr_len = strlen( substr );
    while( str_len >= substr_len ) {
        if( strnicmp( str, substr, substr_len ) == 0 ) return( str );
        ++str;
        --str_len;
    }
    return( NULL );
}
