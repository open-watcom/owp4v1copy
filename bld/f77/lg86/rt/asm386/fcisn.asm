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
; FCISN         : ISN processing F-Codes
;

.8087

include fcdef.inc
include errcod.inc

        xref    RTErr_
        xref    ChkLimitErr_

        extrn   _ExLinePtr      : dword
        extrn   _StmtRem        : dword
        extrn   _StmtLimit      : dword

        fmodstart fcisn


fcode   RT_SET_LINE                     ; start of new statement
        sub     dword ptr _StmtRem,1    ; decrement # of remaining statements
        jc      CheckLim                ; check if limit reached
OK:
        _fwait                          ; wait for last 8087 intruction to
                                        ; complete in case an exception occurred
        docall  ChkLimitErr_            ; check for limit error
        mov     _ExLinePtr,esi          ; point to ISN number
        add     esi,4                   ; skip over ISN number
        next                            ; execute next F-Code
CheckLim:
        cmp     _StmtLimit,0            ; check if statement limit is infinity
        je      OK                      ; inifite statments, return to program
        mov     _ExLinePtr,esi          ; get pointer to line
        mov     eax,KO_STMT_CNT         ; get error code
        hop     RTError                 ; flag error
efcode  RT_SET_LINE


        xdefp   RTError
defp    RTError                         ; handle an error condition
        exit_fcode                      ; switch to run-time environment
        push    eax                     ; pass error code
        docall  RTErr_                  ; process the error
        ret                             ; return from fcode processor
endproc RTError

        fmodend
        end
