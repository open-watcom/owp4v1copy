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
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
ifndef __WASM__
  include struct.inc
endif
include errcod.inc
include datamap.inc
include xfflags.inc

        extrn   FCLimErr : near
        extrn   RTError : near

        fmodstart fccntrl

        dataseg

        extrn   _ExLinePtr : word
        extrn   _ExCurr : word

        enddata


fcode   BC_ALWAYS               ; jump to another instruction
        getword SI              ; get new address
        next                    ; goto next rtn
efcode  BC_ALWAYS


fcode   BC_TRUE                 ; jump if TRUE
        lodsw                   ; get address of jump
        pop     CX              ; get result
        pop     BX              ; . . .
        or      CX,BX           ; if result is true
        _if     ne              ; then
          mov   SI,AX           ; - jump to new address
        _endif                  ; endif
        next                    ; goto next rtn
efcode  BC_TRUE


fcode   BC_FALSE                ; jump if FALSE
        lodsw                   ; get address of jump
        pop     CX              ; get result
        pop     BX              ; . . .
        or      CX,BX           ; if result is FALSE
        _if      e              ; then
          mov   SI,AX           ; - jump to new address
        _endif                  ; endif
        next                    ; goto next rtn
efcode  BC_FALSE


fcode   BC_EQ                   ; jump if op1 = op2
        lodsw                   ; get target address
        _if      e              ; if op1 = op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_EQ


fcode   BC_NE                   ; jump if op1 <> op2
        lodsw                   ; get target address
        _if      ne             ; if op1 <> op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_NE


fcode   BC_GT                   ; jump if op1 > op2
        lodsw                   ; get target address
        _if      g              ; if op1 > op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_GT


fcode   BC_GE                   ; jump if op1 >= op2
        lodsw                   ; get target address
        _if      ge             ; if op1 >= op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_GE


fcode   BC_LT                   ; jump if op1 < op2
        lodsw                   ; get target address
        _if      l              ; if op1 < op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_LT


fcode   BC_LE                   ; jump if op1 <= op2
        lodsw                   ; get target address
        _if      le             ; if op1 <= op2
          mov   SI,AX           ; - jump to target
        _endif                  ; endif
        next                    ; execute next f-code
efcode  BC_LE


fcode   THREE_WAY_BRANCH        ; 3-way branch for arithmetic IF
        add     DI,DI           ; change result of compare to -2, 0, +2
        add     SI,DI           ; add offset to table address
        mov     SI,2[SI]        ; get addr of label
        next                    ; go to next routine
efcode  THREE_WAY_BRANCH


fcode   EXEC_RB                 ; execute a remote block
        getword bx              ; get the address of the RB
                                ; first word is ret addr storage loc
        cmp     word ptr 0[bx],0; check if return address is NULL
        _if      ne             ; if it's not
           RTErr SP_RECURSE      ; - recursize call error
        _endif                  ; endif
        mov     [bx],si         ; save current program counter
        lea     si,2[bx]        ; set new program counter
        next                    ; execute first instr in remote block
efcode  EXEC_RB


fcode   END_RB                  ; return from a remote block
        getword bx              ; get the addr of the return addr loc
        mov     si,[bx]         ; set the PC to the return addr
        mov     word ptr 0[bx],0; we're done executing this block
        next                    ; do next instr
efcode  END_RB


fcode   ASSIGN                  ; ES:DI points to ASSIGN variable
        getword ax              ; get label address
        stosw                   ; store in low word of variable
        sub     ax,ax           ; zero high word of variable
        stosw                   ; ...
        next                    ; do next F-Code
efcode  ASSIGN


fcode   ASGN_GOTO               ; do an assigned goto without a list
        getword bx              ; get table of branch addresses
        pop     dx              ; get the address to jump to
        pop     ax              ; get the high word of variable
        mov     cx,ST_NUMBER    ; set error message
        or      ax,ax           ; check high word of address
        jne     not_label       ; report error if not zero
        _loop                   ; loop
          mov   ax,[bx]         ; - get statment address
          or    ax,ax           ; - check for end of table
          je    not_label       ; - report error if label not found
          cmp   ax,dx           ; - is this the right label?
          _quif  e              ; - quit if so
          add   bx,4            ; - point at next label
        _endloop                ; - endloop
        mov     bx,2[bx]        ; get block number of label
        cmp     si,ax           ; check direction of branch
        _if     b               ; if a forward branch then
          cmp   bx,[si]         ; - check label block against GOTO block
          ja    bad_branch      ; - report error if required
        _else                   ; else a backward branch
          _loop                 ; - loop
            lodsw               ; - - get block number
            cmp ax,bx           ; - - check for valid block
            _quif e             ; - - quit if OK
            or  ax,ax           ; - - check for end of table
            je  bad_branch      ; - - report error if at end of table
          _endloop              ; - endloop
        _endif                  ; endif
        mov     si,dx           ; point at label address
        next                    ; execute next f-code
