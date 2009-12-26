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
* Description:  DOS implementation of _dos_open (LFN-enabled)
*
****************************************************************************/


#include "variety.h"
#include <dos.h>
#include <fcntl.h>
#include "seterrno.h"
#include "tinyio.h"
#include "rtdata.h"

#define EX_LFN_OPEN         0
#define EX_LFN_CREATE       0x12
#define EX_LFN_CREATE_NEW   0x10

#define CREATE_EX_LFN   \
        "mov ax,716Ch"  \
        "stc"           \
        "int 21h"

#define OPEN_FILE       \
        _MOV_AH DOS_OPEN \
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
extern unsigned __dos_open_sfn( const char *name, unsigned cattr, int *handle );
extern unsigned __dos_open_sfn_chk( const char *name, unsigned cattr, int *handle );
#ifdef _M_I86
  #ifdef __SMALL_DATA__ // 16-bit near data
    #pragma aux __dos_create_ex_lfn = \
        CREATE_EX_LFN   \
        RETURN_HANDLE_EX \
        "call __doserror_" \
        parm caller     [si] [bx] [cx] [dx] [di] \
        modify exact    [cx];

    #pragma aux __dos_open_sfn = \
        "xchg ax,dx"    \
        OPEN_FILE       \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [ax] [dx] [bx] \
        modify exact    [dx];

    #pragma aux __dos_open_sfn_chk = \
        "xchg ax,dx"    \
        OPEN_FILE       \
        RETURN_HANDLE   \
        "sbb ax,ax"     \
        parm caller     [ax] [dx] [bx] \
        modify exact    [dx];

  #else                 // 16-bit far data
    #pragma aux __dos_create_ex_lfn = \
        "push ds"       \
        "mov  ds,ax"    \
        CREATE_EX_LFN   \
        "pop  ds"       \
        RETURN_HANDLE_EX \
        "call __doserror_" \
        parm caller     [ax si] [bx] [cx] [dx] [es di] \
        modify exact    [];

    #pragma aux __dos_open_sfn = \
        "push ds"       \
        "mov  ds,ax"    \
        "mov  ax,cx"    \
        OPEN_FILE       \
        "pop  ds"       \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [ax dx] [cx] [es bx] \
        modify exact    [];

    #pragma aux __dos_open_sfn_chk = \
        "push ds"       \
        "mov  ds,ax"    \
        "mov  ax,cx"    \
        OPEN_FILE       \
        "pop  ds"       \
        RETURN_HANDLE   \
        "sbb ax,ax"     \
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

    #pragma aux __dos_open_sfn = \
        "xchg eax,edx"  \
        OPEN_FILE       \
        RETURN_HANDLE   \
        "call __doserror_" \
        parm caller     [eax] [edx] [ebx] \
        modify exact    [edx];

    #pragma aux __dos_open_sfn_chk = \
        "xchg eax,edx"  \
        OPEN_FILE       \
        RETURN_HANDLE   \
        "sbb eax,eax"   \
        parm caller     [eax] [edx] [ebx] \
        modify exact    [edx];

#endif

#ifdef __WATCOM_LFN__
static unsigned __dos_open_lfn( const char *path, unsigned mode, int *handle )
{
    char        short_name[128];

    // try to open with SFN
    if( _lfntosfn( ( char * )path, short_name ) != NULL ) {
        if( short_name[0] != '\0' ) {
            if( __dos_open_sfn_chk( short_name, mode, handle ) == 0 ) {
                return( 0 );
            }
        }
    }
    return( __dos_create_ex_lfn( path, mode, 0, EX_LFN_OPEN, handle ) );
}
#endif

_WCRTLINK unsigned _dos_open( const char *path, unsigned mode, int *handle )
{
#ifdef __WATCOM_LFN__
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = __dos_open_lfn( path, mode, handle )) == 0 ) {
        return( rc );
    } else if( rc && rc != 0x7100 ) {
        return( rc );
    } else {
#endif
        return( __dos_open_sfn( path, mode, handle ) );
#ifdef __WATCOM_LFN__
    }
#endif
}
