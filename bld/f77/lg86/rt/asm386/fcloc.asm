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
; FCLOC:        the LOC intrinsic function
;

include fcdef.inc
include parmtype.inc

        fmodstart fcloc

fcode   LOCATION_OF
        _guess
          getword dx            ; get parm code/type
          getdw eax             ; get offset
          mov   ecx,edx         ; another copy of flags
          and   dx,CODE_MASK * 256 + TYPE_MASK
          add   eax,ebp         ; convert offset into ptr
          cmp   dh,PC_VARIABLE  ; - if it's not a variable
          _quif ne              ; - do indirect
          cmp   dl,PT_CHAR      ; - if it's a character variable
          _quif e               ; - do indirect
          test  ch,IND_PARM     ; - if it's an indirect parm
          _quif ne              ; - do indirect
          push  eax             ; - push offset
        _admit
          push  [eax]           ; - push indirect offset
        _endguess
        next
efcode  LOCATION_OF

        fmodend
        end
