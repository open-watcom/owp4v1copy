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
;<>     FCSTRING - support for string operations
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include errcod.inc

        rtxref  LexCmp

        extrn   ChkChar : near
        extrn   RTError : near

        fmodstart       fcstring

        dataseg

DestSCB dw      3 dup(0)

        enddata

fcode   UCHAR_CMP               ; undefined var checking on char compares
        mov     BP,SP           ; get stack frame
        les     DI,[BP]         ; point DS:SI at string 1
        mov     CX,4[BP]        ; put the string length in CX
        _guess                  ; guess - no undefined characters
          call  ChkChar         ; - check for undefined in string 1
          _quif e               ; - quit if undefined chars found
          les   DI,6[BP]        ; - point DS:SI at string 2
          mov   CX,0aH[BP]      ; - put the string length in CX
          call  ChkChar         ; - check to see if any undefined chars
          _quif   e             ; - quit if undefined chars found
        _admit                  ; admit - undefined chars found
          push  ax              ; - put last index on the stack
          push  dx              ; - put first index on the stack
          RTErr UV_CH_REL_UNDEFINED; - report the error
        _endguess               ; endguess
;;;;;;;;jmps    CharCmp         ; do the character compare
efcode  UCHAR_CMP


fcode   CHAR_CMP                ; compare two strings lexically
        exit_fcode              ; switch to run-time environment
        mov     AX,SP           ; get pointer to first SCB
        mov     DX,SS           ; . . .
        mov     BX,SP           ; get pointer to second SCB
        add     BX,6            ; . . .
        mov     CX,DX           ; . . .
        docall  LexCmp          ; set ax with result of compare
        add     SP,12           ; remove SCBs from stack
        mov     di,ax           ; move result to proper register
        enter_fcode             ; switch to F-Code environment
        or      di,di           ; set the 8086 flags according to result
        next                    ; execute next f-code
efcode  CHAR_CMP

fcode   RT_SUBSTRING            ; generate a substring SCB
;;;;;;;;hop     DbSStr
efcode  RT_SUBSTRING


        xdefp   DbSStr
defn    DbSStr                  ; debugger entry point
        mov     bp,sp           ; point to args on the stack
        mov     dx,6[BP]        ; get high word of first arg
        mov     ax,4[BP]        ; get low word of first arg
        mov     cx,2[BP]        ; get high word of second arg
        mov     bx,0[BP]        ; get low word of second arg
        or      dx,cx           ; check to see high words are zero
        jne     bad_substr      ; report error if they aren't
        sub     ax,1            ; convert to base zero offset
        jc      bad_substr      ; report error if first arg == 0
        mov     cx,0cH[BP]      ; get source SCB len
        les     di,8H[BP]       ; get source SCB ptr
        sub     cx,ax           ; get remaining length of substring
        jbe     bad_substr      ; report err if Len( str ) <= first arg
        sub     bx,ax           ; get required length of substr
        jbe     bad_substr      ; report error if 2cd arg <= 1st arg
        cmp     bx,cx           ; see if the required length is there
        ja      bad_substr      ; report error if not
        add     sp,14           ; remove all the parms from the stack
        add     di,ax           ; get start offset
        push    bx              ; push the substring SCB
        push    es              ; . . .
        push    di              ; . . .
        next                    ; execute next f-code
endproc DbSStr


defn    bad_substr              ; report out-of-range substring, must...
                                ; ...reorder args on stack - what a drag!
        pop     AX              ; get low word of second arg
        pop     DX              ; get high word of second arg
        xchg    4[BP],AX        ; exchange the low words
        xchg    6[BP],DX        ; exchange the hi words
        push    DX              ; push the hi word of arg 1
        push    AX              ; push the lo word of arg 1
        RTErr   SS_SSTR_RANGE   ; . . .
endproc bad_substr


fcode   FIELD_SUBSTRING
        mov     bp,sp           ; point to args on the stack
        mov     dx,6[BP]        ; get high word of first arg
        mov     cx,4[BP]        ; get low word of first arg
        mov     ax,2[BP]        ; get high word of second arg
        mov     bx,0[BP]        ; get low word of second arg
        or      dx,ax           ; check to see high words are zero
        jne     bad_substr      ; report error if they aren't
        getword ax              ; get source SCB len
        sub     cx,1            ; convert first arg to base zero offset
        jc      bad_substr      ; report error if it was 0
        sub     ax,cx           ; get remaining length of substring
        jbe     bad_substr      ; report err if Len( str ) <= first arg
        sub     bx,cx           ; get required length of substr
        jbe     bad_substr      ; report error if 2cd arg <= 1st arg
        cmp     bx,ax           ; see if the required length is there
        ja      bad_substr      ; report error if not
        add     sp,8            ; remove substring parms from stack
        pop     ax              ; get low word
        pop     dx              ; get high word
        add     ax,cx           ; add offset of field
        adc     dx,0            ; ...
        push    bx              ; push result SCB
        push    dx              ; ...
        push    ax              ; ...
        next
