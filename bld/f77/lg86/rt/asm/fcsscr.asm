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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     FORTRAN-77 run-time support for 8086 based machines
;<>
;<>     FCSSCR - Subscripted variable initialization & support
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc
include errcod.inc
include adv.inc

        xref    ltoa_
        xref    Z_U4Div_

        extrn   RTError : near
        extrn   UndChk : near
        extrn   BuildStrErr : near

        fmodstart       fcsscr

        dataseg

        xred    "C",MsgBuff,  word

        SSList  DD      7 DUP(0) ; 7 32-bit words to save subscripts
        WarpRet dw      0        ; warp return address
        lgxdefp FIELDREF
FIELDREF        db      0

        enddata

;
; ADV initialization routines
;

fcode   GO_WARP                 ; execute ADV init code
        getword AX              ; get the address to go to
        mov     SS:WarpRet,SI   ; save warp return address
        mov     SI,AX           ; set pc to init code
        next                    ; execute init f-codes
efcode  GO_WARP


fcode   END_WARP                ; return from ADV init code
        mov     SI,SS:WarpRet   ; get warp return address
        next                    ; execute next instruction
efcode  END_WARP


fcode   S_L7_FILL_IN            ; fill in the lower bound for index #7
        mov     CL,7            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L7_FILL_IN


fcode   S_L6_FILL_IN            ; fill in the lower bound for index #6
        mov     CL,6            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L6_FILL_IN


fcode   S_L5_FILL_IN            ; fill in the lower bound for index #5
        mov     CL,5            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L5_FILL_IN


fcode   S_L4_FILL_IN            ; fill in the lower bound for index #4
        mov     CL,4            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L4_FILL_IN


fcode   S_L3_FILL_IN            ; fill in the lower bound for index #3
        mov     CL,3            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L3_FILL_IN


fcode   S_L2_FILL_IN            ; fill in the lower bound for index #2
        mov     CL,2            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L2_FILL_IN


fcode   S_L1_FILL_IN            ; fill in the lower bound for index #1
        mov     CL,1            ; set bound number
;;;;;;;;hop     FillLow         ; fill in the lower bound
efcode  S_L1_FILL_IN


defn    FillLow                 ; fill in a lower bound
        getword BX              ; get the ADV address
        mov     DI,adv_numss[BX]; get number of suscripts in array
        and     DI,ADV_SUBSCRS  ; get rid of any shift information
        sub     CH,CH           ; convert bound number to word
        sub     DI,CX           ; get bound position index
        shl     DI,1            ; mul index by 2
        mov     CX,DI           ; save it
        shl     DI,1            ; mul index by 2 again (total = mul by 4)
        add     DI,CX           ; add in mul by 2 (total = mul by 6)
        pop     word ptr adv_bound+0[BX] [DI]; pop low word into ADV
        pop     word ptr adv_bound+2[BX] [DI]; pop high word into ADV
        next                    ; execute next f-code
endproc FillLow


fcode   S_H7_FILL_IN            ; fill in the upper bound for index #7
        mov     CL,7            ; set high bound number
        hop     FillHigh        ; fill in the upper bound
efcode  S_H7_FILL_IN


fcode   S_H6_FILL_IN            ; fill in the upper bound for index #6
        mov     CL,6            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H6_FILL_IN


fcode   S_H5_FILL_IN            ; fill in the upper bound for index #5
        mov     CL,5            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H5_FILL_IN


fcode   S_H4_FILL_IN            ; fill in the upper bound for index #4
        mov     CL,4            ; set high bound number
        hop     FillHigh        ; fill in the upper bound
efcode  S_H4_FILL_IN


fcode   S_H3_FILL_IN            ; fill in the upper bound for index #3
        mov     CL,3            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H3_FILL_IN


fcode   S_H2_FILL_IN            ; fill in the upper bound for index #2
        mov     CL,2            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H2_FILL_IN


fcode   S_H1_FILL_IN            ; fill in the upper bound for index #1
        mov     CL,1            ; get low bound index offset
;;;;;;;;hop     FillHigh        ; fill in the upper bound
efcode  S_H1_FILL_IN


