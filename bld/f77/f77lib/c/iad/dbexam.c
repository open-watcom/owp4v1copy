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
// DBEXAM       : debugger "examine" command
//

#include "ftnstd.h"
#include "buginfo.h"
#include "bugconst.h"
#include "pgmacc.h"
#include "trcback.h"
#include "rundat.h"
#include "xfflags.h"
#include "rcb.h"
#include "lgadv.h"
#include "errcod.h"

#include <stdlib.h>

extern  void            ParseBlanks(void);
extern  bool            ParseChr(char);
extern  bool            ParseName(void);
extern  bool            ParseEOL(void);
extern  char            *ParsePtr(void);
extern  db_item         PGM *FindVar(char *,int);
extern  int             ItemType(db_item PGM *);
extern  int             ItemSubscripts(db_item PGM *);
extern  void            DbMsg(uint);
extern  bool            SubScr(int);
extern  bool            SubStr(string *);
extern  void            PrintArray(int,int,db_item PGM *);
extern  void            PrintSimpValue(int,int);
extern  void            StartInput(void);
extern  void            StartOutput(void);
extern  void            SendWSLStr(char *);
extern  void            SendStr(char PGM *,int);
extern  void            SendEOR(void);
extern  db_item PGM *   DBDecodeItem(db_item PGM *);
extern  bool            DBPrintable(db_item PGM *);
extern  void            Drop(int);

extern  void            PGM *DbInAddr;


static  int     ShowOne( db_item PGM *item, int fmt ) {
//=====================================================

    union {
        string  scb;
        rcb     r;
    } data;
    int         typ;
    int         num_ss;

    typ = ItemType( item );
    num_ss = ItemSubscripts( item );
    if( num_ss != 0 ) {
        if( ((lg_adv PGM *)DbInAddr )->num_ss & ADV_NOT_INIT ){
            DbMsg( MS_DB_ARR_NOT_DIMMED );
            return( 0 );
        }
        if( ParseChr( '(' ) ) {
            switch( typ ) {
            case PT_CHAR:
                data.scb.len = ((lg_adv PGM *)DbInAddr)->elt_size;
                break;
            case PT_STRUCT:
                data.r.defn = *(obj_ptr PGM *)((char PGM *)DbInAddr +
                                    num_ss * ADV_DIM_SIZE + ADV_BASE_SIZE );
                break;
            }
            if( !SubScr( num_ss ) ) {
                return( 0 );
            }
            switch( typ ) {
            case PT_CHAR:
                data.scb.strptr = DbInAddr;
                DbInAddr = &data.scb;
                break;
            case PT_STRUCT:
                data.r.origin = DbInAddr;
                DbInAddr = &data.r;
                break;
            }
            num_ss = 0;
        }
    }
    if( ( typ == PT_CHAR ) && ParseChr( '(' ) ) {
        if( !SubStr( &data.scb ) ) {
            return( 0 );
        }
        DbInAddr = &data.scb;
    }
    StartOutput();
    if( num_ss != 0 ) {
        PrintArray( typ, fmt, item );
        __XcptFlags &= ~XF_LIMIT_ERR;
    } else {
        PrintSimpValue( typ, fmt );
    }
    return( 1 );
}


static  void    ShowArray( int num_ss, db_item PGM *item ) {
//==========================================================

    int         ss_index;
    intstar4    lo_bound;
    lg_adv PGM  *adv;

    adv = (lg_adv PGM *)DbInAddr;
    SendStr( &item->dbi.n.name, item->dbi.n.len );
    Drop( '(' );
    ss_index = num_ss - 1;
    for(;;) {
        lo_bound = adv->subscrs[ ss_index ].lo_bound;
        if( lo_bound != 1 ) {
            ltoa( lo_bound, IOCB->buffer, 10 );
            SendWSLStr( IOCB->buffer );
            Drop( ':' );
        }
        ltoa( adv->subscrs[ ss_index ].num_elts + lo_bound - 1,
                                IOCB->buffer, 10 );
        SendWSLStr( IOCB->buffer );
        if( ss_index == 0 ) break;
        --ss_index;
        Drop( ',' );
    }
    Drop( ')' );
    SendEOR();
}


static  int     ShowAll( int fmt ) {
//==================================

// print all the variables in the local data area

    int         len;
    db_item     PGM *item;
    db_item     PGM *next_item;
    char        PGM *ptr;
    byte        goods;
    int         num_ss;

    StartOutput();
#if _TARGET == _8086
    next_item = MK_FP( FP_SEG( ExCurr ),
                      ((db_traceback PGM *)ExCurr)->data_area );
#else
    next_item = (db_item PGM *)((db_traceback PGM *)ExCurr)->data_area;
#endif
    for(;;) {
        item = next_item;
        for(;;) {
            goods = item->code;
            if( goods != DB_FILLER ) break;
            item = (db_item PGM *)((byte PGM *)item + sizeof( byte ));
        }
        if( goods == (DB_SPECIAL|DB_END_LIST) ) return( 0 );
        next_item = DBDecodeItem( item );
        if( DBPrintable( item ) ) {
            num_ss = ItemSubscripts( item );
            if( num_ss ) {
                ShowArray( num_ss, item );
            } else {
                len = item->dbi.n.len;
                ptr = &item->dbi.n.name;
                if( ( goods & DB_CODE_MASK ) == DB_ENTRY_NAME ) {
                    // if function entry name, length includes file name so get
                    // length of function name only
                    len = pgm_strlen( ptr );
                }
                SendStr( ptr, len );
                SendWSLStr( " = " );
                PrintSimpValue( ItemType( item ), fmt );
            }
        }
        if( __XcptFlags & XF_LIMIT_ERR ) break;
    }
    __XcptFlags &= ~XF_LIMIT_ERR;
    return( 1 );
}


void            DbExamine() {
//===========================

    db_item     PGM *item;
    int         fmt;
    int         len;
    char        *name;

    ParseBlanks();
    fmt = DEFAULT_FMT;
    if( ParseEOL() ) { ShowAll( fmt ); return; }
    if( ParseChr( '(' ) ) {
        ParseBlanks();
        if( ParseChr( 'z' ) ) {
            fmt = Z_FMT;
        } else if( ParseChr( 'a' ) ) {
            fmt = A_FMT;
        }
        if( !ParseChr( ')' ) ) {
            DbMsg( MS_DB_SYNTAX_ERR );
        }
    }
    if( ParseEOL() ) { ShowAll( fmt ); return; }
    for(;;) {
        ParseBlanks();
        name = ParsePtr();
        len = ParseName();
        if( len == 0 ) {
            DbMsg( MS_DB_SYNTAX_ERR );
        }
        item = FindVar( name, len );
        if( item == NULL ) {
            DbMsg( MS_DB_VAR_NOT_FOUND );
        }
        ShowOne( item, fmt ); 
        if( __XcptFlags & XF_LIMIT_ERR ) {
            __XcptFlags &= ~XF_LIMIT_ERR;
        }
        StartInput();
        ParseBlanks();
        if( !ParseChr( ',' ) ) break;
    }
    if( !ParseEOL() ) {
        DbMsg( MS_DB_SYNTAX_ERR );
    }
}
