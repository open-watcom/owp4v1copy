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
* Description:  Screen support functions for the Linux debug client.
*
****************************************************************************/


#include "dbgdefn.h"
#include "dbgio.h"
#include "dbgmem.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#ifdef __WATCOMC__
#include <env.h>
#endif
#include <unistd.h>
#if 0
#include <sys/vt.h>
#endif
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include "curses.h"
#include "term.h"
#include "stdui.h"
#include "../unix/h/ctkeyb.h"
#include "dbgtoggl.h"

extern void     StartupErr( char * );
extern int      GUIInitMouse( int );
extern void     GUIFiniMouse( void );
extern char     *Format(char *,char *,... );
extern void             ReleaseProgOvlay( bool );
extern void             KillDebugger( int );

extern char     *UITermType;
extern char     XConfig[];
extern char     *TxtBuff;

char                *DbgTerminal;
unsigned            DbgConsole;
unsigned            PrevConsole;
unsigned            InitConsole;
int                 DbgConHandle;
int                 DbgLines;
int                 DbgColumns;
int                 PrevLines;
int                 PrevColumns;
struct _console_ctrl *ConCtrl;

pid_t               XTermPid;

enum { C_XWIN, C_QCON, C_TTY, C_CURTTY } ConMode;

void RingBell()
{
    write( DbgConHandle, "\a", 1 );
}


/*
 * ConfigScreen -- figure out screen configuration we're going to use.
 */

unsigned ConfigScreen()
{
    return( 0 );
}


/*
 * InitScreen
 */
static void HupHandler( int signo )
{
    /* Xqsh has gone away -- nothing to do except die */
    signo = signo;
    ReleaseProgOvlay( TRUE );
    KillDebugger( 0 );
}

static bool TryXWindows()
{
    int         slavefd;
    int         masterfd;
    char        buff[64];
    char        **argv;
    char        *p;
    char        *end;
    unsigned    argc;
    char        slavename[] = "/dev/pts/xxxxxx";
    int         unlock = 0;
    char        buf;
    int         res;
    struct termios termio;
    
    /* we're in the X (or helper)environment */
    if ( getenv("DISPLAY") == NULL )
        return( FALSE );
    masterfd = open("/dev/ptmx", O_RDWR);
    if ( masterfd < 0 )
        return( FALSE );            
    fcntl( masterfd, F_SETFD, 0 );
    ioctl( masterfd, TIOCGPTN, &slavefd ); /* slavefd = ptsname(masterfd); */
    ioctl( masterfd, TIOCSPTLCK, &unlock ); /* unlockpt(masterfd); */
    sprintf(slavename + 9, "%d", slavefd);
    slavefd = open(slavename, O_RDWR);
    DbgConHandle = slavefd;
    if( DbgConHandle == -1 ) {
        StartupErr( "unable to open debugger console" );
    }
    tcgetattr(slavefd, &termio);
    termio.c_lflag &= ~ECHO;
    tcsetattr(slavefd, TCSADRAIN, &termio);
    argc = 0;
    p = XConfig;
    for( ;; ) {
        while( isspace( *p ) ) ++p;
        while( !isspace( *p ) && *p != '\0' ) ++p;
        if( *p == '\0' ) break;
        ++argc;
        *p++ = '\0';
    }
    end = p;
    _AllocA( argv, (argc + 10) * sizeof( *argv ) );

    argv[0] = "xterm";
    argv[1] = "-T";
    argv[2] = "Open Watcom Debugger";

    argc = 3;

    if( DbgLines != 0 || DbgColumns != 0 ) {
        argv[argc++] = "-geometry";
        if( DbgLines == 0 ) DbgLines = 25;
        if( DbgColumns == 0 ) DbgColumns = 80;
        p = Format( buff, "%ux%u+0+0", DbgColumns, DbgLines ) + 1;
        argv[argc++] = buff;
    }

    for( p = XConfig; p < end; p += strlen( p ) + 1 ) {
        while( isspace( *p ) ) ++p;
        argv[argc++] = p;
    }
    Format( p, "-SXX%u", masterfd );
    argv[argc++] = p;
    argv[argc] = NULL;
    
    fcntl( slavefd, F_SETFD, FD_CLOEXEC );
    XTermPid = fork();
    if (XTermPid == 0) { /* child */
        execvp( argv[0], (const char **)argv );
        exit( 1 );
    }
    if( XTermPid == (pid_t)-1 ) {
        StartupErr( "unable to create console helper process" );
    }
    do { /* xterm transmits a window ID -- ignore */
        res = read(slavefd, &buf, 1);
    } while ( res != -1 && buf != '\n' );
    termio.c_lflag |= ECHO;
    tcsetattr(slavefd, TCSADRAIN, &termio);
    
    signal( SIGHUP, &HupHandler );
    return( TRUE );
}

