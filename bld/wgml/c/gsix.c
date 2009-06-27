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
* Description: implement .ix (index)  script control word
*                        only used options are implemented
*                        i.e.   index structure 1 only
*                               s1 s2 s3
*                               . dump   (experimental)
*
*               not implemented s1 s2 . ref
*                               . purge
*
*  comments are from script-tso.txt
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

/**************************************************************************/
/*                                                                        */
/*  !not all options are supported / implemented                          */
/*                                                                        */
/*                                                                        */
/* INDEX builds  an index structure  with up  to three levels  of headers */
/* with references, or prints the index structure.                        */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |       |    <1|n> 's1' <'s2' <'s3'>> <<.> <ref>>          |      */
/*      |  .IX  |                                                  |      */
/*      |       |    <1|n> . <DUMP|PURGE>                          |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* This control word does not cause a break.                              */
/*                                                                        */
/* <1|n>:  specifies the index structure (from 1 to 9) on which the oper- */
/*    ation is to be performed.  If omitted, structure 1 is used.         */
/*                                                                        */
/* Building the Index                                                     */
/*                                                                        */
/* 's1' <'s2' <'s3'>>:  adds  up to three levels of index  headers to the */
/*    index structure.  The index headers may be specified as undelimited */
/*    strings,  or as delimited strings  if they contain embedded blanks. */
/*    The current output page number will be used as the reference entry. */
/* <<.>  <ref>>:  specifies  a  "reference string"  that  is  to be  used */
/*    instead of the current output page  number for the reference entry. */
/*    If s3 (or s2 and s3)  is  omitted,  then the control word indicator */
/*    (normally period) must be placed between the index level(s) and the */
/*    reference operand.   If all four operands  are present,  the use of */
/*    the control word indicator as a separator is optional.   Use of the */
/*    control word indicator means a reference must follow,  even if null */
/*    (see the Example below).                                            */
/*                                                                        */
/* It is possible  to designate one or more reference  entries as primary */
/* reference entries,   so that  they will  appear first  in the  list of */
/* reference entries  for that index  header entry,  regardless  of where */
/* they appeared in  the input.   If the reference entry  is an asterisk, */
/* then the  current page number  will be  used as the  primary reference */
/* entry.   If the reference entry is a character string starting with an */
/* asterisk,  then the characters following the  asterisk will be used as */
/* the primary reference entry.   The asterisk character used to identify */
/* a primary reference may be changed with a ".DC PIX" control word.      */
/*                                                                        */
/* Printing the Index Structure                                           */
/*                                                                        */
/* .  DUMP:  DUMP  causes it to be  output and then deleted.    The index */
/*    structure will be printed in  alphabetic order within levels.   For */
/*    purposes of ordering the level entries,  the entries are treated as */
/*    if entered in uppercase as defined by the Translate Uppercase (.TU) */
/*    control word.                                                       */
/*       A list of characters  may be specified that are to  be sorted as */
/*    if they were blanks with the ".DC IXB" control word.  Characters to */
/*    be  totally  ignored  for  sorting purposes  may  be  defined  with */
/*    ".DC IXI".                                                          */
/*       Index references on a range of consecutive pages are joined with */
/*    the "&SYSPRS" symbol,   Page Range Separator,  which  defaults to a */
/*    hyphen  character (-)   but may  be redefined  before printing  the */
/*    index.   Other references are separated  by the Page List Separator */
/*    symbol,  ",  ",  which defaults  to comma/blank (, ).   The default */
/*    range character may be redefined or disabled with ".DC IXJ".   Only */
/*    internally generated page numbers are eligible for joining, never a */
/*    user reference string.   A null reference between two references on */
/*    consecutive  pages  will  make   those  references  ineligible  for */
/*    joining.                                                            */
/* .  PURGE:   PURGE causes  the index  structure to  be deleted  without */
/*    printing it.                                                        */
/*                                                                        */
/* NOTES                                                                  */
/* (1) This control word will be ignored  if the NOINDEX option is speci- */
/*     fied.  This can reduce the processing time for draft documents.    */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) .ix 'level1' 'level2'                                              */
/*     adds  a first-  and a  second-level  header and  the current  page */
/*     number as the reference entry, to index structure 1.               */
/* (2) .ix 2 'level1' . 'see...'                                          */
/*     adds a  first-level header  and the  reference string  "see..." to */
/*     index structure 2.                                                 */
/* (3) .ix 'level1' . ''                                                  */
/*     adds a  first-level header  and a  null reference  entry to  index */
/*     structure 1.                                                       */
/* (4) .ix 'level1' 'level2' . *                                          */
/*     adds a first- and second-level index  header to index structure 1, */
/*     using the current page number as a primary reference entry.        */
/* (5) .ix 'level1' . '*text'                                             */
/*     adds a first-level  index header to index structure  1,  using the */
/*     characters "text" as the primary reference entry.                  */
/* (6) The index  structure is printed with  the DUMP operand.    See the */
/*     Index Entry control word (.IE)  for  customizing the format of the */
/*     result.                                                            */
/*       .ix . DUMP                                                       */
/**************************************************************************/

