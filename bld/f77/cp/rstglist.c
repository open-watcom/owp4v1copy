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
* Description:  Global symbol table routines.
*
****************************************************************************/


//
// RSTGLIST  : global symbol table routines
//

#include "ftnstd.h"
#include "ecflags.h"
#include "errcod.h"
#include "undef.h"
#include "sdfile.h"
#include "global.h"
#include "progsw.h"
#include "cpopt.h"

#if _OPT_CG == _OFF
#include "objutil.h"
#include "tdreloc.h"
#endif

#include <string.h>

extern  void            ClassNameErr(int,sym_id);
extern  void            ClassErr(int,sym_id);
extern  void            PrevDef(sym_id);
extern  void            NameErr(int,sym_id);
extern  void            NameWarn(int,sym_id);
extern  void            Warning(int,...);
extern  bool            IsIntrinsic(unsigned_16);
extern  char            *STGetName(sym_id,char *);
extern  int             AllocName(int);
extern  intstar4        GetComBlkSize(sym_id);
extern  void            SetComBlkSize(sym_id,intstar4);
extern  pointer         FMemAlloc(int);
extern  void            HashInsert(hash_entry *,unsigned,sym_id *,sym_id);
extern  int             CalcHash(char *,int);
#if _OPT_CG == _OFF
extern  lib_handle      LibSearch(char *);
extern  void            FreeNameList(sym_id);
extern  void            FreeChain(void **);
#if (_TARGET != _80386) && (_TARGET != _8086)
extern  void            ProcComList(sym_id);
#endif
#if _TARGET == _370
extern  int             AlignDWord(void);
#endif
extern  void            Sym2Obj(sym_id);
#if (_TARGET != _80386) && (_TARGET != _8086)
extern  void            SetComBlkId(sym_id);
extern  void            SetSectId(sym_id);
#endif
#if _TARGET == _VAX
extern  void            ObjComAlloc(void);
#endif
#endif

extern  int             HashValue;
// Local variables are only added to the GList for relocation purposes.
// They are never searched for; so we will store them in the bucket
// at HASH_PRIME.  (that's the reason for the +1)
hash_entry              GHashTable[HASH_PRIME + 1];


static  sym_id  LnkNewGlobal( sym_id local ) {
//============================================

// Allocate a global symbol and link it into the global list.

    sym_id      global;
    int         len;

    len = sizeof( symbol ) + AllocName( local->ns.name_len );
    global = FMemAlloc( len );
    memcpy( global, local, len );
    HashInsert( GHashTable, HashValue, &GList, global );
    return( global );
}


#if _OPT_CG == _OFF

sym_id        AddVar2GList( sym_id ste_ptr ) {
//============================================

// Add a variable to the global list.

    sym_id      gbl;
    unsigned_16 flags;

    flags = ste_ptr->ns.flags;
#if _DEVELOPMENT == _OFF
    ste_ptr->ns.name_len = 0;
#endif
    HashValue = HASH_PRIME; // see comment before GHashTable
    gbl = LnkNewGlobal( ste_ptr );
#if _OPT_CG == _OFF
    _NULLTargAddr( gbl->ns.reloc_chain.gr );
#endif
    LinkGblSym( ste_ptr, gbl );
    if( flags & SY_SUBSCRIPTED ) {
        gbl->ns.si.va.dim_ext = NULL;
    }
    if( flags & SY_IN_EQUIV ) {
        ChainEqList( gbl, ste_ptr );
        ste_ptr->ns.si.va.vi.ec_ext = NULL;
    }
    return( gbl );
}

#endif


sym_id        SearchGList( sym_id local ) {
//=========================================

// Search the global list for a symbol.

    sym_id      head;
    sym_id      tail;
    int         name_len;

    name_len = local->ns.name_len;
    HashValue = CalcHash( local->ns.name, name_len );
    head = GHashTable[ HashValue ].h_head;
    if( head == NULL ) return( NULL );
    tail = GHashTable[ HashValue ].h_tail;
    for(;;) {
        if( ( head->ns.name_len == name_len ) &&
            ( memcmp( &local->ns.name, &head->ns.name, name_len ) == 0 ) &&
            ( IsIntrinsic(head->ns.flags) == IsIntrinsic(local->ns.flags) ) ) {
             return( head );
        }
        if( head == tail ) return( NULL );
        head = head->ns.link;
    }
}


