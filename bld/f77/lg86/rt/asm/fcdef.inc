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


include struct.inc
include mdef.inc

; change the dword to tbyte when long doubles are implmented
; ALSO change offsets of the precision bytes below //UNCOMMENT_TOKEN
_TBYTE equ qword        ; change to tbyte

ifdef __386__
.386
if _MATH eq _8087
.387
endif
                extrn   LG@FC_TABLE : dword
else
                extrn   LG@FC_TABLE : word
endif

fmodstart       macro   modname,alignment
ifdef __386__
FCODE_TEXT      segment alignment public use32 'CODE'
ifdef _MASM
                assume  cs:FLAT
else
                assume  cs:FCODE_TEXT
endif
else
                extrn   LG@SaveSeg : word
                SaveSeg equ LG@SaveSeg
FCODE_TEXT      segment alignment public 'CODE'
                assume  cs:FCODE_TEXT
endif
                endm


fmodend         macro
FCODE_TEXT      ends
                endm


; declare an fcode at this point in the code
dfcode          macro   sym
                public  FC@&sym
                FC@&sym:
                sym:
                endm

fcode           macro   sym
                public  FC@&sym
        FC@&sym proc    near
        sym:
                endm

efcode          macro   sym
        FC@&sym endp
                endm

fcxref          macro   sym
                extrn   FC@&sym : near
                sym EQU FC@&sym
                endm


; declare a debugger fcode
dbfcode         macro   sym
    if _SA_LIBRARY
                public  DB@&sym
        DB@&sym proc    near
        sym:
    else
                public  FC@&sym
        FC@&sym proc    near
        sym:
    endif
                endm


edbfcode        macro   sym
    if _SA_LIBRARY
        DB@&sym endp
    else
        FC@&sym endp
    endif
                endm

ifxref          macro   sym
                extrn   IF@&sym : near
                sym EQU IF@&sym
                endm


rtxref          macro   sym
                extrn   RT@&sym : near
                sym EQU RT@&sym
                endm

lgxref          macro   sym
                extrn   LG@&sym : near
                sym EQU LG@&sym
                endm

ifdef __WASM__
lgxdefp         macro   sym
                public  LG@&sym
                sym EQU LG@&sym
                endm
else
lgxdefp         macro   sym
                public  LG@&sym
                sym EQU <LG@&sym>
                endm
endif

hop             macro   dest
                jmp     short dest
                endm

ifndef __386__
getword         macro   dest
                lodsw
                ifdif   <dest>,<ax>
                  ifdif   <dest>,<AX>
                    mov     dest,AX
                  endif
                endif
                endm

else

getword         macro   dest
                ifdif   <dest>,<ax>
                    mov dest,word ptr [esi]
                    add esi,2
                else
                    lodsw
                endif
                endm

getdw           macro   dest
                ifdif   <dest>,<eax>
                    mov dest,dword ptr [esi]
                    ifdif <dest>,<esi>
                        add     esi,4
                    endif
                else
                    lodsd
                endif
                endm
endif


next            macro   p,w
ifndef __386__
                lodsw
                ifnb    <w>
                  _fwait
                endif
                ifnb    <p>
                  ife p
                    mov sp,bp
                  else
                    if  p EQ 2
                        pop     bp
                    else
                        add sp,p
                    endif
                  endif
                endif
                mov     bx, ax
                jmp     cs:LG@FC_TABLE[bx]
else
                ifnb    <w>
                  _fwait
                endif
                ifnb    <p>
                  ife p
                    mov esp,ebp
                  else
                    if  p EQ 4
                        pop     eax
                    else
                      if p EQ 8
                        pop     eax
                        pop     ebx
                      else
                        add     esp,p
                      endif
                    endif
                  endif
                endif
                movzx   eax,word ptr [esi]
                lea     esi,2[esi]      ; we must preserve flags
                jmp     LG@FC_TABLE[eax]
endif
                endm


RTErr           macro   err_code
ifdef __386__
                mov     eax,err_code
                jmp     RTError
else
                mov     ax,err_code
                jmp     RTError
endif
                endm


exit_fcode      macro
ifndef __386__
                mov     SS:SaveSeg,DS
endif
                endm


enter_fcode     macro
ifndef __386__
                mov     DS,SS:SaveSeg
endif
                endm


_fwait          macro
                if _MATH eq _8087
                    fwait
                endif
                endm

; The following macros were used when the floating-point model had a cache
;
;enter_fpu      macro
;               if _MATH eq _8087
;                   fstp    ST
;                   fstp    ST
;                   fstp    ST
;                   fstp    ST
;               endif
;               endm
;
;
;; this one is used to pipeline the cpu and fpu instructions
;enter_fcode_fpu        macro
;               if _MATH eq _8087
;                   fstp    ST
;               endif
;ifndef __386__
;               mov     DS,SS:SaveSeg
;endif
;               if _MATH eq _8087
;                   fstp    ST
;                   fstp    ST
;                   fstp    ST
;               endif
;               endm
;
;
;; this one is used to pipeline the cpu and fpu actions
;exit_fcode_fpu macro   stkdepth
;               if _MATH ne _8087
;                   exit_fcode
;               else
;ifdef __386__
;                   exit_fpu stkdepth
;else
;                   ifb <stkdepth>
;                       fldz
;                       mov     SS:SaveSeg,DS
;                       fldz
;                       push    SS
;                       fldz
;                       pop     DS
;                       fldz
;                   else
;                       if stkdepth LT 4
;                           fldz
;                       endif
;                       mov         SS:SaveSeg,DS
;                       if stkdepth LT 3
;                           fldz
;                       endif
;                       push    SS
;                       if stkdepth LT 2
;                           fldz
;                       endif
;                       pop         DS
;                       if stkdepth LT 1
;                           fldz
;                       endif
;                   endif
;endif
;               endif
;               endm
;
;
;exit_fpu       macro   stkdepth
;               if _MATH eq _8087
;                   ifb <stkdepth>
;                       fldz
;                       fldz
;                       fldz
;                       fldz
;                   else
;                       if stkdepth LT 4
;                           fldz
;                       endif
;                       if stkdepth LT 3
;                           fldz
;                       endif
;                       if stkdepth LT 2
;                           fldz
;                       endif
;                       if stkdepth LT 1
;                           fldz
;                       endif
;                   endif
;               endif
;               endm

; The following macros are used with the new floating-point model (no cache)
;
enter_fpu       macro
                endm


exit_fpu        macro   stkdepth
                endm


; this one is used to pipeline the cpu and fpu instructions
enter_fcode_fpu macro
                enter_fcode
                endm


; this one is used to pipeline the cpu and fpu actions
exit_fcode_fpu  macro   stkdepth
                exit_fcode
                endm
