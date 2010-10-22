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
* Description:  Interface for non-blocking thread services
*
****************************************************************************/


#include "dbgdefn.h"
#include "dbgreg.h"
#include "trpcore.h"
#include "trprtrd.h"
#include "dbgio.h"

extern trap_shandle     GetSuppId( char * );

static trap_shandle     SuppRunThreadId;

#define SUPP_THREAD_SERVICE( in, request )      \
        in.supp.core_req        = REQ_PERFORM_SUPPLEMENTARY_SERVICE;    \
        in.supp.id              = SuppRunThreadId; \
        in.req                  = request;


bool InitRunThreadSupp( void )
{
    SuppRunThreadId = GetSuppId( RUN_THREAD_SUPP_NAME );
    if( SuppRunThreadId == 0 ) return( FALSE );
    return( TRUE );
}

bool HaveRemoteRunThread( void )
{
     /* only available on selected hosts for now */
#if defined( __NT__ ) && defined( __GUI__ )
    return( SuppRunThreadId != 0 );
#elif defined( __RDOS__ )
    return( SuppRunThreadId != 0 );
#else
    return( FALSE );
#endif
}
