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
; FCIO          : I/O interface routines
;

.8087

include fcdef.inc
include adv.inc
include parmtype.inc
include errcod.inc
include prtarr.inc
include datamap.inc

        xref    DoRead_
        xref    DoWrite_
        xref    ChkLimitErr_
        xref    StructIOInit_
        xref    StructIOItem_

        rtxref  IOOpen
        rtxref  IOClose
        rtxref  IOBack
        rtxref  IOEndf
        rtxref  IORew
        rtxref  IOInq
        rtxref  IOData
        rtxref  SetIOFlags
        rtxref  SetNoFmt
        rtxref  SetFmt
        rtxref  FmtScan
        rtxref  FmtAScan
        rtxref  SetUnit
        rtxref  SetRec
        rtxref  SetErr
        rtxref  SetEnd
        rtxref  SetIOS
        rtxref  SetRecl
        rtxref  SetOpen
        rtxref  SetNumb
        rtxref  SetNRec
        rtxref  SetNmd
        rtxref  SetNml
        rtxref  SetExst
        rtxref  SetUnFmtd
        rtxref  SetSeq
        rtxref  SetName
        rtxref  SetFmtd
        rtxref  SetDir
        rtxref  SetStat
        rtxref  SetLen
        rtxref  SetForm
        rtxref  SetFile
        rtxref  SetBlnk
        rtxref  SetAcc
        rtxref  SetIntl
        rtxref  SetCCtrl
        rtxref  SetAction
        rtxref  SetRecType
        rtxref  SetBlkSize
        rtxref  InqBlkSize
        rtxref  SetShare

        extrn   StmtLook        : near

        extrn   _IORslt         : dword
        extrn   _FmtBuff        : dword
        extrn   _IOTypeRtn      : dword

        dataseg

        SaveESI         dd      0
        SaveEBP         dd      0
        EndEq           dd      0
        ErrEq           dd      0
        IOSLabel        dd      0

        enddata

        fmodstart fcio


defn    ChkErrEq
        mov     ESI,ErrEq       ; get ERR= label
        or      ESI,ESI         ; check if ERR=
        _if     e               ; if no ERR=
          mov   ESI,IOSLabel    ; - try IOSTAT label
        _endif                  ; endif
        ret
endproc ChkErrEq


fcode   RT_EX_OPEN
        mov     eax,offset32 IOOpen
        hop     IOStmt
efcode  RT_EX_OPEN


fcode   RT_EX_CLOSE
        mov     eax,offset32 IOClose
        hop     IOStmt
efcode  RT_EX_CLOSE


fcode   RT_EX_READ
        mov     eax,offset32 DoRead_
        hop     IOStmt
efcode  RT_EX_READ


fcode   RT_EX_WRITE
        mov     eax,offset32 DoWrite_
        hop     IOStmt
efcode  RT_EX_WRITE


fcode   RT_EX_BACK
        mov     eax,offset32 IOBack
        hop     IOStmt
efcode  RT_EX_BACK


fcode   RT_EX_ENDF
        mov     eax,offset32 IOEndf
        hop     IOStmt
efcode  RT_EX_ENDF


fcode   RT_EX_REW
        mov     eax,offset32 IORew
        hop     IOStmt
efcode  RT_EX_REW


fcode   RT_EX_INQ
        mov     eax,offset32 IOInq
;;;;;;;;hop     IOStmt
efcode  RT_EX_INQ


