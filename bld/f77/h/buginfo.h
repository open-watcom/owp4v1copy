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


#include "parmtype.h"

// Local data area items (compilation with debugger):
//
//
// VARIABLE
//   +---------------------------------------------------------------+
//   |  code  | ind | subscr | len | name        | value             |
//   +---------------------------------------------------------------+
//        4       1      3      8     n bytes      1|2|4|8|16 bytes
//
//
// STATEMENT FUNCTION NAME/VALUE
//   +---------------------------------------------------------------+
//   |  code  |   type/len   | len | name        | value|
//   +-------------------------------------------+-------------------+
//        4        4            8    n bytes       1|2|4|8|16 bytes
//
//
// SUBPROGRAM-NAME or FUNCTION-ENTRY-NAME
//   +-------------------------------------------+  % if lo nibble of 1st
//   |  code  |   type/len   | len | name        |  % byte is PT_NOTYPE,
//   +-------------------------------------------+  % then subroutine name
//        4        4            8    n bytes
//
//
// SUBPROGRAM ADDRESS
//   +---------------------------+
//   |  code  | ... | address    |
//   +---------------------------+
//        4      4        32
//
//
// Statement number list (compilation with debugger):
//
//
// STMT-NUMBER
//   +-----------------------------
//   | binary number |  address   |
//   +-----------------------------
//          32            16
//
//

typedef struct db_named_item {
    byte        len;            //  length in bytes of name which follows
    char        name;           //  first character in name
} db_named_item;

typedef struct db_addr {
    void        * PGM addr;     //  address of subprogram or label
} db_addr;

typedef struct db_item {
    byte                        code;
      union {
        struct db_named_item    n;
        struct db_addr          a;
      } dbi;
} db_item;

typedef struct db_stmtno {
    unsigned_32 stmtno; //  statement number in binary
    obj_ptr     addr;   //  address of statement
} db_stmtno;

#if !_INTEL_CPU

typedef struct src_line {
    unsigned_16 isn_offset;     //  offset to from start of struct to ISN
    pointer     link;           //  link to next line
    unsigned_32 stmtno;         //  statement number
    char        src;            //  source text
    unsigned_16 back;           //  offset to start of source information
    unsigned_16 isn;            //  ISN
} src_line;

#endif

#define DB_CODE_SHIFT   4       //  shift 4 bits to move from lo nibble to hi
#define DB_CODE_MASK    0xf0    //  mask for code

// The following order must be preserved.

#define DB_FILLER               0x00    // alignment byte - must be 0x00
#define DB_LOG1                 (PT_LOG_1<<DB_CODE_SHIFT)       // 0x10
#define DB_LOG4                 (PT_LOG_4<<DB_CODE_SHIFT)       // 0x20
#define DB_INT1                 (PT_INT_1<<DB_CODE_SHIFT)       // 0x30
#define DB_INT2                 (PT_INT_2<<DB_CODE_SHIFT)       // 0x40
#define DB_INT4                 (PT_INT_4<<DB_CODE_SHIFT)       // 0x50
#define DB_REAL                 (PT_REAL_4<<DB_CODE_SHIFT)      // 0x60
#define DB_REAL8                (PT_REAL_8<<DB_CODE_SHIFT)      // 0x70
#define DB_CPLX8                (PT_CPLX_8<<DB_CODE_SHIFT)      // 0x80
#define DB_CPLX16               (PT_CPLX_16<<DB_CODE_SHIFT)     // 0x90
#define DB_CHAR                 (PT_CHAR<<DB_CODE_SHIFT)        // 0xa0
#define DB_STRUCT               (PT_STRUCT<<DB_CODE_SHIFT)      // 0xb0
#define DB_ENTRY_NAME           0xc0    // function/subroutine/program name
#define DB_STMT_FUNC            0xd0    // statement functions
#define DB_SPECIAL              0xe0    // special subclass
#define DB_SPECIAL_CLASS        0x0f    // - mask for special class
#define DB_ADDR                 0x00    // - address of subprogram
#define DB_DUMMY_SP             0x01    // - address of dummy argument
#define DB_END_LIST             0x02    // - end of debugger variable list

#define DB_VAR_INDIRECT         0x08    // indirect variable
#define DB_VAR_SUBSCR           0x07    // mask for subscripts

#define DB_ENTRY_TYP_MASK       0x0f

#define SELECT          1       //  item selected
#define ALL             2       //  print all items
#define ARRAY_ELT       3       //  print array element

#define TB_ASSEMBLER    0       //  subprogram in a user assembler file
#define TB_LIBRARY      1       //  subprogram in a library member
#define TB_FILE         2       //  subprogram in a source file
#define TB_EXTERNAL     3       //  subprogram in a non-WATFOR-77 object file
