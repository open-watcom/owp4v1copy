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
* Description:  Special access routines spliced into the debuggee. See
*               pgmexec.c for description.
*
****************************************************************************/


#include <stddef.h>
#include <string.h>
#include <i86.h>
#define INCL_BASE
#define INCL_DOSDEVICES
#define INCL_DOSMEMMGR
#define INCL_DOSSIGNALS
#include <os2.h>

/* We need separate stack for executing spliced code. We really wouldn't
 * want to mess up debuggee's stack!
 */
char __export TempStack[32768];

void        BreakPoint(ULONG);
#pragma aux BreakPoint = 0xCC parm [eax] aborts;

void doReadWord(void);
#pragma aux doReadWord =           \
    "mov  ax, word ptr gs:[ebx]"   \
    "int  3";

void __export DoReadWord(void)
{
    doReadWord();
}

void doWriteWord(void);
#pragma aux doWriteWord =          \
    "mov  word ptr gs:[ebx], ax"   \
    "int  3";

void __export DoWriteWord(void)
{
    doWriteWord();
}

void __export DoOpen(char *name, int mode, int flags)
{
//  BreakPoint(OpenFile(name, mode, flags));
    BreakPoint(0);
}

void __export DoClose(HFILE hdl)
{
    BreakPoint(DosClose(hdl));
}

void __export DoDupFile(HFILE old, HFILE new)
{
    HFILE       new_t;
    USHORT      rc;

    new_t = new;
    rc = DosDupHandle(old, &new_t);
    if (rc != 0) {
        BreakPoint((HFILE)-1);
    } else {
        BreakPoint(new_t);
    }
}
