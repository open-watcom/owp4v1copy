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
* Description:  Project type functions.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "gui.h"
#include "projtype.h"
#include "rcstr.gh"

#define PROJTYPE_CONFIG_FILE    "projtype.cfg"

bool ReadProjectTypes()
/*********************/
{
    FILE    *fh;
    char    fmt[128];
    char    msg[128];
    char    filepath[PATH_MAX];

    _searchenv( PROJTYPE_CONFIG_FILE, "PATH", filepath );
    if( filepath[0] == '\0' ) {
        GUILoadString( APPWIZ_CFG_MISSING, fmt, 128 );
        sprintf( msg, fmt, PROJTYPE_CONFIG_FILE );
        GUIDisplayMessage( NULL, msg, "", GUI_OK );
        return( FALSE );
    }
    fh = fopen( filepath, "r" );
    fclose( fh );

    return( TRUE );
}

