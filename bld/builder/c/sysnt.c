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
* Description:  Windows NT specific functions for builder
*
****************************************************************************/


#include <sys/types.h>
#include <direct.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include "builder.h"

#include <windows.h>

char    *CmdProc;
#define TITLESIZE 256
char    Title[TITLESIZE];

void SysInit( int argc, char *argv[] )
{
    int         i;

    strcpy( Title, "Builder " );
    for( i=1; i < argc; i++ ) {
        if( strlen( Title ) + strlen( argv[i] ) > TITLESIZE - 3 ) break;
        strcat( Title, argv[i] );
        strcat( Title, " " );
    }
    strcat( Title, "[" );
    getcwd( Title+strlen(Title), TITLESIZE - strlen( Title ) - 2 );
    strcat( Title, "]" );
    SetConsoleTitle( Title );

    CmdProc = getenv( "ComSpec" );
    if( CmdProc == NULL ) {
        Fatal( "Can not find command processor" );
    }
}

/* #define OS_LEVEL_PIPES doesn't seem to work for NT? */
#ifdef OS_LEVEL_PIPES
unsigned SysRunCommandPipe( const char *cmd, int *readpipe )
{
    int         rc;
    HANDLE      pipe_input;
    HANDLE      pipe_output;
    HANDLE      pipe_input_dup;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if( !CreatePipe( &pipe_input, &pipe_output, &sa, 0 ) ) {
        return( GetLastError() );
    }
    SetStdHandle( STD_OUTPUT_HANDLE, pipe_output );
    SetStdHandle( STD_ERROR_HANDLE, pipe_output );
    DuplicateHandle( GetCurrentProcess(), pipe_input,
                GetCurrentProcess(), &pipe_input_dup , 0, FALSE,
                DUPLICATE_SAME_ACCESS);
    CloseHandle( pipe_input );
    rc = spawnl( P_NOWAIT, CmdProc, CmdProc, "/c", cmd, NULL );
    CloseHandle( pipe_output );
    *readpipe = _hdopen( (int) pipe_input_dup, O_RDONLY );
    return rc;
}
#else
unsigned SysRunCommandPipe(const char *cmd, int *readpipe)
{
    int rc;
    int pipe_fd[2];

    if ( _pipe( pipe_fd, 0, 0 ) == -1 )
        return( errno );
    if ( dup2( pipe_fd[1], STDOUT_FILENO ) == -1 )
        return( errno );
    if ( dup2( pipe_fd[1], STDERR_FILENO ) == -1 )
        return( errno );
    close( pipe_fd[1] );
    rc = spawnl( P_NOWAIT, CmdProc, CmdProc, "/c", cmd, NULL );
    *readpipe = pipe_fd[0];
    return rc;
}
#endif

unsigned SysChdir( char *dir )
{
    char        *end;
    unsigned    retval;

    retval = SysDosChdir( dir );

    end = strchr( Title, ']' );
    *(end+1) = '\0';

    strcat( Title, " (" );
    getcwd( Title+strlen(Title), TITLESIZE - strlen( Title ) - 2 );
    strcat( Title, ")" );
    SetConsoleTitle( Title );
    return( retval );
}

void SysSetTitle( char *title )
{
    char        *end;

    end = strchr( Title, ']' );
    *(end+1) = '\0';

    strcat( Title, " (" );
    getcwd( Title+strlen(Title), TITLESIZE - strlen( Title ) - 2 );
    strcat( Title, ")" );
    SetConsoleTitle( Title );
}
