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

.8087

include fcdef.inc
include stword.inc

        xref    ChkI4Mul_
        xref    Z_I4Div_

        fmodstart       fcmath

        extrn   UndChk : near


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                   NEGATIONS
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   NEGI                    ; negate integer*4
        pop     ax              ; get integer
        pop     dx              ; ...
        neg     dx              ; negate it
        neg     ax              ; ...
        sbb     dx,0            ; ...
        push    dx              ; push back
        push    ax              ;
        next
efcode  NEGI


fcode   NEGQ                    ; negate a double prec complex
dfcode  NEGC
dfcode  NEGX
        fincstp                 ; point to imaginary part
        fchs                    ; negate the imaginary part
        fdecstp                 ; point to real part
;;;;;;;;hop     NEGD
efcode  NEGQ


fcode   NEGD                    ; negate double prec real
dfcode  NEGE
dfcode  NEGR
        fchs                    ; negate top element of 8087 stack
        next                    ; execute next FCode
efcode  NEGD


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     addition
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_ADD
        pop     ax              ; get arg 1
        pop     dx              ; ...
        pop     bx              ; get arg 2
        pop     cx              ; ...
        add     ax,bx           ; do the addition
        adc     dx,cx           ; ...
        into                    ; interrupt if overflow
        push    dx              ; push result
        push    ax              ; ...
        next
efcode  II_ADD


fcode   ADD_L_I4
        getword DI              ; get address of operand
AddI4:  pop     CX              ; get previous result
        pop     BX              ; . . .
        add     CX,0[DI]        ; add low order word
        adc     BX,2[DI]        ; add high order word
        into                    ; catch overflow
        push    BX              ; push result back on stack
        push    CX              ; . . .
        next                    ; execute next FCode
efcode  ADD_L_I4


fcode   ADD_UL_I4
        mov     CX,4            ; set size of operand
        mov     BP,offset AddI4 ; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_I4


fcode   RI_ADD
dfcode  DI_ADD
dfcode  EI_ADD
dfcode  CI_ADD
dfcode  QI_ADD
dfcode  XI_ADD
dfcode  IR_ADD
dfcode  ID_ADD
dfcode  IE_ADD
dfcode  IC_ADD
dfcode  IQ_ADD
dfcode  IX_ADD
        mov     BP,SP           ; setup stack frame
        fiadd   dword ptr [BP]  ; do the addition
        next    4,1             ; wait for fpu then pop and continue
efcode  RI_ADD


fcode   DQ_ADD
dfcode  RR_ADD
dfcode  RD_ADD
dfcode  RE_ADD
dfcode  RC_ADD
dfcode  RQ_ADD
dfcode  RX_ADD
dfcode  DR_ADD
dfcode  DD_ADD
dfcode  DE_ADD
dfcode  DC_ADD
dfcode  DX_ADD
dfcode  ER_ADD
dfcode  ED_ADD
dfcode  EE_ADD
dfcode  EC_ADD
dfcode  EQ_ADD
dfcode  EX_ADD
        faddp   ST(1),ST(0)     ; add top 2 elements of the 8087 stack
        next                    ; execute next FCode
efcode  DQ_ADD


fcode   CC_ADD                   ; stack = r1 i1 r2 i2
dfcode  CQ_ADD
dfcode  CX_ADD
dfcode  QC_ADD
dfcode  QQ_ADD
dfcode  QX_ADD
dfcode  XC_ADD
dfcode  XQ_ADD
dfcode  XX_ADD
        faddp   ST(2),ST(0)     ; stack = i1 r1+r2 i2
        faddp   ST(2),ST(0)     ; stack = r1+r2 i1+i2
        next                    ; execute next F-Code
efcode  CC_ADD


fcode   ADD_L_R4
        getword DI              ; get address of operand
;;;;;;;;hop     AddR4
efcode  ADD_L_R4


defn    AddR4
        fadd    dword ptr [DI]  ; perform addition
        next                    ; execute next FCode
endproc AddR4


fcode   ADD_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset AddR4 ; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R4


