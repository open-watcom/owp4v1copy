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
* Description:  Table of x86 opcodes and corresponding decode routines.
*                only for extension 3DNow!, MMX, SSE, SSE2 and SSE3
*
****************************************************************************/

/*
 * It must be processed before standard instruction table in insx86.h.
 * It is opcode map without prefix processing.
 *
 */

/*
      Idx,              Name,           Opcode,         Mask,           Handler
*/

// AMD 3DNow! Instructions

inspick( femms,         "femms",        0x00000e0f,     0x0000ffff,     X86MMNoOp)
inspick( prefetch,      "prefetch",     0x00000d0f,     0x0018ffff,     X86ModRM_24)
inspick( prefetchw,     "prefetchw",    0x00080d0f,     0x0018ffff,     X86ModRM_24)

// 3DNow! Instructions with suffix

inspick( prefix3dnow,   "3dnow",        0x00000f0f,     0x0000ffff,     X86_3DNow)
inspick( pavgusb,       "pavgusb",      0x000000bf,     0x00000000,     NULL)
inspick( pf2id,         "pf2id",        0x0000001d,     0x00000000,     NULL)
inspick( pf2iw,         "pf2iw",        0x0000001c,     0x00000000,     NULL)
inspick( pfacc,         "pfacc",        0x000000ae,     0x00000000,     NULL)
inspick( pfadd,         "pfadd",        0x0000009e,     0x00000000,     NULL)
inspick( pfcmpeq,       "pfcmpeq",      0x000000b0,     0x00000000,     NULL)
inspick( pfcmpge,       "pfcmpge",      0x00000090,     0x00000000,     NULL)
inspick( pfcmpgt,       "pfcmpgt",      0x000000a0,     0x00000000,     NULL)
inspick( pfmax,         "pfmax",        0x000000a4,     0x00000000,     NULL)
inspick( pfmin,         "pfmin",        0x00000094,     0x00000000,     NULL)
inspick( pfmul,         "pfmul",        0x000000b4,     0x00000000,     NULL)
inspick( pfnacc,        "pfnacc",       0x0000008a,     0x00000000,     NULL)
inspick( pfpnacc,       "pfpnacc",      0x0000008e,     0x00000000,     NULL)
inspick( pfrcp,         "pfrcp",        0x00000096,     0x00000000,     NULL)
inspick( pfrcpit1,      "pfrcpit1",     0x000000a6,     0x00000000,     NULL)
inspick( pfrcpit2,      "pfrcpit2",     0x000000b6,     0x00000000,     NULL)
inspick( pfrsqit1,      "pfrsqit1",     0x000000a7,     0x00000000,     NULL)
inspick( pfrsqrt,       "pfrsqrt",      0x00000097,     0x00000000,     NULL)
inspick( pfsub,         "pfsub",        0x0000009a,     0x00000000,     NULL)
inspick( pfsubr,        "pfsubr",       0x000000aa,     0x00000000,     NULL)
inspick( pi2fd,         "pi2fd",        0x0000000d,     0x00000000,     NULL)
inspick( pi2fw,         "pi2fw",        0x0000000c,     0x00000000,     NULL)
inspick( pmulhrw,       "pmulhrw",      0x000000b7,     0x00000000,     NULL)
inspick( pswapd,        "pswapd",       0x000000bb,     0x00000000,     NULL)

// MMX Instructions

