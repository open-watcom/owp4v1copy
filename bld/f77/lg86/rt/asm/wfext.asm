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
;   WFEXT       : part of the WATFOR to external object file interface
;

include fcdef.inc
include parmtype.inc
include structio.inc

        xref            WF77_to_WFC
        xref            WFC_to_WF77
        xref            WF77_to_SYSCALL
        xref            SYSCALL_to_WF77

        fmodstart       wfext

        dataseg

WFAddr  dw      0,0
ArgList dw      0,0
RetAddr dw      0,0
TmpRet  dw      0,0
AXTmp   dw      0
AltRet  dd      0
RetSCB  dd      0
Save80x87 db    108 dup(0)

        enddata

        xdefp   WF77_To_Ext
        defp    WF77_To_Ext
if _MATH eq _8087
        sub     sp,108                  ; save 80387 environment
        mov     bx,sp
        fsave   ss:[bx]                 ; ...
;       fldz                            ; set up 80387 stack
;       fldz                            ; ...
;       fldz                            ; ...
;       fldz                            ; ...
        fwait
endif
        push    SS                      ; call external program
        push    BP                      ; ...
        mov     al,[si]                 ; see if this is a syscall function
        and     al,SYSTEM_CALL          ; ...
        _if     ne                      ; if it is
          call  WF77_to_SYSCALL         ; call sys call function
        _else                           ;
          call  WF77_to_WFC             ; call regular function
        _endif
        pop     si                      ; process possible alternate return
        pop     ds                      ; ...
        mov     di,offset SS:AltRet     ; ...
        push    ss                      ; ...
        pop     es                      ; ...
        mov     ax,[si]                 ; ...
        mov     es:[di],ax              ; ...
if _MATH eq _8087
        mov     bx,sp
        frstor  ss:[bx]                 ; restore 80387 environment
        fwait                           ; ...
        add     sp,108                  ; ...
endif
        ret
endproc WF77_To_Ext



ProcRet dd      Ret4                    ; subroutine
        dd      Ret1                    ; logical*1
        dd      Ret4                    ; logical*4
        dd      Ret1                    ; integer*1
        dd      Ret2                    ; integer*2
        dd      Ret4                    ; integer*4
        dd      RetR4                   ; real*4
        dd      RetR8                   ; real*8
        dd      RetR16                  ; real*16
        dd      RetC8                   ; complex*8
        dd      RetC16                  ; complex*16
        dd      RetC32                  ; complex*32
        dd      RetChr                  ; character
        dd      RetStruct               ; structure

ProcSysRet dd   Ret4                    ; subroutine
        dd      Ret1                    ; logical*1
        dd      Ret4                    ; logical*4
        dd      Ret1                    ; integer*1
        dd      Ret2                    ; integer*2
        dd      Ret4                    ; integer*4
        dd      RetSysR4                ; real*4
        dd      RetSysR8                ; real*8
        dd      RetSysR16               ; real*16
        dd      RetSysC8                ; complex*8
        dd      RetSysC16               ; complex*16
        dd      RetSysC32               ; complex*32
        dd      RetChr                  ; character
        dd      RetSysStruct            ; structure

defp    Ret1
        mov     al,[si]
        hop     regs0
endproc Ret1

defp    Ret2
        mov     ax,[si]
regs0:
        pop     si
        pop     di
        pop     bp
        pop     dx
        pop     cx
        pop     bx
        pop     ds
        pop     es
nukestack:
        mov     ss:AXTmp, ax    ; save ax
        pop     ax              ; get size of stack to nuke
        pop     ss:TmpRet       ; save return address
        pop     ss:TmpRet+2     ; ...
        add     sp, ax          ; nuke stack
        push    ss:TmpRet+2     ; restore return address
        push    ss:TmpRet       ; ...
        mov     ax, ss:AXTmp    ; restore ax
        ret
endproc Ret2

if _MATH eq _8087
defp    RetR4
        fld     dword ptr [si]
        _fwait
        hop     regs0
endproc RetR4
else
RetR4:                          ; fall through if not 8087
endif

defp    Ret4
        mov     ax,[si]
        mov     dx,2[si]
regs1:
        pop     si
        pop     di
        pop     bp
        pop     cx              ; throw away dx
        pop     cx
        pop     bx
        pop     ds
        pop     es
        jmp     nukestack       ; clear of stack and return
endproc Ret4

defp    RetSysR4
        ; es:di already points to destination
        mov     ax,di                   ; return pointer to result
        mov     dx,es                   ; ...
        movsw                           ; copy return value
        movsw                           ; ...
        hop     regs1
endproc RetSysR4

defp    RetR8
if _MATH eq _8087
        fld     qword ptr [si]
        fwait
        jmp     regs0
