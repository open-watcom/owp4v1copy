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
;<> FGNOBJ      : run-time support for object files
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include parmtype.inc
include datamap.inc
include errcod.inc
include undef.inc
include fcodes.inc
include structio.inc

        xref    WF77_To_Ext
        xref    Ext_To_WF77

        extrn   RTError : near

        fmodstart       fgnobj

        dataseg

        extrn   _ExCurr : word
        extrn   _ExLinePtr : word

        enddata


; On entry: ES:DI points to first byte of called subroutine
;           DS:SI points to first word of caller parameter list

        xdefp   F77_to_Ext
defn    F77_to_Ext              ; interface to non_watfor77 subroutines
        mov     BX,[SI]         ; get subroutine type
        and     BL,TYPE_MASK    ; ...
        _guess
          cmp   BL,PT_CHAR      ; - is it a character function
          _quif e
          cmp   bl,PT_STRUCT
          _quif e
          sub   BH,BH           ; - zero the hi bytes
          mov   CL,byte ptr CS:XRetSize[BX]
                                ; - get the size of the return value
          sub   CH,CH           ; - zero the hi byte
          shr   CX,1            ; - get number of words
          mov   AX,U_WORD       ; - set AX to undefined value
          _loop                 ; - loop : undefine the return value
            or  CX,CX           ; - - are we done?
            _quif e             ; - - quit if so
            push AX             ; - - save the value
            dec  CX             ; - - decrement number of words
          _endloop              ; - endloop
          mov   cx,sp           ; get ptr to return storage
        _admit
          pop   BX              ; - get offset to SCB into BX
          pop   CX              ; - trash segment value
          push  4[BX]           ; - copy over the SCB
          push  2[BX]           ; - ...
          push  0[BX]           ; - ...
          mov   cx,sp           ; - get ptr to return storage
        _endguess
        push    DS              ; save the current fcode seg
        push    SI              ; save the current fcode pointer
;
; Link a traceback structure.
;
        sub   SP,size traceback_s; allocate trace-back struct on stack
        mov   BP,SP             ; get struct addr in BP
        sub   AX,AX             ; set up a NULL 3 NULL special name
        push  AX                ; push the 'name'
        mov   AX,0300h          ; ...
        push  AX                ; ...
        push  ES                ; save callee segment
        push  SS                ; set ES to SS
        pop   ES                ; ...
        call  SetTBack          ; set up trace-back structure
        mov   SS:_ExCurr+2,SS   ; set the proper data segment for the traceback
        pop   ES                ; restore callee segment
        mov   NAME_TB[BP],SP    ; save pointer to special name
        sub   AX,AX             ; set ExLinePtr to NULL
        mov   SS:_ExLinePtr,AX  ; ...
        mov   BP,CX             ; put return storage pointer back in BP
;
; Call interface code:
;     ES:DI contains 32-bit address of external subprogram
;     DS:SI points to WATFOR-77 argument list
;     SS:BP points to storage for return value
;
        docall   WF77_To_Ext      ; call the interface code
;
; Unlink traceback structure.
;
        add   SP,4              ; remove special name
        mov   BP,SP             ; get the trace back struct addr in BP
        mov   AX,word ptr LINK_TB[BP]   ; un-link the trace back struct
        mov   SS:_ExCurr,AX     ; ...
        mov   AX,word ptr LINK_TB+2[BP] ; un-link the trace back struct
        mov   SS:_ExCurr+2,AX   ; ...
        mov   AX,LINEPTR_TB[BP] ; un-link the trace back struct
        mov   SS:_ExLinePtr,AX  ; ...
        add   SP,size traceback_s; remove the struct from the stack
;
; Clean-up and return to WATFOR-77 subprogram.
;
        pop   SI                ; restore fcode pointer
        pop   DS                ; ...
        getword ax              ; get the parm count/ret type in AX
        mov   CL,AL             ; see if it is a system call
        and   CL,SYSTEM_CALL    ; if it is, clear call type
        _if   ne                ; ...
          push  ax              ; save ax
          getword ax            ; ...
          pop   ax              ; restore ax
        _endif
        and   AL,TYPE_MASK      ; get the ret type
        _guess
          cmp   AL,PT_CHAR      ; - is it a character function
          _quif ne
          add   SP,6            ; - remove SCB from the stack
        _admit
          cmp   al,PT_STRUCT    ; - is it a structure function
          _quif ne
          add   sp,6            ; - remove RCB from the stack
