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
; FCCNVT        : type conversion F-Codes
;

include fcdef.inc

        xref    Z_ItoR_
        xref    Z_ItoD_
        xref    Z_ItoE_
        xref    Z_RtoI_
        xref    Z_RtoD_
        xref    Z_RtoE_
        xref    Z_DtoI_
        xref    Z_DtoR_
        xref    Z_DtoE_
        xref    Z_EtoI_
        xref    Z_EtoR_
        xref    Z_EtoD_

        fmodstart fccnvt

        dataseg

        retaddr dw      0
        TmpArea dw      48 dup(0)
        extrn   math_rtn        : word

        enddata


fcode   EC_CNV
        exit_fcode              ; switch to run-time environment
        docall  Z_EtoR_         ; convert value
        jmp     scfin           ; finish it
efcode  EC_CNV


fcode   EQ_CNV
        exit_fcode              ; switch to run-time environment
        docall  Z_EtoD_         ; convert value
        sub     DI,DI           ; push imaginary part of 0
        push    DI              ; ...
        push    DI              ; ...
        push    DI              ; ...
        push    DI              ; ...
        jmp     push8           ; finish it
efcode  EQ_CNV


fcode   DC_CNV
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get value
        pop     CX
        pop     BX
        pop     AX
        docall  Z_DtoR_         ; convert value
scfin:  sub     DI,DI           ; push imaginary part of 0
        push    DI              ; ...
        push    DI              ; ...
push4:  push    DX              ; push real part
        push    AX              ; ...
        enter_fcode             ; switch to F-Code environment
;;;;;;;;hop     II_CNV
efcode  DC_CNV


fcode   II_CNV                  ; nop conversions
dfcode  RR_CNV
dfcode  DD_CNV
dfcode  EE_CNV
dfcode  CC_CNV
dfcode  QQ_CNV
dfcode  XX_CNV
        next                    ; execute next F-Code
efcode  II_CNV


fcode   IC_CNV
        call    DoICCnv
        next
efcode  IC_CNV


fcode   RC_CNV
        call    DoRCCnv
        next
efcode  RC_CNV


fcode   RQ_CNV
        call    DoRQCnv
        next
efcode  RQ_CNV


fcode   IQ_CNV
        call    DoIQCnv
        next
efcode  IQ_CNV


fcode   DQ_CNV
        call    DoDQCnv
        next
efcode  DQ_CNV


fcode   CQ_CNV
        call    DoCQCnv
        next
efcode  CQ_CNV


fcode   IX_CNV
        call    DoIXCnv
        next
efcode  IX_CNV


fcode   RX_CNV
        call    DoRXCnv
        next
efcode  RX_CNV


fcode   DX_CNV
        call    DoDXCnv
        next
efcode  DX_CNV


fcode   EX_CNV
        call    DoEXCnv
        next
efcode  EX_CNV


fcode   CX_CNV
        call    DoCXCnv
        next
efcode  CX_CNV


fcode   QX_CNV
        call    DoQXCnv
        next
efcode  QX_CNV


fcode   CI_CNV
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get real part
        pop     DX
        add     SP,4            ; get rid of imaginary part
        docall  Z_RtoI_
        jmp     push4           ; push real part
efcode  CI_CNV


fcode   CR_CNV
        pop     AX              ; get real part
        pop     DX
        add     SP,4            ; get rid of imaginary part
        push    DX              ; push real part
        push    AX              ; ...
        next                    ; execute next F-Code
efcode  CR_CNV


fcode   CD_CNV
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get real part
        pop     DX
        add     SP,4            ; get rid of imaginary part
        docall  Z_RtoD_         ; convert value
push8:  push    AX              ; push result
        push    BX
        push    CX
        push    DX
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
efcode  CD_CNV


fcode   CE_CNV
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get real part
        pop     DX
        sub     SP,12           ; make more rooom on the stack for the r16
        mov     di, si          ; save fcode ptr
        mov     si, sp          ; load result address
        docall  Z_RtoE_         ; convert value
        mov     si, di          ; restore fcode ptr
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
efcode  CE_CNV


fcode   QE_CNV
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get real part
        pop     CX              ;
        pop     BX              ;
        pop     AX              ;
        sub     SP,8            ; make more rooom on the stack for the r16
        mov     di, si          ; save fcode ptr
        mov     si, sp          ; load result address
        docall  Z_DtoE_         ; convert value
        mov     si, di          ; restore fcode ptr
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
efcode  QE_CNV


fcode   QI_CNV
        mov     DI,offset Z_DtoI_
        mov     BP,seg Z_DtoI_
        hop     Qto4
efcode  QI_CNV


fcode   QR_CNV
        mov     DI,offset Z_DtoR_
        mov     BP,seg Z_DtoR_
;;;;;;;;hop     Qto4
efcode  QR_CNV


defn    Qto4
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get real part
        pop     CX
        pop     BX
        pop     AX
        add     SP,8            ; get rid of imaginary part
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        jmp     push4           ; push result
endproc Qto4


fcode   QD_CNV
        pop     DX              ; get real part
        pop     CX
        pop     BX
        pop     AX
        add     SP,8            ; get rid of imaginary part
        push    AX              ; push result
        push    BX
        push    CX
        push    DX
        next                    ; execute next F-Code
efcode  QD_CNV


