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

#include "wgml.h"

static  bool        sym_space;          // compiler workaround

/***************************************************************************/
/*  perform symbol substitution on certain special symbols:                */
/*    AMP (identied by the late_subst tag)                                 */
/*    any symbol whose value is a single character matching GML_char (':'  */
/*      by default)                                                        */
/*  all other items starting with '&' are treated as text                  */
/***************************************************************************/
void process_late_subst( char * buf )
{
    char    *   p;
    char    *   symstart;
    char        tail[BUF_SIZE];
    char    *   tokenend;       // save end of current token
    char    *   tokenstart;     // save position of current '&'
    int         rc;
    sub_index   var_ind;        // subscript value (if symbol is subscripted)
    symsub  *   symsubval;      // value of symbol
    symvar      symvar_entry;

    p = strchr( buf, ampchar );  // look for & in buffer
    while( p != NULL ) {         // & found
        if( *(p + 1) == ' ' ) {  // not a symbol substition, attribute, or function
        } else if( my_isalpha( p[1] ) && p[2] == '\'' && p[3] > ' ' ) {   // attribute
        } else if( *(p + 1) == '\'' ) {          // function or text
            /* all above are ignored */
        } else {                                // symbol
            tokenstart = p;
            p++;                                // over '&'
            symstart = p;                       // remember start of symbol name
            scan_err = false;
            ProcFlags.suppress_msg = true;
            p = scan_sym( symstart, &symvar_entry, &var_ind );
            ProcFlags.suppress_msg = false;
            tokenend = p;
            if( !scan_err ) {                   // potentially qualifying symbol
                if( symvar_entry.flags & local_var ) {  // lookup var in dict
                    rc = find_symvar_l( &input_cbs->local_dict, symvar_entry.name,
                                        var_ind, &symsubval );
                } else {
                    rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                                      &symsubval );
                }
                if( rc == 2 ) {             // variable found + resolved
                    if( (symsubval->base->flags & late_subst) ||
                            ((symsubval->value[0] == GML_char) && (symsubval->value[1] == '\0')) ) {
                        /* replace symbol with value */
                        strcpy_s( tail, buf_size, tokenend );       // copy tail
                        p = tokenstart;
                        strcpy_s( p, buf_size, symsubval->value );  // copy value
                        if( tail[0] == '.' ) {
                            strcat_s( buf, buf_size, tail + 1);     // append tail to buf, skipping initial "."
                        } else {
                            strcat_s( buf, buf_size, tail );        // append tail to buf
                        }
                    }
                    /* all other symbols are ignored */
                }
            }
        }
        p = tokenend;                   // skip argument
        p = strchr( p, ampchar );       // look for next & in buffer
    }
    return;
}


/*  split_input
 *  The (physical) line is split
 *  The second part will be processed by the next getline()
 *   pushing any already split part down
 *
 */

void split_input( char * buf, char * split_pos, bool sol )
{
    inp_line    *   wk;
    size_t          len;

    len = strlen( split_pos );          // length of second part
    if( len > 0 ) {
        wk = mem_alloc( len + sizeof( inp_line ) );
        wk->next = input_cbs->hidden_head;
        wk->sol  = sol;
        wk->fm_symbol = false;
        wk->sym_space = false;
        strcpy(wk->value, split_pos );  // save second part

        input_cbs->hidden_head = wk;
        if( input_cbs->hidden_tail == NULL ) {
            input_cbs->hidden_tail = wk;
        }

        *split_pos = '\0';              // terminate first part
        input_cbs->fmflags &= ~II_eol;  // not last part of line
    }
    return;
}


/*  split_input_var
 *  The second part is constructed from 2 parts
 *  used if a substituted variable starts with CW_sep_char
 */

static void split_input_var( char * buf, char * split_pos, char * part2, bool sol )
{
    inp_line    *   wk;
    size_t          len;

    len = strlen( split_pos ) + strlen( part2 );// length of second part
    if( len > 0 ) {
        wk = mem_alloc( len + sizeof( inp_line ) );
        wk->next = input_cbs->hidden_head;
        wk->sol  = sol;
        wk->fm_symbol = false;
        wk->sym_space = sym_space;

        strcpy(wk->value, part2 );      // second part
        strcat(wk->value, split_pos );  // second part

        input_cbs->hidden_head = wk;
        if( input_cbs->hidden_tail == NULL ) {
            input_cbs->hidden_tail = wk;
        }
        input_cbs->fmflags &= ~II_eol;  // not last part of line
    }
    return;
}


