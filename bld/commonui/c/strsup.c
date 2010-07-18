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
* Description:  Support functions for using strings loaded from resources.
*
****************************************************************************/


#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "mem.h"
#include "ldstr.h"

// No string to be loaded can be more than LDSTR_MAX_STR_LEN bytes long
#define LDSTR_MAX_STR_LEN       500

static char     getStringBuffer[ LDSTR_MAX_STR_LEN ];
static char     tmpBuf[ LDSTR_MAX_STR_LEN ];
static HANDLE   curInst;

/*
 * GetRCString - return a pointer to a string from the resource file.
 *              NB the pointer is only valid until the next call to
 *              GetString
 */
char *GetRCString( DWORD msgid )
{
    LoadString( curInst, msgid, getStringBuffer, LDSTR_MAX_STR_LEN );
    return( getStringBuffer );
}

char *AllocRCString( DWORD id )
{
    char        *ret;
    int         len;

    len = LoadString( curInst, id, tmpBuf, LDSTR_MAX_STR_LEN );
    ret = MemAlloc( len + 1 );
    if( ret != NULL ) {
        strcpy( ret, tmpBuf );
    }
    return( ret );
}

DWORD CopyRCString( DWORD id, char *buf, DWORD bufsize )
{
    DWORD       len;

    len = LoadString( curInst, id, buf, bufsize );
    return( len );
}

void FreeRCString( char *str )
{
    MemFree( str );
}

void SetInstance( HANDLE inst )
{
    curInst = inst;
}
