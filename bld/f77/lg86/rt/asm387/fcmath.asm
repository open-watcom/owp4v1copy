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
; FCMATH        : math routines for each type
;

.8087

include fcdef.inc
include stword.inc

        extrn   UndChk : near

        fmodstart       fcmath

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                   NEGATIONS
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   NEGI                    ; negate integer*4
        neg     dword ptr [esp] ; negate integer
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
dfcode  NEGR
dfcode  NEGE
        fchs                    ; negate top element of 8087 stack
        next                    ; execute next FCode
efcode  NEGD


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     addition
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_ADD
        pop     eax             ; get arg 1
        add     [esp],eax       ; do the addition
        into                    ; check for overflow
        next
efcode  II_ADD


fcode   ADD_L_I4
        getdw   edi             ; get address of operand
        add     edi,ebp
AddI4:
        pop     ecx             ; get previous result
        add     ecx,[edi]       ; add operand
        into                    ; catch overflow
        push    ecx             ; push result
        next
efcode  ADD_L_I4


fcode   ADD_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 AddI4; set return address
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
        fiadd   dword ptr [esp] ; do the addition
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
        faddp   st(1),st(0)     ; add top 2 elements of the 8087 stack
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
        faddp   st(2),st(0)     ; stack = i1 r1+r2 i2
        faddp   st(2),st(0)     ; stack = r1+r2 i1+i2
        next                    ; execute next F-Code
efcode  CC_ADD


fcode   ADD_L_R4
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     AddR4
efcode  ADD_L_R4


defn    AddR4
        fadd    dword ptr [edi] ; perform addition
        next                    ; execute next FCode
endproc AddR4


fcode   ADD_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 AddR4; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R4


fcode   ADD_L_R8
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     AddR8
efcode  ADD_L_R8


defn    AddR8
        fadd    qword ptr [edi] ; perform addition
        next                    ; execute next FCode
endproc AddR8


fcode   ADD_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 AddR8; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R8


fcode   ADD_I_R8
        getdw   eax                     ; get address of operand
        mov     edi,[eax+ebp]           ; ...
        fadd    qword ptr [edi]         ; perform addition
        next                            ; execute next FCode
efcode  ADD_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     subtraction
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_SUB
        pop     eax             ; get op 1
        pop     edx             ; get op 2
        sub     eax,edx         ; do subtraction
        into                    ; catch overflow
        push    eax             ; push result
        next
efcode  II_SUB


fcode   SUB_L_I4
        getdw   edi             ; get address of operand
        add     edi,ebp
SubI4:
        mov     ecx,[edi]       ; get op1
        pop     eax             ; get op2
        sub     ecx,eax         ; do the subtraction
        into                    ; catch overflow
        push    ecx             ; push result
        next
efcode  SUB_L_I4


fcode   SUB_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 SubI4; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_I4


fcode   IR_SUB
dfcode  ID_SUB
dfcode  IE_SUB
        fisubr  dword ptr [esp] ; do the subtraction
        next    4,1             ; wait for fpu then pop and continue
efcode  IR_SUB


fcode   RI_SUB
dfcode  DI_SUB
dfcode  EI_SUB
dfcode  CI_SUB
dfcode  QI_SUB
dfcode  XI_SUB
        fisub   dword ptr [esp] ; do the subtraction
        next    4,1             ; wait for fpu then pop and continue
efcode  RI_SUB


fcode   IC_SUB
dfcode  IQ_SUB
dfcode  IX_SUB
        fisubr  dword ptr [esp] ; do the subtraction
        fincstp                 ; point at imaginary part
        pop     eax             ; pop arg from cpu stack
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
        fsubrp  st(1),st(0)     ; <r1-r2>  i2
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
        fsubrp  st(1),st(0)     ; <r1-r2>
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
        fsubrp  st(2),st(0)     ; stack = i1 r1-r2 i2
        fsubrp  st(2),st(0)     ; stack = r1-r2 i1-i2
        next                    ; execute next F-Code
efcode  CC_SUB


fcode   SUB_L_R4
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     SubR4
efcode  SUB_L_R4


defn    SubR4
        fsubr   dword ptr [edi] ; perform subtraction
        next                    ; execute next FCode
endproc SubR4


fcode   SUB_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 SubR4; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R4


fcode   SUB_L_R8
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     SubR8
efcode  SUB_L_R8


