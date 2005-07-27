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
* Description:  Functions for creating binary and hex load files.
*
****************************************************************************/


#include <string.h>
#include <stdio.h>
#include "linkstd.h"
#include "ring.h"
#include "pcobj.h"
#include "newmem.h"
#include "msg.h"
#include "alloc.h"
#include "wlnkmsg.h"
#include "virtmem.h"
#include "fileio.h"
#include "objcalc.h"
#include "dbgall.h"
#include "loadraw.h"
#include "loadfile.h"


static bool WriteBinGroup( group_entry *group )
/*********************************************/
/* write the data for group to the loadfile */
/* returns TRUE if the file should be repositioned */
{
    unsigned long       loc;
    signed  long        diff;
    group_entry         *wrkgrp;
    section             *sect;
    bool                repos;
    outfilelist         *finfo;

    repos = FALSE;
    if( group->leaders->class->flags & CLASS_COPY ) {
        wrkgrp = group->leaders->class->DupClass->segs->group;
    } else {
        wrkgrp = group;
    }
    if( wrkgrp->size != 0  ) {
        sect = wrkgrp->section;
        CurrSect = sect;
        finfo = sect->outfile;
        loc = SUB_ADDR( group->grp_addr, sect->sect_addr ) + sect->u.file_loc;
        if ( (long)loc >= 0) {  // Offset may make this go negative for NOEMIT classes or segments
            diff = loc - finfo->file_loc;
            if( diff > 0 ) {
                PadLoad( diff );
            } else if( diff != 0 ) {
                SeekLoad( loc );
                repos = TRUE;
            }
            DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                &group->grp_addr, sect->ovl_num, loc, finfo->fname ));
            WriteGroupLoad( group );
            loc += group->size;
            if( loc > finfo->file_loc ) {
                finfo->file_loc = loc;
            }
        }
    }
    return( repos );
}


extern void BinOutput( void )
/***************************/
{
    outfilelist         *fnode;
    group_entry         *group;
    bool                repos;

    OrderGroups( CompareDosSegments );
    CurrSect = Root;        // needed for WriteInfo.
    Root->sect_addr = Groups->grp_addr;
    fnode = Root->outfile;
    fnode->file_loc = 0;
    Root->u.file_loc = (Root->sect_addr.seg << FmtData.SegShift) + Root->sect_addr.off - FmtData.output_offset;
    /* write groups */
    for( group = Groups; group != NULL; group = group->next_group ) {
        repos = WriteBinGroup( group );
        if( repos ) {
            SeekLoad( fnode->file_loc );
        }
    }
    WriteDBI();
}

// The following routine generates Intel Hex records using data buffered in LineBuf.
// A full record is 16 bytes of data.  Partial records are allowed when needed
// because of gaps in the loadfile data.  Initially segmented addressing is assumed
// with a starting segment value of 0 (Original 64K Intel Hex records).  If data
// is encountered above 64K, the routine outputs a segment record, seamlessly
// switching to segmented mode.  Segmented mode can use either the standard 4 bit
// shift (default) or the shift defined in FmtData.SegShift, or if set, the
// value in FmtData.HexSegShift, which is uniquely used by this function.  The
// purpose for this is that 24 bit addressing can be fully handled using segmented
// addressing (rather than extended linear addressing) if SegShift is 8.
// If data is encountered above what Segmented addressing can handle (1 M for default)
// the routine outputs an extended linear address record, seamlessly switching
// to a full 32 bit address range.  This approach provides full backward compatibility
// for systems which cannot read the newer formats when creating files that don't
// need the extra space.

#define HEXLEN 16   // number of bytes of data in a full record
static unsigned long    NextAddr;
static unsigned         Segment;
static bool             Linear;
static char             LineBuf[HEXLEN];
static unsigned         BufOfs;

static void WriteHexLine( void )
/******************************/
{

    char            str_buf[2 * HEXLEN + 15];
    unsigned        checksum;
    unsigned        i;
    unsigned        seg_shift = FmtData.output_hshift ? FmtData.HexSegShift : FmtData.SegShift;
    unsigned        offset;

    if( NextAddr + BufOfs >= (0x10000L << seg_shift) && !Linear ) {
        Linear = 1;
        Segment = 0;
    }

    if( !Linear ) {
        if( NextAddr - (Segment << seg_shift) + BufOfs > 0x10000L ) { // See if we need to output a segment record
            Segment = (unsigned int)(NextAddr >> seg_shift);
            sprintf( str_buf, ":02000002%04x%02x\r\n", Segment, (-(4 + (Segment >> 8) + Segment & 0xFF)) & 0xFF );
            WriteLoad( str_buf, 17 );
        }
        offset = (unsigned int)(NextAddr - (Segment << seg_shift));
    }
    else {
        if( NextAddr - (Segment << 16) + BufOfs > 0x10000L ) {  // See if we need to output
            Segment = (unsigned int)(NextAddr >> 16);           //   an extended linear record
            sprintf( str_buf, ":02000004%04x%02x\r\n", Segment, (-(6 + (Segment >> 8) + Segment & 0xFF)) & 0xFF );
            WriteLoad( str_buf, 17);
        }
        offset = (unsigned int)(NextAddr - (Segment << 16));
    }

    sprintf( str_buf, ":%02x%04x00", BufOfs, offset );      // Intel Hex header
    checksum = BufOfs + (offset >> 8) + (offset & 0xFF);    // Start checksum using above elements

    i = 0;
    while( BufOfs ) { // Do data bytes, leaving BufOfs 0 at end
        sprintf( str_buf + 9 + 2 * i, "%02x", LineBuf[i] );
        checksum += LineBuf[i++];
        --BufOfs;
    }
    sprintf( str_buf + 9 + 2 * i, "%02x\r\n", (-checksum) & 0xFF );
    WriteLoad( str_buf, 13 + 2 * i );
}

