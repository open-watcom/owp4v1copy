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


/*
*   COMMENTS:  Make a Microsoft BackOffice package with a .pdf file for SQL Anywhere,
*              and handle unattended install/uninstalls run by BackOffice by
*              making a .mif file in the windows directory.
*              Also support the creation and execution of a concurrent license package.
*/

// Only NT, Win95 and Win3.x supported for now.
#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) )
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <fcntl.h>
#include "gui.h"
#include "setupinf.h"
#include "setup.h"
#include "genvbl.h"
#include "gendlg.h"
#include "guiutil.h"
#include "utils.h"
#include "ctype.h"
#include "wpi.h"

extern bool CreateDstDir( int i, char *dst_dir );
#define RoundUp( v, r ) (((v)+(r)-1) & ~(unsigned long)((r)-1))

// Make sure BUILDVER is defined when compiled!
#include "dbbanner.h"

// the NUM_FILES_TO_CR.... constants are used for estimating space req.
#define NUM_FILES_TO_CREATE 3 // create: install,uninstall scripts & .pdf file

#define APP_FILENAME     "sqlany" _version_str_no_dot
#define MIF_NAME APP_FILENAME ".mif"
#define PDF_NAME APP_FILENAME ".pdf"
#define PCT_NAME APP_FILENAME ".pct"
#define INSTALL          "Install"    // option name in .pdf
#define UNINSTALL        "UnInstall"  // option name in .pdf
#define INSTALL_SCRIPT   "inst.sms"  // sms = Systems Management Server
#define UNINSTALL_SCRIPT "uninst.sms"
#define DEFAULT_APPNAME  "Sybase SQL Anywhere" // if appname not in setup.inf

// Below: A '!' before a file name means that detailed info
// about the filename is to be recorded, other than just the name.
#define DLM ":" // delimiter: put this between filenames in the checklists below
#define CHECKLIST  "!setup.exe" DLM "!setup.inf" DLM INSTALL_SCRIPT

typedef struct {
    char                name[ _MAX_FNAME + _MAX_EXT ];
    char                collect[ 8 ]; // more than enough to hold "TRUE" or "FALSE"
    long                size;
    char                date[ 12 ];  // more than enough for the date: "MM, DD, YY"
    char                time[ 8 ];  // more than enough for the time: "HH, MM" (24 hour)
    unsigned short      byte_val;
    long                byte_location;
    unsigned int        checksum_val;
    long                checksum_start;
    long                checksum_length;
    unsigned long       long_val;
    long                long_location;
    WORD                word_val;
    long                word_location;
    bool                want_detail;
} pdf_info;

//***********************************************************************************
//**************************************************** Package Execution Functions **
//***********************************************************************************

extern void WriteMIFforMSBackOffice( bool success, char *message )
/****************************************************************/
{
// This gets placed in the windows directory.
// The time and date stamp must be after the setup program started... Keep that
// in mind if you decide to copy the .mif instead of create it here.

    FILE        *fp;
    char        filename[ _MAX_PATH ];

    if( message == NULL || message[ 0 ] == '\0' ) {
        return;
    }
    GetWindowsDirectory( filename, sizeof( filename ) );
    if( filename[ strlen( filename ) - 1 ] != '\\' ) {
        strcat( filename, "\\" );
    }
    strcat( filename, MIF_NAME );

    fp = fopen( filename, "wt+" );
    if( fp == NULL ) {
        return;
    }

    // message should be 128 chars or less (including null char at end).
    // or else it will be truncated here...
    if( strlen( message ) > 127 ) {
        message[ 127 ] = '\0';
    }

    fprintf( fp, "Start Component\n"
                 "Name = \"Workstation\"\n"
                 "Start Group\n"
                 "Name = \"InstallStatus\"\n"
                 "ID = 1\n"
                 "Class = \"MICROSOFT|JOBSTATUS|1.0\"\n"
                 "Start Attribute\n"
                 "Name = \"Status\"\n"
                 "ID = 1\n"
                 "Type = String(16)\n"
                 "Value = \"%s\"\n"
                 "End Attribute\n"
                 "Start Attribute\n"
                 "Name = \"Description\"\n"
                 "ID = 2\n"
                 "Type = String(128)\n"
                 "Value = \"%s\"\n"
                 "End Attribute\n"
                 "End Group\n"
                 "End Component\n",
                 success ? "SUCCESS" : "FAILED", message );

    fclose( fp );
    // since we have written the status file, we are done
    // therefore, delete the percentage file
    BkOfficePercent( -1 );
}

