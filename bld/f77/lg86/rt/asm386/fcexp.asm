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
; FCEXP         : X**Y where X and Y take on all different numeric types
;

include fcdef.inc

        rtxref  PowII
        rtxref  PowRI
        rtxref  PowXI
        rtxref  PowRR
        ifxref  C8Pow
        ifxref  C16Pow
        ifxref  C32Pow
        xref    Z_DtoR_
        xref    Z_F8Pow_
        xref    Z_F16Pow_

        extrn   binop8 : near
        extrn   binop16 : near
        extrn   cplx8 : near
        extrn   cplx16 : near
        extrn   cplx32 : near
        extrn   DoIRCnv : near
        extrn   DoIDCnv : near
        extrn   DoIECnv : near
        extrn   DoICCnv : near
        extrn   DoIQCnv : near
        extrn   DoIXCnv : near
        extrn   DoRICnv : near
        extrn   DoRDCnv : near
        extrn   DoRECnv : near
        extrn   DoRCCnv : near
        extrn   DoRQCnv : near
        extrn   DoRXCnv : near
        extrn   DoDICnv : near
        extrn   DoDRCnv : near
        extrn   DoDECnv : near
        extrn   DoDCCnv : near
        extrn   DoDQCnv : near
        extrn   DoDXCnv : near
        extrn   DoEICnv : near
        extrn   DoERCnv : near
        extrn   DoEDCnv : near
        extrn   DoECCnv : near
        extrn   DoEQCnv : near
        extrn   DoEXCnv : near
        extrn   DoCICnv : near
        extrn   DoCRCnv : near
        extrn   DoCDCnv : near
        extrn   DoCECnv : near
        extrn   DoCQCnv : near
        extrn   DoCXCnv : near
        extrn   DoQICnv : near
        extrn   DoQRCnv : near
        extrn   DoQDCnv : near
        extrn   DoQECnv : near
        extrn   DoQCCnv : near
        extrn   DoQXCnv : near
        extrn   DoXICnv : near
        extrn   DoXRCnv : near
        extrn   DoXDCnv : near
        extrn   DoXECnv : near
        extrn   DoXCCnv : near
        extrn   DoXQCnv : near

        fmodstart       fcexp

        tb_result       dd      8
        SaveESI         dd      0

fcode   II_EXP
        pop     eax
        pop     edx
        exit_fcode              ; switch to run-time environment
        docall  PowII
        enter_fcode             ; switch to F-Code environment
        push    eax
        next
efcode  II_EXP


fcode   RI_EXP
        call    DoRDCnv
        pop     eax             ; get base
        pop     edx             ; ...
        pop     ebx             ; get exponent
        exit_fcode              ; switch to run-time environment
        docall  PowRI           ; perform exponentiation
d_to_r:
        docall  Z_DtoR_         ; convert to single precision
        enter_fcode             ; switch to F-Code environment
        push    eax             ; push result
        next                    ; execute next F-Code
efcode  RI_EXP


fcode   DI_EXP
        pop     eax             ; get base
        pop     edx             ; ...
        pop     ebx             ; get exponent
        exit_fcode              ; switch to run-time environment
        docall  PowRI           ; perform exponentiation
        enter_fcode             ; switch to F-Code environment
        push    edx             ; push result
        push    eax             ; ...
        next                    ; next fcode
efcode  DI_EXP


fcode   EI_EXP
        exit_fcode              ; switch to run-time environment
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, offset32 tb_result ; load address
        docall  PowXI           ; perform exponentiation
        enter_fcode             ; switch to F-Code environment
        push    dword ptr tb_result[12] ; push result
        push    dword ptr tb_result[8]  ; push result
        push    dword ptr tb_result[4]  ; push result
        push    dword ptr tb_result[0]  ; push result
        mov     esi, SaveESI
        next                    ; next fcode
efcode  EI_EXP


fcode   CI_EXP
        call    DoCICnv
        jmp     CC_EXP
efcode  CI_EXP


fcode   QI_EXP
        call    DoQICnv
        jmp     QQ_EXP
efcode  QI_EXP


fcode   XI_EXP
        call    DoXICnv
        jmp     XX_EXP
efcode  XI_EXP


fcode   IR_EXP
        call    DoIRCnv
;;;;;;;;hop     RR_EXP
efcode  IR_EXP


fcode   RR_EXP
        call    DoRDCnv         ; convert arguments to r8
        call    DoDRCnv         ; ...
        pop     eax             ; get base
        pop     edx             ; ...
        pop     ebx             ; get exponent
        pop     ecx             ; ...
        exit_fcode              ; switch to run-time environment
        push    offset32 d_to_r ; where to pick-up execution
        dojmp   PowRR           ; do operation
efcode  RR_EXP


