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


#include "variety.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <malloc.h>
#include <errno.h>
#include "liballoc.h"
#include "ntex.h"
#include "stacklow.h"
#include "sigtab.h"
#include "thread.h"
#include "trdlist.h"
#include "mthread.h"
#include "rtdata.h"
#include "extfunc.h"
#include "seterrno.h"
#include "widechar.h"
#include "initarg.h"

extern  void            __InitMultipleThread( void );

extern  DWORD           __TlsIndex;

extern  void            (*__sig_init_rtn)(void);
extern  void            (*__sig_fini_rtn)(void);

extern  void            _endthread(void);

typedef struct thread_args {
    thread_fn   *rtn;
    void        *argument;
    HANDLE      parent;
    HANDLE      thread_handle;
} thread_args;

static DWORD WINAPI begin_thread_helper( thread_args *td )
/***************************************************/
{
    thread_fn           *rtn;
    void                *arg;
    REGISTRATION_RECORD rr;
    thread_data         *tdata;

    rtn = td->rtn;
    arg = td->argument;
    __THREADDATAPTR->thread_handle = td->thread_handle;
    free( td );

    // For DLLs, __NTAddThread has already been called from _LibMain
    // in DLL_THREAD_ATTACH processing.
    if( !__Is_DLL ) {                                   /* 15-feb-93 */
        if (NULL==(tdata = (thread_data *)alloca( __ThreadDataSize ))) {
            CloseHandle(__THREADDATAPTR->thread_handle);
            return( 0 );
        }
        memset( tdata, 0, __ThreadDataSize );
        // tdata->__allocated = 0;
        tdata->__data_size = __ThreadDataSize;
        if( !__NTAddThread( tdata ) ) {
            CloseHandle(__THREADDATAPTR->thread_handle);
            return( 0 );
        }
    }

    __NewExceptionFilter( &rr );
    __sig_init_rtn(); // fills in a thread-specific copy of signal table
    (*rtn)( arg );
    _endthread();
     return( 0 );
}

unsigned long __CBeginThread(thread_fn *start_addr, unsigned stack_size,
                     void *arglist)
{
    DWORD       tid;
    thread_args *td;
    HANDLE      th;

    if( __TlsIndex == NO_INDEX ) {
        if( __NTThreadInit() == FALSE ) return( -1L );
        __InitMultipleThread();
    }

    td = malloc( sizeof( *td ) );
    if( td == NULL ) {
        __set_errno( ENOMEM );
        return( -1L );
    }

    stack_size = __Align4K( stack_size );

    td->rtn = start_addr;
    td->argument = arglist;
    td->parent = GetCurrentThread();

    th = CreateThread( NULL, stack_size, (LPTHREAD_START_ROUTINE)&begin_thread_helper,
                (LPVOID) td, CREATE_SUSPENDED, &tid );
    if( th ) {
        td->thread_handle = th;
        ResumeThread( th );
    } else {
        // we didn't create the thread so it isn't going to free this
        free( td );
        th = (HANDLE)-1L;
    }
    return( (unsigned long)th );
}

void __CEndThread( void )
{
    __sig_fini_rtn();
    __DoneExceptionFilter();
    // For DLLs, __NTRemoveThread will be called from _LibMain
    // in DLL_THREAD_DETACH processing.
    if( ! __Is_DLL ) {
        __NTRemoveThread( TRUE );
    }
    ExitThread( 0 );
}
