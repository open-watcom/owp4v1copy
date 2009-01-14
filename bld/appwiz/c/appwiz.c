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
* Description:  Application Wizard main source file.
*
****************************************************************************/


#include "gui.h"
#include "newproj.h"
#include "projtype.h"

bool NewProjCallback( gui_window *wnd, gui_event ev, void *extra )
/****************************************************************/
{
    project_type_iterator   iter;
    char                    friendlyname[128];
    
    switch( ev ) {
    case GUI_INIT_DIALOG:
        iter = GetFirstProjectType();
        while( iter != NULL ) {
            GetNextProjectType( &iter, NULL, friendlyname );
            GUIAddText( wnd, IDC_PROJTYPE, friendlyname );
        }
        break;
    }
    return( TRUE );
}

extern void GUImain( void )
/*************************/
{
    gui_create_info newProjInfo = {
        NULL,
        { 0, 0, 0, 0 },
        GUI_NOSCROLL,
        GUI_VISIBLE | GUI_CLOSEABLE,
        NULL,
        0,
        NULL,
        0,
        NULL,
        &NewProjCallback,
        NULL,
        NULL,
        0
    };
    gui_rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = 1000;
    rect.height = 1000;
    GUISetScale( &rect );
    GUIWndInit( 300, GUI_GMOUSE );

    if( !ReadProjectTypes() ) {
        FreeProjectTypes();
        return;
    }
    GUICreateResDialog( &newProjInfo, NEWPROJDLG );
    FreeProjectTypes();
}

