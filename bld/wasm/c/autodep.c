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
* Description:  Autodependency functions
*
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>

#include "autodep.h"
#include "asmalloc.h"
#include "bool.h"

FNAMEPTR FNames = NULL;

char *FilenameFullPath( char *buff, char const *name, size_t max )
{
    char        *p;

    p = _fullpath( buff, name, max );
    if( p == NULL )
        p = (char *)name;

#ifdef __UNIX__
    if( (p[0] == '/' && p[1] == '/') && (name[0] != '/' || name[1] != '/') ) {
        //
        // if the _fullpath result has a node number and
        // the user didn't specify one, strip the node number
        // off before returning
        //
        p += 2;
        while( *(p++) != '/' ) ;
    }
#endif
    return( p );
}

int SrcFileTime( char const *filename, time_t *mtime )
{
    struct stat statbuf;

    if( stat( filename, &statbuf ) == 0 ) {
        *mtime = statbuf.st_mtime;
        return( 0 );
    }
    *mtime = 0;
    return( -1 );
}

void AddFlist( char const *filename )
{
    FNAMEPTR    flist;
    FNAMEPTR    last;
    int         index;
    char        *fname;
    char        buff[2*_MAX_PATH];
        
    index = 0;
    last = FNames;
    for( flist = last; flist != NULL; flist = flist->next ) {
        if( strcmp( filename, flist->name ) == 0 ) return;
        index++;
        last = flist;
    }
    fname = FilenameFullPath( buff, filename, sizeof(buff) );
    flist = (FNAMEPTR)AsmAlloc( sizeof( FNAME ) );
    flist->name = (char*)AsmAlloc( strlen( fname ) + 1 );
    strcpy( flist->name, fname );
    SrcFileTime( fname, &flist->mtime );
    flist->next = NULL;
    if( FNames == NULL ) {
        FNames = flist;
    } else {
        last->next = flist;
    }
    return;
}
