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
* Description:  Platform dependent internal helper functions for wmake.
*
****************************************************************************/


#include "make.h"
#include "mcache.h"
#include "mmemory.h"
#include "mrcmsg.h"
#include "msg.h"
#include "msysdep.h"
#include "mupdate.h"

#include <unistd.h>
#include <signal.h>
#if defined( __DOS__ )
    #include <dos.h>
    #include "tinyio.h"
#else
    #include <fcntl.h>
    #include <sys/stat.h>
    #if defined(__UNIX__)
        #include <utime.h>
    #else
        #include <sys/utime.h>
    #endif
#endif

#if defined( __DOS__ )

static int __far critical_error_handler( unsigned deverr, unsigned errcode,
    unsigned far *devhdr )
{
    deverr = deverr; errcode = errcode; devhdr = devhdr;
    return( _HARDERR_FAIL );
}

extern char             DOSSwitchChar(void);
#pragma aux             DOSSwitchChar = \
        "mov ax,3700h"  \
        "int 21h"       \
        parm caller     [] \
        value           [dl] \
        modify          [ax dx];

#if !defined ( __386__ )
/* see page 90-91 of "Undocumented DOS" */

extern void far *       _DOS_list_of_lists( void );
#pragma aux             _DOS_list_of_lists = \
        "mov ax,5200h"  \
        "int 21h"       \
        parm caller     [] \
        value           [es bx] \
        modify          [ax es bx];

#endif

#endif

extern void InitHardErr( void )
{
#if defined( __DOS__ )
    _harderr( critical_error_handler );
#endif
}

extern int SwitchChar( void )
/***************************/
{
#if defined( __DOS__ )
    return( DOSSwitchChar() );
#elif   defined( __OS2__ ) || defined( __NT__ )
    return( '/' );
#elif   defined( __UNIX__ )
    return( '-' );

#endif
}

extern int OSCorrupted( void )
/****************************/
{
#if defined( __DOS__ ) && !defined ( __386__ )

    _Packed struct mcb {
        uint_8  id;
        uint_16 owner;
        uint_16 len;
    } far *chain;
    uint_16 far *first_MCB;
    uint_16 chain_seg;
    uint_16 new_chain_seg;

    first_MCB = _DOS_list_of_lists();
    if( FP_OFF( first_MCB ) == 1 ) {    /* next instr will hang! */
        /* list of lists DOS call may have been interrupted */
        return( 1 );
    }
    chain_seg = first_MCB[-1];
    for( ;; ) {
        chain = MK_FP( chain_seg, 0 );
        if( chain->id == 'Z' ) {
            break;
        }
        if( chain->id != 'M' ) {
            return( 1 );
        }
        new_chain_seg = chain_seg + ( chain->len + 1 );
        if( new_chain_seg <= chain_seg ) {
            return( 1 );
        }
        chain_seg = new_chain_seg;
    }
    return( 0 );
#else
    return( 0 );
#endif
}

