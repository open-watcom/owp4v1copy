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
* Description:  Implement operand stack.
*
****************************************************************************/


#include "ftnstd.h"
#include "global.h"
#include "parmtype.h"
#include "fcodes.h"
#include "opn.h"
#include "opr.h"
#include "astype.h"
#include "cpopt.h"
#include "rcb.h"
#include "tdreloc.h"

extern  int             ParmType(int,int);
extern  void            EmitOp(unsigned_16);
extern  void            AddConst(itnode *);
extern  void            SetOpn(itnode *,int);
extern  void            ProcList(itnode *);
extern  void            GValue(int,int);
extern  void            GUValue(int,int);
extern  void            GUFValue(int,int);
extern  void            GStore(int,int);
extern  int             TypeSize(int);
extern  sym_id          STConst(ftn_type *,int,int);
extern  void            CompErr(char *);
extern  sym_id          StaticAlloc(int,int);
extern  void            GBegCall(itnode *);
extern  void            GEndCall(itnode *,int);
extern  void            IncStkDepth(uint);
extern  void            DecStkDepth(uint);
extern  void            ReqStkDepth(uint);
extern  uint            GetStkEntries(uint);
extern  void            ChkValidParm(itnode *);
extern  void            OutInt(int);


static  bool    SymIndirect( unsigned_16 flags, byte typ ) {
//==========================================================

// Determine whether a symbol is referenced indirectly.

    if( ( flags & SY_CLASS ) != SY_VARIABLE ) return( FALSE );
    if( typ == TY_CHAR || typ == TY_STRUCTURE ) return( FALSE );
    if( ( flags & (SY_SUB_PARM|SY_IN_EC|SY_IN_DIMEXPR) ) == 0 ) return( FALSE );
    return( TRUE );
}


extern  bool    ChkUDef( itnode *itptr ) {
//=========================================

    int         what;
    int         where;
    unsigned_16 flags;

    flags = itptr->flags;
    if( ( Options & OPT_CHECK ) == 0 ) return( FALSE ); // C$NOCHECK in effect
    if( ( ( flags & SY_SUBSCRIPTED ) == 0 ) &&
        ( ( flags & SY_DATA_INIT ) != 0 ) ) return( FALSE ); // DATA initialized
    if( flags & SY_SPECIAL_PARM ) return( FALSE ); // shadowed variables
    if( flags & SY_DO_PARM ) return( FALSE ); // DO-LOOP variables
    where = itptr->opn & OPN_WHERE;
    if( where == OPN_VAL ) return( FALSE ); // static temps
    if( where == OPN_TMP ) return( FALSE ); // temps
    what = itptr->opn & OPN_WHAT;
    if( what == OPN_CON ) return( FALSE ); // constants
    if( what == OPN_STN ) return( FALSE ); // statement #'s
    return( TRUE );
}


extern  void    Push32Const( intstar4 value ) {
//=============================================

// Generate a push of the specified integer (32-bit) constant.

    uint        type_size;

    IncStkDepth( TY_INTEGER );
    type_size = TypeSize( TY_INTEGER );
    EmitOp( PUSHOPS + ParmType( TY_INTEGER, type_size ) - 1 );
    SymRef( STConst( (ftn_type *)&value, TY_INTEGER, type_size ) );
}


extern  void    PushConst( int value ) {
//======================================

// Generate a push of the specified integer constant.

    Push32Const( value );
}


extern  void    OpnRef( itnode *itptr ) {
//=======================================

// Emit the address of the specified operand.

    SymRef( itptr->sym_ptr );
}


extern  void    PopOpn() {
//========================

// Generate a pop of an operand (assignment - except character and struct).

    unsigned_16 pop;
    unsigned_16 flags;
    int         size;
    byte        typ;

    flags = CITNode->flags;
    size = CITNode->size;
    typ = CITNode->typ;
    if( (CITNode->opn & OPN_WHAT) == OPN_NWL ) {
        // Assignment is to an array element.
        CITNode->opn = OPN_NNL | ( CITNode->opn & OPN_FLD );// stop recursion
        ProcList( CITNode );
        CITNode->opn &= ~OPN_WHAT;
        CITNode->opn |= OPN_NWL;
        // ES:DI now contains value and pointer to result.
        // We are assuming that ES:DI is preserved by the store so that
        // multiple assignments work.
        GStore( typ, size );
        CITNode->opn = OPN_ADR;
    } else if( ( CITNode->opn & OPN_FLD ) && CITNode->value.st.tmp_id != NULL ){
        // we've calculated a field offset already so add it on
        ChkValidParm( CITNode );
        EmitOp( FIELD_ADDR );
        SymRef( CITNode->value.st.tmp_id );
        OpnRef( CITNode );
        GStore( typ, size );
    } else {
        ChkValidParm( CITNode );
        pop = POPOPS + ParmType( typ, size ) - 1;
        if( SymIndirect( flags, typ ) ) {
            pop += POP_INDIRECT;
        }
        ReqStkDepth( GetStkEntries( typ ) );
        EmitOp( pop );
        OpnRef( CITNode );
        DecStkDepth( typ );
    }
}


