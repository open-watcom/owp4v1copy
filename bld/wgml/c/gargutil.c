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
* Description: utility functions for arguments:
*         garginit             --- initialize operand scan in buff2 (SCR)
*         garginitdot          --- initialize operand scan in buff2 (GML)
*         getarg               --- scan (quoted) blank delimited argument
*         getqst               --- scan quoted string
*         test_xxx_char        --- test for allowed char
*         unquote_if_quoted    --- adjust ptrs inside quoted string
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
    while( *p != ' ' && p <= scan_stop ) {// search end of script control word
        p++;
    }
    scan_start = p;                     // store control word end address

    open_paren = NULL;                  // clear open parenthesis pointer
    clos_paren = NULL;                  // clear close parenthesis pointer
    tok_start = NULL;                   // clear token start address
}




void    garginitdot( void )
{
    char    *   p;

    p = buff2;                          // adress of input buffer
    scan_stop = buff2 + buff2_lg - 1;   // store scan stop address
    while( *p != ' ' && *p != '.' && p <= scan_stop ) {// search end of gml tag
        p++;
    }
    scan_start = p;                     // store tag end or space address

    open_paren = NULL;                  // clear open parenthesis pointer
    clos_paren = NULL;                  // clear close parenthesis pointer
    tok_start = NULL;                   // clear token start address
}


/***************************************************************************/
/*  scan blank delimited argument perhaps quoted                           */
/*                                                                         */
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



/***************************************************************************/
/*  scan       quoted string argument                                      */
/***************************************************************************/

condcode    getqst( void )
{
    condcode    cc;
    char    *   p;
    char        c;
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
        c = *p;
        if( c == '\'' || c == '"' || c == '\x7c' || c == '\x9b' ) {
            quote = c;     // single and double quotes, vertical bar and cent
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
            if( *p == quote && *(p+1) != quote ) {// 2 quote chars not end of string
                break;
            }
            if( *p == '\0' ) {
                break;
            }
        }
        if( quoted ) {
            tok_start++;
            scan_start = p + 1;         // start address for next call
            arg_flen = p - tok_start;   // length of arg
        } else {
            scan_start = p;             // address of start for next call
            arg_flen = p - tok_start;   // length of arg
        }
        if( arg_flen > 0 ) {
            if( quoted ) {
                if( *p != quote ) {
                    cc = no;            // only start quote found
                } else {
                    cc = quotes;        // quoted arg found
                }
            } else {
                cc = no;                // not quoted
            }
        } else {
            cc = omit;                  // length zero
        }
    }
    return( cc );
}


/*
 * Test character as valid for an function name
 */
bool    test_function_char( char c )
{
    bool    test;

    test = isalnum( c );
    return( test );
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
    if( !test ) {
        test = ( c == '@' ) || ( c == '#' ) || ( c == '$' ) || ( c == '_' );
    }
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

/*
 * If first and last character are the same and one of the quote chars
 * the start and end pointers are adjusted
 */
void    unquote_if_quoted( char * * a, char * * z )
{
#define s_q     '\''
#define d_q     '\"'
#define vbar    0xdd
#define cent    0x9b

    if( **a == **z ) {
        if( (**a == d_q) || (**a == s_q)
            || (**a == vbar) || ( **a == cent) ) {

            *a += 1;
            *z -= 1;
        }
    }
}


