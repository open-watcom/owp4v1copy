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
// CMDPARMS  : set command line parameters
//

#include <i86.h>
#include <string.h>

#include "ftnstd.h"

#if _OPSYS == _PCDOS

#define PSP_CMD_AREA    0x80    // offset in PSP of DOS command area

extern unsigned_16      _psp;


extern void SetCmdParms( char * ptr, unsigned int len ) {
//=======================================================

//  Set command line parameters for WATFOR-77 program into the DOS
//  command area in the PSP.

    if( len > 127 ) {
        len = 127;
    }
    *(unsigned char __far *)MK_FP( _psp, PSP_CMD_AREA ) = len;
    _fmemcpy( MK_FP( _psp, PSP_CMD_AREA + 1 ), ptr, len );
}

#elif _OPSYS == _OS2

extern  char            *_LpCmdLine;    /* pointer to command line */

extern void SetCmdParms( char * ptr, unsigned int len ) {
//=======================================================

    len = len;
    _LpCmdLine = ptr;
}

#else

extern void SetCmdParms( char * ptr, unsigned int len ) {
//=======================================================
    ptr = ptr;
    len = len;
}

#endif
