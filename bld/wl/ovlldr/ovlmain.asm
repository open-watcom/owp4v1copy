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
;* Description:  Standard Overlay Manager
;*
;*****************************************************************************


ifdef OVL_SMALL
        name    sovlmain
else
        name    lovlmain
endif

        comm    __close_ovl_file:dword

DGROUP  group   _DATA

_DATA   segment byte 'DATA' PUBLIC
_DATA   ends

_TEXT   segment dword '_OVLCODE' PUBLIC
        assume  CS:_TEXT

ifdef OVL_SMALL
        public  __SOVLLDR__
        public  __SOVLINIT__
        public  __SDBG_HOOK__
        extrn   __SOVLTINIT__:near
        extrn   __SOVLLOAD__:near
        extrn   __SOVLSETRTN__:near
        extrn   __SDBG_HANDLER__:far
        extrn   __SOVLPARINIT__:near
else
        public  __LOVLLDR__
        public  __LOVLINIT__
        public  __LDBG_HOOK__
        extrn   __LOVLTINIT__:near
        extrn   __LOVLLOAD__:near
        extrn   __LOVLSETRTN__:near
        extrn   __LDBG_HANDLER__:far
        extrn   __LOVLPARINIT__:near
endif
        extrn   __OVLPSP__:word
        extrn   __OVLCAUSE__:word
        extrn   __OVLDOPAR__:byte
        extrn   __OVLISRET__:byte
        extrn   __SaveRegs__:word
        extrn   __TopStack__:byte
        extrn   __OVLFLAGS__:word
        extrn   __CloseOvl__:far

SaveSS          dw      ?
SaveSP          dw      ?

NullHook proc   far
        ret
NullHook endp

ifdef OVL_SMALL
__SOVLINIT__ proc far
else
__LOVLINIT__ proc far
endif
        jmp     short around
                dw 2112H
ifdef OVL_SMALL
__SDBG_HOOK__   dd NullHook
                dw __SDBG_HANDLER__
else
__LDBG_HOOK__   dd NullHook
                dw __LDBG_HANDLER__
endif
around:
        mov     word ptr __OVLPSP__,ES      ; save segment address of PSP
        mov     word ptr SaveSS,SS          ; save actual SS:SP
        mov     word ptr SaveSP,SP          ; ...
        cli                                 ; set SS:SP to temporary stack
        mov     AX,CS                       ; ...
        mov     SS,AX                       ; ...
        mov     SP,offset __TopStack__      ; ...
        sti                                 ; ...
        mov     CS:__OVLFLAGS__,0           ; initialize __OVLFLAGS__
        mov     ax,3000h                    ; get dos version number
        int     21h                         ; ...
        cmp     al,3                        ; check if version 3 or greater
        jb      not_dos3                    ; ...
        or      CS:__OVLFLAGS__,2           ; set OVL_DOS3 flag
not_dos3:
ifdef OVL_SMALL
        call    __SOVLTINIT__               ; initialize overlays
        mov     BX,AX                       ; save AX register
        call    __SOVLPARINIT__             ; initialize bank stack
else
        call    __LOVLTINIT__               ; initialize overlays
        mov     BX,AX                       ; save AX register
        call    __LOVLPARINIT__             ; initialize bank stack
endif
        mov     byte ptr __OVLDOPAR__,AL    ; save status of || overlay support
        cli                                 ; set SS:SP to actual stack
        mov     SS,word ptr SaveSS          ; ...
        mov     SP,word ptr SaveSP          ; ...
        sti                                 ; ...
        push    DS                          ; save DS
        mov     ax,seg DGROUP               ; get seg of DGROUP
        mov     ds,ax                       ; ...
        mov     word ptr DS:__close_ovl_file,offset __CloseOvl__
        mov     word ptr DS:__close_ovl_file+2,CS
        pop     DS                          ; restore DS
        push    DX                          ; push actual start segment
        push    BX                          ; push actual start offset
ifdef OVL_SMALL
        jmp     dword ptr CS:__SDBG_HOOK__  ; hook into debugger if it's there
__SOVLINIT__ endp
else
        jmp     dword ptr CS:__LDBG_HOOK__  ; hook into debugger if it's there
__LOVLINIT__ endp
endif

align 4

ifdef OVL_SMALL
__SOVLLDR__ proc   near
else
__LOVLLDR__ proc   near
endif
        mov     word ptr CS:__SaveRegs__+0,AX   ; save registers
        mov     word ptr CS:__SaveRegs__+2,BP   ; ...
        mov     BP,SP                           ; peek at the stack
        mov     AX,2[BP]                        ; get cause of overlay load
        mov     CS:__OVLCAUSE__,AX              ; stash it
ifdef OVL_SMALL                                 ; ...
        mov     CS:__OVLCAUSE__+2,CS            ; ...
else
        mov     AX,4[BP]                        ; ...
        mov     CS:__OVLCAUSE__+2,AX            ; stash it
endif
        mov     byte ptr __OVLISRET__,0         ; indicate not a return
        pop     BP                              ; remove return address offset
        mov     AX,CS:[BP]                      ; get overlay to load
        pushf                                   ; save flags
ifdef OVL_SMALL
        call    __SOVLSETRTN__                  ; change the next ret address.
        call    __SOVLLOAD__                    ; load overlay
else
        call    __LOVLSETRTN__                  ; change the next ret address.
        call    __LOVLLOAD__                    ; load overlay
endif
        popf                                    ; restore flags
        add     BP,2                            ; skip overlay # when returning.
        push    BP                              ; restore return offset
        mov     BP,word ptr CS:__SaveRegs__+2   ; restore registers
        mov     AX,word ptr CS:__SaveRegs__+0   ; ...
        ret                                     ; return
ifdef OVL_SMALL
__SOVLLDR__ endp
else
__LOVLLDR__ endp
endif

_TEXT   ends

        end