fcode   QC_CNV
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get real part
        pop     CX
        pop     BX
        pop     AX
        docall  Z_DtoR_         ; convert value
        mov     DI,DX           ; save result
        mov     BP,AX
        pop     DX              ; get imaginary part
        pop     CX
        pop     BX
        pop     AX
        docall  Z_DtoR_
        push    DX              ; push imaginary part
        push    AX
        push    DI              ; push real part
        push    BP
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  QC_CNV


fcode   XC_CNV
        exit_fcode              ; switch to run-time environment
        docall  Z_EtoR_         ; convert value
        mov     DI,DX           ; save result
        mov     BP,AX
        docall  Z_EtoR_
        push    DX              ; push imaginary part
        push    AX
        push    DI              ; push real part
        push    BP
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  XC_CNV


fcode   XQ_CNV
        exit_fcode              ; switch to run-time environment
        mov     bp, sp
        push    30[bp]          ; create more room on the stack and push on
        push    28[bp]          ; the imag first
        push    26[bp]          ; ...
        push    24[bp]          ; ...
        push    22[bp]          ; ...
        push    20[bp]          ; ...
        push    18[bp]          ; ...
        push    16[bp]          ; ...
        docall  Z_EtoD_         ; convert imag value
        mov     30[bp],AX       ; save result
        mov     28[bp],BX       ; ...
        mov     26[bp],CX       ; ...
        mov     24[bp],DX       ; ...
        docall  Z_EtoD_         ; convert real value
        add     SP, 8           ; clear bottom half of the old imag
        jmp     push8           ; finish it by pushing real part on
efcode  XQ_CNV


fcode   IR_CNV
        mov     DI,offset Z_ItoR_
        mov     BP,seg Z_ItoR_
        hop     cnv4to4
efcode  IR_CNV


fcode   RI_CNV
        mov     DI,offset Z_RtoI_
        mov     BP,seg Z_RtoI_
;;;;;;;;hop     cnv4to4
efcode  RI_CNV


defn    cnv4to4
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get value
        pop     DX              ; ...
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        jmp     push4           ; push result
endproc cnv4to4


fcode   ID_CNV
        mov     DI,offset Z_ItoD_
        mov     BP,seg Z_ItoD_
        hop     cnv4to8
efcode  ID_CNV


fcode   RD_CNV
        mov     DI,offset Z_RtoD_
        mov     BP,seg Z_RtoD_
;;;;;;;;hop     cnv4to8
efcode  RD_CNV


defn    cnv4to8
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get value
        pop     DX              ; ...
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        jmp     push8           ; push result
endproc cnv4to8


fcode   DE_CNV
        pop     DX              ; get value
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        mov     di, si          ; save fcode ptr
        sub     sp, 16          ; make room on stack
        mov     si, sp          ; we want it on the stack
        call    Z_DtoE_
        mov     si, di          ; retore  fcodes
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
efcode  DE_CNV


fcode   IE_CNV
        mov     DI,offset Z_ItoE_
        mov     BP,seg Z_ItoE_
        hop     cnv4to16
efcode  IE_CNV


fcode   RE_CNV
        mov     DI,offset Z_RtoE_
        mov     BP,seg Z_RtoE_
;;;;;;;;hop     cnv4to16
efcode  RE_CNV


defn    cnv4to16
        exit_fcode              ; switch to run-time environment
        pop     AX              ; get value
        pop     DX              ; ...
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        mov     di, si          ; save fcode ptr
        sub     sp, 16          ; make room on stack
        mov     si, sp          ; we want it on the stack
        call    dword ptr SS:math_rtn
        mov     si, di          ; retore  fcodes
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
endproc cnv4to16


fcode   DI_CNV
        mov     DI,offset Z_DtoI_
        mov     BP,seg Z_DtoI_
        hop     cnv8to4
efcode  DI_CNV


fcode   DR_CNV
        mov     DI,offset Z_DtoR_
        mov     BP,seg Z_DtoR_
;;;;;;;;hop     cnv8to4
efcode  DR_CNV


defn    cnv8to4
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get value
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        jmp     push4           ; push result
endproc cnv8to4


fcode   XE_CNV
        call    killR16Imag     ; Kill imaginary
        next                    ; next fcode
efcode  XE_CNV


fcode   XD_CNV
        call    killR16Imag     ; Kill imaginary
;;;;;;  hop     ED_CNV          ; convert extended to int
efcode  XD_CNV


fcode   ED_CNV
        exit_fcode              ; switch to run-time environment
        call    Z_EtoD_
        jmp     push8           ; push result
efcode  ED_CNV


defn    killR16Imag
        push    bp              ; save bp
        mov     bp, sp          ; get sp
        add     sp, 36          ; push up sp
        push    18[BP]          ; push the real part up
        push    16[BP]          ; ...
        push    14[BP]          ; ...
        push    12[BP]          ; ...
        push    10[BP]          ; ...
        push    8[BP]           ; ...
        push    6[BP]           ; ...
        push    4[BP]           ; ...
        push    2[BP]           ; push up the return address
        mov     bp, [bp]        ; restore bp
        ret                     ; return
endproc killR16Imag


fcode   XI_CNV
        call    killR16Imag     ; Kill imaginary
;;;;;;  hop     EI_CNV          ; convert extended to int
efcode  XI_CNV


fcode   EI_CNV
        mov     DI,offset Z_EtoI_
        mov     BP,seg Z_EtoI_
        hop     cnv16to4
efcode  EI_CNV


fcode   XR_CNV
        call    killR16Imag     ; Kill imaginary
