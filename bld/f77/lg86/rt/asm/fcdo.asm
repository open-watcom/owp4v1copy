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
;<>     DO loop runtime support for FORTRAN-77
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc
include errcod.inc
include doloop.inc

; used in the double do loops
push_double     macro
                push    SS:6[DI]
                push    SS:4[DI]
                push    SS:2[DI]
                push    SS:[DI]
                endm

push_extended   macro
                push    SS:14[DI]
                push    SS:12[DI]
                push    SS:10[DI]
                push    SS:8[DI]
                push    SS:6[DI]
                push    SS:4[DI]
                push    SS:2[DI]
                push    SS:[DI]
                endm

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
        xref    Z_I4Div_

        extrn   RTError : near

        fmodstart       fcdo

;       prototype of generated code being interpreted
;
; DO var=start,end,incr
;         push start
;         push end
;         push incr
;         push addr of DO variable (loads ES:DI with seg and offset)
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
;       input:  DI - offset from ES of DO variable
;               ES - segment addr containing DO variable
;               (Note that ES:DI are loaded by the PushAddr instruction)
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

fcode   R16DO_INIT               ; init code for real*16 DO-loop
if _MATH eq _EMULATION
        getword bx              ; get addr of DO-loop control block
        mov     word ptr do_var[BX],DI; save addr of DO variable
        mov     word ptr do_var+2[BX],ES; save seg addr of DO variable
        mov     BP,SP           ; get access to parms on stack
        exit_fcode              ; switch to run-time environment
        lea     DI,16[BP]       ; point to end value
        push_extended           ; Create space for return value
        push_extended           ; pusu end value on stack
        lea     DI,32[BP]       ; point to start value
        push_extended           ; pusu end value on stack
        mov     DI, SI          ; save fcode pointer
        mov     AX, ES          ; save ES
        push    SS              ; load return address
        pop     ES              ; ...
        mov     SI, SP          ; ..
        add     SI, 32          ; ...
        docall  Z_F16Sub_       ; calculate end - start
        mov     BP, SP          ; get increment
        add     BP, 16          ; ...
        cmp     word ptr 8[BP],0; check to see if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        mov     DI, SP          ; point to crrent result
        push_extended           ; create more stack room
        mov     SI, SP          ; set return address
        push_extended           ; Psh crrent result on stack
        mov     DI, BP          ; get increment ptr
        push_extended           ; push increment on stack
        docall  Z_F16Add_       ; compute end - start + incr
        push_extended           ; push increment on stack
        mov     SI, SP          ; load return address
        add     SI, 32          ; ...
        docall  Z_F16Div_       ; calculate ( end - start + incr ) / incr
        mov     SI, SP          ; load return address
        mov     DI, SI          ; we make space and duplicate the result
        push_extended           ; ...
        docall  Z_F16Trunc_     ; calculate INT( ( end - start + incr ) / incr )
        docall  Z_DtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        pop     do_incr+0[BX]   ; get increment
        pop     do_incr+2[BX]   ; . . .
        pop     do_incr+4[BX]   ; . . .
        pop     do_incr+6[BX]   ; . . .
        pop     do_incr+8[BX]   ; . . .
        pop     do_incr+10[BX]  ; . . .
        pop     do_incr+12[BX]  ; . . .
        pop     do_incr+14[BX]  ; . . .
        add     SP,16           ; throw away the end value
        les     DI,do_var[BX]   ; get addr of DO variable
        pop     ES:0[DI]        ; assign initial value to DO variable
        pop     ES:2[DI]        ; . . .
        pop     ES:4[DI]        ; . . .
        pop     ES:6[DI]        ; . . .
        pop     ES:8[DI]        ; . . .
        pop     ES:10[DI]       ; . . .
        pop     ES:12[DI]       ; . . .
        pop     ES:14[DI]       ; . . .
        mov     DI,BX           ; get address of DO-loop control block
        jmp     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getword AX                      ; get addr of DO-loop control block
        fstp    _TBYTE ptr ES:[DI]      ; set do variable to start value
        fld     _TBYTE ptr ES:[DI]      ; must reload popped variable
        mov     BX,AX                   ; get addr of DO-loop control block
        fxch    ST(2)                   ; <incr> end start
        mov     CX, 0                   ; no chpping required
        fstp    _TBYTE ptr do_incr[BX]  ; save increment
        fld     _TBYTE ptr do_incr[BX]  ; must reload popped variable
        jmp     F_init                  ; jump to common initialization code
