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


;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<>     FORTRAN-77 run_time support for 8086 based machines
;<>
;<>     F@IPGM - IBM professional fortran IO initialization
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
TITLE           PFINIT

EXTRN           F@IOFRE:NEAR
EXTRN           F@IOFRS:NEAR
EXTRN           F@OSDU:NEAR
EXTRN           F@OSIN:NEAR
EXTRN           F@STW:NEAR
EXTRN           F@OSSSN:NEAR

;
; F@IOCOM - IO common block data
;

F@IOCOM         SEGMENT PARA COMMON
                ORG     00H
                PUBLIC  F@DBGF
                PUBLIC  F@DBAT
                PUBLIC  F@DADR

                ORG     005eH

                DB      0ffH,0ffH,0ffH,0ffH,66H,6fH,72H,74H
                DB      0cH,0dH,0aH,0aH,0dH

                ORG     0070H

                DB      5fH,08H
                DW      F@IOCOM
                DB      67H,00H
                DW      F@IOCOM
                DB      1bH,00H
                DW      F@IOCOM
                DB      5eH,00H
                DW      F@IOCOM
                DB      56H,02H
                DW      F@IOCOM
                DB      62H,00H
                DW      F@IOCOM
                DB      7eH,02H
                DW      F@IOCOM
                DB      66H,00H
                DW      F@IOCOM

                ORG     0094H

                DB      32H,00H
                DW      F@IOCOM
                DB      69H,00H
                DW      F@IOCOM
                DB      68H,00H
                DW      F@IOCOM
                DB      3eH,00H
                DW      F@IOCOM

                ORG     00b0H

                DB      46H,00H
                DW      F@IOCOM
                DB      0a6H,02H
                DW      F@IOCOM
                DB      16H,08H
                DW      F@IOCOM
                DB      5cH,72H,6dH,66H,72H,74H,5cH,65H
                DB      72H,72H,00H

                ORG     0c8H

                DB      0ccH,00H
                DW      F@IOCOM
                DB      0dH,0aH,45H,78H,65H,63H,75H,74H
                DB      69H,6fH,6eH,20H,74H,65H,72H,6dH
                DB      69H,6eH,61H,74H,65H,64H,20H,62H
                DB      65H,63H,61H,75H,73H,65H,20H,6fH
                DB      66H,20H,43H,54H,52H,4cH,2dH,42H
                DB      52H,45H,41H,4bH,0dH,0aH,0feH,00H
                DW      F@IOCOM

                ORG     014EH

                DB      56H,01H
                DW      F@IOCOM
                DB      5eH,01H
                DW      F@IOCOM
                DB      0dH,0aH,20H,20H,20H,20H,20H,20H
                DB      0dH,0aH,45H,72H,72H,6fH,72H,20H
                DB      69H,6eH,20H,72H,65H,61H,64H,69H
                DB      6eH,67H,20H,65H,72H,72H,6fH,72H
                DB      20H,6dH,65H,73H,73H,61H,67H,65H
                DB      73H,20H,66H,69H,6cH,65H,0dH,0aH
                DB      8aH,01H
                DW      F@IOCOM
                DB      0dH,0aH,45H,78H,65H,63H,75H,74H
                DB      69H,6fH,6eH,20H,73H,75H,73H,70H
                DB      65H,6eH,64H,65H,64H,20H,3aH,20H
                DB      0aaH,01H
                DW      F@IOCOM
                DB      0c6H,01H
                DW      F@IOCOM
                DB      0dH,0aH,43H,6fH,6dH,70H,69H,6cH
                DB      61H,74H,69H,6fH,6eH,20H,65H,72H
                DB      72H,6fH,72H,20H,61H,74H,20H,6cH
                DB      69H,6eH,65H,20H,20H,69H,6eH,20H
                DB      0d2H,01H
                DW      F@IOCOM
                DB      0e2H,01H
                DW      F@IOCOM
                DB      0dH,0aH,45H,72H,72H,6fH,72H,20H
                DB      61H,74H,20H,6cH,69H,6eH,65H,20H
                DB      20H,20H,45H,72H,72H,6fH,72H,20H
                DB      6eH,75H,6dH,62H,65H,72H,20H

                ORG     01F2H

                DB      0f6H,01H
                DW      F@IOCOM
                DB      0dH,0aH,45H,78H,65H,63H,75H,74H
                DB      69H,6fH,6eH,20H,74H,65H,72H,6dH
                DB      69H,6eH,61H,74H,65H,64H,20H,3aH
                DB      20H

                ORG     0210H

                DB      14H,02H
                DW      F@IOCOM
                DB      0dH,0aH,45H,72H,72H,6fH,72H,20H
                DB      69H,6eH,20H

                ORG     0220H

                DB      28H,02H
                DW      F@IOCOM
                DB      32H,02H
                DW      F@IOCOM
                DB      43H,61H,6cH,6cH,65H,64H,20H,61H
                DB      74H,20H,20H,2bH,20H,30H,78H

                ORG     023EH

                DB      46H,02H
                DW      F@IOCOM
                DB      46H,02H
                DW      F@IOCOM
                DB      00H,20H,20H,20H,20H,20H,20H,20H
                DB      20H,20H,20H,20H,00H,00H,00H,00H

                ORG     02A6H

                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH
                DB      0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH,0ffH

                ORG     0320H

                DB      0bfH,13H,0bfH,1fH

                ORG     04B8H

                DW      F@IOCOM

                ORG     04BCH

                DW      F@IOCOM
                DB      0b6H,04H

                ORG     05B2H

                DB      0caH,05H
                DW      F@IOCOM

                ORG     05B8H

                DW      F@IOCOM
                DB      0caH,06H
                DW      F@IOCOM
                DB      0c6H,03H
                DW      F@IOCOM
                DB      02H,07H
                DW      F@IOCOM

                ORG     05C8H

                DW      F@IOCOM

                ORG     06D4H

                DB      00H,00H,20H,41H,0aH,00H
                DB      00H,00H,8aH,5dH,78H,45H,63H,01H
                DB      00H,00H,64H,0a7H,0b3H,0b6H,0e0H,0dH
                DB      9aH,99H,99H,99H,99H,99H,0b9H,3fH
                DB      0cdH,0ccH,0ccH,3dH,00H,00H,00H,00H
                DB      80H,84H,2eH,41H,00H,24H,74H,49H

                ORG     07DCH

                DB      43H,4fH,4dH,53H,50H,45H
                DB      43H

                ORG     07E4H

                DB      0dcH,07H
                DW      F@IOCOM
                DB      63H,3aH,5cH,72H,6dH,66H,6fH,72H
                DB      74H,2eH,6dH,73H,67H,00H

                ORG     080AH

                DB      0dH,0aH,0dH,0aH,0cH,08H
                DW      F@IOCOM
                DB      0aH,08H
                DW      F@IOCOM
                DB      70H,72H,6fH,66H,6fH,72H,74H,2eH
                DB      65H,72H,72H,20H,20H,20H,00H

                ORG     0826H

