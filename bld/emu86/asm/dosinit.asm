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
;* Description:  routines for 16-bit DOS FPU emulator control
;*
;*****************************************************************************

; !!!!! must be compiled with -fpi option !!!!!

include mdef.inc

DGROUP  group   _DATA
        assume  ds:DGROUP

_DATA   segment word public 'DATA'
        extrn   __8087cw        : word
        extrn   __init_emu      : word         ; in clib/fpu/a/ini87086.asm
        extrn   __old_8087_emu  : word         ; in clib/fpu/a/old87.asm
        extrn   __dos_emu_fstcw : word         ; in clib/fpu/c/cntrl87.asm
        extrn   __dos_emu_fldcw : word         ; in clib/fpu/c/cntrl87.asm
_DATA   ends


_TEXT segment word public 'CODE'

        xdefp   __dos_init_emu
        xdefp   __dos_fini_emu

___init_emu proc near
        push    bp
        mov     bp,sp
        push    ax
        finit                         ; initialize the '8087' emulator
        fldcw   word ptr [bp - 2]     ; load control word
        mov     sp,bp
        mov     ax,3
        pop     bp
        ret
___init_emu endp

___init_old_emu proc near
        fldz                          ; put 8087 into 4 empty / 4 full state
        fldz                          ; ...
        fldz                          ; ...
        fldz                          ; ...
        ret
___init_old_emu endp

___emu_fstcw proc near
        xchg   ax,bp
        fstcw  word ptr [bp]
        fwait
        xchg   ax,bp
        ret
___emu_fstcw endp

___emu_fldcw proc near
        xchg   ax,bp
        fldcw  word ptr [bp]
        xchg   ax,bp
        ret
___emu_fldcw endp

__dos_init_emu proc near
        mov   ax,__8087cw
        call  ___init_emu
        mov   word ptr __init_emu, ___init_emu
        mov   word ptr __old_8087_emu, ___init_old_emu
        mov   word ptr __dos_emu_fstcw, ___emu_fstcw
        mov   word ptr __dos_emu_fldcw, ___emu_fldcw
        ret
__dos_init_emu endp

__dos_fini_emu proc near
        xor     ax,ax
        mov     word ptr __init_emu, ax
        mov     word ptr __old_8087_emu, ax
        mov     word ptr __dos_emu_fstcw, ax
        mov     word ptr __dos_emu_fldcw, ax
        ret
__dos_fini_emu endp

_TEXT ends

        end
