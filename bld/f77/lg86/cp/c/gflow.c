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
// GFLOW        : Generate code for control flow
//

#include "ftnstd.h"
#include "fcodes.h"
#include "opr.h"
#include "opn.h"
#include "stmtsw.h"
#include "cpopt.h"
#include "global.h"
#include "fcgbls.h"
#include "tdreloc.h"
#include "objutil.h"
#include "genobj.h"

extern  void            IfExpr(void);
extern  void            IntegerExpr(void);
extern  void            EmitOp(unsigned_16);
extern  void            PushConst(int);
extern  void            PushOpn(itnode *);
extern  void            PushAddr(itnode *);
extern  void            OpnRef(itnode *);
extern  void            GFakeSP(void);
extern  void            GSFTrcBack(void);
extern  bool            RecNOpn(void);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  void            DecStkDepth(int);
extern  uint            GetStkEntries(uint);
extern  label_id        NextLabel();
extern  void            FreeLabel(label_id);

static  label_id        SFBranchLabel;

#define LOW_HIGH_PAIRS  0x01 // select data table contains low high pairs
#define INT_4_TABLE     0x02 // select data table entries are 4 bytes wide

typedef struct int4split {
    unsigned_16 low;
    unsigned_16 high;
} int4split;


void    GLabel( label_id sym ) {
//==============================

// Generate a label.

    sym->ns.address.la = ObjPtr;
}


void    GStmtLabel( sym_id sym_ptr ) {
//====================================

// Generate a statement label.

    if( sym_ptr != NULL ) {
        sym_ptr->st.address = ObjPtr;
    }
}


void    GenLabRef( label_id sym_ptr ) {
//=====================================

// Generate a label reference.

    if( sym_ptr != NULL ) {
        CodeSymRef( sym_ptr );
    }
}


void    GStmtBr( sym_id stmt ) {
//==============================

// Generate a branch to a statement label.

    EmitOp( BC_ALWAYS );
    GStmtAddr( stmt );
}


static  void    Branch( unsigned_16 bcode, label_id label ) {
//===========================================================

// Generate a branch according to the given branch code.

    EmitOp( bcode );
    GenLabRef( label );
}


void    GBranch( label_id label ) {
//=================================

// Generate a branch (non-conditional).

    Branch( BC_ALWAYS, label );
}


void    GBrFalse( label_id label ) {
//==================================

// Generate a branch on FALSE.

    Branch( BC_FALSE, label );
}


void    G3WayBranch( label_id lt, label_id eq, label_id gt ) {
//============================================================

// Generate a 3-way branch.

    int depth;

    PushConst( 0 );
    IfExpr();
    depth = GetStkEntries( CITNode->typ ) + GetStkEntries( TY_INTEGER );
    ReqStkDepth( depth );
    switch( CITNode->typ ) {
    case TY_INTEGER:
    case TY_INTEGER_2:
    case TY_INTEGER_1:          EmitOp( II_CMP );       break;
    case TY_REAL:               EmitOp( RI_CMP );       break;
    case TY_DOUBLE:             EmitOp( DI_CMP );       break;
    case TY_TRUE_EXTENDED:      EmitOp( XI_CMP );       break;
    }
    EmitOp( THREE_WAY_BRANCH );
    GenLabRef( lt );
    GenLabRef( eq );
    GenLabRef( gt );
    ModStkDepth( -depth );
}


void    InitSelect() {
//====================

// Initialize SELECT statement.

    PushOpn( CITNode );
    if( CITNode->typ == TY_CHAR ) {
        EmitOp( SELECT_SCB );
    }
    GBranch( CSHead->branch );
    DecStkDepth( TY_INTEGER );
}


