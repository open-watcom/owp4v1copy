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
// IFLOOKUP  : look up an intrinsic function
//

#include "ftnstd.h"
#include "ifflags.h"
#include "ifnames.h"
#include "errcod.h"
#include "global.h"

#include <string.h>

extern  int             KwLookUp(char **,int,char *,int,bool);
extern  void            TypeErr(int,uint);
extern  void            MarkIFUsed(int);
extern  int             TypeSize(uint);
extern  sym_id          STSearch(char *,int,sym_id);
extern  sym_id          STNameSearch(char *,int);
extern  sym_id          STAdd(char *,int);
extern  bool            IFGenInLine(uint);
extern  byte            IFArgType(byte);

extern  char            *IFNames[];
extern  const unsigned_16 __FAR IFFlags[];
extern  const byte __FAR IFSelect[];


int     IFIndex( char *name ) {
//=============================

    return( KwLookUp( IFNames, IF_MAX_NAME, name, strlen( name ), TRUE ) - 1 );
}


int     IFLookUp( ) {
//===================

    return( KwLookUp( IFNames, IF_MAX_NAME, CITNode->opnd, CITNode->opnd_size,
                      TRUE ) - 1 );
}


static  sym_id  IFSymLookup( char *name, uint len ) {
//===================================================

    sym_id      sym;

    sym = STNameSearch( name, len );
    if( sym != NULL ) {
        if( ((sym->ns.flags & SY_CLASS) == SY_SUBPROGRAM) &&
               (sym->ns.flags & SY_INTRINSIC) ) {
            return( sym );
        }
    }
    sym = STSearch( name, len, IFList );
    if( sym == NULL ) {
        sym = STAdd( name, len );
        sym->ns.address = NULL;
        sym->ns.link = IFList;
        IFList = sym;
    }
    return( sym );
}


bool    IFIsGeneric( byte func ) {
//================================

    return( ( IFFlags[ func ] & IF_GENERIC ) != 0 );
}


bool    IFIsMagic( byte func ) {
//==============================

    return( IFSelect[ func ] == MAGIC );
}


byte    IFType( byte func ) {
//===========================

    return( ( IFFlags[ func ] & IF_TYPE ) >> IF_SHIFT_TYPE );
}


byte    IFSpecific( byte typ ) {
//==============================

    byte        func;
    byte        magic;
    sym_id      sym;

    magic = 0;
    func = CITNode->sym_ptr->ns.si.fi.index;
    if( IFIsMagic( func ) ) {
        magic = MAGIC;
    } else if( IFIsGeneric( func ) ) {
        for(;;) {
            if( typ == IFArgType( func ) ) break;
            func = IFSelect[ func ];
            if( func == IF_NO_MORE ) break;
        }
        if( func == IF_NO_MORE ) {
            TypeErr( LI_NO_SPECIFIC, typ );
            return( magic );
        } else {
            sym = IFSymLookup( IFNames[ func ], strlen( IFNames[ func ] ) );
            typ = IFType( func );
            // merge flags - don't assign them from CITNode->sym_ptr->ns.flags
            // since SY_IF_ARGUMENT may be set in sym->flags
            // Consider:        DOUBLE PRECISION X
            //                  INTRINSIC DSIN
            //                  CALL F( DSIN )
            //                  PRINT *, SIN( X )
            // when we process SIN( X ), the specific function DSIN already
            // has SY_IF_ARGUMENT set
            sym->ns.flags |= CITNode->sym_ptr->ns.flags | SY_REFERENCED;
            sym->ns.typ = typ;
            sym->ns.xt.size = TypeSize( typ );
            sym->ns.si.fi.index = func;
            CITNode->sym_ptr = sym;
            if( IFGenInLine( func ) ) {
                magic = MAGIC;
            } else {
                MarkIFUsed( func );
            }
        }
    } else if( IFGenInLine( func ) ) {
        magic = MAGIC;
    } else {
        MarkIFUsed( func );
    }
    typ = IFType( func );
    CITNode->typ = typ;
    CITNode->size = TypeSize( typ );
    return( magic );
}


bool    IsIFMax( int ifn ) {
//==========================

// Any changes here should be made in InlineCnvt in upscan.c

    return( (ifn == IF_AMAX0) || (ifn == IF_AMAX1) || (ifn == IF_DMAX1) ||
            (ifn == IF_QMAX1) || (ifn == IF_I1MAX0) || (ifn == IF_I2MAX0) ||
            ( ifn == IF_MAX0 ) || ( ifn == IF_MAX1 ) );
}


bool    IsIFMin( int ifn ) {
//==========================

// Any changes here should be made in InlineCnvt in upscan.c

    return( (ifn == IF_AMIN0) || (ifn == IF_AMIN1) || (ifn == IF_DMIN1) ||
            (ifn == IF_QMIN1) || (ifn == IF_I1MIN0) || (ifn == IF_I2MIN0) ||
            (ifn == IF_MIN0) || (ifn == IF_MIN1) );
}


bool    IsIntrinsic( unsigned_16 flags ) {
//========================================

    return( ( ( flags & SY_CLASS ) == SY_SUBPROGRAM ) &&
            ( ( flags & SY_INTRINSIC ) != 0 ) );
}
