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
;       Length of an fcode
;
FC_LEN          equ     2

;
; Subprogram headers:
;
ifndef __386__
WF_SEQUENCE     equ     9a2eH   ; WATFOR-77 subprograms "seg CS, far call"
else
WF_SEQUENCE     equ     0e82eH  ; WATFOR-77 subprograms "seg CS, near call"
endif

;
; These constants map the reserved words at the start of subprogram.
;
;
; sp_entry struct - header information for every entry point in a
;                   subprogram. Changes to this struct should be
;                   reflected in ENTRY H.
;
ifndef  __386__
sp_entry_s      struc
    fcode_enable dw     ?       ; WF_SEQUENCE
    fcode_enaddr dd     ?       ; entry point when called externally
    darg_list   dw      ?       ; pointer to dummy argument list
    epi_fcode   dw      ?       ; pointer to epilogue F-Code
    tb_struct   dw      ?       ; pointer to traceback struct
    en_name     dw      ?       ; pointer to entry name
    sp_start    dw      ?       ; start of subprogram
sp_entry_s      ends
else
sp_entry_s      struc
    fcode_enable dw     ?       ; WF_SEQUENCE
    fcode_enaddr dd     ?       ; entry point when called externally
    darg_list   dd      ?       ; pointer to dummy argument list
    epi_fcode   dd      ?       ; code label of epilogue F-Code
    tb_struct   dd      ?       ; code label of traceback struct
    en_name     dd      ?       ; pointer to entry name
    sp_start    dd      ?       ; start of subprogram
sp_entry_s      ends
endif

sp_hdr_id       equ     darg_list; length of subprogram header id

;
; traceback struct - also used for other information since ExCurr points
;                    here at run-time hence we know where to find useful
;                    information. Changes to this struct should be
;                    reflected in TRCBACK H.
;
ifndef __386__
traceback_s     struc
    link_tb     dd      ?       ; pointer to previous traceback
    line_tb     dw      ?       ; indicates it is a load and go traceback
    name_tb     dw      ?       ; offset of subprogram name
    lineptr_tb  dw      ?       ; offset of previous line
    epi_tb      dw      ?       ; offset of epilogue sequence so debugger
                                ; can get function value
    lvar_tb     dw      ?       ; offset of local data area
    fc_call     dw      ?       ; for calling F-Codes from debugger
traceback_s     ends
TB_LG           equ     0ffffh
TB_LG_DB        equ     0fffeh
else
traceback_s     struc
    link_tb     dd      ?       ; pointer to previous traceback
    line_tb     dd      ?       ; indicates it is a load and go traceback
    name_tb     dd      ?       ; offset of subprogram name
    lineptr_tb  dd      ?       ; offset of previous line
    epi_tb      dd      ?       ; offset of epilogue sequence so debugger
                                ; can get function value
    lvar_tb     dd      ?       ; offset of local data area
    fc_call     dd      ?       ; for calling F-Codes from debugger
traceback_s     ends
TB_LG           equ     0ffffffffh
TB_LG_DB        equ     0fffffffeh
endif


;
; This area is pointed to by EPI_FCODE and EPI_TB.
;
ifndef __386__
epi_fcode_s     struc
    fcode_epi   dw      ?       ; epilogue F-Code
    arg_list    dw      ?       ; pointer to actual argument list
    ret_addr    dd      ?       ; return address
    func_val    dw      8 dup(?); function value
epi_fcode_s     ends
else
epi_fcode_s     struc
    fcode_epi   dw      ?       ; epilogue F-Code
    arg_list    dd      ?       ; pointer to actual argument list
    ret_addr    dd      ?       ; return address
    ret_sp_data dd      ?       ; save data seg ptr
    my_sp_data  dd      ?       ; my data seg ptr
    func_val    dd      4 dup(?); function value
epi_fcode_s     ends
endif

;
; This defines the header for statement functions and is at the top of
; the statement function.
; Changes to this struct should be reflected in TRCBACK H.
;
ifndef __386__
sf_s    struc
    sf_ret      dw      ?       ; return address
    sf_link     dd      ?       ;   link to previous trace back
    sf_line     dw      ?       ;   indicates it's lg traceback
    sf_name     dw      ?       ;   statement function name
    sf_lineptr  dw      ?       ;   previous line
    sf_strt     dw      ?       ; start of statement function
sf_s    ends
else
sf_s    struc
    sf_ret      dd      ?       ; return address
    sf_link     dd      ?       ;   link to previous trace back
    sf_line     dd      ?       ;   indicates it's lg traceback
    sf_name     dd      ?       ;   statement function name
    sf_lineptr  dd      ?       ;   previous line
    sf_strt     dd      ?       ; start of statement function
sf_s    ends
endif

sf_tb           equ     sf_link ; statement function trace back struct


;
; This defines the data structure used to pass an array element as a parm.
;
ifndef __386__
arr_elt_parm_s  struc
    elt_addr    dd      ?       ; pointer to element
    elt_size    dw      ?       ; size of the element
    elt_adv     dw      ?       ; offset of the array ADV this is an element of
arr_elt_parm_s  ends
else
arr_elt_parm_s  struc
    elt_addr    dd      ?       ; pointer to element
    elt_size    dd      ?       ; size of the element
    elt_adv     dd      ?       ; offset of the array ADV this is an element of
arr_elt_parm_s  ends
endif

