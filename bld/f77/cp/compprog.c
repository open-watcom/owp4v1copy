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
// COMPPROG  : compile a program
//

#include "ftnstd.h"
#include "progsw.h"
#include "segsw.h"
#include "cpopt.h"
#include "stmtsw.h"
#include "errcod.h"
#include "sdfile.h"
#include "global.h"

extern  void            BIInit();
extern  void            BIEnd();
extern  void            BIStartSubProg();
extern  void            BIResolveUndefTypes();
extern  void            BIEndSubProg();
extern  void            FMemFree(void *);
extern  void            FrlInit(void **);
extern  void            FrlFini(void **);
extern  void            TrapInit(void);
extern  void            TrapFini(void);
extern  void            Error(int,...);
extern  void            ComRead(void);
extern  void            ProcInclude(void);
extern  bool            SetLst(bool);
extern  void            Conclude(void);
extern  void            CompStatement(void);
extern  void            InitCSList(void);
extern  bool            CheckCSList(byte);
extern  void            CSPurge(void);
extern  void            InitStNumbers(void);
extern  void            ClearRem(void);
extern  void            OpenSymTab(void);
extern  void            STInit(void);
extern  void            STResolve(void);
extern  void            STDump(void);
extern  void            DumpEntries(void);
extern  void            EnPurge(void);
extern  void            IFInit(void);
extern  void            TDProgInit(void);
extern  void            TDProgFini(void);
extern  void            TDSubInit(void);
extern  void            TDSubFini(void);
extern  void            Epilogue(void);
extern  void            GReturn(void);
extern  bool            TBreak(void);
extern  void            GEndBlockData(void);
#if _OPT_CG == _OFF
extern  lib_handle      Srch4Undef(void);
extern  void            ProcObjFile(lib_handle);
extern  void            IFDump(void);
extern  void            LibInit(void);
extern  bool            LibFini(bool);
#endif


static  bool    CompSProg( ) {
//============================

    bool        fini_subprog;

    InitSubProg();
    fini_subprog = FALSE;
    for(;;) {
        for(;;) {
            if( CurrFile->flags & INC_PENDING ) {
                CurrFile->flags &= ~INC_PENDING;
                ProcInclude();
                ComRead();
            } else if( CurrFile->flags & CONC_PENDING ) {
#if _OPT_CG == _ON
                if( ( ProgSw & PS_DONT_GENERATE ) &&
                    ( ( Options & OPT_SYNTAX ) == 0 ) &&
                    ( ( ProgSw & PS_ERROR ) == 0 ) &&
                    ( CurrFile->link == NULL ) ) break;
#endif
                Conclude();
                if( CurrFile == NULL ) break;
                ComRead();
            } else {
                break;
            }
        }
        Options = NewOptions;
        if( ProgSw & ( PS_SOURCE_EOF | PS_END_OF_SUBPROG ) ) break;
        CompStatement();
        if( ProgSw & ( PS_SOURCE_EOF | PS_END_OF_SUBPROG ) ) {
            // consider:        call sam
            //                  end
            //          c$include sam
            // Before we open 'sam', we must finish off the subprogram
            // so we get the correct file name in the traceback.
            FiniSubProg();
            fini_subprog = TRUE;
        }
        if( TBreak() ) return( TRUE );
    }
    if( !fini_subprog ) {
        FiniSubProg();
    }
    return( FALSE );
}


static  bool    CompFile() {
//==========================

    bool        tbreak;

    tbreak = FALSE;
    for(;;) {
        if( ProgSw & PS_SOURCE_EOF ) break;
        tbreak = CompSProg();
        if( tbreak ) break;
    }
    return( tbreak );
}


static  void    InitProgram() {
//=============================

    ExtnSw = 0;
    OpenSymTab();
    // VAX/VMS version of TrapInit() goes back 2 stack frames to set
    // exception bits for the whole compile.
    // If the sequence of calls "CompProg()/InitProgram()/TrapInit()"
    // changes, TrapInit() will have to be modified.
    TrapInit();
    ISNNumber = 0;
    ITHead = NULL;
    InitCSList();
    IFInit();
    TDProgInit();
    BIInit(); // Initialize Browser Info Generator
}


