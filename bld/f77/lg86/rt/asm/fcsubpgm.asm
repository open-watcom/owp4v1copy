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
;<>     FCSUBPGM - run time subprogram routines
;<>
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include errcod.inc
include datamap.inc
include parmtype.inc
include adv.inc
include undef.inc
include structio.inc
include xfflags.inc

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
;<>     The array check bit is only used in the parm return type word
;<>
;<> Note 3
;<> ------
;<>     The indirect parm indicator is only used in the parameter info
;<>     word.
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

        xref    PrmCodCo_
        xref    PrmTypTy_
        xref    DatTypMis_

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

        extrn   IFPrologue        : near
        extrn   RTError           : near
        extrn   F77_to_Ext        : near
        xred    "C",StructDefnCmp,  far
if _MATH eq _8087
        extrn   FCLimErr          : near
endif

        fmodstart       fcsubpgm

        dataseg

        xred    "C",ExCurr,      word
        xred    "C",ExLinePtr,   word
        xred    "C",ArgChkFlags, byte

EnName          dw      0 ; pointer to subprogram name
                dw      0
ArgLocale       dw      0 ; Location of argument
                dw      0

        enddata

ParmRtn dw      P_Const
        dw      P_Variable
        dw      P_ArrayElt
        dw      P_SSArray
        dw      P_ArrayName
        dw      P_Procedure
        dw      P_Fn_Sub
        dw      P_Statment

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
R_Rtns  dw      CheckType       ; p=procedure    type,r=procedure    type
        dw      WantSubr        ; p=procedure    type,r=procedure no type
        dw      WantFunc        ; p=procedure no type,r=procedure    type
        dw      NoCheck         ; p=procedure no type,r=procedure no type
        dw      CheckType       ; p=procedure    type,r=fn_or_sub    type
        dw      CompErr         ; p=procedure    type,r=fn_or_sub no type
        dw      NoCheck         ; p=procedure no type,r=fn_or_sub    type
        dw      CompErr         ; p=procedure no type,r=fn_or_sub no type
        dw      CheckType       ; p=fn_or_sub    type,r=procedure    type
        dw      NoCheck         ; p=fn_or_sub    type,r=procedure no type
        dw      CompErr         ; p=fn_or_sub no type,r=procedure    type
        dw      CompErr         ; p=fn_or_sub no type,r=procedure no type
        dw      NoCheck         ; p=fn_or_sub    type,r=fn_or_sub    type
        dw      CompErr         ; p=fn_or_sub    type,r=fn_or_sub no type
        dw      CompErr         ; p=fn_or_sub no type,r=fn_or_sub    type
        dw      CompErr         ; p=fn_or_sub no type,r=fn_or_sub no type

; routines to push function result
PshRtns dw      NoPush          ; PT_NOTYPE
        dw      VAL_LOG1        ; PT_LOG1
        dw      VAL_LOG4        ; PT_LOG4
        dw      VAL_INT1        ; PT_INT1
        dw      VAL_INT2        ; PT_INT2
        dw      VAL_INT4        ; PT_INT4
        dw      VAL_REAL4       ; PT_REAL
        dw      VAL_REAL8       ; PT_DBLE
        dw      VAL_REAL16      ; PT_XTND
        dw      VAL_CPLX8       ; PT_CPLX
        dw      VAL_CPLX16      ; PT_DBCX
        dw      VAL_CPLX32      ; PT_XTCX
        dw      NoPush          ; PT_CHAR
        dw      NoPush          ; PT_STRUCT


defn    xternal
        jmp     F77_To_Ext      ; execute external subprogram
endproc xternal


fcode   RT_PROLOGUE             ; call and prologue of a subprogram unit
if _MATH eq _8087               ; check if 8087 has an exception pending
        _fwait
        test    SS:__XcptFlags,XF_LIMIT_ERR
        _if     ne
          jmp   FCLimErr
        _endif
