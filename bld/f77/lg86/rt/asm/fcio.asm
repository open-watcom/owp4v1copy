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
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

.8087

include fcdef.inc
include adv.inc
include parmtype.inc
include errcod.inc
include prtarr.inc
include datamap.inc
include xfflags.inc

        xref    DoRead_
        xref    DoWrite_
        xref    StructIOItem_
        xref    StructIOInit_
        xref    LimError_

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
        rtxref  SetShare
        rtxref  SetAction
        rtxref  SetRecType
        rtxref  SetBlkSize
        rtxref  InqBlkSize

        extrn   StmtLook        : near

        fmodstart fcio

        dataseg

        extrn   _IORslt         : word
        extrn   _IOTypeRtn      : dword

        SaveSI          dw      0
        EndEq           dw      0
        ErrEq           dw      0
        IOSLabel        dw      0
        IORtn           dw      2 dup(?)

        enddata


defn    ChkErrEq
        mov     SI,SS:ErrEq     ; get ERR= label
        or      SI,SI           ; check if ERR=
        _if     e               ; if no ERR=
          mov   SI,SS:IOSLabel  ; - try IOSTAT label
        _endif                  ; endif
        ret
endproc ChkErrEq


fcode   RT_EX_OPEN
        mov     AX,offset IOOpen
        mov     DX,seg IOOpen
        hop     IOStmt
efcode  RT_EX_OPEN


fcode   RT_EX_CLOSE
        mov     AX,offset IOClose
        mov     DX,seg IOClose
        hop     IOStmt
efcode  RT_EX_CLOSE


fcode   RT_EX_READ
        mov     AX,offset DoRead_
        mov     DX,seg DoRead_
        hop     IOStmt
efcode  RT_EX_READ


fcode   RT_EX_WRITE
        mov     AX,offset DoWrite_
        mov     DX,seg DoWrite_
        hop     IOStmt
efcode  RT_EX_WRITE


fcode   RT_EX_BACK
        mov     AX,offset IOBack
        mov     DX,seg IOBack
        hop     IOStmt
efcode  RT_EX_BACK


fcode   RT_EX_ENDF
        mov     AX,offset IOEndf
        mov     DX,seg IOEndf
        hop     IOStmt
efcode  RT_EX_ENDF


fcode   RT_EX_REW
        mov     AX,offset IORew
        mov     DX,seg IORew
        hop     IOStmt
efcode  RT_EX_REW


fcode   RT_EX_INQ
        mov     AX,offset IOInq
        mov     DX,seg IOInq
;;;;;;;;hop     IOStmt
efcode  RT_EX_INQ


defn    IOStmt
        exit_fcode_fpu          ; switch to run-time environment
        mov     SS:SaveSI,SI    ; save F-Code program counter
        mov     SS:IORtn,ax     ; do the I/O
        mov     SS:IORtn+2,dx   ; ...
        call    dword ptr SS:IORtn ; ...
        xor     SI,SI           ; assume no END= or ERR=
        or      AX,AX           ; check for END= or ERR=
        _if     l               ; if return code < 0
          mov   SI,SS:EndEq     ; - get END= label
          or    SI,SI           ; - check if END=
          _if   e               ; - if no END=
            call ChkErrEq       ; - - check ERR=
          _endif                ; - endif
        _else                   ; else
          _if   g               ; - if return code > 0
            call ChkErrEq       ; - - check ERR=
          _endif                ; - endif
        _endif                  ; endif
        or      SI,SI           ; check if END=, ERR=, or IOSTAT= label
        _if     e               ; if no END=, ERR= or IOSTAT= label
          mov   SI,SS:SaveSI    ; - restore F-Code program counter
        _endif                  ; endif
        mov     SS:EndEq,0      ; reset END=
        mov     SS:ErrEq,0      ; reset ERR=
        mov     SS:IOSLabel,0   ; reset IOSTAT label
        enter_fcode_fpu         ; switch to F-Code environment
        next                    ; do next fcode
endproc IOStmt


        xdefp   ExecData_
