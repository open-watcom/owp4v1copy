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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/

#ifdef __NT__
// ugly hack; mlex.h defines TOKEN_TYPE too
#define TOKEN_TYPE WINTOKEN_TYPE
#include <windows.h>
#undef TOKEN_TYPE
#endif

#include "make.h"
#include "mcache.h"
#include "mmemory.h"
#include "mrcmsg.h"
#include "msg.h"
#include "mupdate.h"
#include "mautodep.h"
#include "autodept.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "pcobj.h"

typedef struct {
#if defined(__NT__)
    HANDLE      hFile,
                hMap;
#else
    int         handle;         // file handle of open obj file
#endif
#if defined(__386__) && !defined(__DOS__) && !defined(__WINDOWS__)
    char        *Data,          // file buffer
                *pData;         // current pointer
    long        len;            // file length
#endif
    time_t      time_stamp;     // time stamp of next dependancy comment
    char        *name;          // point to nameBuffer - name of next dependancy comment
} omf_info;

static omf_info fileHandle;
static char     nameBuffer[ _MAX_PATH2 + 1 ];

#pragma pack( push,1 );
typedef struct {
    uint_8              bits;
    uint_8              type;
    uint_16             dos_time;
    uint_16             dos_date;
    uint_8              name_len;
} obj_comment;
#pragma pack( pop );

static BOOLEAN omfOpen( const char * name )
/************************************/
{
#if defined(__NT__)
    if( INVALID_HANDLE_VALUE==(fileHandle.hFile = CreateFile(name,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL)) )
        return FALSE;
    fileHandle.len = GetFileSize(fileHandle.hFile,NULL);
    if( 0 == fileHandle.len || 0xFFFFFFFF == fileHandle.len ) {
        CloseHandle(fileHandle.hFile);
        return FALSE;
    }
    if( NULL==(fileHandle.hMap = CreateFileMapping(fileHandle.hFile,NULL,PAGE_READONLY,0,0,NULL)) ) {
        CloseHandle(fileHandle.hFile);
        return FALSE;
    }
    if( NULL==(fileHandle.pData = fileHandle.Data = MapViewOfFile(fileHandle.hMap,FILE_MAP_READ,0,0,0)) ) {
        CloseHandle(fileHandle.hMap);
        CloseHandle(fileHandle.hFile);
        return FALSE;
    }
#else
    if(-1 == (fileHandle.handle = open( name, O_RDONLY|O_BINARY )) )
        return FALSE;
#if defined(__386__) && defined(__DOS__) && defined(__WINDOWS__)
    else {
        long len;

        fileHandle.len      = filelength(fileHandle.handle);
        fileHandle.pData    =
        fileHandle.Data     = MallocSafe(fileHandle.len);
        len = read(handl,fileHandle.Data,fileHandle.len);
        close( fileHandle.handle );
        if( fileHandle.len != len ) {
            FreeSafe(fileHandle.Data);
            return( FALSE );
        }
    }
#endif
#endif
    return TRUE;
}
static BOOLEAN verifyObjFile(void)
/************************************/
{
#if !defined(__386__) || defined(__DOS__) || defined(__WINDOWS__)
    auto struct {
        obj_record      header;
        obj_name        name;
    } theadr;

    if( lseek( fileHandle.handle, 0, SEEK_SET ) < 0 ) {
        return( FALSE );
    }
    if( read( fileHandle.handle, &theadr, sizeof(theadr) ) != sizeof(theadr) ) {
        return( FALSE );
    }
    if( theadr.header.command != CMD_THEADR ) {
        return( FALSE );
    }
    if(( theadr.name.len + 2 ) != theadr.header.length ) {
        return( FALSE );
    }
    if( lseek( fileHandle.handle, 0, SEEK_SET ) < 0 ) {
        return( FALSE );
    }
#else
    auto struct obj {
        obj_record      header;
        obj_name        name;
    } *ptheadr;

    ptheadr = (struct obj*)fileHandle.Data;

    if( ptheadr->header.command != CMD_THEADR ) {
        return( FALSE );
    }
    if(( ptheadr->name.len + 2 ) != ptheadr->header.length ) {
        return( FALSE );
    }
#endif
    return( TRUE );
}

