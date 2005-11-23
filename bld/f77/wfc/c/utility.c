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
* Description:  utilities for compiling
*
****************************************************************************/


#include "ftnstd.h"
#include "errcod.h"
#include "opr.h"
#include "opn.h"
#include "astype.h"
#include "cpopt.h"
#include "global.h"
#include "fmemmgr.h"
#include "recog.h"
#include "ferror.h"
#include "insert.h"
#include "frl.h"
#include "utility.h"

#include <string.h>

extern  void            GetConst(void);
extern  void            GetIntConst(void);
extern  void            EatExpr(void);


void    AdvanceITPtr() {
//======================

    if( CITNode->link != NULL ) {
        CITNode = CITNode->link;
    }
}


void    FreeITNodes( itnode *head ) {
//===================================

// Free the internal text list until null link reached.

    itnode      *next;

    while( head != NULL ) {
        if( ( ( head->opn.us & USOPN_WHAT ) == USOPN_NWL ) ||
            ( ( head->opn.us & USOPN_WHAT ) == USOPN_ASS ) ) {
            if( head->list != NULL ) {
                FreeITNodes( head->list );
            }
        }
        next = head->link;
        FrlFree( &ITPool, head );
        head = next;
    }
}


void    ITPurge() {
//=================

// Release all the internal text node structures.

    FreeITNodes( ITHead );
    ITHead = NULL;
    FrlFini( &ITPool );
}


void    FreeOneNode( itnode *itptr ) {
//====================================

// Free one itnode.

    itptr->link = NULL;
    FreeITNodes( itptr );
}


char    *MkNodeStr( itnode *itptr ) {
//===================================

// Put a NULLCHAR at the end of the "opnd" string of an itnode.

    int         opnd_size;
    char        *str;

    opnd_size = itptr->opnd_size;
    str = FMemAlloc( opnd_size + sizeof( char ) );
    memcpy( str, itptr->opnd, opnd_size );
    str[ opnd_size ] = NULLCHAR;
    return( str );
}


void    FrNodeStr( char *str ) {
//==============================

// Free memory allocated by MkNodeStr().

    FMemFree( str );
}


bool    CmpNode2Str( itnode *itptr, char *str ) {
//===============================================

// Compare the "opnd" field of an itnode to a string.

    int         str_len;

    str_len = strlen( str );
    if( itptr->opnd_size != str_len ) return( FALSE );
    return( memcmp( itptr->opnd, str, str_len ) == 0 );
}


void    ProcExpr() {
//==================

// Process an expression.

    ASType = AST_OFF;
    EatExpr();
}


bool TypeIs( TYPE typ ) {
//======================

// Return TRUE if current itnode has specified variable type.

    switch( CITNode->typ ) {
    case( TY_INTEGER ):
    case( TY_INTEGER_2 ):
    case( TY_INTEGER_1 ):
        return( ( typ == TY_INTEGER ) ||
                ( typ == TY_INTEGER_2 ) ||
                ( typ == TY_INTEGER_1 ) );
    case( TY_LOGICAL ):
    case( TY_LOGICAL_1 ):
        return( ( typ == TY_LOGICAL ) ||
                ( typ == TY_LOGICAL_1 ) );
    default:
        return( CITNode->typ == typ );
    }
}


bool    ConstExpr( TYPE typ ) {
//============================

// Process a constant expression.

    ASType = AST_CEX;
    EatExpr();
    if( AError ) return( FALSE );
    if( CITNode->opn.us != USOPN_CON ) {
        Error( SX_NOT_CONST_EXPR );
        return( FALSE );
    } else if( !TypeIs( typ ) && (typ != TY_NO_TYPE) ) {
        TypeErr( SX_WRONG_TYPE, typ );
        return( FALSE );
    }
    return( TRUE );
}


void    ChkType( TYPE typ ) {
//==========================

    if( !TypeIs( typ ) ) {
        TypeErr( SX_WRONG_TYPE, typ );
    }
}


void    DimExpr() {
//=================

// Process a dimension expression.

    ASType = AST_DIM;
    EatExpr();
    ChkType( TY_INTEGER );
}


void    ProcSubExpr() {
//=====================

// Process a subexpression.

    ASType = AST_SUB;
    EatExpr();
}


void    ProcDataExpr() {
//======================

// Process an expression to be initialized in a DATA statement.

    ASType = AST_IO | AST_DEXP;
    EatExpr();
}


void    ProcDataRepExpr() {
//=========================

// Process an expression used as a repeat specifier in a DATA statement.

    ASType = AST_DIEXP;
    GetIntConst();
}


