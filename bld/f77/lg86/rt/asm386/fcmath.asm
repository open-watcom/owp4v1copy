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
; FCMATH        : math routines
;

include fcdef.inc

        xref    Z_F4Add_
        xref    Z_F4Sub_
        xref    Z_F4Mul_
        xref    Z_F4Div_
        xref    Z_F8Cmp_
        xref    Z_F8Add_
        xref    Z_F8Sub_
        xref    Z_F8Mul_
        xref    Z_F8Div_
        xref    Z_F16Cmp_
        xref    Z_F16Add_
        xref    Z_F16Sub_
        xref    Z_F16Mul_
        xref    Z_F16Div_
        ifxref  C8Mul
        ifxref  C8Div
        ifxref  C16Mul
        ifxref  C16Div
        ifxref  C32Mul
        ifxref  C32Div

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

        extrn   UndChk : near

        dataseg

        SaveESI         dd      0
        cplx_res        dd      8 dup(?)

        enddata

        fmodstart       fcmath

fcode   IR_ADD
        call    DoIRCnv
        jmp     RR_ADD
efcode  IR_ADD


fcode   ID_ADD
        call    DoIDCnv
        jmp     DD_ADD
efcode  ID_ADD


fcode   IE_ADD
        call    DoIECnv
        jmp     EE_ADD
efcode  IE_ADD


fcode   IC_ADD
        call    DoICCnv
        jmp     CC_ADD
efcode  IC_ADD


fcode   IQ_ADD
        call    DoIQCnv
        jmp     QQ_ADD
efcode  IQ_ADD


fcode   IX_ADD
        call    DoIQCnv
        jmp     XX_ADD
efcode  IX_ADD


fcode   RI_ADD
        call    DoRICnv
        jmp     RR_ADD
efcode  RI_ADD


fcode   RD_ADD
        call    DoRDCnv
        jmp     DD_ADD
efcode  RD_ADD


fcode   RE_ADD
        call    DoRECnv
        jmp     EE_ADD
efcode  RE_ADD


fcode   RC_ADD
        call    DoRCCnv
        jmp     CC_ADD
efcode  RC_ADD


fcode   RQ_ADD
        call    DoRQCnv
        jmp     QQ_ADD
efcode  RQ_ADD


fcode   RX_ADD
        call    DoRQCnv
        jmp     XX_ADD
efcode  RX_ADD


fcode   DI_ADD
        call    DoDICnv
        jmp     DD_ADD
efcode  DI_ADD


fcode   DR_ADD
        call    DoDRCnv
        jmp     DD_ADD
efcode  DR_ADD


fcode   DE_ADD
        call    DoDECnv
        jmp     EE_ADD
efcode  DE_ADD


fcode   DC_ADD
        call    DoDCCnv
        jmp     QQ_ADD
efcode  DC_ADD


fcode   DQ_ADD
        call    DoDQCnv
        jmp     QQ_ADD
efcode  DQ_ADD


fcode   DX_ADD
        call    DoDXCnv
        jmp     XX_ADD
efcode  DX_ADD


fcode   EI_ADD
        call    DoEICnv
        jmp     EE_ADD
efcode  EI_ADD


fcode   ER_ADD
        call    DoERCnv
        jmp     EE_ADD
efcode  ER_ADD


fcode   ED_ADD
        call    DoEDCnv
        jmp     EE_ADD
efcode  ED_ADD


fcode   EC_ADD
        call    DoECCnv
        jmp     XX_ADD
efcode  EC_ADD


fcode   EQ_ADD
        call    DoEQCnv
        jmp     XX_ADD
efcode  EQ_ADD


fcode   EX_ADD
        call    DoEXCnv
        jmp     XX_ADD
efcode  EX_ADD


fcode   CI_ADD
        call    DoCICnv
        jmp     CC_ADD
efcode  CI_ADD


fcode   CR_ADD
        call    DoCRCnv
        jmp     CC_ADD
efcode  CR_ADD


fcode   CD_ADD
        call    DoCDCnv
        jmp     QQ_ADD
efcode  CD_ADD


fcode   CE_ADD
        call    DoCECnv
        jmp     XX_ADD
efcode  CE_ADD


fcode   CQ_ADD
        call    DoCQCnv
        jmp     QQ_ADD
efcode  CQ_ADD


fcode   CX_ADD
        call    DoCXCnv
        jmp     XX_ADD
efcode  CX_ADD


fcode   QI_ADD
        call    DoQICnv
        jmp     QQ_ADD
efcode  QI_ADD


fcode   QR_ADD
        call    DoQRCnv
        jmp     QQ_ADD
efcode  QR_ADD


fcode   QD_ADD
        call    DoQDCnv
        jmp     QQ_ADD
efcode  QD_ADD


fcode   QE_ADD
        call    DoQECnv
        jmp     XX_ADD
efcode  QE_ADD


fcode   QC_ADD
        call    DoQCCnv
        jmp     QQ_ADD
efcode  QC_ADD


fcode   QX_ADD
        call    DoQXCnv
        jmp     XX_ADD
efcode  QX_ADD


fcode   XI_ADD
        call    DoXICnv
        jmp     XX_ADD
efcode  XI_ADD


fcode   XR_ADD
        call    DoXRCnv
        jmp     XX_ADD
efcode  XR_ADD


fcode   XD_ADD
        call    DoXDCnv
        jmp     XX_ADD
efcode  XD_ADD


fcode   XE_ADD
        call    DoXECnv
        jmp     XX_ADD
efcode  XE_ADD


fcode   XC_ADD
        call    DoXCCnv
        jmp     XX_ADD
efcode  XC_ADD


