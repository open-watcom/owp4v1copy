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
; FCALLOC:      Support for dynamically allocatable arrays
;

        include fcdef.inc
        include adv.inc

        lgxref  Alloc
        lgxref  DeAlloc

        fmodstart fcalloc

        dataseg

        RtRtn           dw      2 dup(?)

        enddata


fcode   RT_ALLOCATE
        mov     di,offset Alloc         ; address of runtime routine
        mov     bp,seg Alloc            ; ...
rtcall:
        mov     SS:RtRtn,di             ; save address of runtime routine
        mov     SS:RtRtn+2,bp           ; ...
        getword ax                      ; get number of args (al) & flags (ah)
        mov     di,ax                   ; save in di
        xor     ah,ah                   ; zero high byte
        xchg    di,ax                   ; now di has # args
        mov     al,ah                   ; isolate flags
        push    di                      ; put #args on stack
        mov     bx, di                  ; calculate amount of flag space to add
        xor     ah,ah                   ; zero high byte of flags
        push    ax                      ; put flags on stack
        test    al,4h                   ; check if ALLOC_STAT bit set
        _if     ne                      ; if it is
          inc   di                      ; - there was an extra arg on stack
        _endif                          ; endif
        test    al,8h                   ; check if ALLOC_LOC bit set
        _if     ne                      ; if it is
          inc   di                      ; - there was an extra arg on stack
        _endif                          ; endif
        exit_fcode                      ; switch to run-time environment
        shl     di,1                    ; multiply # of parms by 2 (for later)
        call    dword ptr SS:RtRtn      ; call runtime routine
        enter_fcode
        inc     di                      ; for the # of args we pushed
        inc     di                      ; for the alloc flags we pushed
        add     di, bx                  ; add flag space requirements
        shl     di,1                    ; multiply by 2
        add     sp,di                   ; pop the args from the stack
        next
efcode  RT_ALLOCATE


fcode   RT_DEALLOCATE
        mov     di,offset DeAlloc       ; address of runtime routine
        mov     bp,seg DeAlloc          ; ...
        hop     rtcall
efcode  RT_DEALLOCATE


fcode   ALLOCATED
        getword di                      ; get offset of var
        mov     ax,adv_array[di]        ; check if array is not null
        or      ax,adv_array+2[di]      ; ...
        _if     ne                      ; if it is not null
          xor   ax,ax                   ; then set true flag
          dec   ax                      ; ...
        _endif
        push    ax                      ; push results
        push    ax                      ; ...
        next
efcode  ALLOCATED


fcode   SET_SCB_LEN
        getword di              ; get pointer to SCB
        pop     ax              ; get integer value
        mov     4[di], ax       ; save new length
        pop     ax              ; trash top half
        or      ax, ax          ; check to see if > 65535
        _if ne                  ; if it is
          mov   word ptr 4[di],0; This will generate an error
        _endif                  ; ....
        next                    ; go on
efcode  SET_SCB_LEN


        fmodend
        end
