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
;   DATAINIT    : create the definitions for the data initializer segments
;

;DGROUP group   _DATA_INIT_B,_DATA_INIT,_DATA_INIT_E,_CB_DEFN_B,_CB_DEFN,_CB_DEFN_E

;
; this triplet surrounds the global data area
;
;_WFDATA_B      segment public para 'FGLOBAL'
;               public __wfdata_start
;__wfdata_start label byte
;_WFDATA_B      ends
;
;_WFDATA                segment public para 'FGLOBAL'
;_WFDATA                ends
;
;_WFDATA_E      segment public byte 'FGLOBAL'
;               public __wfdata_end
;__wfdata_end   label byte
;_WFDATA_E      ends
;
;;
; this triplet forms an array of pointers to DataChains for each WATFOR file
;
_DATA_INIT_B    segment public dword 'DATA'
                public __data_init_start
__data_init_start       label byte
_DATA_INIT_B    ends

_DATA_INIT      segment public dword 'DATA'
_DATA_INIT      ends

_DATA_INIT_E    segment public dword 'DATA'
                public __data_init_end
__data_init_end label byte
_DATA_INIT_E    ends

;
; this triplet forms an array of pointers to COMMON blocks and their sizes
;
_CB_DEFN_B      segment public dword 'DATA'
                public __cb_defn_start
__cb_defn_start label byte
_CB_DEFN_B      ends

_CB_DEFN        segment public dword 'DATA'
_CB_DEFN        ends

_CB_DEFN_E      segment public dword 'DATA'
                public __cb_defn_end
__cb_defn_end   label byte
_CB_DEFN_E      ends

                end
