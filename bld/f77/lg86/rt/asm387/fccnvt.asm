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
; FCCNVT        : type conversion routines
;

.8087

include fcdef.inc
include stword.inc

        fmodstart       fccnvt


fcode   CI_CNV                  ; convert c8 to i4
dfcode  QI_CNV                  ; convert c16 to i4 <a>  b
dfcode  XI_CNV                  ; convert c20 to i4 <a>  b
        fstp    st(1)           ;                   <a>
;;;;;;;;hop     RI_CNV
efcode  CI_CNV


fcode   RI_CNV
dfcode  DI_CNV
dfcode  EI_CNV
        sub     esp,4           ; allocate temps on cpu stack
        call    SetChop         ; set to chop
        fistp   dword ptr [esp] ; move to CPU stack
        call    RestRnd         ; restore round control
;;;;;;;;hop     II_CNV
efcode  RI_CNV


fcode   II_CNV                  ; nop conversions
dfcode  RR_CNV
dfcode  DD_CNV
dfcode  EE_CNV
dfcode  CC_CNV
dfcode  QQ_CNV
dfcode  XX_CNV
        next                    ; execute next F-Code
efcode  II_CNV


fcode   DC_CNV                  ; convert r8 to c8  <a>
        fldz                    ;                   <0>  a
        fxch                    ;                   <a>  0
;;;;;;;;hop     DR_CNV
efcode  DC_CNV


fcode   DR_CNV
        call    ChopDble        ; chop double to real
        next                    ; execute next F-Code
efcode  DR_CNV


fcode   EC_CNV                  ; convert r10 to c8  <a>
        fldz                    ;                   <0>  a
        fxch                    ;                   <a>  0
;;;;;;;;hop     ER_CNV
efcode  EC_CNV


fcode   ER_CNV
        call    ChopLong        ; chop Long to double
        hop     DR_CNV          ; jump to Double->real converter
efcode  ER_CNV


fcode   EQ_CNV                  ; convert r10 to c16  <a>
        fldz                    ;                   <0>  a
        fxch                    ;                   <a>  0
;;;;;;; hop     ED_CNV
efcode  EQ_CNV


fcode   ED_CNV
        call    ChopLong        ; chop long to double
        next                    ; execute next F-Code
efcode  ED_CNV


fcode   XR_CNV                  ; convert c20 to r4  <a>  b
        fstp    st(1)           ;                    <a>
        hop     ER_CNV          ; convert real part to type real
efcode  XR_CNV


fcode   XD_CNV                  ; convert c20 to r8  <a>  b
        fstp    st(1)           ;                    <a>
        hop     ED_CNV          ; convert real part to type double
efcode  XD_CNV


fcode   QR_CNV                  ; convert c16 to r4  <a>  b
        fstp    st(1)           ;                    <a>
        hop     DR_CNV          ; convert real part to type real
efcode  QR_CNV


fcode   XC_CNV                  ; convert c20 to c8
        fincstp                 ; point to imaginary part
        call    ChopLong        ; chop imaginary part
        fdecstp                 ; point to real part
        call    ChopLong        ; chop real part
;;;;;;  hop     QC_CNV
efcode  XC_CNV


fcode   QC_CNV                  ; convert c16 to c8
        fincstp                 ; point to imaginary part
        call    ChopDble        ; chop imaginary part
        fdecstp                 ; point to real part
        hop     DR_CNV
efcode  QC_CNV


fcode   XQ_CNV                  ; convert c20 to c8
        fincstp                 ; point to imaginary part
        call    ChopLong        ; chop imaginary part
        fdecstp                 ; point to real part
        hop     ED_CNV
efcode  XQ_CNV


fcode   IC_CNV                  ; convert i4 to c8
dfcode  IQ_CNV                  ; convert i4 to c16
dfcode  IX_CNV                  ; convert i4 to c20
        fldz                    ; load the imaginary part
;;;;;;;;hop     IR_CNV
efcode  IC_CNV


fcode   IR_CNV                  ; convert i4 to r4
dfcode  ID_CNV                  ; convert i4 to r8
dfcode  IE_CNV                  ; convert i4 to r8
        fild    dword ptr [esp] ; push onto FPU stack
        next    4,1             ; pop from CPU stack and continue
efcode  IR_CNV


fcode   RC_CNV                  ; convert r4 to c8
dfcode  RQ_CNV                  ; convert r4 to c16
dfcode  RX_CNV                  ; convert r4 to c20
dfcode  DQ_CNV                  ; convert r8 to c16 <s>
dfcode  DX_CNV                  ; convert r8 to c20 <s>
dfcode  EX_CNV                  ; convert r8 to c20 <s>
        fldz                    ;                   <0> s
        fxch    st(1)           ;                   <s> 0
        next                    ; execute next F-Code
efcode  RC_CNV


fcode   CR_CNV                  ; convert c8 to r4
dfcode  CD_CNV                  ; convert c8 to r8
dfcode  CE_CNV                  ; convert c8 to r10
dfcode  QD_CNV                  ; convert c16 to r8 <a>  b
dfcode  QE_CNV                  ; convert c16 to r10 <a>  b
dfcode  XE_CNV                  ; convert c20 to r10 <a>  b
        fstp    st(1)           ;                   <a>
;;;;;;;;hop     RD_CNV
efcode  CR_CNV


fcode   RD_CNV                  ; convert r4 to r8
dfcode  RE_CNV                  ; convert r4 to r10
dfcode  DE_CNV                  ; convert r8 to r10
dfcode  CQ_CNV                  ; convert c8 to c20
dfcode  CX_CNV                  ; convert c8 to c20
dfcode  QX_CNV                  ; convert c8 to c20
        next                    ; execute next F-Code
efcode  RD_CNV


        xdefp   ChopDble
defn    ChopDble
        push    eax             ; allocate temporary
        fstp    dword ptr [esp] ; chop the double to real
        fld     dword ptr [esp] ; load it as a real
        fwait                   ; wait for load to complete
        add     esp,4           ; release temporary
        ret                     ; return
endproc ChopDble


        xdefp   ChopLong
defn    ChopLong
        push    eax             ; allocate temporary
        push    eax             ; ...
        fstp    qword ptr [esp] ; chop the internal to double
        fld     qword ptr [esp] ; load it as a double
        fwait                   ; wait for load to complete
        add     esp,8           ; release temporary
        ret                     ; return
endproc ChopLong


        xdefp   SetChop
defn    SetChop
        mov     edx,CW_RC_CHOP  ; set to chop
;;;;;;;;hop     ChngRnd
endproc SetChop


defn    ChngRnd
        push    eax             ; allocate a temporary
        fstcw   [esp]           ; get status word
        fwait                   ; . . .
        pop     eax             ; save current CW
        mov     ebx,eax         ; set new round control
        and     ebx,CW_RC_MASK  ; . . .
        or      ebx,edx         ; . . .
        push    ebx             ; set to specified round control
        fldcw   [esp]           ; . . .
        fwait                   ; wait for load to complete
        pop     edx             ; release temporary
        ret                     ; return (AX contains old CW)
endproc ChngRnd


        xdefp   RestRnd
defn    RestRnd                 ; restore rounding to what it used to be
        push    eax             ; allocate a temporary
        fldcw   word ptr [esp]  ; . . .
        fwait                   ; wait for load to complete
        pop     edx             ; release temporary
        ret                     ; return (AX contains old CW)
endproc RestRnd


        fmodend
        end
