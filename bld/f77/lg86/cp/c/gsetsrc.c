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
// GSETSRC      : Generate debugging information
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "stmtsw.h"
#include "cpopt.h"
#include "objutil.h"

extern  void            EmitOp(unsigned_16);
extern  void            OutSrcLine(void);


void    GISNCall() {
//==================

// Generate ISN call to debugger ISN or normal ISN routine

    if( Options & OPT_DEBUG ) {
        GSetDbugLine();
    } else {
        GSetSrcLine();
    }
}


void    GSetSrcLine() {
//=====================

// Generate code to set the current source line number.

    if( ( StmtSw & SS_ISN_DONE ) == 0 ) {
        EmitOp( RT_SET_LINE );
        OutInt( ISNNumber );
        StmtSw |= SS_ISN_DONE;
    }
}


void    GDbugInfo() {
//===================

// Generate code to call debugger ISN routine if C$DEBUG on

    if( Options & OPT_DEBUG ) {
        GSetDbugLine();
    }
}


void    GSetDbugLine() {
//======================

// Generate code to call debugger ISN routine

    if( ( StmtSw & SS_ISN_DONE ) == 0 ) {
        EmitOp( RT_END_DBUG ); // this is really for the previous line
        EmitOp( RT_ISN_DBUG );
        OutSrcLine();
        OutInt( ISNNumber );
        StmtSw |= SS_ISN_DONE;
    }
}
