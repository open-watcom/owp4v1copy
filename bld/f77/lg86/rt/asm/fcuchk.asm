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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     FORTRAN-77 run-time undefined variable checking push routines
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc
include errcod.inc
include undef.inc
include parmtype.inc
include datamap.inc
include structio.inc

        fcxref  VAL_LOG1
        fcxref  VAL_LOG4
        fcxref  VAL_INT1
        fcxref  VAL_INT2
        fcxref  VAL_INT4
        fcxref  VAL_REAL4
        fcxref  VAL_REAL8
        fcxref  VAL_REAL16
        fcxref  VAL_CPLX8
        fcxref  VAL_CPLX16
        fcxref  VAL_CPLX32
        fcxref  PUSH_SCB

        extrn   RTError         : near
        extrn   UndefArray      : near

        extrn   _SizeVars       : byte

        fmodstart       fcuchk

        dataseg

        extrn   _MsgBuff        : word
        extrn   _ExCurr         : dword

        public  __RCBOffset
__RCBOffset     dw      ?

        enddata


fcode   UPSH_LOG1
        mov     BP,offset VAL_LOG1    ; get address of push routine
        mov     CX,1                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_LOG1


fcode   UPSH_LOG4
        mov     BP,offset VAL_LOG4    ; get address of push routine
        mov     CX,4                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_LOG4


fcode   UPSH_INT1
        mov     BP,offset VAL_INT1    ; get address of push routine
        mov     CX,1                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_INT1


fcode   UPSH_INT2
        mov     BP,offset VAL_INT2    ; get address of push routine
        mov     CX,2                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_INT2


fcode   UPSH_INT4
        mov     BP,offset VAL_INT4    ; get address of push routine
        mov     CX,4                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_INT4


fcode   UPSH_REAL4
        mov     BP,offset VAL_REAL4   ; get address of push routine
        mov     CX,4                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_REAL4


fcode   UPSH_REAL8
        mov     BP,offset VAL_REAL8   ; get address of push routine
        mov     CX,8                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_REAL8


fcode   UPSH_REAL16
        mov     BP,offset VAL_REAL16  ; get address of push routine
        mov     CX,16                 ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_REAL16


fcode   UPSH_CPLX8
        mov     BP,offset VAL_CPLX8   ; get address of push routine
        mov     CX,8                  ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_CPLX8


fcode   UPSH_CPLX16
        mov     BP,offset VAL_CPLX16  ; get address of push routine
        mov     CX,16                 ; get length of value
        hop     UndChk                ; check if undefined
efcode  UPSH_CPLX16


fcode   UPSH_CPLX32
        mov     BP,offset VAL_CPLX32  ; get address of push routine
        mov     CX,32                 ; get length of value
;;;;;;;;hop     UndChk                ; check if undefined
efcode  UPSH_CPLX32


;
; ENTRY: CX - length of value
;        BP - routine to call if defined
;
        xdefp   UndChk
defn    UndChk
        getword DI                    ; point to value
        mov     AX,DS                 ; get segment address in ES
        mov     ES,AX                 ; . . .
        mov     BX,DI                 ; save pointer to name
        hop     UndChk2               ; see if it's undefined
endproc UndChk


fcode   UPSH_IND_LOG1
        mov     CX,1                  ; get length of value
        mov     BP,offset VAL_LOG1    ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_LOG1


fcode   UPSH_IND_LOG4
        mov     CX,4                  ; get length of value
        mov     BP,offset VAL_LOG4    ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_LOG4


fcode   UPSH_IND_INT1
        mov     CX,1                  ; get length of value
        mov     BP,offset VAL_INT1    ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_INT1


fcode   UPSH_IND_INT2
        mov     CX,2                  ; get length of value
        mov     BP,offset VAL_INT2    ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_INT2


fcode   UPSH_IND_INT4
        mov     CX,4                  ; get length of value
        mov     BP,offset VAL_INT4    ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_INT4


fcode   UPSH_IND_REAL4
        mov     CX,4                  ; get length of value
        mov     BP,offset VAL_REAL4   ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_REAL4


fcode   UPSH_IND_REAL8
        mov     CX,8                  ; get length of value
        mov     BP,offset VAL_REAL8   ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_REAL8


fcode   UPSH_IND_REAL16
        mov     CX,16                  ; get length of value
        mov     BP,offset VAL_REAL16   ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_REAL16


