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
; FCUCHK        : undefined checking routines
;

include fcdef.inc
include errcod.inc
include undef.inc
include parmtype.inc
include datamap.inc
include structio.inc

        xref    GetTbName_

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

        extrn   RTError : near
        extrn   UndefArray : near

        extrn   _SizeVars       : byte
        extrn   _MsgBuff        : dword

        fmodstart       fcuchk

fcode   UPSH_LOG1
        mov     ebx,offset32 VAL_LOG1; get address of push routine
        mov     ecx,1           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_LOG1


fcode   UPSH_LOG4
        mov     ebx,offset32 VAL_LOG4; get address of push routine
        mov     ecx,4           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_LOG4


fcode   UPSH_INT1
        mov     ebx,offset32 VAL_INT1; get address of push routine
        mov     ecx,1           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_INT1


fcode   UPSH_INT2
        mov     ebx,offset32 VAL_INT2; get address of push routine
        mov     ecx,2           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_INT2


fcode   UPSH_INT4
        mov     ebx,offset32 VAL_INT4; get address of push routine
        mov     ecx,4           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_INT4


fcode   UPSH_REAL4
        mov     ebx,offset32 VAL_REAL4; get address of push routine
        mov     ecx,4           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_REAL4


fcode   UPSH_REAL8
        mov     ebx,offset32 VAL_REAL8; get address of push routine
        mov     ecx,8           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_REAL8


fcode   UPSH_REAL16
        mov     ebx,offset32 VAL_REAL16; get address of push routine
        mov     ecx,16          ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_REAL16


fcode   UPSH_CPLX8
        mov     ebx,offset32 VAL_CPLX8; get address of push routine
        mov     ecx,8           ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_CPLX8


fcode   UPSH_CPLX16
        mov     ebx,offset32 VAL_CPLX16; get address of push routine
        mov     ecx,16          ; get length of value
        hop     UndChk          ; check if undefined
efcode  UPSH_CPLX16


fcode   UPSH_CPLX32
        mov     ebx,offset32 VAL_CPLX32; get address of push routine
        mov     ecx,32          ; get length of value
;;;;;;;;hop     UndChk          ; check if undefined
efcode  UPSH_CPLX32


;
; ENTRY: ecx - length of value
;        ebx - routine to call if defined
;
        xdefp   UndChk
defn    UndChk
        getdw   edx             ; point to value
        lea     edi,[edx+ebp]   ; ...
        add     edx,ebp         ; ...
        jmp     UndChk2         ; see if it's undefined
endproc UndChk


fcode   UPSH_IND_LOG1
        mov     ecx,1           ; get length of value
        mov     ebx,offset32 VAL_LOG1; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_LOG1


fcode   UPSH_IND_LOG4
        mov     ecx,4           ; get length of value
        mov     ebx,offset32 VAL_LOG4; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_LOG4


fcode   UPSH_IND_INT1
        mov     ecx,1           ; get length of value
        mov     ebx,offset32 VAL_INT1; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_INT1


fcode   UPSH_IND_INT2
        mov     ecx,2           ; get length of value
        mov     ebx,offset32 VAL_INT2; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_INT2


fcode   UPSH_IND_INT4
        mov     ecx,4           ; get length of value
        mov     ebx,offset32 VAL_INT4; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_INT4


fcode   UPSH_IND_REAL4
        mov     ecx,4           ; get length of value
        mov     ebx,offset32 VAL_REAL4; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_REAL4


fcode   UPSH_IND_REAL8
        mov     ecx,8           ; get length of value
        mov     ebx,offset32 VAL_REAL8; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_REAL8


fcode   UPSH_IND_REAL16
        mov     ecx,16          ; get length of value
        mov     ebx,offset32 VAL_REAL16; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_REAL16


fcode   UPSH_IND_CPLX8
        mov     ecx,8           ; get length of value
        mov     ebx,offset32 VAL_CPLX8; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_CPLX8