fcode   XQ_ADD
        call    DoXQCnv
        jmp     XX_ADD
efcode  XQ_ADD


fcode   IR_SUB
        call    DoIRCnv
        jmp     RR_SUB
efcode  IR_SUB


fcode   ID_SUB
        call    DoIDCnv
        jmp     DD_SUB
efcode  ID_SUB


fcode   IE_SUB
        call    DoIECnv
        jmp     EE_SUB
efcode  IE_SUB


fcode   IC_SUB
        call    DoICCnv
        jmp     CC_SUB
efcode  IC_SUB


fcode   IQ_SUB
        call    DoIQCnv
        jmp     QQ_SUB
efcode  IQ_SUB


fcode   IX_SUB
        call    DoIXCnv
        jmp     XX_SUB
efcode  IX_SUB


fcode   RI_SUB
        call    DoRICnv
        jmp     RR_SUB
efcode  RI_SUB


fcode   RD_SUB
        call    DoRDCnv
        jmp     DD_SUB
efcode  RD_SUB


fcode   RE_SUB
        call    DoRECnv
        jmp     EE_SUB
efcode  RE_SUB


fcode   RC_SUB
        call    DoRCCnv
        jmp     CC_SUB
efcode  RC_SUB


fcode   RQ_SUB
        call    DoRQCnv
        jmp     QQ_SUB
efcode  RQ_SUB


fcode   RX_SUB
        call    DoRXCnv
        jmp     XX_SUB
efcode  RX_SUB


fcode   DI_SUB
        call    DoDICnv
        jmp     DD_SUB
efcode  DI_SUB


fcode   DR_SUB
        call    DoDRCnv
        jmp     DD_SUB
efcode  DR_SUB


fcode   DE_SUB
        call    DoDECnv
        jmp     EE_SUB
efcode  DE_SUB


fcode   DC_SUB
        call    DoDCCnv
        jmp     QQ_SUB
efcode  DC_SUB


fcode   DQ_SUB
        call    DoDQCnv
        jmp     QQ_SUB
efcode  DQ_SUB


fcode   DX_SUB
        call    DoDXCnv
        jmp     XX_SUB
efcode  DX_SUB


fcode   EI_SUB
        call    DoEICnv
        jmp     EE_SUB
efcode  EI_SUB


fcode   ER_SUB
        call    DoERCnv
        jmp     EE_SUB
efcode  ER_SUB


fcode   ED_SUB
        call    DoEDCnv
        jmp     EE_SUB
efcode  ED_SUB


fcode   EC_SUB
        call    DoECCnv
        jmp     XX_SUB
efcode  EC_SUB


fcode   EQ_SUB
        call    DoEQCnv
        jmp     XX_SUB
efcode  EQ_SUB


fcode   EX_SUB
        call    DoEXCnv
        jmp     XX_SUB
efcode  EX_SUB


fcode   CI_SUB
        call    DoCICnv
        jmp     CC_SUB
efcode  CI_SUB


fcode   CR_SUB
        call    DoCRCnv
        jmp     CC_SUB
efcode  CR_SUB


fcode   CD_SUB
        call    DoCDCnv
        jmp     QQ_SUB
efcode  CD_SUB


fcode   CE_SUB
        call    DoCECnv
        jmp     XX_SUB
efcode  CE_SUB


fcode   CQ_SUB
        call    DoCQCnv
        jmp     QQ_SUB
efcode  CQ_SUB


fcode   CX_SUB
        call    DoCXCnv
        jmp     XX_SUB
efcode  CX_SUB


fcode   QI_SUB
        call    DoQICnv
        jmp     QQ_SUB
efcode  QI_SUB


fcode   QR_SUB
        call    DoQRCnv
        jmp     QQ_SUB
efcode  QR_SUB


fcode   QD_SUB
        call    DoQDCnv
        jmp     QQ_SUB
efcode  QD_SUB


fcode   QE_SUB
        call    DoQECnv
        jmp     XX_SUB
efcode  QE_SUB


fcode   QC_SUB
        call    DoQCCnv
        jmp     QQ_SUB
efcode  QC_SUB


fcode   QX_SUB
        call    DoQXCnv
        jmp     XX_SUB
efcode  QX_SUB


fcode   XI_SUB
        call    DoXICnv
        jmp     XX_SUB
efcode  XI_SUB


fcode   XR_SUB
        call    DoXRCnv
        jmp     XX_SUB
efcode  XR_SUB


fcode   XD_SUB
        call    DoXDCnv
        jmp     XX_SUB
efcode  XD_SUB


fcode   XE_SUB
        call    DoXECnv
        jmp     XX_SUB
efcode  XE_SUB


fcode   XC_SUB
        call    DoXCCnv
        jmp     XX_SUB
efcode  XC_SUB


fcode   XQ_SUB
        call    DoXQCnv
        jmp     XX_SUB
efcode  XQ_SUB


fcode   IR_MUL
        call    DoIRCnv
        jmp     RR_MUL
efcode  IR_MUL


fcode   ID_MUL
        call    DoIDCnv
        jmp     DD_MUL
efcode  ID_MUL


fcode   IE_MUL
        call    DoIECnv
        jmp     EE_MUL
efcode  IE_MUL


fcode   IC_MUL
        call    DoICCnv
        jmp     CC_MUL
efcode  IC_MUL


fcode   IQ_MUL
        call    DoIQCnv
        jmp     QQ_MUL
efcode  IQ_MUL


fcode   IX_MUL
        call    DoIXCnv
        jmp     XX_MUL
efcode  IX_MUL


fcode   RI_MUL
        call    DoRICnv
        jmp     RR_MUL
efcode  RI_MUL


