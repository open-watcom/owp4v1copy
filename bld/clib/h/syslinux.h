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
* Description:  Prototypes of Linux system calls internal to the clib.
*
****************************************************************************/

#ifndef _CLIB_SYSLINUX_H_INCLUDED
#define _CLIB_SYSLINUX_H_INCLUDED

//#ifndef __TYPES_H_INCLUDED
// #include <sys/types.h>
//#endif

/*
 * internal sub-numbers for SYS_socketcall
 */

#define SYS_SOCKET                1
#define SYS_BIND                  2
#define SYS_CONNECT               3
#define SYS_LISTEN                4
#define SYS_ACCEPT                5
#define SYS_GETSOCKNAME           6
#define SYS_GETPEERNAME           7
#define SYS_SOCKETPAIR            8
#define SYS_SEND                  9
#define SYS_RECV                 10
#define SYS_SENDTO               11
#define SYS_RECVFROM             12
#define SYS_SHUTDOWN             13
#define SYS_SETSOCKOPT           14
#define SYS_GETSOCKOPT           15
#define SYS_SENDMSG              16
#define SYS_RECVMSG              17

/*
 * Prototypes for Linux system call functions
 */

u_long  sys_brk( u_long brk );
long    sys_open( const char * filename, int flags, int mode );
long    sys_exit( int error_code );
int     sys_getdents( u_int fd, struct dirent *dirp, u_int count );
long    __socketcall( int call, u_long *args );

#endif
