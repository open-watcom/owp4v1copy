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

        extrn   LG@FC_TABLE : word

        modstart dbinit

        xdefp   __debug_fcodes__
defp    __debug_fcodes__
        push    bx
        push    ds
        mov     bx,seg LG@FC_TABLE
        mov     ds,bx
        mov     bx,offset LG@FC_TABLE
        mov     word ptr (2*FCODE_RT_ISN_DBUG)[bx],offset DB@RT_ISN_DBUG
        mov     word ptr (2*FCODE_DB_SFPRO)[bx],offset DB@DB_SFPRO
        mov     word ptr (2*FCODE_DB_SSCR_FINI)[bx],offset DB@DB_SSCR_FINI
        mov     word ptr (2*FCODE_DB_SSTR_FINI)[bx],offset DB@DB_SSTR_FINI
        mov     word ptr (2*FCODE_RT_END_DBUG)[bx],offset DB@RT_END_DBUG
        mov     word ptr (2*FCODE_DB_STOP_STUB)[bx],offset DB@DB_STOP_STUB
        mov     word ptr (2*FCODE_DB_QUIT_STUB)[bx],offset DB@DB_QUIT_STUB
        mov     word ptr (2*FCODE_DB_ERR_STUB)[bx],offset DB@DB_ERR_STUB
        mov     word ptr (2*FCODE_RT_DB_PROLOGUE)[bx],offset DB@RT_DB_PROLOGUE
        mov     word ptr (2*FCODE_RT_DB_EPILOGUE)[bx],offset DB@RT_DB_EPILOGUE
        mov     word ptr (2*FCODE_DB_SFEPI)[bx],offset DB@DB_SFEPI
        mov     bx, DEBUG_INIT
        mov     word ptr ss:[bx],offset DebugInit_
        mov     word ptr ss:2[bx],seg DebugInit_
        mov     bx, DEBUG_FINI
        mov     word ptr ss:[bx],offset DebugFini_
        mov     word ptr ss:2[bx],seg DebugFini_
        mov     bx, DEBUG_STOPPED
        mov     word ptr ss:[bx],offset DbStopped_
        mov     word ptr ss:2[bx],seg DbStopped_
        mov     bx, DEBUG_PAUSED
        mov     word ptr ss:[bx],offset DbPaused_
        mov     word ptr ss:2[bx],seg DbPaused_
        mov     bx, STOP_HOOK
        mov     word ptr ss:[bx],offset STOP_HANDLER
        mov     word ptr ss:2[bx],seg STOP_HANDLER
        mov     bx, PAUSE_HOOK
        mov     word ptr ss:[bx],offset PAUSE_HANDLER
        mov     word ptr ss:2[bx],seg PAUSE_HANDLER
        mov     bx, ERR_HOOK
        mov     word ptr ss:[bx],offset ERR_HANDLER
        mov     word ptr ss:2[bx],seg ERR_HANDLER
        pop     ds
        pop     bx
        ret
endproc __debug_fcodes__

        endmod
        end
