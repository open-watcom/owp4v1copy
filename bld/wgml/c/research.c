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
* Description:  Implements the functions common to the research programs:
*                   display_char()
*                   parse_cmdline()
*                   res_initialize_globals()
*
****************************************************************************/

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#define __STDC_WANT_LIB_EXT1__  1 // Activates "Safe C" functions
#include <string.h>
#include "cmdlhelp.h"
#include "common.h"
#include "wstd.h"

/* Global variables */

#define global
#include "research.h"

/* Local data definition */ 

static char     hexchar[] = "0123456789ABCDEF";

/*  Function display_char().
 *  If isgraph() indicates that in_char is displayable, returns a space in
 *  out_chars[0] and in_char in out_chars[1]. Otherwise returns values in
 *  out_chars which, when output together, provide a hexadecimal representation
 *  of in_char. Since isgraph() is used, the space character will appear in
 *  hexadecimal.
 *
 *  Parameters:
 *      out_chars points to a two-char array 
 *      in_char contains the value to be output
 *
 *  Value Returned:
 *      The values indicated above are returned in the out_chars.
 */
void    display_char( char * out_chars, char in_char)
{
    if ( isgraph( in_char ) ) {
        out_chars[0] = ' ';
        out_chars[1] = in_char;
    } else {
        out_chars[0] = hexchar[ ( in_char >> 4 ) & 0x0f ];
        out_chars[1] = hexchar[ in_char & 0x0f ];
    }
    
    return;    
}

/*  Function parse_cmdline().
 *  Extract the path entered by the user into tgt_path. Doublequotes used
 *  to allow embedded spaces are removed.
 *
 *  Parameter:
 *      cmdline contains the command line without the program name
 *
 *  Global Used:
 *      tgt_path is loaded with the directory provided to the program, with any
 *          doublequotes removed
 *
 *  Returns:
 *      FAILURE on any error
 *      SUCCESS if all goes well
 */

int parse_cmdline( char * cmdline )
{
    char *      end;
    char        opt;
    ptrdiff_t   len;

    /* Find the parameter -- there should only be one */
        
    cmdline = skip_spaces( cmdline );

    /* In case someone finds a way to enter an all-whitespace command line */
    
    if( *cmdline == NULLCHAR ) {
        print_usage();
        return ( FAILURE );
    }

    /* Verify that parameter is not adorned */
 
    opt = *cmdline;
    if( opt == '-'  ||  opt == switch_char  ||  opt == '@' ) {
        print_usage();
        return( FAILURE );
    }

    /* Find the length of the parameter */

    end = cmdline;
    end = FindNextWS( end );
    len = end - cmdline;

    /* In case someone managed to enter a zero-length path */
        
    if( len <= 0)
    {
        print_usage();
        return( FAILURE );
    }
        
    /* Allocate memory for the global pointer tgt_path */
        
    len++; /* To allow space for the final null character */
    tgt_path = malloc( len );
    if( tgt_path == NULL ) return( FAILURE );

    /* Copy the parameter into tgt_path */
    /* It will be null-terminated per documentation of strncpy_s() */

    strncpy_s( tgt_path, len, cmdline, len-1 );

    /* Remove doublequotes, if present */

    if(opt == '\"') UnquoteFName( tgt_path, len, tgt_path );

    /* We are done */

    return( SUCCESS );

}

/*  Function res_initialize_globals().
 *  Initialize the global variable common to the research functions.
 */

void res_initialize_globals( void )
{
    tgt_path = NULL;
}


