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

#include "syslinux.h"

/* user-visible error numbers are in the range -1 - -124 */

#define __syscall_return(type, res)                         \
do {                                                        \
    if ((unsigned long)(res) >= (unsigned long)(-125)) {    \
        errno = -(res);                                     \
        res = -1;                                           \
    }                                                       \
    return (type) (res);                                    \
} while (0)

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

ssize_t sys_write(u_int fd, const char * buf,size_t count)
{
    return sys_call3(SYS_write, fd, (u_long)buf, count);
}

