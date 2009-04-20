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
* Description:  WGML input line processing
*   some logic / ideas adopted from Watcom Script 3.2 IBM S/360 Assembler
*   see comment in wgml.c
****************************************************************************/
 
#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */
 
#include "wgml.h"
#include "gvars.h"
 
static  char    *   var_unresolved2;
 
/*  split_input
 *  The (physical) line is split
 *  The second part will be processed by the next getline()
 *
 */
 
void        split_input( char * buf, char * split_pos )
{
    inp_line    *   wk;
    size_t          len;
 
    len = strlen( split_pos );          // length of second part
    if( len > 0 ) {
        wk = mem_alloc( len + sizeof( inp_line ) );
        wk->next = NULL;
        strcpy(wk->value, split_pos );  // save second part
 
        if( input_cbs->hidden_tail != NULL ) {
            input_cbs->hidden_tail->next = wk;
        }
        input_cbs->hidden_tail = wk;
 
        if( input_cbs->hidden_head == NULL ) {
            input_cbs->hidden_head = wk;
        }
        *split_pos = '\0';              // terminate first part
    }
    return;
}
 
 
/*  split_input LIFO
 *  The (physical) line is split
 *  The second part will be processed by the next getline()
 *   pushing any already split part down
 */
 
void        split_input_LIFO( char * buf, char * split_pos )
{
    inp_line    *   wk;
    size_t          len;
 
    len = strlen( split_pos );          // length of second part
    if( len > 0 ) {
        wk = mem_alloc( len + sizeof( inp_line ) );
        wk->next = input_cbs->hidden_head;
        strcpy(wk->value, split_pos );  // save second part
 
        input_cbs->hidden_head = wk;
        if( input_cbs->hidden_tail == NULL ) {
            input_cbs->hidden_tail = wk;
        }
 
        *split_pos = '\0';              // terminate first part
    }
    return;
}
 
/*  split_input_var
 *  The second part is constructed from 2 parts
 *  used if a substituted variable starts with CW_sep_char
 */
 
static  void    split_input_var( char * buf, char * split_pos, char * part2 )
{
    inp_line    *   wk;
    size_t          len;
 
    len = strlen( split_pos ) + strlen( part2 );// length of second part
    if( len > 0 ) {
        wk = mem_alloc( len + sizeof( inp_line ) );
        wk->next = NULL;
 
        strcpy(wk->value, part2 );      // second part
        strcat(wk->value, split_pos );  // second part
 
        if( input_cbs->hidden_tail != NULL ) {
            input_cbs->hidden_tail->next = wk;
        }
        input_cbs->hidden_tail = wk;
 
        if( input_cbs->hidden_head == NULL ) {
            input_cbs->hidden_head = wk;
        }
    }
    return;
}
 
 
/***************************************************************************/
/*  take the contents of the input line in buff2 and try to make the best  */
/*  of it                                                                  */
/*  Processing as specified in wgmlref.pdf chapter 8.1 processing rules    */
/*   incomplete                                                            */
/*                                                                         */
/***************************************************************************/
 