static  void    FromStackToAddr( itnode *itptr ) {
//================================================

// remove an RCB or SCB from stack 'cause we want the addr of it

    switch( itptr->typ ) {
    case TY_CHAR:
        // Subscripting a character array leaves the SCB
        // on the stack - we want the address.
        itptr->sym_ptr = StaticAlloc( sizeof( string ), TY_NO_TYPE );
        itptr->opn = OPN_VAL | OPN_NNL;
        itptr->flags = itptr->sym_ptr->ns.flags;
        // pop scb and assume addr is left in es:di or edi
        EmitOp( POP_SCB );
        OpnRef( itptr );
        break;
    case TY_STRUCTURE:
        // Subscripting a structure array leaves the RCB
        // on the stack - we want the address.
        itptr->sym_ptr = StaticAlloc( sizeof( rcb ), TY_NO_TYPE );
        itptr->opn = OPN_VAL | OPN_NNL;
        itptr->flags = itptr->sym_ptr->ns.flags;
        // pop rcb and assume addr is left in es:di or edi
        EmitOp( POP_RCB );
        OpnRef( itptr );
        break;
    }
}


void    PushFieldOpn( itnode *itptr, bool want_check, bool want_value ) {
//=======================================================================

// we want to value of itptr; itptr has an offset tmp hanging off of it

    switch( itptr->typ ) {
    case TY_CHAR:
        EmitOp( FIELD_ADDR_PUSH_SCB );
        SymRef( itptr->value.st.tmp_id );
        OpnRef( itptr );
        if( !want_value ) {
            FromStackToAddr( itptr );
        }
        break;
    case TY_STRUCTURE:
        EmitOp( FIELD_ADDR_PUSH_RCB );
        SymRef( itptr->value.st.tmp_id );
        OpnRef( itptr );
        if( !want_value ) {
            FromStackToAddr( itptr );
        }
        break;
    default:
        EmitOp( FIELD_ADDR );
        SymRef( itptr->value.st.tmp_id );
        OpnRef( itptr );
        if( want_value ) {
            if( want_check ) {
                GUFValue( itptr->typ, itptr->size );
            } else {
                GValue( itptr->typ, itptr->size );
            }
        }
    }
}


