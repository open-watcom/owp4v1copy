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
// DBPRINT      : display FORTRAN variables
//

#include <stdlib.h>

#include "ftnstd.h"
#include "buginfo.h"
#include "bugconst.h"
#include "rundat.h"
#include "xfflags.h"
#include "rcb.h"
#include "structio.h"
#include "trcback.h"
#include "symdefs.h"

extern  void            FOHex(int);
extern  void            FOString(int);
extern  int             GetLen(void);
extern  void            StructIOInit(sio_cntrl_blk PGM *,uint,rcb);
extern  int             StructIOItem(sio_cntrl_blk PGM *,bool,uint);
extern  void            Drop(int);
extern  void            SendEOR(void);
extern  void            SendStr(char PGM *,int);
extern  void            SendWSLStr(char *);

extern  void            PGM *DbInAddr;
extern  void            (* const __FAR OutRtn[])();


int             ItemType( db_item PGM *item ) {
//=============================================

// Return the type of item in data area.
// For items that are not printable (addresses/subroutines/program)
// return the code.

    int         code;

    code = item->code;
    if( ( ( code & DB_CODE_MASK ) == DB_ENTRY_NAME ) ||
        ( ( code & DB_CODE_MASK ) == DB_STMT_FUNC ) ) {
        return( code & DB_ENTRY_TYP_MASK );
    }
    return( code >> DB_CODE_SHIFT );
}


int             ItemSubscripts( db_item PGM *item ) {
//===================================================

// Return the number of subscripts of the specified item.

    int         code;

    code = item->code;
    if( ( code & DB_CODE_MASK ) > DB_STRUCT ) return( 0 );
    return( code & DB_VAR_SUBSCR );
}


static  void    GetValue( int typ ) {
//===================================

    switch( typ ) {
    case PT_LOG_1 :
        IORslt.logstar4 = *(logstar1 PGM *)DbInAddr;
        break;
    case PT_LOG_4 :
        IORslt.logstar4 = *(logstar4 PGM *)DbInAddr;
        break;
    case PT_INT_1 :
        IORslt.intstar4 = *(logstar1 PGM *)DbInAddr;
        break;
    case PT_INT_2 :
        IORslt.intstar4 = *(intstar2 PGM *)DbInAddr;
        break;
    case PT_INT_4 :
        IORslt.intstar4 = *(intstar4 PGM *)DbInAddr;
        break;
    case PT_REAL_4 :
        IORslt.single = *(single PGM *)DbInAddr;
        break;
    case PT_REAL_8 :
        IORslt.dble = *(double PGM *)DbInAddr;
        break;
    case PT_CPLX_8 :
        IORslt.complex = *(complex PGM *)DbInAddr;
        break;
    case PT_CPLX_16 :
        IORslt.dcomplex = *(dcomplex PGM *)DbInAddr;
        break;
    case PT_CHAR :
        IORslt.string = *(string PGM *)DbInAddr;
        break;
    }
}


static  void    doPrintSimpValue( int typ, int fmt ) {
//====================================================

// Print a scalar.

    int         size;

    IOCB->typ = typ;
    if( fmt == DEFAULT_FMT ) {
        OutRtn[ typ ]();
    } else {
        size = GetLen();
        if( ( typ == PT_CPLX_8 ) || ( typ == PT_CPLX_16 ) ) {
            size *= 2;
        }
        if( fmt == Z_FMT ) {
            FOHex( 2 * size + 1 );
        } else {
            FOString( 0 );
        }
    }
}


#if _TARGET == _8086
#define EXTRA   FP_SEG( ExCurr )
#else
#define EXTRA   extra
#endif

static  void    PrintStruct( int fmt, rcb PGM *r ) {
//==================================================

    sio_cntrl_blk       cb;
    int                 typ;
#if _TARGET == _80386
    uint                extra = ( (epilog_sequence PGM *)CODE_REL(
                                ((db_traceback PGM *)ExCurr)->epilog_seq ) )
                                ->my_data_base;
#endif

    StructIOInit( &cb, EXTRA, *r );
    typ = StructIOItem( &cb, FALSE, EXTRA );
    if( typ != PT_NOTYPE ) {
        for(;;) {
            doPrintSimpValue( typ, fmt );
            typ = StructIOItem( &cb, FALSE, EXTRA );
            if( typ == PT_NOTYPE ) break;
            Drop( ',' );
        }
    }
}


void            PrintSimpValue( int typ, int fmt ) {
//==================================================

    if( typ == PT_STRUCT ) {
        PrintStruct( fmt, (rcb PGM *)DbInAddr );
    } else {
        GetValue( typ );
        doPrintSimpValue( typ, fmt );
    }
    SendEOR();
}


void            PrintArray( int typ, int fmt, db_item PGM *item ) {
//=================================================================

// Print an array.

    signed_32   ss[ MAX_DIM ];
    int         ss_index;
    int         elt_size;
    unsigned_32 num_elts;
    lg_adv PGM  *adv;
    int         num_ss;
    union {
        char HPGM *p;
        string  s;
        rcb     r;
    } data;

    adv = (lg_adv PGM *)DbInAddr;
    num_elts = adv->num_elts;
    elt_size = adv->elt_size;
    num_ss = ( adv->num_ss & ADV_SUBSCRS );
    switch( typ ) {
    case PT_CHAR:
        data.s.len = adv->elt_size;
        data.s.strptr = (char PGM *)adv->origin;
        DbInAddr = &data;
        break;
    case PT_STRUCT:
        data.r.defn = *(obj_ptr PGM *)((char PGM *)DbInAddr +
                                    num_ss * ADV_DIM_SIZE + ADV_BASE_SIZE );
        data.r.origin = (char PGM *)adv->origin;
        DbInAddr = &data;
        break;
    default:
        DbInAddr = adv->origin;
        break;
    }
    for( ss_index = 0; ss_index < num_ss; ++ss_index ) {
        ss[ ss_index ] = adv->subscrs[ ss_index ].lo_bound;
    }
    for(;;) {
        SendStr( &item->dbi.n.name, item->dbi.n.len );
        Drop( '(' );
        ss_index = num_ss - 1;
        for(;;) {
            ltoa( ss[ ss_index ], IOCB->buffer, 10 );
            SendWSLStr( IOCB->buffer );
            if( ss_index == 0 ) break;
            --ss_index;
            Drop( ',' );
        }
        SendStr( ") = ", 4 );
        PrintSimpValue( typ, fmt );
        --num_elts;
        if( num_elts == 0 ) break;
        if( __XcptFlags & XF_LIMIT_ERR ) break;
        // note the need to do huge arithmetic here to handle arrays
        // greater than 64k on the 286
        switch( typ ) {
        case PT_STRUCT:
        case PT_CHAR:
            data.p += elt_size;
            break;
        default:
            DbInAddr = (char HPGM *)DbInAddr + elt_size;
        }
        ss_index = num_ss - 1;
        for(;;) {
            ++ss[ ss_index ];
            if( ss[ ss_index ] - adv->subscrs[ ss_index ].lo_bound <
                        adv->subscrs[ ss_index ].num_elts ) break;
            ss[ ss_index ] = adv->subscrs[ ss_index ].lo_bound;
            --ss_index;
        }
    }
}
