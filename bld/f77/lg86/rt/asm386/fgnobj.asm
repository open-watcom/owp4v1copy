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
; FGNOBJ        : Interface to foreign object files
;

.8087

include fcdef.inc
include parmtype.inc
include datamap.inc
include errcod.inc
include undef.inc
include fcodes.inc
include structio.inc

        xref    LinkXTrace_
        xref    UnLinkTrace_

        extrn   RTError : near
        extrn   WF77_To_Ext : near
        extrn   Ext_To_WF77 : near

        lgxref  CalledDataBase
        lgxref  CallerDataBase

        dataseg

        xred    "C",ExLinePtr,  dword

WF_Addr dd      0

PList   dw      0               ; info word
        dw      3 dup(0)        ; 2 argument list entries for I.F.'s
        dw      3 dup(0)        ; ...
PAddrs  dd      4 dup(0)        ; 2 pointers or SCB's for I.F. arguments
                                ; also used for fn return value

        enddata

        fmodstart       fgnobj

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
        db      0       ; PT_STRUCT


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
        db      0       ; PT_STRUCT

C87Val label dword
        dd     C87Cont          ; PT_NOTYPE
        dd     C87Cont          ; PT_LOG1
        dd     C87Cont          ; PT_LOG4
        dd     C87Cont          ; PT_INT1
        dd     C87Cont          ; PT_INT2
        dd     C87Cont          ; PT_INT4
        dd     C87R4            ; PT_REAL
        dd     C87R8            ; PT_DBLE
        dd     C87R16           ; PT_XTND
        dd     C87C8            ; PT_CPLX
        dd     C87C16           ; PT_DBCX
        dd     C87C32           ; PT_XTCX
        dd     C87Cont          ; PT_CHAR
        dd     C87Cont          ; PT_STRUCT

X87Val  label dword
        dd      X87R4           ; PT_REAL
        dd      X87R8           ; PT_DBLE
        dd      X87R16          ; PT_XTND
        dd      X87C8           ; PT_CPLX
        dd      X87C16          ; PT_DBCX
        dd      X87C32          ; PT_XTCX
        dd      X87Null         ; PT_CHAR
        dd      X87Null         ; PT_STRUCT

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
        db      0       ; PT_STRUCT

endif


; On entry: edi points to first byte of called subroutine
;           esi points to first word of caller parameter list

        xdefp   F77_to_Ext
defn    F77_to_Ext              ; interface to non_watfor77 subroutines
        mov     ebx,[esi]       ; get subroutine type
        and     bl,TYPE_MASK    ; ...
        _guess
          cmp   bl,PT_CHAR      ; - is it a character function
          _quif ne              ; - if it is then go to admit
          pop   ebx             ; - get offset to SCB into ebx
          push  4[ebx]          ; - copy over the SCB
          push  0[ebx]          ; - ...
          mov   ecx,esp         ; - ptr to return storage in ecx
        _admit
          cmp   bl,PT_STRUCT    ; - is it a structure function
          _quif ne              ; - do admit if so
          pop   ecx             ; - get ptr to RCB in ecx
        _admit
          movzx ebx,bl          ; - zero the high part
          mov   cl,byte ptr XRetSize[ebx]
                                ; - get the size of the return value
          shr   cl,2            ; - get number of dwords
          mov   eax,U_DWORD     ; - set AX to undefined value
          _loop                 ; - loop : undefine the return value
            or  cl,cl           ; - - are we done?
            _quif e             ; - - quit if so
            push eax            ; - - save the value
            dec  cl             ; - - decrement number of words
          _endloop              ; - endloop
          mov   ecx,esp         ; - point ecx at return storage
        _endguess
        push    esi             ; save the current fcode pointer
        push    ebp             ; save current data pointer
