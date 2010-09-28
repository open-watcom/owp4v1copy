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
* Description:  Routines for creation of RDOS device driver files
*
****************************************************************************/


#include <string.h>
#include "linkstd.h"
#include <exerdos.h>
#include "pcobj.h"
#include "newmem.h"
#include "msg.h"
#include "alloc.h"
#include "reloc.h"
#include "wlnkmsg.h"
#include "virtmem.h"
#include "fileio.h"
#include "overlays.h"
#include "loadfile.h"
#include "objcalc.h"
#include "ring.h"
#include "dbgall.h"
#include "loadrdv.h"

static unsigned_16  CodeSeg = 0;
static unsigned_16  DataSeg = 0;
static unsigned_16  CodeSize = 0;
static unsigned_16  DataSize = 0;
static int          CodePos = 0;
static int          DataPos = 0;

static bool RelocWalkFn( void *data, unsigned_32 size, void *ctx )
/****************************************************************/
{
    int                 pos;
    int                 i;
    dos_addr            *reloc = (dos_addr *)data;
//    unsigned_16         seg;

    for( i = size; i > 0; i -= sizeof(dos_addr) ) {
        pos = 0;
        if( reloc->seg == CodeSeg )
            pos = CodePos;
        if( reloc->seg == DataSeg )
            pos = DataPos;
        if( pos ) {
            pos += reloc->off;
            Root->outfile->file_loc = pos;
/*            SeekLoad( pos );            
            ReadLoad( &seg, sizeof(seg) );
            if( seg == CodeSeg ) {
                seg = RdosCodeSel;
                SeekLoad( pos );
                WriteLoad( &seg, sizeof(seg) );
            }
            if( seg == DataSeg ) {
                seg = RdosDataSel;
                SeekLoad( pos );            
                WriteLoad( &seg, sizeof(seg) );
            } */
        }
        reloc++;
    }

    return( FALSE );   /* don't stop walking */
}

static unsigned long WriteRDOSData( unsigned_32 hdr_size )
/**********************************************************/
/* copy code from extra memory to loadfile */
{
    group_entry         *group;
    SECTION             *sect;
    RELOC_INFO          *relocs;
    struct seg_leader   *leader;
    SEGDATA             *piece;
    int                 iscode;
    int                 isdata;
    unsigned_32         file_size = 0;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups( CompareDosSegments );
    CurrSect = Root;        // needed for WriteInfo.

    Root->outfile->file_loc = Root->u.file_loc;
    Root->sect_addr = Groups->grp_addr;
    leader = 0;

/* write groups and relocations */
    for( group = Groups; group != NULL; ) {
        if( leader != group->leaders ) {
            iscode = 0;
            isdata = 0;
            leader = group->leaders;
            if( leader && leader->size ) {
                piece = leader->pieces; 
                if( piece ) {
                    if( piece->iscode ) {
                        CodeSeg = leader->seg_addr.seg;
                        CodePos = Root->outfile->file_loc;
                        iscode = 1;
                    }
                    if( piece->isidata || piece->isuninit ) {
                        DataSeg = leader->seg_addr.seg;
                        DataPos = Root->outfile->file_loc;
                        isdata = 1;
                    }
                }
            }
        }
        sect = group->section;
        CurrSect = sect;
        WriteDOSGroup( group );
        if( group->totalsize > group->size )
            PadLoad( group->totalsize - group->size );
        file_size += group->totalsize;
        if( iscode )
            CodeSize += group->totalsize;
        if( isdata )
            DataSize += group->totalsize;
        group = group->next_group;
    }

    relocs = Root->reloclist;
    if( Root->relocs )
        WalkRelocList( &relocs, RelocWalkFn, 0 );

    return( file_size );
}

void FiniRdosLoadFile( void )
/* terminate writing of load file */
{
    unsigned_32         hdr_size;
    unsigned_16         code_size;
    unsigned_16         temp16;
    rdos_dev16_header   exe_head;

    hdr_size = sizeof( rdos_dev16_header );
    SeekLoad( hdr_size );
    Root->u.file_loc = hdr_size;
    code_size = WriteRDOSData( hdr_size );
    DBIWrite();
    SeekLoad( 0 );
    _HostU16toTarg( RDOS_SIGNATURE_16, exe_head.signature );
    _HostU16toTarg( StartInfo.addr.off, exe_head.IP );
    _HostU16toTarg( code_size, exe_head.code_size );
    temp16 = (unsigned_16)RdosCodeSel;
    _HostU16toTarg( temp16, exe_head.code_sel );
    _HostU16toTarg( 0, exe_head.data_size );
    temp16 = (unsigned_16)RdosDataSel;
    _HostU16toTarg( temp16, exe_head.data_sel );
    WriteLoad( &exe_head, sizeof( rdos_dev16_header ) );
}
