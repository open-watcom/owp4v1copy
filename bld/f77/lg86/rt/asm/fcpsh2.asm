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
; FCPSH2        : Push routines common between 8086 and 8087
;

include fcdef.inc

        fmodstart       fcpsh


fcode   PUSH_ADDR               ; push address of variable (for INPUT)
        getword DI              ; get address of variable
        mov     AX,DS           ; get segment addr of variable
        mov     ES,AX           ; . . .
        next                    ; goto next routine
efcode  PUSH_ADDR


fcode   PUSH_IND_ADDR           ; push address of indirect variable
        getword DI              ; get address of variable
        les     DI,0[DI]        ; get address of variable
        next                    ; goto next rtn
efcode  PUSH_IND_ADDR


fcode   PUSH_ADDR_STACK         ; push address of variable on stack
        getword AX              ; get offset
        push    DS              ; push segment
        push    AX              ; push offset
        next                    ; continue
efcode  PUSH_ADDR_STACK


fcode   PUSH_INLINE             ; push an inline word onto the stack
        getword ax              ; get the value to push
        push    ax              ; put it on the stack
        next                    ; goto next routine
efcode  PUSH_INLINE


fcode   PUSH_SCB                ; push string control block
dfcode  PUSH_RCB                ; push record control block
        getword DI              ; get addr of variable
        push    4[DI]           ; push SCB
        push    2[DI]           ; . . .
        push    0[DI]           ; . . .
        next                    ; execute next F-Code
efcode  PUSH_SCB


fcode   PUSH_SCB_LEN            ; push the length of a char variable
        xor     bx,bx           ; get a zero
        getword ax              ; get the address of the SCB
        xchg    ax,bx           ; get zero and pointer into right regs
        push    ax              ; put a zero on stack
        push    4[bx]           ; push the length in the SCB
        next
efcode  PUSH_SCB_LEN


;       Stack looks like this:
;       +8      SCB len
;       +4      SCB ptr
;       +0      Low bound of substring
;
fcode   PUSH_SCB_LEN_SAFE       ; push the length of a char variable
        xor     bx,bx           ; get a zero
        mov     bp,sp           ; get stack frame
        push    bx              ; push zero
        push    8[bp]           ; push length from SCB on stack
        next
efcode  PUSH_SCB_LEN_SAFE


        fmodend
        end
