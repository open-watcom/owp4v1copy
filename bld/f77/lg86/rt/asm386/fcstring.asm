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
; FCSTRING      : string processing routines
;

.8087

include fcdef.inc
include errcod.inc

        rtxref  LexCmp

        extrn   ChkChar : near
        extrn   RTError : near

        dataseg

DestSCB dd      2 dup(0)

        enddata

        fmodstart       fcstring

fcode   UCHAR_CMP               ; undefined var checking on char compares
        mov     edi,[esp]       ; point edi at string 1
        mov     ecx,4[esp]      ; put the string length in ecx
        _guess                  ; guess - no undefined characters
          call  ChkChar         ; - check for undefined in string 1
          _quif e               ; - quit if undefined chars found
          mov   edi,8[esp]      ; - point edi at string 2
          mov   ecx,12[esp]     ; - put string length in ecx
          call  ChkChar         ; - check to see if any undefined chars
          _quif e               ; - quit if undefined chars found
        _admit                  ; admit - undefined chars found
          push  eax             ; - put last index on the stack
          push  edx             ; - put first index on the stack
          RTErr UV_CH_REL_UNDEFINED; - report the error
        _endguess               ; endguess
;;;;;;;;jmps    CharCmp         ; do the character compare
efcode  UCHAR_CMP


fcode   CHAR_CMP                ; compare two strings lexically
        exit_fcode              ; switch to run-time environment
        mov     eax,esp         ; get pointer to first SCB
        lea     edx,8[esp]      ; get pointer to second SCB
        docall  LexCmp          ; set eax with result of compare
        mov     edi,eax         ; move result to proper register
        lea     esp,16[esp]     ; pop args from the stack
        enter_fcode             ; switch to F-Code environment
        or      edi,edi         ; set the 8086 flags according to result
        next                    ; execute next f-code
efcode  CHAR_CMP

fcode   RT_SUBSTRING            ; generate a substring SCB
;;;;;;;;hop     DbSStr
efcode  RT_SUBSTRING


        xdefp   DbSStr
defn    DbSStr                  ; debugger entry point
        mov     eax,4[esp]      ; get first arg
        mov     ebx,[esp]       ; get second arg
        sub     eax,1           ; convert to base 0 offset
        jc      bad_substr      ; report error if arg was 0
        mov     ecx,12[esp]     ; get source SCB len
        mov     edi,8[esp]      ; get source SCB ptr
        sub     ecx,eax         ; get remaining length of substring
        jbe     bad_substr      ; report err if Len( str ) <= first arg
        sub     ebx,eax         ; get required length of substr
        jbe     bad_substr      ; report error if 2cd arg <= 1st arg
        cmp     ebx,ecx         ; see if the required length is there
        ja      bad_substr      ; report error if not
        add     esp,16          ; remove all the parms from the stack
        add     edi,eax         ; get start offset
        push    ebx             ; push the substring SCB
        push    edi             ; ...
        next
endproc DbSStr


defn    bad_substr              ; report out-of-range substring, must...
                                ; ...reorder args on stack - what a drag!
        pop     eax             ; get second arg
        pop     edx             ; get first arg
        push    eax             ; push 2nd arg
        push    edx             ; push first arg
        RTErr   SS_SSTR_RANGE   ; report error
endproc bad_substr


fcode   FIELD_SUBSTRING
        mov     ecx,4[esp]      ; get first arg
        mov     ebx,[esp]       ; get second arg
        getdw   eax             ; get source SCB len
        sub     ecx,1           ; convert to base zero offset
        jc      bad_substr      ; report error if it was 0
        sub     eax,ecx         ; get remaining length of substring
        jbe     bad_substr      ; report err if Len( str ) <= first arg
        sub     ebx,ecx         ; get required length of substr
        jbe     bad_substr      ; report error if 2cd arg <= 1st arg
        cmp     ebx,eax         ; see if the required length is there
        ja      bad_substr      ; report error if not
        add     esp,8           ; remove all the parms from the stack
        pop     eax             ; get start offset
        push    ebx             ; push the substring SCB
        add     eax,ecx         ; get start offset
        push    eax             ; ...
        next
