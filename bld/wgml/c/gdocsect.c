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
* Description:  WGML tags and routines for document section changes
*                    :GDOC, :FRONTM, :BODY, ...
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

uint32_t        titlep_lineno;      // TITLEP tag line number

/***************************************************************************/
/*  error routine for wrong sequence of doc section tags                   */
/***************************************************************************/
static  void    g_err_doc_sect( doc_section  ds )
{
    static  char const  sect[14][9] =
        {                               // same sequence as doc_section enum
            "NONE",
            "GDOC",
            "FRONTM",
            "TITLEP",
            "eTITLEP",
            "ABSTRACT",
            "PREFACE",
            "TOC",
            "FIGLIST",
            "BODY",
            "APPENDIX",
            "BACKM",
            "INDEX",
            "eGDOC"
        };

    err_count++;
    scan_err = true;
    g_err( err_doc_sect, sect[ds] );
    file_mac_info();
}


/***************************************************************************/
/*  set banner pointers for specified doc section                          */
/***************************************************************************/
void set_section_banners( doc_section ds )
{
    banner_lay_tag  * ban;

    /***********************************************************************/
    /*  transform doc_section enum into ban_doc_sect enum                  */
    /***********************************************************************/
    static const ban_docsect sect_2_bansect[doc_sect_egdoc + 1] = {

        no_ban,                         // doc_sect_none,
        no_ban,                         // doc_sect_gdoc,
        no_ban,                         // doc_sect_frontm,
        no_ban,                         // doc_sect_titlep,
        no_ban,                         // doc_sect_etitlep,
        abstract_ban,                   // doc_sect_abstract,
        preface_ban,                    // doc_sect_preface,
        toc_ban,                        // doc_sect_toc,
        figlist_ban,                    // doc_sect_figlist,
        body_ban,                       // doc_sect_body,
        appendix_ban,                   // doc_sect_appendix,
        backm_ban,                      // doc_sect_backm,
        index_ban,                      // doc_sect_index,
        no_ban                          // doc_sect_egdoc
    };

/* not yet coded banner place values               TBD
               not all are document section related
    head0_ban,
    head1_ban,
    head2_ban,
    head3_ban,
    head4_ban,
    head5_ban,
    head6_ban,
    letfirst_ban,
    letlast_ban,
    letter_ban,
    max_ban
} ban_docsect;
****************/

    sect_ban_top[0] = sect_ban_top[1] = NULL;
    sect_ban_bot[0] = sect_ban_bot[1] = NULL;

    if( no_ban != sect_2_bansect[ds]  ) {

        for( ban = layout_work.banner; ban != NULL; ban = ban->next ) {
            if( ban->docsect == sect_2_bansect[ds] ) {  // if our doc section
                switch( ban->place ) {
                case   top_place :
                    sect_ban_top[0] = ban;
                    sect_ban_top[1] = ban;
                    break;
                case   bottom_place :
                    sect_ban_bot[0] = ban;
                    sect_ban_bot[1] = ban;
                    break;

                case   topodd_place :
                    sect_ban_top[1] = ban;
                    break;
                case   topeven_place :
                    sect_ban_top[0] = ban;
                    break;

                case   botodd_place :
                    sect_ban_bot[1] = ban;
                    break;
                case   boteven_place :
                    sect_ban_bot[0] = ban;
                    break;
                default:
                    break;
                }
            }
        }
    }
}


/***************************************************************************/
/*    finish page processing                                               */
/***************************************************************************/

void    finish_page( void )
{
    if( ProcFlags.page_started ) {      // any output for page?
        if( t_line.first != NULL ) {    // incomplete line
            process_line_full( &t_line, false );
        }
        out_ban_bot( sect_ban_bot[page & 1] );  // possible bottom banner
        ProcFlags.page_started = false;
    }
    ProcFlags.top_ban_proc = false;
    ProcFlags.test_widow = false;
}


/***************************************************************************/
/*  set some values for new section                                        */
/***************************************************************************/

static  void    new_section( doc_section ds )
{
    ProcFlags.doc_sect = ds;
    set_section_banners( ds );

    spacing = layout_work.defaults.spacing;
    g_curr_font_num = layout_work.defaults.font;
}


/***************************************************************************/
/*    finish page processing  and section change                           */
/***************************************************************************/

