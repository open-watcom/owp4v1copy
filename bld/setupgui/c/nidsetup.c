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
*   COMMENTS:  Set up NIDynamo to work with ISAPI, NSAPI, and CGI/Win-CGI.
*/

#if defined( WINNT ) && defined( WSQL )
//=============================================================================
//=============================================================================

// Macros

#define RoundUp( v, r ) (((v)+(r)-1) & ~(unsigned long)((r)-1))
#define length( string )        ( sizeof( string ) - 1 )
#define lastchar( string )      ( string[ strlen( string ) - 1 ] )

// File lists =============================================

// A usable list is a sequence of
// null terminated strings finally terminated by a second null

// Nice looking NULL terminators
#define  OO                     "\0"
#define  ENDLIST                OO OO

// Single files
#define  NINSDLL                "NINS.DLL"
#define  NIISDLL                "NIIS.DLL"
#define  CGI_EXCLUSIVE          "NICGI.EXE"
#define  WINCGI_EXCLUSIVE       "NICGIWIN.EXE"

// Partial lists
#define  COMMON_FILES           "DBHTML.DLL"            OO\
                                "NISCRIPT.DLL"          OO\
                                "NIDSENT.DLL"           OO\
                                "NIDEBUG.DLL"           OO\
                                "XDB01.DLL"


#define  COMMON_CGI             COMMON_FILES            OO\
                                "NIDUENT.DLL"

// Usable lists
#define  CGI_FILES              CGI_EXCLUSIVE           OO\
                                COMMON_CGI              ENDLIST

#define  WINCGI_FILES           WINCGI_EXCLUSIVE        OO\
                                COMMON_CGI              ENDLIST

#define  ALL_CGI_FILES          CGI_EXCLUSIVE           OO\
                                WINCGI_EXCLUSIVE        OO\
                                COMMON_CGI              ENDLIST

#define  NSDLLS                 NINSDLL                 OO\
                                COMMON_FILES            ENDLIST

#define  IISDLLS                NIISDLL                 OO\
                                COMMON_FILES            ENDLIST

#define  OPTIMA_DLLS            "MTHR107.DLL"           OO\
                                "PLBR107.DLL"           OO\
                                "CLBR107.DLL"           OO\
                                "WCMDPA15.DLL"          ENDLIST

// ==========================================================

// Used in AddStringToSystemEnvironmentVar()
#define  SYSTEMENVIRONMENT      "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"

// Used in ModifyNetscapeOBJCONF()
#define  NAMETRANS              "NameTrans"
#define  NAMETRANSFN            NAMETRANS " fn=_netimpact_ntrans"
#define  PATHCHECKFN            "PathCheck fn=_netimpact_pathcheck"
#define  INITFN                 "Init fn=\"load-modules\" shlib=%s" NINSDLL " funcs=\"_netimpact_ntrans,_netimpact_fn,_netimpact_pathcheck,_netimpact_init\"" "\n" "Init fn=_netimpact_init"
#define  OBJECTSTART            "<Object name="
#define  OBJECTEND              "</Object>"
#define  NETIMPACT_OBJ          OBJECTSTART "NetImpact>"
#define  SERVICEFN              "Service fn=_netimpact_fn"
#define  NIOBJECT               NETIMPACT_OBJ "\n" SERVICEFN "\n" OBJECTEND
#define  DEFAULT                "<Object name=\"default\">"

// Used in CGISetup()
#define  CGI_OK                 "CGI_Files_OK"
#define  WINCGI_OK              "WinCGI_Files_OK"
#define  NIDCGI_DIALOG          "NIDCGI"
#define  NIDCGI_YES             "NID_CGI_Yes"
#define  NIDCGI_DIR             "NID_CGI_Dir"
#define  NIDCGI_AGAIN           "NID_CGI_Again"
#define  NIDCGI_INST            "NID_CGI_Inst"
#define  NIDWINCGI_INST         "NID_WINCGI_Inst"
#define  NIDCGI_SOURCE          "NID_CGI_Source"

// Used in IISSetup()
#define  W3SVC                  "SYSTEM\\CurrentControlSet\\Services\\W3SVC"
#define  W3SVCVIRTROOTS         W3SVCPARMS "\\Virtual Roots"
#define  NIDYNAMO               "SOFTWARE\\Sybase\\NetImpact\\Dynamo"
#define  NIDFILTERDLLS          "ISAPI Filter Directory Alias"
#define  WIN32_DIR              "win32"
#define  IISPATH                "InstallPath"
#define  ISAPI_SERVERNAME       "DisplayName"
#define  SCRIPTS                "/Scripts"
#define  IIS_CONFIG             "_IISCfg_Dynamic_"
#define  IIS_DEFAULT            "IIS_DLLDefault"
#define  IIS_SYBTOOLS           "IIS_DLLSybtools"
#define  IIS_CUSTOM             "IIS_DLLCustom"
#define  IIS_CUSTOM_DLLDIR      "IIS_DLLCustomDIR"
#define  IIS_ALIAS              "IISAlias"
#define  IIS_ALIAS_DEFAULT      "/Sybase"

// Used in NSSetup()
#define  START_INDEX            1
#define  ASCII_0                48
#define  MODIFY_OBJCONF_FILES   "ModifyNetscapeOBJCONFFiles"
#define  NETSCAPESERVER_N       "NetscapeServer_N"
#define  NETSCAPE_CONFIG_N      "_NS_ServerConfig_Dynamic_N"
#define  OBJCONFLIST_N          "_ObjConfList_Dynamic_N"
#define  NS_DEFAULT             "NS_DLLDefault"
#define  NS_SYBTOOLS            "NS_DLLSybtools"
#define  NS_CUSTOM              "NS_DLLCustom"
#define  NS_CUSTOM_DLLDIR       "NS_DLLCustomDIR"
#define  BACKUP_EXT             ".bak"

// Used in AddFilterDLLString()
#define  FILTERDLLS             "Filter DLLs"

// Used in NIDSetup()
#define  NETSCAPEHOME           "SOFTWARE\\Netscape"
#define  NID_GOOD               "NIDGood"

// Used in FindNetscapeServers()
#define  HTTPS                  "https"
#define  HTTPS20                HTTPS "-"
#define  HTTPS30                HTTPS "30-"
#define  CONFIGPATH             "ConfigurationPath"
#define  PLUGINS                "plugins\\"

// Used in VerifyDynamoFiles()
#define  SYBTOOLS               "SybTools"
#define  REG_SQLANY             "SOFTWARE\\Sybase\\SQL Anywhere\\Sybase SQL Anywhere 5.0"
#define  REG_SYBTOOLS           "SOFTWARE\\Sybase\\Sybase Tools"
#define  INSTALL_LOCATION       "Location"
#define  SYSTEM32               "system32"
#define  SYSTEM                 "system"

// Common (used in more than one FN)
#define  OBJCONF                "OBJ.CONF"
#define  PATH                   "Path"
#define  BACKUPOBJCONF          "BackupOBJCONF"
#define  SERVER_DETECTED        "NIDSetup"
#define  DO_SERVER_SETUP        "DoServerSetup"
#define  WEB_SERVER_DIR         "WebServerDir"
#define  WEB_SERVER_DIR_FOUND   "WebServerDir_Found"
#define  A_SERVER_DETECTED      "AWebServerFound"
#define  A_SERVER_CONFIGURED    "AWebServerConfigured"
#define  WEB_SERVER_NAME        "WebServerName"
#define  W3SVCPARMS             W3SVC "\\Parameters"
#define  DIALOG_WIDTH           "80" // probably not needed everywhere
#define  DYNAMO                 "dynamo\\"

// Dialog strings (just like in a setup.inf)
#define  DIALOG                 "[Dialog]"
#define  NAME                   "name="
#define  STATIC_TEXT            "static_text="
#define  CHECKBOX               "check_box="
#define  RADIO                  "radio_button="
#define  EDIT                   "edit_control="
#define  PUSHBUTTON             "push_button="
#define  DETAILCHECK            "detail_check="
#define  ANYCHECK               "any_check="
#define  SETWIDTH               "width="
#define  CHECK_TRUE             "true"
#define  CHECK_FALSE            "false"

//=============================================================================
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <direct.h>
#include <sys\types.h>
#include <io.h>
#include "gui.h"
#include "setupinf.h"
#include "setup.h"
#include "genvbl.h"
#include "gendlg.h"
#include "guiutil.h"
#include "utils.h"

//=============================================================================
//=============================================================================
extern bool JustDoNIDSetup;

//=============================================================================
//=============================================================================

typedef enum dialog_button {
    DIALOG_OK_BUTTON,
    DIALOG_CANCEL_BUTTON,
    DIALOG_HELP_BUTTON
} dialog_button;

typedef enum obj_conf_state {
    OBJ_CONF_ROOT               = 0x01,
    OBJ_CONF_IN_OBJECT          = 0x02,
    OBJ_CONF_OBJECTEND_SEEN     = 0x04,
    OBJ_CONF_DEFAULT_OBJECT     = 0x08,
} obj_conf_state;

typedef enum data_type {
    STRING_T,
    NS_SERVER_T
} data_type;

typedef enum ns_version {
    NS_2,
    NS_3,
} ns_version;

typedef struct node {
    struct node *next;
    data_type   data_t;
    void        *data;
} node;

typedef struct ns_server {
    char        root[ _MAX_PATH ];
    char        dll_path[ _MAX_PATH ];
    node        *objconfs;
    ns_version  version;
} ns_server;

typedef struct reg_location {
    HKEY        root;
    char        *sub_key;
    char        *name;
} reg_location;

//=============================================================================
//=============================================================================
extern void BumpStatus( long );
extern void NoDupPaths( char *new_value, char *old_value, bool add, char delim );
extern bool NTSpawnWait( char *cmd, DWORD *exit_code, HANDLE in, HANDLE out, HANDLE err );
static bool IsFile( char const *file );
static bool IsDir( char const *dir );
static char * Parent( char const *path );
static bool ForwardSlashes( char *path );
static bool BackSlashes( char *path );
static bool EndSlash( char *path );
static bool IsSpaceForFile( char const *src_path, char const *dst_path, long *space_req );
static bool IsSpaceForFiles( node *files, char const *src_path, char const *dst_path, long *space_req );
static bool CheckStrings( char const *test_string, char const *reference_string );
static bool ModifyNetscapeOBJCONF( char const *path, char const * ninspath );
static bool AddFilterDLLString( char const *fullpath, bool add );
static bool AddRegString( HKEY root, char const *key, char const *name, char const *value );
static bool AddStringToSystemEnvironmentVar( char const *var, char const *string, bool add );
static char * ReadRegValue( HKEY root, char const *key, char const *name );
static node * ListRegSubkeys( HKEY key );
static node * ListRegValues( HKEY key );
static char * StripQuotes( char const *input );
static bool CGISetup();
static bool IISSetup();
static bool NSSetup( node * server_list );
static node * FindNetscapeServers( HKEY hkey, node * server_list );
//static bool CreateSystemDSN( char const *DSN_name );
static char * MakeNewlineDelimitedList( node *list );
static node * MakeListFromString( char const *input );
static node * AddNSServer( node *list_node, char const *root, char const *dll_path, char const *objconf, ns_version version );
static ns_server * FindNSServer( node *list_node, char const *root, ns_version version );
static node * AddString( node *list_node, char const *string );
static ns_server * GetServer( node *curr_node );
static node * GetNextNode( node *curr_node );
static char * GetString( node *curr_node );
static node * FindString( node *list, char const *string, bool case_sensitive );
static bool VerifyOptimaFiles( bool * verified );
static bool VerifyFiles( node *file_list, char const *path );
static bool VerifyDynamoFiles( char const *server_name, node *file_list, char *syb_path, bool cgi );
static bool DeleteList( node *list_node );
static void MakeDialog( char *line );
static bool AddButton( dialog_button button );
static bool AddControl( char const *control, ... );
static bool NewDialog( char const *dialog_name );
static char * RemoveFileFromString( char const *original, char const *path, char delim );
static bool NIDCopyFile( char const *src, char const *dst );

