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
; FCSUBPGM      : Interface to sub-programs (f-code, intrins, external)
;

.8087

include fcdef.inc
include errcod.inc
include datamap.inc
include parmtype.inc
include adv.inc
include undef.inc
include structio.inc

        xref    PrmCodCo_
        xref    PrmTypTy_
        xref    DatTypMis_
        xref    LinkTrace_
        xref    UnLinkTrace_
if _MATH eq _8087
        xref    ChkLimitErr_
endif

        fcxref  VAL_LOG1
        fcxref  VAL_LOG4
        fcxref  VAL_INT1
        fcxref  VAL_INT2
        fcxref  VAL_INT4
        fcxref  VAL_REAL4
        fcxref  VAL_REAL8
        fcxref  VAL_REAL16
        fcxref  VAL_CPLX8
        fcxref  VAL_CPLX16
        fcxref  VAL_CPLX32

        extrn   IFPrologue      : near
        extrn   RTError         : near
        extrn   F77_to_Ext      : near
        extrn   StructDefnCmp_  : near

        extrn   _ArgChkFlags    : byte
        extrn   _MsgBuff        : dword

        dataseg

EnName          dd      0       ; pointer to subprogram name
        lgxdefp CallerDataBase
        lgxdefp CalledDataBase
CallerDataBase  dd      0       ; base of caller's data
CalledDataBase  dd      0       ; base of called sp's data
ArgLocale       dd      0       ; Ptr to the argument (use in error handling)

        enddata

        fmodstart       fcsubpgm

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<> Sample code sequence:
;<>     CALLER:         SPCall
;<>                     pointer to SP offset/segment
;<>                     expected return/parm count
;<>                     p1 info           --+
;<>                     p1 offset           | NOTE : an offset is not
;<>                     p2 info             |        present for
;<>                     p2 offset           |        statement numbers
;<>                     . . .               |
;<>                     pn info             |
;<>                     pn offset         --+
;<>                     next f-code
;<>
;<>******************** Some where else in the code *********************
;<>
;<>     SProgStart      <warp code for dummy arguments>
;<>                            . . .
;<>     StartProl       "flip to fcode environment" code
;<>                     offset of epilogue f-code --+     |    BLOCK DATA
;<>                     offset to traceback struct  |     |    SUBPROGRAM
;<>                     offset to entry name        |     |
;<>                     | subprogram   |            |     |
;<>                     | code         |            |     |
;<>                     | . . .        |            |     |
;<>              2      offset of current arg list        |
;<>              4      return offset storage location    |
;<>              6      return segment  "        "        |
;<>              8      function value location (16 bytes)|
;<>              8/24   (possibly other stuff)            |
;<>                     (possibly call type info ) sizeof( unsigned_16 ) if foriegn
;<>                     a1 info      --+
;<>                     a1 offset      |
;<>                     a2 info        | NOTE : an offset is not present
;<>                     a2 offset      |        for * 's ( alt returns )
;<>                     . . .          |
;<>                     an info        |
;<>                     an offset    --+
;<>
;<> INFO WORD:
;<> 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
;<> 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;<> | | | | |     | | | | | |     |
;<> | | | | |     | | | | | +-----+---> type (range 0..11)
;<> | | | | |     | | | | |
;<> | | | | |     | | | |
;<> | | | | |     | | |
;<> | | | | |     | |
;<> | | | | |     |
;<>
;<> Note 1
;<> ------
;<>     Bit 4 in the function return type is used to indicate an
;<>     intrinsic function is being called and we should use the
;<>     IFProlog routine
;<>
;<> Note 1.5
;<>     This bit is used in the parm and arg ret type and is used to determine
;<>     What type of call is being made
;<>
;<> Note 2
;<> ------
;<>     The character*(*) indicator is only used in the argument info
;<>     word and the function return type.
;<>     The parameter return word uses it as UNDEF_CHK indicator
;<>
;<> Note 2.5
;<>     The array check bit is only usedin the parm return type word
;<>
;<> Note 3
;<> ------
;<>     The indirect parm indicator is only used in the parameter info
;<>     word.
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

