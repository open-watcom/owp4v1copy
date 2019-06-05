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
* Description:  implement .se and .sr script control words
*
****************************************************************************/


#include "wgml.h"


/* construct symbol name and optionally subscript from input
 *
 *
 */

char    *scan_sym( char * p, symvar * sym, sub_index * subscript )
{
    size_t      k;
    char    *   sym_start;
    char        quote;
    char        linestr[MAX_L_AS_STR];

    scan_err = false;
    sym->next = NULL;
    sym->flags = 0;
    *subscript = no_subscript;          // not subscripted

    SkipSpaces( p );                    // skip over spaces
    if( *p == d_q || *p == s_q || *p == l_q ) {
        quote = *p++;
    } else {
        quote = '\0';
    }
    if( *p == '*' ) {                   // local var
        p++;
        sym->flags = local_var;
    }
    sym_start = p;
    sym->name[0] = '\0';

    k = 0;
    while( is_symbol_char( *p ) ) {

        if( k < SYM_NAME_LENGTH ) {
            if( (k == 3) && (sym->name[0] != '$') ) {
                if( sym->name[0] == 's' &&
                    sym->name[1] == 'y' &&
                    sym->name[2] == 's' ) {

                    sym->name[0] = '$';   // create sys shortcut $
                    k = 1;
                }
            }
            sym->name[k++] = my_tolower( *p );
            sym->name[k] = '\0';
        } else {
            if( !scan_err ) {
                scan_err = true;
                if( !ProcFlags.suppress_msg ) {
                    // SC--074 For the symbol '%s'
                    //     The length of a symbol cannot exceed ten characters

                    g_err( err_sym_long, sym_start );
                    g_info( inf_sym_10 );
                    if( input_cbs->fmflags & II_tag_mac ) {
                        ulongtodec( input_cbs->s.m->lineno, linestr );
                        g_info( inf_mac_line, linestr, input_cbs->s.m->mac->name );
                    } else {
                        ulongtodec( input_cbs->s.f->lineno, linestr );
                        g_info( inf_file_line, linestr, input_cbs->s.f->filename );
                    }
                    show_include_stack();
                    err_count++;
                }
            }
        }
        p++;
    }

    if( p == sym_start ) {              // special for &*
        if( *p != '&' ) {               // not &*&xx construct

            if( (sym->flags & local_var)
                && (input_cbs->fmflags & II_tag_mac) ) {

                strcpy_s( sym->name, SYM_NAME_LENGTH, MAC_STAR_NAME );
            } else {
                scan_err = true;
            }
        }
    }
    if( quote != '\0' && quote == *p ) {        // over terminating quote
        p++;
    }
    if( !scan_err && (*p == '(') ) {    // subscripted ?
        char    *   psave = p;
        sub_index   var_ind;
        symsub  *   symsubval;
        int         rc;

        p++;
        if( *p == ')' ) {               // () is auto increment
            p++;
            var_ind = 0;
            if( sym->flags & local_var )  {
                rc = find_symvar( &input_cbs->local_dict, sym->name,
                              var_ind, &symsubval );
            } else {
                rc = find_symvar( &global_dict, sym->name, var_ind,
                                  &symsubval );
            }
            if( rc > 0 ) {              // variable exists use last_auto_inc
                *subscript = symsubval->base->last_auto_inc + 1;
            } else {
                *subscript = 1;         // start with index 1
            }
            sym->flags |= auto_inc + subscripted;
        } else {
            getnum_block    gn;
            condcode        cc;
            char            csave;

            gn.argstart      = p;
            while( *p != '\0' && (*p != ')') ) {
                p++;
            }
            gn.argstop       = p - 1;
            csave            = *p;
            *p               = '\0';    // make nul terminated string
            gn.ignore_blanks = 0;

            cc = getnum( &gn );     // try numeric expression evaluation

            *p = csave;
            if( cc == pos || cc == neg ) {
                *subscript = gn.result;
                if( *p == ')' ) {
                    p++;
                }
                SkipDot( p );
            } else {
                if( !scan_err && !ProcFlags.suppress_msg ) {
                    g_err( err_sub_invalid, psave );
                    err_count++;
                    show_include_stack();
                }
                scan_err = true;
            }

            if( scan_err ) {
               p = psave;
            }
        }
    }
    return( p );
}

