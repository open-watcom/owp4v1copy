;*****************************************************************************
;*
;*                            Open Watcom Project
;*
;*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
;*
;*  ========================================================================
;*
;*    This file contains Original Code and/or Modifications of Original
;*    Code as defined in and that are subject to the Sybase Open Watcom
;*    Public License version 1.0 (the 'License'). You may not use this file
;*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
;*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
;*    provided with the Original Code and Modifications, and is also
;*    available at www.sybase.com/developer/opensource.
;*
;*    The Original Code and all software distributed under the License are
;*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
;*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
;*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
;*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
;*    NON-INFRINGEMENT. Please see the License for the specific language
;*    governing rights and limitations under the License.
;*
;*  ========================================================================
;*
;* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
;*               DESCRIBE IT HERE!
;*
;*****************************************************************************


PT_NOTYPE       equ    0        ; subroutine and hexidecimal data
PT_LOG1         equ    1        ; logical*1
PT_LOG4         equ    2        ; logical*4
PT_INT1         equ    3        ; integer*1
PT_INT2         equ    4        ; integer*2
PT_INT4         equ    5        ; integer*4
PT_REAL         equ    6        ; real*4
PT_DBLE         equ    7        ; real*8
PT_XTND         equ    8        ; real*16
PT_CPLX         equ    9        ; complex*8
PT_DBCX         equ    10       ; complex*16
PT_XTCX         equ    11       ; complex*32
PT_CHAR         equ    12       ; character
PT_STRUCT       equ    13       ; structure
PT_ARRAY        equ    14       ; array
PT_ENDDATA      equ    15       ; end of data set for DATA statement

PC_CONST        equ 0           ; constant
PC_VARIABLE     equ 1           ; simple variable name
PC_ARRAY_ELT    equ 2           ; array element
PC_SS_ARRAY     equ 3           ; substrung array element
PC_ARRAY_NAME   equ 4           ; array name
PC_PROCEDURE    equ 5           ; subprogram name func/subr established
PC_FN_OR_SUB    equ 6           ; subprogram name type not established
PC_STATEMENT    equ 7           ; alternate return specifier
PC_RESULT       equ 8           ; expected result type of subprogram

IF_IND          equ 10H        ; intrinisic function indicator
VAR_LEN_CHR     equ 80H        ; character*(*) argument/return value
IND_PARM        equ 80H        ; parameter is indirect
VALUE_PARM      equ 40H        ; parameter is passed by value
DATA_REF_PARM   equ 20H        ; parameter is passed by data reference
TYPE_MASK       equ 0fH        ; type information mask
XTYP_MASK       equ 1fH        ; expected type mask
UNDEF_CHK       equ 80H        ; undefined checking on argument list
ARRAY_CHK       equ 40H        ; type checking in arrays
SYSTEM_CALL     equ 20H        ; External call is a system call
CODE_MASK       equ 0fH        ; code information mask
COUNT_MASK      equ 03H        ; count mask for intrinsic functions
INFO_MASK       equ 3fH        ; mask to zero unused bits in if info

TY_DEF_CALL     equ 00h        ; call type constants
TY_SYS_CALL     equ 01h        ; call type constants
TY_STD_CALL     equ 02h        ; call type constants
TY_CDECL_CALL   equ 03h        ; call type constants
TY_PASCAL_CALL  equ 04h        ; call type constants
TY_CALL_MASK    equ 07h        ; call type constants
