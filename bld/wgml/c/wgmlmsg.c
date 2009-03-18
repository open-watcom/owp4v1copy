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
* Description:  WGML message definition.
*               adapted from wrc file (bld\sdk\rc\rc\c\rcldstr.c)
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <string.h>
#include "wresall.h"
#include "layer0.h"
#include "errors.h"
#include "global.h"
#include "fcntl.h"
#if !defined( __UNIX__ ) || defined( __WATCOMC__ )
#include "process.h"
#endif
#include "iortns.h"
#include "wgml.h"

#if defined( INCL_MSGTEXT )

#undef pick
#define pick( id, en, jp )  en,

static char *StringTable[] = {
    #include "wgml.msg"
};

int init_msgs( void ) { return( 1 ); }

int get_msg( msg_ids resid, char *buff, unsigned buff_len )
{
    strcpy( buff, StringTable[resid] );
    return( 1 );
}

void fini_msgs( void ) {}

#else

#include "wreslang.h"

static unsigned MsgShift;

int init_msgs( void )
{
    int         error;
    char        fname[_MAX_PATH];
    WResFileID  (*oldopen) (const char *, int, ...);

    error = FALSE;
    if( _cmdname( fname ) == NULL ) {
        error = TRUE;
    } else {
        Instance.filename = fname;

        /* swap open functions so this file handle is not buffered.
         * This makes it easier for layer0 to fool WRES into thinking
         * that the resource information starts at offset 0 */
        oldopen = WResRtns.open;
        WResRtns.open = open;
        OpenResFile( &Instance );
        WResRtns.open = oldopen;

        if( Instance.handle == -1 ) error = TRUE;
        if( !error ) {
            RegisterOpenFile( Instance.handle );
            error = FindResources( &Instance );
        }
        if( !error ) {
            error = InitResources( &Instance );
        }
        MsgShift = WResLanguage() * MSG_LANG_SPACING;
        if( !error && !get_msg( USAGE_MSG_FIRST, fname, sizeof( fname ) ) ) {
            error = TRUE;
        }
    }
    if( error ) {
        if( Instance.handle != -1 ) CloseResFile( &Instance );
        out_msg( "Resources not found\n" );
        g_suicide();
    }
    return( 1 );
}

int get_msg( msg_ids resid, char *buff, unsigned buff_len )
{
    if( WResLoadString( &Instance, resid + MsgShift, buff, buff_len ) != 0 ) {
        buff[0] = '\0';
        return( 0 );
    }
    return( 1 );
}

void fini_msgs( void ) {
    CloseResFile( &Instance );
}

#endif