sym_id      AddSP2GList( sym_id ste_ptr ) {
//=========================================

// Add a subprogram to the global list.

    sym_id      gbl;
    unsigned_16 flags;
    unsigned_16 subprog;
    unsigned_16 gsubprog;

    flags = ste_ptr->ns.flags;
    subprog = flags & SY_SUBPROG_TYPE;
    gbl = SearchGList( ste_ptr );
    if( gbl == NULL ) {
        gbl = LnkNewGlobal( ste_ptr );
        gbl->ns.flags &= ~SY_REFERENCED;
#if _OPT_CG == _OFF
        _NULLTargAddr( gbl->ns.reloc_chain.gr );
        if( !_GenObjectFile() )
            if( IsIntrinsic( flags ) ) {
                SetIFAddr( ste_ptr->ns.si.fi.index, gbl );
            }
#if _TARGET == _370
        // turn off EXTERNAL flag since we have another use for it
        // at global load/relocation time
        gbl->ns.flags &= ~SY_EXTERNAL;
#endif
#endif
    } else if( ( gbl->ns.flags & SY_CLASS ) != SY_SUBPROGRAM ) {
        PrevDef( gbl );
        return( gbl );
    } else {
        gsubprog = gbl->ns.flags & SY_SUBPROG_TYPE;
        if( gsubprog == SY_FN_OR_SUB ) {
            // We don't know what global symbol is - it could be a
            // function, subroutine or block data subprogram.
            // If we know what the local symbol is then the global symbol
            // becomes what the local symbol is.
            gbl->ns.flags &= ~SY_FN_OR_SUB;
            gbl->ns.flags |= subprog;
        } else if( (gsubprog != subprog) && (subprog != SY_FN_OR_SUB) ) {
            PrevDef( gbl );
            return( gbl );
        }
    }
    if( ( flags & SY_PS_ENTRY ) || ( subprog == SY_BLOCK_DATA ) ) {
        if( gbl->ns.flags & SY_ADDR_ASSIGNED ) {
            if( ( ( subprog != SY_PROGRAM ) && ( subprog != SY_BLOCK_DATA ) ) ||
                ( ( flags & SY_UNNAMED ) == 0 ) ) {
                PrevDef( gbl );
            } else {
                ClassErr( SR_TWO_UNNAMED, gbl );
            }
        } else {
#if _OPT_CG == _OFF
            MakeGblAddr( ste_ptr, gbl );
            // Consider:
            //    SUBROUTINE SAM
            //    CALL MAS( SAM )
            //    END
            // We must link the reference to SAM into the global chain
            // so that the recursion will be detected.
            // Not for functions since you can't pass the address of a
            // function; its value will be passed.
            // But for 386 object files we need to do this all the time
            // so that OutIIPtrs match relocations.
            if( ste_ptr->ns.reloc_chain.lr ) {
                LinkGblSym( ste_ptr, gbl );
            }
#endif
            gbl->ns.flags |= SY_ADDR_ASSIGNED;
        }
#if _OPT_CG == _OFF
    } else if( ste_ptr->ns.reloc_chain.lr ) {
        LinkGblSym( ste_ptr, gbl );
#endif
    }
    return( gbl );
}


