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


// Define character sets:
// ======================

#define _ASCII  0
#define _EBCDIC 1
#define _KANJI  2

// Define CPU targets:
// ====================

#define _370   0
#define _8086  1
#define _VAX   2
#define _80386 3
#define _AXP   4
#define _PPC   5

// Define Operating Systems:
// ==========================

#define _VMCMS          0
#define _PCDOS          1
#define _VMS            2
#define _QNX            3
#define _MVS            4
#define _HP             5
#define _OS2            6
#define _PENPOINT       7
#define _NT             8
#define _NETWARE        9
#define _OSI            10

// Define feature switch settings:
// ===============================

#define _OFF   0
#define _ON    1

#include "sdmacro.h"
#include "switch.h"

// Define multi-language constants:
// ================================

#if !defined( MSG_LANG_SPACING )
#define MSG_LANG_SPACING 16000
#endif
#include "wreslang.h"