fcode   RD_MUL
        call    DoRDCnv
        jmp     DD_MUL
efcode  RD_MUL


fcode   RE_MUL
        call    DoRECnv
        jmp     EE_MUL
efcode  RE_MUL


fcode   RC_MUL
        call    DoRCCnv
        jmp     CC_MUL
efcode  RC_MUL


fcode   RQ_MUL
        call    DoRQCnv
        jmp     QQ_MUL
efcode  RQ_MUL


fcode   RX_MUL
        call    DoRXCnv
        jmp     XX_MUL
efcode  RX_MUL


fcode   DI_MUL
        call    DoDICnv
        jmp     DD_MUL
efcode  DI_MUL


fcode   DR_MUL
        call    DoDRCnv
        jmp     DD_MUL
efcode  DR_MUL


fcode   DE_MUL
        call    DoDECnv
        jmp     EE_MUL
efcode  DE_MUL


fcode   DC_MUL
        call    DoDCCnv
        jmp     QQ_MUL
efcode  DC_MUL


fcode   DQ_MUL
        call    DoDQCnv
        jmp     QQ_MUL
efcode  DQ_MUL


fcode   DX_MUL
        call    DoDXCnv
        jmp     XX_MUL
efcode  DX_MUL


fcode   EI_MUL
        call    DoEICnv
        jmp     EE_MUL
efcode  EI_MUL


fcode   ER_MUL
        call    DoERCnv
        jmp     EE_MUL
efcode  ER_MUL


fcode   ED_MUL
        call    DoEDCnv
        jmp     EE_MUL
efcode  ED_MUL


fcode   EC_MUL
        call    DoECCnv
        jmp     XX_MUL
efcode  EC_MUL


fcode   EQ_MUL
        call    DoEQCnv
        jmp     XX_MUL
efcode  EQ_MUL


fcode   EX_MUL
        call    DoEXCnv
        jmp     XX_MUL
efcode  EX_MUL


fcode   CI_MUL
        call    DoCICnv
        jmp     CC_MUL
efcode  CI_MUL


fcode   CR_MUL
        call    DoCRCnv
        jmp     CC_MUL
efcode  CR_MUL


fcode   CD_MUL
        call    DoCDCnv
        jmp     QQ_MUL
efcode  CD_MUL


fcode   CE_MUL
        call    DoCECnv
        jmp     XX_MUL
efcode  CE_MUL


fcode   CQ_MUL
        call    DoCQCnv
        jmp     QQ_MUL
efcode  CQ_MUL


fcode   CX_MUL
        call    DoCXCnv
        jmp     XX_MUL
efcode  CX_MUL


fcode   QI_MUL
        call    DoQICnv
        jmp     QQ_MUL
efcode  QI_MUL


fcode   QR_MUL
        call    DoQRCnv
        jmp     QQ_MUL
efcode  QR_MUL


fcode   QD_MUL
        call    DoQDCnv
        jmp     QQ_MUL
efcode  QD_MUL


fcode   QE_MUL
        call    DoQECnv
        jmp     XX_MUL
efcode  QE_MUL


fcode   QC_MUL
        call    DoQCCnv
        jmp     QQ_MUL
efcode  QC_MUL


fcode   QX_MUL
        call    DoQXCnv
        jmp     XX_MUL
efcode  QX_MUL


fcode   XI_MUL
        call    DoXICnv
        jmp     XX_MUL
efcode  XI_MUL


fcode   XR_MUL
        call    DoXRCnv
        jmp     XX_MUL
efcode  XR_MUL


fcode   XD_MUL
        call    DoXDCnv
        jmp     XX_MUL
efcode  XD_MUL


fcode   XE_MUL
        call    DoXECnv
        jmp     XX_MUL
efcode  XE_MUL


fcode   XC_MUL
        call    DoXCCnv
        jmp     XX_MUL
efcode  XC_MUL


fcode   XQ_MUL
        call    DoXQCnv
        jmp     XX_MUL
efcode  XQ_MUL


fcode   IR_DIV
        call    DoIRCnv
        jmp     RR_DIV
efcode  IR_DIV


fcode   ID_DIV
        call    DoIDCnv
        jmp     DD_DIV
efcode  ID_DIV


fcode   IE_DIV
        call    DoIECnv
        jmp     EE_DIV
efcode  IE_DIV


fcode   IC_DIV
        call    DoICCnv
        jmp     CC_DIV
efcode  IC_DIV


fcode   IQ_DIV
        call    DoIQCnv
        jmp     QQ_DIV
efcode  IQ_DIV


fcode   IX_DIV
        call    DoIXCnv
        jmp     XX_DIV
efcode  IX_DIV


fcode   RI_DIV
        call    DoRICnv
        jmp     RR_DIV
efcode  RI_DIV


fcode   RD_DIV
        call    DoRDCnv
        jmp     DD_DIV
efcode  RD_DIV


fcode   RE_DIV
        call    DoRECnv
        jmp     EE_DIV
efcode  RE_DIV


fcode   RC_DIV
        call    DoRCCnv
        jmp     CC_DIV
efcode  RC_DIV


fcode   RQ_DIV
        call    DoRQCnv
        jmp     QQ_DIV
efcode  RQ_DIV


fcode   RX_DIV
        call    DoRXCnv
        jmp     XX_DIV
efcode  RX_DIV


fcode   DI_DIV
        call    DoDICnv
        jmp     DD_DIV
efcode  DI_DIV


fcode   DR_DIV
        call    DoDRCnv
        jmp     DD_DIV
efcode  DR_DIV


fcode   DE_DIV
        call    DoDECnv
        jmp     EE_DIV
efcode  DE_DIV


fcode   DC_DIV
        call    DoDCCnv
        jmp     QQ_DIV