F@DBGF          EQU     $
                DB      00H
F@DBAT          EQU     $
                DB      00H
F@DADR          EQU     $
                DB      00H,00H,00H,00H,30H,08H
                DW      F@IOCOM
                DB      0dH,0aH,45H,72H,72H,6fH,72H,20H
                DB      69H,6eH,20H,44H,65H,62H,75H,67H
                DB      2eH,20H,20H,45H,72H,72H,6fH,72H
                DB      20H,6eH,75H,6dH,62H,65H,72H,20H

                ORG     0852H

                DB      34H,00H
F@IOCOM         ENDS

;
; F@IPGM - necessary initialization for Professional Fortran to execute
;
;          ASSUMPTION : address 0EH in the WATFOR77 data seg contains
;                       the address of the PSP

F@RT    segment byte public
        PUBLIC PFIO
        ASSUME CS:F@RT


PFIO            proc    far
                xor     BP,BP                   ; set BP to zero
                mov     AX,0EH[BP]              ; set ES to point to PSP
                mov     ES,AX                   ; ...
                call    F@IOFRS
                xor     BX,BX
                mov     DS,BX
                mov     BYTE PTR +4e6H[BP],00H
                mov     BYTE PTR +4f0H[BP],00H
                mov     AX,31AH[BP]
                mov     2CH[BP],AX
                lds     SI,0B8H[BP]             ; point to errfile name
                mov     CX,0EH                  ; set it's length
                call    F@OSSSN
                jb      CONT
                mov     0B8H[BP],SI
                mov     0BAH[BP],DS
CONT:
;
;   allocate some memory
;
                mov     AX,0400H
                mov     DX,0000H
                mov     +36H[BP],AX
                mov     +38H[BP],DX
                add     AX,0017H
                adc     DX,00H
                mov     CL,04H
                shr     AX,CL
                ror     DX,CL
                add     AX,DX
                mov     BX,AX
                mov     AH,48H
                int     21H
                jb      L2
                mov     WORD PTR +854H[BP],0000H
                mov     +856H[BP],AX
                mov     +85aH[BP],AX
                mov     WORD PTR +90H[BP],0004H
                mov     +92H[BP],AX
                mov     WORD PTR +6cH[BP],0003H
                mov     +6eH[BP],AX
                call    F@OSDU
                mov     DI,4c0H
                mov     AX,SS
                mov     ES,AX
                lds     SI,+12H[BP]
                mov     CX,001eH
                repe    movsb
                jmp     F@IOFRE
L1              EQU     $
                mov     AX,0fa1H
                jmp     SHORT L3
                nop
L2              EQU     $
                mov     AX,+10H[BP]
L3              EQU     $
                mov     +50aH[BP],AX
                jmp     F@STW

PFIO            ENDP
F@RT    ends
                END
