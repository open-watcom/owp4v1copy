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
* Description:  System specific functions for builder
*
****************************************************************************/


#include <sys/types.h>
#include <direct.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>
#include <unistd.h>
#include "builder.h"

#if defined( __NT__ ) || defined( __OS2__ )

char    *CmdProc;
extern bool Quiet;

#ifdef __NT__
#define BUFSIZE 256
#define COMSPEC "ComSpec"
#else
/* for some reason OS2 has a global buffer, but NT a local buffer */
#define BUFSIZE 32768
static char    buff[BUFSIZE + 1];
#define COMSPEC "COMSPEC"
#endif

#endif

#ifdef __NT__

#include <windows.h>

#define TITLESIZE 256
static char    Title[TITLESIZE];

void SysInitTitle( int argc, char *argv[] )
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
}

void SysSetTitle( char *title )
{
    char        *end;
    end = strchr( title, ']' );
    *(end+1) = '\0';

    strcat( title, " (" );
    getcwd( title+strlen(title), TITLESIZE - strlen( title ) - 2 );
    strcat( title, ")" );
    SetConsoleTitle( title );
}

#else
#define SysInitTitle(x, y)
#define SysSetTitle(x)
#endif

void SysInit( int argc, char *argv[] )
{
#if defined( __NT__ ) || defined( __OS2__ )
    SysInitTitle(argc, argv);
  
    CmdProc = getenv( COMSPEC );
    
    if( CmdProc == NULL ) {
        Fatal( "Can not find command processor" );
    }
#endif    
}

unsigned SysRunCommand( const char *cmd )
{
#if defined( __OS2__ ) || defined ( __NT__ )
    int         pipe_fd[2];
    int         my_std_output;
    int         my_std_error;
    int         bytes_read;
    int         rc;
#ifdef __NT__
    char        buff[BUFSIZE + 1];
#endif      

    my_std_output = dup( STDOUT_FILENO );
    my_std_error = dup( STDERR_FILENO );
    if( _pipe( pipe_fd, BUFSIZE, 0 ) == -1)
        return( errno );
    rc = dup2( pipe_fd[1], STDOUT_FILENO );
    if (rc != 0) return ( rc );
    rc = dup2( pipe_fd[1], STDERR_FILENO );
    if (rc != 0) return ( rc );
    rc = spawnl( P_NOWAIT, CmdProc, CmdProc, "/c", cmd, NULL );
    close(pipe_fd[1]);
    dup2( my_std_output, STDOUT_FILENO );
    dup2( my_std_error, STDERR_FILENO );
    if( rc == -1 ) {
        close( pipe_fd[0] );
        return( -1 );
    }
    for(;;) {
        bytes_read = read ( pipe_fd[0], buff, sizeof( buff )-1);
        if( bytes_read == 0 ) break;
        buff[bytes_read] = '\0';
        Log( Quiet, "%s", buff );
    }
    close( pipe_fd[0] );
    return( 0 );
#else
    /* no pipes for DOS so we call "system" and hence cannot log */
    return system(cmd);
#endif    
}

unsigned SysChDir( char *dir )
{
    char        *end;
    unsigned    len;
    unsigned    total;
    unsigned    retval;

    if( dir[0] == '\0' ) return( 0 );
    len = strlen( dir );
    end = &dir[len-1];
    switch( *end ) {
    case '\\':
    case '/':
        if( end > dir && end[-1] != ':' ) {
            *end = '\0';
            --len;
        }
        break;
    }
    if( len > 2 && dir[1] == ':' ) {
        _dos_setdrive( toupper( dir[0] ) - 'A' + 1, &total );
    }
    retval = chdir( dir );

    SysSetTitle( Title );

    return( retval );
}