void    ProcDataIExpr() {
//=======================

// Process an expression used to initialize data in a DATA statement.

    ASType = AST_DIEXP;
    GetConst();
}


void    ProcIOExpr() {
//====================

// Process an expression from an i/o list.

    ASType = AST_IO;
    EatExpr();
}


bool    CLogicExpr() {
//====================

// Process a constant logical expression.

    return( ConstExpr( TY_LOGICAL ) );
}


bool    CCharExpr() {
//===================

// Process a constant character expression.

    return( ConstExpr( TY_CHAR ) );
}


bool    CIntExpr() {
//==================

// Process a constant integer expression.

    return( ConstExpr( TY_INTEGER ) );
}


void    CArithExpr() {
//====================

    if( ConstExpr( TY_NO_TYPE ) &&
        ( ( CITNode->typ < TY_INTEGER_1 ) || ( CITNode->typ > TY_XCOMPLEX ) ) ) {
       Error( SX_NOT_SIMPLE_NUMBER );
    }
}


void    BoolExpr() {
//==================

// Process an expression and check that it is boolean.

    ASType = AST_CCR;                   // flag to set condition codes
    EatExpr();
    if( _IsTypeInteger( CITNode->typ ) ) {
        Extension( SP_INTEGER_CONDITION );
    } else if( !_IsTypeLogical( CITNode->typ ) ) {
        TypeErr( SX_WRONG_TYPE, TY_LOGICAL );
    }
}


void    BoolSubExpr() {
//=====================

// Process a subexpression and check that it is boolean.

    ASType = ( AST_CCR | AST_SUB );       // flag to set condition codes
    EatExpr();
    ChkType( TY_LOGICAL );
}


void    SelectExpr() {
//====================

// Process an expression and check that it is LOGICAL, INTEGER or CHARACTER.

    ProcExpr();
    if( ( !_IsTypeLogical( CITNode->typ ) ) &&
        ( !_IsTypeInteger( CITNode->typ ) ) &&
        ( CITNode->typ != TY_CHAR ) ) {
        TypeErr( SX_WRONG_TYPE, TY_INTEGER );
        CITNode->typ = TY_NO_TYPE;
    }
}


void    IntegerExpr() {
//=====================

// Process an expression and check that it is integer.

    ProcExpr();
    ChkType( TY_INTEGER );
}


void    IntSubExpr() {
//====================

// Process a subexpression and check that it is integer.

    ProcSubExpr();
    ChkType( TY_INTEGER );
}


static  void    EatNumb( int ast ) {
//==================================

    ASType = ast;
    EatExpr();
    if( AError ) return;
    if( ( CITNode->typ < TY_INTEGER_1 ) || ( CITNode->typ > TY_EXTENDED ) ) {
        Error( SX_NOT_SIMPLE_NUMBER );
    }
}


void    EatDoParm() {
//===================

// Process an expression and check that it is integer, real, or
// double precision.

    EatNumb( AST_SUB );
}


void    IfExpr() {
//================

// Process an expression and check that it is integer, real, or double precision
// condition codes must be set for the arithmetic IF.

    EatNumb( AST_CCR );
}


void    CharSubExpr() {
//=====================

// Process a character subexpression.

    ProcSubExpr();
    ChkType( TY_CHAR );
}


bool    BitOn( unsigned_16 bits ) {
//=================================

// Check if "flags" field of CITNode has ANY of specified BITS on.

    return( ( CITNode->flags & bits ) != 0 );
}


int    Map2BaseType( TYPE typ ) {
//==============================

// Due to the three different integer sizes we use this function
// we use the equivalent function

    switch( typ ) {
    case( TY_INTEGER ):
    case( TY_INTEGER_2 ):
    case( TY_INTEGER_1 ):
        return( TY_INTEGER );
    case( TY_LOGICAL ):
    case( TY_LOGICAL_1 ):
        return( TY_LOGICAL );
    default:
        return( typ );
    }
}


bool    ClassIs( unsigned_16 class ) {
//====================================

// Return TRUE if current itnode has specified variable class.
// ( SY_COMMON, SY_SUBPROGRAM, ... )

    return( ( CITNode->flags & SY_CLASS ) == class );
}


bool    Subscripted() {
//=====================

// Return TRUE if current itnode is a subscripted variable.

    return( ClassIs( SY_VARIABLE ) && (CITNode->flags & SY_SUBSCRIPTED) != 0 );
}