static  void    FiniProgram() {
//=============================

    TrapFini();
    Options = NewOptions;
#if _OPT_CG == _OFF
    IFDump();
#endif
    TDProgFini();
    CSPurge();
    BIEnd(); // Close down the Browse generator and create the .MBR file
}


#if _OPT_CG == _OFF

static  bool    ResolveUndefined( bool tbreak ) {
//===============================================

    lib_handle  lp;

#if _OBJECT == _ON
    if( Options & OPT_OBJECT ) return( tbreak );
#endif
    // search library only if we are not generating object files - /link
    // generates object files but we still want to search library
    ProgSw |= PS_LIBRARY_PROCESS;
    LibInit();
    for(;;) {
        if( ProgSw & PS_ERROR ) break;
        if( tbreak ) break;
        ProgSw &= ~( PS_LIB_OBJECT );
        lp = Srch4Undef();
        if( lp == NULL ) break;
        if( ProgSw & PS_LIB_OBJECT ) {
            ProcObjFile( lp );
#if _TARGET == _370
            ProgSw &= ~PS_LOADING_IF;
#endif
            tbreak = TBreak();
        } else {
            ProgSw &= ~PS_SOURCE_EOF;
            // SrcInclude( LibMember ) is now done in LIBSUPP
            CurrFile->fileptr = lp;
            CurrFile->flags |= INC_LIB_MEMBER;
            ComRead();
            tbreak = CompFile();
        }
    }
    tbreak = LibFini( tbreak );
    ProgSw &= ~PS_LIBRARY_PROCESS;
    return( tbreak );
}


static  void            CheckForProgram( bool tbreak ) {
//======================================================

    if( tbreak ) return;
    if( ProgSw & PS_PROGRAM_DONE ) return;
#if _OBJECT == _ON
    if( Options & OPT_OBJECT ) return;
#endif
    Error( SR_NO_PROG );
}

#endif


void    CompProg() {
//==================

    bool        tbreak;

    InitProgram();
    tbreak = CompFile();
#if _OPT_CG == _OFF
    tbreak = ResolveUndefined( tbreak );
#endif
    if( tbreak ) {
        Error( KO_INTERRUPT );
    }
    FiniProgram();
#if _OPT_CG == _OFF
    CheckForProgram( tbreak );
#endif
}


void    InitSubProg() {
//=====================

    ProgSw &= ~( PS_END_OF_SUBPROG | PS_IN_SUBPROGRAM | PS_BLOCK_DATA );
    FrlInit( &ITPool );
    SgmtSw       = 0;
    ArgList      = NULL;
    Entries      = NULL;
    EquivSets    = NULL;
    SubProgId    = NULL;
#if _OPT_CG == _OFF
#if _TARGET != _8086 && _TARGET != _80386
    PrevLine     = NULL;
    FirstISN     = NULL;
#endif
    MaxTempIndex = 0; // <-------+
    TempBase     = 0; //         | Must come before TDSubInit().
    TempIndex    = 0; //         |
#endif
    STInit();         // <-------+
    TDSubInit();      // Must come before InitStNumbers().
    InitStNumbers();
    ClearRem();
    BIStartSubProg();
}


void    FiniSubProg() {
//=====================

    FrlFini( &ITPool );
    CheckCSList( CS_EMPTY_LIST ); // all control structures should be finished
    if( !Remember.endstmt ) {
        Error( EN_NO_END );
    }
    if( ( SgmtSw & SG_SYMTAB_RESOLVED ) == 0 ) {
        STResolve();
        SgmtSw |= SG_SYMTAB_RESOLVED;
    }
    if(( ProgSw & PS_BLOCK_DATA ) != 0 ) {
        GEndBlockData();
    } else if(( ProgSw & PS_IN_SUBPROGRAM ) != 0 ) {
        Epilogue();
    } else if( ( ProgSw & PS_ERROR ) == 0 ) {
        GReturn();
    }
    DumpEntries();
    BIResolveUndefTypes();
    STDump();
    TDSubFini();
    BIEndSubProg();
    EnPurge(); // Must come after TDSubFini() for optimizing compiler
    SubProgId = NULL;
}