efcode  DC_DIV


fcode   DQ_DIV
        call    DoDQCnv
        jmp     QQ_DIV
efcode  DQ_DIV


fcode   DX_DIV
        call    DoDXCnv
        jmp     XX_DIV
efcode  DX_DIV


fcode   EI_DIV
        call    DoEICnv
        jmp     EE_DIV
efcode  EI_DIV


fcode   ER_DIV
        call    DoERCnv
        jmp     EE_DIV
efcode  ER_DIV


fcode   ED_DIV
        call    DoEDCnv
        jmp     EE_DIV
efcode  ED_DIV


fcode   EC_DIV
        call    DoECCnv
        jmp     XX_DIV
efcode  EC_DIV


fcode   EQ_DIV
        call    DoEQCnv
        jmp     XX_DIV
efcode  EQ_DIV


fcode   EX_DIV
        call    DoEXCnv
        jmp     XX_DIV
efcode  EX_DIV


fcode   CI_DIV
        call    DoCICnv
        jmp     CC_DIV
efcode  CI_DIV


fcode   CR_DIV
        call    DoCRCnv
        jmp     CC_DIV
efcode  CR_DIV


fcode   CD_DIV
        call    DoCDCnv
        jmp     QQ_DIV
efcode  CD_DIV


fcode   CE_DIV
        call    DoCECnv
        jmp     XX_DIV
efcode  CE_DIV


fcode   CQ_DIV
        call    DoCQCnv
        jmp     QQ_DIV
efcode  CQ_DIV


fcode   CX_DIV
        call    DoCXCnv
        jmp     XX_DIV
efcode  CX_DIV


fcode   QI_DIV
        call    DoQICnv
        jmp     QQ_DIV
efcode  QI_DIV


fcode   QR_DIV
        call    DoQRCnv
        jmp     QQ_DIV
efcode  QR_DIV


fcode   QD_DIV
        call    DoQDCnv
        jmp     QQ_DIV
efcode  QD_DIV


fcode   QE_DIV
        call    DoQECnv
        jmp     XX_DIV
efcode  QE_DIV


fcode   QC_DIV
        call    DoQCCnv
        jmp     QQ_DIV
efcode  QC_DIV


fcode   QX_DIV
        call    DoQXCnv
        jmp     XX_DIV
efcode  QX_DIV


fcode   XI_DIV
        call    DoXICnv
        jmp     XX_DIV
efcode  XI_DIV


fcode   XR_DIV
        call    DoXRCnv
        jmp     XX_DIV
efcode  XR_DIV


fcode   XD_DIV
        call    DoXDCnv
        jmp     XX_DIV
efcode  XD_DIV


fcode   XE_DIV
        call    DoXECnv
        jmp     XX_DIV
efcode  XE_DIV


fcode   XC_DIV
        call    DoXCCnv
        jmp     XX_DIV
efcode  XC_DIV


fcode   XQ_DIV
        call    DoXQCnv
        jmp     XX_DIV
efcode  XQ_DIV


fcode   IR_CMP
        call    DoIRCnv
        jmp     RR_CMP
efcode  IR_CMP


fcode   ID_CMP
        call    DoIDCnv
        jmp     DD_CMP
efcode  ID_CMP


fcode   IE_CMP
        call    DoIECnv
        jmp     EE_CMP
efcode  IE_CMP


fcode   IC_CMP
        call    DoICCnv
        jmp     CC_CMP
efcode  IC_CMP


fcode   IQ_CMP
        call    DoIQCnv
        jmp     QQ_CMP
efcode  IQ_CMP


fcode   IX_CMP
        call    DoIXCnv
        jmp     XX_CMP
efcode  IX_CMP


fcode   RI_CMP
        call    DoRICnv
        jmp     RR_CMP
efcode  RI_CMP


fcode   RD_CMP
        call    DoRDCnv
        jmp     DD_CMP
efcode  RD_CMP


fcode   RE_CMP
        call    DoRECnv
        jmp     EE_CMP
efcode  RE_CMP


fcode   RC_CMP
        call    DoRCCnv
        jmp     CC_CMP
efcode  RC_CMP


fcode   RQ_CMP
        call    DoRQCnv
        jmp     QQ_CMP
efcode  RQ_CMP


fcode   RX_CMP
        call    DoRXCnv
        jmp     XX_CMP
efcode  RX_CMP


fcode   DI_CMP
        call    DoDICnv
        jmp     DD_CMP
efcode  DI_CMP


fcode   DR_CMP
        call    DoDRCnv
        jmp     DD_CMP
efcode  DR_CMP


fcode   DE_CMP
        call    DoDECnv
        jmp     EE_CMP
efcode  DE_CMP


fcode   DC_CMP
        call    DoDCCnv
        jmp     QQ_CMP
efcode  DC_CMP


fcode   DQ_CMP
        call    DoDQCnv
        jmp     QQ_CMP
efcode  DQ_CMP


fcode   DX_CMP
        call    DoDXCnv
        jmp     XX_CMP
efcode  DX_CMP


fcode   EI_CMP
        call    DoEICnv
        jmp     EE_CMP
efcode  EI_CMP


fcode   ER_CMP
        call    DoERCnv
        jmp     EE_CMP
efcode  ER_CMP


fcode   ED_CMP
        call    DoEDCnv
        jmp     EE_CMP
efcode  ED_CMP


fcode   EC_CMP
        call    DoECCnv
        jmp     XX_CMP
efcode  EC_CMP


fcode   EQ_CMP
        call    DoEQCnv
        jmp     XX_CMP
efcode  EQ_CMP


fcode   EX_CMP
        call    DoEXCnv
        jmp     XX_CMP
