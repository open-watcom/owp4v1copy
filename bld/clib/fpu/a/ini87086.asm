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


include struct.inc
include mdef.inc

        xred    __real87, byte

        xdefp   __init_8087_emu
        xdefp   __init_80x87

ifdef __DOS__

        xdefp   __init_emu

DGROUP group _DATA
_DATA segment 'DATA'
        __init_emu dw 0
_DATA ends

endif

        modstart init8087

        xref    __init_8087_            ; in chk8087.c

__init_8087_emu proc near
        call    __init_8087_
        ret
__init_8087_emu endp

        defpe   __init_80x87            ; called from chk8087.c
        push    BP                      ; save BP
        mov     BP,SP                   ; get access to stack
        push    AX                      ; save initial control word value
ifdef __DOS__
assume DS:DGROUP
        push    ds
if _MODEL and _BIG_DATA
        mov     ax,DGROUP
        mov     ds,ax
endif
        cmp     word ptr __init_emu,0
        jz      l1
        mov     ax,word ptr [BP-2]
        call    __init_emu
l1:     pop     ds
endif
        push    AX                      ; allocate space for status word
        finit                           ; use default infinity mode
        fld1                            ; generate infinity by
        fldz                            ;   dividing 1 by 0
        fdiv                            ; ...
        fld     st                      ; form negative infinity
        fchs                            ; ...
        fcompp                          ; compare +/- infinity
        fstsw   word ptr [BP-4]         ; equal for 87/287
        fwait                           ; wait fstsw to complete
        mov     AX,[BP-4]               ; get NDP status word
        mov     AL,2                    ; assume 80287
        sahf                            ; store condition bits in flags
        jz      not387                  ; it's 287 if infinities equal
        mov     AL,3                    ; indicate 80387
not387: finit                           ; re-initialize the 8087
        fldcw   word ptr [BP-2]         ; .(affine,round nearest,64-bit prec)
        mov     SP,BP                   ; clean up stack
        pop     BP                      ; restore BP
        ret                             ; return
        endproc __init_80x87

        endmod
        end