#if 0
/* something like what openvt() does can be done for the Linux
   console. That probably involves some ioctls;
   see console_ioctls(4)
*/
static bool TryQConsole()
{
#if 0
    char                        *ptr;
    char                        *term;
    char *tty_name;

    if( qnx_psinfo( PROC_PID, getpid(), &psinfo, 0, 0 ) != getpid() ) {
        StartupErr( "unable to obtain process information" );
    }
    if( qnx_sid_query( PROC_PID, psinfo.sid, &info ) != psinfo.sid ) {
        StartupErr( "unable to obtain console name" );
    }
    tty_name =
    ptr = &tty_name[ strlen( tty_name ) ];
    for( ;; ) {
        --ptr;
        if( *ptr < '0' || *ptr > '9' ) break;
    }
    if( DbgConsole != 0 ) {
        ptr[ 1 ] = '0' + DbgConsole / 10;
        ptr[ 2 ] = '0' + DbgConsole % 10;
        ptr += 2;
    }
    ptr[1] = NULLCHAR;
    DbgConHandle = open( tty_name, O_RDWR );
    if( DbgConHandle == -1 ) {
        StartupErr( "unable to open system console" );
    }
    term = getenv( "TERM" );
    if( term != NULL && strcmp( term, "qnxw" ) == 0 ) {
        /* in QNX windows */
#define PROP_STRING     "\033\"pwd\""
        write( DbgConHandle, PROP_STRING, sizeof( PROP_STRING ) - 1 );
    }

    ConCtrl = console_open( DbgConHandle, O_WRONLY );
    if( ConCtrl == NULL ) {
        close( DbgConHandle );
        return( FALSE );
    }
    if( dev_info( DbgConHandle, &dev ) == -1 ) {
        StartupErr( "unable to obtain console information" );
    }
    DbgConsole = dev.unit;
    console_size( ConCtrl, DbgConsole, 0, 0, &PrevLines, &PrevColumns );
    console_size( ConCtrl, DbgConsole, DbgLines, DbgColumns, 0, 0 );
    InitConsole = console_active( ConCtrl, -1 );
#endif
    return( FALSE );
}
#endif

static bool TryTTY()
{
    unsigned long       num;
    char                *end;

    if( DbgTerminal == NULL ) return( FALSE );
    num = strtoul( DbgTerminal, &end, 10 );
    if( *end == NULLCHAR && num < 100 ) {
        DbgConsole = num;
        return( FALSE );
    }
    /* guy gave an explicit terminal name */
    end = strchr( DbgTerminal, ':' );
    if( end != NULL ) {
        /* and also told us the terminal type */
        *end = NULLCHAR;
        UITermType = strdup( end + 1 );
    }
    DbgConHandle = open( DbgTerminal, O_RDWR );
    if( DbgConHandle == -1 ) {
        StartupErr( "unable to open system console" );
    }
    return( TRUE );
}

void InitScreen()
{
    extern bool DebugScreen( void );

    if( setpgid( 0, 0 ) != 0 && errno != EPERM ) {
        StartupErr( "unable to create new process group" );
    }
    if( TryTTY() ) {
        ConMode = C_TTY;
#if 0
    } else if( TryQConsole() ) {
        ConMode = C_QCON;
#endif
    } else if( TryXWindows() ) {
        ConMode = C_XWIN;
    } else {
        /* backup: just use the current terminal */
        ConMode = C_CURTTY;
        DbgConHandle = -1;
    }
    _Free( DbgTerminal );
    DbgTerminal = NULL;
    if ( DbgConHandle != -1 ) {
        fcntl( DbgConHandle, F_SETFD, (int)FD_CLOEXEC );
        UIConFile = fdopen( DbgConHandle, "w+" );
        UIConHandle = DbgConHandle;
    }
    if( !uistart() ) {
        StartupErr( "unable to initialize user interface" );
    }
    if( _IsOn( SW_USE_MOUSE ) ) GUIInitMouse( 1 );
    DebugScreen();
}


/*
 * UsrScrnMode -- setup the user screen mode
 */

bool UsrScrnMode()
{
    switch( ConMode ) {
    case C_TTY:
        return( TRUE );
    }
    return( FALSE );
}


void DbgScrnMode()
{
}


static int DebugPutc( int c )
{
    return fputc( c, UIConFile );
}

/*
 * DebugScreen -- swap/page to debugger screen
 */

bool DebugScreen()
{
    extern bool UserForcedTermRefresh;
    
    switch( ConMode ) {
    case C_TTY:
        return( TRUE );
    case C_CURTTY:
        _physupdate( NULL );
        UserForcedTermRefresh = TRUE;
        tputs( enter_ca_mode, 1, DebugPutc );
        break;
#if 0
    case C_QCON:
        PrevConsole = console_active( ConCtrl, DbgConsole );
        break;
#endif
    }
    return( FALSE );
}

bool DebugScreenRecover()
{
    return( TRUE );
}


/*
 * UserScreen -- swap/page to user screen
 */

bool UserScreen()
{
    switch( ConMode ) {
    case C_TTY:
        return( TRUE );
    case C_CURTTY:
        tputs( exit_ca_mode, 1, DebugPutc );
        break;
#if 0
    case C_QCON:
        console_active( ConCtrl, PrevConsole );
        break;
#endif
    }
    return( FALSE );
}

void SaveMainWindowPos()
{
}

void FiniScreen()
{
    if( _IsOn( SW_USE_MOUSE ) ) GUIFiniMouse();
    uistop();
    switch( ConMode ) {
#if 0
    case C_QCON:
        console_active( ConCtrl, InitConsole );
        console_size( ConCtrl, DbgConsole, PrevLines, PrevColumns, NULL, NULL );
        console_close( ConCtrl );
        break;
#endif
    case C_XWIN:
        signal( SIGHUP, SIG_IGN );
        kill( XTermPid, SIGTERM );
        break;
    }
}

void ScrnSpawnStart( void )
{
    char        *term;

    if( ConCtrl == NULL && UITermType != NULL ) {
        term = getenv( "TERM" );
        if( term == NULL ) term = "";
        strcpy( TxtBuff, term );
        setenv( "TERM", UITermType, 1 );
    }
}

void ScrnSpawnEnd( void )
{
    if( ConCtrl == NULL && UITermType != NULL ) {
        setenv( "TERM", TxtBuff, 1 );
    }
}


/*****************************************************************************\
 *                                                                           *
 *            Replacement routines for User Interface library                *
 *                                                                           *
\*****************************************************************************/

void PopErrBox( char *buff )
{
    WriteText( STD_ERR, buff, strlen( buff ) );
}