if _MATH eq _8087
        _admit
          cmp   AL,PT_REAL      ; check if it's floating point
          _quif l
          mov    BL,AL          ; get type of return value
          sub    BH,BH          ; ...
          mov    CL,byte ptr CS:XRetSize[BX]
                                ; get size of return value
          sub    CH,CH          ; ...
          shl    BX,1           ; make index for word table
          mov    BP,SP          ; point to argument on stack
PT_REAL_2 equ   (2*PT_REAL)     ; to get around a bug in WASM
          jmp    word ptr CS:X87Val-PT_REAL_2[BX]
                                ; put return value on 80x87 stack
X87Val:   dw     X87R4          ; PT_REAL
          dw     X87R8          ; PT_DBLE
          dw     X87R16         ; PT_XTND
          dw     X87C8          ; PT_CPLX
          dw     X87C16         ; PT_DBCX
          dw     X87C32         ; PT_XTCX
          dw     X87Null        ; PT_CHAR

X87C8:    fld   dword ptr 4[BP]
X87R4:    fld   dword ptr [BP]
          hop   X87Null

X87C16:   fld   qword ptr 8[BP]
X87R8:    fld   qword ptr [BP]
          hop   X87Null

X87C32:   fld   _TBYTE ptr 16[BP]
X87R16:   fld   _TBYTE ptr [BP]
X87Null:
          _fwait                ; wait for the fpu
          add   SP,CX           ; remove return value from cpu stack
endif
        _endguess
        mov   CL,AH             ; get number of parms
        _loop                   ; loop - step across the parm info
           or CL,CL             ; - have we done all the parms
           _quif e              ; - quit if we have
           getword ax           ; - get the parm info word
           and AH,TYPE_MASK     ; - get the parm type
           cmp AH,PC_STATEMENT  ; - is it an alternate return?
           _if  ne              ; - if it isn't...
             inc  SI            ; - - step past the offset
             inc  SI            ; - - ...
           _endif               ; - endif
           dec    CL            ; - decrement parm count
        _endloop                ; endloop
        next                    ; do the next fcode
endproc F77_to_Ext

;
; This routine allows external subprograms to call WATFOR-77 subprograms.
;
; On Entry:
; =========
;                |   /|\    |
;                |    |     |
;                |          | <-- 4-byte address of WATFOR-77 subprogram
;                |          |     (placed on stack by long call)
;                |          | <-- return address of external subprogram
;                |          |     (placed on stack by long call)
;
; When Ext_to_WF77 receives control:
; ==================================
;
;                |   /|\    |
;                |    |     |
;                |          | <-- address of routine to call WATFOR-77
;                |          |     subprogram
;                |          | <-- 4-byte address of WATFOR-77 argument
;                |          |     list
;                |          | <-- return address of external subprogram
;                |          |     (placed on stack by long call)
;
; NOTE: We cannot assume that SS is WATFOR-77 stack segment.
;       MS FORTRAN has SS=DS which is their data segment.
;       We must save all registers we use before invoking the interface
;       code since we don't know what registers the calling convention
;       uses.

        xdefp   Ext_to_F77
defn    Ext_to_F77
        push  BP                ; save BP
        push  DS                ; save DS
        push  SI                ; save SI
        mov   BP,SP             ; get accessability to stack
        lds   si,6[BP]          ; point to WATFOR-77 subprogram
        sub   SI,SP_HDR_ID      ; ...
        mov   CS:WF_Addr,SI     ; save address of WATFOR-77 subprogram
        mov   CS:WF_Addr+2,DS   ; ... for later use
        cmp   word ptr DARG_LIST[SI],0
        _if   e                 ; if block data subprogram
          lea SI,DARG_LIST[SI]  ; - point to NULL (no args/PT_NOTYPE)
        _else                   ; else
          mov SI,DARG_LIST[SI]  ; - point to dummy argument list
          test byte ptr [SI],IF_IND
          _if  ne               ; - if intrinsic function
            call IFArgList      ; - - build argument list
          _endif                ; - endif
        _endif                  ; endif
        mov   6[BP],SI          ; pass pointer to dummy argument list
        pop   SI                ; restore SI
        pop   DS                ; restore DS
        pop   BP                ; restore BP
        docall Ext_To_WF77      ; call routine to convert argument list
