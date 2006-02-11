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
;
include mdef.inc
include struct.inc
include int21.inc

        xref    __doserror_
        modstart doscreat

        defp    _dos_creat
        xdefp       "C",_dos_creat
;
;       unsigned _dos_creat( char *path, unsigned attr, int *handle )
;
ifdef __STACK__
        push    EBX
        push    EDX
        mov     EAX,12[ESP]     ; get path
        mov     EDX,16[ESP]     ; attr
        mov     EBX,20[ESP]     ; handle
endif
        push    ECX             ; save CX
ifdef __WATCOM_LFN__
        push    ESI
endif
        mov     ECX,EDX         ; get attribute
ifdef __WATCOM_LFN__
        mov     ESI,EAX         ; SI = path in lfn
endif
        mov     EDX,EAX         ; DX = path
ifdef __WATCOM_LFN__
        mov     DX,12h          ; create or truncate file (lfn)
        mov     AX,716Ch        ; create/open file
        push    ECX             ; save attributes
        int21h
        pop     ECX             ; restore ECX (attributes arg)
        jc      nonlfn          ; if error use non-lfn
        cmp     AX,7100h        ; check for lfn support
        jnz     creathandle
nonlfn:
        mov     EDX,ESI         ; prepare for sfn function
endif
        mov     AH,3Ch          ; creat file
        int21h                  ; ...
creathandle:
        _if     nc              ; if no error
          mov   [EBX],EAX       ; - store handle
        _endif                  ; endif
        call    __doserror_     ; set return code
ifdef __WATCOM_LFN__
        pop     ESI
endif
        pop     ECX             ; restore CX
ifdef __STACK__
        pop     EDX
        pop     EBX
endif
        ret                     ; return to caller
        endproc _dos_creat

        defp    _dos_creatnew
        xdefp   "C",_dos_creatnew
;
;       unsigned _dos_creatnew( char *path, unsigned attr, int *handle )
;
ifdef __STACK__
        push    EBX
        push    EDX
        mov     EAX,12[ESP]     ; get path
        mov     EDX,16[ESP]     ; attr
        mov     EBX,20[ESP]     ; handle
endif
        push    ECX             ; save CX
ifdef __WATCOM_LFN__
        push    ESI
endif
        mov     ECX,EDX         ; get attribute
ifdef __WATCOM_LFN__
        mov     ESI,EAX         ; SI = path in LFN
endif
        mov     EDX,EAX         ; DX = path
ifdef __WATCOM_LFN__
        mov     DX,10h          ; create but not truncate in lfn
        mov     AX,716Ch        ; create / open file (lfn)
        push    CX              ; save CX (attributes)
        int21h
        pop     CX              ; restore CX (attributes)
        jc      old             ; if error use non-lfn
        cmp     AX,7100h        ; check for lfn support
        jnz     creatnewhandle  ; return
old:
        mov     DX,SI           ; DX = path in sfn
endif
        mov     AH,5Bh          ; create a new file
        int21h                  ; ...
creatnewhandle:
        _if     nc              ; if no error
          mov   [EBX],EAX       ; - store handle
        _endif                  ; endif
        call    __doserror_     ; set return code
ifdef __WATCOM_LFN__
        pop     ESI
endif
        pop     ECX             ; restore CX
ifdef __STACK__
        pop     EDX
        pop     EBX
endif
        ret                     ; return to caller
        endproc _dos_creatnew

        endmod
        end
