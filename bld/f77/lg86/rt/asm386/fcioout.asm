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
; FCIOOUT       : output routines
;

include fcdef.inc
include parmtype.inc

        xref    ChkLimitErr_

        extrn   IORegs  : near
        extrn   _IORslt : dword

        fmodstart fcioout

DataSize db     0,2,2,2,2,2,2,4,8,4,8,16,4,0,0,0; number of dwords on stack
                                                ; for each data type

fcode   RT_ENDIO                ; terminate I/O statement
        sub     ebx,ebx         ; indicate end of I/O statement
        hop     Prt             ; output a CR/LF
efcode  RT_ENDIO


fcode   RT_OUT_LOG1             ; output logical*1
        mov     ebx,PT_LOG1     ; indicate logical*1
        hop     Prt             ; output 2 byte value
efcode  RT_OUT_LOG1


fcode   RT_OUT_LOG4             ; output logical*4
        mov     ebx,PT_LOG4     ; indicate logical*4
        hop     Prt             ; output a 2 byte value
efcode  RT_OUT_LOG4


fcode   RT_OUT_INT1             ; output integer*1
        mov     ebx,PT_INT1     ; indicate integer*1
        hop     Prt             ; output 4 byte value
efcode  RT_OUT_INT1


fcode   RT_OUT_INT2             ; output integer*2
        mov     ebx,PT_INT2     ; indicate integer*2
        hop     Prt             ; output 4 byte value
efcode  RT_OUT_INT2


fcode   RT_OUT_INT4             ; output integer*4
        mov     ebx,PT_INT4     ; indicate integer*4
        hop     Prt             ; output 4 byte value
efcode  RT_OUT_INT4


fcode   SIMPLE_OUT_I4           ; output integer*4 variable
        getdw   eax             ; get address of variable
        mov     ebx,PT_INT4     ; indicate integer*4
        push    [eax+ebp]       ; push variable onto stack
        hop     Prt             ; output 4 byte value
efcode  SIMPLE_OUT_I4


fcode   RT_OUT_REAL             ; output real*4
        mov     ebx,PT_REAL     ; indicate real*4
        hop     Prt             ; output 4 byte value
efcode  RT_OUT_REAL


fcode   SIMPLE_OUT_R4           ; output real*4 variable
        getdw   eax             ; get address of variable
        mov     ebx,PT_REAL     ; indicate real*4
        push    [eax+ebp]       ; push variable onto stack
        hop     Prt             ; output 4 byte value
efcode  SIMPLE_OUT_R4


fcode   RT_OUT_DBLE             ; output real*8
        mov     ebx,PT_DBLE     ; indicate real*8
        hop     Prt             ; output 8 byte value
efcode  RT_OUT_DBLE


fcode   RT_OUT_XTND             ; output real*16
        mov     ebx,PT_DBLE     ; indicate real*16
        hop     Prt             ; output 8 byte value
efcode  RT_OUT_XTND


fcode   SIMPLE_OUT_R8           ; output real*8 variable
        getdw   eax             ; get address of variable
        mov     ebx,PT_DBLE     ; indicate real*8
        push    4[eax+ebp]      ; push variable onto stack
        push    0[eax+ebp]      ; push variable onto stack
        hop     Prt             ; output 4 byte value
efcode  SIMPLE_OUT_R8


fcode   RT_OUT_CPLX             ; output complex*8
        mov     ebx,PT_CPLX     ; indicate complex*8
        hop     Prt             ; output 8 byte value
efcode  RT_OUT_CPLX


fcode   RT_OUT_DBCX             ; output complex*16
        mov     ebx,PT_DBCX     ; indicate complex*16
        hop     Prt             ; output a 16 byte value
efcode  RT_OUT_DBCX


fcode   RT_OUT_XTCX             ; output complex*32
        mov     ebx,PT_XTCX     ; indicate complex*32
        hop     Prt             ; output a 32 byte value
efcode  RT_OUT_XTCX


fcode   RT_OUT_CHAR             ; output character string
        mov     ebx,PT_CHAR     ; indicate character string
        hop     Prt             ; output a character string
efcode  RT_OUT_CHAR


fcode   RT_END_DATA_VARS        ; end of data set in DATA statement
        mov     ebx,PT_ENDDATA  ; indicate end of data set
;;;;;;;;hop     Prt
efcode  RT_END_DATA_VARS


defn    Prt                             ; put an n-byte item into IORslt
        exit_fcode                      ; switch to run-time environment
        docall  ChkLimitErr_            ; check for limit error
        movzx   ecx,DataSize[ebx]       ; get size of data
        mov     edi,offset32 _IORslt    ; get a pointer to IORslt
        jcxz    done                    ; jump around pops if nothing to do
repeat:                                 ; loop
        pop     ax                      ; - get value
        stosw                           ; - store in IORslt and inc pointer
        loop    repeat                  ; until done
done:   mov     eax,ebx                 ; return type of io item
        jmp     IORegs
endproc Prt

        fmodend
        end
