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
* Description:  Internal header file for the Linux standard debugger
*               trap file.
*
****************************************************************************/

#ifndef _LINUXCOMM_H
#define _LINUXCOMM_H

/* Use 4-byte packing for compatibility with the default packing used by GCC */

#pragma pack(__push,4);

/* Linux PTRACE support defines */

#define PTRACE_TRACEME      0   /* Enable tracing for process */
#define PTRACE_PEEKTEXT     1   /* Read text memory in traced process */
#define PTRACE_PEEKDATA     2   /* Read data memory in traced process */
#define PTRACE_PEEKUSER     3   /* Read user area structure (CPU regs etc) */
#define PTRACE_POKETEXT     4   /* Write text memory in traced process */
#define PTRACE_POKEDATA     5   /* Write data memory in traced process */
#define PTRACE_POKEUSER     6   /* Write user area structure (CPU regs etc) */
#define PTRACE_CONT         7   /* Continue the process */
#define PTRACE_KILL         8   /* Kill the process */
#define PTRACE_SINGLESTEP   9   /* Single step the process */
#define PTRACE_GETREGS      12  /* Get all general purpose registers */
#define PTRACE_SETREGS      13  /* Set all general purpose registers */
#define PTRACE_GETFPREGS    14  /* Get all floating point registers */
#define PTRACE_SETFPREGS    15  /* Set all floating point registers */
#define PTRACE_ATTACH       16  /* Attached to running process */
#define PTRACE_DETACH       17  /* Detach from running process */
#define PTRACE_GETFPXREGS   18  /* Get all extended FPU registers */
#define PTRACE_SETFPXREGS   19  /* Set all extended FPU registers */
#define PTRACE_SETOPTIONS   21  /* Set special ptrace options */
#define PTRACE_SYSCALL      24  /* Continue and stop at next syscall */

/* Options set using PTRACE_SETOPTIONS */
#define PTRACE_O_TRACESYSGOOD   0x00000001

/* This struct defines the way the registers are stored on the
 * stack during a system call.
 */
typedef struct {
    long    ebx;
    long    ecx;
    long    edx;
    long    esi;
    long    edi;
    long    ebp;
    long    eax;
    int     ds;
    int     es;
    long    orig_eax;
    long    eip;
    int     cs;
    long    eflags;
    long    esp;
    int     ss;
} pt_regs;

/* Offsets to registers in the above structure */

#define O_EBX               0
#define O_ECX               1
#define O_EDX               2
#define O_ESI               3
#define O_EDI               4
#define O_EBP               5
#define O_EAX               6
#define O_DS                7
#define O_ES                8
#define O_FS                9
#define O_GS                10
#define O_ORIG_EAX          11
#define O_EIP               12
#define O_CS                13
#define O_EFL               14
#define O_UESP              15
#define O_SS                16
#define O_FRAME_SIZE        17

/* ptrace system call */

u_long sys_ptrace(u_long request, u_long pid, u_long addr, void *data);
#pragma aux sys_ptrace =                        \
    "mov    eax,26"                             \
    "int    0x80"                               \
    parm [ebx] [ecx] [edx] [esi]                \
    value [eax];

/* Direct I/O port access functions */

void outpb(u_long port,u_char val);
#pragma aux outpb =                 \
    "out    dx,al"                  \
    parm [edx] [eax]                \
    modify exact [];

void outpw(u_long port,u_short val);
#pragma aux outpw =                 \
    "out    dx,ax"                  \
    parm [edx] [eax]                \
    modify exact [];

void outpd(u_long port,u_long val);
#pragma aux outpd =                 \
    "out    dx,eax"                 \
    parm [edx] [eax]                \
    modify exact [];

u_char inpb(u_long port);
#pragma aux inpb =                  \
    "in     al,dx"                  \
    parm [edx]                      \
    value [al]                      \
    modify exact [al];

u_short inpw(u_long port);
#pragma aux inpw =                  \
    "in     ax,dx"                  \
    parm [edx]                      \
    value [ax]                      \
    modify exact [ax];

u_long inpd(u_long port);
#pragma aux inpd =                  \
    "in     eax,dx"                 \
    parm [edx]                      \
    value [eax]                     \
    modify exact [eax];

/* Internal helper functions */

extern unsigned TryOnePath( char *, struct stat *, char *, char * );
extern unsigned FindFilePath( int, char *, char * );
extern void print_msg( const char *format, ... );

#pragma pack(__pop);

#endif  /* _LINUXCOMM_H */

