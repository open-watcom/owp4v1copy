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
// DBUTILS      : debugger utility routines
//

#include "ftnstd.h"
#include "rundat.h"
#include "units.h"

extern  void            FindFtnFile(void);
extern  void            ClearEOF(void);


void            StartOutput() {
//=============================

// Switch the debugger IOCB to it's output file

    if( !(IOCB->flags & IOF_OUTPT) ) {
        IOCB->flags |= IOF_OUTPT;
        IOCB->iostmt = IO_WRITE;
        IOCB->unitid = DB_STD_OUTPUT;
        FindFtnFile();
    }
}


void            StartInput() {
//============================

// Switch the debugger IOCB to it's input file

    if( IOCB->flags & IOF_OUTPT ) {
        IOCB->flags &= ~IOF_OUTPT;
        IOCB->iostmt = IO_READ;
        IOCB->unitid = DB_STD_INPUT;
        FindFtnFile();
    }
}


void            DbClearEOF() {
//============================

    StartInput();
    ClearEOF();
}