//=============================================================================
//=============================================================================

//NIDSetup
extern bool NIDSetup()
/********************/
{
    HKEY        hkey;
    int         rc;
    node        *Netscape_Servers = NULL;
    bool        optima_dlls_ok;

    StatusShow( FALSE );

    SetVariableByName( A_SERVER_DETECTED, "0" );
    SetVariableByName( A_SERVER_CONFIGURED, "0" );

    if( JustDoNIDSetup ) {
        if( MsgBox( NULL, "IDS_NID_LOOKFORSERVERS", GUI_OK_CANCEL ) == GUI_RET_CANCEL ) {
            return( FALSE );
        }
    } else {
        rc = MsgBox( NULL, "IDS_NID_QUERY_LOOKFORSERVERS", GUI_YES_NO_CANCEL );
        if( rc == GUI_RET_CANCEL ) {
            return( FALSE );
        } else if( rc == GUI_RET_NO ) {
            return( TRUE );
        }
    }

    if( !VerifyOptimaFiles( &optima_dlls_ok ) ) {
        return( FALSE );
    }

    if( !optima_dlls_ok ) {
        return( TRUE );
    }

    if( !IISSetup() ) {
        if( JustDoNIDSetup ) {
            MsgBox( NULL, "IDS_NID_FAILED", GUI_OK );
        }
        return( FALSE );
    }

    if( GetVariableIntVal( "IsWinNT" ) == 1 ) {
        rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, NETSCAPEHOME, NULL, KEY_READ, &hkey );
        if( rc == 0 ) {
            Netscape_Servers = FindNetscapeServers( hkey, NULL );
            RegCloseKey( hkey );
        }

        if( Netscape_Servers != NULL ) {
            if( !NSSetup( Netscape_Servers ) ) {
                if( JustDoNIDSetup ) {
                    MsgBox( NULL, "IDS_NID_FAILED", GUI_OK );
                }
                return( FALSE );
            }
        }
    }

    if( !GetVariableIntVal( A_SERVER_DETECTED ) ) {
        MsgBox( NULL, "IDS_NID_NOSERVERSFOUND", GUI_OK );
    }

    if( !CGISetup() ) {
        if( JustDoNIDSetup ) {
            MsgBox( NULL, "IDS_NID_FAILED", GUI_OK );
        }
        return( FALSE );
    }

    if( GetVariableIntVal( A_SERVER_CONFIGURED ) ) {
        DoDialog( NID_GOOD );
    }

    if( JustDoNIDSetup ) {
        MsgBox( NULL, "IDS_NID_SUCCESS", GUI_OK );
    }

    return( TRUE );
}

//CGISetup
static bool CGISetup()
/********************/
{
    node                *cgi_files = NULL;
    node                *wincgi_files = NULL;
    node                *allcgi_files = NULL;
    node                *file_list = NULL;
    node                *curr_node = NULL;
    char                syb_path[ _MAX_PATH ];
    char                syb_path_copy[ _MAX_PATH ];
    char                cgi_path[ _MAX_PATH ];
    char                full_cgi_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                dst_path[ _MAX_PATH ];
    gui_message_return  ret;
    long                space;

    wincgi_files = MakeListFromString( WINCGI_FILES );
    cgi_files    = MakeListFromString( CGI_FILES );
    allcgi_files = MakeListFromString( ALL_CGI_FILES );

    if( allcgi_files == NULL || wincgi_files == NULL || allcgi_files == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }

    ret = MsgBox( NULL, "IDS_NID_IS_CGI_PRESENT", GUI_YES_NO_CANCEL );
    if( ret == GUI_RET_CANCEL ) {
        return FALSE;
    } else if( ret == GUI_RET_NO ) {
        return TRUE;
    }

    *syb_path = '\0';
    if( !VerifyDynamoFiles( GetVariableStrVal( "IDS_NID_CGI" ),
                            cgi_files, syb_path, TRUE ) ) {
        return( FALSE );
    }
    if( *syb_path != '\0' ) {
        SetVariableByName( CGI_OK, "1" );
    }

    strcpy( syb_path_copy, syb_path );
    if( !VerifyDynamoFiles( GetVariableStrVal( "IDS_NID_WINCGI" ),
                            wincgi_files, syb_path_copy, TRUE ) ) {
        return( FALSE );
    }
    if( *syb_path_copy != '\0' ) {
        SetVariableByName( WINCGI_OK, "1" );
    }

    EndSlash( syb_path );

    SetVariableByName( NIDCGI_SOURCE, syb_path );

    if( GetVariableIntVal( CGI_OK ) || GetVariableIntVal( WINCGI_OK ) ) {
        for( ;; ) {
            if( DoDialog( NIDCGI_DIALOG ) != DLG_NEXT ) { // Configure server y/n?
                return( FALSE );
            }
            if( !GetVariableIntVal( NIDCGI_YES ) ) {
                return( TRUE );
            }
            strcpy( cgi_path, GetVariableStrVal( NIDCGI_DIR ) );
            if( *cgi_path == '\0' ) {
                continue;
            }
            // Make sure that the directory for the DLLS is valid
            _fullpath( full_cgi_path, cgi_path, sizeof( full_cgi_path ) );
            EndSlash( full_cgi_path );
            if( access( full_cgi_path, F_OK ) != 0 ) {
                ret = MsgBox( NULL, "IDS_NODIR", GUI_YES_NO_CANCEL, full_cgi_path );
                if( ret == GUI_RET_CANCEL ) {
                    return( FALSE );
                } else if( ret == GUI_RET_NO ) {
                    continue;
                }
                if( mkdir( full_cgi_path ) != 0 ) {
                    if( MsgBox( NULL, "IDS_CANTMAKEDIR", GUI_OK_CANCEL, full_cgi_path ) == GUI_RET_CANCEL ) {
                        return( FALSE );
                    }
                    continue;
                }
            } else {
                if( !IsDir( full_cgi_path ) ) {
                    if( MsgBox( NULL, "IDS_INVALID_DIR", GUI_OK_CANCEL, full_cgi_path ) == GUI_RET_CANCEL ) {
                        return( FALSE );
                    }
                    continue;
                }
            }
            file_list = NULL;
            if( GetVariableIntVal( WINCGI_OK ) && GetVariableIntVal( NIDCGI_INST ) ) {
                if( GetVariableIntVal( WINCGI_OK ) && GetVariableIntVal( NIDWINCGI_INST ) ) {
                    file_list = allcgi_files;
                } else {
                    file_list = wincgi_files;
                }
            } else if( GetVariableIntVal( WINCGI_OK ) && GetVariableIntVal( NIDWINCGI_INST ) ) {
                file_list = cgi_files;
            }

            EndSlash( syb_path );
            if( file_list != NULL ) {
                // Is there enough space to copy files from syb_path to full_cgi_path?
                if( !IsSpaceForFiles( file_list, syb_path, full_cgi_path, &space ) ) {
                    return( FALSE );
                }
                StatusAmount( 0, space );

                // Copy DLLs
                StatusShow( TRUE );
                for( curr_node = file_list; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
                    if( GetString( curr_node ) == NULL ) {
                       continue;
                    }
                    strcpy( src_path, syb_path );
                    strcat( src_path, GetString( curr_node ) );
                    strcpy( dst_path, full_cgi_path );
                    strcat( dst_path, GetString( curr_node ) );
                    if( stricmp( src_path, dst_path ) != 0 ) {
                        if( !NIDCopyFile( src_path, dst_path ) ) {
                            return( FALSE );
                        }
                    }
                }
                StatusShow( FALSE );
            }

            SetVariableByName( A_SERVER_CONFIGURED, "1" );

            if( !GetVariableIntVal( NIDCGI_AGAIN ) ) {
                break;
            }
        }
    }

    return( TRUE );
}



