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
; FCMOVE        : Move data in memory
;

.8087

include fcdef.inc

        extrn   UndChk : near

        fmodstart       fcmove

fcode   MOVE_LL_I4
dfcode  MOVE_LL_R4
        getdw   edi             ; get address of src operand
        add     edi,ebp
DoMv4:  getdw   eax             ; get address of dst operand
        _fwait                  ; wait for the 8087
        mov     ecx,[edi]       ; get data
        mov     [eax+ebp],ecx   ; and move it
        next                    ; execute next FCode
efcode  MOVE_LL_I4


fcode   MOVE_ULL_I4
dfcode  MOVE_ULL_R4
        mov     ecx,4           ; set length of operand
        mov     ebx,offset32 DoMv4; set return address
        jmp     UndChk          ; do undefined checking
efcode  MOVE_ULL_I4


fcode   MOVE_ULL_R8
        mov     ecx,8           ; set length of operand
        mov     ebx,offset32 DoMv8; set return address
        jmp     UndChk          ; do undefined checking
efcode  MOVE_ULL_R8


fcode   MOVE_LL_R8
        getdw   edi             ; get address of src operand
        add     edi,ebp
DoMv8:
        getdw   eax             ; get address of dst operand
        _fwait                  ; wait for the 8087
        mov     ecx,[edi]       ; get low dword
        mov     edx,4[edi]      ; get high dword
        mov     [eax+ebp],ecx   ; store low
        mov     4[eax+ebp],edx  ; store high
        next
efcode  MOVE_LL_R8


fcode   MOVE_IL_R8
        getdw   eax             ; get address of src operand
        mov     edi,[eax+ebp]   ; get src addr
        hop     DoMv8
efcode  MOVE_IL_R8

        fmodend
        end
