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
// EXREW        : Run-time REWIND statement processor
//

#include "ftnstd.h"
#include "errcod.h"
#include "rundat.h"
#include "rtenv.h"

extern  void            ChkUnitId(void);
extern  void            ChkConnected(void);
extern  bool            FindFtnFile(void);
extern  void            ChkSequential(int);
extern  void            Rewindf(ftnfile *);
extern  void            ChkIOErr(ftnfile *);
extern  void            ClrBuff(void);
extern  int             IOMain(void (*)());


static  void    RewindFile( ftnfile *fcb ) {
//==========================================

    fcb->flags &= ~FTN_EOF;
    Rewindf( fcb );
    ChkIOErr( fcb );
}


static  void    ExRewind() {
//==========================

    ftnfile     *fcb;

    ChkUnitId();
    FindFtnFile();
    ChkConnected();
    fcb = IOCB->fileinfo;
    ChkSequential( IO_AREW );
    ClrBuff();
    if( fcb->flags & FTN_FSEXIST ) {
        if( _NoRecordOrganization( fcb ) ) {
            RewindFile( fcb );
        } else {
            if( fcb->recnum != 1 ) {
                fcb->recnum = 1;
                RewindFile( fcb );
            }
        }
    }
}


int     IORew() {
//===============

    IOCB->iostmt = IO_REW;
    return( IOMain( &ExRewind ) );
}
