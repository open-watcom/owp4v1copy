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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>
;<>
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc

        extrn   UndChk : near

        fmodstart       fcmove

fcode   MOVE_LL_I4
dfcode  MOVE_LL_R4
        getword BX              ; get address of src operand
DoMv4:  getword DI              ; get address of dst operand
        mov     AX,DS           ; get DS
        xchg    BX,SI           ; get src addr, and save SI
        mov     ES,AX           ; set ES=DS
        _fwait                  ; wait for the 8087
        movsw                   ; move it
        movsw                   ; . . .
        mov     SI,BX           ; restore program counter
        next                    ; execute next FCode
efcode  MOVE_LL_I4


fcode   MOVE_ULL_I4
dfcode  MOVE_ULL_R4
        mov     CX,4            ; set length of operand
        mov     BP,offset MvRet4; set return address
        jmp     UndChk          ; do undefined checking
MvRet4: mov     BX,DI           ; get address of src operand
        hop     DoMv4           ; do the move
efcode  MOVE_ULL_I4


fcode   MOVE_ULL_R8
        mov     CX,8            ; set length of operand
        mov     BP,offset MvRet8; set return address
        jmp     UndChk          ; do undefined checking
MvRet8: mov     BX,DI           ; get address of src operand
        hop     DoMv8           ; do the move
efcode  MOVE_ULL_R8


fcode   MOVE_LL_R8
        getword BX              ; get address of src operand
DoMv8:  getword DI              ; get address of dst operand
        mov     AX,DS           ; get DS
        xchg    BX,SI           ; get src addr, and save SI
        mov     ES,AX           ; set ES=DS
        _fwait                  ; wait for the 8087
        movsw                   ; move it
        movsw                   ; . . .
        movsw                   ; . . .
        movsw                   ; . . .
        mov     SI,BX           ; restore program counter
        next                    ; execute next FCode
efcode  MOVE_LL_R8


fcode   MOVE_IL_R8
        getword BX              ; get address of src operand
        getword DI              ; get address of dst operand
        xchg    BX,SI           ; get src addr, and save SI
        mov     AX,DS           ; save DS
        lds     SI,[SI]         ; get src addr
        mov     ES,AX           ; set ES=DS
        _fwait                  ; wait for the 8087
        movsw                   ; move it
        movsw                   ; . . .
        movsw                   ; . . .
        movsw                   ; . . .
        mov     SI,BX           ; restore program counter
        mov     DS,AX           ; restore DS
        next                    ; execute next FCode
efcode  MOVE_IL_R8

        fmodend
        end
