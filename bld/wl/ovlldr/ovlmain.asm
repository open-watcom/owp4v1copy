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


ifdef OVL_SMALL

    name        sovlmain

else

    name        lovlmain

endif

    OVL_DOS3 = 2

    COMM        __close_ovl_file:dword

DGROUP  group   _DATA

_DATA   segment byte 'DATA' PUBLIC
_DATA   ends

_TEXT   segment para '_OVLCODE' PUBLIC
        assume  CS:_TEXT

ifdef OVL_SMALL
    EXTRN       __SDBG_HANDLER__:near
    EXTRN       __SOVLTINIT__:near
    EXTRN       __SOVLPARINIT__:near
    EXTRN       __SOVLSETRTN__:near
    EXTRN       __SOVLLOAD__:near
else
    EXTRN       __LDBG_HANDLER__:near
    EXTRN       __LOVLTINIT__:near
    EXTRN       __LOVLPARINIT__:near
    EXTRN       __LOVLSETRTN__:near
    EXTRN       __LOVLLOAD__:near
endif
    EXTRN       __OVLPSP__:word
    EXTRN       __TopStack__:BYTE
    EXTRN       __OVLFLAGS__:word
    EXTRN       __OVLDOPAR__:BYTE
    EXTRN       __CloseOvl__:far
    EXTRN       __SaveRegs__:word
    EXTRN       __OVLCAUSE__:word
    EXTRN       __OVLISRET__:BYTE

caller_ss       dw ?
caller_sp       dw ?

NullHook:       retf

;
;   void __?OVLINIT__( DOS EXE startup parms )
;

ifdef OVL_SMALL

__SOVLINIT__ proc far public

else

__LOVLINIT__ proc far public

endif

        jmp         short around
        dw          2112h

ifdef OVL_SMALL

PUBLIC  __SDBG_HOOK__
__SDBG_HOOK__   DD        NullHook
                DW        offset _TEXT:__SDBG_HANDLER__

else

PUBLIC  __LDBG_HOOK__
__LDBG_HOOK__   DD        NullHook
                DW        offset _TEXT:__LDBG_HANDLER__

endif

around:
        mov     word ptr cs:__OVLPSP__,es
        mov     word ptr cs:caller_ss,ss
        mov     word ptr cs:caller_sp,sp
        cli
        mov     ax,cs
        mov     ss,ax
        mov     sp,offset _TEXT:__TopStack__
        sti
        mov     word ptr cs:__OVLFLAGS__,0
        mov     ax,3000H
        int     21H
        cmp     al,3
        jb      L$5
        or      word ptr cs:__OVLFLAGS__,OVL_DOS3
L$5:

ifdef OVL_SMALL

        call    near ptr __SOVLTINIT__
        mov     bx,ax
        call    near ptr __SOVLPARINIT__

else

        call    near ptr __LOVLTINIT__
        mov     bx,ax
        call    near ptr __LOVLPARINIT__

endif

        mov     byte ptr cs:__OVLDOPAR__,al
        cli
        mov     ss,word ptr cs:caller_ss
        mov     sp,word ptr cs:caller_sp
        sti
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     word ptr __close_ovl_file,offset _TEXT:__CloseOvl__
        mov     word ptr __close_ovl_file+2,cs
        pop     ds
        push    dx
        push    bx

ifdef OVL_SMALL

        jmp     dword ptr cs:__SDBG_HOOK__

__SOVLINIT__    endp

else

        jmp     dword ptr cs:__LDBG_HOOK__

__LOVLINIT__    endp

endif

;
;   void __?OVLLDR__( void )
;

ALIGN 4

ifdef OVL_SMALL

__SOVLLDR__ proc near public

else

__LOVLLDR__ proc near public

endif

        mov     word ptr cs:__SaveRegs__,ax
        mov     word ptr cs:__SaveRegs__+2,bp
        mov     bp,sp
        mov     ax,word ptr 2[bp]
        mov     word ptr cs:__OVLCAUSE__,ax
        mov     ax,word ptr 4[bp]
        mov     word ptr cs:__OVLCAUSE__+2,ax
        mov     byte ptr cs:__OVLISRET__,0
        pop     bp
        mov     ax,word ptr cs:[bp]
        pushf

ifdef OVL_SMALL

        call    near ptr __SOVLSETRTN__
        call    near ptr __SOVLLOAD__

else

        call    near ptr __LOVLSETRTN__
        call    near ptr __LOVLLOAD__

endif

        popf
        add     bp,2
        push    bp
        mov     bp,word ptr cs:__SaveRegs__+2
        mov     ax,word ptr cs:__SaveRegs__
        ret

ifdef OVL_SMALL

__SOVLLDR__     endp

else

__LOVLLDR__     endp

endif

_TEXT   ends

END
