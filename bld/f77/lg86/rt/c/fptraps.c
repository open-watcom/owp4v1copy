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
// FPTRAPS      : floating-point exception handling
//

#include "ftnstd.h"
#include "rundat.h"
#include "xfflags.h"
#include "mathopt.h"
#include "fcboth.h"

#include <signal.h>
#include <float.h>

#define MASKED_EXCEPTIONS ( EM_ZERODIVIDE | EM_UNDERFLOW | EM_PRECISION | \
                            EM_DENORMAL | EM_OVERFLOW )

#if defined( __386__ ) && ( _OPSYS == _OS2 )
  extern        byte    __ExceptionHandled;
#endif


static void set87() {
//===================

// Set the control bits to get all the exceptions we want.

    int         bits;

    bits = ~0 & ~EM_ZERODIVIDE;
    if( MathOptions & MATH_FLT_UNDER ) bits &= ~EM_UNDERFLOW;
    if( MathOptions & MATH_FLT_OVER ) bits &= ~EM_OVERFLOW;
    _control87( bits, MASKED_EXCEPTIONS );
}


static  void    FPEHandler( int sig_num, int xcpt ) {
//===================================================

    short       flags;

    // make sure that we are not processing an exception
    // (i.e. avoid cascading exceptions)
    if( __XcptFlags & XF_LIMIT_ERR ) return;
    sig_num = sig_num;
    signal( SIGFPE, (void (*)(int))FPEHandler );
    set87();
    flags = 0;
    if( (xcpt == FPE_STACKOVERFLOW) || (xcpt == FPE_STACKUNDERFLOW) ) {
        flags = XF_CP_TERMINATE;
    } else if( xcpt == FPE_OVERFLOW ) {
        if( !( MathOptions & MATH_FLT_OVER ) ) return;
        flags = XF_KO_FOVERFLOW;
    } else if( xcpt == FPE_UNDERFLOW ) {
        if( !( MathOptions & MATH_FLT_UNDER ) ) return;
        flags = XF_KO_FUNDERFLOW;
    } else if( xcpt == FPE_ZERODIVIDE ) {
        flags = XF_KO_FDIV_ZERO;
    } else if( xcpt == FPE_SQRTNEG ) {
        flags = XF_LI_ARG_NEG;
    } else if( xcpt == FPE_IOVERFLOW ) {
        flags = XF_KO_IOVERFLOW;
#if defined( __386__ ) && ( _OPSYS == _OS2 )
    } else {
        __ExceptionHandled = 0;
#endif
    }
    if( flags ) {
        __XcptFlags &= ~XF_ERR_MASK;
        __XcptFlags |= XF_LIMIT_ERR | flags;
    }
}


void    FPTrapInit() {
//====================

    signal( SIGFPE, (void (*)(int))FPEHandler );
    set87();
}


void    FPTrapFini() {
//====================

}
