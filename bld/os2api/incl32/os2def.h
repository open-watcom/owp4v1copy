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
* Description:  OS/2 typedefs and constants for 32-bit development.
*
****************************************************************************/


#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define FIELDOFFSET(type, field)    ((SHORT)&(((type *)0)->field))

#define MAKEULONG(l, h)  ((ULONG)(((USHORT)(l)) | ((ULONG)((USHORT)(h))) << 16))
#define MAKELONG(l, h)   ((LONG)MAKEULONG(l, h))
#define MAKEUSHORT(l, h) (((USHORT)(l)) | ((USHORT)(h)) << 8)
#define MAKESHORT(l, h)  ((SHORT)MAKEUSHORT(l, h))

#define NULLHANDLE  ((LHANDLE)0)

#define APIENTRY16 _Far16 _Pascal
#define PASCAL16   _Far16 _Pascal

#define EXPENTRY  _System
#define APIENTRY  _System

#define CHAR     char
#define SHORT    short
#define LONG     long
#define INT      int
#define VOID     void

typedef unsigned long   APIRET;

typedef unsigned char   UCHAR,  *PUCHAR;
typedef char            BYTE,   *PBYTE;
typedef unsigned short  USHORT, *PUSHORT;
typedef unsigned int    UINT,   *PUINT;
typedef unsigned long   ULONG,  *PULONG;

typedef unsigned short  SHANDLE;
typedef unsigned long   LHANDLE;

typedef char *PSZ;
typedef char *PCH;
typedef const char *PCSZ;

typedef unsigned long BOOL, *PBOOL;
typedef unsigned LONG BOOL32, *PBOOL32;

typedef CHAR     *PCHAR;
typedef SHORT    *PSHORT;
typedef INT      *PINT;
typedef LONG     *PLONG;

typedef VOID     *PVOID;
typedef PVOID    *PPVOID;

typedef int (APIENTRY _PFN)();
typedef _PFN  *PFN;

typedef USHORT   SEL, *PSEL;

typedef LHANDLE  HFILE, *PHFILE;
typedef LHANDLE  HMODULE, *PHMODULE;
typedef LHANDLE  PID, *PPID;
typedef LHANDLE  TID, *PTID;
typedef ULONG    HEV,  *PHEV;
typedef VOID     *HSEM;
typedef HSEM     *PHSEM;
typedef USHORT   SGID;

typedef struct _QWORD {
    ULONG   ulLo;
    ULONG   ulHi;
} QWORD, *PQWORD;
