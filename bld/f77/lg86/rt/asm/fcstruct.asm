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

        fmodstart       fcfield

        dataseg

        extrn   __RCBOffset : word

        enddata



; add dx:ax to the huge ptr es:di
defn    CalcSegOffs
        add     ax,di
        adc     dx,0
        mov     cx,ax           ; get low word of offset
        shr     dx,1            ; shift offset right 4 bits to get
        rcr     cx,1            ; number of segments
        shr     dx,1            ; . . .
        rcr     cx,1            ; . . .
        shr     dx,1            ; . . .
        rcr     cx,1            ; . . .
        shr     dx,1            ; . . .
        rcr     cx,1            ; . . .
        mov     dx,es
        and     ax,0fh          ; isolate offset from segment
        add     dx,cx           ; add # of segments
        ret
endproc CalcSegOffs


fcode   FIELD_OP_ADV
        pop     ax              ; get field offset
        pop     dx              ; ...
        les     di,es:[di]      ; load pointer to storage
        call    CalcSegOffs     ; add to pointer
        mov     bx,ax           ; save low word of ptr
        mov     cx,dx           ; save high word
        getword ax              ; get dest ADV offset
        push    ds              ; set es==ds
        mov     di,ax           ; put dest ADV offset into di
        getword ax              ; get source ADV offset
        pop     es              ; set es==ds
        xchg    si,ax           ; save f-code i.p. & source ADV into si
        mov     adv_array+2[di],cx; store segment of array
        mov     cx,adv_numss[si]; get # of subscripts
        mov     adv_array[di],bx; store offset of array
        and     cx,ADV_SUBSCRS  ; finish getting # of subscripts
        add     di,adv_link     ; advance past adv_array
        mov     bx,cx           ; save # subscrs
        shl     cx,1            ; multiply by 6
        add     si,adv_link     ; advance past adv_array
        add     cx,bx           ; continue multiply by 6
        shl     cx,1            ; ...
        add     cx,ADV_BASE_SIZE-adv_link; add on extra storage
        rep     movsb           ; copy the ADV
        xchg    si,ax           ; restore f-code pointer
        next
efcode  FIELD_OP_ADV


fcode   FIELD_ADDR
        getword bx              ; get offset to calc'd offset
        getword di              ; load ptr to rcb
        mov     dx,2[bx]        ; load offset
        mov     ax,[bx]         ; ...
        mov     ss:__RCBOffset,di; save ptr to rcb for error reporting
        les     di,[di]         ; load ptr to storage
        call    CalcSegOffs
        mov     es,dx           ; finish up field op calculation
        mov     di,ax           ; ...
        next
efcode  FIELD_ADDR


fcode   FIELD_ADDR_PUSH_SCB
dfcode  FIELD_ADDR_PUSH_RCB
        getword bx              ; get ptr to calc'd offset tmp
        getword di              ; get ptr to rcb
        mov     ax,[bx]         ; load offset
        mov     dx,2[bx]
        push    4[bx]           ; push sdefn/size
        les     di,[di]         ; get ptr to storage
        call    CalcSegOffs
        push    dx
        push    ax
        next
efcode  FIELD_ADDR_PUSH_SCB


fcode   SUBSCR_FIELD_OP         ; done subscript, add on offset
        getword bx              ; get ptr to offset
        inc     byte ptr ss:FIELDREF; note it's a field reference
        add     si,2            ; advance past ADV ptr
        mov     ax,[bx]         ; load offset
        mov     dx,2[bx]        ; ...
        push    cx              ; save cx (since this is after an array subscr)
        call    CalcSegOffs     ; calculate ptr
        pop     cx              ; restore cx
        mov     di,ax           ; setup ptr
        mov     es,dx           ; ...
        next
efcode  SUBSCR_FIELD_OP


fcode   FIELD_OP_CHAR
dfcode  FIELD_OP_STRUCT
        getword cx              ; get size/ptr to sdefn
        getword bx              ; get ptr to tmp
        pop     0[bx]           ; get offset from stack
        pop     2[bx]           ; ...
        mov     4[bx],cx        ; save size/ptr
        next
efcode  FIELD_OP_CHAR


fcode   FIELD_OP_UPDATE_SCB
dfcode  FIELD_OP_UPDATE_RCB
        getword bx              ; get ptr to scb/rcb
        pop     0[bx]           ; fill in scb/rcb
        pop     2[bx]           ; ...
        pop     4[bx]           ; ...
        next
efcode  FIELD_OP_UPDATE_SCB


fcode   STRUCT_SUBSCR           ; finish a structure valued subscript
        getword ax              ; get offset of sdefn
        push    ax              ; create RCB on stack
        push    es              ; push segment
        push    di              ; push offset
        next
efcode  STRUCT_SUBSCR


fcode   FIELD_SCB               ; create an SCB after character field op
        getword cx
        pop     ax              ; get low word
        pop     dx              ; get high word
        push    cx              ; push length
        push    dx              ; push high word
        push    ax              ; push low word
        next
efcode  FIELD_SCB


fcode   FIELD_CHAR_SUBSCR       ; create SCB after character array field op
        getword bx              ; get ptr to tmp string
        push    4[bx]           ; push size
        push    es              ; push origin (STRUCT_SUBSCR done already)
        push    di              ; ...
        next
efcode  FIELD_CHAR_SUBSCR


fcode   ASGN_STRUCT             ; assign a structure
        pop     di              ; get destination RCB
        pop     es              ; ...
        pop     bx              ; ...
        pop     ax              ; get source RCB offset
        mov     cx,sd_total_size[bx]; get size from dest sdefn
        mov     dx,ds           ; save f-code segment
        pop     ds              ; get source RCB segment
        xchg    ax,si           ; save f-code offset
        pop     bp              ; source RCB type (throw away)
        rep     movsb           ; do the assignment
        xchg    si,ax           ; restore f-code i.p.
        mov     ds,dx           ; ...
        next
efcode  ASGN_STRUCT


        fmodend
        end
