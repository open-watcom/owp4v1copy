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
* Description: WGML implement prototype support for default layout
*                   incomplete                                        TBD
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


static    uint32_t    max_char_width;
static    uint32_t    max_line_height;
static    uint32_t    net_page_height;
static    uint32_t    net_page_width;



/***************************************************************************/
/*  init_def_lay            construct the default layout                   */
/***************************************************************************/

void    init_def_lay( void )
{
    static  char    z0[] = "0";
    static  char    lm[] = "1i";
    static  char    rm[] = "7i";
    static  char    dp[] = "9.66i";
    static  char    bd[] = "0";
    static  char    i05[] = "0.5i";
    static  char    i04[] = "0.4i";
    static  char    i03[] = "0.3i";
    static  char    i02[] = "0.25i";
    static  char    n1[] = "1";
    static  char    n2[] = "2";
    static  char    n3[] = "3";
    static  char    n4[] = "4";
    char    *       p;
    long    int     i;
    int             k;

    /***********************************************************************/
    /*  :PAGE  values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.page.top_margin );

    p = &lm;
    to_internal_SU( &p, &layout_work.page.left_margin );

    p = &rm;
    to_internal_SU( &p, &layout_work.page.right_margin );

    p = &dp;
    to_internal_SU( &p, &layout_work.page.depth );

    /***********************************************************************/
    /*  :DEFAULT values                                                    */
    /***********************************************************************/
    layout_work.defaults.spacing = 1;
    layout_work.defaults.columns = 1;
    layout_work.defaults.font = 0;
    layout_work.defaults.justify = 1;
    layout_work.defaults.input_esc = ' ';
    p = &i05;
    to_internal_SU( &p, &layout_work.defaults.gutter );

    p = &bd;
    to_internal_SU( &p, &layout_work.defaults.binding );

    /***********************************************************************/
    /* :WIDOW values                                                       */
    /***********************************************************************/
    layout_work.widow.threshold = 2;

    /***********************************************************************/
    /* :FN    values                                                       */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.fn.line_indent );

    p = &i04;
    to_internal_SU( &p, &layout_work.fn.align );

    p = &n2;
    to_internal_SU( &p, &layout_work.fn.pre_lines );

    p = &n2;
    to_internal_SU( &p, &layout_work.fn.skip );

    layout_work.fn.spacing = 1;

    layout_work.fn.font = 0;

    layout_work.fn.number_font = 0;

    layout_work.fn.number_style = h_style;

    layout_work.fn.frame = 0;

    /***********************************************************************/
    /* :FN    values                                                       */
    /***********************************************************************/
    layout_work.fnref.font = 0;

    layout_work.fnref.number_style = h_style | xp_style;


    /***********************************************************************/
    /*  :P     values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.p.line_indent );

    p = &n1;
    to_internal_SU( &p, &layout_work.p.pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.p.post_skip );


    /***********************************************************************/
    /*  :PC    values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.pc.line_indent );

    p = &n1;
    to_internal_SU( &p, &layout_work.pc.pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.pc.post_skip );


    /***********************************************************************/
    /*  :FIG   values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.fig.left_adjust );

    p = &z0;
    to_internal_SU( &p, &layout_work.fig.right_adjust );

    p = &n2;
    to_internal_SU( &p, &layout_work.fig.pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.fig.post_skip );

    layout_work.fig.spacing = 1;
    layout_work.fig.font = 0;
    layout_work.fig.default_place = top_place;

    layout_work.fig.default_frame.type = rule_frame;

    strcpy( layout_work.fig.default_frame.string, "????" ); // not used


    /***********************************************************************/
    /*  :XMP   values                                                      */
    /***********************************************************************/
    p = &i02;
    to_internal_SU( &p, &layout_work.xmp.left_indent );

    p = &z0;
    to_internal_SU( &p, &layout_work.xmp.right_indent );

    p = &n2;
    to_internal_SU( &p, &layout_work.xmp.pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.xmp.post_skip );

    layout_work.xmp.spacing = 1;

    layout_work.xmp.font = 0;


    /***********************************************************************/
    /*  :NOTE  values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.note.left_indent );

    p = &z0;
    to_internal_SU( &p, &layout_work.note.right_indent );

    p = &n1;
    to_internal_SU( &p, &layout_work.note.pre_skip );

    p = &n1;
    to_internal_SU( &p, &layout_work.note.post_skip );

    layout_work.note.spacing = 1;

    layout_work.note.font = 2;

    strcpy( layout_work.note.string, "NOTE: " );


    /***********************************************************************/
    /*  :H0 - :H6 values                                                   */
    /***********************************************************************/

    k = 0;                              // -----------------

    layout_work.hx[k].group = 0;

    p = &i05;
    to_internal_SU( &p, &layout_work.hx[k].indent );

    p = &n4;
    to_internal_SU( &p, &layout_work.hx[k].pre_top_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.hx[k].pre_skip );

    p = &n4;
    to_internal_SU( &p, &layout_work.hx[k].post_skip );

    layout_work.hx[k].spacing = 1;
    layout_work.hx[k].font = 3;
    layout_work.hx[k].number_font = 3;
    layout_work.hx[k].number_form = num_none;
    layout_work.hx[k].page_position = pos_left;
    layout_work.hx[k].number_style = h_style;
    layout_work.hx[k].page_eject = ej_yes;
    layout_work.hx[k].line_break = true;
    layout_work.hx[k].display_heading = true;
    layout_work.hx[k].number_reset = true;
    layout_work.hx[k].cases = case_mixed;

    p = &z0;
    to_internal_SU( &p, &layout_work.hx[k].align );

    k = 1;                              // -----------------
    memcpy( &layout_work.hx[1], &layout_work.hx[0],
            sizeof( layout_work.hx[0] ) );
    p = &z0;
    to_internal_SU( &p, &layout_work.hx[k].indent );

    p = &n3;
    to_internal_SU( &p, &layout_work.hx[k].pre_top_skip );

    p = &n3;
    to_internal_SU( &p, &layout_work.hx[k].post_skip );

    layout_work.hx[k].number_form = num_new;

    k = 2;                              // -----------------
    memcpy( &layout_work.hx[2], &layout_work.hx[1],
            sizeof( layout_work.hx[0] ) );
    p = &n2;
    to_internal_SU( &p, &layout_work.hx[k].pre_top_skip );

    p = &n2;
    to_internal_SU( &p, &layout_work.hx[k].post_skip );

    layout_work.hx[k].number_form = num_prop;
    layout_work.hx[k].page_eject = ej_no;

    k = 3;                              // -----------------
    memcpy( &layout_work.hx[3], &layout_work.hx[2],
            sizeof( layout_work.hx[0] ) );

    k = 4;                              // -----------------
    memcpy( &layout_work.hx[4], &layout_work.hx[3],
            sizeof( layout_work.hx[0] ) );
    layout_work.hx[k].font = 2;

    k = 5;                              // -----------------
    memcpy( &layout_work.hx[5], &layout_work.hx[4],
            sizeof( layout_work.hx[0] ) );
    layout_work.hx[k].line_break = false;

    k = 6;                              // -----------------
    memcpy( &layout_work.hx[6], &layout_work.hx[5],
            sizeof( layout_work.hx[0] ) );

    layout_work.hx[k].font = 1;


    /***********************************************************************/
    /*  :HEADING  values                                                   */
    /***********************************************************************/
    layout_work.heading.delim = '.';
    layout_work.heading.stop_eject = false;
    layout_work.heading.para_indent = false;
    layout_work.heading.threshold = 2;
    layout_work.heading.max_group = 10;

    /***********************************************************************/
    /*  :LQ       values                                                   */
    /***********************************************************************/
    p = &i02;
    to_internal_SU( &p, &layout_work.lq.left_indent );

    p = &i02;
    to_internal_SU( &p, &layout_work.lq.right_indent );

    p = &n1;
    to_internal_SU( &p, &layout_work.lq.pre_skip );

    p = &n1;
    to_internal_SU( &p, &layout_work.lq.post_skip );

    layout_work.lq.spacing = 1;
    layout_work.lq.font = 0;

    /***********************************************************************/
    /*  tags  with only font as value                                      */
    /***********************************************************************/
    layout_work.cit.font = 1;
    layout_work.dd.font = 0;
    layout_work.ddhd.font = 1;
    layout_work.dt.font = 2;
    layout_work.dthd.font = 1;
    layout_work.gd.font = 0;
    layout_work.gt.font = 2;
    layout_work.ixmajor.font = 2;
    layout_work.ixpgnum.font = 0;


    /***********************************************************************/
    /*  :FIGCAP     values                                                 */
    /***********************************************************************/
    p = &n1;
    to_internal_SU( &p, &layout_work.figcap.pre_lines );

    strcpy( layout_work.figcap.string, "Figure " );
    layout_work.figcap.font = 0;
    layout_work.figcap.string_font = 0;
    layout_work.figcap.delim = '.';

    /***********************************************************************/
    /*  :FIGDESC    values                                                 */
    /***********************************************************************/
    p = &n1;
    to_internal_SU( &p, &layout_work.figdesc.pre_lines );

    layout_work.figdesc.font = 0;


    /***********************************************************************/
    /*  :DD         values                                                 */
    /***********************************************************************/
    p = &i05;
    to_internal_SU( &p, &layout_work.dd.line_left );

    layout_work.dd.font = 0;

    /***********************************************************************/
    /*  :ABSTRACT   values                                                 */
    /***********************************************************************/
    p = &n1;
    to_internal_SU( &p, &layout_work.abstract.post_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.abstract.pre_top_skip );

    layout_work.abstract.font = 1;
    layout_work.abstract.spacing = 1;
    layout_work.abstract.header = true;
    strcpy( layout_work.abstract.string, "ABSTRACT" );
    layout_work.abstract.page_eject = ej_yes;
    layout_work.abstract.page_reset = true;
    layout_work.abstract.columns = 1;

    /***********************************************************************/
    /*  :PREFACE    values                                                 */
    /***********************************************************************/
    p = &n1;
    to_internal_SU( &p, &layout_work.preface.post_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.preface.pre_top_skip );

    layout_work.preface.font = 1;
    layout_work.preface.spacing = 1;
    layout_work.preface.header = true;
    strcpy( layout_work.preface.string, "preface" );
    layout_work.preface.page_eject = ej_yes;
    layout_work.preface.page_reset = false;
    layout_work.preface.columns = 1;

    /***********************************************************************/
    /*  :BODY       values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.body.post_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.body.pre_top_skip );

    layout_work.body.header = false;
    layout_work.body.string[0] = '\0';
    layout_work.body.page_eject = ej_yes;
    layout_work.body.page_reset = true;
    layout_work.body.font = 1;

    /***********************************************************************/
    /*  :BACKM      values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.backm.post_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.backm.pre_top_skip );

    layout_work.backm.header = false;
    layout_work.backm.string[0] = '\0';
    layout_work.backm.page_eject = ej_yes;
    layout_work.backm.page_reset = false;
    layout_work.backm.columns = 1;
    layout_work.backm.font = 1;

    /***********************************************************************/
    /*  :LP       values                                                   */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.lp.left_indent );

    p = &z0;
    to_internal_SU( &p, &layout_work.lp.right_indent );

    p = &n1;
    to_internal_SU( &p, &layout_work.lp.pre_skip );

    p = &n1;
    to_internal_SU( &p, &layout_work.lp.post_skip );

    layout_work.lp.spacing = 1;

    /***********************************************************************/
    /*  :INDEX    values                                                   */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.index.post_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.index.pre_top_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.index.left_adjust );

    p = &z0;
    to_internal_SU( &p, &layout_work.index.right_adjust );

    layout_work.index.spacing = 1;
    layout_work.index.columns = 1;
    strcpy( layout_work.index.see_string, "See " );
    strcpy( layout_work.index.see_also_string, "See also " );
    layout_work.index.header = false;
    strcpy( layout_work.index.index_string, "Index" );
    layout_work.index.page_eject = ej_yes;
    layout_work.index.page_reset = false;
    layout_work.index.font = 1;

    /***********************************************************************/
    /*  :IXHEAD   values                                                   */
    /***********************************************************************/
    p = &n2;
    to_internal_SU( &p, &layout_work.ixhead.pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.ixhead.post_skip );

    layout_work.ixhead.font = 2;
    p = &z0;
    to_internal_SU( &p, &layout_work.ixhead.indent );

    layout_work.ixhead.frame.type = box_frame;
    layout_work.ixhead.frame.string[0] = '\0';
    layout_work.ixhead.header = true;

    /***********************************************************************/
    /*  :I1 - :I3  values                                                   */
    /***********************************************************************/

    k = 0;                              // -----------------
    p = &n1;
    to_internal_SU( &p, &layout_work.ix[k].pre_skip );

    p = &n1;
    to_internal_SU( &p, &layout_work.ix[k].post_skip );

    p = &n1;
    to_internal_SU( &p, &layout_work.ix[k].skip );

    layout_work.ix[0].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].indent );

    p = &i04;
    to_internal_SU( &p, &layout_work.ix[k].wrap_indent );

    layout_work.ix[k].index_delim[0] = ' ';
    layout_work.ix[k].index_delim[1] = ' ';
    layout_work.ix[k].index_delim[2] = '\0';
    layout_work.ix[k].string_font = 0;

    k = 1;                              // -----------------
    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].post_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].skip );

    layout_work.ix[0].font = 0;
    p = &i03;
    to_internal_SU( &p, &layout_work.ix[k].indent );

    p = &i04;
    to_internal_SU( &p, &layout_work.ix[0].wrap_indent );

    layout_work.ix[k].index_delim[0] = ' ';
    layout_work.ix[k].index_delim[1] = ' ';
    layout_work.ix[k].index_delim[2] = '\0';
    layout_work.ix[k].string_font = 0;

    k = 2;                              // -----------------
    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].pre_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].post_skip );

    p = &z0;
    to_internal_SU( &p, &layout_work.ix[k].skip );

    layout_work.ix[k].font = 0;
    p = &i03;
    to_internal_SU( &p, &layout_work.ix[k].indent );

    p = &i04;
    to_internal_SU( &p, &layout_work.ix[0].wrap_indent );

    layout_work.ix[k].index_delim[0] = ' ';
    layout_work.ix[k].index_delim[1] = ' ';
    layout_work.ix[k].index_delim[2] = '\0';