STATIC handle OMFInitFile( const char *name ) {
/*********************************************/

    handle      ret_val = NULL;

    if( omfOpen(name) ) {
        if( verifyObjFile() )
            ret_val = &fileHandle;
        else
            OMFFiniFile( &fileHandle );
    }
    return( ret_val );
}

static BOOLEAN getOMFCommentRecord( omf_info *info ) {
/*************************************************/
#if !defined(__386__) || defined(__DOS__) || defined(__WINDOWS__)
    obj_record          header;
    obj_comment         comment;
    int                 hdl;
    unsigned            len;

    hdl = info->handle;
    for(;;) {
        if( read( hdl, &header, sizeof( header ) ) != sizeof( header ) ) break;
        if( header.command != CMD_COMENT ) {
            // first LNAMES record means objfile has no dependancy info
            if( header.command == CMD_LNAMES ) break;
            lseek( hdl, header.length, SEEK_CUR );
            continue;
        }
        if( read( hdl, &comment, sizeof( comment ) ) != sizeof( comment ) ) break;
        if( comment.type != CMT_DEPENDENCY ) {
            lseek( hdl, header.length - sizeof( comment ), SEEK_CUR );
            continue;
        }
        // NULL dependency means end of dependency info
        if( header.length < sizeof( comment ) ) break;
        // we have a dependency comment!
        len = comment.name_len + 1;
        if( read( hdl, &nameBuffer, len ) != len ) break;
        nameBuffer[ len - 1 ] = '\0';
        info->time_stamp = _DOSStampToTime( comment.dos_date, comment.dos_time );
        info->name = &nameBuffer[ 0 ];
        return( TRUE );
    }
#else
    obj_record          *header;
    obj_comment         *comment;
    unsigned            len;

    for(;;) {
        header = (obj_record*)info->pData;
        info->pData += sizeof(obj_record); // read
        if (info->pData - info->Data >= info->len ) break;

        if( header->command != CMD_COMENT ) {
            // first LNAMES record means objfile has no dependancy info
            if( header->command == CMD_LNAMES ) break;
            info->pData += header->length; // seek
            continue;
        }
        comment = (obj_comment*)info->pData;
        info->pData += sizeof(obj_comment); // read
        if (info->pData - info->Data >= info->len ) break;

        if( comment->type != CMT_DEPENDENCY ) {
            info->pData += header->length - sizeof( obj_comment ); // seek
            continue;
        }
        // NULL dependency means end of dependency info
        if( header->length < sizeof( obj_comment ) ) break;
        // we have a dependency comment!
        len = comment->name_len + 1;

        memcpy(nameBuffer,info->pData,len);
        info->pData += len; // read
        if (info->pData - info->Data >= info->len ) break;

        nameBuffer[ len - 1 ] = '\0';
        info->time_stamp = _DOSStampToTime( comment->dos_date, comment->dos_time );
        info->name = &nameBuffer[ 0 ];
        return( TRUE );
    }
#endif
    return( FALSE );
}

STATIC dep_handle OMFNextDep( dep_handle info ) {
/***********************************************/

    if( getOMFCommentRecord( info ) ) {
        return( info );
    }
    return( NULL );
}

STATIC void OMFTransDep( dep_handle info, char **name, time_t *stamp ) {
/**********************************************************************/

    *name = ((omf_info *)info)->name;
    *stamp = ((omf_info *)info)->time_stamp;
}

STATIC void OMFFiniFile( handle info ) {
/**************************************/

    omf_info    *pinfo  = (omf_info*)info;

#if defined(__NT__)
    UnmapViewOfFile(pinfo->Data);
    CloseHandle(pinfo->hMap);
    CloseHandle(pinfo->hFile);
#else
#if defined(__386__) && !defined(__DOS__) && !defined(__WINDOWS__)
    FreeSafe( pinfo->Data );
#endif
    close( pinfo->handle );
#endif
}

const auto_dep_info OMFAutoDepInfo = {
    NULL,
    OMFInitFile,
    OMFNextDep,
    OMFTransDep,
    OMFNextDep,
    OMFFiniFile,
    NULL
};