void        process_line( void )
{
    static const char   ampchar = '&';
    char            *   workbuf;
    char            *   pw;
    char            *   pwend;
    char            *   p2;
    char            *   pchar;
    char            *   varstart;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;
    int                 k;
    bool                resolve_functions;
    bool                functions_found;
    bool                anything_substituted;
 
    ProcFlags.late_subst = false;
    /***********************************************************************/
    /*  look for GML tag start character and split line if valid GML tag   */
    /***********************************************************************/
 
    pchar = strchr( buff2, GML_char );  // look for GML tag start
    if( (pchar != NULL) && (buff2 < pchar) ) {
        for( p2 = pchar; (p2 < (buff2 + buf_size)) && is_id_char( *p2 ); p2++ )
            ;                           // empty
 
        if( (*p2 == '.') || (*p2 == ' ') ) {// 'good' tag end
            split_input( buff2, pchar );// split line
            buff2_lg = strnlen_s( buff2, buf_size );// new length of first part
        }
 
    }
 
 
    /***********************************************************************/
    /* for :cmt. minimal processing                                        */
    /***********************************************************************/
 
    if( (*buff2 == GML_char) && !strnicmp( buff2 + 1, "cmt.", 4 ) ) {
        return;
    }
 
    /***********************************************************************/
    /*  .xx SCRIPT control line                                            */
    /***********************************************************************/
 
    if( (*buff2 == SCR_char) ) {
 
        if( *(buff2 + 1) == '*' ) {
            return;                     // for .* comment minimal processing
        }
 
        /*******************************************************************/
        /* if macro define ( .dm xxx ... ) supress variable substitution   */
        /* for the sake of single line macro definition                    */
        /* .dm xxx / &*1 / &*2 / &*0 / &* /                                */
        /*  and                                                            */
        /* ..dm xxx / &*1 / &*2 / &*0 / &* /                               */
        /*******************************************************************/
        if( *(buff2 + 1) == SCR_char ) {
            k = 2;
        } else {
            k = 1;
        }
        if( !strnicmp( buff2 + k, "dm ", 3 ) ) {
            return;
        }
 
        /*******************************************************************/
        /*  for lines starting  .' ignore control word seperator           */
        /*******************************************************************/
 
        /*******************************************************************/
        /*  if 2 CW_sep_chars follow, don't split line                     */
        /*  ... this is NOWHERE documented, but wgml 4.0 tested            */
        /*******************************************************************/
 
 
        if( !(*(buff2 + 1) == '\'') ) {
            pchar = strchr( buff2 + 2, CW_sep_char );
            if( (pchar != NULL) ) {
                if( *(pchar + 1) != CW_sep_char ) {
                    split_input( buff2, pchar + 1 );// split after CW_sep_char
 
                    buff2_lg = strnlen_s( buff2, buf_size );// new length of first part
                    buff2_lg--;         // ignore CW_sep_char
                    *(buff2 + buff2_lg) = '\0';
#if 0
                } else {                // ignore 1 CW_sep_char
                    memmove_s( pchar, pchar - buff2 + buff2_lg + 1,
                               pchar + 1, pchar - buff2 + buff2_lg );
                    buff2_lg = strnlen_s( buff2, buf_size );
#endif
                }
 
            }
        }
    }
 
    workbuf = mem_alloc( buf_size );
 
/*
 * look for symbolic variables and single letter functions,
 *  ignore multi letter functions for now
 */
    ProcFlags.substituted = false;
    anything_substituted = false;
    ProcFlags.late_subst = false;
    var_unresolved = NULL;
    ProcFlags.unresolved  = false;
    functions_found = false;
    do {                                // until no more substitutions
        strcpy_s( workbuf, buf_size, buff2 );   // copy input buffer
        buff2_lg = strnlen_s( buff2, buf_size );
        pwend = workbuf + buff2_lg;
        if( var_unresolved == NULL ) {
            pw = workbuf;
            p2 = buff2;
        } else {
            pw = workbuf + (var_unresolved2 - buff2);
            p2 = var_unresolved2;
        }
        varstart = NULL;
 
        anything_substituted |= ProcFlags.substituted;
        ProcFlags.substituted = false;
 
        pchar = strchr( workbuf, ampchar ); // look for & in buffer
        while( pchar != NULL ) {        // & found
            while( pw < pchar ) {       // copy all data preceding &
                *p2++ = *pw++;
            }
            buff2_lg = strnlen_s( buff2, buf_size );
 
            /***********************************************************/
            /*  Some single letter functions are resolved here:        */
            /*                                                         */
            /*  functions used within the OW doc build system:         */
            /*   &e'  existance of variable 0 or 1                     */
            /*   &l'  length of variable content                       */
            /*        or if undefined length of name                   */
            /*   &u'  upper                                            */
            /*                                                         */
            /*   &s'  subscript    These are recognized,   TBD         */
            /*   &S'  superscript  ... but processed as &u'            */
            /*                                                         */
            /*   other single letter functions are not used AFAIK      */
            /*                                                         */
            /***********************************************************/
            if( isalpha( *(pchar + 1) ) && *(pchar + 2) == '\''
                && *(pchar + 3) > ' ' ) {
                // not for .if '&*' eq '' .th ...
                // only    .if '&x'foo' eq '' .th
 
                char * * ppval = &p2;
 
                pw = scr_single_funcs( pchar, pwend, ppval );
                pchar = strchr( pw, ampchar );// look for next & in buffer
                continue;
            }
 
            if( *(pchar + 1) == '\'' ) {// multi letter function
                *p2++ = *pw++;          // over & and copy
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                functions_found = true; // remember there is a function
                continue;               // and ignore function
            }
 
            varstart = pw;              // remember start of var
            pw++;                       // over &
            ProcFlags.suppress_msg = true;
            scan_err = false;
 
            pchar = scan_sym( pw, &symvar_entry, &var_ind );
            if( scan_err && *pchar == '(' ) {   // problem with subscript
 
                if( var_unresolved == NULL ) {
                    ProcFlags.unresolved  = true;
                    var_unresolved = varstart;
                    var_unresolved2 = p2;
                } else {
                    if( var_unresolved != varstart ) {
                        ProcFlags.unresolved  = true;
                    }
                }
                p2 += pchar - varstart;
                pw = pchar;
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                continue;
            }
 
            ProcFlags.suppress_msg = false;
 
            if( symvar_entry.flags & local_var ) {  // lookup var in dict
                rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                                  var_ind, &symsubval );
            } else {
                rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                                  &symsubval );
                if( rc == 2 && (symsubval->base->flags & late_subst) ) {
                    ProcFlags.late_subst = true;// remember special for : &
                    rc = 0;
                }
            }
            if( rc == 2 ) {             // variable found
                ProcFlags.substituted = true;
                if( symsubval->value[0] == CW_sep_char &&
                    symsubval->value[1] != CW_sep_char ) {
 
                                        // split record at control word separator
                                        // if variable starts with SINGLE cw separator
 
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    *p2 = '\0';
                    split_input_var( buff2, pchar, &symsubval->value[1] );
                    pw = pwend + 1;     // stop substitution for this record
                    varstart = NULL;
                    break;
                } else {
                    pw = symsubval->value;
                    if( symsubval->value[0] == CW_sep_char &&
                        symsubval->value[1] == CW_sep_char ) {
                        pw++;           // skip 1 CW_sep_char
                    }
                    strcpy( p2, pw );   // copy value
                    p2 += strlen( pw );
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    pw = pchar;
                }
            } else {
                if( (symvar_entry.flags & local_var )  // local var not found
                    && (input_cbs->fmflags & II_macro) ) {// .. and inside macro
                    if( (symvar_entry.name[0] == '\0') &&
                        (*pchar == ampchar) ) { // only &* as var name
                                                // followed by another var
 
                        if( var_unresolved == NULL ) {
                            ProcFlags.unresolved  = true;
                            var_unresolved = varstart;
                            var_unresolved2 = p2;
                        } else {
                            if( var_unresolved != varstart ) {
                                ProcFlags.unresolved  = true;
                            }
                        }
                        pw = varstart;
                        while( pw < pchar ) {   // treat var name as text
                            *p2++ = *pw++;  // and copy
                        }
                        continue;       // pchar points already to next &
 
                    } else {
                        ProcFlags.substituted = true;
                                        // replace by nullstring
                        if( *pchar == '.' ) {
                            pchar++;    // skip optional terminating dot
                        }
                        pw = pchar;
                    }
                } else {                // global var not found
                                        // .. or local var outside of macro
                    /*******************************************************/
                    /*  keep trying for constructs such as                 */
                    /*                                                     */
                    /* .se prodgml = "Open Watcom GML"                     */
                    /* .se name = "GML"                                    */
                    /*                                                     */
                    /* My name is &prod&name..!                            */
                    /*                                                     */
                    /*  to become                                          */
                    /*                                                     */
                    /* My name is Open Watcom GML!                         */
                    /*                                                     */
                    /* This does not work for local variables, as these are*/
                    /* replaced by nullstring if not found                 */
                    /* My name is &*prod&*name..!                          */
                    /*  will become                                        */
                    /* My name is !                                        */
                    /*******************************************************/
 
                    if( var_unresolved == NULL ) {
                        ProcFlags.unresolved  = true;
                        var_unresolved = varstart;
                        var_unresolved2 = p2;
                    } else {
                        if( var_unresolved != varstart ) {
                            ProcFlags.unresolved  = true;
                        }
                    }
 
                    pw = varstart;
                    if( *pchar == '.' ) {
                        pchar++;        // copy terminating dot, too
                    }
                    while( pw < pchar ) {   // treat var name as text
                        *p2++ = *pw++;  // and copy
                    }
                }
            }
            pchar = strchr( pw, ampchar );  // look for next & in buffer
        }                               // while & found
 
        while( pw <= pwend) {           // copy remaining input
             *p2++ = *pw++;
        }
 
    } while( ProcFlags.unresolved && ProcFlags.substituted );
 
    anything_substituted |= ProcFlags.substituted;
 
