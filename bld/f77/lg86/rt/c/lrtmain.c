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
// LRTMAIN      : load and go main() for stand alone library
//

#include "ftnstd.h"
#include "rundat.h"
#include "fcboth.h"
#include "undef.h"
#include <string.h>
#include "global.h"
#include "bglobal.h"
#include "fcenv.h"
#include "cpopt.h"
#include "saopt.h"

extern  int             RunMain(comp_options);
#if _8087 == _ON
extern  void            InitMath(void);
extern  void            FiniMath(void);
extern  bool            Chk8087(void);
#endif

#if _TARGET == _8086
  #define       SEG_SIZE                0x10000L
  #define       DUAL_memset(x,y,z)      huge_memset(x,y,z)
  #define       DUAL_calcTotal(x,y)     calcTotal(x,y)
#else
  #define       DUAL_memset(x,y,z)      memset(x,y,z)
  #define       DUAL_calcTotal(x,y)     ( y - x )
#endif

extern  char    PGM     _wfdata_start;
extern  char    PGM     _wfdata_end;

extern  cb_defn         _cb_defn_start;
extern  cb_defn         _cb_defn_end;

#pragma aux (lg_sym)    OPTIONS;
extern  sa_options PGM  OPTIONS;

int __fcodes_() {
//===============

// this is the hook that pulls in our library
// we do it like this so that it gets merged with main and takes no storage

    return( main() );
}

#if _TARGET == _8086
static char *justify( char * ptr ) {
//==================================
// Justifies a ptr
    unsigned_16         offset = FP_OFF( ptr );
    unsigned_16         seg    = FP_SEG( ptr );

    seg += offset >> 4;
    offset &= 0xf;
    return( MK_FP( seg, offset ) );
}

static void huge_memset( char *start, int ch, unsigned_32 total ) {
//=================================================================
// This does what memset cannot, cross segment initialization

    char        *p = justify( start );

    while( total >= 0x10000L ) {
        memset( p, ch, 0xffff );        // Do the first 65535 chars
        *( p + 0xffff ) = ch;           // do the last one
        p = MK_FP( ( FP_SEG( p ) + 0x1000L ), 0 ); // increment by a seg
        total -= 0x10000L;
    }
    memset( p , ch, total );            // do the last of total
}

static unsigned_32 calcTotal( char *p, char *q ) {
//=================================================
// Calculates a total size given 2 far pointers

    unsigned_32         start;
    unsigned_32         end;

    // create 2 20 bit longs and subtract the 2
    start = FP_SEG( p );
    start <<= 4;
    start += FP_OFF( p );
    end = FP_SEG( q );
    end <<= 4;
    end += FP_OFF( q );
    return( end - start );
}
#endif

int main() {
//==========

    cb_defn     *cur;

#if _8087 == _ON
    if( Chk8087() ) {
        InitMath();
#endif
        // initialize the fcode global data areas to UNDEF_CHAR
        DUAL_memset( &_wfdata_start, UNDEF_CHAR,
                        DUAL_calcTotal( &_wfdata_start, &_wfdata_end ) );
        // initialize the common blocks to UNDEF_CHAR
        cur = &_cb_defn_start;
        while( cur < &_cb_defn_end ) {
            // If a common block appears in two watfor .obj files there
            // will be two cb_defns for it.  So we check to save some time.
            // We check the last byte instead of the first byte in case the
            // common block has been defined with different sizes in the
            // different files.
            if( cur->origin[cur->size - 1] != UNDEF_CHAR ) {
                DUAL_memset( cur->origin, UNDEF_CHAR, cur->size );
            }
            ++cur;
        }
        MathOptions = OPTIONS.math_options;
        StmtLimit = OPTIONS.stmt_limit;
        StmtRem = StmtLimit - 1;
        PageLimit = OPTIONS.page_limit;
        TotTime = OPTIONS.time_limit;
        RunMain( OPTIONS.options );
#if _8087 == _ON
        FiniMath();
    }
#endif
    return( 0 );
}
