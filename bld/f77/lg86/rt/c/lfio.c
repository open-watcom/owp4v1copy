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
// LFIO         : listing file I/O at run-time
//

#include <string.h>

#include "ftnstd.h"
#include "cioconst.h"
#include "rundat.h"
#include "fio.h"
#include "posio.h"
#include "errcod.h"

extern  uint            FSetCC(file_handle,char,char**);
extern  void            FPutRec(file_handle,char *,uint);
extern  int             Errorf(file_handle);
extern  char            *ErrorMsg(file_handle);
extern  void            MsgLine(uint,char *);


void    LFWrite( char *buff, uint len ) {
//=======================================

    char        *cc;
    uint        cc_len;
    char        errbuff[ERR_BUFF_SIZE+1];

    if( RListFile == NULL ) return;

    // Artificially create a carriage control character

    cc_len = FSetCC( RListFile, ' ', &cc );

    // Since this record doesn't have carriage control
    // we have to turn it off for the FPutRec call.

    ((a_file *)RListFile)->attrs &= ~CARRIAGE_CONTROL;
    ((a_file *)RListFile)->attrs |= CC_NOCR;
    FPutRec( RListFile, cc, cc_len );
    ((a_file *)RListFile)->attrs &= ~CC_NOCR;
    FPutRec( RListFile, buff, len );
    ((a_file *)RListFile)->attrs |= CARRIAGE_CONTROL;
    if( Errorf( RListFile ) == IO_OK ) return;
    strcpy( errbuff, ErrorMsg( RListFile ) );

    // Must set RListFile to NULL before calling MsgLine() so that
    // we don't recursively write error messages to listing file.

    RListFile = NULL;
    MsgLine( MS_ERR_WRT_LST, errbuff );
}
