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
* Description:  Remote async services access.
*
****************************************************************************/


#include <string.h>
#include "dbgdefn.h"
#include "trpcore.h"
#include "trpasync.h"
#include "dbgio.h"

extern trap_shandle GetSuppId( char * );

#define SUPP_ASYNC_SERVICE( in, request )        \
        in.supp.core_req    = REQ_PERFORM_SUPPLEMENTARY_SERVICE;        \
        in.supp.id                  = SuppAsyncId;       \
        in.req                      = request;

static trap_shandle     SuppAsyncId;

bool InitAsyncSupp( void )
{
    SuppAsyncId = GetSuppId( ASYNC_SUPP_NAME );
    if( SuppAsyncId == 0 ) return( FALSE );
    return( TRUE );
}

bool HaveRemoteAsync( void )
{
    return( SuppAsyncId != 0 );
}
