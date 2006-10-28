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
;* Description:  convert long long into long double  __I8LD, __U8LD
;*
;*****************************************************************************


ifdef _BUILDING_MATHLIB

include mdef.inc
include struct.inc
include xception.inc

        modstart    i8ld386, dword

endif

        xdefp   __I8LD
        xdefp   __U8LD

;       __I8LD - convert long long into long double
;       __U8LD - convert unsigned long long into long double
; input:
;       EAX - pointer to long long
;       EDX - pointer to long double

__I8LD  proc    near
        push    eax
        mov     eax,0
        mov     [edx],eax
        mov     4[edx],eax
        mov     8[edx],ax
        pop     eax
        ret
__I8LD  endp

; input:
;       EAX - pointer to unsigned long long
;       EDX - pointer to long double
__U8LD: proc    near
        push    eax
        mov     eax,0
        mov     [edx],eax
        mov     4[edx],eax
        mov     8[edx],ax
        pop     eax
        ret
__I8LD  endp

ifdef _BUILDING_MATHLIB

        endmod

        endf    equ end
else
        endf    equ <>

endif

endf
