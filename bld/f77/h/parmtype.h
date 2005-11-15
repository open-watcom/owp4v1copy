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
* Description:  Run and compile-time consts indicating parameter type
*               and attributes
*
****************************************************************************/

/**************** IMPORTANT NOTE ************************

  data in files:  ptypes.inc, ptypes.h and  parmtype.h
  
  must corespond each to other

********************************************************/

//
// Run and compile-time consts indicating parameter type and attributes
// NOTE: These constants should stay in the following order.
//       ( routines rely on them being sequential )
//

#define         VAR_LEN_CHAR    0x80

enum {
  PT_NOTYPE,            // type of statement #'s and subroutine return values
  PT_LOG_1,             // LOGICAL*1
  PT_LOG_4,             // LOGICAL*4
  PT_INT_1,             // INTEGER*1
  PT_INT_2,             // INTEGER*2
  PT_INT_4,             // INTEGER*4
  PT_REAL_4,            // REAL*4
  PT_REAL_8,            // REAL*8
  PT_REAL_16,           // REAL*16
  PT_CPLX_8,            // COMPLEX*8
  PT_CPLX_16,           // COMPLEX*16
  PT_CPLX_32,           // COMPLEX*32
  PT_CHAR,              // CHARACTER
  PT_STRUCT,            // STRUCTURE
  PT_ARRAY,             // ARRAY
  PT_END_DATASET,       // indicates the end of a data set in DATA statement
  PT_NOT_STARTED        // IOCB->typ is initialized to this for i/o statements
};                      // so that the i/o list may be flushed in halt when an
                        // error occurs

#define       PC_PROC_FAR16   0x80

enum {
  PC_CONST,             //  Non-modifiable argument. Passed by value.
  PC_VARIABLE,          //  Simple variable name.
  PC_ARRAY_ELT,         //  Array element.
  PC_SS_ARRAY,          //  Substrung array element.
  PC_ARRAY_NAME,        //  Array name.
  PC_PROCEDURE,         //  Subprogram name. function/subroutine established
  PC_FN_OR_SUB,         //  Subprogram name. could be function or subroutine
  PC_STATEMENT,         //  Alternate return specifier.
  PC_RESULT             //  Expected result type of subprogram.
};

extern int     ParmType( int typ, int size );