;
; Link a traceback structure.
;
        sub     esp,size traceback_s; allocate trace-back struct on stack
        mov     ebp,esp         ; get struct addr in ebp
        push    00000300h       ; set up a NULL 3 NULL special name
        call    SetTBack        ; set up trace-back structure
        mov     name_tb[ebp],esp; save pointer to special name
        sub     eax,eax         ; set ExLinePtr to NULL
        mov     ebp,ecx         ; put return storage pointer back in ebp
        mov     ExLinePtr,eax   ; set ExLinePtr to NULL
;
; Call interface code:
;     edi contains 32-bit address of external subprogram
;     esi points to WATFOR-77 argument list
;     ebp points to storage for return value
;
        call     WF77_To_Ext    ; call the interface code
;
; Unlink traceback structure.
;
        add     esp,4           ; remove special name
        mov     eax,esp         ; unlink traceback
        docall  UnLinkTrace_    ; ...
        add     esp,size traceback_s; remove the struct from the stack
;
; Clean-up and return to WATFOR-77 subprogram.
;
        pop     ebp             ; restore data pointer
        pop     esi             ; restore fcode pointer
        getword ax              ; get the parm count/ret type in ax
        mov     cl,al           ; we must check if it is a sys_call function
        and     cl, SYSTEM_CALL ; ...
        _if     ne
          getword cx            ; trash the sys_call info word
        _endif
        and     al,TYPE_MASK    ; get the ret type
        cmp     al,PT_CHAR      ; is it a character function?
        _if     e               ; if it is...
          add   esp,8           ; - remove SCB from the stack
        _endif                  ; endif
if _MATH eq _8087
        cmp     al,PT_REAL      ; check if it's floating point
        _if     ge              ; if it's floating point
          movzx ebx,al          ; get type of return value
          movzx ecx,byte ptr XRetSize[ebx]
                                ; get size of return value
          jmp   dword ptr (X87Val-4*PT_REAL)[ebx*4]; put return value on 80x87 stack

X87C8:
        fld   dword ptr 4[esp]
X87R4:
        fld   dword ptr [esp]
        hop     X87Null

X87C16:
        fld   qword ptr 8[esp]
X87R8:
        fld   qword ptr [esp]
        hop     X87Null

X87C32:
        fld   _TBYTE ptr 16[esp]
X87R16:
        fld   _TBYTE ptr [esp]

X87Null:
          _fwait                ; wait for the fpu
          add   esp,ecx         ; remove return value from cpu stack
        _endif
endif
        mov   cl,ah             ; get number of parms
        _loop                   ; loop - step across the parm info
           or cl,cl             ; - have we done all the parms
           _quif e              ; - quit if we have
           getword ax           ; - get the parm info word
           and ah,TYPE_MASK     ; - get the parm type
           cmp ah,PC_STATEMENT  ; - is it an alternate return?
           _if  ne              ; - if it isn't...
             add esi,4          ; - - step past the offset
           _endif               ; - endif
           dec cl               ; - decrement parm count
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
; NOTE:
;       We must save all registers we use before invoking the interface
;       code since we don't know what registers the calling convention
;       uses.

        xdefp   Ext_to_F77
defn    Ext_to_F77
        push    esi             ; save esi
        mov     esi,4[esp]      ; point to WATFOR-77 subprogram
        sub     esi,sp_hdr_id   ; ...
        mov     WF_Addr,esi     ; save address of WATFOR-77 subprogram
        cmp     dword ptr darg_list[esi],0
        _if     e               ; if block data subprogram
          lea   esi,darg_list[esi]; - point to NULL (no args/PT_NOTYPE)
        _else                   ; else
          mov   esi,darg_list[esi]; - point to dummy argument list
          test  byte ptr [esi],IF_IND
          _if   ne              ; - if intrinsic function
            call IFArgList      ; - - build argument list
            mov dword ptr CalledDataBase,0
          _else
            push edi            ; - save edi
            mov edi,WF_Addr     ; - get a ptr to epilog sequence
            lea esi,epi_fcode+4[edi]; - ...
            add esi,-4[esi]     ; - ...
            mov esi,my_sp_data[esi]; - get data base
            mov dword ptr CalledDataBase,esi; - and save it
            mov esi,darg_list[edi]; - restore registers
            pop edi             ; - ...
          _endif                ; - endif
        _endif                  ; endif
        mov     4[esp],esi      ; pass pointer to dummy argument list
        pop     esi             ; restore SI
        call    Ext_To_WF77     ; call routine to convert argument list

