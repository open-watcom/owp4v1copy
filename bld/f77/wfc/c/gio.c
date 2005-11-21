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
// GIO       : I/O code generation routines
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "types.h"
#include "iodefn.h"
#include "fcgbls.h"
#include "stmtsw.h"
#include "opn.h"
#include "cpopt.h"
#include "emitobj.h"

extern  bool            NotFormatted(void);
extern  uint            IOIndex(void);
extern  sym_id          GTempString(uint);
extern  bool            AuxIOStmt(void);
extern  void            GStmtAddr(sym_id);
extern  bool            Already(int);


void    GSetIOCB() {
//==================

// Generate a call to set the IOCB.

    EmitOp( RT_SET_IOCB );
}


void    GStartIO() {
//==================

// Generate code to invoke the run-time routine.

    if( !AuxIOStmt() && NotFormatted() ) {
        EmitOp( RT_SET_NOFMT );
    }
    EmitOp( RT_IO_STMTS + IOIndex() );
    // PRINT, READ and WRITE i/o statements can check for END= and ERR=
    // statement labels when RT_ENDIO is generated; auxilliary i/o
    // statements don't generate RT_ENDIO so generate F-Code to check
    // for statement labels.
    if( AuxIOStmt() || Already( IO_NAMELIST ) ) {
        EmitOp( CHK_IO_STMT_LABEL );
    }
}


void    GIOStruct( sym_id sd ) {
//==============================

// Generate code to do structure i/o.

    PushOpn( CITNode );
    if( StmtProc == PR_READ ) {
        EmitOp( INPUT_STRUCT );
    } else {
        EmitOp( OUTPUT_STRUCT );
    }
    OutPtr( sd ); // structure definition
}


static  void    GIORoutine( TYPE typ, uint size ) {
//=================================================

    unsigned_16 op_code;

    op_code = ParmType( typ, size ) - PT_LOG_1;
    if( StmtProc == PR_READ ) {
        EmitOp( op_code + RT_INP_LOG1 );
    } else {
        EmitOp( op_code + RT_OUT_LOG1 );
    }
}


void    GIOItem() {
//=================

// Generate code to process an i/o list item.

    PushOpn( CITNode );
    GIORoutine( CITNode->typ, CITNode->size );
}


void    GIOArray() {
//==================

// Generate code to do array i/o.

    if( StmtProc == PR_READ ) {
        EmitOp( INP_ARRAY );
    } else {
        EmitOp( PRT_ARRAY );
    }
    OutPtr( CITNode->sym_ptr );
    if( CITNode->opn.us & USOPN_FLD ) {
        OutPtr( CITNode->value.st.field_id );
    } else {
        OutPtr( NULL );
    }
}


void    GIOStructArray() {
//========================

// Generate code to do structured array i/o.

    if( StmtProc == PR_READ ) {
        EmitOp( STRUCT_INP_ARRAY );
    } else {
        EmitOp( STRUCT_PRT_ARRAY );
    }
    OutPtr( CITNode->sym_ptr );
}


void    GStopIO() {
//=================

// Generate code to return a null i/o item to run-time i/o.
// This is done for only PRINT, WRITE and READ statements.

    if( !Already( IO_NAMELIST ) ) {
        EmitOp( RT_ENDIO );
    }
}


void    GPassValue( uint rtn ) {
//==============================

// Pass the value of CITNode on the stack and emit fcode for routine.

    PushOpn( CITNode );
    EmitOp( rtn );
    if( ( rtn == RT_SET_UNIT ) || ( rtn == RT_SET_REC ) ||
        ( rtn == RT_SET_RECL ) || ( rtn == RT_SET_BLOCKSIZE ) ) {
        GenType( CITNode );
    }
}


void    GSetNameList( uint routine ) {
//====================================

// Pass the address of NAMELIST data for run-time routine.

    EmitOp( routine );
    OutPtr( CITNode->sym_ptr );
}


void    GPassAddr( uint routine ) {
//=================================

// Pass the address of CITNode on the stack and emit fcode for routine.

    PushOpn( CITNode );
    EmitOp( routine );
}


void    GPassStmtNo( sym_id sn, uint routine ) {
//==============================================

// Pass the label for a statement number. For example,
//        PRINT 10, ...
// 10     FORMAT( ... )

    EmitOp( routine );
    GStmtAddr( sn );
}


void    GPassLabel( label_id label, uint routine ) {
//==================================================

// Pass the label identifying encoded format string.
// Called when using
//        PRINT <constant character expression>, ...

    EmitOp( PASS_LABEL );
    OutU16( routine );
    OutU16( label );
}


void    GFmtVarSet() {
//====================

// Called when using
//        ASSIGN 10 TO I
//        PRINT I, ...
// 10     FORMAT( ... )

    EmitOp( RT_FMT_ASSIGN );
    OutPtr( CITNode->sym_ptr );
}


static  void    ChkExtendFmt() {
//==============================

    if( Options & OPT_EXTEND_FORMAT ) {
        OutU16( 1 );
    } else {
        OutU16( 0 );
    }
}


void    GFmtArrSet() {
//====================

// Called when using
//        PRINT <character array>, ...

    EmitOp( RT_FMT_ARR_SCAN );
    OutPtr( CITNode->sym_ptr );
    ChkExtendFmt();
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


void    GArrIntlSet() {
//=====================

// Set internal file pointer to array.

    EmitOp( ARR_SET_INTL );
    OutPtr( CITNode->sym_ptr );
    OutPtr( GTempString( 0 ) );
}


void    GIntlSet() {
//==================

// Set internal file pointer to character variable.

    PushOpn( CITNode );
    EmitOp( RT_SET_INTL );
}


void    GCheckEOF( label_id label ) {
//===================================

// Patch the label emitted by GNullEofStmt() to be the label at the end
// of the code of the ATEND statement.

    obj_ptr     curr_obj;

    curr_obj = ObjSeek( AtEndFCode );
    EmitOp( SET_ATEND );
    OutU16( label );
    ObjSeek( curr_obj );
}


void    GNullEofStmt() {
//======================

// Emit the "null" F-Code.
// If an ATEND statement follows, the "null" F-Code will be patched with a
// RT_SET_END F-Code.

    AtEndFCode = ObjTell();
    EmitOp( NULL_FCODE );
    EmitOp( NULL_FCODE );
}
