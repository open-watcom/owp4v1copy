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
#include "sdfile.h"
#include "source.h"
#include "argument.h"
#include "symbol.h"
#include "fmtstg.h"
#include "ftnio.h"
#include "cioconst.h"
#include "fmtdef.h"

#define gbl_defn

//
// shared run-time/compile-time stuff
//

#include "rundeflg.h"
#ifndef _SA_LIBRARY
#include "ciovars.h"
#endif
#include "gblboth.h"

//
// Any data declared in fcboth.h CAN NOT be in the BSS... so it must
// appear here explicitly.
//

#ifdef _SA_LIBRARY
    extern      void FMAIN(void);
    #pragma aux FMAIN "*";
    gbl_defn    void PGM        *ProgLabel = {&FMAIN};
#else
    gbl_defn    targ_addr       ProgLabel = {0};
    gbl_defn    targ_addr       DataChain = {0};
#endif

#if _TARGET == _8086
#include "fcenv.h"
#pragma aux (lg_sym) SaveSeg;
gbl_defn        unsigned_16     SaveSeg;
#endif

gbl_defn        byte            MathOptions = {0};
