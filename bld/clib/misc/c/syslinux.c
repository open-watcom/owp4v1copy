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
* Description:  Module implementing linux system calls interface
*
****************************************************************************/

#include <errno.h>
#include "syslinux.h"

/* user-visible error numbers are in the range -1 - -124 */

#define __syscall_return(type, res)                     \
if ((u_long)(res) >= (u_long)(-125)) {                  \
    errno = -(res);                                     \
    res = (u_long)-1;                                   \
}                                                       \
return (type)(res);

u_long sys_brk(u_long brk)
{
    u_long newbrk;

    newbrk = sys_call1(SYS_brk,brk);
    if( newbrk < brk )
        return (u_long)-1;
    return newbrk;
}

long sys_exit(int error_code)
{
    return sys_call1(SYS_exit,error_code);
}

    // TODO!
#if 0
sys_open
_WCRTLINK int open( const char *__path, int __oflag, ... )
{
    return 0;
}
#endif

_WCRTLINK int fstat( int __fildes, struct stat * __buf )
{
    u_long res = sys_call2(SYS_fstat, __fildes, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK ssize_t read( int __fildes, void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_read, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK ssize_t write( int __fildes, const void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_write, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK off_t lseek( int __fildes, off_t __offset, int __whence )
{
    u_long res = sys_call3(SYS_lseek, __fildes, __offset, __whence);
    __syscall_return(off_t,res);
}

_WCRTLINK int close( int __fildes )
{
    u_long res = sys_call1(SYS_close, __fildes);
    __syscall_return(int,res);
}

_WCRTLINK int fsync( int __fildes )
{
    u_long res = sys_call1(SYS_fsync, __fildes);
    __syscall_return(int,res);
}

