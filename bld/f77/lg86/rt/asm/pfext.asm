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


        .8087
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<> FORTRAN-77 run-time support for 8086 based machines
;<>
;<> PFEXT - interface code used to convert between WATFOR-77
;<>         and IBM Professional FORTRAN argument lists
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

PT_NOTYPE       equ 0            ; no type(also PT_HEX and other asorted)
PT_LOG1         equ 1            ; logical*1
PT_LOG4         equ 2            ; logical*4
PT_INT1         equ 3            ; integer*1
PT_INT2         equ 4            ; integer*2
PT_INT4         equ 5            ; integer*4
PT_REAL         equ 6            ; real*4
PT_DBLE         equ 7            ; real*8
PT_XTND         equ 9            ; real*16
PT_CPLX         equ 10           ; complex*8
PT_DBCX         equ 11           ; complex*16
PT_XTCX         equ 12           ; complex*32
PT_CHAR         equ 13           ; character
PT_DUMM         equ 14           ; dummy
PT_EDAT         equ 15           ; end of data set for DATA statement

PC_CONST        equ 0            ; constant
PC_VARIABLE     equ 1            ; simple variable name
PC_ARRAY_ELT    equ 2            ; array element
PC_SS_ARRAY     equ 3            ; substrung array element
PC_ARRAY_NAME   equ 4            ; array name
PC_PROCEDURE    equ 5            ; subprogram name func/subr established
PC_FN_OR_SUB    equ 6            ; subprogram name type not established
PC_STATEMENT    equ 7            ; alternate return specifier
PC_RESULT       equ 8            ; expected result type of subprogram

IF_IND          equ 10H          ; intrinisic function indicator
VAR_LEN_CHR     equ 80H          ; variable length character argument/ret
IND_PARM        equ 80H          ; parameter is indirect
TYPE_MASK       equ 0fH          ; type information mask
CODE_MASK       equ 0fH          ; code information mask
COUNT_MASK      equ 03H          ; count mask for intrinsic functions
INFO_MASK       equ 3fH          ; mask to zero unused bits in if info

;
;       Array dope vector (ADV) layout
;
adv_array       equ 0            ; offset of array storage
adv_seg         equ 2            ; segment addr of array storage
adv_link        equ 4            ; link to chain all ADVs together
adv_lseg        equ 6            ; . . .
adv_elsize      equ 8            ; element size
adv_elnum       equ 10           ; number of elements (32-bit)
adv_numss       equ 14           ; number of subscripts
adv_bound       equ 16           ; start of bounds for each subscript
;
;       the bounds are in order from subscript n to subscript 1
;       for each subscript, there is
;               +0  low bound (low word)
;               +2  low bound (high word)
;               +4  high bound - low bound  (number of elements - 1)

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;
;   WF77_To_Ext:
;
;   Operations :(1) Convert WATFOR-77 argument list to conform to
;                   external calling conventions.
;               (2) Call the external subprogram.
;               (3) Upon return from the external functions,
;                   convert from the external return value passing
;                   conventions and save return value in storage
;                   provided.
;               (4) Return to WATFOR-77 subprogram.
;
;   Registers  : ES:DI contains the 32 bit address of the subprogram
;                DS:SI point to the argument list
;                SS:BP points to return value storage
;
;   WATFOR77 argument list:
;
;        DS:SI ==>      expected return/parm count
;                       p1 info word      --+
;                       p1 offset           | NOTE : an offset is not
;                       p2 info word        |        present for
;                       p2 offset           |        statement numbers
;                       . . .               |      : each offset and
;                       pn info word        |        info word are 16
;                       pn offset         --+        bits
;
;   INFO WORD:
;   1 X X X 1 1 0 0 X X X X 0 0 0 0
;   5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;   |       |     |         |     |
;   |       |     |         +-----+---> type (range 0..11)
;   |       |     |
;   |       |     |
;   |       |     |
;   |       |     |
;   |       |     |
;   |       |     |
;   |       |     |
;   |
;   |
;   |
;
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