/***************************************************************************/
/*  look for GML tag start character and split line if valid GML tag       */
/*  don't split if blank follows gml_char                                  */
/*  special for  xxx::::TAG construct                                      */
/*  don't split if line starts with :CMT.                                  */
/***************************************************************************/
static void split_at_GML_tag( void )
{
    char    *   p;
    char    *   p2;
    char    *   pchar;
    char        c;
    bool        layoutsw;
    size_t      toklen;

    if( *buff2 == GML_char ) {
        if( !strnicmp( (buff2 + 1), "CMT", 3 ) &&
            ((*(buff2 + 4) == '.') || (*(buff2 + 4) == ' ')) ) {
            return;                     // no split for :cmt. line
        }
    }

    /***********************************************************************/
    /*  Look for GML tag start char(s) until a known tag is found          */
    /*  then split the line                                                */
    /***********************************************************************/
    pchar = strchr( buff2 + 1, GML_char );
    while( pchar != NULL ) {
        while( *(pchar + 1) == GML_char ) {
            pchar++;                    // handle repeated GML_chars
        }
        for( p2 = pchar + 1;
             is_id_char( *p2 ) && (p2 < (buff2 + buf_size));
             p2++ ) /* empty loop */ ;

        if( (p2 > pchar + 1)
            && ((*p2 == '.') ||
                is_space_tab_char( *p2 ) ||
                (*p2 == '\0') ||
                (*p2 == GML_char) ) ) { // 'good' tag end

            c = *p2;
            if( ProcFlags.layout && (c == '\t') ) {
                c = ' ';                // replace tab with space in layout
            }
            *p2 = '\0';                 // null terminate string
            toklen = p2 - pchar - 1;

            /***************************************************************/
            /* Verify valid user or system tag                             */
            /***************************************************************/
            if( (find_tag( &tag_dict, pchar + 1 ) != NULL) ||
                (find_sys_tag( pchar + 1, toklen ) != NULL) ||
                (find_lay_tag( pchar + 1, toklen ) != NULL) ) {

                *p2 = c;

                if( input_cbs->fmflags & II_sol ) {
                // remove spaces before tags at sol in restricted sections
                // in or just before LAYOUT tag
                    layoutsw = ProcFlags.layout;
                    if( !layoutsw && (strncmp( "LAYOUT", pchar + 1, 6 ) == 0 ) ) {
                        layoutsw = true;
                    }
                    if( (rs_loc > 0) || layoutsw ) {
                        p = buff2;
                        SkipSpacesTabs( p );
                        if( p == pchar ) {  // only leading blanks
                            memmove_s( buff2, buf_size, pchar, buf_size - (p - buff2) );
                            buff2_lg = strnlen_s( buff2, buf_size );// new length
                            pchar = strchr( buff2 + 1, GML_char );  // try next GMLchar
                            continue;       // dummy split done try again
                        }
                    }
                }
                split_input( buff2, pchar, false );// split line
                if( ProcFlags.literal ) {   // if literal active
                    if( li_cnt < LONG_MAX ) {// we decrement, adjust for split line
                        li_cnt++;
                    }
                }
                break;                  // we did a split stop now
            } else {
                *p2 = c;
            }
        }
        pchar = strchr( pchar + 1, GML_char );  // try next GMLchar
    }
}


/***************************************************************************/
/*  look for control word separator character and split line if found      */
/*  and other conditions don't prevent it                                  */
/*  if splitpos is not NULL, the CWsep is already known                    */
/***************************************************************************/

