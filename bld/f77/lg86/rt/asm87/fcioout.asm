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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     FORTRAN-77 output routines  for 8086 based machines
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include parmtype.inc
include xfflags.inc

        xref   LimError_

        extrn   IORegs : near

        fmodstart       fcioout

        dataseg

        xred    "C",IORslt,  word

        enddata


fcode   RT_ENDIO                        ; terminate I/O statement
        sub     AX,AX                   ; indicate end of I/O statement
        jmp     Prt                     ; output a CR/LF
efcode  RT_ENDIO


fcode   RT_OUT_LOG1                     ; output logical*1
        mov     AX,PT_LOG1              ; indicate logical*1
        hop     OutLog                  ; output logical
efcode  RT_OUT_LOG1


fcode   RT_OUT_LOG4                     ; output logical*1
        mov     AX,PT_LOG4              ; indicate logical*1
        hop     OutLog
efcode  RT_OUT_LOG4


fcode   RT_OUT_INT1                     ; output logical*1
        mov     AX,PT_INT1              ; indicate logical*1
        hop     OutLog                  ; output logical
efcode  RT_OUT_INT1


fcode   RT_OUT_INT2                     ; output logical*1
        mov     AX,PT_INT2              ; indicate logical*1
        hop     OutLog                  ; output logical
efcode  RT_OUT_INT2


fcode   RT_OUT_INT4                     ; output logical*1
        mov     AX,PT_INT4              ; indicate logical*1
;;;;;;;;hop     OutLog
efcode  RT_OUT_INT4


defn    OutLog
        pop     word ptr SS:IORslt+0    ; get value
        pop     word ptr SS:IORslt+2    ; ...
        jmp     Prt                     ; output value
endproc OutLog


fcode   SIMPLE_OUT_R4                   ; output real*4 variable
        mov     CX,PT_REAL              ; indicate integer*4
        hop     SimpOut                 ; output value
efcode  SIMPLE_OUT_R4


fcode   SIMPLE_OUT_I4                   ; output integer*4 variable
        mov     CX,PT_INT4              ; set type to integer*4
;;;;;;;;hop     SimpOut
efcode  SIMPLE_OUT_I4


defn    SimpOut
        getword AX                      ; get address of variable
        xchg    AX,SI                   ; ... in SI and save F-Code pointer
        push    SS                      ; point to IORslt_
        pop     ES                      ; ...
        mov     DI,offset DGROUP:IORslt ; ...
        movsw                           ; put value in IORslt_
        movsw                           ; ...
        xchg    AX,SI                   ; restore F-Code pointer
        mov     AX,CX                   ; get type into AX
        hop     Prt                     ; output value
endproc SimpOut


fcode   RT_OUT_REAL                     ; output real*4
        mov     AX,PT_REAL              ; indicate real*4
        fstp    dword ptr SS:IORslt     ; get value
        hop     PrtF                    ; output value
efcode  RT_OUT_REAL


fcode   RT_OUT_DBLE                     ; output real*8
        mov     AX,PT_DBLE              ; indicate real*8
        fstp    qword ptr SS:IORslt     ; get value
        hop     PrtF                    ; output value
efcode  RT_OUT_DBLE


fcode   RT_OUT_XTND                     ; output real*16
        mov     AX,PT_XTND              ; indicate real*16
        fstp    _TBYTE ptr SS:IORslt    ; get value
        hop     PrtF                    ; output value
efcode  RT_OUT_XTND


fcode   SIMPLE_OUT_R8                   ; output real*8 variable
        getword AX                      ; get address of variable
        xchg    AX,SI                   ; ... in SI and save F-Code pointer
        push    SS                      ; point to IORslt
        pop     ES                      ; ...
        mov     DI,offset DGROUP:IORslt ; ...
        movsw                           ; put value in IORslt
        movsw                           ; ...
        movsw                           ; ...
        movsw                           ; ...
        xchg    AX,SI                   ; restore F-Code pointer
        mov     AX,PT_DBLE              ; get type
        hop     Prt                     ; output value
efcode  SIMPLE_OUT_R8


fcode   RT_OUT_CPLX                     ; output complex*8
        mov     AX,PT_CPLX              ; indicate complex*8
        fstp    dword ptr SS:IORslt     ; get value
        fstp    dword ptr SS:IORslt+4   ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_CPLX


fcode   RT_OUT_DBCX                     ; output complex*16
        mov     AX,PT_DBCX              ; indicate complex*16
        fstp    qword ptr SS:IORslt     ; get value
        fstp    qword ptr SS:IORslt+8   ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_DBCX


fcode   RT_OUT_XTCX                     ; output complex*32
        mov     AX,PT_XTCX              ; indicate complex*32
        fstp    _TBYTE ptr SS:IORslt    ; get value
        fstp    _TBYTE ptr SS:IORslt+16 ; . . .
        hop     PrtF                    ; output value
efcode  RT_OUT_XTCX


fcode   RT_OUT_CHAR                     ; output character string
        mov     AX,PT_CHAR              ; indicate character string
        pop     word ptr SS:IORslt+0    ; get SCB
        pop     word ptr SS:IORslt+2    ; . . .
        pop     word ptr SS:IORslt+4    ; . . .
        hop     Prt                     ; output a character string
efcode  RT_OUT_CHAR


fcode   RT_END_DATA_VARS                ; end of data set in DATA statement
        mov     AX,PT_ENDDATA           ; indicate end of data set
;;;;;;;;hop     Prt
efcode  RT_END_DATA_VARS


defn    PrtF
        fwait                           ; check for pending 8087 exceptions
defn    Prt                             ; put an n-byte item into IORslt
        exit_fcode                      ; switch to run-time environment
        test    SS:__XcptFlags,XF_LIMIT_ERR; check for limit error
        jne     out_lim                 ; report error if there is one
OK:     jmp     IORegs                  ; return to caller of iotype
endproc Prt
endproc PrtF


defn    out_lim                         ; report a limit error
        push    AX                      ; save AX
        docall  LimError_               ; report the error
        pop     AX                      ; restore AX
        hop     OK                      ; continue the operation
endproc out_lim


        fmodend
        end