PF@EXT  segment byte 'CODE'
        assume  CS:PF@EXT
        public  WF77_To_Ext
        public  Ext_To_WF77

WF77_To_Ext   proc far
        lodsw                   ; get the parm count/ret type in AX
        push  BP                ; save BP
        mov   BX,BP             ; put copy in BX
        push  AX                ; save the info word
        mov   CS:SAVE_SP,SP     ; save the current stack pointer
        and   AL,TYPE_MASK      ; get the type
        cmp   AL,PT_CHAR        ; check if it's a character function
        jne   notchr            ; if it is
        push  2[BP]             ; - save SCB in right order
        push  [BP]              ; - ...
        push  4[BP]             ; - ...
        mov   BX,SP             ; - point BX at new SCB
        sub   SP,4              ; - allocate space for SCB offset:seg
;                               ; endif
notchr: sub   AL,AL             ; zero the return type
        xchg  AL,AH             ; get parm count in AL
        shl   AX,1              ; multiply parm count by 4
        shl   AX,1              ; ...
        sub   SP,AX             ; allocate parmlist on the stack
        mov   DX,SP             ; save beg of parm list in DX
        mov   BP,SP             ; point BP to beg of parm list
        mov   CX,-2[SI]         ; get the type info back
        and   CL,TYPE_MASK      ; get the type
        cmp   CL,PT_CHAR        ; do we need the string SCB addr
        jne   noscb             ; if we do
        mov   [BP],BX           ; - save the offset
        mov   2[BP],SS          ; - save the segment
        add   BP,4              ; - point to the first parm
;                               ; endif
noscb:  or    CH,CH             ; set flags on the parm count
;                               ; loop
ploop:  je    eploop            ; - quit if all parms done
        lodsw                   ; - get parm info in AX
        mov   BX,[SI]           ; - get the offset
;                               ; - guess : create the proper parm list
        cmp   AH,PC_STATEMENT   ; - - is it an alternate return
        jne   altret            ; - - quit if it isn't
        jmp   egues             ; - admit
altret: inc   SI                ; - - point to next parm info
        inc   SI                ; - - ...
        test  AH,IND_PARM       ; - - check if indirect argument
        jne   indir             ; - - quit if indirect argument
        and   AH,CODE_MASK      ; - - isolate code information
        cmp   AH,PC_PROCEDURE   ; - - is it an procedure
        je    indir             ; - - quit if it is
        cmp   AH,PC_FN_OR_SUB   ; - - is it a function or subroutine
        je    indir             ; - - quit if it is
        and   AL,TYPE_MASK      ; - - isolate type information
        cmp   AL,PT_CHAR        ; - - check for character argument
        jne   not_ch            ; - - if character argument
        push  2[BX]             ; - - - set up SCB in PF format
        push  [BX]              ; - - - ...
        cmp   AH,PC_ARRAY_NAME  ; - - - check for array name
        jne   else              ; - - - if an array name
        push  adv_elsize[BX]    ; - - - - use array element size
        jmp   endif             ; - - - else
else:   push  4[BX]             ; - - - - use length from SCB
;                               ; - - - endif
endif:  mov   [BP],SP           ; - - - pass pointer to new SCB
        mov   2[BP],SS          ; - - - ...
        jmp   endif2            ; - - else
not_ch: cmp   AH,PC_ARRAY_NAME  ; - - - is it an array name
        je    indir             ; - - - quit if it is
        cmp   AH,PC_SS_ARRAY    ; - - - is it a substrung array
        je    indir             ; - - - quit if it is
        cmp   AH,PC_ARRAY_ELT   ; - - - is it an array element
        je    indir             ; - - - quit if it is
        mov   [BP],BX           ; - - - pass address of argument
        mov   2[BP],DS          ; - - - ...
;                               ; - - endif
endif2: add   BP,4              ; - - point to next argument list entry
        jmp   egues             ; - admit