//  layout_work.ix[k].string_font = 0; no string_font for :I3

    /***********************************************************************/
    /*  :TOC        values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.toc.left_adjust );
    p = &z0;
    to_internal_SU( &p, &layout_work.toc.right_adjust );
    layout_work.toc.spacing = 1;
    layout_work.toc.columns = 1;
    layout_work.toc.toc_levels = 4;
    layout_work.toc.fill_string[0] = '.';
    layout_work.toc.fill_string[1] = '\0';

    /***********************************************************************/
    /*  :TOCPGNUM   values                                                 */
    /***********************************************************************/
    p = &i04;
    to_internal_SU( &p, &layout_work.tocpgnum.size );
    layout_work.tocpgnum.font = 0;

    /***********************************************************************/
    /*  :TOCHx      values                                                 */
    /***********************************************************************/
    k = 0;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = true;

    k = 1;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = true;

    k = 2;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &i02;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = true;

    k = 3;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &i02;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = true;

    k = 4;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &i02;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = no;

    k = 5;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &i02;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = no;


    k = 6;                              // --------------
    layout_work.tochx[k].group = 0;

    p = &i02;
    to_internal_SU( &p, &layout_work.tochx[k].indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].pre_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].post_skip );

    layout_work.tochx[k].font = 0;
    p = &z0;
    to_internal_SU( &p, &layout_work.tochx[k].align );

    layout_work.tochx[k].display_in_toc = no;

    /***********************************************************************/
    /*  :FIGLIST    values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.figlist.left_adjust );
    p = &z0;
    to_internal_SU( &p, &layout_work.figlist.right_adjust );
    p = &z0;
    to_internal_SU( &p, &layout_work.figlist.skip );
    layout_work.figlist.spacing = 1;
    layout_work.figlist.columns = 1;
    layout_work.figlist.fill_string[0] = '.';
    layout_work.figlist.fill_string[1] = '\0';



    /***********************************************************************/
    /*  remaining tags                           TBD                       */
    /***********************************************************************/


    /* These would normally be set per the :LAYOUT. Here, half-inch
     * top, bottom and right margins and a one-inch left margin are
     * applied to an 8-1/2" x 11" page.
                test with one-inch right margin

     * A4 is 210 mm x 297 mm   8.26772 x 11.69291

     */

    if( bin_driver->y_positive == 0 ) {
        g_page_top = 10 * bin_device->vertical_base_units;
        g_page_bottom = bin_device->vertical_base_units / 2;
    } else {
        g_page_top = bin_device->vertical_base_units / 2;
        g_page_bottom = 10 * bin_device->vertical_base_units;
    }

    g_page_left = bin_device->horizontal_base_units;
    g_page_right = (8 * bin_device->horizontal_base_units) - \
