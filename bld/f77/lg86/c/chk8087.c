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


//
// CHK8087      : 8087 support
//

#include "ftnstd.h"
#include "cioconst.h"
#include "errcod.h"

#include <io.h>
#include <string.h>

extern  void            MsgBuffer(uint,char *,...);

#ifdef _8087
#undef _8087
#endif

extern int _8087;


bool    Chk8087() {
//=================

    char        buff[LIST_BUFF_SIZE+1];

    if( _8087 == 0 ) {
        MsgBuffer( MS_NO_8087, buff );
        write( STDERR_FILENO, buff, strlen( buff ) );
        return( FALSE );
    }
    return( TRUE );
}


void    InitMath() {
//==================

}


void    FiniMath() {
//==================

}


#pragma aux Init8087 "*";

void    Init8087() {
//==================

}
