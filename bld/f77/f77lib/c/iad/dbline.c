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
// DBLINE       : Line-oriented debugger support
//

#include "ftnstd.h"
#include "rundat.h"
#include "bugconst.h"
#include "rtflags.h"
#include "fio.h"
#include "cioconst.h"
#include "trcback.h"
#include "fcodes.h"
#include "xfflags.h"
#include "buginfo.h"
#include "errcod.h"
#include "units.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

extern  void            SendChar(char,int);
extern  void            SwapIOCBs(void);
extern  void            MsgLine(uint,...);
extern  char            *GetTraceInfo(void);
extern  unsigned_16     GetTraceLine(void);
extern  void            StartInput(void);
extern  void            StartOutput(void);
extern  void            SendInt(intstar4);
extern  void            SendWSLStr(char *);
extern  void            NextRec(void);
extern  void            Dsplcm(char *,int,bool);
extern  bool            NoEOF(ftnfile *);
extern  void            ClearEOF(void);
extern  int             Spawn(void (*)());
extern  void            Suicide(void);
extern  void            FPutBuff(ftnfile *);
extern  void            SendInt(intstar4);
extern  void            SendWSLStr(char *);
extern  void            SendLine(char *);
extern  void            SendEOR(void);
extern  void            TraceCalls(void);
extern  void            Blanks(void);
extern  void            StartOutput(void);
extern  bool            ParseEOL(void);
extern  int             ParseName(void);
extern  bool            ParseNum(unsigned_32 *);
extern  bool            ParseChr(char);
extern  char            *ParsePtr(void);
extern  void            DbModify(void);
extern  void            DbExamine(void);
extern  void            DbMsg(uint);
extern  uint            FGetRec(file_handle,char *,uint);
extern  void            FBackspace(file_handle,int);
extern  void            FRewind(file_handle);
extern  long int        FGetFilePos(file_handle);
extern  void            FSeekAbs(file_handle,long int);
extern  int             Errorf(file_handle);
extern  bool            EOFile(file_handle);
extern  void            Drop(char);
extern  int             Shell(char *);
#if _TARGET == _370
extern  void            Dsplcm(char *,int,bool);
#else
extern  void            PutRec(char *,int);
#endif
extern  b_file          *Openf(char *,f_attrs);
extern  void            Closef(b_file *);


extern  char            *AccModTab[];
extern  char            *FormTab[];
extern  char            *OpStatTab[];
extern  char            *BlnkTab[];
extern  char            *RecFmTab[];
extern  char            *CCtrlTab[];
extern  char            *ActionTab[];
extern  void            (*DbOutRoutine)(ftnfile *);
extern  short           DbugRet;
#if _TARGET != _370
extern  file_handle     FStdOut;
#endif

static  char    DebugCmds[] = {
    "Breakpoint\0"
    "Display\0"
    "Go\0"
    "Help\0"
    "Logio\0"
    "Quit\0"
    "Trace\0"
    "Unit\0"
    "Where\0"
    "\0"
};

static  const bool __FAR CmdEnd[] = {
    TRUE,         // bad command
    TRUE,         // null command
    FALSE,        // breakpoint
    FALSE,        // display
    FALSE,        // go
    TRUE,         // help
    FALSE,        // logio
    TRUE,         // quit
    TRUE,         // trace
    FALSE,        // unit
    TRUE,         // where
    FALSE,        // system
    FALSE,        // modify
    FALSE,        // print
};

static  bool            DbFirst;        // debugger entered for 1st time
static  byte            DbTrace;        // debugger TRACE
static  uint            DbTraceLevel;   // call depth while tracing
static  unsigned_16     DbBreakPt;      // breakpoint ISN
static  unsigned_16     DbTraceBreakPt; // "next" isn during trace
static  bool            DbLeave;        // leave debugger?
static  unsigned_16     CurrISN;        // current ISN
#ifndef _SA_LIBRARY
static  long int        SaveLFPos;      // listing file position
#else
static  void PGM        *ListExCurr;    // keep track of which module is open
static  char            CurrListName[ _MAX_FNAME ];
#endif

#define LONGEST_WORD 20