inspick( emms,          "emms",         0x0000770f,     0x0000ffff,     X86MMNoOp)
inspick( movd,          "movd",         0x00006e0f,     0x0000efff,     X86MMRegModRM32)
inspick( movq,          "movq",         0x00006f0f,     0x0000efff,     X86MMRegModRM64)
inspick( packsswb,      "packsswb",     0x0000630f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( packssdw,      "packssdw",     0x00006b0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( packuswb,      "packuswb",     0x0000670f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddb,         "paddb",        0x0000fc0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddw,         "paddw",        0x0000fd0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddd,         "paddw",        0x0000fe0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddsb,        "paddsb",       0x0000ec0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddsw,        "paddsw",       0x0000ed0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddusb,       "paddusb",      0x0000dc0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( paddusw,       "paddusw",      0x0000dd0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pand,          "pand",         0x0000db0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pandn,         "pandn",        0x0000df0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpeqb,       "pcmpeqb",      0x0000740f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpeqw,       "pcmpeqw",      0x0000750f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpeqd,       "pcmpeqd",      0x0000760f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpgtb,       "pcmpgtb",      0x0000640f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpgtw,       "pcmpgtw",      0x0000650f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pcmpgtd,       "pcmpgtd",      0x0000660f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pmaddwd,       "pmaddwd",      0x0000f50f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pmulhw,        "pmulhw",       0x0000e50f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pmullw,        "pmullw",       0x0000d50f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( por,           "por",          0x0000eb0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psllw,         "psllw",        0x0000f10f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pslld,         "pslld",        0x0000f20f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psllq,         "psllq",        0x0000f30f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psllw2,        "psllw",        0x00f0710f,     0x00f8ffff,     X86MMImmMM)
inspick( pslld2,        "pslld",        0x00f0720f,     0x00f8ffff,     X86MMImmMM)
inspick( psllq2,        "psllq",        0x00f0730f,     0x00f8ffff,     X86MMImmMM)
inspick( psraw,         "psraw",        0x0000e10f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psrad,         "psrad",        0x0000e20f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psraw2,        "psraw",        0x00e0710f,     0x00f0ffff,     X86MMImmMM)
inspick( psrad2,        "psrad",        0x00e0720f,     0x00f0ffff,     X86MMImmMM)
inspick( psrlw,         "psrlw",        0x0000d10f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psrld,         "psrld",        0x0000d20f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psrlq,         "psrlq",        0x0000d30f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psrlw2,        "psrlw",        0x00d0710f,     0x00f0ffff,     X86MMImmMM)
inspick( psrld2,        "psrld",        0x00d0720f,     0x00f0ffff,     X86MMImmMM)
inspick( psrlq2,        "psrlq",        0x00d0730f,     0x00f0ffff,     X86MMImmMM)
inspick( psubb,         "psubb",        0x0000f80f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubw,         "psubw",        0x0000f90f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubd,         "psubd",        0x0000fa0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubsb,        "psubsb",       0x0000e80f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubsw,        "psubsw",       0x0000e90f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubusb,       "psubusb",      0x0000d80f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psubusw,       "psubusw",      0x0000d90f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpckhbw,     "punpckhbw",    0x0000680f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpckhwd,     "punpckhwd",    0x0000690f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpckhdq,     "punpckhdq",    0x00006a0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpcklbw,     "punpcklbw",    0x0000600f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpcklwd,     "punpcklwd",    0x0000610f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( punpckldq,     "punpckldq",    0x0000620f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pxor,          "pxor",         0x0000ef0f,     0x0000ffff,     X86MMRegModRM64_B)

// SSE/MMX extended instruction

inspick( fxrstor,       "fxrstor",      0x0008ae0f,     0x0038ffff,     X86ModRM_24)
inspick( fxsave,        "fxsave",       0x0000ae0f,     0x0038ffff,     X86ModRM_24)
inspick( maskmovq,      "maskmovq",     0x0000f70f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( movntq,        "movntq",       0x0000e70f,     0x0000ffff,     X86MMRegModRM64_BX)
inspick( pavgb,         "pavgb",        0x0000e00f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pavgw,         "pavgw",        0x0000e30f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pextrw,        "pextrw",       0x0000c50f,     0x0000ffff,     X86MMRegModRM64_BImm_R)
inspick( pinsrw,        "pinsrw",       0x0000c40f,     0x0000ffff,     X86MMRegModRM64_BImm_RX)
inspick( pmaxsw,        "pmaxsw",       0x0000ee0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pmaxub,        "pmaxub",       0x0000de0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pminsw,        "pminsw",       0x0000ea0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pminub,        "pminub",       0x0000da0f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pmovmskb,      "pmovmskb",     0x0000d70f,     0x0000ffff,     X86MMRegModRM64_R)
inspick( pmulhuw,       "pmulhuw",      0x0000e40f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( psadbw,        "psadbw",       0x0000f60f,     0x0000ffff,     X86MMRegModRM64_B)
inspick( pshufw,        "pshufw",       0x0000700f,     0x0000ffff,     X86MMRegModRM64_BImm)
inspick( prefetchnta,   "prefetchnta",  0x0000180f,     0x0018ffff,     X86ModRM_24)
inspick( prefetcht0,    "prefetcht0",   0x0008180f,     0x0018ffff,     X86ModRM_24)
inspick( prefetcht1,    "prefetcht1",   0x0010180f,     0x0018ffff,     X86ModRM_24)
inspick( prefetcht2,    "prefetcht2",   0x0018180f,     0x0018ffff,     X86ModRM_24)
inspick( sfence,        "sfence",       0x00f8ae0f,     0x00ffffff,     X86XMMNoOp)

// SSE Instructions

inspick( addps,         "addps",        0x0000580f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( addsd,         "addsd",        0x00580ff2,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( andnps,        "andnps",       0x0000550f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( andps,         "andps",        0x0000540f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( cmpps,         "cmpps",        0x0000c20f,     0x0000ffff,     X86XMMRegModRM128_BImm)
inspick( cmpss,         "cmpss",        0x00c20ff3,     0x00ffffff,     X86XMMRegModRM128_BImm)
inspick( comiss,        "comiss",       0x00002f0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( cvtpi2ps,      "cvtpi2ps",     0x00002a0f,     0x0000ffff,     X86XMMRegModRM64_B)
inspick( cvtps2pi,      "cvtps2pi",     0x00002d0f,     0x0000ffff,     X86XMMRegModRM64_B)
inspick( cvtsd2ss,      "cvtsd2ss",     0x005a0ff2,     0x00ffffff,     X86XMMRegModRM64_B)
inspick( cvtsi2ss,      "cvtsi2ss",     0x002a0ff3,     0x00ffffff,     X86XMMRegModRM64_B)
inspick( cvtss2si,      "cvtss2si",     0x002d0ff3,     0x00ffffff,     X86XMMRegModRM64)
inspick( cvttps2pi,     "cvttps2pi",    0x00002c0f,     0x0000ffff,     X86XMMRegModRM64_B)
inspick( cvttss2si,     "cvttss2si",    0x002c0ff3,     0x00ffffff,     X86XMMRegModRM64)
inspick( divps,         "divps",        0x00005e0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( divss,         "divss",        0x005e0ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( ldmxcsr,       "ldmxcsr",      0x0010ae0f,     0x0038ffff,     X86ModRM_24)
inspick( maxps,         "maxps",        0x00005f0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( maxss,         "maxss",        0x005f0ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( minps,         "minps",        0x00005d0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( minss,         "minss",        0x005d0ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( movaps,        "movaps",       0x0000280f,     0x0000feff,     X86XMMRegModRM128)
inspick( movhlps,       "movhlps",      0x00c0120f,     0x00c0ffff,     X86XMMRegModRM128_B)
inspick( movhps,        "movhps",       0x0000160f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movhps2,       "movhps",       0x0040160f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movhps3,       "movhps",       0x0080160f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movlhps,       "movlhps",      0x00c0160f,     0x00c0ffff,     X86XMMRegModRM128_B)
inspick( movlps,        "movlps",       0x0000120f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movlps2,       "movlps",       0x0040120f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movlps3,       "movlps",       0x0080120f,     0x00c0feff,     X86XMMRegModRM64)
inspick( movmskps,      "movmskps",     0x00c0500f,     0x00c0ffff,     X86XMMRegModRM64)
inspick( movss,         "movss",        0x00100ff3,     0x00feffff,     X86XMMRegModRM128)
inspick( movups,        "movups",       0x0000100f,     0x0000feff,     X86XMMRegModRM128)
inspick( mulps,         "mulps",        0x0000590f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( mulss,         "mulss",        0x00590ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( orps,          "orps",         0x0000560f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( rcpps,         "rcpps",        0x0000530f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( rcpss,         "rcpss",        0x00530ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( rsqrtps,       "rsqrtps",      0x0000520f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( rsqrtss,       "rsqrtss",      0x00520ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( shufps,        "shufps",       0x0000c60f,     0x0000ffff,     X86XMMRegModRM128_BImm)
inspick( sqrtps,        "sqrtps",       0x0000510f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( sqrtss,        "sqrtss",       0x00510ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( stmxcsr,       "stmxcsr",      0x0018ae0f,     0x0038ffff,     X86ModRM_24)
inspick( subps,         "subps",        0x00005c0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( subss,         "subss",        0x005c0ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( ucomiss,       "ucomiss",      0x00002e0f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( unpckhps,      "unpckhps",     0x0000150f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( unpcklps,      "unpcklps",     0x0000140f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( xorps,         "xorps",        0x0000570f,     0x0000ffff,     X86XMMRegModRM128_B)
inspick( movntps,       "movntps",      0x00002b0f,     0x0000ffff,     X86XMMRegModRM128_B)

// SSE2 Instructions

inspick( addss,         "addss",        0x00580ff3,     0x00ffffff,     X86XMMRegModRM128_B)
inspick( cvttsd2si,     "cvttsd2si",    0x002c0ff2,     0x00ffffff,     X86XMMRegModRM64)