defn    SubR8
        fsubr   qword ptr [edi] ; perform subtraction
        next
endproc SubR8


fcode   SUB_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 SubR8; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R8


fcode   SUB_I_R8
        getdw   eax                     ; get address of operand
        mov     edi,[eax+ebp]           ; ...
        fsubr   qword ptr [edi]         ; perform subtraction
        next
efcode  SUB_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     multiplication
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_MUL
        pop     eax             ; get op1
        pop     edx             ; get op2
        imul    eax,edx         ; do multiplication
        into                    ; check overflow
        push    eax             ; push result
        next
efcode  II_MUL


fcode   IR_MUL
dfcode  ID_MUL
dfcode  IE_MUL
dfcode  RI_MUL
dfcode  DI_MUL
dfcode  EI_MUL
        fimul   dword ptr [esp] ; do the multiplication
        next    4,1             ; wait for fpu then pop and continue
efcode  IR_MUL


fcode   CI_MUL
dfcode  QI_MUL
dfcode  XI_MUL
dfcode  IC_MUL
dfcode  IQ_MUL
dfcode  IX_MUL
        fimul   dword ptr [esp] ; multiply real part
        fincstp                 ; point at imag part
        fimul   dword ptr [esp] ; multiply imag part
        fdecstp                 ; point at real part
        next    4               ; pop from cpu stack
efcode  CI_MUL


fcode   MUL_L_I4
        getdw   edi             ; get address of operand
        add     edi,ebp
MulI4:
        pop     eax             ; get op1
        imul    eax,[edi]       ; multiply
        into                    ; check overflow
        push    eax             ; push result
        next
efcode  MUL_L_I4


fcode   MUL_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 MulI4; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_I4


fcode   MUL_L_R4
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     MulR4
efcode  MUL_L_R4


defn    MulR4
        fmul    dword ptr [edi] ; perform multiplication
        next                    ; execute next FCode
endproc MulR4


fcode   MUL_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 MulR4; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R4


fcode   MUL_L_R8
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     MulR8
efcode  MUL_L_R8


defn    MulR8
        fmul    qword ptr [edi] ; perform multiplication
        next
endproc MulR8


fcode   MUL_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 MulR8; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R8


fcode   MUL_I_R8
        getdw   edi                     ; get address of operand
        mov     eax,[edi+ebp]           ; ...
        fmul    qword ptr [eax]         ; perform multiplication
        next
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
        fmul    st(2),st(0)             ; r2 r1 r2*i1
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
        fmulp   st(1),st(0)     ; mult. top 2 elements of the 8087 stack
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
        fld     st(3)           ; <d>       a    b     c     d
        fchs                    ; <-d>      a    b     c     d
        fst     st(4)           ; <-d>      a    b     c    -d
        fmul    st(0),st(0)     ; <dd>      a    b     c    -d
        fld     st(3)           ; <c>       dd   a     b     c    -d
        fmul    st(0),st(0)     ; <cc>      dd   a     b     c    -d
        faddp   st(1),st(0)     ; <dd+cc>   a    b     c    -d
        fdiv    st(1),st(0)     ; <dd+cc>   a/q  b     c    -d    (q=dd+cc)
        fdivp   st(2),st(0)     ; <a/q>     b/q  c    -d
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
        fld     st(2)           ; <c>     a      b     c     d
        fmul    st(0),st(1)     ; <ac>    a      b     c     d
        fincstp                 ;  ac    <a>     b     c     d
        fmul    st(0),st(3)     ;  ac    <ad>    b     c     d
        fincstp                 ;  ac     ad    <b>    c     d
        fmul    st(2),st(0)     ;  ac     ad    <b>    c     bd
        fmul    st(0),st(1)     ;  ac     ad    <bc>   c     bd
        fdecstp                 ;  ac    <ad>    bc    c     bd
        fadd    st(1),st(0)     ;  ac    <ad>    ad+bc c     bd
        fdecstp                 ; <ac>    ad     ad+bc c     bd
        fsubrp  st(4),st(0)     ; <ac>    ad+bc  c     ac-bd
        fstp    st(0)           ; <ad+bc> c      ac-bd
        fstp    st(1)           ; <ad+bc> ac-bd