;;;;;;  hop     ER_CNV          ; convert extended to int
efcode  XR_CNV


fcode   ER_CNV
        mov     DI,offset Z_EtoR_
        mov     BP,seg Z_EtoR_
;;;;;;;;hop     cnv16to4
efcode  ER_CNV


defn    cnv16to4
        exit_fcode              ; switch to run-time environment
        mov     SS:math_rtn,DI  ; convert value
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        jmp     push4           ; push result
endproc cnv16to4


        xdefp   DoIRCnv
defn    DoIRCnv
        pop     DI              ; get return address
        pop     AX              ; get the integer
        pop     DX              ; . . .
        exit_fcode              ; switch to run-time environment
        call    Z_ItoR_         ; do the conversion
        enter_fcode             ; switch to F-Code environment
psh4:   push    DX              ; push real
        push    AX              ; . . .
        jmp     DI              ; return to caller
endproc DoIRCnv


        xdefp   DoIDCnv
defn    DoIDCnv
        mov     DI,offset Z_ItoD_
        mov     BP,seg Z_ItoD_
;;;;;;;;hop     cnv_0
endproc DoIDCnv


defn    cnv_0
        mov     SS:math_rtn,DI  ; save address of convert routine
        mov     SS:math_rtn+2,BP; ...
        pop     DI              ; get return address
        pop     AX              ; get the integer
        pop     DX              ; . . .
        exit_fcode              ; switch to run-time environment
        call    dword ptr SS:math_rtn
        enter_fcode             ; switch to F-Code environment
psh8:   push    AX              ; push result
        push    BX              ; . . .
        push    CX              ; . . .
        push    DX              ; . . .
        jmp     DI              ; return to caller
endproc cnv_0


        xdefp   DoRICnv
defn    DoRICnv
        mov     BP,SP           ; get addressability to args
        mov     AX,6[BP]        ; get the integer
        mov     DX,8[BP]        ; . . .
        call    Z_ItoR_         ; do the conversion
        mov     8[BP],DX        ; save result
        mov     6[BP],AX        ; . . .
        ret                     ; return
endproc DoRICnv


        xdefp   DoRDCnv
defn    DoRDCnv
        mov     DI,offset Z_RtoD_
        mov     BP,seg Z_RtoD_
        hop     cnv_0
endproc DoRDCnv


        xdefp   DoDICnv
defn    DoDICnv
        mov     DI,offset Z_ItoD_
        mov     BP,seg Z_ItoD_
;;;;;;;;hop     cnv_1
endproc DoDICnv


defn    cnv_1
        mov     SS:math_rtn,DI          ; save address of convert routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     DI                      ; get return address
        pop     DX                      ; get double argument
        pop     CX
        pop     BX
        pop     AX
        sub     SP,4                    ; make room for resulting double
        push    AX                      ; push double argument
        push    BX
        push    CX
        push    DX
        push    DI                      ; restore return address
        mov     DI,SP                   ; point to destination
        xchg    DI,BP
        mov     DX,16[BP]
        mov     AX,14[BP]
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode                     ; switch to F-Code environment
        mov     16[BP],AX               ; save result
        mov     14[BP],BX
        mov     12[BP],CX
        mov     10[BP],DX
        ret
endproc cnv_1


        xdefp   DoDRCnv
defn    DoDRCnv
        mov     DI,offset Z_RtoD_
        mov     BP,seg Z_RtoD_
        hop     cnv_1
endproc DoDRCnv


        xdefp   DoXRCnv
defn    DoXRCnv
        mov     DI,offset Z_RtoE_
        mov     BP,seg Z_RtoE_
        mov     CX, 16
        hop     cnv_9
endproc DoXRCnv


        xdefp   DoERCnv
defn    DoERCnv
        mov     DI,offset Z_RtoE_
        mov     BP,seg Z_RtoE_
        mov     CX, 8
        hop     cnv_9
endproc DoERCnv


        xdefp   DoXICnv
defn    DoXICnv
        mov     DI,offset Z_ItoE_
        mov     BP,seg Z_ItoE_
        mov     CX, 16
        hop     cnv_9
endproc DoXICnv


        xdefp   DoEICnv
defn    DoEICnv
        mov     DI,offset Z_ItoE_
        mov     BP,seg Z_ItoE_
        mov     CX, 8
;;;;;;;;hop     cnv_9
endproc DoEICnv


defn    cnv_9
        mov     SS:math_rtn,DI          ; save address of convert routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     DI                      ; get return address
        dec     CX                      ; avoid the obeewan problem in the loop
        mov     BX, CX                  ; save the counter
poploop:
        mov     BP, CX                  ; set index;
        shl     BP, 1                   ; shift multiply by 2
        pop     SS:TmpArea[BP]          ; get extended argument
        loop    poploop                 ; ...
        pop     AX                      ; get 4 byte number
        pop     DX                      ;
        cmp     BX, 15                  ; check if we have to push an imag
        jne     doxconv                 ; on the stack
        sub     BP, BP                  ; push the imag
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
        push    BP                      ; ...
doxconv:
        sub     SP,16                   ; make room for resulting tbyte
        push    DI                      ; restore return address
        mov     DI,SI                   ; save fcode ptr
        mov     SI, SP                  ; get address
        add     SI, 2                   ; of the result
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode                     ; switch to F-Code environment
        mov     SI, DI                  ; restore fcode ptr
        pop     DI                      ; restore return address
        mov     CX, BX                  ; restore arg 1
