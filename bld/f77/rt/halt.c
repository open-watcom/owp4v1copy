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
// HALT         : Run-time error processor
//

#include "ftnstd.h"
#include "errcod.h"
#include "rundat.h"
#include "units.h"
#include "cioconst.h"
#include "rtenv.h"
#include "trcback.h"
#include "buginfo.h"
#include "rtflags.h"
#include "xfflags.h"
#include "pgmacc.h"
#include "fcodes.h"
#include "fcenv.h"

#include <stdarg.h>
#include <string.h>
#include <ctype.h>

extern  void            Suicide(void);
extern  void            ErrWrt(char *str);
extern  void            ErrWrtNL(char *str);
extern  void            SwapIOCBs(void);
extern  void            SendWSLStr(char *);
extern  void            SendLine(char *);
extern  bool            FindFtnFile(void);
extern  void            BldErrCode(unsigned int ,char *);
extern  int             ErrCodOrg(uint);
extern  void            WaitForEnter(void);
extern  void            DbPaused(void);
extern  void            DbStopped(void);
extern  void            MsgBuffer(uint,char *,...);
extern  char            *JmpBlanks(char *);
extern  void            WrtListFile(char *,int);
extern  char            *UserResponse(char *,char *,int);

extern  void            (* __BldErrMsg)(unsigned int ,char *,va_list args);

extern  short           DbugRet;
extern  int             NumErrors;

#pragma aux (lg_sym) DEBUG_STOPPED;
#pragma aux (lg_sym) DEBUG_PAUSED;

#ifdef _SA_LIBRARY

extern  void            StdWriteNL(char *,int);
extern  void            StdWrite(char *,int);

#define IS_LG_TB( tb )  ( (tb)->line == TB_LG || (tb)->line == TB_LG_DB )

#undef  GroupTable
#undef  ErrWord
#undef  GrpCodes
extern  char            __FAR * __FAR GroupTable[];
extern  char            __FAR ErrWord[];
extern  char            __FAR GrpCodes[];
extern  char            __FAR * __FAR LGGroupTable[];
extern  char            __FAR LGErrWord[];
extern  char            __FAR LGGrpCodes[];

extern  char            __FAR * __FAR *PGroupTable;
extern  char            __FAR *PErrWord;
extern  char            __FAR *PGrpCodes;

static  char            __FAR * __FAR *savePGroupTable;
static  char            __FAR *savePErrWord;
static  char            __FAR *savePGrpCodes;

static void NoDebug( void ) {}

void (*DEBUG_STOPPED)(void) = NoDebug;
void (*DEBUG_PAUSED)(void) = NoDebug;

#else

#define ERR_PREF_SIZE   5
#define ERR_CODE_SIZE   6

static  char            ErrorPref[] = { "*ERR*" };

void (*DEBUG_STOPPED)(void) = DbStopped;
void (*DEBUG_PAUSED)(void) = DbPaused;

#endif


unsigned_16     GetTraceLine() {
//==============================

// Get traceback line number.

#if _TARGET == _8086
    return( *(unsigned_16 PGM *)MK_FP( FP_SEG( ExCurr ), ExLinePtr ) );
#else
    return( *(unsigned_16 *)ExLinePtr );
#endif
}


char            *GetTraceInfo() {
//===============================

// Get traceback information.

#if _TARGET == _8086
    return( MK_FP( FP_SEG( ExCurr ), ExCurr->name ) );
#else
    return( ( char * )ExCurr->name );
#endif
}


