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

/***************************************************************************/
/*  extern array hd_nums[] will be accessed using hn_lvl                   */
/*  the layout arrays hx_head and hx_sect will be accessed using hds_lvl   */
/*  this allows hds_appendix values to set heading numbers for hn_lvl "1"  */
/***************************************************************************/

#include    "wgml.h"
#include    "gvars.h"

/***************************************************************************/
/*  construct Header numbers  1.2.3.V ...                                  */
/*  and update $headn global variable                                      */
/***************************************************************************/

static void update_headnumx( char *hnumstr, size_t hnsize, hdsrc hn_lvl, hdsrc hds_lvl )
{
    size_t          pos;
    char          * pn;
    int             rc;

    static char hnumx[7] = "$hnumX";

    hnumx[5] = '0' + hn_lvl;

    *hnumstr = 0;
    pos = 0;
    if( layout_work.hx.hx_head[hds_lvl].number_form == none ) {
        return;                         // no number output
    }

    if( hn_lvl > 0 ) {              // reuse formatted number from previous lvl
        if( (layout_work.hx.hx_head[hds_lvl].number_form == num_prop) &&
                (hd_nums[hn_lvl - 1].headnsub != NULL) ) {
            strcpy( hnumstr, hd_nums[hn_lvl - 1].headnsub->value );
            pos = strlen( hnumstr );
        }
    }
    if( *(hnumstr + pos - 1) != layout_work.heading.delim ) {   // if not already delimited
        if( pos > 0 ) {             // we have a formatted number from parent lvl
           *(hnumstr + pos) = layout_work.heading.delim;
           pos++;
           *(hnumstr + pos) = 0;
        }
    }
    pn = format_num( hd_nums[hn_lvl].headn, hnumstr + pos, hnsize - pos,
                                    layout_work.hx.hx_head[hds_lvl].number_style );
    if( pn != NULL ) {
         pos += strlen( pn );           // all ok
    } else {
         pn = hnumstr + pos;
         *pn = '?';                     // create dummy number
         *(pn + 1) = 0;
         pos++;
    }

    if( hd_nums[hn_lvl].headnsub == NULL ) {// first time here
        rc = add_symvar_addr( &global_dict, hnumx, hnumstr, no_subscript, 0,
                              &hd_nums[hn_lvl].headnsub );
    } else {
        if( strlen( hd_nums[hn_lvl].headnsub->value ) < strlen( hnumstr ) ) {     // need more room
            hd_nums[hn_lvl].headnsub->value =
                mem_realloc( hd_nums[hn_lvl].headnsub->value, strlen( hnumstr ) + 1 );
        }
        strcpy_s( hd_nums[hn_lvl].headnsub->value, strlen( hnumstr ) + 1, hnumstr );
    }
}


/***************************************************************************/
/*  output hx Header  only  called if display_heading = yes                */
/***************************************************************************/

static void hx_header( char * h_num, char * h_text, hdsrc hn_lvl, hdsrc hds_lvl )
{
    spacing = layout_work.hx.hx_sect[hds_lvl].spacing;

    /* text_font is used for setting the skips */

    if( hds_lvl < hds_appendix ) {              // from an Hn tag
        if( layout_work.hx.hx_head[hds_lvl].line_break ) {
            set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                           &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                           &layout_work.hx.hx_sect[hds_lvl].post_skip,
                           layout_work.hx.hx_sect[hds_lvl].spacing,
                           layout_work.hx.hx_sect[hds_lvl].text_font );
            if( g_post_skip == 0 ) {            // to match wgml 4.0
                if( spacing == 1 ) {
                    if( nest_cb->prev == NULL ) {
                        g_post_skip = wgml_fonts[nest_cb->font].line_height;
                    } else {
                        g_post_skip = wgml_fonts[nest_cb->prev->font].line_height;
                    }

                } else {
                    if( layout_work.hx.hx_sect[hds_lvl].text_font >=
                            layout_work.hx.hx_head[hds_lvl].number_font ) {
                        g_post_skip = wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].line_height;
                    } else {
                        g_post_skip = wgml_fonts[layout_work.hx.hx_head[hds_lvl].number_font].line_height;
                    }
                    g_post_skip *= (spacing - 1);
                }
            }
        } else {
            set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                           &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                           NULL,
                           layout_work.hx.hx_sect[hds_lvl].spacing,
                           layout_work.hx.hx_sect[hds_lvl].text_font );
        }
    } else {                                    // from a section heading
        if( hds_lvl == hds_appendix ) {
            g_post_skip = 0;
        }
        set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                       &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                       &layout_work.hx.hx_sect[hds_lvl].post_skip,
                       layout_work.hx.hx_sect[hds_lvl].spacing,
                       layout_work.hx.hx_sect[hds_lvl].text_font );
    }

    post_space = 0;

    ProcFlags.stop_xspc = true;     // suppress 2nd space after stop

    if( (hds_lvl < hds_appendix) && (layout_work.hx.hx_head[hds_lvl].number_form != none) ||
            (hds_lvl == hds_appendix) && (layout_work.hx.hx_head[hds_appendix].number_form != none) ) {
        process_text( h_num, layout_work.hx.hx_head[hds_lvl].number_font );        
        post_space /= wgml_fonts[layout_work.hx.hx_head[hds_lvl].number_font].spc_width;     // rescale post_space to correct font
        post_space *= wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].spc_width;
    }

    if( (h_text != NULL) && (*h_text != '\0') ) {
        process_text( h_text, layout_work.hx.hx_sect[hds_lvl].text_font );        
    }
}