endif
        getword di              ; get the offset of the SP offset/seg
        mov     cl,[si]         ; get argument checking flags
        les     di,[di]         ; get the offset/segment of the SP
        mov     SS:ArgChkFlags,cl; set the argument checking flags
        cmp     word ptr es:[di],WF_SEQUENCE; check if it's WATFOR-77 subprogram
        jne     xternal         ; call external if not WATFOR-77 s.p.
        mov     bx,es:en_name[di]; save pointer to entry name for error
        mov     ss:EnName,bx    ; ...
        mov     ss:EnName+2,es  ; ... reporting
        mov     dx,di           ; save offset of start of subprogram
        mov     bx,es:epi_fcode[di]; get pointer to SP Epilogue data
        mov     di,es:darg_list[di]; get pointer to arg list descriptor
        or      di,di           ; check for BLOCK DATA SUBPROGRAM
        _if     e               ; if a block data subprogram called
          RTErr SR_BLKDAT_CALLED; - report error
        _endif                  ; endif
        mov     ax,es:[di]      ; get result type and arg count
        test    al,IF_IND       ; see if intrinsic function
        _if     ne              ; if so then
          jmp   IFPrologue      ; - jump to intrinsic function prologue
        _endif                  ; endif
        mov     cl,al           ; save lo byte info
        and     al,TYPE_MASK    ; get return type only
        mov     ch,[si]         ; get expected return type
        and     CH,XTYP_MASK    ; mask off argument flags
        cmp     AL,CH           ; compare with expected return type
        _if     ne              ; not the same then
          cmp   AL,PT_NOTYPE    ; - check if subprogram was a subroutine
          _if   e               ; - if it was then
            RTErr SR_SUB_AS_FUNC; - - report the error
          _endif                ; - endif
          cmp   CH,PT_NOTYPE    ; - was subroutine wanted?
          _if   e               ; - if it was then
            RTErr SR_FUNC_AS_SUB; - - report the error
          _endif                ; - endif
          cbw                   ; get type expected
          mov   BX,AX           ; ...
          mov   DL,CH           ; get type passed
          sub   DH,DH           ; ...
          mov   AX,SR_BAD_TYPE  ; - set the error code
          exit_fcode            ; - switch to run-time environment
          docall  DatTypMis_    ; - report the error
        _endif                  ; endif
        inc     si              ; point si at the parm count
        cmp     ah,[si]         ; compare # of parms with # of args
        _if     ne              ; if not equal then
          xchg  AH,AL           ; - set # of parms in AL
          sub   AH,AH           ; - zero off the hi byte
          mov   CX,AX           ; - push # expected
          lodsb                 ; - load the # sent
          push  AX              ; - push # sent
          push  CX              ; - push # expected
          call  GetSPName       ; - get subprogram name
          push  ES              ; - ...
          push  DI              ; - ...
          RTErr AR_PARM_COUNT   ; - report the error
        _endif                  ; endif
        cmp     word ptr es:arg_list[bx],0; see if already active
        _if     ne              ; if so then
          RTErr SR_ALREADY_ACTIVE; - report the error
        _endif                  ; endif
        mov     es:arg_list[bx],di; save current arg list descriptor
        _guess                  ; guess
          cmp   al,PT_CHAR      ; - see if character function
          _quif ne              ; - quit if not
          mov   al,cl           ; - restore the previously saved info
          pop   di              ; - get the offset of the destination SCB
          pop   cx              ; - trash the segment value of address
          mov   cx,0[di]        ; - set the string offset value
          mov   es:func_val+0[bx],cx
          mov   cx,2[di]        ; - set the string segment value
          mov   es:func_val+2[bx],cx
          mov   cx,4[di]        ; - get expected length
          _guess                ; - guess
            cmp cx,es:func_val+4[bx]; - - see if return len and expected same
            _quif e             ; - - quit if they are
            test al,VAR_LEN_CHR ; - - see if varying length char func
            _quif ne            ; - - quit if it is
            push es:func_val+4[bx]; - - set funct defined length on stack
            push cx             ; - - set funct refed length on the stack
            RTErr SR_CHAR_SIZE_MISMATCH; - - report error
          _endguess             ; - endguess
          mov   es:func_val+4[bx],cx; - set function return length
          push  es              ; - save subprogram segment
          hop   undef1
        _admit
          cmp   al,PT_STRUCT    ; - see if return type is structure
          _quif ne              ; - quit if not
          pop   di              ; - get the offset of the destination RCB
          pop   cx              ; - trash segment value of it
          mov   cx,0[di]        ; - set the record offset value
          mov   es:func_val+0[bx],cx
          mov   cx,2[di]        ; - set the record segment value
          mov   es:func_val+2[bx],cx
          push  es              ; - save some registers
          push  ax              ; - ...
          push  bx              ; - ...
          push  dx              ; - ...
          mov   ax,4[di]        ; - get offset of caller sdefn
          mov   dx,ds           ; - turn into PGM ptr
          mov   bx,es:func_val+4[bx]; - get offset of callee sdefn
          mov   cx,es           ; - turn into PGM ptr
          push  es:sd_total_size[bx]; - save length of structure
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp   ; - compare the structures
          or    al,al           ; - check if they compare
          _if   e               ; - if they don't
            call  GetSPName     ; - get subprogram name
            push  ES            ; - ...
            push  DI            ; - ...
            RTErr SR_STRUCT_MISMATCH
          _endif
          enter_fcode           ; - switch to f-code environment
          pop   cx              ; - get size of structure
          pop   dx              ; - restore registers
          pop   bx              ; - ...
          pop   ax              ; - ...
