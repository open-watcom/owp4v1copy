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
* Description:  Generate code for subprogram calls.
*
****************************************************************************/


#include "ftnstd.h"
#include "progsw.h"
#include "opn.h"
#include "opr.h"
#include "cpopt.h"
#include "global.h"
#include "fcgbls.h"
#include "fcodes.h"
#include "parmtype.h"
#include "entry.h"
#include "objutil.h"
#include "tdreloc.h"
#include "arrelt.h"
#include "rcb.h"
#include "genobj.h"
#include "trcback.h"
#include "wf77aux.h"

extern  void            AddConst(itnode *);
extern  void            GBranch(label_id);
extern  void            GLabel(label_id);
extern  void            GenLabRef(label_id);
extern  void            GStmtAddr(sym_id);
extern  void            GISNCall(void);
extern  label_id        NextLabel(void);
extern  void            EmitOp(unsigned_16);
extern  void            EmitNulls(inttarg);
extern  int             ParmType(int,int);
extern  int             ParmCode(itnode *);
extern  sym_id          GTempString(itnode *);
extern  void            PopTmp(sym_id,int);
extern  void            PushOpn(itnode *);
extern  void            PushConst(int);
extern  void            OpnRef(itnode *);
extern  void            SetOpn(itnode *,int);
extern  int             TypeSize(int);
extern  void            BackPatch(obj_ptr);
extern  sym_id          TmpAlloc(int);
extern  sym_id          TmpVar(int,int);
extern  void            AdvanceITPtr(void);
extern  bool            RecNOpn(void);
extern  bool            RecCloseParen(void);
extern  bool            RecColon(void);
extern  void            ProcList(itnode *);
extern  void            FlushStk(void);
extern  void            IncStkDepth(int);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  void            DecStkDepth(int);
extern  sym_id          StaticAlloc(int,int);
extern  void            PushFieldOpn(itnode *,bool,bool);
extern  aux_info        *AuxLookup(sym_id);
extern  void            GConvert(uint,uint);
extern  void            GValue(int,int);
extern  void            GUValue(int,int);
extern  bool            ChkUDef(itnode *);

#define INDIRECT_PARM   0x80
#define VALUE_PARM      0x40
#define DATA_REF_PARM   0x20

// must match values in "PARMTYPE.INC"

#define UNDEF_CHK       0x80    // undefined checking on arg list
#define ARRAY_CHK       0x40    // type checking on arrays
#define SYSTEM_CALL     0x20    // make a system call from watfor subpgm

#if _TARGET == _8086
#define FCODE_ENABLE    0x9a2e  // "seg CS, far call"
#else
#define FCODE_ENABLE    0xe82e  // "seg CS, near call"
#endif

static pass_by     *auxArgInfo = NULL;

sym_id  GTempStruct( itnode *itptr ) {
//====================================

// Generate a static temporary string.

    sym_id      sym_ptr;

    itptr->flags = SY_VARIABLE; // treat it like a variable
    sym_ptr = StaticAlloc( sizeof( rcb ), TY_STRUCTURE );
    sym_ptr->ns.xt.record = itptr->sym_ptr->ns.xt.record;
    return( sym_ptr );
}