static  void    PushOp( itnode *itptr, bool want_value, bool want_check ) {
//=========================================================================

// Generate a push of an operand or its address.

    unsigned_16 push;
    unsigned_16 flags;
    unsigned_16 pushi;
    int         size;
    byte        typ;
    byte        what;
    byte        where;
    bool        is_field;

    if( itptr->opn == OPN_PHI ) return;
    where = itptr->opn & OPN_WHERE;
    if( where == OPN_SAFE ) {
        if( want_value ) return;
        FromStackToAddr( itptr );
        SetOpn( itptr, OPN_SAFE );
        return;
    }

    typ = itptr->typ;
    size = itptr->size;
    flags = itptr->flags;
    what = itptr->opn & OPN_WHAT;
    if( ( what == OPN_NWL ) || ( what == OPN_ASS ) ) {
        itptr->opn = OPN_NNL | ( itptr->opn & OPN_FLD );
        ProcList( itptr );
        itptr->opn &= ~OPN_WHAT;
        itptr->opn |= what;
        is_field = (itptr->opn & OPN_FLD) && (itptr->value.st.tmp_id == NULL);
        if( ( ( flags & SY_CLASS ) == SY_VARIABLE ) && !is_field ) {
            switch( typ ) {
            case TY_STRUCTURE:
            case TY_CHAR:
                if( !want_value ) {
                    FromStackToAddr( itptr );
                }
                break;
            default:
                if( want_value ) {
                    // Subscripting an array leaves the pointer to the
                    // result - we want the value.
                    if( want_check && ChkUDef( itptr ) ) {
                        GUValue( typ, size );
                    } else {
                        GValue( typ, size );
                    }
                }
            }
        } else if( typ == TY_STRUCTURE && !want_value ) {
            // must be a function result
            FromStackToAddr( itptr );
        }
    } else if( itptr->opn & OPN_FLD ) {
        if( itptr->value.st.tmp_id != NULL ) {
            // we've calculated a field offset already
            PushFieldOpn( itptr, want_check && ChkUDef( itptr ), want_value );
        } else {
            PushConst( itptr->value.intstar4 );
        }
    } else if( where == OPN_ADR ) {
        switch( typ ) {
        case TY_CHAR:
            EmitOp( RT_CHAR_SUBSCR );
            break;
        case TY_STRUCTURE:
            EmitOp( STRUCT_SUBSCR );
            SymRef( itptr->sym_ptr->ns.xt.sym_record );
            break;
        default:
            GValue( typ, size );
            break;
        }
    } else if( ( ( flags & SY_CLASS ) == SY_SUBPROGRAM ) &&
            ( ( flags & SY_SUBPROG_TYPE ) != SY_STMT_FUNC ) ) {
        // must be a function or subroutine
        GBegCall( itptr );
        GEndCall( itptr, 0 );
    } else {
        if( ( flags & SY_CLASS ) == SY_SUBPROGRAM ) {
            if( ( flags & SY_SUBPROG_TYPE ) == SY_STMT_FUNC ) {
                // treat statement function result as a variable
                flags = SY_TYPE | SY_USAGE | SY_VARIABLE;
            }
        }
        if( want_check && ChkUDef( itptr ) ) {
            switch( typ ) {
            case TY_CHAR:
                push = PUSH_SCB;
                if( flags & SY_SUB_PARM ) {
                    push = PUSH_PARM_SCB;
                }
                break;
            case TY_STRUCTURE:
                push = PUSH_RCB;
                if( flags & SY_SUB_PARM ) {
                    push = PUSH_PARM_RCB;
                }
                break;
            default:
                push = UPUSHOPS + ParmType( typ, size ) - 1;
                pushi = PUSH_U_INDIRECT;
                break;
            }
        } else {
            push = PUSHOPS + ParmType( typ, size ) - 1;
            pushi = PUSH_INDIRECT;
        }
        // Note that character and structure variables are never indirect.
        if( SymIndirect( flags, typ ) ) {
            push += pushi;
            if( want_value == FALSE ) {
                push = PUSH_IND_ADDR;
            }
        } else if( want_value == FALSE ) {
            push = PUSH_ADDR;
        }
        if( want_value ) {
            IncStkDepth( typ );
        }
        EmitOp( push );
        OpnRef( itptr );
    }
    SetOpn( itptr, OPN_SAFE );
}


extern  void    PushOpn( itnode *itptr ) {
//========================================

// Generate a push of an operand.
// Also called for target of character assignment.

    PushOp( itptr, TRUE, TRUE );
}


extern  void    PushIOOpn( itnode *itptr ) {
//==========================================

// Generate a push of an IO operand (no undefined variable checking).

    PushOp( itptr, TRUE, FALSE );
}


extern  void    PushAddr( itnode *itptr ) {
//=========================================

// Push the address of the specified operand.

    PushOp( itptr, FALSE, TRUE );
}


extern  void    GenTmpRef( sym_id tmp ) {
//=======================================

// Generate the address of a temporary.

    SymRef( tmp );
}


extern  void    PushTmp( sym_id tmp, int typ ) {
//==============================================

// Generate a push of a temporary.

    IncStkDepth( typ );
    EmitOp( PUSHOPS + ParmType( typ, TypeSize( typ ) ) - 1 );
    GenTmpRef( tmp );
}


extern  void    PopTmp( sym_id tmp, int typ ) {
//=============================================

// Generate a pop of a temporary.

    ReqStkDepth( GetStkEntries( typ ) );
    EmitOp( POPOPS + ParmType( typ, TypeSize( typ ) ) - 1 );
    GenTmpRef( tmp );
    DecStkDepth( typ );
}


extern  void    PopSym( sym_id sym_ptr ) {
//========================================

// Generate a pop of a value into specified symbol.

    EmitOp( POPOPS + ParmType( sym_ptr->ns.typ, sym_ptr->ns.xt.size ) - 1 );
    SymRef( sym_ptr );
    DecStkDepth( sym_ptr->ns.typ );
}
