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


_SMALL_CODE     =       00h
_BIG_CODE       =       01h
_SMALL_DATA     =       00h
_BIG_DATA       =       02h
_HUGE_DATA      =       04h
_LONG_INTS      =       08h
_USE_32_SEGS    =       10h

_FLAT           =       ( _SMALL_CODE or _SMALL_DATA or _USE_32_SEGS )
_SMALL          =       ( _SMALL_CODE or _SMALL_DATA )
_COMPACT        =       ( _SMALL_CODE or _BIG_DATA )
_MEDIUM         =       ( _BIG_CODE   or _SMALL_DATA )
_LARGE          =       ( _BIG_CODE   or _BIG_DATA )
_HUGE           =       ( _BIG_CODE   or _HUGE_DATA )

include         ./model.inc     ; defines _MODEL symbol

ifdef _MASM
                offset32 equ <offset FLAT:>
else
                offset32 equ offset
endif

if _MODEL and _BIG_CODE

modstart        macro   modname,alignment
                name    modname
_TEXT           segment alignment public 'CODE'
                assume  cs:_TEXT
                endm

codeptr         macro   sym
                extrn   sym : dword
                endm

docall          macro   dest
                call    far ptr dest
                endm

dojmp           macro   dest
                jmp     far ptr dest
                endm

xdefp           macro   xsym
                public  xsym
                endm

defp            macro   dsym
         dsym   proc    far
                endm

defn            macro   dsym
         dsym   proc    near
                endm

xref            macro   dsym
                extrn   dsym : far
                endm

endproc         macro   dsym
         dsym   endp
                endm

calli           macro   regname
                call    dword ptr [regname]
                endm


else

modstart        macro   modname,alignment
                name    modname
if _MODEL and _USE_32_SEGS
_TEXT           segment use32 alignment public 'CODE'
ifdef _MASM
                assume  cs:FLAT
else
                assume  cs:_TEXT
endif
else
_TEXT           segment alignment public 'CODE'
                assume  cs:_TEXT
endif
                endm

codeptr         macro   sym
                extrn   sym : word
                endm

docall          macro   dest
                call    dest
                endm

dojmp           macro   dest
                jmp     dest
                endm

xdefp           macro   xsym
                public  xsym
                endm

defp            macro   dsym
         dsym   proc    near
                endm

defn            macro   dsym
         dsym   proc    near
                endm

xref            macro   dsym
                extrn   dsym : near
                endm

endproc         macro   dsym
         dsym   endp
                endm

calli           macro   regname
                call    regname
                endm

endif

endmod          macro
_TEXT           ends
                endm

dataseg         macro
DGROUP  group _DATA
assume  ds:DGROUP,ss:DGROUP
if _MODEL and _USE_32_SEGS
_DATA   segment use32 word public 'DATA'
else
_DATA   segment word public 'DATA'
endif
                endm

enddata         macro
_DATA   ends
                endm
