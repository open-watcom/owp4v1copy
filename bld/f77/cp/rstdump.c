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
// RSTDUMP    : Resident symbol table dump module.
//

#include "ftnstd.h"
#include "ecflags.h"
#include "errcod.h"
#include "undef.h"
#include "global.h"
#include "progsw.h"
#include "segsw.h"
#include "cpopt.h"

#if _OPT_CG == _OFF
#include "objutil.h"
#include "rcb.h"
#include "structio.h"
#include "parmtype.h"
#include "tdreloc.h"
#include "nmlinfo.h"
#endif

extern  void            Error(int,...);
extern  void            NameErr(int,sym_id);
extern  void            NamNamErr(int,sym_id,sym_id);
extern  void            NameWarn(int,sym_id);
extern  void            NameExt(int,sym_id);
extern  void            Warning(int,...);
extern  void            Extension(int,...);
extern  sym_id          STFreeName(sym_id);
extern  bool            IsIFUsed(int);
extern  bool            IsIntrinsic(unsigned_16);
extern  sym_id          AddSP2GList(sym_id);
extern  sym_id          AddCB2GList(sym_id);
extern  unsigned_32     GetStmtNum(sym_id);
extern  pointer         FMemAlloc(int);
extern  void            FMemFree(pointer);
extern  char            *STGetName(sym_id,char *);
extern  char            *STNmListName(sym_id,char *);
extern  sym_id          STFree(sym_id);
extern  void            FreeSFHeader(sym_id);
#if _OPT_CG == _OFF
extern  void            FreeRList(sym_id);
extern  void            DumpADV(act_dim_list *,uint,bool,bool);
extern  void            *FreeLink(void *);
extern  sym_id          AddVar2GList(sym_id);
extern  int             ParmType(int,int);
#if _TARGET != _8086 && _TARGET != _80386
extern  void            FreeChain(void **);
#endif
extern  void            OutInt(inttarg);
extern  void            EmitNulls(inttarg);
extern  void            GInitAdv(sym_id,bool);
extern  void            TDDump(void);
extern  void            TDDataSize(void);
extern  int             TDTmpAreaSize(void);
extern  void            StartLList(void);
extern  void            EndLList(void);
extern  void            OutVarName(sym_id);
extern  void            OutSPName(sym_id);
extern  void            OutSFName(sym_id);
extern  void            OutSPDumInfo(sym_id);
extern  void            OutSEntryInfo(sym_id);
extern  void            OutAddrInfo(void);
extern  void            GLabel(label_id);
extern  void            GStmtAddr(sym_id);
extern  void            GEndBrTab(void);
extern  void            GEndFmtTab(void);
extern  sym_id          SearchGList(sym_id local);
#else
extern  void            AllocGlobal(unsigned_32,bool);
extern  bool            ForceStatic(unsigned_16);
#endif
extern  void            CkSymDeclared( sym_id );

#if _OPT_CG == _OFF
extern  obj_ptr         ObjPtr;
#else
extern  uint            DataThreshold;
extern  sym_id          ReturnValue;
extern  sym_id          EPValue;
#endif


void    STDump() {
//================

// Dump symbol table.

    sym_id      sym;

    // Merge IFList with NList
    if( IFList != NULL ) {
        sym = IFList;
        while( sym->ns.link != NULL ) {
            sym = sym->ns.link;
        }
        sym->ns.link = NList;
        NList = IFList;
        IFList = NULL;
    }
#if _OPT_CG == _ON
    if( !(ProgSw & PS_DONT_GENERATE) ) return;
#endif
    ProgSw |= PS_SYMTAB_PROCESS;
#if _OPT_CG == _OFF
    if( StNumbers.wild_goto ) {
        DumpBrTable();
    }
    if( StNumbers.var_format ) {
        DumpFmtTable();
    }
    TDDump();
#endif
    DumpStmtNos();              // must be before DumpMagSyms()
#if _OPT_CG == _OFF
    DumpMagSyms();              // can't free up MList (required later)
#endif
    DumpNameLists();            // dump the namelist data structures
#if _OPT_CG == _OFF
    StartLList();               // start local list
#endif
    DumpLocalVars();
#if _OPT_CG == _OFF
    EndLList();                 // end local list
    DumpRList();                // must dump sdefns after DumpLocalVars()
#endif
    DumpConsts();
#if _OPT_CG == _OFF
    DumpLitSCBs();              // dump literal string control blocks
    TDDataSize();
#endif
    DumpStrings();              // dump string data (must be last)
    ProgSw &= ~PS_SYMTAB_PROCESS;
}


