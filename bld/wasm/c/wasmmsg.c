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


#include "asmglob.h"

#include <fcntl.h>
#include <unistd.h>

#ifdef __WATCOMC__
  #include <conio.h>
  #include <process.h>
#endif

#include "wressetr.h"
#include "wreslang.h"


#define NIL_HANDLE      ((int)-1)
#define STDOUT_HANDLE   ((int)1)

static  HANDLE_INFO     hInstance = { 0 };
static  unsigned        MsgShift;
extern  long            FileShift;

extern  int             trademark( void );

#ifdef __OSI__

extern char             *_Copyright;

#endif

#define NO_RES_MESSAGE "Error: could not open message resource file.\r\n"
#define NO_RES_SIZE (sizeof(NO_RES_MESSAGE)-1)

#ifndef __UNIX__

static const unsigned char PressReturn[] = {
"    (Press return to continue)"
};

static void con_output( const unsigned char *text )
{
    char c;

    do {
        c = *text;
        putchar( c );
        ++text;
    } while( *text );
    putchar( '\n' );
}
#endif

static off_t res_seek( int handle, off_t position, int where )
/* fool the resource compiler into thinking that the resource information
 * starts at offset 0 */
{
    if( where == SEEK_SET ) {
        return( lseek( handle, position + FileShift, where ) - FileShift );
    } else {
        return( lseek( handle, position, where ) );
    }
}

WResSetRtns( open, close, read, write, res_seek, tell, malloc, free );

int MsgInit( void )
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
    if( !initerror && !MsgGet( MSG_USE_BASE, name ) ) {
        initerror = 1;
    }
    if( initerror ) {
        write( STDOUT_FILENO, NO_RES_MESSAGE, NO_RES_SIZE );
        MsgFini();
        return( 0 );
    }
    return( 1 );
}

int MsgGet( int resourceid, char *buffer )
{
    if( LoadString( &hInstance, resourceid+MsgShift, (LPSTR) buffer, 128 ) != 0 ) {
        buffer[0] = '\0';
        return( 0 );
    }
    return( 1 );
}

void MsgPrintf( int resourceid )
{
    char        msgbuf[128];

    if( !Options.banner_printed ) {
        Options.banner_printed = TRUE;
        trademark();
    }
    MsgGet( resourceid, msgbuf );
    printf( msgbuf );
}

void MsgPrintf1( int resourceid, char *token )
{
    char        msgbuf[128];

    if( !Options.banner_printed ) {
        Options.banner_printed = TRUE;
        trademark();
    }
    MsgGet( resourceid, msgbuf );
    printf( msgbuf, token );
}

#ifndef __UNIX__
static void Wait_for_return( void )
{
    if( isatty( fileno(stdout) ) ) {
        con_output( PressReturn );
        fflush( stdout );
        getch();
    }
}
#endif

void PrintfUsage( int first_ln )
{
    char        msg_buff[128];
    unsigned    count;

    count = trademark();
#ifdef __OSI__
    if( _Copyright != NULL ) {
        puts( _Copyright );
        count += 1;
    }
#endif
    for( ;; first_ln++ ) {
#ifndef __UNIX__
        if( ++count >= 23 ) {
            Wait_for_return();
            count = 0;
        }
#endif
        MsgGet( first_ln, msg_buff );
        if( ( msg_buff[ 0 ] == '.' ) && ( msg_buff[ 1 ] == 0 ) )
            break;
        puts( msg_buff );
    }
}

void MsgFini( void )
{
    if( hInstance.handle != NIL_HANDLE ) {
        CloseResFile( &hInstance );
        hInstance.handle = NIL_HANDLE;
    }
}