efcode  EX_CMP


fcode   CI_CMP
        call    DoCICnv
        jmp     CC_CMP
efcode  CI_CMP


fcode   CR_CMP
        call    DoCRCnv
        jmp     CC_CMP
efcode  CR_CMP


fcode   CD_CMP
        call    DoCDCnv
        jmp     QQ_CMP
efcode  CD_CMP


fcode   CE_CMP
        call    DoCECnv
        jmp     XX_CMP
efcode  CE_CMP


fcode   CQ_CMP
        call    DoCQCnv
        jmp     QQ_CMP
efcode  CQ_CMP


fcode   CX_CMP
        call    DoCXCnv
        jmp     XX_CMP
efcode  CX_CMP


fcode   QI_CMP
        call    DoQICnv
        jmp     QQ_CMP
efcode  QI_CMP


fcode   QR_CMP
        call    DoQRCnv
        jmp     QQ_CMP
efcode  QR_CMP


fcode   QD_CMP
        call    DoQDCnv
        jmp     QQ_CMP
efcode  QD_CMP


fcode   QE_CMP
        call    DoQECnv
        jmp     XX_CMP
efcode  QE_CMP


fcode   QC_CMP
        call    DoQCCnv
        jmp     QQ_CMP
efcode  QC_CMP


fcode   QX_CMP
        call    DoQXCnv
        jmp     XX_CMP
efcode  QX_CMP


fcode   XI_CMP
        call    DoXICnv
        jmp     XX_CMP
efcode  XI_CMP


fcode   XR_CMP
        call    DoXRCnv
        jmp     XX_CMP
efcode  XR_CMP


fcode   XD_CMP
        call    DoXDCnv
        jmp     XX_CMP
efcode  XD_CMP


fcode   XE_CMP
        call    DoXECnv
        jmp     XX_CMP
efcode  XE_CMP


fcode   XC_CMP
        call    DoXCCnv
        jmp     XX_CMP
efcode  XC_CMP


fcode   XQ_CMP
        call    DoXQCnv
        jmp     XX_CMP
efcode  XQ_CMP


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                   NEGATIONS
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   NEGI                    ; negate integer*4
        neg     dword ptr [esp] ; negate integer
        next
efcode  NEGI


fcode   NEGR                    ; negate single prec real
        lea     edi,2[esp]      ; offset of word to play with
        hop     RNEGATE
efcode  NEGR


fcode   NEGE                    ; negate extended prec real
        lea     edi,8[esp]      ; offset of word to play with
        hop     RNEGATE
efcode  NEGE


fcode   NEGD                    ; negate double prec real
        lea     edi,6[esp]      ; offset of dword to play with
RNEGATE:
        mov     ax,[edi]        ; get sign-exponent word
        or      ax,ax           ; see if this word is zero
        _if     ne              ; if it isnt zero
          xor   ah,80H          ; - negate the real
        _endif                  ; endif
        mov     [edi],ax        ; put back sign-exp word
        next                    ; execute next FCode
efcode  NEGD


fcode   NEGC                    ; negate a single prec complex
        lea     edi,2[esp]      ; offset of first word we're interested in
        lea     ebx,6[esp]      ; offset of second word
        hop     CNEGATE
efcode  NEGC


fcode   NEGX                    ; negate a extended prec complex
        lea     edi,8[esp]      ; offset of first sign-exp word
        lea     ebx,24[esp]     ; offset of second sign-exp word
        hop     CNEGATE
efcode  NEGX


fcode   NEGQ                    ; negate a double prec complex
        lea     edi,6[esp]      ; offset of first sign-exp word
        lea     ebx,14[esp]     ; offset of second sign-exp word
CNEGATE:
        mov     ecx,2           ; negate a complex num
start:                          ; loop
          mov   ax,[edi]        ; - get exp of one part of the num
          or    ax,ax           ; -
          _if   ne              ; - if this part is nonzero
            xor AH,80H          ; - - negate this part
          _endif                ; - endif
          mov   [edi],ax        ; - return the sign-exp word
          mov   edi,ebx         ; - point at next part of num
        loop    start           ; until( we have done both parts )
        next                    ; execute next FCode
efcode  NEGQ


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               INTEGER*4 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   II_ADD
        pop     eax             ; get op 1
        add     [esp],eax       ; perform addition and store result
        into                    ; interrupt on overflows
        next
efcode  II_ADD


fcode   ADD_L_I4
        getdw   edi             ; get address of operand
        add     edi,ebp
AddI4:  mov     ecx,[edi]       ; get dword to add
        add     [esp],ecx       ; add
        into                    ; catch overflow
        next
efcode  ADD_L_I4


fcode   ADD_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 AddI4; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_I4


fcode   II_SUB
        pop     eax             ; get op1
        pop     edx             ; get op2
        sub     eax,edx         ; subtract
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
        sub     ecx,eax         ; do subtract
        into                    ; catch overflow
        push    ecx             ; push result
        next
efcode  SUB_L_I4


fcode   SUB_UL_I4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 SubI4; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_I4


fcode   II_MUL
        pop     eax             ; get op1
        pop     edx             ; get op2
        imul    eax,edx         ; do multiplication
        into                    ; check overflow
        push    eax             ; push result
        next
efcode  II_MUL


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


