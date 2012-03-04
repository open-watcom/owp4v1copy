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
;* Description:  C library setjmp/longjmp support for 386 segmented mode
;*
;*****************************************************************************

.386p
include mdef.inc
include struct.inc

        codeptr         "C",__longjmp_handler

        modstart        setjmp

        xdefp   "C",_setjmp
        defpe   _setjmp
        push    es
        mov     es,edx
        mov     es:[eax],ebx       ; save registers
        mov     es:[eax+4],ecx
        mov     es:[eax+8],edx
        mov     es:[eax+12],esi
        mov     es:[eax+16],edi
        mov     es:[eax+20],ebp
        pop     es:[eax+24]         ; get return address
        mov     es:[eax+28],esp
        push    es:[eax+24]         ; push return address back on stack
        mov     es:[eax+32],es      ; save segment registers
        mov     es:[eax+34],ds
        mov     es:[eax+36],cs
        mov     es:[eax+38],fs
        mov     es:[eax+40],gs
        mov     es:[eax+42],ss
        pop     es
        sub     eax,eax         ; return 0
        ret                     ; return
_setjmp endp

;
;       There is a pragma for longjmp, saying that it aborts, therefore
;       the code generator does a jmp to here as opposed to a call.

        xdefp   "C",longjmp
        defpe   longjmp
ifdef __STACK__
        pop     eax             ; get offset of jmp_buf
        pop     es              ; get segment of jmp_buf
        pop     edx             ; get return code
endif
        ; Warning, warning!
        ; the profiler knows about the stack frame that longjmp generates.
        ; do not change these 3 instructions without also changing the findLongJmpStack
        ; pragma in profilog.c
        ;
        push    edx
        push    es
        push    eax                ; save jmp_buf & retval in safe place
        mov     ebp,esp
        ;
        ; end warning
        ;
        push    es
        push    eax                ; save parm regs
        push    edx
        mov     dx,es:[eax+42]     ; setup old ss:esp as a parm
        mov     eax,es:[eax+28]
        call    __longjmp_handler  ; call handler
        pop     edx                ; restore parm regs
        pop     eax
        pop     es

        mov     ss,es:[eax+42]     ; load old ss:esp
        mov     esp,es:[eax+28]    ; ...
        push    es:[eax+24]        ; push saved eip (our return address)
        or      edx,edx            ; if return code is 0
        _if     e                  ; then
          inc   edx                ; - set it to 1
        _endif                     ; endif
        push    edx                ; save return code
        mov     ebx,es:[eax]       ; load up the saved registers
        mov     ecx,es:[eax+4]
        mov     esi,es:[eax+12]
        mov     edi,es:[eax+16]
        mov     ebp,es:[eax+20]
        mov     dx,es:[eax+38]
        verr    dx                 ; verify segment
        _if     ne
          sub   edx,edx
        _endif
        mov     fs,dx
        mov     dx,es:[eax+40]
        verr    dx                 ; verify segment
        _if     ne
          sub   edx,edx
        _endif
        mov     gs,dx
        mov     edx,es:[eax+8]
        mov     ds,es:[eax+34]
        mov     es,es:[eax+32]
        pop     eax                ; get return code
        ret                        ; return to point following setjmp call
longjmp endp

_TEXT   ends
        end
