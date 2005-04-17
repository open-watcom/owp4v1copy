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
* Description:  Run-time exception handling.
*
****************************************************************************/


#include "ftnstd.h"
#include "errcod.h"
#include "xfflags.h"
#include "rundat.h"

#include <signal.h>
#if _OPSYS == _PCDOS
  #include <dos.h>
  #include "extender.h"
#elif !defined( __AXP__ ) && !defined( __PPC__ )
  #include <i86.h>
#endif

#if ( _OPSYS == _OS2 )
  #if defined( __386__ )
    #define __OS2_386__
  #else
    #define INCL_DOSMISC
    #include <os2.h>
    #define __OS2_286__
  #endif
#endif

#if defined( __NT__ )
  #include "enterdb.h"
#endif

#if defined( __OS2_286__ )
  #define       _handler        interrupt __pascal __far
#elif _OPSYS == _PCDOS
  #define       _handler        interrupt
#else
  #define       _handler
#endif

extern void             RTErr(int,...);
extern void             FPTrapInit(void);
extern void             FPTrapFini(void);

typedef void            (*fsig_func)(intstar4);

#if defined( __OS2_286__ ) || ( _OPSYS == _PCDOS ) && !defined( __WINDOWS_386__ )
  extern byte           IntOverFlow;
         void           (* __UserIOvFlHandler)(intstar4) = { (fsig_func)SIG_DFL };
  extern byte           IntDivBy0;
         void           (* __UserIDivZHandler)(intstar4) = { (fsig_func)SIG_DFL };
  static void           (_handler *ISave)();
  static void           (_handler *ZSave)();
#endif

#if (_OPSYS == _PCDOS) && !defined( __WINDOWS__ )
  extern _WCRTLINK int  __NonIBM;       // 0 if IBM, non-zero if NEC
  extern byte           BreakVector;
         void           (* __UserBreakHandler)(intstar4) = { (fsig_func)SIG_DFL };
  static void           (_handler *CBSave)();
 #if defined( __386__ )
  static unsigned long  CBRealSave;
 #endif
#endif

#if defined( __OS2_386__ ) || defined( __NT__ )
  extern byte           __ExceptionHandled;
  static void           IOvFlSignal(int);
#endif

#if (_OPSYS == _PCDOS) && defined( __386__ )
  extern unsigned_32    _STACKLOW;

  void _movestack( unsigned_32 );
  #pragma aux _movestack = \
        "push ds"            \
        "pop  ss"             \
        "mov  esp,eax"        \
        parm [eax] modify [esp];

  extern unsigned long _dos_getrealvect(int);
  extern void          _dos_setrealvect(int,unsigned long);
  extern void          _dos_setvectp(int,void (interrupt far *)());
#endif


#if !defined( __WINDOWS__ )

static  void    ProcessBreak() {
//==============================

    __XcptFlags &= ~XF_ERR_MASK;
    __XcptFlags |= XF_LIMIT_ERR | XF_KO_INTERRUPT;
    if( __XcptFlags & XF_IO_INTERRUPTABLE ) {
        __XcptFlags |= XF_IO_INTERRUPTED;
 #if defined( __OS2_386__ ) || defined( __NT__ )
    } else {
        __ExceptionHandled = 0;
 #endif
    }
}


static  void    BreakSignal( int sig ) {
//===================================================

    sig = sig;
    signal( SIGINT, BreakSignal );
 #if ( _OPSYS == _OS2 ) || defined( __NT__ )
    signal( SIGBREAK, BreakSignal );
 #endif
    ProcessBreak();
}

 #if ( _OPSYS == _PCDOS )

static  void    _handler BreakHandler() {
//=======================================

    _enable();
    if( __UserBreakHandler != (fsig_func)SIG_DFL ) {
        if( (__UserBreakHandler != (fsig_func)SIG_IGN) &&
            (__UserBreakHandler != (fsig_func)SIG_ERR) ) {
            __UserBreakHandler( SIGBREAK );
        }
    } else {
        ProcessBreak();
    }
}
 #endif
#endif


#if !defined( __WINDOWS_386__ )

 #if ( _OPSYS == _PCDOS ) || ( _OPSYS == _OS2 ) || defined( __NT__ )

static  void    ProcessIDivZ() {
//==============================

    RTErr( KO_IDIV_ZERO );
}
 #endif

 #if defined( __OS2_386__ ) || defined( __NT__ )

static  void    IDivZSignal( int sig ) {
//===================================================

    sig = sig;
    ProcessIDivZ();
}

 #elif ( _OPSYS == _PCDOS ) || defined( __OS2_286__ )

