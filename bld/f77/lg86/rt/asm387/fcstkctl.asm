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
; FCSTKCTL      : stack control functions
;

.8087

include fcdef.inc

        dataseg

        public  AuxTop
        AuxTop  dd 0            ; Auxil stack grows towards higher addresses

        enddata

        fmodstart       fcstkctl


fcode   DUMP_ENTRIES            ; remove all but 4 entries
        getword ax              ; get number of entries to dump
        movzx   ecx,ax          ; put it in ecx
        add     al,ah           ; get post-adjust
        movzx   eax,al          ; zero high part
        _loop                   ; loop
          fincstp               ; - skip over entries to keep
          dec   ch              ; - one less to do
        _until   e              ; until done pre-adjust
        hop     dump_em         ; dump the remaining entries
efcode  DUMP_ENTRIES


fcode   FLUSH_ENTRIES           ; remove all entries on 8087 stack
        getword ax              ; get number of stack entries
        movzx   ecx,ax          ; #stk entries in ecx
        xor     eax,eax         ; zero CX
;;;;;;;;hop     dump_em
efcode  FLUSH_ENTRIES


defn    dump_em                 ; dump number of entries in CX
        mov     edi,AuxTop      ; get top of stack into edi
        lea     ebx,[ecx+ecx*4] ; get number of entries*5 into ebx
        lea     ebx,[edi+ebx*2] ; number of entries*10 into ebx + AuxTop
        mov     AuxTop,ebx      ; save new top of stack
rpt1:                           ; loop
          sub   ebx,10          ; - point at save location
          fstp  tbyte ptr [ebx] ; - save stack entry
        loop    rpt1            ; until no more entries to do
        xchg    ecx,eax         ; get post-adjust in cx
dec_stp:
        jcxz    abort           ; abort if no adjust
rpt2:                           ; loop
          fdecstp               ; - decrement stack top pointer
        loop    rpt2            ; until stack top pointer is restored
abort:  next                    ; execute next f-code
endproc dump_em


fcode   LOAD_ENTRIES            ; reload 8087 stack entries
        getword ax              ; get stack top adjust & # to reload
        movzx   ecx,ax          ; zero extend into ecx
        xor     eax,eax         ; zero eax
        xchg    al,ch           ; get stack top adjust in CX
        mov     ebx,ecx         ; save post-adjustment in BX
        add     ecx,eax         ; get number of entries to skip
rpt3:                           ; loop
          fincstp               ; - skip entry
        loop    rpt3            ; until no more entries to skip
        mov     ecx,eax         ; get number of entries to load into CX
        mov     edx,AuxTop      ; get stack top
        lea     eax,[eax+eax*4] ; multiply number of entries by -10
        neg     eax             ; ...
        lea     edx,[edx+eax*2] ; ... and add top of stack
        mov     AuxTop,edx      ; and save it
rpt4:                           ; loop
          fld   tbyte ptr [edx] ; - load entry
          add   edx,10          ; - point at next stack entry
        loop    rpt4            ; until no more entries to load
        mov     ecx,ebx         ; get number of entries skipped in cx
        hop     dec_stp
efcode  LOAD_ENTRIES


        fmodend
        end