defn    IOStmt
        exit_fcode_fpu          ; switch to run-time environment
        mov     SaveESI,esi     ; save F-Code program counter
        mov     SaveEBP,ebp     ; save base ptr
        call    eax             ; ...
        xor     esi,esi         ; assume no END= or ERR=
        or      AX,AX           ; check for END= or ERR=
        _if     l               ; if return code < 0
          mov   esi,EndEq       ; - get END= label
          or    esi,esi         ; - check if END=
          _if   e               ; - if no END=
            call ChkErrEq       ; - - check ERR= and IOSTAT=
          _endif                ; - endif
        _else                   ; else
          _if   g               ; - if ERR=
            call ChkErrEq       ; - - check ERR= and IOSTAT=
          _endif                ; - endif
        _endif                  ; endif
        or      esi,esi         ; check if END= or ERR= label
        _if     e               ; if no END= or ERR= label
          mov   esi,SaveESI     ; - restore F-Code program counter
        _endif                  ; endif
        mov     ebp,SaveEBP     ; restore base ptr
        mov     EndEq,0         ; reset END=
        mov     ErrEq,0         ; reset ERR=
        mov     IOSLabel,0      ; reset IOSTAT= label
        enter_fcode_fpu         ; switch to F-Code environment
        next                    ; do next fcode
endproc IOStmt


        xdefp   ExecData_
defp    ExecData_
        push    esi             ; save register
        push    ebp             ; ...
        mov     esi,eax         ; setup F-Code environment
        mov     ebp,edx         ; ...
        exit_fcode              ; switch to run-time environment
        mov     SaveESI,esi     ; save F-Code program counter
        mov     SaveEBP,ebp
        mov     _IOTypeRtn,offset32 FC_IOType_; indicate f-code iotype rtn
        docall  IOData          ; execute DATA statement
        mov     esi,SaveESI     ; restore F-Code program counter
        mov     ebp,SaveEBP
        enter_fcode             ; switch to F-Code environment
        pop     ebp             ; ...
        pop     esi             ; restore register
        ret
endproc ExecData_


        xdefp    FC_IOType_
defp    FC_IOType_
        push    ebp             ; ...
        mov     esi,SaveESI     ; restore F-Code program counter
        mov     ebp,SaveEBP     ; restore Data pointer
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
endproc FC_IOType_


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Input Routines: NOTE: edi is assumed to contain the address of
;<>                                the input item.
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   RT_INP_LOG1             ; input logical*1
        mov     ebx,PT_LOG1     ; indicate logical*1
        hop     Inp             ; get address of item
efcode  RT_INP_LOG1


fcode   RT_INP_LOG4             ; input logical*4
        mov     ebx,PT_LOG4     ; indicate logical*4
        hop     Inp             ; get address of item
efcode  RT_INP_LOG4


fcode   RT_INP_INT1             ; input integer*1
        mov     ebx,PT_INT1     ; indicate integer*1
        hop     Inp             ; get address of item
efcode  RT_INP_INT1


fcode   RT_INP_INT2             ; input integer*2
        mov     ebx,PT_INT2     ; indicate integer*2
        hop     Inp             ; get address of item
efcode  RT_INP_INT2


fcode   RT_INP_INT4             ; input integer*4
        mov     ebx,PT_INT4     ; indicate integer*4
        hop     Inp             ; get address of item
efcode  RT_INP_INT4


fcode   RT_INP_REAL             ; input real*4
        mov     ebx,PT_REAL     ; indicate real*4
        hop     Inp             ; get address of item
efcode  RT_INP_REAL


fcode   RT_INP_DBLE             ; input real*8
        mov     ebx,PT_DBLE     ; indicate real*8
        hop     Inp             ; get address of item
efcode  RT_INP_DBLE


fcode   RT_INP_XTND             ; input real*16
        mov     ebx,PT_XTND     ; indicate real*16
        hop     Inp             ; get address of item
efcode  RT_INP_XTND


fcode   RT_INP_CPLX             ; input complex*8
        mov     ebx,PT_CPLX     ; indicate complex*8
        hop     Inp             ; get address of item
efcode  RT_INP_CPLX


fcode   RT_INP_DBCX             ; input complex*16
        mov     ebx,PT_DBCX     ; indicate complex*16
        hop     Inp             ; get address of item
efcode  RT_INP_DBCX


fcode   RT_INP_XTCX             ; input complex*32
        mov     ebx,PT_XTCX     ; indicate complex*32
        hop     Inp             ; get address of item
efcode  RT_INP_XTCX


