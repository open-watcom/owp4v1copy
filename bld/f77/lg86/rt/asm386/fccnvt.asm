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
        xref    FC@EE_FLIP

        fmodstart fccnvt

        tb_result       dd      ?,?,?,?,?,?,?,?
        tb_keep         dd      ?,?,?,?,?,?,?,?
        db_keep         dd      ?,?,?,?,?,?,?,?
        SaveESI         dd      0
        SaveRet         dd      0
        SaveOldRet      dd      0

fcode   DC_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        pop     edx             ; ...
        docall  Z_DtoR_         ; convert value
        push    0               ; push imaginary part of 0
push4:
        push    eax             ; push real part
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
        next
efcode  II_CNV


fcode   CI_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        pop     edx             ; remove imaginary part
        push    offset32 push4  ; return address
        dojmp   Z_RtoI_         ; do conversion and return to push4
efcode  CI_CNV


fcode   CR_CNV
if      0
        ; This code shows a bug in the i486.  4[esp] is calculated using
        ; the incremented value of esp.  i.e.,
        ;
        ;   stack before (addr: value):
        ;       1000000: 33333333       <--esp
        ;       1000004: 22222222
        ;       1000008: 11111111
        ;
        ;   stack after:
        ;       1000000: ????????
        ;       1000004: 22222222       <--esp
        ;       1000008: 33333333
        ;
        pop     4[esp]          ; overwrite imaginary part with real part
        next
else
        pop     eax             ; get real part
        mov     [esp],eax       ; overwrite imag part with real part
        next
endif
efcode  CR_CNV


fcode   CD_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        docall  Z_RtoD_         ; convert value
        mov     [esp],edx       ; push result
        hop     push4           ; ...
efcode  CD_CNV


fcode   RD_CNV
        mov     edi,offset32 Z_RtoD_
;;;;;;;;hop     cnv4to8
efcode  RD_CNV


defn    cnv4to8
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        call    edi             ; convert value
        push    edx             ; push result
        hop     push4           ; ...
endproc cnv4to8


fcode   CE_CNV
        pop     eax             ; get real
        add     esp, 4          ; clear imag
        push    eax             ; back up on stack
;;;;;;;;hop     RE_CNV
efcode  CE_CNV


fcode   RE_CNV
        mov     edi,offset32 Z_RtoE_
;;;;;;;;hop     cnv4to16
efcode  RE_CNV


defn    cnv4to16
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, offset32 tb_result ; return address
        call    edi             ; convert value
        mov     esi, SaveESI    ; restor efcode ptr
push16:
        push    dword ptr tb_result[12] ; push on stack
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        enter_fcode             ; switch to F-Code environment
        next
endproc cnv4to16


fcode   QE_CNV
        pop     eax             ; get real
        pop     edx             ; ...
        add     esp, 8          ; clear imag
        push    edx             ; back up on stack
        push    eax             ; ...
;;;;;;;;hop     DE_CNV
efcode  QE_CNV


fcode   DE_CNV
        mov     edi,offset32 Z_DtoE_
;;;;;;;;hop     cnv8to16
efcode  DE_CNV


defn    cnv8to16
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        pop     edx             ; ...
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, offset32 tb_result ; return address
        call    edi             ; convert value
        mov     esi, SaveESI    ; restor efcode ptr
        hop     push16          ; ...
endproc cnv8to16


fcode   QI_CNV
        mov     edi,offset32 Z_DtoI_
        hop     Qto4
efcode  QI_CNV


fcode   QR_CNV
        mov     edi,offset32 Z_DtoR_
;;;;;;;;hop     Qto4
efcode  QR_CNV


defn    Qto4
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        pop     edx             ; ...
        pop     ebx             ; remove imaginary part
        pop     ebx             ; ...
        push    offset32 push4  ; return address
        jmp     edi             ; do conversion (and jmp to push4)
endproc Qto4