//                                    (bin_device->horizontal_base_units / 2);
                                     bin_device->horizontal_base_units;
    max_char_width = 0;
    max_line_height = 0;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( max_char_width < wgml_fonts[i].default_width ) \
            max_char_width = wgml_fonts[i].default_width;
        if( max_line_height < wgml_fonts[i].line_height ) \
            max_line_height = wgml_fonts[i].line_height;
    }

    net_page_height = bin_device->page_depth;
    net_page_width = bin_device->page_width;

//    set single column mode

    g_cl = g_page_right - g_page_left;  // column length
    g_ll = g_cl;                        // line length
    g_cd = 1;                           // no of columns
}


void  init_def_margins( void )
{
    char        buf[BUF_SIZE];

    g_cur_h_start = g_page_left;
    if( bin_driver->y_positive == 0x00 ) {
        g_cur_v_start = g_page_top - (15 * max_line_height);
    } else {
        g_cur_v_start = g_page_top + (1 * max_line_height);
    }


    if( GlobalFlags.firstpass && GlobalFlags.research ) {
        out_msg( "\ndev:%s page_w:%d page_d:%d hor_u:%d ver_u:%d x_s:%d y_s:%d"
                 " x_o:%d y_o:%d\n",
                 bin_device->driver_name,
                 bin_device->page_width,
                 bin_device->page_depth,
                 bin_device->horizontal_base_units,
                 bin_device->vertical_base_units,
                 bin_device->x_start,
                 bin_device->y_start,
                 bin_device->x_offset,
                 bin_device->y_offset
               );
        out_msg( "\nfont:0 def_width:%d dv:%d em:%d font_h:%d font_s:%d"
                 " line_h:%d line_s:%d spc_w:%d\n\n",
                 wgml_fonts[0].default_width,
                 wgml_fonts[0].dv_base,
                 wgml_fonts[0].em_base,
                 wgml_fonts[0].font_height,
                 wgml_fonts[0].font_space,
                 wgml_fonts[0].line_height,
                 wgml_fonts[0].line_space,
                 wgml_fonts[0].spc_width
               );

        snprintf( buf, buf_size,
                  "page top:%d bottom:%d left:%d right:%d'\n",
                  g_page_top, g_page_bottom, g_page_left, g_page_right );
        out_msg( buf );
        snprintf( buf, buf_size,
           "page net heigth:%d width:%d line height:%d char width:%d'\n\n",
                  net_page_height, net_page_width, max_line_height,
                  max_char_width );
        out_msg( buf );
    }
}