fcode   RT_INP_CHAR                     ; input character string
        exit_fcode                      ; switch to run-time environment
        docall  ChkLimitErr_            ; check for limit error
        mov     eax,[edi]               ; put SCB in IORslt
        mov     ebx,4[edi]              ; ...
        mov     _IORslt,eax             ; ...
        mov     _IORslt+4,ebx           ; ...
        mov     eax,PT_CHAR             ; indicate character
        hop     IORegs
efcode  RT_INP_CHAR


fcode   SIMPLE_INP_I4                   ; read into simple variable
        mov     ebx,PT_INT4             ; indicate integer*4
        hop     SimpInp
efcode  SIMPLE_INP_I4


fcode   SIMPLE_INP_R4                   ; read into simple variable
        mov     ebx,PT_REAL             ; indicate real*4
        hop     SimpInp
efcode  SIMPLE_INP_R4


fcode   SIMPLE_INP_R8                   ; read into simple variable
        mov     ebx,PT_DBLE             ; indicate real*8
;;;;;;;;hop     SimpInp
efcode  SIMPLE_INP_R8


defn    SimpInp
        getdw   edi                     ; get address of variable
        add     edi,ebp
;;;;;;;;hop     Inp
endproc SimpInp


        xdefp   IORegs
defp    Inp
        exit_fcode                      ; switch to run-time environment
        docall  ChkLimitErr_            ; check for limit error
        mov     _IORslt,edi             ; put address in IORslt
        mov     eax,ebx                 ; get type
IORegs:
        mov     SaveESI,esi             ; save F-Code program counter
        mov     SaveEBP,ebp             ; save data pointer
        pop     ebp                     ; ...
        ret                             ; return to run-time system
endproc Inp


fcode   STRUCT_IO_READ_ITEM
        mov     dl,1                    ; indicate this is a read
do_sio:
        getdw   eax                     ; get pointer to sio_cntrl_blk
        exit_fcode                      ; switch to run-time environ
        add     eax,ebp
        mov     ebx,ebp
        call    StructIOItem_           ; call run-time routine
        cmp     al,PT_NOTYPE            ; check if done struct i/o
        _if     ne                      ; - if not done,
          sub   esi,FC_LEN + 4          ; - loop back to ourselves
          jmp   IORegs                  ; - then return to run-time system
        _endif
        enter_fcode                     ; switch to f-code environ
        next
efcode  STRUCT_IO_READ_ITEM


fcode   STRUCT_IO_WRITE_ITEM
        xor     dl,dl                   ; indicate this is a write
        hop     do_sio
efcode  STRUCT_IO_WRITE_ITEM


fcode   STRUCT_IO_INIT
        getdw   eax                     ; get pointer to sio_cntrl_blk
        exit_fcode                      ; switch to run-time environ
        add     eax,ebp
        mov     edx,ebp
        call    StructIOInit_           ; call run-time routine
        enter_fcode                     ; switch to f-code environ
        next
efcode  STRUCT_IO_INIT


fcode   IO_STRUCT_ARRAY
        call    ArrInit                 ; print an array
        mov     edi,AIO_PTR[edi]        ; point to next element
        next                            ; do the next fcode
efcode  IO_STRUCT_ARRAY


fcode   PRT_ARRAY
dfcode  INP_ARRAY
        getdw   edi                     ; get ADV pointer in ebx
        getdw   ebx                     ; get the array type
        add     edi,ebp                 ; ...
        mov     eax,adv_array[edi]      ; get array offset in ecx
        mov     ecx,adv_elsize[edi]     ; get array element size in edx
        mov     edx,adv_elnum[edi]      ; get number of array elts
        mov     dword ptr _IORslt,EAX   ; ... data pointer
        mov     dword ptr _IORslt+4,EDX ; ... number of elements
        mov     dword ptr _IORslt+8,ECX ; ... element size ( for PT_CHAR )
        mov     byte ptr _IORslt+12,BL  ; ... type of array
        exit_fcode                      ; switch to run-time environment
        docall  ChkLimitErr_            ; check for limit error
        mov     EAX,PT_ARRAY            ; return ARRAY type
        jmp     IORegs                  ; Spew out the watermellon seeds
