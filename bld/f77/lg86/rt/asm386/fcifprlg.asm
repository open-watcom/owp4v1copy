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
; FCIFPRLG      : Interface to intrinsic functions
;

include fcdef.inc
include parmtype.inc
include errcod.inc
include datamap.inc
include undef.inc

        xref    PrmCod_
        xref    PrmTypTy_
        xref    DatTypMis_
        xref    IF@LEN
        lgxref  CallerDataBase

        extrn   ChkChar : near
        extrn   RTError : near

        dataseg

        xred    "C",ArgChkFlags,   byte

        IsVarArgs db 0          ; == 1 if varargs == 0 otherwise

        GenIFName db    "intrinsic function",00
        GenIFArg  db    "intrinsic function arg", 00

        enddata

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
;<> | | |     | | | |   | | +-----+--> return type (0..15)
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
;<> | |   | |     | |     | +-----+---> type (range 0..15)
;<> | |   | |     | |     |
;<> | |   | |     |
;<> | |   |
;<> |
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

        fmodstart       fcifprlg

PrmRtn label dword
        dd      P_Const
        dd      P_Variable
        dd      P_ArrayElt
        dd      P_SSArray
        dd      BadCode
        dd      BadCode
        dd      BadCode
        dd      BadCode

IFSize label dword
        dd 0            ; extra storage require on stack for each type
        dd 0
        dd 0
        dd 0
        dd 0
        dd 0
        dd 0
        dd 0
        dd 16
        dd 8
        dd 16
        dd 32
        dd 0

RetRtn label dword
        dd      RetNull         ; no type
        dd      Ret4            ; LOGICAL*1
        dd      Ret4            ; LOGICAL*4
        dd      Ret4            ; INTEGER*1
        dd      Ret4            ; INTEGER*2
        dd      Ret4            ; INTEGER*4
if _MATH eq _8087
        dd      RetR4           ; REAL*4
        dd      RetR8           ; REAL*8
        dd      RetR16          ; REAL*16
        dd      RetC8           ; COMPLEX*8
        dd      RetC16          ; COMPLEX*16
        dd      RetC32          ; COMPLEX*32
else
        dd      Ret4            ; REAL*4
        dd      Ret8            ; REAL*8
        dd      RetNull         ; REAL*16
        dd      RetNull         ; COMPLEX*8
        dd      RetNull         ; COMPLEX*16
        dd      RetNull         ; COMPLEX*32
endif
        dd      RetNull         ; CHARACTER

;
; tables to load args into registers
;
LoadArg1 label dword            ; load 1st arg into registers
        dd      LoadNull_1      ; PT_NO_TYPE
        dd      LoadEAX         ; PT_LOG1
        dd      LoadEAX         ; PT_LOG2
        dd      LoadEAX         ; PT_INT1
        dd      LoadEAX         ; PT_INT2
        dd      LoadEAX         ; PT_INT4
if _MATH eq _8087
        dd      LoadNull_1      ; PT_REAL
        dd      LoadNull_1      ; PT_DBLE
        dd      LoadNull_1      ; PT_XTND
        dd      LoadNull_1      ; PT_CPLX
        dd      LoadNull_1      ; PT_DBCX
        dd      LoadNull_1      ; PT_XTCX
else
        dd      LoadEAX         ; PT_REAL
        dd      LoadEDX_EAX     ; PT_DBLE
        dd      LoadNull_1      ; PT_XTND
        dd      LoadEDX_EAX     ; PT_CPLX
        dd      LoadDBCX        ; PT_DBCX
        dd      LoadNull_1      ; PT_XTCX
endif
        dd      LoadEAX         ; PT_CHAR

LoadArg2 label dword            ; load 2nd arg into registers
        dd      LoadNull_2      ; PT_NO_TYPE
        dd      LoadEDX         ; PT_LOG1
        dd      LoadEDX         ; PT_LOG2
        dd      LoadEDX         ; PT_INT1
        dd      LoadEDX         ; PT_INT2
        dd      LoadEDX         ; PT_INT4
