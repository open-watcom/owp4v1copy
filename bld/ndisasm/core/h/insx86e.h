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

inspick( femms,         "femms",        0x00000e0f,     0x0000ffff,     X86NoOp_16)
inspick( prefetch,      "prefetch",     0x00000d0f,     0x0018ffff,     X86ModRM_24)
inspick( prefetchw,     "prefetchw",    0x00080d0f,     0x0018ffff,     X86ModRM_24)

// AMD 3DNow! Instructions with suffix

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


// Intel MMX, SSE, SSE2 and SSE3 instructions

inspick( addpd,		"addpd",	0x00580f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( addps,		"addps",	0x0000580f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( addsd,		"addsd",	0x00580ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( addss,		"addss",	0x00580ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( addsubpd,	"addsubpd",	0x00d00f66,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( addsubps,	"addsubps",	0x00d00ff2,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( andnpd,	"andnpd",	0x00550f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( andnps,	"andnps",	0x0000550f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( andpd,		"andpd",	0x00540f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( andps,		"andps",	0x0000540f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( clflush00,	"clflush",	0x0038ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse2	
inspick( clflush01,	"clflush",	0x0078ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse2	
inspick( clflush10,	"clflush",	0x00b8ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse2	
inspick( cmppd,		"cmppd",	0x00c20f66,	0x00ffffff,	X86XMMRegModRMImm ) 	// sse2	
inspick( cmpps,		"cmpps",	0x0000c20f,	0x0000ffff,	X86XMMRegModRMImm ) 	// sse	
inspick( cmpsd,		"cmpsd",	0x00c20ff2,	0x00ffffff,	X86XMMRegModRM64Imm ) 	// sse2	
inspick( cmpss,		"cmpss",	0x00c20ff3,	0x00ffffff,	X86XMMRegModRM32Imm ) 	// sse	
inspick( comisd,	"comisd",	0x002f0f66,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( comiss,	"comiss",	0x00002f0f,	0x0000ffff,	X86XMMRegModRM32 )	// sse	
inspick( cvtdq2pd,	"cvtdq2pd",	0x00e60ff3,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( cvtdq2ps,	"cvtdq2ps",	0x00005b0f,	0x0000ffff,	X86XMMRegModRM )	// sse2	
inspick( cvtpd2dq,	"cvtpd2dq",	0x00e60ff2,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( cvtpd2pi,	"cvtpd2pi",	0x002d0f66,	0x00ffffff,	X86MMRegModRMMixed )	// sse2	
inspick( cvtpd2ps,	"cvtpd2ps",	0x005a0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( cvtpi2pd,	"cvtpi2pd",	0x002a0f66,	0x00ffffff,	X86XMMRegModRMMixed )	// sse2	
inspick( cvtpi2ps,	"cvtpi2ps",	0x00002a0f,	0x0000ffff,	X86XMMRegModRMMixed )	// sse	
inspick( cvtps2dq,	"cvtps2dq",	0x005b0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( cvtps2pd,	"cvtps2pd",	0x00005a0f,	0x0000ffff,	X86XMMRegModRM64 )	// sse2	
inspick( cvtps2pi,	"cvtps2pi",	0x00002d0f,	0x0000ffff,	X86MMRegModRMMixed )	// sse	
inspick( cvtsd2si,	"cvtsd2si",	0x002d0ff2,	0x00ffffff,	X86RegModRMMixed )	// sse2	
inspick( cvtsd2ss,	"cvtsd2ss",	0x005a0ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( cvtsi2sd,	"cvtsi2sd",	0x002a0ff2,	0x00ffffff,	X86XMMRegModRMMixed )	// sse2	
inspick( cvtsi2ss,	"cvtsi2ss",	0x002a0ff3,	0x00ffffff,	X86XMMRegModRMMixed )	// sse	
inspick( cvtss2sd,	"cvtss2sd",	0x005a0ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse2	
inspick( cvtss2si,	"cvtss2si",	0x002d0ff3,	0x00ffffff,	X86RegModRMMixed )	// sse	
inspick( cvttpd2dq,	"cvttpd2dq",	0x00e60f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( cvttpd2pi,	"cvttpd2pi",	0x002c0f66,	0x00ffffff,	X86MMRegModRMMixed )	// sse2	
inspick( cvttps2dq,	"cvttps2dq",	0x005b0ff3,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( cvttps2pi,	"cvttps2pi",	0x00002c0f,	0x0000ffff,	X86MMRegModRMMixed )	// sse	
inspick( cvttsd2si,	"cvttsd2si",	0x002c0ff2,	0x00ffffff,	X86RegModRMMixed )	// sse2	
inspick( cvttss2si,	"cvttss2si",	0x002c0ff3,	0x00ffffff,	X86RegModRMMixed )	// sse	
inspick( divpd,		"divpd",	0x005e0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( divps,		"divps",	0x00005e0f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( divsd,		"divsd",	0x005e0ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( divss,		"divss",	0x005e0ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( emms,		"emms",		0x0000770f,	0x0000ffff,	X86NoOp_16 )		// mmx	
//inspick( fisttp00,	"fisttp",	0x000008db,	0x0000f8fb,	X86FType2 )		// sse3	
//inspick( fisttp01,	"fisttp",	0x000048db,	0x0000f8fb,	X86FType2 )		// sse3	
//inspick( fisttp10,	"fisttp",	0x000088db,	0x0000f8fb,	X86FType2 )		// sse3	
//inspick( fisttp6400,	"fisttp64",	0x000008dd,	0x0000f8ff,	X86FType1 )		// sse3	
//inspick( fisttp6401,	"fisttp64",	0x000048dd,	0x0000f8ff,	X86FType1 )		// sse3	
//inspick( fisttp6410,	"fisttp64",	0x000088dd,	0x0000f8ff,	X86FType1 )		// sse3	
//inspick( fxrstor00,	"fxrstor",	0x0008ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
//inspick( fxrstor01,	"fxrstor",	0x0048ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
//inspick( fxrstor10,	"fxrstor",	0x0088ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
//inspick( fxsave00,	"fxsave",	0x0000ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
//inspick( fxsave01,	"fxsave",	0x0040ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
//inspick( fxsave10,	"fxsave",	0x0080ae0f,	0x00f8ffff,	X86ModRM_24 )		// mmx	
inspick( haddpd,	"haddpd",	0x007c0f66,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( haddps,	"haddps",	0x007c0ff2,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( hsubpd,	"hsubpd",	0x007d0f66,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( hsubps,	"hsubps",	0x007d0ff2,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( lddqu00,	"lddqu",	0x00f00ff2,	0xc0ffffff,	X86XMMRegModRM )	// sse3	
inspick( lddqu01,	"lddqu",	0x40f00ff2,	0xc0ffffff,	X86XMMRegModRM )	// sse3	
inspick( lddqu10,	"lddqu",	0x80f00ff2,	0xc0ffffff,	X86XMMRegModRM )	// sse3	
inspick( ldmxcsr00,	"ldmxcsr",	0x0010ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( ldmxcsr01,	"ldmxcsr",	0x0050ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( ldmxcsr10,	"ldmxcsr",	0x0090ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( lfence,	"lfence",	0x00e8ae0f,	0x00ffffff,	X86NoOp_24 )		// sse2	
inspick( maskmovdqu,	"maskmovdqu",	0xc0f70f66,	0xc0ffffff,	X86XMMRegModRM )	// sse2	
inspick( maskmovq,	"maskmovq",	0x00c0f70f,	0x00c0ffff,	X86MMRegModRM )		// sse	
inspick( maxpd,		"maxpd",	0x005f0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( maxps,		"maxps",	0x00005f0f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( maxsd,		"maxsd",	0x005f0ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( maxss,		"maxss",	0x005f0ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( mfence,	"mfence",	0x00f0ae0f,	0x00ffffff,	X86NoOp_24 )		// sse2	
inspick( minpd,		"minpd",	0x005d0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( minps,		"minps",	0x00005d0f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( minsd,		"minsd",	0x005d0ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( minss,		"minss",	0x005d0ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( monitor,	"monitor",	0x00c8010f,	0x00ffffff,	X86NoOp_24 )		// sse3	
inspick( movapd_1,	"movapd",	0x00280f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( movapd_2,	"movapd",	0x00290f66,	0x00ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movaps_1,	"movaps",	0x0000280f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( movaps_2,	"movaps",	0x0000290f,	0x0000ffff,	X86XMMRegModRM_Rev ) 	// sse	
inspick( movd_1,	"movd",		0x00006e0f,	0x0000ffff,	X86MMRegModRMMixed )	// mmx	
inspick( movd_2,	"movd",		0x006e0f66,	0x00ffffff,	X86XMMRegModRMMixed )	// sse2	
inspick( movd_3,	"movd",		0x00007e0f,	0x0000ffff,	X86MMRegModRMMixed_Rev ) 	// mmx	
inspick( movd_4,	"movd",		0x007e0f66,	0x00ffffff,	X86XMMRegModRMMixed_Rev ) 	// sse2	
inspick( movddup,	"movddup",	0x00120ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse3	
inspick( movdq2q,	"movdq2q",	0xc0d60ff2,	0xc0ffffff,	X86MMRegModRMMixed )	// sse2	
inspick( movdqa_1,	"movdqa",	0x006f0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( movdqa_2,	"movdqa",	0x007f0f66,	0x00ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movdqu_1,	"movdqu",	0x006f0ff3,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( movdqu_2,	"movdqu",	0x007f0ff3,	0x00ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movhpd00_1,	"movhpd",	0x00160f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movhpd01_1,	"movhpd",	0x40160f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movhpd10_1,	"movhpd",	0x80160f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movhpd00_2,	"movhpd",	0x00170f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movhpd01_2,	"movhpd",	0x40170f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movhpd10_2,	"movhpd",	0x80170f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movhps00_1,	"movhps",	0x0000160f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movhps01_1,	"movhps",	0x0040160f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movhps10_1,	"movhps",	0x0080160f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movlhps,	"movlhps",	0x00c0160f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movhps00_2,	"movhps",	0x0000170f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movhps01_2,	"movhps",	0x0040170f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movhps10_2,	"movhps",	0x0080170f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movlpd00_1,	"movlpd",	0x00120f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movlpd01_1,	"movlpd",	0x40120f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movlpd10_1,	"movlpd",	0x80120f66,	0xc0ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movlpd00_2,	"movlpd",	0x00130f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movlpd01_2,	"movlpd",	0x40130f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movlpd10_2,	"movlpd",	0x80130f66,	0xc0ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movlps00_1,	"movlps",	0x0000120f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movlps01_1,	"movlps",	0x0040120f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movlps10_1,	"movlps",	0x0080120f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movhlps,	"movhlps",	0x00c0120f,	0x00f8ffff,	X86XMMRegModRM64 )	// sse	
inspick( movlps00_2,	"movlps",	0x0000130f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movlps01_2,	"movlps",	0x0040130f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movlps10_2,	"movlps",	0x0080130f,	0x00c0ffff,	X86XMMRegModRM64_Rev ) 	// sse	
inspick( movmskpd,	"movmskpd",	0xc0500f66,	0xc0ffffff,	X86RegModRMMixed )	// sse2	
inspick( movmskps,	"movmskps",	0x00c0500f,	0x00c0ffff,	X86RegModRMMixed )	// sse	
inspick( movntdq00,	"movntdq",	0x00e70f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movntdq01,	"movntdq",	0x40e70f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movntdq10,	"movntdq",	0x80e70f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movnti00,	"movnti",	0x0000c30f,	0x00c0ffff,	X86RegModRM32_Rev ) 	// sse2	
inspick( movnti01,	"movnti",	0x0040c30f,	0x00c0ffff,	X86RegModRM32_Rev ) 	// sse2	
inspick( movnti10,	"movnti",	0x0080c30f,	0x00c0ffff,	X86RegModRM32_Rev ) 	// sse2	
inspick( movntpd00,	"movntpd",	0x002b0f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movntpd01,	"movntpd",	0x402b0f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movntpd10,	"movntpd",	0x802b0f66,	0xc0ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movntps00,	"movntps",	0x00002b0f,	0x00c0ffff,	X86XMMRegModRM_Rev ) 	// sse	
inspick( movntps01,	"movntps",	0x00402b0f,	0x00c0ffff,	X86XMMRegModRM_Rev ) 	// sse	
inspick( movntps10,	"movntps",	0x00802b0f,	0x00c0ffff,	X86XMMRegModRM_Rev ) 	// sse	
inspick( movntq00,	"movntq",	0x0000e70f,	0x00c0ffff,	X86MMRegModRM_Rev ) 	// sse	
inspick( movntq01,	"movntq",	0x0040e70f,	0x00c0ffff,	X86MMRegModRM_Rev ) 	// sse	
inspick( movntq10,	"movntq",	0x0080e70f,	0x00c0ffff,	X86MMRegModRM_Rev ) 	// sse	
inspick( movq_1,	"movq",		0x00006f0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( movq_2,	"movq",		0x007e0ff3,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movq_3,	"movq",		0x00007f0f,	0x0000ffff,	X86MMRegModRM_Rev ) 	// mmx	
inspick( movq_4,	"movq",		0x00d60f66,	0x00ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movq2dq,	"movq2dq",	0xc0d60ff3,	0xc0ffffff,	X86XMMRegModRMMixed )	// sse2	
inspick( movshdup,	"movshdup",	0x00160ff3,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( movsldup,	"movsldup",	0x00120ff3,	0x00ffffff,	X86XMMRegModRM )	// sse3	
inspick( movupd_1,	"movupd",	0x00100f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( movupd_2,	"movupd",	0x00110f66,	0x00ffffff,	X86XMMRegModRM_Rev ) 	// sse2	
inspick( movups_1,	"movups",	0x0000100f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( movups_2,	"movups",	0x0000110f,	0x0000ffff,	X86XMMRegModRM_Rev ) 	// sse	
inspick( movsd_1,	"movsd",	0x00100ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( movsd_2,	"movsd",	0x00110ff2,	0x00ffffff,	X86XMMRegModRM64_Rev ) 	// sse2	
inspick( movss_1,	"movss",	0x00100ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( movss_2,	"movss",	0x00110ff3,	0x00ffffff,	X86XMMRegModRM32_Rev ) 	// sse	
inspick( mulpd,		"mulpd",	0x00590f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( mulps,		"mulps",	0x0000590f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( mulsd,		"mulsd",	0x00590ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( mulss,		"mulss",	0x00590ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( mwait,		"mwait",	0x00c9010f,	0x00ffffff,	X86NoOp_24 )		// sse3	
inspick( orpd,		"orpd",		0x00560f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( orps,		"orps",		0x0000560f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( packssdw_1,	"packssdw",	0x00006b0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( packssdw_2,	"packssdw",	0x006b0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( packsswb_1,	"packsswb",	0x0000630f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( packsswb_2,	"packsswb",	0x00630f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( packuswb_1,	"packuswb",	0x0000670f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( packuswb_2,	"packuswb",	0x00670f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddb_1,	"paddb",	0x0000fc0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddb_2,	"paddb",	0x00fc0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddd_1,	"paddd",	0x0000fe0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddd_2,	"paddd",	0x00fe0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddq_1,	"paddq",	0x0000d40f,	0x0000ffff,	X86MMRegModRM )		// sse2	
inspick( paddq_2,	"paddq",	0x00d40f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddsb_1,	"paddsb",	0x0000ec0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddsb_2,	"paddsb",	0x00ec0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddsw_1,	"paddsw",	0x0000ed0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddsw_2,	"paddsw",	0x00ed0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddusb_1,	"paddusb",	0x0000dc0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddusb_2,	"paddusb",	0x00dc0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddusw_1,	"paddusw",	0x0000dd0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddusw_2,	"paddusw",	0x00dd0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( paddw_1,	"paddw",	0x0000fd0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( paddw_2,	"paddw",	0x00fd0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pand_1,	"pand",		0x0000db0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pand_2,	"pand",		0x00db0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pandn_1,	"pandn",	0x0000df0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pandn_2,	"pandn",	0x00df0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pause,		"pause",	0x000090f3,	0x0000ffff,	X86NoOp_16 )		// sse2	
inspick( pavgb_1,	"pavgb",	0x0000e00f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pavgb_2,	"pavgb",	0x00e00f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pavgw_1,	"pavgw",	0x0000e30f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pavgw_2,	"pavgw",	0x00e30f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpeqb_1,	"pcmpeqb",	0x0000740f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpeqb_2,	"pcmpeqb",	0x00740f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpeqd_1,	"pcmpeqd",	0x0000760f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpeqd_2,	"pcmpeqd",	0x00760f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpeqw_1,	"pcmpeqw",	0x0000750f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpeqw_2,	"pcmpeqw",	0x00750f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpgtb_1,	"pcmpgtb",	0x0000640f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpgtb_2,	"pcmpgtb",	0x00640f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpgtd_1,	"pcmpgtd",	0x0000660f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpgtd_2,	"pcmpgtd",	0x00660f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pcmpgtw_1,	"pcmpgtw",	0x0000650f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pcmpgtw_2,	"pcmpgtw",	0x00650f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pextrw_1,	"pextrw",	0x00c0c50f,	0x00c0ffff,	X86RegModRMMixedImm ) 	// sse	
inspick( pextrw_2,	"pextrw",	0xc0c50f66,	0xc0ffffff,	X86RegModRMMixedImm ) 	// sse2	
inspick( pinsrw00_1,	"pinsrw",	0x0000c40f,	0x00c0ffff,	X86MMRegModRMMixedImm ) 	// sse	
inspick( pinsrw01_1,	"pinsrw",	0x0040c40f,	0x00c0ffff,	X86MMRegModRMMixedImm ) 	// sse	
inspick( pinsrw10_1,	"pinsrw",	0x0080c40f,	0x00c0ffff,	X86MMRegModRMMixedImm ) 	// sse	
inspick( pinsrw11_1,	"pinsrw11",	0x00c0c40f,	0x00c0ffff,	X86MMRegModRMMixedImm ) 	// sse	
inspick( pinsrw00_2,	"pinsrw",	0x00c40f66,	0xc0ffffff,	X86XMMRegModRMMixedImm ) 	// sse2	
inspick( pinsrw01_2,	"pinsrw",	0x40c40f66,	0xc0ffffff,	X86XMMRegModRMMixedImm ) 	// sse2	
inspick( pinsrw10_2,	"pinsrw",	0x80c40f66,	0xc0ffffff,	X86XMMRegModRMMixedImm ) 	// sse2	
inspick( pinsrw11_2,	"pinsrw11",	0xc0c40f66,	0xc0ffffff,	X86XMMRegModRMMixedImm ) 	// sse2	
inspick( pmaddwd_1,	"pmaddwd",	0x0000f50f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pmaddwd_2,	"pmaddwd",	0x00f50f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmaxsw_1,	"pmaxsw",	0x0000ee0f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pmaxsw_2,	"pmaxsw",	0x00ee0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmaxub_1,	"pmaxub",	0x0000de0f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pmaxub_2,	"pmaxub",	0x00de0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pminsw_1,	"pminsw",	0x0000ea0f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pminsw_2,	"pminsw",	0x00ea0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pminub_1,	"pminub",	0x0000da0f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pminub_2,	"pminub",	0x00da0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmovmskb_1,	"pmovmskb",	0x00c0d70f,	0x00c0ffff,	X86RegModRMMixed )		// sse	
inspick( pmovmskb_2,	"pmovmskb",	0xc0d70f66,	0xc0ffffff,	X86RegModRMMixed )	// sse2	
inspick( pmulhuw_1,	"pmulhuw",	0x0000e40f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( pmulhuw_2,	"pmulhuw",	0x00e40f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmulhw_1,	"pmulhw",	0x0000e50f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pmulhw_2,	"pmulhw",	0x00e50f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmullw_1,	"pmullw",	0x0000d50f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pmullw_2,	"pmullw",	0x00d50f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pmuludq_1,	"pmuludq",	0x0000f40f,	0x0000ffff,	X86MMRegModRM )		// sse2	
inspick( pmuludq_2,	"pmuludq",	0x00f40f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( por_1,		"por",		0x0000eb0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( por_2,		"por",		0x00eb0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( prefetchnta00,	"prefetchnta",	0x0000180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetchnta01,	"prefetchnta",	0x0040180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetchnta10,	"prefetchnta",	0x0080180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht000,	"prefetcht0",	0x0008180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht001,	"prefetcht0",	0x0048180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht010,	"prefetcht0",	0x0088180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht100,	"prefetcht1",	0x0010180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht101,	"prefetcht1",	0x0050180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht110,	"prefetcht1",	0x0090180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht200,	"prefetcht2",	0x0018180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht201,	"prefetcht2",	0x0058180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( prefetcht210,	"prefetcht2",	0x0098180f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( psadbw_1,	"psadbw",	0x0000f60f,	0x0000ffff,	X86MMRegModRM )		// sse	
inspick( psadbw_2,	"psadbw",	0x00f60f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pshufd,	"pshufd",	0x00700f66,	0x00ffffff,	X86XMMRegModRMImm ) 	// sse2	
inspick( pshufhw,	"pshufhw",	0x00700ff3,	0x00ffffff,	X86XMMRegModRMImm ) 	// sse2	
inspick( pshuflw,	"pshuflw",	0x00700ff2,	0x00ffffff,	X86XMMRegModRMImm ) 	// sse2	
inspick( pshufw,	"pshufw",	0x0000700f,	0x0000ffff,	X86MMRegModRMImm ) 	// sse	
inspick( pslldq,	"pslldq",	0xf8730f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( pslld_1,	"pslld",	0x00f0720f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( pslld_2,	"pslld",	0xf0720f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( pslld_3,	"pslld",	0x0000f20f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pslld_4,	"pslld",	0x00f20f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psllq_1,	"psllq",	0x00f0730f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psllq_2,	"psllq",	0xf0730f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psllq_3,	"psllq",	0x0000f30f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psllq_4,	"psllq",	0x00f30f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psllw_1,	"psllw",	0x00f0710f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psllw_2,	"psllw",	0xf0710f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psllw_3,	"psllw",	0x0000f10f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psllw_4,	"psllw",	0x00f10f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psrad_1,	"psrad",	0x00e0720f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psrad_2,	"psrad",	0xe0720f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psrad_3,	"psrad",	0x0000e20f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psrad_4,	"psrad",	0x00e20f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psraw_1,	"psraw",	0x00e0710f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psraw_2,	"psraw",	0xe0710f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psraw_3,	"psraw",	0x0000e10f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psraw_4,	"psraw",	0x00e10f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psrldq,	"psrldq",	0xd8730f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psrld_1,	"psrld",	0x00d0720f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psrld_2,	"psrld",	0xd0720f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psrld_3,	"psrld",	0x0000d20f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psrld_4,	"psrld",	0x00d20f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psrlq_1,	"psrlq",	0x00d0730f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psrlq_2,	"psrlq",	0xd0730f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psrlq_3,	"psrlq",	0x0000d30f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psrlq_4,	"psrlq",	0x00d30f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psrlw_1,	"psrlw",	0x00d0710f,	0x00f8ffff,	X86MMRegImm )		// mmx	
inspick( psrlw_2,	"psrlw",	0xd0710f66,	0xf8ffffff,	X86XMMRegImm )		// sse2	
inspick( psrlw_3,	"psrlw",	0x0000d10f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psrlw_4,	"psrlw",	0x00d10f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubb_1,	"psubb",	0x0000f80f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubb_2,	"psubb",	0x00f80f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubd_1,	"psubd",	0x0000fa0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubd_2,	"psubd",	0x00fa0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubq_1,	"psubq",	0x0000fb0f,	0x0000ffff,	X86MMRegModRM )		// sse2	
inspick( psubq_2,	"psubq",	0x00fb0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubsb_1,	"psubsb",	0x0000e80f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubsb_2,	"psubsb",	0x00e80f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubsw_1,	"psubsw",	0x0000e90f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubsw_2,	"psubsw",	0x00e90f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubusb_1,	"psubusb",	0x0000d80f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubusb_2,	"psubusb",	0x00d80f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubusw_1,	"psubusw",	0x0000d90f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubusw_2,	"psubusw",	0x00d90f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( psubw_1,	"psubw",	0x0000f90f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( psubw_2,	"psubw",	0x00f90f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpckhqdq,	"punpckhqdq",	0x006d0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpckhbw_1,	"punpckhbw",	0x0000680f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpckhbw_2,	"punpckhbw",	0x00680f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpckhdq_1,	"punpckhdq",	0x00006a0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpckhdq_2,	"punpckhdq",	0x006a0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpckhwd_1,	"punpckhwd",	0x0000690f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpckhwd_2,	"punpckhwd",	0x00690f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpcklqdq,	"punpcklqdq",	0x006c0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpcklbw_1,	"punpcklbw",	0x0000600f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpcklbw_2,	"punpcklbw",	0x00600f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpckldq_1,	"punpckldq",	0x0000620f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpckldq_2,	"punpckldq",	0x00620f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( punpcklwd_1,	"punpcklwd",	0x0000610f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( punpcklwd_2,	"punpcklwd",	0x00610f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( pxor_1,	"pxor",		0x0000ef0f,	0x0000ffff,	X86MMRegModRM )		// mmx	
inspick( pxor_2,	"pxor",		0x00ef0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( rcpps,		"rcpps",	0x0000530f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( rcpss,		"rcpss",	0x00530ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( rsqrtps,	"rsqrtps",	0x0000520f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( rsqrtss,	"rsqrtss",	0x00520ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( sfence,	"sfence",	0x00f8ae0f,	0x00ffffff,	X86NoOp_24 )		// sse	
inspick( shufpd,	"shufpd",	0x00c60f66,	0x00ffffff,	X86XMMRegModRMImm ) 	// sse2	
inspick( shufps,	"shufps",	0x0000c60f,	0x0000ffff,	X86XMMRegModRMImm ) 	// sse	
inspick( sqrtpd,	"sqrtpd",	0x00510f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( sqrtps,	"sqrtps",	0x0000510f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( sqrtsd,	"sqrtsd",	0x00510ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( sqrtss,	"sqrtss",	0x00510ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( stmxcsr00,	"stmxcsr",	0x0018ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( stmxcsr01,	"stmxcsr",	0x0058ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( stmxcsr10,	"stmxcsr",	0x0098ae0f,	0x00f8ffff,	X86ModRM_24 )		// sse	
inspick( subpd,		"subpd",	0x005c0f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( subps,		"subps",	0x00005c0f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( subsd,		"subsd",	0x005c0ff2,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( subss,		"subss",	0x005c0ff3,	0x00ffffff,	X86XMMRegModRM32 )	// sse	
inspick( ucomisd,	"ucomisd",	0x002e0f66,	0x00ffffff,	X86XMMRegModRM64 )	// sse2	
inspick( ucomiss,	"ucomiss",	0x00002e0f,	0x0000ffff,	X86XMMRegModRM32 )	// sse	
inspick( unpckhpd,	"unpckhpd",	0x00150f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( unpckhps,	"unpckhps",	0x0000150f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( unpcklpd,	"unpcklpd",	0x00140f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( unpcklps,	"unpcklps",	0x0000140f,	0x0000ffff,	X86XMMRegModRM )	// sse	
inspick( xorpd,		"xorpd",	0x00570f66,	0x00ffffff,	X86XMMRegModRM )	// sse2	
inspick( xorps,		"xorps",	0x0000570f,	0x0000ffff,	X86XMMRegModRM )	// sse