efcode  PRT_ARRAY


defn    ArrInit
        getdw   ebx                     ; get ADV pointer in ebx
        getdw   edi                     ; get the array prt control block addr
        add     ebx,ebp
        add     edi,ebp
        mov     ecx,adv_array[ebx]      ; get array offset in ecx
        mov     edx,adv_elsize[ebx]     ; get array element size in edx
        mov     eax,adv_elnum[ebx]      ; get number of array elts
        mov     AIO_PTR[edi],ecx        ; put it in the control block
        dec     eax                     ; decrement for first element
        mov     AIO_SIZE[edi],edx       ; put it in the control block
        mov     AIO_NUM[edi],eax        ; put it in the control block
        ret
endproc ArrInit


fcode   NEXT_ARR_ELT                    ; setup for the next array element
        call    DoNext                  ; set up for next element to print
        getdw   eax                     ; set branch address in AX
        _if     nc                      ; if there is another element to prt
          mov   edi,AIO_PTR[edi]        ; - point to next element
          add   esi,eax                 ; - set branch address in esi
        _endif                          ; endif
        next                            ; do the next fcode
efcode  NEXT_ARR_ELT


defn    DoNext                          ; set up to print next elt - carry set
                                        ; ... if there are no more to print
        getdw   edi                     ; load control block addr into DI
        add     edi,ebp
        sub     dword ptr AIO_NUM[edi],1; decrement number of elements left
        _if     nc                      ; if there are more to print...
          mov   eax,AIO_SIZE[edi]       ; - get the size of elements
          add   AIO_PTR[edi],eax        ; - advance to next element
          clc                           ; - indicate say we have an element
        _endif                          ; endif
        ret                             ; return to caller
endproc DoNext


fcode   RT_SET_NOFMT            ; set no "FMT=" specified
        exit_fcode              ; switch to run-time environment
        docall SetNoFmt         ; set no "FMT="
        enter_fcode             ; switch to F-Code environment
        next                    ; goto next rtn
efcode  RT_SET_NOFMT


fcode   RT_SET_FMT              ; set format pointer
        getdw   eax             ; get addr of format string
        add     eax,esi         ; ...
call_fmt:
        exit_fcode              ; switch to run-time environment
        docall  SetFmt          ; set the format string
        enter_fcode             ; switch to F-Code environment
        next
efcode  RT_SET_FMT


fcode   RT_FMT_ASSIGN
        getdw   ebx                     ; get address of variable
        getdw   eax                     ; get the address of the format table
        add     ebx,ebp
        push    esi                     ; save F-Code instruction pointer
        add     esi,eax                 ; put format table address in esi
        mov     eax,[ebx]               ; put variable value in eax
        mov     ecx,ST_FORMAT_LABEL     ; set error message
        push    eax                     ; save the format statement address
        call    StmtLook                ; see if the address is in the table
        pop     eax                     ; restore the format statement address
        pop     esi                     ; restore F-Code I.P.
        hop     call_fmt                ; set FMT=
efcode  RT_FMT_ASSIGN


fcode   RT_FMT_SCAN             ; encode format string from character expression
                                ; Note: the SCB is already on the stack
        xor     edx,edx         ; get /FORMAT indicator
        getword dx              ; ...
        mov     eax,esp         ; point to SCB
        exit_fcode              ; switch to run-time environment
        docall FmtScan          ; encode format string
        enter_fcode             ; switch to F-Code environment
        next    8               ; remove SCB from the stack
efcode  RT_FMT_SCAN


fcode   RT_FMT_ARR_SCAN
        xor     ecx,ecx         ; get /FORMAT indicator
        getword cx              ; ...
        getdw   eax             ; get addres of ADV
        add     eax,ebp
        mov     ebx,adv_elsize[eax]; pass element size
        mov     edx,adv_elnum[eax]; pass number of elements
        mov     eax,adv_array[eax]; pass address of array storage
        exit_fcode              ; switch to run-time environment
        docall  FmtAScan        ; encode format string
        enter_fcode             ; switch to F-Code environment
        next
