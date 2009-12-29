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
* Description:  DOS implementation of _dos_creat / _dos_creatnew (LFN-enabled)
*
****************************************************************************/


#include "variety.h"
#include <dos.h>
#include <fcntl.h>
#include "seterrno.h"
#include "tinyio.h"
#include "rtdata.h"
#include "_doslfn.h"

#ifdef _M_I86
  #ifdef __BIG_DATA__   // 16-bit far data
    #define INIT_VALUE
    #define SAVE_VALUE      "mov es:[bx],ax"
    #define SAVE_VALUE_EX   "mov es:[di],ax"
    #define AUX_INFO    \
        parm caller     [dx ax] [cx] [es bx] \
        modify exact    [ax dx];
    #define AUX_INFO_EX \
        parm caller     [si ax] [bx] [cx] [dx] [es di] \
        modify exact    [ax cx si];
  #else                 // 16-bit near data
    #define INIT_VALUE
    #define SAVE_VALUE      "mov [bx],ax"
    #define SAVE_VALUE_EX   "mov [di],ax"
    #define AUX_INFO    \
        parm caller     [dx] [cx] [bx] \
        modify exact    [ax];
    #define AUX_INFO_EX \
        parm caller     [si] [bx] [cx] [dx] [di] \
        modify exact    [ax cx];
  #endif
#else                   // 32-bit near data
    #define INIT_VALUE      "xor eax,eax"
    #define SAVE_VALUE      "mov [ebx],eax"
    #define SAVE_VALUE_EX   "mov [edi],eax"
    #define AUX_INFO    \
        parm caller     [edx] [ecx] [ebx] \
        modify exact    [eax];
    #define AUX_INFO_EX \
        parm caller     [esi] [ebx] [ecx] [edx] [edi] \
        modify exact    [eax ecx];
#endif

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned __dos_create_ex_lfn( const char *name, unsigned oattr, unsigned cattr, unsigned action, int *handle );
#pragma aux __dos_create_ex_lfn = \
        _SET_DSSI       \
        INIT_VALUE      \
        LFN_CREATE_EX   \
        _RST_DS         \
        RETURN_VALUE_EX \
        "call __doserror_" \
        AUX_INFO_EX

extern unsigned __dos_create_sfn( const char *name, unsigned cattr, int *handle );
#pragma aux __dos_create_sfn = \
        _SET_DSDX       \
        INIT_VALUE      \
        _MOV_AH DOS_CREAT \
        _INT_21         \
        _RST_DS         \
        RETURN_VALUE    \
        "call __doserror_" \
        AUX_INFO

extern unsigned __dos_create_new_sfn( const char *name, unsigned cattr, int *handle );
#pragma aux __dos_create_new_sfn = \
        _SET_DSDX       \
        INIT_VALUE      \
        _MOV_AH DOS_CREATE_NEW \
        _INT_21         \
        _RST_DS         \
        RETURN_VALUE    \
        "call __doserror_" \
        "nop"           \
        AUX_INFO


#ifdef __WATCOM_LFN__
static unsigned _dos_create_ex_lfn( const char *path, unsigned mode, unsigned attrib, unsigned style, int *handle )
{
    return( __dos_create_ex_lfn( path, mode, attrib, style, handle ) );
}
#endif

_WCRTLINK unsigned _dos_creat( const char *path, unsigned attrib, int *handle )
{
#ifdef __WATCOM_LFN__
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = _dos_create_ex_lfn( path, O_WRONLY, attrib, EX_LFN_CREATE, handle )) == 0 ) {
        return( rc );
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
#endif
    return( __dos_create_sfn( path, attrib, handle ) );
}

_WCRTLINK unsigned _dos_creatnew( const char *path, unsigned attrib, int *handle )
{
#ifdef __WATCOM_LFN__
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = _dos_create_ex_lfn( path, O_WRONLY, attrib, EX_LFN_CREATE_NEW, handle )) == 0 ) {
        return( rc );
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
#endif
    return( __dos_create_new_sfn( path, attrib, handle ) );
}
