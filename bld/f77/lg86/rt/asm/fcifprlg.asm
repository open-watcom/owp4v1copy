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
;<>     FCIFPRLG - intrinsic function prolog
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

include fcdef.inc
include parmtype.inc
include errcod.inc
include datamap.inc
include undef.inc

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<> Sample code sequence:
;<>     CALLER:         SPCall
;<>                     pointer to SP offset/segment
;<>                     expected return/parm count
;<>                     p1 info
;<>                     p1 offset
;<>                     p2 info
;<>                     p2 offset
;<>                     . . .
;<>                     pn info
;<>                     pn offset
;<>                     next f-code
;<>
;<>******************** Some where else in the code *********************
;<>
;<> | here on entry |   offset of intrinsic function routine
;<> | to IFPrologue |
;<>
;<> IF INFO WORD:
;<> 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
;<> 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;<> | | |     | | | |   | | |     |
;<> | | |     | | | |   | | +-----+--> return type (0..11)
;<> | | |     | | | |   | |
;<> | | |     | | | |   |
;<> | | |     | | |
;<> | | |     |
;<> | |
;<>
;<>
;<> PARM INFO WORD:
;<> 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
;<> 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;<> | |   | |     | |     | |     |
;<> | |   | |     | |     | +-----+---> type (range 0..11)
;<> | |   | |     | |     |
;<> | |   | |     |
;<> | |   |
;<> |
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

        xref    PrmCod_
        xref    PrmTypTy_
        xref    DatTypMis_
        xref    IF@LEN

        extrn   ChkChar : near
        extrn   RTError : near

        fmodstart       fcifprlg

        dataseg

        xred    "C",ArgChkFlags,   byte

        IsVarArgs db 0          ; == 1 if varargs == 0 otherwise

        GenIFName db    "intrinsic function",00
        GenIFArg  db    "intrinsic function arg", 00


        enddata

PrmRtn label word
        dw      P_Const
        dw      P_Variable
        dw      P_ArrayElt
        dw      P_SSArray
        dw      BadCode
        dw      BadCode
        dw      BadCode
        dw      BadCode

IFSize label byte
        db 0    ; PT_NO_TYPE
        db 0    ; PT_LOG*1
        db 0    ; PT_LOG*4
        db 0    ; PT_INT*1
        db 0    ; PT_INT*2
        db 0    ; PT_INT*4
        db 0    ; PT_REAL*4
        db 0    ; PT_REAL*8
        db 16   ; PT_REAL*16
        db 8    ; PT_CMPLX*8
        db 16   ; PT_CMPLX*16
        db 32   ; PT_CMPLX*32
        db 0    ; PT_CHAR

RetRtn label word
        dw      RetNull         ; no type
        dw      Ret2            ; LOGICAL*1
        dw      Ret4            ; LOGICAL*4
        dw      Ret2            ; INTEGER*1
        dw      Ret2            ; INTEGER*2
        dw      Ret4            ; INTEGER*4
if _MATH eq _8087
        dw      RetR4           ; REAL*4
        dw      RetR8           ; REAL*8
        dw      RetR16          ; REAL*16
        dw      RetC8           ; COMPLEX*8
        dw      RetC16          ; COMPLEX*16
        dw      RetC32          ; COMPLEX*32
else
        dw      Ret4            ; REAL*4
        dw      Ret8            ; REAL*8
        dw      RetNull         ; REAL*16
        dw      RetNull         ; COMPLEX*8
        dw      RetNull         ; COMPLEX*16
        dw      RetNull         ; COMPLEX*32
endif
        dw      RetNull         ; CHARACTER


LoadArg1 label word             ; load 1st arg into registers
        dw      LoadNull_1      ; PT_NO_TYPE
        dw      LoadDX_AX       ; PT_LOG1
        dw      LoadDX_AX       ; PT_LOG4
        dw      LoadDX_AX       ; PT_INT1
        dw      LoadDX_AX       ; PT_INT2
        dw      LoadDX_AX       ; PT_INT4
