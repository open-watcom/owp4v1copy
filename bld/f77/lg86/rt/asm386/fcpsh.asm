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
; FCPSH         : routines to push memory onto stack
;

include fcdef.inc

        fmodstart       fcpsh


fcode   PUSH_INT1               ; push integer*1
dfcode  PUSH_LOG1               ; push logical*1
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     VAL_INT1
efcode  PUSH_INT1


fcode   VAL_INT1
dfcode  VAL_LOG1
        movsx   eax,byte ptr [edi]; get byte value to be pushed
        push    eax             ; push value
        next
efcode  VAL_INT1


fcode   PUSH_INT2               ; push integer*2
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     VAL_INT2
efcode  PUSH_INT2


fcode   VAL_INT2
        movsx   eax,word ptr [edi]; get word value to be pushed
        push    eax             ; push value
        next
efcode  VAL_INT2


fcode   PUSH_LOG4               ; push logical*4
dfcode  PUSH_INT4               ; push integer*4
dfcode  PUSH_REAL4              ; push real*4
        getdw   eax             ; get addr of variable
        push    [eax+ebp]       ; push value
        next
efcode  PUSH_LOG4


fcode   PUSH_IND_LOG1           ; push logical*1 indirect
dfcode  PUSH_IND_INT1           ; push integer*1 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_INT1        ; push value
efcode  PUSH_IND_LOG1


fcode   PUSH_IND_INT2           ; push integer*2 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_INT2        ; push value
efcode  PUSH_IND_INT2


fcode   PUSH_IND_LOG4           ; push logical*4 indirect
dfcode  PUSH_IND_INT4           ; push integer*4 indirect
dfcode  PUSH_IND_REAL4          ; push real*4 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_INT4        ; push value
efcode  PUSH_IND_LOG4


fcode   PUSH_IND_REAL8          ; push real*8 indirect
dfcode  PUSH_IND_CPLX8          ; push complex*8 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_REAL8       ; push value
efcode  PUSH_IND_REAL8


fcode   PUSH_REAL8              ; push real*8
dfcode  PUSH_CPLX8              ; push complex*8
        getdw   eax             ; get addr of variable
        push    4[eax+ebp]      ; push value
        push    [eax+ebp]       ; ...
        next
efcode  PUSH_REAL8


fcode   PUSH_IND_REAL16         ; push real*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
;;;;;;;;hop     VAL_REAL16      ; push value
efcode  PUSH_IND_REAL16


fcode   VAL_REAL16
        push    12[edi]         ; push real*16
        push    8[edi]          ; ...
        push    4[edi]          ; ...
        push    [edi]           ; ...
        next                    ; execute next F-Code
efcode  VAL_REAL16


fcode   PUSH_REAL16                     ; push real*16
        getdw   eax                     ; get addr of variable
        push    12[eax+ebp]             ; push value
        push    8[eax+ebp]              ; ...
        push    4[eax+ebp]              ; ...
        push    [eax+ebp]               ; ...
        next
efcode  PUSH_REAL16


fcode   PUSH_IND_CPLX16         ; push complex*16 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_CPLX16      ; push value
efcode  PUSH_IND_CPLX16

fcode   PUSH_CPLX16             ; push complex*16
        getdw   edi             ; get addr of variable
        add     edi,ebp
        hop     VAL_CPLX16
efcode  PUSH_CPLX16

fcode   PUSH_IND_CPLX32         ; push complex*32 indirect
        getdw   eax             ; get addr of pointer
        mov     edi,[eax+ebp]   ; get addr of value
        hop     VAL_CPLX32      ; push value
efcode  PUSH_IND_CPLX32

fcode   PUSH_CPLX32             ; push complex*32
        getdw   edi             ; get addr of variable
        add     edi,ebp
;;;;;;;;hop     VAL_CPLX32
efcode  PUSH_CPLX32


fcode   VAL_CPLX32
        push    28[edi]         ; push value
        push    24[edi]         ; ...
        push    20[edi]         ; ...
        push    16[edi]         ; ...
;;;;;;;;hop     VAL_CPLX16
efcode  VAL_CPLX32


fcode   VAL_CPLX16
        push    12[edi]         ; push value
        push    8[edi]          ; ...
;;;;;;;;hop     VAL_CPLX8
efcode  VAL_CPLX16


fcode   VAL_CPLX8
dfcode  VAL_REAL8
        push    4[edi]          ; ...
;;;;;;;;hop     VAL_LOG4
efcode  VAL_CPLX8


fcode   VAL_LOG4
dfcode  VAL_INT4
dfcode  VAL_REAL4
        push    [edi]           ; ...
        next                    ; execute next F-Code
efcode  VAL_LOG4

        fmodend
        end