fxch_1_next:
        fxch    st(1)           ; <ac-bd> ad+bc
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
        fsubrp  st(2),st(0)     ; <i1>    r1-r2
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
        faddp   st(2),st(0)     ; i1 r1+r2
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
        fxch    st(2)           ; r2 i1 r1
        fmul    st(2),st(0)     ; r2 i1 r1*r2
        fmulp   st(1),st(0)     ; i1*r2 r1*r2
        hop     fxch_1_next
efcode  CR_MUL


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     division
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   II_DIV
        pop     eax             ; get dividend
II_DIV2:
        pop     ecx             ; get divisor
        cdq                     ; sign extend eax
        idiv    ecx             ; do division
        push    eax             ; push result back on stack
        next
efcode  II_DIV


fcode   DIV_L_I4
        getdw   edi             ; get address of operand
        add     edi,ebp
DivI4:  mov     eax,[edi]       ; get dividend
        hop     II_DIV2         ; continue
efcode  DIV_L_I4


fcode   DIV_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 DivI4; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_I4


fcode   IC_DIV
dfcode  IQ_DIV
dfcode  IX_DIV
        fild    dword ptr [esp] ; load the real part
        fld     st(1)           ; <a>      s    a   b
        pop     eax             ; pop from CPU stack
        hop     ic_DIV2
efcode  IC_DIV


fcode   RC_DIV                   ; <s>      a    b
dfcode  RQ_DIV
dfcode  RX_DIV
dfcode  DC_DIV
dfcode  DQ_DIV
dfcode  DX_DIV
dfcode  EC_DIV
dfcode  EQ_DIV
dfcode  EX_DIV
        fld     st(1)           ; <a>      s    a   b
ic_DIV2:
        fmul    st(0),st(0)     ; <aa>     s    a   b
        fld     st(3)           ; <b>      aa   s   a   b
        fmul    st(0),st(0)     ; <bb>     aa   s   a   b
        faddp   st(1),st(0)     ; <aa+bb>  s    a   b
        fdivrp  st(1),st(0)     ; <q/s>    a    b   (q=aa+bb)
        fdiv    st(1),st(0)     ; <q/s>    as/q b
        fchs                    ; <-q/s>   as/q b
        fdivp   st(2),st(0)     ; <as/q>   -bs/q
        next                    ; execute next F-Code
efcode  RC_DIV


fcode   CI_DIV
dfcode  QI_DIV
dfcode  XI_DIV
        fidiv   dword ptr [esp] ; <a/s> b
        fincstp                 ;  a/s <b>
        fidiv   dword ptr [esp] ;  a/s <b/s>
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
        fxch     st(2)          ; <s>    b   a
        fincstp                 ;  s    <b>  a
        fxch     st(1)          ;  s    <a>  b
        fdecstp                 ; <s>    a   b
        fdiv     st(2),st(0)    ; <s>    a   b/s
        fdivp    st(1),st(0)    ; <a/s>  b/s
        next                    ; execute next F-Code
efcode  CR_DIV


fcode   IR_DIV
dfcode  ID_DIV
dfcode  IE_DIV
        fidivr  dword ptr [esp] ; do the division
        next    4,1             ; pop from cpu stack
efcode  IR_DIV


fcode   RI_DIV
dfcode  DI_DIV
dfcode  EI_DIV
        fidiv   dword ptr [esp] ; do the division
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
        fdivrp  st(1),st(0)     ; <x/y>
        next                    ; execute next FCode
efcode  RR_DIV


fcode   DIV_L_R4
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     DivR4
efcode  DIV_L_R4


defn    DivR4
        fdivr   dword ptr [edi] ; perform division
        next                    ; execute next F-Code
endproc DivR4


fcode   DIV_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 DivR4; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R4


fcode   DIV_L_R8
        getdw   edi             ; get address of operand
        add     edi,ebp
;;;;;;;;hop     DivR8
efcode  DIV_L_R8


defn    DivR8
        fdivr   qword ptr [edi] ; perform division
        next                    ; execute next FCode
endproc DivR8


fcode   DIV_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 DivR8; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R8


fcode   DIV_I_R8
        getdw   edi             ; get address of operand
        mov     eax,[edi+ebp]   ; ...
        fdivr   qword ptr [eax] ; perform division
        next