//IISetup
static bool IISSetup()
/********************/
{
    char                alias_dir[ _MAX_PATH + 3 ];
    char                filter_dll[ _MAX_PATH ];
    char                dll_dir[ _MAX_PATH ];
    char                full_dll_dir[ _MAX_PATH ];
    char                syb_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                dst_path[ _MAX_PATH ];
    char                buff[ 4096 ];
    char                *default_dir = NULL;
    char                *iis_path = NULL;
    char                *alias_name = NULL;
    char                *alias_name_comma = NULL;
    char                *index = NULL;
    char                *temp = NULL;
    char                *server_name = NULL;
    node                *alias_list = NULL;
    node                *iis_dlls = NULL;
    node                *curr_node = NULL;
    HKEY                hkey;
    int                 rc;
    bool                alias_has_comma;
    gui_message_return  ret;
    long                space;

    iis_dlls = MakeListFromString( IISDLLS );
    if( iis_dlls == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }

    *syb_path = '\0';
    if( !VerifyDynamoFiles( GetVariableStrVal( "IDS_NID_IIS" ), iis_dlls, syb_path, FALSE ) ) {
        return( FALSE );
    }
    if( *syb_path == '\0' ) {
        return( TRUE ); // can't set up this server in the first place
    }
    EndSlash( syb_path );

    SetVariableByName( WEB_SERVER_DIR_FOUND, "0" );
    iis_path = ReadRegValue( HKEY_LOCAL_MACHINE, W3SVCPARMS, IISPATH );
    if( iis_path != NULL ) {
        if( IsDir( iis_path ) ) {
            SetVariableByName( WEB_SERVER_DIR_FOUND, "1" );
            SetVariableByName( WEB_SERVER_DIR, iis_path );
        }
        GUIFree( iis_path );
    }

    server_name = ReadRegValue( HKEY_LOCAL_MACHINE, W3SVC, ISAPI_SERVERNAME );
    if( server_name == NULL ) {
        return( TRUE ); // server not detected
    } else {
        SetVariableByName( WEB_SERVER_NAME, server_name );
        GUIFree( server_name );
    }

    //============================== BY THIS POINT, THE IIS SERVER IS DETECTED
    //========================================================================
    SetVariableByName( A_SERVER_DETECTED, "1" );


    if( DoDialog( SERVER_DETECTED ) != DLG_NEXT ) { // Configure server y/n?
        return( FALSE );
    }

    // Get the default executable directory for IIS
    default_dir = ReadRegValue( HKEY_LOCAL_MACHINE, W3SVCVIRTROOTS, SCRIPTS );
    if( default_dir == NULL ) {
        default_dir = ReadRegValue( HKEY_LOCAL_MACHINE, W3SVCVIRTROOTS, SCRIPTS "," );
        alias_has_comma = TRUE;
    } else {
        alias_has_comma = FALSE;
    }

    if( default_dir != NULL ) {
        if( lastchar( default_dir ) == '4'
            || lastchar( default_dir ) == '5' ) {
            // '4' or '5' mean dir is at least executable
            // ('1' means read only, and '0' means no access)
            index = strchr( default_dir, ',' );
            if( index != NULL ) {
                *index = '\0';
            }
            if( *default_dir == '\0' ) {
                GUIFree( default_dir );
                default_dir = NULL;
            }
            EndSlash( default_dir );
            default_dir = GUIRealloc( default_dir, strlen( default_dir ) + length( DYNAMO ) + 1 );
            if( default_dir != NULL ) {
                strcat( default_dir, DYNAMO );
            }
        } else {
            GUIFree( default_dir );
            default_dir = NULL;
        }
    }

    if( GetVariableIntVal( DO_SERVER_SETUP ) == 0 ) {
        if( default_dir ) {
            GUIFree( default_dir );
        }
        DeleteList( iis_dlls );
        return( TRUE );
    }
    // Make a dialog that how to configure the server
    if( !NewDialog( IIS_CONFIG ) ) {
        return( FALSE );
    }
    AddControl( SETWIDTH DIALOG_WIDTH, NULL );
    sprintf( buff, STATIC_TEXT "\"%s\"",
                   GetVariableStrVal( "IDS_IISCONFIG_1" ) );
    AddControl( buff, GetVariableStrVal( WEB_SERVER_NAME ), NULL );
    if( GetVariableIntVal( WEB_SERVER_DIR_FOUND ) ) {
        sprintf( buff, STATIC_TEXT "\"%s\"",
                       GetVariableStrVal( "IDS_IISCONFIG_2" ) );
        AddControl( buff, GetVariableStrVal( WEB_SERVER_DIR ), NULL );
    }
    AddControl( STATIC_TEXT, NULL );
    AddControl( STATIC_TEXT "\"%s\"",
                GetVariableStrVal( "IDS_IISCONFIG_3" ), NULL );
    AddControl( STATIC_TEXT, NULL );
    AddControl( STATIC_TEXT "\"%s\"",
                GetVariableStrVal( "IDS_IISCONFIG_4" ), NULL );
    sprintf( buff, RADIO IIS_SYBTOOLS "," CHECK_TRUE ",\"%s\"",  //recommended choice
                   GetVariableStrVal( "IDS_IISCONFIG_5" ) );
    AddControl( buff, syb_path, NULL );
    if( default_dir ) {
        sprintf( buff, RADIO IIS_DEFAULT "," CHECK_FALSE ",\"%s\"",
                       GetVariableStrVal( "IDS_IISCONFIG_6" ) );
        AddControl( buff, default_dir, NULL );
    }
    AddControl( RADIO IIS_CUSTOM "," CHECK_FALSE ",\"%s\"",
                    GetVariableStrVal( "IDS_IISCONFIG_7" ), NULL );
    AddControl( STATIC_TEXT "\"\",," IIS_CUSTOM, NULL );
    AddControl( EDIT IIS_CUSTOM_DLLDIR ",,\"%s\",,%s",
//              default_dir ? default_dir : syb_path,
                GetVariableStrVal( "IDS_IISCONFIG_8" ),
                IIS_CUSTOM, NULL );
    AddControl( STATIC_TEXT "\"\",,%s %s |",
                IIS_CUSTOM, IIS_SYBTOOLS, NULL );
    AddControl( STATIC_TEXT "\"%s\",,%s %s |",
                GetVariableStrVal( "IDS_IISCONFIG_9" ),
                IIS_CUSTOM, IIS_SYBTOOLS, NULL );
    AddControl( EDIT IIS_ALIAS "," IIS_ALIAS_DEFAULT ",\"%s\",,%s %s |",
                GetVariableStrVal( "IDS_IISCONFIG_10" ),
                IIS_CUSTOM, IIS_SYBTOOLS, NULL );
    AddControl( STATIC_TEXT, NULL );
    if( !AddButton( DIALOG_OK_BUTTON ) ) {
        return( FALSE );
    }
    if( !AddButton( DIALOG_CANCEL_BUTTON ) ) {
        return( FALSE );
    }

    // Dialog Loop
    for( ;; ) {
        if( DoDialog( IIS_CONFIG ) != DLG_NEXT ) {
            return( FALSE );
        }

        // Find out where the user wants to put the ISAPI DLLS
        if( GetVariableIntVal( IIS_DEFAULT ) != 0 && default_dir ) {
            strcpy( dll_dir, default_dir );
        } else if( GetVariableIntVal( IIS_SYBTOOLS ) != 0 )  {
            strcpy( dll_dir, syb_path );
        } else {
            strcpy( dll_dir, GetVariableStrVal( IIS_CUSTOM_DLLDIR ) );
        }
        if( *dll_dir == '\0' ) {
            continue;
        }

        // Make sure that the directory for the DLLS is valid
        _fullpath( full_dll_dir, dll_dir, sizeof( full_dll_dir ) );
        EndSlash( full_dll_dir );
        if( access( full_dll_dir, F_OK ) != 0 ) {
            ret = MsgBox( NULL, "IDS_NODIR", GUI_YES_NO_CANCEL, full_dll_dir );
            if( ret == GUI_RET_CANCEL ) {
                return( FALSE );
            } else if( ret == GUI_RET_NO ) {
                continue;
            }
            if( mkdir( full_dll_dir ) != 0 ) {
                if( MsgBox( NULL, "IDS_CANTMAKEDIR", GUI_OK_CANCEL, full_dll_dir ) == GUI_RET_CANCEL ) {
                    return( FALSE );
                }
                continue;
            }
        } else {
            if( !IsDir( full_dll_dir ) ) {
                if( MsgBox( NULL, "IDS_INVALID_DIR", GUI_OK_CANCEL, full_dll_dir ) == GUI_RET_CANCEL ) {
                    return( FALSE );
                }
                continue;
            }
        }

        if( GetVariableIntVal( IIS_DEFAULT ) ) {
            GUIStrDup( SCRIPTS, &alias_name );
            if( alias_name == NULL ) {
                MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
                return( FALSE );
            }
        } else {
            // If the default executable dir not chosen, alias needed
            GUIStrDup( GetVariableStrVal( IIS_ALIAS ), &temp );
            if( temp == NULL ) {
                MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
                return( FALSE );
            }
            alias_name = GUIAlloc( strlen( temp ) + 2 );
            if( alias_name == NULL ) {
                MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
                return( FALSE );
            }
            if( temp[0] != '/' ) {
                sprintf( alias_name, "/%s", temp );
            } else {
                strcpy( alias_name, temp );
            }
            GUIFree( temp );

            // Check all the current aliases to see if the user's request exists already
            rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, W3SVCVIRTROOTS, NULL, KEY_READ, &hkey );
            if( rc != 0 ) {
                MsgBox( NULL, "IDS_REGERROR", GUI_OK );
                return( FALSE );
            }

            if( lastchar( alias_name ) == ',' ) {
                alias_name_comma = alias_name;
                GUIStrDup( alias_name_comma, &alias_name );
                lastchar( alias_name_comma ) = '\0';
                if( alias_name == NULL ) {
                    MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
                    return( FALSE );
                }
            } else {
                alias_name_comma = GUIAlloc( strlen( alias_name ) + 2 );
                if( alias_name_comma == NULL ) {
                    MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
                    return( FALSE );
                }
                sprintf( alias_name_comma, "%s,", alias_name );
            }

            alias_list = ListRegValues( hkey );
            if( FindString( alias_list, alias_name, FALSE ) != NULL ) {
                if( MsgBox( NULL, "IDS_ALIAS_EXISTS", GUI_YES_NO, alias_name ) == GUI_RET_YES ) {
                    continue; //don't replace alias
                }
                alias_has_comma = FALSE;
            } else if( FindString( alias_list, alias_name_comma, FALSE ) != NULL ) {
                if( MsgBox( NULL, "IDS_ALIAS_EXISTS", GUI_YES_NO, alias_name ) == GUI_RET_YES ) {
                    continue; //don't replace alias
                }
                alias_has_comma = TRUE;
            }
            DeleteList( alias_list );
        }
        // input from dialog is valid, so go on.
        break;

    }

    if( GetVariableIntVal( IIS_SYBTOOLS ) == 0
        && stricmp( syb_path, full_dll_dir ) != 0 ) {
        // Is there enough space to copy iis_dlls from syb_path to full_dll_dir?
        if( !IsSpaceForFiles( iis_dlls, syb_path, full_dll_dir, &space ) ) {
            return( FALSE );
        }
        StatusAmount( 0, space );
    }


    BackSlashes( full_dll_dir );

    strcpy( filter_dll, full_dll_dir );

    if( lastchar( full_dll_dir ) == '\\' ) {
        lastchar( full_dll_dir ) = '\0';
    }

    // Add full_dll_dir to the system path
    if( GetVariableIntVal( "IsWinNT" ) == 1
        && !AddStringToSystemEnvironmentVar( PATH, full_dll_dir, TRUE ) ) {
        return( FALSE );
    }

    EndSlash( full_dll_dir );

    // Make NIISDLL a Filter DLL under IIS
    strcat( filter_dll, NIISDLL );
    if( !AddFilterDLLString( filter_dll, TRUE ) ) {
        return( FALSE );
    }

    // Tell Dynamo about the ISAPI Filter DLL
    if( !AddRegString( HKEY_LOCAL_MACHINE, NIDYNAMO, NIDFILTERDLLS, alias_name ) ) {
        return( FALSE );
    }

    if( alias_has_comma ) {
        GUIFree( alias_name );
        alias_name = alias_name_comma;
    }

    // Add virtual alias to IIS
    if( !GetVariableIntVal( IIS_DEFAULT ) ) {
        sprintf( alias_dir, "%s,,4", full_dll_dir );
        if( !AddRegString( HKEY_LOCAL_MACHINE, W3SVCVIRTROOTS, alias_name, alias_dir ) ) {
            return( FALSE );
        }
    }

    GUIFree( alias_name );

    if( GetVariableIntVal( IIS_SYBTOOLS ) == 0
        && stricmp( syb_path, full_dll_dir ) != 0 ) {
        StatusShow( TRUE );
        // Copy DLLs
        for( curr_node = iis_dlls; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
            if( GetString( curr_node ) == NULL ) {
               continue;
            }
            strcpy( src_path, syb_path );
            strcat( src_path, GetString( curr_node ) );
            strcpy( dst_path, full_dll_dir );
            strcat( dst_path, GetString( curr_node ) );
            if( stricmp( src_path, dst_path ) != 0 ) {
                if( !NIDCopyFile( src_path, dst_path ) ) {
                    return( FALSE );
                }
            }
        }
        StatusShow( FALSE );
    }

    DeleteList( iis_dlls );
    if( default_dir ) {
        GUIFree( default_dir );
    }
    SetVariableByName( A_SERVER_CONFIGURED, "1" );
    return( TRUE );
} // IISSetup end