fcode   RR_CMP
        pop     eax             ; get op 1
        pop     edx             ; get op 2
        xor     edi,edi         ; assume equal
        _guess                  ; guess (the two opernds have different signs)
          mov   ebx,eax         ; - get a copy of op 1
          xor   ebx,edx         ; - see if op 1 has same sign as op 2
          _quif s               ; - quit if different signs
          cmp   eax,edx         ; - compare operands
        _endguess               ; endguess
        _if     ne              ; if the two operands are unequal
          rcr   edx,1           ; - save carry from the comparison
          xor   eax,edx         ; - switch sign if op_1 was negative
          shl   eax,1           ; - get sign of result
          sbb   edi,0           ; - make edi into a boolean
          shl   edi,1           ; - . . .
          inc   edi             ; - . . .
        _endif                  ; endif
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  RR_CMP


fcode   DD_CMP
        pop     eax             ; get op 1
        pop     edx             ; ...
        pop     ebx             ; get op 2
        pop     ecx             ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_F8Cmp_        ; compare the two
        enter_fcode             ; switch to F-Code environment
        mov     edi,eax         ; put result in proper register
        or      eax,eax         ; set flags
        next                    ; execute next FCode
efcode  DD_CMP


fcode   EE_CMP
        exit_fcode              ; switch to run-time environment
        docall  Z_F16Cmp_       ; compare the two
        enter_fcode             ; switch to F-Code environment
        add     esp, 14h        ; clear the stack
        mov     edi,eax         ; put result in proper register
        or      eax,eax         ; set flags
        next                    ; execute next FCode
efcode  EE_CMP


fcode   CC_CMP                  ; compare complex*8, complex*8
        pop     eax             ; get op 1
        pop     edx             ; ...
        pop     ebx             ; get op 2
        pop     ecx             ; ...
        sub     eax,ebx         ; subtract low words
        sbb     edx,ecx         ; subtract high words
        or      eax,edx         ; check if result is zero
        setne   dl              ; set dl to 1 if op 1 != op 2
        movzx   edi,dl          ; put result into edi
        or      dl,dl           ; and set flags
        next
efcode  CC_CMP


fcode   QQ_CMP                  ; compare complex*16, complex*16
        lea     ebx,32[esp]     ; to remove args later
        mov     ecx,4           ; 4 dwords to compare
        clc                     ; clear the carry
qq_loop:                        ; loop
          pop   eax             ; - get a dword from op 1
          sbb   eax,12[esp]     ; - subtract corresponding dword in op 2
        loope   qq_loop         ; while result is 0 and ecx > 0
        setne   al              ; al == 1 if op 1 != op 2, 0 otherwise
        mov     esp,ebx         ; pop args from stack
        movzx   edi,al          ; set edi appropriately
        or      edi,edi         ; set flags
        next
efcode  QQ_CMP


fcode   XX_CMP                  ; compare complex*32, complex*32
        lea     ebx,40[esp]     ; to remove args later
        mov     ecx,5           ; 4 dwords to compare
        clc                     ; clear the carry
xx_loop:                        ; loop
          pop   eax             ; - get a dword from op 1
          sbb   eax,16[esp]     ; - subtract corresponding dword in op 2
        loope   xx_loop         ; while result is 0 and ecx > 0
        setne   al              ; al == 1 if op 1 != op 2, 0 otherwise
        mov     esp,ebx         ; pop args from stack
        movzx   edi,al          ; set edi appropriately
        or      edi,edi         ; set flags
        next
efcode  XX_CMP


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                REAL*4 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   RR_ADD
        mov     edi,offset32 Z_F4Add_   ; doing addition
        hop     BinOp4                  ; do it
efcode  RR_ADD


fcode   RR_SUB
        mov     edi,offset32 Z_F4Sub_   ; doing subtraction
        hop     BinOp4                  ; do it
efcode  RR_SUB


fcode   RR_MUL
        mov     edi,offset32 Z_F4Mul_   ; doing multiplication
        hop     BinOp4                  ; do it
efcode  RR_MUL


fcode   RR_DIV
        mov     edi,offset32 Z_F4Div_   ; doing division
;;;;;;;;hop     BinOp4
efcode  RR_DIV


defn    BinOp4                  ; do a binary operation for REAL*4
        pop     eax             ; get op1
Peephole_entry:
        pop     edx             ; get op2
        exit_fcode              ; switch to run-time environment
        call    edi             ; do operation
        enter_fcode             ; switch to F-Code environment
        push    eax             ; push result back
        next                    ; execute next FCode
endproc BinOp4


fcode   ADD_L_R4
        getdw   edi                     ; get address of op1
        add     edi,ebp
AddR4:
        mov     eax,[edi]               ; get op1
        mov     edi,offset32 Z_F4Add_   ; get operation
        hop     Peephole_entry          ; do the operation
efcode  ADD_L_R4


fcode   ADD_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 AddR4; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R4


fcode   SUB_L_R4
        getdw   edi                     ; get address of op1
        add     edi,ebp
SubR4:
        mov     eax,[edi]               ; get op 1
        mov     edi,offset32 Z_F4Sub_   ; get operation
        hop     Peephole_entry          ; do the operation
efcode  SUB_L_R4


fcode   SUB_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 SubR4; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R4


fcode   MUL_L_R4
        getdw   edi                     ; get address of op1
        add     edi,ebp
MulR4:
        mov     eax,[edi]               ; get op 1
        mov     edi,offset32 Z_F4Mul_   ; get operation
        hop     Peephole_entry          ; do the operation
efcode  MUL_L_R4


fcode   MUL_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 MulR4; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R4


fcode   DIV_L_R4
        getdw   edi                     ; get address of op1
        add     edi,ebp
DivR4:
        mov     eax,[edi]               ; get op 1
        mov     edi,offset32 Z_F4Div_   ; get operation
        hop     Peephole_entry          ; do the operation
efcode  DIV_L_R4


