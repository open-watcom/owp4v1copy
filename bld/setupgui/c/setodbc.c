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


#if defined( __WINDOWS__ ) || defined( __NT__ )

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "gui.h"
#include "guiutil.h"
#include "setup.h"
#include "setupinf.h"
#include "genvbl.h"
#include "gendlg.h"

extern void *MemAlloc( int size );
extern void MemFree( void *ptr );

typedef struct a_strlist {
    struct a_strlist *  next;
    char *              str;
} a_strlist, *p_strlist;

static const char * ODBCAppName = "ODBC Data Sources";
static const char * ODBCFileName = "ODBC.INI";
static const char * WSQL40Driver = "Watcom SQL 4.0";
static const char * SQLANY50Driver = "Sybase SQL Anywhere 5.0";
#if defined( __NT__ )
static const char * ODBCRegPathName = "Software\\ODBC\\ODBC.INI";
#endif

static char             replacement_exe[ _MAX_PATH ];
static char             buffer[1024];

bool AskAboutUpdating( bool *prompt )
/***********************************/
{
    dlg_state           return_state;

    if( GetVariableIntVal( "ODBCAll" ) == 0 &&
        GetVariableIntVal( "ODBCPrompt" ) == 0 &&
        GetVariableIntVal( "ODBCNone" ) == 0 ) {

        return_state = DoDialog( "ODBCConvert" );
        if( GetVariableIntVal( "ODBCPrompt" ) == 1 ) {
            *prompt = TRUE;
        } else if( GetVariableIntVal( "ODBCAll" ) == 1 ) {
            *prompt = FALSE;
        }
        if( return_state == DLG_CAN ||
            return_state == DLG_DONE ) {
            return FALSE;
        }
    }
    return TRUE;
}

static char *StrDup( char *str )
/******************************/
{
    char *          new;

    if( str == NULL ) return( NULL );
    new = MemAlloc( strlen( str ) + 1 );
    if( new != NULL ) {
        strcpy( new, str );
    }
    return( new );
}

char * ReplacementExe( char *fname, char *full_path )
/***************************************************/
{
    ReplaceVars( replacement_exe,     GetVariableStrVal( "DstDir" ) );
    _splitpath( full_path, NULL, NULL, fname, NULL );
    if( stricmp( fname, "rtdsk40s" ) == 0 ) {
        strcat( replacement_exe, "\\win\\rtdsk50s.exe" );
    } else if( stricmp( fname, "rtdsk40w" ) == 0 ) {
        strcat( replacement_exe, "\\win\\rtdsk50w.exe" );
    } else if( stricmp( fname, "dbeng40s" ) == 0 ) {
        strcat( replacement_exe, "\\win\\dbeng50s.exe" );
    } else if( stricmp( fname, "dbeng40w" ) == 0 ) {
        strcat( replacement_exe, "\\win\\dbeng50w.exe" );
    } else if( stricmp( fname, "dbclienw" ) == 0 ) {
        strcat( replacement_exe, "\\win\\dbclienw.exe" );
    } else if( stricmp( fname, "dbsrv40w" ) == 0 ) {
        strcat( replacement_exe, "\\win\\dbsrv50w.exe" );

    } else if( stricmp( fname, "rtdsk40" ) == 0 ) {
        strcat( replacement_exe, "\\win32\\rtdsk50.exe" );
    } else if( stricmp( fname, "dbeng40" ) == 0 ) {
        strcat( replacement_exe, "\\win32\\dbeng50.exe" );
    } else if( stricmp( fname, "dbclient" ) == 0 ) {
        strcat( replacement_exe, "\\win32\\dbclient.exe" );
    } else if( stricmp( fname, "dbsrv40" ) == 0 ) {
        strcat( replacement_exe, "\\win32\\dbsrv50.exe" );
    } else if( stricmp( fname, "wod40w" ) == 0 ) {
        strcat( replacement_exe, "\\win\\wod50w.dll" );
    } else if( stricmp( fname, "wod40t" ) == 0 ) {
        strcat( replacement_exe, "\\win32\\wod50t.dll" );
    } else {
        strcpy( replacement_exe, "" );
    }

    return replacement_exe;
}

static void free_strlist( p_strlist list )
/****************************************/
{
    p_strlist       next;

    while( list != NULL ) {
        if( list->str ) {
            MemFree( list->str );
        }
        next = list->next;
        MemFree( list );
        list = next;
    }
}

