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
* Description:  Implements the common functions for the research code:
*                   initialize_globals()
*                   skip_spaces()
*
*               and those needed to reproduce enough of the wgml context for
*               research programs that use parts of wgml to work:               
*                   free_resources()
*                   g_suicide()
*                   mem_alloc()
*                   mem_free()
*                   mem_realloc()
*                   out_msg()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "swchar.h"

/* Define the global variables. */

#define global
#include "common.h"

/*
 *  Initialize the global variables.
 */

void    initialize_globals( void )
{
    switch_char     = _dos_switch_char();
    err_count       = 0;
    wng_count       = 0;
    
    dev_name        = NULL;
    master_fname    = NULL;
    opt_fonts       = NULL;
    out_file        = NULL;
    out_file_attr   = NULL;
}

/*  Function skip_spaces().
 *  Skip whitespace (as defined by isspace()).
 *
 *  Parameter:
 *  start contains the start position.
 *
 *  Returns:
 *  the position of the first non-whitespace character encountered.
 */

char *  skip_spaces( char * start )
{
    while( isspace( *start ) ){
    start++;
    }
    return start;
}

/* Borrowed from wgml. */

/* Error message centralized output. */

void out_msg( char * msg, ... )
{
    va_list args;

    va_start( args, msg );
    vprintf_s( msg, args );
    va_end( args );
}

/* The memory allocation functions. These have been simplified. */

void * mem_alloc( size_t size )
{
    void    *   p;

    p = malloc( size );
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL\n" );
        g_suicide();
    }
    return( p );
}

void * mem_realloc( void * p, size_t size )
{
    p = realloc( p, size );
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL\n" );
        g_suicide();
    }
    return( p );
}

void mem_free( void * p )
{

    /* Not an error, but report for debugging purposes. */

    if( p == NULL ) {
        out_msg( "NULL pointer freed!\n" );
    }

    free( p );
    p = NULL;
}

void g_suicide( void )
{
    exit( 16 );
}

bool free_resources( errno_t in_errno )
{
    if( in_errno == ENOMEM) out_msg( "ERR_NOMEM_AVAIL\n" );
    else out_msg( "Out of file handles!\n" );
    return( false );
}


