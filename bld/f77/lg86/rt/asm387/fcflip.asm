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
; FCFLIP        : Flip args on stack
;

.8087

include fcdef.inc

        fmodstart       fcflip

fcode   II_FLIP
        pop     eax             ; get arg 1
        xchg    eax,[esp]       ; exchange with arg 2
        push    eax             ; push arg 2
;;;;;;;;hop     IR_FLIP
efcode  II_FLIP


fcode   IR_FLIP                 ; no operation needs to be done
dfcode  ID_FLIP
dfcode  IE_FLIP
dfcode  IC_FLIP
dfcode  IQ_FLIP
dfcode  IX_FLIP
dfcode  RI_FLIP
dfcode  DI_FLIP
dfcode  EI_FLIP
dfcode  CI_FLIP
dfcode  QI_FLIP
dfcode  XI_FLIP
        next
efcode  IR_FLIP


                                                                ;  |    +----+--<< complex
                                ;  |    |    |
                                ;  v    v    v
fcode   DQ_FLIP                 ; <a>   b    c
dfcode  RC_FLIP
dfcode  RQ_FLIP
dfcode  RX_FLIP
dfcode  DC_FLIP
dfcode  DX_FLIP
dfcode  EC_FLIP
dfcode  EQ_FLIP
dfcode  EX_FLIP
        fxch    st(2)           ; <c>   b    a
;;;;;;;;hop     RR_FLIP         ; <b>   c    a
efcode  DQ_FLIP


fcode   RR_FLIP
dfcode  RD_FLIP
dfcode  RE_FLIP
dfcode  DR_FLIP
dfcode  DD_FLIP
dfcode  DE_FLIP
dfcode  ER_FLIP
dfcode  ED_FLIP
dfcode  EE_FLIP
        fxch    st(1)           ; exchange
        next
efcode  RR_FLIP


                                                                ;  |    |    +--<< scalar
                                ;  |    |    |
                                ;  v    v    v
fcode   QD_FLIP                  ; <a>   b    c
dfcode  CR_FLIP
dfcode  CD_FLIP
dfcode  CE_FLIP
dfcode  QR_FLIP
dfcode  QE_FLIP
dfcode  XR_FLIP
dfcode  XD_FLIP
dfcode  XE_FLIP
        fxch    st(1)           ; <b>   a    c
fxch_2_next:
        fxch    st(2)           ; <c>   a    b
        next                    ; execute next f-code
efcode  QD_FLIP


fcode   QQ_FLIP                  ; <a>   b    c    d
dfcode  CC_FLIP
dfcode  CQ_FLIP
dfcode  CX_FLIP
dfcode  QC_FLIP
dfcode  QX_FLIP
dfcode  XC_FLIP
dfcode  XQ_FLIP
dfcode  XX_FLIP
        fincstp                 ;  a   <b>   c    d
        fxch    st(2)           ;  a   <d>   c    b
        fdecstp                 ; <a>   d    c    b
        hop     fxch_2_next     ; <c>   d    a    b
efcode  QQ_FLIP


        fmodend
        end