endif
efcode  R16DO_INIT


fcode   R8DO_INIT               ; init code for real*8 DO-loop
if _MATH eq _EMULATION
        getword bx              ; get addr of DO-loop control block
        mov     word ptr do_var[BX],DI; save addr of DO variable
        mov     word ptr do_var+2[BX],ES; save seg addr of DO variable
        mov     BP,SP           ; get access to parms on stack
        push    BX              ; save DO-loop control block addr
        exit_fcode              ; switch to run-time environment
        mov     DX,8[BP]        ; get end value
        mov     CX,10[BP]       ; ...
        mov     BX,12[BP]       ; ...
        mov     AX,14[BP]       ; ...
        lea     DI,16[BP]       ; point to start value
        push_double             ; pusu start value on stack
        docall  Z_F8Sub_        ; calculate end - start
        cmp     word ptr 6[BP],0; check to see if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        mov     DI,BP           ; point to increment
        push_double             ; push increment on stack
        docall  Z_F8Add_        ; compute end - start + incr
        mov     DI,BP           ; point to increment
        push_double             ; push increment on stack
        docall  Z_F8Div_        ; calculate ( end - start + incr ) / incr
        docall  Z_F8Trunc_      ; calculate INT( ( end - start + incr ) / incr )
        docall  Z_DtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        pop     BX              ; restore addr of DO-loop control block
        pop     do_incr+0[BX]   ; get increment
        pop     do_incr+2[BX]   ; . . .
        pop     do_incr+4[BX]   ; . . .
        pop     do_incr+6[BX]   ; . . .
        add     SP,8            ; throw away the end value
        les     DI,do_var[BX]   ; get addr of DO variable
        pop     ES:0[DI]        ; assign initial value to DO variable
        pop     ES:2[DI]        ; . . .
        pop     ES:4[DI]        ; . . .
        pop     ES:6[DI]        ; . . .
        mov     DI,BX           ; get address of DO-loop control block
        jmp     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getword AX                      ; get addr of DO-loop control block
        fst     qword ptr ES:[DI]       ; set do variable to start value
        mov     BX,AX                   ; get addr of DO-loop control block
        fxch    ST(2)                   ; <incr> end start
        mov     CX,offset ChopLong      ; iteration count must be chopped
                                        ; ... to double precision in 8087
        fst     qword ptr do_incr[BX]   ; save increment
        hop     F_init                  ; jump to common initialization code
endif
efcode  R8DO_INIT


fcode   R4DO_INIT               ; init code for real*4 DO-loop
if _MATH eq _EMULATION
        getword bx              ; get addr of DO-loop control block
        mov     word ptr do_var[BX],DI   ; save addr of DO variable
        mov     word ptr do_var+2[BX],ES   ; save seg addr of DO variable
        pop     do_incr+0[BX]   ; get increment
        pop     do_incr+2[BX]   ; ...
        mov     bp,bx           ; save addr of DO-loop control block
        pop     AX              ; get end-value (low word)
        pop     DX              ; get end-value (high-word)
        pop     BX              ; get start-value (low word)
        pop     CX              ; get start-value (high word)
        mov     ES:0[DI],BX     ; assign start-value to DO variable
        mov     ES:2[DI],CX     ; ...
        push    ds              ; set ES to program segment
        pop     es              ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_F4Sub_        ; calculate (end-start)
        mov     di,bp           ; get address of DO-loop control block
        mov     CX,ES:do_incr+2[DI]; get increment
        mov     BX,ES:do_incr+0[DI]; ...
        push    CX              ; save increment
        push    BX              ; ...
        or      CX,CX           ; see if increment is zero
        _if      e              ; if it is
          RTErr DO_0INC         ; - report the error
        _endif                  ; endif
        docall  Z_F4Add_        ; calc. (end-start+incr)
        pop     BX              ; get increment
        pop     CX              ; ...
        docall  Z_F4Div_        ; calc. (end-start+inc)/incr
        docall  Z_F4Trunc_      ; calc. INT( (end-start+incr)/incr )
        docall  Z_RtoI_         ; convert to integer
        enter_fcode             ; switch to F-Code environment
        hop     Com_init        ; jump to common initialization code
