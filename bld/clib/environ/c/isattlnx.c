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
* Description:  Implementation of isatty() for Linux
*
****************************************************************************/

#include <unistd.h>

/* #include <termios.h> defining it here for now -- should go to a better
   place later */
typedef unsigned char   cc_t;
typedef unsigned int    tcflag_t;

#define NCCS 19
struct termios {
  tcflag_t c_iflag;               /* input mode flags */
  tcflag_t c_oflag;               /* output mode flags */
  tcflag_t c_cflag;               /* control mode flags */
  tcflag_t c_lflag;               /* local mode flags */
  cc_t c_line;                    /* line discipline */
  cc_t c_cc[NCCS];                /* control characters */
};
#define TCGETS 0x5401

/* sys/ioctl.h */
int ioctl(int d, int request, void *arg);
/* should be "..." but this way we have register calling */

_WCRTLINK int tcgetattr ( int fd, struct termios *termios_p )
{
  return ioctl( fd, TCGETS, termios_p );
}

_WCRTLINK int isatty( int __fildes )
{
  struct termios  term;

  return( tcgetattr( __fildes, &term ) != -1 );
}