extern RET_T TouchFile( const char *name )
/****************************************/
{
#if defined( __DOS__ )
    tiny_date_t     dt;
    tiny_time_t     tm;
    tiny_ftime_t    p_hms;
    tiny_fdate_t    p_ymd;
    tiny_ret_t      ret;

    ret = TinyOpen( name, TIO_WRITE );
    if( TINY_OK( ret ) ) {
        dt = TinyGetDate();
        p_ymd.year  = (uint_16)(int)(dt.year + (1900 - 1980));
        p_ymd.month = dt.month;
        p_ymd.day   = dt.day_of_month;

        tm = TinyGetTime();
        p_hms.hours   = tm.hour;
        p_hms.minutes = tm.minutes;
        p_hms.twosecs = tm.seconds / 2;

        TinySetFileStamp( TINY_INFO( ret ), p_hms, p_ymd );
        TinyClose( TINY_INFO( ret ) );
    } else {
        ret = TinyCreate( name, TIO_NORMAL );
        if( TINY_OK( ret ) ) {
            TinyClose( TINY_INFO( ret ) );
        } else {
            return( RET_ERROR );
        }
    }
    return( RET_SUCCESS );
#else
    int     fh;

    if( utime( name, 0 ) < 0 ) {
        fh = creat( name, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
        if( fh < 0 ) {
            return( RET_ERROR );
        }
        close( fh );
    }
    return( RET_SUCCESS );
#endif
}

#define FUZZY_DELTA     60      /* max allowed variance from stored time-stamp */

BOOLEAN IdenticalAutoDepTimes( time_t in_obj, time_t stamp )
/**********************************************************/
{
    time_t  diff_time;

    /* in_obj can be a DOS time so we need to round to the nearest two-second */
    if( in_obj == stamp || in_obj == (stamp & ~1) ) {
        return( TRUE );
    }
    if( in_obj < stamp ) {
        /* stamp is newer than time in .OBJ file */
        if( Glob.fuzzy ) {
            /* check for a "tiny" difference in times (almost identical) */
            diff_time = stamp - in_obj;
            if( diff_time <= FUZZY_DELTA ) {
                return( TRUE );
            }
        }
    }
    return( FALSE );
}

#ifdef DLLS_IMPLEMENTED

#include "idedrv.h"

static DLL_CMD  *dllCommandList;

#endif

void OSLoadDLL( char const *cmd_name, char const *dll_name, char const *ent_name )
/********************************************************************************/
{
#ifdef DLLS_IMPLEMENTED
    DLL_CMD     *n;

    // we want newer !loaddlls to take precedence
    n = MallocSafe( sizeof( *n ) );
    n->cmd_name = StrDupSafe( cmd_name );
    n->next = dllCommandList;
    dllCommandList = n;
    IdeDrvInit( &n->inf, StrDupSafe( dll_name ),
                (ent_name == NULL) ? NULL : StrDupSafe( ent_name ) );
#else
    (void)cmd_name, (void)dll_name, (void)ent_name; // Dummy function
#endif
}

DLL_CMD *OSFindDLL( char const *cmd_name )
/****************************************/
{
#ifdef DLLS_IMPLEMENTED
    DLL_CMD     *n;

    for( n = dllCommandList; n != NULL; n = n->next ) {
        if( 0 == stricmp( cmd_name, n->cmd_name ) ) {
            break;
        }
    }
    return( n );
#else
    (void)cmd_name; // Dummy function
    return( NULL );
#endif
}

int OSExecDLL( DLL_CMD *dll, char const *cmd_args )
/**************************************************
 * Returns the error code returned by IdeDrvDLL
 */
{
#ifdef DLLS_IMPLEMENTED
    int const   retcode = IdeDrvExecDLL( &dll->inf, cmd_args );

    setmode( STDOUT_FILENO, O_TEXT );
    return( retcode );
#else
    (void)dll, (void)cmd_args; // Dummy function
    return( -1 );
#endif
}

#ifndef NDEBUG
STATIC void cleanDLLCmd( void )
{
#ifdef DLLS_IMPLEMENTED
    DLL_CMD     *n;
    DLL_CMD     *temp;

    n  = dllCommandList;
    while( n != NULL ) {
        FreeSafe( (char *)n->cmd_name );
        if( n->inf.dll_name != NULL ) {
            FreeSafe( (char*) n->inf.dll_name );
        }
        if( n->inf.ent_name != NULL ) {
            FreeSafe( (char *)n->inf.ent_name );
        }
        temp = n;
        n = n->next;
        FreeSafe( temp );
    }
#endif
}
#endif


#ifndef NDEBUG
extern void DLLFini( void )
{
    cleanDLLCmd();
}
#endif

static sig_atomic_t     sig_count;

extern void CheckForBreak( void )
{
    if( sig_count > 0 ) {
        sig_count = 0;
        PrtMsg( ERR | USER_BREAK_ENCOUNTERED );
        exit( ExitSafe( EXIT_ERROR ) );
    }
}


static void passOnBreak( void )
{
#ifdef DLLS_IMPLEMENTED
    DLL_CMD     *n;

    for( n = dllCommandList; n != NULL; n = n->next ) {
        IdeDrvStopRunning( &n->inf );
    }
#endif
}


static void breakHandler( int sig_number )
{
    (void)sig_number; // Unused
    sig_count = 1;
    passOnBreak();
}

extern void InitSignals( void )
{
    sig_count = 0;
    DoingUpdate = FALSE;
#ifndef __UNIX__
    signal( SIGBREAK, breakHandler );
#endif
    signal( SIGINT, breakHandler );
}
