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
// RUNDEFO   : undefined variable checking in output statements
//

#include "ftnstd.h"
#include "rundat.h"
#include "parmtype.h"
#include "undef.h"
#include "csetinfo.h"

#include <string.h>

extern  uint            GetLen(void);
extern  void            SendStr(char PGM *,uint);
extern  void            SendChar(char,uint);
extern  void            Drop(char);

extern  const byte      __FAR SizeVars[];
extern  character_set   CharSetInfo;


bool    UndefData( char PGM *value, int size ) {
//==============================================

    while( size != 0 ) {
        if( *value != UNDEF_CHAR ) return( FALSE );
        ++value;
        --size;
    }
    return( TRUE );
}


static  bool    ChkUndef( void *value, int size ) {
//=================================================

    if( ( IOCB->flags & UNDCHK_IO ) == 0 ) return( FALSE );
    return( UndefData( value, size ) );
}


bool    UndefInt() {
//==================

    return( ChkUndef( &IORslt, SizeVars[ IOCB->typ ] ) );
}


bool    UndefLog() {
//==================

    return( ChkUndef( &IORslt, SizeVars[ IOCB->typ ] ) );
}


void    UFill( char *buf, uint len ) {
//====================================

    memset( buf, UNDEF_OUTPUT_CHR, len );
}


bool    UndefReal( single *value ) {
//==================================

    return( ChkUndef( value, SizeVars[ PT_REAL_4 ] ) );
}


bool    UndefDouble( double *value ) {
//====================================

    return( ChkUndef( value, SizeVars[ PT_REAL_8 ] ) );
}


bool    UndefExtended( extended *value ) {
//========================================

    return( ChkUndef( value, SizeVars[ PT_REAL_16 ] ) );
}

void    FmtUFloat( char *buf, uint len ) {
//========================================

    buf[ 0 ] = ' ';
    buf[ len+1 ] = NULLCHAR;
    UFill( &buf[ 1 ], len );
}


void    USendStr( char PGM *ptr, uint width ) {
//=============================================

    for(;;) {
        if( CharSetInfo.character_width( ptr ) > 1 ) {
            Drop( *ptr );
            --width;
            // make sure first byte of double-byte character
            // is not last character of string
            if( width == 0 ) break;
            ++ptr;
            Drop( *ptr );
        } else {
            if( ( IOCB->flags & UNDCHK_IO ) && ( *ptr == UNDEF_CHAR ) ) {
                Drop( UNDEF_OUTPUT_CHR );
            } else {
                Drop( *ptr );
            }
        }
        --width;
        if( width == 0 ) break;
        ++ptr;
    }
}


void    SendUChr() {
//==================

    uint        chars;

    chars = 2 * GetLen();
    SendChar( ' ', INTEGER_IO_WINDOW - chars );
    SendNUChr( chars );
}


void    SendNUChr( uint n ) {
//===========================

    SendChar( UNDEF_OUTPUT_CHR, n );
}
