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
* Description:  Table of X64 opcodes and corresponding decode routines.
*                without extension 3DNow!, MMX, SSE, SSE2 and SSE3
*
****************************************************************************/

/*
 * It must be processed as first instruction table before any others
 * It is opcode map with full prefix processing.
 *
 */

/*
      Idx,              Name,           Opcode,         Mask,           Handler
*/

// PREFIXES
inspick( rex_pr,        "rex",          0x00000040,     0x000000f0,     X64PrefixRex )
inspick( opnd_pr,       "opnd",         0X00000066,     0x000000ff,     X64PrefixOpnd )
inspick( addr_pr,       "addr",         0X00000067,     0x000000ff,     X64PrefixAddr )
inspick( cs_pr,         "cs",           0x0000002e,     0x000000ff,     X64PrefixCS )
inspick( fs_pr,         "fs",           0x00000064,     0x000000ff,     X64PrefixFS )
inspick( gs_pr,         "gs",           0x00000065,     0x000000ff,     X64PrefixGS )
inspick( ss_pr,         "ss",           0x00000036,     0x000000ff,     X64PrefixSS )
inspick( ds_pr,         "ds",           0x0000003e,     0x000000ff,     X64PrefixDS )
inspick( es_pr,         "es",           0x00000026,     0x000000ff,     X64PrefixES )

// Instructions
inspick( wait,          "wait",         0x0000009b,     0x000000ff,     X64NoOp_8)
inspick( sahf,          "sahf",         0x0000009e,     0x000000ff,     X64NoOp_8)
inspick( lahf,          "lahf",         0x0000009f,     0x000000ff,     X64NoOp_8)
inspick( leave,         "leave",        0x000000c9,     0x000000ff,     X64NoOp_8)
inspick( retf,          "retf",         0x000000cb,     0x000000ff,     X64NoOp_8)
inspick( iretd,         "iretd",        0x000000cf,     0x000000ff,     X64NoOp_8)
inspick( iret,          "iret",         0x00000000,     0x00000000,     NULL)
inspick( iretq,         "iretq",        0x00000000,     0x00000000,     NULL)
inspick( xlat,          "xlat",         0x000000d7,     0x000000ff,     X64NoOp_8)
inspick( hlt,           "hlt",          0x000000f4,     0x000000ff,     X64NoOp_8)
inspick( stc,           "ctc",          0x000000f9,     0x000000ff,     X64NoOp_8)
inspick( sti,           "sti",          0x000000fb,     0x000000ff,     X64NoOp_8)
inspick( std,           "std",          0x000000fd,     0x000000ff,     X64NoOp_8)

inspick( pop,           "pop",          0x00000058,     0x000000f8,     X64Reg_8)
inspick( popf,          "popf",         0x00000000,     0x00000000,     NULL)
inspick( popfq,         "popfq",        0x0000009d,     0x000000ff,     X64NoOp_8)

inspick( push,          "push",         0x00000050,     0x000000f8,     X64Reg_8)
inspick( pushf,         "pushf",        0x00000000,     0x00000000,     NULL)
inspick( pushfq,        "pushfq",       0x0000009c,     0x000000ff,     X64NoOp_8)

/* sorted */

inspick( adc,           "adc",          0x00000010,     0x000000fc,     X64RegModRM_16)
inspick( adc2,          "adc",          0x00000014,     0x000000fe,     X64ImmAcc_8)
inspick( adc3,          "adc",          0x00001080,     0x000038fc,     X64ModRMImm_16)
inspick( add,           "add",          0x00000000,     0x000000fc,     X64RegModRM_16)
inspick( add2,          "add",          0x00000004,     0x000000fe,     X64ImmAcc_8)
inspick( add3,          "add",          0x00000080,     0x000038fc,     X64ModRMImm_16)
inspick( and,           "and",          0x00000020,     0x000000fc,     X64RegModRM_16)
inspick( and2,          "and",          0x00000024,     0x000000fe,     X64ImmAcc_8)
inspick( and3,          "and",          0x00002080,     0x000038fc,     X64ModRMImm_16)
inspick( bsf,           "bsf",          0x0000bc0f,     0x0000ffff,     X64RegModRM_24B)
inspick( bsr,           "bsr",          0x0000bd0f,     0x0000ffff,     X64RegModRM_24B)
inspick( bswap,         "bswap",        0x0000c80f,     0x0000f8ff,     X64Reg_16)
inspick( bt,            "bt",           0x0000a30f,     0x0000ffff,     X64RegModRM_24B)
inspick( bt2,           "bt",           0x0020ba0f,     0x0038ffff,     X64ModRMImm_24)
inspick( btc,           "btc",          0x0000bb0f,     0x0000ffff,     X64RegModRM_24B)
inspick( btc2,          "btc",          0x0038ba0f,     0x0038ffff,     X64ModRMImm_24)
inspick( btr,           "btr",          0x0000b30f,     0x0000ffff,     X64RegModRM_24B)
inspick( btr2,          "btr",          0x0030ba0f,     0x0038ffff,     X64ModRMImm_24)
inspick( bts,           "bts",          0x0000ab0f,     0x0000ffff,     X64RegModRM_24B)
inspick( bts2,          "bts",          0x0028ba0f,     0x0038ffff,     X64ModRMImm_24)
inspick( call,          "call",         0x000000e8,     0x000000ff,     X64Rel_8)
inspick( cwde,          "cwde",         0x00000098,     0x000000ff,     X64NoOp_8)
inspick( cbw,           "cbw",          0x00000000,     0x00000000,     NULL)
inspick( cdqe,          "cdqe",         0x00000000,     0x00000000,     NULL)
inspick( clc,           "clc",          0x000000f8,     0x000000ff,     X64NoOp_8)
inspick( cld,           "cld",          0x000000fc,     0x000000ff,     X64NoOp_8)


inspick( cli,           "cli",          0x000000fa,     0x000000ff,     X64NoOp_8)


inspick( cmc,           "cmc",          0x000000f5,     0x000000ff,     X64NoOp_8)

inspick( cdq,           "cdq",          0x00000099,     0x000000ff,     X64NoOp_8)
inspick( cwd,           "cwd",          0x00000000,     0x00000000,     NULL)
inspick( cqo,           "cqo",          0x00000000,     0x00000000,     NULL)

