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
* Description:  OS/2 typedefs and #defines for 16-bit development.
*
****************************************************************************/

/* Basic typedefs and defines */

#define NULL       ((void *)0)

#define FAR     _far
#define NEAR    _near
#define PASCAL  _pascal
#define VOID    void

#define EXPENTRY  PASCAL FAR _loadds
#define APIENTRY  PASCAL FAR

#define CHAR     char
#define SHORT    short
#define LONG     long
#define INT      int

typedef unsigned char   UCHAR;
typedef unsigned char   BYTE;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;

typedef unsigned short  SHANDLE;
typedef void _far       *LHANDLE;

typedef SHANDLE    HFILE;
typedef HFILE FAR  *PHFILE;

typedef unsigned char FAR   *PSZ;
typedef unsigned char NEAR  *NPSZ;

typedef unsigned char FAR   *PCH;
typedef unsigned char NEAR  *NPCH;

typedef unsigned short  BOOL;
typedef BOOL FAR        *PBOOL;

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE  0
#endif

typedef CHAR FAR     *PCHAR;
typedef SHORT FAR    *PSHORT;
typedef INT FAR      *PINT;
typedef LONG FAR     *PLONG;
typedef UCHAR FAR    *PUCHAR;
typedef UCHAR FAR    *PBYTE;
typedef USHORT FAR   *PUSHORT;
typedef UINT FAR     *PUINT;
typedef ULONG FAR    *PULONG;

typedef VOID FAR     *PVOID;

typedef int (PASCAL FAR  *PFN)();
typedef int (PASCAL NEAR *NPFN)();
typedef PFN FAR *PPFN;

typedef USHORT       SEL;
typedef SEL FAR      *PSEL;


/* Useful DOS typedefs */
typedef USHORT       HMODULE;
typedef HMODULE FAR  *PHMODULE;
typedef USHORT       PID;
typedef PID FAR      *PPID;
typedef USHORT       TID;
typedef TID FAR      *PTID;
typedef VOID FAR     *HSEM;
typedef HSEM FAR     *PHSEM;