void BkOfficePercent( int percent )
/**********************************

    Write percentage complete out to a file for communication with
    Optima or PowerBuilder install.
*/
{
    FILE                *fp;
    char                filename[ _MAX_PATH ];

    GetWindowsDirectory( filename, sizeof( filename ) );
    if( filename[ strlen( filename ) - 1 ] != '\\' ) {
        strcat( filename, "\\" );
    }
    strcat( filename, PCT_NAME );

    if( percent == -1 ) {
        // delete the file
        remove( filename );
    } else {
        // write percentage out to file
        fp = fopen( filename, "wt+" );
        if( fp == NULL ) {
            return;
        }
        fprintf( fp, "%d%%\n", percent );
        fclose( fp );
    }
}

//***********************************************************************************
//***************************************************** Package Creation Functions **
//***********************************************************************************
static void DumpVariables( FILE *fp )
/***********************************/
{
// Dump out variables set in dialogs by the person creating the package.
    int  var_handle;
    char *var_name;

    if( fp == NULL ) {
        return;
    }
    for( var_handle = 0 ; GetVariableById( var_handle ) != NO_VAR; var_handle++ ) {
        var_name = VarGetName( var_handle );
        if( var_name != NULL
        && VariableNeedsToBeInScriptFile( var_handle ) ) {
            fprintf( fp, "%s=%s\n", var_name, VarGetStrVal( var_handle ) );
        }
    }
}


