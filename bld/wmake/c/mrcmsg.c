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
* Description:  Message resources access functions.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef __WATCOMC__
    #include <process.h>
#endif

#include "watcom.h"
#include "mtypes.h"
#include "mcache.h"
#include "msysdep.h"
#include "mrcmsg.h"
#include "autodep.h"
#include "wressetr.h"
#include "wreslang.h"


#define NIL_HANDLE      ((int)-1)
#define STDOUT_HANDLE   ((int)1)

#define FORMTABLE
static  TABLE_TYPE      PARA_TABLE[] = {
#include "mrcmsg.h"
#undef  FORMTABLE

static  HANDLE_INFO     hInstance = { 0 };
static  unsigned        MsgShift;
extern  long            FileShift;

#define NO_RES_MESSAGE "Error: could not open message resource file.\r\n"
#define NO_RES_SIZE (sizeof(NO_RES_MESSAGE)-1)


static long resSeek( int handle, long position, int where )
/* fool the resource compiler into thinking that the resource information
 * starts at offset 0 */
{
    if( where == SEEK_SET ) {
        return( lseek( handle, position + FileShift, where ) - FileShift );
    } else {
        return( lseek( handle, position, where ) );
    }
}

WResSetRtns( open, close, read, write, resSeek, tell, malloc, free );

extern int MsgInit()
{
    int         initerror;
    char        name[_MAX_PATH];

    hInstance.handle = NIL_HANDLE;
    if( _cmdname( name ) == NULL ) {
        initerror = 1;
    } else {
        hInstance.filename = name;
        OpenResFile( &hInstance );
        if( hInstance.handle == NIL_HANDLE ) {
            initerror = 1;
        } else {
            initerror = FindResources( &hInstance );
            if( !initerror ) {
                initerror = InitResources( &hInstance );
            }
        }
    }
    MsgShift = WResLanguage() * MSG_LANG_SPACING;
    if( !initerror && !MsgGet( USAGE_BASE, name ) ) {
        initerror = 1;
    }
    if( initerror ) {
        write( STDOUT_FILENO, NO_RES_MESSAGE, NO_RES_SIZE );
        MsgFini();
        return( 0 );
    }
    return( 1 );
}

extern int MsgGet( int resourceid, char *buffer )
{
    if( LoadString( &hInstance, resourceid + MsgShift,
        (LPSTR) buffer, MAX_RESOURCE_SIZE ) == -1 ) {
        buffer[0] = '\0';
        return( 0 );
    }
    return( 1 );
}

extern void MsgGetTail( int resourceid, char *buffer )
{
    char        msg[MAX_RESOURCE_SIZE];
    char        *p;

    MsgGet( resourceid, msg );
    p = strchr( msg, '%' );
    while( p != NULL ) {
        if( *(++p) == 'L' ) {
            break;
        }
        p = strchr( ++p, '%' );
    }
    if( p != NULL ) {
        strcpy( buffer, (++p) );
    }
}

extern void MsgFini()
{
    if( hInstance.handle != NIL_HANDLE ) {
        CloseResFile( &hInstance );
        hInstance.handle = NIL_HANDLE;
    }
}

static char *msgInTable( int resourceid )
{
    int         i;

    for( i = 0; PARA_TABLE[i].msgid < END_OF_RESOURCE_MSG; i++ ) {
        if( resourceid == PARA_TABLE[i].msgid ) {
            return( PARA_TABLE[i].msgtype );
        }
        if( resourceid < PARA_TABLE[i].msgid ) {
            break;
        }
    }
    return( NULL );
}

extern int MsgReOrder( int resourceid, char *buff, char **paratype )
{
    int         rvalue = 0;

    MsgGet( resourceid, buff );
    *paratype = msgInTable( resourceid );
    if( *paratype != NULL ) {
        buff = strchr( buff, '%' );
        while( buff != NULL ) {
            if( *(buff+1) == '%' ) {
                buff++;
            } else if( *(buff+1) == **paratype ) {
                break;
            } else if( *(buff+1) == *(*paratype+1) ) {
                rvalue = 1;
                break;
            }
            buff = strchr( (buff+1), '%' );
        }
    }
    return( rvalue );
}