fcode   QD_CNV
if 0
        ; see note in CR_CNV
        pop     8[esp]          ; overwrite imag with real part
        pop     8[esp]          ; ...
        next
else
        pop     eax             ; get real part
        pop     edx             ; ...
        mov     [esp],eax       ; overwrite imaginary part with real part
        mov     4[esp],edx      ; ...
        next
endif
efcode  QD_CNV


fcode   QC_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        pop     edx             ; ...
        docall  Z_DtoR_         ; convert value
        mov     edi,eax         ; save result
        pop     eax             ; get imaginary part
        pop     edx             ; ...
        docall  Z_DtoR_
        push    eax             ; push imaginary part
        push    edi             ; push real part
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  QC_CNV


fcode   QX_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        pop     edx             ; ...
        pop     ecx             ; get imaginary part
        pop     ebx             ; ...
        sub     esp, 32         ; create necessary stack space
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, esp        ; load address
        add     esi, 8          ; ...
        docall  Z_DtoE_         ; convert value
        mov     eax, ecx        ; get imaginary part
        mov     edx, ebx        ; ...
        mov     esi, esp        ; load address
        add     esi, 16         ; ...
        docall  Z_DtoE_
        mov     esi, SaveESI    ; restore fcode pointer
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  QX_CNV


fcode   CX_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get real part
        pop     ebx             ; get imaginary part
        sub     esp, 32         ; create necessary stack space
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, esp        ; load address
        docall  Z_RtoE_         ; convert value
        mov     eax, ebx        ; get imaginary part
        mov     esi, esp        ; load address
        add     esi, 16         ; ...
        docall  Z_RtoE_
        mov     esi, SaveESI    ; restore fcode pointer
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  CX_CNV


fcode   XC_CNV
        exit_fcode              ; switch to run-time environment
        docall  Z_EtoR_         ; convert value
        mov     edx, eax        ; save real part
        docall  Z_EtoR_
        push    eax             ; push imag value
        push    edx             ; push real value
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  XC_CNV


fcode   XQ_CNV
        exit_fcode              ; switch to run-time environment
        docall  Z_EtoR_         ; convert value
        mov     ebx, eax        ; save real part
        mov     ecx, edx        ; ...
        docall  Z_EtoR_
        push    ecx             ; push imag value
        push    ebx             ; ...
        push    edx             ; push real value
        push    eax             ; ...
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next FCode
efcode  XQ_CNV


fcode   IR_CNV
        mov     edi,offset32 Z_ItoR_
        hop     cnv4to4
efcode  IR_CNV


fcode   RI_CNV
        mov     edi,offset32 Z_RtoI_
;;;;;;;;hop     cnv4to4
efcode  RI_CNV


defn    cnv4to4
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        push    offset32 push4  ; return address
        jmp     edi             ; convert value
endproc cnv4to4


fcode   ID_CNV
        mov     edi,offset32 Z_ItoD_
        jmp     cnv4to8
efcode  ID_CNV


fcode   IE_CNV
        mov     edi,offset32 Z_ItoE_
        jmp     cnv4to16
efcode  IE_CNV


fcode   DI_CNV
        mov     edi,offset32 Z_DtoI_
        hop     cnv8to4
efcode  DI_CNV


fcode   DR_CNV
        mov     edi,offset32 Z_DtoR_
;;;;;;;;hop     cnv8to4
efcode  DR_CNV


defn    cnv8to4
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get value
        pop     edx             ; ...
        push    offset32 push4  ; return address
        jmp     edi             ; convert value
endproc cnv8to4


fcode   EI_CNV
        mov     edi,offset32 Z_EtoI_
        hop     cnv16to4
efcode  EI_CNV


fcode   EC_CNV
        exit_fcode              ; enter runtime environment
        pop     eax             ; get extended
        pop     ebx             ; ...
        pop     ecx             ; ...
        pop     edx             ; ...
        push    0               ; push 0 on stack
        push    edx             ; replace extended
        push    ecx             ; ...
        push    ebx             ; ...
        push    eax             ; ...
        enter_fcode             ; enter fcode
