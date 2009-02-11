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

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


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
    int                 varunresolved;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;

    //  look for GML tag start character and split line if found

    pchar = strchr( buff2, GML_char );  // look for GML tag start
    if( (pchar != NULL) && (buff2 < pchar) ) {
        split_input( buff2, pchar );    // if found at pos > 1 split
        buff2_lg = strlen( buff2 );     // new length of first part
    }

    // if macro define ( .dm xxx ... ) supress variable substitution
    // for the sake of single line macro definition
    // .dm xxx /&*1/&*2/&*0/&*/
    // this is the same as wgml 4.0

    if( (*buff2 == SCR_char) && !strnicmp( buff2 + 1, "dm ", 3 ) ) {
        return;
    }

    if( (*buff2 == SCR_char) ) {

        // for lines starting .* or .' ignore control word seperator

        if( !((*(buff2 + 1) == '*') || (*(buff2 + 1) == '\'')) ) {
            pchar = strchr( buff2, CW_sep_char );
            if( (pchar != NULL) ) {
                split_input( buff2, pchar + 1 );// split after CW_sep_char

                buff2_lg = strlen( buff2 ); // new length of first part
                buff2_lg--;             // ignore CW_sep_char
                *(buff2 + buff2_lg) = '\0';
            }
        }
    }

    workbuf = mem_alloc( buf_size );

    // look for symbolic variable or function start
    varunresolved = 0;
    do {
        strcpy_s( workbuf, buf_size, buff2 );   // copy input buffer
        pw = workbuf;
        pwend = workbuf + buff2_lg;
        p2 = buff2;
        varstart = NULL;
        pchar = strchr( workbuf, ampchar ); // look for & in buffer
        while( pchar != NULL ) {        // & found
            buff2_lg = strnlen_s( buff2, buf_size );
            if( workbuf < pchar ) {
                while( pw < pchar ) {   // copy all data preceding &
                    *p2++ = *pw++;
                }
            }

    /***********************************************************************/
    /*  Some single letter functions are resolved here:                    */
    /*                                                                     */
    /*  functions used within the OW doc build system:                     */
    /*   &e'  existance of variable 0 or 1                                 */
    /*   &l'  length of variable content or if undefined length of name    */
    /*   &u'  upper                                                        */
    /*                                                                     */
    /*   &s'  subscript    These are recognized, but processed as &u'      */
    /*   &S'  superscript           TBD                                    */
    /*                                                                     */
    /*   other single letter functions are not used AFAIK                  */
    /*                                                                     */
    /***********************************************************************/
            if( *(pchar + 2) == '\'' ) {
                char * * ppval = &p2;

                pw = scr_single_funcs( pchar, pwend, ppval );
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                continue;
            }

    /***********************************************************************/
    /*  Some multi  letter functions are resolved here:                    */
    /*                                                                     */
    /*  functions used within the OW doc build system:                     */
    /*   &'delstr(                                                         */
    /*   &'d2c(                                                            */
    /*   ...                                                               */
    /*   &'upper(                                                          */
    /*   ...                                                               */
    /*                                                                     */
    /*   Others are recognized but not processed                           */
    /*                                                                     */
    /*                                                                     */
    /***********************************************************************/
            if( *(pchar + 1) == '\'' ) {
                char * * ppval = &p2;

                pw = scr_multi_funcs( pchar, pwend, ppval );
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                continue;
            }


            varstart = pw;              // remember start of var
            pw++;                       // over &
            ProcFlags.suppress_msg = true;
            scan_err = false;

            pchar = scan_sym( pw, &symvar_entry, &var_ind );
            if( scan_err && *pchar == '(' ) {   // problem with subscript
                varunresolved++;
                p2 = pchar - workbuf + buff2;
                pw = pchar;
                pchar = strchr( pw, ampchar );  // look for next & in buffer
                continue;

#if 0
                if( *(pchar+1) == ampchar ) {
                    symvar          symvar_ind;
                    char        *   pchar2;

                    scan_err = false;
                    pchar2 = scan_sym( pchar + 2, &symvar_ind, &var_ind );
                    if( !scan_err ) {
                        if( symvar_ind.flags & local_var ) {
                            rc = find_symvar( &input_cbs->local_dict,
                                              symvar_ind.name,
                                              var_ind, &symsubval );
                        } else {
                            rc = find_symvar( &global_dict, symvar_ind.name,
                                              var_ind, &symsubval );
                        }
                        if( rc == 2 ) {
                            var_ind = atoi( symsubval->value );
                            pchar = pchar2;
                            if( *pchar == '.' ) {
                                pchar++;
                            }
                            if( *pchar == ')' ) {
                                pchar++;
                            }
                        }

                    }
                }
#endif
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
                if( symsubval->value[ 0 ] == CW_sep_char ) {
                    // split record at control word seperator
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    *(varstart - workbuf + buff2) = '\0';
                    split_input_var( buff2, pchar - workbuf + buff2,
                                     &symsubval->value[ 1 ] );
                    pw = pwend + 1;     // stop substitution for this record
                    varstart = NULL;
                    break;
                } else {
                    strcpy( p2, symsubval->value ); // copy value
                    p2 += strlen( symsubval->value );
                    if( *pchar == '.' ) {
                        pchar++;        // skip optional terminating dot
                    }
                    pw = pchar;
                }
            } else {
                if( symvar_entry.flags & local_var ) { // local var not found
                    if( (symvar_entry.name[ 0 ] == '\0') &&
                        (*pchar == ampchar) ) { // only &* as var name
                                                // followed by another var

                        varunresolved++;

                        pw = varstart;
                        while( pw < pchar ) {   // treat var name as text
                            *p2++ = *pw++;  // and copy
                        }
                        continue;       // pchar points already to next &

                    } else {
                                                  // replace by nullstring
                        if( *pchar == '.' ) {
                            pchar++;        // skip optional terminating dot
                        }
                        pw = pchar;
                    }
                } else {                // global var not found

                /***********************************************************/
                /*  keep trying for constructs such as                     */
                /*                                                         */
                /* .se prodgml = "Open Watcom GML"                         */
                /* .se name = "GML"                                        */
                /*                                                         */
                /* My name is &prod&name..!                                */
                /*                                                         */
                /*  to become                                              */
                /*                                                         */
                /* My name is Open Watcom GML!                             */
                /*                                                         */
                /* This does not work for local variables, as these are    */
                /* replaced by nullstring if not found                     */
                /* My name is &*prod&*name..!                              */
                /*  will become                                            */
                /* My name is !                                            */
                /***********************************************************/

                    varunresolved++;

                    if( varunresolved > 10 ) {  // max 10 iterations
                        varunresolved = 0;
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
    } while( (varunresolved > 0) && (varstart != NULL) );

    buff2_lg = strnlen_s( buff2, buf_size );

    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        printf( "> >%s< <\n", buff2 );  // show line with substitution(s)
    }
    mem_free( workbuf );

    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;
    return;
}
