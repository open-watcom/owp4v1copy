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
;* Description:  Alpha AXP division routines. Alpha has no integer divide
;*               instruction, oh joy.
;*
;*****************************************************************************


.set noat
.text

.globl      _OtsRemainder32Unsigned
.globl      _OtsDivide32Unsigned
.globl      _OtsModulus32
.globl      _OtsRemainder32
.globl      _OtsDivide32Overflow
.globl      _OtsDivide32
.globl      _OtsDivide64Overflow
.globl      _OtsDivide64
.globl      _OtsRemainder64
.globl      _OtsModulus64
.globl      _OtsDivide64Unsigned
.globl      _OtsRemainder64Unsigned
.extern     _OtsDivData

_OtsRemainder32Unsigned:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    lda           $at,-0x200($a1)
    subl          $a1,0x00000001,$t0
    blt           $a1,X$4
    and           $a1,$t0,$a2
    bgt           $at,X$3
    addq          $a1,$a1,$v0
    beq           $a2,X$1
    s8addq        $v0,$t12,$t12
    ldq           $t0,($t12)
    cmpult        $a0,$a1,$a2
    zap           $a0,0x000000f0,$v0
    bne           $a2,X$2
    umulh         $v0,$t0,$v0
    mull          $v0,$a1,$v0
    subl          $a0,$v0,$v0
    ret           $zero,($ra),0x00000001
X$1:
    beq           $a1,divzer
    and           $a0,$t0,$v0
    ret           $zero,($ra),0x00000001
X$2:
    mov           $a0,$v0
    ret           $zero,($ra),0x00000001
X$3:
    zap           $a0,0x000000f0,$a0
    bsr           $at,div32
    addl          $t0,0x00000000,$v0
    ret           $zero,($ra),0x00000001
X$4:
    cmpult        $a0,$a1,$t0
    subl          $a0,$a1,$v0
    cmovne        $t0,$a0,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsDivide32Unsigned:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    lda           $at,-0x200($a1)
    blt           $a1,X$8
    addq          $a1,$a1,$a2
    cmpule        $a1,$a0,$v0
    beq           $a1,divzer
    s8addq        $a2,$t12,$t12
    beq           $v0,X$5
    bgt           $at,X$7
    ldq           $t0,($t12)
    zap           $a0,0x000000f0,$a0
    blt           $t0,X$6
    umulh         $a0,$t0,$v0
X$5:
    ret           $zero,($ra),0x00000001
    nop
X$6:
    srl           $a0,$t0,$v0
    addl          $v0,0x00000000,$v0
    ret           $zero,($ra),0x00000001
X$7:
    zap           $a0,0x000000f0,$a0
    bsr           $at,div32
    addl          $v0,0x00000000,$v0
    ret           $zero,($ra),0x00000001
X$8:
    cmpule        $a1,$a0,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsModulus32:
    subq          $zero,$a1,$a2
    cmovge        $a1,$a1,$a2
    subq          $a2,0x00000001,$t0
    beq           $a1,divzer
    and           $a2,$t0,$v0
    beq           $v0,X$9
    xor           $a0,$a1,$at
    clr           $a3
    cmovlt        $at,$a1,$a3
    and           $a0,$a1,$t12
    mov           $a2,$a1
    subq          $zero,$a0,$a2
    cmovlt        $a0,$a2,$a0
    cmplt         $t12,$zero,$v0
    sll           $v0,0x0000003f,$v0
    bis           $v0,$a3,$a3
    bsr           $at,div32
    cmoveq        $t0,$zero,$a3
    addq          $a3,$a3,$a2
    subq          $a3,0x00000001,$at
    negl          $t0,$v0
    cmovlt        $at,$t0,$v0
    cmoveq        $a2,$zero,$a3
    addl          $a3,$v0,$v0
    ret           $zero,($ra),0x00000001
