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
; FCDO          : DO-loop control
;

include fcdef.inc
include errcod.inc
include doloop.inc

        fmodstart       fcdo

if _MATH eq _8087
        extrn   SetChop : near
        extrn   RestRnd : near
        extrn   ChopDble : near
        extrn   ChopLong : near
else
        xref    Z_F16Add_
        xref    Z_F16Sub_
        xref    Z_F16Div_
        xref    Z_F8Add_
        xref    Z_F8Sub_
        xref    Z_F8Div_
        xref    Z_F4Add_
        xref    Z_F4Sub_
        xref    Z_F4Div_
        xref    Z_RtoI_
        xref    Z_DtoI_
        xref    Z_EtoI_
        xref    Z_F4Trunc_
        xref    Z_F8Trunc_
        xref    Z_F16Trunc_
endif

        extrn   RTError : near

;       prototype of generated code being interpreted
;
; DO var=start,end,incr
;         push start
;         push end
;         push incr
;         push addr of DO variable (loads edi with offset)
;         XXDOInit
;         addr of DO-loop control block
;         addr of end of DO-loop (L1)
;                       .
;                 body of DO-loop
;                       .
; enddo
;         XXDOloop
;         addr of DO-loop control block
;     L1:
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;       XXDOInit - setup for a DO-loop
;
;       input:  edi - offset of DO variable
;               (Note that edi is loaded by the PushAddr instruction)
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

        tb_result       dw      ?,?,?,?,?,?,?,?
        SaveESI         dd      0
        SaveEDI         dd      0