/* handle multi letter functions, ignore any unresolved variable
 */
 
    if( functions_found ) {
        resolve_functions = false;
        functions_found   = false;
        var_unresolved = NULL;
        ProcFlags.unresolved  = false;
        do {
            strcpy_s( workbuf, buf_size, buff2 );   // copy input buffer
            buff2_lg = strnlen_s( buff2, buf_size );
            pwend = workbuf + buff2_lg;
            if( var_unresolved == NULL ) {
                pw = workbuf;
                p2 = buff2;
            } else {
                pw = workbuf + (var_unresolved2 - buff2);
                p2 = var_unresolved2;
            }
            varstart = NULL;
 
            ProcFlags.substituted = false;
 
            pchar = strchr( workbuf, ampchar ); // look for & in buffer
            while( pchar != NULL ) {    // & found
                while( pw < pchar ) {   // copy all data preceding &
                    *p2++ = *pw++;
                }
                buff2_lg = strnlen_s( buff2, buf_size );
 
                /***********************************************************/
                /*  Some multi  letter functions are resolved here:        */
                /*                                                         */
                /*  functions used within the OW doc build system:         */
                /*                                                         */
                /*   &'delstr(                                             */
                /*   &'d2c(                                                */
                /*   &'index(                                              */
                /*   &'insert(                                             */
                /*   &'left(                                               */
                /*   &'length(                                             */
                /*   &'lower(                                              */
                /*   &'min(                                                */
                /*   &'pos(                                                */
                /*   &'right(                                              */
                /*   &'strip(                                              */
                /*   &'substr(                                             */
                /*   &'subword(                                            */
                /*   &'translate(                                          */
                /*   &'upper(                                              */
                /*   &'veclastpos(                                         */
                /*   &'vecpos(                                             */
                /*   &'word(                                               */
                /*   &'wordpos(                                            */
                /*   &'words(                                              */
                /*                                                         */
                /*   Others are recognized but not processed               */
                /*                                                         */
                /***********************************************************/
                if( *(pchar + 1) == '\'' ) {
                    char * * ppval = &p2;
 
                    pw = scr_multi_funcs( pchar, pwend, ppval );
                    pchar = strchr( pw, ampchar );// look for next & in buffer
                    continue;
                } else {
                    *p2++ = *pw++;      // over &
                    pchar = strchr( pw, ampchar );// look for next & in buffer
                }
            }                           // while & found
 
            while( pw <= pwend) {       // copy remaining input
                 *p2++ = *pw++;
            }
 
            anything_substituted |= ProcFlags.substituted;
        } while( ProcFlags.unresolved && ProcFlags.substituted );
 
    }                                   // if functions_found
    anything_substituted |= ProcFlags.substituted;
 
    buff2_lg = strnlen_s( buff2, buf_size );
 
    if( GlobalFlags.research && GlobalFlags.firstpass && anything_substituted ) {
        g_info( inf_subst_line, buff2 );// show line with substitution(s)
    }
    mem_free( workbuf );
 
    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;
    return;
}
 
