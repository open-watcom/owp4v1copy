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


#include <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "gui.h"
#include "setup.h"
#include "setupinf.h"
#include "genvbl.h"


#ifndef WSQL

    bool CreateServices()
    {
        return( TRUE );
    }

#else

    /*
     * installService - Installs the service because it doesn't exist yet.
     */
    static bool DoCreateService( char *sername, char *dispname, char *path )
    {
        SC_HANDLE               hservice;
        SC_HANDLE               hscman;
        DWORD           err;
        char            *errstr;
        bool            retcode;

        hscman = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
        if( !hscman ) {
            MsgBox( NULL, "IDS_ERROR", GUI_OK, "cannot open service manager -- log on the administrator account and try again" );
            return( FALSE );
        }

        // Now we attempt to create the service
        hservice = CreateService(
            hscman,                             /* SCManager database     */
            TEXT(sername),                      /* name of service        */
            TEXT(dispname),                     /* name displayed in SCM  */
            SERVICE_ALL_ACCESS,                 /* desired access         */
            SERVICE_WIN32_OWN_PROCESS | 0x100,  /* service type 0x100 (uses Desktop) */
            SERVICE_AUTO_START,                 /* start type             */
            SERVICE_ERROR_NORMAL,               /* error control type     */
            path,                               /* service's binary       */
            NULL,                               /* no load ordering group */
            NULL,                               /* no tag identifier      */
            NULL,                               /* no dependencies        */
            NULL,                               /* LocalSystem account    */
            NULL);                              /* no password            */

        retcode = (hservice != NULL);
        if( hservice == NULL ) {
            err = GetLastError();
            switch( err ) {
            case ERROR_SERVICE_EXISTS:
                errstr = NULL;
                retcode = TRUE;
                break;
            case ERROR_ACCESS_DENIED:
            case ERROR_CIRCULAR_DEPENDENCY:
            case ERROR_DUP_NAME:
            case ERROR_INVALID_HANDLE:
            case ERROR_INVALID_NAME:
            case ERROR_INVALID_PARAMETER:
            case ERROR_INVALID_SERVICE_ACCOUNT:
            default:
                errstr = "service not created -- logon to the administrator account and try again";
                break;
            }
            if( errstr != NULL ) {
                MsgBox( NULL, "IDS_ERROR", GUI_OK, errstr );
            }
        }
        if( hservice != NULL ) {
            CloseServiceHandle( hservice );
        }
        CloseServiceHandle( hscman );

        return( retcode );
    }

    bool CreateServices()
    {
        char            ntexe[_MAX_PATH];

        ReplaceVars( ntexe, GetVariableStrVal( "dbsrvt" ) );
        return( DoCreateService( "WSQL_Sample", "Watcom SQL - Sample", ntexe ) );
    }

#endif
