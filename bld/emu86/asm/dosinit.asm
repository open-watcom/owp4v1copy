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

include mdef.inc
;include struct.inc

        modstart    initemu

        xdefp   ___init_emu
        defn    ___init_emu
        push    bp
        mov     bp,sp
        push    ax
        finit                         ; initialize the '8087' emulator
        fldcw   word ptr [bp - 2]     ; load control word
        mov     sp,bp
        mov     ax,3
        pop     bp
        ret
        endproc ___init_emu

        xdefp   ___init_old_emu
        defn    ___init_old_emu
        fldz                          ; put 8087 into 4 empty / 4 full state
        fldz                          ; ...
        fldz                          ; ...
        fldz                          ; ...
        ret
        endproc ___init_old_emu

        xdefp   ___emu_fstcw
        defn    ___emu_fstcw
        xchg   ax,bp
        fstcw  word ptr [bp]
        fwait
        xchg   ax,bp
        ret
        endproc ___emu_fstcw

        xdefp   ___emu_fldcw
        defn    ___emu_fldcw
        xchg   ax,bp
        fldcw  word ptr [bp]
        fwait
        xchg   ax,bp
        ret
        endproc ___emu_fldcw

        endmod
        end