;;;;;;;;hop     ER_CNV          ; convert it to REAL
efcode  EC_CNV


fcode   ER_CNV
        mov     edi,offset32 Z_EtoR_
;;;;;;;;hop     cnv16to4
efcode  ER_CNV


defn    cnv16to4
        exit_fcode              ; switch to run-time environment
        push    offset32 push4  ; return address
        jmp     edi             ; convert value
endproc cnv16to4


fcode   EQ_CNV
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get extended
        pop     ebx             ; ...
        pop     ecx             ; ...
        pop     edx             ; ...
        push    0               ; push 0 imag on stack
        push    0               ; ...
        push    edx             ; replace extended
        push    ecx             ; ...
        push    ebx             ; ...
        push    eax             ; ...
        enter_fcode
;;;;;;;;hop     ED_CNV          ; convert it to REAL
efcode  EQ_CNV


fcode   ED_CNV
        mov     edi,offset32 Z_EtoD_
;;;;;;;;hop     cnv16to8
efcode  ED_CNV


defn    cnv16to8
        exit_fcode              ; switch to run-time environment
        push    offset32 push8  ; return address
        jmp     edi             ; convert value
endproc cnv16to8


        xdefp   DoIRCnv
defn    DoIRCnv
        pop     edi             ; get return address
        pop     eax             ; get the integer
        exit_fcode              ; switch to run-time environment
        call    Z_ItoR_         ; do the conversion
        enter_fcode             ; switch to F-Code environment
psh4:   push    eax             ; push real
        jmp     edi             ; return to caller
endproc DoIRCnv


fcode   XE_CNV
        call    XtoNonCom
        next
efcode  XE_CNV


fcode   XD_CNV
        exit_fcode              ; switch to run-time environment
        call    XtoNonCom
        call    Z_EtoD_
push8:
        push    edx     ; push an 8 word value
        push    eax     ; ...
        enter_fcode
        next
efcode  XD_CNV


fcode   XR_CNV
        mov     edi,offset32 Z_EtoR_
        hop     Xto4
efcode  XR_CNV


fcode   XI_CNV
        mov     edi,offset32 Z_EtoI_
Xto4:
        exit_fcode              ; switch to run-time environment
        call    XtoNonCom
        call    edi             ; do conversion
        jmp     push4
efcode  XI_CNV


defn    XtoNonCom
        pop     edi             ; get adddress
        pop     eax             ; get real part
        pop     ebx             ; ...
        pop     ecx             ; ...
        pop     edx             ; ...
        add     esp, 16         ; get rid of imag
        push    edx             ; push E back on stack
        push    ecx             ; ...
        push    ebx             ; ...
        push    eax             ; ...
        jmp     edi
endproc XtoNonCom


fcode   EX_CNV
        mov     eax, 0          ; create a 0 tbyte on stack
        push    eax             ; ...
        push    eax             ; ...
        push    eax             ; ...
        push    eax             ; ...
        jmp     FC@EE_FLIP      ; flip the 2 numbers and voila
efcode  EX_CNV


fcode   DX_CNV
        mov     edi,offset32 Z_DtoE_
        pop     eax             ; pop the dble
        pop     edx             ; ...
        hop     NonComtoX
efcode  DX_CNV


fcode   IX_CNV
        mov     edi,offset32 Z_ItoE_
        hop     Do4toX
efcode  IX_CNV


fcode   RX_CNV
        mov     edi,offset32 Z_RtoE_
Do4toX:
        pop     eax             ; pop the real
;;;;;;  hop     NonComtoX
efcode  RX_CNV