sym_id  AddCB2GList( sym_id ste_ptr ) {
//=====================================

// Add a common block to the global list.

    sym_id      gbl;
    unsigned_16 flags;

    flags = ste_ptr->ns.flags;
    gbl = SearchGList( ste_ptr );
    if( gbl == NULL ) {
        gbl = LnkNewGlobal( ste_ptr );
#if _OPT_CG == _OFF
        // Set relocation chain to null in local symbol table entry so
        // the relocation chain doesn't get freed when we free the local
        // symbol table entry.
        ste_ptr->ns.reloc_chain.cr = NULL;
        if( flags & SY_COMMON_INIT ) {
            MakeGblAddr( ste_ptr, gbl );
        }
#endif
    } else if( ( gbl->ns.flags & SY_CLASS ) != SY_COMMON ) {
        PrevDef( gbl );
    } else {
        if( ( gbl->ns.flags & SY_SAVED ) != ( flags & SY_SAVED ) ) {
            gbl->ns.flags |= SY_SAVED;
            if( ( flags & SY_COMMON_LOAD ) == 0 ) {
                NameWarn( SA_COMMON_NOT_SAVED, ste_ptr );
            }
        }
        CkComSize( gbl, GetComBlkSize( ste_ptr ) );
        if( flags & gbl->ns.flags & SY_IN_BLOCK_DATA ) {
            NameErr( CM_BLKDATA_ALREADY, gbl );
        }
#if _OPT_CG == _OFF
        if( flags & SY_COMMON_LOAD ) {
            // If common block loaded from text deck and it has been
            // initialized then reflect it in the gbl entry.
            if( flags & SY_COMMON_INIT ) {
                gbl->ns.flags |= SY_COMMON_ALLOC | SY_COMMON_INIT;
                MakeGblAddr( ste_ptr, gbl );
            }
            gbl->ns.flags |= SY_COMMON_LOAD;
        }
#endif
        gbl->ns.flags |= flags & ( SY_COMMON_INIT | SY_EQUIVED_NAME );
    }
    return( gbl );
}


void    CkComSize( sym_id sym_ptr, unsigned_32 size ) {
//=====================================================

// Check for matching sizes of common blocks.

    char        buff[MAX_SYMLEN+1];
    intstar4    com_size;

    com_size = GetComBlkSize( sym_ptr );
    if( size != com_size ) {
        if( size > com_size ) {
            SetComBlkSize( sym_ptr, size );
        }
        if( ( sym_ptr->ns.flags & SY_COMSIZE_WARN ) == 0 ) {
            // It's nice to give a warning message when the blank common
            // block appears as different sizes even though the standard
            // permits it.
            if( sym_ptr->ns.flags & SY_BLANK_COMMON ) {
                Warning( CM_BLANK_DIFF_SIZE );
            } else {
                STGetName( sym_ptr, buff );
                Warning( CM_NAMED_DIFF_SIZE, buff );
            }
            sym_ptr->ns.flags |= SY_COMSIZE_WARN;
        }
    }
}


#if _OPT_CG == _OFF

void    ChainEqList( sym_id new_location, sym_id local ) {
//========================================================

// Chain the equivalenced variable into the equivalence link.
// Note that both the global and name list must be checked.

    EquivList( GList, new_location, local );
    EquivList( NList, new_location, local );
}


void    EquivList( sym_id list, sym_id new_location, sym_id local ) {
//===================================================================

// Chain the equivalenced variable into the equivalence link.
// Note that both the global and name list must be checked.

    sym_id    ste_ptr;

    ste_ptr = list;
    while( ste_ptr != NULL ) {
        if( ( ( ste_ptr->ns.flags & SY_CLASS ) == SY_VARIABLE ) &&
            ( ( ste_ptr->ns.flags & SY_IN_EC ) != 0 ) &&
            ( ste_ptr->ns.si.va.vi.ec_ext != NULL ) &&
            ( ste_ptr->ns.si.va.vi.ec_ext->link_eqv == local ) ) {
            ste_ptr->ns.si.va.vi.ec_ext->link_eqv = new_location;
        }
        ste_ptr = ste_ptr->ns.link;
    }
}


