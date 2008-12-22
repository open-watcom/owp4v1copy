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
* Description: functions:
*                  garginit      --- initialize operand scan in buff2 (SCR)
*                  garginitdot   --- initialize operand scan in buff2 (GML)
*                  getarg        --- scan (quoted) blank delimited argument
*                  test_xxx_char --- test for allowed char
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


void    garginit( void )
{
    char    *   p;

    p = buff2;                          // adress of input buffer
    scan_stop = buff2 + buff2_lg - 1;   // store scan stop address
    while( *p != ' ' && p <= scan_stop ) {  // search end of keyword
        p++;
    }
    scan_start = p;                     // store argument start address

    open_paren = NULL;                  // clear open parenthesis pointer
    clos_paren = NULL;                  // clear close parenthesis pointer
    tok_start = NULL;                   // clear token start address
}




void    garginitdot( void )
{
    char    *   p;

    p = buff2;                          // adress of input buffer
    scan_stop = buff2 + buff2_lg - 1;   // store scan stop address
    while( *p != ' ' && *p != '.' && p <= scan_stop ) {// search end of keyword
        p++;
    }
    scan_start = p;                     // store argument start address

    open_paren = NULL;                  // clear open parenthesis pointer
    clos_paren = NULL;                  // clear close parenthesis pointer
    tok_start = NULL;                   // clear token start address
}


/***************************************************************************/
/*  scan blank delimited argument perhaps quoted                           */
/***************************************************************************/

condcode    getarg( void )
{
    condcode    cc;
    char    *   p;
    char        quote;
    bool        quoted;

    if( scan_stop <= scan_start ) {     // already at end
        cc = omit;                      // arg omitted
    } else {
        p = scan_start;
        while( *p && *p == ' ' && p <= scan_stop ) {// skip leading blanks
            p++;
        }

        tok_start = p;
        if( *p == '\'' || *p == '"' ) {
            quote = *p;
            p++;
            quoted = true;
        } else {
            quote = '\0';
            quoted = false;
        }
        for( ; p <= scan_stop; p++ ) {

            if( *p == ' ' && quote == '\0' ) {
                break;
            }
            if( *p == quote ) {
                break;
            }
            if( *p == '\0' ) {
                break;
            }
        }
        if( quoted ) {
            tok_start++;
            scan_start = p + 1;         // address of start for next call
            arg_flen = p - tok_start;   // length of arg
        } else {
            scan_start = p;             // address of start for next call
            arg_flen = p - tok_start;   // length of arg
        }
        if( arg_flen > 0 ) {
            if( quoted ) {
                cc = quotes;            // quoted arg found
            } else {
                cc = pos;               // arg found
            }
        } else {
            cc = omit;                  // length zero
        }
    }
    return( cc );
}


/*
 * Test character as valid for an identifier name
 */
bool    test_identifier_char( char c )
{
    bool    test;

    test = isalnum( c );
    return( test );
}

/*
 * Test character as valid for a macro name
 */
bool    test_macro_char( char c )
{
    bool    test;

    test = isalnum( c );
    return( test );
}

/*
 * Test character as valid for a symbol name
 */
bool    test_symbol_char( char c )
{
    bool    test;

    test = isalnum( c );
    if( !test ) {
        test = ( c == '@' ) || ( c == '#' ) || ( c == '$' ) || ( c == '_' );
    }
    return( test );
}
