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

        fmodstart       fcstkctl

        dataseg

        extrn   _IORslt : near
        extrn   __STACKLOW : word

        public  AuxTop
        AuxTop  dw 0            ; Auxil stack grows towards higher addresses

        enddata


fcode   DUMP_ENTRIES            ; remove all but 4 entries
        getword ax              ; get number of entries to dump
        mov     cx,ax           ; put it in cx
        add     al,ah           ; get post-adjust
        cbw                     ; ... (zero ah)
        _loop                   ; loop
          fincstp               ; - skip over entries to keep
          dec   ch              ; - one less to do
        _until   e              ; until done pre-adjust
        hop     dump_em         ; dump the remaining entries
efcode  DUMP_ENTRIES


fcode   FLUSH_ENTRIES           ; remove all entries on 8087 stack
        getword ax              ; get number of stack entries
        xor     cx,cx           ; zero CX
        xchg    ax,cx           ; #stk entries in CX, no post-adjust in ax
;;;;;;;;hop     dump_em
efcode  FLUSH_ENTRIES


defn    dump_em                 ; dump number of entries in CX
        mov     bp,cx           ; get number of entries into BP
        shl     bp,1            ; multiply by 10
        shl     bp,1            ; ...
        add     bp,cx           ; ...
        shl     bp,1            ; ...
        add     bp,SS:AuxTop    ; point at storage area
        mov     SS:AuxTop,bp    ; save new top of stack
rpt1:                           ; loop
          sub   bp,10           ; - point at save location
          fstp  tbyte ptr [BP]  ; - save stack entry
        loop    rpt1            ; until no more entries to do
        xchg    cx,ax           ; get post-adjust in cx
dec_stp:
        jcxz    abort           ; abort if no adjust
rpt2:                           ; loop
          fdecstp               ; - decrement stack top pointer
        loop    rpt2            ; until stack top pointer is restored
abort:  next                    ; execute next f-code
endproc dump_em


fcode   LOAD_ENTRIES            ; reload 8087 stack entries
        getword ax              ; get stack top adjust & # to reload
        xor     cx,cx           ; zero cx
        xchg    ax,cx           ; cx=stack top adjust & # to reload, ax=0
        xchg    al,ch           ; get stack top adjust in CX
        mov     bx,cx           ; save post-adjustment in BX
        add     cx,ax           ; get number of entries to skip
rpt3:                           ; loop
          fincstp               ; - skip entry
        loop    rpt3            ; until no more entries to skip
        mov     cx,ax           ; get number of entries to load into CX
        mov     bp,SS:AuxTop    ; get stack top
        shl     ax,1            ; multiply number of entries by 10
        shl     ax,1            ; ...
        add     ax,cx           ; ...
        shl     ax,1            ; ...
        sub     bp,ax           ; get new stack top
        mov     SS:AuxTop,bp    ; and save it
rpt4:                           ; loop
          fld   tbyte ptr [bp]  ; - load entry
          add   bp,10           ; - point at next stack entry
        loop    rpt4            ; until no more entries to load
        mov     cx,bx           ; get number of entries skipped in cx
        hop     dec_stp
efcode  LOAD_ENTRIES


        fmodend
        end