X$9:
    cmovge        $a1,$zero,$a1
    and           $a0,$t0,$t0
    cmoveq        $t0,$zero,$a1
    addl          $a1,$t0,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsRemainder32:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    subq          $zero,$a1,$a2
    cmovlt        $a1,$a2,$a1
    subq          $a1,0x00000001,$t0
    and           $a1,$t0,$v0
    sra           $a0,0x0000003f,$a3
    subq          $zero,$a0,$a2
    cmovlt        $a0,$a2,$a0
    beq           $v0,X$10
    lda           $at,-0x200($a1)
    bgt           $at,X$11
    addq          $a1,$a1,$v0
    s8addq        $v0,$t12,$t12
    ldq           $t0,($t12)
    umulh         $a0,$t0,$v0
    mull          $v0,$a1,$v0
    subl          $a0,$v0,$v0
    xor           $v0,$a3,$v0
    subl          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
X$10:
    and           $a0,$t0,$v0
    beq           $a1,divzer
    xor           $v0,$a3,$v0
    subl          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
    nop
X$11:
    bsr           $at,div32
    xor           $t0,$a3,$v0
    subl          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsDivide32Overflow:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    not           $a1,$t0
    bne           $t0,X$12
    negl/v        $a0,$v0
    ret           $zero,($ra),0x00000001
    nop
    nop
_OtsDivide32:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
X$12:
    lda           $at,-0x200($a1)
    ble           $a1,X$16
    addq          $a1,$a1,$v0
    subq          $zero,$a0,$a2
    bgt           $at,X$17
    s8addq        $v0,$t12,$t12
    cmpule        $a1,$a2,$v0
    cmovge        $a0,$a0,$a2
    beq           $v0,X$13
    ldq           $t0,($t12)
    blt           $t0,X$15
    umulh         $a2,$t0,$v0
    blt           $a0,X$14
X$13:
    ret           $zero,($ra),0x00000001
X$14:
    negl          $v0,$v0
    ret           $zero,($ra),0x00000001
X$15:
    srl           $a2,$t0,$a2
    sra           $a0,0x0000003f,$a0
    xor           $a2,$a0,$a2
    subl          $a2,$a0,$v0
    ret           $zero,($ra),0x00000001
X$16:
    beq           $a1,divzer
    negl          $a1,$a1
    subq          $zero,$a0,$a0
    bgt           $a1,X$12
    addl          $a0,0x00000000,$a0
    cmpeq         $a0,$a1,$v0
    ret           $zero,($ra),0x00000001
    nop
X$17:
    sra           $a0,0x0000003f,$a3
    cmovlt        $a0,$a2,$a0
    bsr           $at,div32
    xor           $v0,$a3,$v0
    subl          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
div32:
    cmpule        $a1,$a0,$v0
    sll           $a1,0x00000020,$a2
    sll           $a1,0x00000008,$t0
    beq           $v0,X$21
X$18:
    mov           0x00000005,$a1
    cmpule        $t0,$a0,$v0
    sll           $t0,0x00000008,$t0
    beq           $v0,X$19
    mov           0x00000006,$a1
    cmpule        $t0,$a0,$v0
    sll           $t0,0x00000008,$t0
    beq           $v0,X$19
    mov           0x00000007,$a1
    cmpule        $t0,$a0,$v0
    nop
    cmovne        $v0,0x00000008,$a1
X$19:
    extqh         $a0,$a1,$v0
    subq          $a2,0x00000001,$t0
    s8subq        $a1,0x00000022,$a1
    addq          $v0,$v0,$v0
X$20:
    subq          $v0,$t0,$a2
    cmovge        $a2,$a2,$v0
    addq          $v0,$v0,$v0
    subq          $v0,$t0,$a2
    cmovge        $a2,$a2,$v0
    subq          $a1,0x00000002,$a1
    addq          $v0,$v0,$v0
    bgt           $a1,X$20
    subq          $v0,$t0,$a2
    cmovge        $a2,$a2,$v0
    addq          $v0,$v0,$v0
    subq          $v0,$t0,$a2
    cmovge        $a2,$a2,$v0
    srl           $v0,0x00000020,$t0
    zap           $v0,0x000000f0,$v0
    nop