fcode   ADD_L_R8
        getword DI              ; get address of operand
;;;;;;;;hop     AddR8
efcode  ADD_L_R8


defn    AddR8
        fadd    qword ptr [DI]  ; perform addition
        next                    ; execute next FCode
endproc AddR8


fcode   ADD_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset AddR8 ; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R8


fcode   ADD_I_R8
        getword DI                      ; get address of operand
        les     DI,[DI]                 ; . . .
        fadd    qword ptr ES:[DI]       ; perform addition
        next                            ; execute next FCode
efcode  ADD_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     subtraction
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_SUB
        pop     BX              ; get low order word of op1
        pop     AX              ; get high order word
        pop     DX              ; get lo word of op2
        pop     CX              ; get high word of op2
        sub     BX,DX           ; subtract low order words
        sbb     AX,CX           ; subtract high order words
        into                    ; gen interrupt on overflow condition
        push    AX              ; push result back on stack
        push    BX              ; . . .
        next                    ; execute next FCode
efcode  II_SUB


fcode   SUB_L_I4
        lodsw                   ; get address of operand
        mov     DI,AX           ; . . .
SubI4:  mov     CX,0[DI]        ; get low order word (op1)
        mov     BX,2[DI]        ; get high order word (op1)
        pop     AX              ; get low word (op2)
        pop     DX              ; get high word (op2)
        sub     CX,AX           ; subtract low order word
        sbb     BX,DX           ; subtract high order word
        into                    ; catch overflow
        push    BX              ; push high word of answer
        push    CX              ; push low word of answer
        next                    ; execute next FCode
efcode  SUB_L_I4


fcode   SUB_UL_I4
        mov     CX,4            ; set size of operand
        mov     BP,offset SubI4 ; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_I4


fcode   IR_SUB
dfcode  ID_SUB
dfcode  IE_SUB
        mov     bp,sp           ; get stack frame
        fisubr  dword ptr [bp]  ; do the subtraction
        next    4,1             ; wait for fpu then pop and continue
efcode  IR_SUB


fcode   RI_SUB
dfcode  DI_SUB
dfcode  EI_SUB
dfcode  CI_SUB
dfcode  QI_SUB
dfcode  XI_SUB
        mov     bp,sp           ; get stack frame
        fisub   dword ptr [bp]  ; do the subtraction
        next    4,1             ; wait for fpu then pop and continue
efcode  RI_SUB


fcode   IC_SUB
dfcode  IQ_SUB
dfcode  IX_SUB
        mov     bp,sp           ; get stack frame
        fisubr  dword ptr [bp]  ; do the subtraction
        fincstp                 ; point at imaginary part
        add     sp,4            ; pop arg from cpu stack
        hop     chs_dec_next
efcode  IC_SUB


fcode   RC_SUB                   ; <r1>     r2     i2
dfcode  RQ_SUB
dfcode  RX_SUB
dfcode  DC_SUB
dfcode  DQ_SUB
dfcode  DX_SUB
dfcode  EC_SUB
dfcode  EQ_SUB
dfcode  EX_SUB
        fsubrp  ST(1),ST(0)     ; <r1-r2>  i2
        fincstp                 ;  r1-r2  <i2>
chs_dec_next:
        fchs                    ;  r1-r2  <-i2>
        fdecstp                 ; <r1-r2> <-i2>
        next
efcode  RC_SUB


fcode   RR_SUB                  ; r1    r2
dfcode  RD_SUB
dfcode  RE_SUB
dfcode  DR_SUB
dfcode  DD_SUB
dfcode  DE_SUB
dfcode  ER_SUB
dfcode  ED_SUB
dfcode  EE_SUB
        fsubrp  ST(1),ST(0)     ; <r1-r2>
        next
efcode  RR_SUB


fcode   CC_SUB                   ; stack = r1 i1 r2 i2
dfcode  CQ_SUB
dfcode  CX_SUB
dfcode  QC_SUB
dfcode  QQ_SUB
dfcode  QX_SUB
dfcode  XC_SUB
dfcode  XQ_SUB
dfcode  XX_SUB
        fsubrp  ST(2),ST(0)     ; stack = i1 r1-r2 i2
        fsubrp  ST(2),ST(0)     ; stack = r1-r2 i1-i2
        next                    ; execute next F-Code
