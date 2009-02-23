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
* Description:  WGML controls multi letter functions
*               and calls corresponding function see gsfuncs.h
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*    SCR multi letter functions                                           */
/***************************************************************************/

#define pick( name, length, parms, optparms, routine ) \
            { name, length, parms, optparms, routine },

const   scrfunc scr_functions[] = {

#include "gsfuncs.h"

    { " ", 0, 0, 0, NULL }              // end
};

#define SCR_FUNC_MAX (sizeof( scr_functions ) / sizeof( scr_functions[ 0 ] ) - 1)


/***************************************************************************/
/*  scr_multi_funcs  isolate function name, lookup name in table           */
/*                   scan function arguments                               */
/*                   call corresponding function                           */
/***************************************************************************/

char    *scr_multi_funcs( char * in, char * end, char * * result )
{
    static const char   ampchar = '&';
    char            *   pchar;
    int                 rc;
    char            *   pval;
    char            *   pfunc;
    size_t              fnlen;
    int                 funcind;
    int                 k;
    int                 m;
    int                 paren_level;
    int                 paren_level_start;
    char                fn[ FUN_NAME_LENGTH + 1 ];
    bool                found;
    parm                parms[ MAX_FUN_PARMS ];
    int                 parmcount;
    condcode            cc;
    bool                multiletter_function;


    end   = end;
    rc = 0;
    pval = end;
    pfunc = end;

    fnlen = 0;
    pchar = in + 2;                     // over &'

    // collect function name
    while( *pchar && pchar <= end && test_function_char( *pchar ) ) {
        fn[ fnlen ] = *pchar++;
        if( fnlen <= FUN_NAME_LENGTH ) {
            fnlen++;
        } else {
            break;
        }
    }
    fn[ fnlen ] = '\0';

    if( *pchar != '(' ) {         // open paren does not follow function name
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION parms missing no ( found\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION parms missing no ( found\n"
                     "\t\t\tLine %d of file '%s'\n",
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }

    // test for valid functionname
    found = false;
    for( k = 0; k < SCR_FUNC_MAX; k++ ) {
        if( fnlen == scr_functions[ k ].length
            && !stricmp( fn, scr_functions[ k ].fname ) ) {

            found = true;
            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                out_msg( " Function %s found\n", scr_functions[ k ].fname );
            }
            break;
        }
    }
    if( !found ) {
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION unknown function '%s'\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     fn,
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION unknown function '%s'\n"
                     "\t\t\tLine %d of file '%s'\n",
                     fn,
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }
    funcind = k;
    multiletter_function = false;

    // collect the mandatory parm(s)
    paren_level_start = 0;

    for( k = 0; k < scr_functions[ funcind ].parm_cnt; k++ ) {
        parms[ k ].a = ++pchar;         // first over ( then over ,
        paren_level = paren_level_start;

        while( (paren_level > paren_level_start) ||
                ((*pchar != ',') && (*pchar != ')')) ) {
            if( pchar > end ) {
                break;
            }
            paren_level += (*pchar == '(');
            paren_level -= (*pchar == ')');
            if( (*pchar == ampchar) && (*(pchar + 1) == '\'') ) {
                multiletter_function = true;
            }
            pchar++;
        }
        parms[ k ].e = pchar - 1;
        parms[ k + 1 ].a = pchar + 1;

        if( *pchar == ')' ) {
            break;                      // end of parms
        }
    }
    m = k + (k < scr_functions[ funcind ].parm_cnt);// mandatory parm count

    if( m < scr_functions[ funcind ].parm_cnt ) {
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION %d parms missing\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     scr_functions[ funcind ].parm_cnt - m,
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION %d parms missing\n"
                     "\t\t\tLine %d of file '%s'\n",
                     scr_functions[ funcind ].parm_cnt - m,
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }



    // collect the optional parm(s)
    if( *pchar == ')' ) {               // no optional parms
        k = 0;
    } else {
        for( k = 0; k < scr_functions[ funcind ].opt_parm_cnt; k++ ) {
            parms[ m + k ].a = ++pchar;
            paren_level = paren_level_start;

            while( (paren_level > paren_level_start) ||
                    ((*pchar != ',') && (*pchar != ')')) ) {
                if( pchar > end ) {
                    break;
                }
                paren_level += (*pchar == '(');
                paren_level -= (*pchar == ')');
                if( (*pchar == ampchar) && (*(pchar + 1) == '\'') ) {
                    multiletter_function = true;
                }
                pchar++;
            }
            parms[ m + k ].e     = pchar - 1;
            parms[ m + k + 1 ].a = pchar + 1;
            if( *pchar == ')' ) {
                break;                  // end of parms
            }
        }
        k +=  (k < scr_functions[ funcind ].opt_parm_cnt);
    }
    parmcount = m + k;                  // total parmcount
    parms[ parmcount ].a = NULL;        // end of parms indicator

    if( *pchar != ')' ) {
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION missing closing paren\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION missing closing paren\n"
                     "\t\t\tLine %d of file '%s'\n",
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }

    ProcFlags.suppress_msg = multiletter_function;

    cc = scr_functions[ funcind ].fun( parms, parmcount, result );

    ProcFlags.suppress_msg = false;
    if( cc != pos ) {                   // error in function
        **result = '&';                 // result is & to preserve the input

        ProcFlags.unresolved = true;
        *result += 1;
        **result = '\0';
        return( in + 1 );
    }

    ProcFlags.substituted = true;
    return( pchar + 1 );                // all OK new scan position
}


#if 0

+++++++++++++++++++++++++++++++  used functions in OW documentation +++++++++
used single letter functions: others are unused
&e'  exist 0 or 1
&l'  length, if undefined length of name
&s'  subscript   low
&S'  superscript high
&u'  upper

used other functions:

&'delstr(
&'d2c(
&'index(
&'insert(
&'left(
&'length(
&'lower(
&'min(
&'pos(
&'right(
&'strip(
&'substr(
&'subword(
&'translate(
&'upper(
&'veclastpos(
&'vecpos(
&'word(
&'wordpos(
&'words(

#endif