lib_handle Srch4Undef() {
//=======================

// Check the global list for undefined global symbols.

    sym_id      sym;
    unsigned_16 flags;
    unsigned_16 subprog;
    lib_handle  lp;

    for( sym = GList; sym != NULL; sym = sym->ns.link ) {
        flags = sym->ns.flags;
        if( ( flags & SY_CLASS ) != SY_SUBPROGRAM ) continue;
        subprog = flags & SY_SUBPROG_TYPE;
        if( ( subprog != SY_FUNCTION   ) && ( subprog != SY_SUBROUTINE ) &&
            ( subprog != SY_FN_OR_SUB  ) ) continue;
        if( flags & ( SY_ADDR_ASSIGNED | SY_MACLIB ) ) continue;
#if _TARGET == _370
        if( flags & SY_WEAK_EXTERN ) continue;
#endif
        if( sym->ns.flags & SY_EXTERNAL ) {
            if( sym->ns.reloc_chain.gr.offset == 0 ) {
                if( sym->ns.flags & SY_RELAX_EXTERN ) continue;
            }
        }
        LibMember = &SymBuff;
        STGetName( sym, LibMember );
        lp = LibSearch( LibMember );
        sym->ns.flags |= SY_MACLIB;
        if( lp != NULL ) {
#if _TARGET == _370
            if( flags & ( SY_INTRINSIC | SY_INTERNAL ) ) {
                ProgSw |= PS_LOADING_IF;
            }
#endif
            return( lp );
        }
    }
    return( NULL );
}


unsigned_32     ComBlkAlloc( sym_id com_blk ) {
//=============================================

// Allocate space for a common block.

    unsigned_32 size_alloc;

    size_alloc = 0;
    if( ( com_blk->ns.flags & SY_COMMON_ALLOC ) == 0 ) {
        com_blk->ns.flags |= SY_COMMON_ALLOC;
#if _TARGET == _370
        size_alloc = AlignDWord( 0 );
#endif
        SetComAddr( com_blk );
        size_alloc += GetComBlkSize( com_blk );
    }
#if _TARGET == _8086
    SetCommonBlockSize( size_alloc );
#endif
    return( size_alloc );
}


unsigned_32     RelocVariable( sym_id ste_ptr ) {
//===============================================

// Relocate a variable.

    sym_id      base;
    com_eq      *eq_ext;
    unsigned_16 flags;
    unsigned_32 offset;
    unsigned_32 size_alloc;

    size_alloc = 0;
    offset     = 0;
    base       = ste_ptr;
    flags      = ste_ptr->ns.flags;
    if( flags & SY_IN_EQUIV ) {
        for(;;) {
            eq_ext = base->ns.si.va.vi.ec_ext;
            if( ( eq_ext->ec_flags & LEADER ) != 0 ) break;
            offset += eq_ext->offset;
            base = eq_ext->link_eqv;
        }
        if( ( eq_ext->ec_flags & EQUIV_SET_ALLOC ) == 0 ) {
            eq_ext->ec_flags |= EQUIV_SET_ALLOC;
            size_alloc = eq_ext->high - eq_ext->low;
            SetGblOrg( base );
            eq_ext->offset = GblOffset( base ) - eq_ext->low;
#if _TARGET == _8086
            if( _GenObjectFile() ) {
                SetEQLeadersAddr( base );
            }
#endif
        }
        offset += eq_ext->offset;
#if _TARGET == _8086
        if( _GenObjectFile() ) {
            if( base != ste_ptr ) {
                SetEQVariablesAddr( ste_ptr, base, offset );
            }
            // relocate the equivalenced variable
            base = ste_ptr;
            offset = 0;
        }
#endif
    } else {
        SetGblOrg( base );
        offset = GblOffset( base );
        // Note that TY_STRUCTURE variables have had their size moved
        // into ns.xt.size by DumpVariable in RSTDUMP.
        size_alloc = ste_ptr->ns.xt.size;
        if( flags & SY_SUBSCRIPTED ) {
            size_alloc *= ArrNumElts( MK_PGM( ste_ptr->ns.reloc_chain.gr ) );
        }
#if _TARGET == _8086
        SetCurrentGblAddr( base, &offset );
#endif
    }
#if _TARGET == _370
    GblVarReloc( &ste_ptr->ns.reloc_chain.gr, &base->ns.address.ga, offset,
                 ( flags & SY_DATA_INIT ) != 0 );
#else
    GblVarReloc( &ste_ptr->ns.reloc_chain.gr, &base->ns.address.ga, offset );
#if _TARGET == _8086
    // we only have 64kb segments to work with.
    PackGblData( size_alloc );
#endif
#endif
    return( size_alloc );
}