static  int     Lookup( char *table, char *word, int wordlen ) {
//==============================================================

    char            *wp;
    char            *tp;
    unsigned        wl;
    unsigned        cnt;
    unsigned char   c;
    char            temp[LONGEST_WORD+2];

    wp = word;
    wl = wordlen;
    tp = &temp[0];
    cnt = 0;
    if( wl > LONGEST_WORD ) return( cnt );
    while( wl != 0 ) {
        *tp++ = toupper( *wp++ );
        wl--;
    }
    *tp = '\0';
    wp = table;
    while( *wp != '\0' ) {
        cnt++;
        tp = &temp[0];
        for(;;) {
            c = toupper( *wp );
            if( c != *tp ) break;
            if( *tp == '\0' ) return( cnt );
            wp++;
            tp++;
        }
        if( *tp == '\0' ) {
            if( *wp < 'A' ) return( cnt );
            if( *wp > 'Z' ) return( cnt );
        }
        wp = strchr( wp, '\0' );
        wp++;
    }
    cnt = 0;
    return( cnt );
}


static  void    ExNull() {
//========================

// Perform NULL command.

}


static  void    ExJunk() {
//========================

// Display syntax error message.

    DbMsg( MS_DB_SYNTAX_ERR );
}


static  bool    FindSrcLine( char *src, bool bwd ) {
//==================================================

    char        *ptr;
    char        *digs;

    for(;;) { // find a line with an ISN
        if( bwd ) {
            FBackspace( RListFile, 0 );
            FBackspace( RListFile, 0 );
            if( FGetFilePos( RListFile ) == 0 ) {
                CurrISN = 0;
                return( TRUE );
            }
        }
        src[ FGetRec( RListFile, src, LIST_BUFF_SIZE ) ] = NULLCHAR;
        if( EOFile( RListFile ) ) {
            CurrISN = MAX_ISN;
            return( FALSE );
        }
        if( Errorf( RListFile ) ) return( FALSE );
        ptr = src;
        while( *ptr == ' ' ) ++ptr;
        digs = ptr;
        while( isdigit( *ptr ) ) ++ptr;
        if( ( ptr - src == 7 ) && ( digs < ptr ) ) break;
    }
    CurrISN = strtol( digs, NULL, 10 );
    return( TRUE );
}


static  bool    ShowSrcLine( unsigned_16 isn, bool hi_lite ) {
//============================================================

// Display source line.

    char        src[LIST_BUFF_SIZE];

#ifdef _SA_LIBRARY
    if( ExCurr != ListExCurr || RListFile == NULL ) {
        // attempt to open the appropriate listing file
        char PGM *src_file;

        ListExCurr = ExCurr;
        src_file = GetTraceInfo();
        src_file += strlen( src_file ) + sizeof( byte );
        if( *src_file == TB_FILE ) {
            // FIXME: currently we only look in the current directory
            ++src_file;
            _splitpath( src_file, NULL, NULL, src, NULL );
            if( strcmp( src, CurrListName ) != 0 ) {
                _makepath( src + _MAX_FNAME, NULL, NULL, src, ".LST" );
                if( RListFile != NULL ) {
                    // close any other list file we have open
                    Closef( RListFile );
                }
                RListFile = Openf( src + _MAX_FNAME, RDONLY|REC_TEXT );
                CurrISN = 0;
            }
        }
    }
#endif
    if( (RListFile == NULL) || (RTFlags & LF_IS_TERMINAL ) ) return( FALSE );
    if( isn == CurrISN ) {
        FBackspace( RListFile, 0 );
        if( !FindSrcLine( src, FALSE ) ) return( FALSE );
    } else if( isn < CurrISN ) { // search backward in the listing file
        for(;;) {
            if( CurrISN <= isn ) break;
            if( !FindSrcLine( src, TRUE ) ) return( FALSE );
        }
    } else { // search forward in the listing file
        for(;;) {
            if( CurrISN >= isn ) break;
            if( !FindSrcLine( src, FALSE ) ) return( FALSE );
        }
    }
    if( CurrISN == isn ) {
        if( hi_lite ) {
            Drop( '*' );
        } else {
            Drop( ' ' );
        }
        SendLine( src );
        return( TRUE );
    }
    if( hi_lite ) {
        SendWSLStr( "*      " );
    } else {
        SendWSLStr( "       " );
    }
    SendInt( isn );
    SendLine( "+" );
    return( TRUE );
}