#if _OPT_CG == _OFF

static  void    DumpMagSyms() {
//=============================

    sym_id      sym;
    obj_ptr     temp_base;
    sym_id      stat_off;

    temp_base = ObjPtr;
    BumpPtr( TDTmpAreaSize() );
    sym = MList;
    while( sym != NULL ) {
        switch( _MgcClass( sym ) ) {
        case MAGIC_TEMP:
            VarReloc( sym, sym->ns.si.ms.tmp_info.tmp_index + temp_base );
            break;
        case MAGIC_STATIC:
            if( !( sym->ns.flags & SY_PS_ENTRY ) ) {
                VarReloc( sym, ObjPtr );
                stat_off = sym->ns.si.ms.tmp_info.stat_off;
                while( stat_off != NULL ) {
                    VarReloc( stat_off, ObjPtr + stat_off->ns.xt.size );
                    stat_off = STFree( stat_off );
                }
                switch( sym->ns.typ ) {
                case TY_CHAR:
                    sym->ns.address.la = ObjPtr + offsetof( string, strptr );
                    OutSCB( sym->ns.xt.size, 1, _Allocatable( sym ) );
                    break;
                case TY_STRUCTURE:
                    sym->ns.address.la = ObjPtr + offsetof( rcb, origin );
                    OutRCB( sym->ns.xt.record, 1 );
                    // see note in dump variable
                    sym->ns.xt.size = sym->ns.xt.record->size;
                    break;
                default:
                    BumpPtr( sym->ns.xt.size );
                    break;
                }
            }
            break;
        default:        // LABEL
            CodeReloc( sym, sym->ns.address.la );
            break;
        }
        sym = sym->ns.link;
    }
}

#endif


static  bool    CkInCommon( sym_id sym ) {
//========================================

// Check for variables in common.

    sym_id      leader;
    com_eq      *eq_ext;
    unsigned_32 offset;
    bool        global_item;

    global_item = FALSE;
    if( sym->ns.flags & SY_IN_EQUIV ) {
        offset = 0;
        leader = sym;
        for(;;) {
            eq_ext = leader->ns.si.va.vi.ec_ext;
            if( eq_ext->ec_flags & LEADER ) break;
            offset += eq_ext->offset;
            leader = eq_ext->link_eqv;
        }
        if( ( eq_ext->ec_flags & MEMBER_IN_COMMON ) == 0 ) {
#if _OPT_CG == _ON
            if( !( eq_ext->ec_flags & EQUIV_SET_ALLOC ) ) {
                if( ForceStatic(sym->ns.flags) || !(Options & OPT_AUTOMATIC) ) {
                    if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
                        AllocGlobal( eq_ext->high - eq_ext->low,
                             (eq_ext->ec_flags & MEMBER_INITIALIZED) != 0 );
                        eq_ext->ec_flags |= EQUIV_SET_ALLOC;
                    }
                }
            }
#else
            global_item = TRUE;
        } else {
            AddComOffset( eq_ext->com_blk, eq_ext->offset + offset, sym );
#endif
        }
#if _OPT_CG == _OFF
    } else {
        // symbol in common and NOT equivalenced
        AddComOffset( sym->ns.si.va.vi.ec_ext->com_blk,
                      sym->ns.si.va.vi.ec_ext->offset, sym );
#endif
    }
    return( global_item );
}