; The following will be the called to invoke the WATFOR-77 subprogram.
; If a character function is called, then edi points to the
; destination and ecx is the length of the string. ecx is 0 if external
; subprogram only passes pointer to string and not length.
; if it's a structure function, edi points to the storage for the ret val

        mov     esi,WF_Addr     ; get addr of WATFOR-77 subprogram
        mov     ebx,darg_list[esi]; get arg list pointer
        or      ebx,ebx         ; check if block data subprogram
        _if     e               ; if block data subprogram
          RTErr SR_BLKDAT_CALLED; - report the error
        _endif                  ; endif
        mov     dx,[ebx]        ; get info word
        and     dl,IF_IND       ; see if intrinsic function
        _if     ne              ; if it is ...
          dojmp IFCall          ; - call the intrinsic function
        _endif                  ; endif
        mov     ebx,epi_fcode[esi]; get pointer to epilogue fcode
        lea     ebx,epi_fcode+4[esi+ebx]; ...
        cmp     dword ptr arg_list[ebx],0
        _if     ne              ; if subprogram already active
          RTErr SR_ALREADY_ACTIVE;- report the error
        _endif                  ; endif
        mov   eax,darg_list[esi]; indicate we are active
        mov   arg_list[ebx],eax ; ...
        mov   ebx,eax           ; point to dummy argument list
        movzx eax,byte ptr [ebx]; get ret type, arg count
        and   al,TYPE_MASK      ; get the return type
        mov   ebx,epi_fcode[esi]; get pointer to epilogue fcode
        push  eax               ; save return type
        lea     ebx,func_val+epi_fcode+4[esi+ebx]; point to return value
        _guess                  ; guess it's a function
          cmp al,PT_CHAR        ; - is it a character function?
          _quif ne              ; - quit if it's not a character function
          mov [ebx],edi         ; - set pointer to return value
          or  ecx,ecx           ; - check if length passed
          _if  ne               ; - if length passed
            mov 4[ebx],ecx      ; - - set length of string
          _endif                ; - endif
        _admit
          cmp   al,PT_STRUCT    ; - is it a structure function?
          _quif ne
          mov   ecx,4[ebx]      ; - get sdefn ptr from RCB
          mov   ebp,dword ptr CalledDataBase; load start
          add   ecx, ebp        ; - get proper address of sdefn
          mov   ecx,sd_total_size[ecx]; - and get the size of the struct
          _guess
            cmp ecx,1           ; - is it a 1 byte struct?
            _quif e
            cmp ecx,2           ; - or a 2 byte struct?
            _quif e
            cmp ecx,4           ; - or a 4 byte struct?
            _quif e
          _admit
            lea edi,8[ebx]      ; - use extra space in ret val area
          _endguess
            mov [ebx],edi       ; - if it isn't any, then set ptr to ret storage
        _admit                  ; admit
          cmp   al,PT_NOTYPE    ; - is it a subroutine?
          _quif e               ; - quit if it's a subroutine
          mov   eax,U_DWORD     ; - undefine function value
          mov   [ebx],eax       ; - ...
          mov   4[ebx],eax      ; - ...
          mov   8[ebx],eax      ; - ...
          mov   12[ebx],eax     ; - ...
        _admit                  ; admit
          sub   eax,eax         ; - save 0 for alternate returns
          mov   [ebx],eax       ; - ...
        _endguess               ; endguess
;
; Link a trace-back structure.
;
        mov     ebp,tb_struct[esi]; put addr of trace-back struc in BP
        lea     ebp,tb_struct+4[esi+ebp]
        call    SetTBack        ; set up trace-back structure
        mov     eax,en_name[esi]; get the subroutine name
        mov     name_tb[ebp],eax; save the name
        pop     ebp             ; get return type
        push    ebp             ; ...
        push    FCODE_RETWF77*4 ; save return addr on the stack
        mov     edi,epi_fcode[esi]; get addr of epilogue
        lea     edi,epi_fcode+4[esi+edi]
        mov     ret_addr[edi],esp; save the return address offset
