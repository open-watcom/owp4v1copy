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


// Compiler/Editor interface

#define EDIT_INTERRUPT  0xec            // editor interface interrupt

#define ED_FINIEDITOR   0
#define ED_EDITFILE     1
#define ED_RESUMEEDITOR 2
#define ED_OPENF        3
#define ED_GETREC       4
#define ED_CLOSEF       5
#define ED_EOF          6
#define ED_GETABS       7
#define ED_GETRAW       8
#define ED_SCRATCHF     9


//
// Used by GEDITINT.C and GEDIT.c
//

#if _TARGET == _80386
#define DS_VALUE        unsigned_16,
#else
#define DS_VALUE
#endif

extern  unsigned_16     EditInt1(DS_VALUE unsigned_16);
extern  unsigned_16     EditInt2(DS_VALUE unsigned_16,unsigned_16);
extern  unsigned_16     EditInt3(DS_VALUE unsigned_16,unsigned_16,unsigned_16);
extern  unsigned_16     EditInt4(DS_VALUE unsigned_16,unsigned_16,unsigned_16,unsigned_16);
extern  unsigned_16     EditInt5(DS_VALUE unsigned_16,unsigned_16,unsigned_16,unsigned_16,unsigned_16);

#undef DS_VALUE

#if _TARGET != _80386

#pragma aux EditInt1 = \
        0xcd 0xec       /* int 0ech */ \
        parm routine [si] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt2 = \
        0xcd 0xec       /* int 0ech */ \
        parm routine [si] [ax] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt3 = \
        0xcd 0xec       /* int 0ech */ \
        parm routine [si] [ax] [dx] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt4 = \
        0xcd 0xec       /* int 0ech */ \
        parm routine [si] [ax] [dx] [bx] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt5 = \
        0xcd 0xec       /* int 0ech */ \
        parm routine [si] [ax] [dx] [bx] [cx] \
        value [ax] \
        modify [ax bx cx dx si di];

#else

#pragma aux EditInt1 = \
        "push   ebp" /* high order word of EBP was getting hammered */ \
        "int    0ech" \
        "pop    ebp" \
        parm routine [di] [si] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt2 = \
        "push   ebp" /* high order word of EBP was getting hammered */ \
        "int    0ech" \
        "pop    ebp" \
        parm routine [di] [si] [ax] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt3 = \
        "push   ebp" /* high order word of EBP was getting hammered */ \
        "int    0ech" \
        "pop    ebp" \
        parm routine [di] [si] [ax] [dx] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt4 = \
        "push   ebp" /* high order word of EBP was getting hammered */ \
        "int    0ech" \
        "pop    ebp" \
        parm routine [di] [si] [ax] [dx] [bx] \
        value [ax] \
        modify [ax bx cx dx si di];

#pragma aux EditInt5 = \
        "push   ebp" /* high order word of EBP was getting hammered */ \
        "int    0ech" \
        "pop    ebp" \
        parm routine [di] [si] [ax] [dx] [bx] [cx] \
        value [ax] \
        modify [ax bx cx dx si di];

#endif

