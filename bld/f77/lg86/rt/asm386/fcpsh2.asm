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
        getdw   edi             ; get address of variable
        add     edi,ebp
        next
efcode  PUSH_ADDR


fcode   PUSH_IND_ADDR           ; push address of indirect variable
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        next
efcode  PUSH_IND_ADDR


fcode   PUSH_ADDR_STACK         ; push address of variable on stack
        getdw   eax             ; get offset
        add     eax,ebp         ; turn into abs addr
        push    eax             ; push it
        next
efcode  PUSH_ADDR_STACK


fcode   PUSH_INLINE             ; push inline integer
        getdw   eax             ; get offset
        push    eax             ; push it
        next
efcode  PUSH_INLINE


fcode   PUSH_SCB                ; push string control block
dfcode  PUSH_RCB                ; push record control block
        getdw   eax             ; get addr of variable
        push    4[eax+ebp]      ; push SCB
        push    [eax+ebp]       ; ...
        next
efcode  PUSH_SCB


fcode   PUSH_SCB_LEN            ; push the length of a char variable
        getdw   eax             ; get the address of the SCB
        push    4[eax+ebp]      ; push the length from the SCB
        next
efcode  PUSH_SCB_LEN


;       Stack looks like this:
;       +8      SCB len
;       +4      SCB ptr
;       +0      Low bound of substring
;
fcode   PUSH_SCB_LEN_SAFE       ; push the length of a char variable
        push    8[esp]          ; push length from SCB on stack
        next
efcode  PUSH_SCB_LEN_SAFE


        fmodend
        end