;
; When we return from WATFOR-77, we want to know how to get the type of
; the return value. We do this by making sure there are 16 bytes of
; return value on the stack.
;
        movzx   eax,byte ptr IFRetSize[ebp]; get size of return value
        neg     eax             ; compute 16 - size of return value
        add     eax,16          ; ...
        sub     esp,eax         ; allocate return value padding
;
; Execute WATFOR-77 subprogram.
;
        mov     ebp,my_sp_data[edi]
        lea     esi,sp_start[esi]
        next
endproc Ext_to_F77


; Execution resumes here after WATFOR-77 subprogram executed.
;
; On entry: edi points to return value

fcode   RETWF77
        add     esp,16+4        ; remove return address and return value
        pop     ebp             ; get return type

; On Entry: ebp contains the type of the return value.

        mov     esi,edi         ; set esi to point to return value
if _MATH eq _8087
        jmp     C87Val[ebp*4]   ; get return value from 80x87 stack

C87R4:
        fstp  dword ptr [edi]
        hop   C87Cont

C87R8:
        fstp  qword ptr [edi]
        hop   C87Cont

C87R16:
        fstp  _TBYTE ptr [edi]
        hop   C87Cont

C87C8:
        fstp  dword ptr 0[edi]
        fstp  dword ptr 4[edi]
        hop   C87Cont

C87C16:
        fstp  qword ptr 0[edi]
        fstp  qword ptr 8[edi]
        hop   C87Cont

C87C32:
        fstp  _TBYTE ptr 0[edi]
        fstp  _TBYTE ptr 16[edi]

C87Cont:
endif

        ret                     ; return to interface code
efcode  RETWF77

; This routine allows external subprograms to call WATFOR-77 intrinsic
; functions.
;
; On entry: esi points to intrinsic function info. word

defn    IFArgList
        push    eax             ; save registers
        push    edx             ; ...
        push    edi             ; ...
        mov     eax,[esi]       ; get the info word
        and     ah,INFO_MASK    ; ...
        mov     dl,ah           ; get type of arguments
        shr     dl,2            ; ...
        cmp     dl,PT_CHAR      ; check if character arguments
        _if     e               ; if character arguments
          or    dl,VAR_LEN_CHR  ; - indicate character*(*)
        _endif                  ; endif
        sub     dh,dh           ; ...
        and     ah,COUNT_MASK   ; get parm count
        and     al,TYPE_MASK    ; get type of intrinsic function
        mov     esi,offset32 PAddrs; get address for parm storage
        mov     edi,offset32 PList; get address of parm list
        mov     [edi],ax        ; save the info word
        add     edi,2           ; point to storage for rest of parm list
        _loop                   ; loop - build a parm list
          or    ah,ah           ; - are all parms done?
          _quif e               ; - quit if so
          mov   [edi],dx        ; - save the info word
          mov   2[edi],esi      ; - save the offset
          add   esi,8           ; - point to next parm storage
          add   edi,6           ; - point to next parmlist storage
          dec   ah              ; - decrement number of parms
        _endloop                ; endloop
        lea    esi,PList        ; point to i.f. argument list
        pop    edi              ; restore registers
        pop    edx              ; ...
        pop    eax              ; ...
        ret
endproc IFArgList


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
        dd      LoadREAL_1      ; PT_REAL
        dd      LoadDBLE_1      ; PT_DBLE
        dd      LoadXTND_1      ; PT_XTND
        dd      LoadCPLX_1      ; PT_CPLX
        dd      LoadDBCX_1      ; PT_DBCX
        dd      LoadXTCX_1      ; PT_XTCX