static bool WriteScriptsForMSBackOffice( char *dst_path )
/*******************************************************/
{
// Write script files to be used for unattended installs of uninstalls by BackOffice
// The variabls set in the install script were chosen by the user creating
// the BackOffice package through the setup dialogs.
    FILE        *fp;
    char        path[ _MAX_PATH ];
    char        filename[ _MAX_PATH ];

    strcpy( path, dst_path );
    if( path[ strlen( path ) - 1 ] != '\\' ) {
        strcat( path, "\\" );
    }
    StatusShow( TRUE );

    // Install script file
    strcpy( filename, path );
    strcat( filename, INSTALL_SCRIPT );

    StatusLines( STAT_CREATING_FILE, filename );

    fp = fopen( filename, "wt+" );
    if( fp == NULL ) {
        return( FALSE );
    }

    fprintf( fp, "# Script for Automated Install\n"
                 "Install=1\n"
                 "UnInstall=0\n"
                 "MakeDisks=0\n"
                 "MakePackage=0\n"   // Like choosing install on the HiThere dialog
                 "DoLicenseNLM=0\n" );// <-- because this is set true in the
                                      // LicenseAllWelcome dialog by default
                                      // (variables are still set by skipped
                                      //  dialogs) [FOR LICENSE PACKAGES]

    // Dump out variables set in dialogs by the person creating the package.
    DumpVariables( fp );
    fclose( fp );

    // UnInstall script file
    strcpy( filename, path );
    strcat( filename, UNINSTALL_SCRIPT );

    StatusLines( STAT_CREATING_FILE, filename );
    StatusShow( TRUE );

    fp = fopen( filename, "wt+" );
    if( fp == NULL ) {
        return( FALSE );
    }

    fprintf( fp, "# Script for Automated UnInstall\n"
                 "Install=0\n"
                 "UnInstall=1\n"
                 "MakeDisks=0\n"
                 "MakePackage=0\n" ); // Like choosing uninstall on the HiThere dialog

    fclose( fp );

    return( TRUE );
}
static bool GetFileInfoForPDF( char *src_path, pdf_info *file )
/************************************************************/
{
    struct stat buf;
    char        fullpath[ _MAX_PATH ];
    int         handle;
    char        *buffer;
    int         buffer_size;
    int         amount_read;
    int         i;

    if( !file->want_detail ) {
        return( TRUE );
    }
    strcpy( fullpath, src_path );
    if( fullpath[ strlen( fullpath ) - 1 ] != '\\' ) {
        strcat( fullpath, "\\" );
    }
    strcat( fullpath, file->name );
    if( stat( fullpath, &buf ) != 0 ) {
        close( handle );
        return( FALSE );
    }
    handle = open( fullpath, O_RDONLY | O_BINARY );
    if( handle == -1 ) {
        return( FALSE );
    }
    // COLLECT
    strcpy( file->collect, "FALSE" );

    // SIZE
    file->size = buf.st_size;

    // TIME
    strftime( file->time, sizeof( file->time ), "%H, %M", localtime( &buf.st_mtime ) );

    // DATE
    strftime( file->date, sizeof( file->date ), "%m, %d, %y" , localtime( &buf.st_mtime ) );

    // BYTE
    file->byte_location = lseek( handle, file->size / 3, SEEK_SET );
    if( file->byte_location == -1 ) {
        close( handle );
        return( FALSE );
    }
    read( handle, (char *) &file->byte_val, 1 );

    // WORD
    file->word_location = lseek( handle, file->size * 4 / 5, SEEK_SET );
    if( file->word_location == -1 ) {
        close( handle );
        return( FALSE );
    }
    read( handle, (char *) &file->word_val, sizeof( file->word_val ) );

    // LONG
    file->long_location = lseek( handle, file->size * 2 / 3, SEEK_SET );
    if( file->long_location == -1 ) {
        close( handle );
        return( FALSE );
    }
    read( handle, (char *) &file->long_val, sizeof( file->long_val ) );

    // CHECKSUM
    file->checksum_start = lseek( handle, 0, SEEK_SET );
    buffer_size = min( file->size, 4096 );
    buffer = malloc( buffer_size );
    if( buffer == NULL ) {
        close( handle );
        return( FALSE );
    }
    file->checksum_length = file->size;
    file->checksum_val = 0;
    for( ;; ) {
        amount_read = read( handle, buffer, buffer_size );
        for( i = 0; i < amount_read; i++ ) {
            file->checksum_val += (int) buffer[ i ];
        }
        if( amount_read < buffer_size ) {
            break;
        }
    }

    close( handle );
    return( TRUE );
}

