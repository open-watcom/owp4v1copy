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
* Description:  Routines for creating DOS/16M load files.
*
****************************************************************************/


#include <string.h>
#include "linkstd.h"
#include "alloc.h"
#include "msg.h"
#include "exedos.h"
#include "exe16m.h"
#include "load16m.h"
#include "pcobj.h"
#include "reloc.h"
#include "specials.h"
#include "wlnkmsg.h"
#include "virtmem.h"
#include "loadfile.h"
#include "fileio.h"
#include "objcalc.h"
#include "dbgall.h"


/* Notes: 
 *  - Currently we only support RSI-1 style relocations (2 segments, one containing
 * fixup segment values and the other offsets, presumably limited to 32K relocs).
 * Newer tools create RSI-2 style relocations in slightly more complex format - a word 
 * containing segment value, a word containing count, and specified number of word-sized 
 * offsets, repeated for each segment to be fixed up. RSI-2 style relocs may span multiple
 * segments (huge seg). Support for RSI-2 could be added relatively easily.
 *   - It might be good to default to 'AUTO' (ie. relocatable) modules, since 
 * non-relocatable DOS/16M modules only run on VCPI or raw systems, not DPMI.
 */

static unsigned long InitAlloc;      // size of initial allocation.
static unsigned long PrevSeg = 1;

#define EXE_HEAD_GAP (0x30 - sizeof( dos_exe_header ) - sizeof( unsigned_32 ) )


static void WriteGDT( bool twoextra )
/***********************************/
// write out the GDTs
{
    gdt_info        gdt;
    group_entry     *currgrp;

// write gdt[0].
    gdt.gdtreserved = 0; //((InitAlloc + 15) >> 4) + FmtData.u.d16m.datasize;
    gdt.gdtlen = FmtData.u.d16m.options;
    gdt.gdtaddr = 0;
    gdt.gdtaccess = 0;
    gdt.gdtaddr_hi = 0;
    if( LinkState & MAKE_RELOCS ) {
        gdt.gdtaddr_hi = 1;
    }
    WriteLoad( &gdt, sizeof( gdt_info ) );
// write gdt[1].
    gdt.gdtlen = (NumGroups + 1 + NUM_RESERVED_SELS) * sizeof( gdt_info ) - 1;
    if( twoextra ) {
        gdt.gdtlen += sizeof( gdt_info );
    }
    gdt.gdtaccess = D16M_ACC_DATA;
    gdt.gdtaddr = FmtData.u.d16m.selstart;
    gdt.gdtreserved = FmtData.u.d16m.buffer;
    gdt.gdtaddr_hi = FmtData.u.d16m.strategy;
    WriteLoad( &gdt, sizeof( gdt_info ) );
// write out the rest of the reserved selectors.
    PadLoad( (NUM_RESERVED_SELS - 2) * sizeof( gdt_info ) );
// write program gdt's.
    gdt.gdtaddr_hi = 0;
    for( currgrp = Groups; currgrp != NULL; currgrp = currgrp->next_group ) {
        if( currgrp->size > 0 ) {
            gdt.gdtlen = MAKE_PARA( currgrp->size ) - 1; // para align.
        } else {
            gdt.gdtlen = 0;
        }
        gdt.gdtaddr = 0; // currgrp->u.dos_segment;
        gdt.gdtreserved = ((unsigned_32)gdt.gdtlen + 1) >> 4;   // mem size in paras
        if( currgrp->segflags & SEG_DATA ) {
            gdt.gdtaccess = D16M_ACC_DATA;
            if( currgrp == DataGroup ) {
                gdt.gdtreserved = FmtData.u.d16m.datasize;
                if( FmtData.u.d16m.flags & TRANS_STACK ) {
                    gdt.gdtreserved |= TRANSPARENT;
                }
            }
            if( FmtData.u.d16m.flags & TRANS_DATA ) {
                gdt.gdtreserved |= TRANSPARENT;
            }
        } else {
            gdt.gdtaccess = D16M_ACC_CODE;
        }
        WriteLoad( &gdt, sizeof( gdt_info ) );
    }
// write out gdt[maxindex] and gdt[maxindex+1]
    gdt.gdtreserved = 0;
    gdt.gdtaddr = 0;
    if( LinkState & MAKE_RELOCS ) {
        gdt.gdtaccess = D16M_ACC_DATA;
        gdt.gdtlen = MAKE_PARA( Root->relocs * sizeof( unsigned_16 ) ) - 1;
        gdt.gdtreserved = ((unsigned_32)gdt.gdtlen + 1) >> 4;   // mem size in paras
    } else {
        gdt.gdtaccess = 0;
        gdt.gdtlen = 0;
    }
    WriteLoad( &gdt, sizeof( gdt_info ) );
    if( twoextra ) {
        WriteLoad( &gdt, sizeof( gdt_info ) );
    }
}

static unsigned_32 Write16MData( unsigned hdr_size )
/**************************************************/
{
    group_entry     *group;
    unsigned_32     wrote;
    unsigned_32     temp;

    DEBUG(( DBG_BASE, "Writing DOS/16M data" ));

//    CurrSect = Root;      // needed for WriteInfo.
    SeekLoad( hdr_size );
    /* write groups.*/
    wrote = hdr_size;
    for( group = Groups; group != NULL; group = group->next_group ) {
        WriteGroupLoad( group );
        temp = NullAlign( 0x10 );       // paragraph alignment.
        if( !(group->segflags & SEG_DATA) ) {
            InitAlloc += temp - wrote;    // code counts toward initial alloc.
        }
        wrote = temp;
    }
    return( wrote );
}

