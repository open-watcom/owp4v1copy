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
// CPTRAPS      : compile-time exception handling
//

#include "ftnstd.h"
#include "errcod.h"
#include "xfflags.h"
#include "_handler.h"

#include <stddef.h>
#include <signal.h>
#include <float.h>

extern  void            Warning(int,...);


static void _FPEHandler( int fpe_type ) {
//=======================================

    if( fpe_type == FPE_OVERFLOW ) {
        Warning( KO_FOVERFLOW );
    } else if( fpe_type == FPE_UNDERFLOW ) {
        Warning( KO_FUNDERFLOW );
    } else if( fpe_type == FPE_ZERODIVIDE ) {
        Warning( KO_FDIV_ZERO );
    }
}


#if _OPSYS == _OSI

extern void (*__FPE_handler)(int);

#else

static  void    FPEHandler( int sig_num, int fpe_type ) {
//=======================================================

    // reset the signal so we can get subsequent signals
    signal( SIGFPE, (void (*)(int))&FPEHandler );
#if ( _OPT_CG == _OFF ) && ( _8087 == _ON )
    // we do the _control87 so that we can be sure we get the signals we want
    _control87( 0, EM_ZERODIVIDE | EM_OVERFLOW | EM_UNDERFLOW );
#endif
    sig_num = sig_num;
    _FPEHandler( fpe_type );
}

#endif

#if _OPT_CG == _OFF

static volatile bool BreakFlag;


static  void    CtrlBreak() {
//===========================

// Compile-time Ctrl/Break handler.

    signal( SIGINT, CtrlBreak );
    BreakFlag = TRUE;
}


bool    TBreak() {
//================

// Check if Ctrl/Break has been hit.

    bool        break_flag;

    break_flag = BreakFlag;
    BreakFlag = FALSE;
    return( break_flag );
}

#endif


void    TrapInit() {
//==================

    __XcptFlags = 0;
#if _OPSYS == _OSI
    __FPE_handler = &_FPEHandler;
#else
  #if _OPT_CG == _OFF
    signal( SIGINT, CtrlBreak );
  #endif
    signal( SIGFPE, (void (*)(int))&FPEHandler );
  #if ( _OPT_CG == _OFF ) && ( _8087 == _ON )
    // we do the _control87 so that we can be sure we get the signals we want
    _control87( 0, EM_ZERODIVIDE | EM_OVERFLOW | EM_UNDERFLOW );
  #endif
#endif
}


void    TrapFini() {
//==================

}