void    GBegCall( itnode *itptr ) {
//=================================

// Initialize for subprogram invocation.

    int         count;
    int         typ;
    sym_id      stat;
    aux_info    *aux;

    aux = AuxLookup( itptr->sym_ptr );
    count = AdjCallList( itptr->list, aux );
    FlushStk();
    ChkValidParm( itptr );
    typ = PT_NOTYPE;
    if( ( itptr->flags & SY_SUBPROG_TYPE ) != SY_SUBROUTINE ) {
        typ = ParmType( itptr->typ, itptr->size );
        switch( typ ) {
        case PT_CHAR:
            EmitOp( PUSH_ADDR_STACK );
            stat = GTempString( itptr );
            SymRef( stat );
            break;
        case PT_STRUCT:
            EmitOp( PUSH_ADDR_STACK );
            stat = GTempStruct( itptr );
            SymRef( stat );
            break;
        }
    }
    EmitOp( RT_PROLOGUE );
    OpnRef( itptr );
    // save the symbol table entry for the subprogram
    // - FinishCall() and GParms() need it
    itptr->value.st.sp_id = itptr->sym_ptr;
    if( typ == PT_CHAR || typ == PT_STRUCT ) {
        itptr->sym_ptr = stat;
        SetOpn( itptr, OPN_VAL );
    }
    if( typ != PT_NOTYPE ) {
        IncStkDepth( itptr->typ );
    }
    if( Options & OPT_CHECK ) {
        typ |= UNDEF_CHK;
    }
    if( Options & OPT_ARRCHECK ) {
        typ |= ARRAY_CHK;
    }
    if( aux && ( _IsCall( aux, AUX_SYSCALL ) || _IsCall( aux, AUX_PASCAL ) ||
                 _IsCall( aux, AUX_CDECL ) ) ){
        typ |= SYSTEM_CALL;
    }
    OutU16( ( count << 8 ) | typ );
    if( typ & SYSTEM_CALL ) {
        OutU16( _GetCall( aux ) );
    }

}


void    GEndCall( itnode *itptr, int num_stmts ) {
//================================================

// Finish off a subprogram invocation.

    itnode      *arg;

    if( ( itptr->opn & OPN_WHERE ) == OPN_VAL ) {
        if( (itptr->opn & OPN_FLD) && (itptr->value.st.tmp_id != NULL) ) {
            // structure valued function has an offset calc'd already
            PushFieldOpn( itptr, FALSE, TRUE );
        } else {
            switch( itptr->typ ) {
            case TY_CHAR:
                EmitOp( PUSH_SCB );
                break;
            case TY_STRUCTURE:
                EmitOp( PUSH_RCB );
                break;
            }
            OpnRef( itptr );
        }
    }
    SetOpn( itptr, OPN_SAFE );
    if( num_stmts > 0 ) {
        EmitOp( ALT_RET );
        OutU16( num_stmts );
        arg = itptr->list;
        for(;;) {
            if( ( arg->opn & OPN_WHAT ) == OPN_STN ) {
                GStmtAddr( arg->sym_ptr );
                num_stmts--;
            }
            arg = arg->link;
            if( num_stmts == 0 ) break;
        }
    }
}


static  void    MaybeSaveNode( itnode *node ) {
//=============================================

    if( ( node->opn & OPN_WHERE ) == OPN_SAFE ) {
        switch( node->typ ) {
        case TY_CHAR:
            node->sym_ptr = TmpVar( node->typ, sizeof( string ) );
            break;
        case TY_STRUCTURE:
            node->sym_ptr = TmpVar( node->typ, sizeof( rcb ) );
            break;
        default:
            node->sym_ptr = TmpVar( node->typ, node->size );
            break;
        }
        node->flags = SY_VARIABLE;    // treat it like a variable
        node->opn = OPN_TMP;
        PopTmp( node->sym_ptr, node->typ );
    }
}


void    GArg() {
//==============

// Generate an argument for subprogram, subscript, or substring.

    MaybeSaveNode( CITNode );
}


int     GParms( itnode *sp ) {
//============================

// Process argument list.

    int         num_stmts;
    aux_info    *aux;
    pass_by     *arg;

    num_stmts = 0;
    aux = AuxLookup( sp->value.st.sp_id );
    arg = aux->arg_info;
    for(;;) {
        if( RecNOpn() == FALSE ) {  // consider f()
            if( GParm( arg ) == PC_STATEMENT ) {
                num_stmts++;
            }
        }
        AdvanceITPtr();
        if( RecCloseParen() || RecColon() ) break;
        if( ( arg != NULL ) && ( arg->link != NULL ) ) {
            arg = arg->link;
        }
    }
    return( num_stmts );
}