defp    ExecData_
        push    DS              ; save registers
        push    SI              ; ...
        mov     DS,DX           ; set up F-Code environment
        mov     SI,AX           ; ...
        exit_fcode              ; switch to run-time environment
        mov     SS:SaveSI,SI    ; save F-Code program counter
                                ; indicate fcode iotype rtn
        mov     AX,offset FC_IOType_            ; To avoid a wasm bug, we
        mov     word ptr SS:_IOTypeRtn,AX       ; use a register
        mov     word ptr SS:_IOTypeRtn+2,cs     ; ...
        docall  IOData          ; execute DATA statement
        mov     SI,SS:SaveSI    ; restore F-Code program counter
        enter_fcode             ; switch to F-Code environment
        pop     SI              ; restore registers
        pop     DS              ; ...
        ret
endproc ExecData_


        xdefp    FC_IOType_
defp    FC_IOType_
        push    DS              ; ...
        push    BP              ; ...
        mov     SI,SS:SaveSI    ; restore F-Code program counter
        enter_fcode             ; switch to F-Code environment
        next                    ; execute next F-Code
endproc FC_IOType_


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>    Input Routines: NOTE: ES:DI is assumed to contain the address of
;<>                                the input item.
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


fcode   RT_INP_LOG1             ; input logical*1
        mov     ax,PT_LOG1      ; indicate logical*1
        hop     Inp             ; get address of item
efcode  RT_INP_LOG1


fcode   RT_INP_LOG4             ; input logical*4
        mov     ax,PT_LOG4      ; indicate logical*4
        hop     Inp             ; get address of item
efcode  RT_INP_LOG4


fcode   RT_INP_INT1             ; input integer*1
        mov     ax,PT_INT1      ; indicate integer*1
        hop     Inp             ; get address of item
efcode  RT_INP_INT1


fcode   RT_INP_INT2             ; input integer*2
        mov     ax,PT_INT2      ; indicate integer*2
        hop     Inp             ; get address of item
efcode  RT_INP_INT2


fcode   RT_INP_INT4             ; input integer*4
        mov     ax,PT_INT4      ; indicate integer*4
        hop     Inp             ; get address of item
efcode  RT_INP_INT4


fcode   RT_INP_REAL             ; input real*4
        mov     ax,PT_REAL      ; indicate real*4
        hop     Inp             ; get address of item
efcode  RT_INP_REAL


fcode   RT_INP_DBLE             ; input real*8
        mov     ax,PT_DBLE      ; indicate real*8
        hop     Inp             ; get address of item
efcode  RT_INP_DBLE


fcode   RT_INP_XTND             ; input real*16
        mov     ax,PT_XTND      ; indicate real*16
        hop     Inp             ; get address of item
efcode  RT_INP_XTND


fcode   RT_INP_CPLX             ; input complex*8
        mov     ax,PT_CPLX      ; indicate complex*8
        hop     Inp             ; get address of item
efcode  RT_INP_CPLX


fcode   RT_INP_DBCX             ; input complex*16
        mov     ax,PT_DBCX      ; indicate complex*16
        hop     Inp             ; get address of item
efcode  RT_INP_DBCX


fcode   RT_INP_XTCX             ; input complex*32
        mov     ax,PT_XTCX      ; indicate complex*32
        hop     Inp             ; get address of item
efcode  RT_INP_XTCX


fcode   RT_INP_CHAR                     ; input character string
        exit_fcode                      ; switch to run-time environment
        test    SS:__XcptFlags,XF_LIMIT_ERR; check for limit error
        jne     in_lim                  ; report it if there is one
        mov     AX,ES:0[DI]             ; put SCB in IORslt
        mov     SS:_IORslt,AX           ; ...
        mov     AX,ES:2[DI]             ; ...
        mov     SS:_IORslt+2,AX         ; ...
        mov     AX,ES:4[DI]             ; ...
        mov     SS:_IORslt+4,AX         ; ...
        mov     AX,PT_CHAR              ; indicate character
        hop     IORegs
efcode  RT_INP_CHAR


