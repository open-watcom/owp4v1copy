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
; FCSSCR        : subscripting routines
;

include fcdef.inc
include errcod.inc
include adv.inc

        xref    ltoa_

        extrn   RTError : near
        extrn   UndChk : near
        extrn   BuildStrErr : near

        extrn   _MsgBuff : dword

        dataseg

        SSList  DD      7 DUP(0) ; 7 32-bit words to save subscripts
        WarpRet dd      0        ; warp return address
        lgxdefp FIELDREF
FIELDREF        db      0

        enddata

        fmodstart       fcsscr

;
; ADV initialization routines
;

fcode   GO_WARP                 ; execute ADV init code
        getdw   eax             ; get the address to go to
        mov     WarpRet,esi     ; save warp return address
        add     esi,eax         ; set pc to init code
        next
efcode  GO_WARP


fcode   END_WARP                ; return from ADV init code
        mov     esi,WarpRet     ; get warp return address
        next
efcode  END_WARP


fcode   S_L7_FILL_IN            ; fill in the lower bound for index #7
        mov     cl,7            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L7_FILL_IN


fcode   S_L6_FILL_IN            ; fill in the lower bound for index #6
        mov     cl,6            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L6_FILL_IN


fcode   S_L5_FILL_IN            ; fill in the lower bound for index #5
        mov     cl,5            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L5_FILL_IN


fcode   S_L4_FILL_IN            ; fill in the lower bound for index #4
        mov     cl,4            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L4_FILL_IN


fcode   S_L3_FILL_IN            ; fill in the lower bound for index #3
        mov     cl,3            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L3_FILL_IN


fcode   S_L2_FILL_IN            ; fill in the lower bound for index #2
        mov     cl,2            ; set bound number
        hop     FillLow         ; fill in the lower bound
efcode  S_L2_FILL_IN


fcode   S_L1_FILL_IN            ; fill in the lower bound for index #1
        mov     cl,1            ; set bound number
;;;;;;;;hop     FillLow         ; fill in the lower bound
efcode  S_L1_FILL_IN


defn    FillLow                 ; fill in a lower bound
        getdw   eax             ; get the ADV offset
        add     eax,ebp         ; convert into abs address
        movzx   edx,byte ptr adv_numss[eax]; get number of subscripts in array
        and     dl,ADV_SUBSCRS  ; get rid of any shift information
        sub     dl,cl           ; get bound position index
        pop     dword ptr adv_bound[eax+edx*8]; pop into ADV
        next
endproc FillLow


fcode   S_H7_FILL_IN            ; fill in the upper bound for index #7
        mov     cl,7            ; set high bound number
        hop     FillHigh        ; fill in the upper bound
efcode  S_H7_FILL_IN


fcode   S_H6_FILL_IN            ; fill in the upper bound for index #6
        mov     cl,6            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H6_FILL_IN


fcode   S_H5_FILL_IN            ; fill in the upper bound for index #5
        mov     cl,5            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H5_FILL_IN


fcode   S_H4_FILL_IN            ; fill in the upper bound for index #4
        mov     cl,4            ; set high bound number
        hop     FillHigh        ; fill in the upper bound
efcode  S_H4_FILL_IN


fcode   S_H3_FILL_IN            ; fill in the upper bound for index #3
        mov     cl,3            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H3_FILL_IN


fcode   S_H2_FILL_IN            ; fill in the upper bound for index #2
        mov     cl,2            ; get low bound index offset
        hop     FillHigh        ; fill in the upper bound
efcode  S_H2_FILL_IN


fcode   S_H1_FILL_IN            ; fill in the upper bound for index #1
        mov     cl,1            ; get low bound index offset
;;;;;;;;hop     FillHigh        ; fill in the upper bound
efcode  S_H1_FILL_IN


defn    FillHigh                ; fill in an upper bound
        getdw   eax             ; get the ADV address
        add     eax,ebp         ; convert into absolute address
        movzx   edx,byte ptr adv_numss[eax]
                                ; get the number of subscripts
        and     dl,ADV_SUBSCRS  ; get rid of any shift information
        pop     ebx             ; get high bound
        sub     dl,cl           ; get bound position index
        lea     eax,adv_bound[eax+edx*8]
                                ; point at bound entry
        cmp     ebx,[eax]       ; check if high < low
        jl      baddim          ; issue error if high < low
        sub     ebx,[eax]       ; # of elements = high - low + 1
        inc     ebx             ; ...
        mov     4[eax],ebx      ; store # of elements in ADV
        next
endproc FillHigh


defn    baddim
        RTErr   SV_BAD_SSCR     ; report array incorrectly dimensioned
endproc baddim