indir:  mov   AX,[BX]           ; - - put the indirect address in the list
        mov   [BP],AX           ; - - ...
        mov   AX,2[BX]          ; - - ...
        mov   2[BP],AX          ; - - ...
        add   BP,4              ; - - point to next argument list entry
;                               ; - endguess
egues:  dec   CH                ; - decrement parm count
        jmp   ploop             ; endloop
;
;   call the subroutine
;
eploop: mov   AX,CS:SAVE_SP     ; get the bottom of stack addr
        push  AX                ; put it on the stack
        mov   AX,ES             ; put the callee segment in AX
        mov   BX,DX             ; set ES:BX to point to the parm list
        push  SS                ; ...
        pop   ES                ; ...
        push  CS                ; save the CS for a long return
        call  lcall             ; do a long call
        cld                     ; clear direction flag
        pop   BX                ; remove all parm info from the stack
        mov   SP,BX             ; ...
        pop   BX                ; get subroutine info in BX
        pop   BP                ; restore BP
;
;       process the function return value
;
        and   BL,TYPE_MASK      ; isolate return type
        sub   BH,BH             ; set BH to zero
        shl   BX,1              ; get index into table
        mov   BX,CS:rettyp[BX]  ; get the proper return value routine
        jmp   BX                ; process the return parm

Byte0:  ret                     ; return

ByteL1: cbw                     ; make it into a word
        mov   [BP],AX           ; save logical
        mov   2[BP],AX          ; ...
        ret                     ; return

ByteI1: cbw                     ; make it into a word
ByteI2: sub   DX,DX             ; make it into a double word
        mov   [BP],AX           ; save the result
        mov   2[BP],DX          ; ...
        ret                     ; return

ByteI4: fistp dword ptr [BP]    ; save the result
        fwait                   ; wait for completion
        ret                     ; return

ByteR4: fstp  dword ptr [BP]    ; save the result
        fwait                   ; wait for completion
        ret                     ; return

ByteR8: fstp  qword ptr [BP]    ; save the result
        fwait                   ; wait for completion
        ret                     ; return

ByteR16:fstp  _TBYTE ptr [BP]   ; save the result
        fwait                   ; wait for completion
        ret                     ; return

ByteC8: fstp  dword ptr [BP]    ; save the result
        fwait                   ; wait for completion
        fstp  dword ptr 4[BP]   ; ...
        fwait                   ; wait for completion
        ret                     ; return

ByteDC: fstp  qword ptr [BP]    ; save the result
        fwait                   ; wait for completion
        fstp  qword ptr 8[BP]   ; ...
        fwait                   ; wait for completion
        ret                     ; return

ByteXC: fstp  _TBYTE ptr [BP]   ; save the result
        fwait                   ; wait for completion
        fstp  _TBYTE ptr 16[BP] ; ...
        fwait                   ; wait for completion
        ret                     ; return

ByteRI: sub   DX,DX             ; zero the high word
        lea   DI,Alt_Ret        ; save the index
        mov   CS:[DI],AX        ; ...
        mov   CS:2[DI],DX       ; ...
        push  CS                ; point ES:DI to the index
        pop   ES                ; ...
        ret                     ; return

rettyp  dw    ByteRI            ; PT_NOTYPE
        dw    ByteL1            ; PT_LOG1
        dw    ByteL1            ; PT_LOG4
        dw    ByteI1            ; PT_INT1
        dw    ByteI2            ; PT_INT2
        dw    ByteI4            ; PT_INT4
        dw    ByteR4            ; PT_REAL
        dw    ByteR8            ; PT_DBLE
        dw    ByteR16           ; PT_XTND
        dw    ByteC8            ; PT_CPLX
        dw    ByteDC            ; PT_DBCX
        dw    ByteXC            ; PT_XTCX
        dw    Byte0             ; PT_CHAR

lcall:  mov   BP,SP             ; save the current SP in BP
        add   BP,4              ; point to the before the return addr
        push  AX                ; push the callee seg addr
        push  DI                ; push the callee offset
        ret                     ; call the subroutine