static bool WritePDFforMSBackOffice( char *dest_path, char *src_path )
/******************************************************************************/
{

    FILE        *fp;
    char        setupinf[ _MAX_PATH ];
    char        fullpath[ _MAX_PATH ];
    char        appname[ 128 ];
    char        platforms[ 1024 ];
    char        *filelist;
    char        *filename;
    char        *index;
    char        delimiter[ 1 ];
    pdf_info    file;
    int         detection_rule;
    int         i;

    strcpy( fullpath, dest_path );
    if( fullpath[ strlen( fullpath ) - 1 ] != '\\' ) {
        strcat( fullpath, "\\" );
    }
    strcat( fullpath, PDF_NAME );

    StatusLines( STAT_CREATING_FILE, fullpath );
    StatusShow( TRUE );

    fp = fopen( fullpath, "wt+" );
    if( fp == NULL ) {
        return( FALSE );
    }

    if( GetVariableIntVal( "IsWin95" ) == 1 ) {
        strcpy( platforms, "Windows95" );
    } else if( GetVariableIntVal( "IsWin" ) == 1 ) {
        strcpy( platforms, "Windows 3.1" );
    } else if( GetVariableIntVal( "IsWinNT" ) == 1 ) {
        // Its strange, but the 3.1 below is expected by MSBackOffice
        // The entry without the 3.1 is just there for looks.
        strcpy( platforms, "Windows NT (x86), Windows NT 3.1 (x86)" );
    } else {
        strcpy( platforms, "Could not determine platform!" );
    }

    // Find out the application name.
    strcpy( setupinf, src_path );
    if( setupinf[ strlen( setupinf ) - 1 ] != '\\' ) {
        strcat( setupinf, "\\" );
    }
    strcat( setupinf, "setup.inf" );
    GetPrivateProfileString( "Application", "AppName",
                             DEFAULT_APPNAME, // <- used if no name found
                             appname, sizeof( appname ), setupinf );

    // Write the .pdf file's contents

    fprintf( fp, "[PDF]\n"
                 "Version=1.0\n"
                 "\n" );

    fprintf( fp, "[" INSTALL " Setup]\n"
                 "CommandName=Automated " INSTALL "\n"
                 "CommandLine=setup.exe -b -f=" INSTALL_SCRIPT "\n"
                 "UserInputRequired=FALSE\n"
                 "SynchronousSystemExitRequired=FALSE\n"
                 "SupportedPlatforms=%s\n"
                 "\n", platforms );

    fprintf( fp, "[" UNINSTALL " Setup]\n"
                 "CommandName=Automated " UNINSTALL "\n"
                 "CommandLine=setup.exe -b -f=" UNINSTALL_SCRIPT "\n"
                 "UserInputRequired=FALSE\n"
                 "SynchronousSystemExitRequired=FALSE\n"
                 "SupportedPlatforms=%s\n"
                 "\n", platforms );

    fprintf( fp, "[Package Definition]\n"
                 "Product=Sybase SQL Anywhere\n"
                 "Version=" _version_str_banner PATCH_VER "\n"
                 "Comment=%s\n"
                 "SetupVariations=" INSTALL ", " UNINSTALL "\n"
                 "WorkStationAccess=UserRead, UserWrite, GuestRead\n"
                 "Company=Sybase, Inc.\n"
                 "\n", appname );

    fprintf( fp, "[Setup Package for Inventory]\n"
                 "InventoryThisPackage=TRUE\n" );

    // Write info about files to be "inventoried" by MSBackoffice in the .pdf file

    // CHECKLIST should be #defined above somewhere
    filelist = strdup( CHECKLIST );

    if( filelist == NULL ) {
        fclose( fp );
        return( FALSE );
    }
    index = filelist;
    strcpy( delimiter, DLM );
    if( *index != '\0' ) {
        i = 1;
        detection_rule = 1;
        fprintf( fp, "Detection Rule Part 1=File1\n" );
        index = strchr( filelist, *delimiter );
        while( index != NULL ) {
            detection_rule++;
            fprintf( fp, "Detection Rule Part %d=AND\n", detection_rule );
            detection_rule++;
            i++;
            fprintf( fp, "Detection Rule Part %d=File%d\n", detection_rule, i );
            index++;
            index = strchr( index, *delimiter );
        }
    }
    free( filelist );
    fprintf( fp, "\n" );

    filelist = strdup( CHECKLIST );

    if( filelist == NULL ) {
        fclose( fp );
        return( FALSE );
    }

    filename = strtok( filelist, delimiter );
    i = 1;
    while( filename != NULL ) {
        if( *filename == '!' ) {      // put detailed info about this file in the .pdf
            file.want_detail = TRUE;
            filename++;
        } else {
            file.want_detail = FALSE;   // or else put minimal info in the .pdf file
        }
        strcpy( file.name, filename );

        if( !GetFileInfoForPDF( src_path, &file ) ) {
            return( FALSE );
        }

        if( file.want_detail ) {
            fprintf( fp, "[File%d]\n"
                         "FILE=%s\n"
                         "COLLECT=%s\n"
                         "Checksum=%lu, %lu, %d\n"
                         "CRC=\n"
                         "DATE=%s\n"
                         "SIZE=%lu\n"
                         "TIME=%s\n"
                         "BYTE=%lu, %d\n"
                         "WORD=%lu, %lu\n"
                         "LONG=%lu, %lu\n"
                         "TOKEN1=\n"
                         "TOKEN2=\n"
                         "TOKEN3=\n"
                         "TOKEN4=\n"
                         "\n",
                         i, file.name, file.collect,
                         file.checksum_start, file.checksum_length, file.checksum_val,
                         file.date, file.size, file.time,
                         file.byte_location, file.byte_val,
                         file.word_location, file.word_val,
                         file.long_location, file.long_val );
        } else {
            fprintf( fp, "[File%d]\n"
                         "FILE=%s\n"
                         "COLLECT=%s\n"
                         "Checksum=\n"
                         "CRC=\n"
                         "DATE=\n"
                         "SIZE=\n"
                         "TIME=\n"
                         "BYTE=\n"
                         "WORD=\n"
                         "LONG=\n"
                         "TOKEN1=\n"
                         "TOKEN2=\n"
                         "TOKEN3=\n"
                         "TOKEN4=\n"
                         "\n",
                         i, file.name, file.collect );
        }
        filename = strtok( NULL, DLM );
        i++;
    }
    free( filelist );
    fclose( fp );
    return( TRUE );
}


