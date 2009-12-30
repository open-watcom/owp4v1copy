/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  DOS LFN specific constant definitions.
*
****************************************************************************/


#define DOS_GET_DTA         0x2F
#define DOS_CREATE_NEW      0x5B

#define EX_LFN_OPEN         0x00
#define EX_LFN_CREATE       0x12
#define EX_LFN_CREATE_NEW   0x10

#define PUSH_BX             0x53
#define PUSH_SI             0x56
#define POP_BX              0x5B
#define POP_SI              0x5E

#define _XCHG_AX_DX         0x92
#define _XCHG_AX_SI         0x96

#ifdef __BIG_DATA__
    #define _RST_DS     _POP_DS
    #define _SET_DSDX   _PUSH_DS _XCHG_AX_DX _MOV_DS_AX
    #define _SET_DSSI   _PUSH_DS _XCHG_AX_SI _MOV_DS_AX
#else
    #define _RST_DS
    #define _SET_DSDX
    #define _SET_DSSI
#endif

#define LFN_CREATE_EX   \
        "mov ax,716Ch"  \
        "stc"           \
        "int  21h"

#define LFN_FIND_FIRST  \
        PUSH_SI         \
        "mov  si,1"     \
        "mov  ax,714Eh" \
        "stc"           \
        "int 21h"       \
        POP_SI

#define LFN_FIND_NEXT   \
        PUSH_SI         \
        "mov  si,1"     \
        "mov  ax,714fh" \
        "stc"           \
        "int 21h"       \
        POP_SI

#define LFN_FIND_CLOSE  \
        "mov  ax,71A1h" \
        "stc"           \
        "int  21h"

#define LFN_GET_FILE_ATTR \
        PUSH_BX         \
        "mov  bl,0"     \
        "mov  ax,7143h" \
        "stc"           \
        "int  21h"      \
        POP_BX

#define LFN_SET_FILE_ATTR \
        PUSH_BX         \
        "mov  bl,1"     \
        "mov  ax,7143h" \
        "stc"           \
        "int  21h"      \
        POP_BX

#define LFN_DOS_RENAME  \
        "mov  ax,7156h" \
        "stc"           \
        "int  21h"

#define LFN_DOS_UNLINK  \
        PUSH_SI         \
        "mov  si,0"     \
        "mov  ax,7141h" \
        "stc"           \
        "int  21h"      \
        POP_SI

#define MOV_DTA         \
        "mov  ecx,43"   \
        "rep movsb"

#define MOV_DATA_TO_DTA \
        "mov  esi,edx"  \
        "mov  edi,ebx"  \
        MOV_DTA

#define MOV_DATA_FROM_DTA \
        "mov  esi,ebx"  \
        "mov  edi,edx"  \
        "mov  ebx,ds"   \
        "push es"       \
        "pop  ds"       \
        "mov  es,ebx"   \
        MOV_DTA         \
        "mov  ds,ebx"

#define RETURN_VALUE    \
        "jc short L1"   \
        SAVE_VALUE      \
"L1:"

#define RETURN_VALUE_EX \
        "jc short L1"   \
        SAVE_VALUE_EX   \
"L1:"

#define IS_LFN_ERROR(x)     ((x)!=0&&(x)!= 0x7100)

#define HANDLE_OF(x)        ((x)->lfnax)