defn    NonComtoX
        exit_fcode              ; switch to run-time environment
        push    0               ; push 0 imag on the stack
        push    0               ; ...
        push    0               ; ...
        push    0               ; ...
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, 0          ; push last word
        push    si              ; ...
        mov     esi, offset32 tb_result ; set result address
        push    offset32 push16 ;
        jmp     edi             ; do conversion
endproc NonComtoX


        xdefp   DoIDCnv
defn    DoIDCnv
        mov     ebx,offset32 Z_ItoD_
;;;;;   hop     cnv_0
endproc DoIDCnv


defn    cnv_0
        pop     edi             ; get return address
        pop     eax             ; get the integer
        exit_fcode              ; switch to run-time environment
        call    ebx             ; do the conversion
        enter_fcode             ; switch to F-Code environment
psh8:   push    edx             ; push result
        push    eax             ; ...
        jmp     edi             ; return to caller
endproc cnv_0


        xdefp   DoRICnv
defn    DoRICnv
        mov     eax,8[esp]      ; get the integer
        call    Z_ItoR_         ; do the conversion
        mov     8[esp],eax      ; save result
        ret                     ; return
endproc DoRICnv


        xdefp   DoRDCnv
defn    DoRDCnv
        mov     ebx,offset32 Z_RtoD_
        hop     cnv_0
endproc DoRDCnv


        xdefp   DoDICnv
defn    DoDICnv
        mov     ebx,offset32 Z_ItoD_
;;;;;;;;hop     cnv_1
endproc DoDICnv


defn    cnv_1
        pop     edi                     ; get return address
        pop     eax                     ; get double argument
        push    [esp]                   ; make room for resulting double
        push    eax                     ; ...
        mov     eax,12[esp]             ; get integer
        exit_fcode                      ; switch to run-time environment
        call    ebx                     ; do conversion
        enter_fcode                     ; switch to F-Code environment
        mov     8[esp],eax              ; save result
        mov     12[esp],edx             ; ...
        jmp     edi                     ; return to caller
endproc cnv_1


        xdefp   DoDRCnv
defn    DoDRCnv
        mov     ebx,offset32 Z_RtoD_
        hop     cnv_1
endproc DoDRCnv


        xdefp   DoIECnv
defn    DoIECnv
        mov     ebx,offset32 Z_ItoE_
        hop     Mk4216                  ; do conversion
endproc DoIECnv


        xdefp   DoRECnv
defn    DoRECnv
        mov     ebx,offset32 Z_RtoE_
Mk4216:
        pop     edi                     ; get return address
        pop     eax                     ; get argument
        hop     cnv_6
endproc DoRECnv


        xdefp   DoRXCnv
defn    DoRXCnv
        mov     ebx,offset32 Z_RtoE_
        hop     Mk4232                  ; do conversion
endproc DoRXCnv


        xdefp   DoIXCnv
defn    DoIXCnv
        mov     ebx,offset32 Z_RtoE_
Mk4232:
        pop     edi                     ; get return address
        pop     eax                     ; get argument
Mk32Imag:
        mov     ebx, 0                  ; make an imag number
        push    ebx                     ; ...
        push    ebx                     ; ...
        push    ebx                     ; ...
        push    ebx                     ; ...
        hop     cnv_6
endproc DoIXCnv


        xdefp   DoDECnv
defn    DoDECnv
        mov     ebx,offset32 Z_DtoE_
        pop     edi                     ; get return address
        pop     eax                     ; get double argument
        pop     edx                     ; ...
        hop     cnv_6
endproc DoDECnv


        xdefp   DoDXCnv
defn    DoDXCnv
        mov     ebx,offset32 Z_DtoE_
        pop     edi                     ; get return address
        pop     eax                     ; get double argument
        pop     edx                     ; ...
        hop     Mk32Imag
endproc DoDXCnv


defn    cnv_6
        mov     SaveESI, esi            ; save fcode ptr
        mov     esi, offset32 tb_result ; load result address
        exit_fcode                      ; switch to run-time environment
        call    ebx                     ; do conversion
        enter_fcode                     ; switch to F-Code environment
        mov     esi, SaveESI            ; restore fcode ptr
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        jmp     edi                     ; return to caller
endproc cnv_6


