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
* Description:  Linux module to load debugger trap files.
*
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "trpimp.h"
#include "tcerr.h"
#include "peloader.h"

static PE_MODULE *TrapFile;
static trap_version (TRAPENTRY *InitFunc)();
static void (TRAPENTRY *FiniFunc)();
extern trap_version     TrapVer;
extern unsigned         (TRAPENTRY *ReqFunc)( unsigned, mx_entry *, unsigned, mx_entry * );

extern  int      PathOpen(char *,unsigned, char *);
extern  int      GetSystemHandle(int);

void KillTrap()
{
    ReqFunc = NULL;
    if( FiniFunc != NULL ) FiniFunc();
    FiniFunc = NULL;
    InitFunc = NULL;
    if( TrapFile != 0 ) PE_freeLibrary( TrapFile );
    TrapFile = 0;
}

char *LoadTrap( char *trapbuff, char *buff, trap_version *trap_ver )
{
    char                init_error[256];
    int                 filehndl;
    char                *ptr;
    char                *parm;

    if( trapbuff == NULL ) trapbuff = "std";
    for( ptr = trapbuff; *ptr != '\0' && *ptr != ';'; ++ptr ) ;
    parm = (*ptr != '\0') ? ptr + 1 : ptr;
    filehndl = PathOpen( trapbuff, ptr - trapbuff, "trp" );

    parm = (*ptr != '\0') ? ptr + 1 : ptr;

    TrapFile = PE_loadLibrary_handle( GetSystemHandle( filehndl ) );
    if( TrapFile == NULL ) {
        TrapFile = 0;
        strcpy( buff, TC_ERR_CANT_LOAD_TRAP );
        return( buff );
    }
    InitFunc = PE_getProcAddress( TrapFile, "TrapInit_" );
    FiniFunc = PE_getProcAddress( TrapFile, "TrapFini_" );
    ReqFunc  = PE_getProcAddress( TrapFile, "TrapRequest_" );
    strcpy( buff, TC_ERR_WRONG_TRAP_VERSION );
    if( InitFunc == NULL || FiniFunc == NULL || ReqFunc == NULL) {
        KillTrap();
        return( buff );
    }
    *trap_ver = InitFunc( parm, init_error, trap_ver->remote );
    if( init_error[0] != '\0' ) {
        KillTrap();
        strcpy( buff, init_error );
        return( buff );
    }
    if( !TrapVersionOK( *trap_ver ) ) {
        KillTrap();
        return( buff );
    }
    TrapVer = *trap_ver;
    return( NULL );
}