fcode   DR_EXP
        call    DoDRCnv
        hop     DD_EXP
efcode  DR_EXP


fcode   ER_EXP
        call    DoERCnv
        hop     EE_EXP
efcode  ER_EXP


fcode   CR_EXP
        call    DoCRCnv
        jmp    CC_EXP
efcode  CR_EXP


fcode   QR_EXP
        call    DoQRCnv
        jmp     QQ_EXP
efcode  QR_EXP


fcode   XR_EXP
        call    DoXRCnv
        jmp     XX_EXP
efcode  XR_EXP


fcode   ID_EXP
        call    DoIDCnv
        jmp    DD_EXP
efcode  ID_EXP


fcode   RD_EXP
        call    DoRDCnv
;;;;;;;;hop     DD_EXP
efcode  RD_EXP


fcode   DD_EXP
        mov     edi,offset32 Z_F8Pow_
        jmp     binop8
efcode  DD_EXP


fcode   ED_EXP
        call    DoEDCnv
        hop     EE_EXP
efcode  ED_EXP


fcode   CD_EXP
        call    DoCQCnv
;;;;;;;;hop     QD_EXP
efcode  CD_EXP


fcode   QD_EXP
        call    DoQDCnv
        jmp    QQ_EXP
efcode  QD_EXP


fcode   XD_EXP
        call    DoXDCnv
        jmp    XX_EXP
efcode  XD_EXP


fcode   IE_EXP
        call    DoIECnv
        hop     EE_EXP
efcode  IE_EXP


fcode   RE_EXP
        call    DoRECnv
        hop     EE_EXP
efcode  RE_EXP


fcode   DE_EXP
        call    DoDECnv
;;;;;;;;hop     EE_EXP
efcode  DE_EXP


fcode   EE_EXP
        mov     edi,offset32 Z_F16Pow_
        jmp     binop16
efcode  EE_EXP


fcode   CE_EXP
        call    DoCECnv
        jmp     XX_EXP
efcode  CE_EXP


fcode   QE_EXP
        call    DoQECnv
        jmp     XX_EXP
efcode  QE_EXP


fcode   XE_EXP
        call    DoXECnv
        jmp     XX_EXP
efcode  XE_EXP


fcode   IC_EXP
        call    DoICCnv
        hop     CC_EXP
efcode  IC_EXP


fcode   RC_EXP
        call    DoRCCnv
        hop     CC_EXP
efcode  RC_EXP


fcode   DC_EXP
        call    DoDQCnv
        call    DoQCCnv
        hop     QQ_EXP
efcode  DC_EXP


fcode   EC_EXP
        call    DoEXCnv
        call    DoXCCnv
        hop     XX_EXP
efcode  EC_EXP


fcode   CC_EXP
        mov     edi,offset32 C8Pow
        jmp     cplx8
efcode  CC_EXP


fcode   QC_EXP
        call    DoQCCnv
        hop     QQ_EXP
efcode  QC_EXP


fcode   XC_EXP
        call    DoXCCnv
        hop     XX_EXP
efcode  XC_EXP


fcode   IQ_EXP
        call    DoIQCnv
        hop     QQ_EXP
efcode  IQ_EXP


fcode   RQ_EXP
        call    DoRQCnv
        hop     QQ_EXP
efcode  RQ_EXP


fcode   DQ_EXP
        call    DoDQCnv
        hop     QQ_EXP
efcode  DQ_EXP


fcode   EQ_EXP
        call    DoEQCnv
        call    DoEXCnv
        hop     XX_EXP
efcode  EQ_EXP


fcode   CQ_EXP
        call    DoCQCnv
;;;;;;;;hop     QQ_EXP
efcode  CQ_EXP


fcode   QQ_EXP
        mov     edi,offset32 C16Pow
        jmp     cplx16
efcode  QQ_EXP


fcode   XQ_EXP
        call    DoXQCnv
        hop     XX_EXP
efcode  XQ_EXP


fcode   IX_EXP
        call    DoIXCnv
        hop     XX_EXP
efcode  IX_EXP


fcode   RX_EXP
        call    DoRXCnv
        hop     XX_EXP
efcode  RX_EXP


fcode   DX_EXP
        call    DoDXCnv
        hop     XX_EXP
efcode  DX_EXP


fcode   EX_EXP
        call    DoEXCnv
        hop     XX_EXP
efcode  EX_EXP


fcode   CX_EXP
        call    DoCXCnv
        hop     XX_EXP
efcode  CX_EXP


fcode   QX_EXP
        call    DoQXCnv
;;;;;;;;hop     XX_EXP
efcode  QX_EXP


fcode   XX_EXP
        mov     edi,offset32 C32Pow
        jmp     cplx32
efcode  XX_EXP


        fmodend
        end
