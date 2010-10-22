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
    REQ_RUN_THREAD_INFO,            /* 00 */
    REQ_RUN_THREAD_GET_NEXT,        /* 01 */
    REQ_RUN_THREAD_GET_RUNTIME,     /* 02 */
    REQ_RUN_THREAD_STOP,            /* 03 */
    REQ_RUN_THREAD_SIGNAL_STOP,     /* 04 */
};

/*=================== REQ_RUN_THREAD_INFO ===================*/

#define RUN_THREAD_INFO_TYPE_NONE       0
#define RUN_THREAD_INFO_TYPE_NAME       1
#define RUN_THREAD_INFO_TYPE_STATE      2
#define RUN_THREAD_INFO_TYPE_CS_EIP     3
#define RUN_THREAD_INFO_TYPE_EXTRA      4

typedef struct {
    supp_prefix         supp;
    access_req          req;
    unsigned_16         col;        /* column in thread list to get info about */
} _WCUNALIGNED run_thread_info_req;

typedef struct {
    unsigned_8          info;       /* type of info to present */
    unsigned_16         width;      /* width to display results in */
    /* followed by header string */
} run_thread_info_ret;

/*=================== REQ_RUN_THREAD_GET_NEXT ===================*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    unsigned_32         thread;     /* thread to ask about */
} _WCUNALIGNED run_thread_get_next_req;

typedef struct {
    unsigned_32         thread;     /* next thread */
    /* followed by thread-name/id string */
} run_thread_get_next_ret;

/*=================== REQ_RUN_THREAD_GET_RUNTIME ===================*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    unsigned_32         thread;     /* thread to ask about */
} _WCUNALIGNED run_thread_get_runtime_req;

typedef struct {
    unsigned_8          state;      /* thread state */
    unsigned_16         cs;         /* current cs, if any */
    unsigned_32         eip;        /* current eip, if any */
    /* followed by thread-extra (for instance, execution time) string */
} run_thread_get_runtime_ret;

/*=================== REQ_RUN_THREAD_STOP ===================*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    unsigned_32         thread;     /* thread to stop */
} _WCUNALIGNED run_thread_stop_req;

/*=================== REQ_RUN_THREAD_SIGNAL_STOP ============*/

typedef struct {
    supp_prefix         supp;
    access_req          req;
    unsigned_32         thread;     /* thread to signal and stop */
} _WCUNALIGNED run_thread_signal_stop_req;


#pragma pack( pop )

#define TRPRTRD_H

#endif