pshloop:
        mov     BP, BX                  ; ...
        sub     BP, CX                  ; set index;
        shl     BP, 1                   ; shift multiply by 2
        push    SS:TmpArea[BP]          ; get extended argument
        loop    pshloop                 ; ...
        jmp     DI                      ; ret
endproc cnv_9


        xdefp   DoIECnv
defn    DoIECnv
        mov     DI,offset Z_ItoE_
        mov     BP,seg Z_ItoE_
        mov     CX, 2           ; set 4 byte
        hop     cnv_10
endproc DoIECnv


        xdefp   DoRECnv
defn    DoRECnv
        mov     DI,offset Z_RtoE_
        mov     BP,seg Z_RtoE_
        mov     CX, 2           ; set 4 byte
        hop     cnv_10
endproc DoRECnv


        xdefp   DoDECnv
defn    DoDECnv
        mov     DI,offset Z_DtoE_
        mov     BP,seg Z_DtoE_
        mov     CX, 4           ; set double
;;;;;;;;hop     cnv_10
endproc DoDECnv

; if cx has a value 4 or greater a double is assummed

defn    cnv_10
        mov     SS:math_rtn,DI          ; save address of convert routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     DI                      ; get return address
        cmp     CX, 4                   ; is it double or real
        jge     popdouble               ;
        pop     AX                      ; pop 4 byte
        pop     DX                      ; ...
        jmp     donccon                 ; do the conversion
popdouble:
        pop     DX                      ; pop double
        pop     CX                      ; ...
        pop     BX                      ; ...
        pop     AX                      ; ...
donccon:
        sub     SP, 16                  ; make room on the stack
        push    DI                      ; save return address
        mov     DI, SI                  ; save fcode ptr
        mov     SI, SP                  ; where we want the result to go
        add     SI, 2                   ; ...
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode                     ; switch to F-Code environment
        mov     SI, DI                  ; restore fcode ptr
        pop     DI                      ; get return address
        jmp     DI                      ; adiue
endproc cnv_10


        xdefp   DoEDCnv
defn    DoEDCnv
        pop     SS:retaddr      ; get return address
        pop     SS:TmpArea[0]   ; get all 16 bytes
        pop     SS:TmpArea[2]   ; ...
        pop     SS:TmpArea[4]   ; ...
        pop     SS:TmpArea[6]   ; ...
        pop     SS:TmpArea[8]   ; ...
        pop     SS:TmpArea[10]  ; ...
        pop     SS:TmpArea[12]  ; ...
        pop     SS:TmpArea[14]  ; ...
        call    DoDECnv         ; convert
        push    SS:TmpArea[14]  ; get all 16 bytes
        push    SS:TmpArea[12]  ; ...
        push    SS:TmpArea[10]  ; ...
        push    SS:TmpArea[8]   ; ...
        push    SS:TmpArea[6]   ; ...
        push    SS:TmpArea[4]   ; ...
        push    SS:TmpArea[2]   ; ...
        push    SS:TmpArea[0]   ; ...
        mov     DI, SS:retaddr  ; load for jump
        jmp     DI              ; adiue
endproc DoEDCnv


        xdefp   DoXDCnv
defn    DoXDCnv
        pop     SS:retaddr      ; get return address
        pop     SS:TmpArea[0]   ; get all 32 bytes
        pop     SS:TmpArea[2]   ; ...
        pop     SS:TmpArea[4]   ; ...
        pop     SS:TmpArea[6]   ; ...
        pop     SS:TmpArea[8]   ; ...
        pop     SS:TmpArea[10]  ; ...
        pop     SS:TmpArea[12]  ; ...
        pop     SS:TmpArea[14]  ; ...
        pop     SS:TmpArea[16]  ; ...
        pop     SS:TmpArea[18]  ; ...
        pop     SS:TmpArea[20]  ; ...
        pop     SS:TmpArea[22]  ; ...
        pop     SS:TmpArea[24]  ; ...
        pop     SS:TmpArea[26]  ; ...
        pop     SS:TmpArea[28]  ; ...
        pop     SS:TmpArea[30]  ; ...
        call    DoDXCnv         ; convert
        push    SS:TmpArea[30]  ; get all 32 bytes
        push    SS:TmpArea[28]  ; ...
        push    SS:TmpArea[26]  ; ...
        push    SS:TmpArea[22]  ; ...
        push    SS:TmpArea[20]  ; ...
        push    SS:TmpArea[18]  ; ...
        push    SS:TmpArea[16]  ; ...
        push    SS:TmpArea[14]  ; ...
        push    SS:TmpArea[12]  ; ...
        push    SS:TmpArea[10]  ; ...
        push    SS:TmpArea[8]   ; ...
        push    SS:TmpArea[6]   ; ...
        push    SS:TmpArea[4]   ; ...
        push    SS:TmpArea[2]   ; ...
        push    SS:TmpArea[0]   ; ...
        mov     DI, SS:retaddr  ; load for jump
        jmp     DI              ; adiue
endproc DoXDCnv


        xdefp   DoXECnv
