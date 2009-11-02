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
* Description:  OS/2 implementation of fstat().
*
****************************************************************************/


#include "variety.h"
#include "widechar.h"
#include "i64.h"
#include <stddef.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <direct.h>
#include <wos2.h>
#include "iomode.h"
#include "rtcheck.h"
#include "seterrno.h"
#include "d2ttime.h"

#if defined( __WARP__ )
    #define FF_UINT     ULONG
  #if defined( __INT64__ )
    #define FF_LEVEL    FIL_STANDARDL
    #define FF_BUFFER   FILESTATUS3L
  #else
    #define FF_LEVEL    FIL_STANDARD
    #define FF_BUFFER   FILESTATUS3
  #endif
#else
    #define FF_UINT     USHORT
    #define FF_LEVEL    1
    #define FF_BUFFER   FILESTATUS
#endif


static unsigned short at2mode( FF_UINT attr )
/*********************************************/
    {
        register unsigned short         mode;

        if( attr & _A_SUBDIR ) {
            mode = S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
        } else {
            mode = S_IFREG;
        }
        mode |= S_IRUSR | S_IRGRP | S_IROTH;
        if( !(attr & (_A_SYSTEM|_A_RDONLY ) ) ) {
            mode |= S_IWUSR | S_IWGRP | S_IWOTH;
        }
        return( mode );
    }


#ifdef __INT64__
_WCRTLINK int __F_NAME(_fstati64,_wfstati64)( int handle, struct _stati64 *buf )
#else
_WCRTLINK int __F_NAME(fstat,_wfstat)( int handle, struct __F_NAME(stat,_stat) *buf )
#endif
{
    APIRET          error;
    OS_UINT         hand_type;
    OS_UINT         device_attr;
    FF_BUFFER       info;
    unsigned        iomode_flags;

    __handle_check( handle, -1 );

    buf->st_mode = 0;
    iomode_flags = __GetIOMode( handle );
    if( iomode_flags & _READ ) {
        buf->st_mode |= S_IRUSR | S_IRGRP | S_IROTH;
    }
    if( iomode_flags & _WRITE ) {
        buf->st_mode |= S_IWUSR | S_IWGRP | S_IWOTH;
    }

    error = DosQHandType( handle, &hand_type, &device_attr );
    if( error ) {
        return( __set_errno_dos( error ) );
    }
    if( ( hand_type & ~HANDTYPE_NETWORK ) == HANDTYPE_FILE ) {
        /* handle file */
        error = DosQFileInfo( handle, FF_LEVEL, (PBYTE)&info, sizeof( info ) );
        if( error ) {
            return( __set_errno_dos( error ) );
        }
        /* handle attributes */
        buf->st_attr = info.attrFile;
        buf->st_mode |= at2mode( info.attrFile );
        buf->st_dev = buf->st_rdev = 0;
        /* handle timestamps */
        buf->st_ctime = _d2ttime( TODDATE( info.fdateCreation ),
                                  TODTIME( info.ftimeCreation ) );
        buf->st_atime = _d2ttime( TODDATE( info.fdateLastAccess ),
                                  TODTIME( info.ftimeLastAccess ) );
        buf->st_mtime = _d2ttime( TODDATE( info.fdateLastWrite ),
                                  TODTIME( info.ftimeLastWrite ) );
        buf->st_btime = buf->st_mtime;
        /* handle size */
#if defined( __INT64__ ) && defined( __WARP__ )
        U64Set( (unsigned_64 *)&buf->st_size, info.cbFile.ulLo, info.cbFile.ulHi );
#else
        buf->st_size = info.cbFile;
#endif
    } else {
        /* handle device */
        /* handle attributes */
        buf->st_attr = 0;
        buf->st_mode |= S_IRUSR | S_IRGRP | S_IROTH;
        if( ( hand_type & ~HANDTYPE_NETWORK ) == HANDTYPE_DEVICE ) {
            buf->st_mode |= S_IFCHR;
        } else if( ( hand_type & ~HANDTYPE_NETWORK ) == HANDTYPE_PIPE ) {
            buf->st_mode |= S_IFIFO;
        }
        buf->st_dev = buf->st_rdev = 1;
        /* handle timestamps */
        buf->st_atime = 0;
        buf->st_ctime = 0;
        buf->st_mtime = 0;
        buf->st_btime = 0;
        /* handle size */
        buf->st_size = 0;
    }
    buf->st_nlink = 1;
    buf->st_ino = handle;
    buf->st_uid = buf->st_gid = 0;

    buf->st_archivedID = 0;
    buf->st_updatedID = 0;
    buf->st_inheritedRightsMask = 0;
    buf->st_originatingNameSpace = 0;
    return( 0 );
}

