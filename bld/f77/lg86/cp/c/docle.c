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
// DOCLE        : System dependent compile/link/execute
//

#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <malloc.h>

#include "ftnstd.h"
#include "progsw.h"
#include "cpopt.h"
#include "global.h"
#include "bglobal.h"
#include "fcgbls.h"
#include "rundat.h"
#include "objutil.h"
#include "xfflags.h"
#include "errcod.h"

extern  void            CompProg(void);
extern  int             RunMain(targ_addr *,ftnoption,file_handle,
                                file_handle,char *, file_handle,char *);
extern  void            PrtLstNL(char *);
extern  void            LFSkip(void);
extern  void            LFNewPage(void);
extern  file_handle     SrcPointer(void);
extern  file_handle     LstPointer(void);
extern  int             GetSrcName(char *);
extern  int             GetLstName(char *);
extern  void            FmtInteger(char *,int,int);
extern  void            SDClose(file_handle);
extern  void            Spawn(void (*)());
extern  char            *SDFName(char *);
extern  void            InfoError(int,...);
extern  void            MsgBuffer(uint,char *,...);
extern  void            TOutNL(char *);
extern  uint            MakeName(char *,char *,char *);
extern  void            CloseLst(void);
extern  void            ReOpenLst(void);

extern  unsigned_32     CompTime;
extern  unsigned_32     ExecTime;


void    DoCompile() {
//===================

// Compile a program.

    Executing = FALSE;
    __XcptFlags &= ~XF_LIMIT_ERR; // must initialize here so that data init
                          // won't give error when I/O checks for limit error
    Spawn( &CompProg );
    RelUnused();
}


void    SetRunVars() {
//====================

// Setup run-time variables.

    TotTime = TimeLimit;
    StmtRem = StmtLimit;
    Executing = TRUE;
}


void    DoExecute() {
//===================

// Execute a program.

    file_handle src_file;
    int         src_len;

    // DATA statements will be executed when EXE file is executed.
    if( Options & OPT_LINK ) {
        // Close the source file in case C$DATA used and we are generating
        // an EXE file.
        src_file = SrcPointer();
        if( src_file != NULL ) {
            SDClose( src_file );
        }
        if( Options & OPT_RUN ) {
            _heapshrink();
            if( Options & OPT_DEBUG ) {
                CloseLst();
            }
            RetCode = spawnlp( P_WAIT, SDFName( SrcName ), SDFName( SrcName ), NULL);
            if( Options & OPT_DEBUG ) {
                ReOpenLst();
            }
            if( RetCode == -1 ) {
                InfoError( SM_SPAWN_ERR, SDFName( SrcName ), strerror( errno ) );
            }
        }
    } else {
        src_len = GetSrcName( TokenBuff );
        GetLstName( &TokenBuff[ src_len + 1 ] );
        RetCode = RunMain( &DataChain, Options, ListFile,
                 SrcPointer(), TokenBuff, LstPointer(),
                 &TokenBuff[ src_len + 1 ] );
    }
}


static  void    FmtU32( char *buff, unsigned_32 num, uint len ) {
//===============================================================

    int         offset;
    int         num_len;
    char        *num_ptr;
    char        num_buff[MAX_INT_SIZE+1];

    memset( buff, ' ', len );
    num_ptr = num_buff;
    ltoa( num, num_ptr, 10 );
    num_len = strlen( num_ptr );
    offset = len - num_len;
    if( offset < 0 ) {
        num_ptr -= offset;
        offset = 0;
    }
    strcpy( buff + offset, num_ptr );
}


#define COLUMN_WIDTH    39