defn    DoXECnv
        pop     DI              ; get return address
        pop     SS:TmpArea[0]   ; get all 48 bytes
        pop     SS:TmpArea[2]   ; ...
        pop     SS:TmpArea[4]   ; ...
        pop     SS:TmpArea[6]   ; ...
        pop     SS:TmpArea[8]   ; ...
        pop     SS:TmpArea[10]  ; ...
        pop     SS:TmpArea[12]  ; ...
        pop     SS:TmpArea[14]  ; ...
        pop     SS:TmpArea[16]  ; ...
        pop     SS:TmpArea[18]  ; ...
        pop     SS:TmpArea[20]  ; ...
        pop     SS:TmpArea[22]  ; ...
        pop     SS:TmpArea[24]  ; ...
        pop     SS:TmpArea[26]  ; ...
        pop     SS:TmpArea[28]  ; ...
        pop     SS:TmpArea[30]  ; ...
        pop     SS:TmpArea[32]  ; ...
        pop     SS:TmpArea[34]  ; ...
        pop     SS:TmpArea[36]  ; ...
        pop     SS:TmpArea[38]  ; ...
        pop     SS:TmpArea[40]  ; ...
        pop     SS:TmpArea[42]  ; ...
        pop     SS:TmpArea[44]  ; ...
        pop     SS:TmpArea[46]  ; ...
        xor     BP, BP          ; add 16 bytes to the stack
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    SS:TmpArea[46]  ; ...
        push    SS:TmpArea[44]  ; ...
        push    SS:TmpArea[42]  ; ...
        push    SS:TmpArea[40]  ; ...
        push    SS:TmpArea[38]  ; ...
        push    SS:TmpArea[36]  ; ...
        push    SS:TmpArea[34]  ; ...
        push    SS:TmpArea[32]  ; ...
        push    SS:TmpArea[30]  ; ...
        push    SS:TmpArea[28]  ; ...
        push    SS:TmpArea[26]  ; ...
        push    SS:TmpArea[24]  ; ...
        push    SS:TmpArea[22]  ; ...
        push    SS:TmpArea[20]  ; ...
        push    SS:TmpArea[18]  ; ...
        push    SS:TmpArea[16]  ; ...
        push    SS:TmpArea[14]  ; ...
        push    SS:TmpArea[12]  ; ...
        push    SS:TmpArea[10]  ; ...
        push    SS:TmpArea[8]   ; ...
        push    SS:TmpArea[6]   ; ...
        push    SS:TmpArea[4]   ; ...
        push    SS:TmpArea[2]   ; ...
        push    SS:TmpArea[0]   ; ...
        jmp     DI
endproc DoXECnv

        xdefp   DoCECnv
defn    DoCECnv
        pop     SS:retaddr      ; get return address
        call    DoCXCnv         ; conver to XE
        push    SS:retaddr      ; load for jump
        jmp     DoXECnv         ; convet to XX
endproc DoCECnv


        xdefp   DoQECnv
defn    DoQECnv
        pop     SS:retaddr      ; get return address
        call    DoQXCnv         ; conver to XE
        push    SS:retaddr      ; load for jump
        jmp     DoXECnv         ; convet to XX
endproc DoQECnv


        xdefp   DoECCnv
defn    DoECCnv
        pop     SS:retaddr      ; get return address
        call    DoEXCnv         ; conver to XC
        push    SS:retaddr      ; load for jump
        jmp     DoXCCnv         ; convet to XX
endproc DoECCnv


        xdefp   DoEQCnv
defn    DoEQCnv
        pop     SS:retaddr      ; get return address
        call    DoEXCnv         ; conver to XD
        push    SS:retaddr      ; load for jump
        jmp     DoXDCnv         ; convet to XX
endproc DoEQCnv


        xdefp   DoXQCnv
defn    DoXQCnv
        pop     SS:retaddr      ; get return address
        pop     SS:TmpArea[0]   ; get all 32 bytes
        pop     SS:TmpArea[2]   ; ...
        pop     SS:TmpArea[4]   ; ...
        pop     SS:TmpArea[6]   ; ...
        pop     SS:TmpArea[8]   ; ...
        pop     SS:TmpArea[10]  ; ...
        pop     SS:TmpArea[12]  ; ...
        pop     SS:TmpArea[14]  ; ...
        pop     SS:TmpArea[16]  ; ...
        pop     SS:TmpArea[18]  ; ...
        pop     SS:TmpArea[20]  ; ...
        pop     SS:TmpArea[22]  ; ...
        pop     SS:TmpArea[24]  ; ...
        pop     SS:TmpArea[26]  ; ...
        pop     SS:TmpArea[28]  ; ...
        pop     SS:TmpArea[30]  ; ...
        call    DoQXCnv         ; conver to X
        push    SS:TmpArea[30]  ; get all 32 bytes
        push    SS:TmpArea[28]  ; ...
        push    SS:TmpArea[26]  ; ...
        push    SS:TmpArea[24]  ; ...
        push    SS:TmpArea[22]  ; ...
        push    SS:TmpArea[20]  ; ...
        push    SS:TmpArea[18]  ; ...
        push    SS:TmpArea[16]  ; ...
        push    SS:TmpArea[14]  ; ...
        push    SS:TmpArea[12]  ; ...
        push    SS:TmpArea[10]  ; ...
        push    SS:TmpArea[8]   ; ...
        push    SS:TmpArea[6]   ; ...
        push    SS:TmpArea[4]   ; ...
        push    SS:TmpArea[2]   ; ...
        push    SS:TmpArea[0]   ; ...
        mov     DI,SS:retaddr   ; load for jump
        jmp     DI              ; convet to XX
endproc DoXQCnv


        xdefp   DoXCCnv
