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
// TBACC        : access traceback information
//

#include "ftnstd.h"
#include "trcback.h"
#include "rundat.h"


obj_ptr GetTbName() {
//===================

    return( ExCurr->name );
}


void    *GetTBFCCall() {
//======================

    return( &((db_traceback *)ExCurr)->fc_call );
}


void    LinkTrace( lg_traceback *new, obj_ptr name ) {
//====================================================

    new->link = ExCurr;
    new->lineptr = ExLinePtr;
    new->name = name;
    ExCurr = new;
}


void    LinkXTrace( lg_traceback *new ) {
//=======================================

    new->link = ExCurr;
    new->line = TB_LG;
    new->lineptr = ExLinePtr;
    ExCurr = new;
}


void    UnLinkTrace() {
//=====================

    ExLinePtr = ExCurr->lineptr;
    ExCurr = ExCurr->link;
}


void    LinkSFTrace( sf_trace *new ) {
//====================================

    new->link = (sf_trace *)ExCurr;
    new->line_ptr = ExLinePtr;
    ExCurr = (lg_traceback *)&new->link;
}


void    UnLinkSFTrace( sf_trace *curr ) {
//=======================================

    ExCurr = (lg_traceback *)curr->link;
    ExLinePtr = curr->line_ptr;
}