static  bool    ProcessTrace() {
//==============================

    char        ch;
    unsigned_16 isn;

    DbTraceLevel = 0;
    isn = GetTraceLine();
    ShowSrcLine( isn, FALSE );
    ch = getch();
    DbTrace &= ~TRACE_INTO;
    switch( ch ) {
    case ' ':
        DbTrace |= TRACE_DEPTH;
        return( FALSE );
    case 'i':
    case 'I':
        DbTrace |= TRACE_INTO;
        return( FALSE );
    case 'n':
    case 'N':
        DbTrace &= ~TRACE;
        DbTrace |= TRACE_DEPTH;
        DbTraceBreakPt = isn + 1;
        return( FALSE );
    }
    return( TRUE );
}


static  void    ExTrace() {
//=========================

    StartOutput();
    MsgLine( MS_DB_TRACE_INSTR );
    DbTrace = TRACE;
    DbLeave = !ProcessTrace();
}


static  unsigned_16     GetISN() {
//================================

// Get an ISN.

    unsigned_16 isn;
    unsigned_32 num;

    isn = GetTraceLine();
    if( ParseChr( '.' ) ) return( isn );
    if( ParseChr( '+' ) ) {
        if( !ParseNum( &num ) ) return( 0 );
        if( isn + num < isn ) return( MAX_ISN );
        return( isn + num );
    } else if( ParseChr( '-' ) ) {
        if( !ParseNum( &num ) ) return( 0 );
        if( isn <= num ) return( 1 );
        return( isn - num );
    } else {
        if( !ParseNum( &num ) ) return( 0 );
        return( num );
    }
}


static  bool    BreakPointValue() {
//=================================

    unsigned_16 isn;

    isn = GetISN();
    if( isn != 0 ) {
        DbBreakPt = isn;
        return( TRUE );
    } else {
        DbMsg( MS_DB_BAD_ISN );
        return( FALSE );
    }
}


static  bool    SetBreakpoint() {
//===============================

    if( ParseChr( '/' ) ) {
        if( ParseChr( 'c' ) ) {
            DbBreakPt = 0;
            return( TRUE );
        }
        DbMsg( MS_DB_SYNTAX_ERR );
        return( FALSE );
    } else {
        return( BreakPointValue() );
    }
}


static  void    ExGo() {
//======================

    if( !ParseEOL() ) {
        if( !BreakPointValue() ) return;
    }
    DbLeave = TRUE;
}


static  void    ExBreakpoint() {
//==============================

    if( ParseEOL() ) {
        if( DbBreakPt != 0 ) {
            StartOutput();
            MsgLine( MS_DB_BREAK, DbBreakPt );
        }
    } else {
        SetBreakpoint();
    }
}


static  void    ExHelp() {
//========================

// Perform HELP command.

    StartOutput();
    MsgLine( MS_DB_HELP_1 );
    MsgLine( MS_DB_HELP_2 );
    MsgLine( MS_DB_HELP_3 );
    MsgLine( MS_DB_HELP_4 );
    MsgLine( MS_DB_HELP_5 );
    MsgLine( MS_DB_HELP_6 );
    MsgLine( MS_DB_HELP_7 );
    MsgLine( MS_DB_HELP_8 );
    MsgLine( MS_DB_HELP_9 );
    MsgLine( MS_DB_HELP_10 );
    MsgLine( MS_DB_HELP_11 );
    MsgLine( MS_DB_HELP_12 );
}


static  void    ExQuit() {
//========================

// Perform QUIT command.

    SetupQuit();
    DbLeave = TRUE;
}


static  void    ExWhere() {
//=========================

// Perform WHERE command.

    StartOutput();
    TraceCalls();
    ShowSrcLine( GetTraceLine(), FALSE );
}


static  void    ExSys() {
//=======================

// Perform SYS command.

    char        *cmd;

    Blanks();
    cmd = ParsePtr(); // Do this before StartOutput() - he changes current
                      // file.
    StartOutput();    // Shell() may do output
    Shell( cmd );
}