/***************************************************************************/
/*  processing  SET                                                        */
/*                                                                         */
/*                          = <character string>                           */
/*         .SE       symbol = <numeric expression>                         */
/*                          <OFF>                                          */
/*  symbol may be subscripted (3) or () for auto increment 1 - n           */
/*                                                                         */
/*  This summarizes SR, since SE extends SR and SR is not implemented      */
/*         .se x1     off                                                  */
/*         .se x2a    'string (Note 1)                                     */
/*         .se x2b    'string (Note 1)                                     */
/*         .se x3a =  string                                               */
/*         .se x3b =  'string (Note 2)                                     */
/*         .se x4c =  'string'(Note 2)                                     */
/*         .se n1  =  1234                                                 */
/*         .se n2a =  (1+(2+5)/6)                                          */
/*         .se n2b =  -1+(2+5)/6)                                          */
/*  Note 1: these apply only to ', the first of which must be present      */
/*  Note 2: these apply to all delimiters: ', ", /, |, !, ^, 0x9b and,     */
/*          apparently, 0xdd and 0x60                                      */
/*          the final delimiter must be followed by a space or '\0'        */
/*          this means that 'abc'def' is a seven-character delimited value */
/*          provided it is at the end of the line or followed by a space   */
/*                                                                         */
/*  Except for local symbol *, spaces are removed from the end of the line */
/*  before further processing (so that spaces inside delimiters are not    */
/*  afftected, but any following the closing delimiter are)                */
/*                                                                         */
/***************************************************************************/

void    scr_se( void )
{
    char        *   p;
    char        *   valstart;
    int             rc;
    sub_index       subscript;
    symsub      *   symsubval;
    symvar          sym;
    symvar      * * working_dict;
    size_t          len;

    subscript = no_subscript;                       // not subscripted
    scan_err = false;
    p = scan_sym( scan_start, &sym, &subscript );

    if( strcmp( sym.name, MAC_STAR_NAME ) != 0 ) {  // remove trailing blanks from all symbols except *
        valstart = p;
        for( len = strlen( p ); len-- > 0; ) {
            if( p[len] != ' ' )
                break;
            p[len] = '\0';
        }
        p = valstart;
    }

    if( sym.flags & local_var ) {
        working_dict = &input_cbs->local_dict;
    } else {
        working_dict = &global_dict;
    }

    if( ProcFlags.blanks_allowed ) {
        SkipSpaces( p );                        // skip over spaces
    }
    if( *p == '\0' ) {
        if( !ProcFlags.suppress_msg ) {
            xx_line_err ( err_eq_expected, p);
        }
        scan_err = true;
    }
    if( !scan_err ) {
        valstart = p;
        if( *p == '=' ) {                       // all other cases have no equal sign (see above)
            p++;
            if( ProcFlags.blanks_allowed ) {
                SkipSpaces( p );                // skip over spaces to value
            }
            valstart = p;
            if( is_quote_char( *valstart ) ) {      // quotes ?
                p++;
                while( *p != '\0' ) {   // look for quote end (must match and be at eol or followed by a space)
                    if( (*valstart == *p) && (!*(p+1) || (*(p+1) == ' ')) ) {
                        break;
                    }
                    ++p;
                }
                if( (valstart < p) && (*p == *valstart) ) { // delete quotes if more than one character
                    valstart++;
                    *p = '\0';
                }
            } else {                                // numeric or undelimited string
                getnum_block    gn;
                condcode        cc;

                gn.argstart      = valstart;
                gn.argstop       = scan_stop;
                gn.ignore_blanks = 1;

                cc = getnum( &gn );             // try numeric expression evaluation
                if( cc != notnum ) {
                    valstart = gn.resultstr;
                }                               // if notnum treat as character value
            }

            rc = add_symvar( working_dict, sym.name, valstart, subscript, sym.flags );

        } else if( *p == '\'' ) {                       // \' without equal sign
            p++;
            while( *p != '\0' && (*valstart != *p) ) {  // look for final \'
                p++;
            }
            valstart++;                                 // delete initial \'
            if( (valstart < p) && (*p == '\'') ) {      // delete \' at end
                *p = '\0';
            }
            rc = add_symvar( working_dict, sym.name, valstart, subscript, sym.flags );
        } else if( !strncmp( p, "off", 3 ) ) {       // OFF
            p += 3;
            rc = find_symvar( working_dict, sym.name, subscript, &symsubval );
            if( rc == 2 ) {
                symsubval->base->flags |= deleted;
            }
        } else {
            if( !ProcFlags.suppress_msg ) {
                xx_warn_cc( wng_miss_inv_value, sym.name, p );
            }
            scan_err = true;
        }
    }
    scan_restart = scan_stop;
    return;
}

