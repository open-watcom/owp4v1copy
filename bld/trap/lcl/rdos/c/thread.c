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
#include "debug.h"

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

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->thread = 0;
    ret->state = THREAD_THAWED;

    obj = GetCurrentDebug();
	if (obj)
        ret->thread = GetNextThread( obj, acc->thread );

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