static  void    ChkUndefined( sym_id sym ) {
//==========================================

    if( sym->ns.flags & SY_ADDR_ASSIGNED ) return;
#if _TARGET == _370
    if( sym->ns.flags & SY_WEAK_EXTRN ) return;
#endif
    if( sym->ns.flags & SY_EXTERNAL ) {
        if( sym->ns.reloc_chain.gr.offset == 0 ) {
            if( sym->ns.flags & SY_RELAX_EXTERN ) return;
        }
    }
    ClassNameErr( VA_UNDEFINED, sym );
}


unsigned_32     STGDump() {
//=========================

// Dump the global list.

    sym_id      sym;
    unsigned_32 size_alloc;
    unsigned_16 flags;
    unsigned_16 class;
    unsigned_16 sp;
    unsigned_32 global_size;

    global_size = 0;
    sym = GList;

    // Align global data on segment boundary so that when we write an
    // EXE file, SS will point to start of global data (we need this value
    // in EXEentry()).
    StartGD();

    while( sym != NULL ) {
#if _TARGET == _8086
        NewGItem();
#endif
        size_alloc = 0;
        flags = sym->ns.flags;
        class = flags & SY_CLASS;
        if( class == SY_VARIABLE ) {
            size_alloc = RelocVariable( sym );
            GblUndefined( size_alloc );
        } else if( class == SY_SUBPROGRAM ) {
            sp = flags & SY_SUBPROG_TYPE;
            if( (sp == SY_SUBROUTINE) || (sp == SY_FUNCTION) ||
                (sp == SY_FN_OR_SUB) ||
                (sp == SY_PROGRAM) ||
                ( (sp == SY_BLOCK_DATA) && !(flags & SY_UNNAMED) ) ) {
                if( _GenObjectFile() ) {
                    Sym2Obj( sym );
                } else {
                    ChkUndefined( sym );
                }
                GblReloc( sym );
            }
        } else if( class == SY_COMMON ) {
#if (_TARGET != _80386) && (_TARGET != _8086)
            if( ( Options & OPT_OBJECT ) == 0 ) {
#endif
                size_alloc = ComBlkAlloc( sym );
                GblUndefined( size_alloc );
#if (_TARGET != _80386) && (_TARGET != _8086)
                if( sym->ns.flags & SY_COMMON_LOAD ) {
                    ProcComList( sym );
                }
            } else {
                if( ( sym->ns.flags & SY_COMMON_INIT ) == 0 ) {
                    SetComAddr( sym );

                // On the VAX, don't allocate initialized common blocks
                // in the global data area since they must go into
                // their own program section

#if _TARGET != _VAX
                } else {
                    size_alloc = ComBlkAlloc( sym );
                    GblUndefined( size_alloc );
#endif
                }
                Sym2Obj( sym );
            }
#endif
#if (_TARGET == _80386) || (_TARGET ==_8086)
            if( _GenObjectFile() ) Sym2Obj( sym );
#endif
            RelocComSyms( sym );
        }
        if( ( ProgSw & PS_ERROR ) == 0 ) {
            global_size += size_alloc;
        }
        sym = sym->ns.link;
    }
    FreeNameList( GList );
    GList = NULL;
#if _TARGET == _VAX
    if( Options & OPT_OBJECT ) {
        ObjComAlloc();
    }
#endif
    return( global_size );
}


void    RelocComSyms( sym_id com ) {
//==================================

// Relocate all symbols in this common block.

    com_reloc   *reloc;

#if (_TARGET != _80386) && (_TARGET != _8086)
    if( Options & OPT_OBJECT ) {
        SetComBlkId( com );
    }
#endif
    reloc = com->ns.reloc_chain.cr;
    while( reloc != NULL ) {
        ComVarReloc( reloc, &com->ns.address );
        reloc = reloc->link;
    }
    FreeChain( &com->ns.reloc_chain.cr );
#if (_TARGET != _80386) && (_TARGET != _8086)
    if( Options & OPT_OBJECT ) {
        SetSectId( com );
    }
#endif
#if _TARGET == _8086
    CloseCommonBlock();
#endif
}

#endif