static void split_at_CW_sep_char( char * splitpos ) {
    /***********************************************************/
    /*  if 2 CW_sep_chars follow, don't split line             */
    /*  ... this is NOWHERE documented, but wgml 4.0 tested    */
    /*  also don't split if last char of record                */
    /***********************************************************/

    if( !ProcFlags.CW_sep_ignore ) {
        if( splitpos == NULL ) {        // splitpos not yet known
            splitpos = strchr( buff2 + 2, CW_sep_char );
        }
        if( (splitpos != NULL) && (*(splitpos + 1) != '\0') ) {

            if( (*(splitpos - 1) == '\'') && (*(splitpos + 1) == '\'') ) {
                ;
            // hack for testing macro repchars docs\doc\hlp\fmtmacro.gml(174)
                                        // don't split &'index("&x.",';') TBD
            } else {

                if( *(splitpos + 1) != CW_sep_char ) {
                    split_input( buff2, splitpos + 1, true );// split after CW_sep_char

                    buff2_lg = strnlen_s( buff2, buf_size ) - 1;
                    *(buff2 + buff2_lg) = '\0'; // terminate 1. part
#if 0
                } else {                // ignore 1 CW_sep_char
                    memmove_s( splitpos, splitpos - buff2 + buff2_lg + 1,
                               splitpos + 1, splitpos - buff2 + buff2_lg );
                    buff2_lg = strnlen_s( buff2, buf_size );
#endif
                }
            }
        }
    }
}


/***************************************************************************/
/*   Split input line at GML tag or script Control word separator          */
/*   returns false if :cmt .cm  .dm found                                  */
/***************************************************************************/

static bool split_input_buffer( void )
{
    char            *   p2;
    char            *   pchar;
    int                 k;

    /***********************************************************************/
    /*  look for GML tag start character and split line at GML tag         */
    /*  special for script control line: possibly split at CW_sep_char     */
    /***********************************************************************/
    if( !ProcFlags.literal && (*buff2 == SCR_char) ) {
        pchar = strchr( buff2 + 1, CW_sep_char );
        p2 = strchr( buff2 + 1, GML_char );
        if( pchar && (p2 > pchar) ) {// GML_char follows CW_sepchar in buffer

            for( p2 = buff2 + 1; *p2 == SCR_char; p2++ ) {
                ; /* empty */
            }
            if( !(  (*p2 == '\'') ||    // suppress CW_separator
                    (*p2 == '*')  ||    // comment
                    (CW_sep_char == '\0') ||// no CW sep char
                    !strnicmp( p2, "cm ", 3) // comment
                 ) ) {

                split_at_CW_sep_char( pchar );  // now split record
            }
        }
    }
    split_at_GML_tag();

    if( !ProcFlags.literal ) {

        /*******************************************************************/
        /* for :cmt. minimal processing                                    */
        /*******************************************************************/

        if( (*buff2 == GML_char) && !strnicmp( buff2 + 1, "cmt.", 4 ) ) {
            return( false );
        }

        /*******************************************************************/
        /*  .xx SCRIPT control line                                        */
        /*******************************************************************/

        if( (*buff2 == SCR_char) ) {

            if( (*(buff2 + 1) == '*') || !strnicmp( buff2 + 1, "cm ", 3 ) ) {
                return( false );  // for .* or .cm comment minimal processing
            }

            /***************************************************************/
            /* if macro define (.dm xxx ... ) supress variable substitution*/
            /* for the sake of single line macro definition                */
            /* .dm xxx / &*1 / &*2 / &*0 / &* /                            */
            /*  and                                                        */
            /* ..dm xxx / &*1 / &*2 / &*0 / &* /                           */
            /***************************************************************/
            if( *(buff2 + 1) == SCR_char ) {
                k = 2;
            } else {
                k = 1;
            }
            if( !strnicmp( buff2 + k, "dm ", 3 ) ) {
                return( false );
            }

            /***************************************************************/
            /*  for records starting  .' which were not indented ignore    */
            /*  control word separator                                     */
            /*  or if control word separator is 0x00                       */
            /***************************************************************/

            if( (!ProcFlags.CW_indented && (*(buff2 + k) == '\'')) || (CW_sep_char == '\0') ) {
                ProcFlags.CW_sep_ignore = true;
            } else {
                ProcFlags.CW_sep_ignore = false;

                split_at_CW_sep_char( NULL );
            }
        }
    }
    return( true );                     // further processing needed
}


/***************************************************************************/
/*  parse the current input buffer from right to left, using the stack of  */
/*  sym_list_entry instances created previously                            */
/*  this is the algorithm for unwinding the stack:                         */
/*    if curr->type is not sl_text, then                                   */
/*      if curr->type is sl_split, then                                    */
/*        append tail to buf and split the line                            */
/*      otherwise                                                          */
/*          copy text from curr->end on from buf to tail                   */
/*          copy the value from curr->value to curr->start                 */
/*          if the first character in the tail is ".", then                */
/*            append tail+1 to buf thus skipping the "."                   */
/*          otherwise append tail to buf                                   */
/*    otherwise do nothing                                                 */
/***************************************************************************/

