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
; DBFC  : interface for subscripting/substringing in debugger
;

.8087

include fcdef.inc
include datamap.inc
include fcodes.inc

        extrn   DbSScr : near
        extrn   DbSStr : near

        fmodstart       dbfc

        dataseg

        xred    "C",ExCurr,    word
        xred    "C",DbInAddr,  word

        enddata


; void          DbSubScr( int num_ss, signed_32 *ss )
; ===================================================

        xdefp   DbSubScr_
defp    DbSubScr_
        push    BP                      ; save registers
        push    SI                      ; ...
        push    DI                      ; ...
        push    AX                      ; ...
        push    BX                      ; ...
        push    CX                      ; ...
        push    DX                      ; ...
        push    ES                      ; ...
        push    DS                      ; ...
        mov     DS,CX                   ; point to subscripts
        _loop                           ; loop
          push  2[BX]                   ; - push subscript
          push  0[BX]                   ; - ...
          add   BX,4                    ; - point to next subscript
          dec   AL                      ; - decrement subscript count
        _until  e                       ; until all subscripts pushed
        lds     SI,dword ptr SS:ExCurr  ; setup F-Code program pointers
        add     SI,FC_CALL              ; ...
        mov     word ptr [SI],FCODE_DB_SSCR_FINI*2
        mov     AX,SS:DbInAddr          ; get offset of ADV
        jmp     DbSScr                  ; do subscripting
endproc DbSubScr_


dbfcode DB_SSCR_FINI
        mov     SS:DbInAddr,DI          ; set pointer to array element
        mov     SS:DbInAddr+2,ES        ; . . .
r_regs: pop     DS                      ; restore registers
        pop     ES                      ; ...
        pop     DX                      ; ...
        pop     CX                      ; ...
        pop     BX                      ; ...
        pop     AX                      ; ...
        pop     DI                      ; ...
        pop     SI                      ; ...
        pop     BP                      ; ...
        retf
edbfcode DB_SSCR_FINI


; export DbSubStr( int ss1, int ss2, string *scb )
; ================================================

        xdefp   DbSubStr_
defp    DbSubStr_
        push    BP                      ; save registers
        push    SI                      ; ...
        push    DI                      ; ...
        push    AX                      ; ...
        push    BX                      ; ...
        push    CX                      ; ...
        push    DX                      ; ...
        push    ES                      ; ...
        push    DS                      ; ...
        push    CX                      ; save pointer to result SCB
        push    BX                      ; ...
        les     DI,dword ptr SS:DbInAddr; point to source SCB
        push    ES:4[DI]                ; push it on the stack
        push    ES:2[DI]                ; ...
        push    ES:0[DI]                ; ...
        sub     CX,CX                   ; push first subscript
        push    CX                      ; ...
        push    AX                      ; ...
        push    CX                      ; push second subscript
        push    DX                      ; ...
        lds     SI,dword ptr SS:ExCurr  ; setup F-Code program pointers
        add     SI,FC_CALL              ; ...
        mov     word ptr [SI],FCODE_DB_SSTR_FINI*2
        jmp     DbSStr                  ; do substringing
endproc DbSubStr_


dbfcode DB_SSTR_FINI
        pop     DI                      ; get resulting SCB
        pop     ES                      ; ...
        pop     CX                      ; ...
        pop     BX                      ; get pointer to result SCB
        pop     DS                      ; ...
        mov     [BX],DI                 ; return result
        mov     2[BX],ES                ; ...
        mov     4[BX],CX                ; ...
        hop     r_regs
edbfcode DB_SSTR_FINI


        fmodend
        end