fcode   UPSH_IND_CPLX8
        mov     CX,8                  ; get length of value
        mov     BP,offset VAL_CPLX8   ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_CPLX8


fcode   UPSH_IND_CPLX16
        mov     CX,16                 ; get length of value
        mov     BP,offset VAL_CPLX16  ; get address of push routine
        hop     UndChk1               ; check if undefined
efcode  UPSH_IND_CPLX16


fcode   UPSH_IND_CPLX32
        mov     CX,32                 ; get length of value
        mov     BP,offset VAL_CPLX32  ; get address of push routine
;;;;;;;;hop     UndChk1               ; check if undefined
efcode  UPSH_IND_CPLX32


;
; ENTRY: CX - length of value
;        BP - routine to call if defined
;

defn    UndChk1
        getword BX                    ; point to value
        les     DI,0[BX]              ; . . .
        mov     AX,ES                 ; get segment value
        or      AX,DI                 ; see if address is NULL
        je      BadParm               ; report invalid parameter
;;;;;;;;hop     UndChk2
endproc UndChk1


defn    UndChk2
        mov     AL,U_BYTE             ; get undefined character
        mov     DX,DI                 ; save DI since scasb auto incrs.
        repz    scasb                 ; scan for U_BYTE
        _if     ne                    ; if not undefined
          mov   DI,DX                 ; - restore DI
          jmp   BP                    ; - push the operand
        _endif                        ; endif
        xor     ax,ax                 ; not a field ref
uv_undefd_err:
        call    BuildStrErr           ; build the name
        push    SS:_MsgBuff+2         ; pass address of name
        push    SS:_MsgBuff           ; ...
        mov     AX,UV_UNDEFD          ; set undefined error message
        jmp     RTError               ; report the error
endproc UndChk2


;
;       bad parm checking
;
defn    BadParm
        xor     ax,ax                   ; not a field ref
        call    BuildStrErr             ; build the error message
        push    SS:_MsgBuff+2           ; pass address of name
        push    SS:_MsgBuff             ; ...
        mov     AX,SR_ARG_USED_NOT_PASSED
        jmp     RTError                 ; report the error
endproc BadParm


