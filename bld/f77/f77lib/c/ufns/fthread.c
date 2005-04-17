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
// FTHREAD      : thread proccessing functions
//

#include "ftnstd.h"
#include "rundat.h"
#include "xfflags.h"
#include "trcback.h"
#include "fthread.h"

#include <process.h>
#include <stdlib.h>
#include <stddef.h>

extern  int             Spawn(void (*)());
extern  void            Suicide(void);
extern  void            R_TrapInit(void);
extern  void            R_TrapFini(void);
extern  unsigned        RTSysInit(void);
extern  void            *RMemAlloc(unsigned);
extern  void            RMemFree(void *);
extern  void            __InitFThreadData(fthread_data *);
extern  unsigned        __InitFThreadProcessing(void);
extern  void            __FiniFThreadProcessing(void);

typedef struct {
    void        (*rtn)();
    void        *arglist;
} thread_info;

static  int             (*__BeginThread)();
static  void            (*__EndThread)();
static  int             (*__InitDataThread)();
static  bool            ThreadsInitialized;


static  unsigned  InitFThreads() {
//================================

    if( ThreadsInitialized ) return( 0 );
    if( __InitFThreadProcessing() != 0 ) return( 1 );
    ThreadsInitialized = TRUE;
    RTSysInit();
    return( 0 );
}


static  void    FiniFThreads() {
//==============================

    if( ThreadsInitialized ) {
        __FiniFThreadProcessing();
    }
}


static void     FThreadInit() {
//=============================

    R_TrapInit();
}


static void     FThreadFini() {
//=============================

    R_TrapFini();
}


static  void    ThreadStarter() {
//===============================

    __FTHREADDATAPTR->__rtn( __FTHREADDATAPTR->__arglist );
}


static  void    ThreadHelper( thread_info *ti ) {
//===============================================

    FThreadInit();
    __FTHREADDATAPTR->__rtn = ti->rtn;
    __FTHREADDATAPTR->__arglist = ti->arglist;
    RMemFree( ti );
    Spawn( ThreadStarter );
    FThreadFini();
    __EndThread();
}


#if defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
  #ifdef __NT__
int FBeginThread( void (*rtn)(void *), unsigned stk_size, void *arglist ) {
//=========================================================================
  #else
int FBeginThread( void (*rtn)(void *), void *stack, unsigned stk_size, void *arglist ) {
//======================================================================================
  #endif
#else
int FBeginThread( void (*rtn)(void *), void *stack, unsigned stk_size, void *arglist ) {
//======================================================================================
#endif

    thread_info *ti;

    if( InitFThreads() != 0 ) return( -1 );
    ti = RMemAlloc( sizeof( thread_info ) );
    if( ti == NULL ) return( -1 );
    ti->rtn = rtn;
    ti->arglist = arglist;

#if defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
  #ifdef __NT__
    return( __BeginThread( ThreadHelper, stk_size, ti ) );
  #else
    return( __BeginThread( ThreadHelper, stack, stk_size, ti ) );
  #endif
#else
    return( __BeginThread( ThreadHelper, stack, stk_size, ti ) );
#endif
}


void    FEndThread() {
//====================

    Suicide();
}


int     FInitDataThread( void *td ) {
//===================================

    __InitFThreadData( (fthread_data *)((char *)td + __FThreadDataOffset) );
    return( __InitDataThread( td ) );
}


// User-callable thread functions:
// -------------------------------

int     fortran BEGINTHREAD( void (*rtn)(void *), unsigned long *stk_size ) {
//===========================================================================

#if defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
  #ifdef __NT__
    return( (int)_beginthread( rtn, *stk_size, NULL ) );
  #else
    return( _beginthread( rtn, NULL, *stk_size, NULL ) );
  #endif
#else
    return( _beginthread( rtn, NULL, *stk_size, NULL ) );
#endif

}


void    fortran ENDTHREAD() {
//===========================

    _endthread();
}


unsigned        fortran THREADID() {
//==================================

    return( *__threadid() );
}


// Initializer/finalizer for thread processing:
// --------------------------------------------


void    __FiniBeginThread() {
//===========================

    FiniFThreads();
}

#pragma off (check_stack)
void    __InitBeginThread() {
//===========================

    __BeginThread = &FBeginThread;
    __EndThread = &FEndThread;
    __InitDataThread = &FInitDataThread;
    __RegisterThreadData( &__BeginThread, &__EndThread, &__InitDataThread );
    ThreadsInitialized = FALSE;
}