static bool parse_r2l( sym_list_entry * stack, char * buf )
{
    char            *   p;
    char                tail[BUF_SIZE];
    size_t              cw_lg;
    sym_list_entry  *   curr            = stack;

    ProcFlags.substituted = false;
    tail[0] = '\0';

    while( curr != NULL ) {
        switch( curr->type ) {
        case sl_text:
            break;
        case sl_attrib:
            ProcFlags.substituted = true;
            strcpy_s( tail, buf_size, curr->end );      // copy tail
            p = curr->start;
            if( curr->value[0] == CW_sep_char && curr->value[1] == CW_sep_char ) {
                p++;                                    // skip 1 CW_sep_char
            }
            strcpy_s( p, buf_size, curr->value );       // copy value
            if( tail[0] == '.' ) {
                strcat_s( buf, buf_size, tail + 1);     // append tail to buf, skipping initial "."
            } else {
                strcat_s( buf, buf_size, tail );        // append tail to buf
            }
            break;
        case sl_funct:
            ProcFlags.substituted = true;
            p = curr->start;
            if( curr->value[0] == CW_sep_char && curr->value[1] == CW_sep_char ) {
                p++;                                    // skip 1 CW_sep_char
            }

            strcpy_s( tail, buf_size, curr->end );      // copy tail
            p = curr->start;
            strcpy_s( p, buf_size, curr->value );       // copy value
            if( tail[0] == '.' ) {
                strcat_s( buf, buf_size, tail + 1);     // append tail to buf, skipping initial "."
            } else {
                strcat_s( buf, buf_size, tail );        // append tail to buf
            }
            break;
        case sl_symbol:
            ProcFlags.substituted = true;
            if( !ProcFlags.if_cond && !ProcFlags.dd_macro && !curr->value[0] ) {
                ProcFlags.null_value = true;
            }
            strcpy_s( tail, buf_size, curr->end );      // copy tail
            p = curr->start;
            if( curr->value[0] == CW_sep_char && curr->value[1] == CW_sep_char ) {
                p++;                                    // skip 1 CW_sep_char
            }
            strcpy_s( p, buf_size, curr->value );       // copy value
            if( tail[0] == '.' ) {
                strcat_s( buf, buf_size, tail + 1);     // append tail to buf, skipping initial "."
            } else {
                strcat_s( buf, buf_size, tail );        // append tail to buf
            }
            input_cbs->fm_symbol = false;               // no new logical input record
            if( input_cbs->fm_symbol ) {                    
                /* keep value if from prior symbol which created its own logical input record */
                sym_space = input_cbs->sym_space;
            } else {
                sym_space = false;
                if( curr->start == buf ) {              // symbol at start of input line
                    sym_space = true;
                } else {                                // symbol not at start of input line
                    if( *curr->value == !SCR_char ) {   // not an scw or macro
                        sym_space = (*(curr->start - 1) == ' ');
                    }
                }
            }
            input_cbs->sym_space = sym_space;
            break;
        case sl_split:
            ProcFlags.substituted = true;
            if( !ProcFlags.if_cond && !ProcFlags.dd_macro && !curr->value[0] ) {
                ProcFlags.null_value = true;
            }
            strcat_s( buf, buf_size, tail);             // append tail to buf
            split_input_var( buf, p, &curr->value[1], true );
            input_cbs->hidden_head->fm_symbol = true;   // new logical input record
            cw_lg = 0;
            for( p = buf; *p != ' '; p++ ) cw_lg++;     // length of . plus CW
            cw_lg++;                                    // plus space after CW
            if( ProcFlags.scr_cw && (buf + cw_lg == curr->start) ) {
                input_cbs->hidden_head->sym_space = false;  // space is space after cw
            } else {
                if( curr->start == buf ) {                  // symbol at start of input line
                    sym_space = true;
                } else {                                    // symbol not at start of input line
                    input_cbs->hidden_head->sym_space = (*(curr->start - 1) == ' ');
                }
            }
            break;
        default:
            internal_err( __FILE__, __LINE__ );
            break;
        }
        curr = curr->prev;
    }
    add_sym_list_entry_to_pool( stack );
    return( ProcFlags.substituted );
}