fcode   UPSH_IND_CPLX16
        mov     ecx,16          ; get length of value
        mov     ebx,offset32 VAL_CPLX16; get address of push routine
        hop     UndChk1         ; check if undefined
efcode  UPSH_IND_CPLX16


fcode   UPSH_IND_CPLX32
        mov     ecx,32          ; get length of value
        mov     ebx,offset32 VAL_CPLX32; get address of push routine
;;;;;;;;hop     UndChk1         ; check if undefined
efcode  UPSH_IND_CPLX32


;
; ENTRY: ecx - length of value
;        ebx - routine to call if defined
;

defn    UndChk1
        getdw   edx             ; point to value
        mov     edi,[edx+ebp]   ; ...
        add     edx,ebp         ; ...
        or      edi,edi         ; see if address is NULL
        je      BadParm         ; report invalid parameter
;;;;;;;;hop     UndChk2
endproc UndChk1


defn    UndChk2
        mov     AL,U_BYTE       ; get undefined character
        push    edi             ; save edi since scasb auto incrs.
        repz    scasb           ; scan for U_BYTE
        pop     edi             ; restore edi (preserve flags)
        _if     ne              ; if not undefined
          jmp   ebx             ; - push the operand
        _endif                  ; endif
        xor     eax,eax         ; indicate it's not a field reference
uv_undefd_err:
        mov     ebx,edx         ; put pointer in proper register
        call    BuildStrErr     ; build the name
        push    _MsgBuff        ; pass address of name
        mov     AX,UV_UNDEFD    ; set undefined error message
        jmp     RTError         ; report the error
endproc UndChk2


;
; check for valid parms
;

defn    BadParm
        xor     eax,eax         ; indicate it wasn't structure field
        mov     ebx,edx         ; put pointer in proper register
        call    BuildStrErr     ; build the error message
        push    _MsgBuff        ; pass address of name
        mov     AX,SR_ARG_USED_NOT_PASSED; report the error
        jmp     RTError         ; report the error
endproc BadParm


fcode   PUSH_PARM_SCB           ; push a parameter SCB
dfcode  PUSH_PARM_RCB           ; push a parameter RCB
        getdw   edx             ; get address of SCB
        mov     eax,[edx+ebp]   ; get offset
        add     edx,ebp         ; fix edx (for BadParm)
        or      eax,eax         ; check if NULL
        je      BadParm         ; report error if so
        push    4[edx]          ; push length/sdefn
        push    eax             ; push offset
        next
efcode  PUSH_PARM_SCB


fcode   CHK_VALID_PARM          ; make sure that parm address is valid
        getdw   edx             ; get pointer to the parm address
        mov     eax,[edx+ebp]   ; get the value
        add     edx,ebp         ; fix edx
        or      eax,eax         ; test if NULL
        je      BadParm         ; report bad parm if NULL
        next
efcode  CHK_VALID_PARM


;
; undefined checking for structure fields
;
defn    UndChk3
        mov     al,U_BYTE       ; get undefined character
        push    edi             ; save edi since scasb auto incrs.
        repz    scasb           ; scan for U_BYTE
        pop     edi             ; restore edi (preserve flags)
        _if     ne              ; if not undefined
          jmp   ebx             ; - push the operand
        _endif                  ; endif
        mov     eax,1           ; indicate it's a field reference
        jmp     uv_undefd_err
endproc UndChk3


fcode   UVALF_LOG1
        mov     ebx,offset32 VAL_LOG1
uvf_ecx_1:
        mov     ecx,1
        hop     UndChk3
efcode  UVALF_LOG1


fcode   UVALF_INT2
        mov     ebx,offset32 VAL_INT2
        mov     ecx,2
        hop     UndChk3
efcode  UVALF_INT2


fcode   UVALF_INT4
        mov     ebx,offset32 VAL_INT4
uvf_ecx_4:
        mov     ecx,4
        hop     UndChk3
efcode  UVALF_INT4


fcode   UVALF_REAL4
        mov     ebx,offset32 VAL_REAL4
        hop     uvf_ecx_4
efcode  UVALF_REAL4


