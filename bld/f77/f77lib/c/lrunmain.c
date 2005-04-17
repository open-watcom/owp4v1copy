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
// LRUNMAIN     : load and go RunMain and associated routines
//

#include "ftnstd.h"
#include "units.h"
#include "rundat.h"
#include "cpopt.h"
#include "cioconst.h"
#include "errcod.h"
#include "rtflags.h"
#include "xfflags.h"
#include "iotype.h"
#include "bglobal.h"
#include "fcboth.h"
#include "undef.h"
#include "trcback.h"
#include <string.h>
#include "fcenv.h"

#if _TARGET == _8086
  #include <i86.h>
#endif
#if defined( _SA_LIBRARY )
 #include <stdlib.h>
#endif

extern  int             Spawn(void (*)());
extern  void            RMemFree(void *);
extern  void            *RChkAlloc(uint);
extern  void            *LocUnit(int);
#if _TARGET == _VAX
extern  file_handle     OpenRead(char *);
extern  file_handle     OpenWrite(char *);
#endif
extern  void            F77Data(targ_addr *);
extern  void            Program(void);
extern  void            DebugInit(comp_options);
extern  void            DebugFini(void);
extern  void            R_TrapInitLG(void);
extern  void            R_TrapFiniLG(void);
extern  ftnfile         *_GetFtnFile(int,int,void *,char *);
extern  void            _SetStd(int,int,char *,file_handle);
extern  void            _FiniEx(void);
extern  void            InitUndefIO(void);
extern  void            RTSysInit(void);
extern  void            RTSysFini(void);
extern  void            __InitUnit6CC(void);
extern  void            __LineFeedWithFormFeed(void);

extern  char            SDTermOut[];
extern  char            SDTermIn[];

#if defined( _SA_LIBRARY )
extern  void            IOSysInit(void);
extern  void            IOSysFini(void);
extern  targ_addr       _data_init_start;
extern  targ_addr       _data_init_end;
extern  void            FPTrapInit(void);
extern  void            FPTrapFini(void);
extern  void            TraceCalls(void);
#else
extern  int             _InitIO();
#endif

#if !defined( _SA_LIBRARY )
typedef struct comp_info {
    pointer             u5_ptr;
    char                *u5_name;
    pointer             u6_ptr;
    char                *u6_name;
    targ_addr           *data_init;
    file_handle         list_file;
} comp_info;

static  comp_info       CompInfo;
#endif
static  comp_options    CompOptions;

extern  file_handle     FStdIn;
extern  file_handle     FStdOut;
extern  file_handle     FStdErr;


#pragma aux (lg_sym) DEBUG_INIT;
#pragma aux (lg_sym) DEBUG_FINI;
#if defined( _SA_LIBRARY )
static void NoDebugInit( comp_options opt) {
    opt = opt;
}

static void NoDebugFini( void ) {}

void (*DEBUG_INIT)(comp_options) = NoDebugInit;
void (*DEBUG_FINI)(void) = NoDebugFini;

#else

void (*DEBUG_INIT)(comp_options) = DebugInit;
void (*DEBUG_FINI)(void) = DebugFini;

#endif


#if defined( _SA_LIBRARY )
#define SetStd _SetStd
#else
void    SetStd( int unit, int mode, char *term_name, file_handle fp ) {
//=====================================================================

    _SetStd( unit, mode, term_name, fp );
    if( __XcptFlags & XF_FATAL_ERROR ) return;
    if( CompOptions & OPT_LOG_IO ) {
        IOCB->fileinfo->flags |= FTN_LOG_IO;
    }
}
#endif