/***************************************************************************/
/*  parse the current input buffer from left to right, building a stack of */
/*  sym_list_entry instances                                               */
/*  fully resolve all items                                                */
/*  if breakable is true, then:                                            */
/*    return when a symbol (as opposed to an attribute or a function) has  */
/*    a value which starts with the control word separator (";" by         */
/*    default) or the end of buf is reached                                */
/*  if breakable is false, then:                                           */
/*    return only when the end of buf is reached                           */
/***************************************************************************/

static sym_list_entry * parse_l2r( char * buf, bool breakable )
{
    char            *       p;
    char            *       pa;
    char            *   *   ppval;
    char            *       symstart;           // start of symbol name
    char                    valbuf[BUF_SIZE];   // buffer for attribute function and function values
    int                     rc;
    int32_t                 valsize;
    sub_index               var_ind;            // subscript value (if symbol is subscripted)
    symsub          *       symsubval;          // value of symbol
    symvar                  symvar_entry;
    sym_list_entry  *       curr    = NULL;     // current top-of-stack
    sym_list_entry  *       temp    = NULL;     // used to create new top-of-stack

    p = strchr( buf, ampchar );  // look for & in buffer
    while( p != NULL ) {         // & found
        temp = alloc_sym_list_entry();
        if( curr == NULL ) {
            curr = temp;            // initialize stack
        } else {
            temp->prev = curr;      // push stack down
            curr = temp;
        }
        if( *(p + 1) == ' ' ) {  // not a symbol substition, attribute, or function
            curr->start = p;
            curr->end = p + 2;
            curr->type = sl_text;               // text
        } else if( my_isalpha( p[1] ) && p[2] == '\'' && p[3] > ' ' ) {   // attribute
            curr->start = p;
            curr->end = curr->start;
            while( !is_space_tab_char( *curr->end ) && (*curr->end != '\0') ) curr->end++;
            curr->type = sl_attrib;
            pa = valbuf;
            ppval = &pa;

            if( GlobalFlags.firstpass && (input_cbs->fmflags & II_research) ) {
                add_single_func_research( curr->start + 1 );
            }

            curr->end = scr_single_funcs( curr->start, curr->end, ppval );
            strcpy_s( curr->value, buf_size, valbuf );      // save value in current stack entry
        } else if( *(p + 1) == '\'' ) {          // function or text
            curr->start = p;
            p += 2;                             // over "&'"
            while( is_function_char(*p) ) p++;  // find end of function name

            if( *p == '(' ) {                   // &'xyz( is start of multi char function
                curr->type = sl_funct;
                pa = valbuf;
                ppval = &pa;
                valsize = buf_size - (curr->end - curr->start);
                curr->end = scr_multi_funcs( curr->start, ppval, valsize );
                strcpy_s( curr->value, buf_size, valbuf );  // save value in current stack entry
            } else {
                curr->end = p;
                curr->type = sl_text;           // text
            }
        } else {                                // symbol
            curr->start = p;
            p++;                                // over '&'
            symstart = p;                       // remember start of symbol name
            scan_err = false;
            ProcFlags.suppress_msg = true;
            p = scan_sym( symstart, &symvar_entry, &var_ind );
            ProcFlags.suppress_msg = false;
            curr->end = p;
            if( scan_err && *p == '(' ) {    // problem with subscript
                pa = valbuf;
                ppval = &pa;
                curr->end = finalize_subscript( curr->start, ppval, valsize );
                strcpy_s( curr->value, buf_size, valbuf );  // save value in current stack entry
                var_ind = atol( curr->value );  // save value for use
                *curr->value = '\0';            // overwrite with nothing
                curr->end++;                    // over ')'
            }
            if( symvar_entry.flags & local_var ) {  // lookup var in dict
                rc = find_symvar_l( &input_cbs->local_dict, symvar_entry.name,
                                    var_ind, &symsubval );
            } else {
                rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                                  &symsubval );
            }
            if( rc == 2 ) {             // variable found + resolved
                if( !ProcFlags.CW_sep_ignore && breakable && CW_sep_char != 0x00 &&
                        curr->value[0] == CW_sep_char && curr->value[1] != CW_sep_char ) {
                    curr->type = sl_split;
                    strcpy_s( curr->value, buf_size, symsubval->value );  // save value in current stack entry
                    break;              // line split terminates processing
                } else if( (symsubval->base->flags & late_subst) ||
                        ((symsubval->value[0] == GML_char) && (symsubval->value[1] == '\0')) ) {
                    curr->type = sl_text;   // save for late substitution
                } else {
                    curr->type = sl_symbol;
                    strcpy_s( curr->value, buf_size, symsubval->value );  // save value in current stack entry
                }
            } else {
                curr->type = sl_text;   // TBD
            }
        }
        if( *curr->end == '\0' ) {
            break;                      // end of text terminates processing
        }
        p = curr->end;                  // skip argument
        p = strchr( p, ampchar );       // look for next & in buffer
    }
    return( curr );
}