;
; The following will be the called to invoke the WATFOR-77 subprogram.
; We MUST have the WATFOR-77 stack segment in SS at this point.
; If a character function is called, then ES:DI points to the
; destination and CX is the length of the string. CX is 0 if external
; subprogram only passes pointer to string and not length.
; if it's a structure function, es:di points to the storage for the ret val

        lds   si,dword ptr CS:WF_Addr; get address of WATFOR-77 subprogram
        mov   BX,DARG_LIST[SI]  ; get arg list pointer
        or    BX,BX             ; check if block data subprogram
        _if   e                 ; if block data subprogram
          RTErr SR_BLKDAT_CALLED; - report the error
        _endif                  ; endif
        mov   DX,[BX]           ; get info word
        and   DL,IF_IND         ; see if intrinsic function
        _if   ne                ; if it is ...
          dojmp IFCall          ; - call the intrinsic function
        _endif                  ; endif
        mov   BX,EPI_FCODE[SI]  ; get pointer to epilogue fcode
        cmp   word ptr ARG_LIST[BX],0
        _if   ne                ; if subprogram already active
          RTErr SR_ALREADY_ACTIVE;- report the error
        _endif                  ; endif
        mov   AX,DARG_LIST[SI]  ; indicate we are active
        mov   ARG_LIST[BX],AX   ; ...
        mov   BX,AX             ; point to dummy argument list
        mov   AX,[BX]           ; get ret type, arg count
        and   AX,TYPE_MASK      ; get the return type
        mov   BX,EPI_FCODE[SI]  ; get pointer to epilogue fcode
        lea   BX,FUNC_VAL[BX]   ; point to return value
        push  AX                ; save return type
        _guess                  ; guess it's a function
          cmp AL,PT_CHAR        ; - is it a character function?
          _quif ne              ; - quit if it's not a character function
          mov [BX],DI           ; - set pointer to return value
          mov 2[BX],ES          ; - ...
          or  CX,CX             ; - check if length passed
          _if  ne               ; - if length passed
            mov 4[BX],CX        ; - - set length of string
          _endif                ; - endif
        _admit
          cmp   al,PT_STRUCT    ; - is it a structure function?
          _quif ne
          push  bx
          mov   bx,4[bx]        ; - get sdefn ptr from RCB
          mov   cx,sd_total_size[bx]; ...
          pop   bx              ; ...
          _guess
            cmp cx,1            ; - is it a 1 byte struct?
            _quif e
            cmp cx,2            ; - or a 2 byte struct?
            _quif e
            cmp cx,4            ; - or a 4 byte struct?
            _quif e
          _admit
            lea di,8[bx]        ; - use extra space in ret val area
            push ds             ; - ...
            pop es              ; - ...
          _endguess
          mov   [bx],di         ; - set ptr to ret storage
          mov   2[bx],es        ; - ...
        _admit                  ; admit
          cmp AL,PT_NOTYPE      ; - is it a subroutine?
          _quif e               ; - quit if it's a subroutine
          mov CX,8              ; - undefine function value
          mov AX,U_WORD         ; - ...
          _loop                 ; - ...
            mov [BX],AX         ; - ...
            inc  BX             ; - ...
            inc  BX             ; - ...
            dec  CX             ; - ...
          _until e              ; - ...
        _admit                  ; admit
          sub AX,AX             ; - save 0 for alternate returns
          mov [BX],AX           ; - ...
          mov 2[BX],AX          ; - ...
        _endguess               ; endguess
