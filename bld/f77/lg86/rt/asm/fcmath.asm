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

        xref    Z_I4Div_
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
        xref    ChkI4Mul_

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

        fmodstart       fcmath

        dataseg

        public math_rtn

        cplx_res        dw      16 dup(?)
        math_rtn        dw      2 dup(?)

        enddata


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
        call    DoIXCnv
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
        call    DoRDCnv
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
        call    DoRXCnv
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
        pop     AX              ; get low order word
        pop     BX              ; get high order word
        not     BX              ; flip all bits in high word
        neg     AX              ; negate the low word
        sbb     BX,-1           ; add carry to high word if necessary
        push    BX              ; push result back on stack
        push    AX              ; . . .
        next                    ; execute next FCode
efcode  NEGI


fcode   NEGR                    ; negate single prec real
        mov     DI,2
        hop     RNEGATE
efcode  NEGR


fcode   NEGE                    ; negate extended prec real
        mov     DI,8
        hop     RNEGATE
efcode  NEGE


fcode   NEGD                    ; negate double prec real
        mov     DI,6
RNEGATE:
        mov     BP,SP           ; point BP at stack
        mov     AX,[BP][DI]     ; get sign-exponent word
        or      AX,AX           ; see if this word is zero
        _if     ne              ; if it isnt zero
          xor   AH,80H          ; - negate the real
        _endif                  ; endif
        mov     [BP][DI],AX     ; put back sign-exp word
        next                    ; execute next FCode
efcode  NEGD


fcode   NEGC                    ; negate a single prec complex
        mov     DI,4
        mov     BX, 0           ; additional offset for non packed floats
        hop     CNEGATE
efcode  NEGC


fcode   NEGX                    ; negate a extended prec complex
        mov     DI,10           ; ...
        mov     BX, 6           ; additional offset for non packed floats
        hop     CNEGATE
efcode  NEGX


fcode   NEGQ                    ; negate a double prec complex
        mov     DI,8
        mov     BX, 0           ; additional offset for non packed floats
CNEGATE:
        mov     CX,2            ; negate a complex num
        mov     BP,SP           ; point BP to stack
start:                          ; loop
          mov   AX,-2[BP][DI]   ; - get exp of one part of the num
          or    AX,AX           ; -
          _if   ne              ; - if this part is nonzero
            xor AH,80H          ; - - negate this part
          _endif                ; - endif
          mov   -2[BP][DI],AX   ; - return the sign-exp word
          add   DI, BX          ; - additional padding
          shl   DI,1            ; - point to next part of the num
        loop    start           ; until( we have done both parts )
        next                    ; execute next FCode
efcode  NEGQ


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               INTEGER*4 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   II_ADD
        pop     BX              ; get low order word of op1
        pop     AX              ; get high order word
        pop     DX              ; get lo word of op2
        pop     CX              ; get high word of op2
        add     BX,DX           ; add low order words
        adc     AX,CX           ; add high order words
        into                    ; gen interrupt on overflow condition
        push    AX              ; push result back on stack
        push    BX              ; . . .
        next                    ; execute next FCode
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
        getword DI              ; get address of operand
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


fcode   II_MUL
        pop     BX              ; get low order word of op2
        pop     CX              ; get high order word
        mov     AX, SP          ; get pointer to op 1
        mov     DX, SS          ; ...
        docall  ChkI4Mul_       ; multiply with overflow checking
        next                    ; execute next FCode
efcode  II_MUL


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


fcode   RR_CMP
        pop     AX              ; get op_1
        pop     BX              ; . . .
        pop     DX              ; get op_2
        pop     CX              ; . . .
        _guess                  ; guess (the two opernds have different signs)
          mov   DI,BX           ; - get a copy of high word of op_1
          xor   DI,CX           ; - see if op_1 has same sign as op_2
          _quif s               ; - quit if different signs
          cmp   BX,CX           ; - compare high order words
          _quif ne              ; - quif they are nonequal
          cmp   AX,DX           ; - compare low order words
        _endguess               ; endguess
        mov     DI,0            ; clear out DI
        _if     ne              ; if the two operands are unequal
          rcr   DX,1            ; - save carry from the comparison
          xor   BX,DX           ; - switch sign if op_1 was negative
          shl   BX,1            ; - get sign of result
          sbb   DI,0            ; - make DI into a boolean
          shl   DI,1            ; - . . .
          inc   DI              ; - . . .
        _endif                  ; endif
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  RR_CMP