static bool WriteHexData( void *_sdata, void *_addr )
/***************************************************/
{
    segdata         *sdata = _sdata;
    unsigned long   PieceAddr = *(unsigned long *)_addr + sdata->a.delta;
    unsigned int    len = sdata->length;
    unsigned int    offset;
    unsigned int    piece;

    if( !sdata->isuninit && !sdata->isdead && len > 0 ) {
        if( PieceAddr != NextAddr + BufOfs ) { // Must start new record if address not contiguous
            if( BufOfs ) {              // If partial record in buffer, flush
                WriteHexLine();
            }
            NextAddr = PieceAddr;
        }
        offset = 0;
        while( len ) {                  // Now lob out records
            piece = HEXLEN - BufOfs;    // Handle partially filled buffer
            if( piece > len ) {         // Handle not enough to fill buffer
                piece = len;
            }
            ReadInfo( sdata->data + offset, LineBuf + BufOfs, piece );
            BufOfs += piece;
            if( BufOfs == HEXLEN ) {
                WriteHexLine();         // Only write full buffers
                NextAddr += HEXLEN;     // NextAddr reflects start of line
            }                           // Partial records will be written later
            offset += piece;            //   if address is not contiguous
            len -= piece;
        }
    }
    return( FALSE );
}

static bool DoHexLeader( void *seg, void *addr )
/**********************************************/
{
    if ( !(((seg_leader *)seg)->class->flags & CLASS_NOEMIT ||
           ((seg_leader *)seg)->segflags & SEG_NOEMIT) ) {
        unsigned long   segaddr = *(unsigned long *)addr + GetLeaderDelta( seg );

        RingLookup( ((seg_leader *)seg)->pieces, WriteHexData, &segaddr );
    }
    return( FALSE );
}

static bool DoHexDupLeader( void *seg, void *addr )
/************************************************/
{
    unsigned long   segaddr = *(unsigned long *)addr + GetLeaderDelta( seg );

    RingLookup( ((seg_leader *)seg)->pieces, WriteHexData, &segaddr );
    return( FALSE );
}

void WriteStart( void )
/*********************/
{
    char    str_buf[22];

    if( StartInfo.addr.off > 0xffff ) {
        sprintf( str_buf, ":04000005%08lx%02x\r\n", StartInfo.addr.off,
                (-(9 + (StartInfo.addr.off >> 24) + ((StartInfo.addr.off >> 16) & 0xFF) +
                ((StartInfo.addr.off >> 8) & 0xFF) + (StartInfo.addr.off & 0xFF) )) & 0xFF );
    } else {
        sprintf( str_buf, ":04000003%04lx%04lx%02x\r\n", StartInfo.addr.seg, StartInfo.addr.off,
                (-(7 + (StartInfo.addr.seg >> 8) + (StartInfo.addr.seg & 0xFF) +
                (StartInfo.addr.off >> 8) + (StartInfo.addr.off & 0xFF) )) & 0xFF );
    }
    WriteLoad( str_buf, 21 );
}

extern void HexOutput( void )
/***************************/
{
    outfilelist         *fnode;
    group_entry         *group;
    group_entry         *wrkgrp;
    unsigned long       addr;
    section             *sect;
    outfilelist         *finfo;

    NextAddr = 0L;  // Start at absolute linear address 0
    Linear   = 0;   //       in segmented mode
    Segment  = 0;
    BufOfs   = 0;
    OrderGroups( CompareDosSegments );
    CurrSect = Root;    // needed for WriteInfo.
    Root->sect_addr = Groups->grp_addr;
    fnode = Root->outfile;
    fnode->file_loc = 0;
    Root->u.file_loc = (Root->sect_addr.seg << FmtData.SegShift) + Root->sect_addr.off - FmtData.output_offset;
    /* write groups */
    for( group = Groups; group != NULL; group = group->next_group ) {
        if( group->leaders->class->flags & CLASS_COPY ) {
            wrkgrp = group->leaders->class->DupClass->segs->group;
        } else {
            wrkgrp = group;
        }
        if( wrkgrp->size != 0 ) {
            sect = wrkgrp->section;
            CurrSect = sect;
            finfo = sect->outfile;
            addr = SUB_ADDR( group->grp_addr, sect->sect_addr ) + sect->u.file_loc;
            DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                &group->grp_addr, sect->ovl_num, loc, finfo->fname ));
            if( group->leaders->class->flags & CLASS_COPY ) {
               Ring2Lookup( wrkgrp->leaders, DoHexDupLeader, &addr );
           } else {
               Ring2Lookup( wrkgrp->leaders, DoHexLeader, &addr );
           }
        }
    }
    if( FmtData.output_start ) {
       WriteStart();
    }
    WriteLoad( ":00000001ff\r\n", 13 );
    WriteDBI();
}