fcode   RT_ADV_ASSUME           ; assumed dim array check
        getdw   eax             ; get the parm ADV address
        lea     ebx,[eax+ebp]   ; put pointer into ebx
        or      byte ptr adv_numss+1[eax+ebp],ADV_NOT_INIT; say not initialized
        call    GetSizePassed   ; return the size passed in eax
        push    eax             ; save size
        mov     dword ptr adv_bound+4[ebx],1; set last #elts to 1
        call    GetNElts        ; get the number of elements
        mul     dword ptr adv_elsize[ebx]       ; compute #elts * elt size
        mov     ecx,eax         ; save result
        pop     eax             ; get size passed
        xor     edx,edx         ; setup for division
        div     ecx             ; do the division
        or      eax,eax         ; see if #elts in last dim is zero
        je      TooBig          ; report arg array too big if required
        mov     ecx,eax         ; save last #elts
        mov     dword ptr adv_bound+4[ebx],eax; set last #elts bound
        mov     eax,adv_elnum[ebx]; get true number of elements
        mul     ecx             ; multiply
        mov     adv_elnum[ebx],eax; save number of elements
        and     byte ptr adv_numss+1[ebx],ADV_INIT_MSK; adv initialized
        next
efcode  RT_ADV_ASSUME


fcode   RT_ADV_VAR              ; variable dimension check
        mov     ebx,[esi]       ; get the parm ADV address
        add     ebx,ebp         ; convert into abs address
        call    GetNElts        ; set the number of elements
;;;;;;;;hop     RT_ADV_CONST
efcode  RT_ADV_VAR


fcode   RT_ADV_CONST            ; constant arg array check
        getdw   ebx             ; get ADV address
        add     ebx,ebp         ; convert into abs address
        or      byte ptr adv_numss+1[ebx],ADV_NOT_INIT; say not initialized
        call    GetSizePassed   ; get the array size passed
        mov     ecx,eax         ; save the parm array total size
        mov     eax,adv_elnum[ebx]; get arg ADV num_elts
        mul     dword ptr adv_elsize[ebx]       ; multiply by arg element size
        cmp     eax,ecx         ; check size of arrays
        ja      TooBig          ; report error if required
        and     byte ptr adv_numss+1[ebx],ADV_INIT_MSK; adv initialized
        next
efcode  RT_ADV_CONST


defn    TooBig
        xor     eax,eax         ; indicate not a field ref
        call    BuildStrErr     ; . . .
        push    _MsgBuff        ; push address of array name
        RTErr   DM_RANGE_ERR_ON_DIMN; report arg array is too big
endproc TooBig


defn    GetSizePassed           ; get the size of array that was passed
        mov     edi,adv_link[ebx]; get parm ADV address
        or      edi,edi         ; check if ADV passed is NULL
        _if     e               ; if it is (array from external s.p.)
          mov   eax,07fffffffH  ; - load eax with maxint
          ret                   ; - return
        _endif                  ; endif
        mov     eax,adv_elnum[edi]; get no. of elts in parm array
        mov     ecx,adv_array[ebx]; get arg array start offset
        mul     dword ptr adv_elsize[edi]       ; #elts * elt size
        sub     ecx,adv_array[edi]; subtract parm array start offset
        sub     eax,ecx         ; calculate total size passed
        ret
endproc GetSizePassed


defn    GetNElts                ; get the number of elements in an array
        lea     edi,adv_bound+4[ebx]; point at first hi - low
        movzx   ecx,byte ptr adv_numss[ebx]; get the number of subscripts
        mov     eax,1           ; init number of elements count
        and     cl,ADV_SUBSCRS  ; setup ecx properly
repeat:                         ; loop
          mul   dword ptr [edi] ; - update running total
          add   edi,8           ; - point at next (hi-lo)
        loop    repeat          ; - dec #sub's left to do, loop if more
        mov     adv_elnum[ebx],eax; set the # of elements in the array
        ret
endproc GetNElts


;
; Subscripting routines
;

        xdefp   UndefArray
defn    UndefArray              ; build error msg for undef array elt
        mov     eax,offset32 SSList; point to subscript list