static  bool    GetISNRange( unsigned_16 *lo, unsigned_16 *hi ) {
//===============================================================

// Get an ISN range.

    unsigned_16 lo_isn;
    unsigned_16 hi_isn;

    lo_isn = GetISN();
    if( lo_isn == 0 ) return( FALSE );
    hi_isn = lo_isn;
    if( ParseChr( ':' ) ) {
        hi_isn = GetISN();
        if( hi_isn == 0 ) return( FALSE );
    }
    if( hi_isn < lo_isn ) return( FALSE );
    *lo = lo_isn;
    *hi = hi_isn;
    return( TRUE );
}


static  void    ExDisplay() {
//===========================

// Perform DISPLAY command.

    unsigned_16 curr_isn;
    unsigned_16 start_isn;
    unsigned_16 end_isn;

    curr_isn = GetTraceLine();
    if( ParseEOL() ) {
        start_isn = curr_isn;
        end_isn = start_isn + DISPLAY_WINDOW;
        if( start_isn <= DISPLAY_WINDOW ) {
            start_isn = 1;
        } else {
            start_isn -= DISPLAY_WINDOW;
        }
    } else if( ParseChr( '*' ) ) {
        if( !ParseEOL() ) {
            ExJunk();
            return;
        }
        start_isn = 1;
        end_isn = MAX_ISN;
    } else {
        if( !GetISNRange( &start_isn, &end_isn ) ) {
            DbMsg( MS_DB_BAD_ISN );
            return;
        }
        if( !ParseEOL() ) {
            ExJunk();
            return;
        }
    }
    if( ( RListFile == NULL ) || ( RTFlags & LF_IS_TERMINAL ) ) return;
    StartOutput();
    for(;;) {
        if( start_isn > end_isn ) break;
        if( !ShowSrcLine( start_isn, start_isn == curr_isn ) ) break;
        if( __XcptFlags & XF_LIMIT_ERR ) break;
        ++start_isn;
    }
    __XcptFlags &= ~XF_LIMIT_ERR;
}


static  void    FCBUnit( ftnfile *fcb ) {
//=======================================

// Dump a unit.

    if( fcb->unitid == STANDARD_INPUT ) {
        SendWSLStr( "*<" );
    } else if( fcb->unitid == STANDARD_OUTPUT ) {
        SendWSLStr( "*>" );
    } else if( fcb->unitid >= 0 ) {
        SendInt( fcb->unitid );
    } else {
        return;
    }
    SendChar( ' ', 5 );
    if( fcb->internal != NULL ) {
        SendLine( "INTERNAL" );
    } else {
        SendLine( fcb->filename );
    }
}


static  void    FCBInt( char *msg, intstar4 value ) {
//===================================================

// Display information about an FCB.

    SendWSLStr( msg );
    SendInt( value );
    SendEOR();
}


static  void    FindStr( char *table[], char *msg, uint idx ) {
//=============================================================

// Display information about an FCB.

    SendWSLStr( msg );
    if( idx == 0 ) {
        SendLine( "default" );
    } else {
        SendLine( table[ idx-1 ] );
    }
}


static  void    FCBDump( ftnfile *fcb ) {
//=======================================

// Dump an FCB.

    if( fcb->internal != NULL ) {
        SendLine(       "NAME=            INTERNAL" );
    } else {
        SendWSLStr(     "NAME=            " );
        SendLine( fcb->filename );
    }
    FindStr( AccModTab, "  ACCESS=          ", fcb->accmode );
    FindStr( ActionTab, "  ACTION=          ", fcb->action );
    FindStr( BlnkTab,   "  BLANK=           ", fcb->blanks );
    FCBInt(             "  BLOCKSIZE=       ", fcb->blocksize );
    FindStr( CCtrlTab,  "  CARRIAGECONTROL= ", fcb->cctrl );
    FindStr( FormTab,   "  FORM=            ", fcb->formatted );
    FCBInt(             "  IOSTAT=          ", fcb->error );
    SendWSLStr(         "  NEXTREC=         " );
    SendInt( fcb->recnum );
    if( fcb->flags & FTN_EOF ) {
        SendWSLStr( "  (EOF)" );
    }
    SendEOR();
    FCBInt(             "  RECL=            ", fcb->bufflen );
    FindStr( RecFmTab,  "  RECORDTYPE=      ", fcb->recfm );
    FindStr( OpStatTab, "  STATUS=          ", fcb->status );
}