else
        mov     dx,[si]
        mov     cx,2[si]
        mov     bx,4[si]
        mov     ax,6[si]
        pop     si
        pop     di
        pop     bp
        add     sp,6                    ; throw away dx cx bx
        pop     ds
        pop     es
        jmp     nukestack               ; clear of stack and return
endif
endproc RetR8

defp    RetSysR8
        ; es:di already points to destination
        mov     ax,di                   ; return pointer to result
        mov     dx,es                   ; ...
        hop     regs2
endproc RetSysR8

defp    RetC8
        pop     di                      ; set edi to point to storage for
        push    di                      ; ... structured return
        push    ss                      ; set es to ss
        pop     es                      ; ...
regs2:
        movsw                           ; copy return value
        movsw                           ; ...
        movsw                           ; ...
        movsw                           ; ...
        hop     regs0
endproc RetC8

defp    RetSysC8
        ; es:di already points to destination
        mov     ax,di                   ; return pointer to result
        mov     dx,es                   ; ...
        hop     regs2
endproc RetSysC8

defp    RetC32
        pop     di                      ; set edi to point to storage for
        push    di                      ; ... structured return
        push    ss                      ; set es to ss
        pop     es                      ; ...
        movsw                           ; copy return value
        movsw                           ; ...
        movsw                           ; ...
        movsw                           ; ...
        hop     Ret32Cont               ; continue
endproc RetC32

defp    RetC16
RetR16:
        pop     di                      ; set edi to point to storage for
        push    di                      ; ... structured return
        push    ss                      ; set es to ss
        pop     es                      ; ...
Ret32Cont:
        movsw                           ; copy return value
        movsw                           ; ...
        movsw                           ; ...
        movsw                           ; ...
        hop     regs2
endproc RetC16

defp    RetSysC16
RetSysR16:
        ; es:di already points to destination
        mov     ax,di                   ; return pointer to result
        mov     dx,es                   ; ...
        hop     Ret32Cont
endproc RetSysC16


defp    RetSysC32
        ; es:di already points to destination
        mov     ax,di                   ; return pointer to result
        mov     dx,es                   ; ...
        movsw                           ; copy return value
        movsw                           ; ...
        movsw                           ; ...
        movsw                           ; ...
        hop     Ret32Cont
endproc RetSysC32

defp    RetChr
        mov     ax,di
        mov     dx,ds
        hop     regs1
endproc RetChr

defp    RetStruct
        les     di,[si]                 ; get ptr to storage from RCB
        mov     dx,es:2[di]             ; and load first dword of storage
        mov     ax,es:[di]              ; to handle 1,2,4 byte structs
        hop     regs1
endproc RetStruct

defp    RetSysStruct
        mov     ax,[si]                 ; get ptr to storage from RCB
        mov     dx,2[si]                ; ...
        hop     regs0
endproc RetSysStruct


        xdefp   Ext_To_WF77
defp    Ext_To_WF77
        pop     SS:WFAddr               ; get address of WATFOR-77 subprogram
        pop     SS:WFAddr+2
        pop     SS:ArgList              ; get address of WATFOR-77 argument list
        pop     SS:ArgList+2
        pop     SS:RetAddr              ; get return addr of external subpgm
        pop     SS:RetAddr+2
if _MATH eq _8087
        fsave   SS:Save80x87            ; save state of 80x87
        exit_fpu                        ; set up for WFC
endif
        push    ax                      ; save ax
        push    si                      ; save si and ds
        push    ds                      ; ...
        mov     si,ss:ArgList           ; load ptr to arglist
        mov     ds,ss:ArgList+2         ; ...
        mov     al,[si]                 ; load ret type
        mov     ah,2[si]                ; locad possible call type
        pop     ds                      ; restore ds, si
        pop     si                      ; ...
        push    ax                      ; save ax
        and     al,SYSTEM_CALL          ; is it a system call
        pop     ax                      ; restore ax
        _if     e                       ; if it is not, call normal process
          pop   ax                      ; restore ax
          push  cx                      ; stack arguments in registers
          push  bx                      ; ...
          push  dx                      ; ...
          push  ax                      ; ...
          push  ss                      ; push addr of SCB for ret value
          mov   ax,offset RetSCB        ; ...
          push  ax                      ; ...
          push  ss:ArgList+2            ; restore address of arg list
          push  ss:ArgList
          call  WFC_to_WF77             ; process argument list
          cmp   ax,08h                  ; check for # of bytes passed
          _if   l                       ; if less than 8 bytes
            mov ax,08h                  ; - make it 8 bytes
          _endif                                ; endif
        _else
          and   ah, TY_CALL_MASK        ; what call tyoe is it?
          cmp   ah, TY_PASCAL_CALL      ; is it pascal
          _if   e
            and al,TYPE_MASK            ; what type is it?
            _guess                      ; case statement
              cmp al, PT_REAL           ; ...
              _quif e                   ; ...
              cmp al, PT_DBLE           ; ...
              _quif e                   ; ...
              cmp al, PT_XTND           ; ...
              _quif e                   ; ...
              cmp al, PT_CPLX           ; ...
              _quif e                   ; ...
              cmp al, PT_DBCX           ; ...
              _quif e                   ; ...
              cmp al, PT_XTCX           ; ...
              _quif e                   ; ...
              cmp al, PT_STRUCT         ; ...
              _quif e                   ; ...
              pop ax                    ; clear off stack
            _admit
              pop ax                    ; clear off stack
              pop ax                    ; get offset
              push ss                   ; push segment of return address
              push ax                   ; restore offset
            _endguess
          _else
            pop ax                      ; pop off ax
          _endif
          push  ss                      ; push addr of SCB for ret value
          mov   ax,offset RetSCB        ; ...
          push  ax                      ; ...
          push  ss:ArgList+2            ; restore address of arg list
          push  ss:ArgList
          call  SYSCALL_to_WF77         ; else process system arguments
        _endif
