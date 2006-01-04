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
* Description:  Executable dumper I/O routines.
*
****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#ifdef __WATCOMC__
    #include <conio.h>
#endif

#include "wdglb.h"
#include "wdfunc.h"

/*
 * read from the image
 */
void Wread( void *buf, unsigned_32 amount )
/*****************************************/
{
    char    *buffer = buf;

    errno = 0;
    if( amount > Sizeleft ) {
        if( Sizeleft ) {
            memcpy( buffer, &Read_buff[Num_read-Sizeleft], Sizeleft );
            amount -= Sizeleft;
        }
        if( amount/BSIZE >= 1 ) {
            Num_read = read( Handle, &buffer[Sizeleft], amount );
            chkread( amount );
            Sizeleft = 0;
            Num_buff = -1;      // must read in new buffer on next seek
        } else {
            Num_read = read( Handle, Read_buff, BSIZE );
            chkread( amount );
            memcpy( &buffer[Sizeleft], Read_buff, amount );
            Sizeleft = Num_read - amount;
            Num_buff++;
        }
    } else {
        memcpy( buffer, &Read_buff[Num_read-Sizeleft], amount );
        Sizeleft -= amount;
    }
}

/*
 * read from the image
 */
static void chkread( unsigned_32 amount )
/***************************************/
{
    if( Num_read < amount ) {
        Wdputs( "Error! Couldn't read from executable: " );
        if (errno == 0) {
            Wdputs( "reached EOF" );
        } else {
            Wdputs( strerror( errno ) );
        }
        Wdputslc( ".\n" );
        longjmp( Se_env, 1 );
    }
}

/*
 * lseek in the image
 */
void Wlseek( long offset )
/************************/
{
    long    nbuff;

    errno = 0;
    nbuff = offset / BSIZE;
    if( nbuff != Num_buff ) {
        if( lseek( Handle, nbuff * BSIZE, SEEK_SET ) == -1l ) {
            Wdputs( "Error! Couldn't seek in the executable: " );
            Wdputs( strerror( errno ) );
            Wdputslc( ".\n" );
            longjmp( Se_env, 1 );
        }
        Num_buff = nbuff;
        Num_read = read( Handle, Read_buff, BSIZE );
        chkread( offset % BSIZE );
    }
    Sizeleft = Num_read - ( offset % BSIZE );
}

long WFileSize()
/**************/
{
    return filelength( Handle );
}

/*
 * writeout a char
 */
void Wdputc( char c )
/*******************/
{
    if( Options_dmp & LST_REQD ) {
        Write_buff[WSize] = c;
        WSize++;
        if( WSize == BSIZE ) {
            write( Lhandle, Write_buff, BSIZE );
            WSize = 0;
            Write_buff[WSize] = 0;
        }
    } else {
        putchar( c );
    }
}

/*
 * writeout a msg - part of a line
 */
void Wdputs( char *buf )
/**********************/
{
    if( Options_dmp & LST_REQD ) {
        int     len, len2;

        len = strlen( buf );
        if( WSize + strlen( buf ) >= BSIZE ) {
            len2 = BSIZE - WSize ;
            memcpy( &Write_buff[WSize], buf, len2 );
            write( Lhandle, Write_buff, BSIZE );
            memcpy( &Write_buff[0], &buf[len2], len - len2 );
            WSize = len - len2;
        } else {
            memcpy( &Write_buff[WSize], buf, len );
            WSize += len;
        }
    } else {
        fputs( buf, stdout );
    }
}

int Ch = 0;

/*
 * writeout a msg - pause after NUMLINE lines printed if required
 */
void Wdputslc( char * buf )
/*************************/
{
    if( Options_dmp & LST_REQD ) {
        int len, len2;

        len = strlen( buf );
        if( WSize + strlen( buf ) >= BSIZE ) {
            len2 = BSIZE - WSize ;
            memcpy( &Write_buff[WSize], buf, len2 );
            write( Lhandle, Write_buff, BSIZE );
            memcpy( &Write_buff[0], &buf[len2], len - len2 );
            WSize = len - len2;
        } else {
            memcpy( &Write_buff[WSize], buf, len );
            WSize += len;
        }
    } else {
        fputs( buf, stdout );
        if( Options_dmp & PAGE_DMP ) {
            Line_count++;
            if( Line_count == NUMLINE || Ch == 13 ) {
                Line_count = 0;
#ifdef __WATCOMC__
                Ch = getch();
#else
                Ch = getchar();
#endif
            }
            if ( Ch == 113 || Ch == 27 ) {
                close( Handle );
                exit( 1 );
            }
        }
    }
}

void Dump_header( char *data, char **msg )
/****************************************/
{
    for( ; *msg != NULL; ++msg ) {
        Wdputs( &msg[0][1] );
        switch( msg[0][0] ) {
        case '1':
            Wdputs( "      " );
            Puthex( *data, 2 );
            data += sizeof( unsigned_8 );
            break;
        case '2':
            Wdputs( "    " );
            Puthex( *(unsigned_16 *)data, 4 );
            data += sizeof( unsigned_16 );
            break;
        case '4':
            Puthex( *(unsigned_32 *)data, 8 );
            data += sizeof( unsigned_32 );
            break;
        default:
            Wdputs( "*** invalid size" );
            break;
        }
        Wdputslc( "H\n" );
    }
}

#define MAX_FLAG_SIZE   256

extern void DumpFlags( unsigned_32 flags, unsigned_32 ignore, char **msg,
               char *prefix )
/***********************************************************************/
{
    char            name[MAX_FLAG_SIZE];
    char            *currpos;
    unsigned_32     mask;
    size_t          len;

    Wdputs( "          flags = " );
    Puthex( flags, 8 );
    Wdputs( " :  " );
    flags &= ~ignore;
    mask = 1;
    currpos = name;
    len = strlen( prefix );
    if( len > 0 ) {
        memcpy( currpos, prefix, len );
        currpos += len;
        *currpos = '|';
        currpos++;
    }
    while( flags != 0 ) {
        if( flags & 1 ) {
            if( *msg == NULL ) {
            if( currpos - name + 9 > MAX_FLAG_SIZE ) break;
                *currpos = '?';
                currpos++;
                ultoa( mask, currpos, 16 );
                currpos += strlen( currpos );
            } else {
                len = strlen( *msg );
                if( currpos - name + len + 1 > MAX_FLAG_SIZE ) break;
                memcpy( currpos, *msg, len );
                currpos += len;
            }
            *currpos = '|';
            currpos++;
        }
        msg++;
        flags >>= 1;
        mask <<= 1;
    }
    if( currpos != name ) {
        currpos--;
    }
    *currpos = '\0';
    Wdputs( name );
    Wdputslc( "\n" );
}

/*
 * allocate memory
 */
void * Wmalloc( size_t size )
/***************************/
{
    void    *start;

    start = malloc( size );
    if( start == NULL ) {
        Wdputslc( "Error! Dynamic memory exausted.\n" );
        longjmp( Se_env, 1 );
    }
    return( start );
}

/*
 * print out a banner
 */
void Banner( char *title )
/************************/
{
    char    line[LINE_LEN + 1];
    int     title_len;

    line[ LINE_LEN ] = '\0';

    title_len = strlen( title );
    if (title_len > LINE_LEN) {
        title_len = LINE_LEN;
    }

    memset( line, ' ', LINE_LEN );
    memcpy( &line[ (LINE_LEN - title_len) / 2 ], title, title_len );

    Wdputs( line );
    Wdputslc( "\n" );
    memset( line, '=', LINE_LEN );
    Wdputs( line );
    Wdputslc( "\n" );
}