void    FiniSelect() {
//====================

// Terminate SELECT statement.

    case_entry  *currcase;
    case_entry  *last_case;
    uint        case_count;
    byte        sel_type;

    sel_type = 0; // INTEGER*2 WITH SINGLE ENTRIES
    case_count = 0;
    currcase = CSHead->cs_info.cases->link;
    if( currcase != NULL ) {
        if( currcase->low < -32768 ) {
            sel_type |= INT_4_TABLE;
        }
        for(;;) {
            GenLabRef( currcase->label.g_label );
            if( currcase->low != currcase->high ) {
                sel_type |= LOW_HIGH_PAIRS;
            }
            ++case_count;
            last_case = currcase;
            currcase = currcase->link;
            if( currcase == NULL ) break;
        }
        if( last_case->high > 32767 ) {
            sel_type |= INT_4_TABLE;
        }
    }
    GenLabRef( CSHead->cs_info.cases->label.g_label ); // default label
    GLabel( CSHead->branch );
    EmitOp( I2_SELECT_SINGLES + sel_type );
    OutInt( case_count );
    currcase = CSHead->cs_info.cases->link;
    if( currcase != NULL ) {
        for(;;) {
            OutU16( ((int4split *)(&currcase->low))->low );
            if( sel_type & INT_4_TABLE ) {
                OutU16( ((int4split *)(&currcase->low))->high );
            }
            if( ( sel_type & LOW_HIGH_PAIRS ) != 0 ) {
                OutU16( ((int4split *)(&currcase->high))->low );
                if( ( sel_type & INT_4_TABLE ) != 0 ) {
                    OutU16( ((int4split *)(&currcase->high))->high );
                }
            }
            currcase = currcase->link;
            if( currcase == NULL ) break;
         }
    }
}


void    GAsgnGoTo( bool list ) {
//==============================

// Generate an ASSIGNed GOTO.

    csnode      *cs_ptr;

    PushOpn( CITNode );
    if( list ) {
        EmitOp( ASGN_GOTO_WITH_LIST );
    } else {
        EmitOp( ASGN_GOTO );
        GenLabRef( StNumbers.branches );
        cs_ptr = CSHead;
        for(;;) {
            OutU16( cs_ptr->block );
            if( cs_ptr->block == 0 ) break;
            cs_ptr = cs_ptr->link;
        }
    }
    DecStkDepth( TY_INTEGER );
}


void    GStmtAddr( sym_id stmtno ) {
//==================================

// Generate an entry in the statement table.

    if( stmtno != NULL ) {
        CodeSymRef( stmtno );
    }
}


void    GEndBrTab() {
//===================

// Mark the end of the statement table.

    OutInt( 0 );        // end of statment number list
#if _TARGET == _80386
    OutU16( 0 );        // Kludge to ensure that nothing in list has 0 offset
#endif
}


void    GEndFmtTab() {
//====================

// Mark the end of the format statement table.

    OutInt( 0 );        // end of format statment table
}


void    GAssign( label_id label ) {
//=================================

// Generate an ASSIGN <label> to I.

    PushAddr( CITNode );
    EmitOp( ASSIGN );
    GenLabRef( label );
}


void    GBreak( int routine ) {
//=============================

// Generate a STOP or a PAUSE.

    EmitOp( routine );
    if( RecNOpn() ) {
        OutInt( 0 );
    } else {
        OpnRef( CITNode );
    }
}


void    GStartSF() {
//==================

// Start a statement function.

    EmitOp( BC_ALWAYS );
    SFBranchLabel = NextLabel();
    GenLabRef( SFBranchLabel );
    SFSymId->ns.address.la = ObjPtr;
    GSFTrcBack();
    if( Options & OPT_DEBUG ) {
        EmitOp( DB_SFPRO );
        OutLocalRef( SFSymId->ns.address.la );
    }
}


void    GEndSF() {
//================

// Terminate a statement function.

    if( Options & OPT_DEBUG ) {
        EmitOp( DB_SFEPI );
    }
    EmitOp( SF_RETURN );
    OutLocalRef( SFSymId->ns.address.la );
    // Patch branch around statement function.
    GLabel( SFBranchLabel );
    FreeLabel( SFBranchLabel );
}


void    GStartBlock() {
//=====================

// Start a REMOTE-block.

    CITNode->sym_ptr->ns.address.la = ObjPtr;
    OutInt( 0 );        // emit initial return address (for recursion check)
}


void    GExecute() {
//==================

// Execute a REMOTE-block.

    EmitOp( EXEC_RB );
    CodeSymRef( CITNode->sym_ptr );
}


void    GEndBlock() {
//===================

// Terminate a REMOTE-block.

    EmitOp( END_RB );
    OutLocalRef( CSHead->cs_info.rb->ns.address.la );
}


void    GPgmLabel() {
//===================

// Generate a program label.

    ProgLabel.offset = ObjPtr;
    SProgStart.offset = ObjPtr;
#if _TARGET == _8086
    ProgLabel.seg = CurrSeg;
    SProgStart.seg = CurrSeg;
#endif
    GFakeSP();
}


void    GSegLabel() {
//===================

// Generate a subprogram label.

    SProgStart.offset = ObjPtr;
#if _TARGET == _8086
    SProgStart.seg = CurrSeg;
#endif
}
