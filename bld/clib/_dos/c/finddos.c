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
* Description:  DOS implementation of _dos_find... functions (LFN-enabled)
*
****************************************************************************/


#include "variety.h"
#include <string.h>
#include <dos.h>
#include "tinyio.h"
#include "rtdata.h"
#include "seterrno.h"
#include "_doslfn.h"

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned char _Extender;

extern unsigned __dos_find_first_dta( const char *path, unsigned attr, struct find_t *fdta );
extern unsigned __dos_find_next_dta( struct find_t *fdta );
extern unsigned __dos_find_close_dta( struct find_t *fdta );

#if defined( _M_I86 )
  #ifdef __BIG_DATA__   // 16-bit far data
    #pragma aux __dos_find_first_dta = \
        _SET_DSDX       \
        "push es"       \
        "push bx"       \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        "pop  dx"       \
        "pop  ds"       \
        _MOV_AH DOS_FIND_FIRST \
        _INT_21         \
        _RST_DS         \
        "call __doserror_" \
        parm caller     [es bx] [cx] [dx ax] \
        modify exact    [ax dx];

    #pragma aux __dos_find_next_dta = \
        _SET_DSDX       \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        _RST_DS         \
        _MOV_AH DOS_FIND_NEXT \
        _INT_21         \
        "call __doserror_" \
        parm caller     [dx ax] \
        modify exact    [ax dx];

    #pragma aux __dos_find_close_dta = \
        "xor  ax,ax"    \
        parm caller     [dx ax] \
        modify exact    [ax dx];

  #else                 // 16-bit near data
    #pragma aux __dos_find_first_dta = \
        "xchg bx,dx"    \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        "xchg bx,dx"    \
        _MOV_AH DOS_FIND_FIRST \
        _INT_21         \
        "call __doserror_" \
        parm caller     [dx] [cx] [bx] \
        modify exact    [ax];

    #pragma aux __dos_find_next_dta = \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        _MOV_AH DOS_FIND_NEXT \
        _INT_21         \
        "call __doserror_" \
        parm caller     [dx] \
        modify exact    [ax];

    #pragma aux __dos_find_close_dta = \
        "xor  ax,ax"    \
        parm caller     [dx] \
        modify exact    [ax];

  #endif
#elif defined( __OSI__ )    // 32-bit near data
    #pragma aux __dos_find_first_dta = \
        _MOV_AH DOS_FIND_FIRST \
        _INT_21         \
        "call __doserror_" \
        parm caller     [edx] [ecx] [ebx] \
        modify exact    [eax];

    #pragma aux __dos_find_next_dta = \
        _MOV_AX_W 0 DOS_FIND_NEXT \
        _INT_21         \
        "call __doserror_" \
        parm caller     [edx] \
        modify exact    [eax];

    #pragma aux __dos_find_close_dta = \
        _MOV_AX_W 1 DOS_FIND_NEXT \
        _INT_21         \
        "call __doserror_" \
        "xor  eax,eax"  \
        parm caller     [edx] \
        modify exact    [eax];

#else                   // 32-bit near data
    #pragma aux __dos_find_first_dta = \
        "push edx"      \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        "mov  edx,ebx"  \
        _MOV_AH DOS_FIND_FIRST \
        _INT_21         \
        "pop  edx"      \
        "call __doserror_" \
        "test eax,eax"  \
        "jnz  L1"       \
        "cmp  _Extender,9" \
        "jnz  L1"       \
        "push es"       \
        _MOV_AH DOS_GET_DTA \
        _INT_21         \
        MOV_DATA_FROM_DTA \
        "pop  es"       \
        "xor  eax,eax"  \
    "L1:"                   \
        parm caller     [ebx] [ecx] [edx] \
        modify exact    [eax ebx ecx edi esi];

    #pragma aux __dos_find_next_dta = \
        "push es"       \
        _MOV_AH DOS_SET_DTA \
        _INT_21         \
        "cmp  _Extender,9" \
        "jnz  L1"       \
        _MOV_AH DOS_GET_DTA \
        _INT_21         \
        MOV_DATA_TO_DTA \
    "L1:"               \
        _MOV_AH DOS_FIND_NEXT \
        _INT_21         \
        "call __doserror_" \
        "test eax,eax"  \
        "jnz  L2"       \
        "cmp  _Extender,9" \
        "jnz  L2"       \
        MOV_DATA_FROM_DTA \
    "L2: pop  es"       \
        parm caller     [edx] \
        modify exact    [eax ebx ecx edi esi];

    #pragma aux __dos_find_close_dta = \
        "xor  eax,eax"  \
        parm caller     [edx] \
        modify exact    [eax];

#endif

#ifdef __WATCOM_LFN__
/* The find block for the LFN find */
typedef struct {
    long    attributes;
    long    creattime;
    long    creatdate;
    long    accesstime;
    long    accessdate;
    long    wrtime;
    long    wrdate;
    long    hfilesize;
    long    lfilesize;
    char    reserved[8];
    char    lfn[NAME_MAX + 1];
    char    sfn[14];
} lfnfind_t;

