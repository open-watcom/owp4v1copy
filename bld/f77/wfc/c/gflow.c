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
// GFLOW     : control flow code generation routines
//

#include "ftnstd.h"
#include "fcodes.h"
#include "global.h"
#include "fcgbls.h"
#include "opn.h"
#include "cpopt.h"

extern  void            GSPProlog(void);
extern  void            EmitOp(unsigned_16);
extern  void            OutPtr(pointer);
extern  void            OutConst32(signed_32);
extern  void            OutU16(unsigned_16);
extern  obj_ptr         ObjTell(void);
extern  obj_ptr         ObjSeek(obj_ptr);
extern  void            PushOpn(itnode *);
extern  void            IfExpr(void);
extern  sym_id          StaticAlloc(int,int);
extern  label_id        NextLabel(void);
extern  int             TypeSize(uint);
extern  bool            RecNOpn(void);
extern  bool            TypeCmplx(int typ);
extern  void            GenType(itnode *);
extern  void            DumpType(uint,uint);
extern  void            DumpTypes(uint,uint,uint,uint);
extern  obj_ptr         ObjTell(void);
extern  void            OutObjPtr(obj_ptr);
extern  void            PushSym(sym_id);
extern  uint            MapTypes(uint,uint);


void    GLabel( int label ) {
//===========================

// Generate a label.

    EmitOp( DEFINE_LABEL );
    OutU16( label );
}


void    GStmtLabel( sym_id stmt ) {
//=================================

// Generate a statement label.

    EmitOp( STMT_DEFINE_LABEL );
    GStmtAddr( stmt );
}


void    GStmtBr( sym_id stmt ) {
//==============================

// Generate a branch to a statement label.

    EmitOp( STMT_JMP_ALWAYS );
    GStmtAddr( stmt );
}


void    GBranch( int label ) {
//============================

// Generate a branch (non-conditional).

    EmitOp( JMP_ALWAYS );
    OutU16( label );
}


void    GBrFalse( int label ) {
//=============================

// Generate a branch on FALSE.

    EmitOp( JMP_FALSE );
    GenType( CITNode );
    OutU16( label );
}


void    G3WayBranch( sym_id lt, sym_id eq, sym_id gt ) {
//======================================================

// Generate a 3-way branch.

    IfExpr();
    EmitOp( IF_ARITH );
    GenType( CITNode );
    GStmtAddr( lt );
    GStmtAddr( eq );
    GStmtAddr( gt );
}


void    InitSelect() {
//====================

// Initialize SELECT statement.

    sym_id      sel_expr;

    if( !AError ) {
        if( ( ( CITNode->opn & OPN_WHERE ) == 0 ) &&
            ( ( CITNode->opn & OPN_WHAT ) == OPN_NAM ) ) {
            // must be a variable name
            CSHead->cs_info.cases->sel_expr = CITNode->sym_ptr;
        } else {
            if( CITNode->typ == TY_CHAR ) {
                sel_expr = StaticAlloc( 1, TY_CHAR );
                CSHead->cs_info.cases->sel_expr = sel_expr;
                PushSym( sel_expr );
                EmitOp( CHAR_1_MOVE );
                DumpType( TY_INTEGER_1, 1 );
                OutPtr( NULL );
            } else {
                sel_expr = StaticAlloc( CITNode->size,
                                        MapTypes( TY_INTEGER, CITNode->size));
                CSHead->cs_info.cases->sel_expr = sel_expr;
                PushOpn( CITNode );
                EmitOp( POP );  // pop select expression into temporary
                OutPtr( sel_expr );
                DumpTypes( MapTypes( TY_INTEGER, CITNode->size ), CITNode->size,
                           CITNode->typ, CITNode->size );
            }
            EmitOp( EXPR_DONE );
        }
        GBranch( CSHead->branch );          // branch to "select table"
    }
}