if _MATH eq _8087
        dd      LoadNull_2      ; PT_REAL
        dd      LoadNull_2      ; PT_DBLE
        dd      LoadNull_2      ; PT_XTND
        dd      LoadNull_2      ; PT_CPLX
        dd      LoadNull_2      ; PT_DBCX
        dd      LoadNull_2      ; PT_XTCX
else
        dd      LoadEDX         ; PT_REAL
        dd      LoadECX_EBX     ; PT_DBLE
        dd      LoadNull_2      ; PT_XTND
        dd      LoadECX_EBX     ; PT_CPLX
        dd      LoadNull_2      ; PT_DBCX
        dd      LoadNull_2      ; PT_XTCX
endif
        dd      LoadEDX         ; PT_CHAR

LoadEAX:
        pop     eax
LoadNull_1:
        jmp     esi

LoadEDX:
        pop     edx
LoadNull_2:
        jmp     loadarg2_ret

if _MATH eq _EMULATION

LoadEDX_EAX:
        pop     eax
        pop     edx
        jmp     esi

LoadECX_EBX:
        pop     ebx
        pop     ecx
        jmp     loadarg2_ret

LoadDBCX:
        pop     eax
        pop     edx
        pop     ebx
        pop     ecx
        jmp     esi

endif



        xdefp   IFPrologue
