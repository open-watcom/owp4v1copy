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

#define DOS_GET_DTA     0x2F

#define HANDLE_OF(x)    ((x)->lfnax)

#define SET_DTA         \
        _MOV_AH DOS_SET_DTA \
        _INT_21

#define GET_DTA         \
        _MOV_AH DOS_GET_DTA \
        _INT_21

#define FIND_FIRST      \
        _MOV_AH DOS_FIND_FIRST \
        _INT_21

#define FIND_NEXT       \
        _MOV_AH DOS_FIND_NEXT \
        _INT_21

#define FIND_FIRST_LFN  \
        "mov  ax,714Eh" \
        "mov  si,1"     \
        "stc"           \
        "int  21h"

#define FIND_NEXT_LFN   \
        "mov  ax,714fh" \
        "mov  si,1"     \
        "stc"           \
        "int  21h"

#define FIND_CLOSE_LFN  \
        "mov  ax,71A1h" \
        "stc"           \
        "int  21h"

#define MOV_DTA         \
        "mov  ecx,43"   \
        "rep movsb"

#define MOV_DATA_TO_DTA \
        "mov  esi,edx"  \
        "mov  edi,ebx"  \
        MOV_DTA

#define MOV_DATA_FROM_DTA \
        "mov  esi,ebx"  \
        "mov  edi,edx"  \
        "mov  ebx,ds"   \
        "push es"       \
        "pop  ds"       \
        "mov  es,ebx"   \
        MOV_DTA         \
        "mov  ds,ebx"

extern unsigned __doserror_( unsigned );
#pragma aux __doserror_ "*"

extern unsigned __dos_find_first_dta( const char *path, unsigned attr, struct find_t *fdta );
extern unsigned __dos_find_next_dta( struct find_t *fdta );
extern unsigned __dos_find_close_dta( struct find_t *fdta );

#if defined( _M_I86 )   // 16-bit
  #ifdef __SMALL_DATA__   // near data
    #pragma aux __dos_find_first_dta = \
        "xchg bx,dx"    \
        SET_DTA         \
        "xchg bx,dx"    \
        FIND_FIRST      \
        "call __doserror_" \
        parm caller     [dx] [cx] [bx] \
        modify exact    [];

    #pragma aux __dos_find_next_dta = \
        SET_DTA         \
        FIND_NEXT       \
        "call __doserror_" \
        parm caller     [dx] \
        modify exact    [];

    #pragma aux __dos_find_close_dta = \
        "xor  ax,ax"    \
        parm caller     [dx] \
        modify exact    [];

  #else                   // far data
    #pragma aux __dos_find_first_dta = \
        "push ds"       \
        "mov  ds,ax"    \
        "push es"       \
        "push bx"       \
        SET_DTA         \
        "pop  dx"       \
        "pop  ds"       \
        FIND_FIRST      \
        "call __doserror_" \
        "pop  ds"       \
        parm caller     [es bx] [cx] [ax dx] \
        modify exact    [dx];

    #pragma aux __dos_find_next_dta = \
        "push ds"       \
        "mov  ds,ax"    \
        SET_DTA         \
        "pop  ds"       \
        FIND_NEXT       \
        "call __doserror_" \
        parm caller     [ax dx] \
        modify exact    [];

    #pragma aux __dos_find_close_dta = \
        "xor  ax,ax"    \
        parm caller     [ax dx] \
        modify exact    [];

  #endif
#elif defined( __OSI__ )
    #pragma aux __dos_find_first_dta = \
        FIND_FIRST      \
        "call __doserror_" \
        parm caller     [edx] [cx] [ebx] \
        modify exact    [];

    #pragma aux __dos_find_next_dta = \
        "mov  al,0"     \
        FIND_NEXT       \
        "call __doserror_" \
        parm caller     [edx] \
        modify exact    [];

    #pragma aux __dos_find_close_dta = \
        "mov  al,1"     \
        FIND_NEXT       \
        "call __doserror_" \
        "xor  eax,eax"  \
        parm caller     [edx] \
        modify exact    [];