else
                                        ; <incr> end start
        fxch    ST(2)                   ; <start> end incr
        getword AX                      ; get addr of DO-loop control block
        fst     dword ptr ES:[DI]       ; set do variable to start value
        mov     BX,AX                   ; get addr of DO control block
        fxch    ST(2)                   ; <incr> end start
        mov     CX,offset ChopDble      ; iteration count must be chopped
                                        ; to single precision in 8087
        fst     dword ptr do_incr[BX]   ; save increment
;;;;;;;;hop     F_init                  ; jump to common initialization code
endif
efcode  R4DO_INIT


if _MATH eq _8087
defn    F_init                          ; <incr> end start
        push    AX                      ; allocate temporary for status word
        ftst                            ; check for zero increment
        mov     BP,SP                   ; point to temporary
        mov     word ptr do_var[BX],DI          ; save addr of DO variable
        fstsw   [BP]                    ; store result of test
        mov     word ptr do_var+2[BX],ES        ; save seg addr of DO variable
        fxch    ST(2)                   ; FPU: <start> end inc
        pop     AX                      ; (fstsw complete) get result of test
        fsubp   ST(1),ST(0)             ; FPU: <end-start> incr
        sahf                            ; check for zero increment
        _if     e                       ; if zero increment then
          RTErr DO_0INC                 ; - report error
        _endif                          ; endif
        fadd    ST(0),ST(1)             ; FPU: <end-start+incr> incr
        mov     DI,BX                   ; save do control block addr
        fdivrp  ST(1),ST(0)             ; calc. (end-start+incr)/incr
        test    CX, 0                   ; if we must call special proc
        je      no_proc                 ; no special proc reqiured
        call    CX                      ; do special processing
no_proc:
        call    SetChop                 ; set rounding control to chop
        fistp   dword ptr do_cnt[DI]    ; store iteration count
        call    RestRnd                 ; restore rounding control
        mov     ax,do_cnt+0[DI]         ; (fistp complete) get low word of iter
        mov     dx,do_cnt+2[DI]         ; get high word of iter
        hop     Com_Init
endproc F_init

endif

fcode   I1DO_INIT               ; init code for integer*1 DO-loop
        mov     bp,sp           ; get access to parms on stack
        mov     al,8[bp]        ; get start value for DO-loop var
        mov     es:[di],al      ; store start value in DO-loop var
        hop     I_init          ; jump to common integer DO init code
efcode  I1DO_INIT


fcode   I2DO_INIT               ; init code for integer*2 DO-loop
        mov     bp,sp           ; get access to parms on stack
        mov     ax,8[bp]        ; get start value for DO-loop var
        mov     es:[di],ax      ; store start value in DO-loop var
        hop     I_init          ; jump to common integer DO initialization code
efcode  I2DO_INIT


fcode   I4DO_INIT               ; init code for integer*4 DO-loops
        mov     bp,sp           ; get access to parms on stack
        mov     ax,8[bp]        ; get start value for DO-loop (low word)
        mov     dx,10[bp]       ; get start value for DO-loop (high word)
        mov     es:0[di],ax     ; store start value in DO-loop var (low word)
        mov     es:2[di],dx     ; store start value in DO-loop var (high word)
;;;;;;;;hop     I_Init          ; jump to common integer DO initialization code
efcode  I4DO_INIT


defn    I_init
        getword bx              ; get addr of DO-loop control block
        mov     word ptr do_var[bx],di   ; save DO var offset in control block
        mov     word ptr do_var+2[bx],es   ; save DO var segement in control block
        pop     do_incr+0[bx]   ; save DO-loop increment (low word)
        pop     do_incr+2[bx]   ; save DO-loop increment (high word)
        mov     di,bx           ; save DO-loop control block addr
        pop     ax              ; get end value (low word)
        pop     dx              ; . . . (high word)
        pop     bx              ; get start value (low word)
        pop     cx              ; . . . (high word)
        sub     ax,bx           ; calc (end-start) (low word)
        sbb     dx,cx           ; . . . (high word)
        mov     bx,do_incr+0[di]; get increment (low word)
        mov     cx,do_incr+2[di]; . . . (high word)
        or      cx,cx           ; see if increment for zero inc
        _if     e               ; if high word is zero
          or    bx,bx           ; - check for low word zero
          _if   e               ; - if it is
            RTErr DO_0INC       ; - - report the error
          _endif                ; endif
          cmp   bx,1            ; - see if increment is one
        _endif                  ; endif
        _if     ne              ; if increment is not one
          add     AX,BX         ; - calc (end-start+incr)
          adc     DX,CX         ; - . . .
          exit_fcode            ; - switch to run-time environment
          docall  Z_I4Div_      ; - calc (end-start+incr)/incr
          enter_fcode           ; - switch to F-Code environment
        _else
          add     AX,BX         ; - calc (end-start+1)
          adc     DX,CX         ; - . . .
        _endif                  ; endif
