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
* Description:  Supplementary non-blocking thread functions
*
****************************************************************************/


#ifndef TRPRTRD_H

#include "trptypes.h"

#pragma pack( push, 1 )

#define RUN_THREAD_SUPP_NAME                "RunThread"

enum {
    REQ_RUN_THREAD_STOP,              /* 00 */
    REQ_RUN_THREAD_SIGNAL_STOP,       /* 01 */
};

/*=================== REQ_RUN_THREAD_STOP ===================*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    trap_thandle        thread;
} _WCUNALIGNED thread_stop_req;


/*=================== REQ_RUN_THREAD_SIGNAL_STOP ============*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    trap_thandle        thread;
} _WCUNALIGNED thread_signal_stop_req;


#pragma pack( pop )

#define TRPRTRD_H

#endif