fcode   IC_CNV
        push    offset32 II_CNV ; return address
;;;;;;;;hop     DoICCnv
efcode  IC_CNV


        xdefp   DoICCnv
defn    DoICCnv
        pop     edi             ; get return address
        pop     eax             ; get value
        exit_fcode              ; switch to run-time environment
        docall  Z_ItoR_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        push    0               ; push imaginary part of 0
        push    eax             ; push real part
        jmp     edi             ; return to caller
endproc DoICCnv


fcode   IQ_CNV
        push    offset32 II_CNV ; return address
;;;;;;;;hop     DoIQCnv
efcode  IQ_CNV


        xdefp   DoIQCnv
defn    DoIQCnv
        mov     ebx,offset32 Z_ItoD_
;;;;;;;;hop     cnv_2
endproc DoIQCnv


defn    cnv_2
        pop     edi             ; get return address
        pop     eax             ; get value
        exit_fcode              ; switch to run-time environment
        call    ebx             ; do conversion
        enter_fcode             ; switch to F-Code environment
cnv_3:  push    0               ; push imaginary part of 0
        push    0               ; ...
        push    edx             ; push real part
        push    eax             ; ...
        jmp     edi
endproc cnv_2


fcode   RC_CNV
        push    [esp]           ; move r4 down the stack
        mov     dword ptr 4[esp],0; make imaginary part 0
        next
efcode  RC_CNV


        xdefp   DoRCCnv
defn    DoRCCnv
        pop     edi             ; save return address
        push    [esp]           ; move r4 down the stack
        mov     dword ptr 4[esp],0; make imaginary part 0
        jmp     edi             ; return to caller
endproc DoRCCnv


fcode   RQ_CNV
        push    offset32 II_CNV ; return address
;;;;;;;;hop     DoRQCnv
efcode  RQ_CNV


        xdefp   DoRQCnv
defn    DoRQCnv
        mov     ebx,offset32 Z_RtoD_
        hop     cnv_2
endproc DoRQCnv


        xdefp   DoDCCnv
defn    DoDCCnv
        pop     edi             ; get return address
        mov     eax,[esp]       ; get r8 (and leave scratch room on stack)
        mov     edx,4[esp]      ; ...
        push    0               ; store an imaginary part of 0
        push    0               ; ...
        push    edx             ; store r8
        push    eax             ; ...
        exit_fcode              ; enter run-time environment
        mov     eax,24[esp]     ; get real part
        docall  Z_RtoD_         ; do conversion
        mov     16[esp],eax     ; save result
        mov     20[esp],edx     ; ...
        mov     eax,28[esp]     ; get imaginary part
        docall  Z_RtoD_         ; do conversion
        mov     24[esp],eax     ; ...
        mov     28[esp],edx     ; ...
        enter_fcode             ; enter fcode environment
        jmp     edi             ; return to caller
endproc DoDCCnv


fcode   DQ_CNV
        mov     edi,offset32 II_CNV; return address
        pop     eax             ; get value
        pop     edx             ; ...
        hop     cnv_3
efcode  DQ_CNV


        xdefp   DoDQCnv
defn    DoDQCnv
        pop     edi             ; get return address
        pop     eax             ; get value
        pop     edx             ; ...
        hop     cnv_3
endproc DoDQCnv


        xdefp   DoCICnv
defn    DoCICnv
        pop     edi             ; get return address
        pop     eax             ; move complex*8 arg 4 bytes down stack
        push    [esp]           ; ...
        push    eax             ; ...
        mov     eax,12[esp]     ; get integer
        exit_fcode              ; switch to run-time environment
        docall  Z_ItoR_         ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     8[esp],eax      ; save real part
        mov     dword ptr 12[esp],0; save imaginary part of 0
        jmp     edi             ; return to caller
