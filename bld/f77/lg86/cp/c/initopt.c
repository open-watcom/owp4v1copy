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
// INITOPT   : initialize default options
//

#include <string.h>

#include "ftnstd.h"
#include "cpopt.h"
#include "global.h"
#include "bglobal.h"
#include "mathopt.h"
#include "fcboth.h"

extern  void            __UseEnglishCharSet(void);

static  ftnoption       SaveOptions;

#define INIT_OPTS       (OPT_LIST | OPT_CHECK | OPT_WARN | OPT_TERM | \
                         OPT_XTYPE | OPT_ARRCHECK | OPT_REFERENCE | OPT_RUN)

#define MATH_OPTS       MATH_FLT_OVER;

#define LINES_PER_PAGE  66      // default lines per page in listing file


void    SetDefaultOpts() {
//========================

// Initialize options.

    MaxStLim = 0;
    MaxTimeLim = 0;
    MaxPageLim = 0;
    StmtLimit = 0;
    TimeLimit  = 0;
    PageLimit  = 0;
#if _TARGET == _80386
    ObjLimit = 1024UL*1024UL;
#elif _OPSYS == _QNX
    ObjLimit = 512UL*1024UL;
#elif _TARGET == _8086
    ObjLimit = 256UL*1024UL;
#endif
    Options = INIT_OPTS;
    MathOptions = MATH_OPTS;
    LinesPerPage = LINES_PER_PAGE;
    NewOptions = Options;
    __UseEnglishCharSet();
}


void    InitOptions() {
//=====================

// Gets called by WATFOR-77 after command line options have been
// processed (NewOptions contains the options).

    Options = NewOptions;
    if( PageLimit == 0 ) {
        PageLimit = MaxPageLim;
    }
    if( StmtLimit == 0 ) {
        StmtLimit = MaxStLim;
    }
    if( TimeLimit == 0 ) {
        TimeLimit = MaxTimeLim;
    }
}


void    SaveInitialOpts() {
//=========================

// Save value of options specified on command line when invoked from DOS.
// These options become default options when invoking WATFOR-77 from the
// editor.

    SaveOptions = NewOptions;
}


void    SetInitialOpts() {
//========================

// Initialize options to those specified on the command line when
// WATFOR-77 was invoked from DOS.

    NewOptions = SaveOptions;
}
