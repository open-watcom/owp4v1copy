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
* Description:  Handle based file operations for RDOS.
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include <rdos.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <time.h>
#include "liballoc.h"
#include "rtdata.h"
#include "rtinit.h"

#define FILE_HANDLE 0x3AB60000

/* values for type */
#define HANDLE_TYPE_FILE    0
#define HANDLE_TYPE_INPUT   1
#define HANDLE_TYPE_OUTPUT  2

typedef struct rdos_handle_type
{
    int rdos_handle;
    int access;
    int ref_count;
    char type;
} rdos_handle_type;

rdos_handle_type **handle_ptr;
int handle_count;
int handle_section;

rdos_handle_type *AllocHandleObj( void )
{
    rdos_handle_type *h;

    h = ( rdos_handle_type * )lib_malloc( sizeof( rdos_handle_type ) );
    h->rdos_handle = HANDLE_TYPE_FILE;
    h->access = 0;
    h->type = 0;
    h->ref_count = 1;
    return( h );
}

void FreeHandleObj( rdos_handle_type *h)
{
    h->ref_count--;

    if( h->ref_count == 0 ) {
        if( h->rdos_handle )
            RdosCloseFile( h->rdos_handle );
        lib_free( h );
    }
}

void GrowHandleArr( void )
{
    int                 i;
    int                 new_count;
    rdos_handle_type  **new_ptr;

    new_count = 1 + handle_count / 2;
    new_ptr = ( rdos_handle_type ** )lib_malloc( new_count * sizeof( rdos_handle_type * ) );

    for( i = 0; i < handle_count; i++ )
        new_ptr[i] = handle_ptr[i];
    
    for( i = handle_count; i < new_count; i++ )
        new_ptr[i] = 0;

    lib_free( handle_ptr );
    handle_ptr = new_ptr;
    handle_count = new_count;
}
    
int AllocHandleEntry( void )
{
    int i;

    RdosEnterSection( handle_section );

    for( i = 0; i < handle_count; i++)
        if( handle_ptr[i] == 0 )
            break;

    if( i == handle_count )
        GrowHandleArr();

    RdosLeaveSection( handle_section );

    return( i );
}
    
void FreeHandleEntry( int handle )
{
    RdosEnterSection( handle_section );

    if( handle >= 0 && handle < handle_count ) {
        if( handle_ptr[handle] ) {
            FreeHandleObj( handle_ptr[handle]);
            handle_ptr[handle] = 0;
        }
    }

    RdosLeaveSection( handle_section );
}
    
rdos_handle_type *LockHandle( int handle )
{
    RdosEnterSection( handle_section );

    if( handle >= 0 && handle < handle_count )
        return( handle_ptr[handle] );
    else
        return( 0 );
}

void UnlockHandle( void ) 
{
    RdosLeaveSection( handle_section );
}

void InitHandle( void )
{
    int i;
    rdos_handle_type *h;

    handle_section = RdosCreateSection();    
    handle_count = 5;
    handle_ptr = ( rdos_handle_type ** )lib_malloc( handle_count * sizeof( rdos_handle_type * ) );

    for( i = 0; i < handle_count; i++ )
        handle_ptr[i] = 0;

    h = AllocHandleObj();
    h->type = HANDLE_TYPE_INPUT;
    handle_ptr[0] = h;

    h = AllocHandleObj();
    h->type = HANDLE_TYPE_OUTPUT;
    handle_ptr[1] = h;

    h = AllocHandleObj();
    h->type = HANDLE_TYPE_OUTPUT;
    handle_ptr[2] = h;        
}

void FiniHandle( void )
{
    int i;

    for( i = 0; i < handle_count; i++ )
        if( handle_ptr[i] )
            FreeHandleObj( handle_ptr[i] );

    lib_free( handle_ptr );

    RdosDeleteSection( handle_section );
}

_WCRTLINK int unlink( const CHAR_TYPE *filename ) 
{
    __ptr_check( filename, 0 );

    if( RdosDeleteFile( filename ) )
        return( 0 );
    else
        return( -1 );  
}

_WCRTLINK int creat( const CHAR_TYPE *name, mode_t pmode )
{
    int handle;
    
    __ptr_check( name, 0 );

    handle = RdosCreateFile( name, 0 );

    if( handle )
        return( FILE_HANDLE | handle );
    else
        return( -1 );
}

_WCRTLINK int open( const CHAR_TYPE *name, int mode, ... )
{
    int     permission;
    va_list args;
    int     handle;
    
    va_start( args, mode );
    permission = va_arg( args, int );
    va_end( args );
    handle = RdosOpenFile( name, 0 );    
    
    if( handle )
        return( FILE_HANDLE | handle );
    else
        return( -1 );
}


_WCRTLINK int sopen( const CHAR_TYPE *name, int mode, int shflag, ... )
{
    va_list             args;
    int                 handle;

    va_start( args, shflag );

    if( mode & O_CREAT )
        handle = RdosCreateFile( name, 0 );    
    else
        handle = RdosOpenFile( name, 0 );    

    if( handle )
        if( mode & O_TRUNC )
            RdosSetFileSize( handle, 0 );
    
    if( handle )
        return( FILE_HANDLE | handle );
    else
        return( -1 );
}

_WCRTLINK int close( int handle )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        RdosCloseFile( handle );
        return( 0 );
    }
    else
        return( -1 );
}

_WCRTLINK int dup( int handle )
{
    int newhandle;
    
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        newhandle = RdosDuplFile( handle );

        if( newhandle )
            return( FILE_HANDLE | newhandle );
        else
            return( -1 );
    }
    else
        return( handle );
}