fcode   DD_CMP
        pop     DX              ; get first argument
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_F8Cmp_        ; compare the two
        enter_fcode             ; switch to F-Code environment
        mov     DI,AX           ; put result in proper register
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  DD_CMP


fcode   EE_CMP
        exit_fcode              ; switch to run-time environment
        docall  Z_F16Cmp_       ; compare the two
        enter_fcode             ; switch to F-Code environment
        mov     DI,AX           ; put result in proper register
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  EE_CMP


fcode   CC_CMP                   ; compare complex*8, complex*8
        mov     DI,8            ; get size of operand
        hop     cmplxcmp        ; compare complex numbers
efcode  CC_CMP


fcode   XX_CMP                   ; compare complex*32, complex*32
        ; we must make the padding all zeroed out
        mov     BP, SP
        mov     word ptr 10[BP], 0
        mov     word ptr 12[BP], 0
        mov     word ptr 14[BP], 0
        mov     word ptr 26[BP], 0
        mov     word ptr 28[BP], 0
        mov     word ptr 30[BP], 0
        mov     DI,32            ; get size of operand
        hop     cmplxcmp         ; compare complex numbers
efcode  XX_CMP


fcode   QQ_CMP                   ; compare complex*16, complex*16
        mov     DI,16           ; get size of operand
cmplxcmp:                       ; complex compare rtn
        mov     CX,DI           ; get number of words to compare
        shr     CX,1            ; . . .
        mov     BP,SP           ; get address of op2
        _loop                   ; loop
          mov   AX,[BP]         ; - get word from op1
          cmp   AX,[BP][DI]     ; - compare with word from op2
          _quif ne              ; - quit if not equal
          add   BP,2            ; - point at next word
          dec   CX              ; - decrement # of words to do
        _until  e               ; until done
        add     SP,DI           ; remove arguments from stack
        add     SP,DI           ; . . .
        mov     DI,CX           ; put result of compare in proper reg
        or      DI,DI           ; set flags
        next                    ; execute next FCode
efcode  QQ_CMP


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>                REAL*4 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   RR_ADD
        mov     di,offset Z_F4Add_      ; doing addition
        mov     bp,seg Z_F4Add_         ; ...
        hop     BinOp4                  ; do it
efcode  RR_ADD


fcode   RR_SUB
        mov     di,offset Z_F4Sub_      ; doing subtraction
        mov     bp,seg Z_F4Sub_         ; ...
        hop     BinOp4                  ; do it
efcode  RR_SUB


fcode   RR_MUL
        mov     di,offset Z_F4Mul_      ; doing multiplication
        mov     bp,seg Z_F4Mul_         ; ...
        hop     BinOp4                  ; do it
efcode  RR_MUL


fcode   RR_DIV
        mov     di,offset Z_F4Div_      ; doing division
        mov     bp,seg Z_F4Div_         ; ...
;;;;;;;;hop     BinOp4
efcode  RR_DIV


defn    BinOp4                  ; do a binary operation for REAL*4
        pop     AX              ; get low order word of op1
        pop     DX              ; get high order word
Peephole_entry:
        pop     BX              ; get lo word of op2
        pop     CX              ; get high word of op2
        exit_fcode              ; switch to run-time environment
        mov     SS:math_rtn,DI  ; perform binary operation
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        enter_fcode             ; switch to F-Code environment
        push    DX              ; push result back on stack
        push    AX              ; . . .
        next                    ; execute next FCode
endproc BinOp4