if _MATH eq _8087
        dw      LoadNull_1      ; PT_REAL
        dw      LoadNull_1      ; PT_DBLE
        dw      LoadNull_1      ; PT_XTND
        dw      LoadNull_1      ; PT_CPLX
        dw      LoadNull_1      ; PT_DBCX
        dw      LoadNull_1      ; PT_XTCX
else
        dw      LoadDX_AX       ; PT_REAL
        dw      LoadDBLE        ; PT_DBLE
        dw      LoadNull_1      ; PT_XTND
        dw      LoadCPLX        ; PT_CPLX
        dw      LoadDBLE        ; PT_DBCX
        dw      LoadNull_1      ; PT_XTCX
endif
        dw      LoadDX_AX       ; PT_CHAR


LoadArg2 label word             ; load 2nd arg into registers
        dw      LoadNull_2      ; PT_NO_TYPE
        dw      LoadDX          ; PT_LOG1
        dw      LoadCX_BX       ; PT_LOG4
        dw      LoadDX          ; PT_INT1
        dw      LoadDX          ; PT_INT2
        dw      LoadCX_BX       ; PT_INT4
if _MATH eq _8087
        dw      LoadNull_2      ; PT_REAL
        dw      LoadNull_2      ; PT_DBLE
        dw      LoadNull_2      ; PT_XTND
        dw      LoadNull_2      ; PT_CPLX
        dw      LoadNull_2      ; PT_DBCX
        dw      LoadNull_2      ; PT_XTCX
else
        dw      LoadCX_BX       ; PT_REAL
        dw      LoadNull_2      ; PT_DBLE
        dw      LoadNull_2      ; PT_XTND
        dw      LoadNull_2      ; PT_CPLX
        dw      LoadNull_2      ; PT_DBCX
        dw      LoadNull_2      ; PT_XTCX
endif
        dw      LoadCX_BX       ; PT_CHAR

LoadAX:
        pop     ax
LoadNull_1:
        jmp     si

LoadDX_AX:
        pop     ax
        pop     dx
        jmp     si

LoadDX:
        pop     dx
        add     sp, 2   ; clear 2 off the stack
LoadNull_2:
        jmp     loadarg2_ret

LoadCX_BX:
        pop     bx
        pop     cx
        jmp     loadarg2_ret


if _MATH eq _EMULATION

LoadDBLE:
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        jmp     si

LoadCPLX:
        pop     ax
        pop     dx
        pop     bx
        pop     cx
        jmp     si

endif


        xdefp   IFPrologue
