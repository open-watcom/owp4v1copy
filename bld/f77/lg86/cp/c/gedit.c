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
// GEDIT        : editor interface
//

#include "ftnstd.h"
#include "edit.h"

int     DoEdit( char *str, int line_num ) {
//=========================================

// Invoke editor.

    return( EditInt3( ED_EDITFILE, (unsigned_16)str, line_num ) );
}


int     ResumeEditor( char *str, int line_num ) {
//===============================================

// Resume editor.

    return( EditInt3( ED_RESUMEEDITOR, (unsigned_16)str, line_num ) );
}


int     EOpenf( char *buffname ) {
//================================

// Open editor buffer.

    return( EditInt2( ED_OPENF, (unsigned_16)buffname ) );
}


int     EGetRec( unsigned_16 handle, char *buff, int len ) {
//==========================================================

// Get record from editor buffer.

    return( EditInt4( ED_GETREC, handle, (unsigned_16)buff, len ) );
}


void    EClosef( unsigned_16 handle ) {
//=====================================

// Close editor buffer.

    EditInt2( ED_CLOSEF, handle );
}


int     EEof( unsigned_16 handle ) {
//==================================

// Check for end of buffer.

    return( EditInt2( ED_EOF, handle ) );
}
