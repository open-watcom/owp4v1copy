/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2011 The Open Watcom Contributors. All Rights Reserved.
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
*                  stitle is not implemented: not used in OW documentation
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static char hnumx[7] = "$hnumX";

/***************************************************************************/
/*  construct Header numbers  1.2.3.V ...                                  */
/*  and update $headn global variable                                      */
/***************************************************************************/

static void update_headnumx( int lvl, char *hnumstr, size_t hnsize )
{
    size_t          pos;
    char          * pn;
    int             rc;

    *hnumstr = 0;
    pos = 0;
    if( layout_work.hx[lvl].number_form == none ) {
        return;                         // no number output
    }

    if( lvl > 0 ) {               // reuse formatted number from previous lvl
       if( (layout_work.hx[lvl].number_form == num_prop) &&
           (layout_work.hx[lvl - 1].headnsub != NULL) ) {

           strcpy( hnumstr, layout_work.hx[lvl - 1].headnsub->value );
           pos = strlen( hnumstr );
       }
    }
    if( pos > 0 ) {             // we have a formatted number from parent lvl
       *(hnumstr + pos) = layout_work.heading.delim;
       pos++;
       *(hnumstr + pos) = 0;
    }
    pn = format_num( layout_work.hx[lvl].headn, hnumstr + pos, hnsize - pos,
                     layout_work.hx[lvl].number_style );
    if( pn != NULL ) {
         pos += strlen( pn );           // all ok
    } else {
         pn = hnumstr + pos;
         *pn = '?';                     // create dummy number
         *(pn + 1) = 0;
         pos++;
    }

    if( layout_work.hx[lvl].headnsub == NULL ) {// first time here
        rc = add_symvar_addr( &global_dict, hnumx, hnumstr, no_subscript, 0,
                              &layout_work.hx[lvl].headnsub );
    } else {
        if( strlen( layout_work.hx[lvl].headnsub->value ) < strlen( hnumstr ) ) {     // need more room
            layout_work.hx[lvl].headnsub->value =
                mem_realloc( layout_work.hx[lvl].headnsub->value, strlen( hnumstr ) + 1 );
        }
        strcpy_s( layout_work.hx[lvl].headnsub->value, strlen( hnumstr ) + 1, hnumstr );
    }
}


/***************************************************************************/
/*  output hx Header  only  called if display_heading = yes                */
/***************************************************************************/

static void hx_header( su * p_sk, su * top_sk, font_number n_font, font_number t_font,
                       int8_t spc, int hx_lvl, const char * hnumstr, const char * txt,
                       hdsrc src )
{
    spacing = spc;

    if( src == hs_hn ) {             // from an Hn tag
        if( layout_work.hx[hx_lvl].line_break ) {
            set_skip_vars( &layout_work.hx[hx_lvl].pre_skip, top_sk, p_sk,
                           spacing, n_font );
        } else {
            set_skip_vars( &layout_work.hx[hx_lvl].pre_skip, top_sk, NULL,
                           spacing, n_font );
        }
    } else {                        // from a section heading
        set_skip_vars( NULL, top_sk, p_sk, spacing, t_font );
    }

    post_space = 0;

    ProcFlags.stop_xspc = true;     // suppress 2nd space after stop

    if( layout_work.hx[hx_lvl].number_form != none ) {
        process_text( hnumstr, n_font );        
    }

    if( (txt != NULL) && (*txt != '\0') ) {
        process_text( txt, t_font );        
    }
}

/***************************************************************************/
/*  :H0 - :H6  common processing                                           */
/***************************************************************************/