fcode   DIV_UL_R4
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 DivR4; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R4


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               REAL*8 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   ADD_I_R8
        getdw   eax             ; get addr of operand
        mov     eax,[eax+ebp]   ; get indirect operand offset
        push    4[eax]          ; push operand
        push    [eax]           ; ...
        hop     DD_ADD          ; do the add
efcode  ADD_I_R8


fcode   ADD_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 AddR8; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R8


fcode   ADD_L_R8
        getdw   edi             ; get addr of operand
        add     edi,ebp
AddR8:
        push    4[edi]          ; push operand
        push    0[edi]          ; ...
;;;;;;;;hop     DD_ADD
efcode  ADD_L_R8


fcode   DD_ADD
        mov     edi,offset32 Z_F8Add_
        jmp     binop8
efcode  DD_ADD


fcode   SUB_I_R8
        getdw   eax             ; get addr of operand
        mov     eax,[eax+ebp]   ; get indirect operand offset
        push    4[eax]          ; push operand
        push    0[eax]          ; ...
        hop     DD_SUB           ; do the subtraction
efcode  SUB_I_R8


fcode   SUB_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 SubR8; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R8


fcode   SUB_L_R8
        getdw   edi             ; get addr of operand
        add     edi,ebp
SubR8:
        push    4[edi]          ; push operand
        push    [edi]           ; ...
;;;;;;;;hop     DD_SUB
efcode  SUB_L_R8


fcode   DD_SUB
        mov     edi,offset32 Z_F8Sub_
        hop     binop8
efcode  DD_SUB


fcode   MUL_I_R8
        getdw   eax             ; get addr of operand
        mov     eax,[eax+ebp]   ; get indirect operand offset
        push    4[eax]          ; push operand
        push    0[eax]          ; ...
        hop     DD_MUL          ; do the mul
efcode  MUL_I_R8


fcode   MUL_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 MulR8; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R8


fcode   MUL_L_R8
        getdw   edi             ; get addr of operand
        add     edi,ebp
MulR8:
        push    4[edi]          ; push operand
        push    [edi]           ; ...
;;;;;;;;hop     DD_MUL
efcode  MUL_L_R8


fcode   DD_MUL
        mov     edi,offset32 Z_F8Mul_
        hop     binop8
efcode  DD_MUL


fcode   DIV_I_R8
        getdw   eax             ; get addr of operand
        mov     eax,[eax+ebp]   ; get indirect operand offset
        push    4[eax]          ; push operand
        push    0[eax]          ; ...
        hop     DD_DIV          ; do the divide
efcode  DIV_I_R8


fcode   DIV_UL_R8
        mov     ecx,8           ; set size of operand
        mov     ebx,offset32 DivR8; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R8


fcode   DIV_L_R8
        getdw   edi             ; get addr of operand
        add     edi,ebp
DivR8:
        push    4[edi]          ; push operand
        push    [edi]           ; ...
;;;;;;;;hop     DD_DIV
efcode  DIV_L_R8


fcode   DD_DIV
        mov     edi,offset32 Z_F8Div_
;;;;;;;;hop     binop8
efcode  DD_DIV


; This routine performs a binary operation on two four word numbers.
; The address of the operation is passed in AX

        xdefp   binop8
defn    binop8
        exit_fcode              ; switch to run-time environment
        pop     eax             ; get op 1
        pop     edx             ; ...
        pop     ebx             ; get op 2
        pop     ecx             ; ...
        call    edi             ; do the binary operation
        enter_fcode             ; switch to F-Code environment
        push    edx             ; push result
        push    eax             ; ...
        next
endproc binop8

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               REAL*16 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   EE_ADD
        mov     edi,offset32 Z_F16Add_
        jmp     binop16
efcode  EE_ADD


fcode   EE_SUB
        mov     edi,offset32 Z_F16Sub_
        hop     binop16
efcode  EE_SUB


fcode   EE_MUL
        mov     edi,offset32 Z_F16Mul_
        hop     binop16
efcode  EE_MUL


fcode   EE_DIV
        mov     edi,offset32 Z_F16Div_
;;;;;;;;hop     binop16
efcode  EE_DIV


; This routine performs a binary operation on two five word numbers.
; The address of the operation is passed in AX

        xdefp   binop16
defn    binop16
        exit_fcode              ; switch to run-time environment
        mov     SaveESI, esi    ; save fcode ptr
        mov     esi, offset32 cplx_res  ; save return result address
        call    edi             ; do the binary operation
        enter_fcode             ; switch to F-Code environment
        push    cplx_res[12]    ; push result on stack
        push    cplx_res[8]     ; push result on stack
        push    cplx_res[4]     ; push result on stack
        push    cplx_res[0]     ; push result on stack
        mov     esi, SaveESI    ; restore fcode ptr
        next
endproc binop16

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>              COMPLEX*8 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   CC_ADD
        mov     edi,offset32 Z_F4Add_
        hop     cmplx8
efcode  CC_ADD


fcode   CC_SUB
        mov     edi,offset32 Z_F4Sub_
;;;;;;;;hop     cmplx8
efcode  CC_SUB


defn    cmplx8
        exit_fcode                      ; switch to run-time environment
        pop     eax                     ; get real part of arg 1
        pop     ebx                     ; get imag part of arg 1
        pop     edx                     ; get real part of arg 2
        call    edi                     ; do operation on real parts
        xchg    eax,ebx                 ; save result and get imag 1
        pop     edx                     ; get imag part of arg 2
        call    edi                     ; do operation on imag parts
        push    eax                     ; push imag result
        push    ebx                     ; push real result
        enter_fcode                     ; switch to f-code environ
        next
endproc cmplx8


fcode   CC_MUL
        mov     edi,offset32 C8Mul
        hop     cplx8
efcode  CC_MUL


