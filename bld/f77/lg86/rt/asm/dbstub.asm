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
; DBSTUB :      stubs for debugging fcodes in s.a. library
;

include fcdef.inc

        fmodstart dbstub

fcode   RT_ISN_DBUG
        add     si,2            ; skip word
dfcode  DB_SFPRO
        add     si,2            ; skip word
dfcode  DB_SSCR_FINI
dfcode  DB_SSTR_FINI
dfcode  RT_END_DBUG
dfcode  DB_STOP_STUB
dfcode  DB_QUIT_STUB
dfcode  DB_ERR_STUB
dfcode  RT_DB_PROLOGUE
dfcode  RT_DB_EPILOGUE
dfcode  DB_SFEPI
        next
efcode  RT_ISN_DBUG

        fmodend
        end