fcode   SIMPLE_INP_I4                   ; read into simple variable
        mov     DI,PT_INT4              ; indicate integer*4
        hop     SimpInp
efcode  SIMPLE_INP_I4


fcode   SIMPLE_INP_R4                   ; read into simple variable
        mov     DI,PT_REAL              ; indicate real*4
        hop     SimpInp
efcode  SIMPLE_INP_R4


fcode   SIMPLE_INP_R8                   ; read into simple variable
        mov     DI,PT_DBLE              ; indicate real*8
;;;;;;;;hop     SimpInp
efcode  SIMPLE_INP_R8


defn    SimpInp
        getword AX                      ; get address of variable
        xchg    AX,DI                   ; AX==type, DI==offset
        push    DS                      ; get segment address of variable
        pop     ES                      ; . . .
;;;;;;;;hop     Inp
endproc SimpInp


        xdefp   IORegs
defp    Inp
        exit_fcode                      ; switch to run-time environment
        test    SS:__XcptFlags,XF_LIMIT_ERR; check for limit error
        jne     in_lim                  ; report error if there is one
OK:     mov     SS:_IORslt,DI           ; put address in IORslt
        mov     SS:_IORslt+2,ES         ; ...
IORegs:
        mov     SS:SaveSI,SI            ; save F-Code program counter
        pop     BP                      ; restore registers
        pop     DS                      ; ...
        ret                             ; return to run-time system
endproc Inp


defn    in_lim                          ; report a limit error
        push    ax                      ; save AX
        push    es                      ; save ES
        docall  LimError_               ; . . .
        pop     es                      ; restore ES
        pop     ax                      ; restore AX
        hop     OK                      ; did not want the CTRL/Break, continue
endproc in_lim


fcode   STRUCT_IO_READ_ITEM
        mov     bl,1                    ; indicate this is a read
do_sio:
        getword ax                      ; get pointer to sio_cntrl_blk
        mov     dx,ds                   ; make it into a PGM ptr
        mov     cx,ds                   ; set segment
        exit_fcode                      ; switch to run-time environ
        call    StructIOItem_           ; call run-time routine
        cmp     al,PT_NOTYPE            ; check if done struct i/o
        _if     ne                      ; - if not done,
          sub   si,FC_LEN + 2           ; - loop back to ourselves
          jmp   IORegs                  ; - then return to run-time system
        _endif
        enter_fcode                     ; switch to f-code environ
        next
efcode  STRUCT_IO_READ_ITEM


fcode   STRUCT_IO_WRITE_ITEM
        xor     bl,bl                   ; indicate this is a write
        hop     do_sio
efcode  STRUCT_IO_WRITE_ITEM


fcode   STRUCT_IO_INIT
        getword ax                      ; get pointer to sio_cntrl_blk
        mov     dx,ds                   ; turn into a pgm ptr
        mov     bx,ds                   ; get segment
        exit_fcode                      ; switch to run-time environ
        call    StructIOInit_           ; call run-time routine
        enter_fcode                     ; switch to f-code environ
        next
efcode  STRUCT_IO_INIT


fcode   IO_STRUCT_ARRAY
        call    ArrInit                 ; print a structure array
        les     DI,AIO_PTR[DI]          ; point to next element
        next                            ; do the next fcode
efcode  IO_STRUCT_ARRAY


fcode   PRT_ARRAY
dfcode  INP_ARRAY
        getword BX                      ; get ADV pointer in BX
        mov     AX,ADV_ARRAY[BX]        ; get array offset in AX
        mov     DX,ADV_ARRAY+2[BX]      ; get array segment in CX
        mov     SS:_IORslt,AX           ; ... data pointer
        mov     SS:_IORslt+2,DX         ; ...
        mov     AX,word ptr ADV_ELNUM[BX]; get lo word - num of array elts
        mov     DX,word ptr ADV_ELNUM+2[BX]; get hi word - num of array elts
        mov     CX,word ptr ADV_ELSIZE[BX] ; get array element size in CX
        mov     SS:_IORslt+4,AX         ; ... number of elements
        mov     SS:_IORslt+6,DX         ; ...
        mov     SS:_IORslt+8,CX         ; ... element size
        getword BX                      ; get the type
        mov     byte ptr SS:_IORslt+10,BL;... type
        mov     AX,PT_ARRAY             ; return ARRAY type
        exit_fcode                      ; switch to run-time environment
        test    SS:__XcptFlags,XF_LIMIT_ERR; check for limit error
        jne     in_lim                  ; report it if there is one
        jmp     IORegs                  ; Spew out the watermellon seeds
