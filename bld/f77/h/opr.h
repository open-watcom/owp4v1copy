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


#define OPR_PHI 0x00    // null operator
#define OPR_TRM 0x01    // start/terminate symbol
#define OPR_FBR 0x02    // function/array bracket
#define OPR_LBR 0x03    // (
#define OPR_COM 0x04    // ,
#define OPR_COL 0x05    // :
#define OPR_EQU 0x06    // =
#define OPR_RBR 0x07    // )
#define OPR_DPT 0x08    // .
#define OPR_AST 0x09    // * for alternate return specifier

// relational operators

#define OPR_EQ  0x0A    // .EQ.
#define OPR_NE  0x0B    // .NE.
#define OPR_LT  0x0C    // .LT.
#define OPR_GE  0x0D    // .GE.
#define OPR_LE  0x0E    // .LE.
#define OPR_GT  0x0F    // .GT.

// logical operators

#define OPR_EQV         0x10    // .EQV.
#define OPR_NEQV        0x11    // .NEQV.
#define OPR_OR          0x12    // .OR.
#define OPR_AND         0x13    // .AND.
#define OPR_NOT         0x14    // .NOT.

// field selection operator

#define OPR_FLD         0x15    // %

#if _CPU == _VAX
//  special operators
#define OPR_AMP 0x16    //  pass by address
#endif

//  arithmetic operators

#define OPR_PLS 0x1A    //   +
#define OPR_MIN 0x1B    //   -
#define OPR_MUL 0x1C    //   *
#define OPR_DIV 0x1D    //   /
#define OPR_EXP 0x1E    //   **
#define OPR_CAT 0x1F    //   //   character operator

//  operator group ranges

#define FIRST_LOGOP     OPR_EQV
#define LAST_LOGOP      OPR_NOT
#define FIRST_RELOP     OPR_EQ
#define LAST_RELOP      OPR_GT
#define FIRST_ARITHOP   OPR_PLS
#define LAST_ARITHOP    OPR_CAT

// Constants below used in UPSCAN for operator sequence table and are
// modified (along with UPSCAN) if operators are added/deleted/modified

//  dimensions of operator sequence table

#define OPR_CLASSES             18
#define OPR_SEQ_MAT_SIZE        (OPR_CLASSES*OPR_CLASSES-1)     //  base 0
#define OPR_SEQ_MAT_COLS        OPR_CLASSES
