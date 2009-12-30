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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <stdio.h>
#include "tinyio.h"
#include "seterrno.h"
#include "rtdata.h"
#include "_doslfn.h"
#ifdef __WIDECHAR__
    #include "mbwcconv.h"
#endif

#ifdef _M_I86
  #ifdef __BIG_DATA__   // 16-bit far data
    #define AUX_INFO    \
        parm caller     [dx ax] [es di]\
        modify exact    [ax];
  #else                 // 16-bit near data
    #define AUX_INFO    \
        parm caller     [dx] [es di]\
        modify exact    [ax];
  #endif
#else                   // 32-bit near data
    #define AUX_INFO    \
        parm caller     [edx] [edi]\
        modify exact    [eax];
#endif

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern int __dos_rename_sfn( const char *old, const char _WCI86FAR *new );
#pragma aux __dos_rename_sfn = \
        _SET_DSDX       \
        _MOV_AH DOS_RENAME \
        _INT_21         \
        _RST_DS         \
        "call __doserror_" \
        AUX_INFO

extern int __dos_rename_lfn( const char *old, const char _WCI86FAR *new );
#pragma aux __dos_rename_lfn = \
        _SET_DSDX       \
        LFN_DOS_RENAME  \
        _RST_DS         \
        "call __doserror_" \
        AUX_INFO


_WCRTLINK int __F_NAME(rename,_wrename)( const CHAR_TYPE *old, const CHAR_TYPE *new )
{
#ifdef __WIDECHAR__
    char        mbOld[MB_CUR_MAX*_MAX_PATH];    /* single-byte char */
    char        mbNew[MB_CUR_MAX*_MAX_PATH];    /* single-byte char */

    __filename_from_wide( mbOld, old );
    __filename_from_wide( mbNew, new );
    return( rename( mbOld, mbNew ) );
#else
  #if defined( __WATCOM_LFN__ )
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = __dos_rename_lfn( old, new )) == 0 ) {
        return( rc );
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
  #endif
    return( __dos_rename_sfn( old, new ) );
#endif
}