MkSubLst:                       ; form subscript list error message
        push    eax             ; save offset of the subscript list
        push    edi             ; save the ADV offset
        mov     ebx,edi         ; pass pointer to item in ebx
        xor     eax,eax         ; delay processing of field information
        call    BuildStrErr     ; put the array name in MsgBuff
        pop     ebx             ; restore the ADV offset
        xchg    esi,[esp]       ; get ptr to subscript list & save f-code PC
        mov     al,'('          ; put a left paren in AL
        stosb                   ; store it
        movzx   ecx,byte ptr adv_numss[ebx]; get the number of subscripts
        and     cl,ADV_SUBSCRS  ; mask off extra bits
        lea     esi,-4[esi+ecx*4]; point to the first subscript
        _loop                   ; loop : put the subscripts in MsgBuff
          mov   eax,[esi]       ; - get subscript
          mov   edx,edi         ; - point to buffer
          mov   ebx,10          ; - set radix
          docall  ltoa_         ; - convert the subscript on the stack
          xor   al,al           ; - get a zero byte
          _loop                 ; - loop
            scasb               ; - - scan a byte of output
            _quif e             ; - - quit if NULLCHAR found
          _endloop              ; - endloop
          dec   edi             ; - went one byte too far
          sub   esi,4           ; - point to next subscript in SSList
          dec   ecx             ; - decrement subscript counter
          _quif e               ; - quit if all subscrs done
          mov   al,','          ; - seperate the subscripts by a comma
          stosb                 ; - ...
        _endloop                ; until no subscripts left
        mov     al,')'          ; store right parenthesis
        stosb                   ; ...
        cmp     byte ptr FIELDREF,0
        _if     ne              ; if it was a field reference
          mov   al,'.'          ; - indicate field reference
          mov   ah,'?'          ; - ...
          stosw                 ; - ...
        _endif                  ; endif
        xor     al,al           ; store NULLCHAR
        stosb                   ; ...
        pop     esi             ; restore current fcode offset
        ret
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
;<>     output:
;<>             edi - offset from of the subscripted element
;<>             ecx - size of the element (used by character subscript)
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   RT_PSUBSCRIPT           ; parameter subscripted variable
        mov     edi,[esi]       ; get the address of the ADV
        mov     eax,adv_array[edi+ebp]; get the offset of the origin
        or      eax,eax         ; check if NULL
        je      BadParm         ; report invalid parm
;;;;;;;;hop     RT_USUBSCRIPT   ; do undef var check
efcode  RT_PSUBSCRIPT


fcode   RT_USUBSCRIPT
        mov     ebx,[esi]       ; get ADV addr
        mov     eax,esp         ; point to subscr's on stack
        movzx   ecx,byte ptr adv_numss[ebx+ebp]; get number of subscripts
        mov     edi,offset32 SSList; set edi to subscript buffer
        and     cl,ADV_SUBSCRS  ; setup number of subscripts
        xchg    esi,eax         ; save fcode IP and get subscr ptr into esi
        rep     movsd           ; copy subscripts into SSList
        mov     esi,eax         ; restore fcode ptr
;;;;;;;;hop     RT_SUBSCRIPT    ; do the subscripting
efcode  RT_USUBSCRIPT


fcode   RT_SUBSCRIPT            ; run-time subscripting routine
dfcode  FIELD_SUBSCR            ; field subscripting routine
        getdw   eax             ; get addr of ADV
        add     eax,ebp
;;;;;;;;hop     DbSScr
efcode  RT_SUBSCRIPT


        xdefp   DbSScr
defn    DbSScr                  ; debugger subscripting entry point
        mov     byte ptr FIELDREF,0; assume it's not a field ref
        mov     edi,eax         ; get addr of ADV
        push    ebp             ; save ebp
        mov     ecx,dword ptr adv_numss[edi]; get # of subscripts to process
                                ; (and extra bits)
        lea     ebp,4[esp]      ; setup ebp to access subscrs on stack
        push    edi             ; save address of ADV
        and     cl,ADV_SUBSCRS  ; extract subscr field
        lea     edi,adv_bound[edi]; point to first of subscript bounds
        xor     eax,eax         ; set offset to 0
        _loop                   ; loop (for each subscript) (pipelined)
          mov   ebx,[ebp]       ; - get subscript
          add   ebp,4           ; - point to next subscript on stack
          sub   ebx,[edi]       ; - subtract low bound
          add   edi,8           ; - point to next subscript bounds
          add   eax,ebx         ; - add this subscript to prev result
          cmp   ebx,-4[edi]     ; - check to see if out of range
          jae   out_of_bounds   ; - quit if out of range
          dec   cl              ; - decrement # of subscripts to do
          _quif e               ; - quit if done
          mul   dword ptr 4[edi]; - multiply by # of elements in this dim
        _endloop                ; endloop
        pop     edi             ; restore address of ADV
        pop     ebp             ; restore ebp
        hop     Scale           ; scale offset by element size
endproc DbSScr


defn    BadParm                 ; report a bad parameter array
        mov     ebx,edi         ; pass address of item in ebx
        xor     eax,eax         ; indicate not a field ref
        call    BuildStrErr     ; get the name
        push    _MsgBuff        ; pass address of name
        mov     eax,SR_ARG_USED_NOT_PASSED; report the error
        jmp     RTError         ; ...
