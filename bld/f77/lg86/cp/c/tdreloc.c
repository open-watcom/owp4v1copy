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
// TDRELOC   : symbol table relocation
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


void    RelocAddr( targ_addr *reloc, targ_addr *address ) {
//=========================================================

// Relocate a global relocation chain.

    targ_addr   where;
    targ_addr   curr;

    if( ( ProgSw & PS_ERROR ) == 0 ) {
        where = *address;
        curr  = *reloc;
        if( _GenObjectFile() ) {
            int         seg;

            if( curr.offset != NULL ) {
                seg = GObjFindSeg( where );
                do {
                    GObjPutAddr( &curr, &where, seg );
                } while( curr.offset != NULL );
            }
            return;
        }
        while( curr.offset != NULL ) {
            PutAddr( &curr, &where );
        }
    }
}


static  void    SCBReloc( obj_ptr rchain, uint size ) {
//=====================================================

// Relocate SCB for constants.

    targ_addr   gbl_chn;
    targ_addr   gbl_addr;

    gbl_chn.offset = rchain;
    size = size;
#if _TARGET == _8086
    if( _GenObjectFile() ) {
        if ( !GObjExistLiteralSegment() ) {
            NewGItem();
            ObjPtr = 0x10;
            GObjMakeNewLiteralSeg();
        } else if ( GObjPackedSegment( size ) ) {
            GObjFlushLEDATA();
            GObjSetLiteralSegmentLen();
            NewGItem();
            ObjPtr = 0x10;
            GObjMakeNewLiteralSeg();
        }
    } else {
        NewGItem();
    }
    gbl_chn.seg = SProgStart.seg;
    gbl_addr.seg = CurrSeg;
#endif
    gbl_addr.offset = ObjPtr;
    RelocAddr( &gbl_chn, &gbl_addr );
}


void    LitSCBReloc( sym_id lit ) {
//=================================

    SCBReloc( lit->lt.address, lit->lt.length );
}


void    TmpSCBReloc( sym_id tmp ) {
//=================================

// Relocate SCB for static temporaries.

    SCBReloc( tmp->ns.address.la, tmp->ns.xt.size );
}


void    TmpRCBReloc( sym_id record ) {
//====================================

// Relocate RCB for static temporaries.

    SCBReloc( record->ns.address.la, record->ns.xt.size );
}


void    SetGblOrg( sym_id gbl ) {
//===============================

// Set base address to start of object code.

    gbl->ns.address.ga.offset = 0;
#if _TARGET == _8086
    if( _GenObjectFile() ) {
        gbl->ns.address.ga.seg = 0;
    } else {
        gbl->ns.address.ga.seg = ObjCode;
    }
#endif
}


unsigned_32     GblOffset( sym_id gbl ) {
//=======================================

// Compute offset relative to base address.

    unsigned_32 offset;

#if _TARGET == _8086
    if( _GenObjectFile() ) return( 0 );
    offset = CurrSeg - gbl->ns.address.ga.seg;
    offset = ( offset << 4 ) + ( ObjPtr - gbl->ns.address.la );
#else
    offset = ObjPtr - gbl->ns.address.la;
#endif
    return( offset );
}


static  void    SegBoundary() {
//=============================

// Align on a segment boundary (offset assumed to be < 16).

#if _TARGET == _8086
    if( ObjPtr != NULL ) {
        ++CurrSeg;
        ObjPtr = NULL;
    }
#endif
}


void    StartGD() {
//=================

// Starting to dump global data.

#if _TARGET == _8086
    NewGItem();
    SegBoundary();
#endif
    if( _GenObjectFile() ) {
        GObjStartGD();
    }
}


#if _TARGET == _8086

void    AlignSeg() {
//==================

// Align on segment boundary (offset is not necessarily < 15).

    NewGItem();
    SegBoundary();
}

#endif


void    SetComAddr( sym_id gbl ) {
//================================

// Set address of common block.

    SegBoundary();
    gbl->ns.address.ga.offset = ObjPtr;
#if _TARGET == _8086
    gbl->ns.address.ga.seg = CurrSeg;
#endif
    if( _GenObjectFile() ) {
        GObjComBlk( gbl );
    }
}


void    SetIFAddr( int if_num, sym_id gbl ) {
//===========================================

// Set address of intrinsic function.

    gbl->ns.address.ga = GetIFAddr( if_num );
    gbl->ns.flags |= SY_ADDR_ASSIGNED;
}


void    SetIFName( sym_id sym_ptr ) {
//===================================

// Set name of intrinsic function.

    sym_ptr = sym_ptr;
}


void    MakeGblAddr( sym_id local, sym_id gbl ) {
//===============================================

// Make address a global address.

#if _TARGET == _8086
    gbl->ns.address.ga.seg = SProgStart.seg;
#endif
    gbl->ns.address.ga.offset = local->ns.address.la;
}