ParmRtn dd      P_Const
        dd      P_Variable
        dd      P_ArrayElt
        dd      P_SSArray
        dd      P_ArrayName
        dd      P_Procedure
        dd      P_Fn_Sub
        dd      P_Statment

;==========+=============================+==============================+
;\  passed |                             |                              |
; \   as   |        PC_FN_OR_SUB         |         PC_PROCEDURE         |
;  +----+  |                             |                              |
;   as    \|    type      |   no type    | type (func)  | no type (sub) |
;=====+====+==============+==============+==============+===============+
;     |    |              |              |              |               |
; PC_ |    |     don't    |   COMPILER   |              |    don't      |
;     |type|     check    |              | check types  |    check      |
; FN_ |    |     types    |    ERROR     |              |    types      |
;     |    |              |              |              |               |
;     |    |              |              |              |               |
; SUB | no |   COMPILER   |   COMPILER   |   COMPILER   |    COMPILER   |
;     |    |              |              |              |               |
;     |type|    ERROR     |    ERROR     |    ERROR     |     ERROR     |
;     |    |              |              |              |               |
;=====+====+==============+==============+==============+===============+
;     |    |              |              |              |               |
;     |    |              |   COMPILER   |              | check types:  |
; PC_ |type| check types  |              | check types  |    ERROR      |
;     |    |              |    ERROR     |              | (expect func) |
; PRO |    |              |              |              |               |
; CED |    |              |              |              |               |
;     | no |    don't     |   COMPILER   | check types: |  check types: |
; URE |    |    check     |              |    ERROR     |               |
;     |type|    types     |    ERROR     | (expect sub) | (both no type)|
;     |    |              |              |              |               |
;=====+====+==============+==============+==============+===============+

;Routine parm jump table
R_Rtns  dd      CheckType       ; p=procedure    type,r=procedure    type
        dd      WantSubr        ; p=procedure    type,r=procedure no type
        dd      WantFunc        ; p=procedure no type,r=procedure    type
        dd      NoCheck         ; p=procedure no type,r=procedure no type
        dd      CheckType       ; p=procedure    type,r=fn_or_sub    type
        dd      CompErr         ; p=procedure    type,r=fn_or_sub no type
        dd      NoCheck         ; p=procedure no type,r=fn_or_sub    type
        dd      CompErr         ; p=procedure no type,r=fn_or_sub no type
        dd      CheckType       ; p=fn_or_sub    type,r=procedure    type
        dd      NoCheck         ; p=fn_or_sub    type,r=procedure no type
        dd      CompErr         ; p=fn_or_sub no type,r=procedure    type
        dd      CompErr         ; p=fn_or_sub no type,r=procedure no type
        dd      NoCheck         ; p=fn_or_sub    type,r=fn_or_sub    type
        dd      CompErr         ; p=fn_or_sub    type,r=fn_or_sub no type
        dd      CompErr         ; p=fn_or_sub no type,r=fn_or_sub    type
        dd      CompErr         ; p=fn_or_sub no type,r=fn_or_sub no type

; routines to push function result
PshRtns dd      NoPush          ; PT_NOTYPE
        dd      VAL_LOG1        ; PT_LOG1
        dd      VAL_LOG4        ; PT_LOG4
        dd      VAL_INT1        ; PT_INT1
        dd      VAL_INT2        ; PT_INT2
        dd      VAL_INT4        ; PT_INT4
        dd      VAL_REAL4       ; PT_REAL
        dd      VAL_REAL8       ; PT_DBLE
        dd      VAL_REAL16      ; PT_XTND
        dd      VAL_CPLX8       ; PT_CPLX
        dd      VAL_CPLX16      ; PT_DBCX
        dd      VAL_CPLX32      ; PT_XTCX
        dd      NoPush          ; PT_CHAR
        dd      NoPush          ; PT_STRUCT


