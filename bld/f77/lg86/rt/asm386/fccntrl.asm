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
; FCCNTRL       : flow control
;

.8087

include fcdef.inc
include errcod.inc
include datamap.inc

        xref    ChkLimitErr_
        xref    UnLinkSFTrace_
        xref    LinkSFTrace_

        extrn   RTError : near

        fmodstart fccntrl


fcode   BC_ALWAYS               ; jump to another instruction
        getdw   eax             ; get new address
        add     esi,eax         ; ...
        next                    ; goto next rtn
efcode  BC_ALWAYS


fcode   BC_TRUE                 ; jump if TRUE
        getdw   eax             ; get address of jump
        pop     edx             ; get result
        or      edx,edx         ; if result is true
        _if     ne              ; then
          add   esi,eax         ; - jump to new address
        _endif                  ; endif
        next                    ; goto next rtn
efcode  BC_TRUE


fcode   BC_FALSE                ; jump if FALSE
        getdw   eax             ; get address of jump
        pop     edx             ; get result
        or      edx,edx         ; if result is false
        _if     e               ; then
          add   esi,eax         ; - jump to new address
        _endif                  ; endif
        next                    ; goto next rtn
efcode  BC_FALSE


fcode   BC_EQ                   ; jump if op1 = op2
        getdw   eax             ; get target address
        _if     e               ; if op1 = op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_EQ


fcode   BC_NE                   ; jump if op1 <> op2
        getdw   eax             ; get target address
        _if     ne              ; if op1 <> op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_NE


fcode   BC_GT                   ; jump if op1 > op2
        getdw   eax             ; get target address
        _if     g               ; if op1 > op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_GT


fcode   BC_GE                   ; jump if op1 >= op2
        getdw   eax             ; get target address
        _if     ge              ; if op1 >= op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_GE


fcode   BC_LT                   ; jump if op1 < op2
        getdw   eax             ; get target address
        _if     l               ; if op1 < op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_LT


fcode   BC_LE                   ; jump if op1 <= op2
        getdw   eax             ; get target address
        _if     le              ; if op1 <= op2
          add   esi,eax         ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_LE


fcode   THREE_WAY_BRANCH        ; 3-way branch for arithmetic IF
        lea     esi,8[esi+edi*4]; point esi at dword following label
        add     esi,-4[esi]     ; add label offset
        next                    ; go to next routine
efcode  THREE_WAY_BRANCH


fcode   EXEC_RB                 ; execute a remote block
        getdw   eax             ; get the offset of the RB
                                ; first dword is ret addr storage loc
        add     eax,esi         ; convert offset to address
        cmp     dword ptr [eax],0; check if return address is NULL
        _if      ne             ; if it's not
           RTErr SP_RECURSE     ; - recursize call error
        _endif                  ; endif
        mov     [eax],esi       ; save current program counter
        lea     esi,4[eax]      ; set new program counter
        next                    ; execute first instr in remote block
efcode  EXEC_RB


fcode   END_RB                  ; return from a remote block
        getdw   eax             ; get pointer to the return addr loc
        xor     esi,esi         ; zero PC
        xchg    esi,[eax]       ; set the mem to 0 and get return addr
        next                    ; do next instr
efcode  END_RB


fcode   ASSIGN                  ; es:edi points to ASSIGN variable
        getdw   eax             ; get label offset
        add     eax,esi         ; convert into an address
        stosd                   ; store in variable
        next                    ; do next F-Code
efcode  ASSIGN