static p_strlist string_to_list( char *str, int len )
/***************************************************/
{
    int             pos;
    int             stringlen;
    p_strlist       head = NULL;
    p_strlist       cur;

    for( pos = 0; pos < len; pos += stringlen + 1 ) {
        stringlen = strlen( str );
        if( stringlen > 0 ) {
            cur = MemAlloc( sizeof( a_strlist ) );
            if( cur == NULL ) {
                free_strlist( head );
                return( NULL );
            }
            cur->next = head;
            cur->str = StrDup( str );
            if( cur->str == NULL ) {
                MemFree( cur );
                free_strlist( head );
                return( NULL );
            }
            head = cur;
        }
        str += stringlen + 1;
    }
    return( head );
}

static void load_dlg_vars( char *source_name, char *ODBCType )
/************************************************************/
{
    static char     strbuff[512];
    int             len;

    strcpy( strbuff, "    Source: " );
    strcat( strbuff, source_name );
    SetVariableByName( "ODBCDSN", strbuff );

    strcpy( strbuff, "    Description: " );
    len = strlen( strbuff );
    GetPrivateProfileString( source_name,
                             "Description",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCFileName );
    SetVariableByName( "ODBCDesc", strbuff );

    strcpy( strbuff, "    DatabaseFile: " );
    len = strlen( strbuff );
    GetPrivateProfileString( source_name,
                             "DatabaseFile",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCFileName );
    SetVariableByName( "ODBCDB", strbuff );

    strcpy( strbuff, "    Start: " );
    len = strlen( strbuff );
    GetPrivateProfileString( source_name,
                             "Start",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCFileName );
    SetVariableByName( "ODBCStartLine", strbuff );

    SetVariableByName( "ODBCType", ODBCType );
}

static bool ManipStartLine( char *              name,
                            int                 len,
                            char *              full_path,
                            char *              buffer,
                            BOOL *              result )
/********************************************************/
{
    char            fname[ _MAX_FNAME ];
    char *          remainder;
    char *          replacement;

    /* This code is to update the startline. */

    // 'remainder' points just past end of full_path
    remainder = full_path + strlen( full_path ) + 1;
    if( remainder - buffer > len ) {
        remainder = NULL;
    } else {
        remainder = StrDup( remainder );
        if( remainder == NULL ) {
            *result = FALSE;
            return( FALSE );
        }
    }

    replacement = ReplacementExe( fname, full_path );

    if( strlen( replacement ) == 0 ) {
        dlg_state           state;

#if defined( __NT__ )
        load_dlg_vars( name, "16-bit " );
#else
        load_dlg_vars( name, "" );
#endif
        state = DoDialog( "ODBCAppNotRecognized" );
        MemFree( remainder );
        if( state == DLG_SKIP ) {
            *result = TRUE;
            return FALSE;
        } else if( state == DLG_CAN || state == DLG_DONE ) {
            *result = FALSE;
            return FALSE;
        }
        strcpy( buffer, "" );
        return TRUE;
    }

    if( replacement == NULL ) {
        MemFree( remainder );
        *result = TRUE;
        return FALSE;
    }

    strcpy( buffer, replacement );
    if( remainder != NULL ) {
        strcat( buffer, " " );
        strcat( buffer, remainder );
        MemFree( remainder );
    }
    return TRUE;
}

static bool update_data_source( char *name )
/******************************************/
{
    int                         len;
    char *                      full_path;
    BOOL                        result;
    char                        fname[ _MAX_FNAME ];
    char *                      replace;

    len = GetPrivateProfileString( name,
                                   "Start", /* Look up start line. */
                                   "",
                                   buffer,
                                   sizeof( buffer ),
                                   ODBCFileName );

    full_path = strtok( buffer, " " );

    if( full_path != NULL ) {
        if( ManipStartLine( name, len, full_path, buffer, &result ) == FALSE ) {
            return result;
        }
        if( strlen( buffer ) > 0 ) {
            WritePrivateProfileString( name,
                                       "Start",
                                       buffer,
                                       ODBCFileName );
        }
    }

    /* Change the 'source_name'='driver_name' entry at the top of the
     * 'ODBC Data Sources' section.
     */
    WritePrivateProfileString( ODBCAppName,
                               name,
                               SQLANY50Driver,
                               ODBCFileName );
    /* Also update the 'Driver'='dll_name' line of the 'source_name' section.
     */
    len = GetPrivateProfileString( name,
                                   "Driver", /* Look up start line. */
                                   "",
                                   buffer,
                                   sizeof( buffer ),
                                   ODBCFileName );

    replace = ReplacementExe( fname, buffer );

    if( strlen( replace ) > 0 ) {
        WritePrivateProfileString( name,
                                   "Driver",
                                   replace,
                                   ODBCFileName );
    }

    return( TRUE );
}

