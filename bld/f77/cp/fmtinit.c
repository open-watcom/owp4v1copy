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
// FmtInit      : FORMAT statement initialization routines
//

#include "ftnstd.h"
#include "fmterr.h"
#include "fmtdef.h"
#include "fmtdat.h"
#include "format.h"
#include "prdefn.h"
#include "global.h"

extern  void            GFEmEnd(void);
extern  void            GFEmNum(int);
extern  void            FmtError(int);
extern  void            GStmtLabel(sym_id);
extern  void            R_FDoSpec(void);
#if _OPT_CG == _ON
extern  void            StartFmt(cs_label);
extern  void            EndFmt(void);
#else
extern  void            GLabel(label_id);
extern  void            GBranch(label_id);
extern  label_id        NextLabel(void);
extern  void            FreeLabel(label_id);
#endif

extern  void            (* const __FAR CFmtTab[])();


void    FScan( int fmt_length, char *fmt_string, cs_label fmt_label ) {
//=====================================================================

// FORMAT statement parsing (only compile-time).

#if _OPT_CG == _OFF
    label_id    end_label;
#endif

    FInit( fmt_length, fmt_string );
#if _OPT_CG == _OFF
    end_label = NextLabel();
    GBranch( end_label );
    if( StmtProc == PR_FMT ) {    // if FORMAT statement
        GStmtLabel( fmt_label.st_label );
    } else {
        GLabel( fmt_label.g_label );
    }
#else
    StartFmt( fmt_label );
#endif
    R_FDoSpec();
    FFinish();
    GFEmEnd();
#if _OPT_CG == _OFF
    GLabel( end_label );
    FreeLabel( end_label );
#else
    EndFmt();
#endif
}


static  void    FInit( int fmt_length, char *fmt_string ) {
//=========================================================

    Fmt_start = fmt_string;
    Fmt_end = fmt_string + fmt_length;
    Fmt_paren_level = 0;
    Fmt_charptr = fmt_string;
    Fmt_delimited = NO_DELIM;
    FmtEmTab = CFmtTab;
}


static  void    FFinish() {
//=========================

    if( StmtProc == PR_FMT ) {
        for(;;) {
            if( *Fmt_charptr != ' ' ) break;
            ++Fmt_charptr;
            if( Fmt_charptr >= Fmt_end ) break;
        }
        if( ( Fmt_charptr < Fmt_end ) && ( *Fmt_charptr != '!' ) ) {
            FmtError( FM_NO_EOS );
        }
    }
}