;;;;;;;;hop     Com_init        ; fall into common DO-loop init code
endproc I_init

;
; input: di - pointer to DO-loop control block
;        dx - INT( (end-start+incr) / incr ) (high word)
;        ax - INT( (end-start+incr) / incr ) (low word)
;

defn    Com_init                ; common DO-loop init code
        mov     cx,ax           ; save high word of iteration count
        getword ax              ; get address of end of DO-loop
        xchg    cx,ax           ; . . .
        xchg    si,cx           ; set PC to bottom of loop (guess 0-iter)
        _guess  zero_iter       ; guess
          or    dx,dx           ; - see if count is negative
          _quif  s              ; - quit if so
          _if    e              ; - if high word of count is zero
            or  ax,ax           ; - - see if low word is zero
            _quif e,zero_iter   ; - - quit if count is zero
          _endif                ; - endif
          sub   ax,1            ; - decrement iteration count
          sbb   dx,0            ; - ...
          mov   do_cnt+0[di],ax ; - save count (low word)
          mov   do_cnt+2[di],dx ; - . . . (high word)
          mov   si,cx           ; - reset PC, have to execute loop
          mov   do_addr[di],cx  ; - save addr of top of loop
        _endguess               ; endguess
        next                    ; execute next f-code
endproc Com_init


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;       XXDO_LOOP- increment and test at end of DO-loop
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   R16DO_LOOP              ; looping part of R*8 DO loop
        getword BX              ; get addr of DO loop control block
if _MATH eq _EMULATION
        mov     AX, BX          ; save address of DO-loop control block
        push    do_incr+14[BX]  ; push value of increment on stack
        push    do_incr+12[BX]  ; ...
        push    do_incr+10[BX]  ; ...
        push    do_incr+8[BX]   ; ...
        push    do_incr+6[BX]   ; ...
        push    do_incr+4[BX]   ; ...
        push    do_incr+2[BX]   ; ...
        push    do_incr+0[BX]   ; ...
        les     BX,do_var[BX]   ; get addr of DO-loop variable
        push    ES:14[BX]       ; get current value of DO variable
        push    ES:12[BX]       ; ...
        push    ES:10[BX]       ; ...
        push    ES:8[BX]        ; ...
        push    ES:6[BX]        ; ...
        push    ES:4[BX]        ; ...
        push    ES:2[BX]        ; ...
        push    ES:0[BX]        ; ...
        mov     DI, SI          ; save fcode ptr
        mov     SI, BX          ; this where we want the result
        exit_fcode              ; switch to run-time environment
        docall  Z_F16Add_       ; calculate new value for DO-loop variable
        enter_fcode             ; switch to F-Code environment
        mov     SI, DI          ; restore fcode ptr
        mov     BX, AX          ; restore doloop address block ptr
else
        fld     _TBYTE ptr do_incr[BX]  ; get increment value
        les     DI,do_var[BX]           ; get address of do variable
        fld     _TBYTE ptr ES:[DI]      ; add increment to do variable
        faddp   st(1), st(0)            ; ...
        fstp    _TBYTE ptr ES:[DI]      ; store the result back into the var
endif
        jmp     Com_loop                ; jump to common loop code
efcode  R16DO_LOOP


fcode   R8DO_LOOP               ; looping part of R*8 DO loop
        getword BX              ; get addr of DO loop control block
