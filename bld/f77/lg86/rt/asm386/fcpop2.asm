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
; FCPOP2        : pop routines common to 386/387 version
;

include fcdef.inc
include structio.inc

        fmodstart       fcpop2

fcode   POP_SCB                 ; fill in an SCB
dfcode  POP_RCB                 ; fill in an RCB
        getdw   edi             ; get addr of variable
        add     edi,ebp         ; we must leave edi pointing at scb/rcb
;;;;;;;;hop     STOR_SCB
efcode  POP_SCB


fcode   STOR_SCB
dfcode  STOR_RCB
        pop     [edi]           ; assign an SCB
        pop     4[edi]          ; ...
        next
efcode  STOR_SCB


fcode   POP_ARR_ELT_PARM        ; fill in an array element parm location
        getdw   eax             ; get addr of information destination
        pop     [eax+ebp]       ; get elm addr
        pop     4[eax+ebp]      ; get elm length/sdefn
        pop     8[eax+ebp]      ; get ADV addr
        next
efcode  POP_ARR_ELT_PARM


fcode   POP_ADDR
        getdw   eax             ; get offset of storage
        mov     [eax+ebp],edi   ; store pointer
        next
efcode  POP_ADDR


        fmodend
        end