/***************************************************************************/
/*  process late substitute symbols &gml, &amp                             */
/*  this is done after gml tag and script control word recognition         */
/***************************************************************************/
 
void        process_late_subst( void )
{
    static const char   ampchar = '&';
    char            *   workbuf;
    char            *   pw;
    char            *   pwend;
    char            *   p2;
    char            *   pchar;
    char            *   varstart;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;
    int                 k;
    bool                anything_substituted;
 
    ProcFlags.late_subst = false;
 
    buff2_lg = strnlen_s( buff2, buf_size );
 
 
    /***********************************************************************/
    /* for :cmt. minimal processing                                        */
    /***********************************************************************/
 
    if( (*buff2 == GML_char) && !strnicmp( buff2 + 1, "cmt.", 4 ) ) {
        return;
    }
 
    /***********************************************************************/
    /*  .xx SCRIPT control line                                            */
    /***********************************************************************/
 
    if( (*buff2 == SCR_char) ) {
 
        if( *(buff2 + 1) == '*' ) {
            return;                     // for .* comment minimal processing
        }
 
        /*******************************************************************/
        /* if macro define ( .dm xxx ... ) supress variable substitution   */
        /* for the sake of single line macro definition                    */
        /* .dm xxx / &*1 / &*2 / &*0 / &* /                                */
        /*  and                                                            */
        /* ..dm xxx / &*1 / &*2 / &*0 / &* /                               */
        /*******************************************************************/
        if( *(buff2 + 1) == SCR_char ) {
            k = 2;
        } else {
            k = 1;
        }
        if( !strnicmp( buff2 + k, "dm ", 3 ) ) {
            return;
        }
    }
 
    workbuf = mem_alloc( buf_size );
 
    /***********************************************************************/
    /*  Look for late-subst variables, ignore all others                   */
    /*                                                                     */
    /***********************************************************************/
 
 
    ProcFlags.substituted = false;
    anything_substituted = false;
    var_unresolved = NULL;
    ProcFlags.unresolved  = false;
    do {                                // until no more substitutions
        strcpy_s( workbuf, buf_size, buff2 );   // copy input buffer
        buff2_lg = strnlen_s( buff2, buf_size );
        pwend = workbuf + buff2_lg;
        if( var_unresolved == NULL ) {
            pw = workbuf;
            p2 = buff2;
        } else {
            pw = workbuf + (var_unresolved2 - buff2);
            p2 = var_unresolved2;
        }
        varstart = NULL;
 
        anything_substituted |= ProcFlags.substituted;
        ProcFlags.substituted = false;
 
        pchar = strchr( workbuf, ampchar ); // look for & in buffer
        while( pchar != NULL ) {        // & found
            while( pw < pchar ) {       // copy all data preceding &
                *p2++ = *pw++;
            }
            buff2_lg = strnlen_s( buff2, buf_size );
#if 0
            /***********************************************************/
            /*  Some single letter functions are resolved here:        */
            /*                                                         */
            /*  functions used within the OW doc build system:         */
            /*   &e'  existance of variable 0 or 1                     */
            /*   &l'  length of variable content                       */
            /*        or if undefined length of name                   */
            /*   &u'  upper                                            */
            /*                                                         */
            /*   &s'  subscript    These are recognized,   TBD         */
            /*   &S'  superscript  ... but processed as &u'            */
            /*                                                         */
            /*   other single letter functions are not used AFAIK      */
            /*                                                         */
            /***********************************************************/
            if( isalpha( *(pchar + 1) ) && *(pchar + 2) == '\''
                && *(pchar + 3) > ' ' ) {
                // not for .if '&*' eq '' .th ...
                // only    .if '&x'foo' eq '' .th
 
                char * * ppval = &p2;
 
                pw = scr_single_funcs( pchar, pwend, ppval );
                pchar = strchr( pw, ampchar );// look for next & in buffer
                continue;
            }
 
            if( *(pchar + 1) == '\'' ) {// multi letter function
                *p2++ = *pw++;          // over & and copy
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                functions_found = true; // remember there is a function
                continue;               // and ignore function
            }
#endif
            varstart = pw;              // remember start of var
            pw++;                       // over &
            ProcFlags.suppress_msg = true;
            scan_err = false;
 
            pchar = scan_sym( pw, &symvar_entry, &var_ind );
            if( scan_err && *pchar == '(' ) {   // problem with subscript
 
                if( var_unresolved == NULL ) {
                    ProcFlags.unresolved  = true;
                    var_unresolved = varstart;
                    var_unresolved2 = p2;
                } else {
                    if( var_unresolved != varstart ) {
                        ProcFlags.unresolved  = true;
                    }
                }
                p2 += pchar - varstart;
                pw = pchar;
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                continue;
            }
 
            ProcFlags.suppress_msg = false;
 
            if( symvar_entry.flags & local_var ) {  // lookup var in dict
                rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                                  var_ind, &symsubval );
            } else {
                rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                                  &symsubval );
            }
            if( rc == 2 ) {             // variable found
                ProcFlags.substituted = true;
                if( symsubval->value[0] == CW_sep_char &&
                    symsubval->value[1] != CW_sep_char ) {
 
                                        // split record at control word separator
                                        // if variable starts with SINGLE cw separator
 
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    *p2 = '\0';
                    split_input_var( buff2, pchar, &symsubval->value[1] );
                    pw = pwend + 1;     // stop substitution for this record
                    varstart = NULL;
                    break;
                } else {
                    pw = symsubval->value;
                    if( symsubval->value[0] == CW_sep_char &&
                        symsubval->value[1] == CW_sep_char ) {
                        pw++;           // skip 1 CW_sep_char
                    }
                    strcpy( p2, pw );   // copy value
                    p2 += strlen( pw );
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    pw = pchar;
                }
            } else {
                if( symvar_entry.flags & local_var ) { // local var not found
                    if( (symvar_entry.name[0] == '\0') &&
                        (*pchar == ampchar) ) { // only &* as var name
                                                // followed by another var
 
                        if( var_unresolved == NULL ) {
                            ProcFlags.unresolved  = true;
                            var_unresolved = varstart;
                            var_unresolved2 = p2;
                        } else {
                            if( var_unresolved != varstart ) {
                                ProcFlags.unresolved  = true;
                            }
                        }
                        pw = varstart;
                        while( pw < pchar ) {   // treat var name as text
                            *p2++ = *pw++;  // and copy
                        }
                        continue;       // pchar points already to next &
 
                    } else {
                        ProcFlags.substituted = true;
                                        // replace by nullstring
                        if( *pchar == '.' ) {
                            pchar++;    // skip optional terminating dot
                        }
                        pw = pchar;
                    }
                } else {                // global var not found
 
                    /*******************************************************/
                    /*  keep trying for constructs such as                 */
                    /*                                                     */
                    /* .se prodgml = "Open Watcom GML"                     */
                    /* .se name = "GML"                                    */
                    /*                                                     */
                    /* My name is &prod&name..!                            */
                    /*                                                     */
                    /*  to become                                          */
                    /*                                                     */
                    /* My name is Open Watcom GML!                         */
                    /*                                                     */
                    /* This does not work for local variables, as these are*/
                    /* replaced by nullstring if not found                 */
                    /* My name is &*prod&*name..!                          */
                    /*  will become                                        */
                    /* My name is !                                        */
                    /*******************************************************/
 
                    if( var_unresolved == NULL ) {
                        ProcFlags.unresolved  = true;
                        var_unresolved = varstart;
                        var_unresolved2 = p2;
                    } else {
                        if( var_unresolved != varstart ) {
                            ProcFlags.unresolved  = true;
                        }
                    }
 
                    pw = varstart;
                    if( *pchar == '.' ) {
                        pchar++;        // copy terminating dot, too
                    }
                    while( pw < pchar ) {   // treat var name as text
                        *p2++ = *pw++;  // and copy
                    }
                }
            }
            pchar = strchr( pw, ampchar );  // look for next & in buffer
        }                               // while & found
 
        while( pw <= pwend) {           // copy remaining input
             *p2++ = *pw++;
        }
 
    } while( ProcFlags.unresolved && ProcFlags.substituted );
 
    anything_substituted |= ProcFlags.substituted;
 
    buff2_lg = strnlen_s( buff2, buf_size );
 
    if( GlobalFlags.research && GlobalFlags.firstpass && anything_substituted ) {
        g_info( inf_subst_line, buff2 );// show line with substitution(s)
    }
    mem_free( workbuf );
 
    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;
    return;
}