efcode  ASGN_GOTO


defn    bad_branch
        RTErr ST_BRANCH_INTO    ; report a branch into a structure
endproc bad_branch


fcode   ASGN_GOTO_WITH_LIST     ; do an assigned goto with a list
        pop     ax              ; get the address to jump to
        pop     dx              ; get the high word of variable
        mov     CX,ST_NUMBER    ; set error message
        call    StmtLook        ; check table to see if given addr
                                ; is a valid statement. If we return,
                                ; it is valid.
        mov   si,ax             ; jump to new location
        next                    ; . . .
efcode  ASGN_GOTO_WITH_LIST


; This routine takes an address in DX ( high word ) and AX ( low word )
; and searches a table ( addressed by SI ) to see if the given address is
; in the table.  If the address is not found, RTErr is called which does
; a Suicide. This routine is called by AsgnGWithList and RTFmtCheck.

        xdefp   StmtLook
defn    StmtLook
        or      dx,dx           ; make sure high word is zero
        jne     not_label       ; report a bad stmt number if not
        mov     di,si           ; get a pointer to the address list
        mov     bx,ds           ; set the extra segment to the
        mov     es,bx           ; data segment
        _loop                   ; loop
          cmp   word ptr 0[di],0; - check for end of list
          je    not_label       ; - report error if so
          scasw                 ; - see if addr matches one in list
        _until   e              ; until a match is found
        ret
endproc StmtLook


defn    not_label
        RTErr    CX          ; report a stmt number not in list
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
;<>            2cd case addr
;<>            3d  case addr
;<>                 .
;<>                 .
;<>                 .
;<>            nth case addr
;<>            otherwise addr
;<>            SELECT F-CODE
;<>            number of case statments ( has value n)
;<>            1st table entry
;<>            2cd table entry
;<>            3d  table entry
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
        lea     bx,-4[si]       ; remember otherwise addr of jump table
        getword cx              ; get entry count
        pop     ax              ; get low order word of control value
        pop     di              ; get high order word of control value
        cwd                     ; sign extend low word
        cmp     dx,di           ; see if control value fits in 16-bits
        _if     e               ; if it does then
          mov     dx,ds         ; - set the extra segment to the data seg
          mov     es,dx         ; - . . .
          mov     di,si         ; - point at the table of values
          inc     cx            ; - inc #entries since repne dec's too much
          repne   scasw         ; - find the data entry
          add     cx,cx         ; - double number of entries remaining
          sub     bx,cx         ; - find the jump table entry
        _endif
        mov     si,[bx]         ; get new PC address
        next                    ; execute instr there
efcode  I2_SELECT_SINGLES


fcode   I2_SELECT_LOW_HIGH      ; do an I*2 select table (low-high pairs)
        lea     bx,-4[si]       ; remember otherwise addr of jump table
        getword cx              ; get entry count
        pop     ax              ; get low order word of control value
        pop     di              ; get high order word of control value
        cwd                     ; sign extend low word
        cmp     dx,di           ; see if control value fits in 16-bits
        jne     nocase2         ; if not then no match no matter what
        add     si,2            ; point at the first high value in table
findit:                         ; loop
        cmp     ax,[si]         ; - check the high entry
        jle     couldbe         ; - check low entry if control <= high
        add     si,4            ; - inc to next high entry
        loop    findit          ; dec cx and goto findit if cx <> 0
nocase2:
        mov     si,[bx]         ; no case match, take default
        next                    ; . . .
couldbe:                        ; we might have a case match !!!
        cmp     ax,-2[si]       ; chk control value against low entry
        _if      l              ; if not a match
          sub   cx,cx           ; - zero cx to take otherwise case
        _endif                  ; endif
        add     cx,cx           ; double number of entries remaining
        sub     bx,cx           ; get pointer to jump address
        mov     si,[bx]         ; set PC to first instr in block
        next                    ; and execute it
efcode  I2_SELECT_LOW_HIGH


fcode   I4_SELECT_SINGLES       ; do an I*4 select table (single entries)
        lea     bx,-4[si]       ; remember otherwise addr of jump table
        getword cx              ; get entry count
        pop     ax              ; get low order word of control value
        pop     dx              ; get high order word of control value
        mov     di,ds           ; set the extra segment to the data seg
        mov     es,di           ; . . .
        mov     di,si           ; point at the table of values