struct context {
    unsigned_16     *reloc_data;
    bool            do_offsets;
};

static bool RelocWalkFn( void *data, unsigned_32 size, void *ctx )
/****************************************************************/
{
    struct context      *info = ctx;
    struct {
        unsigned_16     off;
        unsigned_16     seg;
    }                   *reloc = data;
    unsigned_16         item;
    int                 i;

    for( i = 0; i < size / sizeof( *reloc ); ++i ) {
        item = info->do_offsets ? reloc->off : reloc->seg;
        *info->reloc_data++ = item;
        ++reloc;
    }

    return( FALSE );    /* don't stop walking */
}

static unsigned_32 WriteRelocBlock( bool dooffsets )
/**************************************************/
{
    RELOC_INFO          *relocs;
    unsigned_16         *reloc_data;
    unsigned            num_relocs;
    struct context      info;

    relocs = Root->reloclist;
    num_relocs = Root->relocs;
    reloc_data = ChkLAlloc( 2 * sizeof( unsigned_16 ) * num_relocs );
    info.do_offsets = dooffsets;
    info.reloc_data = reloc_data;
    WalkRelocList( &relocs, RelocWalkFn, &info );
    WriteLoad( reloc_data, sizeof( unsigned_16 ) * num_relocs );
    LFree( reloc_data );
    return( NullAlign( 0x10 ) );
}

// relocations for DOS/16 are stored internally in one virtual memory block,
// with the segment selectors being stored in the first half of the memory
// block, and the segment offsets being stored in the second half (RSI-1 style).

static void Write16MRelocs( unsigned_32 *exe_size )
/*************************************************/
{
    if( Root->relocs != 0 ) {
        WriteRelocBlock( FALSE );
        *exe_size = WriteRelocBlock( TRUE );
    }
}

extern void Fini16MLoadFile( void )
/*********************************/
{
    unsigned_32         hdr_size;
    unsigned_32         temp;
    unsigned_32         exe_size;
    dos_exe_header      exe_head;
    bool                twoextra;  // TRUE iff 2 (not 1) extra GDT's at end

    memset( &exe_head, 0, sizeof( exe_head ) );
    twoextra = FALSE;
    InitAlloc = (NumGroups + 1) * sizeof( gdt_info );
    hdr_size = InitAlloc + sizeof( dos_exe_header ) + EXE_HEAD_GAP + sizeof( unsigned_32 )
                                   + NUM_RESERVED_SELS * sizeof( gdt_info );
    if( hdr_size & 0xF ) {    // if not a complete paragraph..
        hdr_size += sizeof( gdt_info );   // make it a complete paragraph.
        InitAlloc += sizeof( gdt_info );
        twoextra = TRUE;
    }

    exe_size = Write16MData( hdr_size );
    Write16MRelocs( &exe_size );
    DBIWrite();

    SeekLoad( 0 );
    _HostU16toTarg( 0x5742, exe_head.signature );
    temp = hdr_size / 16U;
    _HostU16toTarg( exe_size % 512U, exe_head.mod_size );
    temp = exe_size / 512U;
    _HostU16toTarg( temp, exe_head.file_size );
    _HostU16toTarg( FmtData.u.d16m.extended, exe_head.max_16 );
    _HostU16toTarg( 0, exe_head.min_16 );
    _HostU16toTarg( StartInfo.addr.off, exe_head.IP );
    _HostU16toTarg( StartInfo.addr.seg, exe_head.CS_offset );
    _HostU16toTarg( StackAddr.seg, exe_head.SS_offset );
    _HostU16toTarg( StackAddr.off, exe_head.SP );
    _HostU16toTarg( 0, exe_head.chk_sum );
    _HostU16toTarg( FmtData.u.d16m.gdtsize, exe_head.reloc_offset );
    _HostU16toTarg( 2000, exe_head.overlay_num );   // pretend GLU version 2.0
    WriteLoad( &exe_head, sizeof( dos_exe_header ) );
    WriteLoad( &exe_size, sizeof( unsigned_32 ) );
    PadLoad( EXE_HEAD_GAP );
    WriteGDT( twoextra );
}

extern unsigned NextDos16Seg( void )
/**********************************/
{
    if( PrevSeg == 1 ) {
        PrevSeg = FmtData.u.d16m.selstart;
    } else {
        PrevSeg += 8;
        if( PrevSeg >= 0xA000 && !(PrevSeg & 0x1FF) ) {  // reserved selector
            PrevSeg += 8;
        }
    }
    if( PrevSeg > 0xFFFF ) {
        LnkMsg( FTL + MSG_TOO_MANY_SELECTORS, NULL );
    }
    return( (unsigned)PrevSeg );
}

extern segment Find16MSeg( segment selector )
/*******************************************/
// this finds the dos_segment value which corresponds to the given selector.
{
    group_entry     *currgrp;
    segment         result;

    result = 0;
    for( currgrp = Groups; currgrp != NULL; currgrp = currgrp->next_group ) {
        if( currgrp->grp_addr.seg == selector ) {
            result = currgrp->u.dos_segment;
            break;
        }
    }
    return( result );
}

extern void CalcGrpSegs( void )
/*****************************/
// go through group list & calculate what the segment would be if we were
// running under DOS.
{
    group_entry     *currgrp;
    offset          addr;

    addr = 0;
    for( currgrp = Groups; currgrp != NULL; currgrp = currgrp->next_group ) {
        addr = MAKE_PARA( addr );       // addr is paragraph aligned.
        currgrp->u.dos_segment = addr >> 4;
        addr += currgrp->totalsize;
    }
}
