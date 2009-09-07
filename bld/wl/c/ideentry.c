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
* Description:  IDE interface entry points for wlink.
*
****************************************************************************/

#include "linkstd.h"
#include "msg.h"
#include "fileio.h"
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "idedll.h"
#include "idedrv.h"
#include "ideentry.h"
#include "wlink.h"
#include "wlnkmsg.h"

#if IDE_GET_TARGET_FILE != EXTRA_NAME_DIR
  || IDE_GET_OBJ_FILE != EXTRA_OBJ_FILE
  || IDE_GET_LIB_FILE != EXTRA_LIB_FILE
  || IDE_GET_RES_FILE != EXTRA_RES_FILE
#error idedll.h is out of date with fileio.h
#endif

static IDECBHdl         IdeHdl;
static IDEInitInfo      InitInfo;
static IDECallBacks     *IdeCB;

#if defined( __OS2__ )
//extern int InitMsg( void );
//extern int FiniMsg( void );

static bool     RunOnce;
#endif

static IDEMsgSeverity SeverityMap[] = {
    IDEMSGSEV_NOTE_MSG, IDEMSGSEV_NOTE_MSG, IDEMSGSEV_WARNING, IDEMSGSEV_ERROR,
    IDEMSGSEV_ERROR, IDEMSGSEV_ERROR, IDEMSGSEV_BANNER
};

#if defined( DLLS_IMPLEMENTED )
bool ExecDLLPgm( char *pname, char *cmdline )
/********************************************/
// return TRUE if an error
{
    IDEDRV              inf;
    IDEDRV_STATUS       status;

    status = IDEDRV_ERR_LOAD;
    IdeDrvInit( &inf, pname, NULL );
    IdeDrvChainCallbacks( IdeCB, &InitInfo );
    status = IdeDrvExecDLL( &inf, cmdline );
    IdeDrvUnloadDLL( &inf );
    return( status != IDEDRV_SUCCESS );
}
#endif

/* routines which are called by the linker core */

void WriteStdOut( char *str )
/**********************************/
{
    CheckBreak();
    if( IdeCB != NULL ) {
        IdeCB->PrintWithCRLF( IdeHdl, str );
    }
}

void WriteNLStdOut( void )
/*******************************/
{
    if( IdeCB != NULL ) {
        IdeCB->PrintWithCRLF( IdeHdl, "\n" );
    }
}

void WriteInfoStdOut( char *str, unsigned level, char *symbol )
/********************************************************************/
{
    IDEMsgInfo  info;
    unsigned    msgclass;

    CheckBreak();
    if( IdeCB != NULL ) {
        IdeMsgInit( &info, SeverityMap[(level & CLASS_MSK) >> NUM_SHIFT], str );
        msgclass = level & CLASS_MSK;
        if( msgclass != BANNER && msgclass >= (WRN & CLASS_MSK) ) {
            IdeMsgSetMsgNo( &info, CalcMsgNum( level ) );
            IdeMsgSetHelp( &info, "wlnkerrs.hlp", level & NUM_MSK );
        }
        if( symbol != NULL ) {
            IdeMsgSetLnkSymbol( &info, symbol );
        }
        IdeCB->PrintWithInfo( IdeHdl, &info );
    }
}

char * GetEnvString( char *envname )
/*****************************************/
{
    char *retval;

    if( IdeCB == NULL || InitInfo.ignore_env )
        return( NULL );
    IdeCB->GetInfo( IdeHdl, IDE_GET_ENV_VAR,(IDEGetInfoWParam)envname,
                    (IDEGetInfoLParam) &retval );
    return( retval );
}

bool IsStdOutConsole( void )
/*********************************/
{
    return InitInfo.console_output;
}

bool GetAddtlCommand( unsigned cmd, char *buf )
/****************************************************/
{
    cmd = cmd;
    buf = buf;
    return FALSE;
#if 0
    if( InitInfo.cmd_line_has_files ) return FALSE;
    return !IdeCB->GetInfo( IdeHdl, cmd, NULL, (IDEGetInfoLParam) &buf );
#endif
}

typedef IDEBool __stdcall (*IDEGetInfoFn)( IDECBHdl hdl, IDEInfoType type,
                                IDEGetInfoWParam wparam, IDEGetInfoLParam lparam );



/* routines which are called by the DLL driver */

IDEBool IDEDLL_EXPORT IDEPassInitInfo( IDEDllHdl hdl, IDEInitInfo *info )
/******************************************************************************/
{
    hdl = hdl;
    if( info->ver < IDE_CUR_INFO_VER5 ) return TRUE;
    InitInfo = *info;
    return FALSE;
}

unsigned IDEDLL_EXPORT IDEGetVersion( void )
/*************************************************/
{
    return IDE_CUR_DLL_VER;
}

void IDEDLL_EXPORT IDEStopRunning( void )
/**********************************************/
{
    LinkState |= STOP_WORKING|LINK_ERROR;
}

void IDEDLL_EXPORT IDEFreeHeap( void )
/*******************************************/
{
    _heapshrink();
}

IDEBool IDEDLL_EXPORT IDEInitDLL( IDECBHdl hdl, IDECallBacks *cb,
                                         IDEDllHdl *info )
/**********************************************************************/
{
    info = info;
    IdeHdl = hdl;
    IdeCB = cb;
    InitSubSystems();
#if defined( __OS2__ )
    RunOnce = FALSE;
#endif
    return FALSE;
}

void IDEDLL_EXPORT IDEFiniDLL( IDEDllHdl hdl )
/***************************************************/
{
    hdl = hdl;
    FiniSubSystems();
}

IDEBool IDEDLL_EXPORT IDERunYourSelf( IDEDllHdl hdl, const char * opts,
                                             IDEBool *fatalerr )
/****************************************************************************/
{
    hdl = hdl;
#if defined( __OS2__ )
    if( RunOnce ) {
        InitMsg();
    }
    RunOnce = TRUE;
#endif
    LinkMainLine( (char *) opts );
#if defined( __OS2__ )
    FiniMsg();
#endif
    *fatalerr = (LinkState & LINK_ERROR) != 0;
    return( *fatalerr );
}

IDEBool IDEDLL_EXPORT IDERunYourSelfArgv( IDEDllHdl hdl, int argc, char **argv,
                                             IDEBool *fatalerr )
/******************************************************************************/
{
    argc = argc;        /* to avoid a warning */
    argv = argv;
#ifndef __WATCOMC__
    _argv = argv;
    _argc = argc;
#endif
    LinkMainLine( NULL );
    *fatalerr = (LinkState & LINK_ERROR) != 0;
    return( *fatalerr );
}