static long NukePathWithStatusBar( char *dst_path, int stat, long size, bool root )
/*********************************************************************************/
// There is a NukePath function in utils.c,
// but I wanted it to support a status bar, hence this.
//
// When called, pass in the space that this deletion will free in bytes,
// (taking into account cluster size), as the size parameter.
// The 'root' parameter must be TRUE.
// Returns: size deleted, in bytes
//      OR  -1 if an error occurs
//      OR  -2 if the user aborted.
{
    DIR                 *d;
    struct dirent       *info;
    char                *path_end;
    char                path[ _MAX_PATH ];
    static long         total_size;
    static long         cluster_size;

    strcpy( path, dst_path );
    if( root ) {
        total_size = size;
        size = 0;
        cluster_size = ClusterSize( path );
        if( cluster_size == 0 ) {
            return( -1 );
        }
        StatusLines( stat, "" );
        StatusAmount( 0, 1 );
        StatusShow( TRUE );
    }
    d = opendir( path );
    path_end = path+strlen(path);
    if( path_end > path && path_end[-1] == '\\' ) --path_end;
    *path_end = '\\';
    while( info = readdir( d ) ) {
        if( StatusCancelled() ) {
            return( -2 );
        }
        strcpy( path_end+1, info->d_name );
        if( info->d_attr & _A_SUBDIR ) {
            if( info->d_name[0] != '.' ) {
                size = NukePathWithStatusBar( path, stat, size, FALSE );
                if( size < 0 ) {
                    return( size );
                }
                rmdir( path );
            }
        } else {
            if( info->d_attr & (_A_RDONLY+_A_SYSTEM+_A_HIDDEN ) ) {
                chmod( path, S_IWRITE );
            }
            if( remove( path ) != 0 ) {
                MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
                return( -1 );
            }
            size += RoundUp( info->d_size, cluster_size );
        }
        StatusLines( stat, path );
        StatusAmount( size, total_size );
    }
    *path_end = '\0';
    closedir( d );

    if( root ) {
        StatusAmount( 1, 1 );
        StatusShow( TRUE );
    }

    return( size );
}

static long SpaceRequiredForDirContents( char *src_path, char *dst_path,
                                         long size, bool root )
/***********************************************************************/
{
// When called, pass 0 in as the size parameter, and make the root parameter TRUE.
// Returns: size taken up in destination path in bytes (taking into account cluster
//          size), if the directory tree under src_path were to be copied
//      OR  -1 if an error occurs
//      OR  -2 if the user aborted.
    DIR                 *dirp;
    struct dirent       *direntp;
    char                tmp[ _MAX_PATH ];
    char                src[ _MAX_PATH ];
    static long         cluster_size;

    if( root ) {
        cluster_size = ClusterSize( dst_path );
        if( cluster_size == 0 ) {
            return( -1 );
        }
    }
    StatusLines( STAT_CHECKINGPATH, "" );
    strcpy( tmp, src_path );
    if( tmp[ strlen( tmp ) - 1 ] != '\\' ) {
        strcat( tmp, "\\" );
    }
    strcat( tmp, "*.*" );
    dirp = opendir( tmp );
    strcpy( tmp, src_path );
    if( tmp[ strlen( tmp ) - 1 ] != '\\' ) {
        strcat( tmp, "\\" );
    }

    if( dirp == NULL ) {
        return( size );
    } else {
        for( ;; ) {
            direntp = readdir( dirp );
            if( direntp == NULL ) {
                break;
            }
            if( direntp->d_name[ 0 ] != '.' ) {
                strcpy( src, tmp );
                strcat( src, direntp->d_name );
                if( direntp->d_attr & 0x10 ) {
                    size = SpaceRequiredForDirContents( src, dst_path, size, FALSE );
                    if( size < 0 ) {
                        closedir( dirp );
                        return( size );
                    }
                } else {
                    size += RoundUp( direntp->d_size, cluster_size );
                }
            }
            if( StatusCancelled() ) {
                return( -2 );
            }
        }
        closedir( dirp );
    }
    return( size );
}

