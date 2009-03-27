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

.386p

                name            realmode

.data

extrn           "C", Meg1 : dword

.code

public          "C", GetDosLong
GetDosLong      proc near
ifdef __STACK__
                mov     eax,4[esp]
endif
                push    fs
                mov     fs,Meg1
                mov     eax,dword ptr fs:[eax]
                pop     fs
                ret
GetDosLong      endp

public          "C", GetDosByte
GetDosByte      proc near
ifdef __STACK__
                mov     eax,4[esp]
endif
                push    fs
                mov     fs,Meg1
                movzx   eax,byte ptr fs:[eax]
                pop     fs
                ret
GetDosByte      endp

public          "C", PutDosByte
PutDosByte      proc near
ifdef __STACK__
                push    edx
                mov     eax,8[esp]
                mov     edx,12[esp]
endif
                push    fs
                mov     fs,Meg1
                mov     byte ptr fs:[eax],dl
                pop     fs
ifdef __STACK__
                pop     edx
endif
                ret
PutDosByte      endp

public          "C", PutDosLong
PutDosLong      proc near
ifdef __STACK__
                push    edx
                mov     eax,8[esp]
                mov     edx,12[esp]
endif
                push    fs
                mov     fs,Meg1
                mov     fs:[eax],edx
                pop     fs
ifdef __STACK__
                pop     edx
endif
                ret
PutDosLong      endp

ifndef CAUSEWAY

public          "C", CallRealMode
CallRealMode    proc near
ifdef __STACK__
                mov     eax,4[esp]
endif
                push    ebx
                push    ecx
                push    edx
                push    esi
                push    edi
                push    ebp
                mov     ebx,eax
                xor     ecx,ecx
                mov     ax,250eH
                int     21H
                pop     ebp
                pop     edi
                pop     esi
                pop     edx
                pop     ecx
                pop     ebx
                ret
CallRealMode    endp

endif

end
