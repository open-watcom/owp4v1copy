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
* Description:  Implementation of _status87() and _clear87().
*
****************************************************************************/


#include "variety.h"
#include <math.h>
#include <float.h>
#include "rtdata.h"

extern  void    __fclex( void );
extern  void    __fstsw( unsigned short * );

#if defined(__386__)
#pragma aux __fclex = \
                ".387" \
                "fclex";
#pragma aux __fstsw = \
                ".387" \
                "fstsw ss:[edi]" \
                "fwait" \
                parm caller [edi];
#else
#pragma aux __fclex = \
                ".8087" \
                float "fclex";
#pragma aux __fstsw = \
                ".8087" \
                "xchg ax,bp" \
                float "fstsw 0[bp]" \
                float "fnop" \
                "xchg ax,bp" \
                parm caller [ax];
#endif


_WMRTLINK unsigned _status87( void )
/**********************************/
{
    unsigned short  status;

    status = 0;
    if( _RWD_8087 ) {
        __fstsw( &status );
    }
    return( status );
}


_WMRTLINK unsigned _clear87( void )
/*********************************/
{
    int status;

    status = 0;
    if( _RWD_8087 ) {
        status = _status87();
        __fclex();
    }
    return( status );
}