defn    DoXCCnv
        pop     SS:retaddr      ; get return address
        pop     SS:TmpArea[0]   ; get all 32 bytes
        pop     SS:TmpArea[2]   ; ...
        pop     SS:TmpArea[4]   ; ...
        pop     SS:TmpArea[6]   ; ...
        pop     SS:TmpArea[8]   ; ...
        pop     SS:TmpArea[10]  ; ...
        pop     SS:TmpArea[12]  ; ...
        pop     SS:TmpArea[14]  ; ...
        pop     SS:TmpArea[16]  ; ...
        pop     SS:TmpArea[18]  ; ...
        pop     SS:TmpArea[20]  ; ...
        pop     SS:TmpArea[22]  ; ...
        pop     SS:TmpArea[24]  ; ...
        pop     SS:TmpArea[26]  ; ...
        pop     SS:TmpArea[28]  ; ...
        pop     SS:TmpArea[30]  ; ...
        call    DoCXCnv         ; conver to X
        push    SS:TmpArea[30]  ; get all 32 bytes
        push    SS:TmpArea[28]  ; ...
        push    SS:TmpArea[26]  ; ...
        push    SS:TmpArea[24]  ; ...
        push    SS:TmpArea[22]  ; ...
        push    SS:TmpArea[20]  ; ...
        push    SS:TmpArea[18]  ; ...
        push    SS:TmpArea[16]  ; ...
        push    SS:TmpArea[14]  ; ...
        push    SS:TmpArea[12]  ; ...
        push    SS:TmpArea[10]  ; ...
        push    SS:TmpArea[8]   ; ...
        push    SS:TmpArea[6]   ; ...
        push    SS:TmpArea[4]   ; ...
        push    SS:TmpArea[2]   ; ...
        push    SS:TmpArea[0]   ; ...
        mov     DI,SS:retaddr   ; load for jump
        jmp     DI              ; convet to XX
endproc DoXCCnv


        xdefp   DoICCnv
defn    DoICCnv
        pop     DI              ; get return address
        pop     AX              ; get value
        pop     DX              ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_ItoR_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        sub     BP,BP           ; push imaginary part of 0
        push    BP              ; ...
        push    BP              ; ...
        jmp     psh4            ; push real part
endproc DoICCnv


        xdefp   DoIQCnv
defn    DoIQCnv
        mov     DI,offset Z_ItoD_
        mov     BP,seg Z_ItoD_
;;;;;;;;hop     cnv_2
endproc DoIQCnv


defn    cnv_2
        mov     SS:math_rtn,DI  ; save address of convert routine
        mov     SS:math_rtn+2,BP; ...
        pop     DI              ; get return address
        pop     AX              ; get value
        pop     DX              ; ...
        exit_fcode              ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode             ; switch to F-Code environment
cnv_3:  sub     BP,BP           ; push imaginary part of 0
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        jmp     psh8            ; push real part
endproc cnv_2


        xdefp   DoEXCnv
defn    DoEXCnv
        pop     DI              ; get return address
        mov     BP, SP          ; copy real on stack
        push    14[BP]          ; ...
        push    12[BP]          ; ...
        push    10[BP]          ; ...
        push    8[BP]           ; ...
        push    6[BP]           ; ...
        push    4[BP]           ; ...
        push    2[BP]           ; ...
        push    [BP]            ; ...
        mov     word ptr 14[BP], 0      ; make a new imag
        mov     word ptr 12[BP], 0      ; ...
        mov     word ptr 10[BP], 0      ; ...
        mov     word ptr  8[BP], 0      ; ...
        mov     word ptr  6[BP], 0      ; ...
        mov     word ptr  4[BP], 0      ; ...
        mov     word ptr  2[BP], 0      ; ...
        mov     word ptr   [BP], 0      ; ...
        jmp     di
endproc DoEXCnv


        xdefp   DoDXCnv
defn    DoDXCnv
        pop     DI              ; get return address
        pop     DX              ; get value
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        xor     BP, BP          ; push imag on stack
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        sub     sp, 16          ; create room for result
        mov     bp, si          ; save fcode ptr
        mov     si, sp          ; point to result address
        exit_fcode              ; switch to run-time environment
        call    Z_DtoE_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     si, bp          ; restore
        jmp     di
endproc DoDXCnv


        xdefp   DoRXCnv
defn    DoRXCnv
        mov     DI,offset Z_RtoE_
        mov     BP,seg Z_RtoE_
        hop     cnv_4
endproc DoRXCnv


        xdefp   DoIXCnv
defn    DoIXCnv
        mov     DI,offset Z_ItoE_
        mov     BP,seg Z_ItoE_
;;;;;;;;hop     cnv_4
endproc DoIXCnv


defn    cnv_4
        mov     SS:math_rtn,DI  ; save address of convert routine
        mov     SS:math_rtn+2,BP; ...
        pop     DI              ; get return address
        pop     AX              ; get value
        pop     DX              ; ...
        xor     CX, CX          ; push imag on stack
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        push    CX              ; ...
        sub     sp, 16          ; create room for result
        mov     cx, si          ; save fcode ptr
        mov     si, sp          ; point to result address
        exit_fcode              ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     si, cx          ; restore
        jmp     di
endproc cnv_4


        xdefp   DoRCCnv
defn    DoRCCnv
        pop     DI              ; save return address
        pop     AX              ; get r4
        pop     DX              ; ...
        sub     BP,BP           ; push imaginary part of 0
        push    BP              ; ...
        push    BP              ; ...
        jmp     psh4            ; push real part