fcode   UVALF_REAL8
        mov     ebx,offset32 VAL_REAL8
uvf_ecx_8:
        mov     ecx,8
        hop     UndChk3
efcode  UVALF_REAL8


fcode   UVALF_REAL16
        mov     ebx,offset32 VAL_REAL16
uvf_ecx_16:
        mov     ecx,16
        hop     UndChk3
efcode  UVALF_REAL16


fcode   UVALF_CPLX8
        mov     ebx,offset32 VAL_CPLX8
        hop     uvf_ecx_8
efcode  UVALF_CPLX8


fcode   UVALF_CPLX16
        mov     ecx,16
        mov     ebx,offset32 VAL_CPLX16
        hop     UndChk3
efcode  UVALF_CPLX16


fcode   UVALF_CPLX32
        mov     ecx,32
        mov     ebx,offset32 VAL_CPLX32
        hop     UndChk3
efcode  UVALF_CPLX32


fcode   UVALF_LOG4
        mov     ebx,offset32 VAL_LOG4
        hop     uvf_ecx_4
efcode  UVALF_LOG4


fcode   UVALF_INT1
        mov     ebx,offset32 VAL_INT1
        hop     uvf_ecx_1
efcode  UVALF_INT1


;
; undefined variable checking for array elements
;


fcode   UVAL_LOG1
        mov     ebx,offset32 VAL_LOG1
        hop     ChkElt
efcode  UVAL_LOG1


fcode   UVAL_LOG4
        mov     ebx,offset32 VAL_LOG4
        hop     ChkElt
efcode  UVAL_LOG4


fcode   UVAL_INT1
        mov     ebx,offset32 VAL_INT1
        hop     ChkElt
efcode  UVAL_INT1


fcode   UVAL_INT2
        mov     ebx,offset32 VAL_INT2
        hop     ChkElt
efcode  UVAL_INT2


fcode   UVAL_INT4
        mov     ebx,offset32 VAL_INT4
        hop     ChkElt
efcode  UVAL_INT4


fcode   UVAL_REAL4
        mov     ebx,offset32 VAL_REAL4
        hop     ChkElt
efcode  UVAL_REAL4


fcode   UVAL_REAL8
        mov     ebx,offset32 VAL_REAL8
        hop     ChkElt
efcode  UVAL_REAL8


fcode   UVAL_REAL16
        mov     ebx,offset32 VAL_REAL16
        hop     ChkElt
efcode  UVAL_REAL16


fcode   UVAL_CPLX8
        mov     ebx,offset32 VAL_CPLX8
        hop     ChkElt
efcode  UVAL_CPLX8


fcode   UVAL_CPLX16
        mov     ebx,offset32 VAL_CPLX16
        hop     ChkElt
efcode  UVAL_CPLX16


fcode   UVAL_CPLX32
        mov     ebx,offset32 VAL_CPLX32
;;;;;;;;hop     ChkElt
efcode  UVAL_CPLX32


;
;  ChkElt - edi points to the array element and ecx is the length of
;           the element, ebx points to routine to call if defined.

defn    ChkElt
        mov     al,U_BYTE       ; get undefined character
        mov     edx,edi         ; save data offset in edx
        push    ecx             ; save data len
        repz    scasb           ; see if the variable is undefined
        _if     ne              ; if the variable is defined...
          mov   edi,edx         ; - restore the data offset
          pop   ecx             ; - restore data len
          jmp   ebx             ; - generate value
        _endif                  ; endif
        mov     edi,-( FC_LEN + 4 )[esi]; get the ADV address
        pop     ecx             ; clean stack
        add     edi,ebp         ; add data base to ADV address
        call    UndefArray      ; build error parameter
        push    _MsgBuff        ; pass address of name
        mov     eax,UV_UNDEFD   ; set undefined error message
        jmp     RTError         ; report the error
endproc ChkElt


;
; undefined variable checking for function return values
;

