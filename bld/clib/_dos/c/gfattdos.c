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
* Description:  DOS implementation of get file attributes (LFN-enabled)
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
    #define SAVE_VALUE  "mov es:[bx],cx"
    #define AUX_INFO    \
        parm caller     [dx ax] [es bx] \
        modify exact    [ax cx dx];
  #else
    #define INIT_VALUE
    #define SAVE_VALUE  "mov [bx],cx"
    #define AUX_INFO    \
        parm caller     [dx] [bx] \
        modify exact    [ax cx];
  #endif
#else
    #define INIT_VALUE  "xor  ecx,ecx"
    #define SAVE_VALUE  "mov [ebx],ecx"
    #define AUX_INFO    \
        parm caller     [edx] [ebx] \
        modify exact    [eax ecx];
#endif

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned __dos_get_file_attr_sfn( const char *path, unsigned *attr );
#pragma aux __dos_get_file_attr_sfn = \
        _SET_DSDX       \
        INIT_VALUE      \
        _MOV_AX_W _GET_ DOS_CHMOD \
        _INT_21         \
        _RST_DS         \
        RETURN_VALUE    \
        "call __doserror_" \
        AUX_INFO

extern unsigned __dos_get_file_attr_lfn( const char *path, unsigned *attr );
#pragma aux __dos_get_file_attr_lfn = \
        _SET_DSDX       \
        INIT_VALUE      \
        LFN_GET_FILE_ATTR \
        _RST_DS         \
        RETURN_VALUE    \
        "call __doserror_" \
        AUX_INFO

_WCRTLINK unsigned _dos_getfileattr( const char *path, unsigned *attrib )
{
#ifdef __WATCOM_LFN__
    unsigned    rc = 0;

    if( _RWD_uselfn && (rc = __dos_get_file_attr_lfn( path, attrib )) == 0 ) {
        return( rc );
    }
    if( IS_LFN_ERROR( rc ) ) {
        return( rc );
    }
#endif
    return( __dos_get_file_attr_sfn( path, attrib ) );
}
