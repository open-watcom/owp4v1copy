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
; FCPOP2        : pop routines common to 86/87 version
;

include fcdef.inc
include structio.inc

        fmodstart       fcpop2

fcode   POP_SCB                 ; fill in an SCB
dfcode  POP_RCB                 ; fill in an RCB
        getword DI              ; get addr of variable
        mov     ax,ds
        mov     es,ax           ; es:di must point at scb
;;;;;;;;hop     STOR_SCB
efcode  POP_SCB


fcode   STOR_SCB
dfcode  STOR_RCB
        pop     es:0[DI]        ; assign an SCB
        pop     es:2[DI]        ; . . .
        pop     es:4[DI]        ; . . .
        next                    ; execute next F-Code
efcode  STOR_SCB


fcode   POP_ARR_ELT_PARM        ; fill in an array element parm location
        getword DI              ; get addr of information destination
        mov     AX,DS           ; set ES=DS
        mov     ES,AX           ; . . .
        pop     AX              ; get element address
        stosw                   ; . . .
        pop     AX              ; . . .
        stosw                   ; . . .
        pop     AX              ; get element length
        stosw                   ; . . .
        pop     AX              ; get ADV address
        stosw                   ; . . .
        pop     AX              ; trash segment value
        next                    ; execute next F-Code
efcode  POP_ARR_ELT_PARM


fcode   POP_ADDR
        getword bx              ; get offset of storage
        mov     [bx],di         ; store pointer
        mov     2[bx],es        ; ...
        next
efcode  POP_ADDR


        fmodend
        end