efcode  CC_SUB


fcode   SUB_L_R4
        getword DI              ; get address of operand
;;;;;;;;hop     SubR4
efcode  SUB_L_R4


defn    SubR4
        fsubr   dword ptr [DI]  ; perform subtraction
        next                    ; execute next FCode
endproc SubR4


fcode   SUB_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset SubR4 ; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R4


fcode   SUB_L_R8
        getword DI              ; get address of operand
;;;;;;;;hop     SubR8
efcode  SUB_L_R8


defn    SubR8
        fsubr   qword ptr [DI]  ; perform subtraction
        next                    ; execute next FCode
endproc SubR8


fcode   SUB_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset SubR8 ; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R8


fcode   SUB_I_R8
        getword DI                      ; get address of operand
        les     DI,[DI]                 ; . . .
        fsubr   qword ptr ES:[DI]       ; perform subtraction
        next                            ; execute next FCode
efcode  SUB_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     multiplication
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_MUL
        pop     BX              ; get lo word of op2
        pop     CX              ; get high word of op2
        mov     AX, SP          ; get pointer to op 1
        mov     DX, SS          ; ...
        docall  ChkI4Mul_       ; multiply with overflow checking
        next                    ; execute next FCode
efcode  II_MUL


fcode   IR_MUL
dfcode  ID_MUL
dfcode  IE_MUL
dfcode  RI_MUL
dfcode  DI_MUL
dfcode  EI_MUL
        mov     bp,sp           ; get stack frame
        fimul   dword ptr [bp]  ; do the multiplication
        next    4,1             ; wait for fpu then pop and continue
efcode  IR_MUL


fcode   CI_MUL
dfcode  IQ_MUL
dfcode  IX_MUL
dfcode  IC_MUL
dfcode  QI_MUL
dfcode  XI_MUL
        mov     bp,sp           ; get stack frame
        fimul   dword ptr [bp]  ; multiply real part
        fincstp                 ; point at imag part
        fimul   dword ptr [bp]  ; multiply imag part
        fdecstp                 ; point at real part
        next    4               ; pop from cpu stack
efcode  CI_MUL


fcode   MUL_L_I4
        getword DI              ; get address of operand
MulI4:  mov     BX,0[DI]        ; get low order word
        mov     CX,2[DI]        ; get high order word
        mov     AX, SP          ; get pointer to other op
        mov     DX, SS          ; ...
        docall  ChkI4Mul_       ; multiply with overflow checking
        next                    ; execute next FCode
efcode  MUL_L_I4


fcode   MUL_UL_I4
        mov     CX,4            ; set size of operand
        mov     BP,offset MulI4 ; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_I4


fcode   MUL_L_R4
        getword DI              ; get address of operand
;;;;;;;;hop     MulR4
efcode  MUL_L_R4


defn    MulR4
        fmul    dword ptr [DI]  ; perform multiplication
        next                    ; execute next FCode
endproc MulR4


fcode   MUL_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset MulR4 ; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R4


fcode   MUL_L_R8
        getword DI              ; get address of operand
;;;;;;;;hop     MulR8
efcode  MUL_L_R8


defn    MulR8
        fmul    qword ptr [DI]  ; perform multiplication
        next                    ; execute next FCode
endproc MulR8


fcode   MUL_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset MulR8 ; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R8


fcode   MUL_I_R8
        getword DI                      ; get address of operand
        les     DI,[DI]                 ; . . .
        fmul    qword ptr ES:[DI]       ; perform multiplication
        next                            ; execute next FCode
efcode  MUL_I_R8


fcode   RC_MUL
dfcode  DC_MUL
dfcode  EC_MUL
dfcode  RQ_MUL
dfcode  DQ_MUL
dfcode  EQ_MUL
dfcode  RX_MUL
dfcode  DX_MUL
dfcode  EX_MUL
        fmul    ST(2),ST(0)             ; r2 r1 r2*i1