static  void    AdjGblAddr( targ_addr *gbl, signed_32 adjust ) {
//==============================================================

// Adjust global address by the specified offset.

#if _TARGET == _8086
    unsigned_32 addr;

    // don't adjust segment if generating an object file
    if( _GenObjectFile() ) return;
    addr = gbl->seg;
    addr <<= 4;
    addr += gbl->offset;
    addr += adjust;
    gbl->seg = addr >> 4;
    gbl->offset = addr & 0xf;
#else
    gbl->offset += adjust;
#endif
}


void    GblReloc( sym_id sym_ptr ) {
//==================================

// Relocate global relocation chain for subprogram.

    if( _GenObjectFile() && !(sym_ptr->ns.flags & SY_ADDR_ASSIGNED) ) {
        GObjRelocExt( sym_ptr );
        return;
    }
    RelocAddr( &sym_ptr->ns.reloc_chain, &sym_ptr->ns.address );
}


void    GblVarReloc( reloc_head *reloc, targ_addr *base, signed_32 adjust ) {
//===========================================================================

// Relocate global variables.

    targ_addr   addr;

    addr = *base;
    AdjGblAddr( &addr, adjust );
    RelocAddr( reloc, &addr );
}


#if _TARGET == _8086

static  void    SetEquivalenceAddr( sym_id sym, signed_32 offset ) {
//==================================================================

// Adjust address of equivalenced variable.

    unsigned_16         eq_seg;
    unsigned_16         eq_off;

    eq_off = sym->ns.address.ga.offset;
    eq_seg = sym->ns.address.ga.seg;

    // calculate address of leader in equivalence block
    if( eq_off + offset >= MAX_SEG ) {
        offset -= (MAX_SEG - eq_off);
        eq_seg++;
        while( offset > MAX_SEG ) {
            offset -= MAX_SEG;
            eq_seg++;
        }
    }
    eq_off += offset;

    sym->ns.address.ga.offset = eq_off;
    sym->ns.address.ga.seg = eq_seg;
}


void    SetEQVariablesAddr( sym_id sym, sym_id leader, signed_32 offset ) {
//=========================================================================

// Set address of leader of equivalence set.

    signed_32           lead_off;
    unsigned_16         eq_seg;
    unsigned_16         eq_off;

    // calculate address of start of equivalence block
    eq_off = leader->ns.address.ga.offset;
    eq_seg = leader->ns.address.ga.seg;
    lead_off = -leader->ns.si.va.vi.ec_ext->low;
    if( eq_off < lead_off ) {
        lead_off -= eq_off;
        eq_off = 0;
        while( lead_off > MAX_SEG ) {
            lead_off -= MAX_SEG;
            eq_seg--;
        }
    }
    eq_off -= lead_off;
    sym->ns.address.ga.offset = eq_off;
    sym->ns.address.ga.seg = eq_seg;
    SetEquivalenceAddr( sym, offset );
}


void SetEQLeadersAddr( sym_id sym ) {
//===================================

// Set address of leader of equivalence set.

    // get address of start of equivalence block
    sym->ns.address.ga.offset = GObjGetGlobalVirtualPtr();
    sym->ns.address.ga.seg = CurrSeg;

    // get address of leader from start of equivalence block
    SetEquivalenceAddr( sym, -sym->ns.si.va.vi.ec_ext->low );
}


void SetCurrentGblAddr( sym_id base, unsigned_32 *offset ) {
//==========================================================

// Set the current address.

    if ( _GenObjectFile() ) {
        *offset = 0L;
        base->ns.address.ga.offset = GObjGetGlobalVirtualPtr();
        base->ns.address.ga.seg = CurrSeg;
    }
}


void SetCommonBlockSize( unsigned_32 size ) {
//=========================================

// Set common segment size.  This is necessary, otherwise
// because common blocks can be more than 64K

    if ( _GenObjectFile() ) {
        GObjSetCommonBlockSize( size );
    }
}


void PackGblData( unsigned long fin_size ){
//=========================================

// Create all segments necessary for huge data

    long        x;
    long        segnum;

    if ( _GenObjectFile() ) {
        // Check to see if current segment is filled, is so make more
        if ( ( fin_size > MAX_SEG ) || GObjGlobalPackedSegment( fin_size ) ) {
            GObjSetGlobalSegFull();
            segnum = GObjCalculateNumOfGblSegs( fin_size );
            // Calculate the amount that will fill current segment
            fin_size -= ( MAX_SEG - GObjGetGlobalVirtualPtr() );
            // Distribute the rest over the created segments
            for( x = 0; x < segnum; x++ ) {
                NewGItem();
                SegBoundary();
                CurrSeg++;
                GObjMakeNewGlobalSeg();
                GObjSetGlobalSegFull();
                fin_size -= MAX_SEG;
            }
            // last segment is not full filled so we cannot restore the
            // last decrementation made of fin_size.  We now set the mew
            // virtual pointer.
            fin_size += MAX_SEG;
            GObjSetGlobalVirtualPtr( fin_size );
        } else {
            GObjIncGlobalVirtualPtr( fin_size );
        }
    }
}


