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


fcode   RT_ALLOCATE
        mov     ebx,offset32 Alloc; address of runtime routine
rtcall:
        getword ax              ; get number of args and flags
        movzx   edi,al          ; # args in edi
        movzx   eax,ah          ; flags into eax
        push    edi             ; put #args on stack
        push    eax             ; put flags on stack
        mov     ecx, edi        ; save number of allocations to do
        test    al,4h           ; check if ALLOC_STAT bit set
        _if     ne              ; if it is
          inc   edi             ; - then there was an extra arg on stack
        _endif
        test    al,8h           ; check if ALLOC_LOC bit set
        _if     ne              ; if it is
          inc   edi             ; - then there was an extra arg on stack
        _endif
        exit_fcode              ; switch to run-time environment
        call    ebx             ; call the routine
        enter_fcode
        sub     edi, ecx        ; calculate amount of stack to clear
        shl     ecx, 3          ; allocation arguments take 8 bytes ( ecx * 8 )
        shl     edi, 2          ; other arguments take 4 bytes, ( edi * 4 )
        add     edi, ecx        ; total space required
        lea     esp,8[esp+edi]  ; remove args from stack
        next
efcode  RT_ALLOCATE


fcode   RT_DEALLOCATE
        mov     ebx,offset32 DeAlloc; address of runtime routine
        hop     rtcall
efcode  RT_DEALLOCATE


fcode   ALLOCATED
        getdw   eax             ; get offset of var
        mov     ebx,adv_array[eax+ebp]; check if array is not null
        test    ebx,ebx         ; ...
        _if     ne              ; if it is not null
          xor   ebx,ebx         ; then set true flag
          dec   ebx             ; ...
        _endif
        push    ebx             ; push result
        next
efcode  ALLOCATED

fcode   SET_SCB_LEN
        getdw   eax             ; get pointer to SCB
        add     eax, ebp        ; ...
        pop     ebx             ; get integer value
        mov     4[eax], ebx     ; save new length
        next                    ; adieu amigo
efcode  SET_SCB_LEN


        fmodend
        end