int     GParm( pass_by *arg ) {
//==============================

// Generate the parm code corresponding to a subprogram parameter.

    int         code;
    int         typ;
    uint        size;
    unsigned_16 flags;

    typ = PT_NOTYPE;
    code = ParmCode( CITNode );
    if( code != PC_STATEMENT ) {
        flags = CITNode->flags;
        if( ( ( flags & SY_CLASS ) != SY_SUBPROGRAM ) ||
            ( ( flags & SY_SUBPROG_TYPE ) != SY_SUBROUTINE ) ) {
            size = CITNode->size;
#if _TARGET == _8086
            if( arg != NULL ) {
                if( (CITNode->typ == TY_INTEGER) && (arg->info & ARG_SIZE_2) ) {
                    size = sizeof( intstar2 );
                }
            }
#endif
            typ = ParmType( CITNode->typ, size );
            if( ( ( flags & SY_CLASS ) == SY_VARIABLE ) &&
                ( CITNode->typ != TY_CHAR ) &&
                ( CITNode->typ != TY_STRUCTURE ) &&
                ( flags & ( SY_SUB_PARM | SY_IN_EC | SY_IN_DIMEXPR ) ) ) {
                code |= INDIRECT_PARM;
            }
        }
        if( arg != NULL ) {
            if( arg->info & PASS_BY_VALUE ) {
                code |= VALUE_PARM;
            } else if( arg->info & PASS_BY_DATA ) {
                code |= DATA_REF_PARM;
            }
        }
    }
    OutU16( ( code << 8 ) | typ );
    if( code != PC_STATEMENT ) {
        OpnRef( CITNode );
    }
    return( code );
}


static  void    GEntryStruct() {
//==============================

    GWF77Id();
    OutObjPtr( NULL );  // pointer to empty dummy argument list.
    GenLabRef( EpiFCode );
    GenLabRef( TBLabel );
    OutObjPtr( NULL );  // for subprogram/entry names
}


void    GFakeSP() {
//=================

// Simulate a subprogram prologue for a program.

    EpiLabel = NextLabel();
    EpiFCode = NextLabel();
    SubProgId->ns.address.la = ObjPtr;
    GEntryStruct();
    if( Options & OPT_DEBUG ) {
        EmitOp( RT_DB_PROLOGUE );
    }
}


static  void    GWF77Id() {
//=========================

// Generate WATFOR-77 subprogram identification.

    OutU16( FCODE_ENABLE );
    if( _GenObjectFile() ) {
        GObjEnable();
        return;
    }
    // WATFOR-77 code will never get called by external code when
    // running stricly load-n-go so just allocate space for the
    // address of WATFOR-77 enable routine (linking to external code is
    // code is achieved by generating object files)
    OutIIPtr();
}


void    GBlockLabel() {
//=====================

// Generate a block data subprogram label.

// It's possible that a block data subprogram can get called:
//
//      block data f
//      end
//
//      program test
//      external f
//      call sam( f )
//      end
//
//      subroutine sam( g )
//      print *, g()
//      end
//
// so we'll generate a NULL pointer to the argument list which will
// then be checked by SPCall.

    NewGItem();
    SProgStart.offset = ObjPtr;
#if _TARGET == _8086
    SProgStart.seg = CurrSeg;
#endif
    SubProgId->ns.address.la = ObjPtr;
    GWF77Id();
    OutInt( NULL );
    EpiFCode = NextLabel();
    TBLabel = NextLabel();
    TBLabelPatched = FALSE;
}


void    GSPProlog() {
//===================

// Generate a subprogram prologue.

    EpiFCode = NextLabel();
    EpiLabel = NextLabel();
    GEPProlog();
}


void    GEPProlog() {
//===================

// Generate an entry point prologue.

    ArgList->id->ns.address.la = ObjPtr;
    GEntryStruct();
    if( Options & OPT_DEBUG ) {
        EmitOp( RT_DB_PROLOGUE );
    }
}


void    GTrcBack() {
//==================

// Generate the traceback struct.

    GLabel( TBLabel );
    // traceback struct
    OutRTPtr();                         // pointer to previous traceback
    if( Options & OPT_DEBUG ) {
        OutInt( TB_LG_DB );             // indicate it's a debugging traceback
    } else {
        OutInt( TB_LG );
    }
    OutObjPtr( NULL );                  // offset to entry name
    EmitNulls( sizeof( obj_ptr ) );     // offset of previous ISN
    GenLabRef( EpiFCode );              // offset to epilogue sequence
    if( Options & OPT_DEBUG ) {
        OutObjPtr( NULL );              // offset of local data area
        EmitNulls( sizeof( obj_ptr ) ); // for calling F-Codes in debugger
    }
}