undef1:
          les   di,[di]         ; - get segment/offset of result loc.
          mov   al,U_BYTE       ; - set undefined value
          rep   stosb           ; - UNDEFINE the result location
          pop   es              ; - restore subprogram segment
        _admit                  ; admit
          lea   di,func_val[bx] ; - pointer to function return value
          cmp   al,PT_NOTYPE    ; - see if a subroutine
          _quif e               ; - quit if not
          mov   cx,16           ; - undefine the function return value
          mov   al,U_BYTE       ; - . . .
          rep   stosb           ; - . . .
        _admit                  ; admit
          mov   cx,4            ; - undefine the return index
          sub   al,al           ; - . . .
          rep   stosb           ; - . . .
        _endguess               ; endguess
        mov     di,es:arg_list[bx]; get back pointer to arg list
        inc     si              ; point at first parameter info
        inc     di              ; point at first argument info
        push    dx              ; save subprogram start address
        push    bx              ; save pointer to epilogue data
        inc     di              ; point at first argument info cont'd
        mov     ch,ah           ; put arg count into safe reg
        sub     bp,bp           ; set the arg count to zero
        _loop                   ; loop
          or    ch,ch           ; - see if any args left
          _quif e               ; - quit if not
          lodsw                 ; - get parameter info word
          mov   dx,es:[di]      ; - get arg info word
          mov   bl,ah           ; - put parameter code into BX
          inc   bp              ; - increment the parm count
          and   bx,CODE_MASK    ; - mask all but code
          push  bx              ; - save parm code fore later
          add   bx,bx           ; - multiply by two
          mov   bx,cs:ParmRtn[bx];- get offset of processing routine
          push  di              ; - save di
          push  si              ; - save si
          mov   si,[si]         ; - get parm offset
          mov   di,es:2[di]     ; - get arg offset
          mov   ss:ArgLocale, di; - Save for error handler
          mov   ss:ArgLocale+2,es;- ...
          test  ah,VALUE_PARM   ; - check if passed by value
          jne   PassedByValue   ; - report error if passed by value
          call  bx              ; - call the routine
          mov   ss:0[ArgLocale], 0; - Restore to NULL
          mov   ss:2[ArgLocale], 0; - ...
          pop   si              ; - restore si
          pop   di              ; - restore di
          pop   bx              ; - restore parmcode
          cmp   bx,PC_STATEMENT ; - was the parm a statement #
          _if   ne              ; - if it wasn't
            add di,2            ; - - must step past offset too
            add si,2            ; - - ...
          _endif                ; - endif
          add   di,2            ; - point at next arg
          dec   ch              ; - one less to do
        _endloop                ; endloop
        pop     bx              ; get back pointer to epilogue data
        mov     ax,es           ; set DS to segment address of subprogram
        mov     word ptr es:ret_addr+2[bx],ds; save return segment
        mov     ds,ax           ; continue setting DS...
        mov     word ptr es:ret_addr+0[bx],si; save return offset
        pop     di              ; get offset of start of subprogram
        lea     si,sp_start[di] ; point to first F-Code
        mov     ax,en_name[di]  ; point to entry name
        mov     di,tb_struct[di]; point to traceback struct
        mov     bx,ss:ExCurr    ; save pointer to prev. trace back struct
        mov     cx,ss:ExCurr+2  ; . . .
        mov     dx,ss:ExLinePtr ; save pointer to prev. line
        mov     name_tb[di],ax  ; point traceback struct to entry name
        mov     word ptr link_tb[di],bx  ; save pointer to prev. trace back struct
        mov     word ptr link_tb+2[di],cx; . . .
        mov     lineptr_tb[di],dx  ; save pointer to prev. line
        mov     ss:ExCurr,di    ; point to current traceback struct
        mov     ss:ExCurr+2,ds  ; . . .
        next                    ; get first opcode of SP and execute it