endproc DoCICnv


        xdefp   DoCRCnv
defn    DoCRCnv
        pop     edi             ; get return address
        pop     eax             ; get complex*8 arg
        pop     edx             ; ...
        pop     ecx             ; get real arg
        push    0               ; push imaginary 0
        push    ecx             ; push real arg
        push    edx             ; push complex*8 arg
        push    eax             ; ...
        jmp     edi             ; return to caller
endproc DoCRCnv


        xdefp   DoCDCnv
defn    DoCDCnv
        pop     edi             ; get return address
        pop     ebx             ; get real part
        pop     ecx             ; get imaginary part
        pop     eax             ; get r8 arg
        pop     edx             ; ...
        push    0               ; push imaginary 0
        push    0               ; ...
        push    edx             ; replace r8
        push    eax             ; ...
        mov     eax,ecx         ; get imag part of C arg
cq_work:
        exit_fcode              ; switch to run-time environment
        docall  Z_RtoD_         ; do conversion
        push    edx             ; push result
        push    eax             ; ...
        mov     eax,ebx         ; get real part of C arg
        docall  Z_RtoD_         ; do conversion
        push    edx             ; push result
        push    eax             ; ...
        enter_fcode             ; switch to f-code environment
        jmp     edi             ; return to caller
endproc DoCDCnv


        xdefp   DoEXCnv
defn    DoEXCnv
;;;;;;  hop MakeCXts            ; hop make an extended complex from an ex
endproc DoEXCnv


defn    MakeCXts
        pop     edi                     ; get return address
        pop     dword ptr tb_result[0]  ; pop the real
        pop     dword ptr tb_result[4]  ; ...
        pop     dword ptr tb_result[8]  ; ...
        pop     dword ptr tb_result[12] ; ...
        mov     eax, 0                  ; make imag
        push    eax                     ; ...
        push    eax                     ; ...
        push    eax                     ; ...
        push    eax                     ; ...
        push    dword ptr tb_result[12] ; push the real
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        jmp     edi                     ; go home
endproc MakeCXts



        xdefp   DoCECnv
defn    DoCECnv
        pop     edx             ; get return address
        pop     ebx             ; get real part
        pop     ecx             ; get imaginary part
        call    MakeCXts        ; make the EXTENDED into an EX complex
        mov     edi, edx        ; place return address in edi
        mov     eax,ecx         ; get imag part of C arg
cx_work:
        exit_fcode              ; switch to run-time environment
        mov     SaveESI, esi    ; save fcode pointer
        mov     esi, offset32 tb_result ; load result address
        docall  Z_RtoE_         ; do conversion
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        mov     eax,ebx         ; get real part of C arg
        mov     esi, offset32 tb_result ; load result address
        docall  Z_RtoE_         ; do conversion
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        enter_fcode             ; switch to f-code environment
        jmp     edi             ; return to caller
endproc DoCECnv


        xdefp   DoQECnv
defn    DoQECnv
        pop     edx             ; get return address
        pop     dword ptr db_keep[0]    ; pop the double complex
        pop     dword ptr db_keep[4]    ; ...
        pop     dword ptr db_keep[8]    ; ...
        pop     dword ptr db_keep[12]   ; ...
        call    MakeCXts        ; make the EXTENDED into an EX complex
        mov     edi, edx        ; place return address in edi
        mov     eax, db_keep[8] ; get imag part of C arg
        mov     edx, db_keep[12]; ...
qx_work:
        exit_fcode              ; switch to run-time environment
        mov     SaveESI, esi    ; save fcode pointer
        mov     esi, offset32 tb_result ; load result address
        docall  Z_DtoE_         ; do conversion
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        mov     eax, db_keep[0] ; get imag part of C arg
        mov     edx, db_keep[4] ; ...
        mov     esi, offset32 tb_result ; load result address
        docall  Z_DtoE_         ; do conversion
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        enter_fcode             ; switch to f-code environment
        jmp     edi             ; return to caller
