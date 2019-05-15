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
*           These tag :Hn attributes are not used in the OW docs and so
*               are not implemented:
*                   align
*                   stitle
*           Only one of the attributes of tag :HEADING is implemented:
*                   delim
****************************************************************************/

/***************************************************************************/
/*  extern array hd_nums[] will be accessed using hn_lvl                   */
/*  the layout arrays hx_head and hx_sect will be accessed using hds_lvl   */
/*  this allows hds_appendix values to set heading numbers for hn_lvl "1"  */
/***************************************************************************/


#include    "wgml.h"


/***************************************************************************/
/*  construct Header numbers  1.2.3.V ...                                  */
/***************************************************************************/

static void update_headnumx( hdsrc hn_lvl, hdsrc hds_lvl )
{
    size_t          pos;
    char          * pn;

    static char hnumx[7] = "$hnumX";

    hnumx[5] = '0' + hn_lvl;

    hd_nums[hn_lvl].hnumstr[0] = '\0';
    pos = 0;
    if( layout_work.hx.hx_head[hds_lvl].number_form == none ) {
        return;                         // no number output
    }

    if( hn_lvl > 0 ) {              // reuse formatted number from previous lvl
        if( (layout_work.hx.hx_head[hds_lvl].number_form == num_prop) &&
                (hd_nums[hn_lvl - 1].hnumsub != NULL) ) {
            strcpy( hd_nums[hn_lvl].hnumstr, hd_nums[hn_lvl - 1].hnumstr );
            pos = strlen( hd_nums[hn_lvl].hnumstr );
        }
    }
    if( *(hd_nums[hn_lvl].hnumstr + pos - 1) != layout_work.heading.delim ) {   // if not already delimited
        if( pos > 0 ) {             // we have a formatted number from parent lvl
           *(hd_nums[hn_lvl].hnumstr + pos) = layout_work.heading.delim;
           pos++;
           *(hd_nums[hn_lvl].hnumstr + pos) = 0;
        }
    }
    pn = format_num( hd_nums[hn_lvl].headn, hd_nums[hn_lvl].hnumstr + pos,
                                    sizeof( hd_nums[hn_lvl].hnumstr ) - pos,
                                    layout_work.hx.hx_head[hds_lvl].number_style );
    if( pn != NULL ) {
         pos += strlen( pn );           // all ok
    } else {
         pn = hd_nums[hn_lvl].hnumstr + pos;
         *pn = '?';                     // create dummy number
         *(pn + 1) = 0;
         pos++;
    }
}


/***************************************************************************/
/*  output the heading                                                     */
/***************************************************************************/

static void hx_header( char * h_num, char * h_text, hdsrc hn_lvl, hdsrc hds_lvl )
{
    spacing = layout_work.hx.hx_sect[hds_lvl].spacing;

    /* text_font is used for setting the skips */

    if( hds_lvl < hds_abstract ) {              // from an Hn tag
        line_position = layout_work.hx.hx_head[hds_lvl].line_position;
        if( layout_work.hx.hx_head[hds_lvl].line_break ) {
            set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                           &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                           &layout_work.hx.hx_sect[hds_lvl].post_skip,
                           layout_work.hx.hx_sect[hds_lvl].spacing,
                           layout_work.hx.hx_sect[hds_lvl].text_font );
        } else {
            set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                           &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                           NULL,
                           layout_work.hx.hx_sect[hds_lvl].spacing,
                           layout_work.hx.hx_sect[hds_lvl].text_font );
        }
    } else {                                    // from a section heading
        line_position = pos_center;
        set_skip_vars( &layout_work.hx.hx_head[hds_lvl].pre_skip,
                       &layout_work.hx.hx_sect[hds_lvl].pre_top_skip,
                       &layout_work.hx.hx_sect[hds_lvl].post_skip,
                       layout_work.hx.hx_sect[hds_lvl].spacing,
                       layout_work.hx.hx_sect[hds_lvl].text_font );
    }

    t_page.cur_width = conv_hor_unit( &layout_work.hx.hx_head[hds_lvl].indent,
                                       layout_work.hx.hx_sect[hds_lvl].text_font);
    post_space = 0;

    /* display_heading is always "true" for section headings */

    if( (hds_lvl > hds_appendix) || layout_work.hx.hx_head[hds_lvl].display_heading ) {
        t_page.cur_left = 0;
        ProcFlags.keep_left_margin = true;  // keep left margin
        if( (hds_lvl < hds_abstract) && (layout_work.hx.hx_head[hds_lvl].number_form != none) ) {
            ProcFlags.as_text_line = true;      // treat as <text_line>
            process_text( h_num, layout_work.hx.hx_head[hds_lvl].number_font );
            post_space /= wgml_fonts[layout_work.hx.hx_head[hds_lvl].number_font].spc_width;     // rescale post_space to correct font
            post_space *= wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].spc_width;
        }

        if( (h_text != NULL) && (*h_text != '\0') ) {
            ProcFlags.as_text_line = true;      // treat as <text_line>
            process_text( h_text, layout_work.hx.hx_sect[hds_lvl].text_font );
        }
    }
}