efcode  FIELD_SUBSTRING


fcode   RT_CAT                  ; concatenate strings together
        getword ax              ; get number of strings to concatenate
        or      AH,AH           ; check if concatenating into static SCB
        _if     e               ; if not concatenating into static SCB
          pop   di              ; - get the destination SCB
          pop   es              ; - . . .
          pop   dx              ; - . . .
        _else                   ; else
          mov   BX,DI           ; - save pointer to destination SCB
          mov   DX,4[DI]        ; - get destination SCB
          les   DI,[DI]         ; - . . .
        _endif                  ; endif
        call    RTDoCat         ; do the concatenation
        les     DI,dword ptr SS:DestSCB+0; restore dest SCB for mult assignment
        mov     CX,SS:DestSCB+4 ; . . .
        next                    ; do the next fcode
efcode  RT_CAT


;
; Note that RTUCAT is not generated for concatenation into static
; temporary SCB's.
;

fcode   RT_UCAT
        getword ax              ; get number of strings to concatenate
        pop     di              ; get the destination SCB
        pop     es              ; . . .
        pop     dx              ; . . .
        call    RTDoCat         ; do the concatenation
        les     DI,dword ptr SS:DestSCB+0; restore dest SCB for mult assignment
        mov     CX,SS:DestSCB+4 ; . . .
        _guess                  ; guess - no undefined characters
          call  ChkChar         ; - check to see if any undefined chars
          _quif   ne            ; - quit if no undefined chars found
          push    ax            ; - put last index on the stack
          push    dx            ; - put first index on the stack
          RTErr UV_CH_ASGN_UNDEFINED; - report the error
        _endguess               ; endguess
        les     DI,dword ptr SS:DestSCB+0; restore dest SCB for mult assignment
        mov     CX,SS:DestSCB+4 ; . . .
        next                    ; do the next fcode
efcode  RT_UCAT


defn    RTDoCat
        mov     bp,sp           ; point at args
        push    si              ; save the fcode IP
        push    ds              ; save fcode segment
        add     bp,2            ; point above the return code
        mov     SS:DestSCB+0,DI ; save dest SCB for multiple assignment
        mov     SS:DestSCB+2,ES ; . . .
        mov     SS:DestSCB+4,DX ; . . .
        or      ah,ah           ; check if concatenating into temp
        mov     ah,al           ; save # of SCB's on stack
        _if     e               ; if not concatenating into temp
          _loop                 ; - loop
            lds   si,0[bp]      ; - - get SCB
            mov   cx,4[bp]      ; - - . . .
            add   bp,6          ; - - advance to next SCB
            cmp   cx,dx         ; - - see if room in destination string
            _if   a             ; - - if not then
              mov cx,dx         ; - - - set to amount of room left
            _endif              ; - - endif
            sub   dx,cx         ; - - subtract amount used
            rep   movsb         ; - - copy the string over
            _quif e             ; - - quit if no more room in destination
            dec   al            ; - one less string to concat
          _until  e             ; - until none left
          mov   cx,dx           ; - get amount of room left
          mov   al,20H          ; - get padding char
          rep   stosb           ; - pad out remaining destination
          pop   ds              ; - restore fcode segment
        _else                   ; else
          sub   DX,DX           ; - initialize length moved
          _loop                 ; - loop
            lds   si,0[bp]      ; - - get SCB
            mov   cx,4[bp]      ; - - . . .
            add   bp,6          ; - - point at next SCB
            add   DX,CX         ; - - increment length moved
            rep   movsb         ; - - copy the string over
            _quif e             ; - - quit if no more room in destination
            dec   al            ; - one less string to concat
          _until  e             ; - until none left
          mov   SS:DestSCB+4,DX ; - set length in destination SCB
          pop   ds              ; - restore fcode segment
          mov   4[BX],DX        ; - set length of static SCB
        _endif                  ; endif
        pop     si              ; restore the fcode IP
        mov     al,ah           ; get # of SCB's on stack
        xor     ah,ah           ; ...
        mov     bp,ax           ; multiply by 6
        shl     bp,1            ; ...
        add     bp,ax           ; ...
        pop     ax              ; get return address from stack
        shl     bp,1            ; ...
        add     sp,bp           ; pop SCB's from stack
        jmp     ax              ; return to caller
endproc RTDoCat

        fmodend
        end
