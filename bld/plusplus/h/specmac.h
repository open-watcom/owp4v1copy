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
* Description:  Special macros that can't be undefined or redefined.
*
****************************************************************************/


pick( "__LINE__",       MACRO_LINE,        0 )
pick( "__FILE__",       MACRO_FILE,        0 )
pick( "__DATE__",       MACRO_DATE,        0 )
pick( "__TIME__",       MACRO_TIME,        0 )
pick( "__cplusplus",    MACRO_CPLUSPLUS,   0 )
pick( "__FUNCTION__",   MACRO_FUNCTION,    MACRO_CAN_BE_REDEFINED )

// __func__ shouldn't be a macro, define it special for now
pick( "__func__",       MACRO_FUNC,        MACRO_CAN_BE_REDEFINED | MACRO_SPECIAL )

// alternative tokens for C++
pick( NULL,             MACRO_ALT_MARKER,  0 )
pick( "and",            MACRO_ALT_AND,     MACRO_SPECIAL )
pick( "bitand",         MACRO_ALT_BITAND,  MACRO_SPECIAL )
pick( "and_eq",         MACRO_ALT_AND_EQ,  MACRO_SPECIAL )
pick( "or",             MACRO_ALT_OR,      MACRO_SPECIAL )
pick( "bitor",          MACRO_ALT_BITOR,   MACRO_SPECIAL )
pick( "or_eq",          MACRO_ALT_OR_EQ,   MACRO_SPECIAL )
pick( "xor",            MACRO_ALT_XOR,     MACRO_SPECIAL )
pick( "xor_eq",         MACRO_ALT_XOR_EQ,  MACRO_SPECIAL )
pick( "not",            MACRO_ALT_NOT,     MACRO_SPECIAL )
pick( "not_eq",         MACRO_ALT_NOT_EQ,  MACRO_SPECIAL )
pick( "compl",          MACRO_ALT_COMPL,   MACRO_SPECIAL )
#undef pick