_WCRTLINK int eof( int handle )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        if( RdosGetFileSize( handle ) == RdosGetFilePos( handle ) )
            return( 1 );
        else
            return( 0 );        
    }
    else {
        if( handle == 0)
            return( !RdosPollKeyboard( ) );
        else
            return( 1 );
    }
}

_WCRTLINK long filelength( int handle )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE)
        return( RdosGetFileSize( handle ) );
    else
        return( 0 );
}

_WCRTLINK int chsize( int handle, long size )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        RdosSetFileSize( handle, size );
        return( 0 );
    }
    else
        return( -1 );
}

_WCRTLINK int fstat( int handle, struct stat *buf )
{
    unsigned long       msb;
    unsigned long       lsb;
    int                 ms;
    int                 us;
    struct tm           tm;

    buf->st_attr = 0;
    buf->st_archivedID = 0;
    buf->st_updatedID = 0;
    buf->st_inheritedRightsMask = 0;
    buf->st_originatingNameSpace = 0;
    buf->st_nlink = 1;
    buf->st_uid = buf->st_gid = 0;
    buf->st_ino = handle;
    buf->st_mode = 0;
    buf->st_mode |= S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;

    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        buf->st_dev = RdosIsDevice( handle );
        buf->st_rdev = buf->st_dev;

        if( buf->st_dev ) {
            buf->st_size = 0;
            buf->st_atime = buf->st_ctime = buf->st_mtime = 0;
            buf->st_mode |= S_IFCHR;
        } else {                /* file */
            RdosGetFileTime( handle, &msb, &lsb );
            RdosDecodeMsbTics( msb, 
                               &tm.tm_year, 
                               &tm.tm_mon,
                               &tm.tm_mday,
                               &tm.tm_hour );

            RdosDecodeLsbTics( lsb,
                               &tm.tm_min,
                               &tm.tm_sec,
                               &ms,
                               &us );
                           
            tm.tm_year -= 1900;
            tm.tm_mon--;
            tm.tm_isdst = -1;
            tm.tm_wday = -1;
            tm.tm_yday = -1;
    
            buf->st_mtime = mktime( &tm );
            buf->st_atime = buf->st_ctime = buf->st_mtime;
            buf->st_size = filelength( handle );
            buf->st_mode |= S_IFREG;
        }
        buf->st_btime = buf->st_mtime;
        return( 0 );
    } else {
        buf->st_rdev = buf->st_dev = 1;
        buf->st_size = 0;
        buf->st_atime = buf->st_btime = buf->st_ctime = buf->st_mtime = 0;
        buf->st_mode |= S_IFCHR;
        return( 0 );
    }    
}

_WCRTLINK int setmode( int handle, int mode )
{
    return( mode );
}

_WCRTLINK int fsync( int handle )
{
    return( -1 );
}

_WCRTLINK int lock( int handle, unsigned long offset, unsigned long nbytes )
{
    return( -1 );
}

_WCRTLINK int unlock( int handle, unsigned long offset, unsigned long nbytes )
{
    return( -1 );
}

_WCRTLINK int (locking)( int handle, int mode, unsigned long nbytes )
{
    return( -1 );
}

_WCRTLINK off_t lseek( int handle, off_t offset, int origin )
{
    off_t pos;

    if( ( handle & FILE_HANDLE ) == FILE_HANDLE) {
        switch( origin ) {
            case SEEK_SET:
                RdosSetFilePos( handle, offset );
                break;
            
            case SEEK_CUR:
                pos = RdosGetFilePos( handle );
                pos += offset;
                RdosSetFilePos( handle, pos );
                break;

            case SEEK_END:
                pos = RdosGetFileSize( handle );
                pos += offset;
                RdosSetFilePos( handle, pos );
                break;
        }
        return( RdosGetFilePos( handle ) );            
    }
    else
        return( -1 );
}

_WCRTLINK off_t tell( int handle )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE)
        return( RdosGetFilePos( handle ) );
    else
        return( -1 );
}

int __qread( int handle, void *buffer, unsigned len )
{
    int i;
    char *ptr = (char *)buffer;
    
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE)
        return( RdosReadFile( handle, buffer, len ) );
    else {
        if( handle == 0) {
            for( i = 2; i < len; i++ ) {
                *ptr = ( char )RdosReadKeyboard( );    
                RdosWriteChar( *ptr );
                if( *ptr == 0xd ) {
                    ptr++;
                    *ptr = 0xa;
                    RdosWriteChar( *ptr );
                    break;
                }
                else
                    ptr++;
            }
            return( i );
        }
        else
            return( 0 );            
    }
}

int __qwrite( int handle, const void *buffer, unsigned len )
{
    if( ( handle & FILE_HANDLE ) == FILE_HANDLE)
        return( RdosWriteFile( handle, buffer, len ) );
    else {
        if( handle == 1 || handle == 2 ) {
            RdosWriteSizeString( ( const char * )buffer, len );
            return( len );
        }
        else
            return( 0 );
    }    
}

_WCRTLINK int read( int handle, void *buffer, unsigned len )
{
    return( RdosReadFile( handle, buffer, len ) );
}

_WCRTLINK int write( int handle, const void *buffer, unsigned len )
{
    return( RdosWriteFile( handle, buffer, len ) );
}

AXI(InitHandle,INIT_PRIORITY_LIBRARY);
AYI(FiniHandle,INIT_PRIORITY_LIBRARY);