static  void    ExUnit() {
//========================

    ftnfile     *fcb;
    unsigned_32 unit;
    bool        stdio;

    if( ParseEOL() ) {
        fcb = Files;
        StartOutput();
        while( fcb != NULL ) {
            FCBUnit( fcb );
            fcb = fcb->link;
        }
    } else {
        if( ParseChr( '*' ) ) {
            stdio = TRUE;
        } else if( ParseNum( &unit ) ) {
            stdio = FALSE;
        } else {
            DbMsg( MS_DB_SYNTAX_ERR );
            return;
        }
        if( !ParseEOL() ) {
            DbMsg( MS_DB_SYNTAX_ERR );
            return;
        }
        fcb = Files;
        StartOutput();
        for(;;) {
            if( fcb == NULL ) {
                if( stdio ) break;
                DbMsg( MS_DB_BAD_UNIT );
                break;
            }
            if( stdio ) {
                if( (fcb->unitid == STANDARD_INPUT) || (fcb->unitid == STANDARD_OUTPUT) ) {
                    FCBDump( fcb );
                }
            }
            if( fcb->unitid == unit ) {
                FCBDump( fcb );
                break;
            }
            fcb = fcb->link;
        }
    }
}


static  void    ExLogio() {
//=========================

// Perform LOGIO command.

    unsigned_32 unit;
    ftnfile     *fcb;
    bool        stdio;

    Blanks();
    if( ParseEOL() ) {
        fcb = Files;
        StartOutput();
        while( fcb != NULL ) {
            if( fcb->flags & FTN_LOG_IO ) {
                FCBUnit( fcb );
            }
            fcb = fcb->link;
        }
    } else {
        if( ParseChr( '*' ) ) {
            stdio = TRUE;
        } else if( ParseNum( &unit ) ) {
            stdio = FALSE;
        } else {
            DbMsg( MS_DB_SYNTAX_ERR );
            return;
        }
        if( !ParseEOL() ) {
            DbMsg( MS_DB_SYNTAX_ERR );
            return;
        }
        fcb = Files;
        StartOutput();
        for(;;) {
            if( fcb == NULL ) {
                if( stdio ) break;
                DbMsg( MS_DB_BAD_UNIT );
                break;
            }
            if( stdio ) {
                if( (fcb->unitid == STANDARD_INPUT) || (fcb->unitid == STANDARD_OUTPUT) ) {
                    fcb->flags ^= FTN_LOG_IO;
                }
            } else if( fcb->unitid == unit ) {
                fcb->flags ^= FTN_LOG_IO;
                break;
            }
            fcb = fcb->link;
        }
    }
}


static  void    (* const __FAR CmdRtns[])() = {
            &ExJunk,
            &ExNull,
            &ExBreakpoint,
            &ExDisplay,
            &ExGo,
            &ExHelp,
            &ExLogio,
            &ExQuit,
            &ExTrace,
            &ExUnit,
            &ExWhere,
            &ExSys,
            &DbModify,
            &DbExamine,
    };


static  void    DbOutRtn( ftnfile *fcb ) {
//========================================

    FPutBuff( fcb );
}


void            DebugInit() {
//===========================

    DbTrace = 0;
    DbTraceLevel = 0;
    DbBreakPt = 0;
    DbTraceBreakPt = 0;
    DbOutRoutine = &DbOutRtn;
#ifdef _SA_LIBRARY
    ListExCurr = NULL;
    CurrListName[0] = 0;
#else
    if( ( RListFile == NULL ) || ( RTFlags & LF_IS_TERMINAL ) ) {
        DbMsg( MS_DB_NO_LISTING_FILE );
    } else {
        SaveLFPos = FGetFilePos( RListFile );
        FRewind( RListFile );
    }
#endif
    CurrISN = 0;
}