static  void    finish_page_section( doc_section ds, bool eject )
{
    if( eject ) {
        g_skip = 0;                     // ignore remaining skip value
        g_skip_wgml4 = 0;
    }
    finish_page();
    new_section( ds );
}


/***************************************************************************/
/*    output section header ABSTRACT, PREFACE, ...                         */
/***************************************************************************/

static  void    doc_header( su *p_sk, su *top_sk, xx_str *h_string,
                            int8_t font, int8_t spc )
{
    text_chars  *   curr_t;
    text_line       hd_line;
    uint32_t        pre_top_sk;
    uint32_t        post_sk;
    uint32_t        header_post_sk;
    int32_t         h_left;
    int8_t          s_font;

    post_sk = 0;
    if( post_skip != NULL ) {
        if( ProcFlags.page_started ) {  // ignore post skip at page start
            post_sk = conv_vert_unit( post_skip, spacing );
        }
        post_skip = NULL;
    }

    s_font = g_curr_font_num;
    g_curr_font_num = font;
    if( top_sk != NULL ) {
        pre_top_sk = conv_vert_unit( top_sk, spc );
    } else {
        pre_top_sk = 0;
    }
    if( p_sk != NULL ) {
        header_post_sk = conv_vert_unit( top_sk, spc );
    } else {
        header_post_sk = 0;
    }
    g_curr_font_num = s_font;

    /***********************************************************************/
    /*  MERGING pre top skip with previous post skip  HOW?       TBD       */
    /*  for now it is added                                                */
    /***********************************************************************/
    pre_top_sk += post_sk;

    if( bin_driver->y_positive == 0 ) {
        g_cur_v_start -= pre_top_sk;
    } else {
        g_cur_v_start += pre_top_sk;
    }

    curr_t = alloc_text_chars( h_string, strlen( h_string ), font );
    hd_line.first = curr_t;
    curr_t->width = cop_text_width( curr_t->text, curr_t->count, font );

    hd_line.line_height = wgml_fonts[font].line_height;
    hd_line.y_address = g_cur_v_start;

    h_left = g_page_left +(g_page_right - g_page_left - curr_t->width) / 2;
    curr_t->x_address = h_left;

    /*******************************************************************/
    /*  adjust vertical position from upper to lower border of line    */
    /*******************************************************************/
    if( bin_driver->y_positive == 0x00 ) {
        hd_line.y_address -= hd_line.line_height;
        g_cur_v_start -= hd_line.line_height;
    } else {
        hd_line.y_address += hd_line.line_height;
        g_cur_v_start += hd_line.line_height;
    }
    if( GlobalFlags.lastpass && hd_line.first != NULL) {
        if( input_cbs->fmflags & II_research ) {
            test_out_t_line( &hd_line );
        }
        fb_output_textline( &hd_line );
    }

    if( hd_line.first != NULL) {
        add_text_chars_to_pool( &hd_line );
        hd_line.first = NULL;
    }

    if( bin_driver->y_positive == 0x00 ) {
        g_cur_v_start -= header_post_sk;
    } else {
        g_cur_v_start += header_post_sk;
    }
}


/***************************************************************************/
/*  set new vertical position depending on banner existance                */
/***************************************************************************/

static void document_new_position( void )
{

    if( sect_ban_top[page & 1] != NULL ) {
        g_cur_v_start = ban_top_pos( sect_ban_top[page & 1] );
    } else {
        g_cur_v_start = g_page_top;
    }
    g_cur_h_start = g_page_left_org;

    if( GlobalFlags.lastpass ) {
        if( ProcFlags.fb_position_done ) {
            fb_new_section( g_cur_v_start );
        } else {
            fb_position( g_page_left_org, g_cur_v_start );
            ProcFlags.fb_position_done = true;
        }
    }
    return;
}


/***************************************************************************/
/*    start_doc_sect true section start                                    */
/***************************************************************************/

