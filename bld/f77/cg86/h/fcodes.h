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


// Define F-Codes:
// ===============

// Define run-time routines:

#define RT_FIRST_FORTRAN        0

#include "ftnrt.h"

enum {

// Define run-time support routines for complex arithmetic:

    RT_C8MUL    = RT_LAST_FORTRAN + 1,
    RT_C8DIV,
    RT_C8POW,
    RT_C8POWI,

    #define C_DOUBLE        4       //  RT_C16xxx = RT_C8xxx + C_DOUBLE

    RT_C16MUL,
    RT_C16DIV,
    RT_C16POW,
    RT_C16POWI,

    #define C_EXTENDED      8       //  RT_C20xxx = RT_C8xxx + C_EXTENDED

    RT_C32MUL,
    RT_C32DIV,
    RT_C32POW,
    RT_C32POWI,

// Define run-time support routine for character comparision:

    RT_LEXCMP,

// Define array i/o run-time routines:

    RT_PRT_ARRAY,
    RT_PRT_CHAR_ARRAY,
    RT_INP_ARRAY,
    RT_INP_CHAR_ARRAY,

// Define run-time character move (a = b):

    RT_MOVE,

// Define run-time concatenation into temporary:

    RT_TCAT,

// ADV run-time checking routines:

    RT_ADV_FILL_HI,
    RT_ADV_FILL_HI_LO1

};

enum {

// Define "end of F-Code sequence" F-Code:

    END_OF_SEQUENCE     = RT_LAST_FORTRAN + 1,

// Define binary operators:     +,-,*,/,**

    #define     NUM_BINOPS      5

    BINOPS,
    ADD                 = BINOPS,
    SUB,
    MUL,
    DIV,
    EXP,

    FLIP,

// Define unary minus.

    UMINUS,

// Define F-Code to push operands.

    PUSH,

// Define F-Code to pop operands.

    POP,

// Define F-Code to terminate an expression.

    EXPR_DONE,

// Define F-Codes for subprogram processing.

    SELECT,
    PUSH_LIT,

// Define logical operators:     .EQV.,.NEQV.,.OR.,.AND.,.NOT.

    LOGOPS,
    EQV                 = LOGOPS,
    NEQV,
    OR,
    AND,
    NOT,

// Define F-Code to push constants.

    PUSH_CONST,

// Define comparison operators:  .EQ.,.NE.,.LT.,.GE.,.LE.,.GT.

    RELOPS,
    CMP_EQ              = RELOPS,
    CMP_NE,
    CMP_LT,
    CMP_GE,
    CMP_LE,
    CMP_GT,

// Define control flow F-Codes:

    WARP_RETURN,
    JMP_ALWAYS,
    JMP_FALSE,
    DEFINE_LABEL,
    FCODE_SEEK,
    ASSIGN,
    IF_ARITH,
    EXECUTE,
    END_RB,
    SF_CALL,

// Define "initialize dummy argument" F-Code:

    DARG_INIT,

// Define debugging information F-Codes:

    FC_DBG_LINE,

// Define "call" F-Code:

    CALL,

// Define "define statment number" F-Code

    STMT_DEFINE_LABEL,

// Define "convert" F-Codes:

    CONVERT,
    MAKE_COMPLEX,
    MAKE_DCOMPLEX,
    MAKE_XCOMPLEX,
    START_SF,
    END_SF,

// Define "ADV fill" F-Codes:

    ADV_FILL_LO,
    ADV_FILL_HI,
    ADV_FILL_HI_LO_1,

// Define "trash cg-name" F-Code:

    TRASH,

// Define "alternate return" F-Code:

    ALT_RET,

// Define "push length in SCB" F-Code:

    PUSH_SCB_LEN,

// Define complex binary operators:

    CMPLX_BINOPS,

// Add CMPLX_OPS to a binary F-Code to get the complex/complex F-Code.

    #define CMPLX_OPS       (CMPLX_BINOPS - BINOPS)

    CCADD       = CMPLX_BINOPS,
    CCSUB,
    CCMUL,
    CCDIV,
    CCEXP,

    CMPLX_FLIP,

// Add CX_MIXED to a binary F-Code to get the complex/scalar F-Code.

    #define CX_MIXED        (CMPLX_OPS + NUM_BINOPS + 1)

    CXADD,
    CXSUB,
    CXMUL,
    CXDIV,
    CXEXP,

    CX_FLIP,

//  Add XC_MIXED to a binary F-Code to get the scalar/complex F-Code.

    #define XC_MIXED        (CMPLX_OPS + 2 * ( NUM_BINOPS + 1 ))