fcode   CC_DIV
        mov     edi,offset32 C8Div
;;;;;;;;hop     cplx8
efcode  CC_DIV


        xdefp   cplx8
defn    cplx8
        pop     eax                     ; get real part of arg 1
        pop     edx                     ; get imag part of arg 1
        pop     ebx                     ; get real part of arg 2
        pop     ecx                     ; get imag part of arg 2
        push    esi                     ; save fcode ptr
        mov     esi,offset32 cplx_res   ; pass ptr to result
        exit_fcode                      ; switch to run-time environment
        call    edi                     ; ...
        enter_fcode                     ; switch to f-code environ
        pop     esi                     ; restore fcode ptr
        push    cplx_res+4              ; push result
        push    cplx_res
        next
endproc cplx8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;               COMPLEX*16 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   QQ_ADD
        mov     edi,offset32 Z_F8Add_
        hop     cmplx16
efcode  QQ_ADD


fcode   QQ_SUB
        mov     edi,offset32 Z_F8Sub_
;;;;;;;;hop     cmplx16
efcode  QQ_SUB


defn    cmplx16
        exit_fcode                      ; switch to run-time environment
        pop     eax                     ; get real part of arg 1
        pop     edx                     ; ...
        mov     ebx,8[esp]              ; get real part of arg 2
        mov     ecx,12[esp]             ; ...
        call    edi                     ; do operation on real parts
        mov     8[esp],eax              ; store result
        mov     12[esp],edx             ; ...
        pop     eax                     ; get imag part of arg 1
        pop     edx                     ; ...
        mov     ebx,8[esp]              ; get imag part of arg 2
        mov     ecx,12[esp]             ; ...
        call    edi                     ; do operation on real parts
        mov     8[esp],eax              ; store result
        mov     12[esp],edx             ; ...
        enter_fcode                     ; switch to f-code environment
        next
endproc cmplx16


fcode   QQ_MUL
        mov     edi,offset32 C16Mul
        hop     cplx16
efcode  QQ_MUL


fcode   QQ_DIV
        mov     edi,offset32 C16Div
;;;;;;;;hop     cplx16
efcode  QQ_DIV


        xdefp   cplx16
defn    cplx16
        pop     eax                     ; get real part of arg 1
        pop     edx                     ; ...
        pop     ebx                     ; get imag part of arg 1
        pop     ecx                     ; ...
        mov     SaveESI,esi             ; save fcode ptr
        mov     esi,offset32 cplx_res   ; pass pointer to result
        exit_fcode                      ; switch to run-time environment
        call    edi                     ; ...
        mov     esi,SaveESI             ; restore fcode ptr
        push    cplx_res+12             ; push result
        push    cplx_res+8              ; ...
        push    cplx_res+4              ; ...
        push    cplx_res                ; ...
        next
endproc cplx16


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;               COMPLEX*32 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   XX_ADD
        mov     edi,offset32 Z_F16Add_
        hop     cmplx32
efcode  XX_ADD


fcode   XX_SUB
        mov     edi,offset32 Z_F16Sub_
;;;;;;;;hop     cmplx32
efcode  XX_SUB


defn    cmplx32
        exit_fcode                      ; switch to run-time environment
        push    12[esp]                 ; create second storage on stack
        push    12[esp]                 ; and store first real part
        push    12[esp]                 ; ...
        push    12[esp]                 ; ...
        mov     eax, 48[esp]            ; mov second real part to where arg1
        mov     16[esp], eax            ; used to be.
        mov     eax, 52[esp]            ; ...
        mov     20[esp], eax            ; ...
        mov     eax, 56[esp]            ; ...
        mov     24[esp], eax            ; ...
        mov     eax, 60[esp]            ; ...
        mov     28[esp], eax            ; ...
        mov     SaveESI,esi             ; save fcode ptr
        mov     esi, esp                ; push return result address
        add     esi, 48                 ; ...
        call    edi                     ; do operation on real parts
        push    12[esp]                 ; create second storage on stack
        push    12[esp]                 ; and store first imag part
        push    12[esp]                 ; ...
        push    12[esp]                 ; ...
        mov     eax, 48[esp]            ; mov second imag part to where arg1
        mov     16[esp], eax            ; used to be.
        mov     eax, 52[esp]            ; ...
        mov     20[esp], eax            ; ...
        mov     eax, 56[esp]            ; ...
        mov     24[esp], eax            ; ...
        mov     eax, 60[esp]            ; ...
        mov     28[esp], eax            ; ...
        mov     esi, esp                ; push return result address
        add     esi, 48                 ; ...
        call    edi                     ; do operation on real parts
        enter_fcode                     ; switch to f-code environment
        next
endproc cmplx32


fcode   XX_MUL
        mov     edi,offset32 C32Mul
        hop     cplx32
efcode  XX_MUL


fcode   XX_DIV
        mov     edi,offset32 C32Div
;;;;;;;;hop     cplx32
efcode  XX_DIV


        xdefp   cplx32
defn    cplx32
        mov     SaveESI,esi             ; save fcode ptr
        mov     esi,offset32 cplx_res   ; pass pointer to result
        exit_fcode                      ; switch to run-time environment
        call    edi                     ; ...
        mov     esi,SaveESI             ; restore fcode ptr
        push    cplx_res+28             ; push result
        push    cplx_res+24             ; ...
        push    cplx_res+20             ; ...
        push    cplx_res+16             ; ...
        push    cplx_res+12             ; ...
        push    cplx_res+8              ; ...
        push    cplx_res+4              ; ...
        push    cplx_res                ; ...
        mov     esi, SaveESI            ; restore fcode ptr
        next
endproc cplx32

        fmodend
        end
