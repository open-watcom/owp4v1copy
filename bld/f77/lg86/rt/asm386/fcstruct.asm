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
; FCSTRUCT      : Structure support
;

include fcdef.inc
include structio.inc
include adv.inc
include undef.inc

        lgxref  FIELDREF

        fmodstart       fcstruct

fcode   FIELD_OP_ADV
        pop     ebx             ; get offset calc'd so far
        getdw   edx             ; get dest ADV offset
        getdw   eax             ; get source ADV offset
        add     ebx,[edi]       ; calculate offset of array storage
        lea     edi,[edx+ebp]   ; get dest ADV ptr
        add     eax,ebp         ; add on data base
        mov     adv_array[edi],ebx; store ptr to storage
        movzx   ecx,byte ptr adv_numss[eax]; get # of subscripts
        xchg    esi,eax         ; save f-code i.p. & source ADV into si
        and     cl,ADV_SUBSCRS  ; finish getting # of subscripts
        add     edi,adv_link    ; advance past adv_array
        lea     ecx,((ADV_BASE_SIZE-adv_link)/2)[ecx*4]; calc words to copy
        add     esi,adv_link    ; advance past adv_array
        rep     movsw           ; copy the ADV
        xchg    esi,eax         ; restore f-code pointer
        next
efcode  FIELD_OP_ADV


fcode   FIELD_OP_UPDATE_SCB
dfcode  FIELD_OP_UPDATE_RCB
        getdw   eax             ; offset of tmp scb
        pop     [eax+ebp]       ; store calc'd offset
        pop     4[eax+ebp]      ; store size/sdefn
        next
efcode  FIELD_OP_UPDATE_SCB


fcode   FIELD_OP_CHAR
dfcode  FIELD_OP_STRUCT
        getdw   ecx             ; get size/ptr to sdefn
        getdw   eax             ; get ptr to tmp storage
        pop     [eax+ebp]       ; get offset from stack
        mov     4[eax+ebp],ecx  ; store size/ptr
        next
efcode  FIELD_OP_CHAR


fcode   STRUCT_SUBSCR           ; finish a structure valued subscript
        getdw   eax             ; get offset of sdefn
        push    eax             ; create RCB on stack
        push    edi             ; push offset
        next
efcode  STRUCT_SUBSCR


fcode   FIELD_SCB               ; create an SCB after character field op
        getdw   ecx
        pop     eax             ; get offset from stack
        push    ecx             ; push length
        push    eax             ; push offset back on stack
        next
efcode  FIELD_SCB


fcode   FIELD_CHAR_SUBSCR       ; create SCB after character array field op
        getdw   eax             ; get ptr to tmp string
        push    4[eax+ebp]      ; push size of string
        push    edi             ; push origin (STRUCT_SUBSCR done already)
        next
efcode  FIELD_CHAR_SUBSCR


fcode   ASGN_STRUCT             ; assign a structure
        pop     edi             ; get dest RCB
        pop     ebx             ; ...
        pop     eax             ; get source RCB offset
        mov     ecx,sd_total_size[ebx+ebp]; get size from dest sdefn
        xchg    eax,esi         ; save f-code offset
        add     esp,4           ; source RCB type (throw away)
        rep     movsb           ; do the assignment
        xchg    esi,eax         ; restore f-code i.p.
        next
efcode  ASGN_STRUCT


fcode   FIELD_ADDR
        getdw   eax             ; get ptr to calc'd offset
        getdw   edx             ; get ptr to rcb
        add     edx,ebp         ; edx must pt to rcb for error reporting
        mov     edi,[edx]       ; get ptr to storage
        add     edi,[eax+ebp]   ; add offset of field
        next
efcode  FIELD_ADDR


fcode   FIELD_ADDR_PUSH_SCB
dfcode  FIELD_ADDR_PUSH_RCB
        getdw   eax             ; get ptr to calc'd offset tmp
        getdw   edx             ; get ptr to rcb
        mov     edi,[edx+ebp]   ; get ptr to storage
        push    4[eax+ebp]      ; push sdefn/size
        add     edi,[eax+ebp]   ; add on offset
        push    edi             ; finish scb/rcb
        next
efcode  FIELD_ADDR_PUSH_SCB


fcode   SUBSCR_FIELD_OP         ; done subscript, add on offset
        getdw   eax             ; get ptr to offset
        inc     byte ptr FIELDREF; indicate it was a field ref for error report
        add     esi,4           ; advance past ADV ptr
        add     edi,[eax+ebp]   ; add offset
        next
efcode  SUBSCR_FIELD_OP


        fmodend
        end
