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
* Description:  Autodepend section processing using ORL.
*
****************************************************************************/


#if !defined( __I86__ )
#include "make.h"
#include "mcache.h"
#include "mmemory.h"
#include "mrcmsg.h"
#include "msg.h"
#include "mupdate.h"
#include "mautodep.h"
#include "orl.h"
#include "autodep.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

typedef struct orl_info {
    int                 handle;
    orl_file_handle     file;
    char                *buffer;
    DepInfo             *curr;
} orl_info;

static orl_info                 orlInfo;
static orl_sec_handle           orlDependsInfo;
static orl_handle               orlHandle;
static char                     *orlBuffer;
static int                      orlFilePosition;
static int                      orlFileSize;

// note: this should match name in rscobj.c in codegen
static const char               *dependSectionName = ".depend";


static long fileSize( int handle )
{
    long                old;
    long                size;

    old = tell( handle );
    lseek( handle, 0, SEEK_END );
    size = tell( handle );
    lseek( handle, old, SEEK_SET );
    if( size == 0 ) {
        // MallocSafe returns NULL for size == 0
        ++size;
    }
    return( size );
}


static void bufferInit( void )
/****************************/
{
    orlFilePosition = 0;
    orlFileSize = 0;
    orlBuffer = NULL;
}


static void *orlRead( void *handle, size_t bytes )
/*********************************************/
{
    int                 n;
    int                 old_pos;


    if( orlBuffer == NULL ) {
        orlFileSize = fileSize( (int)handle );
        orlBuffer = MallocSafe( orlFileSize );
        // just suck it right in :)
        n = read( (int)handle, orlBuffer, orlFileSize );
        if( n != orlFileSize ) {
            return( NULL );
        }
    }
    if( orlFilePosition < orlFileSize ) {
        old_pos = orlFilePosition;
        orlFilePosition += bytes;
        return( &orlBuffer[old_pos] );
    }
    return( NULL );
}


static long int orlSeek( void *handle, long int offset, int mode )
/****************************************************************/
{
    switch( mode ) {
    case SEEK_SET:
        orlFilePosition = offset;
        break;
    case SEEK_CUR:
        orlFilePosition += offset;
        break;
    case SEEK_END:
        assert( 0 );            // not used by ORL - cheesy, I know
        break;
    }
    return( orlFilePosition );
}


void AutoORLInit( void )
/**********************/
{
    static orl_funcs    funcs = { orlRead, orlSeek, MallocSafe, FreeSafe };

    orlHandle = ORLInit( &funcs );
}


static orl_return findDependInfo( orl_sec_handle section )
/********************************************************/
{
    char                *name;

    name = ORLSecGetName( section );
    if( !strcmp( name, dependSectionName ) ) {
        orlDependsInfo = section;
        return( ORL_FALSE );
    }
    return( ORL_TRUE );
}


static char *orlGetDependsInfo( orl_file_handle file )
/****************************************************/
{
    char                *buffer;

    orlDependsInfo = NULL;
    buffer = NULL;
    ORLFileScan( file, NULL, findDependInfo );
    if( orlDependsInfo != NULL ) {
        ORLSecGetContents( orlDependsInfo, &buffer );
    }
    return( buffer );
}


handle AutoORLFileInit( const char *name )
/****************************************/
{
    orl_file_format     type;
    orl_file_handle     file;
    int                 handle;
    char                *buffer;

    bufferInit();
    handle = open( name, O_RDONLY | O_BINARY );
    if( handle != -1 ) {
        orlInfo.handle = handle;
        type = ORLFileIdentify( orlHandle, (void *)handle );
        switch( type ) {
        case ORL_COFF:
        case ORL_ELF:
            file = ORLFileInit( orlHandle, (void *)handle, type );
            if( file != NULL ) {
                orlInfo.file = file;
                buffer = orlGetDependsInfo( file );
                if( buffer != NULL ) {
                    orlInfo.buffer = buffer;
                    return( &orlInfo );
                }
            }
            break;
        case ORL_OMF:
        case ORL_UNRECOGNIZED_FORMAT:
            break;
        }
        if( orlBuffer != NULL ) {
            FreeSafe( orlBuffer );
            orlBuffer = NULL;
        }
        close( handle );
    }
    return( NULL );
}


dep_handle AutoORLFirstDep( handle hdl )
/**************************************/
{
    orl_info *hndl = hdl;

    hndl->curr = (DepInfo *)hndl->buffer;
    if( hndl->curr->len != 0 ) {
        return( hdl );
    }
    return( NULL );
}


void AutoORLTransDep( dep_handle hdl, char **name, time_t *stamp )
/****************************************************************/
{
    *name = ((orl_info *)hdl)->curr->name;
    *stamp = ((orl_info *)hdl)->curr->time;
}


dep_handle AutoORLNextDep( dep_handle hndl )
/******************************************/
{
    DepInfo     *p;
    orl_info    *hdl = hndl;

    p = hdl->curr;
    p = (DepInfo *)((char *)p + sizeof( DepInfo ) + p->len - 1);
    if( p->len == 0 ) {
        hdl->curr = NULL;
        return( NULL );
    }
    hdl->curr = p;
    return( hdl );
}


void AutoORLFileFini( handle hdl )
/********************************/
{
    ORLFileFini( ((orl_info*)hdl)->file );
    close( ((orl_info*)hdl)->handle );
    FreeSafe( orlBuffer );
    orlBuffer = NULL;
}


void AutoORLFini()
/****************/
{
    ORLFini( orlHandle );
}


const auto_dep_info ORLAutoDepInfo = {
    AutoORLInit,
    AutoORLFileInit,
    AutoORLFirstDep,
    AutoORLTransDep,
    AutoORLNextDep,
    AutoORLFileFini,
    AutoORLFini,
};
#endif
