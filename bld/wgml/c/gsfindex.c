/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML implement multi letter function &'index( ) and &'pos( )
*               They are only different in the parameter order:
*                   &'index( haystack, needle,   ... )
*                   &'pos  ( needle,   haystack, ... )
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*  script string function &'index(                                        */
/*  script string function &'pos(                                          */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* &'index(haystack,needle<,start>):   The  Index  function  returns  the  */
/*    character position of the string 'needle' in the string 'haystack'.  */
/*    If not found, the function returns zero.  The first character posi-  */
/*    tion to be searched in 'haystack' may be specified with the 'start'  */
/*    number and  it defaults to the  first character position  in 'hays-  */
/*    tack'.                                                               */
/*      &'index('abcdef','c') ==> 3                                        */
/*      &'index('abcdef','cd') ==> 3                                       */
/*      &'index('abcdef','yz') ==> 0                                       */
/*      &'index('ab','abcdef') ==> 0                                       */
/*      &'index('ababab','ab',2) ==> 3                                     */
/*      &'index('ababab','ab',6) ==> 0                                     */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/*                                                                         */
/* &'pos(needle,haystack<,start>):   The  Position function  returns  the  */
/*    character position  of the first  occurrence of 'needle'  in 'hays-  */
/*    tack'.   The  search for a  match starts  at the first  position of  */
/*    'haystack' but  may be  overridden by  adding a  'start' column  in  */
/*    'haystack'.    If the  'needle' string  is  not found,   a zero  is  */
/*    returned.                                                            */
/*      &'pos('c','abcde') ==> 3                                           */
/*      &'pos(x,abcde) ==> 0                                               */
/*      &'pos(abcde,abcde) ==> 1                                           */
/*      &'pos(a,aaaaa) ==> 1                                               */
/*      &'pos('a','aaaaa') ==> 1                                           */
/*      &'pos(a,aaaaa,3) ==> 3                                             */
/*      &'pos(12345678,abc) ==> 0                                          */
/*      &'pos(a) ==> error, too few operands                               */
/*      &'pos(a,abcd,junk) ==> error, 'start' not numeric                  */
/*      &'pos(a,abcd,3,'.') ==> error, too many operands                   */
/*                                                                         */
/***************************************************************************/

condcode    scr_index( parm parms[ MAX_FUN_PARMS ], size_t parmcount, char * * result )
{
    char            *   pneedle;
    char            *   pneedlend;
    char            *   phay;
    char            *   phayend;
    condcode            cc;
    int                 index;
    int                 n;
    int                 hay_len;
    int                 needle_len;
    getnum_block        gn;
    char            *   ph;
    char            *   pn;

    if( (parmcount < 2) || (parmcount > 3) ) {
        cc = neg;
        return( cc );
    }

    phay = parms[ 0 ].a;
    phayend = parms[ 0 ].e;

    unquote_if_quoted( &phay, &phayend );
    hay_len = phayend - phay + 1;       // haystack length

    pneedle = parms[ 1 ].a;
    pneedlend = parms[ 1 ].e;

    unquote_if_quoted( &pneedle, &pneedlend );
    needle_len = pneedlend - pneedle + 1;   // needle length

    n   = 0;                            // default start pos
    gn.ignore_blanks = false;

    if( parmcount > 2 ) {               // evalute start pos
        if( parms[ 2 ].e >= parms[ 2 ].a ) {// start pos specified
            gn.argstart = parms[ 2 ].a;
            gn.argstop  = parms[ 2 ].e;
            cc = getnum( &gn );
            if( (cc != pos) || (gn.result == 0) ) {
                if( input_cbs->fmflags & II_macro ) {
                    out_msg( "ERR_FUNCTION parm 3 (startpos) invalid\n"
                             "\t\t\tLine %d of macro '%s'\n",
                             input_cbs->s.m->lineno,
                             input_cbs->s.m->mac->name );
                } else {
                    out_msg( "ERR_FUNCTION parm 3 (startpos) invalid\n"
                             "\t\t\tLine %d of file '%s'\n",
                             input_cbs->s.f->lineno,
                             input_cbs->s.f->filename );
                }
                err_count++;
                show_include_stack();
                return( cc );
            }
            n = gn.result - 1;
        }
    }

    if( (hay_len <= 0) ||               // null string nothing to do
        (needle_len <= 0) ||            // needle null nothing to do
        (needle_len > hay_len) ||       // needle longer haystack
        (n + needle_len > hay_len) ) {  // startpos + needlelen > haystack
                                        // ... match impossible

        **result = '0';                 // return index zero
        *result += 1;
        **result = '\0';
        return( pos );
    }

    ph = phay + n;                      // startpos in haystack
    pn = pneedle;

    for( ph = phay + n; ph <= phayend - needle_len - 1; ph++ ) {
        pn = pneedle;
        while( (*ph == *pn) && (pn <= pneedlend)) {
            ph++;
            pn++;
        }
        if( pn > pneedlend ) {
            index = ph - phay;          // found set index
            break;
        }
    }

    *result += sprintf( *result, "%d", index );

    return( pos );
}

/*
 * scr_pos : swap parm1 and parm2, then call scr_index
 *
 */

condcode    scr_pos( parm parms[ MAX_FUN_PARMS ], size_t parmcount, char * * result )
{
    char            *   pwk;

    if( parmcount < 2 ) {
        return( neg );
    }

    pwk = parms[ 0 ].a;
    parms[ 0 ].a = parms[ 1 ].a;
    parms[ 1 ].a = pwk;

    pwk = parms[ 0 ].e;
    parms[ 0 ].e = parms[ 1 ].e;
    parms[ 1 ].e = pwk;

    return( scr_index( parms, parmcount, result ) );
}

