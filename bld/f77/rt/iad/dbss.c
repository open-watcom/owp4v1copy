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
// DBSS         : debugger subscripting and substringing support
//

#include "ftnstd.h"
#include "symbol.h"
#include "errcod.h"

extern  int             Spawn(void (*)());
extern  void            DbMsg(uint);
extern  void            DbSubScr(int,signed_32 *);
extern  void            DbSubStr(int,int,string *);
extern  bool            ParseSNum(signed_32 *);
extern  bool            ParseChr(char);

extern  void            PGM *DbInAddr;

static  signed_32       *DbSS;
static  uint            DbNumSS;
static  long int        DbSS1;
static  long int        DbSS2;
static  string          *DbSCB;


static  void    DoSubscript() {
//=============================

    DbSubScr( DbNumSS, DbSS );
}


bool            SubScr( int num_ss ) {
//====================================

// Get a subscript list.

    signed_32   ss[MAX_DIM];
    int         idx;

    idx = 0;
    for(;;) {
        if( !ParseSNum( &ss[ idx ] ) ) {
            DbMsg( MS_DB_SSCR_BAD );
            return( FALSE );
        }
        ++idx;
        if( idx == num_ss ) break;
        if( !ParseChr( ',' ) ) break;
    }
    if( !ParseChr( ')' ) ) {
        DbMsg( MS_DB_SSCR_BAD );
        return( FALSE );
    } else if( idx != num_ss ) {
        DbMsg( MS_DB_SSCR_BAD );
        return( FALSE );
    }
    DbNumSS = num_ss;
    DbSS = &ss;
    if( Spawn( &DoSubscript ) != 0 ) return( FALSE );
    return( TRUE );
}


static  void    DoSubstring() {
//=============================

    DbSubStr( (int)DbSS1, (int)DbSS2, DbSCB );
}


bool            SubStr( string *scb ) {
//=====================================

// Get a substring list.

    DbSS1 = 1;
    DbSS2 = ((string PGM *)DbInAddr)->len;
    DbSCB = scb;
    if( !ParseChr( ':' ) ) {
        if( !ParseSNum( &DbSS1 ) ) {
            DbMsg( MS_DB_SSTR_BAD );
            return( FALSE );
        }
        if( !ParseChr( ':' ) ) {
            DbMsg( MS_DB_SSTR_BAD );
            return( FALSE );
        }
    }
    if( !ParseChr( ')' ) ) {
        if( !ParseSNum( &DbSS2 ) ) {
            DbMsg( MS_DB_SSTR_BAD );
            return( FALSE );
        }
        if( !ParseChr( ')' ) ) {
            DbMsg( MS_DB_SSTR_BAD );
            return( FALSE );
        }
    }
    if( Spawn( &DoSubstring ) != 0 ) return( FALSE );
    return( TRUE );
}