static bool update_list( p_strlist list, bool prompt )
/****************************************************/
{
    int             len;
    dlg_state       state;

    for( ; list != NULL; list = list->next ) {
        len = GetPrivateProfileString( ODBCAppName,
                                       list->str,   /* Look up source's driver. */
                                       "",          /* Empty by default. */
                                       buffer,
                                       sizeof( buffer ),
                                       ODBCFileName );
        if( stricmp( buffer, WSQL40Driver ) != 0 ) {
            continue;
        }

        if( AskAboutUpdating( &prompt ) == FALSE ) {
            return FALSE;
        }
        if( GetVariableIntVal( "ODBCNone" ) == 1 ) {
            return TRUE;
        }

        /* We have a WATCOM 4.0 data source; process it. */
        if( prompt ) {
#if defined( __NT__ )
            load_dlg_vars( list->str, "16-bit " );
#else
            load_dlg_vars( list->str, "" );
#endif
            state = DoDialog( "ODBCConvertOne" );
            if( state == DLG_NEXT ) {
                if( !update_data_source( list->str ) ) break;
            } else if( state == DLG_CAN || state == DLG_DONE ) {
                break;
            }
        } else {
            if( !update_data_source( list->str ) ) break;
        }
    }
    return( TRUE );
}

extern bool UpdateODBC( void )
/****************************/
{
    int                 len;
    bool                retval = TRUE;
    p_strlist           strlist = NULL;


    len = GetPrivateProfileString( ODBCAppName,
                                   NULL,    /* Enumerate all keynames. */
                                   "",      /* Empty by default. */
                                   buffer,
                                   sizeof( buffer ),
                                   ODBCFileName );

    strlist = string_to_list( buffer, len );
    if( strlist == NULL ) {
        /* Nothing to do. */
        return( TRUE );
    }

    if( GetVariableIntVal( "ODBCAll" ) == 1 ) {
        retval = update_list( strlist, FALSE );
    } else if( GetVariableIntVal( "ODBCPrompt" ) == 1 ||
               GetVariableIntVal( "ODBCNone" ) == 0 ) {
        retval = update_list( strlist, TRUE );
    }

    free_strlist( strlist );
    return( retval );
}

#if defined( __NT__ )

BOOL GetValuesIntoBuffer( HKEY                  key_handle,
                          int                   num_values,
                          DWORD                 value_name_max_length,
                          DWORD                 value_data_max_length,
                          LPCTSTR               lpKeyName,
                          LPTSTR                lpReturnedString,
                          LPDWORD               nSize )
/********************************************************************/
{
    DWORD                       value_name_length;
    DWORD                       value_data_length;
    char *                      value_name;
    char *                      value_data;
    int                         pos;
    int                         i;
    DWORD                       type;
    LONG                        result;

    value_name = GUIMemAlloc( value_name_max_length + 1 );
    if( value_name == NULL ) {
        return FALSE;
    }
    value_data = GUIMemAlloc( value_data_max_length + 1 );
    if( value_data == NULL ) {
        GUIMemFree( value_name );
        return FALSE;
    }
    pos = 0;
    for( i = 0; i < num_values; i++ ) {
        value_name_length = value_name_max_length + 1;
        value_data_length = value_data_max_length + 1;
        result = RegEnumValue( key_handle,
                               i,
                               value_name,
                               &value_name_length,
                               0,
                               &type,
                               value_data,
                               &value_data_length );

        if( result != ERROR_SUCCESS ) {
            GUIMemFree( value_name );
            GUIMemFree( value_data );
            return FALSE;
        }

        if( strlen( value_name ) + strlen( value_data ) + 2 >
            *nSize - pos ) {
            GUIMemFree( value_name );
            GUIMemFree( value_data );
            return FALSE;
        }

        if( lpKeyName != NULL ) {
            if( strlen( lpKeyName ) > 0 ) {
                if( stricmp( value_name, lpKeyName ) == 0 ) {
                    sprintf( &lpReturnedString[ pos ], "%s", value_data );
                    pos += strlen( &lpReturnedString[ pos ] ) + 1;
                }
            } else {
                sprintf( &lpReturnedString[ pos ], "%s", value_name );
                pos += strlen( &lpReturnedString[ pos ] ) + 1;
            }
        } else {
            sprintf( &lpReturnedString[ pos ], "%s", value_name );
            pos += strlen( &lpReturnedString[ pos ] ) + 1;
        }
    }
    GUIMemFree( value_name );
    GUIMemFree( value_data );

    *nSize = pos - 1;

    return TRUE;
}

