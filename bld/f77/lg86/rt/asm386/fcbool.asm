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
; FCBOOL        : boolean and bitwise operators
;

include fcdef.inc

        fmodstart       fcbool


fcode   BITAND                  ; perform binary AND function
        pop     eax             ; get operand
        and     [esp],eax       ; perform operation and store result
        next
efcode  BITAND


fcode   LOGAND                  ; perform logical AND function
        pop     eax             ; get arg1
        cmp     eax,1           ; C=0 if arg1!=0, C=1 if arg1==0
        sbb     eax,eax         ; eax=0 if arg1!=0, eax=-1 if arg1==0
        not     eax             ; eax=-1 if arg1!=0, eax=0 if arg1==0
        and     [esp],eax       ; perform the AND and store result
        next
efcode  LOGAND


fcode   LOGOR                   ; perform logical OR function
dfcode  BITOR                   ; perform binary OR function
        pop     eax             ; get arg1
        or      [esp],eax       ; perform operation and store result
        next
efcode  LOGOR


fcode   BITNOT                  ; perform binary NOT function
        not     dword ptr [esp] ; perform operation and store result
        next
efcode  BITNOT


fcode   LOGNOT                  ; perform logical NOT function
        pop     eax             ; get arg
        cmp     eax,1           ; C=0 if arg!=0, C=1 if arg==0
        sbb     eax,eax         ; eax=0 if arg!=0, eax=-1 if arg==0
        push    eax             ; push the result
        next
efcode  LOGNOT


fcode   LOGEQV                  ; perform logical EQV function
        pop     eax             ; get arg1
        pop     edx             ; get arg2
        cmp     eax,1           ; C=0 if arg1!=0, C=1 if arg1==0
        sbb     eax,eax         ; eax=0 if arg1!=0, eax=-1 if arg1==0
        cmp     edx,1           ; C=0 if arg2!=0, C=1 if arg2==0
        sbb     edx,edx         ; edx=0 if arg2!=0, edx=-1 if arg2==0
        xor     eax,edx         ; perform the comparison
        not     eax             ; not the result
        push    eax             ; push the result
        next
efcode  LOGEQV


fcode   BITEQV                  ; perform binary EQV function
        pop     eax             ; get arg1
        not     eax             ; ~arg1
        xor     [esp],eax       ; perform operation and store result
        next
efcode  BITEQV


fcode   LOGNEQV                 ; perform logical NEQV function
        pop     eax             ; get arg1
        pop     edx             ; get arg2
        cmp     eax,1           ; C=0 if arg1!=0, C=1 if arg1==0
        sbb     eax,eax         ; eax=0 if arg1!=0, eax=-1 if arg1==0
        cmp     edx,1           ; C=0 if arg2!=0, C=1 if arg2==0
        sbb     edx,edx         ; edx=0 if arg2!=0, edx=-1 if arg2==0
        xor     eax,edx         ; perform the comparison
        push    eax             ; push the result
        next
efcode  LOGNEQV


fcode   BITNEQV                 ; perform binary NEQV function
        pop     eax             ; get arg1
        xor     [esp],eax       ; perform operation and store result
        next
efcode  BITNEQV


;
; these functions are called right after a compare operation
; their job is to check for appropriate condition code and set
; TRUE or FALSE on the stack
;

fcode   EQ2LOGIC
dfcode  FLIP_EQ2LOGIC
        je      set_true        ; if operands equal set true
;;;;;;;;hop     set_false
efcode  EQ2LOGIC


defn    set_false
        push    0               ; push result
        next
endproc set_false


defn    set_true
        push    -1              ; push result
        next
endproc set_true


fcode   NE2LOGIC
dfcode  FLIP_NE2LOGIC
        jne     set_true        ; if operands were not equal set TRUE
        hop     set_false
efcode  NE2LOGIC


fcode   LT2LOGIC
dfcode  FLIP_GT2LOGIC
        jl      set_true        ; if op1 < op2 set TRUE
        hop     set_false
efcode  LT2LOGIC


fcode   LE2LOGIC
dfcode  FLIP_GE2LOGIC
        jle     set_true        ; if op1 <= op2 set TRUE
        hop     set_false
efcode  LE2LOGIC


fcode   GT2LOGIC
dfcode  FLIP_LT2LOGIC
        jg      set_true        ; if op1 > op2 set TRUE
        hop     set_false
efcode  GT2LOGIC


fcode   GE2LOGIC
dfcode  FLIP_LE2LOGIC
        jge     set_true        ; if op1 >= op2 set TRUE
        hop     set_false
efcode  GE2LOGIC

        fmodend
        end
