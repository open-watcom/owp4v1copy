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
// LGTRAPS      : run-time exception handling
//

#include "ftnstd.h"
#include "errcod.h"
#include "xfflags.h"
#include "rundat.h"
#include "global.h"


#if _OPSYS == _PCDOS
#include <signal.h>
#include "_handler.h"
#include <dos.h>

#define TIMER_RATE              1820
#define TIMER_VECTOR            0x08

static  void                    (_handler *TSave)();


static  void    _handler TickHandler() {
//======================================

    TotTime--;
    if( TotTime == 0 ) {
        __XcptFlags &= ~XF_ERR_MASK;
        __XcptFlags |= XF_LIMIT_ERR | XF_KO_TIME_OUT;
    }
    _chain_intr( TSave );
}


void    R_TrapInitLG() {
//======================

    TSave = _dos_getvect( TIMER_VECTOR );
    TotTime = TIMER_RATE * TotTime / 100;
    if( TotTime != 0 ) {
        _dos_setvect( TIMER_VECTOR,
         (void (interrupt far *)(void))(void (near *)(void))TickHandler );
    }
}


void    R_TrapFiniLG() {
//======================

    _dos_setvect( TIMER_VECTOR, TSave );
}

#elif _OPSYS == _OS2

#define INCL_DOSPROCESS
#include <os2.h>

static  TID     timerThreadID;


static void APIENTRY timerThread( void *parm ) {
//=========================================

    parm = parm;
    while( TotTime ) {
        DosSleep( 1000 );
        --TotTime;
    }
    __XcptFlags &= ~XF_ERR_MASK;
    __XcptFlags |= XF_LIMIT_ERR | XF_KO_TIME_OUT;
    timerThreadID = 0;
}


void    R_TrapInitLG() {
//======================

    timerThreadID = 0;
    if( TotTime != 0 ) {
        DosCreateThread( &timerThreadID, (PFNTHREAD)timerThread, 0, 0, 512 );
    }
}


void    R_TrapFiniLG() {
//======================

    if( timerThreadID != 0 ) {
        DosKillThread( timerThreadID );
    }
}

#else


void    R_TrapInitLG() {
//======================
}


void    R_TrapFiniLG() {
//======================
}


#endif