;;;;;;;;hop     RR_MUL
efcode  RC_MUL


fcode   RR_MUL
dfcode  RD_MUL
dfcode  RE_MUL
dfcode  DR_MUL
dfcode  DD_MUL
dfcode  DE_MUL
dfcode  ER_MUL
dfcode  ED_MUL
dfcode  EE_MUL
        fmulp   ST(1),ST(0)     ; mult. top 2 elements of the 8087 stack
        next                    ; execute next FCode
efcode  RR_MUL


; complex division
;    (a,b)/(c,d) = ((a,b)*(c,-d))/(c**2+d**2)

fcode   CC_DIV                  ; <a>       b    c     d
dfcode  CQ_DIV
dfcode  CX_DIV
dfcode  QC_DIV
dfcode  QQ_DIV
dfcode  QX_DIV
dfcode  XC_DIV
dfcode  XQ_DIV
dfcode  XX_DIV
        fld     ST(3)           ; <d>       a    b     c     d
        fchs                    ; <-d>      a    b     c     d
        fst     ST(4)           ; <-d>      a    b     c    -d
        fmul    ST(0),ST(0)     ; <dd>      a    b     c    -d
        fld     ST(3)           ; <c>       dd   a     b     c    -d
        fmul    ST(0),ST(0)     ; <cc>      dd   a     b     c    -d
        faddp   ST(1),ST(0)     ; <dd+cc>   a    b     c    -d
        fdiv    ST(1),ST(0)     ; <dd+cc>   a/q  b     c    -d    (q=dd+cc)
        fdivp   ST(2),ST(0)     ; <a/q>     b/q  c    -d
;;;;;;;;hop     CC_MUL          ; multiply
efcode  CC_DIV


; complex multiplication
;    (a,b) * (c,d) = (a*c-b*d, a*d+b*c)
;    < > indicates the top element of the 8087 stack

fcode   CC_MUL                  ;  a      b      c     d
dfcode  CQ_MUL
dfcode  CX_MUL
dfcode  QC_MUL
dfcode  QQ_MUL
dfcode  QX_MUL
dfcode  XC_MUL
dfcode  XQ_MUL
dfcode  XX_MUL
        fld     ST(2)           ; <c>     a      b     c     d
        fmul    ST(0),ST(1)     ; <ac>    a      b     c     d
        fincstp                 ;  ac    <a>     b     c     d
        fmul    ST(0),ST(3)     ;  ac    <ad>    b     c     d
        fincstp                 ;  ac     ad    <b>    c     d
        fmul    ST(2),ST(0)     ;  ac     ad    <b>    c     bd
        fmul    ST(0),ST(1)     ;  ac     ad    <bc>   c     bd
        fdecstp                 ;  ac    <ad>    bc    c     bd
        fadd    ST(1),ST(0)     ;  ac    <ad>    ad+bc c     bd
        fdecstp                 ; <ac>    ad     ad+bc c     bd
        fsubrp  ST(4),ST(0)     ; <ac>    ad+bc  c     ac-bd
        fstp    ST(0)           ; <ad+bc> c      ac-bd
        fstp    ST(1)           ; <ad+bc> ac-bd
fxch_1_next:
        fxch    ST(1)           ; <ac-bd> ad+bc
        next
efcode  CC_MUL


fcode   CR_SUB                   ; <r1>    i1    r2
dfcode  CD_SUB
dfcode  CE_SUB
dfcode  QR_SUB
dfcode  QD_SUB
dfcode  QE_SUB
dfcode  XR_SUB
dfcode  XD_SUB
dfcode  XE_SUB
        fsubrp  ST(2),ST(0)     ; <i1>    r1-r2
        hop     fxch_1_next     ; <r1-r2> i1
efcode  CR_SUB