fcode   ASGN_GOTO               ; do an assigned goto without a list
        getdw   ebx             ; get table of branch addresses
        pop     edx             ; get the address to jump to
        add     ebx,esi
        mov     ecx,ST_NUMBER   ; set error message
        _loop                   ; loop
          mov   eax,[ebx]       ; - get statment offset
          or    eax,eax         ; - check for end of table
          je    not_label       ; - report error if label not found
          lea   eax,4[eax+ebx]  ; - calc statement addr
          cmp   eax,edx         ; - is this the right label?
          _quif  e              ; - quit if so
          add   ebx,6           ; - point at next label
        _endloop                ; - endloop
        mov     bx,4[ebx]       ; get block number of label
        cmp     esi,edx         ; check direction of branch
        _if     b               ; if a forward branch then
          cmp   bx,[esi]        ; - check label block against GOTO block
          ja    bad_branch      ; - report error if required
        _else                   ; else a backward branch
          _loop                 ; - loop
            getword ax          ; - - get block number
            cmp ax,bx           ; - - check for valid block
            _quif e             ; - - quit if OK
            or  ax,ax           ; - - check for end of table
            je  bad_branch      ; - - report error if at end of table
          _endloop              ; - endloop
        _endif                  ; endif
        mov     esi,edx         ; point at label address
        next                    ; execute next f-code
efcode  ASGN_GOTO


defn    bad_branch
        RTErr ST_BRANCH_INTO    ; report a branch into a structure
endproc bad_branch


fcode   ASGN_GOTO_WITH_LIST     ; do an assigned goto with a list
        pop     eax             ; get the address to jump to
        mov     ecx,ST_NUMBER   ; set error message
        call    StmtLook        ; check table to see if given addr
                                ; is a valid statement. If we return,
                                ; it is valid.
        mov     esi,eax         ; jump to new location
        next                    ; . . .
efcode  ASGN_GOTO_WITH_LIST


; This routine takes an address in eax
; and searches a table ( addressed by esi ) to see if the given address is
; in the table.  If the address is not found, RTErr is called which does
; a Suicide. This routine is called by AsgnGWithList and RTFmtCheck.

        xdefp   StmtLook
defn    StmtLook
        mov     edx,eax         ; save address in edx
        _loop                   ; loop
          lodsd                 ; - get element from list
          test  eax,eax         ; - check for end of list
          je    not_label       ; - report error if so
          add   eax,esi         ; - convert offset to abs address
          cmp   eax,edx         ; - compare against edx
        _until   e              ; until a match is found
        ret
endproc StmtLook


defn    not_label
        RTErr    ecx          ; report a stmt number not in list
endproc not_label


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>  SELECT statment processing
;<>         I2SelSngl - handles a single entry table with I*2 values
;<>         I2SelLH   - handles a low-high table with I*2 values
;<>         I4SelSngl - handles a single entry table with I*4 values
;<>         I4SelLH   - handles a low-high table with I*4 values
;<>
;<>  Proto-type code:
;<>            1st case addr
;<>            2nd case addr
;<>            3rd case addr
;<>                 .
;<>                 .
;<>                 .
;<>            nth case addr
;<>            otherwise addr
;<>            SELECT F-CODE
;<>            number of case statments ( has value n)
;<>            1st table entry
;<>            2nd table entry
;<>            3rd table entry
;<>                 .
;<>                 .
;<>                 .
;<>            nth table entry
;<>
;<> A table entry has four forms:
;<>
;<> offset | Single I*2 | Low-High I*2 | Single  I*4 |  Low-High I*4
;<>        |            |              |             |
;<>   0    | case value | case low val | case val (l)|  case low val (l)
;<>   2    |            | case high val| case val (h)|  case low val (h)
;<>   4    |            |              |             |  case high val (l)
;<>   6    |            |              |             |  case high val (h)
;<>
;<>
;<> where the case low value is the lowest value for that case
;<> and case high value is the highest value for that case
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   I2_SELECT_SINGLES       ; do an I*2 select table (single entries)
        lea     ebx,-FC_LEN[esi]; remember otherwise addr of jump table
        pop     edx             ; get control value
        getdw   ecx             ; get entry count
        movsx   eax,dx          ; sign extend dx into eax
        cmp     eax,edx         ; see if control value fits in 16-bits
        _if     e               ; if it does fit then scan table
          mov   edi,esi         ; point at the table of values
          inc   ecx             ; inc #entries since repne dec's too much
          repne scasw           ; find the data entry
          neg   ecx             ; negate the number of entries remaining
          lea   esi,[ebx+ecx*4] ; get addr of new PC
          add   esi,-4[esi]     ; ...
          next                  ; execute instr there
        _endif
        add     esi,-4[ebx]     ; get new PC address
        next                    ; execute instr there