fcode   R16DO_INIT               ; init code for real*16 DO-loop
if _MATH eq _EMULATION
        getdw   eax             ; get addr of DO-loop control block
        mov     do_var[eax+ebp],edi ; save addr of DO variable
        lea     edi,[eax+ebp]   ; save pointer to DO-loop control block
        exit_fcode              ; switch to run-time environment
        mov     SaveEDI, edi    ; save edi;
        mov     SaveESI, esi    ; save fcode pointer;
        add     edi, do_incr    ; copy the increment from the
        mov     esi, esp        ; stack
        movsd                   ; ...
        movsd                   ; ...
        movsd                   ; ...
        movsd                   ; ...
        sub     esp, 32         ; create more stack space
        mov     esi, esp        ; prepare to mass copy (end and start )
        mov     edi, esp        ; ...
        add     esi, 48         ; ...
        movsd                   ; lets copy (end and star varibles
        movsd                   ; ...
        movsd                   ; ...
        movsd                   ; ...
        movsd                   ; ...
        mov     edi, SaveEDI    ; restore edi;
        mov     esi, esp        ; load address of return variable
        add     esi, 48         ; the result will be placed where the (end)
                                ; was ( originally )
        docall  Z_F16Sub_       ; calculate end - start
        mov     ebx,0[esp]      ; get increment
        mov     ecx,4[esp]      ; ...
        test    ecx,0ffff0000h  ; check to see if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        mov     esi, offset32 tb_result         ; load address of return variable
        docall  Z_F16Add_       ; compute end - start + incr
        push    do_incr+12[edi] ; create another copy of the increment
        push    do_incr+8[edi]  ; ...
        push    do_incr+4[edi]  ; ...
        push    do_incr[edi]    ; ...
        push    dword ptr tb_result[12] ; push the result (end-start+incr) on
        push    dword ptr tb_result[8]  ; the stack
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        mov     esi, offset32 tb_result ; load address of return variable
        docall  Z_F16Div_       ; calculate ( end - start + incr ) / incr
        push    dword ptr tb_result[12] ; push the result (end-start+inc)/incr
        push    dword ptr tb_result[8]  ; on the stack
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        mov     esi, offset32 tb_result ; load address of return variable
        docall  Z_F16Trunc_     ; calculate INT( ( end - start + incr ) / incr )
        push    dword ptr tb_result[12] ; push the result (end-start+incr)/incr
        push    dword ptr tb_result[8]  ; on the stack
        push    dword ptr tb_result[4]  ; ...
        push    dword ptr tb_result[0]  ; ...
        mov     esi, offset32 tb_result ; load address of return variable
        docall  Z_EtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        mov     ebx,do_var[edi] ; get addr of DO variable
        pop     [ebx]           ; assign initial value to DO variable
        pop     4[ebx]          ; ...
        pop     8[ebx]          ; ...
        pop     12[ebx]         ; ...
        mov     esi, SaveESI    ; restore fcode pointer;
        jmp     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getdw   eax                     ; get addr of DO-loop control block
        fstp    _TBYTE ptr [edi]        ; set do variable to start value ...
        fld     _TBYTE ptr [edi]        ; and reload it again!!! (fst annoyance)
        lea     ebx,[eax+ebp]           ; get addr of DO-loop control block
        fxch    ST(2)                   ; <incr> end start
        mov     ecx,offset32 ChopLong   ; iteration count must be chopped
                                        ; ... to double precision in 8087
        fstp    _TBYTE ptr do_incr[ebx] ; save increment, but we need it on
        fld     _TBYTE ptr do_incr[ebx] ; the stack, so we reload it again!
        hop     F_init                  ; jump to common initialization code
endif
efcode  R16DO_INIT


fcode   R8DO_INIT               ; init code for real*8 DO-loop
if _MATH eq _EMULATION
        getdw   eax             ; get addr of DO-loop control block
        mov     do_var[eax+ebp],edi ; save addr of DO variable
        lea     edi,[eax+ebp]   ; save pointer to DO-loop control block
        exit_fcode              ; switch to run-time environment
        mov     eax,8[esp]      ; get end value
        mov     edx,12[esp]     ; ...
        mov     ebx,16[esp]     ; get start value
        mov     ecx,20[esp]     ; ...
        docall  Z_F8Sub_        ; calculate end - start
        mov     ebx,0[esp]      ; get increment
        mov     ecx,4[esp]      ; ...
        test    ecx,0ffff0000h  ; check to see if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        docall  Z_F8Add_        ; compute end - start + incr
        pop     ebx             ; get increment
        pop     ecx             ; ...
        mov     do_incr+0[edi],ebx; save increment
        mov     do_incr+4[edi],ecx; ...
        docall  Z_F8Div_        ; calculate ( end - start + incr ) / incr
        docall  Z_F8Trunc_      ; calculate INT( ( end - start + incr ) / incr )
        docall  Z_DtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        add     esp,8           ; throw away the end value
        mov     ebx,do_var[edi] ; get addr of DO variable
        pop     [ebx]           ; assign initial value to DO variable
        pop     4[ebx]          ; ...
        jmp     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getdw   eax                     ; get addr of DO-loop control block
        fst     qword ptr [edi]         ; set do variable to start value
        lea     ebx,[eax+ebp]           ; get addr of DO-loop control block
        fxch    ST(2)                   ; <incr> end start
        mov     ecx,offset32 ChopLong   ; iteration count must be chopped
                                        ; ... to double precision in 8087
        fst     qword ptr do_incr[ebx]  ; save increment
        hop     F_init                  ; jump to common initialization code
endif
efcode  R8DO_INIT


fcode   R4DO_INIT               ; init code for real*4 DO-loop
if _MATH eq _EMULATION
        getdw   ebx             ; get addr of DO-loop control block
        add     ebx,ebp
        mov     do_var[ebx],edi ; save addr of DO variable
        pop     ecx             ; get increment
        pop     eax             ; get end-value
        pop     edx             ; get start-value
        mov     do_incr[ebx],ecx; save increment
        mov     [edi],edx       ; assign start-value to DO variable
        mov     edi,ebx         ; save addr of DO-loop control block
        exit_fcode              ; switch to run-time environment
        docall  Z_F4Sub_        ; calculate (end-start)
        mov     edx,ecx         ; move incr into edx
        test    ecx,0ffff0000h  ; check if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        docall  Z_F4Add_        ; calc. (end-start+incr)
        mov     edx,ecx         ; get increment
        docall  Z_F4Div_        ; calc. (end-start+inc)/incr
        docall  Z_F4Trunc_      ; calc. INT( (end-start+incr)/incr )
        docall  Z_RtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        hop     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getdw   eax                     ; get addr of DO-loop control block
        fst     dword ptr [edi]         ; set do variable to start value
        lea     ebx,[eax+ebp]           ; get addr of DO control block
        fxch    ST(2)                   ; <incr> end start
        mov     ecx,offset32 ChopDble   ; iteration count must be chopped
                                        ; to single precision in 8087
        fst     dword ptr do_incr[ebx]  ; save increment
;;;;;;;;hop     F_init                  ; jump to common initialization code
endif
efcode  R4DO_INIT


if _MATH eq _8087
defn    F_init                          ; <incr> end start
        ftst                            ; check for zero increment
        mov     do_var[ebx],edi         ; save addr of DO variable
        fstsw   ax                      ; store result of test
        fxch    ST(2)                   ; FPU: <start> end inc
        fsubp   ST(1),ST(0)             ; FPU: <end-start> incr
        sahf                            ; check for zero increment
        _if     e                       ; if zero increment then
          RTErr DO_0INC                 ; - report error
        _endif                          ; endif
        fadd    ST(0),ST(1)             ; FPU: <end-start+incr> incr
        mov     edi,ebx                 ; save do control block addr
        fdivrp  ST(1),ST(0)             ; calc. (end-start+incr)/incr
        call    ecx                     ; do special processing
        call    SetChop                 ; set rounding control to chop
        fistp   dword ptr do_cnt[edi]   ; store iteration count
        call    RestRnd                 ; restore rounding control
        mov     eax,do_cnt[edi]         ; (fistp complete) get iter count
        hop     Com_init
endproc F_init

endif

fcode   I1DO_INIT               ; init code for integer*1 DO-loop
        mov     al,8[esp]       ; get start value for DO-loop var
        mov     [edi],al        ; store start value in DO-loop var
        hop     I_init          ; jump to common integer DO init code
efcode  I1DO_INIT


fcode   I2DO_INIT               ; init code for integer*2 DO-loop
        mov     ax,8[esp]       ; get start value for DO-loop var
        mov     [edi],ax        ; store start value in DO-loop var
        hop     I_init          ; jump to common integer DO initialization code
efcode  I2DO_INIT


fcode   I4DO_INIT               ; init code for integer*4 DO-loops
        mov     eax,8[esp]      ; get start value for DO-loop
        mov     [edi],eax       ; store start value in DO-loop var
;;;;;;;;hop     I_Init          ; jump to common integer DO initialization code
efcode  I4DO_INIT


defn    I_init
        getdw   eax             ; get addr of DO-loop control block
        add     eax,ebp
        mov     do_var[eax],edi ; save DO var offset in control block
        pop     ebx             ; get DO-loop increment
        mov     edi,eax         ; save DO-loop control block addr
        mov     do_incr[eax],ebx; save DO-loop increment
        pop     eax             ; get end value
        pop     edx             ; get start value
        sub     eax,edx         ; calc (end-start)
        cmp     ebx,1           ; check increment against 1
        _if     b               ; if increment < 1 (unsigned... i.e. incr=0)
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        _if     ne              ; if increment is not one
          add   eax,ebx         ; - calc (end-start+incr)
          cdq                   ; - sign extend eax into edx
          idiv  ebx             ; - calc (end-start+incr)/incr
        _else
          inc   eax             ; - calc (end-start+1)
        _endif                  ; endif
;;;;;;;;hop     Com_init        ; fall into common DO-loop init code
endproc I_init

;
; input: edi - pointer to DO-loop control block
;        eax - INT( (end-start+incr) / incr )
;

defn    Com_init                ; common DO-loop init code
        mov     ecx,eax         ; mov iteration count into ecx
        getdw   eax             ; get address of end of DO-loop
        add     eax,esi         ; ...
        xchg    esi,eax         ; set PC to bottom of loop (guess 0-iter)
        _guess  zero_iter       ; guess
          or    ecx,ecx         ; - see if count is <= 0
          _quif le              ; - quit if so
          dec   ecx             ; - decrement iteration count
          mov   do_cnt[edi],ecx ; - save count
          mov   esi,eax         ; - reset PC, have to execute loop
          mov   do_addr[edi],eax; - save addr of top of loop
        _endguess               ; endguess
        next                    ; execute next f-code
endproc Com_init


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;       XXDO_LOOP- increment and test at end of DO-loop
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   R16DO_LOOP              ; looping part of R*16 DO loop
        getdw   eax             ; get addr of DO loop control block
        add     eax,ebp
if _MATH eq _EMULATION
        push    dword ptr do_incr+12[eax]; push value of increment
        push    dword ptr do_incr+8[eax]; ...
        push    dword ptr do_incr+4[eax]; ...
        push    dword ptr do_incr+0[eax]; ...
        mov     edi,eax         ; save DO-loop control pointer
        mov     eax,do_var[eax] ; get addr of DO-loop variable
        push    dword ptr 12[eax]; get current value of DO variable
        push    dword ptr 8[eax]; ...
        push    dword ptr 4[eax]; ...
        push    dword ptr 0[eax]; ...
        mov     SaveESI, esi    ; save fcode ptr;
        mov     esi, eax        ; load result address
        exit_fcode              ; switch to run-time environment
        docall  Z_F16Add_       ; calculate new value for DO-loop variable
        enter_fcode             ; switch to F-Code environment
        mov     eax,edi         ; get addr of DO-loop control ptr
else
        fld     _TBYTE ptr do_incr[eax] ; get increment value
        mov     edi,do_var[eax] ; get address of DO variable
        fld     _TBYTE ptr [edi] ; load the todo variable
        faddp   st(1),st(0)     ; add increment to do variable
        fstp    _TBYTE ptr [edi] ; store the result back into the var
endif
        hop     Com_loop        ; jump to common loop code
efcode  R16DO_LOOP

fcode   R8DO_LOOP               ; looping part of R*8 DO loop
        getdw   eax             ; get addr of DO loop control block
        add     eax,ebp
if _MATH eq _EMULATION
        mov     ebx,do_incr+0[eax]; get value of increment
        mov     ecx,do_incr+4[eax]; ...
        mov     edi,eax         ; save DO-loop control pointer
        mov     eax,do_var[eax] ; get addr of DO-loop variable
        mov     edx,4[eax]      ; get current value of DO variable
        mov     eax,0[eax]      ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_F8Add_        ; calculate new value for DO-loop variable
        enter_fcode             ; switch to F-Code environment
        mov     ebx,do_var[edi] ; get addr of DO-loop variable
        mov     0[ebx],eax      ; update DO loop variable
        mov     4[ebx],edx      ; ...
        mov     eax,edi         ; get addr of DO-loop control ptr
else
        fld     qword ptr do_incr[eax]  ; get increment value
        mov     edi,do_var[eax] ; get address of DO variable
        fadd    qword ptr [edi] ; add increment to do variable
        fstp    qword ptr [edi] ; store the result back into the var
endif
        hop     Com_loop        ; jump to common loop code
efcode  R8DO_LOOP


fcode   R4DO_LOOP               ; looping part of R*4 DO loop
        getdw   eax             ; get address of DO-loop control block
        add     eax,ebp
if _MATH eq _EMULATION
        mov     edi,do_var[eax] ; get address of DO loop variable
        mov     ebx,eax         ; save ptr to DO-loop control block
        mov     eax,do_incr[eax]; get the increment
        mov     edx,[edi]       ; get current value of variable
        exit_fcode              ; switch to run-time environment
        docall  Z_F4Add_        ; calc. new value for variable
        enter_fcode             ; switch to F-Code environment
        mov     [edi],eax       ; update DO loop variable
        mov     eax,ebx         ; ptr to DO-loop control block
else
        fld     dword ptr do_incr[eax]  ; get increment value
        mov     edi,do_var[eax] ; get address of do variable
        fadd    dword ptr [edi] ; add increment to do variable
        fstp    dword ptr [edi] ; store the result back into the var
endif
        hop     Com_loop                ; jump to common loop code
efcode  R4DO_LOOP


fcode   I1DO_LOOP               ; looping part of I*1 DO-loop
        getdw   eax             ; get address of DO-loop control block
        add     eax,ebp
        mov     edi,do_var[eax] ; get offset of DO-loop var
        mov     cl,byte ptr do_incr[eax]; get DO-loop increment
        add     [edi],cl        ; add on the increment
        hop     Com_loop        ; jump to common loop code
efcode  I1DO_LOOP


fcode   I2DO_LOOP               ; looping part of I*2 DO-loop
        getdw   eax             ; get address of DO-loop control block
        add     eax,ebp
        mov     edi,do_var[eax] ; get offset of DO-loop var
        mov     cx,word ptr do_incr[eax]; get DO-loop increment
        add     [edi],cx        ; add on increment
        hop     Com_loop        ; jump to common loop code
efcode  I2DO_LOOP


fcode   I4DO_LOOP               ; looping part of I*4 DO-loop
        getdw   eax             ; get address of DO-loop control block
        add     eax,ebp
        mov     edi,do_var[eax] ; get offset of DO-loop var
        mov     ecx,do_incr[eax]; get DO-loop increment
        add     [edi],ecx       ; add on increment
;;;;;;;;hop     Com_loop
efcode  I4DO_LOOP


;
; input: ebx - pointer to DO-loop control block
;

defn    Com_loop                ; common DO-loop incr/test code
        mov     ebx,do_addr[eax]; get address of top of DO-loop
        xchg    ebx,esi         ; assume loop not finished
        _guess                  ; guess
          sub   dword ptr do_cnt[eax],1; - decrement iteration count
          _quif c               ; - quit if loop finished
          next                  ; - goto next f-code
        _admit
          mov   esi,ebx         ; - loop done: set addr to next code
          next                  ; - goto next f-code routine
        _endguess               ; endguess
endproc Com_loop

        fmodend
        end