#ifndef _SA_LIBRARY

void            LFIOStart() {
//===========================

    long int    db_pos;

    if( ( RListFile != NULL ) && !(RTFlags & LF_IS_TERMINAL) ) {
        db_pos = FGetFilePos( RListFile );
        FSeekAbs( RListFile, SaveLFPos );
        SaveLFPos = db_pos;
    }
}


void            LFIOEnd() {
//=========================

    long int    db_pos;

    if( ( RListFile != NULL ) && !(RTFlags & LF_IS_TERMINAL) ) {
        db_pos = SaveLFPos;
        SaveLFPos = FGetFilePos( RListFile );
        FSeekAbs( RListFile, db_pos );
    }
}

#endif


void            DebugFini() {
//===========================

#ifndef _SA_LIBRARY
    if( ( RListFile != NULL ) && !(RTFlags & LF_IS_TERMINAL) ) {
        FSeekAbs( RListFile, SaveLFPos );
    }
#endif
}


void    DbProlog() {
//==================

// Called at entry to every subprogram.

    RTFlags |= DB_INSIDE;
    DbFirst = TRUE;
    if( DbTrace ) { // if we are tracing
        ++DbTraceLevel;
        if( ( DbTrace & TRACE_INTO ) == 0 ) { // if we are to trace over call
            DbTrace &= ~TRACE;
        }
    }
    RTFlags &= ~DB_INSIDE;
}


static  void    SPReturn() {
//==========================

// Indicate we are returning from a subprogram.

    SwapIOCBs();
    MsgLine( MS_DB_RET_FROM, GetTraceInfo() );
    SwapIOCBs();
}


void    DbEpilog() {
//==================

// Called at exit of every subprogram.

    RTFlags |= DB_INSIDE;
    if( ( DbTrace & TRACE ) || ( ExCurr->link == NULL ) ) {
        SPReturn();
    }
    if( DbTraceLevel > 0 ) {
        --DbTraceLevel;
        if( DbTraceLevel == 0 ) { // if we are returning to caller of
            DbTrace |= TRACE;     // subprogram that has been traced over
            DbTrace &= ~TRACE_DEPTH;
        }
    }
    RTFlags &= ~DB_INSIDE;
}


void    DbStopped() {
//===================

}


void    DbPaused() {
//==================

}


#if _TARGET != _370

static  void    Dsplcm( char *prompt, int len, bool curs_eol ) {
//==============================================================

    if( curs_eol ) {
        ((a_file *)FStdOut)->attrs |= CC_NOCR;
    }
    PutRec( prompt, len );
    ((a_file *)FStdOut)->attrs &= ~CC_NOCR;
}

#endif


static  void    Prompt( char *prompt ) {
//======================================

// Prompt for debugger input.

    ftnfile     *fcb;
    char        *rsp;
    uint        rsp_len;
    uint        prompt_len;

    prompt_len = strlen( prompt );
    StartOutput();
    Dsplcm( prompt, prompt_len, TRUE );
    StartInput();
    NextRec();
    fcb = IOCB->fileinfo;
    rsp_len = fcb->len;
    rsp = fcb->buffer;
    for(;;) { // skip leading blanks in response
       if( rsp_len == 0 ) break;
       if( *rsp != ' ' ) break;
       ++rsp;
       --rsp_len;
    }
    // remove the prompt if it precedes the response
    if( (prompt_len <= rsp_len) && (memcmp( prompt, rsp, prompt_len ) == 0) ) {
        rsp += prompt_len;
        rsp_len -= prompt_len;
    }
    memcpy( fcb->buffer, rsp, rsp_len );
    fcb->buffer[ rsp_len ] = NULLCHAR;
    fcb->len = rsp_len;
}


static  void    DbPrompt() {
//==========================

    Prompt( "DEBUG>" );
}


static  void    DbClearEOF() {
//============================

// Clear EOF.

    StartInput();
    ClearEOF();
}


static  void    SetupQuit() {
//===========================

// Setup for quitting debugger.

#if _TARGET == _80386
    DbugRet = DB_QUIT_STUB*4;
#else
    DbugRet = DB_QUIT_STUB*2;
#endif
    RTFlags &= ~DB_INSIDE;
    __XcptFlags |= XF_FATAL_ERROR;
}


