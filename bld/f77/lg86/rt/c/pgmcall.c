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
// PGMCALL      : Bootstrap used by FCDRVR to start up f-code machine
//

#include "ftnstd.h"
#include "fcodes.h"

extern void PGM *ProgLabel;

#if _TARGET == _8086

    struct {
        void            (near *rt_prol)(void);
        void            (near *proglabel)(void);
        unsigned_16     zero;
        void            (near *rt_stoppgm)(void);
    } PgmCall[] = {
        RT_PROLOGUE * 2,
        &ProgLabel,
        0,
        RT_STOPPGM * 2
    };
#else

    struct {
        unsigned_16     rt_prol;
        void PGM        *proglabel;
        unsigned_16     zero;
        unsigned_16     rt_stoppgm;
    } PgmCall = {
        RT_PROLOGUE * 4,
        &ProgLabel,
        0,
        RT_STOPPGM * 4
    };
#endif