efcode  PRT_ARRAY


defn    ArrInit
        getword BX                      ; get ADV pointer in BX
        getword DI                      ; get the array prt control block addr
        mov     AX,ADV_ARRAY[BX]        ; get array offset in AX
        mov     CX,adv_array+2[BX]      ; get array segment in CX
        mov     word ptr AIO_PTR[DI],AX ; put it in the control block
        mov     word ptr AIO_PTR+2[DI],CX; put it in the control block
        mov     CX,ADV_ELSIZE[BX]       ; get array element size in CX
        mov     AX,word ptr ADV_ELNUM[BX]; get lo word - num of array elts
        mov     DX,word ptr ADV_ELNUM+2[BX]; get hi word - num of array elts
        mov     AIO_SIZE[DI],CX         ; put it in the control block
        sub     AX,1                    ; decrement for first element
        sbb     DX,0                    ; subtract borrow if necessary
        mov     word ptr AIO_NUM+0[DI],AX; put it in the control block
        mov     word ptr AIO_NUM+2[DI],DX; put it in the control block
        push    DS                      ; put current data seg on the stack
        pop     ES                      ; set it in ES
        ret
endproc ArrInit


fcode   NEXT_ARR_ELT                    ; setup for the next array element
        call    DoNext                  ; set up for next element to print
        getword AX                      ; set branch address in AX
        _if     nc                      ; if there is another element to prt
          les   DI,AIO_PTR[DI]          ; - point to next element
          mov   SI,AX                   ; - set branch address in SI
        _endif                          ; endif
        next                            ; do the next fcode
efcode  NEXT_ARR_ELT


defn    DoNext                          ; set up to print next elt - carry set
                                        ; ... if there are no more to print
        getword DI                      ; load control block addr into DI
        sub word ptr AIO_NUM+0[DI],1    ; decrement number of elements left
        sbb word ptr AIO_NUM+2[DI],0    ; ...
        _if     nc                      ; if there are more to print...
          mov   AX,word ptr AIO_PTR[DI] ; - get the current offset
          mov   CX,AIO_SIZE[DI]         ; - get the size of element
          add   AX,CX                   ; - point to the next element to print
          _if   c                       ; - if we overflowed the offset...
            add word ptr AIO_PTR+2[DI],1000H; - - update element segment
          _endif                        ; - endif
          mov   CX,AX                   ; - save new offset
          and   CX,0fH                  ; - get low four bits of offset
          mov   word ptr AIO_PTR[DI],CX ; - save updated offset
          shr   AX,1                    ; - get top 12 bits of new offset
          shr   AX,1                    ; - ...
          shr   AX,1                    ; - ...
          shr   AX,1                    ; - ...
          add   word ptr AIO_PTR+2[DI],AX; - update element segment
          mov   AX,DS                   ; - set ES = DS
          mov   ES,AX                   ; - ...
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
        getword AX              ; get addr of format string
call_fmt:
        mov     DX,DS           ; get seg of format string
        exit_fcode              ; switch to run-time environment
        docall  SetFmt          ; set the format string
        enter_fcode             ; switch to F-Code environment
        next
efcode  RT_SET_FMT


fcode   RT_FMT_ASSIGN
        getword BX                      ; get address of variable
        getword AX                      ; get the address of the format table
        push    SI                      ; save F-Code instruction pointer
        mov     SI,AX                   ; put format table address in SI
        mov     AX,0[BX]                ; put the low word of variable in AX
        mov     DX,2[BX]                ; put the high word of variable in DX
        push    AX                      ; save the format statement address
        mov     CX,ST_FORMAT_LABEL      ; set error message
        call    StmtLook                ; see if the address is in the table
        pop     AX                      ; restore the format statement address
        pop     SI                      ; restore F-Code I.P.
        hop     call_fmt                ; set FMT=