/***************************************************************************/
/*  symbolic variables, single-letter attribute functions and multi-letter */
/*  functions are resolved here                                            */
/*  currently, this is done using two helper functions                     */
/***************************************************************************/

bool resolve_symvar_functions( char * buf, bool breakable )
{
    bool                anything_substituted    = false;
    sym_list_entry  *   stack;

    if( buf == NULL ) {
        return( false );                    // no text to process
    }
    stack = parse_l2r( buf, breakable );
    if( stack == NULL ) {
        return( false);                     // no stack to process
    }
    parse_r2l( stack, buf );
    anything_substituted |= ProcFlags.substituted;
    while( ProcFlags.substituted ) {
        stack = parse_l2r( buf, breakable );
        if( stack == NULL ) {
            break;                      // no stack to process
        }
        parse_r2l( stack, buf );
        anything_substituted |= ProcFlags.substituted;
    }
    
    return( anything_substituted );
}


/***************************************************************************/
/*  set  some switches if char is script controlword start or              */
/*  gml tag start                                                          */
/***************************************************************************/

void classify_record( char c )
{
    if( c == GML_char ) {               // classify input
        ProcFlags.gml_tag = true;
        ProcFlags.scr_cw = false;
    } else {
        ProcFlags.gml_tag = false;
        if( c == SCR_char ) {
            ProcFlags.scr_cw = true;
        } else {
            ProcFlags.scr_cw = false;
        }
    }
}


/***************************************************************************/
/*  remove leading space                                                   */
/*  to catch lines such as: "    :TAG xyz."  -> ":TAG xyz."                */
/*  returns true if spaces removed                                         */
/***************************************************************************/

static bool remove_leading_space( void )
{
    char    * p;
    char    * p2;
    bool    removed = false;

    if( ProcFlags.literal ) {           // .li active
        return( false );                // don't change input
    }
    p = buff2;
    SkipSpacesTabs( p );
    if( (p != buff2) && (*p == GML_char) && (cur_group_type != gt_xmp) ) {
        p2 = buff2;
        do {
            *p2++ = *p++;
        } while( *p != '\0' );
        *p2++ = '\0';
        *p2   = '\0';
        removed = true;
    }
    return( removed );
}


/***************************************************************************/
/*  take the contents of the input line in buff2 and try to make the best  */
/*  of it                                                                  */
/*  Processing as specified in wgmlref.pdf chapter 8.1 processing rules    */
/*  and results from testing with wgml 4                                   */
/*                                        imcomplete               TBD     */
/*                                                                         */
/***************************************************************************/

void process_line( void )
{
    bool    anything_substituted;

    anything_substituted = remove_leading_space();// for "   :TAG   " constructs

    classify_record( *buff2 );      // classify script CW, GML tag or nothing

    if( !split_input_buffer() ) {
        return;                         // .* .cm .dm :cmt found
    }

    if( ProcFlags.scr_cw && (toupper(*(buff2 + 1)) == 'I') && (toupper(*(buff2 + 2)) == 'F') ) {
        ProcFlags.if_cond = true;
    } else {
        ProcFlags.if_cond = false;
    }
    anything_substituted |= resolve_symvar_functions( buff2, true );

    classify_record( *buff2 );      // classify script CW, GML tag or nothing after symbol substitution etc

    buff2_lg = strnlen_s( buff2, buf_size );

    if( (input_cbs->fmflags & II_research) && GlobalFlags.firstpass &&
        anything_substituted ) {
        g_info_lm( inf_subst_line, buff2 ); // show line with substitution(s)
    }

    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;
    return;
}