findit2:                        ; loop
        scasw                   ; - check the low word and pnt at high
        je      couldbe2        ; - check high word if low words equal
continu:
        inc     di              ; - inc past high word of entry
        inc     di              ; - . . .
        loop    findit2         ; dec cx and goto findit if cx <> 0
        mov     si,[bx]         ; no case match, take default
        next                    ; . . .
couldbe2:                       ; we might have a case match !!!
        cmp     dx,[di]         ; chk control value high word
        jne     continu         ; try next entry if not a match
        add     cx,cx           ; double number of entries remaining
        sub     bx,cx           ; get pointer to jump address
        mov     si,[bx]         ; set PC to first instr in block
        next                    ; and execute it
efcode  I4_SELECT_SINGLES


fcode   I4_SELECT_LOW_HIGH      ; do an I*4 select table (low-high pairs)
        lea     bx,-4[si]       ; remember otherwise addr of jump table
        getword cx              ; get entry count
        pop     ax              ; get low order word of control value
        pop     dx              ; get high order word of control value
        add     si,4            ; point at first high value in table
findit3:                        ; loop
        cmp     dx,2[si]        ; - check high order words
        _if      e              ; - if high words equal
          cmp   ax,0[si]        ; - - check low words
        _endif                  ; - endif
        jle     couldbe3        ; - possible if control <= table high val
        add     si,8            ; - point at next entry
        loop    findit3         ; endloop
        mov     si,[bx]         ; no case match, take default
        next                    ; . . .
couldbe3:                       ; we might have a case match !!!
        cmp     dx,-2[si]       ; is control < table low value
        _if      e              ; if high words equal
          cmp   ax,-4[si]       ; - check low words
        _endif                  ; endif
        _if      l              ; if no match
          sub   cx,cx           ; - zero cx to take otherwise case
        _endif                  ; endif
        add     cx,cx           ; double number of entries remaining
        sub     bx,cx           ; get pointer to jump address
        mov     si,[bx]         ; set F-Code PC to first instruction in block
        next                    ; and execute it
efcode  I4_SELECT_LOW_HIGH


fcode   SELECT_SCB              ; for character SELECTs
        pop     di              ; get offset
        pop     es              ; get seg
        pop     cx              ; get length (throw away)
        mov     al,es:[di]      ; get low byte
        xor     ah,ah           ; zero low word high byte
        xor     dx,dx           ; zero high word
        push    dx              ; push high word
        push    ax              ; push low word
        next
efcode  SELECT_SCB


fcode   SF_CALL                 ; call a statement function
        getword BX              ; get the address
        mov     SF_RET[BX],SI   ; save current F-Code PC
        mov     AX,SS:_ExCurr   ; save pointer to previous traceback
        mov     word ptr SF_LINK[BX],AX  ; ...
        mov     AX,SS:_ExCurr+2 ; ...
        mov     word ptr SF_LINK+2[BX],AX; ...
        mov     AX,SS:_ExLinePtr; save pointer to prev. line
        mov     SF_LINEPTR[BX],AX  ; ...
        lea     DI,SF_TB[BX]    ; point to current traceback
        mov     SS:_ExCurr,DI   ; point to current traceback
        mov     SS:_ExCurr+2,DS ; ...
        lea     SI,SF_STRT[BX]  ; point to start of statement function
        next                    ; execute it
efcode  SF_CALL


fcode   SF_RETURN               ; return from a statement function
        call  SFLimErr          ; check for limit error
        getword BX              ; get the addr of the start of the SF
        mov     AX,word ptr SF_LINK[BX]  ; get offset to previous traceback
        mov     SS:_ExCurr,AX   ; . . .
        mov     AX,word ptr SF_LINK+2[BX]; get segment of previous traceback
        mov     SS:_ExCurr+2,AX ; . . .
        mov     AX,SF_LINEPTR[BX]  ; restore offset of previous line
        mov     SS:_ExLinePtr,AX; . . .
        mov     SI,SF_RET[BX]   ; set F-Code PC to return address
        next                    ; execute next instruction
efcode  SF_RETURN


        xdefp   SFLimErr
defn    SFLimErr                        ; check for limit error after S.F.
        test    SS:__XcptFlags,XF_LIMIT_ERR
        _if      e                      ; if no error
          ret                           ; - return
        _endif                          ; endif
        jmp     FCLimErr                ; report error
endproc SFLimErr

        fmodend
        end
