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
; FCPOP         : pop from stack into memory
;

include fcdef.inc
include parmtype.inc

        fmodstart       fcpop


fcode   POP_LOG1                ; assign to logical*1
dfcode  POP_INT1                ; assign to integer*1
        getdw   eax             ; get addr of variable
        pop     edx             ; pop value
        mov     [eax+ebp],dl    ; assign to 1-byte variable
        next
efcode  POP_LOG1


fcode   POP_IND_LOG1            ; assign to logical*1 indirect
dfcode  POP_IND_INT1            ; assign to integer*1 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_INT1
efcode  POP_IND_LOG1


fcode   STOR_INT1
dfcode  STOR_LOG1
        pop     eax             ; get value
        mov     [edi],al        ; store value
        next
efcode  STOR_INT1


fcode   POP_INT2                ; assign to integer*2
        getdw   eax             ; get addr of variable
        pop     edx             ; get value
        mov     [eax+ebp],dx    ; store value
        next
efcode  POP_INT2


fcode   POP_IND_INT2            ; assign to integer*2 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_INT2
efcode  POP_IND_INT2


fcode   STOR_INT2
        pop     eax             ; get value
        mov     [edi],ax        ; store value
        next
efcode  STOR_INT2


fcode   POP_REAL4               ; assign to real*4
dfcode  POP_LOG4                ; assign to logical*4
dfcode  POP_INT4                ; assign to integer*4
        getdw   eax             ; get addr of variable
        pop     [eax+ebp]       ; assign 4-byte value
        next
efcode  POP_REAL4


fcode   POP_IND_REAL4           ; assign to real*4 indirect
dfcode  POP_IND_LOG4            ; assign to logical*4 indirect
dfcode  POP_IND_INT4            ; assign to integer*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_REAL4
efcode  POP_IND_REAL4


fcode   STOR_REAL4
dfcode  STOR_LOG4
dfcode  STOR_INT4
        pop     [edi]           ; assign 4-byte value
        next
efcode  STOR_REAL4


fcode   POP_CPLX8               ; assign to complex*8
dfcode  POP_REAL8               ; assign to real*8
        getdw   eax             ; get addr of variable
        pop     [eax+ebp]       ; assign value
        pop     4[eax+ebp]      ; ...
        next
efcode  POP_CPLX8


fcode   POP_IND_CPLX8           ; assign to complex*8 indirect
dfcode  POP_IND_REAL8           ; assign to real*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_CPLX8
efcode  POP_IND_CPLX8


fcode   STOR_CPLX8              ; assign to complex*8 indirect
dfcode  STOR_REAL8              ; assign to real*8 indirect
        pop     [edi]           ; assign 8-byte value
        pop     4[edi]          ; ...
        next
efcode  STOR_CPLX8

fcode   POP_REAL16              ; assign to real*16
        getdw   eax             ; get addr of variable
        pop     [eax+ebp]       ; assign value
        pop     4[eax+ebp]      ; ...
        pop     8[eax+ebp]      ; ...
        pop     12[eax+ebp]     ; ...
        next
efcode  POP_REAL16


fcode   POP_IND_REAL16          ; assign to real*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_REAL16
efcode  POP_IND_REAL16


fcode   STOR_REAL16             ; assign to real*16 indirect
        pop     [edi]           ; assign 16-byte value
        pop     4[edi]          ; ...
        pop     8[edi]          ; ...
        pop     12[edi]         ; ...
        next
efcode  STOR_REAL16

fcode   POP_CPLX16              ; assign to complex*16
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     STOR_CPLX16
efcode  POP_CPLX16


fcode   STOR_CPLX16
        pop     eax             ; assign 16-byte value
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        sub     edi,16          ; restore edi
        next
efcode  STOR_CPLX16


fcode   POP_IND_CPLX16          ; assign to complex*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     STOR_CPLX16
efcode  POP_IND_CPLX16


fcode   POP_CPLX32              ; assign to complex*32
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     STOR_CPLX32
efcode  POP_CPLX32


fcode   STOR_CPLX32
        pop     eax             ; assign 32-byte value
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        pop     eax             ; ...
        stosd                   ; ...
        sub     edi,32          ; restore edi
        next
efcode  STOR_CPLX32


fcode   POP_IND_CPLX32          ; assign to complex*32 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     STOR_CPLX32
efcode  POP_IND_CPLX32


ValSize:
        db      0       ; PT_NOTYPE
        db      4       ; PT_LOG_1
        db      4       ; PT_LOG_4
        db      4       ; PT_INT_1
        db      4       ; PT_INT_2
        db      4       ; PT_INT_4
        db      4       ; PT_REAL_4
        db      8       ; PT_REAL_8
        db      16      ; PT_REAL_16
        db      8       ; PT_CPLX_8
        db      16      ; PT_CPLX_16
        db      32      ; PT_CPLX_32
        db      8       ; PT_CHAR
        db      8       ; PT_STRUCT


fcode   TRASH_VALUE
        getdw   eax                             ; get type of value
        movzx   eax,byte ptr ValSize[eax]       ; get size of value
        add     esp,eax                         ; cleanup CPU stack
        next                                    ; execute next F-Code
efcode  TRASH_VALUE

        fmodend
        end