SAVE_SP dw    0                 ; temporary storage for stack pointer
Alt_Ret dd    0                 ; storage for alternate return address

WF77_To_Ext   endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;   Ext_To_WF77:
;
;   Operations :(1) Convert external argument list to conform to
;                   WATFOR-77 calling conventions.
;               (2) Call WATFOR-77 subprogram.
;               (3) Process return value.
;               (4) Return to external subprogram.
;
;   On entry   : All registers are as they were when external subprogram
;                called WATFOR-77 program except CS, IP, and SP.
;
;       |         | 4 byte address of WATFOR-77 subprogram
;       |         |
;       |         | 4 byte pointer to WATFOR-77 dummy argument list
;       |         |
;       |         | 4 byte return address to external subprogram
;       |         |
;
;   On exit    : ES:DI points to result destination if character function
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WF_addr dd    0

Ext_To_WF77   proc far
        pop   word ptr CS:WF_addr   ; get address of WATFOR-77 subprogram
        pop   word ptr CS:WF_addr+2 ; get address of WATFOR-77 subprogram
        pop   SI                ; point to WATFOR-77 argument list
        pop   DS                ; ...
        lodsw                   ; get ret type, arg count
;
;  if a character function, get the SCB and save it on the stack
;
        and   AL,TYPE_MASK      ; get the return type
        cmp   AL,PT_CHAR        ; check if character function
        jne   retval            ; if character function
        push  BX                ; - save pointer to address of SCB
;                               ; endif
;
;   fix up the parms
;
retval: mov   DI,BX             ; put parm list addr in DI
        mov   CX,AX             ; get the subroutine info in CX
;                               ; loop : process the parms
aloop:  or    CH,CH             ; - set the flags
        je    ealoop            ; - quit if parms are done
        dec   CH                ; - decrement number of parms
        lodsw                   ; - get the info word
        mov   DL,AL             ; - save *(*) info
        and   AL,TYPE_MASK      ; - isolate the type info
        and   AH,CODE_MASK      ; - isolate the code info
        push  AX                ; - save type info
;                               ; - guess : what type of parm
        cmp   AH,PC_STATEMENT   ; - - is it an alternate return
        jne   pcst              ; - - quit if isn't
        jmp   egues1            ; - admit
pcst:   cmp   AL,PT_CHAR        ; - - is it a char
        jne   pcchr             ; - - quit if isn't
        lodsw                   ; - - get the arg offset
        mov   BX,AX             ; - - put arg offset in BX
        push  SI                ; - - save current arg pointer
        mov   SI,ES:[DI]        ; - - get SCB offset
        push  ES                ; - - save current DS
        mov   ES,ES:2[DI]       ; - - set DS to SCB seg
        mov   AX,ES:2[SI]       ; - - copy over the SCB offset
        mov   [BX],AX           ; - - ...
        mov   AX,ES:4[SI]       ; - - ... seg
        mov   2[BX],AX          ; - - ...
        test  DL,VAR_LEN_CHR    ; - - check if character*(*)
        je    nstar             ; - - if it's character*(*)
        mov   AX,ES:0[SI]       ; - - - get length passed
        mov   4[BX],AX          ; - - - ...
;                               ; - - endif
nstar:  pop   ES                ; - - restore ES
        pop   SI                ; - - restore SI
        add   DI,4              ; - - point DI at next parm
        jmp   egues1            ; - admit
pcchr:  lodsw                   ; - - get the arg offset
        mov   BX,AX             ; - - put it in BX
        mov   DX,ES:[DI]        ; - - copy the 32 bit addr
        mov   [BX],DX           ; - - ...
        mov   DX,ES:2[DI]       ; - - ...
        mov   2[BX],DX          ; - - ...
        add   DI,4              ; - - point DI at next parm