endproc DoRCCnv


        xdefp   DoRQCnv
defn    DoRQCnv
        mov     DI,offset Z_RtoD_
        mov     BP,seg Z_RtoD_
        jmp     cnv_2
endproc DoRQCnv


        xdefp   DoDCCnv
defn    DoDCCnv
        pop     DI              ; get return address
        pop     DX              ; get value
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        sub     SP,8            ; make room for complex*16 argument
        sub     BP,BP           ; push imaginary part of 0
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    BP              ; ...
        push    AX              ; push real part
        push    BX              ; ...
        push    CX              ; ...
        push    DX              ; ...
        mov     BP,SP           ; get addressability to argument
        exit_fcode              ; enter run-time environment
        mov     AX,24[BP]       ; get real part
        mov     DX,26[BP]       ; ...
        docall  Z_RtoD_         ; do conversion
        mov     16[BP],DX       ; save result
        mov     18[BP],CX       ; ...
        mov     20[BP],BX       ; ...
        mov     22[BP],AX       ; ...
        mov     AX,28[BP]       ; get imaginary part
        mov     DX,30[BP]       ; ...
        docall  Z_RtoD_         ; do conversion
        mov     24[BP],DX       ; save result
        mov     26[BP],CX       ; ...
        mov     28[BP],BX       ; ...
        mov     30[BP],AX       ; ...
        enter_fcode             ; enter fcode environment
        jmp     DI              ; return to caller
endproc DoDCCnv


        xdefp   DoDQCnv
defn    DoDQCnv
        pop     DI              ; get return address
        pop     DX              ; get value
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        jmp     cnv_3
endproc DoDQCnv


        xdefp   DoCICnv
defn    DoCICnv
        pop     DI              ; get return address
        pop     AX              ; get real part
        pop     DX              ; ...
        pop     BX              ; get imaginary part
        pop     CX              ; ...
        sub     SP,4            ; make room for complex*8 argument
        push    CX              ; push imaginary part
        push    BX              ; ...
        push    DX              ; push real part
        push    AX              ; ...
        mov     BP,SP           ; get addressability to argument
        mov     AX,12[BP]       ; get value
        mov     DX,14[BP]       ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_ItoR_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     10[BP],DX       ; save real part
        mov     8[BP],AX       ; ...
        sub     AX,AX           ; save imaginary part of 0
        mov     12[BP],AX       ; ...
        mov     14[BP],AX       ; ...
        jmp     DI              ; return to caller
endproc DoCICnv


        xdefp   DoCRCnv
defn    DoCRCnv
        pop     DI              ; get return address
        pop     AX              ; get real part
        pop     DX              ; ...
        pop     BX              ; get imaginary part
        pop     CX              ; ...
        sub     SP,4            ; make room for complex*8
        mov     BP,SP           ; point to destination
        push    CX              ; push imaginary part
        push    BX              ; ...
        push    DX              ; push real part
        push    AX              ; ...
        mov     AX,4[BP]        ; get real part
        mov     DX,6[BP]        ; ...
        mov     [BP],AX         ; save real part
        mov     2[BP],DX        ; ...
        sub     CX,CX           ; save imaginary part of 0
        mov     4[BP],CX        ; ...
        mov     6[BP],CX        ; ...
        jmp     DI              ; return to caller
endproc DoCRCnv


        xdefp   DoCDCnv
defn    DoCDCnv
        pop     DI              ; get return address
        pop     AX              ; get real part
        pop     DX              ; ...
        sub     SP,20           ; make room for results
        mov     BP,SP           ; get addressability to arguments
        exit_fcode              ; switch to run-time environment
        docall  Z_RtoD_         ; do conversion
        mov     6[BP],AX        ; save real part
        mov     4[BP],BX        ; ...
        mov     2[BP],CX        ; ...
        mov     [BP],DX         ; ...
        mov     AX,20[BP]       ; get imaginary part
        mov     DX,22[BP]       ; ...
        docall  Z_RtoD_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     14[BP],AX       ; save imaginary part
        mov     12[BP],BX       ; ...
        mov     10[BP],CX       ; ...
        mov     8[BP],DX        ; ...
        mov     DX,24[BP]       ; get real part
        mov     CX,26[BP]       ; ...
        mov     BX,28[BP]       ; ...
        mov     AX,30[BP]       ; ...
        mov     22[BP],AX       ; save real part
        mov     20[BP],BX       ; ...
        mov     18[BP],CX       ; ...
        mov     16[BP],DX       ; ...
        sub     AX,AX           ; save imaginary part of 0
        mov     30[BP],AX       ; save real part
        mov     28[BP],AX       ; ...
        mov     26[BP],AX       ; ...
        mov     24[BP],AX       ; ...
        jmp     DI              ; return to caller
endproc DoCDCnv


        xdefp   DoCQCnv
defn    DoCQCnv
        pop     DI              ; get return address
        pop     AX              ; get real part
        pop     DX              ; ...
        sub     SP,12           ; make room for result
        mov     BP,SP           ; get addressability to arguments
        exit_fcode              ; switch to run-time environment
        call    Z_RtoD_         ; convert real part
        mov     6[BP],AX        ; save real part
        mov     4[BP],BX        ; ...
        mov     2[BP],CX        ; ...
        mov     [BP],DX         ; ...
        mov     AX,12[BP]       ; get imaginary part
        mov     DX,14[BP]       ; ...
        call    Z_RtoD_         ; convert imaginary part
        enter_fcode             ; switch to F-Code environment
        mov     14[BP],AX       ; save imaginary part
        mov     12[BP],BX       ; ...
        mov     10[BP],CX       ; ...
        mov     8[BP],DX        ; ...
        jmp     DI              ; return to caller