static  bool    DumpVariable( sym_id sym ) {
//==========================================

// Allocate space for the given variable.

    unsigned_16         flags;
    act_dim_list        *dim_list;
    bool                global_item;
    bool                cp_reloc; // is array compile-time relocatable

    global_item = FALSE;
    flags = sym->ns.flags;
    CkDataOk( sym );
    if( !(flags & (SY_SUB_PARM | SY_DATA_INIT | SY_IN_EC)) ) {
        if( (flags & SY_REFERENCED) && !(sym->ns.xflags & SY_DEFINED) ) {
            NameWarn( VA_USED_NOT_DEFINED, sym );
        }
    }
    if( ( flags & SY_IN_EQUIV ) &&
        ( sym->ns.si.va.vi.ec_ext->ec_flags & LEADER ) &&
        ( ( sym->ns.si.va.vi.ec_ext->ec_flags & ES_TYPE ) == ES_MIXED ) ) {
        Extension( EV_MIXED_EQUIV );
    }
    if( flags & SY_SUBSCRIPTED ) {
        dim_list = sym->ns.si.va.dim_ext;
        cp_reloc = !( flags & SY_SUB_PARM ) && !_Allocatable( sym );
        if( cp_reloc && ( dim_list->num_elts == 0 ) ) {
            NameErr( SV_ARR_PARM, sym );
        } else {
#if _OPT_CG == _OFF
            Local2GblReloc( sym, ObjPtr + offsetof( lg_adv, origin ) );
            GInitAdv( sym, cp_reloc );
            if( sym->ns.typ == TY_STRUCTURE ) {
                // this is output after the ADV so that it can be used
                // by the debugger and by fcsubpgm to compare arrays of
                // structs passed as args
                SymRef( sym->ns.xt.record );
                // We have to move the size into ns.xt.size so that we know how
                // much room to allocate for it in the global data area.
                sym->ns.xt.size = sym->ns.xt.record->size;
            }
#endif
            if( cp_reloc ) {
                global_item = TRUE;
                if( flags & SY_IN_EC ) {
                    global_item = CkInCommon( sym );
#if _OPT_CG == _ON
                } else if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
                    if( ForceStatic( flags ) || !(Options & OPT_AUTOMATIC) ) {
                        if( _SymSize( sym ) * _ArrElements( sym ) >
                            DataThreshold ) {
                            AllocGlobal( _SymSize( sym )*_ArrElements( sym ),
                                         (flags & SY_DATA_INIT) != 0 );
                        }
                    }
#endif
                }
            }
        }
    } else if( sym->ns.typ == TY_STRUCTURE ) {
#if _OPT_CG == _OFF
        Local2GblReloc( sym, ObjPtr + offsetof( rcb, origin ) );
        OutRCB( sym->ns.xt.record, ( flags & SY_SUB_PARM ) == 0 );
        // We have to move the size into ns.xt.size so that we know how
        // much room to allocate for it in the global data area.
        sym->ns.xt.size = sym->ns.xt.record->size;
#endif
        if( ( flags & SY_SUB_PARM ) == 0 ) {
            if( (flags & SY_IN_DIMEXPR) && (flags & SY_IN_COMMON) == 0 ) {
                NameErr( SV_ARR_DECL, sym );
            } else {
                global_item = TRUE;
                if( flags & SY_IN_EC ) {
                    global_item = CkInCommon( sym );
#if _OPT_CG == _ON
                } else if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
                    if( ForceStatic( flags ) || !(Options & OPT_AUTOMATIC) ) {
                        if( sym->ns.xt.record->size > DataThreshold ) {
                            AllocGlobal( sym->ns.xt.record->size,
                                         (flags & SY_DATA_INIT) != 0 );
                        }
                    }
#endif
                }
            }
        }
    } else if( sym->ns.typ == TY_CHAR ) {
        if( ( flags & SY_SUB_PARM ) == 0 ) {
            global_item = !_Allocatable( sym );
            if( ( sym->ns.xt.size == 0 ) && !_Allocatable( sym ) ){
                NameErr( CV_CHARSTAR_ILLEGAL, sym );
            } else {
                if( _Allocatable( sym ) ) {
                    Extension( VA_ALLOCATABLE_STORAGE, sym->ns.name );
                }
#if _OPT_CG == _OFF
                Local2GblReloc( sym, ObjPtr + offsetof( string, strptr ) );
                OutSCB( sym->ns.xt.size, 1, _Allocatable( sym ) );
#endif
                if( flags & SY_IN_EC ) {
                    global_item = CkInCommon( sym );
#if _OPT_CG == _ON
                } else if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
                    if( ForceStatic( flags ) || !(Options & OPT_AUTOMATIC) ) {
                        AllocGlobal( sym->ns.xt.size,
                            (flags & SY_DATA_INIT) != 0 );
                    }
#endif
                }
            }
#if _OPT_CG == _OFF
        } else {
            Local2GblReloc( sym, ObjPtr + offsetof( string, strptr ) );
            OutSCB( sym->ns.xt.size, 0, 0 );
#endif
        }
    } else if( ( flags & ( SY_IN_EC | SY_SUB_PARM ) ) == 0 ) {
        if( flags & SY_IN_DIMEXPR ) {
            NameErr( SV_ARR_DECL, sym );
#if _OPT_CG == _OFF
        } else {
            VarReloc( sym, ObjPtr );
            Undefined( sym->ns.xt.size );
#endif
        }
    } else {
#if _OPT_CG == _OFF
        Local2GblReloc( sym, ObjPtr );
        if( flags & SY_SUB_PARM ) {
            OutRTPtr();                 // sub parms relocated at run-time
        } else {
            OutIIPtr();                 // common/equiv relocate at comp time
            global_item = CkInCommon( sym );
        }
#else
        if( ( flags & SY_SUB_PARM ) == 0 ) {
            global_item = CkInCommon( sym );
        }
#endif
    }
    return( global_item );
}