defn    IFPrologue              ; prologue of a intrinsic function
        and     al,TYPE_MASK    ; get return type only
        mov     dl,[esi]        ; get expected type
        and     dl,XTYP_MASK    ; mask argument checking flags
        cmp     al,dl           ; check if types match
        _if     ne              ; if not the same then
          movzx ebx,al          ; - get type expected
          movzx edx,dl          ; - get type sent
          mov   eax,SR_BAD_TYPE ; - get error code
          exit_fcode            ; - switch to run-time environment
          docall  DatTypMis_    ; - report the error (we won't return)
        _endif                  ; endif
        xor     edx,edx         ; zero out edx
        inc     esi             ; point esi at the parm count
        mov     dh,ah           ; get the argument type
        and     ah,COUNT_MASK   ; leave only the arg count in AH
        mov     ch,[esi]        ; get the parm count
        shr     dh,1            ; put argument type into position (almost)
        or      ch,ch           ; see if parm count zero
        je      NoParms         ; report error no matter what
        or      ah,ah           ; see if #args = 0 (no parm count chk)
        sete    dl              ; set dl to 1 if is varargs
        _guess                  ; guess
          _quif e               ; - quit if #args = 0 (no parm count chk)
          cmp   ah,ch           ; - compare # of args with # of parms
          _quif e               ; - quit if same
NoParms:
          movzx ecx,ch          ; - get # of parms in ecx
          movzx eax,ah          ; - set # of parms in eax
          push  ecx             ; - push # expected
          push  eax             ; - push # sent
          push  offset32 GenIFName; - push name
          RTErr AR_PARM_COUNT   ; - report the error
        _endguess               ; endguess
        ror     edx,1           ; move varargs bit into top of edx and
                                ; put argument type into position (cont'd)
        cmp     al,PT_CHAR      ; check if characeter i.f.
        movzx   eax,al          ; get result type into eax (don't modify flags)
        _if     e               ; if character i.f. (flags from 2 instrs back)
          pop   ebx             ; - get offset of result SCB
        _endif                  ; endif
        sub     esp,IFSize[eax*4]; make extra room on stack for result
        push    RetRtn[eax*4]   ; push address of return processing rtn
        push    ebx             ; save ebx
        movzx   ebx,ch          ; put # of parms in ebx
        lea     ebx,[ebx+ebx*2] ; multiply by 3
        mov     dl,al           ; save result type
        lea     esi,1[esi+ebx*2]; first parm info + # of parms * 6
        pop     ebx             ; get offset of result SCB
        push    2[edi]          ; save pointer to I.F.
        push    esi             ; save address of next F-Code
        cmp     dl,PT_CHAR      ; check if characeter i.f.
        push    ebp             ; link ebp
        mov     ebp,esp         ; remember SP so we can clean args off stack
        _if     e               ; if character i.f. (flags from 2 instrs back)
          or    ch,80h          ; - indicates it's character i.f.
          push  ebx             ; - push address of result SCB
        _endif                  ; endif
        mov     cl,ch           ; save number of parms in cl
        exit_fpu                ; do this now so we can put parms on fpu stack
        and     cl,7fh
        _loop                   ; loop
          sub   esi,6           ; - point at previous parm
          mov   eax,[esi]       ; - get parameter info word
          movzx ebx,ah          ; - put parameter code into ebx
          mov   dl,al           ; - put the parameter type into dl
          shl   bl,1            ; - multiply index by 2 and move ind. bit
          ; Note: the indirect parm indicator has shifted into the carry bit.
          jmp   PrmRtn[ebx*2]   ; - call the processing routine
prmrtn_ret:                     ; - location routines return to
          dec   cl              ; - one less to do
        _until  e               ; until there are no more parms left
        or      edx,edx         ; check if it is a varargs function
        _if     s               ; if variable # of arguments
          movzx ecx,ch          ; - make ecx number of parms on stack
          push  ecx             ; - push # of arguments
        _else                   ; - load args into registers
          movzx edi,dh          ; - get offset into jump table
          ; once we start loading args we can't reference eax/ebx/ecx/edx
          _guess
            test ch,2           ; - are there 2 args?
            _quif e
            mov esi,LoadArg2[edi*4]; - return addr
          _admit
            test ch,80h         ; - is it a char i.f.?
            _quif e
            mov esi,offset32 LoadEDX
          _admit
            mov esi,offset32 loadarg2_ret
          _endif
          jmp   LoadArg1[edi*4] ; - load args
        _endif                  ; endif
loadarg2_ret:
        lea     esi,16[ebp]     ; point to destination of return value
        exit_fcode              ; switch to run-time environment
        call    dword ptr 8[ebp]; call intrinsic function
        mov     esp,ebp         ; clean arguments off stack
        pop     ebp             ; restore ebp
        pop     esi             ; restore next f-code offset
        pop     ebx             ; get rid of I.F. address
        enter_fcode             ; switch to F-Code environment
        ret                     ; process return value
endproc IFPrologue


defn    Ret4
        push    eax
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
        fld     dword ptr +4[esp]; load imag part
        fld     dword ptr [esp] ; load real part
        next    8,1             ; remove args from stack and continue
endproc RetC8


defn    RetC16
        enter_fpu
        fld     qword ptr +8[esp]; load imag part
        fld     qword ptr [esp] ; load real part
        next    16,1            ; remove args from stack and continue
endproc RetC16

defn    RetC32
        enter_fpu
        fld     _TBYTE ptr 16[esp]; load imag part
        fld     _TBYTE ptr [esp]; load real part
        next    32,1            ; remove args from stack and continue
endproc RetC32

else

defn    Ret8
        push    edx
        push    eax
        next
endproc Ret8

endif


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Register Use:
;<>
;<>    eax - free
;<>    ebx - pointer to parm value
;<>    cl  - number of parameters left to process
;<>    ch  - number of parameters
;<>    dl  - parameter type
;<>    dh  - argument type
;<>    high bit of edx is 1 if fn is varargs, 0 otherwise
;<>    esi - pointer to parameter info word
;<>    edi - free
;<>    ebp - points at old BP on stack
;<>         If the carry flag is set, the parm is indirect
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

;
; ChkRtns are called when ArgChkFlags set.  They may only modify ax.
;
ChkRtn label byte
        dd      BadType         ; no type
        dd      ChkDef1         ; LOGICAL*1
        dd      ChkDef4         ; LOGICAL*4
        dd      ChkDef1         ; INTEGER*1
        dd      ChkDef2         ; INTEGER*2
        dd      ChkDef4         ; INTEGER*4
        dd      ChkDef4         ; REAL*4
        dd      ChkDef8         ; REAL*8
        dd      ChkDef16        ; REAL*16
        dd      ChkDef8         ; COMPLEX*8
        dd      ChkDef16        ; COMPLEX*16
        dd      ChkDef32        ; COMPLEX*32
        dd      ChkString       ; CHARACTER*?

;
; PshRtns are called to place the args on the stack.  They return by
; jumping to prmrtn_ret
;
PshRtn label byte
        dd      BadType         ; no type
        dd      PshL1           ; LOGICAL*1
        dd      Psh4            ; LOGICAL*4
        dd      PshI1           ; INTEGER*1
        dd      PshI2           ; INTEGER*2
        dd      Psh4            ; INTEGER*4
        dd      Psh4            ; REAL*4
        dd      Psh8            ; REAL*8
        dd      Psh16           ; REAL*16
        dd      Psh8            ; COMPLEX*8
        dd      Psh16           ; COMPLEX*16
        dd      Psh32           ; COMPLEX*32
        dd      PshChar         ; CHARACTER

if _MATH eq _8087
;
; Psh87Rtns push parms on the FPU stack.  They return like PshRtns do.
;
Psh87Rtn label byte
        dd      BadType         ; no type
        dd      PshL1           ; LOGICAL*1
        dd      Psh4            ; LOGICAL*4
        dd      PshI1           ; INTEGER*1
        dd      PshI2           ; INTEGER*2
        dd      Psh4            ; INTEGER*4
        dd      PshR4           ; REAL*4
        dd      PshR8           ; REAL*8
        dd      PshR16          ; REAL*16
        dd      PshC8           ; COMPLEX*8
        dd      PshC16          ; COMPLEX*16
        dd      PshC32          ; COMPLEX*32
        dd      PshChar         ; CHARACTER
endif


defn    BadCode                 ; report a parm/arg code mismatch
        movzx   ebx,ah          ; set actual code in ebx
        movzx   edx,cl          ; get argument number
        mov     eax,AR_CODI_MISMATCH; set the error code
        exit_fcode              ; switch to run-time environment
        docall  PrmCod_         ; report the error
endproc BadCode


defn    BadType
        movzx   edx,dh                  ; get passed type
        push    edx                     ; ...
        movzx   edx,cl                  ; get argument number
        movzx   ecx,al                  ; get expected type
        push    ecx                     ; ...
        lea     ecx,GenIFName           ; get the name
        lea     ebx,GenIFArg            ; get arg name
        mov     eax,AR_TYP_MISMATCH     ; set the error code
        exit_fcode                      ; switch to run-time environment
        docall  PrmTypTy_               ; report the error
endproc BadType


defn    P_SSArray               ; process a substrung array element
P_ArrayElt:                     ; process an array element
        cmp     dl,PT_CHAR      ; see if character parm
        clc                     ; guess that it is (parm not indirect)
        _if     ne              ; if not character parm
          stc                   ; - act like parm is indirect
        _endif                  ; endif

P_Const:                        ; process a constant parameter
P_Variable:                     ; process a variable parameter
        mov     eax,dword ptr CallerDataBase
        mov     ebx,2[esi]      ; get parmeter offset
        lea     ebx,[ebx+eax]   ; don't modify carry flag
        _if     c               ; if parameter is indirect
          mov   ebx,[ebx]       ; - get the indirect parm
        _endif                  ; endif
        movzx   eax,dl          ; get the parm type into eax
        cmp     dh,dl           ; check if it's proper type
        jne     BadType         ; ...
        lea     edi,[eax*4]     ; move parm type into edi and mult by 4
        test    byte ptr ArgChkFlags,UNDEF_CHK
                                ; see if undef checking is on
        _if     ne
          call  dword ptr ChkRtn[edi]; call routine to check the type
        _endif
if _MATH eq _8087
        or      edx,edx         ; check if varargs
        _if     ns              ; if it isn't varargs then use 8087 stack
          jmp   dword ptr Psh87Rtn[edi]
        _endif
endif
        jmp     dword ptr CS:PshRtn[edi]; push the value on the stack
endproc P_SSArray


defn    ChkString               ; check for character
        cmp     dword ptr 8[ebp],offset32 IF@LEN
                                ; see if it's the LEN I.F.
        _if     ne              ; if not so
          push  edx             ; - save edx
          push  ecx             ; - save ecx
          push  edi             ; - save edi
          mov   ecx,4[ebx]      ; - get string length
          mov   edi,0[ebx]      ; - get string pointer
          call  ChkChar         ; - check the string for undefineds
          _if   e               ; - if something was undefined then
            push eax            ; - - push location of last undef char
            push edx            ; - - push location of first undef char
            RTErr UV_IF_CHAR_ARG_UNDEFINED;- - report error
          _endif                ; - endif
          pop   edi             ; - restore edi
          pop   ecx             ; - restore ecx
          pop   edx             ; - restore edx
        _endif                  ; endif
        ret
endproc ChkString


defn    ChkDef1
        cmp     byte ptr [ebx],U_BYTE; see if parm is undefined
        je      UnDefNum        ; report if undefined
        ret
endproc ChkDef1


defn    ChkDef2
        cmp     word ptr [ebx],U_WORD; see if parm is undefined
        je      UnDefNum        ; report if undefined
        ret
endproc ChkDef2


defn    ChkDef4
        cmp     dword ptr [ebx],U_DWORD; see if parm is undefined
        je      UnDefNum        ; report if undefined
        ret
endproc ChkDef4


defn    ChkDef16                ; check that a parm is defined
        mov     eax,U_DWORD     ; get undef'd dword
        cmp     8[ebx],eax      ; check 3rd dword
        _if     e
          cmp   12[ebx],eax     ; - check 4th dword
          je    ChkDefL         ; check rest of arg
        _endif
        ret
endproc ChkDef16


defn    ChkDef32                ; check that a parm is defined
        mov     eax,U_DWORD     ; get undef'd dword
        cmp     28[ebx],eax     ; check 7 dword
        jne     fin32chk
        cmp     24[ebx],eax     ; check 7 dword
        jne     fin32chk
        cmp     20[ebx],eax     ; check 7 dword
        jne     fin32chk
        cmp     16[ebx],eax     ; check 7 dword
        _if     e
          je    ChkDef16        ; check rest of arg
        _endif
fin32chk:
        ret
endproc ChkDef32


defn    ChkDef8
        mov     eax,U_DWORD     ; get undef'd dword
ChkDefL:
        cmp     [ebx],eax       ; check first dword
        _if     e
          cmp   4[ebx],eax      ; - check 2nd dword
          je    UnDefNum        ; - it is undef'd
        _endif
        ret
endproc ChkDef8


defn    UnDefNum                ; report an undefined numeric parm
        movzx   ecx,cl          ; get arg number into ecx
        push    ecx             ; put on stack
        RTErr   UV_ARG_UNDEFINED; report the error
endproc UnDefNum


defn    PshChar
        push    ebx             ; push address of SCB
        jmp     prmrtn_ret      ; return to prologue mainline
endproc PshChar


defn    PshL1
defn    PshI1
        movsx   eax,byte ptr [ebx]; get parm
        push    eax             ; push parm
        jmp     prmrtn_ret      ; return to prologue main line
endproc PshI1
endproc PshL1


defn    PshI2
        movsx   eax,word ptr [ebx]; get parm
        push    eax             ; push on stack
        jmp     prmrtn_ret
endproc PshI2


defn    Psh32
        push    28[ebx]         ; push 32 bytes
        push    24[ebx]         ; ...
        push    20[ebx]         ; ...
        push    16[ebx]         ; ...
;;;;;;  hop     Psh16           ; push the rest
endproc Psh32


defn    Psh16
        push    12[ebx]         ; push 16 bytes
        push    8[ebx]          ; ...
Psh8:   push    4[ebx]          ; ...
Psh4:   push    [ebx]           ; ...
        jmp     prmrtn_ret      ; return to prologue main line
endproc Psh16


if _MATH eq _8087

defn    PshR4
        fld     dword ptr [ebx] ;push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR4


defn    PshR8
        fld     qword ptr [ebx] ;push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR8


defn    PshR16
        fld     _TBYTE ptr [ebx]        ;push parm on fpu stack
        jmp     prmrtn_ret
endproc PshR16


defn    PshC8
        fld     dword ptr 4[ebx];push parm on fpu stack
        fld     dword ptr [ebx] ; ...
        jmp     prmrtn_ret
endproc PshC8


defn    PshC16
        fld     qword ptr 8[ebx];push parm on fpu stack
        fld     qword ptr [ebx] ; ...
        jmp     prmrtn_ret
endproc PshC16


defn    PshC32
        fld     _TBYTE ptr 16[ebx];push parm on fpu stack
        fld     _TBYTE ptr [ebx]        ; ...
        jmp     prmrtn_ret
endproc PshC32

endif

        fmodend
        end