#ifdef _SA_LIBRARY
static  void    PrintPlace( char *buff, uint lg_text, uint cg_text ) {
//====================================================================
#else
static  void    PrintPlace( char *buff, uint lg_text ) {
//======================================================
#endif

// Print an entry in the traceback chain.

    char        *name;
    char        *ptr;
    uint        len;

#ifdef _SA_LIBRARY
    if( IS_LG_TB( ExCurr ) ) {
#endif
        name = GetTraceInfo();
        ptr = name + strlen( name ) + sizeof( char );
        MsgBuffer( lg_text, buff );
        len = strlen( buff );
        if( *ptr == TB_LIBRARY ) {
            MsgBuffer( MS_TB_LIBRARY, &buff[len],
                       name, (uint)GetTraceLine(), ptr + sizeof( byte ) );
        } else if( *ptr == TB_EXTERNAL ) {
            MsgBuffer( MS_EXT_PROC, &buff[len] );
        } else {
            MsgBuffer( MS_TB_FILE, &buff[len],
                       name, (uint)GetTraceLine(), ptr + sizeof( byte ) );
        }
#ifndef _SA_LIBRARY
        SendLine( buff );
#else
    } else {
        MsgBuffer( cg_text, buff, ExCurr->line, ExCurr->name );
    }
    StdWriteNL( buff, strlen( buff ) );
#endif
}


#ifdef _SA_LIBRARY

void            TraceCalls( char *buff ) {
//========================================

// Produce a traceback of the calling subprograms.

    obj_ptr             ex_line_ptr;
    lg_traceback PGM    *ex_curr;
    bool                first_lg_tb;

    useLGTables();
    ex_curr = ExCurr;         // save ExCurr, ExLinePtr for debugger
    ex_line_ptr = ExLinePtr;
    first_lg_tb = TRUE;
    if( ExCurr != NULL ) {
        PrintPlace( buff, MS_EXEC_IN, MS_TRACE_INFO );
        for(;;) {
            if( IS_LG_TB( ExCurr ) ) {
                if( first_lg_tb ) {
                    // save ExCurr, ExLinePtr for debugger
                    ex_curr = ExCurr;
                    ex_line_ptr = ExLinePtr;
                    first_lg_tb = FALSE;
                }
                ExLinePtr = ExCurr->lineptr;
            } else if( ExCurr->link != NULL &&
                    IS_LG_TB( (lg_traceback PGM *)ExCurr->link ) ) {
                // This is an extra traceback put here by the external
                // interface code.  That code has no way of knowing that
                // it is going to call a function that will do it's own
                // traceback.
                ExCurr = ExCurr->link;
                ExLinePtr = ExCurr->lineptr;
            }
            ExCurr = ExCurr->link;
            if( ExCurr == NULL ) break;
            PrintPlace( buff, MS_CALLED_FROM_LG, MS_CALLED_FROM );
        }
    }
    // restore ExCurr and ExLinePtr for debugger
    ExCurr = ex_curr;
    ExLinePtr = ex_line_ptr;
    useDfltTables();
}


void    useLGTables() {
//=====================

    if( PGroupTable != LGGroupTable ) {
        savePGroupTable = PGroupTable;
        savePErrWord = PErrWord;
        savePGrpCodes = PGrpCodes;
        PGroupTable = LGGroupTable;
        PErrWord = LGErrWord;
        PGrpCodes = LGGrpCodes;
    }
}


void    useDfltTables() {
//=====================

    PGroupTable = savePGroupTable;
    PErrWord = savePErrWord;
    PGrpCodes = savePGrpCodes;
}

#else


void            TraceCalls( char *buff ) {
//========================================

// Produce a traceback of the calling subprograms.

    obj_ptr             ex_line_ptr;
    lg_traceback PGM    *ex_curr;

    ex_curr = ExCurr;         // save ExCurr, ExLinePtr for debugger
    ex_line_ptr = ExLinePtr;
    if( ExCurr != NULL ) {
        PrintPlace( buff, MS_EXEC_IN );
        for(;;) {
            ExLinePtr = ExCurr->lineptr;
            ExCurr = ExCurr->link;
            if( ExCurr == NULL ) break;
            PrintPlace( buff, MS_CALLED_FROM_LG );
        }
    }
    ExCurr = ex_curr;         // restore for use in debugger
    ExLinePtr = ex_line_ptr;
}

#endif


static  void    StopPgm() {
//=========================

    ++NumErrors;    // increment error count
    // this suicide will return us from (in order)
    //    1. spawned i/o statement, which will suicide from ...
    // or 2. spawned debugger command, leaving us in debugger
    // or 3. spawned Program, or program code (in RUNMAIN)
    __XcptFlags |= XF_FATAL_ERROR; // must be set here in case using debugger
#ifndef _SA_LIBRARY
    if( ( RTFlags & DB_INSIDE ) == 0 ) {
        SwapIOCBs();
    }
#else
    useDfltTables();
#endif
    Suicide();
}


#pragma aux (lg_rtn) ERR_HANDLER;

void    ERR_HANDLER( int err, char *buff ) {
//==========================================

    uint        len;

    if( RTFlags & DB_USED ) {
        if( ( RTFlags & DB_INSIDE ) == 0 ) {
            TraceCalls( buff );
#if _TARGET == _80386
            DbugRet = DB_ERR_STUB*4;
#else
            DbugRet = DB_ERR_STUB*2;
#endif
            RTFlags |= DB_ERROR;
        }
        StopPgm();
    } else {
        TraceCalls( buff );
    }
    if( err != KO_INTERRUPT ) StopPgm();
    __XcptFlags &= ~( XF_LIMIT_ERR | XF_ERR_MASK | XF_IO_INTERRUPTED );
    MsgBuffer( MS_INTERRUPT_PROMPT, buff );
    len = strlen( buff );
    buff = UserResponse( buff, &buff[len], ERR_BUFF_SIZE - len );
    if( tolower( *JmpBlanks( buff ) ) == 'y' ) {
        __XcptFlags |= XF_LIMIT_ERR | XF_KO_INTERRUPT | XF_FATAL_ERROR;
        StopPgm();
    }
}


#pragma aux (lg_rtn) STOP_HANDLER;

void    STOP_HANDLER() {
//======================

    DEBUG_STOPPED();
#if _TARGET == _80386
    DbugRet = DB_STOP_STUB*4;
#else
    DbugRet = DB_STOP_STUB*2;
#endif
}


#pragma aux (lg_rtn) PAUSE_HANDLER;

void    PAUSE_HANDLER() {
//=======================

    DEBUG_PAUSED();
    RTFlags |= DB_ERROR;
}


#ifdef _SA_LIBRARY

extern  void    ErrHandler(int,va_list);


void    RTErr( int err, ... ) { // really called LGRTErr()
//=============================

// Print a run time error message and halt exection.

    va_list     args;

    useLGTables();
    va_start( args, err );
    ErrHandler( err, args );
    va_end( args );
    useDfltTables();
}

#else


void    WriteErr( int err, va_list args ) {
//=========================================

// Print a error message. This is used when we are setting up the run-time
// environment and get an error (i.e. out of memory). We can't use
// normal error handling routines since we may not have allocated the
// necessary data structures.

    char        buffer[ERR_PREF_SIZE+ERR_BUFF_SIZE+1];

    strcpy( buffer, ErrorPref );
    BldErrCode( err, &buffer[ ERR_PREF_SIZE ] );
    __BldErrMsg( err, &buffer[ ERR_PREF_SIZE + ERR_CODE_SIZE ], args );
    ErrWrtNL( buffer );
    WrtListFile( buffer, strlen( buffer ) );
    __XcptFlags |= XF_FATAL_ERROR;
    NumErrors++;
}


void    ErrHandler( int err, va_list args ) {
//===========================================

// Print a run time error message and halt exection.

    int         unit_in_err;
    char        buffer[ERR_BUFF_SIZE+1];
    char        err_code[ERR_CODE_SIZE+1];

    unit_in_err = NULL_UNIT;
    if( err == IO_FILE_PROBLEM ) {
        unit_in_err = IOCB->fileinfo->unitid;
    }
    BldErrCode( err, err_code );
    __BldErrMsg( err, buffer, args );
    if( ( RTFlags & DB_INSIDE ) == 0 ) {
        SwapIOCBs();
    }
    // Make sure that we are set to log errors while in the debugger.
    // For example, if we get an i/o error while reading a
    // debugger command, IOCB->unitid will be DB_STD_INPUT so we have
    // to set IOCB->unitid to DB_STD_OUTPUT for logging purposes.
    IOCB->flags |= IOF_OUTPT;
    IOCB->unitid  = DB_STD_OUTPUT;
    IOCB->set_flags |= SET_UNIT;
    FindFtnFile();
    // If its an error writing to the error file, then don't write again
    // so as to avoid infinite attempts to write to the error file.
    if( unit_in_err == DB_STD_OUTPUT ) {
        ErrWrt( ErrorPref );
        ErrWrt( err_code );
        ErrWrtNL( buffer );
    } else {
        SendWSLStr( ErrorPref );
        SendWSLStr( err_code );
        SendLine( buffer );
    }
    ERR_HANDLER( err, buffer );
    if( ( RTFlags & DB_INSIDE ) == 0 ) {
        SwapIOCBs();
    }
}


void    RTErr( int err, ... ) {
//=============================

// Print a run time error message and halt exection.

    va_list     args;

    va_start( args, err );
    if( RTFlags & IO_INITIALIZED ) {
        ErrHandler( err, args );
    } else {
        WriteErr( err, args );
    }
    va_end( args );
}


void    Stop( string PGM *ptr ) {
//===============================

// Process a STOP statement.

    if( ptr != NULL ) {
        SwapIOCBs();
        SendLine( ptr->strptr );
        SwapIOCBs();
    }
    __XcptFlags |= XF_FATAL_ERROR;
    if( RTFlags & DB_USED ) {
        STOP_HANDLER();
    }
    Suicide();
}


void    Pause( string PGM *ptr ) {
//================================

// Process a PAUSE statement.

    SwapIOCBs();
    if( ptr != NULL ) {
        IOCB->flags |= IOF_NOCR;
        SendLine( ptr->strptr );
        IOCB->flags &= ~IOF_NOCR;
    }
    if( RTFlags & DB_USED ) {
        PAUSE_HANDLER();
    } else {
        WaitForEnter();
    }
    SwapIOCBs();
}

#endif
