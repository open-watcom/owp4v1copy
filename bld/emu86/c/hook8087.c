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
* Description:  Intel x87 emulator hook for 16-bit OS/2.
*
****************************************************************************/


#include <string.h>
#include <i86.h>

#define INCL_DOSDEVICES
#define INCL_SUB
#define INCL_DOSSEMAPHORES
#define INCL_DOSINFOSEG
#define INCL_DOSMISC
#include <wos2.h>

extern void __interrupt __far __int7( void );
#pragma aux __int7 "*";

static void __interrupt (*old_int7)( void );

static int checkenv_NO87( void )
{
    unsigned short  seg;
    unsigned short  count;
    char __far      *p;

    DosGetEnv( (PUSHORT)&seg, (PUSHORT)&count );
    p = (char __far *)MK_FP( seg, 0 );
    while( *p ) {
        if( _fstrnicmp( p, "NO87=", 5 ) == 0 )
            return( 1 );
        while( *p )
            p++;
        p++;          // skip 0
    }
    return( 0 );
}


int __far __hook8087( void )
/**************************/
{
    char    devinfo;

    DosDevConfig( &devinfo, 3, 0 ); /* returns 1=present 0=notpresent */
    if( checkenv_NO87() || ( devinfo == 0 ) ) {
        DosSetVec( 7, (PFN)&__int7, (PFN FAR*)&old_int7 );
        return( 0 );
    }
    return( devinfo );
}

void __far __unhook8087( void )
/*****************************/
{
    if( old_int7 != NULL ) {
        DosSetVec( 7, (PFN)old_int7, (PFN FAR*)&old_int7 );
    }
}
