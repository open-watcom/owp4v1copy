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
;<>     FCFLIP - support for reversing operands on the stack
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc

        fmodstart       fcflip

        dataseg

        TmpArea db      48 dup(0)

        enddata


fcode   II_FLIP
dfcode  RI_FLIP
dfcode  IR_FLIP
dfcode  RR_FLIP
        pop     ax              ; pop op 1
        pop     bx
        pop     cx              ; pop op 2
        pop     dx
        push    bx              ; push op 1
        push    ax
        push    dx              ; push op 2
        push    cx
        next
efcode  II_FLIP


fcode   DI_FLIP
dfcode  CI_FLIP
dfcode  DR_FLIP
dfcode  CR_FLIP
        pop     ax              ; pop op 1
        pop     bx
        pop     cx
        pop     dx
        pop     bp              ; pop op 2
        pop     di
        push    dx              ; push op 1
        push    cx
        push    bx
        push    ax
        push    di              ; push op 2
        push    bp
        next
efcode  DI_FLIP


fcode   ID_FLIP
dfcode  IC_FLIP
dfcode  RD_FLIP
dfcode  RC_FLIP
        pop     bp              ; pop op 1
        pop     di
        pop     ax              ; pop op 2
        pop     bx
        pop     cx
        pop     dx
        push    di              ; push op 1
        push    bp
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        next
efcode  ID_FLIP


fcode   DD_FLIP
dfcode  CD_FLIP
dfcode  DC_FLIP
dfcode  CC_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop     0[bp]           ; pop op 1
        pop     2[bp]
        pop     4[bp]
        pop     6[bp]
        pop     ax              ; pop op 2
        pop     bx
        pop     cx
        pop     dx
        push    6[bp]           ; push op 1
        push    4[bp]
        push    2[bp]
        push    0[bp]
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        next
efcode  DD_FLIP


fcode   IQ_FLIP
dfcode  RQ_FLIP
dfcode  IE_FLIP
dfcode  RE_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop     ax              ; pop op 1
        pop     bx
        pop       [bp]          ; pop op 2
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        hop     push16
efcode  IQ_FLIP


fcode   DQ_FLIP
dfcode  CQ_FLIP
dfcode  DE_FLIP
dfcode  CE_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop     ax              ; pop op 1
        pop     bx
        pop     cx
        pop     dx
        pop       [bp]          ; pop op 2
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        push    dx              ; push op 1
        push    cx
push16:
        push    bx
        push    ax
        push    14[bp]          ; push op 2
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        next
efcode  DQ_FLIP


fcode   IX_FLIP
dfcode  RX_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop     ax              ; pop op 1
        pop     bx
        pop       [bp]          ; pop op 2
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        push    bx              ; push op 1
        push    ax
push32:                         ; push op 2
        push    30[bp]
        push    28[bp]
        push    26[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]
        push    16[bp]
        push    14[bp]
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        next
efcode  IX_FLIP


fcode   DX_FLIP
dfcode  CX_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop     ax              ; pop op 1
        pop     bx
        pop     cx
        pop     dx
        pop       [bp]          ; pop op 2
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        push    dx              ; push op 1
        push    cx
        push    bx
        push    ax
        hop     push32          ; push op 2
efcode  DX_FLIP


fcode   QI_FLIP
dfcode  QR_FLIP
dfcode  EI_FLIP
dfcode  ER_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     ax              ; pop op 2
        pop     bx
        push    14[bp]          ; push op 1
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    bx              ; push op 2
        push    ax
        next
efcode  QI_FLIP


fcode   QD_FLIP
dfcode  QC_FLIP
dfcode  ED_FLIP
dfcode  EC_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     ax              ; pop op 2
        pop     bx
        pop     cx
        pop     dx
        push    14[bp]          ; push op 1
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        next
efcode  QD_FLIP


fcode   XI_FLIP
dfcode  XR_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        pop     ax              ; pop op 2
        pop     bx
        push    30[bp]
        push    28[bp]
        push    26[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]          ; push op 1
        push    16[bp]
        push    14[bp]
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    bx              ; push op 2
        push    ax
        next
efcode  XI_FLIP


fcode   XD_FLIP
dfcode  XC_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        pop     ax              ; pop op 2
        pop     bx
        pop     cx
        pop     dx
        push    30[bp]
        push    28[bp]
        push    26[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]          ; push op 1
        push    16[bp]
        push    14[bp]
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        next
efcode  XD_FLIP


fcode   QQ_FLIP
dfcode  EE_FLIP
dfcode  QE_FLIP
dfcode  EQ_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]          ; pop op 2
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        push    14[bp]          ; push op 1
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        push    22[bp]
        push    20[bp]
        push    18[bp]
        push    16[bp]
        next
efcode  QQ_FLIP


fcode   QX_FLIP
dfcode  EX_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]          ; pop op 2
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        pop     32[bp]
        pop     34[bp]
        pop     36[bp]
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        pop     di
        push    14[bp]          ; push op 1
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    di              ; push op 2
        push    dx
        push    cx
        push    bx
        push    ax
        push    30[bp]
        push    32[bp]
        push    34[bp]
        push    36[bp]
        push    28[bp]
        push    26[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]
        push    16[bp]
        next
efcode  QX_FLIP


fcode   XQ_FLIP
dfcode  XE_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        pop     32[bp]          ; pop op 2
        pop     34[bp]
        pop     36[bp]
        pop     38[bp]
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        push    30[bp]          ; push op 1
        push    28[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]
        push    16[bp]
        push    14[bp]
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    dx              ; push op 2
        push    cx
        push    bx
        push    ax
        push    38[bp]
        push    36[bp]
        push    34[bp]
        push    32[bp]
        next
efcode  XQ_FLIP


fcode   XX_FLIP
        mov     bp,offset DGROUP:TmpArea
        pop       [bp]          ; pop op 1
        pop      2[bp]
        pop      4[bp]
        pop      6[bp]
        pop      8[bp]
        pop     10[bp]
        pop     12[bp]
        pop     14[bp]
        pop     16[bp]
        pop     18[bp]
        pop     20[bp]
        pop     22[bp]
        pop     24[bp]
        pop     26[bp]
        pop     28[bp]
        pop     30[bp]
        pop     32[bp]         ; pop op 2
        pop     34[bp]
        pop     36[bp]
        pop     38[bp]
        pop     40[bp]
        pop     42[bp]
        pop     44[bp]
        pop     46[bp]
        pop     48[bp]
        pop     50[bp]
        pop     52[bp]
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        pop     di
        push    30[bp]          ; push op 1
        push    28[bp]
        push    26[bp]
        push    24[bp]
        push    22[bp]
        push    20[bp]
        push    18[bp]
        push    16[bp]
        push    14[bp]
        push    12[bp]
        push    10[bp]
        push     8[bp]
        push     6[bp]
        push     4[bp]
        push     2[bp]
        push      [bp]
        push    di              ; push op 2
        push    dx
        push    cx
        push    bx
        push    ax
        push    52[bp]
        push    50[bp]
        push    48[bp]
        push    46[bp]
        push    44[bp]
        push    42[bp]
        push    40[bp]
        push    38[bp]
        push    36[bp]
        push    34[bp]
        push    32[bp]
        next
efcode  XX_FLIP

        fmodend
        end
