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
; DBINIT :      initialize the debugger F-Codes in a stand-alone application
;

include fcdef.inc
include fcodes.inc
include xinit.inc

        xref    DB@RT_ISN_DBUG
        xref    DB@DB_SFPRO
        xref    DB@DB_SSCR_FINI
        xref    DB@DB_SSTR_FINI
        xref    DB@RT_END_DBUG
        xref    DB@DB_STOP_STUB
        xref    DB@DB_QUIT_STUB
        xref    DB@DB_ERR_STUB
        xref    DB@RT_DB_PROLOGUE
        xref    DB@RT_DB_EPILOGUE
        xref    DB@DB_SFEPI
        xref    DebugInit_
        xref    DebugFini_
        lgxref  DEBUG_INIT
        lgxref  DEBUG_FINI
        xref    DbStopped_
        xref    DbPaused_
        lgxref  DEBUG_STOPPED
        lgxref  DEBUG_PAUSED
        lgxref  STOP_HOOK
        lgxref  STOP_HANDLER
        lgxref  PAUSE_HOOK
        lgxref  PAUSE_HANDLER
        lgxref  ERR_HOOK
        lgxref  ERR_HANDLER

xinit   __debug_fcodes__,DEF_PRIORITY

        extrn   LG@FC_TABLE : dword

        modstart dbinit

        xdefp   __debug_fcodes__
defp    __debug_fcodes__
        mov     eax,offset32 LG@FC_TABLE
        mov     dword ptr (4*FCODE_RT_ISN_DBUG)[eax],offset32 DB@RT_ISN_DBUG
        mov     dword ptr (4*FCODE_DB_SFPRO)[eax],offset32 DB@DB_SFPRO
        mov     dword ptr (4*FCODE_DB_SSCR_FINI)[eax],offset32 DB@DB_SSCR_FINI
        mov     dword ptr (4*FCODE_DB_SSTR_FINI)[eax],offset32 DB@DB_SSTR_FINI
        mov     dword ptr (4*FCODE_RT_END_DBUG)[eax],offset32 DB@RT_END_DBUG
        mov     dword ptr (4*FCODE_DB_STOP_STUB)[eax],offset32 DB@DB_STOP_STUB
        mov     dword ptr (4*FCODE_DB_QUIT_STUB)[eax],offset32 DB@DB_QUIT_STUB
        mov     dword ptr (4*FCODE_DB_ERR_STUB)[eax],offset32 DB@DB_ERR_STUB
        mov     dword ptr (4*FCODE_RT_DB_PROLOGUE)[eax],offset32 DB@RT_DB_PROLOGUE
        mov     dword ptr (4*FCODE_RT_DB_EPILOGUE)[eax],offset32 DB@RT_DB_EPILOGUE
        mov     dword ptr (4*FCODE_DB_SFEPI)[eax],offset32 DB@DB_SFEPI
        mov     dword ptr DEBUG_INIT,offset32 DebugInit_
        mov     dword ptr DEBUG_FINI,offset32 DebugFini_
        mov     dword ptr DEBUG_STOPPED,offset32 DbStopped_
        mov     dword ptr DEBUG_PAUSED,offset32 DbPaused_
        mov     dword ptr STOP_HOOK,offset32 STOP_HANDLER
        mov     dword ptr PAUSE_HOOK,offset32 PAUSE_HANDLER
        mov     dword ptr ERR_HOOK,offset32 ERR_HANDLER
        ret
endproc __debug_fcodes__

        endmod
        end
