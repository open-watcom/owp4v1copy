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


// include files for WATFOR-77 PC linker.

#include <ftnstd.h>
#include <global.h>
#include <fcgbls.h>
#include <errcod.h>
#include <objstruc.h>
#include <cpopt.h>
#include <progsw.h>
#include <cioconst.h>

#define TOKBUFFLEN      1320    //  size of TokBuff
#define MAX_OBJ_LEN     512     //  size of physical output record
#define LIBHEADERLEN    9       //  length of a library header
#define UNDEFINED       0xffff  //  undefined segment

//
// bits in "info" field of "seg_entry" struct.
//

#define COMB_MASK       0x0007  //  combination mask
#define A_MASK          0x0038  //  alignment mask
#define IN_GROUP_MASK   0x0040  //  segment is in a defined group
#define ADDSEG_MASK     0x0080  //  segment is member of addseg list
#define SEG_MASK        0x0300  //  segment mask
#define ABOVE_ALL       0x0100  //     segment is above all others
#define NON_COMBINABLE  0x0200  //     has a non-combinable segment