endproc DoCQCnv


        xdefp   DoCXCnv
defn    DoCXCnv
        pop     DI              ; get return address
        pop     AX              ; get real part
        pop     DX              ; ...
        pop     CX              ; get imag part
        pop     DX              ; ...
        sub     SP, 32          ; make room for result
        mov     BP, SI          ; save fcode ptr
        mov     SI, SP          ; this is where we want the real placed
        exit_fcode              ; switch to run-time environment
        call    Z_RtoE_         ; convert real part
        mov     AX, CX          ; get real part
        mov     DX, BX          ; imag part
        mov     si, sp          ; where do we want the imag
        add     si, 16          ; above the real
        call    Z_RtoE_         ; convert imaginary part
        enter_fcode             ; switch to F-Code environment
        jmp     DI              ; return to caller
endproc DoCXCnv


        xdefp   DoQXCnv
defn    DoQXCnv
        pop     DI              ; get return address
        pop     DX              ; get real part
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        sub     SP,24           ; make room for result
        mov     BP, SI          ; save fcode ptr
        mov     SI, SP          ; this is where we want the real placed
        exit_fcode              ; switch to run-time environment
        call    Z_RtoE_         ; convert real part
        mov     si, bp          ; juggle si
        mov     bp, sp          ; get sp;
        mov     AX, 30[bp]      ; get imag part
        mov     BX, 28[bp]      ; ...
        mov     CX, 26[bp]      ; ...
        mov     DX, 24[bp]      ; ...
        xchg    BP, SI          ; save fcode ptr
        add     si, 16          ; above the real
        call    Z_RtoE_         ; convert imaginary part
        enter_fcode             ; switch to F-Code environment
        mov     si, bp          ; restore fcode ptr
        jmp     DI              ; return to caller
endproc DoQXCnv


        xdefp   DoQICnv
defn    DoQICnv
        mov     DI,offset Z_ItoD_
        mov     BP,seg Z_ItoD_
        hop     cnv_5
endproc DoQICnv


        xdefp   DoQRCnv
defn    DoQRCnv
        mov     DI,offset Z_RtoD_
        mov     BP,seg Z_RtoD_
;;;;;;;;hop     cnv_5
endproc DoQRCnv


defn    cnv_5
        mov     SS:math_rtn,DI  ; save address of convert routine
        mov     SS:math_rtn+2,BP; ...
        sub     SP,12           ; allocate extra room for result
        mov     BP,SP           ; point to destination
        mov     CX,8+1          ; transfer complex*16 + return address
cnv_6:                          ; loop
        mov     AX,12[BP]       ; - get a word
        mov     [BP],AX         ; - transfer word
        add     BP,2            ; - point to next word
        loop    cnv_6           ; until done
        mov     AX,12[BP]       ; get real part
        mov     DX,14[BP]       ; ...
        exit_fcode              ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     [BP],DX         ; save real part
        mov     2[BP],CX        ; ...
        mov     4[BP],BX        ; ...
        mov     6[BP],AX        ; ...
        sub     AX,AX           ; save imaginary part of 0
        mov     8[BP],AX        ; ...
        mov     10[BP],AX       ; ...
        mov     12[BP],AX       ; ...
        mov     14[BP],AX       ; ...
        ret
endproc cnv_5


        xdefp   DoQCCnv
defn    DoQCCnv
        sub     SP,8            ; allocate extra room for result
        mov     BP,SP           ; point to destination
        mov     CX,8+1          ; transfer complex*16 + return address
cnv_7:                          ; loop
        mov     AX,8[BP]        ; - get a word
        mov     [BP],AX         ; - transfer word
        add     BP,2            ; - point to next word
        loop    cnv_7           ; until done
        mov     AX,8[BP]        ; get real part
        mov     DX,10[BP]       ; ...
        exit_fcode              ; switch to run-time environment
        call    Z_RtoD_         ; do conversion
        mov     [BP],DX         ; save real part
        mov     2[BP],CX        ; ...
        mov     4[BP],BX        ; ...
        mov     6[BP],AX        ; ...
        mov     AX,12[BP]       ; get imaginary part
        mov     DX,14[BP]       ; ...
        call    Z_RtoD_         ; do conversion
        mov     8[BP],DX        ; save imaginary part
        mov     10[BP],CX       ; ...
        mov     12[BP],BX       ; ...
        mov     14[BP],AX       ; ...
        enter_fcode             ; switch to F-Code environment
        ret
endproc DoQCCnv


        xdefp   DoQDCnv
defn    DoQDCnv
        sub     SP,8            ; allocate extra room for result
        mov     BP,SP           ; point to destination
        mov     CX,8+4+1        ; transfer complex*16 + real*8 + return address
cnv_8:                          ; loop
        mov     AX,8[BP]        ; - get a word
        mov     [BP],AX         ; - transfer word
        add     BP,2            ; - point to next word
        loop    cnv_8           ; until done
        sub     AX,AX           ; save imaginary part of 0
        mov     [BP],AX         ; ...
        mov     2[BP],AX        ; ...
        mov     4[BP],AX        ; ...
        mov     6[BP],AX        ; ...
        ret
endproc DoQDCnv

        fmodend
        end