;
; Link a trace-back structure.
;
        mov    BP,TB_STRUCT[SI] ; put addr of trace-back struc in BP
        push   DS               ; set ES=DS
        pop    ES               ; ...
        call   SetTBack         ; set up trace-back structure
        mov    SS:_ExCurr+2,DS  ; set up proper data segment for traceback
        mov    AX,EN_NAME[SI]   ; get the subroutine name
        mov    ES:NAME_TB[BP],AX; save the name
        pop    BP               ; get return type
        push   BP               ; ...
        mov    DI,FCODE_RETWF77*2
        push   DI               ; save return addr on the stack
        mov    DI,EPI_FCODE[SI] ; get addr of epilogue
        mov    word ptr RET_ADDR[DI],SP ; save the return address offset
        mov    word ptr RET_ADDR+2[DI],SS; save the return address segment
;
; When we return from WATFOR-77, we want to know how to get the type of
; the return value. We do this by making sure there are 16 bytes of
; return value on the stack.
;
        mov    AL,byte ptr CS:IFRetSize[BP]
                                ; get size of return value
        sub    AH,AH            ; ...
        neg    AX               ; compute 16 - size of return value
        add    AX,16            ; ...
        sub    SP,AX            ; allocate return value padding
;
; Execute WATFOR-77 subprogram.
;
        lea    SI,SP_START[SI]
        next
endproc Ext_to_F77


; Execution resumes here after WATFOR-77 subprogram executed.
;
; On entry: ES:DI points to return value

fcode   RETWF77
        add     SP,16+2         ; remove return address and return value
        pop     BP              ; get return type

; On Entry: BP contains the type of the return value.

        mov     SI,DI           ; set DS:SI to point to return value
        push    ES              ; ...
        pop     DS              ; ...
if _MATH eq _8087
        mov    BX,BP            ; get return type
        shl    BX,1             ; make index for word table
        jmp    word ptr CS:C87Val[BX]   ; get return value from 80x87 stack

C87Val: dw     C87Cont          ; PT_NOTYPE
        dw     C87Cont          ; PT_LOG1
        dw     C87Cont          ; PT_LOG4
        dw     C87Cont          ; PT_INT1
        dw     C87Cont          ; PT_INT2
        dw     C87Cont          ; PT_INT4
        dw     C87R4            ; PT_REAL
        dw     C87R8            ; PT_DBLE
        dw     C87R16           ; PT_XTND
        dw     C87C8            ; PT_CPLX
        dw     C87C16           ; PT_DBCX
        dw     C87C32           ; PT_XTCX
        dw     C87Cont          ; PT_CHAR

C87R4:  fstp  dword ptr [DI]
        jmp   C87Cont

C87R8:  fstp  qword ptr [DI]
        jmp   C87Cont

C87R16: fstp  _TBYTE ptr [DI]
        jmp   C87Cont

C87C8:  fstp  dword ptr [DI]
        fstp  dword ptr 4[DI]
        jmp   C87Cont

C87C16: fstp  qword ptr [DI]
        fstp  qword ptr 8[DI]
        jmp   C87Cont

C87C32: fstp  _TBYTE ptr [DI]
        fstp  _TBYTE ptr 16[DI]

C87Cont:

endif
        retf                    ; return to interface code
efcode  RETWF77

WF_addr dw     0,0


; This routine allows external subprograms to call WATFOR-77 intrinsic
; functions.
;
; On entry: DS:SI points to intrinsic function info. word

defn    IFArgList
        push   AX               ; save registers
        push   DX               ; ...
        push   DI               ; ...
        mov    AX,[SI]          ; get the info word
        and    AH,INFO_MASK     ; ...
        mov    DL,AH            ; get type of arguments
        shr    DL,1             ; ...
        shr    DL,1             ; ...
        cmp    DL,PT_CHAR       ; check if character arguments
        _if     e               ; if character arguments
          or   DL,VAR_LEN_CHR   ; - indicate character*(*)
        _endif                  ; endif
        sub    DH,DH            ; ...
        and    AH,COUNT_MASK    ; get parm count
        and    AL,TYPE_MASK     ; get type of intrinsic function
        mov    SI,offset PAddrs ; get address for parm storage
        mov    DI,offset PList  ; get address of parm list
        mov    CS:[DI],AX       ; save the info word
        inc    DI               ; point to storage for rest of parm list
        inc    DI               ; ...
        _loop                   ; loop - build a parm list
          or   AH,AH            ; - are all parms done?
          _quif e               ; - quit if so
          mov  CS:[DI],DX       ; - save the info word
          mov  CS:2[DI],SI      ; - save the offset
          add  SI,6             ; - point to next parm storage
          add  DI,4             ; - point to next parmlist storage
          dec  AH               ; - decrement number of parms
        _endloop                ; endloop
        lea    SI,PList         ; point to i.f. argument list
        pop    DI               ; restore registers
        pop    DX               ; ...
        pop    AX               ; ...
        ret
