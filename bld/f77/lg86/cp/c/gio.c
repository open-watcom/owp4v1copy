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
* Description:  Generate code for I/O.
*
****************************************************************************/


#include "ftnstd.h"
#include "fcodes.h"
#include "parmtype.h"
#include "prdefn.h"
#include "global.h"
#include "fcgbls.h"
#include "opn.h"
#include "cpopt.h"
#include "iodefn.h"
#include "structio.h"
#include "objutil.h"
#include "tdreloc.h"
#include "prtarr.h"

extern  void            EmitOp(unsigned_16);
extern  int             IOIndex(void);
extern  bool            NotFormatted(void);
extern  int             ParmType(int,int);
extern  void            PushOpn(itnode *);
extern  void            PushIOOpn(itnode *);
extern  void            PushAddr(itnode *);
extern  void            OpnRef(itnode *);
extern  void            GenTmpRef(label_id);
extern  void            GenLabRef(label_id);
extern  void            GStmtAddr(sym_id);
extern  void            GLabel(label_id);
extern  void            FreeLabel(label_id);
extern  label_id        NextLabel(void);
extern  sym_id          TmpAlloc(int);
extern  sym_id          StaticAlloc(int,int);
extern  void            GValue(int,int);
extern  void            DecStkDepth(int);
extern  bool            AuxIOStmt(void);
extern  void            ChkValidParm(itnode *);
extern  void            SetOpn(itnode *,int);
extern  bool            Already(int);

static  label_id        IOSLabel = { 0 };


void    GSetIOCB() {
//==================

// Generate a call to set the IOCB.

    if( Options & OPT_CHECK ) {
        EmitOp( RT_SET_IOCB_CHECK );
    } else {
        EmitOp( RT_SET_IOCB );
    }
}


void    GStartIO() {
//==================

// Generate code to invoke the run-time routine.

    if( !AuxIOStmt() && NotFormatted() ) {
        EmitOp( RT_SET_NOFMT );
    }
    EmitOp( RT_IO_STMTS + IOIndex() );
    if( AuxIOStmt() || Already( IO_NAMELIST ) ) {
        if( IOSLabel != 0 ) {
            GLabel( IOSLabel );
            FreeLabel( IOSLabel );
            IOSLabel = 0;
        }
    }
}


void    GIOItem() {
//=================

// Generate code to process an io list item.

    int         typ;

    typ = ParmType( CITNode->typ, CITNode->size );
    ChkValidParm( CITNode );
    if( StmtProc == PR_READ ) {
        PushAddr( CITNode );
        EmitOp( typ - PT_LOG_1 + RT_INP_LOG1 );
    } else {
        PushIOOpn( CITNode );
        EmitOp( typ - PT_LOG_1 + RT_OUT_LOG1 );
        DecStkDepth( CITNode->typ );
    }
}


void    DoStruct() {
//==================

//  emit code to process a single structure (RCB on stack already)

    sym_id      cntrl_blk;

    cntrl_blk = TmpAlloc( sizeof( sio_cntrl_blk ) );
    EmitOp( STRUCT_IO_INIT );
    GenTmpRef( cntrl_blk );
    if( StmtProc == PR_READ ) {
        EmitOp( STRUCT_IO_READ_ITEM );
    } else {
        EmitOp( STRUCT_IO_WRITE_ITEM );
    }
    GenTmpRef( cntrl_blk );
}


void    GIOStruct( sym_id sd ) {
//==============================

// Generate code to do structure i/o.

    sd = sd;
    ChkValidParm( CITNode );
    PushOpn( CITNode );         // put RCB on stack
    DoStruct();
}


void    GIOStructArray() {
//========================

// Generate code to do array of structures i/o

    label_id    label;
    uint        typ;
    sym_id      cntrl_blk;

    ChkValidParm( CITNode );
    cntrl_blk = TmpAlloc( sizeof( arr_prt_block ) );
    typ = ParmType( CITNode->typ, CITNode->size );
    EmitOp( IO_STRUCT_ARRAY );  // initialize array output
    OpnRef( CITNode );
    GenTmpRef( cntrl_blk );
    label = NextLabel();
    GLabel( label );
    EmitOp( STRUCT_SUBSCR );    // put RCB onto stack
    SymRef( CITNode->sym_ptr->ns.xt.sym_record );
    DoStruct();
    EmitOp( NEXT_ARR_ELT );     // get next array element
    GenTmpRef( cntrl_blk );
    GenLabRef( label );
    FreeLabel( label );
}


