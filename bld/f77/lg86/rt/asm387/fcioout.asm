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
; FCIOOUT       : output values
;

.8087

include fcdef.inc
include parmtype.inc

        xref    ChkLimitErr_

        extrn   IORegs : near

        xred    "C",IORslt,  dword

        fmodstart fcioout


fcode   RT_ENDIO                        ; terminate I/O statement
        sub     ebx,ebx                 ; indicate end of I/O statement
        jmp     Prt
efcode  RT_ENDIO


fcode   RT_OUT_LOG1
        mov     ebx,PT_LOG1
        hop     OutLog
efcode  RT_OUT_LOG1


fcode   RT_OUT_LOG4
        mov     ebx,PT_LOG4
        hop     OutLog
efcode  RT_OUT_LOG4


fcode   RT_OUT_INT1
        mov     ebx,PT_INT1
        hop     OutLog
efcode  RT_OUT_INT1


fcode   RT_OUT_INT2
        mov     ebx,PT_INT2
        hop     OutLog
efcode  RT_OUT_INT2


fcode   RT_OUT_INT4
        mov     ebx,PT_INT4
;;;;;;;;hop     OutLog
efcode  RT_OUT_INT4


defn    OutLog
        pop     dword ptr IORslt        ; get value
        jmp     Prt                     ; output value
endproc OutLog


fcode   SIMPLE_OUT_R4                   ; output real*4 variable
        mov     ecx,PT_REAL             ; indicate real*4
        hop     SimpOut                 ; output value
efcode  SIMPLE_OUT_R4


fcode   SIMPLE_OUT_I4                   ; output integer*4 variable
        mov     ecx,PT_INT4             ; set type to integer*4
;;;;;;;;hop     SimpOut
efcode  SIMPLE_OUT_I4


defn    SimpOut
        getdw   eax                     ; get address of variable
        mov     edx,[eax+ebp]           ; get value
        mov     dword ptr IORslt,edx    ; store value
        mov     ebx,ecx                 ; get type into ebx
        jmp     Prt                     ; output value
endproc SimpOut


fcode   RT_OUT_REAL                     ; output real*4
        mov     ebx,PT_REAL             ; indicate real*4
        fstp    dword ptr IORslt        ; get value
        jmp     PrtF                    ; output value
efcode  RT_OUT_REAL


fcode   RT_OUT_DBLE                     ; output real*8
        mov     ebx,PT_DBLE             ; indicate real*8
        fstp    qword ptr IORslt        ; get value
        hop     PrtF                    ; output value
efcode  RT_OUT_DBLE


fcode   RT_OUT_XTND                     ; output real*8
        mov     ebx,PT_XTND             ; indicate real*8
        fstp    _TBYTE ptr IORslt       ; get value
        hop     PrtF                    ; output value
efcode  RT_OUT_XTND


fcode   SIMPLE_OUT_R8                   ; output real*8 variable
        getdw   eax                     ; get address of variable
        mov     edx,[eax+ebp]           ; get value
        mov     eax,4[eax+ebp]          ; ...
        mov     ebx,PT_DBLE             ; get type
        mov     dword ptr IORslt,edx    ; store value
        mov     dword ptr IORslt+4,eax  ; ...
        hop     Prt                     ; output value
efcode  SIMPLE_OUT_R8


fcode   RT_OUT_CPLX                     ; output complex*8
        mov     ebx,PT_CPLX             ; indicate complex*8
        fstp    dword ptr IORslt        ; get value
        fstp    dword ptr IORslt+4      ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_CPLX


fcode   RT_OUT_DBCX                     ; output complex*16
        mov     ebx,PT_DBCX             ; indicate complex*16
        fstp    qword ptr IORslt        ; get value
        fstp    qword ptr IORslt+8      ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_DBCX


fcode   RT_OUT_XTCX                     ; output complex*16
        mov     ebx,PT_XTCX             ; indicate complex*16
        fstp    _TBYTE ptr IORslt       ; get value
        fstp    _TBYTE ptr IORslt+16    ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_XTCX


fcode   RT_OUT_CHAR                     ; output character string
        mov     ebx,PT_CHAR             ; indicate character string
        pop     dword ptr IORslt        ; get SCB
        pop     dword ptr IORslt+4      ; . . .
        hop     Prt                     ; output a character string
efcode  RT_OUT_CHAR


fcode   RT_END_DATA_VARS                ; end of data set in DATA statement
        mov     ebx,PT_ENDDATA          ; indicate end of data set
;;;;;;;;hop     Prt
efcode  RT_END_DATA_VARS


defn    PrtF
        fwait                           ; check for pending 8087 exceptions
defn    Prt                             ; put an n-byte item into IORslt
        exit_fcode                      ; switch to run-time environment
        docall  ChkLimitErr_            ; check for limit error
        mov     eax,ebx                 ; get type in eax
        jmp     IORegs                  ; return to caller of iotype
endproc Prt
endproc PrtF

        fmodend
        end
