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
; FCPOP         : pop routines
;

.8087

include fcdef.inc
include parmtype.inc

        fmodstart       fcpop


fcode   POP_LOG1                ; assign to logical*1
dfcode  POP_INT1                ; assign to integer*1
        getdw   eax             ; get addr of variable
        pop     edx             ; get value
        mov     [eax+ebp],dl    ; store value
        next
efcode  POP_LOG1


fcode   POP_IND_LOG1            ; assign to logical*1 indirect
dfcode  POP_IND_INT1            ; assign to integer*1 indirect
        getdw   eax             ; get addr of destination
        mov     edi,[eax+ebp]   ; ...
;;;;;;;;hop     STORL1
efcode  POP_IND_LOG1


fcode   STOR_LOG1
dfcode  STOR_INT1
        pop     eax             ; get value
        mov     [edi],al        ; store value
        next
efcode  STOR_LOG1


fcode   POP_INT2                ; assign to integer*2
        getdw   eax             ; get addr of variable
        pop     edx             ; get value
        mov     [eax+ebp],dx    ; store value
        next
efcode  POP_INT2


fcode   POP_IND_INT2            ; assign to integer*2 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STORI2
efcode  POP_IND_INT2


fcode   STOR_INT2
        pop     eax             ; get value
        mov     [edi],ax        ; store value
        next
efcode  STOR_INT2


fcode   POP_LOG4                ; assign to logical*4
dfcode  POP_INT4                ; assign to integer*4
        getdw   eax             ; get addr of variable
        pop     [eax+ebp]       ; assign 4-byte value
        next
efcode  POP_LOG4


fcode   POP_IND_LOG4            ; assign to logical*4 indirect
dfcode  POP_IND_INT4            ; assign to integer*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STORL4
efcode  POP_IND_LOG4


fcode   STOR_LOG4
dfcode  STOR_INT4
        pop     [edi]           ; assign 4-byte value
        next
efcode  STOR_LOG4


fcode   POP_REAL4               ; assign to real*4
        getdw   eax             ; get addr of variable
        fstp    dword ptr [eax+ebp]; pop real*4 from the 8087 stack
        next                    ; execute next F-Code
efcode  POP_REAL4


fcode   POP_IND_REAL4           ; assign to integer*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_REAL4
efcode  POP_IND_REAL4


fcode   STOR_REAL4
        fstp    dword ptr [edi] ; pop real*4 from the 8087 stack
        next
efcode  STOR_REAL4


fcode   POP_REAL8               ; assign to real*8
        getdw   eax             ; get addr of variable
        fstp    qword ptr [eax+ebp]; pop real*8 from 8087 stack
        next                    ; execute next F-Code
efcode  POP_REAL8


fcode   POP_IND_REAL8           ; assign to real*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_REAL8
efcode  POP_IND_REAL8


fcode   STOR_REAL8
        fstp    qword ptr [edi] ; pop real*8 from 8087 stack
        next
efcode  STOR_REAL8


fcode   POP_REAL16              ; assign to real*16
        getdw   eax             ; get addr of variable
        fstp    _TBYTE ptr [eax+ebp]; pop real*8 from 8087 stack
        next                    ; execute next F-Code
efcode  POP_REAL16


fcode   POP_IND_REAL16          ; assign to real*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_REAL16
efcode  POP_IND_REAL16


fcode   STOR_REAL16
        fstp    _TBYTE ptr [edi]        ; pop real*16 from 8087 stack
        next
efcode  STOR_REAL16


fcode   POP_CPLX8               ; assign to complex*8
        getdw   eax             ; get addr of variable
        fstp    dword ptr [eax+ebp]; pop real*4 from 8087 stack
        fstp    dword ptr 4[eax+ebp]; pop imag*4 from 8087 stack
        next
efcode  POP_CPLX8


fcode   POP_IND_CPLX8           ; assign to complex*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_CPLX8
efcode  POP_IND_CPLX8


fcode   STOR_CPLX8
        fstp    dword ptr [edi] ; pop real*4 from 8087 stack
        fstp    dword ptr 4[edi]; pop imag*4 from 8087 stack
        next
efcode  STOR_CPLX8


fcode   POP_CPLX16              ; assign to complex*16
        getdw   eax             ; get addr of variable
        fstp    qword ptr [eax+ebp]; pop real*8 from 8087 stack
        fstp    qword ptr 8[eax+ebp]; pop imag*8 from 8087 stack
        next
efcode  POP_CPLX16


fcode   POP_IND_CPLX16          ; assign to complex*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_CPLX16
efcode  POP_IND_CPLX16


fcode   STOR_CPLX16
        fstp    qword ptr [edi] ; pop real*8 from 8087 stack
        fstp    qword ptr 8[edi]; pop imag*8 from 8087 stack
        next
efcode  STOR_CPLX16


fcode   POP_CPLX32              ; assign to complex*32
        getdw   eax             ; get addr of variable
        fstp    _TBYTE ptr [eax+ebp]; pop real*16 from 8087 stack
        fstp    _TBYTE ptr 16[eax+ebp]; pop imag*16 from 8087 stack
        next
efcode  POP_CPLX32


fcode   POP_IND_CPLX32          ; assign to complex*32 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     STOR_CPLX32
efcode  POP_IND_CPLX32


fcode   STOR_CPLX32
        fstp    _TBYTE ptr [edi]        ; pop real*16 from 8087 stack
        fstp    _TBYTE ptr 16[edi]      ; pop imag*16 from 8087 stack
        next
efcode  STOR_CPLX32


ValSize:
        db      0       ; PT_NOTYPE
        db      4       ; PT_LOG_1
        db      4       ; PT_LOG_4
        db      4       ; PT_INT_1
        db      4       ; PT_INT_2
        db      4       ; PT_INT_4
        db      1       ; PT_REAL_4
        db      1       ; PT_REAL_8
        db      1       ; PT_REAL_16
        db      2       ; PT_CPLX_8
        db      2       ; PT_CPLX_16
        db      2       ; PT_CPLX_32
        db      8       ; PT_CHAR
        db      8       ; PT_STRUCT


fcode   TRASH_VALUE
        getdw   eax             ; get type
        _guess                  ; guess it's floating-point
          cmp   al,PT_REAL
          _quif b
          cmp   al,PT_CPLX
          _quif a
          fstp  st(0)           ; - clear floating-point stack
          cmp   al,PT_XTND
          _if   a               ; - if it's complex
            fstp st(0)          ; - - clear floating-point stack
          _endif                ; - endif
        _admit                  ; admit it's not floating-point
          movzx eax,byte ptr ValSize[eax] ; - cleanup CPU stack
          add   esp,eax         ; - cleanup CPU stack
        _endguess               ; endguess
        next
efcode  TRASH_VALUE

        fmodend
        end
