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
//  TDINIT:  Target dependent initialization for (8086)
//

#include "ftnstd.h"
#include "global.h"
#include "bglobal.h"
#include "fcgbls.h"
#include "cpopt.h"
#include "progsw.h"
#include "objutil.h"
#include "genobj.h"
#include "errcod.h"
#include <errno.h>
#include <string.h>
#include <process.h>
#include <malloc.h>

extern  void            OpenSymTab(void);
extern  void            CloseSymTab(void);
extern  unsigned_32     STGDump(void);
extern  label_id        NextLabel(void);
extern  void            InitPeepHole(void);
extern  void            InitStkDepth(void);
extern  void            DefaultLibInfo(void);
extern  char            *SDFName(char *);
extern  int             MakeName(char *,char *,char *);
extern  char            ObjExtn[];
extern  void            InfoError(int,...);
extern  void            SDScratch(char *);
extern  void            MsgFormat(char *,char *,...);
extern  int             RunLinker(char *);


void    TDProgInit() {
//====================

// Initialize for compilation of program.

    InitObj();
    if( _GenObjectFile() ) {
        GObjInit();
    }
    OpenSymTab();
    CodeSize = 0;
    DataSize = 0;
    GlobalSize = 0;
    PrevDChain.offset = NULL;
    DataChain.offset = NULL;
#if _TARGET == _8086
    PrevDChain.seg = NULL;
    DataChain.seg = NULL;
#endif
}


#define LINKER  "wlink"

static  void    MakeExe() {
//=========================

    char        obj_file[MAX_FILE];
    int         rc;

    if( ProgSw & PS_ERROR ) return;
    RelMem();
    MakeName( SDFName( SrcName ), ObjExtn, obj_file );
    _heapshrink();
#if _TARGET == _8086
    #include <malloc.h>

    {
        char    *cmd_line;

        cmd_line = malloc( 128 );
        if( LinkFile != NULL ) {
            MsgFormat( "%s1 %s2 %s3 %s4 %s5 %s6", cmd_line,
                          "op quiet name", SDFName( SrcName ),
                          "@", LinkFile, "file", obj_file );
        } else {
            MsgFormat( "%s1 %s2 %s3 %s4", cmd_line,
                          "op quiet name", SDFName( SrcName ),
                          "file", obj_file );
        }
        // Note: the linker will free the memory allocated for "cmd_line"
        rc = RunLinker( cmd_line );
        _heapshrink();
    }
#else
    if( LinkFile != NULL ) {
        rc = spawnlp( P_WAIT, LINKER, LINKER,
                      "op quiet name", SDFName( SrcName ),
                      "@", LinkFile, "file", obj_file, NULL );
    } else {
        rc = spawnlp( P_WAIT, LINKER, LINKER,
                      "op quiet name", SDFName( SrcName ),
                      "file", obj_file, NULL );
    }
#endif
    switch( rc ) {
    case 0:
        break;
    case -1:
        // error during spawn
        InfoError( SM_SPAWN_ERR, LINKER, strerror( errno ) );
        break;
    default:
        // error from wlink
        InfoError( SM_LINK_ERR );
        break;
    }
    if( !(Options & OPT_OBJECT) ) {
        SDScratch( obj_file );
    }
}


void    TDProgFini() {
//====================

// Finish off compilation of program.

    if( !(ProgSw & PS_ERROR) ) {
        GlobalSize = STGDump();
    }
    if( ProgSw & PS_PROGRAM_DONE ) {
        DefaultLibInfo();
    }
    if( _GenObjectFile() ) {
        if( PrevDChain.offset != 0 ) {
            GObjNoReloc( PrevDChain );
        }
        GObjFini();
    }
    if( Options & OPT_LINK ) {
        MakeExe();
    }
    CloseSymTab();
}


void    TDSubInit() {
//===================

// Initialize compilation of a subprogram.

    AlignEven( 0 );
    NewSPUnit();
    TBLabel = NextLabel();
    TBLabelPatched = FALSE;
    InitPeepHole();
    InitStkDepth();
    if( _GenObjectFile() ) {
        GObjSubInit();
    }
}


void    TDSubFini() {
//===================

// Finish off compilation of a subprogram.

    if( ( ProgSw & PS_ERROR ) == 0 ) {
        CodeSize += LocalData;         // must be two statements to
        CodeSize -= SProgStart.offset; // prevent sign extension.
#if _TARGET == _8086
        DataSize += ( ( CurrSeg - SProgStart.seg ) << 4 ) +
                    ObjPtr - LocalData;
#else
        DataSize += ObjPtr - LocalData;
#endif
        if( _GenObjectFile() ) {
            GObjSubFini();
        }
    }
}


void    TDStmtInit() {
//====================

// Target dependent statement initialization.

}


void    TDPurge() {
//=================

// Free all allocated structures.

}
