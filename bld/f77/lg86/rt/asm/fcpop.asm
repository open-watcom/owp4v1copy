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
;<>     FORTRAN-77 run-time support for 8086 based machines
;<>
;<>     - split frt8086 into many modules, this is one of them
;<>     - added def macro, started using stosw instruction
;<>  modified by G. Coschi, 22-mar-85, WATCOM Systems Inc.
;<>     - logicals are now 4-bytes on the stack
;<>  modified by G. Coschi, 24-apr-85, WATCOM Systems Inc.
;<>     - STORxx must preserve ES:DI for multiple assignment
;<> 93/11/27   Alex Brodsky      implemented COMPLEX*32 and REAL*16 support
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc

        fmodstart       fcpop


fcode   POP_LOG1                ; assign to logical*1
dfcode  POP_INT1                ; assign to integer*1
        getword DI              ; get addr of variable
        pop     AX              ; get value
        pop     DX              ; . . .
        mov     [DI],AL         ; assign to 1-byte variable
        next                    ; execute next F-Code
efcode  POP_LOG1


fcode   POP_IND_LOG1            ; assign to logical*1 indirect
dfcode  POP_IND_INT1            ; assign to integer*1 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
;;;;;;;;hop     STOR_INT1
efcode  POP_IND_LOG1


fcode   STOR_INT1
dfcode  STOR_LOG1
        pop     AX              ; get value
        pop     DX              ; . . .
        stosb                   ; assign to 1-byte variable
        dec     DI              ; reset pointer to value
        next                    ; execute next F-Code
efcode  STOR_INT1


fcode   POP_INT2                ; assign to integer*2
        getword DI              ; get addr of variable
        pop     AX              ; get value
        pop     DX              ; . . .
        mov     [DI],AX         ; assign to 2-byte variable
        next                    ; execute next F-Code
efcode  POP_INT2


fcode   POP_IND_INT2            ; assign to integer*2 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
;;;;;;;;hop     STOR_INT2
efcode  POP_IND_INT2


fcode   STOR_INT2
        pop     AX              ; get value
        pop     DX              ; . . .
        stosw                   ; assign to 2-byte variable
        dec     DI              ; reset pointer to value
        dec     DI              ; . . .
        next                    ; execute next F-Code
efcode  STOR_INT2


fcode   POP_REAL4               ; assign to real*4
dfcode  POP_LOG4                ; assign to logical*4
dfcode  POP_INT4                ; assign to integer*4
        getword DI              ; get addr of variable
        pop     0[DI]           ; assign 4-byte value
        pop     2[DI]           ; . . .
        next                    ; execute next F-Code
efcode  POP_REAL4


fcode   POP_IND_REAL4           ; assign to real*4 indirect
dfcode  POP_IND_LOG4            ; assign to logical*4 indirect
dfcode  POP_IND_INT4            ; assign to integer*4 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
;;;;;;;;hop     STOR_REAL4
efcode  POP_IND_REAL4


fcode   STOR_REAL4
dfcode  STOR_LOG4
dfcode  STOR_INT4
        pop     AX              ; assign 4-byte value
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        sub     DI,4            ; reset pointer to value
        next                    ; execute next F-Code
efcode  STOR_REAL4


fcode   POP_CPLX8               ; assign to complex*8
dfcode  POP_REAL8               ; assign to real*8
        getword DI              ; get addr of variable
        mov     AX,DS           ; set ES=DS
        mov     ES,AX           ; . . .
;;;;;;;;hop     STOR_CPLX8
efcode  POP_CPLX8


fcode   STOR_CPLX8              ; assign to complex*8 indirect
dfcode  STOR_REAL8              ; assign to real*8 indirect
        pop     AX              ; assign 8-byte value
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        sub     DI,8            ; reset pointer to value
        next                    ; execute next F-Code
efcode  STOR_CPLX8


fcode   POP_IND_CPLX8           ; assign to complex*8 indirect
dfcode  POP_IND_REAL8           ; assign to real*8 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
        hop     STOR_REAL8      ; pop value
efcode  POP_IND_CPLX8


fcode   POP_REAL16               ; assign to real*16
        getword DI              ; get addr of variable
        mov     AX,DS           ; set ES=DS
        mov     ES,AX           ; . . .
        hop     STOR_REAL16
efcode  POP_REAL16


fcode   POP_IND_REAL16          ; assign to real*10 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
        hop     STOR_REAL16     ; pop value
efcode  POP_IND_REAL16


fcode   POP_CPLX16              ; assign to complex*16
        getword DI              ; get addr of variable
        mov     AX,DS           ; set ES=DS
        mov     ES,AX           ; . . .
;;;;;;;;hop     STOR_CPLX16
efcode  POP_CPLX16


fcode   STOR_CPLX16
dfcode  STOR_REAL16
        pop     AX              ; assign 16-byte value
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        sub     DI,16           ; reset pointer to value
        next                    ; execute next F-Code
efcode  STOR_CPLX16


fcode   POP_IND_CPLX16          ; assign to complex*16 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
        hop     STOR_CPLX16     ; pop value
efcode  POP_IND_CPLX16


fcode   POP_CPLX32              ; assign to complex*32
        getword DI              ; get addr of variable
        mov     AX,DS           ; set ES=DS
        mov     ES,AX           ; . . .
;;;;;;;;hop     STOR_CPLX32
efcode  POP_CPLX32


fcode   STOR_CPLX32
        pop     AX              ; assign 20-byte value
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        sub     DI,32           ; reset pointer to value
        next                    ; execute next F-Code
efcode  STOR_CPLX32


fcode   POP_IND_CPLX32          ; assign to complex*32 indirect
        getword BX              ; get addr of pointer
        les     DI,0[BX]        ; get addr of value
        hop     STOR_CPLX32     ; pop value
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
        db      16      ; PT_REAL_10
        db      8       ; PT_CPLX_8
        db      16      ; PT_CPLX_16
        db      32      ; PT_CPLX_32
        db      6       ; PT_CHAR
        db      6       ; PT_STRUCT


fcode   TRASH_VALUE
        getword bx                              ; get type of value
        mov     al,byte ptr cs:ValSize[bx]      ; get size of value
        xor     ah,ah                           ; ...
        add     sp,ax                           ; cleanup CPU stack
        next                                    ; execute next F-Code
efcode  TRASH_VALUE

        fmodend
        end