DWORD GetRegKeyStrings( LPCTSTR                 lpAppName,
                        LPCTSTR                 lpKeyName,
                        LPCTSTR                 lpDefault,
                        LPTSTR                  lpReturnedString,
                        DWORD                   nSize,
                        LPCTSTR                 lpFileName )
/***************************************************************/
{
    LONG                        result;
    DWORD                       num_values;
    HKEY                        key_handle;
    DWORD                       value_name_max_length;
    DWORD                       value_data_max_length;
    DWORD                       temp[ 5 ];
    FILETIME                    file_time;
    char                        class[ 20 ];
    char *                      reg_path_name;

    strcpy( lpReturnedString, lpDefault );

    reg_path_name = GUIMemAlloc( strlen( lpFileName ) + strlen( lpAppName ) + 2 );
    if( reg_path_name == NULL ) {
        return 0;
    }
    sprintf( reg_path_name, "%s\\%s", lpFileName, lpAppName );

    result = RegOpenKeyEx( HKEY_CURRENT_USER,
                           reg_path_name,
                           0,
                           KEY_READ,
                           &key_handle );
    if( result != ERROR_SUCCESS ) {
        GUIMemFree( reg_path_name );
        return 0;
    }

    temp[ 0 ] = sizeof( class );
    result = RegQueryInfoKey( key_handle,
                              class,
                              &temp[ 0 ],
                              NULL,
                              &temp[ 1 ],
                              &temp[ 2 ],
                              &temp[ 3 ],
                              &num_values,
                              &value_name_max_length,
                              &value_data_max_length,
                              &temp[ 4 ],
                              &file_time );

    result = GetValuesIntoBuffer( key_handle,
                                  num_values,
                                  value_name_max_length,
                                  value_data_max_length,
                                  lpKeyName,
                                  lpReturnedString,
                                  &nSize );
    GUIMemFree( reg_path_name );

    if( RegFlushKey( key_handle ) != ERROR_SUCCESS ) return 0;

    if( RegCloseKey( key_handle ) != ERROR_SUCCESS ) return 0;

    if( result == FALSE ) return 0;
    return nSize;
}

static void reg_load_dlg_vars( char *source_name, char *ODBCType )
/************************************************************/
{
    static char     strbuff[512];
    int             len;

    strcpy( strbuff, "    Source: " );
    strcat( strbuff, source_name );
    SetVariableByName( "ODBCDSN", strbuff );

    strcpy( strbuff, "    Description: " );
    len = strlen( strbuff );
    GetRegKeyStrings( source_name,
                             "Description",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCRegPathName );
    SetVariableByName( "ODBCDesc", strbuff );

    strcpy( strbuff, "    DatabaseFile: " );
    len = strlen( strbuff );
    GetRegKeyStrings( source_name,
                             "DatabaseFile",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCRegPathName );
    SetVariableByName( "ODBCDB", strbuff );

    strcpy( strbuff, "    Start: " );
    len = strlen( strbuff );
    GetRegKeyStrings( source_name,
                             "Start",
                             "",
                             &strbuff[len],
                             sizeof( strbuff ) - len,
                             ODBCRegPathName );
    SetVariableByName( "ODBCStartLine", strbuff );

    SetVariableByName( "ODBCType", ODBCType );
}

BOOL WriteRegString( LPCTSTR  lpszSection,
                     LPCTSTR  lpszKey,
                     LPCTSTR  lpszString,
                     LPCTSTR  lpszFile )