fcode   PUSH_PARM_SCB           ; push a parameter SCB
dfcode  PUSH_PARM_RCB           ; push a parameter RCB
        getword BX              ; get address of SCB
        mov     AX,0[BX]        ; get offset
        mov     DX,2[BX]        ; get segment
        mov     CX,AX           ; save offset
        or      AX,DX           ; check to see if offset and seg are NULL
        je      BadParm         ; report error if so
        push    4[BX]           ; push length   (or structure def'n)
        push    DX              ; push segment
        push    CX              ; push offset
        next                    ; do next f-code
efcode  PUSH_PARM_SCB


fcode   CHK_VALID_PARM          ; make sure that parm address is valid
        getword BX              ; get the pointer to the parm address
        mov     AX,0[BX]        ; get the offset value
        or      AX,2[BX]        ; or in the segment value
        je      BadParm         ; report bad parm if both are NULL
        next                    ; execute next f-code
efcode  CHK_VALID_PARM          ; make sure that parm address is valid


;
; undefined checking for structure fields
;
defn    UndChk3
        mov     AL,U_BYTE             ; get undefined character
        mov     BX,DI                 ; save DI since scasb auto incrs.
        repz    scasb                 ; scan for U_BYTE
        _if     ne                    ; if not undefined
          mov   DI,BX                 ; - restore DI
          jmp   BP                    ; - push the operand
        _endif                        ; endif
        mov     bx,SS:__RCBOffset     ; point to RCB
        mov     ax,1                  ; indicate it's a field ref
        jmp     uv_undefd_err
endproc UndChk3


fcode   UVALF_LOG1
        mov     bp,offset VAL_LOG1
uvf_cx_1:
        mov     cx,1
        hop     UndChk3
efcode  UVALF_LOG1


fcode   UVALF_INT2
        mov     bp,offset VAL_INT2
        mov     cx,2
        hop     UndChk3
efcode  UVALF_INT2


fcode   UVALF_INT4
        mov     bp,offset VAL_INT4
uvf_cx_4:
        mov     cx,4
        hop     UndChk3
efcode  UVALF_INT4


fcode   UVALF_REAL4
        mov     bp,offset VAL_REAL4
        hop     uvf_cx_4
efcode  UVALF_REAL4


fcode   UVALF_REAL8
        mov     bp,offset VAL_REAL8
uvf_cx_8:
        mov     cx,8
        hop     UndChk3
efcode  UVALF_REAL8


fcode   UVALF_REAL16
        mov     bp,offset VAL_REAL16
        mov     cx,16
        hop     UndChk3
efcode  UVALF_REAL16


fcode   UVALF_CPLX8
        mov     bp,offset VAL_CPLX8
        hop     uvf_cx_8
efcode  UVALF_CPLX8


fcode   UVALF_CPLX16
        mov     cx,16
        mov     bp,offset VAL_CPLX16
        hop     UndChk3
efcode  UVALF_CPLX16


fcode   UVALF_CPLX32
        mov     cx,32
        mov     bp,offset VAL_CPLX32
        hop     UndChk3
efcode  UVALF_CPLX32


fcode   UVALF_LOG4
        mov     bp,offset VAL_LOG4
        hop     uvf_cx_4
efcode  UVALF_LOG4


fcode   UVALF_INT1
        mov     bp,offset VAL_INT1
        hop     uvf_cx_1
efcode  UVALF_INT1


;
; undefined variable checking for array elements
;


fcode   UVAL_LOG1
        mov     BX,offset VAL_LOG1    ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_LOG1


fcode   UVAL_LOG4
        mov     BX,offset VAL_LOG4    ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_LOG4


fcode   UVAL_INT1
        mov     BX,offset VAL_INT1    ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_INT1


fcode   UVAL_INT2
        mov     BX,offset VAL_INT2    ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_INT2


fcode   UVAL_INT4
        mov     BX,offset VAL_INT4    ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_INT4


fcode   UVAL_REAL4
        mov     BX,offset VAL_REAL4   ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_REAL4


fcode   UVAL_REAL8
        mov     BX,offset VAL_REAL8   ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_REAL8


fcode   UVAL_REAL16
        mov     BX,offset VAL_REAL16   ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_REAL16


fcode   UVAL_CPLX8
        mov     BX,offset VAL_CPLX8   ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_CPLX8


fcode   UVAL_CPLX16
        mov     BX,offset VAL_CPLX16  ; get address of push routine
        hop     ChkElt                ; check if element undefined
efcode  UVAL_CPLX16


fcode   UVAL_CPLX32
        mov     BX,offset VAL_CPLX32  ; get address of push routine
;;;;;;;;hop     ChkElt                ; check if element undefined
efcode  UVAL_CPLX32


;
;  ChkElt - ES:DI points to the array element and CX is the length of
;           the element

defn    ChkElt
        mov     DX,DI           ; save data offset in DX
        mov     BP,CX           ; save data length in BX
        mov     AL,U_BYTE       ; get undefined character
        repz    scasb           ; see if the variable is undefined
        _if     ne              ; if the variable is defined...
          mov   DI,DX           ; - restore the data offset
          mov   CX,BP           ; - restore the data length
          jmp   BX              ; - generate value
        _endif                  ; endif
        mov     DI,-( FC_LEN + 2 )[SI]; get the ADV address
        call    UndefArray      ; build error parameter
        push    SS:_MsgBuff+2   ; pass address of name
        push    SS:_MsgBuff     ; ...
        mov     AX,UV_UNDEFD    ; set undefined error message
        jmp     RTError         ; report the error
endproc ChkElt


;
; undefined variable checking for function return values
;

fcode   CHK_RET_VAL
        getword DI              ; get pointer to epilogue F-Code
        mov     BX,ARG_LIST[DI] ; point to current arglist
        add     DI,FUNC_VAL     ; point at function return value
        mov     bl,0[bx]        ; get function return type
        and     bl,TYPE_MASK    ; get the data type in bl
        _guess
          cmp   bl,PT_CHAR      ; - check for character type
          _quif ne              ; - quit if not character
          mov   CX,4[DI]        ; - set length in cx
          les   DI,0[DI]        ; - point to the string storage
        _admit
          cmp   bl,PT_STRUCT    ; - check for structure type
          _quif ne              ; - quit if not structure
          mov   ax,4[di]        ; - get offset of sdefn from RCB
          les   di,0[di]        ; - load pointer to structure storage
          xchg  ax,di           ; - get pointer to sdefn
          mov   cx,sd_total_size[di]; - get size of structure
          xchg  ax,di           ; - restore pointer to storage
        _admit
          sub   bh,bh           ; - clear high byte of index
          mov   ax,seg _SizeVars; - get size of return value
          mov   es,ax           ; ...
          mov   cl,es:_SizeVars[BX]
          sub   ch,ch           ; - . . .
          push  ds              ; - set ES=DS
          pop   es              ; - . . .
        _endguess               ; endif
                                ; check the Function return value
                                ; ES:DI point to the storage, CX = length
        mov     AL,U_BYTE       ; get undefined character
        repz    scasb           ; see if the variable is undefined
        _if     ne              ; if the variable is defined...
          next                  ; - execute next F-Code
        _endif                  ; endif
        call    TBName          ; put function name in FmtBuff
        push    SS:_MsgBuff+2   ; pass address of name
        push    SS:_MsgBuff     ; ...
        mov     AX,UV_UNDEFD    ; set undefined error message
        jmp     RTError         ; report the error
efcode  CHK_RET_VAL

;
; ChkChar - check an SCB for undefined chars
;
; INPUT : ES:DI - pointer to actual string
;         CX    - length of the string
; OUPUT : zero flag set if error found
;         AX    - index of last undefined char found
;         DX    - index of first undefined char found

        xdefp   ChkChar
defn    ChkChar
        mov     DX,CX           ; save the length in DX
        mov     AL,U_BYTE       ; set al to an undefined char
        _guess                  ; guess - no undefined chars
          repnz scasb           ; - look for an undefined char
          _quif ne              ; - quit if no U_BYTE's were found
          push  DX              ; - save the length on the stack
          sub   DX,CX           ; - calc the index of the first U_BYTE
          repz  scasb           ; - find the end of the undefined part
          pop   AX              ; - get the string length
; We must check CX in case string was of length 1
          or    CX,CX           ; - check if we are at end of string
          _if   ne              ; - if we aren't at end of string
            sub AX,CX           ; - - calc the index of the last U_BYTE
            dec AX              ; - - point to last U_BYTE
          _endif                ; - endif
          sub   CX,CX           ; - set zero flag to indicate error
        _endguess               ; endguess
        ret
endproc ChkChar


;
; BuildStrErr
;       - build string for error msg routine, given a pointer to the
;         byte following the last byte in the name string (passed in DS:BX)
;       - on return, DI points to NULL at end of string
;       - ax is non-zero if we want the extra field msg
; NOTE: the start of the name is found by scanning backwards from the
;       end until the first char with value less than $20 is found;
;       this will be the length byte

        xdefp   BuildStrErr
defn    BuildStrErr
        push    ax                      ; save field indicator
        les     di,dword ptr SS:_MsgBuff; get pointer to buffer
        lea     SI,-1[BX]               ; get address of variable
        std                             ; set for auto decrement
        _loop                           ; loop
          lodsb                         ; - get the next char
          cmp   AL,20H                  ; - is it the length byte?
        _until   b                      ; until it is
        mov     CL,AL                   ; set the length in CL
        sub     CH,CH                   ; zero off the hi byte
        add     si,2                    ; point to the first character
        cld                             ; set for auto increment
        rep     movsb                   ; copy the string
        pop     ax                      ; restore field indicator
        test    ax,ax                   ; check if it's a field reference
        _if     ne                      ; if it's a field reference
          mov   AL,'.'                  ; - indicate field reference
          mov   AH,'?'                  ; - ...
          stosw                         ; - ...
        _endif                          ; endif
        mov     ES:[DI],CL              ; place NULLCHAR at end of string
        ret                             ; return
endproc BuildStrErr


defn    TBName
        les      di,dword ptr SS:_MsgBuff; point to buffer
        lds      SI,SS:_ExCurr          ; point to traceback struct
        mov      SI,NAME_TB[SI]         ; point to the name
        mov      CX,2                   ; move 2 strings
        _loop                           ; loop
          lodsb                         ; - get a byte
          stosb                         ; - store a byte
          cmp    AL,00H                 ; - check for NULLCHAR
          _if     e                     ; - if a NULLCHAR
            dec  CX                     ; - - decrement string count
          _endif                        ; - endif
        _until    e                     ; until 2 strings moved
        ret                             ; return
endproc TBName

        fmodend
        end