static  void    DumpLocalVars() {
//===============================

// Dump local variables.

    unsigned_16 flags;
    unsigned_16 class;
    unsigned_16 subprog_type;
    sym_id      sym;

    sym = NList;
    while( sym != NULL ) {
        flags = sym->ns.flags;
        class = flags & SY_CLASS;
        if( class == SY_VARIABLE ) {
#if _OPT_CG == _ON
            if( (sym != ReturnValue) && (sym != EPValue) ) {
#endif
                CkSymDeclared( sym );
                if( (flags & (SY_REFERENCED | SY_IN_EC)) == 0 ) {
                    UnrefSym( sym );
                }
#if _OPT_CG == _OFF
                OutVarName( sym );
                if( DumpVariable( sym ) ) {
                    AddVar2GList( sym );
                }
#else
                DumpVariable( sym );
            }
#endif
        } else if( class == SY_SUBPROGRAM ) {
            if( ( flags & ( SY_REFERENCED | SY_EXTERNAL ) ) == 0 ) {
                UnrefSym( sym );
            }
#if _OPT_CG == _OFF
            if( flags & SY_PS_ENTRY ) { // a subroutine or function
                OutSPName( sym );
            }
#endif
            subprog_type = flags & SY_SUBPROG_TYPE;
            if( subprog_type == SY_REMOTE_BLOCK ) {
                if( ( flags & SY_RB_DEFINED ) == 0 ) {
                    NameErr( SP_RB_UNDEFINED, sym );
#if _OPT_CG == _OFF
                } else {
#if (_TARGET == _370) || (_TARGET == _VAX)
                    OutAddrInfo();
#endif
                    CodeReloc( sym, sym->ns.address.la );
#endif
                }
            } else if( subprog_type == SY_STMT_FUNC ) {
#if _OPT_CG == _OFF
                OutSFName( sym );
                VarReloc( sym, ObjPtr );
                if( sym->ns.typ == TY_CHAR ) {
                    OutSCB( 0, 0, 0 );
                } else {
                    BumpPtr( sym->ns.xt.size );
                }
                FreeSFHeader( sym );
#endif
            } else if( subprog_type == SY_BLOCK_DATA ) {
                AddSP2GList( sym );
#if _OPT_CG == _OFF
            } else if( flags & SY_SUB_PARM ) {
                OutSPDumInfo( sym );
                DataReloc( sym, ObjPtr );
                OutRTPtr();
#endif
            } else if( !IsIntrinsic( flags ) ) {
#if _OPT_CG == _OFF
                if( sym->ns.reloc_chain.lr ) {
                    OutAddrInfo();
                    Local2GblReloc( sym, ObjPtr );
                    OutIIPtr();
                }
#endif
                AddSP2GList( sym );
            } else if( IsIFUsed( sym->ns.si.fi.index ) ) {
#if _OPT_CG == _OFF
                SetIFName( sym );
                OutAddrInfo();
                Local2GblReloc( sym, ObjPtr );
                OutIIPtr();
#endif
                AddSP2GList( sym );
            }
        } else { // if( class == SY_PARAMETER ) {
            if( ( flags & SY_REFERENCED ) == 0 ) {
                UnrefSym( sym );
            }
        }
        sym = sym->ns.link;
    }
    // Common block list must be dumped after the name list so that the
    // SY_COMMON_INIT bit gets set in the common block flags
    // before we add the common block to the global list so that we can
    // detect whether common blocks appear in more than one block data
    // subprogram.
    sym = BList;
    while( sym != NULL ) {    // common block list
#if _OPT_CG == _OFF
        if( SgmtSw & SG_BIG_SAVE ) {
#else
        if( ( SgmtSw & SG_BIG_SAVE ) || (Options & OPT_SAVE) ) {
#endif
            sym->ns.flags |= SY_SAVED;
        }
        AddCB2GList( sym );
        sym = STFreeName( sym );
    }
    BList = NULL;
    // Free NList after processing it since we need to compute offsets
    // of equivalenced names in the equivalence set
    while( NList != NULL ) {
        NList = STFreeName( NList );
    }
}


#if _OPT_CG == _OFF

static  void    Local2GblReloc( sym_id sym, obj_ptr location ) {
//==============================================================

// Do local relocation and link into global relocation chain.

#if _TARGET != _8086 && _TARGET != _80386
    sym_reloc   *curr_reloc;

    DataReloc( sym, ObjPtr );
    FreeChain( &sym->ns.reloc_chain );
    curr_reloc = FMemAlloc( sym_reloc );
    curr_reloc->link = NULL;
    curr_reloc->head = location;
    sym->ns.reloc_chain = curr_reloc;
#else
    DataReloc( sym, ObjPtr );
    sym->ns.reloc_chain.lr = location;
#endif
}

#endif


static  void    UnrefSym( sym_id sym ) {
//======================================

// Issue a warning for unreferenced symbol.

    if( !(sym->ns.xflags & SY_FAKE_REFERENCE) ) {
        NameWarn( VA_UNREFERENCED, sym );
    }
}


#if _OPT_CG == _OFF

static  void    AddComOffset( sym_id com, unsigned_32 offset, sym_id sym ) {
//==========================================================================

// Add a relocation entry for the given offset for a common block.

    sym_id      g_com;
    com_reloc   *reloc;

    g_com = SearchGList( com );
    if( ( g_com == NULL ) || ( ( g_com->ns.flags & SY_CLASS ) != SY_COMMON ) ) {
        g_com = com;
    }
    reloc = g_com->ns.reloc_chain.cr;
    for(;;) {
        if( reloc == NULL ) {
            reloc = FMemAlloc( sizeof( com_reloc ) );
            reloc->link = g_com->ns.reloc_chain.cr;
            g_com->ns.reloc_chain.cr = reloc;
            reloc->offset = offset;
#if _TARGET == _370
            reloc->in_data = FALSE;
#endif
            _NULLTargAddr( reloc->reloc_chain );
            break;
        }
        if( reloc->offset == offset ) break;
        reloc = reloc->link;
    }
#if _TARGET == _370
    reloc->in_data |= ( sym->ns.flags & SY_DATA_INIT ) != 0;
#endif
    LinkComReloc( sym, reloc );
}

#endif


static  void    CkDataOk( sym_id sym ) {
//======================================

// Check that a data initialized variable in common is OK.

    sym_id      name;
    com_eq      *eq_ext;
    unsigned_16 flags;

    flags = sym->ns.flags;
    if( ( flags & SY_DATA_INIT ) == 0 ) return;
    name = sym;
    if( flags & SY_IN_EC ) {
        if( flags & SY_IN_COMMON ) {
            sym = sym->ns.si.va.vi.ec_ext->com_blk;
        } else { // if( flags & SY_IN_EQUIV ) {
            for(;;) {
                eq_ext = sym->ns.si.va.vi.ec_ext;
                if( ( eq_ext->ec_flags & LEADER ) != 0 ) break;
                sym = eq_ext->link_eqv;
            }
            if( ( eq_ext->ec_flags & MEMBER_IN_COMMON ) == 0 ) {
                if( ProgSw & PS_BLOCK_DATA ) {
                    NameErr( BD_BLKDAT_NOT_COMMON, name );
                }
                return;
            } else {
                sym = eq_ext->com_blk;
            }
        }
        if( ( sym->ns.flags & SY_BLANK_COMMON ) != 0 ) {
            NameErr( DA_BLANK_INIT, name );
        } else {
            sym->ns.flags |= SY_COMMON_INIT;
            if( ( ProgSw & PS_BLOCK_DATA ) == 0 ) {
                NameExt( CM_COMMON, name );
            }
        }
    } else {
        if( ProgSw & PS_BLOCK_DATA ) {
            NameErr( BD_BLKDAT_NOT_COMMON, name );
        }
    }
}


bool    StmtNoRef( sym_id sn ) {
//==============================

// Check if statement number has been referenced.

    if( StNumbers.wild_goto ) return( TRUE );
    if( ( sn->st.flags & SN_AFTR_BRANCH ) == 0 ) return( TRUE );
    if( sn->st.flags & ( SN_ASSIGNED | SN_BRANCHED_TO ) ) return( TRUE );
    return( FALSE );
}


static  void    DumpStmtNos() {
//=============================

// Check that statement numbers are all defined.

    sym_id      sn;
    unsigned_16 sn_flags;
    unsigned_32 st_number;

    sn = SList;
    while( sn != NULL ) {
        sn_flags = sn->st.flags;
        st_number = GetStmtNum( sn );
        if( ( sn_flags & SN_DEFINED ) == 0 ) {
            Error( ST_UNDEFINED, st_number, sn->st.line );
        } else {
            if( !StmtNoRef( sn ) ) {
                Warning( ST_UNREFERENCED, st_number );
            }
#if _OPT_CG == _OFF
            CodeReloc( sn, sn->st.address );
#endif
        }
        sn = STFree( sn );
    }
    SList = NULL;
}


#if _OPT_CG == _OFF

static  void    DumpBrTable() {
//=============================

// Dump the branch table ( for wild goto's ).

    sym_id      ste_ptr;

    GLabel( StNumbers.branches );
    ste_ptr = SList;
    while( ste_ptr != NULL ) {
        if( ( ste_ptr->st.flags & SN_ASSIGNED ) &&
            ( ( ste_ptr->st.flags & SN_BAD_BRANCH ) == 0 ) ) {
            GStmtAddr( ste_ptr );
            OutU16( ste_ptr->st.block ); // for branch into structure check
        }
        ste_ptr = ste_ptr->st.link;
    }
    GEndBrTab();
}


static  void    DumpFmtTable() {
//==============================

// Dump the format table ( for variable format labels ).

    sym_id      ste_ptr;

    GLabel( StNumbers.formats );
    ste_ptr = SList;
    while( ste_ptr != NULL ) {
        if( ( ste_ptr->st.flags & SN_FORMAT ) &&
            ( ste_ptr->st.flags & SN_ASSIGNED ) ) {
            GStmtAddr( ste_ptr );
        }
        ste_ptr = ste_ptr->st.link;
    }
    GEndFmtTab();
}

#endif


static  void    DumpConsts() {
//============================

// Dump constants from the symbol table.

#if _OPT_CG == _OFF
    byte        typ;
#endif

    while( CList != NULL ) {
#if _OPT_CG == _OFF
        typ = CList->cn.typ;
        ConstReloc( CList, ObjPtr );
        if( _IsTypeLogical( typ ) ) {
            CList->cn.value.logstar4 = _LogValue( CList->cn.value.logstar4 );
        }
        OutBytes( (char *)&CList->cn.value, CList->cn.size );
#endif
        CList = STFree( CList );
    }
}


#if _OPT_CG == _OFF

static  void    DumpLitSCBs() {
//=============================

// Dump string control blocks for constant literals.

    sym_id     ste_ptr;

    ste_ptr = LList;
    while( ste_ptr != NULL ) {
        DataReloc( ste_ptr, ObjPtr );
        ste_ptr->lt.address = ObjPtr + offsetof( string, strptr);
        OutSCB( ste_ptr->lt.length, 1, 0 );
        ste_ptr = ste_ptr->lt.link;
    }
}

#endif


static  void    DumpStrings() {
//=============================

// Dump constant literals and space for static temporaries.

    while( LList != NULL ) {
#if _OPT_CG == _OFF
        LitSCBReloc( LList );
        OutBytes( &LList->lt.value, LList->lt.length );
#endif
        LList = STFree( LList );
    }
    for( ; MList != NULL; MList = STFree( MList ) ) {
        // check if shadow for function return value
        if( MList->ns.flags & SY_PS_ENTRY ) continue;
#if _OPT_CG == _OFF
        if( _MgcClass( MList ) == MAGIC_STATIC ) {
            switch( MList->ns.typ ) {
            case TY_CHAR:
                TmpSCBReloc( MList );
                BumpPtr( MList->ns.xt.size );
                break;
            case TY_STRUCTURE:
                TmpRCBReloc( MList );
                // record size put here earlier... see DumpVariable
                BumpPtr( MList->ns.xt.size );
                break;
            }
        }
#else
        if( MList->ns.typ == TY_CHAR ) {
            if( ( MList->ns.xt.size != 0 ) && !( Options & OPT_AUTOMATIC ) ) {
                AllocGlobal( MList->ns.xt.size, FALSE );
            }
        }
#endif
    }
}


static  void    DumpNameLists() {
//===============================

// Dump NAMELIST information.

    char        buff1[MAX_SYMLEN+1];
    char        buff2[MAX_SYMLEN+1];
    sym_id      nl;
    grp_entry   *ge;
    sym_id      sym;
    unsigned_16 flags;
#if _OPT_CG == _OFF
    byte        nl_info;
#endif

    nl = NmList;
    while( nl != NULL ) {
#if _OPT_CG == _OFF
        ConstReloc( nl, ObjPtr );
        OutByte( nl->nl.name_len );
        OutBytes( nl->nl.name, nl->nl.name_len );
#endif
        ge = nl->nl.group_list;
        while( ge != NULL ) {
            sym = ge->sym;
            flags = sym->ns.flags;
            // do error checks
            if( ((flags & SY_CLASS) != SY_VARIABLE) || (flags & SY_SUB_PARM) ||
                (sym->ns.typ == TY_STRUCTURE) || _Allocatable( sym ) ) {
                STGetName( sym, buff1 );
                STNmListName( nl, buff2 );
                Error( VA_BAD_SYM_IN_NAMELIST, buff1, buff2 );
            }

#if _OPT_CG == _OFF
            OutByte( sym->ns.name_len );
            OutBytes( sym->ns.name, sym->ns.name_len );
            nl_info = 0;
            _SetNMLType( nl_info, ParmType( sym->ns.typ, sym->ns.xt.size ) );
            if( sym->ns.flags & SY_SUBSCRIPTED ) {
                _SetNMLSubScrs( nl_info,
                                _DimCount( sym->ns.si.va.dim_ext->dim_flags ) );
                nl_info |= NML_LG_ADV;  // indicate it's a load and go ADV
            }
            OutByte( nl_info );
            OutRTPtr();
            ge = FreeLink( ge ); // free the grp_entry
#else
            ge = ge->link;
#endif
        }
#if _OPT_CG == _OFF
        OutByte( 0 );
        nl = STFree( nl );      // free the namelist
#else
        nl = nl->nl.link;
#endif
    }
#if _OPT_CG == _OFF
    NmList = NULL;
#endif
}


#if _OPT_CG == _OFF

static  bool    RefSDefn( sym_id sd ) {
//=====================================

    // To indicate that a structure definition has been dumped already, we
    // set the size field to zero and put the address in the reloc_chain
    // field.  See DumpSDefn().
    if( sd->sd.size == 0 ) {    // this is a backward reference
        OutDataPtr( sd->sd.reloc_chain );
        return( FALSE );
    }
    SymRef( sd );               // this is a forward reference
    return( TRUE );
}


static  void    DumpFieldADV( field *fd, inttarg size ) {
//=======================================================

// Emit the ADV for the specified field.

    OutByte( fd->name_len );
    OutBytes( fd->name, fd->name_len );
    LclReloc( fd->dim_ext->l.reloc_chain, ObjPtr );
    DumpADV( fd->dim_ext, size, TRUE, FALSE );
}


static  sym_id  *FindBiggestMap( field *fd ) {
//============================================

    inttarg     size;
    sym_id      *map_walk;
    sym_id      *big_map;

    size = 0;
    map_walk = &fd->xt.record;
    while( *map_walk != NULL ) { // find biggest map
        if( (*map_walk)->sd.size > size ) {
            size = (*map_walk)->sd.size;
            big_map = map_walk;
        }
        map_walk = &(*map_walk)->sd.link;
    }
    return( big_map );
}


static  bool    DumpSDefn( sym_id sd ) {
//======================================

// dump a structure definition

    byte        info;
    field       *fd;
    inttarg     size;
    sym_id      map;
    sym_id      *big_map;
    bool        forward_ref;

    // Since structures can be referenced forward (i.e., from fcodes and
    // from rcbs) we do the relocation here.  However, structures can
    // also be referenced backwards from other structure definitions.  To
    // indicate that a structure definition has been dumped already, we
    // set the size field to zero and put the address in the reloc_chain
    // field.  See RefSDefn().
    if( sd->sd.size == 0 ) return( FALSE );     // have already dumped sdefn
                                                // no refs to this sdefn yet
    if( sd->sd.reloc_chain == NULL ) return( FALSE );
    ConstReloc( sd, ObjPtr );
    sd->sd.reloc_chain = ObjPtr;                // addr in reloc_chain
    OutInt( sd->sd.size );
    sd->sd.size = 0;                            // size field to 0
    EmitNulls( 2 * sizeof( obj_ptr ) );         // for back_lnik, back_offs
    OutU32( 0 );                                // for elts_left
    forward_ref = FALSE;
    fd = sd->sd.fields;
    while( fd != NULL ) {
        info = 0;
        switch( fd->typ ) {
        case TY_UNION:
            OutByte( SDEFN_UNION | PT_STRUCT );
            big_map = FindBiggestMap( fd );
            map = *big_map;                     // save pointer to biggest map
            *big_map = (*big_map)->sd.link;     // unlink big_map from the union
            size = map->sd.size;
#if 1
            forward_ref |= RefSDefn( map );     // reference the biggest map
            map->sd.link = RList;               // put biggest map on RList
            RList = map;
#else
            // this code outputs a reference to every map in the union
            OutInt( num_maps );
            map->sd.link = fd->xt.record;       // move biggest map to front
            fd->xt.record = NULL;
            map_walk = &map;
            while( *map_walk != NULL ) {        // output map references
                forward_ref |= RefSDefn( *map_walk );
                map_walk = &(*map_walk)->sd.link;
            }
            *map_walk = RList;                  // queue up maps on RList
            RList = map;
#endif
            break;
        case TY_STRUCTURE:
            if( fd->dim_ext != NULL ) {
                info = SDEFN_ARRAY;
            }
            OutByte( info | PT_STRUCT );
            size = fd->xt.record->size;
            forward_ref |= RefSDefn( fd->xt.record );
            break;
        case TY_CHAR:
            if( fd->dim_ext != NULL ) {
                info = SDEFN_ARRAY;
            }
            OutByte( info | PT_CHAR );
            size = fd->xt.size;
            OutInt( size );
            break;
        default:
            if( fd->dim_ext != NULL ) {
                info = SDEFN_ARRAY;
            }
            OutByte( info | ParmType( fd->typ, fd->xt.size ) );
            size = fd->xt.size;
        }
        if( info ) {
            DumpFieldADV( fd, size );
        }
        fd = fd->link;
    }
    OutByte( PT_NOTYPE );       /* end of list */
    return( forward_ref );
}


static void DumpRList( void ) {
//=============================

    sym_id      saved_rlist;
    sym_id      *rlist_walk;
    bool        resolve_forward_refs;

    // DumpSDefn treats RList like a queue of map definitions that need
    // to be output.
    saved_rlist = RList;
    do {
        // We iterate over this inner loop until all forward references
        // have been resolved.  This allows us to output only those
        // structure definitions which are actually referenced from within
        // the subprogram.
        rlist_walk = &saved_rlist;
        resolve_forward_refs = FALSE;
        while( *rlist_walk != NULL ) {
            RList = NULL;                       // initialize the queue
            while( *rlist_walk != NULL ) {      // output definitions
                resolve_forward_refs |= DumpSDefn( *rlist_walk );
                rlist_walk = &(*rlist_walk)->sd.link;
            }

            // At this point, RList will point to a linked list of maps
            // from unions referenced by structure definitions just output.
            // rlist_walk contains the address of the link field of the last
            // entry in our saved_rlist.
            *rlist_walk = RList;

            // Now *rlist_walk points to the list of maps that need to be
            // output.  This list has been attached to our saved_rlist.
        }
    } while( resolve_forward_refs );
    // Now saved_rlist points to every struct fstruct allocated by the
    // compiler.  RList is also NULL.  We couldn't free the structures
    // before since they are needed to do relocation.
    FreeRList( saved_rlist );
}

#endif