void    start_doc_sect( void )
{
    int         ind;
    page_ej     page_e;
    bool        header;
    bool        page_r;
    xx_str  *   h_string;
    su      *   top_sk;
    su      *   p_sk;
    int8_t      font;
    int8_t      h_spc;
    doc_section ds;
    bool        first_section;

    if( ProcFlags.start_section ) {
        return;                         // once is enough
    }
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }

    first_section = (ProcFlags.doc_sect == doc_sect_none);

    header = false;              // no header string (ABSTRACT, ... )  output
    page_r = false;                     // no page number reset
    page_e = ej_no;                     // no page eject
    ProcFlags.start_section = true;
    ProcFlags.keep_left_margin = false;
    ds = ProcFlags.doc_sect_nxt;        // new section

    if( ds == doc_sect_none ) {
        ds = doc_sect_body;      // if text without section start assume body
    }

    /***********************************************************************/
    /*  process special section attributes                                 */
    /***********************************************************************/

    switch( ds ) {
    case   doc_sect_body:
        page_r   = layout_work.body.page_reset;
        page_e   = layout_work.body.page_eject;
        if( layout_work.body.header ) {
            header   = true;
            h_string = &layout_work.body.string;
            top_sk   = &layout_work.body.pre_top_skip;
            p_sk     = &layout_work.body.post_skip;
            font     = layout_work.body.font;
            h_spc    = spacing;         // standard spacing
        }
        break;
    case   doc_sect_titlep:             // for preceding :BINCLUDE/:GRAPHIC
        page_e   = ej_yes;
        init_nest_cb();
        nest_cb->p_stack = copy_to_nest_stack();
        nest_cb->c_tag = t_TITLEP;
        nest_cb->p_stack->lineno = titlep_lineno; // correct line number
        break;
    case   doc_sect_abstract:
        page_r   = layout_work.abstract.page_reset;
        page_e   = layout_work.abstract.page_eject;
        if( layout_work.abstract.header ) {
            header = true;
            h_string = &layout_work.abstract.string;
            top_sk   = &layout_work.abstract.pre_top_skip;
            p_sk     = &layout_work.abstract.post_skip;
            font     = layout_work.abstract.font;
            h_spc    = layout_work.abstract.spacing;
        }
        break;
    case   doc_sect_preface:
        page_r   = layout_work.preface.page_reset;
        page_e   = layout_work.preface.page_eject;
        if( layout_work.preface.header ) {
            header = true;
            h_string = &layout_work.preface.string;
            top_sk   = &layout_work.preface.pre_top_skip;
            p_sk     = &layout_work.preface.post_skip;
            font     = layout_work.preface.font;
            h_spc    = layout_work.preface.spacing;
        }
        break;
    case   doc_sect_appendix:
        page_r   = layout_work.appendix.page_reset;
        page_e   = layout_work.appendix.page_eject;
        if( layout_work.appendix.header ) {
            header = true;
            h_string = &layout_work.appendix.string;
            top_sk   = &layout_work.appendix.pre_top_skip;
            p_sk     = &layout_work.appendix.post_skip;
            font     = layout_work.appendix.font;
            h_spc    = layout_work.appendix.spacing;
        }
        break;
    case   doc_sect_backm:
        page_r   = layout_work.backm.page_reset;
        page_e   = layout_work.backm.page_eject;
        if( layout_work.backm.header ) {
            header = true;
            h_string = &layout_work.backm.string;
            top_sk   = &layout_work.backm.pre_top_skip;
            p_sk     = &layout_work.backm.post_skip;
            font     = layout_work.backm.font;
            h_spc    = spacing;         // standard spacing
        }
        break;
    case   doc_sect_index:
        page_r   = layout_work.index.page_reset;
        page_e   = layout_work.index.page_eject;
        if( layout_work.index.header ) {
            header = true;
            h_string = &layout_work.index.index_string;
            top_sk   = &layout_work.index.pre_top_skip;
            p_sk     = &layout_work.index.post_skip;
            font     = layout_work.index.font;
            h_spc    = layout_work.index.spacing;
        }
        break;
    default:
        new_section( ds );
        break;
    }

    switch( page_e ) {                  // page eject requested
    case ej_yes :
        if( ProcFlags.page_started ) {
            finish_page_section( ds, true );
        } else {
            new_section( ds );
        }
        if( page_r ) {
            page = 0;
        }
        if( first_section ) {
            page++;
            document_new_position();
        } else {
            document_new_page();
        }
        document_top_banner();
        break;
    case ej_odd :
        if( ProcFlags.page_started || !(page & 1) ) {
            finish_page_section( ds, true );
            if( page_r ) {
                page = 0;
            }
            document_new_page();
            document_new_position();
            document_top_banner();
            if( !(page & 1) ) {
                finish_page();
                document_new_page();
                document_new_position();
                document_top_banner();
            }
        }
        break;
    case ej_even :
        if( ProcFlags.page_started || (page & 1) ) {
            finish_page_section( ds, true );
            if( page_r ) {
                page = 0;
            }
            document_new_page();
            document_new_position();
            document_top_banner();
            if( (page & 1) ) {
                finish_page();
                document_new_page();
                document_new_position();
                document_top_banner();
            }
        }
        break;
    default:
        new_section( ds );
        if( first_section ) {
            page++;
            document_new_position();
        }
        break;
    }

    /***********************************************************************/
    /*  set page bottom limit                                              */
    /***********************************************************************/

    ind = page & 1;

    if( sect_ban_bot[ind] != NULL ) {
        if( bin_driver->y_positive == 0 ) {
            g_page_bottom = g_page_bottom_org
                            + conv_vert_unit( &sect_ban_bot[ind]->depth, 0 );
        } else {
            g_page_bottom = g_page_bottom_org
                            - conv_vert_unit( &sect_ban_bot[ind]->depth, 0 );
        }
    } else {
        g_page_bottom = g_page_bottom_org;
    }
    g_cur_left = g_page_left_org;
    g_cur_h_start = g_page_left_org +
                    conv_hor_unit( &layout_work.p.line_indent );// TBD

    if( header ) {
        doc_header( p_sk, top_sk, h_string, font, h_spc );
    }

    ProcFlags.para_started = false;
}