efcode  RT_FMT_ARR_SCAN


fcode   RT_SET_IOCB
        xor     al,al
        hop     do_set_iocb
efcode  RT_SET_IOCB


fcode   RT_SET_IOCB_CHECK
        mov     al,1                    ; check undef's
do_set_iocb:
        exit_fcode                      ; switch to run-time environment
        call    SetIOFlags              ; ...
        mov     _IOTypeRtn,offset32 FC_IOType_; indicate f-code iotype rtn
        enter_fcode                     ; switch to f-code environ
        next
efcode  RT_SET_IOCB_CHECK


fcode   RT_SET_ERR                      ; ERR=
        getdw   eax                     ; get ERR= label
        add     eax,esi
        exit_fcode                      ; switch to run-time environment
        mov     ErrEq,eax               ; save ERR= label
        docall  SetErr                  ; call i/o routine
        enter_fcode                     ; switch to F-Code environment
        next
efcode  RT_SET_ERR


fcode   RT_SET_END                      ; END=
        getdw   eax                     ; get END= label
        test    eax,eax                 ; check if non-null END= label
        _if     ne                      ; ...
          add   eax,esi                 ; - convert offset to abs
          exit_fcode                    ; - switch to run-time environment
          mov   EndEq,eax               ; - save END= label
          docall SetEnd                 ; - call i/o routine
          enter_fcode                   ; - switch to F-Code environment
        _endif
        next
efcode  RT_SET_END


fcode   RT_SET_REC                      ; REC=
        mov     ebx,offset32 SetRec     ; get address of run-time routine
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_REC


fcode   RT_INQ_BLOCKSIZE                ; BLOCKSIZE=
        mov     ebx,offset32 InqBlkSize ; get address of run-time routine
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_INQ_BLOCKSIZE


fcode   RT_SET_BLOCKSIZE                ; BLOCKSIZE=
        mov     ebx,offset32 SetBlkSize ; get address of run-time routine
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_BLOCKSIZE


fcode   RT_SET_RECL                     ; RECL=
        mov     ebx,offset32 SetRecl    ; get address of run-time routine
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_RECL


fcode   RT_SET_UNIT                     ; setup default unit numbers
        mov     ebx,offset32 SetUnit    ; get address of run-time routine
;;;;;;;;hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_UNIT


defn    GetI4                           ; pass INTEGER*4 variable
        exit_fcode                      ; switch to run-time environment
        pop     eax                     ; get argument
        call    ebx                     ; call run-time routine
        enter_fcode                     ; switch to F-Code environment
        next
endproc GetI4


fcode   RT_SET_OPEN                     ; OPENED=
        mov     ebx,offset32 SetOpen    ; get address of run-time routine
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_OPEN


fcode   RT_SET_NMD                      ; NAMED=
        mov     ebx,offset32 SetNmd     ; get address of run-time routine
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_NMD


fcode   RT_SET_EXST                     ; EXIST=
        mov     ebx,offset32 SetExst    ; get address of run-time routine
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_EXST


fcode   RT_SET_UFMTD                    ; UNFORMATTED=
        mov     ebx,offset32 SetUnFmtd  ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_UFMTD


fcode   RT_SET_SEQ                      ; SEQUENTIAL=
        mov     ebx,offset32 SetSeq     ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_SEQ


fcode   RT_SET_CCTRL                    ; CARRIAGE_CONTROL=
        mov     ebx,offset32 SetCCtrl   ;get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_CCTRL


fcode   RT_SET_RECTYPE                  ; RECORD_TYPE=
        mov     ebx,offset32 SetRecType ;get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_RECTYPE


fcode   RT_SET_SHARE                    ; SHARE=
        mov     ebx,offset32 SetShare   ;get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_SHARE


fcode   RT_SET_ACTION                   ; ACTION=
        mov     ebx,offset32 SetAction  ;get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_ACTION


fcode   RT_SET_NAME                     ; NAME=
        mov     ebx,offset32 SetName    ;get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_NAME


fcode   RT_SET_FMTD                     ; FORMATTED=
        mov     ebx,offset32 SetFmtd    ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FMTD