endproc BadParm


defn    out_of_bounds           ; subscript out of range error
        pop     edi             ; get the adv address
        pop     ebp             ; restore ebp
out_of_1_bound:                 ; entry point for 1-D subscripting rtns
        mov     eax,esp         ; point at subscript list
        call    MkSubLst        ; build the error message parm
        push    _MsgBuff        ; pass address of name
        mov     eax,SS_SSCR_RANGE; set the error code
        jmp     RTError         ; report the error
endproc out_of_bounds


;
; Special FCodes to handle a one dimension array - generated by peephole
; optimizer
;

fcode   PH_1_SUBSCR
        getdw   eax             ; get the address of subscript variable
        push    [eax+ebp]       ; push subscript on the stack
        hop     RT_1_SUBSCR     ; do subscript operation
efcode  PH_1_SUBSCR


fcode   PH_P1_SUBSCR            ; parameter subscripted variable (1 dim)
        getdw   eax             ; get the address of subscript variable
        push    [eax+ebp]       ; push it on stack
        hop     RT_P1_SUBSCR    ; do subscript operation
efcode  PH_P1_SUBSCR


fcode   PH_U1_SUBSCR
        mov     ecx,4           ; set size of operand
        mov     ebx,offset32 SSUnd; set return address
        jmp     UndChk          ; check operand
SSUnd:
        push    [edi]           ; push it on the stack
        hop     RT_U1_SUBSCR    ; do subscript operation
efcode  PH_U1_SUBSCR


fcode   RT_P1_SUBSCR            ; parameter subscripted variable (1 dim)
        mov     edi,[esi]       ; get the address of the ADV
        mov     eax,adv_array[edi+ebp]; get offset value of origin
        or      eax,eax         ; test if NULL
        je      BadParm         ; report invalid parm if both NULL
;;;;;;;;hop     RT_U1_SUBSCR    ; do undef var check
efcode  RT_P1_SUBSCR


fcode   RT_U1_SUBSCR
        mov     eax,[esp]       ; get subscript
        mov     byte ptr FIELDREF,0; assume it's not a field reference
        mov     SSList,eax      ; put it in SSList location
;;;;;;;;hop     RT_1_SUBSCR     ; do the subscripting
efcode  RT_U1_SUBSCR


fcode   RT_1_SUBSCR             ; do a one dimesional subscript
dfcode  FIELD_SUBSCR_1
        getdw   edi             ; get the ADV pointer
        add     edi,ebp
        mov     eax,[esp]       ; get subscript
        sub     eax,dword ptr adv_bound[edi]; subtract lower bound
        cmp     eax,dword ptr adv_bound+4[edi]; check against number of elements
        jae     out_of_1_bound  ; report error if out of range
        mov     ch,byte ptr adv_numss+1[edi]; get shifting information for Scale
;;;;;;;;hop     Scale           ; jump to Scale
efcode  RT_1_SUBSCR


defn    Scale                   ; scale the offset in DX:AX by elt size
        mov     ebx,adv_elsize[edi]; get size of each element
        _guess                  ; guess
          and   ch,ADV_POW_OF_2 ; - see if element size is a power of 2
          _quif s               ; - quit if not
          mov   cl,ch           ; - move into proper register
          shl   eax,cl          ; - scale by a power of 2
        _admit                  ; admit
          mul   ebx             ; - multiply by size of each element
        _endguess               ; endguess
        mov     ecx,ebx         ; put element size in proper register
        movzx   ebx,byte ptr adv_numss[edi]; get the number of subscripts
        test    bl,ADV_FIELD    ; check if field subscripting
        jne     FieldScale      ; if so, finish up field subscript
        add     eax,adv_array[edi]; add on start offset of array
        and     bl,ADV_SUBSCRS  ; extract subscript field
        mov     edi,eax         ; move array element pointer into proper reg
        lea     esp,[esp+ebx*4] ; remove subscripts from the stack
        next
endproc Scale


defn    FieldScale
        and     bl,ADV_SUBSCRS  ; remove subscripts from stack
        lea     esp,[esp+ebx*4] ; ...
        mov     ebx,eax         ; save low word of offset into array
        getdw   eax             ; get offset of array in struct
        add     ebx,eax         ; add to offset into array
        push    ebx             ; push result onto stack
        next
endproc FieldScale


fcode   PUSH_ARRAY_DATA         ; push array data on stack
dfcode  RT_CHAR_SUBSCR          ; and make up an SCB on the stack
        push    ecx             ; length first
        push    edi             ; then offset
        next
efcode  PUSH_ARRAY_DATA

        fmodend
        end
