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

static unsigned long WriteRDOSData( unsigned_32 hdr_size )
/**********************************************************/
/* copy code from extra memory to loadfile */
{
    group_entry         *group;
    unsigned_32         file_size = 0;

    DEBUG(( DBG_BASE, "Writing data" ));

    Root->outfile->file_loc = Root->u.file_loc;
    Root->sect_addr = Groups->grp_addr;

    for( group = Groups; group != NULL; ) {
        CurrSect = group->section;
        file_size += group->size;
        WriteDOSGroup( group );
        group = group->next_group;
    }
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