void    GIOArray() {
//==================

// Generate code to output an array.

    ChkValidParm( CITNode );
    if( StmtProc == PR_READ ) {
        EmitOp( INP_ARRAY );
    } else {
        EmitOp( PRT_ARRAY );
    }
    OpnRef( CITNode );
    // Emit the type of array it is.
    OutInt( ParmType( CITNode->typ, CITNode->size ) );
}


void    GStopIO() {
//=================

// Generate code to return a null i/o item to run-time i/o.

    if( !Already( IO_NAMELIST ) ) {
        EmitOp( RT_ENDIO );
    }
    if( IOSLabel != 0 ) {
        GLabel( IOSLabel );
        FreeLabel( IOSLabel );
        IOSLabel = 0;
    }
}


void    GPassValue( uint routine ) {
//==================================

// Pass the value of CITNode on the stack and emit F-Code for routine.

    PushOpn( CITNode );
    EmitOp( routine );
    DecStkDepth( CITNode->typ );
}


void    GPassAddr( uint routine ) {
//=================================

// Pass the address of CITNode on the stack and emit F-Code for routine.

    PushAddr( CITNode );
    EmitOp( routine );
    if( routine == RT_SET_IOS ) {
        IOSLabel = NextLabel();
        GenLabRef( IOSLabel );
    }
}


void    GPassStmtNo( sym_id label, uint routine ) {
//=================================================

// Pass the label identifying encoded format string.
// Called when using
//        PRINT 10, ...
// 10     FORMAT( ... )

    EmitOp( routine );
    GStmtAddr( label );
}


void    GPassLabel( label_id label, uint routine ) {
//==================================================

// Pass the label identifying encoded format string.
// Called when using
//        PRINT <constant character expression>, ...

    EmitOp( routine );
    GenLabRef( label );
}


void    GFmtVarSet() {
//====================

// Called when using
//        ASSIGN 10 TO I
//        PRINT I, ...
// 10     FORMAT( ... )

    EmitOp( RT_FMT_ASSIGN );
    OpnRef( CITNode );
    GStmtAddr( StNumbers.formats );
}


static  void    ChkExtendFmt() {
//==============================

    if( Options & OPT_EXTEND_FORMAT ) {
        OutU16( 0 );
    } else {
        OutU16( 1 );
    }
}


void    GFmtArrSet() {
//====================

// Called when using
//        PRINT <character array>, ...

    EmitOp( RT_FMT_ARR_SCAN );
    ChkExtendFmt();
    OpnRef( CITNode );
}


void    GFmtExprSet() {
//=====================

// Pass the label identifying encoded format string.
// Called when using
//        PRINT <character expression>, ...

    PushOpn( CITNode );
    EmitOp( RT_FMT_SCAN );
    ChkExtendFmt();
}


void    GArrIntlSet( void ) {
//===========================

// Set internal file pointer to array.

    EmitOp( RT_SET_INTL );
    OpnRef( CITNode ); // emit address of ADV
    CITNode->sym_ptr = StaticAlloc( sizeof( string ), TY_NO_TYPE );
    CITNode->opn = OPN_VAL;
    OpnRef( CITNode ); // emit address of SCB
}


void    GIntlSet() {
//==================

// Set internal file pointer to character variable.

    PushAddr( CITNode );
    EmitOp( RT_SET_INTL );
    OutInt( 0 );                // indicate a variable
}


void    GCheckEOF( label_id label ) {
//===================================

// Patch the label emitted by GNullEofStmt() to be the label of the code
// for the ATEND statement.

    obj_ptr     save_objptr;

    save_objptr = ObjPtr;
    ObjPtr = EndEqLabel;
    GenLabRef( label );
    ObjPtr = save_objptr;
}


void    GNullEofStmt() {
//======================

// Generate a call to RT_SET_END of NULL for a READ statement.
// If an ATEND statement follows, the NULL will be patched to be the
// ATEND code.

    EmitOp( RT_SET_END );
    EndEqLabel = ObjPtr;
    OutInt( 0 );
}


void    GSetNameList( uint routine ) {
//====================================

// Pass the address of NAMELIST data for run-time routine.

    grp_entry   *ge;
    unsigned_16 num_args;

    num_args = 0;
    ge = CITNode->sym_ptr->nl.group_list;
    while( ge != NULL ) {       // count number of args
        ++num_args;
        ge = ge->link;
    }
    EmitOp( routine );
    OutU16( num_args );
    OpnRef( CITNode );
    SetOpn( CITNode, OPN_SAFE );
    ge = CITNode->sym_ptr->nl.group_list;
    while( ge != NULL ) {       // output the args
        SymRef( ge->sym );
        ge = ge->link;
    }
}