fcode   ADD_L_R4
        getword DI                      ; get address of op1
AddR4:  mov     DX,2[DI]                ; get high word of op1
        mov     AX,0[DI]                ; get low word of op1
        mov     DI,offset Z_F4Add_      ; doing addition
        mov     BP,seg Z_F4Add_         ; ...
        hop     Peephole_entry          ; do the operation
efcode  ADD_L_R4


fcode   ADD_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset AddR4 ; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R4


fcode   SUB_L_R4
        getword DI                      ; get address of op1
SubR4:  mov     DX,2[DI]                ; get high word of op1
        mov     AX,0[DI]                ; get low word of op1
        mov     DI,offset Z_F4Sub_      ; doing subtraction
        mov     BP,seg Z_F4Sub_         ; ...
        hop     Peephole_entry          ; do the operation
efcode  SUB_L_R4


fcode   SUB_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset SubR4 ; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R4


fcode   MUL_L_R4
        getword DI                      ; get address of op1
MulR4:  mov     DX,2[DI]                ; get high word of op1
        mov     AX,0[DI]                ; get low word of op1
        mov     DI,offset Z_F4Mul_      ; doing multiplication
        mov     BP,seg Z_F4Mul_         ; ...
        hop     Peephole_entry          ; do the operation
efcode  MUL_L_R4


fcode   MUL_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset MulR4 ; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R4


fcode   DIV_L_R4
        getword DI                      ; get address of op1
DivR4:  mov     DX,2[DI]                ; get high word of op1
        mov     AX,0[DI]                ; get low word of op1
        mov     DI,offset Z_F4Div_      ; doing division
        mov     BP,seg Z_F4Div_         ; doing division
        hop     Peephole_entry          ; do the operation
efcode  DIV_L_R4


fcode   DIV_UL_R4
        mov     CX,4            ; set size of operand
        mov     BP,offset DivR4 ; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R4


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               REAL*8 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   ADD_I_R8
        getword DI              ; get addr of operand
        mov     AX,DS           ; save the data segment
        lds     DI,[DI]         ; get the operand data segment & offset
        push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
        mov     DS,AX           ; restore program data segment
        hop     DD_ADD          ; do the add
efcode  ADD_I_R8


fcode   ADD_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset AddR8 ; set return address
        jmp     UndChk          ; check operand
efcode  ADD_UL_R8


fcode   ADD_L_R8
        getword DI              ; get addr of operand
AddR8:  push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
;;;;;;;;hop     DD_ADD
efcode  ADD_L_R8


fcode   DD_ADD
        mov     DI,offset Z_F8Add_
        mov     BP,seg Z_F8Add_
        jmp     binop8
efcode  DD_ADD


fcode   SUB_I_R8
        getword DI              ; get addr of operand
        mov     AX,DS           ; save the data segment
        lds     DI,[DI]         ; get the operand data segment & offset
        push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
        mov     DS,AX           ; restore program data segment
        hop     DD_SUB           ; do the subtraction
efcode  SUB_I_R8


fcode   SUB_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset SubR8 ; set return address
        jmp     UndChk          ; check operand
efcode  SUB_UL_R8


fcode   SUB_L_R8
        getword DI              ; get addr of operand
SubR8:  push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
;;;;;;;;hop     DD_SUB
efcode  SUB_L_R8


fcode   DD_SUB
        mov     DI,offset Z_F8Sub_
        mov     BP,seg Z_F8Sub_
        hop     binop8
efcode  DD_SUB


fcode   MUL_I_R8
        getword DI              ; get addr of operand
        mov     AX,DS           ; save the data segment
        lds     DI,[DI]         ; get the operand data segment & offset
        push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
        mov     DS,AX           ; restore program data segment
        hop     DD_MUL           ; do the mul
efcode  MUL_I_R8


fcode   MUL_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset MulR8 ; set return address
        jmp     UndChk          ; check operand
efcode  MUL_UL_R8