endproc IFArgList


;
; tables to load args into registers
;
LoadArg1 label word             ; load 1st arg into registers
        dw      LoadNull_1      ; PT_NO_TYPE
        dw      LoadDX_AX       ; PT_LOG1
        dw      LoadDX_AX       ; PT_LOG2
        dw      LoadDX_AX       ; PT_INT1
        dw      LoadDX_AX       ; PT_INT2
        dw      LoadDX_AX       ; PT_INT4
if _MATH eq _8087
        dw      LoadREAL_1      ; PT_REAL
        dw      LoadDBLE_1      ; PT_DBLE
        dw      LoadXTND_1      ; PT_XTND
        dw      LoadCPLX_1      ; PT_CPLX
        dw      LoadDBCX_1      ; PT_DBCX
        dw      LoadXTCX_1      ; PT_XTCX
else
        dw      LoadDX_AX       ; PT_REAL
        dw      LoadDBLE        ; PT_DBLE
        dw      LoadNull_1      ; PT_XTND
        dw      LoadCPLX        ; PT_CPLX
        dw      LoadDBLE        ; PT_DBCX
        dw      LoadNull_1      ; PT_XTCX
endif
        dw      LoadCHAR_1      ; PT_CHAR

LoadArg2 label word             ; load 2nd arg into registers
        dw      LoadNull_2      ; PT_NO_TYPE
        dw      LoadCX_BX       ; PT_LOG1
        dw      LoadCX_BX       ; PT_LOG2
        dw      LoadCX_BX       ; PT_INT1
        dw      LoadCX_BX       ; PT_INT2
        dw      LoadCX_BX       ; PT_INT4
if _MATH eq _8087
        dw      LoadREAL_2      ; PT_REAL
        dw      LoadDBLE_2      ; PT_DBLE
        dw      LoadXTND_2      ; PT_XTND
        dw      LoadCPLX_2      ; PT_CPLX
        dw      LoadDBCX_2      ; PT_DBCX
        dw      LoadXTCX_2      ; PT_XTCX
else
        dw      LoadCX_BX       ; PT_REAL
        dw      LoadNull_2      ; PT_DBLE
        dw      LoadNull_2      ; PT_XTND
        dw      LoadNull_2      ; PT_CPLX
        dw      LoadNull_2      ; PT_DBCX
        dw      LoadNull_2      ; PT_XTCX
endif
        dw      LoadCHAR_2      ; PT_CHAR

LoadDX_AX:
        pop     ax
        pop     dx
LoadNull_1:
        jmp     si

LoadCX_BX:
        pop     bx
        pop     cx
LoadNull_2:
        jmp     loadarg2_ret

LoadCHAR_1:
        mov     ax,sp
        mov     dx,ss
        jmp     si

LoadCHAR_2:
        mov     bx,sp
        mov     cx,ss
        add     bx,6
        jmp     loadarg2_ret

if _MATH eq _8087

LoadREAL_1:
        fld     dword ptr [bp]
        jmp     si

LoadDBLE_1:
        fld     qword ptr [bp]
        jmp     si

LoadXTND_1:
        fld     _TBYTE ptr [bp]
        jmp     si

LoadCPLX_1:
        fld     dword ptr 4[bp]
        fld     dword ptr [bp]
        jmp     si

LoadDBCX_1:
        fld     qword ptr 8[bp]
        fld     qword ptr [bp]
        jmp     si

LoadXTCX_1:
        fld     _TBYTE ptr 16[bp]
        fld     _TBYTE ptr [bp]
        jmp     si

LoadREAL_2:
        fld     dword ptr 4[bp]
fxch_ret:
        fxch
        jmp     loadarg2_ret

LoadDBLE_2:
        fld     qword ptr 8[bp]
        jmp     fxch_ret

LoadXTND_2:
        fld     _TBYTE ptr 16[bp]
        jmp     fxch_ret

