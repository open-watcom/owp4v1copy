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


;
; FCPSH         : push routines
;

.8087

include fcdef.inc
include instr.inc

        fmodstart       fcpsh


fcode   PUSH_INT1               ; push integer*1
dfcode  PUSH_LOG1               ; push logical*1
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     VAL_INT1
efcode  PUSH_INT1


fcode   VAL_INT1
dfcode  VAL_LOG1
        movsx   eax,byte ptr [edi]; get byte value to be pushed
        push    eax             ; push value
        next
efcode  VAL_INT1


fcode   PUSH_IND_LOG1           ; push logical*1 indirect
dfcode  PUSH_IND_INT1           ; push integer*1 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_LOG1        ; push value
efcode  PUSH_IND_LOG1


fcode   PUSH_INT2               ; push integer*2
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     VAL_INT2
efcode  PUSH_INT2


fcode   VAL_INT2
        movsx   eax,word ptr [edi]; get word value to be pushed
        push    eax             ; push value
        next
efcode  VAL_INT2


fcode   PUSH_IND_INT2           ; push integer*2 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_INT2        ; push value
efcode  PUSH_IND_INT2


fcode   PUSH_LOG4               ; push logical*4
dfcode  PUSH_INT4               ; push integer*4
        getdw   eax             ; get addr of variable
        push    [eax+ebp]       ; push value
        next
efcode  PUSH_LOG4


fcode   PUSH_IND_LOG4           ; push logical*4 indirect
dfcode  PUSH_IND_INT4           ; push integer*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_LOG4        ; push value
efcode  PUSH_IND_LOG4


fcode   VAL_LOG4
dfcode  VAL_INT4
        push    [edi]           ; push value
        next
efcode  VAL_LOG4


fcode   PUSH_REAL4              ; push real*4
        getdw   edi             ; get addr of variable
        fld     dword ptr [edi+ebp]; push value onto 8087 stack
        next
efcode  PUSH_REAL4


fcode   PUSH_IND_REAL4          ; push real*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_REAL4       ; push value
efcode  PUSH_IND_REAL4


fcode   VAL_REAL4
        fld     dword ptr [edi] ; push value
        next
efcode  VAL_REAL4


fcode   PUSH_CPLX8              ; push complex*8
        getdw   eax             ; get addr of variable
        fld     dword ptr 4[eax+ebp]; push the operand
        fld     dword ptr [eax+ebp]; push value onto 8087 stack
        next
efcode  PUSH_CPLX8


fcode   PUSH_IND_CPLX8          ; push complex*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_CPLX8       ; push value
efcode  PUSH_IND_CPLX8


fcode   VAL_CPLX8
        fld     dword ptr 4[edi]; push value
        fld     dword ptr [edi] ; push value
        next
efcode  VAL_CPLX8


fcode   PUSH_REAL8              ; push real*8
        getdw   eax             ; get addr of variable
        fld     qword ptr [eax+ebp]; push the operand
        next                    ; execute next F-Code
efcode  PUSH_REAL8


fcode   PUSH_IND_REAL8          ; push real*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_REAL8       ; push value
efcode  PUSH_IND_REAL8


fcode   VAL_REAL8
        fld     qword ptr [edi] ; push value
        next
efcode  VAL_REAL8


fcode   PUSH_CPLX16             ; push complex*16
        getdw   eax             ; get addr of variable
        fld     qword ptr 8[eax+ebp]; push value onto 8087 stack
        fld     qword ptr [eax+ebp]; push the operand
        next
efcode  PUSH_CPLX16


fcode   PUSH_IND_CPLX16         ; push complex*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_CPLX16      ; push value
efcode  PUSH_IND_CPLX16


fcode   VAL_CPLX16
        fld     qword ptr 8[edi]; push value onto 8087 stack
        fld     qword ptr [edi] ; push value
        next
efcode  VAL_CPLX16


fcode   PUSH_REAL16             ; push real*16
        getdw   eax             ; get addr of variable
        fld     _TBYTE ptr [eax+ebp]; push the operand
        next                    ; execute next F-Code
efcode  PUSH_REAL16


fcode   PUSH_IND_REAL16         ; push real*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_REAL16      ; push value
efcode  PUSH_IND_REAL16


fcode   VAL_REAL16
        fld     _TBYTE ptr [edi] ; push value
        next
efcode  VAL_REAL16


fcode   PUSH_CPLX32             ; push complex*32
        getdw   eax             ; get addr of variable
        fld     _TBYTE ptr 16[eax+ebp]; push value onto 8087 stack
        fld     _TBYTE ptr [eax+ebp]; push the operand
        next
efcode  PUSH_CPLX32


fcode   PUSH_IND_CPLX32         ; push complex*32 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_CPLX32      ; push value
efcode  PUSH_IND_CPLX32


fcode   VAL_CPLX32
        fld     _TBYTE ptr 16[edi]      ; push value onto 8087 stack
        fld     _TBYTE ptr [edi]        ; push value
        next
efcode  VAL_CPLX32

        fmodend
        end