/***************************************************************************/
/*  helper functions for .ix .dump processing (experimental)               */
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

    for( ew = e; ew != NULL ; ew = ew->next ) {
        ix_out( ew->refs, 0 );
    }
    ix_out_cr();
}

/***************************************************************************/
/*  .ix .dump processing                                                   */
/***************************************************************************/

static  void    ixdump( ix_h_blk * dict )
{
    ix_h_blk    *   ixh;
    ix_h_blk    *   ixh2;
    ix_h_blk    *   ixh3;
    char            letter[2];

    letter[0]  = 0;
    letter[1]  = 0;
    ixh = dict;
    if( ixh == NULL ) {                 // empty dict quit
        return;
    }
    out_msg( ".IX . DUMP print the index structure --- still nearly dummy!\n" );
    while( ixh != NULL ) {              // level 1
        if( letter[0] != toupper( ixh->text[0] ) ) {
            letter[0] = toupper( ixh->text[0] );
            ix_out_cr();
            ix_out( letter, 1 );
            ix_out_cr();
        }
        ix_out_cr();
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
/*  .ix control word processing                                            */
/***************************************************************************/

void    scr_ix( void )
{
    condcode        cc;
    char            cwcurr[4];
    int             lvl;
    int             k;
    int             comp_len;
    int             comp_res;
    char        *   ix[3];
    uint32_t        ixlen[3];
    symsub      *   pageval;
    symsub      *   ixrefval;
    ix_h_blk    * * ixhprev;
    ix_h_blk    *   ixhwk;
    ix_e_blk    *   ixewk;
    ix_e_blk    *   ixewkprev;
    bool            do_nothing;
    int             currlen;


    scan_restart = scan_stop + 1;
    if( !GlobalFlags.index ) {          // no index option specified
        return;                         // no need to process .ix
    }
    cwcurr[0] = SCR_char;
    cwcurr[1] = 'i';
    cwcurr[2] = 'x';
    cwcurr[3] = '\0';
    lvl = 0;                            // index level


    garginit();                         // over control word

    while( lvl < 3 ) {                  // try to get 3 lvls of index

        cc = getarg();

        if( cc == omit || cc == quotes0 ) { // no (more) arguments
            if( lvl == 0 ) {
                parm_miss_err( cwcurr );
            } else {
                break;
            }
        } else {
            if( *tok_start == '.' && arg_flen == 1  ) {
                if( lvl > 0 ) {
                    xx_opt_err( cwcurr, tok_start );
                    break;
                }
                cc = getarg();
                if( cc == pos || cc == quotes ) {   // .ix . dump ???
                    if( arg_flen == 4 ) {
                        if( !strnicmp( tok_start, "DUMP", 4 ) ) {

                            ixdump( index_dict );   // print index

                            break;
                        }
                    }
                    xx_opt_err( cwcurr, tok_start );
                } else {
                    parm_miss_err( cwcurr );
                }
                break;                  // no index entry text
            }
            ix[lvl] = tok_start;
            *(tok_start + arg_flen) = 0;
            ixlen[lvl] = arg_flen;
            lvl++;
        }
    }

    if( lvl > 0 ) {
                                // get pageno and ixref string from dictionary
        find_symvar( &sys_dict, "$page", no_subscript, &pageval);
        find_symvar( &sys_dict, "$ixref", no_subscript, &ixrefval);

        k = 0;
        ixhprev = &index_dict;
        for( k = 0; k < lvl; ++k ) {
            do_nothing = false;
            while( *ixhprev != NULL ) { // find alfabetic point to insert
                comp_len = min( ixlen[k], (*ixhprev)->len ) + 1;
                comp_res = strnicmp( ix[k], (*ixhprev)->text, comp_len );
                if( comp_res > 0 ) {
                    ixhprev = &((*ixhprev)->next);
                    continue;
                }
                if( comp_res == 0 ) {
                    if( ixlen[k] == (*ixhprev)->len ) {
                        do_nothing = true;
                        break;          // entry already there
                    }
                    if( ixlen[k] > (*ixhprev)->len ) {
                        ixhprev = &((*ixhprev)->next);
                        continue;       // new is longer
                    }
                }
                break;
            }
            if( !do_nothing ) {
                // insert point reached
                ixhwk = mem_alloc( sizeof( ix_h_blk ) + ixlen[k]  );
                ixhwk->next  = *ixhprev;
                ixhwk->lower = NULL;
                ixhwk->entry = NULL;
                ixhwk->len   = ixlen[k];
                strcpy_s( ixhwk->text, ixlen[k] + 1, ix[k] );
                if( *ixhprev == NULL ) {
                    *ixhprev = ixhwk;
                } else {
                    *ixhprev    = ixhwk;
                }
                if( k < lvl ) {
                    ixhprev =&(ixhwk->lower);
                }
            } else {
                ixhwk = *ixhprev;
                if( k < lvl ) {
                    ixhprev = &((*ixhprev)->lower); // next lower level
                }
            }
        }


        currlen = strlen( pageval->value ) + strlen( ixrefval->value );
        if( ixhwk->entry == NULL ) {
            ixewk = mem_alloc( sizeof( ix_e_blk ) );

            ixewk->next    = NULL;
            ixewk->freelen = reflen - currlen;
            strcpy_s( ixewk->refs, reflen, ixrefval->value );
            strcat_s( ixewk->refs, reflen, pageval->value );

            ixhwk->entry   = ixewk;
        } else {
            ixewk = ixhwk->entry;
            if( currlen < ixewk->freelen ) {
                strcat_s( ixewk->refs, reflen, ixrefval->value );
                strcat_s( ixewk->refs, reflen, pageval->value );
                ixewk->freelen -= currlen;
            } else {
                ixewkprev = ixewk;
                while( (ixewk = ixewk->next) != NULL ) {
                    ixewkprev = ixewk;
                    if( currlen < ixewk->freelen ) {
                        strcat_s( ixewk->refs, reflen, ixrefval->value );
                        strcat_s( ixewk->refs, reflen, pageval->value );
                        ixewk->freelen -= currlen;
                        currlen = 0;    // complete
                        break;
                    }
                }
                if( currlen > 0 ) {     // no space need new block
                   ixewk = mem_alloc( sizeof( ix_e_blk ) );

                   ixewk->next = NULL;
                   ixewk->freelen = reflen - currlen;
                   strcpy_s( ixewk->refs, reflen, ixrefval->value );
                   strcat_s( ixewk->refs, reflen, pageval->value );

                   ixewkprev->next = ixewk;
                }
            }
        }
    }
    return;
}


/***************************************************************************/
/*  free all ix_e_blk s belonging to one ix_h_blk                          */
/***************************************************************************/

static  void    free_ix_entries( ix_e_blk * e )
{
    ix_e_blk    *   ewk;
    ix_e_blk    *   ew  = e;

    while( ew != NULL ) {
        ewk = ew->next;
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

    ixh = * dict;
    *dict = NULL;
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
}


