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
#include "_doslfn.h"

#ifdef _M_I86
  #ifdef __BIG_DATA__
    #define INIT_VALUE
    #define SAVE_VALUE      "mov es:[bx],ax"
    #define SAVE_VALUE_EX   "mov es:[di],ax"
    #define RETURN_CY       "sbb ax,ax"
    #define AUX_INFO    \
        parm caller     [dx ax] [cx] [es bx] \
        modify exact    [ax dx];
    #define AUX_INFO_EX \
        parm caller     [si ax] [bx] [cx] [dx] [es di] \
        modify exact    [ax si];
  #else
    #define INIT_VALUE
    #define SAVE_VALUE      "mov [bx],ax"
    #define SAVE_VALUE_EX   "mov [di],ax"
    #define RETURN_CY       "sbb ax,ax"
    #define AUX_INFO    \
        parm caller     [dx] [cx] [bx] \
        modify exact    [ax dx];
    #define AUX_INFO_EX \
        parm caller     [si] [bx] [cx] [dx] [di] \
        modify exact    [ax cx];
  #endif
#else
    #define INIT_VALUE      "xor eax,eax"
    #define SAVE_VALUE      "mov [ebx],eax"
    #define SAVE_VALUE_EX   "mov [edi],eax"
    #define RETURN_CY       "sbb eax,eax"
    #define AUX_INFO    \
        parm caller     [edx] [ecx] [ebx] \
        modify exact    [eax edx];
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

#define __DOS_OPEN_SFN  \
        _SET_DSDX       \
        INIT_VALUE      \
        "mov  al,cl"    \
        _MOV_AH DOS_OPEN \
        _INT_21         \
        _RST_DS         \
        RETURN_VALUE

extern unsigned __dos_open_sfn_chk( const char *name, unsigned cattr, int *handle );
#pragma aux __dos_open_sfn_chk = \
        __DOS_OPEN_SFN  \
        RETURN_CY       \
        AUX_INFO

extern unsigned __dos_open_sfn_err( const char *name, unsigned cattr, int *handle );
#pragma aux __dos_open_sfn_err = \
        __DOS_OPEN_SFN  \
        "call __doserror_" \
        AUX_INFO

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
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
#endif
    return( __dos_open_sfn_err( path, mode, handle ) );
}
