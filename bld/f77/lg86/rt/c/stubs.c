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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "ftnstd.h"

#pragma aux __STACKLOW "*";
void    *__STACKLOW = { NULL };

void    SysExit( void );
#pragma aux SysExit =                                   \
        0xb8 0x00 0x4c  /* mov ax,0x4c00 */             \
        0xcd 0x21       /* int 0x21 */                  \
        aborts                                          \
        modify [ax];

void __exit_with_msg( char far *msg, int exit_code ) {

    msg = msg;
    exit_code = exit_code;
    SysExit();
}


void __exit() {
    SysExit();
}

void RTErr() {
    SysExit();
}

void __math2err( unsigned int err_info, double *arg1, double *arg2 ) {

    err_info = err_info;
    arg1 = arg1;
    arg2 = arg2;
    SysExit();
}

void __math1err( unsigned int err_info, double *arg1 ) {

    err_info = err_info;
    arg1 = arg1;
    SysExit();
}

void __setEFGfmt( void ) {
}

#if _8087 == _ON

#pragma aux __init_80x87 "*";
void __init_80x87( void ) {
}

#endif
