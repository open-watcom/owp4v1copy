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
        xref            WF77_to_SYSCALL
        xref            WFC_to_WF77
        xref            SYSCALL_to_WF77

        dataseg

WFAddr  dd      0
ArgList dd      0
RetAddr dd      0
TmpAddr dd      0
TmpRet  dd      0
AltRet  dd      0
RetSCB  dd      0
Save80x87 db    108 dup(0)

        enddata

        fmodstart       wfext

        xdefp   WF77_To_Ext
        defp    WF77_To_Ext
if _MATH eq _8087
        sub     esp,108                 ; save 80387 environment
        fsave   [esp]                   ; ...
;       fldz                            ; set up 80387 stack
;       fldz                            ; ...
;       fldz                            ; ...
;       fldz                            ; ...
        fwait
endif
        push    ebp
        mov     al, [esi]               ; look at syscall bit
        and     al, SYSTEM_CALL         ; check if it is a system call
        _if     e                       ; if not call wfc sub program
          call  WF77_to_WFC             ; ...
        _else                           ; ...
          call WF77_to_SYSCALL          ; Do a System call
        _endif                          ; ...
        pop     esi                     ; process possible alternate return
        mov     edi,offset32 AltRet     ; ...
        mov     eax,[esi]               ; ...
        mov     [edi],eax               ; ...
if _MATH eq _8087
        frstor  [esp]                   ; restore 80387 environment
        fwait                           ; ...
        add     esp,108                 ; ...
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
        dd      RetR4                   ; real*4
        dd      RetR8                   ; real*8
        dd      RetSysR16               ; real*16
        dd      RetSysC8                ; complex*8
        dd      RetSysC16               ; complex*16
        dd      RetSysC32               ; complex*32
        dd      RetChr                  ; character
        dd      RetSysStruct            ; structure

ProcStdRet dd   Ret4                    ; subroutine
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
        mov     al,[esi]
        hop     regs0
endproc Ret1

defp    Ret2
        mov     ax,[esi]
        hop     regs0
endproc Ret2

if _MATH eq _8087
defp    RetR4
        fld     dword ptr [esi]
        _fwait
        hop     regs0
endproc RetR4
else
RetR4:                          ; fall through if not 8087
endif

defp    Ret4
        mov     eax,[esi]
regs0:
        pop     esi
        pop     edi
        pop     ebp
        pop     edx
        pop     ecx
        pop     ebx

        ; stack clearing operation
        mov     TmpRet, eax             ; save return value
        pop     eax                     ; get amount of stack to clear
        pop     TmpAddr                 ; save return address
        lea     esp,8[esp+eax]          ; clean args from stack
        push    TmpAddr                 ; restore address
        mov     eax, TmpRet             ; restore eax
        ret
endproc Ret4

defp    RetSysR4
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        hop     regs0
endproc RetSysR4

defp    RetR8
if _MATH eq _8087
        fld     qword ptr [esi]
        fwait
        jmp     regs0
else
        mov     eax,[esi]
        mov     edx,4[esi]
        pop     esi
        pop     edi
        pop     ebp
        pop     ecx                     ; throw away edx
        pop     ecx
        pop     ebx
        ret
endif
endproc RetR8

defp    RetSysR8
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        movsd                           ; ...
        hop     regs0
endproc RetSysR8

defp    RetR16
if _MATH eq _8087
        fld     _TBYTE ptr [esi]
        fwait
        jmp     regs0
else
        pop     edi                     ; set edi to point to storage for
        push    edi                     ; ... structured return
        movsd                           ; copy return value
        movsd                           ; ...
        movsw                           ; ...
        hop     regs0
endif
endproc RetR16

defp    RetSysR16
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        movsd                           ; ...
        movsw                           ; ...
        hop     regs0
endproc RetSysR16

defp    RetC8
        pop     edi                     ; set edi to point to storage for
        push    edi                     ; ... structured return
        movsd                           ; copy return value
        movsd                           ; ...
        hop     regs0
endproc RetC8

defp    RetSysC8
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        movsd                           ; ...
        hop     regs0
endproc RetSysC8

defp    RetC16
        pop     edi                     ; set edi to point to storage for
        push    edi                     ; ... structured return
        movsd                           ; copy return value
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        hop     regs0
endproc RetC16

defp    RetSysC16
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        hop     regs0
endproc RetSysC16

defp    RetC32
        pop     edi                     ; set edi to point to storage for
        push    edi                     ; ... structured return
        movsd                           ; copy return value
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        hop     regs0
endproc RetC32

defp    RetSysC32
        ; edi already points to destination
        mov     eax, edi                ; return pointer to result
        movsd                           ; copy return value
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        movsd                           ; ...
        hop     regs0
endproc RetSysC32

defp    RetChr
        mov     eax,edi
        hop     regs0
endproc RetChr

defp    RetStruct
        mov     eax,[esi]               ; get ptr to storage from RCB
        mov     eax,[eax]               ; and load first dword of storage
                                        ; to handle 1,2,4 byte structs
        hop     regs0
