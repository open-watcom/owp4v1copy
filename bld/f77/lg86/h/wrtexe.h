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
// WRTEXE.H     : write EXE file definitions (used by wrtexe and wrtexef)
//

// Modified:    By:             Reason:
// ---------    ---             -------
// 91/05/21     D.J.Gaudet      Initial Implementation
// 91/06/18     D.J.Gaudet      was setting cx 1 word too small in SALoader
// 91/07/28     D.J.Gaudet      split into two parts for overlaying

#include <i86.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <ftnstd.h>
#include <global.h>
#include <fcgbls.h>
#define hash_entry lib_hash_entry
#include <pcobj.h>
#include <tinyio.h>

typedef targ_addr dos_addr;
#include "ovltab.h"             // from WLINK
#include "exedos.h"             // from WLINK

/*
    This implementation relies on the behaviour of the current overlay loader.
    It also makes assumptions about what is in each section and that all
    overlays are stored in the same .exe file.  Thus it just uses the
    overlay loader's variable __OVLHDLPREV__ to access the exe.

    EXESORT <file> /o /w must have been performed on the compiler exe.
    This code assumes that the relocation tables are sorted in
    ascending order.  Also, the magic cookie that EXESORT places in
    the chk_sum field of the exe header to indicate the number of root
    relocations that don't belong to the overlay loader must be present.
*/

// The section numbers.  NOTE: These are actually 1 less than the
// section numbers the linker reports in the map file!!
#define TAB_RUN_TIME    _OVLTAB__.entries[ 0 ]
#define TAB_DATA        _OVLTAB__.entries[ 6 ]

/*
    Here's the plan:

    We want the standalone exe to look like this:

        root code
        runtime system code
        saLoader (see below)
        fcodes
        fortran global data area
        far data
        DGROUP

    However at compile time, memory looks like this: (all pointers are
        segment pointers that indicate the first segment of what they
        point to.)

    CP_ROOT_LOW         --> root code
    CP_OVLLDR_LOW       --> overlay loader
    CP_RT_LOW           --> runtime system code (overlaid)
    CP_RT_HIGH          --> padding (the compile time overlay is much
                                larger than the runtime overlay)
    CP_DATA_LOW         --> DGROUP
    CP_DATA_HIGH        --> DGROUP expanded to 64k
    CP_FCODE_LOW        --> fcodes
                            fortran global data area
    CP_FCODE_HIGH       -->

    So a fair bit of shuffling has to be done.
*/

extern  unsigned_16     ObjCode;

/* from the overlay manager */
extern  const ovl_table far _OVLTAB__;

#define LD_PARA         2       /* number of paragraphs for the loader */

#define CP_ROOT_LOW     _OVLTAB__.prolog.delta
#define CP_OVLLDR_LOW   FP_SEG( &_OVLTAB__ )
#define CP_RT_LOW       ( TAB_RUN_TIME.start_para + CP_ROOT_LOW )
#define CP_RT_HIGH      ( CP_RT_LOW + TAB_RUN_TIME.num_paras )
#define CP_DATA_LOW     ( TAB_DATA.start_para + CP_ROOT_LOW )
#define CP_DATA_HIGH    ( TAB_DATA.num_paras + CP_DATA_LOW )
#define CP_FCODE_LOW    ObjCode
#define CP_FCODE_HIGH   CurrSeg

#define SA_ROOT_LOW     0
#define SA_RT_LOW       ( CP_OVLLDR_LOW - CP_ROOT_LOW )
#define SA_LOADER       ( SA_RT_LOW + TAB_RUN_TIME.num_paras )
#define SA_FCODE_LOW    ( SA_LOADER + LD_PARA )
#define SA_FCODE_HIGH   ( SA_FCODE_LOW + ( CP_FCODE_HIGH - CP_FCODE_LOW ) )
#define SA_DATA_LOW     ( SA_FCODE_HIGH )
#define SA_DATA_HIGH    ( SA_DATA_LOW + TAB_DATA.num_paras )

extern  unsigned_16     ExeDoReloc( unsigned_16 );
extern  void            ExeIOError( int );
extern  void            ExeUnlink( void );