static  void    LGInitIO() {
//==========================

    file_handle db_file;
    char        *db_fname;
#if _OPSYS == _VMS
    char        errbuff[ERR_BUFF_SIZE+1];
#endif

#if defined( _SA_LIBRARY )
    IOSysInit();
#else
    if( _InitIO() ) return;
#endif
    InitUndefIO();
#ifndef _SA_LIBRARY
    RListFile = CompInfo.list_file;
#endif
    if( CompOptions & OPT_TYPE ) {
        RTFlags |= LF_IS_TERMINAL;
    }
    LinesOut = 0;
    PagesOut = 0;
    ExCurr = NULL;
    AltIOCB = NULL;
    MsgBuff = RChkAlloc( 256 );
    if( MsgBuff == NULL ) return;
    AltIOCB = RChkAlloc( sizeof( iocb ) );
    if( AltIOCB == NULL ) return;
    memset( AltIOCB, 0, sizeof( iocb ) );
    // connect the alternate IOCB to debugger std output for error
    // message reporting
#if _OPSYS == _VMS
    db_fname = "DBG$OUTPUT";
    db_file = OpenWrite( db_fname );
    if( db_file == NULL ) {
        RTErr( IO_FILE_PROBLEM );
    }
    if( __XcptFlags & XF_FATAL_ERROR ) return;
#else
    db_fname = &SDTermOut;
    db_file = FStdErr;
#endif
    AltIOCB->fileinfo =
        _GetFtnFile( DB_STD_OUTPUT, ACTION_WRITE, db_file, db_fname );
    if( __XcptFlags & XF_FATAL_ERROR ) return;
    AltIOCB->flags |= IOF_OUTPT;
    AltIOCB->unitid = DB_STD_OUTPUT;
    AltIOCB->iostmt = IO_WRITE;
    AltIOCB->flags = UNDCHK_IO; // for displaying variables in debugger
#ifndef _SA_LIBRARY
    if( CompInfo.u5_ptr == NULL ) {
        SetStd( STANDARD_INPUT, ACTION_READ, SDTermIn, FStdIn );
    } else {
        SetStd( STANDARD_INPUT, ACTION_READ, CompInfo.u5_name, CompInfo.u5_ptr );
    }
    if( __XcptFlags & XF_FATAL_ERROR ) return;
    if( CompInfo.u6_ptr == NULL ) {
        SetStd( STANDARD_OUTPUT, ACTION_WRITE, SDTermOut, FStdOut );
    } else {
        SetStd( STANDARD_OUTPUT, ACTION_WRITE, CompInfo.u6_name, CompInfo.u6_ptr );
    }
    if( __XcptFlags & XF_FATAL_ERROR ) return;
#endif
    // open debug input file, if debugger used
    if( ( RTFlags & DB_USED ) == 0 ) return;
#if _OPSYS == _VMS
    db_fname = "DBG$INPUT";
    db_file = OpenRead( db_fname );
    if( db_file == NULL ) {
        RTErr( IO_FILE_PROBLEM );
    }
    if( __XcptFlags & XF_FATAL_ERROR ) return;
#else
    db_fname = &SDTermIn;
    db_file = FStdIn;
#endif
    _GetFtnFile( DB_STD_INPUT, ACTION_READ, db_file, db_fname );
}


static  void    DataInitialize() {
//================================

#ifdef _SA_LIBRARY
    targ_addr   *p;

    p = &_data_init_start;
    while( p < &_data_init_end ) {
        F77Data( p );
        ++p;
    }
#else
    F77Data( CompInfo.data_init );
#endif
}


static  void    LGSysFini() {
//===========================

#ifdef _SA_LIBRARY
    FPTrapFini();
#endif
    R_TrapFiniLG();
    if( MsgBuff != NULL ) {
        RMemFree( MsgBuff );
    }
    if( AltIOCB != NULL ) {
        RMemFree( AltIOCB );
    }
    _FiniEx();
    RTSysFini();
}


#ifdef _SA_LIBRARY


int     RunMain( comp_options options ) {
//=======================================

    {
        extern  void    TraceCalls(void);
        extern  void    (*TraceRoutine)(void);

        TraceRoutine = &TraceCalls;
    }
#else

int     RunMain( targ_addr *data_init, comp_options options, file_handle lf,
                 file_handle u5_ptr, char *u5_name,
                 file_handle u6_ptr, char *u6_name ) {
//==========================================================================

    __RetCode = 0;
    CompInfo.data_init = data_init;
    CompInfo.list_file = lf;
    CompInfo.u5_name = u5_name;
    CompInfo.u5_ptr = u5_ptr;
    CompInfo.u6_name = u6_name;
    CompInfo.u6_ptr = u6_ptr;
#endif
    CompOptions = options;
    RTFlags = 0;
    if( options & OPT_DEBUG ) {
        RTFlags |= DB_USED;
    }
    if( options & OPT_UNIT_6_CC ) {
        __InitUnit6CC();
    }
    if( options & OPT_LF_WITH_FF ) {
        __LineFeedWithFormFeed();
    }
    RTSysInit();
    R_TrapInitLG();
#ifdef _SA_LIBRARY
    FPTrapInit();
    atexit( &LGSysFini );
#else
    IOTypeRtn = &FC_IOType;
#endif
    Spawn( &LGInitIO );
    if( ( __XcptFlags & XF_FATAL_ERROR ) == 0 ) {
        RTFlags |= IO_INITIALIZED;
#ifdef _SA_LIBRARY
        if( &_data_init_start < &_data_init_end )
#else
        if( data_init != NULL )
#endif
        {
            RTFlags &= ~DB_USED;
            Spawn( &DataInitialize );
            if( ( __XcptFlags & XF_FATAL_ERROR ) == 0 ) {
                if( options & OPT_DEBUG ) {
                    RTFlags |= DB_USED;
                }
            }
        }
        if( ( __XcptFlags & XF_FATAL_ERROR ) == 0 ) {
            if( options & OPT_DEBUG ) {
                DEBUG_INIT( options );
            }
            Program();
            if( options & OPT_DEBUG ) {
                DEBUG_FINI();
            }
        }
    }
#ifdef _SA_LIBRARY
    return( 0 );
#else
    LGSysFini();
    return( __RetCode );
#endif
}


void    SwapIOCBs() {
//===================

    pointer     temp;

    temp = IOCB;
    IOCB = AltIOCB;
    AltIOCB = temp;
}