fcode   CR_ADD                   ; r1 i1 r2
dfcode  CD_ADD
dfcode  CE_ADD
dfcode  QR_ADD
dfcode  QD_ADD
dfcode  QE_ADD
dfcode  XR_ADD
dfcode  XD_ADD
dfcode  XE_ADD
        faddp   ST(2),ST(0)     ; i1 r1+r2
        hop     fxch_1_next     ; r1+r2 i1
efcode  CR_ADD


fcode   CR_MUL                  ; r1 i1 r2
dfcode  CD_MUL
dfcode  CE_MUL
dfcode  QR_MUL
dfcode  QD_MUL
dfcode  QE_MUL
dfcode  XR_MUL
dfcode  XD_MUL
dfcode  XE_MUL
        fxch    ST(2)           ; r2 i1 r1
        fmul    ST(2),ST(0)     ; r2 i1 r1*r2
        fmulp   ST(1),ST(0)     ; i1*r2 r1*r2
        hop     fxch_1_next
efcode  CR_MUL


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     division
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_DIV
        pop     AX              ; get low order word of dividend
        pop     DX              ; get high order word
II_DIV2: pop     BX              ; get lo word of divisor
        pop     CX              ; get high word
        mov     DI,CX           ; see if divisor is zero
        or      DI,BX           ; ...
        _if     ne              ; if not then
          docall  Z_I4Div_      ; - do 4-byte integer divide
          push  DX              ; - push result back on stack
          push  AX              ; - ...
          next                  ; - execute next FCode
        _endif                  ; endif
        int     0H              ; report a divide by zero attempt
efcode  II_DIV


fcode   DIV_L_I4
        getword DI              ; get address of operand
DivI4:  mov     AX,0[DI]        ; get low order word
        mov     DX,2[DI]        ; get high order word
        hop     II_DIV2          ; continue
efcode  DIV_L_I4


fcode   DIV_UL_I4
        mov     CX,4            ; set size of operand
        mov     BP,offset DivI4 ; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_I4


fcode   IC_DIV
dfcode  IQ_DIV
dfcode  IX_DIV
        mov     bp,sp           ; get stack frame
        fild    dword ptr [bp]  ; load the real part
        fld     ST(1)           ; <a>      s    a   b
        add     sp,4            ; pop from CPU stack
        hop     ic_DIV2
efcode  IC_DIV


fcode   RC_DIV                   ; <s>      a    b
dfcode  DC_DIV
dfcode  EC_DIV
dfcode  RQ_DIV
dfcode  DQ_DIV
dfcode  EQ_DIV
dfcode  RX_DIV
dfcode  DX_DIV
dfcode  EX_DIV
        fld     ST(1)           ; <a>      s    a   b
ic_DIV2:
        fmul    ST(0),ST(0)     ; <aa>     s    a   b
        fld     ST(3)           ; <b>      aa   s   a   b
        fmul    ST(0),ST(0)     ; <bb>     aa   s   a   b
        faddp   ST(1),ST(0)     ; <aa+bb>  s    a   b
        fdivrp  ST(1),ST(0)     ; <q/s>    a    b   (q=aa+bb)
        fdiv    ST(1),ST(0)     ; <q/s>    as/q b
        fchs                    ; <-q/s>   as/q b
        fdivp   ST(2),ST(0)     ; <as/q>   -bs/q
        next                    ; execute next F-Code
efcode  RC_DIV


fcode   CI_DIV
dfcode  QI_DIV
dfcode  XI_DIV
        mov     bp,sp           ; get stack frame
        fidiv   dword ptr [bp]  ; <a/s> b
        fincstp                 ;  a/s <b>
        fidiv   dword ptr [bp]  ;  a/s <b/s>
        fdecstp                 ;  a/s  b/s
        next    4
efcode  CI_DIV


fcode   CR_DIV                   ; <a>    b   s
dfcode  CD_DIV
dfcode  CE_DIV
dfcode  QR_DIV
dfcode  QD_DIV
dfcode  QE_DIV
dfcode  XR_DIV
dfcode  XD_DIV
dfcode  XE_DIV
        fxch     ST(2)          ; <s>    b   a
        fincstp                 ;  s    <b>  a
        fxch     ST(1)          ;  s    <a>  b
        fdecstp                 ; <s>    a   b
        fdiv     ST(2),ST(0)    ; <s>    a   b/s
        fdivp    ST(1),ST(0)    ; <a/s>  b/s
        next                    ; execute next F-Code