endproc DoQECnv


        xdefp   DoECCnv
defn    DoECCnv
        mov     edi, offset32 DoCXCnv   ; get function call
        pop     SaveOldRet              ; get the return address
        call    cnv_7                   ; convert C to X
        call    DoEXCnv                 ; convert E to X
        jmp     SaveOldRet              ; return
endproc DoECCnv


        xdefp   DoEQCnv
defn    DoEQCnv
        mov     edi, offset32 DoQXCnv   ; get function call
        pop     SaveOldRet              ; get the return address
        call    cnv_7                   ; convert Q to X
        call    DoEXCnv                 ; convert E to X
        jmp     SaveOldRet              ; return
endproc DoEQCnv


        xdefp   DoEICnv
defn    DoEICnv
        mov     edi, offset32 DoIECnv   ; get function call
        hop     cnv_7
endproc DoEICnv


        xdefp   DoERCnv
defn    DoERCnv
        mov     edi, offset32 DoRECnv   ; get function call
        hop     cnv_7
endproc DoERCnv


        xdefp   DoEDCnv
defn    DoEDCnv
        mov     edi, offset32 DoDECnv   ; get function call
;;;;;;; hop     cnv_7
endproc DoEDCnv


defn    cnv_7
        pop     SaveRet                 ; get the return address
        pop     dword ptr tb_keep[0]    ; pop the xtended
        pop     dword ptr tb_keep[4]    ; ...
        pop     dword ptr tb_keep[8]    ; ...
        pop     dword ptr tb_keep[12]   ; ...
        call    edi                     ; do conversion
        push    dword ptr tb_result[12] ; push the result
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        jmp     SaveRet                 ; return to caller
endproc cnv_7


        xdefp   DoXCCnv
defn    DoXCCnv
        mov     edi, offset32 DoCXCnv   ; get function call
        call    cnv_8                   ; convert C to X
endproc DoXCCnv


        xdefp   DoXQCnv
defn    DoXQCnv
        mov     edi, offset32 DoQXCnv   ; get function call
        call    cnv_8                   ; convert Q to X
endproc DoXQCnv


        xdefp   DoXICnv
defn    DoXICnv
        mov     edi, offset32 DoIXCnv   ; get function call
        hop     cnv_8
endproc DoXICnv


        xdefp   DoXRCnv
defn    DoXRCnv
        mov     edi, offset32 DoRXCnv   ; get function call
        hop     cnv_8
endproc DoXRCnv


        xdefp   DoXDCnv
defn    DoXDCnv
        mov     edi, offset32 DoDXCnv   ; get function call
        hop     cnv_8
endproc DoXDCnv


        xdefp   DoXECnv
defn    DoXECnv
        mov     edi, offset32 DoEXCnv   ; get function call
;;;;;;; hop     cnv_8
endproc DoXECnv


defn    cnv_8
        pop     SaveRet                 ; get the return address
        pop     dword ptr tb_keep[0]    ; pop the xtended
        pop     dword ptr tb_keep[4]    ; ...
        pop     dword ptr tb_keep[8]    ; ...
        pop     dword ptr tb_keep[12]   ; ...
        pop     dword ptr tb_keep[16]   ; ...
        pop     dword ptr tb_keep[20]   ; ...
        pop     dword ptr tb_keep[24]   ; ...
        pop     dword ptr tb_keep[28]   ; ...
        call    edi                     ; do conversion
        push    dword ptr tb_result[28] ; push the result
        push    dword ptr tb_result[24] ; ...
        push    dword ptr tb_result[20] ; ...
        push    dword ptr tb_result[16] ; ...
        push    dword ptr tb_result[12] ; ...
        push    dword ptr tb_result[8]  ; ...
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        jmp     SaveRet                 ; return to caller
endproc cnv_8


