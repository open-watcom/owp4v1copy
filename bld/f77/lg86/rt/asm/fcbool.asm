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
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc

        fmodstart       fcbool


fcode   BITAND                  ; perform binary AND function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        and     AX,CX           ; and them
        and     BX,DX           ; . . .
        hop     push_bx_ax      ; push new result
efcode  BITAND


fcode   LOGAND                  ; perform logical AND function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        or      AX,BX           ; check if arg1 non-zero
        cmp     AX,1            ; C=0 if arg1!=0, C=1 if arg1==0
        sbb     AX,AX           ; AX=0 if arg1!=0, AX=-1 if arg1==0
        or      CX,DX           ; check if arg2 non-zero
        not     AX              ; AX=-1 if arg1!=0, AX=0 if arg1==0
        and     AX,CX           ; perform the AND operation
push_ax_ax:
        push    AX              ; push result
        push    AX              ; ...
        next
efcode  LOGAND


fcode   LOGOR                   ; perform logical OR function
dfcode  BITOR                   ; perform binary OR function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        or      AX,CX           ; or them together
        or      BX,DX           ; . . .
        hop     push_bx_ax      ; push the result
efcode  LOGOR


fcode   BITNOT                  ; perform binary NOT function
        pop     AX              ; get operand
        pop     BX              ; . . .
not_ax_bx:
        not     AX              ; not it
        not     BX              ; . . .
push_bx_ax:
        push    BX              ; push new result
        push    AX              ; . . .
        next                    ; go to next routine
efcode  BITNOT


fcode   LOGNOT                  ; perform logical NOT function
        pop     AX              ; get operand
        pop     BX              ; . . .
        or      AX,BX           ; test if non-zero (TRUE)
        cmp     AX,1            ; C=0 if arg!=0, C=1 if arg==0
        sbb     AX,AX           ; AX=0 if arg!=0, AX=-1 if arg==0
        hop     push_ax_ax      ; push the result
efcode  LOGNOT


fcode   LOGEQV                  ; perform logical EQV function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        or      AX,BX           ; test if TRUE
        cmp     AX,1            ; C=0 if arg1!=0, C=0 if arg1==0
        sbb     AX,AX           ; AX=0 if arg1!=0, AX=-1 if arg1==0
        or      CX,DX           ; test if TRUE
        cmp     CX,1            ; C=0 if arg2!=0, C=1 if arg2==0
        sbb     CX,CX           ; CX=0 if arg1!=0, CX=-1 if arg1==0
        xor     AX,CX           ; perform the comparison
        not     AX              ; not the result
        hop     push_ax_ax      ; push the result
efcode  LOGEQV


fcode   BITEQV                  ; perform binary EQV function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        xor     AX,CX           ; xor the operands
        xor     BX,DX           ; . . .
        hop     not_ax_bx       ; and not the result
efcode  BITEQV


fcode   LOGNEQV                 ; perform logical NEQV function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        or      AX,BX           ; test if TRUE
        cmp     AX,1            ; C=0 if arg1!=0, C=0 if arg1==0
        sbb     AX,AX           ; AX=0 if arg1!=0, AX=-1 if arg1==0
        or      CX,DX           ; test if TRUE
        cmp     CX,1            ; C=0 if arg2!=0, C=1 if arg2==0
        sbb     CX,CX           ; CX=0 if arg1!=0, CX=-1 if arg1==0
        xor     AX,CX           ; perform the comparison
        hop     push_ax_ax      ; push result
efcode  LOGNEQV


fcode   BITNEQV                 ; perform binary EQV function
        pop     AX              ; get operand
        pop     BX              ; . . .
        pop     CX              ; get operand
        pop     DX              ; . . .
        xor     AX,CX           ; xor the operands
        xor     BX,DX           ; . . .
        hop     push_bx_ax      ; push new result
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
        xor     AX,AX           ; set FALSE
        push    AX              ; push result
        push    AX              ; ...
        next
endproc set_false


defn    set_true
        mov     AX,-1           ; set TRUE
        push    AX              ; push boolean result
        push    AX              ; . . .
        next                    ; go to next routine
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