efcode  DIV_I_R8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                COMPARISONS
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   II_CMP
        pop     eax             ; get op1
        pop     edx             ; get op2
        xor     edi,edi         ; assume numbers are equal
        sub     eax,edx         ; subtract operands
        _guess                  ; guess
          _quif ge              ; - quit if op1 >= op2
          dec   edi             ; - say op1 < op2
        _admit                  ; admit
          _quif e               ; - quit if op1 = op2
          inc   edi             ; - say op1 > op2
        _endguess               ; endguess
        or      edi,edi         ; set flags
        next
efcode  II_CMP


fcode   IR_CMP
dfcode  ID_CMP
dfcode  IE_CMP
        ficomp  dword ptr [esp] ; do the comparison (backwards)
        sub     edi,edi         ; assume equal
        fstsw   ax              ; get results
        pop     edx             ; remove arg from stack
        sahf                    ; get flags
        _guess
          _quif be              ; - quit if op2 <= op1
          dec   edi             ; - set flags
        _admit
          _quif e               ; - quit if op2 == op1
          inc   edi             ; - set flags
        _endif
        or      edi,edi         ; set condition codes
        next
efcode  IR_CMP


fcode   RI_CMP
dfcode  DI_CMP
dfcode  EI_CMP
        ficomp  dword ptr [esp] ; do the comparison
        sub     edi,edi         ; assume equal
        pop     edx             ; remove arg from stack
        fstsw   ax              ; get the results
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
        fstsw   ax              ; get condition codes (status word)
        sub     edi,edi         ; assume equal
DoCMP:
        sahf                    ; set flags
        _guess
          _quif be              ; - quit if op1 <= op2
          inc   edi             ; - set flags
        _admit
          _quif e               ; - quit if op1 == op2
          dec   edi             ; - set flags
        _endguess
        or      edi,edi         ; set condition codes
        next
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
        fxch    st(1)           ; <b>    a    s
        fxch    st(2)           ; <s>    a    b
;;;;;;;;hop     RC_CMP
efcode  CR_CMP


fcode   RC_CMP                   ; <s>    a    b
dfcode  RQ_CMP
dfcode  RX_CMP
dfcode  DC_CMP
dfcode  DQ_CMP
dfcode  DX_CMP
dfcode  EC_CMP
dfcode  EQ_CMP
dfcode  EX_CMP
        fldz                    ; <0>    s    a    b
        fxch    st(1)           ; <s>    0    a    b
;;;;;;;;hop     CC_CMP
efcode  RC_CMP


fcode   CC_CMP                  ; compare complex*8, complex*8
dfcode  CQ_CMP                  ; compare complex*8, complex*16
dfcode  CX_CMP                  ; compare complex*8, complex*20
dfcode  QC_CMP                  ; compare complex*16, complex*8
dfcode  QQ_CMP                  ; compare complex*16, complex*16
dfcode  QX_CMP                  ; compare complex*16, complex*20
dfcode  XC_CMP                  ; compare complex*20, complex*8
dfcode  XQ_CMP                  ; compare complex*20, complex*16
dfcode  XX_CMP                  ; compare complex*20, complex*20
                                ; stack = r1 i1 r2 i2
        fxch    st(1)           ; stack = i1 r1 r2 i2
        fcomp   st(3)           ; compare imaginary parts
                                ; stack = r1 r2 i2
        fstsw   ax              ; get condition codes (status word)
        sub     edi,edi         ; assume equal (for later)
        fcompp                  ; compare real parts
                                ; stack = i2
        mov     edx,eax         ; save condition code for imaginary part
        fstsw   ax              ; get condition codes (status word)
        fstp    st(0)           ; pop i2 from stack (wait done here)
        and     eax,edx         ; get final condition codes
        sahf                    ; get condition codes
        _if     ne              ; if op1 <> op2
          inc   edi             ; - set flag
        _endif                  ; endif
        or      edi,edi         ; set condition codes
        next                    ; execute next FCode
efcode  CC_CMP


fcode   IC_CMP
dfcode  IQ_CMP
dfcode  IX_CMP
dfcode  CI_CMP
dfcode  QI_CMP
dfcode  XI_CMP
        fldz                    ; 0   a   b
        fild    dword ptr [esp] ; s   0   a   b
        fwait                   ; wait for load to finish
        pop     eax             ; pop from CPU stack
        hop     CC_CMP
efcode  IC_CMP


        fmodend
        end