fcode   CHK_RET_VAL
        getdw   eax             ; get pointer to epilogue F-Code
        add     eax,esi         ; ...
        mov     ebx,arg_list[eax]; point to current arglist
        lea     edi,func_val[eax]; point at function return value
        movzx   ebx,byte ptr [ebx]; get function return type
        and     bl,TYPE_MASK    ; get the data type in bl
        _guess
          cmp   bl,PT_CHAR      ; check for character type
          _quif ne
          mov   ecx,4[edi]      ; - set length in cx
          mov   edi,[edi]       ; - point to the string storage
        _admit
          cmp   bl,PT_STRUCT    ; - check for structure type
          _quif ne              ; - quit if not structure
          mov   eax,4[edi]      ; - get offset of sdefn from RCB
          mov   edi,[edi]       ; - load pointer to structure storage
          mov   ecx,sd_total_size[eax+ebp]; - get size of structure
        _admit
          movzx ecx,byte ptr _SizeVars[ebx]; - get size of return value
        _endguess
                                ; check the Function return value
                                ; edi points to the storage, ecx = length
        mov     al,U_BYTE       ; get undefined character
        repz    scasb           ; see if the variable is undefined
        _if     ne              ; if the variable is defined...
          next                  ; - execute next F-Code
        _endif                  ; endif
        call    GetTbName_      ; put function name in FmtBuff
        push    eax             ; pass address of name
        mov     eax,UV_UNDEFD   ; set undefined error message
        jmp     RTError         ; report the error
efcode  CHK_RET_VAL

;
; ChkChar - check an SCB for undefined chars
;
; INPUT : edi   - pointer to actual string
;         ecx   - length of the string
; OUPUT : zero flag set if error found
;         eax   - index of last undefined char found
;         edx   - index of first undefined char found

        xdefp   ChkChar
defn    ChkChar
        mov     edx,ecx         ; save the length in edx
        mov     al,U_BYTE       ; set al to an undefined char
        _guess                  ; guess - no undefined chars
          repnz scasb           ; - look for an undefined char
          _quif ne              ; - quit if no U_BYTE's were found
          push  edx             ; - save the length on the stack
          sub   edx,ecx         ; - calc the index of the first U_BYTE
          repz  scasb           ; - find the end of the undefined part
          pop   eax             ; - get the string length
; We must check CX in case string was of length 1
          neg   ecx             ; - negate the length scanned
          _if   ne              ; - if we aren't at end of string
            lea eax,-1[eax+ecx] ; - - calc the index of the last U_BYTE
          _endif                ; - endif
          sub   ecx,ecx         ; - set zero flag to indicate error
        _endguess               ; endguess
        ret
endproc ChkChar


;
; BuildStrErr
;       - build string for error msg routine, given a pointer to the
;         byte following the last byte in the name string (passed in ebx)
;       - on return, edi points to NULL at end of string
;       - eax is non-zero if it was a field in a structure that was undef'd
; NOTE: the start of the name is found by scanning backwards from the
;       end until the first char with value less than 20h is found;
;       this will be the length byte

        xdefp   BuildStrErr
defn    BuildStrErr
        push    eax                     ; save field reference information
        mov     edi,_MsgBuff            ; get pointer to buffer
        lea     esi,-1[ebx]             ; get address of variable
        std                             ; set for auto decrement
        _loop                           ; loop
          lodsb                         ; - get the next char
          cmp   al,20H                  ; - is it the length byte?
        _until  b                       ; until it is
        movzx   ecx,al                  ; set the length in ecx
        add     esi,2                   ; point to the first character
        cld                             ; set for auto increment
        rep     movsb                   ; copy the string
        pop     eax                     ; restore field reference information
        test    eax,eax                 ; was it a structure field?
        _if     ne                      ; if it was a field reference
          mov   al,'.'                  ; - indicate field reference
          mov   ah,'?'                  ; - ...
          stosw                         ; - ...
        _endif                          ; endif
        mov     [edi],cl                ; place NULLCHAR at end of string
        ret                             ; return

endproc BuildStrErr

        fmodend
        end
