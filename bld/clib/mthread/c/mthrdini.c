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
* Description:  Thread data allocation/initialization/freeing. Since this
*               module is always linked in, it should contain functions that
*               are required by other parts of the runtime even for non-MT
*               programs.
*
****************************************************************************/


#include "variety.h"
#include "stacklow.h"
#include <stdio.h>
#include <stdlib.h>
#include "liballoc.h"
#include "thread.h"
#include "trdlist.h"
#include "mthread.h"
#include "rtdata.h"
#include "rtinit.h"
#include "exitwmsg.h"

extern        int             __Is_DLL;

_WCRTLINK unsigned __ThreadDataSize = sizeof( thread_data );

void __InitThreadData( thread_data *tdata )
/*****************************************/
{
    if( tdata != NULL ) {
    tdata->__randnext = 1;
    #if defined( __OS2__ )
        // We want to detect stack overflow before it actually
        // happens; let's have a page in reserve
        if( !__Is_DLL ) {
            tdata->__stklowP = __threadstack() + 4096;
        }
    #elif defined( __NT__ )
        __init_stack_limits( &tdata->__stklowP, 0 );
        tdata->thread_id = GetCurrentThreadId();
    #elif defined( __QNX__ )
        tdata->thread_id = GetCurrentThreadId();
    #endif
    }
}

thread_data *__AllocInitThreadData( thread_data *tdata )
/******************************************************/
{
    if( tdata == NULL ) {
        tdata = lib_malloc( __ThreadDataSize );
        if( tdata != NULL ) {
            tdata->__allocated = 1;
            tdata->__data_size = __ThreadDataSize;
        }
    }
    __InitThreadData( tdata );
    return( tdata );
}

void __FreeInitThreadData( thread_data *tdata )
/******************************************************/
{
    if( tdata != NULL ) {
        if( tdata->__allocated == 1 )
            lib_free( tdata );
    }
}