//NSSetup
static bool NSSetup( node * server_list )
/***************************************/
// Modify the obj.conf files for Netscape Server:s
{
    node                *curr_server = NULL;
    node                *curr_objconf = NULL;
    char                netscape_server_n[ sizeof( NETSCAPESERVER_N ) ];
    char                objconf_list_n[ sizeof( OBJCONFLIST_N ) ];
    char                netscape_config_n[ sizeof( NETSCAPE_CONFIG_N ) ];
    int                 i;
    int                 server_n;
    char                dll_dir[ _MAX_PATH ];
    char                full_dll_dir[ _MAX_PATH ];
    char                syb_path[ _MAX_PATH ];
    char                src_path[ _MAX_PATH ];
    char                dst_path[ _MAX_PATH ];
    char                buff[ 4096 ];
    node                *ns_dlls = NULL;
    node                *curr_node = NULL;
    bool                user_wants_to_configure;
    gui_message_return  ret;
    long                space;

    SetVariableByName( WEB_SERVER_DIR_FOUND, "0" );
    ns_dlls = MakeListFromString( NSDLLS );
    if( ns_dlls == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }

    *syb_path = '\0';
    if( !VerifyDynamoFiles( GetVariableStrVal( "IDS_NID_NS" ), ns_dlls, syb_path, FALSE ) ) {
        return( FALSE );
    }
    if( *syb_path == '\0' ) {
        return( TRUE ); // can't set up this server in the first place
    }

    if( server_list == NULL ) {
        return( FALSE );
    }

    EndSlash( syb_path );
    strcpy( netscape_server_n, NETSCAPESERVER_N );
    strcpy( objconf_list_n, OBJCONFLIST_N );
    curr_server = server_list;
    strcpy( netscape_config_n, NETSCAPE_CONFIG_N );
    user_wants_to_configure = FALSE;

    for( server_n = 1; curr_server != NULL;
         server_n++, curr_server = GetNextNode( curr_server ) ) {
        if( GetServer( curr_server )->version == NS_3 ) {
            continue;   // Don't configure Netscape v.3 for now
        }
        lastchar( netscape_config_n ) = ASCII_0 + server_n;
        lastchar( objconf_list_n ) = ASCII_0 + server_n;

        //============================== BY THIS POINT, THE NS SERVER IS DETECTED
        //========================================================================
        SetVariableByName( A_SERVER_DETECTED, "1" );

        SetVariableByName( WEB_SERVER_DIR_FOUND, "1" );
        SetVariableByName( WEB_SERVER_DIR, GetServer( curr_server )->root );
        SetVariableByName( WEB_SERVER_NAME, GetVariableStrVal( "IDS_NID_NS" ) );
        if( DoDialog( SERVER_DETECTED ) != DLG_NEXT ) {  // configure server y/n?
            return( FALSE );
        }

        if( GetVariableIntVal( DO_SERVER_SETUP ) == 0 ) {
            curr_server = GetNextNode( curr_server );
            continue;
        } else {
            user_wants_to_configure = TRUE;
        }

        // Make a dialog that asks how to configure Netscape Server
        if( !NewDialog( netscape_config_n ) ) {
            return( FALSE );
        }
        AddControl( SETWIDTH DIALOG_WIDTH, NULL );
        AddControl( STATIC_TEXT "\"%s %s\"",
                    GetVariableStrVal( "IDS_NSCONFIG_1" ),
                    GetServer( curr_server )->root, NULL );
        AddControl( STATIC_TEXT "\"%s\"",
                    GetVariableStrVal( "IDS_NSCONFIG_2" ), NULL );
        AddControl( STATIC_TEXT, NULL );
        AddControl( STATIC_TEXT "\"%s\"",
                    GetVariableStrVal( "IDS_NSCONFIG_3" ), NULL );
        sprintf( buff, RADIO NS_SYBTOOLS "," CHECK_TRUE ",\"%s\"", // recommended choice
                       GetVariableStrVal( "IDS_NSCONFIG_4" ) );
        AddControl( buff, syb_path, NULL );
        sprintf( buff, RADIO NS_DEFAULT "," CHECK_FALSE ",\"%s\"",
                       GetVariableStrVal( "IDS_NSCONFIG_5" ) );
        AddControl( buff, GetServer( curr_server )->dll_path, NULL );
        AddControl( RADIO NS_CUSTOM "," CHECK_FALSE ",\"%s\"",
                    GetVariableStrVal( "IDS_NSCONFIG_6" ), NULL );
        AddControl( STATIC_TEXT "\"\",," NS_CUSTOM, NULL );
        AddControl( EDIT NS_CUSTOM_DLLDIR ",,\"%s\",,%s",
//                  GetServer( curr_server )->dll_path,
                    GetVariableStrVal( "IDS_NSCONFIG_7" ),
                    NS_CUSTOM, NULL );
        AddControl( STATIC_TEXT, NULL );
        sprintf( buff, STATIC_TEXT "\"%s\"", GetVariableStrVal( "IDS_NSCONFIG_8" ) );
        AddControl( buff, OBJCONF, OBJCONF, NULL );
        sprintf( buff, DETAILCHECK "%s,\"%s\"," MODIFY_OBJCONF_FILES "," CHECK_TRUE ",\"%s\"",
                       objconf_list_n, GetVariableStrVal( "IDS_NSCONFIG_9" ),
                       GetVariableStrVal( "IDS_NSCONFIG_10" ) );
        AddControl( buff, OBJCONF, NULL );

        AddControl( STATIC_TEXT "\"\",," MODIFY_OBJCONF_FILES, NULL );
        sprintf( buff, STATIC_TEXT "\"%s\",," MODIFY_OBJCONF_FILES,
                       GetVariableStrVal( "IDS_NSCONFIG_12" ) );
        AddControl( buff, OBJCONF, BACKUP_EXT, NULL );
        sprintf( buff, CHECKBOX BACKUPOBJCONF "," CHECK_TRUE ",\"%s\",," MODIFY_OBJCONF_FILES,
                       GetVariableStrVal( "IDS_NSCONFIG_13" ) );
        AddControl( buff, OBJCONF, NULL );
        AddControl( STATIC_TEXT, NULL );
        if( !AddButton( DIALOG_OK_BUTTON ) ) {
            return( FALSE );
        }
        if( !AddButton( DIALOG_CANCEL_BUTTON ) ) {
            return( FALSE );
        }

        // Make a child dialog that asks which of the detected servers to update
        if( !NewDialog( objconf_list_n ) ) {
            return( FALSE );
        }
        AddControl( ANYCHECK MODIFY_OBJCONF_FILES, NULL );
        AddControl( SETWIDTH DIALOG_WIDTH, NULL );
        sprintf( buff, STATIC_TEXT "\"%s\"", GetVariableStrVal( "IDS_NSCONFIG_11" ) );
        AddControl( buff, OBJCONF, OBJCONF, NULL );
        curr_objconf = GetServer( curr_server )->objconfs;
        // add a checkbox for each obj.conf file under this install of Netscape Server
        for( i = START_INDEX; curr_objconf != NULL; i++ ) {
            lastchar( netscape_server_n ) = i + ASCII_0;
            SetVariableByName( netscape_server_n, "1" );  // Set these to true
                                                          // in case this dialog never comes up
            AddControl( CHECKBOX "%s," CHECK_TRUE ",\"%s\"",
                        netscape_server_n,
                        GetString( curr_objconf ), NULL );
            curr_objconf = GetNextNode( curr_objconf );
        }
        AddControl( STATIC_TEXT, NULL );
        if( !AddButton( DIALOG_OK_BUTTON ) ) {
            return( FALSE );
        }

        // Dialog loop
        for( ;; ) {
            if( DoDialog( netscape_config_n ) != DLG_NEXT ) {
                return( FALSE );
            }

            // Find out where the user wants to put the NSAPI DLLS
            if( GetVariableIntVal( NS_DEFAULT ) != 0 ) {
                strcpy( dll_dir, GetServer( curr_server )->dll_path );
            } else if( GetVariableIntVal( NS_SYBTOOLS ) != 0 )  {
                strcpy( dll_dir, syb_path );
            } else {
                strcpy( dll_dir, GetVariableStrVal( NS_CUSTOM_DLLDIR ) );
            }
            if( *dll_dir == '\0' ) {
                continue;
            }

            // Make sure that the directory for the DLLS is valid
            _fullpath( full_dll_dir, dll_dir, sizeof( full_dll_dir ) );
            EndSlash( full_dll_dir );
            if( access( full_dll_dir, F_OK ) != 0 ) {
                ret = MsgBox( NULL, "IDS_NODIR", GUI_YES_NO_CANCEL, full_dll_dir );
                if( ret == GUI_RET_CANCEL ) {
                    return( FALSE );
                } else if( ret == GUI_RET_NO ) {
                    continue;
                }
                if( mkdir( full_dll_dir ) != 0 ) {
                    if( MsgBox( NULL, "IDS_CANTMAKEDIR", GUI_OK_CANCEL, full_dll_dir ) == GUI_RET_CANCEL ) {
                        return( FALSE );
                    }
                    continue;
                }
            } else {
                if( !IsDir( full_dll_dir ) ) {
                    if( MsgBox( NULL, "IDS_INVALID_DIR", GUI_OK_CANCEL, full_dll_dir ) == GUI_RET_CANCEL ) {
                        return( FALSE );
                    }
                    continue;
                }
            }

            // input from dialog is valid, so go on.
            break;

        }

        if( GetVariableIntVal( NS_SYBTOOLS ) == 0
            && stricmp( syb_path, full_dll_dir ) != 0 ) {
            // Is there enough space to copy ns_dlls from syb_path to full_dll_dir?
            if( !IsSpaceForFiles( ns_dlls, syb_path, full_dll_dir, &space ) ) {
                return( FALSE );
            }
            StatusAmount( 0, space );

            // Copy DLLs
            StatusShow( TRUE );
            for( curr_node = ns_dlls; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
                if( GetString( curr_node ) == NULL ) {
                   continue;
                }
                strcpy( src_path, syb_path );
                strcat( src_path, GetString( curr_node ) );
                strcpy( dst_path, full_dll_dir );
                strcat( dst_path, GetString( curr_node ) );
                if( stricmp( src_path, dst_path ) != 0 ) {
                    if( !NIDCopyFile( src_path, dst_path ) ) {
                        return( FALSE );
                    }
                }
            }
            StatusShow( FALSE );
        }

        BackSlashes( full_dll_dir );
        if( lastchar( full_dll_dir ) == '\\' ) {
            lastchar( full_dll_dir ) = '\0';
        }

        // Add full_dll_dir to the system path
        if( !AddStringToSystemEnvironmentVar( PATH, full_dll_dir, TRUE ) ) {
            return( FALSE );
        }

        strcat( full_dll_dir, "\\" );

        curr_objconf = GetServer( curr_server )->objconfs;
        if( GetVariableIntVal( MODIFY_OBJCONF_FILES ) ) {
            // Update the selected server config files
            for( i = START_INDEX; curr_objconf != NULL; i++ ) {
                lastchar( netscape_server_n ) = i + ASCII_0;
                if( GetVariableIntVal( netscape_server_n ) != 1
                    || GetString( curr_objconf ) == NULL ) {
                    curr_objconf = GetNextNode( curr_objconf );
                    continue;
                }

                if( !ModifyNetscapeOBJCONF( GetString( curr_objconf ),
                    full_dll_dir ) ) {
                    if( MsgBox( NULL, "IDS_FAILED_MODIFYING_FILE", GUI_YES_NO,
                                GetString( curr_objconf ) ) == GUI_RET_NO ) {
                        return( FALSE );
                    }

                }
                curr_objconf = GetNextNode( curr_objconf );
            }
        }
    } //for loop

    DeleteList( ns_dlls );
    if( user_wants_to_configure ) {
        SetVariableByName( A_SERVER_CONFIGURED, "1" );
    }
    return( TRUE );
}  // NSSetup end

//FindNetscapeServers
static node * FindNetscapeServers( HKEY hkey, node * server_list )
/********************************************************************/
// Recursively find Netscape server directories under the registry key
// passed in and add them (disk directory) to the list
{
    int         rc;
    node        *subkeys_list = NULL;
    node        *curr_key = NULL;
    node        *objconf = NULL;
    char        server_path[ _MAX_PATH ];
    char        config_file[ _MAX_PATH ];
    char        *parent = NULL;
    char        *root = NULL;
    char        dll_path[ _MAX_PATH ];
    DWORD       size;
    HKEY        sub_hkey;
    ns_version  version;
    ns_server   *server;

    subkeys_list = ListRegSubkeys( hkey );
    for( curr_key = subkeys_list; curr_key != NULL; curr_key = GetNextNode( curr_key ) ) {
        if( GetString( curr_key ) == NULL ) {
            continue;
        }
        rc = RegOpenKeyEx( hkey, GetString( curr_key ), NULL, KEY_READ, &sub_hkey );
        if( rc != 0 ) {
            continue;
        }
        if( strnicmp( GetString( curr_key ), HTTPS, length( HTTPS ) ) == 0 ) {
            for( ;; ) {
                rc = RegQueryValueEx( sub_hkey, CONFIGPATH, NULL, NULL, NULL, &size );
                if( rc != 0 ) {
                    break;
                }
                // There is an entry in this subkey called CONFIGPATH

                rc = RegQueryValueEx( sub_hkey, CONFIGPATH, NULL, NULL, &server_path, &size );
                if( rc != 0 || server_path == NULL ) {
                    break;
                }

                BackSlashes( server_path );
                if( !IsDir( server_path ) ) {
                    break;
                }
                // CONFIGPATH exists on disk

                sprintf( config_file, "%s\\" OBJCONF, server_path );
                if( !IsFile( config_file ) ) {
                    break;
                }
                // config_file exists

                parent   = Parent( server_path );
                root     = Parent( parent );

                GUIFree( parent );

                strcpy( dll_path, root );

                if( !IsDir( root ) ) {
                    break;
                }
                // root exists

                strcat( dll_path, PLUGINS );

                strcat( dll_path, DYNAMO );
                // dll_path will be a choice for where to place to put dynamo DLLs.

                // Add more cool requirements here!!

                // A Netscape server exists!

                if( strnicmp( GetString( curr_key ), HTTPS20, length( HTTPS20 ) ) == 0 ) {
                    version = NS_2;
                } else if( strnicmp( GetString( curr_key ), HTTPS30, length( HTTPS30 ) ) == 0 ) {
                    version = NS_3;
                }

                server = FindNSServer( server_list, root, version );
                if( server == NULL ) {
                    server_list = AddNSServer( server_list, root, dll_path, config_file, version );
                } else {
                    objconf = FindString( server->objconfs, config_file, FALSE );
                    if( objconf == NULL ) {
                        server->objconfs = AddString( server->objconfs, config_file );
                    }
                }
                break;
            }
        } else {
            server_list = FindNetscapeServers( sub_hkey, server_list );
        }
        RegCloseKey( sub_hkey );
    }
    DeleteList( subkeys_list );
    return( server_list );
}