else
        dd      LoadEAX         ; PT_REAL
        dd      LoadEDX_EAX     ; PT_DBLE
        dd      LoadNull_1      ; PT_XTND
        dd      LoadNull_1      ; PT_CPLX
        dd      LoadDBCX        ; PT_DBCX
        dd      LoadNull_1      ; PT_XTCX
endif
        dd      LoadCHAR_1      ; PT_CHAR

LoadArg2 label dword            ; load 2nd arg into registers
        dd      LoadNull_2      ; PT_NO_TYPE
        dd      LoadEDX         ; PT_LOG1
        dd      LoadEDX         ; PT_LOG2
        dd      LoadEDX         ; PT_INT1
        dd      LoadEDX         ; PT_INT2
        dd      LoadEDX         ; PT_INT4
if _MATH eq _8087
        dd      LoadREAL_2      ; PT_REAL
        dd      LoadDBLE_2      ; PT_DBLE
        dd      LoadXTND_2      ; PT_XTND
        dd      LoadCPLX_2      ; PT_CPLX
        dd      LoadDBCX_2      ; PT_DBCX
        dd      LoadXTCX_2      ; PT_XTCX
else
        dd      LoadEDX         ; PT_REAL
        dd      LoadECX_EBX     ; PT_DBLE
        dd      LoadNull_2      ; PT_XTND
        dd      LoadECX_EBX     ; PT_CPLX
        dd      LoadNull_2      ; PT_DBCX
        dd      LoadNull_2      ; PT_XTCX
endif
        dd      LoadCHAR_2      ; PT_CHAR

LoadEAX:
        pop     eax
LoadNull_1:
        jmp     esi

LoadEDX:
        pop     edx
LoadNull_2:
        jmp     loadarg2_ret

LoadCHAR_1:
        mov     eax,esp
        jmp     esi

LoadCHAR_2:
        lea     edx,8[esp]
        jmp     loadarg2_ret

if _MATH eq _8087

LoadREAL_1:
        fld     dword ptr [esp]
        jmp     esi

LoadDBLE_1:
        fld     qword ptr [esp]
        jmp     esi

LoadXTND_1:
        fld     _TBYTE ptr [esp]
        jmp     esi

LoadCPLX_1:
        fld     dword ptr 4[esp]
        fld     dword ptr [esp]
        jmp     esi

LoadDBCX_1:
        fld     qword ptr 8[esp]
        fld     qword ptr [esp]
        jmp     esi

LoadXTCX_1:
        fld     _TBYTE ptr 16[esp]
        fld     _TBYTE ptr [esp]
        jmp     esi

LoadREAL_2:
        fld     dword ptr 4[esp]
fxch_ret:
        fxch
        jmp     loadarg2_ret

LoadDBLE_2:
        fld     qword ptr 8[esp]
        jmp     fxch_ret

LoadXTND_2:
        fld     _TBYTE ptr 16[esp]
        jmp     fxch_ret

LoadCPLX_2:
        fld     dword ptr 12[esp]
        fxch    st(2)
        fld     dword ptr 8[esp]
fxch2_ret:
        fxch    st(2)
        jmp     loadarg2_ret

LoadDBCX_2:
        fld     qword ptr 24[esp]
        fxch    st(2)
        fld     qword ptr 16[esp]
        jmp     fxch2_ret


LoadXTCX_2:
        fld     _TBYTE ptr 48[esp]
        fxch    st(2)
        fld     _TBYTE ptr 32[esp]
        jmp     fxch2_ret


else

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

;
; return processing for i.f.s
;
RetIF   label   dword
        dd      RetNull         ; PT_NOTYPE
        dd      RetEAX          ; PT_LOG1
        dd      RetEAX          ; PT_LOG4
        dd      RetEAX          ; PT_INT1
        dd      RetEAX          ; PT_INT2
        dd      RetEAX          ; PT_INT4
if _MATH eq _8087
        dd      RetREAL         ; PT_REAL
        dd      RetDBLE         ; PT_DBLE
        dd      RetDBLE         ; PT_DBLE