;                               ; - endguess
egues1: pop   AX                ; - restore type info
;                               ; - guess
        cmp   AH,PC_ARRAY_NAME  ; - - are we expecting an array name
        jne   arr               ; - - quit if we aren't
        sub   DX,DX             ; - - zero the DX register
        mov   adv_link[BX],DX   ; - - set adv link to NULL
        mov   adv_lseg[BX],DX   ; - - ...
        and   AL,TYPE_MASK      ; - - get the element type
        sub   AH,AH             ; - - ...
        push  SI                ; - - save the current SI pointer
        mov   SI,AX             ; - - put table index in SI
        mov   DL,CS:DataLen[SI] ; - - get element size in DX
        mov   adv_elsize[BX],DX ; - - save it in the ADV
        pop   SI                ; - - restore SI
;                               ; - endguess
arr:    jmp   aloop             ; endloop
;
;   fix up return address
;
ealoop: mov   BX,CX             ; save return type
        cmp   CL,PT_CHAR        ; check if character function
        jne   nchr              ; if character function
        pop   DI                ; - point to address of SCB
        les   DI,ES:[DI]        ; - get address of SCB
        mov   CX,ES:[DI]        ; - get length of string
        les   DI,ES:2[DI]       ; - get address of string
;                               ; endif
nchr:   push  BX                ; save return type
        call  [WF_addr]         ; call WATFOR-77 subprogram
        pop   BX                ; get return type
        shl   BX,1              ; calculate offset into table
        mov   BX,CS:stktype[BX] ; get the routine addr
        call  BX                ; get address of return
        ret                     ; return PF subprogram

stktype dw    StkAlt            ; PT_NOTYPE
        dw    StkAL             ; PT_LOG1
        dw    StkAL             ; PT_LOG4
        dw    StkAL             ; PT_INT1
        dw    StkAX             ; PT_INT2
        dw    StkI4             ; PT_INT4
        dw    StkR4             ; PT_REAL
        dw    StkR8             ; PT_DBLE
        dw    StkR16            ; PT_XTND
        dw    StkC8             ; PT_CPLX
        dw    StkC16            ; PT_DBCX
        dw    StkC32            ; PT_XTCX
        dw    StkNull           ; PT_CHAR

DataLen db    0,1,4,1,2,4,4,8,16,8,16,32 ; size of arguments in bytes

Ext_To_WF77   endp

StkAL   proc  near
        mov   AL,[SI]           ; get value
        ret
StkAL   endp

StkAX   proc  near
        mov   AX,[SI]           ; get value
        ret
StkAX   endp

StkI4   proc  near
        fild  dword ptr [SI]    ; load integer into stack
        fwait                   ; wait for completion
        ret
StkI4   endp

StkR4   proc  near
        fld   dword ptr [SI]    ; load real into stack
        fwait                   ; wait for completion
        ret
StkR4   endp

StkR8   proc  near
        fld   qword ptr [SI]    ; load real*8 into stack
        fwait                   ; wait for completion
        ret
StkR8   endp

StkR16  proc  near
        fld   _TBYTE ptr [SI]   ; load real*8 into stack
        fwait                   ; wait for completion
        ret
StkR16  endp

StkC8   proc  near
        fld   dword ptr 4[SI]   ; load imaginary part of complex into stack
        fwait                   ; wait for completion
        fld   dword ptr 0[SI]   ; load real part of complex into stack
        fwait                   ; wait for completion
        ret
StkC8   endp

StkC16  proc  near
        fld   qword ptr 8[SI]   ; load imaginary part of complex into stack
        fwait                   ; wait for completion
        fld   qword ptr 0[SI]   ; load real part of complex into stack
        fwait                   ; wait for completion
        ret
StkC16  endp

StkC32  proc  near
        fld   _TBYTE ptr 16[SI] ; load imaginary part of complex into stack
        fwait                   ; wait for completion
        fld   _TBYTE ptr 0[SI]  ; load real part of complex into stack
        fwait                   ; wait for completion
        ret
StkC32  endp

StkNull proc  near
        ret
StkNull endp

StkAlt  proc  near
        mov   AX,[SI]           ; get return index
        ret
StkAlt  endp

PF@EXT  ends

        end