endproc RetStruct

defp    RetSysStruct
        mov     eax,[esi]               ; get ptr to storage from RCB
        hop     regs0
endproc RetSysStruct

        xdefp   Ext_To_WF77
defp    Ext_To_WF77
        pop     WFAddr                  ; get address of WATFOR-77 subprogram
        pop     ArgList                 ; get address of WATFOR-77 argument list
        pop     RetAddr                 ; get return addr of external subpgm
        push    eax                     ; save eax,
        mov     eax, ArgList            ; check if it is a sys call
        mov     al, [eax]               ; ...
        and     al, SYSTEM_CALL         ; ...
if _MATH eq _8087
          fsave Save80x87               ; save state of 80x87
          exit_fpu                      ; set up for WFC
endif
        _if     e
          pop   eax                     ; restore eax
          push  ecx                     ; stack arguments in registers
          push  ebx                     ; ...
          push  edx                     ; ...
          push  eax                     ; ...
          push  offset32 RetSCB         ; push address of SCB for return value
          push  ArgList                 ; restore address of arg list
          call  WFC_to_WF77             ; process argument list
          cmp   eax,010h                ; check for # of args
          _if   l                       ; if less than 4 args
            mov eax,010h                ; - make it 4
          _endif                        ; endif
        _else
          pop   eax                     ; restore eax
          push  offset32 RetSCB         ; push address of SCB for return value
          push  ArgList                 ; restore address of arg list
          call  SYSCALL_to_WF77         ; process sys_call arg list
          ; if eax contains amount of stack we must pop off, if this is a
          ; true sys call ( instead of pascal ), then eax should be 0
        _endif
if _MATH eq _8087
        frstor  Save80x87               ; ...
endif
        push    RetAddr                 ; restore return addr of external subpgm
        push    eax                     ; save size of stack to pop in regs0
        push    ebx                     ; save registers
        push    ecx                     ; ...
        push    edx                     ; ...
        push    ebp                     ; ...
        push    edi                     ; ...
        push    esi                     ; must be saved last
        mov     edi,RetSCB              ; get addr of possible return SCB
if _MATH eq _8087
        sub     esp,108                 ; save 80387 environment
        fsave   [esp]                   ; ...
endif
        mov     ebx,ArgList             ; get ptr to arg list
        push    edi                     ; save pointer to SCB
        mov     bl,[ebx]                ; get return type
        mov     bh, bl                  ; save for later test
        push    ArgList                 ; put address of argument list on (in case of recursion)
        and     bl,TYPE_MASK            ; get return type
        and     bh, SYSTEM_CALL         ; is it a system call
        _if     ne                      ; if it is
          jmp   Fini_Sys_Call           ; finish system call
        _endif
;;;;;;;;;;hop   Fini_WFC_Call           ; fini normal call
endproc Ext_To_WF77

defp    Fini_WFC_Call
        _guess
          cmp   bl,PT_CHAR              ; - see if character function
          _quif ne
          mov   ecx,4[edi]              ; - get length in ecx
          mov   edi,[edi]               ; - get data pointer in edi
        _admit
          cmp   bl,PT_STRUCT            ; - see if structure function
          _quif ne
          mov   edi,esi                 ; - get ptr to structure storage
        _endif
        call    [WFAddr]                ; call WATFOR-77 subprogram
        pop     ebx                     ; get address of argument list
if _MATH eq _8087
        frstor  4[esp]                  ; restore 80387 environ
endif
        movzx   ebx,byte ptr [ebx]      ; get return type
        pop     edi                     ; get address of possible return SCB
        and     bl,TYPE_MASK            ; finish getting ret type
if _MATH eq _8087
        fwait                           ; wait for FPU before removing environ
        add     esp,108                 ; ...
endif
        jmp     ProcRet[ebx*4]          ; process return value
endproc Fini_WFC_Call

defp    Fini_Sys_Call
        cmp     bl,PT_CHAR              ; - see if character function
        _if     e
          mov   ecx,4[edi]              ; - get length in ecx
          mov   edi,[edi]               ; - get data pointer in edi
        _endif
        call    [WFAddr]                ; call WATFOR-77 subprogram
        pop     ebx                     ; get address of argument list
if _MATH eq _8087
        frstor  4[esp]                  ; restore 80387 environ
endif
        mov     ax, 2[ebx]              ; get call type
        and     ax, TY_CALL_MASK        ; ...
        movzx   ebx,byte ptr [ebx]      ; get return type
        pop     edi                     ; get address of possible return SCB
        and     bl,TYPE_MASK            ; finish getting ret type
if _MATH eq _8087
        fwait                           ; wait for FPU before removing environ
        add     esp,108                 ; ...
endif
        cmp     ax, TY_SYS_CALL         ; is not system call ?
        _if     ne                      ; use different jump table
          jmp   ProcStdRet[ebx*4]       ; process return value
        _endif
        jmp     ProcSysRet[ebx*4]       ; process return value
endproc Fini_Sys_Call

        fmodend
        end