extern tiny_ret_t __dos_find_first_lfn( const char *path, unsigned attr, lfnfind_t __far *lfndta );
extern unsigned __dos_find_next_lfn( unsigned handle, lfnfind_t __far *lfndta );
extern unsigned __dos_find_close_lfn( unsigned handle );

#if defined( _M_I86 )
  #ifdef __BIG_DATA__   // 16-bit far data
    #pragma aux __dos_find_first_lfn = \
        _SET_DSDX       \
        LFN_FIND_FIRST  \
        _SBB_DX_DX      \
        _RST_DS         \
        parm caller     [dx ax] [cx] [es di] \
        value           [dx ax] \
        modify exact    [ax cx dx];
  #else                 // 16-bit near data
    #pragma aux __dos_find_first_lfn = \
        LFN_FIND_FIRST  \
        _SBB_DX_DX      \
        parm caller     [dx] [cx] [es di] \
        value           [dx ax] \
        modify exact    [ax cx dx];
  #endif
    #pragma aux __dos_find_next_lfn = \
        LFN_FIND_NEXT   \
        "call __doserror_" \
        parm caller     [bx] [es di] \
        modify exact    [ax cx];

    #pragma aux __dos_find_close_lfn = \
        LFN_FIND_CLOSE  \
        "call __doserror_" \
        parm caller     [bx] \
        modify exact    [ax];

#else                   // 32-bit near data
    #pragma aux __dos_find_first_lfn = \
        LFN_FIND_FIRST  \
        "rcl eax,1"     \
        "ror eax,1"     \
        parm caller     [edx] [ecx] [es edi] \
        modify exact    [eax ecx];

    #pragma aux __dos_find_next_lfn = \
        LFN_FIND_NEXT   \
        "call __doserror_" \
        parm caller     [ebx] [es edi] \
        modify exact    [eax ecx];

    #pragma aux __dos_find_close_lfn = \
        LFN_FIND_CLOSE  \
        "call __doserror_" \
        parm caller     [ebx] \
        modify exact    [eax];

#endif


static void convert_to_find_t( struct find_t *fdta, lfnfind_t *lfnblock )
/***********************************************************************/
{
    fdta->attrib  = lfnblock->attributes;
    fdta->cr_time = lfnblock->creattime;
    fdta->cr_date = lfnblock->creatdate;
    fdta->ac_time = lfnblock->accesstime;
    fdta->ac_date = lfnblock->accessdate;
    fdta->wr_time = lfnblock->wrtime;
    fdta->wr_date = lfnblock->wrdate;
    fdta->size    = lfnblock->lfilesize;
    strcpy( fdta->name, ( *lfnblock->lfn != '\0' ) ? lfnblock->lfn : lfnblock->sfn );
}
#endif //__WATCOM_LFN__

_WCRTLINK unsigned _dos_findfirst( const char *path, unsigned attr,
                                  struct find_t *fdta )
/******************************************************/
{
#ifdef __WATCOM_LFN__
    lfnfind_t       lfndta;
    tiny_ret_t      rc = 0;

    if( _RWD_uselfn && TINY_OK( rc = __dos_find_first_lfn( path, attr, &lfndta ) ) ) {
        convert_to_find_t( fdta, &lfndta );
        fdta->lfnsup = _LFN_SIGN;
        HANDLE_OF( fdta ) = TINY_INFO( rc );
        return( 0 );
    }
    if( TINY_ERROR( rc ) && TINY_INFO( rc ) != 0x7100 ) {
        return( __set_errno_dos_reterr( TINY_INFO( rc ) ) );
    }
#endif
    return( __dos_find_first_dta( path, attr, fdta ) );
}


_WCRTLINK unsigned _dos_findnext( struct find_t *fdta )
/*****************************************************/
{
#ifdef __WATCOM_LFN__
    lfnfind_t       lfndta;
    unsigned        rc;

    if( _RWD_uselfn && fdta->lfnsup == _LFN_SIGN && HANDLE_OF( fdta ) ) {
        rc = __dos_find_next_lfn( HANDLE_OF( fdta ), &lfndta );
        if( rc == 0 ) {
            convert_to_find_t( fdta, &lfndta );
        }
        return( rc );
    }
#endif
    return( __dos_find_next_dta( fdta ) );
}


_WCRTLINK unsigned _dos_findclose( struct find_t *fdta )
/******************************************************/
{
#if defined( __WATCOM_LFN__ )
    if( _RWD_uselfn && fdta->lfnsup == _LFN_SIGN && HANDLE_OF( fdta ) ) {
        return( __dos_find_close_lfn( HANDLE_OF( fdta ) ) );
    }
#endif
#ifdef __OSI__
    return( __dos_find_close_dta( fdta ) );
#else
    return( 0 );
#endif
}
