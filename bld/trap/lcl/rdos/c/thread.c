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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdrdos.h"
#include "rdos.h"
#include "debug.h"

#define THD_WAIT        2
#define THD_SIGNAL      3
#define THD_KEYBOARD    4
#define THD_BLOCKED     5
#define THD_RUN         6
#define THD_DEBUG       7

unsigned ReqThread_freeze( void )
{
    thread_freeze_req   *acc;
    thread_freeze_ret   *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqThread_thaw( void )
{
    thread_thaw_req *acc;
    thread_thaw_ret *ret;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

    ret->err = 0;
    return( sizeof( *ret ) );
}

unsigned ReqThread_set( void )
{
    thread_set_req      *acc;
    thread_set_ret      *ret;
    struct TDebug       *obj;
    struct TDebugThread *t = 0;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

    ret->old_thread = 0;
    ret->err = 0;

    obj = GetCurrentDebug();
	if (obj)
	    t = obj->CurrentThread;

	if( t )
	    ret->old_thread = t->ThreadID;

    if( obj && acc->thread != 0 )
        SetCurrentThread( obj, acc->thread );

    return( sizeof( *ret ) );
}

unsigned ReqThread_get_next( void )
{
    thread_get_next_req *acc;
    thread_get_next_ret *ret;
    struct TDebug       *obj;
    int                 id;
    char                list = THD_BLOCKED;
    int                 ok;
    int                 i;
    struct ThreadState  state;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->thread = 0;
    ret->state = THREAD_THAWED;

    obj = GetCurrentDebug();
	if (obj)
        id = GetNextThread( obj, acc->thread );
    else
        id = 0;

    if( id ) {
        ok = FALSE;    
        for ( i = 0; i < 256 && !ok; i++ ) {
            RdosGetThreadState(i, &state);
            if (state.ID == id)
                ok = TRUE;
        }
        if (ok) {
            if (strstr(state.List, "Ready"))
                list = THD_RUN;

            if (strstr(state.List, "Run"))
                list = THD_RUN;
                
            if (strstr(state.List, "Debug"))
                list = THD_DEBUG;
                
            if (strstr(state.List, "Wait"))
                list = THD_WAIT;

            if (strstr(state.List, "Signal"))
                list = THD_SIGNAL;

            if (strstr(state.List, "Keyboard"))
                list = THD_KEYBOARD;
        }
    }    
    ret->thread = list;

    return( sizeof( *ret ) );
}

unsigned ReqThread_get_extra( void )
{
    thread_get_extra_req    *acc;
    char                    *name;
    struct TDebug           *obj;
    struct TDebugThread     *t = 0;

    acc = GetInPtr( 0 );
    name = GetOutPtr( 0 );
    strcpy( name, "" );

    if( acc->thread == 0 )
        strcpy( name, "Name" );
    else {
        obj = GetCurrentDebug();
	    if (obj) {
            t = LockThread( obj, acc->thread );

            if( t )
                strcpy( name, t->ThreadName );

            UnlockThread( obj );
        }
    }

    return( strlen( name ) + 1 );
}
