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
; FCSTOP        : Stop and Pause routines
;

include fcdef.inc

        rtxref  Stop
        rtxref  Pause

        fmodstart       fcstop

fcode   RT_STOP
        mov     ebx,offset32 Stop       ; get address of "stop" processor
        hop     StopPause
efcode  RT_STOP


fcode   RT_PAUSE
        mov     ebx,offset32 RT@Pause   ; get address of "pause" processor
;;;;;;;;hop     StopPause
efcode  RT_PAUSE


defn    StopPause
        getdw   eax                     ; get pointer to SCB
        exit_fcode                      ; switch to run-time environment
        or      eax,eax                 ; check if it's a null ptr
        _if     ne
          add   eax,ebp
        _endif
        call    ebx                     ; ...
        enter_fcode                     ; switch to F-Code environment
        next
endproc StopPause

        fmodend
        end