/******************************************************************************/
/* generate the heading -- used not only with the Hn tags but also with those */
/* document sections which have headings                                      */
/******************************************************************************/

void gen_heading( char * h_text, char * id, hdsrc hn_lvl, hdsrc hds_lvl )
{
    char            hnumstr[64];
    char        *   headp;
    char        *   h_num;
    char        *   prefix      = NULL;
    doc_element *   cur_el;
    group_type      sav_group_type;         // save prior group type
    int             k;
    int             rc;
    size_t          current;
    size_t          headlen;
    size_t          prefixlen;
    size_t          txtlen;
    ref_entry   *   cur_ref;
    uint32_t        hx_depth;

    static char     headx[7]    = "$headX";
    static char     htextx[8]   = "$htextX";

    headx[5] = '0' + hn_lvl;
    htextx[6] = '0' + hn_lvl;

    h_num = NULL;   // where to set? how to set?

    /************************************************************************/
    /*  set the global vars $headx, $headnumx, $htextx                      */
    /*    perhaps text translated to upper or lower case                    */
    /************************************************************************/

    if( *h_text ) {                     // text exists
        if( layout_work.hx.hx_head[hds_lvl].hd_case == case_lower ) {
            strlwr( h_text );
        } else if( layout_work.hx.hx_head[hds_lvl].hd_case == case_upper ) {
            strupr( h_text );
        }
    }
    rc = add_symvar( &global_dict, htextx, h_text, no_subscript, 0 );

    update_headnumx( hnumstr, sizeof( hnumstr ), hn_lvl, hds_lvl );  // sets $headnumx

/// this will require a "prefix" pointer that is null except for APPENDIX
    if( (ProcFlags.doc_sect == doc_sect_appendix) && (hn_lvl == hds_h1) ) {
        prefixlen = strlen( layout_work.appendix.string ) + strlen( hnumstr );
        prefix = (char *) mem_alloc( prefixlen + 1 );
        strcpy_s( prefix, prefixlen, layout_work.appendix.string); // prefix
        strcat_s( prefix, prefixlen + 1, hnumstr ); // numbered header
        headlen = prefixlen + strlen( h_text ) + 2;
        headp = (char *) mem_alloc( headlen );
        if( layout_work.hx.hx_head[hds_appendix].number_form != num_none ) {
            strcpy_s( headp, headlen, prefix ); // numbered header
            strcat_s( headp, headlen, " " );
        } else {
            *headp = '\0';
        }
    } else {
        headlen = strlen( hnumstr ) + strlen( h_text ) + 2;
        headp = (char *) mem_alloc( headlen );
        if( layout_work.hx.hx_head[hds_lvl].number_form != num_none ) {
            strcpy_s( headp, headlen, hnumstr ); // numbered header
            strcat_s( headp, headlen, " " );
        } else {
            *headp = '\0';
        }
    }
    strcat_s( headp, headlen, h_text );
    rc = add_symvar( &global_dict, headx, headp, no_subscript, 0 );
    mem_free( headp );

    txtlen = strlen( h_text );

    /* Only create the entry on the first pass */

    if( pass == 1 ) {                       // add this Hn to hd_list
        hd_entry = init_ffh_entry( hd_list );
        hd_entry->flags = ffh_hn;           // mark as Hn
        hd_entry->number = hn_lvl;  // add heading level
        if( hd_list == NULL ) {             // first entry
            hd_list = hd_entry;
        }

        /****************************************************************/
        /* For a section heading, hnumstr will be NULL                  */
        /* For H0, hnumstr will not be NULL, but the first byte will be */
        /* 0x00                                                         */
        /* NOTE: this applied to hd_info.h_num, not current h_num       */
        /****************************************************************/

        if( (h_num != NULL) && h_num[0] ) {
            current = strlen( h_num );
            hd_entry->prefix = (char *) mem_alloc( current + 1 );
            strcpy_s(hd_entry->prefix, current + 1, h_num );
        }
        if( txtlen > 0 ) {              // text line not empty
            hd_entry->text = (char *) mem_alloc( txtlen + 1 );
            strcpy_s( hd_entry->text, txtlen + 1, h_text );
        }
        if( id != NULL ) {              // add this entry to fig_ref_dict
            cur_ref = find_refid( hd_ref_dict, id );
            if( cur_ref == NULL ) {             // new entry
                cur_ref = (ref_entry *) mem_alloc( sizeof( ref_entry ) );
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
    /* The positioning of this code was originally in the common Hn        */
    /* function but was moved here to work with section headings as well   */
    /* It always followed the computation of the heading number.           */
    /***********************************************************************/

    if( (layout_work.hx.hx_head[hds_lvl].number_reset) || (hds_lvl > hds_h6) ) {
        for( k = hn_lvl + 1; k < hds_appendix; k++ ) {
            hd_nums[k].headn = 0;// reset following levels
            if( hd_nums[k].headnsub != NULL ) {
                *(hd_nums[k].headnsub->value) = '\0';
            }
        }
    }

    /***********************************************************************/
    /* If a page is ejected above, then wgml 4.0 forms a block of the      */
    /* entire heading and puts it into the full_page section of the output */
    /* page, unless it will not fit on one page, in which case it produces */
    /* an error message                                                    */
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
    cur_doc_el_group->next = t_doc_el_group;
    t_doc_el_group = cur_doc_el_group;
    cur_doc_el_group = NULL;

    /***********************************************************************/
    /* If this is a section heading (hds_lvl > hds_appendix) then this     */
    /* function is only called if the heading is to be displayed: there    */
    /* is no display_heading attribute. Note that APPENDIX has no section  */
    /* heading as such, but instead modifies the H1 headings.              */
    /* Otherwise, this is an Hx tag, possibly an H1 tag in APPENDIX, and   */
    /* the value of display_heading does determine whether or not the      */
    /* heading is displayed.                                               */
    /***********************************************************************/

    if( hds_lvl > hds_appendix ) {
        hx_header( hnumstr, h_text, hn_lvl, hds_lvl );
    } else if( (layout_work.hx.hx_head[hds_lvl].display_heading) ) {
        if( hds_lvl == hds_appendix ) {
            hx_header( prefix, h_text, hn_lvl, hds_lvl );
        } else {
            hx_header( hnumstr, h_text, hn_lvl, hds_lvl );
        }
        scr_process_break();                    // commit the header
    }
    if( prefix != NULL ) {
        mem_free( prefix );
    }

    cur_group_type = sav_group_type;
    if( t_doc_el_group != NULL) {
        cur_doc_el_group = t_doc_el_group;      // detach current element group
        t_doc_el_group = t_doc_el_group->next;  // processed doc_elements go to next group, if any
        cur_doc_el_group->next = NULL;

        if( !ProcFlags.page_ejected ) {
            hx_depth = cur_doc_el_group->depth +
                            wgml_fonts[g_curr_font].line_height +
                            g_post_skip;
            if( (hx_depth + t_page.cur_depth) > t_page.max_depth ) {

                /* the block won't fit on this page */

                if( hx_depth <= t_page.max_depth ) {

                    /* the block will be on the next page */

                    do_page_out();
                    reset_t_page();
                }
            }

            while( cur_doc_el_group->first != NULL ) {
                cur_el = cur_doc_el_group->first;
                cur_doc_el_group->first = cur_doc_el_group->first->next;
                cur_el->next = NULL;
                insert_col_main( cur_el );
            }
            add_doc_el_group_to_pool( cur_doc_el_group );
            cur_doc_el_group = NULL;
        } else {
            hx_depth = cur_doc_el_group->depth +
                            wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].line_height +
                            g_post_skip;
            if( (hx_depth + t_page.cur_depth) > t_page.max_depth ) {
                xx_err( err_heading_too_deep );     /* the block won't fit on this page */
            } else {
                cur_doc_el_group->post_skip = g_post_skip;
                insert_page_width( cur_doc_el_group );
            }
        }
        if( pass == 1 ) {                        // only on first pass
            hd_entry->pageno = page + 1;
        } else {
            if( (page + 1) != hd_entry->pageno ) {  // page number changed
                hd_entry->pageno = page + 1;
                if( GlobalFlags.lastpass ) {
                    if( (id != NULL) && id[0] ) {
                        hd_fwd_refs = init_fwd_ref( hd_fwd_refs, id );
                    }
                    ProcFlags.new_pagenr = true;
                }
            }
        }
    } else if( pass == 1 ) {                        // only on first pass
        hd_entry->pageno = page + 1;
    }
    if( pass > 1 ) {                    // not on first pass
        hd_entry = hd_entry->next;      // get to next Hn
    }

    return;
}

/***************************************************************************/
/*  :H0 - :H6  common processing                                           */
/*  NOTE: this function converts hn_lvl to hds_lvl, if needed              */
/***************************************************************************/

static void gml_hx_common( const gmltag * entry, hdsrc hn_lvl )
{
    bool            id_seen     = false;
    char            id[ID_LEN];
    char        *   p;
    char        *   pa;
    hdsrc           hds_lvl;
    page_pos        old_line_pos;
    uint32_t        sav_spacing;

    static char     hxstr[4]    = ":HX";

    hxstr[2] = '0' + hn_lvl;

    scr_process_break();                    // commit any prior text 
    start_doc_sect();                       // in case not already done

    id[0] = '\0';                           // null string if no id found
    switch( hn_lvl ) {
    case   hds_h0:
        if( !((ProcFlags.doc_sect == doc_sect_body) ||
            (ProcFlags.doc_sect_nxt == doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, hxstr, ":BODY section" );
            err_count++;
            file_mac_info();
        }
        break;
    case  hds_h1:
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

    if( (ProcFlags.doc_sect == doc_sect_appendix) && (hn_lvl == hds_h1) ) {
        hds_lvl = hds_appendix;
    } else {
        hds_lvl = hn_lvl;
    }

    /* From this point on, hds_lvl will access the correct LAYOUT data */

    if( layout_work.hx.hx_head[hds_lvl].number_form != num_none ) {
        hd_nums[hn_lvl].headn++;
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

    /*  eject page(s) if specified                                         */

    if( layout_work.hx.hx_head[hds_lvl].page_eject == ej_no ) {
        ProcFlags.page_ejected = false;
    } else {
        last_page_out();                // ensure we are on a new page
        if( ProcFlags.page_started ) {
            do_page_out();
            reset_t_page();
        }
        if( (layout_work.hx.hx_head[hds_lvl].page_eject == ej_odd) && (page & 1) ) {
            do_page_out();              // next page would be even
            reset_t_page();
        } else if( (layout_work.hx.hx_head[hds_lvl].page_eject == ej_even) && !(page & 1) ) {
            do_page_out();              // next page would be odd
            reset_t_page();
        }
        ProcFlags.page_ejected = true;
        if( hds_lvl < hds_appendix ) {      // Hx only, but not APPENDIX H1
            set_headx_banners( hn_lvl );    // set possible banners
            reset_t_page();                 // and adjust page margins
        }
    }

    /***********************************************************************/
    /*  creation of actual heading                                         */
    /***********************************************************************/

    sav_spacing = spacing;
    if( *p == '.' ) p++;                // possible tag end
    while( *p == ' ' ) p++;             // skip initial spaces
    if( *p ) {                          // text exists
        pa = &p[strlen( p )];
        pa--;
        while( *pa == ' ' ) {
            *pa = '\0';                 // remove trailing spaces
            pa--;
        }
    }

    old_line_pos = line_position;
    line_position = layout_work.hx.hx_head[hds_lvl].line_position;

    gen_heading( p, id, hn_lvl, hds_lvl );

    line_position = old_line_pos;
    spacing = sav_spacing;
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
/*                                                                            */
/*  NOTE: these functions provide the actual heading level (hn_lvl)           */
/******************************************************************************/

void gml_h0( const gmltag * entry )
{
    gml_hx_common( entry, hds_h0 );
}

void gml_h1( const gmltag * entry )
{
    gml_hx_common( entry, hds_h1 );
}

void gml_h2( const gmltag * entry )
{
    gml_hx_common( entry, hds_h2 );
}

void gml_h3( const gmltag * entry )
{
    gml_hx_common( entry, hds_h3 );
}

void gml_h4( const gmltag * entry )
{
    gml_hx_common( entry, hds_h4 );
}

void gml_h5( const gmltag * entry )
{
    gml_hx_common( entry, hds_h5 );
}

void gml_h6( const gmltag * entry )
{
    gml_hx_common( entry, hds_h6 );
}

