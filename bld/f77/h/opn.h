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


//
// Flags for 'opn' field of 'itnode'
//

// DownScan codes:
// ================

#define OPN_PHI 0       // null operand
#define OPN_NAM 1       // name
#define OPN_LIT 2       // literal
#define OPN_LGL 3       // logical
#define OPN_INT 4       // integer
#define OPN_REA 5       // real
#define OPN_DBL 6       // double
#define OPN_EXT 7       // extended
#define OPN_OCT 8       // octal constant
#define OPN_HEX 9       // hexadecimal constant
#define OPN_FMT 10      // FORMAT string

// UpScan codes:
// =============

#define OPN_SS1         0x80    // chr(i:i)
#define OPN_ASY         0x40    // symbol table entry is alternate
#define OPN_FLD         0x08    // field name of structure

#define WHAT_SHIFT      0       // bits to shift to make OPN_WHAT origin 0
#define OPN_WHAT        0x07    // what type of operand is it?
#define OPN_NNL         0x01    // - name, no argument list
#define OPN_NWL         0x02    // - name, with argument list
#define OPN_ARR         0x03    // - array name, without subscript list
#define OPN_STN         0x04    // - statement number
#define OPN_CON         0x05    // - constant
#define OPN_ASS         0x06    // - array name subscripted and substrung
#define OPN_SSR         0x07    // - high bound in a(n:)

#define WHERE_SHIFT     4       // bits to shift to make OPN_WHERE origin 0
#define OPN_WHERE       0x30    // where is the operand?
#define OPN_SAFE        0x10    // - value in a register
#define OPN_TMP         0x20    // - value in a temporary
#define OPN_VAL         0x30    // - static temporary SCB
