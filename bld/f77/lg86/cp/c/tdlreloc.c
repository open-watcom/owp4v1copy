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
// TDLRELOC   : Local symbol table relocation
//

#include <string.h>

#include "ftnstd.h"
#include "global.h"
#include "fcgbls.h"
#include "progsw.h"
#include "cpopt.h"
#define hash_entry lib_hash_entry
#include "pcobj.h"
#include "objutil.h"
#include "tdreloc.h"
#include "genobj.h"

extern  targ_addr       GetIFAddr(int);
extern  unsigned_32     ADVNumElts(targ_addr);
#if _TARGET == _8086
#define MAX_SEG         0x10000L
#endif

extern  unsigned_16     ObjCode;


unsigned_32     ArrNumElts( lg_adv PGM *arr ) {
//=============================================

// Get number of array elements.

    return( arr->num_elts );
}


void    LclReloc( obj_ptr chn, obj_ptr where ) {
//==============================================

// Do local relocations.

    if( ( ProgSw & PS_ERROR ) == 0 ) {
#if _TARGET == _80386
        where -= LocalData;     // data relocs relative to base of local data
#endif
        while( chn != NULL ) {
            chn = PutPtr( chn, where );
        }
    }
}


void    ConstReloc( sym_id sym, obj_ptr where ) {
//===============================================

// Relocate constants.

    LclReloc( sym->cn.reloc_chain, where );
}


void    VarReloc( sym_id sym, obj_ptr where ) {
//=============================================

// Relocate variables.

    LclReloc( sym->ns.reloc_chain.lr, where );
}


void    DataReloc( sym_id sym_ptr, obj_ptr where ) {
//==================================================

// Relocate data references

    LclReloc( sym_ptr->ns.reloc_chain.lr, where );
}


void    CodeReloc( sym_id sym_ptr, obj_ptr where ) {
//==================================================

// Relocate code references.

#if _TARGET == _8086
    LclReloc( sym_ptr->ns.reloc_chain.lr, where );
#else
    // on the 386 we put offsets relative to PC+sizeof( obj_ptr )

    obj_ptr     chn;

    if( ( ProgSw & PS_ERROR ) == 0 ) {
        where -= sizeof( obj_ptr );
        chn = sym_ptr->ns.reloc_chain.lr;
        while( chn != NULL ) {
            chn = PutPtr( chn, where - chn );
        }
    }
#endif
}


obj_ptr         LinkReloc( obj_ptr *head ) {
//==========================================

    obj_ptr     curr_offset;

    curr_offset = *head;
    *head = ObjPtr;
    return( curr_offset );
}


obj_ptr         LinkSym( sym_id sym_ptr ) {
//=========================================

// Link a reference to a symbol into the symbol table entry chain.
// Note: This routine is used for statement numbers, symbol table entries,
//       magic symbols, constants, and literals. Therefore, the
//       "reloc_chain" field must be in the same offset in all these
//       structs.

    return( LinkReloc( &sym_ptr->ns.reloc_chain.lr ) );
}


void    SymRef( sym_id sym_ptr ) {
//================================

    OutDataPtr( LinkSym( sym_ptr ) );
}


void    CodeSymRef( sym_id sym_ptr ) {
//====================================

// output a reference to a code symbol

    OutCodePtr( LinkSym( sym_ptr ) );
}