X$21:
    cmoveq        $v0,$a0,$t0
    ret           $zero,($at),0x00000001
    sll           $a1,0x00000020,$a2
    cmpule        $a1,$a0,$v0
    srl           $a1,0x0000001f,$t0
    beq           $v0,X$25
    cmpule        $a2,$a0,$v0
    bis           $t0,$v0,$v0
    sll           $a1,0x00000008,$t0
    beq           $v0,X$18
    cmpbge        $zero,$a1,$v0
    sll           $v0,0x00000004,$v0
    subq          $t12,$v0,$v0
    ldq           $t0,0x1000($v0)
    subq          $t0,0x00000001,$t0
    extbl         $a1,$t0,$v0
    addq          $v0,$v0,$v0
    s8addq        $v0,$t12,$v0
    ldq           $v0,0x10($v0)
    subq          $zero,$t0,$t0
    s8subq        $t0,$v0,$v0
    and           $v0,0x0000003f,$v0
    sll           $a1,$v0,$a1
    srl           $a1,0x00000036,$t0
    addq          $t0,0x00000001,$t0
    bic           $t0,0x00000001,$t0
    s8addq        $t0,$t12,$t12
    sll           $t0,0x00000036,$t0
    ldq           $a2,($t12)
    subq          $t0,$a1,$t0
    beq           $t0,X$23
    inswl         $a2,0x00000006,$a2
    blt           $t0,X$22
    umulh         $t0,$a2,$t0
    ldq           $t12,0x8($t12)
    br            div32
X$22:
    umulh         $t0,$a2,$t0
    ldq           $t12,0x8($t12)
    s4addq        $a2,0x00000000,$a2
    subq          $t12,$a2,$t12
    s4addq        $t0,$t12,$a2
    umulh         $a2,$a1,$t0
    addq          $t0,$a1,$t0
    subq          $zero,$t0,$t0
    umulh         $a2,$t0,$t12
    cmovlt        $t0,0x00000000,$a2
    addq          $a2,$t0,$t0
    addq          $t12,$t0,$a2
    umulh         $a2,$a1,$t0
    addq          $t0,$a1,$t0
    subq          $zero,$t0,$t0
    umulh         $a2,$t0,$t12
    cmovlt        $t0,0x00000000,$a2
    addq          $a2,$t0,$t0
    addq          $t12,$t0,$t0
    umulh         $t0,$a0,$a2
    srl           $a1,$v0,$a1
    subq          $zero,$v0,$v0
    subq          $v0,0x00000008,$v0
    addq          $a2,$a0,$a2
    cmpult        $a2,$a0,$t0
    srl           $a2,0x00000008,$a2
    sll           $t0,0x00000038,$t0
    addq          $t0,$a2,$t0
    srl           $t0,$v0,$v0
    mulq          $a1,$v0,$t0
    subq          $a0,$t0,$t0
    cmpule        $a1,$t0,$a2
    subq          $t0,$a1,$t12
    cmovne        $a2,$t12,$t0
    addq          $v0,$a2,$v0
    ret           $zero,($at),0x00000001
X$23:
    ldq           $t0,0x8($t12)
    srl           $a1,$v0,$a1
    blt           $a2,X$24
    umulh         $t0,$a0,$a2
    subq          $zero,$v0,$v0
    and           $a0,0x000000ff,$t0
    subq          $v0,0x00000008,$v0
    srl           $a0,0x00000008,$t12
    addq          $a2,$t0,$t0
    srl           $t0,0x00000008,$t0
    addq          $t0,$t12,$t0
    srl           $t0,$v0,$v0
    mulq          $a1,$v0,$t0
    subq          $a0,$t0,$t0
    ret           $zero,($at),0x00000001
X$24:
    not           $v0,$v0
    subq          $a1,0x00000001,$t0
    srl           $a0,$v0,$v0
    and           $a0,$t0,$t0
    ret           $zero,($at),0x00000001
X$25:
    mov           $a0,$t0
    ret           $zero,($at),0x00000001
    nop