/****************************************/
{
    LONG                        result;
    HKEY                        key_handle;
    DWORD                       type;
    DWORD                       old_type;
    char *                      reg_path_name;

    reg_path_name = GUIMemAlloc( strlen( lpszSection ) + strlen( lpszFile ) + 2 );
    if( reg_path_name == NULL ) {
        return 0;
    }
    sprintf( reg_path_name, "%s\\%s", lpszFile, lpszSection );

    result = RegOpenKeyEx( HKEY_CURRENT_USER,
                           reg_path_name,
                           0,
                           KEY_WRITE,
                           &key_handle );
    if( result != ERROR_SUCCESS ) {
        GUIMemFree( reg_path_name );
        return 0;
    }
    result = RegQueryValueEx( key_handle, lpszKey, NULL, &old_type, NULL, NULL );
    if( result == ERROR_SUCCESS ) {
        type = old_type;
    } else {
        type = REG_SZ;
    }
    result = RegSetValueEx( key_handle,
                            lpszKey,
                            0,
                            type,
                            lpszString,
                            strlen( lpszString ) + 1 );

    if( RegFlushKey( key_handle ) != ERROR_SUCCESS ) return ERROR_INVALID_HANDLE;

    if( RegCloseKey( key_handle ) != ERROR_SUCCESS ) return ERROR_INVALID_HANDLE;

    return( result == ERROR_SUCCESS );
}

static bool reg_update_data_source( char *name )
/**********************************************/
{
    int                         len;
    char *                      full_path;
    BOOL                        result;
    char                        fname[ _MAX_FNAME ];
    char *                      replace;

    len = GetRegKeyStrings( name,
                            "Start", /* Look up start line. */
                            "",
                            buffer,
                            sizeof( buffer ),
                            ODBCRegPathName );

    full_path = strtok( buffer, " " );

    if( full_path != NULL ) {
        if( ManipStartLine( name, len, full_path, buffer, &result ) == FALSE ) {
            return result;
        }
        if( strlen( buffer ) > 0 ) {
            WriteRegString( name,
                            "Start",
                            buffer,
                            ODBCRegPathName );
        }
    }

    /* Change the 'source_name'='driver_name' entry at the top of the
     * 'ODBC Data Sources' section.
     */
    WriteRegString( ODBCAppName,
                    name,
                    SQLANY50Driver,
                    ODBCRegPathName );
    /* Also update the 'Driver'='dll_name' line of the 'source_name' section.
     */
    len = GetRegKeyStrings( name,
                            "Driver", /* Look up start line. */
                            "",
                            buffer,
                            sizeof( buffer ),
                            ODBCRegPathName );

    replace = ReplacementExe( fname, buffer );

    if( strlen( replace ) > 0 ) {
        WriteRegString( name, "Driver", replace, ODBCRegPathName );
    }

    return( TRUE );
}

static bool reg_update_list( p_strlist list, bool prompt )
/********************************************************/
{
    int             len;
    dlg_state       state;

    for( ; list != NULL; list = list->next ) {
        strcpy( buffer, "" );
        len = GetRegKeyStrings( ODBCAppName,
                                list->str,   /* Look up source's driver. */
                                "",
                                buffer,
                                sizeof( buffer ),
                                ODBCRegPathName );
        if( stricmp( buffer, WSQL40Driver ) != 0 ) {
            continue;
        }

        if( AskAboutUpdating( &prompt ) == FALSE ) {
            return FALSE;
        }
        if( GetVariableIntVal( "ODBCNone" ) == 1 ) {
            return TRUE;
        }

        /* We have a WATCOM 4.0 data source; process it. */
        if( prompt ) {
            reg_load_dlg_vars( list->str, "32-bit " );
            state = DoDialog( "ODBCConvertOne" );
            if( state == DLG_NEXT ) {
                if( !reg_update_data_source( list->str ) ) break;
            } else if( state == DLG_CAN || state == DLG_DONE ) {
                break;
            }
        } else {
            if( !reg_update_data_source( list->str ) ) break;
        }
    }
    return( TRUE );
}

extern bool RegUpdateODBC( void )
/*******************************/
{
    int                 len;
    bool                retval = TRUE;
    p_strlist           strlist = NULL;

    // Get the registry key values.
    len = GetRegKeyStrings( ODBCAppName,
                            NULL,
                            "",
                            buffer,
                            sizeof( buffer ),
                            ODBCRegPathName );

    strlist = string_to_list( buffer, len );
    if( strlist == NULL ) {
        /* Nothing to do. */
        return( TRUE );
    }

    if( GetVariableIntVal( "ODBCAll" ) == 1 ) {
        retval = reg_update_list( strlist, FALSE );
    } else if( GetVariableIntVal( "ODBCPrompt" ) == 1 ||
               GetVariableIntVal( "ODBCNone" ) == 0 ) {
        retval = reg_update_list( strlist, TRUE );
    }

    free_strlist( strlist );
    return( retval );
}

#endif
#endif