//ModifyNetscapeOBJCONF
static bool ModifyNetscapeOBJCONF( char const *path, char const *ninspath )
/**************************************************************************/
{
    FILE                *fp = NULL;
    struct stat         buf;
    long                size_read;
    char                *text = NULL;
    char                *line = NULL;
    char                ninspath_copy[ _MAX_PATH ];
    char                dll_path[ _MAX_PATH ];
    char                backup_path[ _MAX_PATH ];
    char                drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ];
    char                filename[ _MAX_FNAME ];
    bool                InitWritten;
    bool                NameTransWritten;;
    bool                PathCheckWritten;
    obj_conf_state      state;
    long                space;

    if( path == NULL ) {
        return( FALSE );
    }

    strcpy( dll_path, path );

    if( stat( dll_path, &buf ) != 0 ) {
        return( FALSE );
    }
    text = GUIAlloc( buf.st_size + 1 );
    if( text == NULL ) {
        return( FALSE );
    }

    // Read the file
    fp = fopen( dll_path, "rb" );
    if( fp == NULL ) {
        return( FALSE );
    }
    size_read = fread( text, sizeof( *text ), buf.st_size, fp );
    text[ size_read ] = '\0';

    if( size_read != buf.st_size ) {
        return( FALSE );
    }

    fclose( fp );

    // Backup the file to filename.bak
    _splitpath( dll_path, drive, dir, filename, NULL );
    _makepath( backup_path, drive, dir, filename, BACKUP_EXT );
    if( GetVariableIntVal( BACKUPOBJCONF ) ) {
        if( !IsSpaceForFile( dll_path, backup_path, &space ) ) {
            return( FALSE );
        }
        StatusAmount( 0, space );
        if( !NIDCopyFile( dll_path, backup_path ) ) {
            if( MsgBox( NULL, "IDS_FAILEDBACKUP", GUI_YES_NO, dll_path, backup_path ) == GUI_RET_NO ) {
                return( FALSE );
            }
        }
        StatusAmount( 1, 1 );
        StatusShow( FALSE );
    }

    // Open the file, truncating it to zero length
    fp = fopen( path, "wt" );
    if( fp == NULL ) {
        return( FALSE );
    }

    line = strtok( text, "\r" );

    // Write the beggining section of the file (comments and empty lines)
    // unchanged from the original
    while( line != NULL &&
           ( *line == '\0' || *line == '#' ) ) {
        fprintf( fp, "%s", line );      //write TEXT AT BEGINNING OF FILE
        line = strtok( NULL, "\r" );
        while( *line == '\n' ) {
            fprintf( fp, "\n" );
            line++;
        }
    }

    // Process the rest of the original file, remove lines that are going to
    // be added (the old ones may be incorrect), and add some new lines.
    InitWritten      = FALSE;
    NameTransWritten = FALSE;
    PathCheckWritten = FALSE;
    state            = OBJ_CONF_ROOT;
    while( line != NULL ) {
        if( strcmp( line, "\n" ) == 0 ) { // newline alone:
            fprintf( fp, "\n" );          // write NEWLINE
            line = strtok( NULL, "\r" );
            continue;
        }
        if( *line == '\n' ) { //newline then text: ignore the newline
            line++;
        }
        if( *line == '#' ) {
            fprintf( fp, "%s\n", line ); // write COMMENTS right away just in case
            line = strtok( NULL, "\r" );
            continue;
        }


        if( state & OBJ_CONF_ROOT ) {
            // Get rid of multiple consecutive OBJECTEND lines.
            if( state & OBJ_CONF_OBJECTEND_SEEN && strcmp( line, OBJECTEND ) == 0 ) {
                line = strtok( NULL, "\r" );
                continue;
            }

            if( !InitWritten ) {
                strcpy( ninspath_copy, ninspath );
                ForwardSlashes( ninspath_copy );
                fprintf( fp, INITFN "\n", ninspath_copy ); //write INITFN
                InitWritten = TRUE;
            }

            // Discard the whole NETIMPACT_OBJ section
            if( strcmp( line, NETIMPACT_OBJ ) == 0 ) {
                while( line != NULL && strcmp( line, OBJECTEND ) != 0 ) {
                    line = strtok( NULL, "\r" );
                    if( line != NULL && *line == '\n' ) {
                        line++;
                    }
                }
                line = strtok( NULL, "\r" );

                // get rid of extra spaces
                while( line != NULL && strcmp( line, "\n" ) == 0 ) {
                    line = strtok( NULL, "\r" );
                }
                continue;
            }
        }

        if( CheckStrings( line, INITFN )
            || CheckStrings( line, PATHCHECKFN )
            || CheckStrings( line, NAMETRANSFN ) )
        {
            // Skip this line no matter where it is, because it is going to be replaced.
            line = strtok( NULL, "\r" );
            continue;
        }

        if( state & OBJ_CONF_DEFAULT_OBJECT ) {
            if( !NameTransWritten ) {
                fprintf( fp, NAMETRANSFN "\n" ); //write NAMETRANSFN
                NameTransWritten = TRUE;
            }
            if( !PathCheckWritten
                && strncmp( line, NAMETRANS, length( NAMETRANS ) ) != 0 ) {
                fprintf( fp, PATHCHECKFN "\n" ); //write PATHCHECKFN
                PathCheckWritten = TRUE;
            }
            if( strcmp( line, OBJECTEND ) == 0 ) {
                state = OBJ_CONF_ROOT | OBJ_CONF_OBJECTEND_SEEN;
            }
        }

        if( state & OBJ_CONF_IN_OBJECT ) {
            if( strcmp( line, OBJECTEND ) == 0 ) {
                if( strncmp( line, OBJECTSTART, length( OBJECTSTART ) ) == 0 ) {
                    fprintf( fp, OBJECTEND "\n" );  // write OBJECTEND
                }
                state = OBJ_CONF_ROOT | OBJ_CONF_OBJECTEND_SEEN;
            }
        }

        if( state & OBJ_CONF_ROOT && strncmp( line, OBJECTSTART, length( OBJECTSTART ) ) == 0 ) {
            state = OBJ_CONF_IN_OBJECT;
            if( CheckStrings( line, DEFAULT ) ) {
                state |= OBJ_CONF_DEFAULT_OBJECT;
            }
        }

        fprintf( fp, "%s\n", line );    // write LINE FROM ORIGINAL FILE

        line = strtok( NULL, "\r" );
    }

    fprintf( fp, NIOBJECT );            // write NIOBJECT section at end
    fclose( fp );
    GUIFree( text );
    return( TRUE );
}

//VerifyFiles
static bool VerifyFiles( node *file_list, char const *path )
/**********************************************************/
// Make sure all the files in file_list are in the directory
{
    char        full_path[ _MAX_PATH ];
    char        path_copy[ _MAX_PATH ];
    node        *curr_node = NULL;

    if( file_list == NULL || path == NULL ) {
        return( FALSE );
    }

    strcpy( path_copy, path );
    EndSlash( path_copy );

    for( curr_node = file_list; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
        if( GetString( curr_node ) == NULL ) {
            continue;
        }
        strcpy( full_path, path_copy );
        strcat( full_path, GetString( curr_node ) );
        if( !IsFile( full_path ) ) {
            return( FALSE );
        }
    }
    return( TRUE );
}

//VerifyOptimaFiles
static bool VerifyOptimaFiles( bool * verified )
/**********************************************/
// See if the Optima DLLs are in the windows \system32 dir  (\system in Win95)
{
    node        *optima_dlls = NULL;
    char        systemdir[ _MAX_PATH ];
    char        *files = NULL;

    optima_dlls = MakeListFromString( OPTIMA_DLLS );
    GetWindowsDirectory( &systemdir, _MAX_PATH );
    EndSlash( systemdir );
    if( GetVariableIntVal( "IsWinNT" ) == 1 ) {
        strcat( systemdir, SYSTEM32 );
    } else {
        strcat( systemdir, SYSTEM );
    }

    // Optima DLLs must be in the Windows \system32 dir
    if( VerifyFiles( optima_dlls, systemdir ) ) {
        *verified = TRUE;
        return( TRUE );
    }

    *verified = FALSE;

    files = MakeNewlineDelimitedList( optima_dlls );
    if( files == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }
    MsgBox( NULL, "IDS_NID_OPTIMAFILESMISSING", GUI_OK, systemdir, files );
    DeleteList( optima_dlls );
    GUIFree( files );

    return( TRUE );
}

//VerifyDynamoFiles
static bool VerifyDynamoFiles( char const *server_name,
                               node *file_list, char *syb_path, bool cgi )
/************************************************************************/
// Make sure that Dynamo files exist in the sybtools\win32 dir or the sqlany50\win32 dir
// and return the dir in syb_path (assumed to be of size _MAX_PATH.
// If the path is not verified, syb_path will be "".
// If syb_path is not "" when passed in, it will be used as the path to check
// False is returned on errors.
{
    char                candidate[ _MAX_PATH ];
    char                drive[ _MAX_DRIVE ];
    char                dir[ _MAX_DIR ];
    char                *files = NULL;
    char                *temp = NULL;
    node                *curr_node = NULL;
    int                 i;
    #define             NUM_REG_LOCATIONS 3
    reg_location        location[ NUM_REG_LOCATIONS ] =
                        {
                          { HKEY_CURRENT_USER, REG_SYBTOOLS, INSTALL_LOCATION },
                          { HKEY_LOCAL_MACHINE, REG_SYBTOOLS, INSTALL_LOCATION },
                          { HKEY_CURRENT_USER, REG_SQLANY, INSTALL_LOCATION }
                        };

    if( syb_path == NULL ) {
        return( FALSE );
    }

    if( file_list == NULL || server_name == NULL ) {
        return( FALSE );
    }

    if( *syb_path != '\0' ) {
        EndSlash( syb_path );
        strcpy( candidate, syb_path );
        if( VerifyFiles( file_list, candidate ) ) {
            return( TRUE );
        }
        strcpy( syb_path, "" );
    } else {
        // check the sybtools variable for the sybase tools dir, (add \win32 to it)
        strcpy( candidate, GetVariableStrVal( SYBTOOLS ) );
        if( *candidate != '\0' ) {
            EndSlash( candidate );
            strcat( candidate, WIN32_DIR );
            if( VerifyFiles( file_list, candidate ) ) {
                strcpy( syb_path, candidate );
                return( TRUE );
            }
        }

        // check registry locations for possible sybase tools/sqlany dirs (add \win32 to them)
        for( i = 0; i < NUM_REG_LOCATIONS; i++ ) {
            temp = ReadRegValue( location[i].root, location[i].sub_key, location[i].name );
            if( temp != NULL ) {
                strcpy( candidate, temp );
                GUIFree( temp );
                EndSlash( candidate );
                strcat( candidate, WIN32_DIR );
                if( VerifyFiles( file_list, candidate ) ) {
                    strcpy( syb_path, candidate );
                    return( TRUE );
                }
            }
        }

        // As a last resort, check the path for each file in the list.
        // When a file is found, search that dir for all of the files.
        for( curr_node = file_list; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
            if( GetString( curr_node ) == NULL ) {
                continue;
            }
            _searchenv( GetString( curr_node ), PATH, candidate );
            if( *candidate == '\0' ) {
                break;
            }
            _splitpath( candidate, drive, dir, NULL, NULL );
            _makepath( candidate, drive, dir, NULL, NULL );
            EndSlash( candidate );
            if( VerifyFiles( file_list, candidate ) ) {
                strcpy( syb_path, candidate );
                return( TRUE );
            }
        }
    }

    files = MakeNewlineDelimitedList( file_list );
    if( files == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }
    if( cgi ) {
        MsgBox( NULL, "IDS_NID_CANTSETUPCGI", GUI_OK, server_name, files );
    } else {
        MsgBox( NULL, "IDS_NID_CANTSETUPSERVERS", GUI_OK, server_name, files );
    }
    GUIFree( files );
    return( TRUE );

}



