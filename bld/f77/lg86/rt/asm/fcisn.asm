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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<> FCDRVR      : ISN processing F-Codes
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include errcod.inc
include xfflags.inc

        xref    RTErr_
        xref    LimError_

        fmodstart    fcdrvr

        dataseg

        xred    "C",ExLinePtr,   word
        xred    "C",StmtRem,     word
        xred    _StmtLimit,      word

        enddata


fcode   RT_SET_LINE                     ; start of new statement
        sub     word ptr ss:StmtRem+0,1 ; decrement # of remaining statements
        jc      DecHi                   ; decrement hi word (if necessary)
OK:
        _fwait                          ; wait for last 8087 intruction to
                                        ; complete in case an exception occurred
        test    SS:__XcptFlags,XF_LIMIT_ERR; check for limit error
        jne     LimErrRep               ; process error
        mov     ss:ExLinePtr,SI         ; point to ISN number
        add     SI,2                    ; skip over ISN number
        next                            ; execute next F-Code
DecHi:  sbb     word ptr ss:StmtRem+2,0 ; . . .
        jnc     OK                      ; if more statements to execute
        mov     AX,SS:_StmtLimit+0      ; check if statement limit is infinity
        or      AX,SS:_StmtLimit+2      ; . . .
        je      OK                      ; inifite statments, return to program
        mov     ss:ExLinePtr,SI         ; get pointer to line
        mov     AX,KO_STMT_CNT          ; get error code
        hop     RTError                 ; flag error
efcode  RT_SET_LINE


defn    LimErrRep                       ; report a limit error
        call    FCLimErr                ; report the error
        enter_fcode                     ; switch to F-Code environment
        hop     OK                      ; we returned, restart execution
endproc LimErrRep


        xdefp   FCLimErr
defn    FCLimErr
        exit_fcode                      ; switch to run-time environment
        docall  LimError_               ; process the error
        ret
endproc FCLimErr


        xdefp   RTError
defp    RTError                         ; handle an error condition
        exit_fcode                      ; switch to run-time environment
        push    AX                      ; pass error code
        docall  RTErr_                  ; process the error
        ret                             ; return from fcode processor
endproc RTError

        fmodend
        end
