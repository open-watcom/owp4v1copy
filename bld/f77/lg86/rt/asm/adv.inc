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
;       the bounds are in order from subscript n to subscript 1
;       for each subscript, there is
ifndef __386__
bound_s struc
    bound_lo    dd      ?       ; 32-bit low bound
    bound_num   dw      ?       ; high bound - low bound (#elms-1)
bound_s ends
else
bound_s struc
    bound_lo    dd      ?       ; 32-bit low bound
    bound_num   dd      ?       ; high bound - low bound (#elms-1)
bound_s ends
endif

;
;       Array dope vector (ADV) layout
;
ifndef __386__
adv_s   struc
    adv_array   dw      ?       ; pointer to array storage
                dw      ?       ; ...
    adv_link    dw      ?       ; link to chain all ADVs together
                dw      ?       ; ...
    adv_elsize  dw      ?       ; element size
    adv_elnum   dd      ?       ; number of elements
    adv_numss   dw      ?       ; number of subscripts
adv_s   ends
else
adv_s   struc
    adv_array   dd      ?       ; pointer to array storage
    adv_link    dd      ?       ; link to chain all ADVs together
    adv_elsize  dd      ?       ; element size
    adv_elnum   dd      ?       ; number of elements
    adv_numss   dw      ?       ; number of subscripts
adv_s   ends
endif

adv_bound       equ size adv_s
ADV_BASE_SIZE   equ size adv_s

; these bits are hidden in the high byte of the adv_numss field
ADV_NO_SCALE equ 80H            ; element size is not a power of two
ADV_NOT_INIT equ 40H            ; ADV has not yet been initialized
ADV_INIT_MSK equ 0bfH           ; to turn off above bit
ADV_ALLOC_LOC equ 20H           ; ADV was allocated with location=
ADV_SUBSCRS equ 07H             ; the # of subscripts field
ADV_POW_OF_2 equ 8fH            ; the power of 2 field (high bit must be 0)
ADV_FIELD equ 80h               ; indicates that ADV is part of structure