/***************************************************************************/
/*          routine to init    document section change                     */
/***************************************************************************/
static  void    gml_doc_xxx( doc_section ds )
{

    if( ProcFlags.doc_sect >= ds ) {    // wrong sequence of sections
        g_err_doc_sect( ds );
    }
    ProcFlags.doc_sect_nxt = ds;        // remember new section
    ProcFlags.start_section = false;    // do real section start later

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  Document section tags                                                  */
/*                                                                         */
/*  :GDOC                 the only one with attributes                     */
/*  :FRONTM    optional                                                    */
/*  :TITLEP    optional                                                    */
/*  :ETITLEP   optional                                                    */
/*  :ABSTRACT  optional                                                    */
/*  :PREFACE   optional                                                    */
/*  :TOC       optional                                                    */
/*  :FIGLIST   optional                                                    */
/*  :BODY              default                                             */
/*  :APPENDIX  optional                                                    */
/*  :BACKM     optional                                                    */
/*  :INDEX     optional                                                    */
/*  :EGDOC     optional                                                    */
/***************************************************************************/

extern  void    gml_abstract( const gmltag * entry )
{
    if( ProcFlags.doc_sect_nxt == doc_sect_egdoc ) {
        xx_line_err( err_eof_expected, tok_start );
        return;
    }
    if( !ProcFlags.frontm_seen ) {
        xx_line_err( err_doc_sec_expected_1, tok_start );
        return;
    }
    gml_doc_xxx( doc_sect_abstract );
    spacing = layout_work.abstract.spacing;
    g_cur_left = g_page_left;
    g_cur_h_start = g_page_left;

}

extern  void    gml_appendix( const gmltag * entry )
{
    if( layout_work.appendix.page_eject ) {
        finish_page();
    }
    gml_doc_xxx( doc_sect_appendix );
    spacing = layout_work.appendix.spacing;
    ProcFlags.frontm_seen = false;  // no longer in FRONTM section
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }
}

extern  void    gml_backm( const gmltag * entry )
{
    if( layout_work.backm.page_eject ) {
        finish_page();
    }
    gml_doc_xxx( doc_sect_backm );
    ProcFlags.frontm_seen = false;  // no longer in FRONTM section
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }
}

extern  void    gml_body( const gmltag * entry )
{
//  static su skip_TBD = { "1", 1, 0, 0, 0, false, SU_chars_lines };

    if( layout_work.body.page_eject ) {
        finish_page();
    }
    gml_doc_xxx( doc_sect_body );

    ProcFlags.just_override = true;     // justify for first line ?? TBD
    g_cur_left = g_page_left;
    g_cur_h_start = g_page_left
                    + conv_hor_unit( &layout_work.p.line_indent );
    spacing = layout_work.defaults.spacing;

    pre_top_skip = 0;
    post_top_skip = 0;
    post_skip = NULL;

    /***********************************************************************/
    /*  for 1. body page try H0   skip or others                           */
    /***********************************************************************/

//  post_skip = &layout_work.hx[0].pre_top_skip;// TBD
//  post_skip = &layout_work.p.pre_skip;// TBD
//  post_skip = &skip_TBD;              // TBD

//  ProcFlags.para_line1 = true;        // simulate :P start  ?? TBD
    ProcFlags.frontm_seen = false;      // no longer in FRONTM section
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }
}