_OtsDivide64Overflow:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    not           $a1,$t0
    bne           $t0,X$26
    subq/v        $zero,$a0,$v0
    ret           $zero,($ra),0x00000001
    nop
    nop
_OtsDivide64:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
X$26:
    xor           $a0,$a1,$a3
X$27:
    lda           $at,-0x200($a1)
    ble           $a1,X$31
    addq          $a1,$a1,$v0
    subq          $zero,$a0,$a2
    bgt           $at,X$32
    s8addq        $v0,$t12,$t12
    srl           $a0,0x00000021,$t0
    cmpule        $a1,$a2,$v0
    bne           $t0,X$28
    cmovge        $a0,$a0,$a2
    beq           $v0,X$30
    ldq           $t12,($t12)
    sra           $a3,0x0000003f,$a3
    blt           $t12,X$29
    umulh         $a2,$t12,$v0
    beq           $a3,X$30
    subq          $zero,$v0,$v0
    ret           $zero,($ra),0x00000001
X$28:
    cmovge        $a0,$a0,$a2
    beq           $v0,X$30
    ldq           $t0,0x8($t12)
    sra           $a3,0x0000003f,$a3
    ldq           $t12,($t12)
    beq           $t0,X$29
    umulh         $a2,$t0,$v0
    addq          $v0,$a2,$a2
X$29:
    srl           $a2,$t12,$a2
    xor           $a2,$a3,$a2
    subq          $a2,$a3,$v0
X$30:
    ret           $zero,($ra),0x00000001
X$31:
    beq           $a1,divzer
    subq          $zero,$a1,$a1
    subq          $zero,$a0,$a0
    bgt           $a1,X$27
    cmpeq         $a0,$a1,$v0
    ret           $zero,($ra),0x00000001
X$32:
    sra           $a3,0x0000003f,$a3
    cmovlt        $a0,$a2,$a0
    bsr           $at,div32
    xor           $v0,$a3,$v0
    subq          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsRemainder64:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    subq          $zero,$a1,$a2
    cmovlt        $a1,$a2,$a1
    subq          $a1,0x00000001,$t0
    and           $a1,$t0,$v0
    sra           $a0,0x0000003f,$a3
    subq          $zero,$a0,$a2
    cmovlt        $a0,$a2,$a0
    beq           $v0,X$33
    lda           $at,-0x200($a1)
    bgt           $at,X$34
    addq          $a1,$a1,$v0
    s8addq        $v0,$t12,$t12
    ldq           $t0,0x8($t12)
    ldq           $a2,($t12)
    umulh         $a0,$t0,$v0
    addq          $v0,$a0,$v0
    srl           $v0,$a2,$v0
    mulq          $v0,$a1,$v0
    subq          $a0,$v0,$v0
    xor           $v0,$a3,$v0
    subq          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
X$33:
    subq          $zero,$a0,$a2
    cmovlt        $a0,$a2,$a0
    and           $a0,$t0,$v0
    beq           $a1,divzer
    xor           $v0,$a3,$v0
    subq          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
X$34:
    bsr           $at,div32
    xor           $t0,$a3,$v0
    subq          $v0,$a3,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsModulus64:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    subq          $zero,$a1,$a2
    cmovge        $a1,$a1,$a2
    subq          $a2,0x00000001,$t0
    beq           $a1,divzer
    and           $a2,$t0,$v0
    beq           $v0,X$35
    xor           $a0,$a1,$at
    clr           $a3
    cmovlt        $at,$a1,$a3
    and           $a0,$a1,$at
    mov           $a2,$a1
    subq          $zero,$a0,$a2
    cmovlt        $a0,$a2,$a0
    cmplt         $at,$zero,$v0
    sll           $v0,0x0000003f,$v0
    bis           $v0,$a3,$a3
    bsr           $at,div32
    cmoveq        $t0,$zero,$a3
    addq          $a3,$a3,$a2
    subq          $a3,0x00000001,$at
    subq          $zero,$t0,$v0
    cmovlt        $at,$t0,$v0
    cmoveq        $a2,$zero,$a3
    addq          $a3,$v0,$v0
    ret           $zero,($ra),0x00000001
