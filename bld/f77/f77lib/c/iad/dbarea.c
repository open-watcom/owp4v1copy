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
// DBAREA       : data area processing for debugger
//

#include <dos.h>
#include <stddef.h>
#include <ctype.h>

#include "ftnstd.h"
#include "trcback.h"
#include "buginfo.h"
#include "bugconst.h"
#include "rundat.h"
#include "pgmacc.h"
#include "rcb.h"

extern  void            PGM *DbInAddr;

const byte __FAR ItemSize[] = {
        0,                      // alignment byte or end of list
        sizeof( logstar1 ),
        sizeof( logstar4 ),
        sizeof( intstar1 ),
        sizeof( intstar2 ),
        sizeof( intstar4 ),
        sizeof( single ),
        sizeof( double ),
        sizeof( complex ),
        sizeof( dcomplex ),
        sizeof( string ),
        sizeof( rcb ),
        0,                      // entry name
        0,                      // statement function
        sizeof( void PGM * )    // special class (all currently this size)
    };


bool     DBPrintable( db_item PGM *item ) {
//=========================================

// Determine whether the specified item can be printed.

    byte        goods;
    byte        code;
    int         typ;

    goods = item->code;
    code = goods & DB_CODE_MASK;
    // check if address or statement function
    if( code > DB_ENTRY_NAME ) return( FALSE );
    if( code == DB_ENTRY_NAME ) { // if its an entry name
        typ = goods & DB_ENTRY_TYP_MASK;
        // check if its an entry name of a subroutine or program
        if( typ == PT_NOTYPE ) return( FALSE );
        // the type must be the same as the type of the entry name
        code = ((db_item PGM *)(ExCurr->name -
                               offsetof( db_item, dbi.n.name )))->code;
        if( typ != code ) return( FALSE );
    } else { // it's a variable
        // check to ensure dummy argument is in dummy argument list of
        // entry name
        if( goods & DB_VAR_INDIRECT ) {
            if( DbInAddr == NULL ) return( FALSE );
        } else if( goods & DB_VAR_SUBSCR ) {
            if( ((lg_adv PGM *)DbInAddr)->origin == NULL ) return(FALSE);
        } else if( code == DB_CHAR ) {
            if( ((string PGM *)DbInAddr)->strptr == NULL ) return( FALSE );
        }
    }
    return( TRUE );
}


db_item PGM *DBDecodeItem( db_item PGM *item ) {
//==============================================

// Point to the next item in the data area.

    byte        PGM *value_ptr;
    int         item_size;
    int         dims;
    int         namelen;
    byte        goods;
    byte        code;

    goods = item->code;
    code = goods & DB_CODE_MASK;
    item_size = ItemSize[ goods >> DB_CODE_SHIFT ];
    namelen = 0;
    if( code != DB_SPECIAL || ( goods & DB_SPECIAL_CLASS ) != DB_ADDR ) {
        // if it's a named item, include length byte
        namelen = sizeof( byte ) + item->dbi.n.len;
    }
    value_ptr = (char PGM *)item + sizeof( byte ) + namelen;
    DbInAddr = value_ptr;
    if( code <= DB_STRUCT ) { // if it's a variable
        if( goods & DB_VAR_INDIRECT ) {
            item_size = sizeof( void PGM * );
            DbInAddr = *(void PGM * PGM *)DbInAddr;
        } else {
            dims = goods & DB_VAR_SUBSCR;
            if( dims != 0 ) { // if its subscripted
                item_size = ADV_BASE_SIZE + dims * ADV_DIM_SIZE;
                if( code == DB_STRUCT ) {
                    item_size += sizeof( obj_ptr );
                }
            }
        }
    } else if( code == DB_STMT_FUNC ) {
        item_size = ItemSize[ goods & DB_ENTRY_TYP_MASK ];
    } else if( ( code == DB_ENTRY_NAME ) && ( goods != DB_ENTRY_NAME ) ) {
        obj_ptr tmp;

#if _TARGET == _80386
        tmp = CODE_REL( ((db_traceback PGM *)ExCurr)->epilog_seq );
#else
        tmp = ((db_traceback PGM *)ExCurr)->epilog_seq;
#endif
        DbInAddr =&((epilog_sequence PGM *)MK_FP(FP_SEG(ExCurr),tmp))->func_val;
    }
    return( (db_item PGM *)(value_ptr + item_size) );
}


db_item         PGM *FindVar( char *name, int namelen ) {
//=======================================================

// Find the specified variable in the local data area.

    int         len;
    db_item     PGM *item;
    db_item     PGM *next_item;
    char        PGM *ptr;
    byte        goods;

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
        if( goods == ( DB_SPECIAL | DB_END_LIST ) ) return( NULL );
        next_item = DBDecodeItem( item );
        if( DBPrintable( item ) ) {
            len = item->dbi.n.len;
            ptr = &item->dbi.n.name;
            if( ( goods & DB_CODE_MASK ) == DB_ENTRY_NAME ) {
                // if function entry name, length includes file name so get
                // length of function name only
                len = pgm_strlen( ptr );
            }
            if( namelen == len ) {
                for(;;) {
                    --len;
                    if( toupper( name[ len ] ) != ptr[ len ] ) break;
                    if( len == 0 ) return( item );
                }
            }
        }
    }
}