efcode  RT_FMT_ASSIGN


fcode   RT_FMT_SCAN             ; encode format string from character expression
                                ; Note: the SCB is already on the stack
        getword bx              ; get /FORMAT indicator
        mov     ax,sp           ; point to SCB
        mov     dx,ss           ; ...
        exit_fcode              ; switch to run-time environment
        docall  FmtScan         ; encode format string
        enter_fcode             ; switch to F-Code environment
        next    6               ; remove SCB from the stack
efcode  RT_FMT_SCAN


fcode   RT_FMT_ARR_SCAN
        getword AX                              ; get /FORMAT indicator
        push    AX                              ; pass /FORMAT indicator
        getword DI                              ; get addres of ADV
        mov     AX,adv_elsize[DI]               ; pass element size
        push    AX                              ; ...
        mov     BX,word ptr adv_elnum[DI]       ; pass number of elements
        mov     CX,word ptr adv_elnum+2[DI]     ; ...
        mov     DX,adv_array+2[DI]              ; pass address of array storage
        mov     AX,adv_array[DI]                ; ...
        exit_fcode                              ; switch to run-time environment
        docall  FmtAScan                        ; encode format string
        enter_fcode                             ; switch to F-Code environment
        next                                    ; execute next F-Code
efcode  RT_FMT_ARR_SCAN


fcode   RT_SET_IOCB
        xor     al,al                   ; zero ax
        hop     do_set_iocb
efcode  RT_SET_IOCB


fcode   RT_SET_IOCB_CHECK
        mov     al,1                    ; check undef's
do_set_iocb:
        exit_fcode                      ; switch to run-time environment
        docall  SetIOFlags              ; call run-time routine
        mov     ax,offset FC_IOType_    ; indicate it's fcode iotype rtn
        mov     word ptr SS:_IOTypeRtn,ax
        mov     word ptr SS:_IOTypeRtn+2,cs
        enter_fcode                     ; switch to f-code environ
        next
efcode  RT_SET_IOCB_CHECK


fcode   RT_SET_ERR                      ; ERR=
        getword AX                      ; get ERR= label
        exit_fcode                      ; switch to run-time environment
        mov     SS:ErrEq,AX             ; save ERR= label
        docall  SetErr                  ; call i/o routine
        enter_fcode                     ; switch to F-Code environment
        next
efcode  RT_SET_ERR


fcode   RT_SET_END                      ; END=
        getword AX                      ; get END= label
        test    AX,AX                   ; check if non-null END= label
        _if     ne                      ; ...
          exit_fcode                    ; - switch to run-time environment
          mov   SS:EndEq,AX             ; - save END= label
          docall SetEnd                 ; - call i/o routine
          enter_fcode                   ; - switch to F-Code environment
        _endif
        next
efcode  RT_SET_END


fcode   RT_SET_REC                      ; REC=
        mov     AX,offset SetRec        ; get address of run-time routine
        mov     DX,seg SetRec           ; ...
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_REC


fcode   RT_INQ_BLOCKSIZE                ; BLOCKSIZE=
        mov     AX,offset InqBlkSize    ; get address of run-time routine
        mov     DX,seg InqBlkSize       ; ...
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_INQ_BLOCKSIZE


fcode   RT_SET_BLOCKSIZE                ; BLOCKSIZE=
        mov     AX,offset SetBlkSize    ; get address of run-time routine
        mov     DX,seg SetBlkSize       ; ...
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_BLOCKSIZE


fcode   RT_SET_RECL                     ; RECL=
        mov     AX,offset SetRecl       ; get address of run-time routine
        mov     DX,seg SetRecl          ; ...
        hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_RECL


fcode   RT_SET_UNIT                     ; setup default unit numbers
        mov     AX,offset SetUnit       ; get address of run-time routine
        mov     DX,seg SetUnit          ; ...
