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
;   These definitions correspond to those in structio.h
;
SDEFN_ARRAY     equ     80h     ; structure field is an array
SDEFN_UNION     equ     40h     ; structure field is an union
SDEFN_TYPE      equ     0fh     ; type bitfield

ifndef __386__
sdefn_header    struc
    sd_total_size       dw      ?       ; total size of structure
    sd_back_link        dw      ?       ; for stack when doing i/o
    sd_back_offs        dw      ?       ; ...
sdefn_header    ends
else
sdefn_header    struc
    sd_total_size       dd      ?       ; total size of structure
    sd_back_link        dd      ?       ; for stack when doing i/o
    sd_back_offs        dd      ?       ; ...
sdefn_header    ends
endif

ifndef __386__
sio_cntrl_blk   struc
    sio_origin          dd      ?       ; pointer to storage
    sio_cur_sdefn       dw      ?       ; current structure definition
    sio_cur_offs        dw      ?       ; offset into current structure defn
sio_cntrl_blk   ends
else
sio_cntrl_blk   struc
    sio_origin          dd      ?       ; pointer to storage
    sio_cur_sdefn       dd      ?       ; current structure definition
    sio_cur_offs        dd      ?       ; offset into current structure defn
sio_cntrl_blk   ends
endif
