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
* Description:  Installer utility functions for Unix.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#if defined( __UNIX__ ) && !defined( __WATCOMC__ )
  #include <alloca.h>
#endif
#include <fcntl.h>
#include <ctype.h>
#include <setjmp.h>
#include <limits.h>
#include <process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
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

#include "errno.h"


extern char     *NextToken( char *buf, char delim );
extern void     BumpStatus( long );
static char *   GetInstallName();

bool            ConfigModified = FALSE;
static enum { SRC_UNKNOWN, SRC_CD, SRC_DISK } SrcInstState;

int             SkipDialogs;
char            *VariablesFile;
//DEF_VAR         *ExtraVariables;
int             Invisible;

#ifdef PATCH
extern int      InitIO( void );
extern int      Decode( arccmd *);

static int      DecodeError;
extern int      IsPatch;
#endif
extern bool     CancelSetup;
extern vhandle  UnInstall;
extern vhandle  PreviousInstall;

extern bool ModifyEnvironment( bool uninstall )
/*********************************************/
{
    bool                ret;

    ret = TRUE;
    uninstall = uninstall;
    return( ret );
}

extern bool ModifyStartup( bool uninstall )
/*****************************************/
{
    bool                ret;

    ret = ModifyAutoExec();
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

typedef __far (HANDLER)( unsigned deverr,
                                  unsigned errcode,
                                  unsigned far *devhdr );

static bool DoSpawnCmd( char *cmd )
{
    bool        rc;

    system( cmd );
    rc = TRUE;
    GUIWndDirty( NULL );
    return( rc );
}


void DoSpawn( when_time when )
{
    char buff[2*_MAX_PATH];
    int         i, num_spawns;

    num_spawns = SimNumSpawns();
    for( i = 0; i < num_spawns; ++i ) {
        if( when != SimWhen( i ) ) continue;
        if( !SimEvalSpawnCondition( i ) ) continue;
        SimGetSpawnCommand( buff, i );
        if( buff[0] == '\0' ) continue;
        DoSpawnCmd( buff );
    }
}


#define TMPFILENAME "_watcom_.tmp"
static void GetTmpFileName( unsigned drive, char *buff )
{
    strcpy( buff, "/tmp" );
    strcat( buff, "/" TMPFILENAME );
}

static void GetTmpFileNameInTarget( unsigned drive, char *buff )
{
    int         i;
    int         max_targets = SimNumTargets();
    int         len;

    for( i = 0; i < max_targets; ++i ) {
        SimTargetDir( i, buff );
        if( tolower( buff[0] ) == tolower( drive ) && buff[1] == ':' ) {
            len = strlen( buff );
            if( len != 0 && buff[len-1] != '/' ) {
                strcat( buff, "/" );
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
    return 0;
}

static long GetFreeDiskSpace( unsigned drive, bool removable )
/**********************************************************/
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
    if( drive == 0 ) return( FALSE );
    // don't bang diskette every time!
    if( Drives[ GetDriveNum( drive ) ].diskette ) return( FALSE );
    return( Drives[ GetDriveInfo( drive, FALSE ) ].fixed != 0 );
}

unsigned GetClusterSize( unsigned drive )
/***************************************/
{
    if( drive == 0 ) return( 1 );
    return( Drives[ GetDriveInfo( drive, FALSE ) ].cluster_size );
}

unsigned UseTargetForTmpFile( unsigned drive )
/*************************************/
{
    if( drive == 0 ) return( 0 );
    return( Drives[ GetDriveInfo( drive, FALSE ) ].use_target_for_tmp_file );
}

// ********** Functions for Creating the destination directory tree **********

static void RemoveDstDir( int dir_index, char *dst_dir )
/**********************************************/
{
    int         child;
    int         max_dirs = SimNumDirs();

    SimDirNoSlash( dir_index, dst_dir );
    if( access( dst_dir, F_OK ) != 0 ) return;
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
    if( path_len == 0 ) return;
    if( *end == '\\' || *end == '/' ) {
        *end = '\0';
    }
    if( path[0] == '\0' ) return;
    parent = alloca( strlen( drive ) + path_len + 10 ); // lotsa room
    strcpy( parent, drive );
    strcat( parent, path );
    MakeParentDir( parent, drive, path );
    mkdir( parent, 0777 );
}


static bool CreateDstDir( int i, char *dst_dir )
/**********************************************/
// check for directory existence.  If dir exists return TRUE.
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
    if( access( dst_dir, F_OK ) == 0 ) return( TRUE );          // check for existence
    MakeParentDir( dst_dir, drive, path );
    if( mkdir( dst_dir, 0777 ) == 0 ) return( TRUE );
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

static bool MatchEnd( char *path, char *end )
/*******************************************/
{
    int plen, endlen;

    plen = strlen( path );
    endlen = strlen( end );
    if( endlen > plen ) return( FALSE );
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
    struct stat         statbuf;
    char                full_path[_MAX_PATH];

    StatusAmount( 0,1 );
    StatusLines( STAT_CHECKING, path );
    if( StatusCancelled() ) return( FALSE );
    d = opendir( path );
    path_end = path+strlen(path);
    if( path_end > path && path_end[-1] == '/' ) --path_end;
    *path_end = '/';
    while( ( info = readdir( d ) ) ) {
        strcpy( path_end+1, info->d_name );
        _fullpath( full_path, info->d_name, _MAX_PATH );
        stat( full_path, &statbuf );
        if( S_ISDIR( statbuf.st_mode ) ) {
            if( info->d_name[0] != '.' ) {
                if( FindUpgradeFile( path ) ) return( TRUE );
            }
        } else {
            for( i = 0; i < upgrades; ++i ) {
                if( MatchEnd( path, SimGetUpgradeName( i ) ) ) {
                    return( TRUE );
                }
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
        if( !IsFixedDisk( disk[0] ) ) continue;
        strcpy( disk+1, ":\\" );
        StatusCancelled();
        if( FindUpgradeFile( disk ) ) return( TRUE );
    }
    return_state = DoDialog( "UpgradeNotQualified" );
    return( return_state != DLG_CAN && return_state != DLG_DONE );
}

extern bool CheckDrive( bool issue_message )
/******************************************/
//check if there is enough disk space
{
// For now assume that there will always be enough space to install the product until we learn how to find out how much
// available disk space there is.

    bool                ret;
    long                free_disk_space;
    long                disk_space_needed;
    long                max_tmp_file;
    int                 max_targs;
    int                 i, j, targ_num;
    char                disks[ MAX_DRIVES ];
    char                disk;
    gui_message_return  reply;
    char                buff[_MAX_PATH];
    char                drive[20];
    struct {
        char    drive;
        long    needed;
        long    max_tmp;
        long    free;
        int     num_files;
    } space[ MAX_DRIVES ];


    for( i = 0; i < MAX_DRIVES; ++i ) {
        space[i].drive = 0;
    }
    ret = TRUE;
    if( !SimCalcTargetSpaceNeeded() ) return( FALSE );
    max_targs = SimNumTargets();
    for( i = 0; i < max_targs; ++i ) {
        // get drive letter for each target
        disks[ i ] = tolower( *SimGetDriveLetter( i ) );
    }
    // check for enough disk space, combine drives that are the same
    for( i = 0; i < max_targs; ++i ) {
        if( islower( disks[ i ] ) || (disks[ i ] == '/') ) {
            targ_num = i;
            disk_space_needed = SimTargetSpaceNeeded( i );
            max_tmp_file = SimMaxTmpFile( i );
            for( j = i + 1; j < max_targs; ++j ) {
                if( disks[ j ] == disks[ i ] ) {
                    targ_num = j;
                    disk_space_needed += SimTargetSpaceNeeded( j );
                    if( SimMaxTmpFile( j ) > max_tmp_file ) {
                        max_tmp_file = SimMaxTmpFile( j );
                    }
                    disks[ j ] = toupper( disks[ j ] );
                }
            }
            free_disk_space = GetFreeDiskSpace( disks[i], FALSE );
            if( free_disk_space == -1 ) free_disk_space = 0;
            space[i].drive = disks[i];
            space[i].free = free_disk_space;
            space[i].needed = disk_space_needed;
            space[i].max_tmp = max_tmp_file;
            space[i].num_files = SimGetTargNumFiles( targ_num );
            if( free_disk_space < disk_space_needed + max_tmp_file ) {
                for( disk = 'c'; disk <= 'z'; ++disk ) {
                    if( disk == tolower( disks[ i ] ) ) continue;
                    if( !IsFixedDisk( disk ) ) continue;
                    if( GetFreeDiskSpace( disk, FALSE ) > max_tmp_file ) {
                        SimSetTargTempDisk( i, disk );
                        for( j = i + 1; j < max_targs; ++j ) {
                            if( tolower( disks[ j ] ) == disks[ i ] ) {
                                SimSetTargTempDisk( j, disk );
                            }
                        }
                        break;
                    }
                    if( disk == 'z' && issue_message ) {
                        MsgBox( NULL, "IDS_NODISKSPACE", GUI_OK,
                                max_tmp_file/1000 );
                        return( FALSE );
                    }
                }
            }
            if( issue_message ) {
                if( free_disk_space < disk_space_needed ) {
                    reply = MsgBox( NULL, "IDS_NODISKSPACE", GUI_YES_NO,
                                disks[ i ], free_disk_space/1000, disk_space_needed/1000 );
                    if( reply == GUI_RET_NO ) {
                        return( FALSE );
                    }
                }
            }
        }
    }
    for( i = 0; i < max_targs; ++i ) {
        strcpy( drive, "DriveFreeN" );
        if( space[i].drive != 0 && SimTargetNeedsUpdate( i ) ) {
            sprintf( buff, GetVariableStrVal( "IDS_DRIVE_SPEC" ), toupper( space[i].drive ) );
            if( space[i].needed < 0 ) {
                catnum( buff, -space[i].needed );
                strcat( buff, GetVariableStrVal( "IDS_DRIVE_FREED" ) );
            } else {
                catnum( buff, space[i].needed );
                strcat( buff, GetVariableStrVal( "IDS_DRIVE_REQUIRED" ) );
                catnum( buff, space[i].free );
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
/**************************************************/
{
    struct utimbuf      timebuf;

    timebuf.modtime = date;
    timebuf.actime = date;
    utime( dst_path, &timebuf );
}

static void SameFileDate( char *src_path, char *dst_path )
/****************************************************/
{
    struct stat         statblk;

    stat( src_path, &statblk );
    SetFileDate( dst_path, statblk.st_mtime );
}

// ********************Functions for Deleting Files***********************

extern bool DoDeleteFile( char *Path )
/**********************************/
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
/******************************************************************************/
{
#if 0
    const WORD          buffer_size = 32 * 1024;
    int                 src_files, dst_files;
    WORD                bytes_read, date, time, style, bytes_written;
    OFSTRUCT            src, dst;
    char _far           *pbuff;
    GLOBALHANDLE        mem_handle;

    src_files = OpenFile( src_path, &src, OF_READ );
    if( src_files == -1 ) return( CFE_CANTOPENSRC );

    mem_handle = GlobalAlloc( GMEM_MOVEABLE, buffer_size );
    if( mem_handle == NULL ) {
        _lclose( src_files );
        return( CFE_NOMEMORY );
    }

    if( append ) {
        style = OF_READWRITE;
    } else {
        style = OF_CREATE | OF_WRITE;
    }
    dst_files = OpenFile( dst_path, &dst, style );
    if( dst_files == -1 ) {
        _lclose( src_files );
        GlobalFree( mem_handle );
        return( CFE_CANTOPENDST );
    }
    if( append ) {
        _llseek( dst_files, 0, SEEK_END );
    }

    pbuff = GlobalLock( mem_handle );
    do {
        bytes_read = _lread( src_files, pbuff, buffer_size );
        bytes_written = _lwrite( dst_files, pbuff, bytes_read );
        BumpStatus( bytes_written );
        if( bytes_written != bytes_read || StatusCancelled() ) {
            _lclose( dst_files );
            _lclose( src_files );
            GlobalUnlock( mem_handle );
            GlobalFree( mem_handle );
            if( bytes_written == bytes_read ) {
                // copy was aborted, delete destination file
                DoDeleteFile( src_path );
                return( CFE_ABORT );
            }
            // error writing file - probably disk full
            SetupError( "IDS_WRITEERROR" );
            return( CFE_ERROR );
        }
    } while( bytes_read == buffer_size );
    GlobalUnlock( mem_handle );

    // Make the destination file have the same time stamp as the source file.
    _dos_getftime( src_files, (unsigned short *)&date, (unsigned short*)&time );
    _dos_setftime( dst_files, date, time );
    _lclose( dst_files );

    GlobalFree( mem_handle );
    _lclose( src_files );
#else
static char lastchance[1024];
    int           buffer_size = 16 * 1024;
    int                 src_files, dst_files;
#if 0
    int                 date, time;
#endif
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
        return( CFE_CANTOPENDST );
    }
    if( append ) {
        lseek( dst_files, 0, SEEK_END );
    }

    do {
        bytes_read = read( src_files, pbuff, buffer_size );
        bytes_written = write( dst_files, pbuff, bytes_read );

        #ifdef PATCH
            if( !IsPatch ) {
        #endif
                // if a patch, don't change status because denominator of status
                // fraction is the number of operations, not a number of bytes
                BumpStatus( bytes_written );
        #ifdef PATCH
            }
        #endif
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
#if 0
  #ifdef __OS2__
    {
        APIRET          rc;
        FILESTATUS3     srcinfobuff;
        FILESTATUS3     dstinfobuff;

        rc = DosQueryFileInfo( src_files, FIL_STANDARD, &srcinfobuff,
                                sizeof(FILESTATUS3) );
        if( rc !=0 ) {
            MsgBox( NULL, "IDS_QUERYFILEERROR", GUI_OK, rc);
        }
        rc = DosQueryFileInfo( dst_files, FIL_STANDARD, &dstinfobuff,
                                sizeof(FILESTATUS3) );
        if( rc !=0 ) {
            MsgBox( NULL, "IDE_QUERYFILEERROR", GUI_OK, rc);
        }
        dstinfobuff.fdateCreation = srcinfobuff.fdateCreation;
        dstinfobuff.ftimeCreation = srcinfobuff.ftimeCreation;

        rc = DosSetFileInfo( dst_files, FIL_STANDARD, &dstinfobuff,
                                sizeof(FILESTATUS3) );
        if( rc !=0 ) {
            MsgBox( NULL, "IDE_SETFILEERROR", GUI_OK, rc);
        }

    }
  #else
    _dos_getftime( src_files, (unsigned short *)&date, (unsigned short *)&time );
    _dos_setftime( dst_files, (unsigned short)date, (unsigned short)time );
  #endif
#endif
    close( dst_files );

    SameFileDate( src_path, dst_path );
    if( pbuff != lastchance ) GUIMemFree( pbuff );
    close( src_files );
#endif
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


static void GetSrcPath( char *src_path, char *file_name, int disk_num )
/********************************************************************/
{
    char        src_dir[ _MAX_DIR ];
    int         len;

    strcpy( src_dir, GetVariableStrVal( "SrcDir" ) );
    switch( SrcInstState ) {
    case SRC_CD:
        len = strlen( src_dir );
        if( len > 0 && src_dir[ len - 1 ] != '/' ) {
            src_dir[ len - 1 ] = '/';
            ++len;
        }
        sprintf( &src_dir[len], "diskimgs/disk%2.2u", disk_num+1 );
        break;
    case SRC_DISK:
        break;
    default:
        break;
    }
    _makepath( src_path, NULL, src_dir, file_name, NULL );
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

static int PromptUser( char *name, char *dlg, char *skip,
                        char *replace, int *value )
/*************************************************/
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

static int DoDecode( char *src, char *dst );
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
    char                file_ext[ _MAX_EXT ], *p;
    long                num_total_install;
    long                num_installed;
    unsigned            i;
    split_file          *split = NULL;
    split_file          **owner_split = &split;
    split_file          *junk;
    int                 num_splits = 0;
    int                 value;
    vhandle             var_handle;
    gui_message_return  ret = GUI_RET_OK;
    int                 max_files = SimNumFiles();
    dlg_state           return_state;

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
                    _splitpath( file_desc, NULL, NULL, NULL, file_ext );
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

        _splitpath( file_desc, NULL, NULL, NULL, file_ext );
        _makepath( dst_path, NULL, dir, file_desc, NULL );

        do {
            SimGetFileName( filenum, file_name );
            GetSrcPath( src_path, file_name, disk_num );
            if( StatusCancelled() ) {
                return( FALSE );
            }
            if( SimFileLastSplit( filenum ) ) {
                bool append;
                unsigned drive;

                copy_error = CFE_NOERROR;
                drive = *SimGetTargTempDisk( SimDirTargNum( SimFileDirNum( filenum ) ) );
                if( UseTargetForTmpFile( drive ) ) {
                    GetTmpFileNameInTarget( drive, tmp_path );
                } else {
                    GetTmpFileName( drive, tmp_path );
                }
                append = FALSE;
                i = 0;
                while( split != NULL ) {
                    junk = split;
                    split = split->next;
                    StatusLines( STAT_COPYINGFILE, junk->src_path );
                    for( ;; ) {
                        copy_error = DoCopyFile( junk->src_path, tmp_path, append );
                        if( copy_error == CFE_CANTOPENSRC ) {
                            SetVariableByName( "DiskDesc", junk->disk_desc );
                            return_state = DoDialog( "InsertDisk" );
                            if( return_state == DLG_CAN || return_state == DLG_DONE ) {
                                return( FALSE );
                            }
                        } else {
                            break;
                        }
                    }
                    ++i;
                    StatusAmount( num_installed + i * SimFileSize( filenum ) / num_splits, num_total_install );
                    if( StatusCancelled() ) return( FALSE );
                    append = TRUE;
                    if( copy_error != CFE_NOERROR ) break;
                    GUIMemFree( junk->src_path );
                    GUIMemFree( junk->disk_desc );
                    GUIMemFree( junk );
                }
                owner_split = &split;
                *owner_split = NULL;
                num_splits = 0;
                if( copy_error == CFE_NOERROR ) {
                    StatusLines( STAT_COPYINGFILE, file_desc );
                    for(;;) {
                        copy_error = DoCopyFile( src_path, tmp_path, TRUE );
                        if( copy_error == CFE_CANTOPENSRC ) {
                            SetVariableByName( "DiskDesc", disk_desc );
                            return_state = DoDialog( "InsertDisk" );
                            if( return_state == DLG_CAN || return_state == DLG_DONE ) {
                                return( FALSE );
                            }
                        } else {
                            break;
                        }
                    }
                    if( StatusCancelled() ) return( FALSE );
                    if( copy_error == CFE_NOERROR ) {
                        num_installed += SimFileSize( filenum );
                        StatusAmount( num_installed, num_total_install );
                        copy_error = DoDecode( tmp_path, dir );
                        remove( tmp_path );
                    }
                }
            } else if( SimFileSplit( filenum ) ) {
                *owner_split = GUIMemAlloc( sizeof( split_file ) );
                ++num_splits;
                GUIStrDup( src_path, &((*owner_split)->src_path) );
                GUIStrDup( disk_desc, &((*owner_split)->disk_desc) );
                owner_split = &((*owner_split)->next);
                *owner_split = NULL;
                copy_error = CFE_NOERROR;
            } else {
                StatusLines( STAT_COPYINGFILE, file_desc );
                copy_error = DoDecode( src_path, dir );
            }

            switch( copy_error ) {
            case CFE_ERROR:
                ret = GUI_RET_CANCEL;
                break;
            case CFE_BAD_CRC:
                MsgBox( NULL, "IDS_BADCRC", GUI_OK, src_path );
                ret = GUI_RET_CANCEL;
                break;
            case CFE_NOERROR:
                max_subfiles = SimNumSubFiles( filenum );
                for( subfilenum = 0; subfilenum < max_subfiles; ++subfilenum ) {
                    SimSubFileName( filenum, subfilenum, file_desc );
                    var_handle = SimSubFileVar( filenum, subfilenum );
                    _makepath( tmp_path, NULL, dir, file_desc, NULL );
                    SetVariableByHandle( var_handle, tmp_path );
                    UpdateCheckList( tmp_path, var_handle );
                }
                TransferCheckList();
                break;
            case CFE_NOMEMORY:
                ret = MsgBox( NULL, "IDS_NOMEMORYCOPY", GUI_RETRY_CANCEL );
                break;
            case CFE_CANTOPENSRC:
                SetVariableByName( "DiskDesc", disk_desc );
                return_state = DoDialog( "InsertDisk" );
                if( return_state == DLG_CAN || return_state == DLG_DONE ) {
                    return( FALSE );
                }
                break;
            case CFE_CANTOPENDST:
                ret = MsgBox( NULL, "IDS_CANTOPENDST", GUI_RETRY_CANCEL );
                break;
            default:
                break;
            }
            if( ret == GUI_RET_CANCEL ) return( FALSE );

        } while( copy_error != CFE_NOERROR );
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

//  TODO: Figure out what supplemental files will be applicable for the UNIX environment

}

extern  void DetermineSrcState( char *src_dir )
/*********************************************/
{
    int         len;

//  if( SrcInstState != SRC_UNKNOWN ) return;

    // if installing from CD or hard disk, add DISK# to source path
    len = strlen( src_dir );
    if( len > 0 && src_dir[ len - 1 ] != '/' ) {
        src_dir[ len ] = '/';
        src_dir[ len + 1 ] = '\0';
    }
    strcat( src_dir, "diskimgs/disk01" );
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
    if( !DoCopyFiles() ) return( FALSE );
    RemoveExtraFiles();
    RemoveUnusedDirs();
    StatusCancelled(); /* make sure display gets updated */
    return( TRUE );
}

#define isRdonly( m )   (((m) & S_IFMT) == S_IREAD)
#define isSystem( m )   !(((m) & S_IFMT) == S_IFREG)

static bool NukePath( char *path, int stat )
/******************************************/
{
    DIR                 *d;
    struct dirent       *info;
    char                *path_end;
    struct stat         statbuf;
    char                full_path[_MAX_PATH];

    d = opendir( path );
    path_end = path+strlen(path);
    if( path_end > path && path_end[-1] == '/' ) --path_end;
    *path_end = '/';
    while( ( info = readdir( d ) ) ) {
        strcpy( path_end+1, info->d_name );
        _fullpath( full_path, info->d_name, _MAX_PATH );
        if( S_ISDIR( statbuf.st_mode ) ) {
            if( info->d_name[0] != '.' ) {
                if( !NukePath( path, stat ) ) return( FALSE );
                rmdir( path );
            }
        } else {
            if( isRdonly( statbuf.st_mode ) || isSystem( statbuf.st_mode ) ) {
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

static char *GetInstallName()
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
    int                 len;
    int                 inst_len;
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
    char *              errormessage;
    va_list             arglist;


    errormessage = GetVariableStrVal( messageid );
    if( errormessage == NULL ) {
        strcpy( buff, GetVariableStrVal( "IDS_UNKNOWNERROR" ) );
    }
    else {
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
    result = GUIDisplayMessage( gui == NULL ? MainWnd : gui, buff, GetInstallName(), wType );
    return( result );
}


#ifdef PATCH
// *********************** DECODE Functions **********************************


static jmp_buf          PackFailed;


static int DoDecode( char *src, char *dst )
/*****************************************/
{
    arccmd              cmd;
    int                 len;
    wpackfile           files[ 1 ];

    // append trailing slash if not already there
    len = strlen( dst );
    if( dst[ len - 1 ] != '/' ) {
        dst[ len ] = '/';
        dst[ len + 1 ] = '\0';
    }
    cmd.u.path = dst;
    files[ 0 ].filename = NULL;
    files[ 0 ].packname = NULL;
    cmd.files = files;
    cmd.arcname = src;
    cmd.flags = PREPEND_PATH | BE_QUIET;
    DecodeError = CFE_NOERROR;
    if( setjmp( PackFailed ) == 0 ) {
        Decode( &cmd );
    } else {
        // we longjmp'd to here
    }
    return( DecodeError );
}

extern void PackExit( void );           // PackExit is defined further down in the file
void QueryCancel()
{
    if( StatusCancelled() ) {
        PackExit();
    }
}


// functions required for WPack Decoder


extern void *MemAlloc( int size )
/*******************************/
{
    void                *stg;

    stg = GUIMemAlloc( size );
    if( stg == NULL ) {
        SetupError( "IDS_NOMEMORY" );
    }
    return( stg );
}


extern void MemFree( void *mem )
/******************************/
{
    GUIMemFree( mem );
}


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
#else

static int DoDecode( char *src, char *dst )
{
    return( 1 );
}

#endif



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
        _makepath( name, drive, dir, fname, "._N_" );
        return( 1 );
    } else if( stricmp( ext, ".DLL" ) == 0 ) {
        NewFileToCheck( name, TRUE );
#ifdef EXTRA_CAUTIOUS_FOR_DLLS
        _makepath( name, drive, dir, fname, "._D_" );
#endif
        return( 1 );
    }
    return( 0 );
}

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

extern bool GetDirParams( int                   argc,
                          char **               argv,
                          char **               inf_name,
                          char **               tmp_path,
                          char **               arc_name )
/********************************************************/
{
    char                dir[ _MAX_DIR ];
    char                drive[ _MAX_DRIVE ];

    *inf_name = GUIMemAlloc( _MAX_PATH );
    if( *inf_name == NULL ) {
        return FALSE;
    }

    *tmp_path = GUIMemAlloc( _MAX_PATH );
    if( *tmp_path == NULL ) {
        GUIMemFree( *inf_name );
        return FALSE;
    }

    *arc_name = GUIMemAlloc( _MAX_PATH );
    if( *tmp_path == NULL ) {
        GUIMemFree( *inf_name );
        GUIMemFree( *tmp_path );
        return FALSE;
    }

    if( argc > 1 ) {
        strcpy( *arc_name, argv[1] );
    } else {
        strcpy( *arc_name, argv[0] );
    }

    if( argc > 2 ) {
        strcpy( *inf_name, argv[2] );
    } else {
        char        buff[_MAX_PATH];

        // If archive exists, expect setup.inf inside. Otherwise assume
        // it's right next to the setup executable.
        if( access( *arc_name, R_OK ) == 0 ) {
            strcpy( *inf_name, "setup.inf" );
        } else {
            _splitpath( argv[0], drive, dir, NULL, NULL );
            _makepath( buff, drive, dir, "setup", "inf" );
            _fullpath( *inf_name, buff, _MAX_PATH );
        }
    }

    if( argc > 3 ) {
        strcpy( *tmp_path, argv[3] );
    } else {
        _splitpath( *inf_name, drive, dir, NULL, NULL );
        _makepath( *tmp_path, drive, dir, NULL, NULL );
    }

    return TRUE;
}

extern bool FreeDirParams( char **              inf_name,
                           char **              tmp_path )
/********************************************************/
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
                if( name == NULL || ( name != NULL
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
    if( len > 0 && tmp_path[len-1] == '/' ) {
        tmp_path[len-1] = '\0';
    }
    SetVariableByName( "SrcDir2", tmp_path );
    _splitpath( inf_name, drive, dir, NULL, NULL );
    _makepath( tmp_path, drive, dir, "diskset", "inf" );

    ret = SimInit( inf_name );
    if( ret == SIM_INIT_NOERROR ) {
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
/****************************************/
{
    if( installed_ok ) {
        if( VarGetIntVal( UnInstall ) != 0 ) {
            MsgBox( NULL, "IDS_UNSETUPCOMPLETE", GUI_OK );
        } else {
            if( ConfigModified ) {

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
