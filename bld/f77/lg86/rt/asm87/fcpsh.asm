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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     FORTRAN-77 run-time PUSH support for 8086 based machines
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include instr.inc

        fmodstart       fcpsh


fcode   PUSH_INT1               ; push integer*1
dfcode  PUSH_LOG1               ; push logical*1
        getword DI              ; get addr of variable
        db      HIDE_1
;;;;;;;;hop     VAL_INT1
efcode  PUSH_INT1


fcode   VAL_INT1
dfcode  VAL_LOG1
        db      ES_OVERRIDE
        mov     AL,0[DI]        ; get byte, value to be pushed
        cbw                     ; convert to word
        cwd                     ; convert to integer*4
        push    DX              ; push value
        push    AX              ; . . .
        next                    ; execute next F-Code
efcode  VAL_INT1


fcode   PUSH_IND_LOG1           ; push logical*1 indirect
dfcode  PUSH_IND_INT1           ; push integer*1 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
        hop     VAL_LOG1        ; push value
efcode  PUSH_IND_LOG1


fcode   PUSH_INT2               ; push integer*2
        getword DI              ; get addr of variable
        db      HIDE_1
;;;;;;;;hop     VAL_INT2
efcode  PUSH_INT2


fcode   VAL_INT2
        db      ES_OVERRIDE
        mov     AX,0[DI]        ; get word, value to be pushed
        cwd                     ; convert to integer*4
        push    DX              ; push value
        push    AX              ; . . .
        next                    ; execute next F-Code
efcode  VAL_INT2


fcode   PUSH_IND_INT2                  ; push integer*2 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
        hop     VAL_INT2           ; push value
efcode  PUSH_IND_INT2


fcode   PUSH_LOG4               ; push logical*4
dfcode  PUSH_INT4               ; push integer*4
        getword DI              ; get addr of variable
        push    2[DI]           ; push value
        push    0[DI]           ; . . .
        next                    ; execute next F-Code
efcode  PUSH_LOG4


fcode   PUSH_IND_LOG4           ; push logical*4 indirect
dfcode  PUSH_IND_INT4           ; push integer*4 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_LOG4        ; push value
efcode  PUSH_IND_LOG4


fcode   VAL_LOG4
dfcode  VAL_INT4
        push    ES:2[DI]        ; push value
        push    ES:0[DI]        ; . . .
        next                    ; execute next F-Code
efcode  VAL_LOG4


fcode   PUSH_REAL4              ; push real*4
        getword DI              ; get addr of variable
fld_dword_ds:
        fld     dword ptr [DI]  ; push value onto 8087 stack
        next                    ; execute next F-Code
efcode  PUSH_REAL4


fcode   PUSH_IND_REAL4          ; push real*4 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_REAL4       ; push value
efcode  PUSH_IND_REAL4


fcode   VAL_REAL4
fld_dword_es:
        fld     dword ptr ES:[DI] ; push value
        next                    ; execute next F-Code
efcode  VAL_REAL4


fcode   PUSH_CPLX8              ; push complex*8
        getword DI              ; get addr of variable
        fld     dword ptr 4[DI] ; push the operand
        hop     fld_dword_ds    ; . . .
efcode  PUSH_CPLX8


fcode   PUSH_IND_CPLX8          ; push complex*8 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_CPLX8       ; push value
efcode  PUSH_IND_CPLX8


fcode   VAL_CPLX8
        fld     dword ptr ES:4[DI]; push value
        hop     fld_dword_es    ; ...
efcode  VAL_CPLX8


fcode   PUSH_REAL8              ; push real*8
        getword DI              ; get addr of variable
fld_qword_ds:
        fld     qword ptr [DI]  ; push the operand
        next                    ; execute next F-Code
efcode  PUSH_REAL8


fcode   PUSH_IND_REAL8          ; push real*8 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_REAL8           ; push value
efcode  PUSH_IND_REAL8


fcode   VAL_REAL8
fld_qword_es:
        fld     qword ptr ES:0[DI]; push value
        next                    ; execute next F-Code
efcode  VAL_REAL8


fcode   PUSH_REAL16             ; push real*16
        getword DI              ; get addr of variable
fld_tbyte_ds:
        fld     _TBYTE ptr [DI] ; push the operand
        next                    ; execute next F-Code
efcode  PUSH_REAL16


fcode   PUSH_IND_REAL16         ; push real*16 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_REAL16      ; push value
efcode  PUSH_IND_REAL16


fcode   VAL_REAL16
fld_tbyte_es:
        fld     _TBYTE ptr ES:0[DI]     ; push value
        next                    ; execute next F-Code
efcode  VAL_REAL16


fcode   PUSH_CPLX16             ; push complex*16
        getword DI              ; get addr of variable
        fld     qword ptr 8[DI] ; push value onto 8087 stack
        hop     fld_qword_ds    ; ...
efcode  PUSH_CPLX16


fcode   PUSH_IND_CPLX16         ; push complex*16 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_CPLX16      ; push value
efcode  PUSH_IND_CPLX16


fcode   VAL_CPLX16
        fld     qword ptr ES:8[DI] ; push value onto 8087 stack
        hop     fld_qword_es    ; ...
efcode  VAL_CPLX16


fcode   PUSH_CPLX32             ; push complex*32
        getword DI              ; get addr of variable
        fld     _TBYTE ptr 16[DI]; push value onto 8087 stack
        hop     fld_tbyte_ds    ; ...
efcode  PUSH_CPLX32


fcode   PUSH_IND_CPLX32         ; push complex*32 indirect
        getword DI              ; get addr of pointer
        les     DI,0[DI]        ; get addr of value
;;;;;;;;hop     VAL_CPLX32      ; push value
efcode  PUSH_IND_CPLX32


fcode   VAL_CPLX32
        fld     _TBYTE ptr ES:16[DI] ; push value onto 8087 stack
        hop     fld_tbyte_es    ; ...
efcode  VAL_CPLX32


        fmodend
        end