void    GSFTrcBack() {
//====================

// Generate the traceback struct for a statement function.

    EmitNulls( sizeof( obj_ptr )        // ret addr
        + sizeof( void PGM * ) );       // pointer to previous traceback
    OutInt( TB_LG );                    // indicate it's load and go traceback
    OutObjPtr( NULL );                  // offset to entry name
    EmitNulls( sizeof( obj_ptr ) );     // offset of previous ISN
}


void    GEpilog() {
//=================

// Generate a subprogram epilogue.

    GISNCall();         // <-- for FUNCTION A()
                        //         END
    GLabel( EpiLabel );
    if( ( ( SubProgId->ns.flags & SY_SUBPROG_TYPE ) == SY_FUNCTION ) &&
        ( Options & OPT_CHECK ) ) {
        EmitOp( CHK_RET_VAL );
        GenLabRef( EpiFCode );
    }
    if( Options & OPT_DEBUG ) {
        EmitOp( RT_END_DBUG );
        EmitOp( RT_DB_EPILOGUE );
    }
    GLabel( EpiFCode );
    EmitOp( RT_EPILOGUE );
#if _TARGET == _80386
    // current arg list pointer, return offs/seg storage, ret data area ptr
    EmitNulls( sizeof( obj_ptr ) + sizeof( pgm_ptr ) + sizeof( pgm_ptr ) );
    OutIIPtr();         // my_data_base ptr
#else
    // current arg list pointer, return offs/seg storage
    EmitNulls( sizeof( obj_ptr ) + sizeof( pgm_ptr ) );
#endif
    if( ( SubProgId->ns.flags & SY_SUBPROG_TYPE ) == SY_FUNCTION ) {
        OutRTPtr();                     // in case its a character function
        if( SubProgId->ns.typ == TY_STRUCTURE ) {
            SymRef( SubProgId->ns.xt.sym_record );
        } else {
            OutInt( SubProgId->ns.xt.size );
        }
#if _TARGET == _80386
        BumpPtr( 4 );
#else
        BumpPtr( 6 );
#endif
    }
    OutRTPtr();                         // for subroutine alternate returns
}


void    GEndBlockData() {
//=======================

    GLabel( EpiFCode );
#if _TARGET == _80386
    EmitOp( RT_EPILOGUE );
    // current arg list pointer, return offs/seg storage, ret data area ptr
    EmitNulls( sizeof( obj_ptr ) + sizeof( pgm_ptr ) + sizeof( pgm_ptr ) );
    OutIIPtr();         // my_data_base ptr
#endif
}


void    GReturn() {
//=================

// Generate a return from the program.

    GLabel( EpiLabel );
    if( Options & OPT_DEBUG ) {
        EmitOp( RT_END_DBUG );
        EmitOp( RT_DB_EPILOGUE );
    }
    GLabel( EpiFCode );
    EmitOp( RT_EPILOGUE );
    EmitNulls( sizeof( obj_ptr )// cur arg list pointer (NULL for recurs chk)
        + sizeof( void PGM * )  // return offset and segment storage
#if _TARGET == _80386
        + sizeof( obj_ptr )     // ret_data_base
#endif
                                );
#if _TARGET == _80386
    OutIIPtr();                 // my_data_base
#endif
    // d_arg just has to point at a 0
    BackPatch( SubProgId->ns.address.la + offsetof( sp_entry, d_arg ) );
    EmitNulls( sizeof( intstar4 ) ); // alt return storage
}


void    GGotoEpilog() {
//=====================

// Generate a branch to the epilogue.

    GBranch( EpiLabel );
}


void    GRetIdx() {
//=================

// Generate an alternate return.

    PushOpn( CITNode );
    IncStkDepth( TY_INTEGER );
    EmitOp( SAVERETIDX );
    GenLabRef( EpiFCode );
    DecStkDepth( TY_INTEGER );
}