fcode   RT_SET_NUMB                     ; NUMBER=
        mov     ebx,offset32 SetNumb    ; get address of run-time routine
;;;;;;;;hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_NUMB


defn    GetChrPtr                       ; pass pointer to SCB on stack
GetI4Ptr:                               ; pass pointer to INTEGER*4 variable
GetL4Ptr:                               ; pass pointer to LOGICAL*4 variable
        exit_fcode                      ; switch to run-time environment
        mov     eax,edi                 ; pass addr to routine
        call    ebx                     ; ...
        enter_fcode                     ; switch to F-Code environment
        next                            ; go do next F-Code
endproc GetChrPtr


fcode   RT_SET_DIR                      ; DIRECT=
        mov     ebx,offset32 SetDir     ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_DIR


fcode   RT_SET_STAT                     ; STATUS=
        mov     ebx,offset32 SetStat    ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_STAT


fcode   RT_SET_FORM                     ; FORM=
        mov     ebx,offset32 SetForm    ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FORM


fcode   RT_SET_BLNK                     ; BLANK=
        mov     ebx,offset32 SetBlnk    ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_BLNK


fcode   RT_SET_ACC                      ; ACCESS=
        mov     ebx,offset32 SetAcc     ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_ACC


fcode   RT_SET_FILE                     ; FILE=
        mov     ebx,offset32 SetFile    ; get address of run-time routine
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FILE


fcode   RT_SET_IOS                      ; set address for IOSTAT= clause
        getdw   eax                     ; get label for IOSTAT=
        add     eax,esi                 ; ...
        mov     IOSLabel,eax            ; ...
        mov     ebx,offset32 SetIOS     ; get address of run-time routine
        hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_IOS


fcode   RT_SET_NREC                     ; set address for IOSTAT= clause
        mov     ebx,offset32 SetNRec    ; get address of run-time routine
        hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_NREC


fcode   RT_SET_LEN                      ; RECL= for inquire statement
        mov     ebx,offset32 SetLen     ; get address of run-time routine
        hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_LEN


fcode   RT_SET_INTL                     ; UNIT= for internal files
        getdw   edx                     ; get pointer to ADV in edx
        or      edx,edx                 ; check if null
        _if     ne                      ; if not null then it is an array
          getdw eax                     ; - get pointer to SCB in eax
          add   edx,ebp
          add   eax,ebp
          mov   ebx,adv_array[edx]      ; - get array origin and put it in SCB
          mov   ecx,adv_elsize[edx]     ; - get size of an array element and put
          mov   0[eax],ebx              ; - put it in the SCB
          mov   4[eax],ecx              ; - ...
          mov   edx,adv_elnum[edx]      ; - number of elements
        _else                           ; else it's a character variable
          inc   edx                     ; - indicate only 1 element
          mov   eax,edi                 ; - pass address to run-time routine
        _endif                          ; endif
        exit_fcode                      ; switch to run-time environment
        docall  SetIntl                 ; call run-time routine
        enter_fcode                     ; switch to F-Code environment
        next                            ; go do next F-Code
efcode  RT_SET_INTL


fcode   RT_SET_NML
        getword ax                      ; number of args
        mov     ebx,esp                 ; make stack frame
        movzx   ecx,ax                  ; get counter
        std                             ; let's go backwards
        lea     esi,[esi+ecx*4]         ; point at the last arg
        inc     ecx                     ; +1 for the namelist address
        lea     edi,4[esi]              ; save new program counter
nml_loop:
          lodsd                         ; - get offset of variable
          add   eax,ebp                 ; - convert into abs addr
          push  eax                     ; - push it on stack
        loop    nml_loop                ; until no args left
        mov     esi,edi                 ; restore program counter
        cld                             ; go forward again
        exit_fcode                      ; switch to run-time environment
        docall  SetNml                  ; call run-time routine
        enter_fcode                     ; switch to F-Code environment
        mov     esp,ebx                 ; remove args from stack
        next
efcode  RT_SET_NML


        fmodend
        end
