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

        xref    DbSScr
        xref    DbSStr
        xref    GetTBFCCall_

        extrn   _DbInAddr : dword

        fmodstart       dbfc

; void          DbSubScr( int num_ss, signed_32 *ss )
; ===================================================

        xdefp   DbSubScr_
defp    DbSubScr_
        push    ebx                     ; save registers
        push    ecx                     ; ...
        push    edi                     ; ...
        push    esi                     ; ...
        push    ebp                     ; ...
        mov     esi,edx                 ; point to subscripts
        _loop                           ; loop
          push  dword ptr [esi]         ; - push subscript
          add   esi,4                   ; - point to next subscript
          dec   eax                     ; - decrement subscript count
        _until  e                       ; until all subscripts pushed
        docall  GetTBFCCall_            ; setup F-Code program pointers
        mov     esi,eax                 ; ...
        mov     word ptr [esi],FCODE_DB_SSCR_FINI*4
        mov     eax,_DbInAddr           ; get offset of ADV
        jmp     DbSScr                  ; do subscripting
endproc DbSubScr_


dbfcode DB_SSCR_FINI
        mov     _DbInAddr,edi           ; set pointer to array element
        pop     ebp                     ; ...
        pop     esi                     ; ...
        pop     edi                     ; ...
        pop     ecx                     ; ...
        pop     ebx                     ; restore registers
        ret
edbfcode DB_SSCR_FINI


; export DbSubStr( int ss1, int ss2, string *scb )
; ================================================

        xdefp   DbSubStr_
defp    DbSubStr_
        push    ecx                     ; save registers
        push    edi                     ; ...
        push    esi                     ; ...
        push    ebp                     ; ...
        push    ebx                     ; save pointer to result SCB
        mov     edi,_DbInAddr           ; point to source SCB
        push    dword ptr 4[edi]        ; push it on the stack
        push    dword ptr [edi]         ; ...
        push    eax                     ; push first subscript
        push    edx                     ; push second subscript
        docall  GetTBFCCall_            ; setup F-Code program pointers
        mov     word ptr [eax],FCODE_DB_SSTR_FINI*4
        jmp     DbSStr                  ; do substringing
endproc DbSubStr_


dbfcode DB_SSTR_FINI
        pop     edi                     ; get resulting SCB
        pop     ecx                     ; ...
        pop     ebx                     ; get pointer to result SCB
        mov     [ebx],edi               ; return result
        mov     4[ebx],ecx              ; ...
        pop     ebp                     ; ...
        pop     esi                     ; ...
        pop     edi                     ; ...
        pop     ecx                     ; save registers
        ret
edbfcode DB_SSTR_FINI

        fmodend
        end
