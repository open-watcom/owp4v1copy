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
// TDDUMP       : Target dependent symbol table processing
//

#include <stddef.h>

#include "ftnstd.h"
#include "global.h"
#include "fcgbls.h"
#include "errcod.h"
#include "progsw.h"
#include "entry.h"
#include "trcback.h"
#include "objutil.h"
#include "cpopt.h"
#include "tdreloc.h"
#include "genobj.h"

extern  void            GTrcBack(void);
extern  sym_id          FindShadow(sym_id);


void    TDDump() {
//================

// Target dependent symbol table dump.

    obj_ptr     where;
    entry_pt    *entry;

    LocalData = ObjPtr;
    if( _GenObjectFile() ) {
        GObjStartLD();
    }
#if _TARGET == _80386
    BackPatch(EpiFCode->ns.address.la + offsetof(epilog_sequence,my_data_base));
#endif
    if( ( SubProgId != NULL ) &&
            ( SubProgId->ns.flags & SY_SUBPROG_TYPE ) == SY_FUNCTION ) {
        where = EpiFCode->ns.address.la + offsetof( epilog_sequence, func_val );
        entry = Entries;
        while( entry != NULL ) {
            LclReloc( FindShadow( entry->id )->ns.reloc_chain.lr, where );
            entry = entry->link;
        }
    }
    GTrcBack();
}


int     TDTmpAreaSize() {
//=======================

// Size of temporary data area.

    return( MaxTempIndex );
}


void    BackPatch( obj_ptr loc ) {
//================================

// Patch the given location with ObjPtr.

    if( ( ProgSw & PS_ERROR ) == 0 ) {
#if _TARGET == _80386

        targ_addr       ta;

        if( _GenObjectFile() ) {
            ta.offset = ObjPtr;
            GObjPutPtr( loc, ObjPtr, GObjFindSeg( ta ) );
            return;
        }
#endif
        PutPtr( loc, ObjPtr );
    }
}


void    FixTrcBack( sym_id ste_ptr ) {
//====================================

// Fix up traceback for subprograms.

    if( ( ste_ptr->ns.flags & SY_SUBPROG_TYPE ) != SY_BLOCK_DATA ) {
        BackPatch( ste_ptr->ns.address.la + offsetof( sp_entry, name ) );
    }
    if( ! TBLabelPatched ) {
        // For DATA statement tracebacks
        BackPatch( TBLabel->ns.address.la + offsetof( traceback, name ) );
        TBLabelPatched = TRUE;
    }
}


void    FixLListHead() {
//======================

// Fix up pointer to dumped variables for debugger.

    if( Options & OPT_DEBUG ) {
        BackPatch( TBLabel->ns.address.la+offsetof( db_traceback, data_area ));
    }
}


void    FixSFTrcBack( sym_id ste_ptr ) {
//======================================

// Fix up traceback for statement function.

    BackPatch( ste_ptr->ns.address.la + offsetof( sf_trace, name ) );
}


void    TDDataSize() {
//====================

// Check if code generated for subroutine is too big.
// This has already been done so we do something else here.
// We make an end of SP segment pointer.

#if _TARGET == _8086
    if( _GenObjectFile() ) {
        GObjFlushLEDATA();
        GObjEndOfSPSeg();
    }
#endif
}