//AddStringToSystemEnvironmentVar
static bool AddStringToSystemEnvironmentVar( char const *var,
                                             char const *string,
                                             bool add )
/***************************************************************/
// Add or remove a string from a particular variable in the local machine
// environment.  If the string already exists, it will not be repeated.
// This works with ; separated values.
{
    char        *old_string = NULL;
    char        *new_string = NULL;

    if( var == NULL || string == NULL ) {
        return( FALSE );
    }

    old_string = ReadRegValue( HKEY_LOCAL_MACHINE, SYSTEMENVIRONMENT, var );
    if( old_string == NULL ) {
        GUIStrDup( (char * ) string, &old_string );
        if( old_string == NULL ) {
            MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
            return( FALSE );
        }
    }

    NoDupPaths( (char *) string, old_string, TRUE, ';' );
    new_string = GUIAlloc( strlen( old_string ) + strlen( string ) + 1 );
    if( new_string == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        return( FALSE );
    }

    if( add ) {
        sprintf( new_string, "%s%s", old_string, string );
    } else {
        strcpy( new_string, old_string );
    }

    if( !AddRegString( HKEY_LOCAL_MACHINE, SYSTEMENVIRONMENT, var, new_string ) ) {
        return( FALSE );
    }

    GUIFree( old_string );
    GUIFree( new_string );
    return( TRUE );
}

//AddRegString
static bool AddRegString( HKEY root, char const * key, char const *name, char const *value )
/**************************************************************************************************/
// Add or remove a name and value from the registry.
// Remove if value == NULL
{
    HKEY        hkey;
    int         rc;
    DWORD       type;
    DWORD       old_type;

    if( key == NULL || name == NULL ) {
        return( FALSE );
    }

    type = REG_SZ;
    if( value == NULL ) {
        rc = RegOpenKeyEx( root, key, NULL, KEY_WRITE, &hkey );
        if( rc != 0 ) {
            return( TRUE ); // no key means no values underneath it that can be deleted
        }
        rc = RegDeleteValue( hkey, name );

    } else {
        rc = RegCreateKeyEx( root, key, 0, NULL, REG_OPTION_NON_VOLATILE,
                             KEY_ALL_ACCESS, NULL, &hkey, NULL );
        if( rc != 0 ) {
            MsgBox( NULL, "IDS_REGERROR", GUI_OK );
            return( FALSE );
        }
        rc = RegQueryValueEx( hkey, name, NULL, &old_type, NULL, NULL );
        if( rc == 0 ) {
            type = old_type;
        }
        rc = RegSetValueEx( hkey, name, NULL, type, value, strlen( value ) + 1 );
    }

    RegCloseKey( hkey );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REGERROR", GUI_OK );
        return( FALSE );
    }
    return( TRUE );
}

//RemoveFileFromString
static char * RemoveFileFromString( char const *original, char const *path, char delim )
/**************************************************************************************/
// Remove any paths from a string of paths separated by delim, if they
// have the filename contained in the path parameter
// BUT:  Returns a buffer with the new string along with at least enough space
//       to allow delim + path to be added to the end.
{
    char        *new_string = NULL;
    char        *index = NULL;
    char        *orig_copy = NULL;
    char        origfile[ _MAX_FNAME + _MAX_EXT ];
    char        origext[ _MAX_EXT ];
    char        file[ _MAX_FNAME + _MAX_EXT ];
    char        ext[ _MAX_EXT ];
    char        dlm[ 2 ];

    if( original == NULL || original == '\0' ) {
        GUIStrDup( (char *) path, &new_string );
        return( new_string );
    }


    if( path == NULL ) {
        GUIStrDup( (char *) original, &new_string );
        return( new_string );
    }

    _splitpath( path, NULL, NULL, origfile, origext );
    if( *origfile == '\0' ) {
        GUIStrDup( (char *) original, &new_string );
        return( new_string );
    }
    strcat( origfile, origext );

    new_string = GUIAlloc( strlen( original ) + strlen( path ) + 2 );
    if( new_string == NULL ) {
        return( NULL );
    }
    *new_string = '\0';

    GUIStrDup( (char *) original, &orig_copy );
    if( orig_copy == NULL ) {
        GUIFree( new_string );
        return( NULL );
    }

    dlm[ 0 ] = delim;
    dlm[ 1 ] = '\0';

    index = strtok( orig_copy, dlm );
    while( index != NULL ) {
        _splitpath( index, NULL, NULL, file, ext );
        strcat( file, ext );
        if( stricmp( file, origfile ) != 0 ) {
            strcat( new_string, index );
            strcat( new_string, dlm );
        }
        index = strtok( NULL, dlm );
    }
    lastchar( new_string ) = '\0'; // get rid of extra dlm

    GUIFree( orig_copy );
    return( new_string );

}

//AddFilterDLLString
static bool AddFilterDLLString( char const * fullpath, bool add )
/****************************************************************/
// Add (or remove) fullpath from the Registry entrykey shown...
{
    HKEY        hkey;
    int         rc;
    DWORD       size;
    DWORD       type;
    DWORD       old_type;
    char        *old_value = NULL;
    char        *new_value = NULL;

    if( fullpath == NULL ) {
        return( FALSE );
    }

    rc = RegCreateKeyEx( HKEY_LOCAL_MACHINE, W3SVCPARMS, 0, NULL, REG_OPTION_NON_VOLATILE,
                         KEY_ALL_ACCESS, NULL, &hkey, NULL );

    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REGERROR", GUI_OK );
    }
    type = REG_SZ;
    rc = RegQueryValueEx( hkey, FILTERDLLS, NULL, NULL, NULL, &size );
    if( rc != 0 ) {
        GUIStrDup( "", &old_value );
        if( old_value == NULL ) {
            MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
            RegCloseKey( hkey );
            return( FALSE );
        }
    } else {
        old_value = GUIAlloc( size + 1 );

        if( old_value == NULL ) {
            MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
            RegCloseKey( hkey );
            return( FALSE );
        }
        rc = RegQueryValueEx( hkey, FILTERDLLS, NULL, &old_type, old_value, &size );

        if( rc != 0 ) {
            *old_value = '\0';
            type = old_type;
        }
    }
    new_value = RemoveFileFromString( old_value, fullpath, ',' );
    if( new_value == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }
    if( add ) {
        if( *new_value != '\0' ) {
            strcat( new_value, "," );
        }
        strcat( new_value, fullpath );
    }

    rc = RegSetValueEx( hkey, FILTERDLLS, NULL, type, new_value, strlen( new_value ) + 1 );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REGERROR", GUI_OK );
    }

    RegCloseKey( hkey );
    GUIFree( old_value );
    GUIFree( new_value );
    return( TRUE );

}


//ReadRegValue
static char * ReadRegValue( HKEY root, char const *key, char const *name )
/**************************************************************************/
// Don't forget to free the string returned if you need to.
{
    int                 rc;
    HKEY                hkey;
    unsigned long       size;
    char                *value = NULL;

    if( key == NULL ) {
        return( NULL );
    }

    rc = RegOpenKeyEx( root, key, NULL, KEY_READ, &hkey );
    if( rc != 0 ) {
        return( NULL );
    }

    rc = RegQueryValueEx( hkey, name, NULL, NULL, NULL, &size );
    if( rc != 0 ) {
        RegCloseKey( hkey );
        return( NULL );
    }

    value = GUIAlloc( size );
    if( value == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        RegCloseKey( hkey );
        return( NULL );
    }

    rc = RegQueryValueEx( hkey, name, NULL, NULL, value, &size );

    RegCloseKey( hkey );

    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REGERROR", GUI_OK );
        GUIFree( value );
        return( NULL );
    }

    return( value );
}

//ListRegSubKeys
static node * ListRegSubkeys( HKEY key )
/***************************************/
// List the subkeys in an open registry key
{
    DWORD               i;
    char                *string = NULL;
    DWORD               max_size;
    DWORD               string_size;
    DWORD               num_keys;
    node                *list = NULL;

    if( RegQueryInfoKey( key, NULL, NULL, NULL, &num_keys, &max_size,
                         NULL, NULL, NULL, NULL, NULL, NULL ) != ERROR_SUCCESS ) {
        return( NULL );
    }

    max_size += 1;
    string = GUIAlloc( max_size );
    if( string == NULL ) {
        return( NULL );
    }

    list = NULL;

    for( i = 0; i < num_keys; i++ ) {
        string_size = max_size;
        if( RegEnumKeyEx( key, i, string, &string_size,
                          NULL, NULL, NULL, NULL )
                          != ERROR_SUCCESS ) {
            break;
        }
        if( string != NULL ) {
            list = AddString( list, string );
        }
    }

    GUIFree( string );
    return( list );
}

//ListRegValues
static node * ListRegValues( HKEY key )
/*************************************/
// List the values in an open registry key
{
    long        i;
    char        *string = NULL;
    DWORD       max_size;
    DWORD       string_size;
    DWORD       num_values;
    node        *list = NULL;

    if( RegQueryInfoKey( key, NULL, NULL, NULL, NULL, NULL, NULL,
                         &num_values, &max_size, NULL, NULL, NULL )
                         != ERROR_SUCCESS ) {
        return( NULL );
    }

    max_size += 1;
    string = GUIAlloc( max_size );
    if( string == NULL ) {
        return( NULL );
    }

    list = NULL;

    for( i = 0; i < num_values; i++ ) {
        string_size = max_size;
        if( RegEnumValue( key, i, string, &string_size,
                          NULL, NULL, NULL, NULL ) != ERROR_SUCCESS ) {
            break;
        }
        if( string != NULL ) {
            list = AddString( list, string );
        }
    }

    GUIFree( string );
    return( list );
}

//StripQuotes
static char * StripQuotes( char const *input )
/********************************************/
// Don't forget to free the string returned if you need to.
{
    char const *index = NULL;
    char       *result = NULL;
    char       *result_index = NULL;

    if( input == NULL ) {
        return( NULL );
    }

    result = GUIAlloc( strlen( input ) + 1 );
    result_index = result;
    index = input;

    while( *index != '\0' ) {
        if( *index != '\"' ) {
            *result_index = *index;
            result_index++;
        }
        index++;
    }
    *result_index = '\0';

    return( result );
}

//IsSpaceForFiles
static bool IsSpaceForFiles( node *files, char const *src_path,
                             char const *dst_path, long *space_req )