fcode   MUL_L_R8
        getword DI              ; get addr of operand
MulR8:  push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
;;;;;;;;hop     DD_MUL
efcode  MUL_L_R8


fcode   DD_MUL
        mov     DI,offset Z_F8Mul_
        mov     BP,seg Z_F8Mul_
        hop     binop8
efcode  DD_MUL


fcode   DIV_I_R8
        getword DI              ; get addr of operand
        mov     AX,DS           ; save the data segment
        lds     DI,[DI]         ; get the operand data segment & offset
        push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
        mov     DS,AX           ; restore program data segment
        hop     DD_DIV           ; do the divide
efcode  DIV_I_R8


fcode   DIV_UL_R8
        mov     CX,8            ; set size of operand
        mov     BP,offset DivR8 ; set return address
        jmp     UndChk          ; check operand
efcode  DIV_UL_R8


fcode   DIV_L_R8
        getword DI              ; get addr of operand
DivR8:  push    6[DI]           ; push operand
        push    4[DI]           ; . . .
        push    2[DI]           ; . . .
        push     [DI]           ; . . .
;;;;;;;;hop     DD_DIV
efcode  DIV_L_R8


fcode   DD_DIV
        mov     DI,offset Z_F8Div_
        mov     BP,seg Z_F8Div_
;;;;;;;;hop     binop8
efcode  DD_DIV


; This routine performs a binary operation on two four word numbers.
; The address of the operation is passed in AX

        xdefp   binop8
defn    binop8
        exit_fcode              ; switch to run-time environment
        pop     DX              ; get first argument
        pop     CX              ; ...
        pop     BX              ; ...
        pop     AX              ; ...
        mov     SS:math_rtn,DI  ; do the binary operation
        mov     SS:math_rtn+2,BP; ...
        call    dword ptr SS:math_rtn
        enter_fcode             ; switch to F-Code environment
        push    AX              ; push result
        push    BX              ; ...
        push    CX              ; ...
        push    DX              ; ...
        next                    ; execute next F-Code
endproc binop8

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>               REAL*16 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   EE_ADD
        mov     DI,offset Z_F16Add_
        mov     BP,seg Z_F16Add_
        jmp     binop16
efcode  EE_ADD


fcode   EE_SUB
        mov     DI,offset Z_F16Sub_
        mov     BP,seg Z_F16Sub_
        hop     binop16
efcode  EE_SUB


fcode   EE_MUL
        mov     DI,offset Z_F16Mul_
        mov     BP,seg Z_F16Mul_
        hop     binop16
efcode  EE_MUL


fcode   EE_DIV
        mov     DI,offset Z_F16Div_
        mov     BP,seg Z_F16Div_
;;;;;;;;hop     binop16
efcode  EE_DIV


; This routine performs a binary operation on two eight word numbers.
; The address of the operation is passed in DI

        xdefp   binop16
defn    binop16
        exit_fcode              ; switch to run-time environment
        mov     SS:math_rtn,DI  ; do the binary operation
        mov     SS:math_rtn+2,BP; ...
        mov     di, si          ; save fcode ptr
        mov     si, offset SS:cplx_res; push result address
        call    dword ptr SS:math_rtn
        mov     CX,8                    ; set # of word in result
        mov     si,offset SS:cplx_res+14; point to last word in result
        jmp     stk_cplx                ; push result
endproc binop16

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>              COMPLEX*8 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   CC_ADD
        mov     DI,offset Z_F4Add_
        mov     BP,seg Z_F4Add_
        hop     cmplx8
efcode  CC_ADD


fcode   CC_SUB
        mov     DI,offset Z_F4Sub_
        mov     BP,seg Z_F4Sub_
;;;;;;;;hop     cmplx8
efcode  CC_SUB