if _MATH eq _8087
        frstor  SS:Save80x87            ; ...
endif
        add     sp,8                    ; clean off first 2 args of stack
        push    ss:RetAddr+2            ; restore addr of external subpgm
        push    ss:RetAddr              ; ...
        push    ax                      ; save size of stack to clear
        push    es                      ; save registers
        push    ds                      ; ...
        push    bx                      ; ...
        push    cx                      ; ...
        push    dx                      ; ...
        push    bp                      ; ...
        push    di                      ; ...
        push    si                      ; must be saved last
        les     di,SS:RetSCB            ; get addr of possible return SCB
if _MATH eq _8087
        sub     sp,108                  ; save 80387 environment
        mov     bx,sp                   ; ...
        fsave   ss:[bx]                 ; ...
endif
        push    es                      ; save ptr to SCB
        push    di                      ; ...
        lds     bx,dword ptr SS:ArgList ; get ptr to arg list
        push    ds                      ; put address of argument list on (in case of recursion)
        push    bx                      ; ...
        mov     bl,[bx]                 ; get return type
        mov     bh, bl                  ; save for later user
        and     bl,TYPE_MASK            ; ...
        and     bh,SYSTEM_CALL
        _if     ne                      ; if it is
          jmp   Fini_Sys_Call           ; finish system call
        _endif
;;;;;;;;;;hop   Fini_WFC_Call           ; fini normal call
endproc Ext_To_WF77

defp    Fini_WFC_Call
        _guess
          cmp   bl,PT_CHAR              ; - see if character function
          _quif ne
          mov   cx,es:4[di]             ; - get length in cx
          les   di,es:[di]              ; - get data pointer in es:di
        _admit
          cmp   bl,PT_STRUCT            ; - see if structure function
          _quif ne
          mov   di,si                   ; - get ptr to structure storage
          push  ss                      ; - ...
          pop   es                      ; - ...
        _endif
        call    dword ptr SS:[WFAddr]   ; call WATFOR-77 subprogram
        pop     bx                      ; get address of argument list
        pop     es                      ; ...
if _MATH eq _8087
        mov     bp,sp
        frstor  4[bp]                   ; restore 80387 environ
endif
        mov     bl,es:[bx]              ; get return type
        and     bx,TYPE_MASK            ; ...
        pop     di                      ; get address of possible return SCB
        pop     es                      ; ...
if _MATH eq _8087
        fwait                           ; wait for FPU before removing environ
        add     sp,108                  ; ...
endif
        shl     bx,1
        shl     bx,1
        jmp     cs:ProcRet[bx]          ; process return value
endproc Fini_WFC_Call

defp    Fini_Sys_Call
        _guess
          cmp   bl,PT_CHAR              ; - see if character function
          _quif ne
          mov   cx,es:4[di]             ; - get length in cx
          les   di,es:[di]              ; - get data pointer in es:di
        _endif
        call    dword ptr SS:[WFAddr]   ; call WATFOR-77 subprogram
        pop     bx                      ; get address of argument list
        pop     es                      ; ...
if _MATH eq _8087
        mov     bp,sp
        frstor  4[bp]                   ; restore 80387 environ
endif
        mov     bx,es:[bx]              ; get return type
        and     bx,TYPE_MASK            ; ...
        pop     di                      ; get address of possible return SCB
        pop     es                      ; ...
if _MATH eq _8087
        fwait                           ; wait for FPU before removing environ
        add     sp,108                  ; ...
endif
        shl     bx,1
        shl     bx,1
        jmp     cs:ProcSysRet[bx]       ; process other value
endproc Fini_Sys_Call

        fmodend
        end