efcode  CR_DIV


fcode   IR_DIV
dfcode  ID_DIV
dfcode  IE_DIV
        mov     bp,sp           ; get stack frame
        fidivr  dword ptr [bp]  ; do the division
        next    4,1             ; pop from cpu stack
efcode  IR_DIV


fcode   RI_DIV
dfcode  DI_DIV
dfcode  EI_DIV
        mov     bp,sp           ; get stack frame
        fidiv   dword ptr [bp]  ; do the division
        next    4,1             ; pop from CPU stack
efcode  RI_DIV


fcode   RR_DIV                  ; <x>   y
dfcode  RD_DIV
dfcode  RE_DIV
dfcode  DR_DIV
dfcode  DD_DIV
dfcode  DE_DIV
dfcode  ER_DIV
dfcode  ED_DIV
dfcode  EE_DIV
        fdivrp  ST(1),ST(0)     ; <x/y>
        next                    ; execute next FCode
efcode  RR_DIV


fcode   DIV_L_R4
        getword DI              ; get address of operand
;;;;;;;;hop     DivR4
efcode  DIV_L_R4


defn    DivR4
        fdivr   dword ptr [DI]  ; perform division
        next                    ; execute next F-Code
endproc DivR4


fcode   DIV_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset DivR4 ; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R4


fcode   DIV_L_R8
        getword DI              ; get address of operand
;;;;;;;;hop     DivR8
efcode  DIV_L_R8


defn    DivR8
        fdivr   qword ptr [DI]  ; perform division
        next                    ; execute next FCode
endproc DivR8


fcode   DIV_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset DivR8 ; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R8


fcode   DIV_I_R8
        getword DI                      ; get address of operand
        les     DI,[DI]                 ; . . .
        fdivr   qword ptr ES:[DI]       ; perform division
        next                            ; execute next FCode
efcode  DIV_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                COMPARISONS
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   II_CMP
        pop     AX              ; get low order word of op1
        pop     BX              ; get high order word
        pop     CX              ; get lo word of op2
        pop     DX              ; get high word of op2
        sub     DI,DI           ; assume numbers are equal
        sub     AX,CX           ; subtract low order words
        sbb     BX,DX           ; subtract high order words
        _guess                  ; guess
          _quif ge              ; - quit if op1 >= op2
          dec   DI              ; - say op1 < op2
        _admit                  ; admit
          _if   e               ; - if high words are equal
            or  AX,AX           ; - - check low words
          _endif                ; - endif
          _quif e               ; - quif if op1 = op2
          inc   DI              ; - say op1 > op2
        _endguess               ; endguess
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  II_CMP


fcode   IR_CMP
dfcode  ID_CMP
dfcode  IE_CMP
        mov     bp,sp           ; get stack frame
        ficomp  dword ptr [bp]  ; do the comparison (backwards)
        sub     di,di           ; assume equal
        fstsw   word ptr 2[bp]  ; get the results
        pop     DX              ; throw away
        fwait                   ; wait for the fpu
        pop     AX              ; get results
        sahf                    ; get flags
        _guess
          _quif be              ; - quit if op2 <= op1
          dec   di              ; - set flags
        _admit
          _quif e               ; - quit if op2 == op1
          inc   di              ; - set flags
        _endif
        or      DI,DI           ; set condition codes
        next                    ; execute next FCode
efcode  IR_CMP


fcode   RI_CMP
dfcode  DI_CMP
dfcode  EI_CMP
        mov     bp,sp           ; get stack frame
        ficomp  dword ptr [bp]  ; do the comparison
        sub     di,di           ; assume equal
        fstsw   word ptr 2[bp]  ; get the results
        pop     DX              ; throw away
        hop     DoCMP
efcode  RI_CMP