void    GNullRetIdx() {
//=====================

// No alternate return.

    PushConst( 0 );
    IncStkDepth( TY_INTEGER );
    EmitOp( SAVERETIDX );
    GenLabRef( EpiFCode );
    DecStkDepth( TY_INTEGER );
}


static  void    FinishCALL( itnode *sp ) {
//========================================

    sym_id      sym;

    sym = sp->value.st.sp_id;
    if( ( sym->ns.flags & SY_SUBPROG_TYPE ) == SY_FUNCTION ) {
        // a FUNCTION invoked in a CALL statement
        EmitOp( TRASH_VALUE );
        OutInt( ParmType( sym->ns.typ, sym->ns.xt.size ) );
    }
}


void    GCallNoArgs() {
//=====================

// Generate a call with no arguments.

    PushOpn( CITNode );
    CITNode->opn = OPN_SAFE;
    FinishCALL( CITNode );
}


void    GCallWithArgs() {
//=======================

// Generate a call with arguments.

    PushOpn( CITNode );
    CITNode->opn = OPN_SAFE;
    FinishCALL( CITNode );
}


void    GArgList( entry_pt *arg_list, uint args, uint typ ) {
//===========================================================

// Dump start of an argument list.

    aux_info    *aux;

    aux = AuxLookup( arg_list->id );
    if( aux ) {
        if( _IsCall( aux, AUX_SYSCALL ) || _IsCall( aux, AUX_PASCAL ) ||
            _IsCall( aux, AUX_CDECL ) ) {
            typ |= SYSTEM_CALL;
        }
        auxArgInfo = aux->arg_info;
    } else {
        auxArgInfo = NULL;
    }
    BackPatch( arg_list->id->ns.address.la + offsetof( sp_entry, d_arg ) );
    OutU16( ( args << 8 ) | typ );
    if( typ & SYSTEM_CALL ) {
        OutU16( _GetCall( aux ) );
    }
}


void    GArgInfo( sym_id sym_ptr, uint code, uint typ ) {
//=======================================================

// Dump information for an argument.

    if( ( auxArgInfo != NULL ) && ( auxArgInfo->info & PASS_BY_VALUE ) ) {
        code |= VALUE_PARM;
    }
    OutU16( ( code << 8 ) | typ );
    if( code != PC_STATEMENT ) {
        SymRef( sym_ptr );
    }
    if( ( auxArgInfo != NULL ) && ( auxArgInfo->link != NULL ) ) {
        auxArgInfo = auxArgInfo->link;
    }
}


static  void    PassBy( itnode *node, pass_by *arg ) {
//=====================================================

    if( (node->typ == TY_REAL) && (arg->info & ARG_SIZE_8) ) {
        GConvert( TY_REAL, TY_DOUBLE );
        node->typ = TY_DOUBLE;
        node->size = TypeSize( TY_DOUBLE );
    }
    if( (node->typ == TY_DOUBLE) && (arg->info & ARG_SIZE_4) ) {
        GConvert( TY_DOUBLE, TY_REAL );
        node->typ = TY_REAL;
        node->size = TypeSize( TY_REAL );
    }
}


static  void    ArgArrElt( itnode *node ) {
//=========================================

    node->sym_ptr = TmpAlloc( sizeof( arr_elt_parm ) );
    // Don't change OPN_WHAT so that GParm() can emit the
    // correct parm code.
    SetOpn( node, OPN_TMP );
    EmitOp( POP_ARR_ELT_PARM );
    OpnRef( node );
}


