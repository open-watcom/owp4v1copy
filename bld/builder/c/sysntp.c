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
* Description:  Windows NT builder pipe support
*
****************************************************************************/

#include <sys/types.h>
#include <direct.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>
#include <fcntl.h>
#include <unistd.h>
#include "builder.h"

#include <windows.h>

char            *CmdProc;
#define TITLESIZE 256
char            Title[TITLESIZE];

void SysInitTitle( int argc, char *argv[] )
{
    int         i;

    strcpy( Title, "Builder " );
    for( i = 1; i < argc; i++ ) {
        if( strlen( Title ) + strlen( argv[i] ) > TITLESIZE - 3 )
	    break;
        strcat( Title, argv[i] );
        strcat( Title, " " );
    }
    strcat( Title, "[" );
    getcwd( Title + strlen( Title ), TITLESIZE - strlen( Title ) - 2 );
    strcat( Title, "]" );
    SetConsoleTitle( Title );
}

void SysSetTitle( char *title )
{
    char        *end;

    end = strchr( Title, ']' );
    *( end + 1 ) = '\0';

    strcat( Title, " (" );
    getcwd( Title + strlen( Title ), TITLESIZE - strlen( Title ) - 2 );
    strcat( Title, ")" );
    SetConsoleTitle( Title );
}

int RunChildProcessCmdl( char *prg, char *cmdl ) {

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO         siStartInfo;

    ZeroMemory( &piProcInfo, sizeof( PROCESS_INFORMATION ) );
    ZeroMemory( &siStartInfo, sizeof( STARTUPINFO ) );
    siStartInfo.cb = sizeof( STARTUPINFO );
    return( CreateProcess( prg, cmdl, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo ) != 0 ) ? 0 : 1;
}

void SysInit( int argc, char *argv[] )
{
    SysInitTitle( argc, argv );
    CmdProc = getenv( "ComSpec" );
    if( CmdProc == NULL ) {
        Fatal( "Can not find command processor" );
    }
}


static void LogStream( bool quiet, const char *str, size_t len )
{
    if( !quiet ) {
        fwrite( str, 1, len, stderr );
    }
    if( LogFile != NULL ) {
        fwrite( str, 1, len, LogFile );
    }
}

unsigned SysRunCommand( const char *cmd )
{
    HANDLE              pipe_input;
    HANDLE              pipe_output;
    HANDLE              std_output;
    HANDLE              std_error;
    HANDLE              my_std_output;
    HANDLE              my_std_error;
    HANDLE              read_output;
    DWORD               bytes_read;
    char                buff[256 + 1];
    int                 rc;
    SECURITY_ATTRIBUTES saAttr;
    int                 fh;

    saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    rc = 1;
    // save stdout and stderr
    my_std_output = GetStdHandle( STD_OUTPUT_HANDLE );
    my_std_error = GetStdHandle( STD_ERROR_HANDLE );
    // create anonymous pipe
    if( CreatePipe( &pipe_input, &pipe_output, &saAttr, 0 ) ) {
        // redirect stdout to pipe
        DuplicateHandle( GetCurrentProcess(), pipe_output, GetCurrentProcess(), &std_output,
            0, TRUE, DUPLICATE_SAME_ACCESS );
        SetStdHandle( STD_OUTPUT_HANDLE, std_output );
        // redirect stderr to pipe
        DuplicateHandle( GetCurrentProcess(), pipe_output, GetCurrentProcess(), &std_error,
            0, TRUE, DUPLICATE_SAME_ACCESS );
        SetStdHandle( STD_ERROR_HANDLE, std_error );
        // redirect reading from pipe to read_output
        DuplicateHandle( GetCurrentProcess(), pipe_input, GetCurrentProcess(), &read_output,
            0, FALSE, DUPLICATE_SAME_ACCESS );
        CloseHandle( pipe_input );
        strcpy( buff, "/c " );
        strcat( buff, cmd );
        // run child process with redirected stdout and stderr
        rc = RunChildProcessCmdl( CmdProc, buff );
        CloseHandle( pipe_output );
        CloseHandle( std_output );
        CloseHandle( std_error );
    }
    // restore stdout and stderr
    SetStdHandle( STD_OUTPUT_HANDLE, my_std_output );
    SetStdHandle( STD_ERROR_HANDLE, my_std_error );
    if( rc == 0 ) {
        fh = _hdopen( ( int ) read_output, O_RDONLY );
        for( ;; ) {
            bytes_read = read( fh, buff, sizeof( buff ) - 1 );
            if( bytes_read == 0 )
	        break;
            buff[bytes_read] = '\0';
            LogStream( Quiet, buff, bytes_read );
        }
        close( fh );
        CloseHandle( read_output );
    }
    return( rc );
}

unsigned SysChDir( char *dir )
{
    unsigned    retval;

    retval = SysDosChdir( dir );

    SysSetTitle( Title );

    return( retval );
}