/******************************************************************************/
/* generate the heading -- used not only with the Hn tags but also with those */
/* document sections which have headings                                      */
/******************************************************************************/

void gen_heading( char * h_text, char * id, hdsrc hn_lvl, hdsrc hds_lvl )
{
    bool            page_width  = false;
    char        *   headp;
    char        *   prefix      = NULL;
    doc_element *   cur_el;
    group_type      sav_group_type;         // save prior group type
    int             k;
    size_t          current;
    size_t          headlen;
    size_t          prefixlen;
    size_t          txtlen;
    page_pos        old_line_pos;
    ref_entry   *   cur_ref;
    uint32_t        bot_depth;
    uint32_t        hx_depth;
    uint32_t        old_bot_depth;
    uint32_t        old_cur_left;
    uint32_t        old_top_depth;
    uint32_t        page_diff;
    uint32_t        top_depth;

    static char     headx[7]    = "$headX";
    static char     htextx[8]   = "$htextX";

    headx[5] = '0' + hn_lvl;
    htextx[6] = '0' + hn_lvl;

    /* Only Hx headings on multi-column pages actually go into the page_width section */

    page_width = (hds_lvl < hds_abstract) && (t_page.last_pane->col_count > 1);
    if( page_width ) {
        t_page.max_width = t_page.page_width;
   }
    update_headnumx( hn_lvl, hds_lvl );

    if( hds_lvl == hds_appendix ) {
        prefixlen = strlen( layout_work.appendix.string ) + strlen( hd_nums[hn_lvl].hnumstr );
        prefix = (char *) mem_alloc( prefixlen + 1 );
        strcpy_s( prefix, prefixlen, layout_work.appendix.string); // prefix
        strcat_s( prefix, prefixlen + 1, hd_nums[hn_lvl].hnumstr ); // numbered header
        headlen = prefixlen + strlen( h_text ) + 2;
        headp = (char *) mem_alloc( headlen );
        if( layout_work.hx.hx_head[hds_appendix].number_form != num_none ) {
            strcpy_s( headp, headlen, prefix ); // numbered header
            strcat_s( headp, headlen, " " );
        } else {
            *headp = '\0';
        }
    } else {
        prefix = hd_nums[hn_lvl].hnumstr;
        headlen = strlen( hd_nums[hn_lvl].hnumstr ) + strlen( h_text ) + 2;
        headp = (char *) mem_alloc( headlen );
        if( (hds_lvl < hds_abstract) &&
                (layout_work.hx.hx_head[hds_lvl].number_form != num_none) ) {
            strcpy_s( headp, headlen, prefix );     // numbered header
            strcat_s( headp, headlen, " " );
        } else {
            *headp = '\0';
        }
    }
    strcat_s( headp, headlen, h_text );

    if( *headp == '\0' ) {              // no text on line
        ProcFlags.need_text = true;
    }

    /* Reset $HEADx */

    if( strlen( hd_nums[hn_lvl].headsub->value ) < strlen( headp ) ) {     // need more room
        hd_nums[hn_lvl].headsub->value =
                mem_realloc( hd_nums[hn_lvl].headsub->value, strlen( headp ) + 1 );
    }
    strcpy_s( hd_nums[hn_lvl].headsub->value, strlen( headp ) + 1, headp );
    mem_free( headp );

    /* Reset $HNUMx */

    if( strlen( hd_nums[hn_lvl].hnumsub->value ) < strlen( prefix ) ) {    // need more room
        hd_nums[hn_lvl].hnumsub->value = mem_realloc( hd_nums[hn_lvl].hnumsub->value,
                                                            strlen( prefix ) + 1 );
    }
    strcpy_s( hd_nums[hn_lvl].hnumsub->value, strlen( prefix ) + 1, prefix );

    /* Reset $HTEXTx */

    if( strlen( hd_nums[hn_lvl].htextsub->value ) < strlen( h_text ) ) {     // need more room
        hd_nums[hn_lvl].htextsub->value =
                mem_realloc( hd_nums[hn_lvl].htextsub->value, strlen( h_text ) + 1 );
    }
    strcpy_s( hd_nums[hn_lvl].htextsub->value, strlen( h_text ) + 1, h_text );

    /* Only create the entry on the first pass */

    if( pass == 1 ) {                       // add this Hn to hd_list
        hd_entry = init_ffh_entry( hd_list );
        if( ((ProcFlags.doc_sect == doc_sect_abstract) ||
             (ProcFlags.doc_sect == doc_sect_preface)) && !ProcFlags.first_hdr ) {
            hd_entry->abs_pre = true;       // first header in ABSTRACT or PREFACE
            ProcFlags.first_hdr = true;
        }
        hd_entry->flags = ffh_hn;           // mark as Hn
        hd_entry->number = hn_lvl;          // add heading level
        if( hd_list == NULL ) {             // first entry
            hd_list = hd_entry;
        }

        /****************************************************************/
        /* The first byte of prefix will be 0x00 if there is no heading */
        /* level number or APPENDIX string to prepend to the text       */
        /****************************************************************/

        if( prefix[0] ) {
            current = strlen( prefix );
            hd_entry->prefix = (char *) mem_alloc( current + 1 );
            strcpy_s(hd_entry->prefix, current + 1, prefix );
        }
        txtlen = strlen( h_text );
        if( txtlen > 0 ) {              // text line not empty
            hd_entry->text = (char *) mem_alloc( txtlen + 1 );
            strcpy_s( hd_entry->text, txtlen + 1, h_text );
        }

        /***********************************************************************/
        /* id is ambiguous, which may be confusing                             */
        /* for Hn, it is a local variable, and starts with '\0' when empty     */
        /* for document sections, it is NULL, as it has no meaning for them    */
        /***********************************************************************/

        if( (id != NULL) && *id ) {             // add this entry to fig_ref_dict
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

    /* Reset heading numbers for Hn tags and APPENDIX H1 tags */

    if( (hds_lvl < hds_abstract) && (layout_work.hx.hx_head[hds_lvl].number_reset) ) {
        for( k = hn_lvl + 1; k < hds_appendix; k++ ) {
            hd_nums[k].headn = 0;// reset following levels
            hd_nums[k].hnumstr[0] = '\0';
            if( hd_nums[k].hnumsub != NULL ) {
                *(hd_nums[k].hnumsub->value) = '\0';
            }
        }
    }

    /***********************************************************************/
    /* wgml 4.0 forms a block of the entire heading text, which wraps      */
    /* normally both when displayed in the document and in the TOC         */
    /* If a page was not ejected above, then the heading is moved to the   */
    /* top of the next page if its depth plus any post_skip and space for  */
    /* one text line will not fit on the current page, provided it will    */
    /* fit on an empty page                                                */
    /* If the heading will not fit on any page, an error message results   */
    /* This requires our wgml to put the heading into a block and enhance  */
    /* its depth when deciding whether or not to move to the next page     */
    /* and whether or not to emit the error message                        */
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

    old_cur_left = t_page.cur_left;
    old_line_pos = line_position;
    hx_header( prefix, h_text, hn_lvl, hds_lvl );

    /************************************************************/
    /* Calling scr_process_break() here forces the header to be */
    /* place on the line following the Hx tag, something that   */
    /* wgml 4.0 does not do. This allows the doc_el_group which */
    /* contains the header to be processed here instead of      */
    /* somewhere else, probably in scr_process_break with a new */
    /* ProcFlags flag to control the processing                 */
    /************************************************************/
    scr_process_break();                    // commit the header
    line_position = old_line_pos;
    t_page.cur_left = old_cur_left;
    if( (prefix != NULL) && (prefix != hd_nums[hn_lvl].hnumstr) ) {
        mem_free( prefix );
    }

    cur_group_type = sav_group_type;
    if( t_doc_el_group != NULL) {
        cur_doc_el_group = t_doc_el_group;      // detach current element group
        t_doc_el_group = t_doc_el_group->next;  // processed doc_elements go to next group, if any
        cur_doc_el_group->next = NULL;

        if( hds_lvl < hds_appendix ) {      // Hx only, but not APPENDIX H1

            set_headx_banners( hn_lvl );    // set possible banners

            if( !ProcFlags.page_ejected ) { // no adjustment if on new page

                /* Get old and new banner depths */

                if( t_page.top_banner != NULL ) {
                    old_top_depth = t_page.top_banner->ban_depth;
                } else {
                    old_top_depth = 0;
                }

                if( t_page.bottom_banner != NULL ) {
                    old_bot_depth = t_page.bottom_banner->ban_depth;
                } else {
                    old_bot_depth = 0;
                }

                if( sect_ban_top[!(page & 1)] != NULL ) {
                    top_depth = sect_ban_top[!(page & 1)]->ban_depth;
                } else {
                    top_depth = 0;
                }

                if( sect_ban_bot[!(page & 1)] != NULL ) {
                    bot_depth = sect_ban_bot[!(page & 1)]->ban_depth;
                } else {
                    bot_depth = 0;
                }

                /***************************************************************/
                /* If the new page will be shorter than the old page and the   */
                /* text already present and/or the current heading would not   */
                /* fit then eject the page and place the heading and the new   */
                /* banners on the the next page. Otherwise, place them on the  */
                /* current page and update t_page.panes_top and                */
                /* t_page.max_depth                                            */
                /***************************************************************/

                if( (top_depth + bot_depth) > (old_top_depth + old_bot_depth) ) {
                    page_diff = (top_depth + bot_depth) -
                                (old_top_depth + old_bot_depth);
                } else {
                    page_diff = 0;
                }
                hx_depth = cur_doc_el_group->depth +
                    wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].line_height +
                    g_post_skip;
                if( ((page_diff + t_page.cur_depth) > t_page.max_depth) ||
                        ((hx_depth + t_page.cur_depth) > t_page.max_depth) ) {
                    next_column();
                    if( t_page.last_pane->cur_col == 0 ) {  // on new page
                        ProcFlags.page_ejected = true;
                    }
                }
                if ( !ProcFlags.page_ejected ) {    // this page will do just fine

                    /* Adjust page vertical metrics */

                    if( top_depth > 0 ) {
                        if( bin_driver->y_positive == 0x00 ) {
                            t_page.panes_top = t_page.page_top - top_depth;
                        } else {
                            t_page.panes_top = t_page.page_top + top_depth;
                        }
                    } else {
                        t_page.panes_top = t_page.page_top;
                    }
                    t_page.panes->page_width_top = t_page.panes_top;

                    if( old_top_depth < top_depth ) {
                        t_page.max_depth -= (top_depth - old_top_depth);
                    } else if( top_depth < old_top_depth ) {
                        t_page.max_depth += (old_top_depth - top_depth);
                    }
                    t_page.top_banner = sect_ban_top[!(page & 1)];

                    if( bot_depth > 0 ) {
                        if( bin_driver->y_positive == 0x00 ) {
                            t_page.bot_ban_top = g_page_bottom_org + bot_depth;
                        } else {
                            t_page.bot_ban_top = g_page_bottom_org - bot_depth;
                        }
                    } else {
                        t_page.bot_ban_top = g_page_bottom_org;
                    }

                    if( old_bot_depth < bot_depth  ) {
                        max_depth -= (bot_depth  - old_bot_depth);
                    } else if( bot_depth  < old_bot_depth ) {
                        max_depth += (old_bot_depth - bot_depth );
                    }
                    t_page.bottom_banner = sect_ban_bot[!(page & 1)];
                }
            }

            if( !ProcFlags.page_ejected ) {
                hx_depth = cur_doc_el_group->depth +
                           wgml_fonts[layout_work.hx.hx_sect[hds_lvl].text_font].line_height +
                           g_post_skip;

                if( (hx_depth + t_page.cur_depth) > t_page.max_depth ) {
                    do_page_out();              // the block won't fit on this page
                    reset_t_page();
                    ProcFlags.page_ejected = true;
                }
            }
        }

        /* display_heading is always "true" for section headings */

        if( (hds_lvl > hds_appendix) || layout_work.hx.hx_head[hds_lvl].display_heading ) {
            if( !ProcFlags.page_ejected ) {
                while( cur_doc_el_group->first != NULL ) {
                    cur_el = cur_doc_el_group->first;
                    cur_doc_el_group->first = cur_doc_el_group->first->next;
                    cur_el->next = NULL;
                    insert_col_main( cur_el );
                }
                add_doc_el_group_to_pool( cur_doc_el_group );
                cur_doc_el_group = NULL;
            } else {                                        // page was ejected
                reset_t_page();                             // update metrics for new banners, if any
                cur_doc_el_group->depth -= cur_doc_el_group->first->subs_skip;  // top of page: no subs_skip
                cur_doc_el_group->depth += cur_doc_el_group->first->top_skip;   // top of page: top_skip
                cur_doc_el_group->first->subs_skip = 0;
                if( cur_doc_el_group->depth > t_page.max_depth ) {
                    xx_err( err_heading_too_deep );     // the block won't fit on any page
                } else {
                    if( page_width ) {

                        /****************************************************/
                        /* this is for multi-column pages where the heading */
                        /* goes to the top of the page                      */
                        /* each column must start at the same vertical      */
                        /* position, whether some of the heading text       */
                        /* above it or not, hence the post_skip is included */
                        /* in the depth -- this appears to match wgml 4.0   */
                        /****************************************************/

                        cur_doc_el_group->post_skip = g_post_skip;
                        insert_page_width( cur_doc_el_group );
                        g_post_skip = 0;
                    } else {
                        insert_col_width( cur_doc_el_group );
                    }
                    cur_doc_el_group = NULL;
                }
            }
        } else {

            /* cur_doc_group will contain a single doc_element with no content at all */

            add_doc_el_group_to_pool( cur_doc_el_group );
            cur_doc_el_group = NULL;
            if( ProcFlags.page_ejected ) {
                reset_t_page();                             // update metrics for new banners, if any
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

    if( page_width ) {
        t_page.max_width = t_page.last_pane->col_width;
    }

    /* Reset $TOPHEADx */

    if( !ProcFlags.tophead_done ) { // first header on page
        if( strlen( t_page.topheadsub->value ) < strlen( h_text ) ) {     // need more room
            t_page.topheadsub->value =
                        mem_realloc( t_page.topheadsub->value, strlen( h_text ) + 1 );
        }
        strcpy_s( t_page.topheadsub->value, strlen( h_text ) + 1, h_text );
        ProcFlags.tophead_done = true;  // will be reset when page output
    }

    /* Reset $BOTHEADx */

    if( strlen( t_page.botheadsub->value ) < strlen( h_text ) ) {     // need more room
        t_page.botheadsub->value =
                        mem_realloc( t_page.botheadsub->value, strlen( h_text ) + 1 );
    }
    strcpy_s( t_page.botheadsub->value, strlen( h_text ) + 1, h_text );


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
        } else {
            hd_level = hn_lvl;              // H0 always valid in BODY
        }
        break;
    case  hds_h1:
        if( !((ProcFlags.doc_sect >= doc_sect_body) ||
            (ProcFlags.doc_sect_nxt >= doc_sect_body)) ) {

            g_err( err_tag_wrong_sect, hxstr, ":BODY :APPENDIX :BACKM sections" );
            err_count++;
            file_mac_info();
        } else if( !((ProcFlags.doc_sect == doc_sect_body) ||
            (ProcFlags.doc_sect_nxt == doc_sect_body)) ) {  // APPENDIX or BACKM
            hd_level = hn_lvl;              // H1 valid at this point
        } else {                                            // BODY
            if( hd_level < hn_lvl - 1 ) {
                g_wng_hlevel( hn_lvl, hd_level + 1 );
                hd_level = hn_lvl;          // H2 to H6 will be valid if none are skipped
            } else {
                hd_level = hn_lvl;          // H1 valid at this point
            }
        }
        break;
    case  hds_h2:
    case  hds_h3:
    case  hds_h4:
    case  hds_h5:
    case  hds_h6:
        if( hd_level < hn_lvl - 1 ) {
            g_wng_hlevel( hn_lvl, hd_level + 1 );
            hd_level = hn_lvl;          // hn_lvl + 1 to H6 will be valid if none are skipped
        } else {
            hd_level = hn_lvl;          // hn_lvl valid at this point
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

    /* After this, hds_lvl will access the correct LAYOUT data */

    if( (ProcFlags.doc_sect == doc_sect_appendix) && (hn_lvl == hds_h1) ) {
        hds_lvl = hds_appendix;
    } else {
        hds_lvl = hn_lvl;
    }

    if( layout_work.hx.hx_head[hds_lvl].number_form != num_none ) {
        hd_nums[hn_lvl].headn++;
    }

    p = scan_start;
    while( *p == ' ' ) p++;
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
        if( ProcFlags.col_started ) {
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
    }

    /***********************************************************************/
    /*  creation of actual heading                                         */
    /***********************************************************************/

    sav_spacing = spacing;

    p = get_text_line( p );

    if( !ProcFlags.reprocess_line ) {
        if( *p ) {                     // text exists

            /* remove trailing spaces */

            pa = &p[strlen( p )];
            pa--;
            while( *pa == ' ' ) {
                *pa = '\0';
            }

            /* Implement the case attribute */

            if( layout_work.hx.hx_head[hds_lvl].hd_case == case_lower ) {
                strlwr( p );
            } else if( layout_work.hx.hx_head[hds_lvl].hd_case == case_upper ) {
                strupr( p );
            }
        }
        gen_heading( p, id, hn_lvl, hds_lvl );
        scan_start = scan_stop + 1;
    } else {
        gen_heading( "", id, hn_lvl, hds_lvl );
    }

    spacing = sav_spacing;
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

