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
* Description:  Installer support for licensing/branding.
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#if !defined( __UNIX__ )
  #include <dos.h>
#endif
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
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
#include "standard.h"
#include "license.h"
#include "dbparms.h"

extern char     *VariablesFile;
extern void     ReadVariablesFile( char * );
extern a_bool   ReadBlock( char *, char *, void *, long );
extern a_bool   WriteBlock( char *, char *, void *, long );
#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
extern int      MSBackOffice;
#endif

static bool is_null_name( char *str )
/***********************************/
{
    if( str == NULL ) return( TRUE );
    for( ; isspace( *str ); ++str );
    if( *str == '\0' ) return( TRUE );
    return( FALSE );
}


extern bool LicenseFileName( char *full_path )
/********************************************/
{
    char                *p;
    char                lic_file[ _MAX_FNAME ];
    char                lic_ext[ _MAX_EXT ];
    char                lic_dir[ _MAX_DIR ];
    char                lic_drive[ _MAX_DRIVE ];

    lic_file[ 0 ] = '\x0';
    p = GetVariableStrVal("LicenseFile");
    if( p == NULL ) {
        return( FALSE );
    }
    ReplaceVars( full_path, p );
    _splitpath( full_path, NULL, NULL, lic_file, lic_ext );
    if( lic_file[ 0 ] == '\x0' ) return FALSE;
    p = GetVariableStrVal( "LicenseExe" );
    if( p == NULL ) {
        return( FALSE );
    }
    ReplaceVars( full_path, p );
    _splitpath( full_path, lic_drive, lic_dir, NULL, NULL );
    _makepath( full_path, lic_drive, lic_dir, lic_file, lic_ext );
    if( full_path[ 0 ] == '\x0' ) return FALSE;

    return TRUE;
}

extern bool ApplyLicense(void)
/****************************/
{
    char                *p;
    char                *licname;
    char                *liccompname;
    char                *err;
    int                 tryagain;
    char                drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ];
    char                lic_file[ _MAX_PATH ];
    char                lic_file_path[ _MAX_PATH ];
    char                lic_exe_path[ _MAX_PATH ];
    char                licstr[ 32 ];           // SQl aNY Platform ???????
    struct stat         stat_buf;
    bool                alreadylicensed;
    dlg_state           return_state;
#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
    char                sms_ini[ _MAX_PATH ];
    char                name[ 80 ];
    char                company[ 80 ];
#endif


#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
    if( GetVariableIntVal( "ApplyLicense" ) != 1
    && GetVariableIntVal( "$IsLicConCur" ) == 1
    && MSBackOffice ) {
        return( TRUE );
    }
    if( GetVariableIntVal( "ApplyLicense" ) == 1 && MSBackOffice ) {
        SetVariableByName( "$IsLicConCur", "1" );
    }
#endif
    alreadylicensed = FALSE;
    p = GetVariableStrVal("LicenseExe");
    if( p == NULL ) {
        return( TRUE );
    }
    ReplaceVars( lic_exe_path, p );
    p = GetVariableStrVal("LicenseFile");
    if( p == NULL ) {
        return( TRUE );
    }
    ReplaceVars( lic_file, p );

    // if this is part of a server install, and not just a license install,
    // put up a dialog to tell the user what is happening
    if( GetVariableIntVal( "DoLicenseIntro" ) == 1 ) {
        return_state = DoDialog( "LicenseIntro" );
        if( return_state == DLG_CAN || return_state == DLG_DONE ) {
            return( FALSE );
        }
    }

    // determine license method
    if( GetVariableIntVal( "$IsLicConCur" ) != 0 ) {
        // Concurrent license (requires license disk)
        // if installing from CD, the prompt for the license disk also
        // points to the CD, instead of a disk drive
        if( GetVariableIntVal( "SrcIsCD" ) == 1 ) {
            SetVariableByName( "SrcDir", "a:\\" );      // assume A:
        }
        _makepath( lic_file_path, NULL, GetVariableStrVal("SrcDir"), lic_file, NULL );
    } else {
        // Per Seat license
        // need to build license file (if not already there)
        _splitpath( lic_exe_path, drive, dir, NULL, NULL );
        _makepath( lic_file_path, drive, dir, lic_file, NULL );
        if( stat( lic_file_path, &stat_buf ) != 0 ) {
            // build license file
            strcpy( licstr, "SQl aNY Platform " );
            strcat( licstr, GetVariableStrVal( "LicPlat" ) );
            make_license( lic_file_path, licstr, 10000, 10000, LICENSE_TYPE_PERSEAT );
        }
    }

    for( ;; ) {
        err = find_license_names( lic_file_path, &licname, &liccompname, &tryagain );
        if( err == NULL ) break;
        if( !tryagain ) {
            MsgBox( NULL, "IDS_ERROR", GUI_OK, err );
            return( FALSE );
        }
        SetVariableByName( "DiskDesc", "License Disk" );
        return_state = DoDialog( "InsertDisk" );
        if( return_state == DLG_CAN || return_state == DLG_DONE ) {
            return( FALSE );
        }
        _makepath( lic_file_path, NULL, GetVariableStrVal("SrcDir"), lic_file, NULL );
    }
    if( licname != NULL ) {
        alreadylicensed = TRUE;
    }
