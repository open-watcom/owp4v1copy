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
* Description:  Internal data and routines for "Safer C", or bounds-checking
*               library extension.
*
****************************************************************************/


#ifndef _SAFERLIB_H_INCLUDED
#define _SAFERLIB_H_INCLUDED

#define __STDC_WANT_LIB_EXT1__  1
#include <stdlib.h>
#include <stdint.h>

// Horrible temporary HACK!
// Needs to go when we decide how to handle 'restrict' keyword.
#define restrict


// Maximum length of runtime-constraint error message
#define RTCT_MSG_MAX            128

#define _RWD_rtcthandler        __runtime_constraint_handler

extern  constraint_handler_t    __runtime_constraint_handler;

extern  void    __rtct_fail( const char *fn, const char *reason, void *reserved );

// Runtime-constraint validation macros. Call the handler and return zero if check
// failed, return non-zero value if check succeeded.

#define __check_constraint_nullptr( arg )   \
    ((arg == NULL) ? __rtct_fail( __func__, #arg " == NULL", NULL ), 0 : 1)

#define __check_constraint_maxsize( arg )   \
    ((arg > RSIZE_MAX) ? __rtct_fail( __func__, #arg " > RSIZE_MAX", NULL ), 0 : 1)

#define __check_constraint_zero( arg )   \
    ((arg == 0) ? __rtct_fail( __func__, #arg " == 0", NULL ), 0 : 1)

#define __check_constraint_toosmall( name, left )   \
    ((left == 0) ? __rtct_fail( __func__, #name " is too small to hold data", NULL ), 0 : 1)


// For 16-bit targets, the RSIZE_MAX check is effectively no-op. Object sizes up
// to SIZE_MAX are legal and not uncommon.
#if RSIZE_MAX == SIZE_MAX
    #undef __check_constraint_maxsize
    #define __check_constraint_maxsize( arg )   1
#endif

#endif // _SAFERLIB_H_INCLUDED
