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

#include <stddef.h>
#include "machtype.h"

/* Use 4-byte packing for compatibility with the default packing used by GCC */

#pragma pack( push, 4 )

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

/* This defines the structure used to read and write the entire
 * set of general purpose CPU registers using sys_ptrace().
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
    int     fs;
    int     gs;
    long    orig_eax;
    long    eip;
    int     cs;
    long    eflags;
    long    esp;
    int     ss;
} user_regs_struct;

/* This defines the structure used to read and write all the floating
 * point registers using sys_ptrace().
 */
typedef struct {
    long    cwd;
    long    swd;
    long    twd;
    long    fip;
    long    fcs;
    long    foo;
    long    fos;
    long    st_space[20];   /* 8*10 bytes for each FP-reg = 80 bytes */
} user_i387_struct;

/* This defines the structure used to read and write all the extended
 * floating point registers using sys_ptrace().
 */
typedef struct {
    u_short cwd;
    u_short swd;
    u_short twd;
    u_short fop;
    long    fip;
    long    fcs;
    long    foo;
    long    fos;
    long    mxcsr;
    long    reserved;
    long    st_space[32];   /* 8*16 bytes for each FP-reg = 128 bytes */
    long    xmm_space[32];  /* 8*16 bytes for each XMM-reg = 128 bytes */
    long    padding[56];
} user_fxsr_struct;

/* This defines the user structure that we can read and write using the
 * sys_ptrace() system call. This allows us to adjust the CPU registers,
 * FPU registers, debug registers and find out information about the
 * running process in memory. This is also the structure that heads up
 * a core dump when the kernel dumps core for a faulting process.
 */
typedef struct {
  user_regs_struct  regs;           /* Where the registers are actually stored */
  int               u_fpvalid;      /* True if math co-processor being used. Not yet used. */
  user_i387_struct  i387;           /* Math Co-processor registers. */
  u_long            u_tsize;        /* Text segment size (pages). */
  u_long            u_dsize;        /* Data segment size (pages). */
  u_long            u_ssize;        /* Stack segment size (pages). */
  u_long            start_code;     /* Starting virtual address of text. */
  u_long            start_stack;    /* Starting virtual address of stack area (bottom). */
  long              signal;         /* Signal that caused the core dump. */
  int               reserved;       /* No longer used */
  void              *u_ar0;         /* Used by gdb to help find the values for */
  user_i387_struct  *u_fpstate;     /* Math Co-processor pointer. */
  u_long            magic;          /* To uniquely identify a core file */
  char              u_comm[32];     /* User command that was responsible */
  int               u_debugreg[8];  /* Hardware debug registers */
} user_struct;

/* Define macros to get the offset of debug registers in user structure */

#define O_DEBUGREG(r)   offsetof(user_struct,u_debugreg[r])

/* Structure used internally to set hardware watch points */

typedef struct {
    addr48_ptr  loc;
    u_long      value;
    u_long      linear;
    u_short     len;
    u_short     dregs;
} watch_point;


#define MAX_WP          32

#define TRACE_BIT       0x100
#define BRK_POINT       0xCC

/* Rendezvous structure for communication between the dynamic linker and
 * the debugger. If executable's .dynamic section contains a DT_DEBUG element,
 * the dynamic linker sets it to point to this structure.
 */

struct r_debug {
    int                 r_version;    /* Protocol version */
    struct link_map     *r_map;       /* Start of loaded objects list */
    Elf32_Addr          r_brk;        /* Address to set a breakpoint at */
    enum {
        RT_CONSISTENT,                /* Mapping change is complete */
        RT_ADD,                       /* Loading new shared object */
        RT_DELETE                     /* Removing a shared object */
    }                   r_state;      /* Current state of dynamic linker */
    Elf32_Addr          r_ldbase;     /* Dynamic linker's load base */
};

/* Structure describing a loaded shared object - part of a doubly linked list */
struct link_map {
    Elf32_Addr          l_addr;       /* Offset from object's va to load base */
    char                *l_name;      /* Name (absolute path) of shared object */
    Elf32_Dyn           *l_ld;        /* Shared object's dynamic section */
    struct link_map     *l_next;      /* Next entry in chain */
    struct link_map     *l_prev;      /* Previous entry in chain */
};

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
extern u_long   GetDR6( void );
extern void     ClearDebugRegs( void );
extern int      SetDebugRegs( void );
extern int      CheckWatchPoints( void );
extern int      GetLinkMap( struct link_map *, struct link_map * );
extern int      AddInitialLibs( struct link_map * );
extern int      AddOneLib( struct link_map * );
extern int      DelOneLib( struct link_map * );
extern void     AddProcess( void );
extern void     DelProcess( void );
extern char     *dbg_strcpy( char *, const char * );

extern void print_msg( const char *format, ... );

/* Copy of parent's environment */
extern char     **dbg_environ;

/* Internal debugging functions */

#ifdef DEBUG_OUT
extern void Out( const char *str );
extern void OutNum( unsigned long i );
#else
#define Out( x )
#define OutNum( x )
#endif

/* Global trap file vairables */

extern u_short          flatCS;
extern u_short          flatDS;
extern pid_t            pid;

#pragma pack( pop )

#endif  /* _LINUXCOMM_H */
