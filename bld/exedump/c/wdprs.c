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
* Description:  Executable dumper utility routines.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ctype.h>

#include "wdglb.h"
#include "wdfunc.h"

static char             hexchar[] = "0123456789ABCDEF";

/*
 * Put a hex number
 */
void Puthex( unsigned_32 num, unsigned_16 width )
/***********************************************/
{
    while( width-- ) {
        Wdputc( hexchar[ ( num >> ( 4 * width ) ) & 0x000f ] );
    }
}

/*
 * Put a decimal number
 */
void Putdec( unsigned_16 num )
/****************************/
{
    if( num >= 10 )   Putdec( num / 10 );
    Wdputc( num % 10 + '0' );
}

/*
 * Put a signed decimal number
 */
void Putdecs( signed_16 num )
/***************************/
{
    if( num < 0 ) {
        Wdputc( '-' );
        Putdec( -num );
    } else {
        Putdec( num );
    }
}

/*
 * put a decimal number, always printing 'len' characters.
 * this will print 0 if num == 0.
*/

void Putdecl( unsigned_16 num, unsigned_16 len )
/**********************************************/
{
    Putdecbz( num/10, len - 1 );
    Wdputc( num % 10 + '0' );
}

/*
 * put a decimal number, always printing 'len' characters.
 * this will print only spaces if num == 0.
 * (i.e. PUT DECimal Blank when Zero)
*/

void Putdecbz( unsigned_16 num, unsigned_16 len )
/***********************************************/
{
    if( num == 0 ) {
        while( len > 1 ) {
            Wdputc( ' ' );
            len--;
        }
        Wdputc( '0' );
    } else {
        DecBZRecurse( num, len );
    }
}


static void DecBZRecurse( unsigned_16 num, unsigned_16 len )
/**********************************************************/
{
    if( len > 1 ) {
        DecBZRecurse( num / 10, len - 1 );
    }
    if( num == 0 ) {        /* we are to the left of the 1st digit */
        Wdputc( ' ' );
    } else {
        Wdputc( num % 10 + '0' );
    }
}

char *get_file( bool lstf )
/*************************/
{
    unsigned_16     i;
    char            *fname;

    fname = Wmalloc( BUFFERSIZE );
    for( i = 0; *Cmd != '\0'; Cmd++ ) {
        if( *Cmd == '/') break;
        if( lstf ) {
            if( *Cmd == ' ') break;
        }
        if( !isspace( *Cmd ) ) {
            fname[i++] = *Cmd;
        }
    }
    fname[i] = '\0';
    return( fname );
}

void Parse_option( void )
/***********************/
{
    Options_dmp = EXE_INFO;
    Segspec = 0;
    Hexoff = 0;
    skip_blank();
    while( *Cmd == '/' || *Cmd == '-' ) {
        Cmd++;
        options( *Cmd++ );
        skip_blank();
    }
    Name = get_file( FALSE );
    while( *Cmd++ == '/' || *Cmd++ == '-' ) {
        options( *Cmd++ );
        skip_blank();
    }
}

static void options( char ch )
/****************************/
{
    switch( tolower( ch ) ) {
    case 'q':
        Options_dmp |= QUIET;
        break;
    case 'd':
        Options_dmp |= DEBUG_INFO;
        Options_dmp &= ~EXE_INFO;
        Debug_options = 0;
        while( islower( *Cmd ) || isupper( *Cmd ) ) {
            debug_opts( *Cmd++ );
        }
        break;
    case 'e':
        Options_dmp |= EXE_INFO;
        break;
    case 'b':
        Options_dmp |= BINARY_DMP;
        Options_dmp &= ~EXE_INFO;
        skip_blank();
        if( *Cmd == '=' ) {
            get_hex();
        }
        break;
#if 0
    case 'm':
        Debug_info = TRUE;
        Microsoft_dbi = TRUE;
        break;
#endif
    case 'r':
        Options_dmp |= EXE_INFO;
        Options_dmp |= RESRC_DMP;
        break;
    case 'f':
        Options_dmp |= EXE_INFO;
        Options_dmp |= FIX_DMP;
        break;
    case 'a':
        Options_dmp |= FIX_DMP;
        Options_dmp |= RESRC_DMP;           // note fall through
    case 's':
        Options_dmp |= EXE_INFO;
        Options_dmp |= DOS_SEG_DMP;
        Options_dmp |= OS2_SEG_DMP;
        skip_blank();
        if( *Cmd == '=' ) {
            get_number();
        }
        break;
    case 'p':
        Options_dmp |= PAGE_DMP;
        break;
    case 'l':
        Options_dmp |= LST_REQD;
        if( *Cmd == '=' ) {
            Cmd++;
            Lstf = get_file( TRUE );
        }
        break;
    case 'i':
        Options_dmp |= IMPORT_LIB;
        Options_dmp |= QUIET;
        Options_dmp &= ~EXE_INFO;
        break;
    default:
        Wdputs( "invalid option:  " );
        Wdputc( ch );
        Wdputslc( "\n" );
    }
}

static void get_hex( void )
/*************************/
{
    unsigned        i,m;
    unsigned        j,l;
    char            *num;

    Cmd++;
    l = 0;
    num = Wmalloc( 16 );
    for( m = 0; isxdigit( *Cmd ); Cmd++ ) {
        num[m++] = *Cmd;
    }
    num[m] = '\0';
    for( i = 0; num[i] != '\0'; i++ ) {
        for( j = 0; hexchar[j] != '\0'; j++ ) {
            if( toupper( num[m-i-1] ) == hexchar[j] ) break;
        }
        if( l == 0 ) {
            l++;
        } else {
            l = l * 16;
        }
        Hexoff += j * l;
    }
}

static void get_number( void )
/****************************/
{
    unsigned_16     i;
    char            *num;

    Cmd++;
    num = Wmalloc( 16 );
    for( i = 0; isdigit( *Cmd ); Cmd++ ) {
        num[i++] = *Cmd;
    }
    num[i] = '\0';
    Segspec = atoi( num );
    if( Segspec == 0 ) {
        Options_dmp &= ~OS2_SEG_DMP;
    } else {
        Options_dmp &= ~DOS_SEG_DMP;
    }
}

/*
 * skip the blanks
 */
static void skip_blank( void )
/****************************/
{
    while isspace( *Cmd ) {
        Cmd++;
    }
}

/*
 * debug options
 */
static void debug_opts( char ch )
/*******************************/
{
    switch( tolower( ch ) ) {
    case 'x':
        Debug_options = 0xff;
        break;
    case 'm':
        Debug_options |= MODULE_INFO;
        break;
    case 'g':
        Debug_options |= GLOBAL_INFO;
        break;
    case 'a':
        Debug_options |= ADDR_INFO;
        break;
    case 'n':
        Debug_options |= LINE_NUMS;
        break;
    case 'l':
        Debug_options |= LOCALS;
        break;
    case 't':
        Debug_options |= TYPES;
        break;
    }
}
