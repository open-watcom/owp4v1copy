/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML tags :H0 :H1 :H2 :H3 :H4 :H5 :H6 processing
*
*                         stitle attribute not implemented
*                         other   options TBD
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static char hnumx[7] = "$hnumX";


/***************************************************************************/
/*  construct Header numbers  1.2.3.V ...                                  */
/*  and update $headn global variable                                      */
/***************************************************************************/

static  void    update_headnumx( int level, char * hnumstr, size_t hnsize )
{
    size_t          pos;
    char          * pn;
    int             rc;

    *hnumstr = 0;
    pos = 0;
    if( layout_work.hx[level].number_form == none ) {
        return;                         // no number output
    }

    if( level > 0 ) {           // reuse formatted number from previous level
       if( (layout_work.hx[level].number_form == num_prop) &&
           (layout_work.hx[level - 1].headnsub != NULL) ) {

           strcpy( hnumstr, layout_work.hx[level - 1].headnsub->value );
           pos = strlen( hnumstr );
       }
    }
    if( pos > 0 ) {             // we have formatted number from previous lvl
       *(hnumstr + pos) = layout_work.heading.delim;
       pos++;
       *(hnumstr + pos) = 0;
    }
    pn = format_num( layout_work.hx[level].headn, hnumstr + pos, hnsize - pos,
                     layout_work.hx[level].number_style );
    if( pn != NULL ) {
         pos += strlen( pn );           // all ok
    } else {
         pn = hnumstr + pos;
         *pn = '?';                     // dummy number
         *(pn + 1) = 0;
         pos++;
    }
    if( layout_work.hx[level].headnsub == NULL ) {  // first time here
        rc = add_symvar_addr( &global_dict, hnumx, hnumstr, no_subscript, 0,
                              &layout_work.hx[level].headnsub );
    } else {
        if( strlen( layout_work.hx[level].headnsub->value )
            < strlen( hnumstr ) ) {     // need more room
            layout_work.hx[level].headnsub->value =
                mem_realloc( layout_work.hx[level].headnsub->value,
                             strlen( hnumstr ) + 1 );
        }
        strcpy_s( layout_work.hx[level].headnsub->value, strlen( hnumstr ) + 1,
                  hnumstr );
    }
}