static long CopyDirContents( char *dst_path, char *src_path, long size, bool root )
/*********************************************************************************/
{
// When called, pass in the size the directory contents will occupy when copied
// in bytes, (taking into account cluster size), as the size parameter.
// The 'root' parameter must be TRUE.
// Returns: size taken up in destination path, in bytes
//      OR  -1 if an error occurs
//      OR  -2 if the user aborted.
    DIR                 *dirp;
    struct dirent       *direntp;
    char                tmp_src[ _MAX_PATH ];
    char                tmp_dst[ _MAX_PATH ];
    char                src[ _MAX_PATH ];
    char                dst[ _MAX_PATH ];
    static long         total_size;
    static long         cluster_size;
    COPYFILE_ERROR      ret;

    if( root ) {
        cluster_size = ClusterSize( dst_path );
        if( cluster_size == 0 ) {
            return( -1 );
        }
        total_size = size;
        size = 0;

        StatusShow( TRUE );
        StatusLines( STAT_COPYINGFILE, "" );
        StatusAmount( 0, 1 );
    }

    strcpy( tmp_src, src_path );
    if( tmp_src[ strlen( tmp_src ) - 1 ] != '\\' ) {
        strcat( tmp_src, "\\" );
    }
    strcat( tmp_src, "*.*" );
    dirp = opendir( tmp_src );
    strcpy( tmp_src, src_path );
    if( tmp_src[ strlen( tmp_src ) - 1 ] != '\\' ) {
        strcat( tmp_src, "\\" );
    }
    strcpy( tmp_dst, dst_path );
    if( tmp_dst[ strlen( tmp_dst ) - 1 ] != '\\' ) {
        strcat( tmp_dst, "\\" );
    }

    if( dirp == NULL ) {
        return( -1 );
    } else {
        for( ;; ) {
            direntp = readdir( dirp );
            if( direntp == NULL ) {
                break;
            }
            if( direntp->d_name[ 0 ] != '.' ) {
                strcpy( dst, tmp_dst );
                strcat( dst, direntp->d_name );
                strcpy( src, tmp_src );
                strcat( src, direntp->d_name );
                if( direntp->d_attr & 0x10 ) { // directory
                    if( mkdir( dst ) != 0 ) {
                        MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
                        closedir( dirp );
                        return( -1 );
                    }
                    size = CopyDirContents( dst, src, size, FALSE );
                    if( size < 0 ) {
                        closedir( dirp );
                        return( size );
                    }
                } else {                       // file
                    StatusLines( STAT_COPYINGFILE, dst );
/* BUG that hasn't been fixed yet:  the StatusLines call above (and probably in other
   places in this file) is delayed by 1 output.
   If files are copied in this order: file1 file2, then when file2 is being copied,
   the status line will display file1.
*/
                    StatusAmount( size, total_size );
                    ret = DoCopyFile( src, dst, FALSE );
                    if( ret == CFE_ABORT ) {
                        return( -2 );
                    } else if( ret != CFE_NOERROR ) {
                        if( ret == CFE_CANTOPENDST ) {
                            MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
                        }
                        return( -1 );
                    } else {
                        size += RoundUp( direntp->d_size, cluster_size );
                    }
                }
            }
            if( StatusCancelled() ) {
                return( -2 );
            }
        }
        closedir( dirp );
    }

    return( size );
}