fcode   RT_PROLOGUE             ; call and prologue of a subprogram unit
if _MATH eq _8087               ; check if 8087 has an exception pending
        _fwait
        docall  ChkLimitErr_
endif
        getdw   eax             ; get the offset of the SubPgm offset dword
        mov     CallerDataBase,ebp
        add     eax,ebp         ; ...
        mov     cl,[esi]        ; get argument checking flags
        mov     edi,[eax]       ; get the offset of the SubPgm
        mov     _ArgChkFlags,cl ; set the argument checking flags
        cmp     word ptr [edi],WF_SEQUENCE; check if it's WATFOR-77 subprogram
        jne     F77_to_Ext      ; call external if not WATFOR-77 s.p.
        mov     edx,darg_list[edi]; get ptr to arg list descriptor
        or      edx,edx         ; check for BLOCK DATA SUBPROGRAM
        _if     e               ; if a block data subprogram called
          RTErr SR_BLKDAT_CALLED; - report error
        _endif                  ; endif
        mov     eax,[edx]       ; get result type and arg count (into ax)
        xchg    edx,edi         ; swap registers
        test    al,IF_IND       ; see if intrinsic function
        _if     ne              ; if so then
          jmp   IFPrologue      ; - jump to intrinsic function prologue
        _endif                  ; endif
        mov     ebx,epi_fcode[edx]; get pointer to SP Epilogue data
        mov     ecx,en_name[edx]; save pointer to entry name
        lea     ebx,epi_fcode+4[edx+ebx]; continue making ptr to SP Epilogue
        mov     EnName,ecx      ; ... save ptr to entry name cont'd
        mov     ecx,my_sp_data[ebx]; get called subpgm's data offset
        mov     CalledDataBase,ecx; and save it
        mov     cl,al           ; save lo byte info
        and     al,TYPE_MASK    ; get return type only
        mov     ch,[esi]        ; get expected return type
        and     ch,XTYP_MASK    ; mask off argument flags
        cmp     al,ch           ; compare with expected return type
        _if     ne              ; not the same then
          cmp   al,PT_NOTYPE    ; - check if subprogram was a subroutine
          _if   e               ; - if it was then
            RTErr SR_SUB_AS_FUNC; - - report the error
          _endif                ; - endif
          cmp   ch,PT_NOTYPE    ; - was subroutine wanted?
          _if   e               ; - if it was then
            RTErr SR_FUNC_AS_SUB; - - report the error
          _endif                ; - endif
          movzx ebx,al          ; get type expected
          movzx edx,ch          ; get type passed
          mov   eax,SR_BAD_TYPE ; - set the error code
          exit_fcode            ; - switch to run-time environ
          docall  DatTypMis_    ; - report the error
        _endif                  ; endif
        inc     esi             ; point si at the parm count
        cmp     ah,[esi]        ; compare # of parms with # of args
        _if     ne              ; if not equal then
          movzx ecx,ah          ; - get # of expected args
          xor   eax,eax         ; - zero eax
          lodsb                 ; - get # sent
          push  eax             ; - push # sent
          push  ecx             ; - push # expected
          call  GetSPName       ; - copy SP name into MsgBuff
          push  eax             ; - push pointer to name
          RTErr AR_PARM_COUNT   ; - report the error
        _endif                  ; endif
        cmp     dword ptr arg_list[ebx],0       ; see if already active
        _if     ne              ; if so then
          RTErr SR_ALREADY_ACTIVE; - report the error
        _endif                  ; endif
        mov     arg_list[ebx],edi; save current arg list descriptor
        _guess                  ; guess
          cmp   al,PT_CHAR      ; - see if character function
          _quif ne              ; - quit if not
          mov   al,cl           ; - restore the previously saved info
          pop   edi             ; - get the offset of the destination SCB
          mov   ecx,4[edi]      ; - get expected length
          mov   edi,0[edi]      ; - set the string offset value
          _guess                ; - guess
            cmp ecx,func_val+4[ebx]; - - see if return len and expected same
            _quif e             ; - - quit if they are
            test al,VAR_LEN_CHR ; - - see if varying length char func
            _quif ne            ; - - quit if it is
            push func_val+4[ebx]; - - set funct defined length on stack
            push ecx            ; - - set funct refed length on the stack
            RTErr SR_CHAR_SIZE_MISMATCH; - - report error
          _endguess             ; - endguess
          mov   func_val+4[ebx],ecx; - set function return length
          mov   func_val[ebx],edi; - set function return pointer
          mov   al,U_BYTE       ; - set undefined value
          rep   stosb           ; - UNDEFINE the result location
        _admit
          cmp   al,PT_STRUCT    ; - see if return type is structure
          _quif ne              ; - quit if not
          pop   edi             ; - get the offset of the destination RCB
          push  eax             ; - save some registers
          push  edx             ; - ...
          push  ebx             ; - ...
          mov   eax,4[edi]      ; - get offset of caller sdefn
          mov   edi,[edi]       ; - get offset of storage
          mov   edx,ebp         ; - set up parm
          mov   func_val[ebx],edi; - set offset of storage
          push  sd_total_size[edx+eax]; - save length of structure
          mov   ebx,func_val+4[ebx]; - get offset of callee sdefn
          mov   ecx,CalledDataBase
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp_  ; - compare the structures
          or    al,al           ; - check if they compare
          _if   e               ; - if they don't
            call GetSPName      ; - get subprogram name
            push eax            ; - ...
            RTErr SR_STRUCT_MISMATCH
          _endif
          enter_fcode           ; - switch to f-code environment
          pop   ecx             ; - get len of struct
          pop   ebx             ; - restore registers
          pop   edx             ; - ...
          pop   eax             ; - ...
          mov   al,U_BYTE       ; - set undefined value
          rep   stosb           ; - UNDEFINE the result location
        _admit                  ; admit
          lea   edi,func_val[ebx]; - pointer to return value
          xor   ecx,ecx         ; - assume it's a subroutine
          cmp   al,PT_NOTYPE    ; - see if a subroutine
          xchg  eax,ecx         ; - swap to save ah and get 0 into eax
          _if   ne              ; - - if not, then it's a function
            mov eax,U_DWORD     ; - undefined characters
            stosd               ; - undefine the function return value
            stosd               ; - ...
            stosd               ; - ...
          _endif
          stosd                 ; - undefine return index
          xchg  eax,ecx         ; - restore ah
        _endguess               ; endguess
        mov     edi,arg_list[ebx]; get back pointer to arg list
        inc     esi             ; point at first parameter info
        inc     edi             ; point at first argument info
        push    edx             ; save subprogram start address
        push    ebx             ; save pointer to epilogue data
        mov     ret_sp_data[ebx],ebp; save ptr to data segment
        inc     edi             ; point at first arg info cont'd
        mov     ch,ah           ; put arg count into safe reg
        sub     ebp,ebp         ; set the arg count to zero
        _loop                   ; loop
          or    ch,ch           ; - see if any args left
          _quif e               ; - quit if not
          lodsw                 ; - get parameter info word
          mov   edx,[edi]       ; - get arg info word (into dx)
          movzx ebx,ah          ; - put parameter code into ebx
          inc   ebp             ; - increment the parm count
          and   bl,CODE_MASK    ; - mask all but code
          push  edi             ; - save edi
          push  esi             ; - save esi
          push  ebx             ; - save parm code for later
          mov   esi,[esi]       ; - get parm offset
          mov   edi,2[edi]      ; - get arg offset
          add   esi,CallerDataBase
          add   edi,CalledDataBase
          mov   ArgLocale, edi  ; save for later use
          test  ah,VALUE_PARM   ; - check if argument passed by value
          jne   PassedByValue   ; - report error if passed by value
          call  dword ptr ParmRtn[ebx*4]; - call the routine
          mov   ArgLocale, 0    ; reset to NULL
          pop   ebx             ; - restore parmcode
          pop   esi             ; - restore esi
          pop   edi             ; - restore edi
          cmp   bl,PC_STATEMENT ; - was the parm a statement #
          _if   ne              ; - if it wasn't
            add edi,4           ; - - must step past offset too
            add esi,4           ; - - ...
          _endif                ; - endif
          add   edi,2           ; - point at next arg
          dec   ch              ; - one less to do
        _endloop                ; endloop
        pop     ebx             ; get back pointer to epilogue data
        pop     eax             ; get offset of start of subprogram
        mov     ret_addr[ebx],esi; save return offset
        lea     esi,sp_start[eax]; point to first F-Code
        mov     ebp,CalledDataBase; point to subprogram data
        mov     edx,en_name[eax]; point to entry name
        add     eax,tb_struct[eax]; point to traceback struct
        add     eax,tb_struct+4
        docall  LinkTrace_      ; setup traceback
        next                    ; get first opcode of SP and execute it