static void gml_hx_common( const gmltag * entry, int hx_lvl )
{
    bool            id_seen     = false;
    char        *   headp;
    char            hnumstr[64];
    char            id[ID_LEN];
    char        *   p;
    char        *   pa;
//    group_type      sav_group_type;         // save prior group type
    int             k;
    int             rc;
//    size_t          current;
    size_t          headlen;
//    size_t          txtlen;
//    ref_entry   *   cur_ref;
//    uint32_t        hx_depth;

    static char     headx[7]    = "$headX";
    static char     htextx[8]   = "$htextX";
    static char     hxstr[4]    = ":HX";

    *(hxstr + 2) = '0' + hx_lvl;
    htextx[6] = '0' + hx_lvl;
    hnumx[5] = '0' + hx_lvl;
    headx[5] = '0' + hx_lvl;
    id[0] = '\0';                           // null string if no id found

    switch( hx_lvl ) {
    case   0:
        if( !((ProcFlags.doc_sect == doc_sect_body) ||
            (ProcFlags.doc_sect_nxt == doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, hxstr, ":BODY section" );
            err_count++;
            file_mac_info();
        }
        break;
    case  1:
        if( !((ProcFlags.doc_sect >= doc_sect_body) ||
            (ProcFlags.doc_sect_nxt >= doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, hxstr, ":BODY :APPENDIX :BACKM sections" );
            err_count++;
            file_mac_info();
        }
        break;
    default:
        if( !((ProcFlags.doc_sect >= doc_sect_abstract) ||
            (ProcFlags.doc_sect_nxt >= doc_sect_abstract)) ) {

            g_err( err_tag_wrong_sect, hxstr, ":ABSTRACT section or later" );
            err_count++;
            file_mac_info();
        }
        break;
    }
    if( layout_work.hx[hx_lvl].number_form != num_none ) {
        layout_work.hx[hx_lvl].headn++;
    }

    p = scan_start;
    if( *p == '.' ) {
        /* already at tag end */
    } else {
        for( ;; ) {
            pa = get_att_start( p );
            p = att_start;
            if( ProcFlags.reprocess_line ) {
                break;
            }
            if( !strnicmp( "id", p, 2 ) ) {
                p += 2;
                p = get_refid_value( p, id );
                if( val_start == NULL ) {
                    break;
                }
                id_seen = true;             // valid id attribute found
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else if( !strnicmp( "stitle", p, 6 ) ) {
                p += 6;
                p = get_att_value( p );
                if( val_start == NULL ) {
                    break;
                }
                g_warn( wng_unsupp_att, "stitle" );
                wng_count++;
                file_mac_info();
                if( ProcFlags.tag_end_found ) {
                    break;
                }
            } else {    // no match = end-of-tag in wgml 4.0
                p = pa; // restore spaces before text
                break;
            }
        }
    }

    /************************************************************************/
    /*  set the global vars $headx, $headnumx, $htextx                      */
    /*    perhaps text translated to upper or lower case                    */
    /************************************************************************/

    if( *p == '.' ) p++;                // possible tag end
    while( *p == ' ' ) p++;             // skip initial spaces
    if( *p ) {                          // text exists
        if( layout_work.hx[hx_lvl].cases == case_lower ) {
            strlwr( p );
        } else if( layout_work.hx[hx_lvl].cases == case_upper ) {
            strupr( p );
        }
    }
    rc = add_symvar( &global_dict, htextx, p, no_subscript, 0 );

    update_headnumx( hx_lvl, hnumstr, sizeof( hnumstr ) );  // sets $headnumx

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

    if( layout_work.hx[hx_lvl].number_reset ) {
        for( k = hx_lvl + 1; k < 7; k++ ) {
            layout_work.hx[k].headn = 0;// reset following levels
            if( layout_work.hx[k].headnsub != NULL ) {
                *(layout_work.hx[k].headnsub->value) = '\0';
            }
        }
    }

    /*  eject page(s) if specified                                         */

    if( layout_work.hx[hx_lvl].page_eject != ej_no ) {

        if( ProcFlags.page_started ) {
            do_page_out();
            reset_t_page();
        }

        if( !ProcFlags.start_section ) {
            start_doc_sect();
        }
        set_headx_banners( hx_lvl );        // set possible banners
        reset_t_page();                     // and adjust page margins

        if( (layout_work.hx[hx_lvl].page_eject == ej_odd) && (page & 1) ) {
            do_page_out();              // next page would be even
            reset_t_page();
        } else if( (layout_work.hx[hx_lvl].page_eject == ej_even) && !(page & 1) ) {
            do_page_out();              // next page would be odd
            reset_t_page();
        }
    }

    /***********************************************************************/
    /*  creation of actual heading                                         */
    /***********************************************************************/

    gen_heading( &layout_work.hx[hx_lvl].post_skip,
                 &layout_work.hx[hx_lvl].pre_top_skip, 
                 layout_work.hx[hx_lvl].number_font, layout_work.hx[hx_lvl].font,
                 layout_work.hx[hx_lvl].page_eject, spacing, hnumstr, p, hx_lvl, id,
                 hs_hn );

    scan_start = scan_stop + 1;
    return;
}

/******************************************************************************/
/* generate the heading -- used not only with the Hn tags but also with those */
/* document sections which have headings                                      */
/******************************************************************************/

void gen_heading( su * p_sk, su * top_sk, font_number n_font, font_number t_font,
                  int8_t spc, page_ej page_e, char * hnumstr, char * p,
                  int hx_lvl, char * id, hdsrc src )
{
    group_type      sav_group_type;         // save prior group type
    size_t          current;
    size_t          txtlen;
    ref_entry   *   cur_ref;
    uint32_t        hx_depth;

    scr_process_break();                    // commit any prior text 

    txtlen = strlen( p );

    /* Only create the entry on the first pass */

    if( pass == 1 ) {                   // add this Hn to hd_list
        hd_entry = init_ffh_entry( hd_list );
        hd_entry->flags = ffh_hn;       // mark as Hn
        hd_entry->number = hx_lvl;      // add heading level
        if( hd_list == NULL ) {         // first entry
            hd_list = hd_entry;
        }

        /****************************************************************/
        /* For a section heading, hnumstr will be NULL                  */
        /* For H0, hnumstr will not be NULL, but the first byte will be */
        /* 0x00                                                         */
        /****************************************************************/

        if( (hnumstr != NULL) && hnumstr[0] ) {
            current = strlen( hnumstr );
            hd_entry->prefix = (char *) mem_alloc( current + 1 );
            strcpy_s(hd_entry->prefix, current + 1, hnumstr );
        }
        if( txtlen > 0 ) {              // text line not empty
            hd_entry->text = mem_alloc( txtlen + 1 );
            strcpy_s( hd_entry->text, txtlen + 1, p );
        }
        if( id != NULL ) {              // add this entry to fig_ref_dict
            cur_ref = find_refid( hd_ref_dict, id );
            if( cur_ref == NULL ) {             // new entry
                cur_ref = mem_alloc( sizeof( ref_entry ) );
                init_ref_entry( cur_ref, id );
                cur_ref->flags = rf_ffh;
                cur_ref->entry = hd_entry;
                add_ref_entry( &hd_ref_dict, cur_ref );
            } else {                // duplicate id
                dup_id_err( cur_ref->id, "heading" );
            }
        }
    }

    /***********************************************************************/
    /* If a page is ejected above, then wgml 4.0 forms a block of the      */
    /* entire heading and (apparently) puts it into the full_page section  */
    /* of the output page                                                  */
    /* If a page is not ejected above, then wgml 4.0 forms a block of the  */
    /* entire heading plus any post_skip and space for one text line       */
    /* This requires our wgml to put the heading into a block and enhance  */
    /* its depth when deciding whether or not to move to the next page     */
    /* Note: heading text is wraps normally both when displayed in the     */
    /* document and in the TOC; even in the first case above, the block    */
    /* may contain more than one line                                      */
    /***********************************************************************/

    sav_group_type = cur_group_type;
    cur_group_type = gt_hx;
    cur_doc_el_group = alloc_doc_el_group( gt_hx );
    cur_doc_el_group->prev = t_doc_el_group;
    t_doc_el_group = cur_doc_el_group;
    cur_doc_el_group = NULL;

    if( layout_work.hx[hx_lvl].display_heading ) {
        hx_header( p_sk, top_sk, n_font, t_font, spc, hx_lvl, hnumstr, p, src );
        scr_process_break();                    // commit the header
    }

    cur_group_type = sav_group_type;
    if( t_doc_el_group != NULL) {
        cur_doc_el_group = t_doc_el_group;      // detach current element group
        t_doc_el_group = t_doc_el_group->prev;  // processed doc_elements go to next group, if any
        cur_doc_el_group->prev = NULL;

        if( page_e == ej_no ) {
            hx_depth = cur_doc_el_group->depth + wgml_fonts[layout_work.hx[hx_lvl].font].line_height + g_post_skip;
            if( (hx_depth + t_page.cur_depth) > t_page.max_depth ) {

                /* the block won't fit on this page */

                if( hx_depth <= t_page.max_depth ) {

                    /* the block will be on the next page */

                    do_page_out();
                    reset_t_page();
                }
            }

            while( cur_doc_el_group->first != NULL ) {
                insert_col_main( cur_doc_el_group->first );
                cur_doc_el_group->first = cur_doc_el_group->first->next;
            }
        } else {
            while( cur_doc_el_group->first != NULL ) {
                insert_page_width( cur_doc_el_group->first );
                cur_doc_el_group->first = cur_doc_el_group->first->next;
            }
        }

        if( pass == 1 ) {                        // only on first pass
            hd_entry->pageno = page + 1;
        }
        add_doc_el_group_to_pool( cur_doc_el_group );
        cur_doc_el_group = NULL;
    } else if( pass == 1 ) {                        // only on first pass
        hd_entry->pageno = page + 1;
    }
    if( pass > 1 ) {                    // not on first pass
        if( (page + 1) != hd_entry->pageno ) {  // page number changed
            hd_entry->pageno = page + 1;
            hd_fwd_refs = init_fwd_ref( hd_fwd_refs, id );
        }
        hd_entry = hd_entry->next;      // get to next Hn
    }
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

void gml_h0( const gmltag * entry )
{
    gml_hx_common( entry, 0 );
}

void gml_h1( const gmltag * entry )
{
    gml_hx_common( entry, 1 );
}

void gml_h2( const gmltag * entry )
{
    gml_hx_common( entry, 2 );
}

void gml_h3( const gmltag * entry )
{
    gml_hx_common( entry, 3 );
}

void gml_h4( const gmltag * entry )
{
    gml_hx_common( entry, 4 );
}

void gml_h5( const gmltag * entry )
{
    gml_hx_common( entry, 5 );
}

void gml_h6( const gmltag * entry )
{
    gml_hx_common( entry, 6 );
}