defn    cmplx8
        exit_fcode                      ; switch to run-time environment
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     AX                      ; get real part of arg 1
        pop     DX                      ; ...
        mov     BP,SP                   ; get addressability to args
        mov     BX,4[BP]                ; get real part of arg2
        mov     CX,6[BP]                ; ...
        call    dword ptr SS:math_rtn   ; do the operation
        mov     4[BP],AX                ; save result
        mov     6[BP],DX                ; ...
        pop     AX                      ; get imaginary part of arg 1
        pop     DX                      ; ...
        mov     BX,8[BP]                ; get imaginary part of arg 2
        mov     CX,10[BP]               ; ...
        call    dword ptr SS:math_rtn   ; do the operation
        mov     8[BP],AX                ; save result
        mov     10[BP],DX               ; ...
        enter_fcode                     ; restore F-Code environment
        next                            ; execute next F-Code
endproc cmplx8


fcode   CC_MUL
        mov     DI,offset C8Mul
        mov     BP,seg C8Mul
        hop     cplx8
efcode  CC_MUL


fcode   CC_DIV
        mov     DI,offset C8Div
        mov     BP,seg C8Div
;;;;;;;;hop     cplx8
efcode  CC_DIV


        xdefp   cplx8
defn    cplx8
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     AX                      ; get real part of arg 1
        pop     DX                      ; ...
        pop     BX                      ; get imaginary part of arg 1
        pop     CX                      ; ...
        mov     di,si                   ; save fcode ptr
        mov     si,offset SS:cplx_res   ; pass pointer to result
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do the operation
        mov     CX,4                    ; set # of words in result
        mov     si,offset SS:cplx_res+6 ; point to last word in result
        jmp     stk_cplx
endproc cplx8


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;               COMPLEX*16 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   QQ_ADD
        mov     DI,offset Z_F8Add_
        mov     BP,seg Z_F8Add_
        hop     cmplx16
efcode  QQ_ADD


fcode   QQ_SUB
        mov     DI,offset Z_F8Sub_
        mov     BP,seg Z_F8Sub_
;;;;;;;;hop     cmplx16
efcode  QQ_SUB

;    |     R       | The way complexs are stored on stack
;    |     I       |
;    |     R       |
;    |     I       |
defn    cmplx16
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        exit_fcode                      ; switch to run-time environment
        pop     DX                      ; get real part of arg 1
        pop     CX                      ; ...
        pop     BX                      ; ...
        pop     AX                      ; ...
        mov     BP,SP                   ; point to real part of arg2
        add     BP,8                    ; the last word first.
        push    6[BP]                   ; push arg on stack
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        call    dword ptr SS:math_rtn   ; do the operation
        mov     [BP],DX                 ; save result
        mov     2[BP],CX                ; ...
        mov     4[BP],BX                ; ...
        mov     6[BP],AX                ; ...
        pop     DX                      ; get imaginary part of arg 1
        pop     CX                      ; ...
        pop     BX                      ; ...
        pop     AX                      ; ...
        add     BP,8                    ; point to imaginary part of arg 2
        push    6[BP]                   ; push arg on stack
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        call    dword ptr SS:math_rtn   ; do the operation
        mov     [BP],DX                 ; save result
        mov     2[BP],CX                ; ...
        mov     4[BP],BX                ; ...
        mov     6[BP],AX                ; ...
        enter_fcode                     ; switch to F-Code environment
        next                            ; execute next F-Code
endproc cmplx16


fcode   QQ_MUL
        mov     DI,offset C16Mul
        mov     BP,seg C16Mul
        hop     cplx16
efcode  QQ_MUL


fcode   QQ_DIV
        mov     DI,offset C16Div
        mov     BP,seg C16Div
;;;;;;;;hop     cplx16
efcode  QQ_DIV


        xdefp   cplx16
defn    cplx16
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        pop     DX                      ; get real part of arg 1
        pop     CX                      ; ...
        pop     BX                      ; ...
        pop     AX                      ; ...
        mov     di,si                   ; save fcode ptr
        mov     si,offset SS:cplx_res   ; pass pointer to result
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do the operation
        mov     CX,8                    ; set # of word in result
        mov     si,offset SS:cplx_res+14; point to last word in result
        jmp     stk_cplx                ; push result