fcode   CQ_CNV
        mov     edi,offset32 II_CNV; return address
        pop     ebx             ; get C arg
        pop     eax             ; ...
        jmp     cq_work         ; do conversion
efcode  CQ_CNV


        xdefp   DoCQCnv
defn    DoCQCnv
        pop     edi             ; get return address
        pop     ebx             ; get C arg
        pop     eax             ; ...
        jmp     cq_work         ; do conversion
endproc DoCQCnv


        xdefp   DoCXCnv
defn    DoCXCnv
        pop     edi             ; get return address
        pop     ebx             ; get C arg
        pop     eax             ; ...
        jmp     cx_work         ; do conversion
endproc DoCXCnv


        xdefp   DoQXCnv
defn    DoQXCnv
        pop     edi             ; get return address
        pop     dword ptr db_keep[0]    ; pop the double complex
        pop     dword ptr db_keep[4]    ; ...
        pop     dword ptr db_keep[8]    ; ...
        pop     dword ptr db_keep[12]   ; ...
        jmp     qx_work         ; do conversion
endproc DoQXCnv


        xdefp   DoQICnv
defn    DoQICnv
        mov     ebx,offset32 Z_ItoD_
        hop     cnv_5
endproc DoQICnv


        xdefp   DoQRCnv
defn    DoQRCnv
        mov     ebx,offset32 Z_RtoD_
;;;;;;;;hop     cnv_5
endproc DoQRCnv


defn    cnv_5
        pop     edi             ; get return address
        pop     eax             ; get complex*16
        pop     edx             ; ...
        pop     ecx             ; ...
        sub     esp,8           ; make extra room on stack
        push    8[esp]          ; replace complex*16
        push    ecx             ; ...
        push    edx             ; ...
        push    eax             ; ...
        mov     eax,28[esp]     ; get 4 byte argument
        exit_fcode              ; switch to run-time environment
        call    ebx             ; do conversion
        enter_fcode             ; switch to F-Code environment
        mov     16[esp],eax     ; save real part
        mov     20[esp],edx     ; ...
        xor     eax,eax         ; save imaginary part of 0
        mov     24[esp],eax     ; ...
        mov     28[esp],eax     ; ...
        jmp     edi             ; return to caller
endproc cnv_5


        xdefp   DoQCCnv
defn    DoQCCnv
        pop     edi             ; get return address
        pop     eax             ; get complex*16
        pop     edx             ; ...
        pop     ecx             ; ...
        pop     ebx             ; ...
        sub     esp,8           ; make extra room on stack
        push    ebx             ; replace complex*16
        push    ecx             ; ...
        push    edx             ; ...
        push    eax             ; ...
        mov     eax,24[esp]     ; get real part
        exit_fcode              ; switch to run-time environment
        call    Z_RtoD_         ; do conversion
        mov     16[esp],eax     ; save real part
        mov     20[esp],edx     ; ...
        mov     eax,28[esp]     ; get imaginary part
        call    Z_RtoD_         ; do conversion
        mov     24[esp],eax     ; save imag part
        mov     28[esp],edx     ; ...
        enter_fcode             ; switch to F-Code environment
        jmp     edi             ; return to caller
endproc DoQCCnv


        xdefp   DoQDCnv
defn    DoQDCnv
        pop     edi             ; get return address
        pop     eax             ; pop complex*16 from stack
        pop     edx             ; ...
        pop     ebx             ; ...
        pop     ecx             ; ...
        push    4[esp]          ; mov real*8 arg down stack 8 bytes
        push    4[esp]          ; ...
        push    ecx             ; replace complex*16 arg
        push    ebx             ; ...
        push    edx             ; ...
        push    eax             ; ...
        xor     eax,eax         ; set imaginary part to 0
        mov     24[esp],eax     ; ...
        mov     28[esp],eax     ; ...
        jmp     edi             ; return to caller
endproc DoQDCnv

        fmodend
        end
