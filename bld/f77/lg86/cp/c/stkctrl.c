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
// STKCTRL      : 80x87 stack control
//

#include "ftnstd.h"
#include "fcodes.h"
#include "optr.h"
#include "symflgs.h"
#include "objutil.h"

#if _8087 == _ON

extern  void            EmitOp(unsigned_16);
extern  void            CompErr(char *);

#define KEEP_DEPTH      0
#define MAX_DEPTH       8

static  uint            StkDepth;

//                                        L1 L  I1 I2 I
static  const byte __FAR StkEntries[] = { 0, 0, 0, 0, 0,
//                                        R  D  E  C  Q  X
                                          1, 1, 1, 2, 2, 2,
//                                        char struct
                                          0,   0 };


static  const byte __FAR ExpStk[NUM_TYPES][NUM_TYPES] = {
    //  I   R   D   E   C   Q   X   left /
    //                                 / right
    {   0,  7,  7,  7,  6,  6,  6   },  //      I
    {   7,  6,  6,  6,  5,  5,  5   },  //      R
    {   7,  6,  6,  6,  5,  5,  5   },  //      D
    {   7,  6,  6,  6,  5,  5,  5   },  //      E
    {   6,  5,  5,  5,  4,  4,  4   },  //      C
    {   6,  5,  5,  5,  4,  4,  4   },  //      Q
    {   6,  5,  5,  5,  4,  4,  4   }   //      X
};


static  void    NeedTmps( uint tmps ) {
//=====================================

    if( StkDepth + tmps > MAX_DEPTH ) {
        EmitOp( DUMP_ENTRIES );
        tmps -= MAX_DEPTH - StkDepth;
        StkDepth -= tmps;
        OutU16( (StkDepth << 8) + tmps );
    }
}


extern  void    ReqStkTemps( uint op, uint typ1, uint typ2 ) {
//============================================================

// note that typ1 & typ2 are 0 based (i.e., TY_INTEGER has been
// subtracted)

    switch( op ) {
    case OPTR_MUL:
        if( typ1 >= ( TY_COMPLEX - TY_INTEGER ) &&
            typ2 >= ( TY_COMPLEX - TY_INTEGER ) ) {
            // CC, CQ, QC, and QQ require a temp
            NeedTmps( 1 );
        }
        break;
    case OPTR_DIV:
        if( typ2 >= ( TY_COMPLEX - TY_INTEGER ) ) {
            if( typ1 == ( TY_INTEGER - TY_INTEGER ) ) {
                // int / cplx requires 3 tmps
                NeedTmps( 3 );
            } else {
                // division of a real/cplx by a cplx requires 2 tmps
                NeedTmps( 2 );
            }
        }
        break;
    case OPTR_EXP:
        NeedTmps( ExpStk[ typ1 ][ typ2 ] );
        break;
    }
}


extern  void    InitStkDepth() {
//==============================

    StkDepth = 0;
}


extern  uint    GetStkEntries( uint typ ) {
//=========================================

    return( StkEntries[ typ ] );
}


extern  void    ModStkDepth( int entries ) {
//==========================================

    StkDepth += entries;
    if( entries >= 0 ) {
        if( StkDepth > MAX_DEPTH ) {
            EmitOp( DUMP_ENTRIES );
            OutU16( (KEEP_DEPTH << 8) + (StkDepth-entries-KEEP_DEPTH) );
            StkDepth = entries + KEEP_DEPTH;
        }
    }
}


extern  void    IncStkDepth( uint typ ) {
//=======================================

    ModStkDepth( GetStkEntries( typ ) );
}


extern  void    DecStkDepth( uint typ ) {
//=======================================

    ModStkDepth( -GetStkEntries( typ ) );
}


extern  void    FlushStk() {
//==========================

    if( StkDepth > 0 ) {
        EmitOp( FLUSH_ENTRIES );
        OutU16( StkDepth );
        StkDepth = 0;
    }
}


extern  void    ReqStkDepth( uint depth ) {
//=========================================

    if( depth > StkDepth ) {
        EmitOp( LOAD_ENTRIES );
        OutU16( ((depth-StkDepth) << 8) + StkDepth );
        StkDepth = depth;
    }
}


#else
#pragma off (unreferenced);
extern  void    ReqStkTemps( uint op, uint typ1, uint typ2 ) {
//============================================================

}


extern  void    InitStkDepth() {
//==============================

}


extern  uint    GetStkEntries( uint typ ) {
//=========================================

    return( 0 );
}


extern  void    IncStkDepth( uint typ ) {
//=======================================

}


extern  void    DecStkDepth( uint typ ) {
//=======================================

}


extern  void    ModStkDepth( int entries ) {
//==========================================

}


extern  void    FlushStk() {
//==========================

}


extern  void    ReqStkDepth( uint depth ) {
//=========================================

}
#pragma on (unreferenced);
#endif