#else                   // 32-bit
    #pragma aux __dos_find_first_dta = \
        FIND_FIRST      \
        "call __doserror_" \
        "test eax,eax"  \
        "jnz  L1"       \
        "push es"       \
        "mov  edx,ebx"  \
        GET_DTA         \
        MOV_DATA_FROM_DTA \
        "pop  es"       \
        "xor  eax,eax"  \
    "L1:"                   \
        parm caller     [edx] [cx] [ebx] \
        modify exact    [ebx ecx edx edi esi];

    #pragma aux __dos_find_next_dta = \
        "push es"       \
        GET_DTA         \
        MOV_DATA_TO_DTA \
        FIND_NEXT       \
        "call __doserror_" \
        "test eax,eax"  \
        "jnz  L1"       \
        MOV_DATA_FROM_DTA \
    "L1: pop  es"       \
        parm caller     [edx] \
        modify exact    [ebx ecx edi esi];

    #pragma aux __dos_find_close_dta = \
        "xor  eax,eax"  \
        parm caller     [edx] \
        modify exact    [];

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

extern unsigned __dos_find_first_LFN( const char *path, unsigned attr, lfnfind_t __far *lfndta );
extern unsigned __dos_find_next_LFN( unsigned handle, lfnfind_t __far *lfndta );
extern unsigned __dos_find_close_LFN( unsigned handle );

#if defined( _M_I86 )   // 16-bit
  #ifdef __SMALL_DATA__   // near data
    #pragma aux __dos_find_first_LFN = \
        FIND_FIRST_LFN  \
        "call __doserror_" \
        parm caller     [dx] [cx] [es di] \
        modify exact    [cx si];
  #else                   // far data
    #pragma aux __dos_find_first_LFN = \
        FIND_FIRST_LFN  \
        "call __doserror_" \
        parm caller     [ds dx] [cx] [es di]\
        modify exact    [cx si];
  #endif
    #pragma aux __dos_find_next_LFN = \
        FIND_NEXT_LFN  \
        "call __doserror_" \
        parm caller     [bx] [es di] \
        modify exact    [cx si];

#else                   // 32-bit
    #pragma aux __dos_find_first_LFN = \
        FIND_FIRST_LFN  \
        "call __doserror_" \
        parm caller     [edx] [cx] [es edi] \
        modify exact    [cx si];

    #pragma aux __dos_find_next_LFN = \
        FIND_NEXT_LFN  \
        "call __doserror_" \
        parm caller     [bx] [es edi] \
        modify exact    [ecx esi];

#endif

#pragma aux __dos_find_close_LFN = \
        FIND_CLOSE_LFN  \
        "call __doserror_" \
        parm caller     [bx] \
        modify exact    [];


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
    unsigned        rc = 0;

    if( _RWD_uselfn && (rc = __dos_find_first_LFN( path, attr, &lfndta )) == 0 ) {
        convert_to_find_t( fdta, &lfndta );
        fdta->lfnsup = _LFN_SIGN;
        HANDLE_OF( fdta ) = rc;
        return( 0 );
    } else if( rc && rc != 0x7100 ) {
        return( rc );
    } else {
#endif
        return( __dos_find_first_dta( path, attr, fdta ) );
#ifdef __WATCOM_LFN__
    }
#endif
}


_WCRTLINK unsigned _dos_findnext( struct find_t *fdta )
/*****************************************************/
{
#ifdef __WATCOM_LFN__
    lfnfind_t       lfndta;
    unsigned        rc;

    if( _RWD_uselfn && fdta->lfnsup == _LFN_SIGN && HANDLE_OF( fdta ) ) {
        rc = __dos_find_next_LFN( HANDLE_OF( fdta ), &lfndta );
        if( rc ) {
            return( rc );
        }
        convert_to_find_t( fdta, &lfndta );
        return( 0 );
    } else {
#endif
        return( __dos_find_next_dta( fdta ) );
#ifdef __WATCOM_LFN__
    }
#endif
}


_WCRTLINK unsigned _dos_findclose( struct find_t *fdta )
/******************************************************/
{
#if defined( __WATCOM_LFN__ )
    if( _RWD_uselfn && fdta->lfnsup == _LFN_SIGN && HANDLE_OF( fdta ) ) {
        return( __dos_find_close_LFN( HANDLE_OF( fdta ) ) );
    } else {
#endif
#ifdef __OSI__
        return( __dos_find_close_dta( fdta ) );
#else
        return( 0 );
#endif
#if defined( __WATCOM_LFN__ )
    }
#endif
}