LoadCPLX_2:
        fld     dword ptr 12[bp]
        fxch    st(2)
        fld     dword ptr 8[bp]
fxch2_ret:
        fxch    st(2)
        jmp     loadarg2_ret

LoadDBCX_2:
        fld     qword ptr 24[bp]
        fxch    st(2)
        fld     qword ptr 16[bp]
        jmp     fxch2_ret

LoadXTCX_2:
        fld     _TBYTE ptr 48[bp]
        fxch    st(2)
        fld     _TBYTE ptr 32[bp]
        jmp     fxch2_ret

else

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

;
; return processing for i.f.s
;
RetIF   label   dword
        dd      RetNull         ; PT_NOTYPE
        dd      RetDX_AX        ; PT_LOG1
        dd      RetDX_AX        ; PT_LOG4
        dd      RetDX_AX        ; PT_INT1
        dd      RetDX_AX        ; PT_INT2
        dd      RetDX_AX        ; PT_INT4
if _MATH eq _8087
        dd      RetREAL         ; PT_REAL
        dd      RetDBLE         ; PT_DBLE
else
        dd      RetDX_AX        ; PT_REAL
        dd      RetDBLE         ; PT_DBLE
endif
        dd      RetXTND         ; PT_XTND
        dd      RetCPLX         ; PT_CPLX
        dd      RetDBCX         ; PT_DBCX
        dd      RetXTCX         ; PT_XTCX
        dd      RetNull         ; PT_CHAR


RetDX_AX:
        mov     cs:2[di],dx
        mov     cs:[di],ax
RetNull:
        ret


if _MATH eq _8087

RetREAL:
        fstp    dword ptr cs:[di]
        ret

RetDBLE:
        fstp    qword ptr cs:[di]
        ret
else

RetDBLE:
        mov     cs:[di],dx
        mov     cs:2[di],cx
        mov     cs:4[di],bx
        mov     cs:6[di],ax
        ret

endif


RetDBCX:
        mov     cx,8
load_from_stack:
        push    cs
        pop     es
        _loop
          pop   ax
          stosw
          dec   cx
        _until  e
        ret

RetXTND:
        mov     cx,8
        hop     load_from_stack

RetCPLX:
        mov     cx,4
        hop     load_from_stack

RetXTCX:
        mov     cx,16
        hop     load_from_stack


IFSize label byte
        db 0            ; extra storage require on stack for each type
        db 0
        db 0
        db 0
        db 0
        db 0
        db 0
        db 0
        db 16
        db 8
        db 16
        db 32
        db 0

; On entry: DS:SI points to an intrinsic function table entry.

defp    IFCall                  ; call intrinsic function
        mov    DI,DARG_LIST[SI] ;get pointer to I.F. info. word
        mov    BP,2[DI]         ; get the address of intrinsic function
        lea    SI,PList         ; point to parameter list
        mov    DH,1[SI]         ; get number of parms
        mov    BL,0[SI]         ; get type of intrinsic function
        sub    BH,BH            ; ...
        mov     al,byte ptr IFSize[bx]; find out how much extra storage req'd
        sub    AH,AH            ; get size of result
        sub    SP,AX            ; allocate room for result
        push   BX               ; save type of result
        push   BP               ; save ptr to fn
        mov     bp,sp           ; and get stack frame
        sub    SP,32            ; allocate room for max. arguments
        mov    DI,SP            ; point to room for arguments
        push   SS               ; ...
        pop    ES               ; ...
        mov    BL,2[SI]         ; get type of arguments
        and    BL,TYPE_MASK     ; ...
        cmp    BL,PT_CHAR       ; check if character arguments
        _if     e               ; if character arguments
          mov  AH,6             ; - set size of SCB
        _else                   ; else
          mov  AH,byte ptr CS:XRetSize[BX]
                                ; - get size of non-character argument
        _endif                  ; endif
        lea    SI,PAddrs        ; point to list to containing addresses
        push   CS               ; ... of arguments
        pop    DS               ; ...
        mov     dl,dh           ; save number of parms
        _loop                   ; loop : copy parms onto stack
          or   DH,DH            ; - are all the parms done
          _quif e               ; - quit if so
          mov  CL,AH            ; - get size of argument
          sub  CH,CH            ; - ...
          push SI               ; - save pointer into list containing
          push DS               ; - ... addresses of arguments
          cmp  BL,PT_CHAR       ; - check if it's character
          _if   ne              ; - if it's not character
            lds SI,[SI]         ; - - get pointer to argument
          _endif                ; - endif
          rep  movsb            ; - copy the argument
          pop  DS               ; - restore pointer to parameter list
          pop  SI               ; - ...
          add  SI,6             ; - point to next entry in parameter list
          dec  DH               ; - decrement argument count
        _endloop                ; endloop
        exit_fcode_fpu          ; setup for calling i.f.
        mov     di,bx           ; index into jump table
        shl     di,1            ; ...
        shl     di,1            ; ...
        cmp     dl,2            ; are there 2 args?
        _if     ae
          mov   si,cs:LoadArg2[di]; - return addr
        _else
          mov   si,offset loadarg2_ret
        _endif
        jmp     cs:LoadArg1[di] ; load the args