efcode  FIELD_SUBSTRING


fcode   RT_CAT                  ; concatenate strings together
        getword ax              ; get number of strings to concatenate
        or      ah,ah           ; check if concatenating into static SCB
        _if     ns              ; if not concatenating into static SCB
          pop   edi             ; - get destination SCB
          pop   edx             ; - ...
        _else                   ; else
          mov   ebx,edi         ; - save pointer to dest SCB
          mov   edx,4[edi]      ; - get SCB
          mov   edi,[edi]       ; - ...
        _endif                  ; endif
        call    RTDoCat         ; do the concatenation
        mov     edi,DestSCB     ; restore dest SCB for mult assignment
        mov     ecx,DestSCB+4; ...
        next
efcode  RT_CAT


;
; Note that RTUCAT is not generated for concatenation into static
; temporary SCB's.
;

fcode   RT_UCAT
        getword ax              ; get number of strings to concatenate
        pop     edi             ; get the destination SCB
        pop     edx             ; ...
        call    RTDoCat         ; do the concatenation
        mov     edi,DestSCB     ; restore dest SCB for mult assignment
        mov     ecx,DestSCB+4; ...
        _guess                  ; guess - no undefined characters
          call  ChkChar         ; - check to see if any undefined chars
          _quif ne              ; - quit if no undefined chars found
          push  eax             ; - put last index on the stack
          push  edx             ; - put first index on the stack
          RTErr UV_CH_ASGN_UNDEFINED; - report the error
        _endguess               ; endguess
        mov     edi,DestSCB     ; restore dest SCB for mult assignment
        mov     ecx,DestSCB+4; ...
        next
efcode  RT_UCAT


defn    RTDoCat
        push    ebp             ; save ebp
        lea     ebp,8[esp]      ; get stack frame (above ret code)
        push    esi             ; save the fcode IP
        mov     DestSCB,edi     ; save dest SCB for multiple assignment
        mov     DestSCB+4,edx; ...
        or      ah,ah           ; check if concatenating into temp
        mov     ah,al           ; save # of SCB's on stack
        _if     ns              ; if not concatenating into temp
          _loop                 ; - loop
            mov esi,[ebp]       ; - - ...
            mov ecx,4[ebp]      ; - - ...
            add ebp,8           ; - - point at next SCB
            cmp ecx,edx         ; - - see if room in destination string
            _if   a             ; - - if not then
              mov ecx,edx       ; - - - set to amount of room left
            _endif              ; - - endif
            sub   edx,ecx       ; - - subtract amount used
            rep   movsb         ; - - copy the string over
            _quif e             ; - - quit if no more room in destination
            dec   al            ; - one less string to concat
          _until  e             ; - until none left
          mov   ecx,edx         ; - get amount of room left
          mov   al,20H          ; - get padding char
          rep   stosb           ; - pad out remaining destination
        _else                   ; else
          sub   edx,edx         ; - initialize length moved
          _loop                 ; - loop
            mov esi,[ebp]       ; - - get SCB
            mov ecx,4[ebp]      ; - - ...
            add ebp,8           ; - - point at next SCB
            add edx,ecx         ; - - increment length moved
            rep movsb           ; - - copy the string over
            _quif e             ; - - quit if no more room in destination
            dec al              ; - one less string to concat
          _until  e             ; - until none left
          mov   DestSCB+4,edx   ; - set length in destination SCB
          mov   4[ebx],edx      ; - seg length of static SCB
        _endif                  ; endif
        pop     esi             ; restore the fcode IP
        pop     ebp             ; restore ebp
        pop     ebx             ; get return address
        movzx   eax,ah          ; get # of SCB's on stack
        lea     esp,[esp+eax*8] ; pop SCB's from stack
        jmp     ebx             ; return to caller
endproc RTDoCat

        fmodend
        end