/******************************************************************/
{
    node        *curr_node = NULL;
    long        cs;
    long        space_total;
    long        free_space;
    bool        guess_cs;
    char        path[ _MAX_PATH ];
    char        src[ _MAX_PATH ];
    struct stat buf;

    if( space_req == NULL ) {
        return( FALSE );
    }
    *space_req = -1;
    if( files == NULL || src_path == NULL || dst_path == NULL ) {
        return( FALSE );
    }
    strcpy( src, src_path );

    cs = ClusterSize( (char *) dst_path );
    guess_cs = FALSE;

    if( cs == 0 ) {
        cs = 32728;  // gererously large guess
        guess_cs = TRUE;
    }
    free_space = FreeSpace( (char *) dst_path );

    space_total = 0;
    EndSlash( src );

    for( curr_node = files; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
        if( GetString( curr_node ) == NULL ) {
           continue;
        }
        strcpy( path, src );
        strcat( path, GetString( curr_node ) );
        if( stat( path, &buf ) != -1 ) {
            space_total += RoundUp( buf.st_size, cs );
        }
    }
    if( guess_cs && space_total > 0 ) {
        if( MsgBox( NULL, "IDS_NODESTINFO", GUI_YES_NO, dst_path, space_total / 1024 )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( guess_cs && free_space == 0 ) {
        if( MsgBox( NULL, "IDS_NOFREESPACEINFO", GUI_YES_NO, dst_path, space_total / 1024 )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( space_total == 0 ) {
        if( MsgBox( NULL, "IDS_NOSRCINFO", GUI_YES_NO, src )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( free_space < space_total ) {
        if( MsgBox( NULL, "IDS_NODISKSPACE_UNC", GUI_YES_NO, dst_path,
            free_space / 1024, space_total / 1024 ) == GUI_RET_NO ) {
            return( FALSE );
        }
    }

    *space_req = space_total;
    return( TRUE );
}

//IsSpaceForFile
static bool IsSpaceForFile( char const *src_path, char const *dst_path,
                            long *space_req )
/**********************************************************************/
{
    long        cs;
    long        space_total;
    long        free_space;
    bool        guess_cs;
    char        drive[ _MAX_DRIVE ];
    char        dir[ _MAX_DIR ];
    char        dst_dir[ _MAX_PATH ];
    char        src_dir[ _MAX_PATH ];
    struct stat buf;

    if( space_req == NULL ) {
        return( FALSE );
    }
    *space_req = -1;

    if( src_path == NULL || dst_path == NULL ) {
        return( FALSE );
    }

    _splitpath( dst_path, drive, dir, NULL, NULL );
    _makepath( dst_dir, drive, dir, NULL, NULL );
    _splitpath( src_path, drive, dir, NULL, NULL );
    _makepath( src_dir, drive, dir, NULL, NULL );
    cs = ClusterSize( (char *) dst_dir );
    guess_cs = FALSE;

    if( cs == 0 ) {
        cs = 8192;
        guess_cs = TRUE;
    }
    free_space = FreeSpace( (char *) dst_dir );

    if( stat( src_dir, &buf ) != -1 ) {
        space_total = RoundUp( buf.st_size, cs );
    }
    if( guess_cs && space_total > 0 ) {
        if( MsgBox( NULL, "IDS_NODESTINFO", GUI_YES_NO, dst_dir, space_total / 1024 )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( space_total == 0 ) {
        if( MsgBox( NULL, "IDS_NOSRCINFO", GUI_YES_NO, src_dir )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( !guess_cs && free_space == 0 ) {
        if( MsgBox( NULL, "IDS_NOFREESPACEINFO", GUI_YES_NO, dst_dir, space_total / 1024 )
            == GUI_RET_NO ) {
            return( FALSE );
        }
    } else if( free_space < space_total ) {
        if( MsgBox( NULL, "IDS_NODISKSPACE_UNC", GUI_YES_NO, dst_dir,
            free_space / 1024, space_total / 1024 ) == GUI_RET_NO ) {
            return( FALSE );
        }
    }

    *space_req = space_total;
    return( TRUE );
}

//Parent
static char * Parent( char const *path )
/**************************************/
{
    char buff[ _MAX_PATH ];
    char *index = NULL;
    char *parent = NULL;

    if( path == NULL ) {
        return( NULL );
    }

    strcpy( buff, path );

    if( lastchar( buff ) == '\\' ) {
        lastchar( buff ) = '\0';
    }
    index = &lastchar( buff );
    while( *index != '\\' && *index != '/' && index >= buff ) {
        *index = '\0';
        index--;
    }
    GUIStrDup( buff, &parent );
    return( parent );
}

//ForwardSlashes
static bool ForwardSlashes( char *path )
/**************************************/
{
    char *index = NULL;
    char *end = NULL;
    if( path == NULL ) {
        return( FALSE );
    }
    index = path;
    end   = &lastchar( path );
    while( index <= end ) {
        if( *index == '\\' ) {
            *index = '/';
        }
        index++;
    }
    return( TRUE );
}

//BackSlashes
static bool BackSlashes( char *path )
/***********************************/
{
    char *index = NULL;
    char *end = NULL;
    if( path == NULL ) {
        return( FALSE );
    }
    index = path;
    end   = &lastchar( path );
    while( index <= end ) {
        if( *index == '/' ) {
            *index = '\\';
        }
        index++;
    }
    return( TRUE );
}

//EndSlash
static bool EndSlash( char *path )
/********************************/
{
    if( path == NULL ) {
        return( FALSE );
    }

    BackSlashes( path );
    if( lastchar( path ) != '\\' ) {
        strcat( path, "\\" );
    }
    return( TRUE );
}

//IsFile
static bool IsFile( char const *file )
/************************************/
{
    if( file == NULL ) {
        return( FALSE );
    }
    if( access( file, F_OK ) == 0
        && GetFileAttributes( file ) & ~FILE_ATTRIBUTE_DIRECTORY ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}


//IsDir
static bool IsDir( char const *dir )
/**********************************/
{
    if( dir == NULL ) {
        return( FALSE );
    }
    if( access( dir, F_OK ) == 0
        && GetFileAttributes( dir ) & FILE_ATTRIBUTE_DIRECTORY ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}

//CheckStrings
static bool CheckStrings( char const *test_string, char const *reference_string )
/******************************************************************************/
// Check that test_string and reference_string are the same, apart from %char
// (i.e. %s, %d, etc.) entries in reference_string, where there can be
// gaps in test_string.  [case insensitive]
{
    char        *test_string_index = NULL;
    char        *test_string_copy = NULL;
    char        *reference_string_index = NULL;
    char        *reference_string_copy = NULL;
    char        *reference_string_terminator = NULL;
    bool        ret;


    if( test_string == NULL || reference_string == NULL ) {
        return( FALSE );
    }

    test_string_copy = StripQuotes( test_string );
    reference_string_copy = StripQuotes( reference_string );

    if( reference_string_copy == NULL || test_string_copy == NULL ) {
         return( FALSE );
    }

    test_string_index = test_string_copy;
    reference_string_index = reference_string_copy;

    reference_string_terminator = strstr( reference_string_copy, "%" );
    if( reference_string_terminator != NULL ) {
        *reference_string_terminator = '\0';
    }

    for( ;; ) {

        test_string_index = stristr( (char *)test_string_index, reference_string_index );
        if( test_string_index != NULL ) {
            test_string_index += strlen( reference_string_index );
        } else {
            ret = FALSE; //not all substrings present
            break;
        }
        if( reference_string_terminator == NULL ) { //got to end of reference string
            ret = TRUE; //all substrings present
            break;
        }
        reference_string_index = reference_string_terminator + 2 * sizeof( char );
        reference_string_terminator = strstr( reference_string_index, "%" );
        if( reference_string_terminator != NULL ) {
            *reference_string_terminator = '\0';
        }
    }

    GUIFree( test_string_copy );
    GUIFree( reference_string_copy );
    return( ret );
}
//GetNextNode
static node * GetNextNode( node *curr_node )
/*****************************************/
{
    if( curr_node == NULL ) {
        return( NULL );
    }
    return( curr_node->next );
}

//GetServer
static ns_server * GetServer( node *curr_node )
/*********************************************/
{
    if( curr_node == NULL ) {
        return( FALSE );
    }
    return( (ns_server *)curr_node->data );
}

//GetString
static char * GetString( node *curr_node )
/****************************************/
{
    if( curr_node == NULL ) {
        return( NULL );
    }
    return( (char *) curr_node->data );
}

//AddString
static node * AddString( node *list_node, char const *string )
/************************************************************/
{
    node * new_node = NULL;

    if( string == NULL ) {
        DeleteList( list_node );
        return( NULL );
    }

    new_node = GUIAlloc( sizeof( node ) );
    if( new_node == NULL ) {
        DeleteList( list_node );
        return( NULL );
    }

    GUIStrDup( (char *) string, &new_node->data );
    if( GetString( new_node ) == NULL ) {
        GUIFree( new_node );
        DeleteList( list_node );
        return( NULL );
    }

    new_node->data_t = STRING_T;
    new_node->next = list_node;

    return( new_node );
}

//FindString
static node * FindString( node *list, char const *string, bool case_sensitive )
/*****************************************************************************/
// Find a string in a list
{
    node *curr_node = NULL;

    if( string == NULL || list == NULL ) {
        return( NULL );
    }

    for( curr_node = list; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
        if( GetString( curr_node ) == NULL ) {
           continue;
        }
        if( case_sensitive ) {
            if( strcmp( GetString( curr_node ), string ) == 0 ) {
                return( curr_node );
            }
        } else {
            if( stricmp( GetString( curr_node ), string ) == 0 ) {
                return( curr_node );
            }
        }
    }
    return( NULL );
}

//MakeNewlineDelimitedList
static char *MakeNewlineDelimitedList( node *list )
/*************************************************/
{
    node *curr_node = NULL;
    char *string = NULL;

    if( list == NULL ) {
        return( NULL );
    }

    GUIStrDup( "\0", &string );
    if( string == NULL ) {
        return( NULL );
    }

    for( curr_node = list; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
        if( GetString( curr_node ) == NULL ) {
            continue;
        }
        string = GUIRealloc( string, strlen( string )
                 + strlen( GetString( curr_node ) ) + 2 );
        if( string == NULL ) {
            return( NULL );
        }
        strcat( string, GetString( curr_node ) );
        strcat( string, "\n" );
    }
    return( string );
}


//MakeListFromString
static node * MakeListFromString( char const *input )
/***************************************************/
// Makes a list using a block of memory containing
// consecutive null terminated strings and ultimately
// terminated by an extra null.
// i.e. string1\0string2\0....\0stringN\0\0
{
    node       *list = NULL;
    char const *curr_string = NULL;

    if( input == NULL ) {
        return( NULL );
    }
    curr_string = input;
    list = NULL;
    while( *curr_string != '\0' ) {
        list = AddString( list, curr_string );
        curr_string += strlen( curr_string ) + 1;
    }
    return( list );
}

//AddNSServer
static node * AddNSServer( node *list_node,
                           char const *root,
                           char const *dll_path,
                           char const *objconf,
                           ns_version version )
/******************************************************/
{
    node        *new_node = NULL;
    ns_server   *new_server = NULL;

    new_node = GUIAlloc( sizeof( node ) );
    if( new_node == NULL ) {
        DeleteList( list_node );
        return( NULL );
    }

    new_server = GUIAlloc( sizeof( ns_server ) );
    if( new_server == NULL ) {
        GUIFree( new_node );
        DeleteList( list_node );
        return( NULL );
    }

    if( root == NULL ) {
        new_server->root[ 0 ] = '\0';
    } else {
        strcpy( new_server->root, root );
    }

    if( dll_path == NULL ) {
        new_server->dll_path[ 0 ] = '\0';
    } else {
        strcpy( new_server->dll_path, dll_path );
    }

    new_server->objconfs = AddString( NULL, objconf );
    new_server->version = version;

    new_node->data = new_server;
    new_node->data_t = NS_SERVER_T;
    new_node->next = list_node;
    return( new_node );
}

//FindNSServer
static ns_server * FindNSServer( node *list_node, char const *root, ns_version version )
/**************************************************************************************/
{
    node      *curr_node = NULL;
    ns_server *curr_server = NULL;
    char      path[ _MAX_PATH ];

    if( list_node == NULL || root == NULL ) {
        return( NULL );
    }

    strcpy( path, root );
    path[ _MAX_PATH - 1 ] = '\0';
    if( lastchar( path ) == '\\' ) {
        lastchar( path ) = '\0';
    }

    for( curr_node = list_node; curr_node != NULL; curr_node = GetNextNode( curr_node ) ) {
        if( curr_node->data_t != NS_SERVER_T ) {
            continue;
        }

        curr_server = curr_node->data;
        if( curr_server->version == version
            && curr_server->root != NULL
            && strnicmp( path, curr_server->root, strlen( path ) ) == 0 ) {
            return( curr_server );
        }
    }

    return( NULL );
}

//DeleteList
static bool DeleteList( node *list_node )
/***************************************/
{
    node * temp = NULL;
    if( list_node == NULL ) {
        return( FALSE );
    }
    while( list_node != NULL ) {
        switch( list_node->data_t ) {
        case( STRING_T ):
            GUIFree( list_node->data );
            break;
        case( NS_SERVER_T ):
            DeleteList( GetServer( list_node )->objconfs );
            GUIFree( list_node->data );
        }
        temp = GetNextNode( list_node );
        GUIFree( list_node );
        list_node = temp;
    }
    return( TRUE );
}


//NewDialog
static bool NewDialog( char const *dialog_name )
/**********************************************/
//Start constructing a new dialog as if it is being read from a setup.inf
{
    char * line = NULL;
    if( dialog_name == NULL ) {
        return( FALSE );
    }
    line = GUIAlloc( length( NAME ) + strlen( dialog_name ) + 1 );
    if( line == NULL ) {
        return( FALSE );
    }
    if( !ProcLine( DIALOG, FINAL_SCAN ) ) {
        return( FALSE );
    }
    sprintf( line, NAME "%s", dialog_name );
    if( !ProcLine( line, FINAL_SCAN ) ) {
        return( FALSE );
    }
    return( TRUE );
}

//AddControl
static bool AddControl( char const *control, ... )
/************************************************/
// Add a control to the current dialog as if it is being read from a setup.inf
// Use NewDialog() to start a dialog.
{
    va_list     arglist;
    char        *buff = NULL;
    long        length;
    char const  *curr_arg = NULL;

    if( control == NULL ) {
        return( FALSE );
    }
    va_start( arglist, control );
    length = strlen( control );
    curr_arg = va_arg( arglist, char const * );
    while( curr_arg != NULL ) {
        length += strlen( curr_arg );
        curr_arg = va_arg( arglist, char const * );
    }
    length += 1;
    va_end( arglist );

    buff = GUIAlloc( length );
    if( buff == NULL ) {
        return( FALSE );
    }

    va_start( arglist, control );
    vsprintf( buff, control, arglist );
    va_end( arglist );
    if( !ProcLine( buff, FINAL_SCAN ) ) {
        return( FALSE );
    }
    return( TRUE );
}

//AddButton
static bool AddButton( dialog_button button )
/*******************************************/
// Add a button to the current dialog.
{
    char        *text = NULL;

    if( button == DIALOG_OK_BUTTON ) {
        text = GUIAlloc( length( PUSHBUTTON ) +
               strlen( GetVariableStrVal( "IDS_OK_BUTTON" ) ) + 1 );
        if( text == NULL ) {
            return( FALSE );
        }
        sprintf( text, PUSHBUTTON "%s", GetVariableStrVal( "IDS_OK_BUTTON" ) );

    } else if( button == DIALOG_CANCEL_BUTTON ) {
        text = GUIAlloc( length( PUSHBUTTON ) +
               strlen( GetVariableStrVal( "IDS_CANCEL_BUTTON" ) ) + 1 );
        if( text == NULL ) {
            return( FALSE );
        }
        sprintf( text, PUSHBUTTON "%s", GetVariableStrVal( "IDS_CANCEL_BUTTON" ) );

    } else if( button == DIALOG_HELP_BUTTON ) {
        text = GUIAlloc( length( PUSHBUTTON ) +
               strlen( GetVariableStrVal( "IDS_HELP_BUTTON" ) ) + 1 );
        if( text == NULL ) {
            return( FALSE );
        }
        sprintf( text, PUSHBUTTON "%s", GetVariableStrVal( "IDS_HELP_BUTTON" ) );

    } else {
        return( FALSE );
    }

    if( !AddControl( text, NULL ) ) {
        return( FALSE );
    }
    GUIFree( text );
    return( TRUE );
}

//NIDCopyFile
static bool NIDCopyFile( char const *src, char const *dst )
/*********************************************************/
// Code copied out of DoCopyFiles in utils.c
{
    COPYFILE_ERROR      copy_error;
    gui_message_return  ret;
    int                 value;
    long                size;
    struct stat         src_buf;
    struct stat         dst_buf;
    bool                src_buf_ok;

    StatusLines( STAT_COPYINGFILE, (char *) dst );

    if( src == NULL || dst == NULL ) {
        return( FALSE );
    }

    if( stat( src, &src_buf ) == 0 ) {
        src_buf_ok = TRUE;
        size = RoundUp( src_buf.st_size, ClusterSize( (char *) dst ) );
    } else {
        src_buf_ok = FALSE;
        size = 0;
    };

    if( IsFile( dst ) ) {
        if( GetFileAttributes( dst ) & FILE_ATTRIBUTE_READONLY ) {
            if( !PromptUser( (char *) dst,
                             "ReadOnlyFile",
                             "RO_Skip_Dialog",
                             "RO_Replace_Old",
                             &value ) ) {
                return( FALSE );
            }
            if( value ) {
                SetFileAttributes( dst, FILE_ATTRIBUTE_NORMAL );
            } else {
                BumpStatus( size );
                return( TRUE );
            }
        }
        if( src_buf_ok
            && stat( dst, &dst_buf ) == 0
            && src_buf.st_mtime < dst_buf.st_mtime ) {
            if( !PromptUser( (char *) dst,
                            "NewerFile",
                            "Newer_Skip_Dialog",
                            "Newer_Replace_Old",
                            &value ) ) {
                return( FALSE );
            }
            if( !value ) {
                BumpStatus( size );
                return( TRUE );
            }
        }
    }

    if( remove( dst ) != 0 ) {
        MsgBox( NULL, "IDS_NID_ACCESSDENIED", GUI_OK, dst );
        return( FALSE );
    }

    do {
        copy_error = DoCopyFile( (char *) src, (char *) dst, FALSE );
        ret = GUI_RET_OK;
        switch( copy_error ) {
        case CFE_ABORT:
        case CFE_ERROR:
            ret = GUI_RET_CANCEL;
            break;
        case CFE_BAD_CRC:
            MsgBox( NULL, "IDS_BADCRC", GUI_OK, src );
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
            ret = MsgBox( NULL, "IDS_CANTOPENSRC", GUI_RETRY_CANCEL, src  );
            break;
        case CFE_CANTOPENDST:
            ret = MsgBox( NULL, "IDS_CANTOPENDST", GUI_RETRY_CANCEL, dst );
            break;
        }
        if( ret == GUI_RET_CANCEL ) return( FALSE );
    } while( copy_error != CFE_NOERROR );

    return( TRUE );
}


// Do self-registration for Dynamo
//     - nidynamo.exe, nidebug.dll and niscript.dll

static void self_register_dll( char *dllname )
{
    HMODULE             module;
    FARPROC             proc;

    if( access( dllname, F_OK ) == 0 ) {
        module = LoadLibrary( dllname );
        if( module != NULL ) {
            proc = GetProcAddress( module, "DllRegisterServer" );
            if( proc != NULL ) {
                (*proc)();
            }
        }
    }
}


static void self_register_exe( char *exename )
{
    DWORD               exit_code;
    char                cmd[ _MAX_PATH ];

    if( access( exename, F_OK ) == 0 ) {
        cmd[0] = '-';   // for quiet
        strcpy( &cmd[ 1 ], exename );
        strcat( cmd, " /register" );
        NTSpawnWait( cmd, &exit_code, 0, 0, 0 );
    }
}


void SelfRegisterDynamo()
{
    char                filebuf[ _MAX_PATH ];

    strcpy( filebuf, GetVariableStrVal( SYBTOOLS ) );
    strcat( filebuf, "\\" );
    strcat( filebuf, WIN32_DIR );
    strcat( filebuf, "\\nidebug.dll" );
    self_register_dll( filebuf );

    strcpy( filebuf, GetVariableStrVal( SYBTOOLS ) );
    strcat( filebuf, "\\" );
    strcat( filebuf, WIN32_DIR );
    strcat( filebuf, "\\niscript.dll" );
    self_register_dll( filebuf );

    strcpy( filebuf, GetVariableStrVal( SYBTOOLS ) );
    strcat( filebuf, "\\" );
    strcat( filebuf, WIN32_DIR );
    strcat( filebuf, "\\nidynamo.exe" );
    self_register_exe( filebuf );
}

// CreateSystemDSN - don't need it now, but if you ever do, here is a start...
/*
// Used in CreateSystemDSN
#define  ODBCDRIVER  "Sybase SQL Anywhere 5.0"
#define  ODBCDRIVERS "SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Drivers"
#define  ODBCINI     "SOFTWARE\\ODBC\\ODBC.INI"
#define  ODBCSOURCES ODBCINI "\\ODBC Data Sources"
#define  INSTALLED   "Installed"
*/
//CreateSystemDSN
//static bool CreateSystemDSN( char const *DSN_name )
/***************************************/
// Create a System DSN for Dynamo to use [unfinished]
/*{
    HKEY        hkey;
    int         rc;
    DWORD       size;
    char        *reg_string = NULL;

    if( DSN_name == NULL ) {
        return( FALSE );
    }

    rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, ODBCDRIVERS, NULL, KEY_READ, &hkey );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REG_ODBC_ERROR", GUI_OK );
        return( FALSE );
    }

    rc = RegQueryValueEx( hkey, ODBCDRIVER, NULL, NULL, NULL, &size );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REG_ODBC_ERROR", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }

    reg_string = GUIAlloc( size );
    if( reg_string == NULL ) {
        MsgBox( NULL, "IDS_CANTALLOCATEMEMORY", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }

    rc = RegQueryValueEx( hkey, ODBCDRIVER, NULL, NULL, reg_string, &size );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REG_ODBC_ERROR", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }

    // Make sure SQL Anywhere ODBC Driver is present in reg.
    if( strcmp( reg_string, INSTALLED ) != 0 ) {
        MsgBox( NULL, "IDS_REG_ODBC_ERROR", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }

    RegCloseKey( hkey );
    GUIFree( reg_string );

    rc = RegCreateKeyEx( HKEY_LOCAL_MACHINE, ODBCSOURCES, 0, NULL, REG_OPTION_NON_VOLATILE,
                         KEY_READ, NULL, &hkey, NULL );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REG_ODBC_ERROR", GUI_OK );
        RegCloseKey( hkey );
        return( FALSE );
    }

    // Set string DSN_name = ODBCDRIVER in ...\ODBC.INI\ODBC Data Sources
    rc = RegSetValueEx( hkey, DSN_name, NULL, REG_SZ, ODBCDRIVER, strlen( new_value ) + 1 );
    if( rc != 0 ) {
        MsgBox( NULL, "IDS_REGERROR", GUI_OK );
    }

    return( TRUE );
}
*/
//=============================================================================
//===================================================================== The End
//=============================================================================
#endif