else
        dd      RetEAX          ; PT_REAL
        dd      RetEDX_EAX      ; PT_DBLE
        dd      RetXTND         ; PT_XTND
endif
        dd      RetCPLX         ; PT_CPLX
        dd      RetDBCX         ; PT_DBCX
        dd      RetXTCX         ; PT_XTCX
        dd      RetNull         ; PT_CHAR


if _MATH eq _EMULATION
RetXTND:
        pop     [edi]
        pop     4[edi]
        pop     word ptr 8[edi]
        ret

RetEDX_EAX:
        mov     4[edi],edx
endif
RetEAX:
        mov     [edi],eax
RetNull:
        ret


if _MATH eq _8087
RetREAL:
        fstp    dword ptr [edi]
        ret

RetDBLE:
        fstp    qword ptr [edi]
        ret

RetXTND:
        fstp    _TBYTE ptr [edi]
        ret
endif


RetDBCX:
        pop     [edi]
        pop     4[edi]
        pop     8[edi]
        pop     12[edi]
        ret

RetXTCX:
        pop     [edi]
        pop     4[edi]
        pop     8[edi]
        pop     12[edi]
        pop     16[edi]
        ret

RetCPLX:
        pop     [edi]
        pop     4[edi]
        ret


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
        mov     edi,darg_list[esi];get pointer to I.F. info. word
        mov     ebp,2[edi]      ; get the address of intrinesic function
        lea     esi,PList       ; point to parameter list
        mov     dh,1[esi]       ; get number of parms
        movzx   ebx,byte ptr [esi]; get type of intrinsic function
        movzx   eax,IFSize[ebx] ; find out how much extra storage req'd
        sub     esp,eax         ; allocate room for result
        push    ebx             ; save type of result
        push    ebp             ; save ptr to fn
        mov     ebp,esp         ; and get stack frame
        sub     esp,32          ; allocate room for max. arguments
        mov     edi,esp         ; point to room for arguments
        mov     bl,2[esi]       ; get type of arguments
        and     bl,TYPE_MASK    ; ...
        cmp     bl,PT_CHAR      ; check if character arguments
        _if     e               ; if character arguments
          mov   ah,8            ; - set size of SCB
        _else                   ; else
          mov   ah,byte ptr XRetSize[ebx]
                                ; - get size of non-character argument
        _endif                  ; endif
        lea     esi,PAddrs      ; point to list to containing addresses
        mov     dl,dh           ; save number of parms
        _loop                   ; loop : copy parms onto stack
          or    dh,dh           ; - are all the parms done
          _quif e               ; - quit if so
          movzx ecx,ah          ; - get size of argument
          push  esi             ; - save pointer into list containing
          cmp   bl,PT_CHAR      ; - check if it's character
          _if   ne              ; - if it's not character
            mov esi,[esi]       ; - - get pointer to argument
          _endif                ; - endif
          rep   movsb           ; - copy the argument
          pop   esi             ; - restore pointer to parameter list
          add   esi,8           ; - point to next entry in parameter list
          dec   dh              ; - decrement argument count
        _endloop                ; endloop
        exit_fcode_fpu          ; setup for calling i.f.
        lea     edi,[ebx*4]     ; get offset into jump table
        cmp     dl,2            ; - are there 2 args?
        _if     ae
          mov   esi,LoadArg2[edi]; - return addr
        _else
          mov   esi,offset32 loadarg2_ret
        _endif
        jmp     LoadArg1[edi]   ; - load args
loadarg2_ret:
        lea     esi,8[ebp]      ; set esi to point to return value
        call    [ebp]           ; call the instrinesic function
        lea     esp,4[ebp]      ; remove args from stack (and fn addr)
        pop     ebp             ; get type of result
        lea     edi,PAddrs      ; point to destination of return value
        mov     esi,edi         ; point at return value
        jmp     RetIF[ebp*4]    ; do return processing
endproc IFCall


defn    SetTBack
        mov     eax,ebp         ; link traceback
        docall  LinkXTrace_     ; ...
        ret                     ; return
endproc SetTBack

        fmodend
        end