void    FiniSelect() {
//====================

// Terminate SELECT statement.

    int         count;
    obj_ptr     curr_obj;
    case_entry  *ptr;

    GLabel( CSHead->branch );                   // label start of select table
    if( CSHead->typ == CS_COMPUTED_GOTO ) {
        EmitOp( COMPUTED_GOTO );
    } else {
        EmitOp( SELECT );
    }
    curr_obj = ObjTell();
    OutU16( 0 );                                // emit count
    count = 0;
    ptr = CSHead->cs_info.cases;
    OutU16( ptr->label.g_label );               // label for default case
    ptr = ptr->link;
    while( ptr != NULL ) {                      // emit lab,hi,lo for ea. case
        if( CSHead->typ == CS_COMPUTED_GOTO ) {
            GStmtAddr( ptr->label.st_label );
        } else {
            OutU16( ptr->label.g_label );
        }
        OutConst32( ptr->high );
        OutConst32( ptr->low );
        count++;
        ptr = ptr->link;
    }
    curr_obj = ObjSeek( curr_obj );
    OutU16( count );
    ObjSeek( curr_obj );
    OutPtr( CSHead->cs_info.cases->sel_expr );
}


void    GAsgnGoTo( bool list ) {
//==============================

// Generate an ASSIGNed GOTO.

    if( !list ) {
        if( WildLabel == NULL ) {
            WildLabel = StaticAlloc( sizeof( inttarg ), TY_INTEGER_TARG );
        }
        EmitOp( PUSH );
        OutPtr( CITNode->sym_ptr );
        EmitOp( POP );
        OutPtr( WildLabel );
        DumpTypes( TY_INTEGER_TARG, sizeof( inttarg ), CITNode->typ, CITNode->size );
        EmitOp( EXPR_DONE );
        GBranch( StNumbers.branches );      // goto select table
    } else {
        EmitOp( ASSIGNED_GOTO_LIST );
        OutPtr( CITNode->sym_ptr );
    }
}


void    GStmtAddr( sym_id stmtno ) {
//==================================

// Generate an entry in the statement table.

    if( stmtno != NULL ) {
        OutPtr( stmtno );
    }
}


void    GEndBrTab() {
//===================

// Mark the end of the statement table.

    OutPtr( NULL );
}


void    GAssign( sym_id label ) {
//===============================

// Generate an ASSIGN <label> to I.

    EmitOp( ASSIGN );
    OutPtr( label );
    OutPtr( CITNode->sym_ptr );
}


void    GBreak( int routine ) {
//=============================

// Generate a STOP or a PAUSE.

    EmitOp( routine );
    if( RecNOpn() ) {
        OutPtr( NULL );
    } else {
        OutPtr( CITNode->sym_ptr );
        CITNode->sym_ptr->lt.flags |= LT_SCB_REQUIRED;
    }
}


void    GStartSF() {
//==================

// Start a statement function.

    EmitOp( START_SF );
    if( OZOpts & OZOPT_O_INLINE ) {
        SFSymId->ns.si.sf.sequence = ObjTell();
        OutObjPtr( 0 );
    } else {
        OutPtr( SFSymId );
        SFSymId->ns.si.sf.location = NextLabel();
        OutU16( NextLabel() );
    }
}


void    GEndSF() {
//================

// Terminate a statement function.

    obj_ptr     curr_obj;

    if( OZOpts & OZOPT_O_INLINE ) {
        EmitOp( END_OF_SEQUENCE );
        curr_obj = ObjSeek( SFSymId->ns.si.sf.sequence );
        OutObjPtr( curr_obj );
        ObjSeek( curr_obj );
    } else {
        if( TypeCmplx( SFSymId->ns.typ ) ) {
            EmitOp( CMPLX_EXPR_DONE );
        } else {
            EmitOp( EXPR_DONE );
        }
        EmitOp( END_SF );
        OutPtr( SFSymId );
    }
}


void    GStartBlock() {
//=====================

// Start a REMOTE BLOCK.

    EmitOp( START_RB );
    OutPtr( CITNode->sym_ptr );
}


void    GExecute() {
//==================

// Execute a REMOTE BLOCK.

    EmitOp( EXECUTE );
    OutPtr( CITNode->sym_ptr );
}


void    GEndBlock() {
//===================

// Terminate a REMOTE-block.

    EmitOp( END_RB );
}


void    GPgmLabel() {
//===================

// Generate a program label.

    EmitOp( RT_PROLOGUE );
    OutPtr( SubProgId );
}


void    GSegLabel() {
//===================

// Generate a subprogram label.

    EmitOp( RT_PROLOGUE );
    OutPtr( SubProgId );
}


void    GBlockLabel() {
//=====================

// Generate a block data subprogram label.

    EmitOp( RT_PROLOGUE );
    OutPtr( SubProgId );
}


void    FreeLabel( int label ) {
//==============================

// Generate F-Code indicating we are done with label.

    EmitOp( FREE_LABEL );
    OutU16( label );
}
