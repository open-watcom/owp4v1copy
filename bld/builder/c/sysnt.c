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

unsigned SysRunCommandPipe( const char *cmd, int *readpipe )
{
    int         rc;
    HANDLE      pipe_input;
    HANDLE      pipe_output;
    HANDLE      std_output;
    HANDLE      std_error;
        
    if( !CreatePipe( &pipe_input, &pipe_output, NULL, 0 ) ) {
        return( GetLastError() );
    }
    DuplicateHandle( GetCurrentProcess(), pipe_output, GetCurrentProcess(), &std_output,
                0, TRUE, DUPLICATE_SAME_ACCESS );
    DuplicateHandle( GetCurrentProcess(), pipe_output, GetCurrentProcess(), &std_error,
                0, TRUE, DUPLICATE_SAME_ACCESS );
    SetStdHandle( STD_OUTPUT_HANDLE, std_output );
    SetStdHandle( STD_ERROR_HANDLE, std_error );
    CloseHandle( pipe_output );
    rc = spawnl( P_NOWAIT, CmdProc, CmdProc, "/c", cmd, NULL );
    CloseHandle( std_output );
    CloseHandle( std_error );
    *readpipe = _hdopen( (int) pipe_input, O_RDONLY );
    return rc;
}

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
