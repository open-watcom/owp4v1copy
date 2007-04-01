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
* Description: Utility program to run DOS programs from builder.
* It starts dosemu to run itself and converts forward slashes into backward
* slashes if in path names (= not in the beginning of a parameter).
* Use RUNASDOS_CMD environment parameter to pass name and parameters, so that
* there can be no conflict with existing dosemu parameters.
* Use RUNASDOS_CWD parameter to pass the current working directory to dosemu,
* relative to OpenWatcom's OWROOT; it is expected that dosemu executes an OW's
* setvars.bat which sets D: to OWROOT .
* This program should be copied under the name it should run as in the build/binl
* directory, such as wgml, hcdos .
****************************************************************************/

#include <env.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char *argv[] );


int main( int argc, char *argv[] )
{
    int    i, j, TotalLength, owrootlen;
    char  *Envvar, *curdir, *owroot;

    owroot = getenv( "OWROOT" );              /* get OWROOT for stripping off if needed */
    owrootlen = strlen( owroot );
    TotalLength = 1 + strlen( argv[0] );
    for( i = 1; i < argc; i++ ) {
        TotalLength += 1 + strlen( argv[i] ); /* include separator space */
    }
    Envvar = (char *)malloc( TotalLength );
    Envvar[0] = 0;
    for( i = 0; i < argc; i++ ) {
        if( i != 0 )
            strcat( Envvar, " " );
        if( strncmp( argv[i], owroot, owrootlen ) == 0 ) {
            argv[i] += owrootlen;
        }
        /* Convert forward slashes to backward ones. Note: may give problems if
           parameters start with forward slash */
        for( j = 0; j < strlen( argv[i] ); j++ )
            if (argv[i][j] == '/')
                argv[i][j] = '\\';
        strcat( Envvar, argv[i] );
    }
    setenv( "RUNASDOS_CMD", Envvar, 1 );      /* Will be picked up by dosemu autoexec using unix -s */
    /* Pass cwd to dosemu relative to devdir */
    curdir = getcwd( NULL, 0 );
    if( owroot && curdir )
    {
        setenv( "DOSDRIVE_D", owroot, 1 );    /* Tell DOSEMU OWROOT */
        for( i = 0, j = 0; i < strlen( curdir ) && j < strlen( owroot ); i++, j++ )
            if( curdir[i] != owroot[j] )
                break;
        if( curdir[i] == '/' )  /*owroot doesn't end in slash, skip leading slash also */
            i++;
        for( j = i; j < strlen( curdir ); j++ ) /* convert fwd slashes */
            if( curdir[j] == '/' )
                curdir[j] = '\\';
        setenv( "RUNASDOS_CWD", &curdir[i], 1 );    /* tell dosemu CWD */
    }

    return spawnlp( P_WAIT, "dosemu.bin", "dosemu.bin", "-D-a", "-I video{none}" , NULL );
}
