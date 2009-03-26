/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Error and warning message output.
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"
#include <stdarg.h>

#define MAX_ERR_LEN     1020

static  char    err_buf[ MAX_ERR_LEN + 2 ]; // +2 for \n and \0
static  char    str_buf[ MAX_ERR_LEN + 2 ];



void g_suicide( void )
{
    out_msg( "\n\nWGML suicide\n\n" );
    if( environment ) {
        longjmp( *environment, 1 );
    }
    my_exit( 16 );
}


void out_msg( const char *msg, ... )
{
    va_list args;

    va_start( args, msg );
    vprintf_s( msg, args );
    va_end( args );
}


/***************************************************************************/
/*  construct msg  inserting string variables optionally                   */
/***************************************************************************/

#define MAX_LINE_LEN            75
static void g_msg_var( msg_ids errornum, int sev, va_list arglist )
/*****************************************************************/
{
    int                 len;
    const char      *   prefix;
    int                 indent;
    char            *   start;
    char            *   end;

    switch( sev ) {
#if 0
    case SEV_INFO:
        prefix = "Info:";
        break;
#endif
    case SEV_WARNING:
        prefix = "Warning!";
        break;
    case SEV_ERROR:
        prefix = "Error!";
        break;
    case SEV_FATAL_ERR:
        prefix = "Fatal Error!";
        break;
    default:
        prefix = "";
        break;
    }

    switch( errornum ) {
    case ERR_STR_NOT_FOUND:
        /* this message means the error strings cannot be obtained from
         * the exe so its text is hard coded */
        sprintf( err_buf, "%s %d: %nResource strings not found", prefix,
                    errornum, &len );
        break;
    case ERR_DUMMY:
        /* dont print anything */
        return;
    default:
        get_msg( errornum, err_buf, sizeof( err_buf ) );
        vsprintf( str_buf, err_buf, arglist );
        if( *prefix == '\0' ) {
            // no prefix and errornumber
            sprintf( err_buf, "%n%s", &len, str_buf );
        } else {
            sprintf( err_buf, "%s %d: %n%s", prefix, errornum, &len, str_buf );
        }
        break;
    }

    indent = 0;
    start = err_buf;
    while( strlen( start ) > MAX_LINE_LEN - indent ) {
        end = start + MAX_LINE_LEN - indent;
        while( !isspace( *end ) && end > start ) end--;
        if( end != start )  {
            *end = '\0';
        } else {
            break;
        }
        out_msg( "%*s%s\n", indent, "", start );
        start = end + 1;
        indent = len;
    }
    out_msg( "%*s%s\n", indent, "", start );
}

/***************************************************************************/
/*  error msg                                                              */
/***************************************************************************/

void g_err( const msg_ids num, ... )
{
    va_list args;

    va_start( args, num );
    g_msg_var( num, SEV_ERROR, args );
    va_end( args );
}

/***************************************************************************/
/*  warning msg                                                            */
/***************************************************************************/

void g_warn( const msg_ids num, ... )
{
    va_list args;

    va_start( args, num );
    g_msg_var( num, SEV_WARNING, args );
    va_end( args );
}

/***************************************************************************/
/*  informational msg                                                      */
/***************************************************************************/

void g_info( const msg_ids num, ... )
{
    va_list args;

    va_start( args, num );
    g_msg_var( num, SEV_INFO, args );
    va_end( args );
}