X$35:
    cmovge        $a1,$zero,$a1
    and           $a0,$t0,$t0
    cmoveq        $t0,$zero,$a1
    addq          $a1,$t0,$v0
    ret           $zero,($ra),0x00000001
    nop
    nop
_OtsDivide64Unsigned:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    lda           $at,-0x200($a1)
    blt           $a1,X$40
    addq          $a1,$a1,$a2
    srl           $a0,0x00000021,$a3
    beq           $a1,divzer
    s8addq        $a2,$t12,$a2
    bgt           $at,X$39
    cmpule        $a1,$a0,$v0
    bne           $a3,X$36
    ldq           $t12,($a2)
    beq           $v0,X$38
    blt           $t12,X$37
    umulh         $a0,$t12,$v0
    ret           $zero,($ra),0x00000001
    nop
X$36:
    ldq           $t0,0x8($a2)
    ldq           $t12,($a2)
    beq           $v0,X$38
    beq           $t0,X$37
    umulh         $a0,$t0,$v0
    zap           $a0,0x0000000f,$a2
    zapnot        $a0,0x0000000f,$a0
    srl           $a2,$t12,$a2
    addq          $v0,$a0,$v0
    srl           $v0,$t12,$v0
    addq          $v0,$a2,$v0
    ret           $zero,($ra),0x00000001
X$37:
    srl           $a0,$t12,$v0
X$38:
    ret           $zero,($ra),0x00000001
X$39:
    bsr           $at,div32
    ret           $zero,($ra),0x00000001
X$40:
    cmpule        $a1,$a0,$v0
    ret           $zero,($ra),0x00000001
    nop
_OtsRemainder64Unsigned:
    ldah          $t12,h^_OtsDivData($zero)
    lda           $t12,l^_OtsDivData($t12)
    lda           $at,-0x200($a1)
    subq          $a1,0x00000001,$t0
    blt           $a1,X$44
    and           $a1,$t0,$a2
    bgt           $at,X$45
    addq          $a1,$a1,$v0
    beq           $a2,X$42
    s8addq        $v0,$t12,$t12
    ldq           $t0,0x8($t12)
    cmpult        $a0,$a1,$a2
    bne           $a2,X$43
    ldq           $a3,($t12)
    umulh         $a0,$t0,$v0
    blt           $a0,X$41
    addq          $v0,$a0,$v0
    srl           $v0,$a3,$v0
    mulq          $v0,$a1,$v0
    subq          $a0,$v0,$v0
    ret           $zero,($ra),0x00000001
X$41:
    zap           $a0,0x0000000f,$a2
    zapnot        $a0,0x0000000f,$t0
    srl           $a2,$a3,$a2
    addq          $v0,$t0,$v0
    srl           $v0,$a3,$v0
    addq          $v0,$a2,$v0
    mulq          $v0,$a1,$v0
    subq          $a0,$v0,$v0
    ret           $zero,($ra),0x00000001
X$42:
    beq           $a1,divzer
    and           $a0,$t0,$v0
    ret           $zero,($ra),0x00000001
X$43:
    mov           $a0,$v0
    ret           $zero,($ra),0x00000001
X$44:
    cmpult        $a0,$a1,$t0
    subq          $a0,$a1,$v0
    cmovne        $t0,$a0,$v0
    ret           $zero,($ra),0x00000001
    nop
X$45:
    bsr           $at,div32
    mov           $t0,$v0
    ret           $zero,($ra),0x00000001
divzer:
    mov           0xfffffffe,$a0
    clr           $v0
    clr           $t0
    call_pal      0x000000aa
    ret           $zero,($ra),0x00000001
    nop
    nop
    nop

.new_section .pdata, "dr4"

//  0000                Procedure descriptor for .text
.long   .text                   //  BeginAddress      : 0
.long   .text+0x7e4             //  EndAddress        : 2020
.long   00000000                //  ExceptionHandler  : 0
.long   00000000                //  HandlerData       : 0
.long   .text+0x724             //  PrologEnd         : 1828