void    StatProg() {
//==================

    char        stats[LIST_BUFF_SIZE];
    char        *ptr;
    uint        len;

    if( Options & OPT_OBJECT ) {
        if( !(Options & OPT_QUIET) && !(Options & OPT_TYPE) ) {
            len = MakeName( SrcName, SrcExtn, stats );
            stats[len] = ':';
            ++len;
            MsgBuffer( MS_STATISTICS, &stats[len],
                       ISNNumber, CodeSize + DataSize + GlobalSize,
                       NumExtens, NumWarns, NumErrors );
            TOutNL( stats );
        }
    }

    LFSkip();

    MsgBuffer( MS_COMPILE_TIME, stats );
    ptr = stats;
    if ( stats[0] == ' ' ) {
        ptr++; // don't include leading space
    }
    len = strlen( ptr );
    FmtU32( &ptr[len], CompTime, COLUMN_WIDTH - len - 1 );
    ptr[COLUMN_WIDTH-1] = ' ';
    MsgBuffer( MS_EXECUTION_TIME, &ptr[COLUMN_WIDTH] );
    len = strlen( &ptr[COLUMN_WIDTH] );
    FmtU32( &ptr[COLUMN_WIDTH+len], ExecTime, COLUMN_WIDTH - len );
    PrtLstNL( ptr );

    MsgBuffer( MS_CODE_SIZE, stats );
    ptr = stats;
    if ( stats[0] == ' ' ) {
        ptr++; // don't include leading space
    }
    len = strlen( ptr );
    FmtU32( &ptr[len], CodeSize, COLUMN_WIDTH - len - 1 );
    ptr[COLUMN_WIDTH-1] = ' ';
    MsgBuffer( MS_NUMBER_OF_EXTENSIONS, &ptr[COLUMN_WIDTH] );
    len = strlen( &ptr[COLUMN_WIDTH] );
    FmtInteger( &ptr[COLUMN_WIDTH+len], NumExtens, COLUMN_WIDTH - len );
    PrtLstNL( ptr );

    MsgBuffer( MS_LOCAL_DATA_SIZE, stats );
    ptr = stats;
    if ( stats[0] == ' ' ) {
        ptr++; // don't include leading space
    }
    len = strlen( ptr );
    FmtU32( &ptr[len], DataSize, COLUMN_WIDTH - len - 1 );
    ptr[COLUMN_WIDTH-1] = ' ';
    MsgBuffer( MS_NUMBER_OF_WARNINGS, &ptr[COLUMN_WIDTH] );
    len = strlen( &ptr[COLUMN_WIDTH] );
    FmtInteger( &ptr[COLUMN_WIDTH+len], NumWarns, COLUMN_WIDTH - len );
    PrtLstNL( ptr );

    MsgBuffer( MS_GLOBAL_DATA_SIZE, stats );
    ptr = stats;
    if ( stats[0] == ' ' ) {
        ptr++; // don't include leading space
    }
    len = strlen( ptr );
    FmtU32( &ptr[len], GlobalSize, COLUMN_WIDTH - len - 1 );
    ptr[COLUMN_WIDTH-1] = ' ';
    MsgBuffer( MS_NUMBER_OF_ERRORS, &ptr[COLUMN_WIDTH] );
    len = strlen( &ptr[COLUMN_WIDTH] );
    FmtInteger( &ptr[COLUMN_WIDTH+len], NumErrors, COLUMN_WIDTH - len );
    PrtLstNL( ptr );

    MsgBuffer( MS_BYTES_FREE, stats );
    ptr = stats;
    if ( stats[0] == ' ' ) {
        ptr++; // don't include leading space
    }
    len = strlen( ptr );
    FmtU32( &ptr[len], AvailMem(), COLUMN_WIDTH - len - 1 );
    ptr[COLUMN_WIDTH-1] = ' ';
    MsgBuffer( MS_STATEMENTS_EXECUTED, &ptr[COLUMN_WIDTH] );
    len = strlen( &ptr[COLUMN_WIDTH] );
    if( ProgSw & PS_ERROR ) {
        FmtU32( &ptr[COLUMN_WIDTH+len], 0, COLUMN_WIDTH - len );
    } else {
        FmtU32( &ptr[COLUMN_WIDTH+len], StmtLimit - StmtRem, COLUMN_WIDTH - len );
    }
    PrtLstNL( ptr );

    LFNewPage();       // force a form feed
    PrtLstNL( "" );    // ...
    RelMem();
}