    XCADD,
    XCSUB,
    XCMUL,
    XCDIV,
    XCEXP,

    XC_FLIP,

// Define complex unary minus:

    CUMINUS,

// Define F-Code to terminate a complex expression.

    CMPLX_EXPR_DONE,

// Define F-Code to indicate a statement function was referenced.

    SF_REFERENCED,

// Define F-Code to pass a label to a run-time routine.

    PASS_LABEL,

// Define DATA statement F-Codes:

    START_DATA_STMT,
    END_VAR_SET,

// Define "pass character array" F-Code:

    PASS_CHAR_ARRAY,

// Define array i/o F-Codes:

    PRT_ARRAY,
    INP_ARRAY,

// Define "branch to statement number" F-Code.

    STMT_JMP_ALWAYS,

// Define "set internal file to character array" F-Code:

    ARR_SET_INTL,

// Define "null" F-Code:

    NULL_FCODE,

// Define "implied-DO for DATA statements" F-Code:

    DATA_DO_LOOP,

// Define "assign alternate return index" F-Code:

    ASSIGN_ALT_RET,

// Define "complex comparison" F-Codes:

    CC_CMP_EQ,
    CC_CMP_NE,
    XC_CMP_EQ,
    XC_CMP_NE,
    CX_CMP_EQ,
    CX_CMP_NE,

// Define character comparison operators:  .EQ. .NE. .LT. .GE. .LE. .GT.

    CHAR_RELOPS,
    CHAR_CMP_EQ         = CHAR_RELOPS,
    CHAR_CMP_NE,
    CHAR_CMP_LT,
    CHAR_CMP_GE,
    CHAR_CMP_LE,
    CHAR_CMP_GT,

//
// Define "done with label" F-Code:
//

    FREE_LABEL,

//
// Define "DO loop" F-Codes:
//

    DO_BEGIN,
    DO_END,
    COMPUTED_GOTO,
    START_RB,
    WARP,
    SET_ATEND,
    ASSIGNED_GOTO_LIST,
    CHK_IO_STMT_LABEL,
    UNARY_MUL,

// In-line intrinsic functions:

    ICHAR,
    MODULUS,
    CHAR_LEN,
    IMAG,
    MAX,
    MIN,
    CONJG,
    DPROD,
    XPROD,
    SIGN,
    BIT_TEST,
    BIT_SET,
    BIT_CLEAR,

    BITOPS,

    BIT_EQUIV           = BITOPS,
    BIT_EXCL_OR,
    BIT_OR,
    BIT_AND,
    BIT_NOT,

// Define character*1 comparison operators:  .EQ. .NE. .LT. .GE. .LE. .GT.

    CHAR_1_RELOPS,
    CHAR_1_CMP_EQ         = CHAR_1_RELOPS,
    CHAR_1_CMP_NE,
    CHAR_1_CMP_LT,
    CHAR_1_CMP_GE,
    CHAR_1_CMP_LE,
    CHAR_1_CMP_GT,

// Define single character move:

    CHAR_1_MOVE,

// Field Selections operator:

    FIELD_OP,

// STRUCTURE i/o F-Codes:

    INPUT_STRUCT,
    OUTPUT_STRUCT,

// Field subcript operator:

    FIELD_SUBSCRIPT,

// STRUCTURE array i/o F-Codes:

    STRUCT_PRT_ARRAY,
    STRUCT_INP_ARRAY,

// Field substring operator:

    FIELD_SUBSTRING,

    MAKE_SCB,

// ALLOCATED intrinsic function:

    ALLOCATED,

// Math intrinsic functions:

    MATH_MOD,
    MATH_ABS,
    MATH_ASIN,
    MATH_ACOS,
    MATH_ATAN,
    MATH_ATAN2,
    MATH_LOG,
    MATH_LOG10,
    MATH_COS,
    MATH_SIN,
    MATH_TAN,
    MATH_SINH,
    MATH_COSH,
    MATH_TANH,
    MATH_SQRT,
    MATH_EXP,

// LOC intrinsic function:

    LOC,

// Change-Bit intrinsic function:

    BIT_CHANGE,

// Done parenthesized expression:

    DONE_PAREN_EXPR,

// Done compiling statement:

    STMT_DONE,

// Make reference to symbol volatile:

    VOLATILE,

// Pass character array that is a field within a structure as an argument:

    PASS_FIELD_CHAR_ARRAY,

// Set SCB for dynamic allocation

    SET_SCB_LEN,

// LSHIFT and RSHIFT opcodes,

    LEFT_SHIFT,
    RIGHT_SHIFT,

// Define an equal character move of non optimal length strings:

    CHAR_N_MOVE

};