;;;;;;;;hop     GetI4                   ; pass INTEGER*4 variable
efcode  RT_SET_UNIT


defn    GetI4                           ; pass INTEGER*4 variable
        mov     SS:IORtn,AX             ; save address of run-time routine
        mov     SS:IORtn+2,DX           ; ...
        exit_fcode                      ; switch to run-time environment
        pop     AX                      ; pass argument to routine
        pop     DX                      ; ...
        call    dword ptr SS:IORtn      ; call the run-time routine
        enter_fcode                     ; switch to F-Code environment
        next
endproc GetI4


fcode   RT_SET_UFMTD                    ; UNFORMATTED=
        mov     AX,offset SetUnFmtd     ; get address of run-time routine
        mov     DX,seg SetUnFmtd        ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_UFMTD


fcode   RT_SET_SEQ                      ; SEQUENTIAL=
        mov     AX,offset SetSeq        ; get address of run-time routine
        mov     DX,seg SetSeq           ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_SEQ


fcode   RT_SET_CCTRL                    ; CARRIAGE_CONTROL=
        mov     AX,offset SetCCtrl      ; get address of run-time routine
        mov     DX,seg SetCCtrl         ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_CCTRL


fcode   RT_SET_SHARE                    ; SHARE=
        mov     AX,offset SetShare      ; get address of run-time routine
        mov     DX,seg SetShare         ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_SHARE


fcode   RT_SET_RECTYPE                  ; RECORD_TYPE=
        mov     AX,offset SetRecType    ; get address of run-time routine
        mov     DX,seg SetRecType       ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_RECTYPE


fcode   RT_SET_ACTION                   ; ACTION=
        mov     AX,offset SetAction     ; get address of run-time routine
        mov     DX,seg SetAction        ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_ACTION


fcode   RT_SET_NAME                     ; NAME=
        mov     AX,offset SetName       ; get address of run-time routine
        mov     DX,seg SetName          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_NAME


fcode   RT_SET_FMTD                     ; FORMATTED=
        mov     AX,offset SetFmtd       ; get address of run-time routine
        mov     DX,seg SetFmtd          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FMTD


fcode   RT_SET_DIR                      ; DIRECT=
        mov     AX,offset SetDir        ; get address of run-time routine
        mov     DX,seg SetDir           ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_DIR


fcode   RT_SET_STAT                     ; STATUS=
        mov     AX,offset SetStat       ; get address of run-time routine
        mov     DX,seg SetStat          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_STAT


fcode   RT_SET_FORM                     ; FORM=
        mov     AX,offset SetForm       ; get address of run-time routine
        mov     DX,seg SetForm          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FORM


fcode   RT_SET_BLNK                     ; BLANK=
        mov     AX,offset SetBlnk       ; get address of run-time routine
        mov     DX,seg SetBlnk          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_BLNK


fcode   RT_SET_ACC                      ; ACCESS=
        mov     AX,offset SetAcc        ; get address of run-time routine
        mov     DX,seg SetAcc           ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_ACC


fcode   RT_SET_FILE                     ; FILE=
        mov     AX,offset SetFile       ; get address of run-time routine
        mov     DX,seg SetFile          ; ...
        hop     GetChrPtr               ; pass pointer to SCB
efcode  RT_SET_FILE


fcode   RT_SET_NREC                     ; set address for IOSTAT= clause
        mov     AX,offset SetNRec       ; get address of run-time routine
        mov     DX,seg SetNRec          ; ...
        hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_NREC


fcode   RT_SET_LEN                      ; RECL= for inquire statement
        mov     AX,offset SetLen        ; get address of run-time routine
        mov     DX,seg SetLen           ; ...
        hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_LEN


fcode   RT_SET_NUMB                     ; NUMBER=
        mov     AX,offset SetNumb       ; get address of run-time routine
        mov     DX,seg SetNumb          ; ...
;;;;;;;;hop     GetI4Ptr                ; pass pointer to INTEGER*4 variable
efcode  RT_SET_NUMB


