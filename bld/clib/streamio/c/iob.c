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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "rtdata.h"
#include "rtinit.h"
#include "tmpfname.h"

/*
//	The following shows what was the underlying NetWare FILE structure (struct __iobuf). We are now using the OpenWatcom stdio
//	declaration rather than an external derivation. Still I believe that all macros and direct access to the FILE structure
//	should be curtailed so we can abstract as much as possible!
//
//	As I say, the following definitions are now meaningless!
*/

// *
// * NetWare FILE struct
// *
//typedef struct  __iobuf {
//        unsigned char   *_ptr;          /* next character position */
//        int             _cnt;           /* number of characters left */
//        unsigned char   *_base;         /* location of buffer */
//        unsigned        _flag;          /* mode of file access */
//        int             _handle;        /* file handle */
//        unsigned        _bufsize;       /* size of buffer */
//        unsigned char   _ungotten;      /* character placed here by ungetc */
//        unsigned char   _tmpfchar;      /* tmpfile number */
//} FILE;
// *
// * All others FILE struct
// *
//typedef struct  __iobuf {
//    unsigned char        *_ptr;         /* next character position */
//    int                   _cnt;         /* number of characters left */
//    struct __stream_link *_link;        /* location of associated struct */
//    unsigned              _flag;        /* mode of file access */
//    int                   _handle;      /* file handle */
//    unsigned              _bufsize;     /* size of buffer */
//    unsigned short        _ungotten;    /* used by ungetc and ungetwc */
//} FILE;


_WCRTLINK FILE _WCNEAR __iob[_NFILES] = {
    { NULL, 0, NULL, _READ  ,       0, 0, 0  }  /* stdin */
   ,{ NULL, 0, NULL, _WRITE ,       1, 0, 0  }  /* stdout */
   ,{ NULL, 0, NULL, _WRITE ,       2, 0, 0  }  /* stderr */
  #if !defined(__QNX__) && !defined(__NETWARE__) && !defined(__NT__)
   ,{ NULL, 0, NULL, _READ|_WRITE , 3, 0, 0  }  /* stdaux */
   ,{ NULL, 0, NULL, _WRITE ,       4, 0, 0  }  /* stdprn */
  #endif
};

__stream_link *__ClosedStreams;
__stream_link *__OpenStreams;

#if !defined(__QNX__)
_WCRTLINK int _WCNEAR _fmode = O_TEXT;  /* default file translation mode */
#endif

extern void __InitFiles();
extern void __full_io_exit();

AXI(__InitFiles,INIT_PRIORITY_LIBRARY);
AYI(__full_io_exit,INIT_PRIORITY_LIBRARY);
