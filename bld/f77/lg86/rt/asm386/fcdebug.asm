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
;<> FCDEBUG     : run-time debugger support
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include datamap.inc

        xref    DbLine_
        xref    DbProlog_
        xref    DbEpilog_
        xref    "C",Spawn
        xref    Suicide_
        xref    ChkLimitErr_
        xref    UnLinkSFTrace_

        fcxref  RT_SET_LINE

        xred    "C",ExLinePtr,    dword
        xred    "C",DbugRet,      word

        dataseg

        SaveESI dd      0
        SaveEBP dd      0
        SFFlag  db      0

        enddata

        fmodstart       fcdebug


; ISN code executed when using the debugger.

dbfcode RT_ISN_DBUG
        add     esi,4                   ; skip source line number
        mov     ExLinePtr,esi           ; save pointer to ISN
DbErr:
        exit_fcode                      ; switch to run-time environment
        mov     eax,offset32 BegDbug      ; ...
        mov     SaveESI,esi             ; ...
        mov     SaveEBP,ebp             ; ...
        docall  Spawn                   ; ...
        movzx   eax,word ptr DbugRet    ; get debugger return address
        mov     esi,ExLinePtr           ; get pointer to ISN
        enter_fcode                     ; switch to F-Code environment
        jmp     LG@FC_TABLE[eax]        ; execute debugger return code
edbfcode RT_ISN_DBUG


; Enter debugger.
; The only way we will NOT return here from the debugger is if the
; "quit" command was issued; otherwise we want to execute the current line.

defn    BegDbug
        docall  DbLine_                 ; call the debugger
        enter_fcode                     ; switch to F-Code environment
        mov     esi,SaveESI             ; ...
        mov     ebp,SaveEBP             ; ...
        jmp     RT_SET_LINE             ; execute ISN code
endproc BegDbug


;
; We have successfully executed the current line.
;

dbfcode RT_END_DBUG
if _MATH eq _8087
        _fwait                          ; check for pending 8087 exceptions
        docall  ChkLimitErr_
endif
        getword ax                      ; get next F-Code
        exit_fcode                      ; switch to run-time environment
        mov     ExLinePtr,esi           ; save F-Code program counter
        mov     DbugRet,ax              ; set to execute next line
        dojmp   Suicide_                ; go execute next line
edbfcode RT_END_DBUG


; We will return here when the STOP statement is executed.

dbfcode DB_STOP_STUB
;;;;;;;;hop     DbgQuit_
edbfcode DB_STOP_STUB


; We will return here when the "quit" command is issued from the debugger.

dbfcode DB_QUIT_STUB
        exit_fcode                      ; switch to run-time environment
        dojmp   Suicide_                ; suicide
edbfcode DB_QUIT_STUB


; An error has occurred while executing the line; re-execute the line.

dbfcode DB_ERR_STUB
        hop     DbErr                   ; go execute the line again
edbfcode DB_ERR_STUB


; Code executed at entry to a subprogram when using debugger.
;     Code generated for each line is
;         <EndDebug>    (really for previous line)
;         <ISNDebug>
;         <actual code>
;     We have a problem with the first line since there is no previous
;     line. This F-Code simulates a previous line upon entry to the
;     subprogram. One might suggest we shouldn't generate <EndDebug>
;     for the first line in a subprogram; we have to since the first
;     line can be branched to!

dbfcode RT_DB_PROLOGUE
        exit_fcode                      ; switch to WSL environment
        docall  DbProlog_               ; call debugger prologue routine
        mov     eax,offset32 DbProl     ; spawn so when first line suicides
        mov     SaveESI,esi             ; save program counter
        mov     SaveEBP,ebp             ; save data ptr
        docall  Spawn                   ; ... it comes here
        movzx   eax,word ptr DbugRet    ; get address of line to execute
        mov     esi,ExLinePtr           ; get F-Code program counter
        enter_fcode                     ; switch to F-Code environment
        jmp     LG@FC_TABLE[eax]        ; go execute line
edbfcode RT_DB_PROLOGUE


defn    DbProl
        enter_fcode                     ; switch to F-Code environment
        mov     esi,SaveESI             ; restore program counter
        mov     ebp,SaveEBP             ; restore data ptr
        next                            ; execute next F-Code
endproc DbProl


;
; Code executed when leaving a subprogram when using debugger.
;

dbfcode RT_DB_EPILOGUE
        exit_fcode                      ; switch to run-time environment
        docall  DbEpilog_               ; call debug epilogue routine
        enter_fcode                     ; switch to F-Code environment
        next                            ; execute next F-Code
edbfcode RT_DB_EPILOGUE


; This F-Code is generated when using the debugger. It is generated before
; any of the statement function F-Codes. We must spawn the statement function
; so that if an error occurs we can remove the statement function from the
; traceback chain.

dbfcode DB_SFPRO
        getdw   eax                     ; get address of statement function
        push    eax                     ; save it
        exit_fcode                      ; switch to run-time environment
        mov     SaveESI,esi             ; ...
        mov     SaveEBP,ebp             ; ...
        mov     byte ptr SFFlag,1       ; assume unsuccessful completion
        mov     eax,offset32 SFStrt     ; spawn statement function
        docall  Spawn                   ; ...
        pop     eax                     ; restore address of statement function
        cmp     byte ptr SFFlag,0       ; check for successful completion
        _if     e                       ; if successful completion
          mov   byte ptr SFFlag,1       ; - in case there is recursion
                                        ; - ... assume unsuccessful completion
          mov   esi,SaveESI             ; - restore F-Code pointer
          mov   ebp,SaveEBP             ; - restore data ptr
          enter_fcode                   ; - switch to F-Code environment
          next                          ; - execute next F-Code (s. f. return)
        _else                           ; else an error has occurred
          docall UnLinkSFTrace_         ; - ...
          dojmp Suicide_                ; - suicide
        _endif                          ; endif
edbfcode DB_SFPRO


; This F-Code is generated when using the debugger. It is generated
; at the end of the statement function. Set successful completion and call
; Suicide().

dbfcode DB_SFEPI
        docall  ChkLimitErr_            ; check for limit error
        exit_fcode                      ; switch to run-time environment
        mov     SaveESI,esi             ; save current F-Code pointer
        mov     SaveEBP,ebp             ; save current data ptr
        mov     byte ptr SFFlag,0       ; set successful completion
        dojmp   Suicide_                ; call Suicide()
edbfcode DB_SFEPI


defn    SFStrt
        enter_fcode                     ; switch to F-Code environment
        mov     esi,SaveESI             ; get f-code ptr
        mov     ebp,SaveEBP             ; get data ptr
        next                            ; execute the statement function
endproc SFStrt

        fmodend
        end
