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

#ifdef DEFINE_ASMOPS

#ifndef _WASM_
#include "inlnops.gh"
#else
#include "fullops.gh"
#endif
#undef DEFINE_ASMOPS

#else

#ifndef ASMOPS2_H
#define ASMOPS2_H

#ifndef _WASM_
#include "inlnops.gh"
#else
#include "fullops.gh"
#endif

typedef enum {
        MT_BYTE   = T_BYTE,
        MT_WORD   = T_WORD,
        MT_DWORD  = T_DWORD,
        MT_QWORD  = T_QWORD,
        MT_FWORD  = T_FWORD,
        MT_TBYTE  = T_TBYTE,
//        MT_OWORD  = T_OWORD,

        MT_SHORT  = T_SHORT,
        MT_NEAR   = T_NEAR,
        MT_FAR    = T_FAR,

        MT_PTR    = T_PTR,

#ifdef _WASM_
        MT_SBYTE  = T_SBYTE,
        MT_SWORD  = T_SWORD,
        MT_SDWORD = T_SDWORD,

        MT_STRUCT = T_STRUCT,

        MT_PROC   = T_PROC,
        MT_ABS    = T_ABS
#endif
} memtype;

#endif

#endif
