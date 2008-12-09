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
* Description:  WGML input line processing functions
*               not yet functional
*   some logic / ideas adopted from Watcom Script 3.2 IBM S/360 Assembler
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/*  split_input
 *  The second part will be processed by the next read
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
    char            *   p1;
    char            *   p1end;
    char            *   p2;
    char            *   pchar;
    char            *   varstart;
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

    // look for symbolic variable start

    strcpy( buff1, buff2 );
    p1 = buff1;
    p1end = buff1 + buff2_lg;
    p2 = buff2;
    varstart = NULL;
    pchar = strchr( buff1, ampchar );   // look for & in buffer
    while( pchar != NULL ) {            // & found
        if( buff1 < pchar ) {
            while( p1 < pchar ) {       // copy all data preceding &
                *p2++ = *p1++;
            }
        }
        varstart = p1;                  // remember start of var
        p1++;                           // over &

        pchar = scan_sym( p1, &symvar_entry, &var_ind );// isolate symbolic var

        if( symvar_entry.flags & local_var ) {  // lookup var in dict
            rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                              var_ind, &symsubval );
        } else {
            rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                              &symsubval );
        }
        if( rc == 2 ) {                  // found
            strcpy( p2, symsubval->value );
            p2 += strlen( symsubval->value );
            if( *pchar == '.' ) {
                pchar++;                // skip terminating dot
            }
            p1 = pchar;
        } else {
            if( symvar_entry.flags & local_var ) { // local var not found
                                                   // replace by nullstring
                if( *pchar == '.' ) {
                    pchar++;            // skip terminating dot
                }
                p1 = pchar;
            } else {                    // global var not found
                p1 = varstart;
                if( *pchar == '.' ) {
                    pchar++;            // copy terminating dot, too
                }
                while( p1 < pchar ) {   // treat var name as text
                    *p2++ = *p1++;
                }
            }
        }
        *p2 = '\0';
        pchar = strchr( p1, ampchar );  // look for next & in buffer
    }

    while( p1 <= p1end) {               // copy remaining input
         *p2++ = *p1++;
    }
    buff2_lg = strnlen_s( buff2, buf_size );

    if( (GlobalFlags.research && GlobalFlags.firstpass && (varstart != NULL)) ) {
        printf( "> >%s\n", buff2 );     // show line if something replaced
    }

    arg_start = buff2;
    arg_stop  = buff2 + buff2_lg;
    return;
}