#if defined( WSQL ) && ( defined( WINNT ) || defined( WIN ) ) // Microsoft BackOffice
        // For MSBackOffice, with per-seat licensing, get the user name
        // and company name from c:\SMS.ini.  If that fails, do not license.

        if( !alreadylicensed ) {
            if( MSBackOffice ) {
                strcpy( sms_ini, "c:\\sms.ini" );
                GetPrivateProfileString( "Local", "UserName", "",
                                         name, sizeof( name ), sms_ini );
                GetPrivateProfileString( "Local", "CompanyName", "",
                                         company, sizeof( company ), sms_ini );
                if( *name == '\0' || *company == '\0' ) {
                    MsgBox( NULL, "IDS_NO_LICENSEINFO", GUI_OK );
                    return( FALSE );
                }
                licname = name;
                liccompname = company;
            } else {
#if defined( WINNT )
                HKEY    key_handle;
                long    result;
                DWORD   size;
                DWORD   size_copy;
                char    *reg_name;
                char    *reg_company;
                result = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                       "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                                       0L,
                                       KEY_READ,
                                       &key_handle );
                for( ;; ) {
                    if( result != ERROR_SUCCESS ) {
                        break;
                    }

                    // Get longest string length for values in key
                    result = RegQueryInfoKey( key_handle,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              &size,
                                              NULL,
                                              NULL );

                    size_copy = size;
                    if( result != ERROR_SUCCESS ) {
                        RegCloseKey( key_handle );
                        break;
                    }
                    reg_name = malloc( size + 1 );
                    reg_company = malloc( size + 1 );
                    if( reg_name == NULL || reg_company == NULL ) {
                        RegCloseKey( key_handle );
                        break;
                    }
                    result = RegQueryValueEx( key_handle,
                                              "RegisteredOwner",
                                              NULL,
                                              NULL,
                                              reg_name,
                                              &size );

                    if( result != ERROR_SUCCESS ) {
                        RegCloseKey( key_handle );
                        break;
                    }
                    result = RegQueryValueEx( key_handle,
                                              "RegisteredOrganization",
                                              NULL,
                                              NULL,
                                              reg_company,
                                              &size_copy );
                    if( result != ERROR_SUCCESS ) {
                        RegCloseKey( key_handle );
                        break;
                    }
                    if( licname != NULL ) {
                        GUIMemFree( licname );
                    }
                    if( liccompname != NULL ) {
                        GUIMemFree( liccompname );
                    }
                    licname = reg_name;
                    liccompname = reg_company;
                    RegCloseKey( key_handle );
                    break;
                }

#elif defined( WIN )
                #define STRING_SIZE 256
                char    ini_name[ STRING_SIZE ];
                char    ini_company[ STRING_SIZE ];
                GetPrivateProfileString( "mswindows",
                                         "username",
                                         "",
                                         ini_name,
                                         STRING_SIZE,
                                         "serialno.ini" );

                GetPrivateProfileString( "mswindows",
                                         "company",
                                         "",
                                         ini_company,
                                         STRING_SIZE,
                                         "serialno.ini" );

                if( ini_name[ 0 ] != '\0' && ini_company[ 0 ] != '\0' ) {
                    if( licname != NULL ) {
                        GUIMemFree( licname );
                    }
                    if( liccompname != NULL ) {
                        GUIMemFree( liccompname );
                    }
                    licname = ini_name;
                    liccompname = ini_company;
                }
#endif
            }
        }

#endif

    SetVariableByName( "LicenseName", licname );
    SetVariableByName( "LicenseCompanyName", liccompname );
    if( VariablesFile != NULL ) {  //Let a script file override at the last minute
        ReadVariablesFile( "LicenseName" );
        ReadVariablesFile( "LicenseCompanyName" );
    }
    if( !alreadylicensed ) {
        for( ;; ) {
            return_state = DoDialog( "License" );
            if( return_state == DLG_CAN || return_state == DLG_DONE ) {
                return( FALSE );
            }
            licname = GetVariableStrVal("LicenseName");
            liccompname = GetVariableStrVal("LicenseCompanyName");
            if( is_null_name( licname )  ||  is_null_name( liccompname ) ) {
                SetVariableByName( "LicenseMessage", GetVariableStrVal( "IDS_LICENSEMESSAGE" ) );
                continue;
            }
            break;
        }
    }
    for( ;; ) {
        err = apply_license_to_exe( lic_exe_path, lic_file_path, licname, liccompname, &tryagain );
        if( err == NULL ) return( TRUE );
        MsgBox( NULL, "IDS_ERROR", GUI_OK, err );
        if( !tryagain ) break;
    }
    return( FALSE );
}


void StampFile( char *filename )
{
    time_t              time_of_day;
    struct tm           tmbuf;
    a_db_parms          parm;
    char                filebuf[ _MAX_PATH ];

    ReplaceVars( filebuf, filename );
    if( ReadBlock( filebuf, DBPARMS_TAG_STR, &parm, sizeof( a_db_parms ) ) ) {
        time_of_day = time( NULL );
        _localtime( &time_of_day, &tmbuf );
        parm.EvalTimeStamp = tmbuf.tm_year * 365L + tmbuf.tm_yday;      // day since 1900

        WriteBlock( filebuf, DBPARMS_TAG_STR, &parm, sizeof( a_db_parms ) );
    }
}


void StampEvalFiles()
{
    char                *filelist;
    char                *p;

    filelist = GetVariableStrVal( "EvalList" );
    if( filelist != NULL && filelist[ 0 ] != '\0' ) {
        p = strtok( filelist, ";" );
        while( p != NULL ) {
            StampFile( p );
            p = strtok( NULL, ";" );
        }
    }
}
