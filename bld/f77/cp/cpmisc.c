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
// CPMISC    : compile miscellaneous statements
//

#include "ftnstd.h"
#include "errcod.h"
#include "rtconst.h"
#include "progsw.h"
#include "global.h"
#include "opn.h"

extern  void            Error(int,...);
extern  void            Extension(int,...);
extern  bool            RecNumber(void);
extern  bool            RecLiteral(void);
extern  bool            RecNOpn(void);
extern  bool            RecNOpr(void);
extern  bool            RecEOS(void);
extern  bool            ReqEOS(void);
extern  void            AdvanceITPtr(void);
extern  void            GBreak(int);
extern  void            GReturn(void);
extern  void            CSNoMore(void);
extern  void            AddConst(itnode *);
extern  void            GetConst(void);
extern  void            FScan(int,char *,cs_label);
extern  sym_id          FmtPointer(void);
extern  void            LFSkip(void);
extern  void            BadStmt(void);

extern  char            *StmtKeywords[];


void    CpStop() {
//================

    NumOrLit( RT_STOP );
    Remember.transfer = TRUE;
    Remember.stop_or_return = TRUE;
}


void    CpPause() {
//=================

    NumOrLit( RT_PAUSE );
}


static  void    NumOrLit( int routine ) {
//=======================================

    if( RecNOpn() ) {
        AdvanceITPtr();
    }
    if( RecNOpn() ) {
        GBreak( routine );
    } else if( !RecLiteral() ) {
        if( CITNode->opn == OPN_INT ) {
            if( CITNode->opnd_size > 5 ) {
                Extension( ST_LONG_NUM, StmtKeywords[ StmtProc - 1 ] );
            }
            BreakOpn( routine );
        } else {
            Error( SX_NUM_OR_LIT );
        }
    } else {
        BreakOpn( routine );
    }
}


static  void    BreakOpn( int routine ) {
//=======================================

    CITNode->opn = OPN_LIT;
    GetConst();
    AddConst( CITNode );
    GBreak( routine );
    AdvanceITPtr();
    if( !RecEOS() ) {
        Error( SX_NUM_OR_LIT );
    }
}


void    CpFormat() {
//==================

    cs_label    fmt;

    fmt.st_label = FmtPointer();
    FScan( CITNode->opnd_size, CITNode->opnd, fmt );
}


void    CpEnd() {
//===============

    LFSkip();
    if( RecNOpn() ) {
        ProgSw |= PS_END_OF_SUBPROG;
        AdvanceITPtr();
        Remember.endstmt = TRUE;
        ReqEOS();
    } else {
        BadStmt();
    }
}
