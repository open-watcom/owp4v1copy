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


//
// GEDITINI     : editor interface initialization
//

#include "ftnstd.h"
#include "edit.h"

#include <process.h>
#include <dos.h>

static const char __FAR _EditMessage[] = { "WATCOM Systems Inc." };
static char             *EdtName = { "weditf.exe" };
static void (__interrupt __far *OldHndlr)();

int     InitEditor() {
//====================

// Initialize editor.

    // Before spawning the editor, set the interrupt vector to point at the
    // copyright string so that the editor knows that it is being spawned.

    OldHndlr = _dos_getvect( EDIT_INTERRUPT );
    _dos_setvect( EDIT_INTERRUPT, (void __FAR *)_EditMessage );
    if( spawnlp( P_WAIT, EdtName, NULL ) == 0x0300 ) {
        return( 0x0300 );
    }

    // could not invoke editor
    _dos_setvect( EDIT_INTERRUPT, OldHndlr );

    return( 0 );
}


void    FiniEditor() {
//====================

// Terminate editor.

    EditInt1( ED_FINIEDITOR );
    _dos_setvect( EDIT_INTERRUPT, OldHndlr ); // restore old interrupt handler
}
