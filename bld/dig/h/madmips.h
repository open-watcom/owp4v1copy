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
* Description:  MIPS specific register structure for MAD.
*
****************************************************************************/


#ifndef MADMIPS_H
#define MADMIPS_H
#include "digpck.h"
enum mips_cputypes {
    MIPS_DUNNO,
    MIPS_R2000,
    MIPS_R3000,
    MIPS_R4000,
    MIPS_R5000
};

typedef union {
    unsigned_64         u64;
    lreal               f;
} mipsfloat;

#if 0
enum {
    MR_f0,
    MR_f1,
    MR_f2,
    MR_f3,
    MR_f4,
    MR_f5,
    MR_f6,
    MR_f7,
    MR_f8,
    MR_f9,
    MR_f10,
    MR_f11,
    MR_f12,
    MR_f13,
    MR_f14,
    MR_f15,
    MR_f16,
    MR_f17,
    MR_f18,
    MR_f19,
    MR_f20,
    MR_f21,
    MR_f22,
    MR_f23,
    MR_f24,
    MR_f25,
    MR_f26,
    MR_f27,
    MR_f28,
    MR_f29,
    MR_f30,
    MR_f31,

    MR_r0,
    MR_r1,
    MR_r2,
    MR_r3,
    MR_r4,
    MR_r5,
    MR_r6,
    MR_r7,
    MR_r8,
    MR_r9,
    MR_r10,
    MR_r11,
    MR_r12,
    MR_r13,
    MR_r14,
    MR_r15,
    MR_r16,
    MR_r17,
    MR_r18,
    MR_r19,
    MR_r20,
    MR_r21,
    MR_r22,
    MR_r23,
    MR_r24,
    MR_r25,
    MR_r26,
    MR_r27,
    MR_r28,
    MR_r29,
    MR_r30,
    MR_r31,

    MR_pc,
    MR_lo,
    MR_hi,

    MR_NUM_REGS,

    MR_zero     = MR_r0,
    MR_at       = MR_r1,
    MR_v0       = MR_r2,
    MR_v1       = MR_r3,
    MR_a0       = MR_r4,
    MR_a1       = MR_r5,
    MR_a2       = MR_r6,
    MR_a3       = MR_r7,
    MR_t0       = MR_r8,
    MR_t1       = MR_r9,
    MR_t2       = MR_r10,
    MR_t3       = MR_r11,
    MR_t4       = MR_r12,
    MR_t5       = MR_r13,
    MR_t6       = MR_r14,
    MR_t7       = MR_r15,
    MR_s0       = MR_r16,
    MR_s1       = MR_r17,
    MR_s2       = MR_r18,
    MR_s3       = MR_r19,
    MR_s4       = MR_r20,
    MR_s5       = MR_r21,
    MR_s6       = MR_r22,
    MR_s7       = MR_r23,
    MR_t8       = MR_r24,
    MR_t9       = MR_r25,
    MR_k0       = MR_r26,
    MR_k1       = MR_r27,
    MR_gp       = MR_r28,
    MR_sp       = MR_r29,
    MR_s8       = MR_r30,
    MR_ra       = MR_r31
};
#endif

struct mips_mad_registers {
    union {
        unsigned_64     r0;
        unsigned_64     zero;
    };
    union {
        unsigned_64     r1;
        unsigned_64     at;
    };
    union {
        unsigned_64     r2;
        unsigned_64     v0;
    };
    union {
        unsigned_64     r3;
        unsigned_64     v1;
    };
    union {
        unsigned_64     r4;
        unsigned_64     a0;
    };
    union {
        unsigned_64     r5;
        unsigned_64     a1;
    };
    union {
        unsigned_64     r6;
        unsigned_64     a2;
    };
    union {
        unsigned_64     r7;
        unsigned_64     a3;
    };
    union {
        unsigned_64     r8;
        unsigned_64     t0;
    };
    union {
        unsigned_64     r9;
        unsigned_64     t1;
    };
    union {
        unsigned_64     r10;
        unsigned_64     t2;
    };
    union {
        unsigned_64     r11;
        unsigned_64     t3;
    };
    union {
        unsigned_64     r12;
        unsigned_64     t4;
    };
    union {
        unsigned_64     r13;
        unsigned_64     t5;
    };
    union {
        unsigned_64     r14;
        unsigned_64     t6;
    };
    union {
        unsigned_64     r15;
        unsigned_64     t7;
    };
    union {
        unsigned_64     r16;
        unsigned_64     s0;
    };
    union {
        unsigned_64     r17;
        unsigned_64     s1;
    };
    union {
        unsigned_64     r18;
        unsigned_64     s2;
    };
    union {
        unsigned_64     r19;
        unsigned_64     s3;
    };
    union {
        unsigned_64     r20;
        unsigned_64     s4;
    };
    union {
        unsigned_64     r21;
        unsigned_64     s5;
    };
    union {
        unsigned_64     r22;
        unsigned_64     s6;
    };
    union {
        unsigned_64     r23;
        unsigned_64     s7;
    };
    union {
        unsigned_64     r24;
        unsigned_64     t8;
    };
    union {
        unsigned_64     r25;
        unsigned_64     t9;
    };
    union {
        unsigned_64     r26;
        unsigned_64     k0;
    };
    union {
        unsigned_64     r27;
        unsigned_64     k1;
    };
    union {
        unsigned_64     r28;
        unsigned_64     gp;
    };
    union {
        unsigned_64     r29;
        unsigned_64     sp;
    };
    union {
        unsigned_64     r30;
        unsigned_64     s8;
    };
    union {
        unsigned_64     r31;
        unsigned_64     ra;
    };

    unsigned_64 pc;
    unsigned_32 lo;
    unsigned_32 hi;

    mipsfloat   f0;
    mipsfloat   f1;
    mipsfloat   f2;
    mipsfloat   f3;
    mipsfloat   f4;
    mipsfloat   f5;
    mipsfloat   f6;
    mipsfloat   f7;
    mipsfloat   f8;
    mipsfloat   f9;
    mipsfloat   f10;
    mipsfloat   f11;
    mipsfloat   f12;
    mipsfloat   f13;
    mipsfloat   f14;
    mipsfloat   f15;
    mipsfloat   f16;
    mipsfloat   f17;
    mipsfloat   f18;
    mipsfloat   f19;
    mipsfloat   f20;
    mipsfloat   f21;
    mipsfloat   f22;
    mipsfloat   f23;
    mipsfloat   f24;
    mipsfloat   f25;
    mipsfloat   f26;
    mipsfloat   f27;
    mipsfloat   f28;
    mipsfloat   f29;
    mipsfloat   f30;
    mipsfloat   f31;
};

#include "digunpck.h"
#endif