efcode  RT_PROLOGUE


defn    PassedByValue
        call    GetSPName               ; get subprogram name
        push    eax                     ; ...
        call    GetArgName              ; get argument name if one exists
        push    eax                     ; ...
        push    ebp                     ; arg num
        RTErr   AR_PASS_BY_MISMATCH     ; report the error
endproc PassedByValue

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Register Use:
;<>
;<>    ax  - parameter info word (free to modify)
;<>    ebx - free
;<>    cl  - free
;<>    ch  - number of arguments left to process
;<>    dx  - argument info word (free to modify)
;<>    esi - parameter offset
;<>    edi - argument offset
;<>    ebp - current argument number
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


defn    P_Const                 ; process a constant parameter
;;;;;;;;hop     P_Variable
endproc P_Const


defn    P_Variable              ; process a variable parameter
        and     dh,CODE_MASK    ; mask all but code information
        cmp     dh,PC_VARIABLE  ; see if variable
        jne     BadCode         ; report bad code if not
;;;;;;;;hop     AsVar
endproc P_Variable


defn    AsVar
        mov     cl,dl           ; save type byte of arg info
        and     dl,TYPE_MASK    ; get rid of all but type
        cmp     dl,al           ; see if types are the same
        jne     BadType         ; if bad type report error
        _guess                  ; guess
          cmp   dl,PT_CHAR      ; - see if character type
          _quif ne              ; - quit if not
          movsd                 ; - set arg SCB offset
          test  cl,VAR_LEN_CHR  ; - see if varying length char
          _if   ne              ; - if so then
            movsd               ; - - set arg length to parm length
          _else                 ; - else
            cmpsd               ; - - is parm len >= arg len?
            jb  BadSize         ; - - report bad size if not
          _endif                ; - endif
        _admit                  ; admit
          cmp   dl,PT_STRUCT    ; - see if structure type
          _quif ne              ; - quit if not
          movsd                 ; - set arg RCB offset
          push  ecx             ; - save registers
          mov   eax,[esi]       ; - get ptr to first sdefn
          mov   edx,CallerDataBase
          mov   ebx,[edi]       ; - get ptr to 2nd sdefn
          mov   ecx,CalledDataBase
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp_  ; - compare structures
          enter_fcode           ; - switch to f-code environment
          or    al,al           ; - are they the same?
          pop   ecx             ; - restore registers (don't modify flags)
          je    BadStruct       ; - report error if not
        _admit                  ; admit
          test  ah,IND_PARM     ; - see if parameter is indirect
          _quif e               ; - quit if not
          movsd                 ; - set arg offset
        _admit                  ; admit
          mov   [edi],esi       ; - set arg offset
        _endguess               ; endguess
        ret                     ; return
endproc AsVar


defn    BadStruct
        call    GetSPName               ; get subprogram name
        push    eax                     ; ...
        call    GetArgName              ; get argument name
        push    eax                     ; ...
        push    ebp                     ; arg num
        RTErr   AR_STRUCT_MISMATCH      ; report error
endproc BadStruct


defn    P_SSArray               ; process a substrung array element
;;;;;;;;hop     P_ArrayElt;
endproc P_SSArray


defn    P_ArrayElt              ; process an array element
        and     dh,CODE_MASK    ; get arg code info
        or      ah,IND_PARM     ; act like parm is indirect (for AsVar)
        cmp     dh,PC_VARIABLE  ; see if arg is a variable
        je      AsVar           ; handle as such if so
        mov     ebx,elt_adv[esi]; get the parm ADV offset
        push    elt_size[esi]   ; push the array element size
        hop     AsArray         ; try to use arg as an array
endproc P_ArrayElt


defn    P_ArrayName             ; process an array name
        mov     ebx,esi         ; set the parm ADV address
        push    adv_elsize[esi] ; push the array element size
        and     dh,CODE_MASK    ; get arg code info
;;;;;;;;hop     AsArray
endproc P_ArrayName


defn    AsArray
        cmp     dh,PC_ARRAY_NAME; see if arg is an array name
        jne     BadCode         ; report bad code if not
        mov     cl,dl           ; save arg type info
        and     dl,TYPE_MASK    ; isolate arg type info
        _guess                  ; guess
          cmp   al,dl           ; - see if types agree
          _quif e               ; - quit if they do
          test  byte ptr _ArgChkFlags,ARRAY_CHK
                                ; - array type checking on?
          jne   BadType         ; - report error if so
        _admit
          cmp   al,PT_STRUCT    ; - check if structure type
          _quif ne              ; - quit if not
          test  byte ptr _ArgChkFlags,ARRAY_CHK
          _quif e               ; - don't compare structs if no array chk
          push  eax             ; - save registers
          push  ebx             ; - ...
          push  ecx             ; - ...
          push  edx             ; - ...
          movzx eax,byte ptr adv_numss[ebx]; - find first sdefn
          movzx edx,byte ptr adv_numss[edi]; - find 2nd sdefn
          and   al,ADV_SUBSCRS  ; - calc # subscrs
          and   dl,ADV_SUBSCRS
          mov   eax,dword ptr adv_bound[ebx+eax*8]; - get ptr to first sdefn
          mov   ecx,CallerDataBase
          mov   ebx,dword ptr adv_bound[edi+edx*8]; - get ptr to 2nd sdefn
          mov   edx,ecx
          mov   ecx,sd_total_size[eax+ecx]; - get size of struct
          mov   16[esp],ecx     ; - save size of struct
          mov   ecx,CalledDataBase
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp_  ; - compare structures
          enter_fcode           ; - switch to f-code environment
          or    al,al           ; - are they the same?
          pop   edx             ; - restore registers (don't modify flags)
          pop   ecx             ; - ...
          pop   ebx             ; - ...
          pop   eax             ; - ...
          je    BadStruct       ; - report error if not
        _endguess               ; endguess
        or      byte ptr adv_numss+1[edi],ADV_NOT_INIT; say ADV in not init'd
        movsd                   ; save parm data offset in arg ADV
        pop     eax             ; get array element size
        mov     [edi],ebx       ; save parm ADV offset in arg ADV
        test    cl,VAR_LEN_CHR  ; see if arg is varying length character
        _if     ne              ; if so then
          mov   4[edi],eax      ; - set the arg array element size
        _endif                  ; endguess
        ret                     ; return
endproc AsArray


defn    BadType
        movzx   edx,dl                  ; get dummy code
        push    edx                     ; ...
        movzx   ecx,al                  ; get actual code
        push    ecx                     ; ...
        call    GetSPName               ; get subprogram name
        mov     ecx,eax                 ; ...
        call    GetArgName              ; get argument name
        mov     ebx,eax                 ; ...
        mov     edx,ebp                 ; get argument number
        mov     eax,AR_TYP_MISMATCH     ; set the error code
        docall  PrmTypTy_               ; report the error
endproc BadType


defn    BadCode                 ; report a parm/arg code mismatch
        movzx   edx,dh          ; get dummy code
        push    edx             ; ...
        movzx   ecx,ah          ; get actual code
        and     cl,CODE_MASK    ; only return code information
        push    ecx             ; ...
        call    GetSPName       ; get subprogram name
        mov     ecx,eax         ; ...
        call    GetArgName      ; Get Arg name
        mov     ebx, eax        ; ...
        mov     edx,ebp         ; get argument number
        mov     eax,AR_COD_MISMATCH; set the error code
        docall  PrmCodCo_       ; report the error
endproc BadCode


defn    BadSize                 ; report a character size mismatch
        push    -4[esi]         ; place arg size on stack
        push    -4[edi]         ; place parm size on stack
        call    GetSPName       ; ...
        push    eax             ; ...
        call    GetArgName      ; Get argument name
        push    eax             ; ...
        push    ebp             ; place arg number on stack
        RTErr   AR_ARG_CHAR_MISMATCH;
endproc BadSize


defn    P_Statment              ; process a statment number
        and     dh,CODE_MASK    ; get the arg code
        cmp     dh,PC_STATEMENT ; see if stament number
        jne     BadCode         ; report error if not
        ret                     ; return
endproc P_Statment


defn    CheckType               ; have to check types
        cmp     al,dl           ; check the types
        jne     BadType         ; report error if type mismatch
;;;;;;;;hop     NoCheck
endproc CheckType


defn    NoCheck                 ; don't have to do any type checking
        ret                     ; return
endproc NoCheck


defn    CompErr                 ; oh oh! compiler error
endproc CompErr


defn    WantSubr                ; we wanted a subroutine but got a func
        mov     ebx,AR_SUB_EXPECT; ...
;;;;;;;;hop     WantSP
endproc WantSubr


defn    WantSP
        call    GetSPName       ; ...
        push    eax             ; ...
        call    GetArgName      ; ...
        push    eax             ; ...
        push    ebp             ; place arg number on stack
        RTErr   ebx             ; report the error
endproc WantSP


defn    WantFunc                ; we wanted a func but got a subroutine
        mov     ebx,AR_FUN_EXPECT; ...
        hop     WantSP          ; report the error
endproc WantFunc


defn    P_Procedure             ; process a procedure parm
;;;;;;;;hop     P_Fn_Sub
endproc P_Procedure


defn    P_Fn_Sub                ; process a function or subroutine parm
        and     dh,CODE_MASK    ; get argument code
        cmp     dh,PC_FN_OR_SUB ; see if func/subr (not sure which)
        _if     ne              ; if not then
          cmp   dh,PC_PROCEDURE ; - see if func/subr (we know which type)
          _if   ne              ; - if not
            jmp BadCode         ; - - report bad code
          _endif                ; - endif
        _endif                  ; endif
        sub     ah,PC_PROCEDURE ; turn parm code into index
        sub     dh,PC_PROCEDURE ; turn arg code into index
        movzx   ebx,ah          ; move offset to jump register
        and     dl,TYPE_MASK    ; isolate type
        add     ebx,ebx         ; multiply by 2
        and     al,TYPE_MASK    ; isolate type
        add     bl,dh           ; add to jump register
        cmp     al,1            ; C=0 if al>PT_NOTYPE, C=1 if al==PT_NOTYPE
        adc     ebx,ebx         ; multiply by 2 and add carry
        cmp     dl,1            ; C=0 if dl>PT_NOTYPE, C=1 if dl==PT_NOTYPE
        adc     ebx,ebx         ; multiply by 2
        call    dword ptr R_Rtns[ebx*4]; call routine
        movsd                   ; pass routine to subprog
        ret                     ; return
endproc P_Fn_Sub


; Assumption:
; Arg name is right before Arg location, e.g
; +-----+------+------+
; | len | name | data |
; +-----+------+------+
defn    GetArgName
        mov     edi, ArgLocale          ; load edi with end of arg
        or      edi, edi                ; check if null
        jne     get_arg_name_true       ; if not go on
        mov     eax, offset ArgLocale   ; set to NULL name
        ret                             ; return
get_arg_name_true:                      ; proceed wit determining length
        mov     byte ptr [edi], 0       ; terminate name string
        _loop
          dec   edi                     ; decrement ptr
          cmp   byte ptr [edi], 20h     ; compare characters
        _until le                       ; loop until we find len ( 20h or less )
        inc     edi                     ; make edi point to start of string
        mov     eax, edi                ; prepare to return address
        ret                             ; and adieu
endproc GetArgName

defn    GetSPName
        mov     esi,dword ptr ds:EnName ; get pointer to name
        mov     edi,_MsgBuff            ; point to buffer
        mov     edx,edi                 ; save pointer
        _loop                           ; loop
          lodsb                         ; - get a byte
          stosb                         ; - store a byte
          test  al,al                   ; - check for NULLCHAR
        _until  e                       ; until string moved
        mov     eax,edx                 ; return pointer to name
        ret                             ; return
endproc GetSPName


fcode   RT_EPILOGUE             ; subroutine epilogue
if _MATH eq _8087               ; check if 8087 has an exception pending
        _fwait
        call    ChkLimitErr_
endif
        lea     edx,(func_val-arg_list)[esi]; edx points at fn return value
        mov     edi,(arg_list-arg_list)[esi]; get ptr to current arg list
        movzx   ebx,byte ptr [edi]; get function return type
        and     bl,TYPE_MASK    ; get the data type in bl
        mov     cl,0[edi]       ; check if it is a sys call function
        and     cl, SYSTEM_CALL ; ...
        _if     ne              ; if it is
          lea   edx,4[edi]      ; get get proper argument list
        _else
          lea   edx,2[edi]      ; get pointer to argument list
        _endif
        mov     cl,1[edi]       ; get number of arguments
        sub     eax,eax         ; get a NULL address into eax
        or      cl,cl           ; set the flags for # of args left
        _if     ne              ; if there are any args then
          _loop                 ; - loop
            cmp byte ptr 1[edx],PC_STATEMENT; - - see if alternate return
            _if ne              ; - - if arg isn't an alt return then
              mov edi,2[edx]    ; - - - get arg offset into di
              add edx,4         ; - - - add 4 to edx
              mov [edi+ebp],eax ; - - - NULL out arg ptr
            _endif              ; - - endif
            add edx,2           ; - - point edx at next arg
            dec cl              ; - - one less arg to do
          _until e              ; - until no more args
        _endif
        lea     edi,(func_val-arg_list)[esi]; edi points to fn return area
        mov     dword ptr (arg_list-arg_list)[esi],0; indicate fn is inactive
        docall  UnLinkTrace_    ; unlink traceback
        mov     ebp,(ret_sp_data-arg_list)[esi]; load up return data seg ptr
        mov     esi,(ret_addr-arg_list)[esi]; load up return address
        jmp     PshRtns[ebx*4]  ; push func ret val and exec next intr
efcode  RT_EPILOGUE


defn    NoPush                  ; no push required
        next                    ; execute next intruction
endproc NoPush


fcode   ALT_RET                 ; process an alternate return
        _guess                  ; guess : alt ret in range
          getword bx            ; - get number of alt returns
          mov   ecx,[edi]       ; - get the return index
          or    ecx,ecx         ; - quit if ret index is zero
          _quif e               ; - ...
          cmp   ecx,ebx         ; - quit if ret index > # entry points
          _quif a               ; - ...
          lea   esi,[esi+ecx*4] ; - load new PC
          add   esi,-4[esi]     ; - ...
        _admit                  ; admit : just do a normal return
          movzx ebx,bx          ; - zero high word of ebx
          lea   esi,[esi+ebx*4] ; - point SI to fcode after alt rets
        _endguess               ; endguess
        next                    ; execute next f-code
efcode  ALT_RET


fcode   SAVERETIDX              ; save the return index
        getdw   eax             ; get epifcode label in eax
        lea     edi,func_val[eax+esi]; point to return index storage area
        pop     [edi]           ; get return index off stack
        next                    ; do the next fcode
efcode  SAVERETIDX

        fmodend
        end
