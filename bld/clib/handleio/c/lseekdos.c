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
* Description:  low level lseek without file extend for DOS and Windows
*
****************************************************************************/


#include "variety.h"
#include <stdio.h>
#include <unistd.h>
#include "iomode.h"
#include "tinyio.h"
#include "rtcheck.h"
#include "seterrno.h"

#ifdef __WINDOWS_386__
  #include <dos.h>
#endif


_WCRTLINK long __lseek( int handle, long offset, int origin )
{
    uint_32             pos;

#ifdef __WINDOWS_386__
    union REGS regs;

    __handle_check( handle, -1 );
    
    regs.h.ah = DOS_LSEEK;
    regs.h.al = origin;
    regs.w.bx = handle;
    regs.w.cx = (offset >> 16) & 0xffff;
    regs.w.dx = offset & 0xffff;
    intdos( &regs, &regs );
    pos = (regs.w.dx << 16) | regs.w.ax;
    if( regs.w.cflag ) {
        __set_errno_dos( regs.w.ax );
        return( -1L );
    }
#else
    tiny_ret_t rc;
    
    __handle_check( handle, -1 );
    
    rc = TinyLSeek( handle, offset, origin, (void _WCNEAR *) &pos );
    if( TINY_ERROR(rc) ) {
        __set_errno_dos( TINY_INFO(rc) );
        return( -1L );
    }
#endif
    return( pos );
}
