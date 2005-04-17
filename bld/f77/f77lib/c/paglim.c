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
// PAGELIM   : check that the pages of output limit not exceeded
//

#include "ftnstd.h"
#include "errcod.h"
#include "rundat.h"
#include "bglobal.h"

extern  bool            IsCarriage();
extern  void            RTErr(int errcode,...);


void    CheckPageLimit() {
//========================

    char        *ptr;

    if( PageLimit != 0 ) {
        if( IsCarriage() ) {
            ptr = IOCB->fileinfo->buffer;
            if( *ptr == '1' ) {
                LinesOut = 0;
                PagesOut++;
            } else if( *ptr == '0' ) {
                LinesOut += 2;
            } else if( *ptr == '-' ) {
                LinesOut += 3;
            } else if( *ptr != '+' ) {
                LinesOut++;
            }
        } else {
            LinesOut++;
        }
        if( LinesOut > LinesPerPage ) {
            LinesOut -= LinesPerPage;
            PagesOut++;
        }
        if( PagesOut >= PageLimit ) {
            RTErr( KO_PAGES_OUT );
        }
    }
}