defn    IFPrologue              ; prologue of a intrinsic function
        and     AL,TYPE_MASK    ; get return type only
        mov     DL,[SI]         ; get expected type
        and     DL,XTYP_MASK    ; mask argument checking flags
        cmp     AL,DL           ; check if types match
        _if     ne              ; if not the same then
          cbw                   ; - zero the hi byte
          mov   BX,AX           ; - get type expected
          sub   DH,DH           ; - get type sent
          mov     AX,SR_BAD_TYPE; - get error code
          exit_fcode            ; - switch to run-time environment
          docall  DatTypMis_    ; - report the error (we won't return)
        _endif                  ; endif
        mov     DL,AH           ; get the argmuent type
        shr     DL,1            ; ...
        shr     DL,1            ; ...
        and     AH,COUNT_MASK   ; leave only the arg count in AH
        inc     SI              ; point SI at the parm count
        mov     CH,[SI]         ; get the parm count
        or      CH,CH           ; see if zero
        je      NoParms         ; report error no matter what
        or      AH,AH           ; see if #args = 0 (no parm count chk)
        _if     e
          mov SS:IsVarArgs,1    ; this is a var args function
        _else
          mov SS:IsVarArgs,0    ; this is not a var args function
          _guess                ; guess
            cmp   AH,CH         ; - compare # of args with # of parms
            _quif e             ; - quit if same
NoParms:    mov   CL,CH         ; - set # of parms in CL
            xchg  AH,AL         ; - set # of parms in AL
            sub   CH,CH         ; - zero off the hi byte
            cbw                 ; - zero off the hi byte
            push  CX            ; - push # expected
            push  AX            ; - push # sent
            lea   CX,GenIFName  ; - push name
            push  SS            ; - ...
            push  CX            ; - ...
            RTErr AR_PARM_COUNT ; - report the error
          _endguess             ; endguess
        _endif
        cmp     AL,PT_CHAR      ; check if characeter i.f.
        _if     e               ; if character i.f.
          pop   BP              ; - get offset of result SCB
          pop   BX              ; - trash segment of the thing
        _endif                  ; endif
        mov     DH,AL           ; save result type
        mov     BL,AL           ; get size of result
        sub     BH,BH           ; ...
        mov     AL,byte ptr CS:IFSize[BX]
        cbw                     ; ...
        sub     SP,AX           ; allocate space for result
        shl     BX,1            ; get address of return processing routine
        push    CS:RetRtn[BX]   ; save address of return processing routine
        push    ES:4[DI]        ; push address of I.F.
        push    ES:2[DI]        ; ...
        inc     SI              ; point at first parameter info
        mov     BL,CH           ; put # of parms in bl
        sub     BH,BH           ; zero high byte
        shl     BX,1            ; multiply by 4
        shl     BX,1            ; ...
        add     SI,BX           ; point at F-Code after call
        push    SI              ; save address of next F-Code
        mov     BX,BP           ; save offset of result SCB
        mov     BP,SP           ; remember SP so we can clean args off stack
        cmp     DH,PT_CHAR      ; check if characeter i.f.
        _if     e               ; if character i.f.
          or    ch,80h          ; - indicate it's character i.f.
          push  DS              ; - push address of result SCB
          push  BX              ; - ...
        _endif                  ; endif
        mov     CL,CH           ; save number of parms in cl
        exit_fpu                ; do this now so we can put parms on fpu stack
        and     cl,7fh
        _loop                   ; loop
          sub   SI,4            ; - point at previous parm
          sub   BH,BH           ; - prepare to index BX
          mov   AX,[SI]         ; - get parameter info word
          mov   DH,AL           ; - put the parameter type into DH
          mov   BL,AH           ; - put parameter code into BL
          shl   BL,1            ; - mul index by 2
          ; Note: the indirect parm indicator has shifted into the carry bit.
          ; No instructions from here to the call can affect this bit.
          jmp   CS:PrmRtn[BX]   ; - call the processing routine
prmrtn_ret:                     ; - location routines return to
          dec   CL              ; - one less to do
        _until  e               ; until there are no more parms left
        cmp     SS:IsVarArgs,0  ; check if variable # of arguments
        _if     ne              ; if variable # of arguments
          and   ch,7fh
          xchg  CL,CH           ; - make CX number of parms on stack
          push  CX              ; - push # of arguments
        _else
          xor   dl,dl           ; - get index into table
          xchg  dh,dl           ; - ...
          mov   di,dx           ; - ...
          add   di,di           ; - ...
          _guess
            test ch,2           ; - are there 2 args?
            _quif e
            mov si,cs:LoadArg2[di]; - ret addr
          _admit
            test ch,80h         ; - is it a char i.f.?
            _quif e
            mov si,offset LoadCX_BX
          _admit
            mov si,offset loadarg2_ret
          _endif
          jmp   cs:LoadArg1[di] ; - load args
        _endif                  ; endif
loadarg2_ret:
        lea     SI,8[BP]        ; point to destination of return value
        exit_fcode              ; switch to run-time environment
        call    dword ptr 2[BP] ; execute intrinsic function
        mov     SP,BP           ; clean arguments off stack
        pop     SI              ; restore next f-code offset
        add     SP,4            ; get rid of I.F. address
        enter_fcode             ; switch to F-Code environment
        ret                     ; process return value
endproc IFPrologue


defn    Ret2
        xor     DX, DX          ; zero DX
        push    DX
        push    AX
        hop RetNull
endproc Ret2


defn    Ret4
        push    DX
        push    AX
;;;;;;;;hop RetNull
endproc Ret4


defn    RetNull
        enter_fpu
        next
endproc RetNull


if _MATH eq _8087

defn    RetR4
defn    RetR8
defn    RetR16
;       fxch    ST(4)           ; preserve the return value
        enter_fpu
        next
endproc RetR16
endproc RetR8
endproc RetR4

defn    RetC8
        enter_fpu
        mov     BP,SP           ; setup stack frame
        fld     dword ptr +4[BP]; load imag part
        fld     dword ptr [BP]  ; load real part
        next    8,1             ; remove args from stack and continue
endproc RetC8


defn    RetC16
        enter_fpu
        mov     BP,SP           ; setup stack frame
        fld     qword ptr +8[BP]; load imag part
        fld     qword ptr [BP]  ; load real part
        next    16,1            ; remove args from stack and continue
endproc RetC16


defn    RetC32
        enter_fpu
        mov     BP,SP           ; setup stack frame
        fld     _TBYTE ptr 16[BP]; load imag part
        fld     _TBYTE ptr [BP] ; load real part
        next    16,1            ; remove args from stack and continue
endproc RetC32

else

defn    Ret8
        push    AX
        push    BX
        push    CX
        push    DX
        next
endproc Ret8

endif


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Register Use:
;<>
;<>    AX - free
;<>    BX - pointer to parm value
;<>    CL - number of parameters left to process
;<>    CH - number of parameters
;<>    DL - argument type
;<>    DH - parameter type
;<>    SI - pointer to parameter info word
;<>    DI - free
;<>    BP - points at old BP on stack
;<>         If the carry flag is set, the parm is indirect
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

;
; ChkRtns are called when ArgChkFlags set.  They may only modify ax.
;
ChkRtn label word
        dw      BadType         ; no type
        dw      ChkDef1         ; LOGICAL*1
        dw      ChkDef4         ; LOGICAL*4
        dw      ChkDef1         ; INTEGER*1
        dw      ChkDef2         ; INTEGER*2
        dw      ChkDef4         ; INTEGER*4
        dw      ChkDef4         ; REAL*4
        dw      ChkDef8         ; REAL*8
        dw      ChkDef16        ; REAL*16
        dw      ChkDef8         ; COMPLEX*8
        dw      ChkDef16        ; COMPLEX*16
        dw      ChkDef32        ; COMPLEX*32
        dw      ChkString       ; CHARACTER*?

;
; PshRtns are called to place the args on the stack.  They return by
; jumping to prmrtn_ret
;
PshRtn label word
        dw      BadType         ; no type
        dw      PshL1           ; LOGICAL*1
        dw      Psh2            ; LOGICAL*4
        dw      PshI1           ; INTEGER*1
        dw      PshI2           ; INTEGER*2
        dw      Psh4            ; INTEGER*4
        dw      Psh4            ; REAL*4
        dw      Psh8            ; REAL*8
        dw      Psh16           ; REAL*16
        dw      Psh8            ; COMPLEX*8
        dw      Psh16           ; COMPLEX*16
        dw      Psh32           ; COMPLEX*32
        dw      PshChar         ; CHARACTER

if _MATH eq _8087
;
; Psh87Rtns push parms on the FPU stack.  They return like PshRtns do.
;
Psh87Rtn label word
        dw      BadType         ; no type
        dw      PshL1           ; LOGICAL*1
        dw      Psh2            ; LOGICAL*4
        dw      PshI1           ; INTEGER*1
        dw      PshI2           ; INTEGER*2
        dw      Psh4            ; INTEGER*4
        dw      PshR4           ; REAL*4
        dw      PshR8           ; REAL*8
        dw      PshR16          ; REAL*16
        dw      PshC8           ; COMPLEX*8
        dw      PshC16          ; COMPLEX*16
        dw      PshC32          ; COMPLEX*32
        dw      PshChar         ; CHARACTER
endif


defn    BadCode                 ; report a parm/arg code mismatch
        mov     BL,AH           ; set actual code in AL
        sub     BH,BH           ; zero the hi byte
        mov     DL,CL           ; get argument number
        sub     DH,DH           ; ...
        mov     AX,AR_CODI_MISMATCH ; set the error code
        exit_fcode              ; switch to run-time environment
        docall  PrmCod_         ; report the error
endproc BadCode

defn    BadType
        sub     DH,DH                   ; get passed type
        push    DX                      ; ...
        mov     DL,CL                   ; get argument number
        mov     CL,AL                   ; get expected type
        sub     CH,CH                   ; ...
        push    CX                      ; ...
        push    SS                      ; ...
        lea     CX,GenIFName            ; get the name
        push    CX
        lea     BX,GenIFArg             ; get the argument
        mov     CX, SS                  ; ...
        mov     AX,AR_TYP_MISMATCH      ; set the error code
        exit_fcode                      ; switch to run-time environment
        docall  PrmTypTy_               ; report the error
endproc BadType


defn    P_SSArray               ; process a substrung array element
P_ArrayElt:                     ; process an array element
        cmp     dh,PT_CHAR      ; see if character parm
        clc                     ; guess that it is (parm not indirect)
        _if     ne              ; if not character parm
          stc                   ; - act like parm is indirect
        _endif                  ; endif

P_Const:                        ; process a constant parameter
P_Variable:                     ; process a variable parameter
        mov     bx,2[SI]        ; get parmeter offset
        _if     c               ; if parameter is indirect
          les   bx,dword ptr [bx];- set the extra segment and offset
        _else
          mov   ax,ds           ; - set es=ds
          mov   es,ax           ; - ...
        _endif                  ; endif
        mov     AL,DH           ; get parm type into al
        cbw                     ; zero high byte
        cmp     dl,dh           ; check if it's proper type
        jne     BadType         ; ..
        mov     di, ax          ; load di with arg type
        add     di,di           ; mul by two for indexing
        test    byte ptr ss:ArgChkFlags,UNDEF_CHK
                                ; see if undef checking is on
        _if     ne
          call  CS:ChkRtn[di]   ; call routine to check the type
        _endif
if _MATH eq _8087
        test    SS:IsVarArgs,1
        _if     e
          jmp   CS:Psh87Rtn[di] ; - push the value on the stack
        _endif
endif
        jmp     CS:PshRtn[di]   ; push the value on the stack
endproc P_SSArray


defn    ChkString               ; check for character
        cmp     word ptr 2[bp],offset IF@LEN
                                ; see if it's the LEN I.F.
        _if     ne              ; if not so
          push  es              ; - save es
          push  dx              ; - save dx
          push  cx              ; - save cx
          push  di              ; - save di
          mov   cx,es:4[bx]     ; - get string length
          les   di,es:0[bx]     ; - get string pointer
          call  ChkChar         ; - check the string for undefineds
          _if   e               ; - if something was undefined then
            push ax             ; - - push location of last undef char
            push dx             ; - - push location of first undef char
            RTErr UV_IF_CHAR_ARG_UNDEFINED;- - report error
          _endif                ; - endif
          pop   di              ; - restore di
          pop   cx              ; - restore cx
          pop   dx              ; - restore dx
          pop   es              ; - restore es
        _endif                  ; endif
        ret
endproc ChkString


defn    ChkDef1
        cmp     byte ptr es:[BX],U_BYTE ; see if parm is undefined
        je      UnDefNum        ; report if undefined
        ret
endproc ChkDef1


defn    ChkDef2
        cmp     word ptr es:[bx],U_WORD; see if parm is undefined
        je      UnDefNum        ; report if undefined
        ret                     ; return
endproc ChkDef2


defn    ChkDef4
        mov     ax,U_WORD
        cmp     word ptr es:[bx],ax; see if parm is undefined
        _if     e
          cmp   word ptr es:2[bx],ax; see if parm is undefined
          je    UnDefNum        ; report if undefined
        _endif
        ret
endproc ChkDef4


defn    ChkDef32                ; check that a parm is defined
        push    cx              ; save cx
        mov     cx,16           ; 16 words to check
        jmp     short ChkDefL   ; do the check
endproc ChkDef32


defn    ChkDef16                ; check that a parm is defined
        push    cx              ; save cx
        mov     cx,8            ; 8 words to check
        jmp     short ChkDefL   ; do the check
endproc ChkDef16


defn    ChkDef8
        push    cx              ; save cx
        mov     cx,4            ; 4 words to check

defn    ChkDefL                 ; check that a parm is defined
        push    di              ; save di
        mov     di,bx           ; point DI at the parm (seg is ES)
        mov     ax,U_WORD       ; set the undefined value
        repe    scasw           ; see if parm is undefined
        pop     di              ; restore di
        pop     cx              ; restore cx
        je      UnDefNum        ; report if undefined
        ret                     ; return
endproc ChkDefL
endproc ChkDef8


defn    UnDefNum                ; report an undefined numeric parm
        sub     CH,CH           ; zero off the hi byte of arg number
        push    CX              ; save argument number on stack
        RTErr   UV_ARG_UNDEFINED; report the error
endproc UnDefNum


defn    PshChar
        push    ES              ; push address of SCB
        push    BX              ; ...
        jmp     prmrtn_ret      ; return to prologue mainline
endproc PshChar


defn    PshL1
        mov     al,byte ptr es:[bx];get parm
        cbw                     ; convert to a word
        push    ax              ; push parm
        jmp     prmrtn_ret      ; return to prologue main line
endproc PshL1


defn    PshI2
        mov     ax,word ptr es:[bx];get parm
psh_i2: mov     bx,dx           ; save type information
        cwd                     ; convert to double word
        push    dx              ; push on stack
        push    ax              ; ...
        mov     dx,bx           ; restore type information
        jmp     prmrtn_ret
endproc PshI2


defn    PshI1
        mov     al,byte ptr es:[bx];get parm
        cbw                     ; convert to a word
        jmp     psh_i2
endproc PshI1


defn    Psh32
        push    es:30[bx]       ; push 32 bytes on the stack
        push    es:28[bx]       ; . . .
        push    es:26[bx]       ; . . .
        push    es:24[bx]       ; . . .
        push    es:22[bx]       ; . . .
        push    es:20[bx]       ; . . .
        push    es:18[bx]       ; . . .
        push    es:16[bx]       ; . . .
Psh16:  push    es:14[bx]       ; push 16 bytes and return
        push    es:12[bx]       ; . . .
        push    es:10[bx]       ; . . .
        push    es:8[bx]        ; . . .
Psh8:   push    es:6[bx]        ; . . .
        push    es:4[bx]        ; . . .
Psh4:   push    es:2[bx]        ; . . .
Psh2:   push    es:0[bx]        ; . . .
        jmp     prmrtn_ret      ; return to prologue main line
endproc Psh32


if _MATH eq _8087

defn    PshR4
        fld     dword ptr es:[bx];push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR4


defn    PshR8
        fld     qword ptr es:[bx];push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR8


defn    PshR16
        fld     _TBYTE ptr es:[bx];push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR16


defn    PshC8
        fld     dword ptr es:4[bx];push parm on fpu stack
        fld     dword ptr es:[bx]; ...
        jmp     prmrtn_ret
endproc PshC8


defn    PshC16
        fld     qword ptr es:8[bx];push parm on fpu stack
        fld     qword ptr es:[bx]; ...
        jmp     prmrtn_ret
endproc PshC16


defn    PshC32
        fld     _TBYTE ptr es:16[bx];push parm on fpu stack
        fld     _TBYTE ptr es:[bx]  ; ...
        jmp     prmrtn_ret
endproc PshC32

endif

        fmodend
        end
