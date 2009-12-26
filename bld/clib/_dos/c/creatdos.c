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

#define DOS_CREATNEW        0x5B

#define EX_LFN_CREATE       0x12
#define EX_LFN_CREATE_NEW   0x10

#define CREATE_EX_LFN   \
        "mov ax,716Ch"  \
        "stc"           \
        "int 21h"

#define CREATE_FILE     \
        _MOV_AH DOS_CREAT \
        _INT_21

#define CREATE_NEW_FILE \
        _MOV_AH DOS_CREATNEW \
        _INT_21

#ifdef _M_I86
  #ifdef __SMALL_DATA__ // 16-bit near data
    #define SAVE_HANDLE    "mov [bx],ax"
    #define SAVE_HANDLE_EX "mov [di],ax"
  #else
    #define SAVE_HANDLE    "mov es:[bx],ax"
    #define SAVE_HANDLE_EX "mov es:[di],ax"
  #endif
#else
    #define SAVE_HANDLE    "mov [ebx],eax"
    #define SAVE_HANDLE_EX "mov [edi],eax"
#endif

#define RETURN_HANDLE   \
        "jc short L1"   \
        SAVE_HANDLE     \
"L1:"

#define RETURN_HANDLE_EX \
        "jc short L1"   \
        SAVE_HANDLE_EX  \
"L1:"

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned __dos_create_ex_lfn( const char *name, unsigned oattr, unsigned cattr, unsigned action, int *handle );
extern unsigned __dos_create_sfn( const char *name, unsigned cattr, int *handle );
extern unsigned __dos_create_new_sfn( const char *name, unsigned cattr, int *handle );

#ifdef _M_I86
  #ifdef __SMALL_DATA__ // 16-bit near data
    #pragma aux __dos_create_ex_lfn = \
        CREATE_EX_LFN   \
        RETURN_HANDLE_EX \
        "call __doserror_" \
        parm caller     [si] [bx] [cx] [dx] [di] \
        modify exact    [cx];

    #pragma aux __dos_create_sfn = \
        CREATE_FILE     \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [dx] [cx] [bx] \
        modify exact    [];

    #pragma aux __dos_create_new_sfn = \
        CREATE_NEW_FILE \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [dx] [cx] [bx] \
        modify exact    [];

  #else                 // 16-bit far data
    #pragma aux __dos_create_ex_lfn = \
        "push ds"       \
        "mov  ds,ax"    \
        CREATE_EX_LFN   \
        "pop  ds"       \
        RETURN_HANDLE_EX \
        "call __doserror_" \
        parm caller     [ax si] [bx] [cx] [dx] [es di] \
        modify exact    [cx];

    #pragma aux __dos_create_sfn = \
        "push ds"       \
        "mov  ds,ax"    \
        CREATE_FILE     \
        "pop  ds"       \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [ax dx] [cx] [es bx] \
        modify exact    [];

    #pragma aux __dos_create_new_sfn = \
        "push ds"       \
        "mov  ds,ax"    \
        CREATE_NEW_FILE \
        "pop  ds"       \
        RETURN_HANDLE   \
        "call __doserror_" \
        "nop"           \
        parm caller     [ax dx] [cx] [es bx] \
        modify exact    [];

  #endif
#else                   // 32-bit
    #pragma aux __dos_create_ex_lfn = \
        CREATE_EX_LFN   \
        RETURN_HANDLE_EX \
        "call __doserror_" \
        parm caller     [esi] [ebx] [ecx] [edx] [edi] \
        modify exact    [ecx];

    #pragma aux __dos_create_sfn = \
        CREATE_FILE     \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [edx] [ecx] [ebx] \
        modify exact    [];

    #pragma aux __dos_create_new_sfn = \
        CREATE_NEW_FILE \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [edx] [ecx] [ebx] \
        modify exact    [];

#endif

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
    } else if( rc && rc != 0x7100 ) {
        return( rc );
    } else {
#endif
        return( __dos_create_sfn( path, attrib, handle ) );
#ifdef __WATCOM_LFN__
    }
#endif
}

_WCRTLINK unsigned _dos_creatnew( const char *path, unsigned attrib, int *handle )
{
#ifdef __WATCOM_LFN__
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = _dos_create_ex_lfn( path, O_WRONLY, attrib, EX_LFN_CREATE_NEW, handle )) == 0 ) {
        return( rc );
    } else if( rc && rc != 0x7100 ) {
        return( rc );
    } else {
#endif
        return( __dos_create_new_sfn( path, attrib, handle ) );
#ifdef __WATCOM_LFN__
    }
#endif
}