efcode  I2_SELECT_SINGLES


fcode   I2_SELECT_LOW_HIGH      ; do an I*2 select table (low-high pairs)
        lea     ebx,-FC_LEN[esi]; remember otherwise addr of jump table
        pop     edx             ; get control value
        getdw   ecx             ; get entry count
        movsx   eax,dx          ; sign extend dx into eax
        cmp     eax,edx         ; check if control value fits in 16-bits
        _if     e               ; if control value fits in 16-bits
          _loop                 ; - loop
            cmp ax,2[esi]       ; - - check the high entry
            jle couldbe         ; - - check low entry if control <= high
            add esi,4           ; - - inc to next entry
            dec ecx             ; - - one fewer case
          _until e              ; - until ecx==0
        _endif
        mov     esi,ebx         ; no case match, take default
        add     esi,-4[ebx]     ; ...
        next
couldbe:                        ; we might have a case match !!!
        cmp     ax,[esi]        ; chk control value against low entry
        _if     l               ; if not a match
          sub   ecx,ecx         ; - zero cx to take otherwise case
        _endif                  ; endif
        neg     ecx             ; negate number of entries remaining
        lea     esi,[ebx+ecx*4] ; get new PC
        add     esi,-4[esi]     ; ...
        next                    ; and execute it
efcode  I2_SELECT_LOW_HIGH


fcode   I4_SELECT_SINGLES       ; do an I*4 select table (single entries)
        lea     ebx,-FC_LEN[esi]; remember otherwise addr of jump table
        getdw   eax             ; get entry count
        mov     edi,esi         ; point at the table of values
        lea     ecx,1[eax]      ; inc #entries since repne dec's too much
        pop     eax             ; get control value
        repne   scasd           ; find the data entry
        neg     ecx             ; negate the number of entries remaining
        lea     esi,[ebx+ecx*4] ; get new PC
        add     esi,-4[esi]     ; ...
        next
efcode  I4_SELECT_SINGLES


fcode   I4_SELECT_LOW_HIGH      ; do an I*4 select table (low-high pairs)
        lea     ebx,-FC_LEN[esi]; remember otherwise addr of jump table
        getdw   ecx             ; get entry count
        pop     eax             ; get control value
        add     esi,4           ; point at the first high value in table
findit4:                        ; loop
        cmp     eax,[esi]       ; - check the high entry
        jle     couldbe2        ; - check low entry if control <= high
        add     esi,8           ; - inc to next high entry
        loop    findit4         ; dec ecx and goto findit4 if ecx <> 0
        mov     esi,ebx         ; no case match, take default
        add     esi,-4[ebx]     ; ...
        next                    ; . . .
couldbe2:                       ; we might have a case match !!!
        cmp     eax,-4[esi]     ; chk control value against low entry
        _if     l               ; if not a match
          sub   ecx,ecx         ; - zero cx to take otherwise case
        _endif                  ; endif
        neg     ecx             ; negate number of entries remaining
        lea     esi,[ebx+ecx*4] ; get new PC
        add     esi,-4[esi]     ; ...
        next
efcode  I4_SELECT_LOW_HIGH


fcode   SELECT_SCB              ; for character SELECTs
        pop     edi             ; get offset
        pop     ecx             ; get length (throw away)
        movzx   eax,byte ptr [edi]; get low byte (sign extend)
        push    eax             ; and put it on stack for select fcode
        next
efcode  SELECT_SCB


fcode   SF_CALL                 ; call a statement function
        getdw   eax             ; get the address
        mov     sf_ret[eax],esi ; save current F-Code PC
        lea     esi,sf_strt[eax]; point to start of statement function
        docall  LinkSFTrace_    ; setup traceback
        next
efcode  SF_CALL


fcode   SF_RETURN               ; return from a statement function
        docall  ChkLimitErr_    ; check for limit error
        getdw   eax             ; get the addr of the start of the SF
        mov     esi,sf_ret[eax] ; set F-Code PC to return address
        docall  UnLinkSFTrace_  ; restore previous traceback
        next
efcode  SF_RETURN

        fmodend
        end