static  void    ProcessCmd() {
//============================

// Process user's debugger command.

    int         index;
    char        *cmd;

    Blanks();
    if( ParseEOL() ) {
        index = DB_NULL;
    } else if( ParseChr( '?' ) ) {
        index = DB_PRINT;
    } else if( ParseChr( '/' ) ) {
        index = DB_MODIFY;
    } else if( ParseChr( '!' ) ) {
        index = DB_SYSTEM;
    } else {
        cmd = ParsePtr();
        index = Lookup( DebugCmds, cmd, ParseName() );
        if( index == 0 ) {
            index = DB_JUNK;
        } else {
            index += DB_FIRST_CMD - 1;
        }
    }
    Blanks();
    if( CmdEnd[ index ] && !ParseEOL() ) {
        index = DB_JUNK;
    }
    __XcptFlags &= ~XF_LIMIT_ERR;
    DbLeave = FALSE;
    CmdRtns[ index ]();
    // Reset XF_FATAL_ERROR in case a debugger command set it.
    if( RTFlags & DB_INSIDE ) {
        __XcptFlags &= ~XF_FATAL_ERROR;
    } else {
        __XcptFlags |= XF_FATAL_ERROR;
    }
}


static  void    DbCommands() {
//============================

    for(;;) {
        StartInput();
        // We don't want EOF message in case the user removes the
        // prompt (clear screen) and returns an empty line.
        if( NoEOF( IOCB->fileinfo ) ) {
            IOCB->set_flags |= SET_EOFSTMT;
        }
        Spawn( &DbPrompt );
        IOCB->set_flags &= ~SET_EOFSTMT;
        if( IOCB->status < 0 ) {
            DbClearEOF();
        }
        if( IOCB->status != 0 ) break;
        if( Spawn( &ProcessCmd ) != 0 ) break;
        if( DbLeave ) return;
    }
    // We got an error reading from DB_STD_INPUT or writing to
    // DB_STD_OUTPUT so quit from the debugger.
    SetupQuit();
}


static  void    SPEntered() {
//===========================

// Display name of subprogram entered.

    MsgLine( MS_DB_ENTERED, GetTraceInfo() );
}


static  bool    DbCheck() {
//=========================

    unsigned_16 isn;

    isn = GetTraceLine();
    if( RTFlags & DB_ERROR ) { // an error occurred
        __XcptFlags &= ~XF_FATAL_ERROR;
    } else if( DbBreakPt == isn ) { // breakpoint
        MsgLine( MS_DB_BREAK, DbBreakPt );
    } else if( DbTrace & TRACE ) { // tracing
        if( DbFirst ) {
            SPEntered();
        }
        return( ProcessTrace() );
    } else if( DbTraceBreakPt == isn ) { // trace next
        DbTrace |= TRACE;
        return( ProcessTrace() );
    } else if( DbTraceLevel != 0 ) { // tracing over
        return( FALSE );
    } else if( DbFirst ) { // first time in subprogram
        if( ExCurr->link != NULL ) return( FALSE );
        SPEntered(); // main program entered
    } else {
        return( FALSE );
    }
    ShowSrcLine( isn, FALSE );
    return( TRUE );
}


static  bool    EntryReason() {
//=============================

// Determine why debugger was entered and act appropriately.
// Return TRUE if control to be passed to user.

    bool        user_control;

    SwapIOCBs();
    user_control = DbCheck();
    SwapIOCBs();
    DbFirst = FALSE;
    return( user_control );
}


void    DbLine() {
//================

    RTFlags |= DB_INSIDE;
    if( EntryReason() ) { // check if we're to stay in debugger
        DbTrace = 0; // clear trace information
        DbTraceLevel = 0;
        RTFlags &= ~DB_ERROR;
        SwapIOCBs();
        DbCommands();
        StartOutput();
        SwapIOCBs();
        if( ( RTFlags & DB_INSIDE ) == 0 ) { // if QUIT
            Suicide();
        }
    }
    RTFlags &= ~DB_INSIDE;
}