/***************************************************************************/
/*  :H0 - :H6  common processing                                           */
/***************************************************************************/
static  void    gml_hx_common( const gmltag * entry, int hx_lvl )
{
    char    *   p;
    char    *   pa;
    char    *   headp;
    char        quote;
    bool        idseen;
    bool        stitleseen;
    int         rc;
    size_t      headlen;
    char        hnumstr[64];
    static char hxstring[4] = ":HX";
    static char htextx[8] = "$htextX";
    static char headx[7]  = "$headX";

    switch( hx_lvl ) {
    case   0:
        if( !((ProcFlags.doc_sect == doc_sect_body) ||
            (ProcFlags.doc_sect_nxt == doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, ":H0", ":BODY section" );
            err_count++;
            file_mac_info();
        }
        htextx[6] = '0';
        hnumx[5] = '0';
        headx[5] = '0';
        break;
    case  1:
        if( !((ProcFlags.doc_sect >= doc_sect_body) ||
            (ProcFlags.doc_sect_nxt >= doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, ":H1", ":BODY :APPENDIX :BACKM sections" );
            err_count++;
            file_mac_info();
        }
        htextx[6] = '1';
        hnumx[5] = '1';
        headx[5] = '1';
        break;
    default:
        *(hxstring + 2) = '0' + hx_lvl;
        if( !((ProcFlags.doc_sect >= doc_sect_abstract) ||
            (ProcFlags.doc_sect_nxt >= doc_sect_abstract)) ) {

            g_err( err_tag_wrong_sect, hxstring, ":ABSTRACT section or later" );
            err_count++;
            file_mac_info();
        }
        htextx[6] = '0' + hx_lvl;
        hnumx[5] = '0' + hx_lvl;
        headx[5] = '0' + hx_lvl;
        break;
    }
    if( layout_work.hx[hx_lvl].number_form != num_none ) {
        layout_work.hx[hx_lvl].headn++;
    }

    idseen = false;
    stitleseen = false;
    p = scan_start;

    /***********************************************************************/
    /*  Scan attributes  for :Hx                                           */
    /*  id=                                                                */
    /*  stitle=                                                            */
    /***********************************************************************/

    for( ;; ) {
        while( *p == ' ' ) {
            p++;
        }
        if( *p == '\0' || *p == '.'  ) {
            break;                      // tag end found
        }
        if( !strnicmp( "stitle=", p, 7 ) ) {
            p += 7;
            stitleseen = true;

            /***************************************************************/
            /*  Although unsupported scan stitle='xxx'                     */
            /***************************************************************/
            g_warn( wng_unsupp_att, "stitle" );
            wng_count++;
            file_mac_info();

            while( *p == ' ' ) {
                p++;
            }
            quote = *p;
            if( is_quote_char( *p ) ) {
                p++;
            }
            pa = p;
            while( *p && (*p != quote) ) {
                p++;
            }
#if 0
            len = p - pa;
            c = *p;
            *p = '\0';

            *p = c;
#endif
            if( *p == quote ) {
                p++;
            }
            scan_start = p;
            continue;
        }

        if( !strnicmp( "id=", p, 3 ) ) {
            p += 3;
            idseen = true;

            /***************************************************************/
            /*  Although currently unsupported scan id='xxx'        TBD    */
            /***************************************************************/
            g_warn( wng_not_yet_supp_att, "id" );   // TBD
            wng_count++;
            file_mac_info();

            while( *p == ' ' ) {
                p++;
            }
            quote = *p;
            if( is_quote_char( *p ) ) {
                p++;
            }
            pa = p;
            while( *p && (*p != quote) ) {
                p++;
            }
#if 0
            len = min( 7, p - pa );     // restrict length
            pe = pa + len;
            c = *pe;
            *pe = '\0';
                                        // processing TBD

            *pe = c;
#endif
            if( *p == quote ) {
                p++;
            }

            scan_start = p;
            continue;
        }

        /*******************************************************************/
        /* no more valid attributes, process remaining input as header text*/
        /*******************************************************************/
        break;
    }
    if( *p == '.' ) {                   // tag end ?
        p++;
    }
/******************************************************************************/
/*   Now set the global vars $headx, $headnumx, $htextx                       */
/*                                                                            */
/******************************************************************************/
    if( *p ) {                          // text line follows
        while( *p == ' ' ) {            // ignore leading blanks
            p++;
        }
        rc = add_symvar( &global_dict, htextx, p, no_subscript, 0 );
    } else {                            // empty heading text
        rc = add_symvar( &global_dict, htextx, p, no_subscript, 0 );
    }
    update_headnumx( hx_lvl, &hnumstr, sizeof( hnumstr ) );

    headlen = strlen( hnumstr) + strlen( p ) + 2;
    headp = mem_alloc( headlen );
    if( layout_work.hx[hx_lvl].number_form != num_none ) {
        strcpy_s( headp, headlen, hnumstr); // numbered header
        strcat_s( headp, headlen, " " );
    } else {
        *headp = '\0';
    }
    strcat_s( headp, headlen, p );
    rc = add_symvar( &global_dict, headx, headp, no_subscript, 0 );
    mem_free( headp );

    scan_start = scan_stop + 1;
    return;
}


/******************************************************************************/
/*H0, H1, H2, H3, H4, H5, H6                                                  */
/*                                                                            */
/*Format: :Hn [id='id-name']                                                  */
/*            [stitle='character string'].<text line>                         */
/*        (n=0,1)                                                             */
/*                                                                            */
/*Format: :Hn [id='id-name'].<text line>                                      */
/*        (n=0,1,2,3,4,5,6)                                                   */
/*                                                                            */
/*These tags are used to create headings for sections and subsections of text.*/
/*A common convention uses the headings as follows:                           */
/*                                                                            */
/*    :H0 Major part of document.                                             */
/*    :H1 Chapter.                                                            */
/*    :H2 Section.                                                            */
/*    :H3, :H4, :H5, :H6 Subsections.                                         */
/*                                                                            */
/*The specific layout with which a document is formatted will determine the   */
/*format of the headings. Some layouts cause the headings to be automatically */
/*numbered according to a chosen convention. The heading text specified with  */
/*the tag may also be used in the creation of top and/or bottom page banners. */
/*                                                                            */
/*A heading may be used where a basic document element is permitted to appear,*/
/*with the following restrictions:                                            */
/*                                                                            */
/*    1. :h0 tags may only be used in the body of a document.                 */
/*    2. :h1 tags may not be used in the preface or the abstract.             */
/*                                                                            */
/*The stitle attribute allows you to specify a short title for the heading.   */
/*                                                                            */
/*The short title will be used instead of the heading text when creating the  */
/*top and/or bottom page banners. The short title attribute is valid with a   */
/*level one or level zero heading.                                            */
/*                                                                            */
/*The id attribute assigns an identifier name to the heading. The identifier  */
/*name is used when processing a heading reference, and must be unique within */
/*the document.                                                               */
/******************************************************************************/


void    gml_h0( const gmltag * entry )
{
    gml_hx_common( entry, 0 );
}

void    gml_h1( const gmltag * entry )
{
    gml_hx_common( entry, 1 );
}

void    gml_h2( const gmltag * entry )
{
    gml_hx_common( entry, 2 );
}

void    gml_h3( const gmltag * entry )
{
    gml_hx_common( entry, 3 );
}

void    gml_h4( const gmltag * entry )
{
    gml_hx_common( entry, 4 );
}

void    gml_h5( const gmltag * entry )
{
    gml_hx_common( entry, 5 );
}

void    gml_h6( const gmltag * entry )
{
    gml_hx_common( entry, 6 );
}