static  void    _handler IDivZHandler() {
//=======================================

  #if _OPSYS == _PCDOS
    _enable();
   #ifdef __386__
    // Under pharlap, ss != ds when this interrupt is taken.
    // But in order for our error reporting code to work we have to
    // set ss = ds.
    _movestack( _STACKLOW+512 );
   #endif
  #endif
    if( __UserIDivZHandler != (fsig_func)SIG_DFL ) {
        if( (__UserIDivZHandler != (fsig_func)SIG_IGN) &&
            (__UserIDivZHandler != (fsig_func)SIG_ERR) ) {
            __UserIDivZHandler(SIGIDIVZ);
        }
    } else {
        ProcessIDivZ();
    }
}
 #endif

 #if ( _OPSYS == _PCDOS ) || ( _OPSYS == _OS2 ) || defined( __NT__ )

static  void    ProcessIOvFl() {
//==============================

    // Set flag so that we report an overflow when we read an integer
    // regardless of whether user wants integer overflows reported
    __XcptFlags |= XF_IOVERFLOW;
}
 #endif

 #if defined( __OS2_386__ ) || defined( __NT__ )

static  void    IOvFlSignal( int sig ) {
//===================================================

    sig = sig;
    // re-signal exception since we may get many overflows
    // before we actually report the condition
    signal( SIGIOVFL, IOvFlSignal );
    ProcessIOvFl();
}

 #elif ( _OPSYS == _PCDOS ) || defined( __OS2_286__ )

static  void    _handler IOvFlHandler() {
//=======================================

  #if _OPSYS == _PCDOS
    _enable();
  #endif
    if( __UserIOvFlHandler != (fsig_func)SIG_DFL ) {
        if( (__UserIOvFlHandler != (fsig_func)SIG_IGN) &&
            (__UserIOvFlHandler != (fsig_func)SIG_ERR) ) {
            __UserIOvFlHandler(SIGIOVFL);
        }
    } else {
        ProcessIOvFl();
    }
}
 #endif
#endif

static  void    AbnormalTerm() {
//==============================

    RTErr( CP_TERMINATE );
}

int     __EnableF77RTExceptionHandling( void ) {
//==============================================

#if defined( __NT__ )
    return( !DebuggerPresent() );
#else
    return( 1 );
#endif
}

void    R_TrapInit() {
//====================

    int enable_excpt;

    enable_excpt = __EnableF77RTExceptionHandling();

    if( enable_excpt ) {
        signal( SIGSEGV, AbnormalTerm );
        signal( SIGILL, AbnormalTerm );
    }
#if !defined( __WINDOWS__ )
    signal( SIGINT, BreakSignal );
#endif
#if _OPSYS == _PCDOS
 #if !defined( __WINDOWS__ )
    if( __NonIBM ) {
        // Assume NEC
        BreakVector = 0x06;
    }
    CBSave = _dos_getvect( BreakVector );
    _dos_setvect( BreakVector, (void (interrupt far *)(void))(void (near *)(void))BreakHandler );
  #if defined( __386__ )
    if( _IsPharLap() ) {
        CBRealSave = _dos_getrealvect( BreakVector );
        _dos_setvectp( BreakVector, (void (interrupt far *)(void))(void (near *)(void))BreakHandler );
    }
  #endif
 #endif
 #if !defined( __WINDOWS_386__ )
    ISave = _dos_getvect( IntOverFlow );
    _dos_setvect( IntOverFlow, (void (interrupt far *)(void))(void (near *)(void))IOvFlHandler );
    ZSave = _dos_getvect( IntDivBy0 );
    _dos_setvect( IntDivBy0, (void (interrupt far *)(void))(void (near *)(void))IDivZHandler );
 #endif
#elif defined( __OS2_386__ ) || defined( __NT__ )
    signal( SIGBREAK, BreakSignal );
    if( enable_excpt ) {
        signal( SIGIDIVZ, IDivZSignal );
        signal( SIGIOVFL, IOvFlSignal );
    }
#elif defined( __OS2_286__ )
    signal( SIGBREAK, BreakSignal );
    DosSetVec( IntOverFlow, (PFN)IOvFlHandler, (PFN FAR *)&ISave );
    DosSetVec( IntDivBy0, (PFN)IDivZHandler, (PFN FAR *)&ZSave );
#endif
    FPTrapInit();
}

void    R_TrapFini() {
//====================

#if _OPSYS == _PCDOS
 #if !defined( __WINDOWS__ )
    _dos_setvect( BreakVector, CBSave );
  #if defined( __386__ )
    if( _IsPharLap() ) {
        _dos_setrealvect( BreakVector, CBRealSave );
    }
  #endif
 #endif
 #if !defined( __WINDOWS_386__ )
    _dos_setvect( IntOverFlow, ISave );
    _dos_setvect( IntDivBy0, ZSave );
 #endif
#elif defined( __OS2_286__ )
    DosSetVec( IntOverFlow, (PFN)ISave, (PFN FAR *)&ISave );
    DosSetVec( IntDivBy0, (PFN)ZSave, (PFN FAR *)&ZSave );
#endif
    FPTrapFini();
}

extern void             (*_ExceptionInit)();
extern void             (*_ExceptionFini)();

void    __InitExceptionVectors() {
//================================

    _ExceptionInit = &R_TrapInit;
    _ExceptionFini = &R_TrapFini;
}