extern  void    gml_figlist( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_figlist );
    spacing = layout_work.figlist.spacing;
}

extern  void    gml_frontm( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_frontm );
    spacing = layout_work.defaults.spacing;
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }
    ProcFlags.frontm_seen = true;
}

extern  void    gml_index( const gmltag * entry )
{
    if( ProcFlags.doc_sect_nxt == doc_sect_egdoc ) {
        xx_line_err( err_eof_expected, tok_start );
        return;
    }
    if( !((ProcFlags.doc_sect == doc_sect_backm) ||
          (ProcFlags.doc_sect_nxt == doc_sect_backm)) ) {
        xx_line_err( err_doc_sec_expected_1, tok_start );
        return;
    }
    if( layout_work.index.page_eject ) {
        finish_page();
    }
    gml_doc_xxx( doc_sect_index );
    spacing = layout_work.index.spacing;
}

extern  void    gml_preface( const gmltag * entry )
{
    if( ProcFlags.doc_sect_nxt == doc_sect_egdoc ) {
        xx_line_err( err_eof_expected, tok_start );
        return;
    }
    if( !ProcFlags.frontm_seen ) {
        xx_line_err( err_doc_sec_expected_1, tok_start );
        return;
    }
    if( layout_work.preface.page_eject ) {
        finish_page();
    }
    gml_doc_xxx( doc_sect_preface );
    spacing = layout_work.preface.spacing;
}

extern  void    gml_titlep( const gmltag * entry )
{
    if( ProcFlags.doc_sect_nxt == doc_sect_egdoc ) {
        xx_line_err( err_eof_expected, tok_start );
        return;
    }
    if( !ProcFlags.frontm_seen ) {
        xx_line_err( err_doc_sec_expected_1, tok_start );
        return;
    }
    gml_doc_xxx( doc_sect_titlep );
    pre_top_skip = 0;
    post_top_skip = 0;
    post_skip = NULL;
    spacing = layout_work.titlep.spacing;
    rs_loc = titlep_tag;
    if( input_cbs->fmflags & II_file ) {    // save line number
        titlep_lineno = input_cbs->s.f->lineno;
    } else if( input_cbs->fmflags & II_macro ) {
        titlep_lineno = input_cbs->s.m->lineno;
    } else {
        titlep_lineno = 0;                  // not clear what to do here
    }
}

extern  void    gml_etitlep( const gmltag * entry )
{
    tag_cb  *   wk;

    gml_doc_xxx( doc_sect_etitlep );
    rs_loc = 0;
    titlep_lineno = 0;

    if( nest_cb != NULL ) { // guard against no FRONTM, empty TITLEP section
        wk = nest_cb;
        nest_cb = nest_cb->prev;
        add_tag_cb_to_pool( wk );
    }
}

extern  void    gml_toc( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_toc );
    spacing = layout_work.toc.spacing;
}

extern  void    gml_egdoc( const gmltag * entry )
{
    start_doc_sect();                   // if not already done
    finish_page();
    ProcFlags.test_widow = false;
    gml_doc_xxx( doc_sect_egdoc );
}

/***************************************************************************/
/*  :gdoc sec='TOP secret, destroy before reading'                         */
/***************************************************************************/

extern  void    gml_gdoc( const gmltag * entry )
{
    char        *   p;

    scan_err = false;
    p = scan_start;
    if( *p ) p++;

    while( *p == ' ' ) {                // over WS to attribute
        p++;
    }
    if( *p &&
        ! (strnicmp( "sec ", p, 4 ) &&  // look for "sec " or "sec="
           strnicmp( "sec=", p, 4 )) ) {
        char        quote;
        char    *   valstart;

        p += 3;
        while( *p == ' ' ) {
            p++;
        }
        if( *p == '=' ) {
            p++;
            while( *p == ' ' ) {
                p++;
            }
        }
        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = ' ';
        }
        valstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';

        add_symvar( &global_dict, "$sec", valstart, no_subscript, 0 );
    } else {
        add_symvar( &global_dict, "$sec", "", no_subscript, 0 );// set nullstring
    }

    gml_doc_xxx( doc_sect_gdoc );
    if( !ProcFlags.fb_document_done ) { // the very first section/page
        do_layout_end_processing();
    }
    return;
}

