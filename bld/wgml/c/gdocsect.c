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
* Description:  WGML tags for document sections :GDOC, :FRONTM, :BODY, ...
*               mostly tested :BODY so far
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"



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

    ban = layout_work.banner;
    while( ban != NULL ) {              // for all banners
        if( ban->docsect == sect_2_bansect[ds] ) {  // if our doc section
            switch( ban->place ) {
            case   top_place :
                sect_ban_top[0] = ban;
                /* fallthru*/
            case   topodd_place :
                sect_ban_top[1] = ban;
                break;

            case   bottom_place :
                sect_ban_bot[0] = ban;
                /* fallthru*/
            case   botodd_place :
                sect_ban_bot[1] = ban;
                break;

            case   topeven_place :
                sect_ban_top[0] = ban;
                break;
            case   boteven_place :
                sect_ban_bot[0] = ban;
                break;
            default:
                break;
            }
        }
        ban = ban->next;
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
/*    prepare_doc_sect true section start                             TBD  */
/***************************************************************************/

void    prepare_doc_sect( doc_section ds )
{
    int         ind;
    uint32_t    v_start;
    uint32_t    h_start;

    if( ProcFlags.prep_section ) {
        return;                         // once is enough
    }
    ProcFlags.prep_section = true;
    ProcFlags.keep_left_margin = false;

    if( ds != doc_sect_body ) {
        out_msg( "prepare_doc_sect possibly incomplete\n" );
    }
    if( ds == doc_sect_none ) {
        ds = doc_sect_body;      // if text without section start assume body
    }

    switch( ds ) {
    case   doc_sect_body:
        if( layout_work.body.page_reset  ) {
            page = 1;
        }
        break;
    case   doc_sect_abstract:
        if( layout_work.abstract.page_reset  ) {
            page = 1;
        }
        break;
    case   doc_sect_preface:
        if( layout_work.preface.page_reset  ) {
            page = 1;
        }
        break;
    case   doc_sect_appendix:
        if( layout_work.appendix.page_reset  ) {
            page = 1;
        }
        break;
    case   doc_sect_backm:
        if( layout_work.backm.page_reset  ) {
            page = 1;
        }
        break;
    case   doc_sect_index:
        if( layout_work.index.page_reset  ) {
            page = 1;
        }
        break;
    default:
        break;
    }

    ind = page & 1;
    h_start = g_page_left_org;
    if( sect_ban_top[ind] != NULL ) {
        v_start = ban_top_pos( sect_ban_top[ind] );

    } else {
        v_start = g_page_top;
    }

    if( GlobalFlags.lastpass ) {
        if( ProcFlags.fb_position_done ) {
            fb_new_section( v_start );
        } else {
            fb_position( h_start, v_start );
            ProcFlags.fb_position_done = true;
        }
    }
    g_cur_v_start = v_start;
    g_cur_h_start = h_start;
    out_ban_top( sect_ban_top[ind] );

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

    g_cur_h_start = g_page_left_org +
                    conv_hor_unit( &layout_work.p.line_indent );// TBD
}



/***************************************************************************/
/*          routine to process document section change                     */
/***************************************************************************/
static  void    gml_doc_xxx( doc_section ds, bool eject )
{

    if( ProcFlags.doc_sect >= ds ) {    // wrong sequence of sections
        g_err_doc_sect( ds );
    } else {
        if( eject && ProcFlags.page_started ) {// finish previous section
            finish_page();
        }
        ProcFlags.doc_sect = ds;
    }

    set_section_banners( ds );

    ProcFlags.prep_section = false;     // do real section start later

    spacing = layout_work.defaults.spacing;
    g_curr_font_num = layout_work.defaults.font;
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
/*  :BODY                                                                  */
/*  :APPENDIX  optional                                                    */
/*  :BACKM     optional                                                    */
/*  :INDEX     optional                                                    */
/*  :EGDOC     optional                                                    */
/***************************************************************************/

extern  void    gml_abstract( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_abstract, layout_work.abstract.page_eject );
    spacing = layout_work.abstract.spacing;
}

extern  void    gml_appendix( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_appendix, layout_work.appendix.page_eject );
    spacing = layout_work.appendix.spacing;
}

extern  void    gml_backm( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_backm, layout_work.backm.page_eject );
}

extern  void    gml_body( const gmltag * entry )
{
//  static su skip_TBD = { "1", 1, 0, 0, 0, false, SU_chars_lines };

    gml_doc_xxx( doc_sect_body, layout_work.body.page_eject );

    ProcFlags.just_override = true;     // justify for first line ?? TBD
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
}

extern  void    gml_figlist( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_figlist, false );
    spacing = layout_work.figlist.spacing;
}

extern  void    gml_frontm( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_frontm, true );
    spacing = layout_work.defaults.spacing;
}

extern  void    gml_index( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_index, layout_work.index.page_eject );
    spacing = layout_work.index.spacing;
}

extern  void    gml_preface( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_preface, layout_work.preface.page_eject );
    spacing = layout_work.preface.spacing;
}

extern  void    gml_titlep( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_titlep, true );
    pre_top_skip = 0;
    post_top_skip = 0;
    post_skip = NULL;
    spacing = layout_work.titlep.spacing;
}

extern  void    gml_etitlep( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_etitlep, false );
}

extern  void    gml_toc( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_toc, true );
    spacing = layout_work.toc.spacing;
}

extern  void    gml_egdoc( const gmltag * entry )
{
    ProcFlags.test_widow = false;
    gml_doc_xxx( doc_sect_egdoc, true );
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

    gml_doc_xxx( doc_sect_gdoc, true );
    return;
}