void CloseCommonBlock( void ) {
//=============================

// Create all segments necessary for huge data

    unsigned long       x;
    unsigned long       segnum;
    unsigned long       size;

    if ( _GenObjectFile() ) {
        // Using the size of the common block as the last offset
        // we calculate how many more segs we need and than we close
        // up.
        size = GObjCalculatePrevSize( GObjGetCommonBlockSize() );
        if ( ( size > MAX_SEG ) || GObjGlobalPackedSegment( size ) ) {
            GObjSetCommonSegFull();
            segnum = GObjCalculateNumOfGblSegs( size );
            // Calculate the amount that will fill current segment
            size -= ( MAX_SEG - GObjGetGlobalVirtualPtr() );
            // Distribute the rest over the created segments
            for( x = 0; x < segnum; x++ ) {
                NewGItem();
                SegBoundary();
                CurrSeg++;
                GObjMakeNewCommonSeg();
                GObjSetCommonSegFull();
                size -= MAX_SEG;
            }
            // last segment is not full filled so we cannot restore the
            // last decrementation made of size.  We now set the mew
            // virtual pointer.
            size += MAX_SEG;
            GObjSetGlobalVirtualPtr( size );
        } else {
            GObjIncGlobalVirtualPtr( size );
        }
        GObjSetCommonSegmentLen();
        GObjSetCommonBlockLen();
        GObjSetGlobalVirtualPtr( 0 );
        // Create next global segment
        NewGItem();
        SegBoundary();
        CurrSeg++;
        GObjMakeNewGlobalSeg();
        GObjSetGlobalSegFull();
    }
}


static void packCommonData( com_reloc *reloc, targ_addr *address ) {
//=================================================================

// Create all segments necessary for huge data

    unsigned long       x;
    unsigned long       segnum;
    unsigned long       size;

    if ( _GenObjectFile() ) {
        // Check to see if current segment is filled, if so make more
        // Since we measure common segments in terms of offsets, we
        // calculate the size of the previous item, and use
        // GObjGlobalPackedSegment to determine whether it crossed
        // any boundaries, if it does we allocate the right amount of
        // segments and we find the proper offset in the proper segment
        // of the current address.  We then return.
        // The last item in the common block will be finished off by a
        // separate function.
        size = GObjCalculatePrevSize( reloc->offset );
        reloc->offset = 0;
        if ( ( size > MAX_SEG ) || GObjGlobalPackedSegment( size ) ) {
            GObjSetCommonSegFull();
            segnum = GObjCalculateNumOfGblSegs( size );
            // Calculate the amount that will fill current segment
            size -= ( MAX_SEG - GObjGetGlobalVirtualPtr() );
            // Distribute the rest over the created segments
            for( x = 0; x < segnum; x++ ) {
                NewGItem();
                SegBoundary();
                CurrSeg++;
                GObjMakeNewCommonSeg();
                GObjSetCommonSegFull();
                size -= MAX_SEG;
            }
            // last segment is not full filled so we cannot restore the
            // last decrementation made of size.  We now set the mew
            // virtual pointer.
            size += MAX_SEG;
            GObjSetGlobalVirtualPtr( size );
        } else {
            GObjIncGlobalVirtualPtr( size );
        }
        address->seg = CurrSeg;
        address->offset = GObjGetGlobalVirtualPtr();
    }
}

#endif


void    ComVarReloc( com_reloc *reloc, obj_addr *address ) {
//==========================================================

// Relocate variables in common.

#if _TARGET == _8086
    packCommonData( reloc, address );
#endif
    GblVarReloc( &reloc->reloc_chain, address, reloc->offset );
}


void    LinkGblChain( obj_ptr local, targ_addr *gbl ) {
//=====================================================

// Link local reference into global relocation chain.

    targ_addr   where;
    targ_addr   save;

    if( ProgSw & PS_ERROR ) return;
#if _TARGET == _8086
    if( SProgStart.seg == 0 ) return;
    where.seg = SProgStart.seg;
#endif
    where.offset = local;
    save = where;
    if( _GenObjectFile() ) {
        GObjPutLink( &where, gbl );
    } else {
        PutAddr( &where, gbl );
    }
    *gbl = save;
}


void    LinkComReloc( sym_id local, com_reloc *curr_reloc ) {
//===========================================================

// Link symbol in common into common relocation chain.

    LinkGblChain( local->ns.reloc_chain.lr, &curr_reloc->reloc_chain );
}


void    LinkGblSym( sym_id local, sym_id gbl ) {
//==============================================

// Link global symbols.

    LinkGblChain( local->ns.reloc_chain.lr, &gbl->ns.reloc_chain );
}


void    GblUndefined( unsigned_32 size ) {
//========================================

// Undefine a global symbol.

#if _TARGET == _8086
        while( size > 0xfff0 ) {
            if( Options & (OPT_LINK | OPT_OBJECT) ) {
                ObjPtr += 0xfff0;
            } else {
                Undefined( 0xfff0 );
            }
            NewGItem();                 // this modifies CurrSeg
            size -= 0xfff0;
        }
        if( Options & (OPT_LINK | OPT_OBJECT) ) {
            ObjPtr += size;
        } else {
            Undefined( size );
        }
#else
        if( Options & (OPT_LINK | OPT_OBJECT) ) {
            ObjPtr += size;
        } else {
            Undefined( size );
        }
#endif
}
