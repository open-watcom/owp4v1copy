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
// GENFIXUP     : create fixup records ( split from genobj );
//                      to avoid overlay thrashing
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <io.h>

#include "ftnstd.h"
#include "global.h"
#include "bglobal.h"
#include "fcgbls.h"
#include "sdfile.h"
#include "progsw.h"
#include "genobj.h"
#include "cpopt.h"
#include "ifargs.h"
#include "ifnames.h"
#include "saopt.h"
#include "wf77aux.h"
#include "csetinfo.h"
#include <pgmacc.h>
#include "genfixup.h"

extern  void            OutU32(unsigned_32);

static  obj_ptr         *baseLEDATA;    // lowest byte belonging to LEDATA
static  my_fixup        **curFixups;    // linked list of fixups for cur LEDATA
static  uint            *numFixups;     // number of fixups in list

extern  void            GObjFlushLEDATA( void );
extern  void            *FMemAlloc(size_t);

static my_fixup *newFixup( void ) {
//=================================

    my_fixup    *fix;

    // the next sizeof( obj_ptr ) bytes must be in the same LEDATA
    if( (ObjPtr + sizeof( targ_addr ) - *baseLEDATA) > MAX_LEDATA_THRESHOLD ) {
        GObjFlushLEDATA();
    }
    fix = FMemAlloc( sizeof( my_fixup ) );
    fix->next = *curFixups;
    *curFixups = fix;
    ++(*numFixups);
    return( fix );
}


#if _TARGET == _80386

void GObjOutPtr( obj_ptr value ) {
//================================

    my_fixup    *fix;

    fix = newFixup();
    fix->idx = 0;
    fix->value.offset = value;
    OutU32( (unsigned_32)fix );
}

#endif


#if _TARGET == _8086

void GObjTargAddr( unsigned_32 value ) {
//======================================

    my_fixup    PGM *fix;

    fix = ( my_fixup PGM * )newFixup();
    fix->idx = 0;
    fix->value.offset = value;
    fix->value.seg = CurrSeg;
    OutU32( ( unsigned_32 )fix );
}

#endif


void GObjEnable( void ) {
//=======================

// generate relocation to F77 enable routine

    my_fixup    *fix;

    fix = newFixup();
    fix->idx = -EXTDEF_EXT_TO_F77;
    fix->value.offset = ObjPtr;
#if _TARGET == _8086
    fix->value.seg = CurrSeg;
#endif
    OutU32( 0 );
}


void GOFixSetData( obj_ptr *le_base, my_fixup **list, uint *num ) {
//=================================================================

        baseLEDATA = le_base;
        curFixups = list;
        numFixups = num;
}
