/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2012 The Open Watcom Contributors. All Rights Reserved.
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
* Description: WGML helper functions for index processing
*                   for  .ix control word
*                   and  :I1 :I2 :I3 :IH1 :IH2 :IH3 gml tags
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

static  symsub      *   ixrefval;


/***************************************************************************/
/*  helper functions for .ix .dump processing (experimental)         TBD   */
/***************************************************************************/

static void ix_out_sp( int spaces )
{
    static const char blanks[12] = "           ";
    int k;

    k = 2 * spaces;
    if( k > 10 || k < 0 ) {
        k = 10;
    }
    out_msg("%s", &blanks[10 - k] );
}

static void ix_out( char * t, int spaces )
{
    if( spaces > 0 ) {
        ix_out_sp( spaces );
    }
    out_msg("%s", t );
}

static void ix_out_cr( void )
{
    out_msg("\n" );
}



static void ix_out_pagenos( ix_e_blk * e )
{
    ix_e_blk    *   ew;
    char            str[16];

    for( ew = e; ew != NULL ; ew = ew->next ) {
        ix_out( ixrefval->value, 0 );
        if( ew->entry_typ == pgstring ) {   // 'pageno' is text
            ix_out( ew->page_text, 0 );
        } else {
            utoa( ew->page_no, str, 10 );
            ix_out( str, 0 );
        }
    }
    ix_out_cr();
}

/***************************************************************************/
/*  .ix .dump processing                                                   */
/***************************************************************************/

void    ixdump( ix_h_blk * dict )
{
    ix_h_blk    *   ixh;
    ix_h_blk    *   ixh2;
    ix_h_blk    *   ixh3;
    char            letter[2];

    if( dict == NULL ) {                // empty dict nothing to do
        return;
    }
    letter[0]  = 0;
    letter[1]  = 0;
    ixh = dict;
    find_symvar( &sys_dict, "$ixref", no_subscript, &ixrefval);

    out_msg( ".IX . DUMP print the index structure --- still nearly dummy!\n" );

    while( ixh != NULL ) {              // level 1
        if( letter[0] != toupper( ixh->text[0] ) ) {
            letter[0] = toupper( ixh->text[0] );
            ix_out_cr();
            ix_out( letter, 1 );
            ix_out_cr();
            ix_out_cr();
        }
        ix_out( ixh->text, 1 );

        ixh2 = ixh->lower;
        if( ixh2 == NULL ) {
            ix_out_pagenos( ixh->entry );
        } else {
            ix_out_cr();
            while( ixh2 != NULL ) {     // level 2
                ix_out( ixh2->text, 2 );

                ixh3 = ixh2->lower;
                if( ixh3 == NULL ) {
                    ix_out_pagenos( ixh2->entry );
                } else {
                    ix_out_cr();
                    while( ixh3 != NULL ) { // level 3
                        ix_out( ixh3->text, 3 );
                        ix_out_pagenos( ixh3->entry );

                        ixh3 = ixh3->next;
                    }
                }
                ixh2 = ixh2->next;
            }
        }
        ixh = ixh->next;
    }

    out_msg( ".IX . DUMP print the index structure --- still nearly dummy!\n" );

    wng_count++;
}


/***************************************************************************/
/*  allocate and fill a new index entry                                    */
/*                                                                         */
/***************************************************************************/

void fill_ix_e_blk( ix_e_blk * * anchor, ix_h_blk * ref, ereftyp ptyp,
                    char * text, int text_len )
{
    ix_e_blk    * ixewk;

    ixewk = mem_alloc( sizeof( ix_e_blk ) );
    ixewk->next      = NULL;
    ixewk->corr      = ref;
    ixewk->entry_typ = ptyp;
    if( ptyp == pgstring ) {
        ixewk->page_text = mem_alloc( text_len );
        strcpy_s( ixewk->page_text, text_len, text );
    } else {
        if( ProcFlags.page_started ) {
            ixewk->page_no = page;
        } else {
            ixewk->page_no = page + 1;
        }
    }
    *anchor = ixewk;
    return;
}


/***************************************************************************/
/*  free ix_e_blk chain                                                    */
/***************************************************************************/

static  void    free_ix_entries( ix_e_blk * e )
{
    ix_e_blk    *   ewk;
    ix_e_blk    *   ew  = e;

    while( ew != NULL ) {
        ewk = ew->next;
        if( ew->entry_typ == pgstring ) {
            mem_free( ew->page_text );
        }
        mem_free( ew );
        ew = ewk;
    }
}


/***************************************************************************/
/*  free all entries in index_dict                                         */
/***************************************************************************/

void    free_index_dict( ix_h_blk * * dict )
{
    ix_h_blk    *   ixh;
    ix_h_blk    *   ixh2;
    ix_h_blk    *   ixh3;
    ix_h_blk    *   ixhw;

    ixh = *dict;
    while( ixh != NULL ) {              // level 1

        free_ix_entries( ixh->entry );

        ixh2 = ixh->lower;
        while( ixh2 != NULL ) {         // level 2
            free_ix_entries( ixh2->entry );

            ixh3 = ixh2->lower;
            while( ixh3 != NULL ) {     // level 3
                free_ix_entries( ixh3->entry );
                ixhw = ixh3->next;
                mem_free( ixh3 );
                ixh3 = ixhw;
            }
            ixhw = ixh2->next;
            mem_free( ixh2 );
            ixh2 = ixhw;
        }
        ixhw = ixh->next;
        mem_free( ixh );
        ixh = ixhw;
    }
    *dict = NULL;                       // dict is now empty
}