defn    FillHigh                ; fill in an upper bound
        getword BX              ; get the ADV address
        mov     DI,adv_numss[BX]; get number of suscripts in array
        and     DI,ADV_SUBSCRS  ; get rid of any shift information
        sub     CH,CH           ; convert bound number to word
        sub     DI,CX           ; get bound position index
        shl     DI,1            ; mul index by 2
        mov     CX,DI           ; save it
        shl     DI,1            ; mul index by 2 again (total = mul by 4)
        add     DI,CX           ; add in mul by 2 (total = mul by 6)
        lea     BX,adv_bound[BX] [DI]; point at bound entry
        pop     AX              ; pop high bound low word into ax
        pop     DX              ; pop high bound high word into dx
        sub     AX,0[BX]        ; subtract low bound low word
        sbb     DX,2[BX]        ; subtract low bound high word
        jnz     baddim          ; if hi word is not zero report error
        inc     AX              ; get # of elements
        mov     4[BX],AX        ; store #elements in ADV
        next                    ; execute next f-code
endproc FillHigh


defn    baddim
        RTErr   SV_BAD_SSCR     ; report array incorrectly dimensioned
endproc baddim


fcode   RT_ADV_ASSUME           ; assumed dim array check
        getword BX              ; get the parm ADV address
        or      byte ptr adv_numss+1[BX],ADV_NOT_INIT; say not initialized
        call    GetSizePassed   ; return the size passed in DX:AX
        push    DX              ; save high word
        push    AX              ; save low word
        mov     word ptr adv_bound+4[BX],1; set last #elts to 1
        call    GetNElts        ; get the number of elements
        mov     CX,adv_elsize[BX]; get element size
        mov     BP,AX           ; save low word of # of elts
        mov     AX,DX           ; get high word to proper register
        mul     CX              ; sart multiplication
        xchg    AX,BP           ; save partial prod. and restore low word
        mul     CX              ; do next partial product
        add     DX,BP           ; complete operation (#elts * elt size)
        mov     DI,BX           ; save pointer to ADV
        mov     CX,DX           ; put used storage size in proper regs
        mov     BX,AX           ; ...
        pop     AX              ; get size passed (low word)
        pop     DX              ; . . .           ( high word)
        exit_fcode              ; switch to run-time environ
        docall  Z_U4Div_        ; do the division
        enter_fcode             ; switch to fcode environ
        mov     BX,DI           ; restore pointer to ADV
        or      DX,AX           ; see if #elts in last dim is zero
        je      TooBig          ; report arg array too big if required
        mov     CX,AX           ; save last #elts
        mov     word ptr adv_bound+4[BX],AX; set last #elts bound
        mov     AX,word ptr adv_elnum+2[BX]; get true number of elements
        mul     CX              ; . . .
        mov     DI,DX           ; . . .
        mov     AX,word ptr adv_elnum[BX]; . . .
        mul     CX              ; . . .
        add     DX,DI           ; . . .
        mov     word ptr adv_elnum+2[BX],DX; save number of elements
        mov     word ptr adv_elnum+0[BX],AX;. . .
        and     byte ptr adv_numss+1[BX],ADV_INIT_MSK; adv initialized
        next                    ; execute next f-code
efcode  RT_ADV_ASSUME


fcode   RT_ADV_VAR              ; variable dimension check
        mov     BX,[SI]         ; get the parm ADV address
        call    GetNElts        ; set the number of elements
;;;;;;;;hop     RT_ADV_CONST
efcode  RT_ADV_VAR


fcode   RT_ADV_CONST            ; constant arg array check
        getword BX              ; get ADV address
        or      byte ptr adv_numss+1[BX],ADV_NOT_INIT; say not initialized
        call    GetSizePassed   ; get the array size passed
        mov     CX,DX           ; save the parm array total size
        mov     DI,AX           ; . . .
        mov     AX,word ptr adv_elnum+2[BX]; get arg ADV num_elts (high word)
        mov     BP,adv_elsize[BX]; get arg element size
        mul     BP              ; multiply num elts by arg element size
        mov     DX,BP           ; save element size
        mov     BP,AX           ; save partial product
        mov     AX,word ptr adv_elnum[BX]; get arg ADV num_elts (low word)
        mul     DX              ; multiply by element size
        add     DX,BP           ; complete caculation
        cmp     DX,CX           ; check high words of array sizes
        _if     e               ; if the same then
          cmp   AX,DI           ; - compare the low words
        _endif                  ; endif
        ja      TooBig          ; report error if required
        and     byte ptr adv_numss+1[BX],ADV_INIT_MSK; adv initialized
        next                    ; execute next f-code
efcode  RT_ADV_CONST


defn    TooBig
        xor     ax,ax           ; indicate no field reference
        call    BuildStrErr     ; ...
        push    ss:MsgBuff+2    ; push address of array name
        push    ss:MsgBuff      ; ...
        RTErr   DM_RANGE_ERR_ON_DIMN; report arg array is too big
endproc TooBig


defn    GetSizePassed           ; get the size of array that was passed
        les     DI,dword ptr adv_link[BX] ; get parm ADV address (offset/seg)
        mov     AX,ES           ; get seg addr of passed ADV in AX
        or      AX,AX           ; check if ADV passed is NULL
        _if     e               ; if it is (array from external s.p.)
          mov   AX,0ffffH       ; - load AX:DX with maximum integer
          mov   DX,07fffH       ; - . . .
          ret                   ; - return
        _endif                  ; endif
        mov     AX,adv_array+2[BX]  ; get arg array start segment
        sub     AX,ES:adv_array+2[DI]; subtract parm array start segment
        sub     DX,DX           ; shift segment left 4 bits
        shl     AX,1            ; . . .
        rcl     DX,1            ; . . .
        shl     AX,1            ; . . .
        rcl     DX,1            ; . . .
        shl     AX,1            ; . . .
        rcl     DX,1            ; . . .
        shl     AX,1            ; . . .
        rcl     DX,1            ; . . .
        mov     CX,adv_array[BX]; get arg array start offset
        sub     CX,ES:adv_array[DI]; subtract parm array start offset
        sbb     DX,0            ; catch carry from subtraction
        add     AX,CX           ; get size not passed to arg array
        adc     DX,0            ; . . .
        push    DX              ; save size not passed
        push    AX              ; . . .
        mov     AX,word ptr ES:adv_elnum+2[DI]; get no. of elts. in parm array times
        mov     CX,word ptr ES:adv_elsize[DI];the element size
        mul     CX              ; . . .
        mov     DX,CX           ; . . .
        mov     CX,AX           ; . . .
        mov     AX,word ptr ES:adv_elnum[DI]; . . .
        mul     DX              ; . . .
        add     DX,CX           ; . . .
        pop     CX              ; calculate total size passed
        sub     AX,CX           ; . . .
        pop     CX              ; . . .
        sbb     DX,CX           ; . . .
        ret                     ; return
endproc GetSizePassed


defn    GetNElts                ; get the number of elements in an array
        lea     DI,adv_bound+4[BX]; point at first hi - low
        mov     CL,byte ptr adv_numss[BX]; get the number of subscripts
        and     CX,ADV_SUBSCRS  ; ...
        mov     AX,1            ; init number of elements count
        sub     DX,DX           ; . . .
loop1:                          ; loop
          mov   BP,[DI]         ; - get number of elements
          or    DX,DX           ; - check high word
          _if   ne              ; - if not zero then
            mov ES,AX           ; - - save low order word of total
            mov AX,DX           ; - - get high order word
            mul BP              ; - - multiply by current dimension
            xchg AX,BP          ; - - switch some registers around
            mov DX,ES           ; - - get old low order word
            mul DX              ; - - save partial product
            add DX,BP           ; - - complete multiplication
          _else                 ; - else
            mul BP              ; - - update running total
          _endif                ; - endif
          add   DI,6            ; - point at next (hi-lo)
        loop    loop1           ; - dec #sub's left to do, loop if more
        mov     word ptr adv_elnum+0[BX],AX; set the # of elements in the array
        mov     word ptr adv_elnum+2[BX],DX; . . .
        ret                     ; return to caller
endproc GetNElts


;
; Subscripting routines
;

        xdefp   UndefArray
defn    UndefArray              ; build error msg for undef array elt
        mov     AX,offset SS:SSList; point to subscript list
MkSubLst:                       ; form subscript list error message
        push    AX              ; save offset of the subscript list
        push    DI              ; save the ADV offset
        mov     BX,DI           ; pass pointer to item in DS:BX
        xor     AX,AX           ; delay processing of field information
        call    BuildStrErr     ; put the array name in MsgBuff
        pop     BX              ; restore the ADV offset
        pop     AX              ; restore offset of subscript list
        push    DS              ; save current fcode segment
        push    SI              ; save current fcode offset
        mov     SI,AX           ; point to the subscript list
        mov     AL,'('          ; put a left paren in AL
        stosb                   ; store it
        mov     CL,byte ptr adv_numss[BX]; get the number of subscripts
        and     CX,ADV_SUBSCRS  ; . . .
        mov     AX,CX           ; set number of subscripts in AX
        dec     AX              ; decrement by one
        shl     AX,1            ; calculate # of bytes for subscripts
        shl     AX,1            ; ...
        add     SI,AX           ; point to the first subscript
        _loop                   ; loop : put the subscripts in MsgBuff
          push    CX            ; - save the number of subscripts
          mov     DX,SS:2[SI]   ; - push hi word of subscript on stack
          mov     AX,SS:0[SI]   ; - push lo word of subscript on stack
          mov     CX,10         ; - set radix, push it on stack
          push    CX            ; ...
          mov     BX,DI         ; - point to buffer
          mov     CX,ES         ; - ...
          docall  ltoa_         ; - convert the subscript on the stack
          xor   AL,AL           ; - get a NULLCHAR
          _loop                 ; - loop
            scasb               ; - - scan a byte
            _quif e             ; - - quit if NULLCHAR found
          _endloop              ; - endloop
          dec   DI              ; - went one byte too far
          sub     SI,4          ; - point to next subscript in SSList
          pop     CX            ; - restore number of subscrs left
          dec     CX            ; - decrement subsript counter
          _quif   e             ; - quit if all subscrs done
          mov     AL,','        ; - seperate the subscripts by a comma
          stosb                 ; - ...
        _endloop                ; until no subscripts left
        mov     AL,')'          ; store right parenthesis
        stosb                   ; ...
        cmp     byte ptr SS:FIELDREF,0
        _if     ne              ; if it's a field reference
          mov   AL,'.'          ; - indicate field reference
          mov   AH,'?'          ; - ...
          stosw                 ; - ...
        _endif                  ; endif
        xor     AL,AL           ; store NULLCHAR
        stosb                   ; ...
        pop     SI              ; restore current fcode offset
        pop     DS              ; restore current fcode segment
        ret                     ; return
endproc UndefArray

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     prototype of generated code being interpreted
;<>
;<>     array(s1,s2,...,s7)
;<>             push s1
;<>                .
;<>                .
;<>             push s6
;<>             push s7
;<>             RTSubScr
;<>             addr of array dope vector (ADV)
;<>
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>     output: ES - segment address nearest the subscripted element
;<>             DI - offset from ES to the subscripted element
;<>             CX - size of the element (used by character subscript)
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

defn    out_of_bounds           ; subscript out of range error
        pop     DI              ; restore ADV pointer
        pop     SI              ; restore the fcode IP
far_out_of_1_bound:             ; entry point for 1-D subscripting rtns
        mov     AX,SP           ; point at subscript list
        call    MkSubLst        ; build the error message parm
        push    SS:MsgBuff+2    ; pass address of name
        push    SS:MsgBuff      ; pass address of name
        mov     AX,SS_SSCR_RANGE; set the error code
        jmp     RTError         ; report the error
endproc out_of_bounds


fcode   RT_PSUBSCRIPT           ; parameter subscripted variable
        mov     DI,[SI]         ; get the address of the ADV
        mov     AX,adv_array+2[DI]  ; get the segment value of origin
        or      AX,adv_array[DI]; or in offset value of origin
        je      BadParm         ; report invalid parm if both NULL
;;;;;;;;hop     RT_USUBSCRIPT   ; do undef var check
efcode  RT_PSUBSCRIPT


fcode   RT_USUBSCRIPT
        mov     BX,[SI]         ; get ADV addr
        mov     CL,byte ptr adv_numss[BX]; get number of subscripts
        and     CX,ADV_SUBSCRS  ; convert to a word
        shl     CX,1            ; change # of subscripts to # of words
        mov     AX,SI           ; save fcode IP
        mov     DX,DS           ; save current data seg
        mov     SI,SP           ; set SI to point to subscr's on stack
        mov     BX,SS           ; put the stack segment into DS
        mov     DS,BX           ; ...
        mov     ES,BX           ; put data segment into ES
        mov     DI,offset SS:SSList; set DI to subscript buffer
        rep     movsw           ; copy subscripts into SSList
        mov     DS,DX           ; restore fcode data seg
        mov     SI,AX           ; restore fcode IP
;;;;;;;;hop     RT_SUBSCRIPT    ; do the subscripting
efcode  RT_USUBSCRIPT


fcode   RT_SUBSCRIPT            ; run-time subscripting routine
dfcode  FIELD_SUBSCR            ; field subscripting routine
        getword ax              ; get addr of ADV
;;;;;;;;hop     DbSScr
efcode  RT_SUBSCRIPT


        xdefp   DbSScr
defn    DbSScr                  ; debugger subscripting entry point
        mov     SS:FIELDREF,0   ; zero out the field ref
        mov     DI,AX           ; ...get addr of ADV
        mov     CX,adv_numss[DI]; get # of subscripts to process
        mov     BP,SP           ; setup BP to access subscripts on stack
        and     CL,ADV_SUBSCRS  ; ...get # of subscripts cont'd
        push    SI              ; save the current fcode IP
        push    DI              ; save address of ADV
        lea     DI,adv_bound[DI]; point to first of subscript bounds
        sub     AX,AX           ; set offset to 0
        sub     DX,DX           ; . . .
        _loop                   ; loop (for each subscript)
          mov   BX,0[BP]        ; - get low word of subscript
          mov   SI,2[BP]        ; - get high word of subscript
          sub   BX,0[DI]        ; - subtract low bound
          sbb   SI,2[DI]        ; - . . .
          jne   out_of_bounds   ; - out of range error if high word not 0
          cmp   BX,4[DI]        ; - CHECK TO SEE IF OUT OF RANGE
          jae   out_of_bounds   ; - quit if out of range
          add   AX,BX           ; - add this subscript to prev result
          adc   DX,0            ; - . . .
          add   BP,4            ; - point to next subscript on stack
          dec   CL              ; - decrement # of subscripts to do
          _quif e               ; - quit if done
          add   DI,6            ; - point to next subscript bounds
          mov   SI,4[DI]        ; - GET # OF ELEMENTS IN THIS DIMENSION
          or    DX,DX           ; - test high order word of offset
          _if   ne              ; - if non zero
            mov   BX,AX         ; - - save low order word of offset
            mov   AX,DX         ; - - get high order word of offset
            mul   SI            ; - - calc. partial high order offset
            xchg  AX,BX         ; - - get low order, save high order
            mul   SI            ; - - multiply by # of elements
            add   DX,BX         ; - - add in partial high order result
          _else                 ; - else
            mul   SI            ; - - multiply by # of elements
          _endif                ; - endif
        _endloop                ; endloop
        pop     DI              ; restore address of ADV
        pop     SI              ; restore the fcode IP
        hop     Scale           ; scale offset by element size
endproc DbSScr


defn    BadParm                 ; report a bad parameter array
        mov     bx,di           ; pass address of item in DS:BX
        xor     ax,ax           ; indicate not a field reference
        call    BuildStrErr     ; get the name
        push    ss:MsgBuff+2    ; pass address of name
        push    ss:MsgBuff      ; ...
        mov     ax,SR_ARG_USED_NOT_PASSED
                                ; report the error
        jmp     RTError         ; ...
endproc BadParm


;
; Special FCodes to handle a one dimension array - generated by peephole
; optimizer
;

fcode   PH_1_SUBSCR
        getword DI              ; get the address of subscript variable
        push    2[DI]           ; push subscript on the stack
        push    0[DI]           ; ...
        hop     RT_1_SUBSCR     ; do subscript operation
efcode  PH_1_SUBSCR


fcode   PH_P1_SUBSCR            ; parameter subscripted variable (1 dim)
        getword DI              ; get the address of subscript variable
        push    2[DI]           ; push it on the stack
        push    0[DI]           ; ...
        hop     RT_P1_SUBSCR    ; do subscript operation
efcode  PH_P1_SUBSCR


fcode   PH_U1_SUBSCR
        mov     CX,4            ; set size of operand
        mov     BP,offset SSUnd ; set return address
        jmp     UndChk          ; check operand
SSUnd:  push    2[DI]           ; push it on the stack
        push    0[DI]           ; ...
        hop     RT_U1_SUBSCR    ; do subscript operation
efcode  PH_U1_SUBSCR


out_of_1_bound:
        jmp     far_out_of_1_bound


fcode   RT_P1_SUBSCR            ; parameter subscripted variable (1 dim)
        mov     DI,[SI]         ; get the address of the ADV
        mov     AX,adv_array+2[DI]  ; get the segment value of origin
        or      AX,adv_array[DI]; or in offset value of origin
        je      BadParm         ; report invalid parm if both NULL
;;;;;;;;hop     RT_U1_SUBSCR    ; do undef var check
efcode  RT_P1_SUBSCR


fcode   RT_U1_SUBSCR
        mov     BP,SP           ; point at subscript
        mov     AX,0[BP]        ; get low order word
        mov     word ptr SS:SSList+0,AX  ; put it in SSList location
        mov     AX,2[BP]        ; get high word of subscript
        mov     word ptr SS:SSList+2,AX  ; store it in SSList location
        mov     SS:FIELDREF,0   ; assume not a field ref
;;;;;;;;hop     RT_1_SUBSCR     ; do the subscripting
efcode  RT_U1_SUBSCR


fcode   RT_1_SUBSCR             ; do a one dimesional subscript
dfcode  FIELD_SUBSCR_1          ; field subscripting
        getword DI              ; get the ADV pointer
        mov     BP,SP           ; point at subscript
        mov     AX,0[BP]        ; get low word of subscript
        mov     DX,2[BP]        ; get high word of subscript
        sub     AX,word ptr adv_bound+0[DI]; subtract low word of lower bound
        sbb     DX,word ptr adv_bound+2[DI]; substract high word of lower bound
        jne     out_of_1_bound  ; report error if high word is not zero
        cmp     AX,word ptr adv_bound+4[DI]; check against number of elements
        jae     out_of_1_bound  ; report error if out of range
        mov     CH,byte ptr adv_numss+1[DI]; get shifting information for Scale
;;;;;;;;hop     Scale           ; jump to Scale
efcode  RT_1_SUBSCR


defn    Scale                   ; scale the offset in DX:AX by elt size
        mov     BX,adv_elsize[DI]; - get size of each element
        _guess                  ; guess
          and   CH,ADV_POW_OF_2 ; - see if element size is a power of 2
          _quif s               ; - quit if not
          _loop                 ; - loop
            _quif  e            ; - - quit if scaling is done
            shl    AX,1         ; - - scale by a power of 2
            rcl    DX,1         ; - - . . .
            dec    CH           ; - - one less power of 2 to do
          _endloop              ; - endloop
        _admit                  ; admit
          or    DX,DX           ; - test high order word of offset
          _quif e               ; - quit if high word is zero
          mov   CX,AX           ; - save low order word of offset
          mov   AX,DX           ; - get high order word of offset
          mul   BX              ; - calc. partial high order offset
          xchg  AX,CX           ; - get low order, save high order
          mul   BX              ; - multiply by # of elements
          add   DX,CX           ; - add in partial high order result
        _admit                  ; admit
          mul   BX              ; - multiply by # of elements
        _endguess               ; endguess
        mov     CL,byte ptr adv_numss[DI]; get the number of subscripts
        test    CL,ADV_FIELD    ; check if this is a field subscript op'n
        jne     FieldScale      ; if so, then go do something else
        and     CX,ADV_SUBSCRS  ; ...
        shl     CX,1            ; mutiply by 4
        shl     CX,1            ; . . .
        add     SP,CX           ; remove the subscripts from the stack
        mov     CX,BX           ; put element size in proper register
        add     AX,adv_array[DI]; add on start offset of array
        adc     DX,0            ; . . .
        mov     BX,AX           ; get low order word of offset
        shr     DX,1            ; shift offset right 4 bits to get
        rcr     BX,1            ; number of segments
        shr     DX,1            ; . . .
        rcr     BX,1            ; . . .
        shr     DX,1            ; . . .
        rcr     BX,1            ; . . .
        shr     DX,1            ; . . .
        rcr     BX,1            ; . . .
        add     BX,adv_array+2[DI]  ; calc segment addr nearest the element
        and     AX,000FH        ; isolate offset from segment
        mov     ES,BX           ; put segment addr in ES
        mov     DI,AX           ; put offset of array elt in DI
        next                    ; execute next opcode
endproc Scale


defn    FieldScale
        and     CX,ADV_SUBSCRS  ; remove subscripts from stack
        shl     CX,1            ; mutiply by 4
        shl     CX,1            ; . . .
        add     SP,CX           ; remove the subscripts from the stack
        mov     CX,BX           ; put length in proper register
        mov     BX,AX           ; save low word of offset into array
        getword AX              ; get offset of array in struct
        add     BX,AX           ; add to offset into array
        adc     DX,0            ; ...
        push    DX              ; push the calc'd offset
        push    BX              ; ...
        next
endproc FieldScale


fcode   PUSH_ARRAY_DATA         ; push array data on stack
dfcode  RT_CHAR_SUBSCR
                                ; and make up an SCB on the stack
        push    CX              ; length first
        push    ES              ; then segment address
        push    DI              ; then offset
        next                    ; execute next intstruction
efcode  PUSH_ARRAY_DATA


        fmodend
        end