loadarg2_ret:
        lea     si,4[bp]        ; point to return value storage
        push    cs              ; call the intrinsic function
        call    [BP]            ; ...
        lea     sp,2[bp]        ; remove args and fn addr from stack
        push    SS              ; ...
        pop     DS              ; ...
        pop     BP              ; get type of result
        lea     di,PAddrs       ; point to destination of ret value
        mov     si,di           ; point at return value
        shl     bp,1
        shl     bp,1
        jmp     cs:RetIF[bp]    ; do return processing

PList   dw     0                ; info word
        dd     2 dup(0)         ; 2 argument list entries for I.F.'s
PAddrs  dw     3 dup(0)         ; 2 pointers or SCB's for I.F. arguments
        dw     3 dup(0)         ; ...
        dw     2 dup(0)         ; (also used for I.F. return value)
endproc IFCall


defn    SetTBack
        mov   AX,SS:_ExCurr     ; save pointer to previous traceback
        mov   word ptr ES:LINE_TB[BP],TB_LG
        mov   word ptr ES:LINK_TB[BP],AX        ; ...
        mov   AX,SS:_ExCurr+2   ; ...
        mov   word ptr ES:LINK_TB+2[BP],AX
        mov   AX,SS:_ExLinePtr  ; save current line number
        mov   ES:LINEPTR_TB[BP],AX      ; give a 0 line number
        mov   SS:_ExCurr,BP     ; save pointer to current traceback
        ret                     ; return
endproc SetTBack


XRetSize:
        db      0       ; PT_NOTYPE
        db      4       ; PT_LOG_1
        db      4       ; PT_LOG_4
        db      4       ; PT_INT_1
        db      4       ; PT_INT_2
        db      4       ; PT_INT_4
        db      4       ; PT_REAL_4
        db      8       ; PT_REAL_8
        db      16      ; PT_REAL_16
        db      8       ; PT_CPLX_8
        db      16      ; PT_CPLX_16
        db      32      ; PT_CPLX_32
        db      0       ; PT_CHAR


if _MATH eq _8087

IFRetSize:
        db      0       ; PT_NOTYPE
        db      4       ; PT_LOG_1
        db      4       ; PT_LOG_4
        db      4       ; PT_INT_1
        db      4       ; PT_INT_2
        db      4       ; PT_INT_4
        db      0       ; PT_REAL_4
        db      0       ; PT_REAL_8
        db      0       ; PT_REAL_16
        db      0       ; PT_CPLX_8
        db      0       ; PT_CPLX_16
        db      0       ; PT_CPLX_32
        db      0       ; PT_CHAR

else

IFRetSize:
        db      0       ; PT_NOTYPE
        db      4       ; PT_LOG_1
        db      4       ; PT_LOG_4
        db      4       ; PT_INT_1
        db      4       ; PT_INT_2
        db      4       ; PT_INT_4
        db      4       ; PT_REAL_4
        db      8       ; PT_REAL_8
        db      16      ; PT_REAL_16
        db      8       ; PT_CPLX_8
        db      16      ; PT_CPLX_16
        db      32      ; PT_CPLX_32
        db      0       ; PT_CHAR

endif

        fmodend
        end