int     AdjCallList( itnode *node, aux_info *aux ) {
//==================================================

// Process the expressions in the call argument list.

    int         count;
    pass_by     *arg;

    count = 0;
    if( node != NULL ) {
        if( aux == NULL ) {
            arg = NULL;
        } else {
            arg = aux->arg_info;
        }
        for(;;) {
            if( node->opr == OPR_COL ) break;
            if( node->opr == OPR_RBR ) break;
            if( node->opn == OPN_PHI ) break;
            ++count;
            switch( node->opn & OPN_WHAT ) {
            case OPN_CON:
                PushOpn( node );
                if( (arg != NULL) && (arg->info & PASS_BY_VALUE) ) {
                    PassBy( node, arg );
                }
                break;
            case OPN_ASS:
            case OPN_NWL:
                if( ( ( node->flags & SY_CLASS ) != SY_SUBPROGRAM ) &&
                    ( node->flags & SY_SUBSCRIPTED ) ) {
                    switch( node->typ ) {
                    case TY_STRUCTURE:
                    case TY_CHAR:
                        EmitOp( PUSH_ADDR_STACK );
                        OpnRef( node );
                        ProcList( node );
                        ArgArrElt( node );
                        break;
                    default:
                        if( (arg != NULL) && (arg->info & PASS_BY_VALUE) ) {
                            ProcList( node );
                            if( ChkUDef( node ) ) {
                                GUValue( node->typ, node->size );
                            } else {
                                GValue( node->typ, node->size );
                            }
                            node->opn &= ~OPN_WHAT;
                            node->opn |= OPN_NNL;
                            SetOpn( node, OPN_SAFE );
                            PassBy( node, arg );
                        } else {
                            EmitOp( PUSH_ADDR_STACK );
                            OpnRef( node );
                            ProcList( node );
                            EmitOp( PUSH_ARRAY_DATA );
                            ArgArrElt( node );
                        }
                        break;
                    }
                } else {
                    PushOpn( node );
                }
                break;
            case OPN_STN:
                break;
            case OPN_ARR:
                ChkValidParm( node );
                break;
            default: // OPN_NNL
                ChkValidParm( node );
                if( ( node->opn & OPN_FLD ) &&
                    ( node->value.st.tmp_id != NULL ) ) {
                    // we have a structure reference... so add on offset
                    switch( node->typ ) {
                    case TY_CHAR:
                        EmitOp( FIELD_ADDR_PUSH_SCB );
                        SymRef( node->value.st.tmp_id );
                        OpnRef( node );
                        node->sym_ptr = TmpAlloc( sizeof( string ) );
                        // Don't change OPN_WHAT so that GParm() can emit the
                        // correct parm code.
                        SetOpn( node, OPN_TMP );
                        EmitOp( POP_SCB );
                        OpnRef( node );
                        break;
                    case TY_STRUCTURE:
                        EmitOp( FIELD_ADDR_PUSH_RCB );
                        SymRef( node->value.st.tmp_id );
                        OpnRef( node );
                        node->sym_ptr = TmpAlloc( sizeof( rcb ) );
                        // Don't change OPN_WHAT so that GParm() can emit the
                        // correct parm code.
                        SetOpn( node, OPN_TMP );
                        EmitOp( POP_RCB );
                        OpnRef( node );
                        break;
                    default:
                        EmitOp( FIELD_ADDR );
                        SymRef( node->value.st.tmp_id );
                        OpnRef( node );
                        node->sym_ptr = TmpAlloc( sizeof( void PGM * ) );
                        // we have to fake the node to look like an indirect
                        // variable
                        EmitOp( POP_ADDR );
                        OpnRef( node );
                        SetOpn( node, OPN_ATMP );
                        node->flags |= SY_SUB_PARM;
                    }
                } else {
                    if( (arg != NULL) && (arg->info & PASS_BY_VALUE) ) {
                        PushOpn( node );
                        PassBy( node, arg );
                    }
                }
                break;
            }
            MaybeSaveNode( node );
            if( ( arg != NULL ) && ( arg->link != NULL ) ) {
                arg = arg->link;
            }
            node = node->link;
        }
    }
    return( count );
}


void    ChkValidParm( itnode *it ) {
//==================================

    if( !( Options & OPT_CHECK ) ) return;
    if( !( it->flags & SY_SUB_PARM ) &&
        !( it->opn & OPN_FLD ) &&
        !( ( ( it->opn & OPN_WHAT ) == OPN_ARR ) &&
           _Allocatable( it->sym_ptr ) ) ) return;
    // for arrays, assumption is that the origin field is first in adv
    EmitOp( CHK_VALID_PARM );
    OpnRef( it );
}