endproc cplx16

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;               COMPLEX*32 MATH
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   XX_ADD
        mov     DI,offset Z_F16Add_
        mov     BP,seg Z_F16Add_
        hop     cmplx32
efcode  XX_ADD


fcode   XX_SUB
        mov     DI,offset Z_F16Sub_
        mov     BP,seg Z_F16Sub_
;;;;;;;;hop     cmplx32
efcode  XX_SUB

;    |     R       | The way complexs are stored on stack
;    |     I       |
;    |     R       |
;    |     I       |
defn    cmplx32
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        exit_fcode                      ; switch to run-time environment
        mov     BP, SP                  ; point to secod real argument on stack
        add     BP, 32                  ; ...
        push    14[BP]                  ; push real arg 2 on stack
        push    12[BP]                  ; ...
        push    10[BP]                  ; ...
        push    8[BP]                   ; ...
        push    6[BP]                   ; ...
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        sub     BP, 32                  ; get pointer to first real arg
        push    14[BP]                  ; push real arg 1 on stack
        push    12[BP]                  ; ...
        push    10[BP]                  ; ...
        push    8[BP]                   ; ...
        push    6[BP]                   ; ...
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        mov     DI, SI                  ; save fcode ptr
        add     BP, 32                  ; calaculate result location
        mov     SI, BP                  ; place answer where arg real 2 was
        call    dword ptr SS:math_rtn   ; do the operation
        add     SP, 16                  ; nuke real arg 1
        mov     BP, SP                  ; get imag arg 2
        add     BP, 32                  ; ...
        push    14[BP]                  ; push imag arg 2 on stack
        push    12[BP]                  ; ...
        push    10[BP]                  ; ...
        push    8[BP]                   ; ...
        push    6[BP]                   ; ...
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        sub     BP, 32                  ; get pointer to first imag arg
        push    14[BP]                  ; push real arg 1 on stack
        push    12[BP]                  ; ...
        push    10[BP]                  ; ...
        push    8[BP]                   ; ...
        push    6[BP]                   ; ...
        push    4[BP]                   ; ...
        push    2[BP]                   ; ...
        push    [BP]                    ; ...
        add     BP, 32                  ; calc loaction of return
        mov     SI, BP                  ; place the imag where imag 2 was
        call    dword ptr SS:math_rtn   ; do the operation
        mov     BP, SP                  ; reload sp
        add     SP, 16                  ; nuker first imag
        mov     SI, DI                  ; restore fcode ptr
        enter_fcode                     ; switch to F-Code environment
        next                            ; execute next F-Code
endproc cmplx32


fcode   XX_MUL
        mov     DI,offset C32Mul
        mov     BP,seg C32Mul
        hop     cplx32
efcode  XX_MUL


fcode   XX_DIV
        mov     DI,offset C32Div
        mov     BP,seg C32Div
;;;;;;;;hop     cplx32
efcode  XX_DIV


        xdefp   cplx32
defn    cplx32
        mov     SS:math_rtn,DI          ; save address of math routine
        mov     SS:math_rtn+2,BP        ; ...
        mov     di,si                   ; save fcode ptr
        mov     si,offset SS:cplx_res   ; pass pointer to result
        exit_fcode                      ; switch to run-time environment
        call    dword ptr SS:math_rtn   ; do the operation
        mov     CX,16                   ; set # of word in result
        mov     si,offset SS:cplx_res+18; point to last word in result
        hop     stk_cplx                ; push result
endproc cplx32


defn    stk_cplx                        ; loop
        mov     AX,SS:[SI]              ; - get word
        push    AX                      ; - save word
        sub     SI,2                    ; - point to next word
        loop    stk_cplx                ; until done
        enter_fcode                     ; switch to F-Code environment
        mov     SI,DI                   ; restore f-code ptr
        next                            ; execute next F-Code
endproc stk_cplx

        fmodend
        end