if _MATH eq _EMULATION
        push    BX              ; save address of DO-loop control block
        push    do_incr+6[BX]   ; push value of increment on stack
        push    do_incr+4[BX]   ; ...
        push    do_incr+2[BX]   ; ...
        push    do_incr+0[BX]   ; ...
        les     BX,do_var[BX]   ; get addr of DO-loop variable
        mov     DX,ES:0[BX]     ; get current value of DO variable
        mov     CX,ES:2[BX]     ; ...
        mov     AX,ES:6[BX]     ; ...
        mov     BX,ES:4[BX]     ; ...
        exit_fcode              ; switch to run-time environment
        docall  Z_F8Add_        ; calculate new value for DO-loop variable
        enter_fcode             ; switch to F-Code environment
        pop     BP              ; restore addr of DO-loop control block
        les     DI,ES:do_var[BP]; get addr of DO variable
        mov     ES:0[DI],DX     ; update DO loop variable
        mov     ES:2[DI],CX     ; . . .
        mov     ES:4[DI],BX     ; . . .
        mov     ES:6[DI],AX     ; . . .
        mov     BX,BP           ; get addr of DO-loop control block
else
        fld     qword ptr do_incr[BX]   ; get increment value
        les     DI,do_var[BX]           ; get address of do variable
        fadd    qword ptr ES:[DI]       ; add increment to do variable
        fstp    qword ptr ES:[DI]       ; store the result back into the var
endif
        hop     Com_loop                ; jump to common loop code
efcode  R8DO_LOOP


fcode   R4DO_LOOP               ; looping part of R*4 DO loop
        getword bx              ; get addr of DO loop control block
if _MATH eq _EMULATION
        push    BX              ; save address of DO-loop control block
        les     DI,do_var[BX]   ; get addr of DO loop variable
        mov     DX,do_incr+2[BX]; get the increment
        mov     AX,do_incr+0[BX]; . . .
        mov     BX,ES:0[DI]     ; get current value of variable
        mov     CX,ES:2[DI]     ; . . .
        exit_fcode              ; switch to run-time environment
        docall  Z_F4Add_        ; calc. new value for variable
        enter_fcode             ; switch to F-Code environment
        pop     BX              ; restore addr of DO-loop control block
        les     DI,do_var[BX]   ; get addr of DO loop variable
        mov     ES:0[DI],AX     ; update DO loop variable
        mov     ES:2[DI],DX     ; . . .
else
        fld     dword ptr do_incr[BX]   ; get increment value
        les     DI,do_var[BX]           ; get address of do variable
        fadd    dword ptr ES:[DI]       ; add increment to do variable
        fstp    dword ptr ES:[DI]       ; store the result back into the var
endif
        hop     Com_loop                ; jump to common loop code
efcode  R4DO_LOOP


fcode   I1DO_LOOP               ; looping part of I*1 DO-loop
        getword bx              ; get addr of DO control block
        les     di,do_var[bx]   ; get segment & addr of DO-loop var
        mov     al,byte ptr do_incr[bx]; get DO-loop increment
        add     es:[di],al      ; add on the increment
        hop     Com_loop        ; jump to common loop code
efcode  I1DO_LOOP


fcode   I2DO_LOOP               ; looping part of I*2 DO-loop
        getword bx              ; get addr of DO control block
        les     di,do_var[bx]   ; get segment & addr of DO-loop var
        mov     ax,do_incr+0[bx]; get DO-loop increment
        add     es:[di],ax      ; add on increment
        hop     Com_loop        ; jump to common loop code
efcode  I2DO_LOOP


fcode   I4DO_LOOP               ; looping part of I*4 DO-loop
        getword bx              ; get addr of DO control block
        les     di,do_var[bx]   ; get segment and addr of DO-loop var
        mov     dx,do_incr+0[bx]; get DO-loop increment (low word)
        mov     ax,do_incr+2[bx]; . . . (high word)
        add     es:0[di],dx     ; add on increment (low word)
        adc     es:2[di],ax     ; . . .            (high word)
;;;;;;;;hop     Com_loop
efcode  I4DO_LOOP


;
; input: bx - pointer to DO-loop control block
;

defn    Com_loop                ; common DO-loop incr/test code
        mov     ax,do_addr[bx]  ; get address of top of DO-loop
        xchg    ax,si           ; assume loop not finished
        _guess                  ; guess
          sub   word ptr do_cnt+0[bx],1 ; - decrement iteration count
          sbb   word ptr do_cnt+2[bx],0 ; - ...
          _quif c               ; - quit if loop finished
          next                  ; - goto next f-code
        _admit
          mov   si,ax           ; - loop done: set addr to next code
          next                  ; - goto next f-code routine
        _endguess               ; endguess
endproc Com_loop

        fmodend
        end