fcode   RR_CMP
dfcode  RD_CMP
dfcode  RE_CMP
dfcode  DR_CMP
dfcode  DD_CMP
dfcode  DE_CMP
dfcode  ER_CMP
dfcode  ED_CMP
dfcode  EE_CMP
        fcompp                  ; compare top 2 elements of stack and pop
                                ; them both
        push    AX              ; allocate a temporary
        mov     BP,SP           ; point to temporary
        fstsw   word ptr [bp]   ; get condition codes (status word)
        sub     DI,DI           ; assume equal
DoCMP:
        fwait                   ; wait for 8087
        pop     AX              ; . . . (frees temporary as well)
        sahf                    ; set flags
        _guess
          _quif be              ; - quit if op1 <= op2
          inc   di              ; - set flags
        _admit
          _quif e               ; - quit if op1 == op2
          dec   di              ; - set flags
        _endguess
        or      DI,DI           ; set condition codes
        next                    ; execute next FCode
efcode  RR_CMP


fcode   CR_CMP                   ; <a>    b    s
dfcode  CD_CMP
dfcode  CE_CMP
dfcode  QR_CMP
dfcode  QD_CMP
dfcode  QE_CMP
dfcode  XR_CMP
dfcode  XD_CMP
dfcode  XE_CMP
        fxch    ST(1)           ; <b>    a    s
        fxch    ST(2)           ; <s>    a    b
;;;;;;;;hop     RC_CMP
efcode  CR_CMP


fcode   RC_CMP                   ; <s>    a    b
dfcode  DC_CMP
dfcode  EC_CMP
dfcode  RQ_CMP
dfcode  DQ_CMP
dfcode  EQ_CMP
dfcode  RX_CMP
dfcode  DX_CMP
dfcode  EX_CMP
        fldz                    ; <0>    s    a    b
        fxch    ST(1)           ; <s>    0    a    b
;;;;;;;;hop     CC_CMP
efcode  RC_CMP


fcode   CC_CMP                   ; compare complex*8, complex*8
dfcode  CQ_CMP                   ; compare complex*8, complex*16
dfcode  CX_CMP                   ; compare complex*8, complex*20
dfcode  QC_CMP                   ; compare complex*16, complex*8
dfcode  QQ_CMP                   ; compare complex*16, complex*16
dfcode  QX_CMP                   ; compare complex*16, complex*20
dfcode  XC_CMP                   ; compare complex*16, complex*8
dfcode  XQ_CMP                   ; compare complex*16, complex*16
dfcode  XX_CMP                   ; compare complex*20, complex*20
                                ; stack = r1 i1 r2 i2
        fxch    ST(1)           ; stack = i1 r1 r2 i2
        push    AX              ; allocate a temporary
        fcomp   ST(3)           ; compare imaginary parts
                                ; stack = r1 r2 i2
        mov     BP,SP           ; point to temporary
        fstsw   word ptr [BP]   ; get condition codes (status word)
        sub     DI,DI           ; assume equal (for later)
        fcompp                  ; compare real parts
                                ; stack = i2
        mov     AX,[BP]         ; save condition code for imaginary part
        fstsw   word ptr [BP]   ; get condition codes (status word)
        fstp    ST(0)           ; pop i2 from stack (wait done here)
        pop     DX              ; get condition codes for real part
        and     AX,DX           ; get final condition codes
        sahf                    ; get condition codes
        _if     ne              ; if op1 <> op2
          inc   DI              ; - set flag
        _endif                  ; endif
        or      DI,DI           ; set condition codes
        next                    ; execute next FCode
efcode  CC_CMP


fcode   IC_CMP
dfcode  IQ_CMP
dfcode  IX_CMP
dfcode  CI_CMP
dfcode  QI_CMP
dfcode  XI_CMP
        fldz                    ; 0   a   b
        mov     bp,sp           ; get stack frame
        fild    dword ptr [bp]  ; s   0   a   b
        fwait                   ; wait for load to finish
        add     sp,4            ; pop from CPU stack
        hop     CC_CMP
efcode  IC_CMP


        fmodend
        end
