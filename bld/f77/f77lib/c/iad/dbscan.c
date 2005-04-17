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
// DBSCAN       : debugger scanning routines
//

#include <ctype.h>

#include "ftnstd.h"
#include "rundat.h"

extern  void            Blanks(void);
extern  signed_32       GetNum(void);


void            ParseBlanks() {
//=============================

    Blanks();
}


char            *ParsePtr() {
//===========================

// Point to where we are currently scanning.

    ftnfile     *fcb;

    fcb = IOCB->fileinfo;
    return( &fcb->buffer[ fcb->col ] );
}


bool            ParseChr( char chr ) {
//====================================

// Scan a character.

    if( tolower( *ParsePtr() ) == chr ) {
        IOCB->fileinfo->col++;
        return( TRUE );
    }
    return( FALSE );
}


bool            ParseSNum( signed_32 *num ) {
//===========================================

// Scan a signed number.

    char        *ptr;

    Blanks();
    ptr = ParsePtr();
    if( ( *ptr == '+' ) || ( *ptr == '-' ) ) {
        ++ptr;
    }
    if( isdigit( *ptr ) ) {
        *num = GetNum();
        ParseBlanks();
        return( TRUE );
    } else {
        ParseBlanks();
        return( FALSE );
    }
}


bool            ParseNum( unsigned_32 *num ) {
//============================================

    char        *ptr;

    Blanks();
    ptr = ParsePtr();
    if( isdigit( *ptr ) ) {
        *num = GetNum();
        ParseBlanks();
        return( TRUE );
    } else {
        ParseBlanks();
        return( FALSE );
    }
}


bool            ParseEOL() {
//==========================

// Check for end-of-line.

    ParseBlanks();
    return( *ParsePtr() == NULLCHAR );
}


int             ParseName() {
//===========================

// Scan a variable name.

    char        *name;
    char        *ptr;
    int         len;
    char        chr;

    name = ParsePtr();
    ptr = name;
    for(;;) {
        chr = *ptr;
        if( chr == '(' ) break;
        if( chr == ',' ) break;
        if( chr == ' ' ) break;
        if( chr == NULLCHAR ) break;
        ++ptr;
    }
    len = ptr - name;
    IOCB->fileinfo->col += len;
    ParseBlanks();
    return( len );
}
