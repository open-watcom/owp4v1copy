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
; FCDRVR        : Startup and shutdown code for F-Code interpreter
;

include fcdef.inc

        extrn   Spawn_          : near

        extrn   _PgmCall        : dword
if _MATH eq _8087
        extrn   AuxTop          : dword

        dataseg

AUX_STACK_SIZE = 4096

AuxStack db AUX_STACK_SIZE dup(?) ; Auxilliary stack grows towards higher addresses

        enddata
endif

        fmodstart    fcdrvr


defp    CallPgm_                ; start program execution
        enter_fpu
        pushad                  ; save registers
if _MATH eq _8087
        lea     eax,AuxStack    ; get bottom of CPU stack
        add     eax,AUX_STACK_SIZE
        mov     AuxTop,eax      ; set top of Auxiliary FPU stack
endif
        xor     ebp,ebp         ; initial data offset is 0 for PgmCall
        lea     esi,_PgmCall    ; set to start the program
;;;;;;;;hop     RT_RTN_COUNT
endproc CallPgm_


fcode   RT_RTN_COUNT            ; a dummy place holder
if _MATH eq _EMULATION
dfcode  LOAD_ENTRIES            ; these are nops for non-8087 version
dfcode  DUMP_ENTRIES
dfcode  FLUSH_ENTRIES
endif
        next                    ; execute the program
efcode  RT_RTN_COUNT


        xdefp   Program_
defp    Program_
        mov     eax,offset CallPgm_
        call    Spawn_
        exit_fpu                ; in case we Suicide() because of an error
        ret
endproc Program_


fcode   RT_STOPPGM
        popad                   ; restore registers
        exit_fpu
        ret                     ; return
efcode  RT_STOPPGM

        fmodend

        end