defn    GetChrPtr                       ; pass pointer to SCB on stack
GetI4Ptr:                               ; pass pointer to INTEGER*4 variable
GetL4Ptr:                               ; pass pointer to LOGICAL*4 variable
        mov     SS:IORtn,AX             ; save address of run-time routine
        mov     SS:IORtn+2,DX           ; ...
        exit_fcode                      ; switch to run-time environment
        mov     DX,ES                   ; pass address to routine
        mov     AX,DI                   ; ...
        call    dword ptr SS:IORtn      ; call the run-time routine
        enter_fcode                     ; switch to F-Code environment
        next                            ; go do next F-Code
endproc GetChrPtr


fcode   RT_SET_OPEN                     ; OPENED=
        mov     AX,offset SetOpen       ; get address of run-time routine
        mov     DX,seg SetOpen          ; ...
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_OPEN


fcode   RT_SET_NMD                      ; NAMED=
        mov     AX,offset SetNmd        ; get address of run-time routine
        mov     DX,seg SetNmd           ; ...
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_NMD


fcode   RT_SET_EXST                     ; EXIST=
        mov     AX,offset SetExst       ; get address of run-time routine
        mov     DX,seg SetExst          ; ...
        hop     GetL4Ptr                ; pass pointer to LOGICAL*4 variable
efcode  RT_SET_EXST


fcode   RT_SET_IOS                      ; set address for IOSTAT= clause
        getword AX                      ; get label for IOSTAT=
        exit_fcode                      ; switch to run-time environment
        mov     SS:IOSLabel,AX          ; save label for IOSTAT=
        mov     DX,ES                   ; pass address to routine
        mov     AX,DI                   ; ...
        docall  SetIOS                  ; call the run-time routine
        enter_fcode                     ; switch to F-Code environment
        next                            ; go do next F-Code
efcode  RT_SET_IOS


fcode   RT_SET_INTL                     ; UNIT= for internal files
        getword bx                      ; get pointer to ADV in bx
        or      bx,bx                   ; check if null
        _if     ne                      ; if not null then it's an array
          getword DI                    ; - get pointer to SCB in DI
          mov   AX,adv_array[BX]        ; - get array origin and put it in SCB
          mov   DX,adv_array+2[BX]      ; - ...
          mov   CX,adv_elsize[BX]       ; - get size of an array element for SCB
          mov   0[DI],AX                ; - build the SCB
          mov   2[DI],DX                ; - ...
          mov   4[DI],CX                ; - ...
          mov   AX,DS                   ; - set segment address of SCB
          mov   CX,word ptr adv_elnum+2[BX]; - get number of elements
          mov   BX,word ptr adv_elnum[BX]; - ...
          mov   ES,AX                   ; - set segment address of SCB
        _else                           ; else it's a character variable
          xor   CX,CX                   ; - set high word of elmts to zero
          inc   BX                      ; - set low word to 1
        _endif                          ; endif
        mov     DX,ES                   ; pass address of SCB to routine
        mov     AX,DI                   ; ...
        exit_fcode                      ; switch to run-time environment
        docall  SetIntl                 ; call run-time routine
        enter_fcode                     ; switch to F-Code environment
        next                            ; go do next F-Code
efcode  RT_SET_INTL


fcode   RT_SET_NML
        getword ax                      ; number of args
        mov     bp,sp                   ; make stack frame
        mov     cx,ax                   ; get counter
        shl     ax,1                    ; multiply # of args by 2
        inc     cx                      ; +1 for the namelist address
        add     si,ax                   ; now si points at last arg
        std                             ; let's go backwards
        lea     di,+2[si]               ; get new program counter
nml_loop:
          getword ax                    ; - get a local addr
          push  ds                      ; - push segment
          push  ax                      ; - push offset
        loop    nml_loop                ; until no args left
        mov     si,di                   ; restore program counter
        cld                             ; go forward again
        exit_fcode                      ; switch to run-time environment
        docall  SetNml                  ; call run-time routine
        enter_fcode                     ; switch to F-Code environment
        mov     sp,bp                   ; remove args from stack
        next
efcode  RT_SET_NML


        fmodend
        end