efcode  RT_PROLOGUE

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Register Use:
;<>
;<>    AX - parameter info word (free to modify)
;<>    BX - free
;<>    CL - free
;<>    CH - number of arguments left to process
;<>    DX - argument info word (free to modify)
;<>    SI - parameter offset (relative to DS)
;<>    DI - argument offset  (relative to ES)
;<>    BP - current argument number
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


defn    PassedByValue
        call    GetSPName       ; get subprogram name
        push    ES              ; ...
        push    DI              ; ...
        call    GetArgName      ; get Arg name
        push    ES              ; ...
        push    DI              ; ...
        push    BP              ; place arg number on stack
        RTErr   AR_PASS_BY_MISMATCH ; report error
endproc PassedByValue


defn    BadStruct               ; structure types not equivalent
        call    GetSPName       ; get subprogram name
        push    ES              ; ...
        push    DI              ; ...
        call    GetArgName      ; get Arg name
        push    ES              ; ...
        push    DI              ; ...
        push    BP              ; place arg number on stack
        RTErr   AR_STRUCT_MISMATCH
endproc BadStruct


jmpBadCode:
        jmp     BadCode


defn    P_Statment              ; process a statment number
        and     dh,CODE_MASK    ; get the arg code
        cmp     dh,PC_STATEMENT ; see if stament number
        jne     jmpBadCode      ; report error if not
        ret                     ; return
endproc P_Statment


defn    P_Const                 ; process a constant parameter
;;;;;;;;hop     P_Variable
endproc P_Const