extern bool MakePackageDir()
/**************************/
{
    gui_message_return  ret;
    char                dst_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                root[ _MAX_PATH ];
    long                free_space;
    long                space_req;
    long                extra_space;
    long                size_to_delete;
    long                size;
    int                 files_to_create;

    StatusLines( STAT_CHECKINGPATH, "" );
    StatusShow( TRUE );

    strcpy( src_path, GetVariableStrVal( "SrcDir" ) );
    if( src_path[ strlen( src_path ) - 1 ] != '\\' ) {
        strcat( src_path, "\\" );
    }

    strcpy( dst_path, GetVariableStrVal( "PackageDir" ) );
    if( *dst_path == '\0' ) {
        MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
        return( FALSE );
    }
    if( ( isalpha( *dst_path ) == 0           // dst_path start as either: "x:"
        || dst_path[ 1 ] != ':' )             // where x is a drive letter, or
        && ( dst_path[ 0 ] != '\\'            // it must start with "\\" (UNC)
        || dst_path[ 1 ] != '\\' ) ) {
        MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
        return( FALSE );
    }

    GetRootFromPath( root, dst_path );

    size_to_delete = SpaceRequiredForDirContents( dst_path, dst_path, 0, TRUE );
    if( size_to_delete == -2 ) {  //user abort
        return( FALSE );
    }
    free_space = FreeSpace( dst_path ) + size_to_delete;

    files_to_create = NUM_FILES_TO_CREATE;

    space_req = SpaceRequiredForDirContents( src_path, dst_path, 0, TRUE )
    // (very safe) assumption: the files to be created will each be under 4k...
    // ( .pdf file and script file(s) )
                + max( ClusterSize( dst_path ) * files_to_create, 4096 );

    if( space_req == -2 ) { // user abort
        return( FALSE );
    }
    if( free_space == -1 ) {  // error occured
        ret = MsgBox( NULL, "IDS_BAD_PATH", GUI_OK, dst_path );
        return( FALSE );
    }
    if( space_req > free_space ) {
        extra_space = space_req - free_space;
        if( size_to_delete > 0 ) {
            // even if dst_path's contents are deleted there isn't enough space
            ret = MsgBox( NULL, "IDS_NOSPACE_AFTER_DELETING", GUI_YES_NO,
                          space_req / 1024, dst_path, free_space / 1024,
                          extra_space, extra_space / 1024 );
        } else {
            // there just isn't enough space (dst_path may not exist yet)
            ret = MsgBox( NULL, "IDS_NOTENOUGHSPACE", GUI_YES_NO,
                          space_req / 1024, dst_path, free_space / 1024,
                          extra_space, extra_space / 1024 );
        }

        if( ret == GUI_RET_NO ) {
            return( FALSE );
        }
    }

    if( CreateDstDir( 0, dst_path ) ) {
        if( access( dst_path, F_OK ) == 0   // path exists
        && size_to_delete > 0 ) {
            ret = MsgBox( NULL, "IDS_EMPTY_DIR", GUI_YES_NO, dst_path );
            if( ret == GUI_RET_YES ) {
                StatusShow( TRUE );
                size = NukePathWithStatusBar( dst_path, STAT_DELETEFILES,
                                              size_to_delete, TRUE );
                if( size < 0 ) {
                    return( FALSE );
                }
            } else {
                return( FALSE );
            }
        }
    } else {
        return( FALSE );
    }

    size = CopyDirContents( dst_path, src_path, space_req, TRUE );
    if( size < 0 ) {
        if( size == -1 ) { // error   ( -2 is a user abort )
            MsgBox( NULL, "IDS_FAILED_COPYING", GUI_OK, dst_path );
        }
        return( FALSE );
    }

    if( !WriteScriptsForMSBackOffice( dst_path ) ) {
        MsgBox( NULL, "IDS_SCRIPT_FAILED", GUI_OK );
        return( FALSE );
    }
    if( WritePDFforMSBackOffice( dst_path, src_path ) ) {
        StatusLines( STAT_BLANK, "" );
        StatusAmount( 1, 1 );
        MsgBox( NULL, "IDS_PACKAGE_CREATED", GUI_OK, dst_path, PDF_NAME );
        return( TRUE );
    } else {
        MsgBox( NULL, "IDS_PDF_FAILED", GUI_OK );
        return( FALSE );
    }
}
#endif  // #ifdef at top