defn    P_Variable              ; process a variable parameter
        and     dh,CODE_MASK    ; mask all but code information
        cmp     dh,PC_VARIABLE  ; see if variable
        jne     jmpBadCode      ; report bad code if not
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
          movsw                 ; - set arg SCB offset
          movsw                 ; - set arg SCB segment
          test  cl,VAR_LEN_CHR  ; - see if varying length char
          _if   ne              ; - if so then
            movsw               ; - - set arg length to parm length
          _else                 ; - else
            cmpsw               ; - - is parm len >= arg len?
            _if b               ; - - report bad size if not
                jmp  BadSize    ; - - . . .
            _endif              ; - - . . .
          _endif                ; - endif
        _admit                  ; admit
          cmp   dl,PT_STRUCT    ; - see if structure type
          _quif ne              ; - quit if not
          movsw                 ; - set arg RCB offset
          movsw                 ; - set arg RCB segment
          push  cx              ; - save registers
          mov   ax,[si]         ; - get ptr to first sdefn
          mov   dx,ds           ; - ...
          mov   bx,es:[di]      ; - get ptr to 2nd sdefn
          mov   cx,es           ; - ...
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp   ; - compare structures
          enter_fcode           ; - switch to f-code environment
          or    al,al           ; - are they the same?
          pop   cx              ; - restore regs (don't modify flags)
          je    BadStruct       ; - report error if not
        _admit
          test  ah,IND_PARM     ; - see if parameter is indirect
          _quif e               ; - quit if not
          movsw                 ; - set arg offset
          movsw                 ; - set arg segment
        _admit                  ; admit
          mov   ax,si           ; - set arg offset
          stosw                 ; - . . .
          mov   ax,ds           ; - use current data segment
          stosw                 ; - set the arg segment
        _endguess               ; endguess
        ret                     ; return
endproc AsVar


defn    CheckType               ; have to check types
        cmp     al,dl           ; check the types
        jne     BadType         ; report error if type mismatch
;;;;;;;;hop     NoCheck
endproc CheckType


defn    NoCheck                 ; don't have to do any type checking
        ret                     ; return
endproc NoCheck


defn    P_SSArray               ; process a substrung array element
;;;;;;;;hop     P_ArrayElt;
endproc P_SSArray


defn    P_ArrayElt              ; process an array element
        and     dh,CODE_MASK    ; get arg code info
        or      ah,IND_PARM     ; act like parm is indirect (for AsVar)
        cmp     dh,PC_VARIABLE  ; see if arg is a variable
        je      AsVar           ; handle as such if so
        mov     bx,elt_adv[si]  ; get the parm ADV offset
        push    elt_size[si]    ; push the array element size
        hop     AsArray         ; try to use arg as an array
endproc P_ArrayElt


defn    BadType
        sub     DH,DH                   ; get dummy code
        push    DX                      ; ...
        mov     CL,AL                   ; get actual code
        sub     CH,CH                   ; ...
        push    CX                      ; ...
        call    GetSPName               ; get subprogram name
        push    ES                      ; ...
        push    DI                      ; ...
        call    GetArgName              ; get Arg name
        mov     BX,DI                   ; ...
        mov     CX,ES                   ; ...
        mov     DX,BP                   ; get argument number
        mov     AX,AR_TYP_MISMATCH      ; set the error code
        exit_fcode                      ; switch to run-time environment
        docall  PrmTypTy_               ; report the error
endproc BadType


defn    BadCode                 ; report a parm/arg code mismatch
        mov     DL,DH           ; get dummy code
        sub     DH,DH           ; ...
        push    DX              ; ...

        mov     CL,AH           ; get actual code
        and     CL,CODE_MASK    ; only return code information
        sub     CH,CH           ; ...
        push    CX              ; ...

        call    GetSPName       ; get subprogram name
        push    ES              ; ...
        push    DI              ; ...
        call    GetArgName      ; get Arg name
        mov     BX,DI           ; ...
        mov     CX,ES           ; ...

        mov     DX,BP           ; get argument number

        mov     AX,AR_COD_MISMATCH ; set the error code
        exit_fcode              ; switch to run-time environment
        docall  PrmCodCo_       ; report the error
endproc BadCode


defn    P_ArrayName             ; process an array name
        mov     bx,si           ; set the parm ADV address
        push    adv_elsize[bx]  ; push the array element size
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
          test  byte ptr SS:ArgChkFlags,ARRAY_CHK
                                ; - array type checking on?
          jne   BadType         ; - report error if so
        _admit
          cmp   al,PT_STRUCT    ; - check if they're structures
          _quif ne
          test  byte ptr SS:ArgChkFlags,ARRAY_CHK
          _quif e
          add   sp,2            ; - get rid of sdefn
          push  ax              ; - save registers
          push  bx              ; - ...
          push  cx              ; - ...
          mov   al,byte ptr adv_numss[bx]; - find src sdefn
           mov  dl,byte ptr es:adv_numss[di]; - find dest sdefn
          and   ax,ADV_SUBSCRS
           and  dx,ADV_SUBSCRS
          add   ax,ax           ; - multiply ax by 6 and add to adv_bound + bx
           add  dx,dx           ; - multiply dx by 6 and add to adv_bound + di
           mov  cx,di
          add   bx,ax
           add  cx,dx
          add   ax,ax
           add  dx,dx
          add   bx,ax
           add  dx,cx
          mov   ax,adv_bound[bx]; - get pointer to src sdefn
          mov   bx,dx
          mov   cx,es
          mov   bx,es:adv_bound[bx]; - get dest sdefn
          mov   dx,ds
          push  es:sd_total_size[bx]; - save size of struct
          exit_fcode            ; - switch to run-time environment
          call  StructDefnCmp   ; - compare structures
          enter_fcode           ; - switch to f-code environment
          or    al,al           ; - are they the same?
          pop   dx              ; - restore registers (elm size in dx)
          pop   cx              ; - ...
          pop   bx              ; - ...
          pop   ax              ; - ...
          _if   e
            jmp BadStruct       ; - - and report error
          _endif
        _admit
          pop   dx              ; - get element size
        _endguess               ; endguess
        or      byte ptr es:adv_numss+1[DI],ADV_NOT_INIT; say ADV in not init'd
        movsw                   ; save parm data offset in arg ADV
        movsw                   ; save parm data segment in arg ADV
        mov     ax,bx           ; save parm ADV offset in arg ADV
        stosw                   ; . . .
        mov     ax,ds           ; save parm ADV segment in arg ADV
        stosw                   ; . . .
        mov     ax,dx           ; get array element size
        test    cl,VAR_LEN_CHR  ; see if arg is varying length character
        _if     ne              ; if so then
          stosw                 ; - set the arg array element size
        _endif                  ; endguess
        ret                     ; return
endproc AsArray


defn    BadSize                 ; report a character size mismatch
        push    -2[SI]          ; place arg size on stack
        push    ES:-2[DI]       ; override to subprog seg & place
                                ; parm size on stack
        call    GetSPName       ; ...
        push    ES              ; ...
        push    DI              ; ...
        call    GetArgName      ; ...
        push    ES              ; ...
        push    DI              ; ...
        push    BP              ; place arg number on stack
        RTErr   AR_ARG_CHAR_MISMATCH;
endproc BadSize


defn    CompErr                 ; oh oh! compiler error
endproc CompErr


defn    WantSubr                ; we wanted a subroutine but got a func
        mov     BX,AR_SUB_EXPECT; ...
;;;;;;;;hop     WantSP
endproc WantSubr


defn    WantSP
        call    GetSPName       ; ...
        push    ES              ; ...
        push    DI              ; ...
        call    GetArgName      ; ...
        push    ES              ; ...
        push    DI              ; ...
        push    BP              ; place arg number on stack
        RTErr   BX              ; report the error
endproc WantSP


defn    WantFunc                ; we wanted a func but got a subroutine
        mov     bx,AR_FUN_EXPECT; ...
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
        sub     bx,bx           ; clear jump register

        sub     ah,PC_PROCEDURE ; turn parm code into index
        sub     dh,PC_PROCEDURE ; turn arg code into index
        mov     bl,ah           ; move offset to jump register
        and     dl,TYPE_MASK    ; isolate type
        shl     bx,1            ; multiply by 2
        and     al,TYPE_MASK    ; isolate type
        add     bl,dh           ; add to jump register
        cmp     al,1            ; C=0 if al>PT_NOTYPE, C=1 if al==PT_NOTYPE
        adc     bx,bx           ; multiply by 2 and add carry
        cmp     dl,1            ; C=0 if dl>PT_NOTYPE, C=1 if dl==PT_NOTYPE
        adc     bx,bx           ; multiply by 2
        shl     bx,1            ; turn index into offset
        call    word ptr R_Rtns[bx]; call routine (if we get back it's ok)
        movsw                   ; pass routine to subprog
        movsw                   ; . . .
        ret                     ; return
endproc P_Fn_Sub


; Assumption:
; Arg name is right before Arg location, e.g
; +-----+------+------+
; | len | name | data |
; +-----+------+------+
defn    GetArgName
        les     di, ss:ArgLocale        ; load es:di with end of arg
        mov     ax, es                  ; work on es
        or      ax, ax                  ; check if NULL
        jne     get_arg_name_true       ; if not go on
        mov     ax, ss                  ; set to null string
        mov     es, ax
        mov     di, offset ArgLocale    ; ...
        ret                             ; return
get_arg_name_true:                      ; proceed wit determining length
        cmp     di, 20h                 ; check if we need to adjust the ptr
        ja      go_on_get_arg_name      ; if not go on
        sub     ax, 2                   ; shift down 32 bytes ( 2 paragraphs)
        mov     es, ax                  ; ...
        add     di, 20h                 ; inc di
go_on_get_arg_name:
        mov     byte ptr es:[di], 0     ; terminate name string
        _loop
          dec   di                      ; decrement ptr
          cmp   byte ptr es:[di], 20h   ; compare characters
        _until le                       ; loop until we find len ( 20h or less )
        inc     di                      ; make edi point to start of string
        ret                             ; and adieu
endproc GetArgName


defn    GetSPName
        les     DI,dword ptr SS:EnName  ; get pointer to name
        ret                             ; return
endproc GetSPName


fcode   RT_EPILOGUE             ; subroutine epilogue
if _MATH eq _8087               ; check if 8087 has an exception pending
        _fwait
        test    SS:__XcptFlags,XF_LIMIT_ERR
        _if     ne
          jmp   FCLimErr
        _endif
endif
        push    ds              ; set es=ds
        pop     es              ; . . .
        lea     dx,func_val-arg_list[si]; dx points at fn return value
        mov     di,arg_list-arg_list[si]; get pointer to current arg list
        mov     bl,0[di]        ; get function return type
        sub     bh,bh           ; convert byte to word
        and     bl,TYPE_MASK    ; get the data type in bl
        shl     bx,1            ; mul by two for indexing
        mov     bp,bx           ; save table index
        mov     cl,0[di]        ; see if syscall
        and     cl,SYSTEM_CALL  ; ...
        _if     ne              ; if it is
          lea   bx,4[di]        ; get adjusted pointer to argument list
        _else
          lea   bx,2[di]        ; get pointer to argument list
        _endif
        mov     cl,1[di]        ; get number of arguments
        sub     ax,ax           ; get a NULL address into ax
        or      cl,cl           ; set the flags for # of args left
        _loop                   ; loop
          _quif e               ; - quit if no more args to do
          cmp   byte ptr 1[bx],PC_STATEMENT; - see if alternate return
          _if   ne              ; if arg isn't an alt return then
            mov di,2[bx]        ; - - get arg offset into di
            stosw               ; - - NULL out arg offset and segment
            stosw               ; - - values ...
            add bx,2            ; - - add 2 to bx
          _endif                ; - endif
          add   bx,2            ; - point bx at next arg
          dec   cl              ; - one less arg to do
        _endloop                ; endloop
        mov     di,ss:ExCurr    ; get offset to current traceback struct
        mov     word ptr arg_list-arg_list[si],0; indicate rtn is inactive
        mov     ax,word ptr link_tb[di]; get offset to previous traceback struct
        mov     bx,word ptr link_tb+2[di]; get seg addr of prev traceback
        mov     cx,lineptr_tb[di]  ; restore pointer to previous line
        mov     ss:ExCurr,ax    ; restore it
        mov     ss:ExCurr+2,bx  ; restore it
        mov     ss:ExLinePtr,cx ; . . .
        mov     di,dx           ; restore DI
        lds     si,dword ptr (ret_addr-arg_list)[si]; load up return address
        jmp     cs:PshRtns[bp]  ; push func ret val and exec next intr
efcode  RT_EPILOGUE


defn    NoPush                  ; no push required
        next                    ; execute next intruction
endproc NoPush


fcode   ALT_RET                 ; process an alternate return
        _guess                  ; guess : alt ret in range
          getword bx            ; - get number of alt returns
          mov   CX,ES:[DI]      ; - get the return index
          mov   DX,ES:2[DI]     ; - ...
          or    dx,dx           ; - quit if high word of ret idx not zero
          _quif ne              ; - ...
          or    cx,cx           ; - quit if ret index is zero
          _quif e               ; - ...
          cmp   cx,bx           ; - quit if ret index > # entry points
          _quif a               ; - ...
          dec   cx              ; - decrement index
          shl   cx,1            ; - multiply by 2
          add   si,cx           ; - point to proper return address
          mov   si,[si]         ; - load it into SI
        _admit                  ; admit : just do a normal return
          shl   bx,1            ; - multiply # of alt rets by 2
          add   si,bx           ; - point SI to fcode after alt rets
        _endguess               ; endguess
        next                    ; execute next f-code
efcode  ALT_RET


fcode   SAVERETIDX              ; save the return index
        getword DI              ; get epifcode label in di
        lea     DI,func_val[DI] ; get return index storage address
        pop     [di]            ; pop return index off stack
        pop     2[di]           ; ...
        next                    ; do the next fcode
efcode  SAVERETIDX

        fmodend
        end
