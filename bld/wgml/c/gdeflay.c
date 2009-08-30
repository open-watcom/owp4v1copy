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
*                   and layout processing
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*  init_def_lay            construct the default layout                   */
/***************************************************************************/

void    init_def_lay( void )
{
    static  char    z0[] = "0";
    static  char    i966[] = "9.66i";
    static  char    i7[] = "7i";
    static  char    i1[] = "1i";
    static  char    i05[] = "0.5i";
    static  char    i04[] = "0.4i";
    static  char    i03[] = "0.3i";
    static  char    i02[] = "0.2i";
    static  char    i025[] = "0.25i";
    static  char    n1[] = "1";
    static  char    n2[] = "2";
    static  char    n3[] = "3";
    static  char    n4[] = "4";
    static  char    n15[] = "15";
    static  char    n25[] = "25";
    char    *       p;
    int             k;

    /***********************************************************************/
    /*  :PAGE  values                                                      */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.page.top_margin );

    p = &i1;
    to_internal_SU( &p, &layout_work.page.left_margin );

    p = &i7;
    to_internal_SU( &p, &layout_work.page.right_margin );

    p = &i966;
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

    p = &z0;
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
    p = &i025;
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
    p = &i025;
    to_internal_SU( &p, &layout_work.lq.left_indent );

    p = &i025;
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
    strcpy( layout_work.preface.string, "PREFACE" );
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

    p = &z0;
    to_internal_SU( &p, &layout_work.lp.line_indent );

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
    to_internal_SU( &p, &layout_work.ix[k].wrap_indent );

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
    to_internal_SU( &p, &layout_work.ix[k].wrap_indent );

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

    layout_work.tochx[k].display_in_toc = false;

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

    layout_work.tochx[k].display_in_toc = false;


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

    layout_work.tochx[k].display_in_toc = false;

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
    /*  :FLPGNUM   values                                                  */
    /***********************************************************************/
    p = &i04;
    to_internal_SU( &p, &layout_work.flpgnum.size );
    layout_work.flpgnum.font = 0;

    /***********************************************************************/
    /*  :TITLEP    values                                                  */
    /***********************************************************************/
    layout_work.titlep.spacing = 1;
    layout_work.titlep.columns = 1;

    /***********************************************************************/
    /*  :TITLE      values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.title.left_adjust );
    p = &i1;
    to_internal_SU( &p, &layout_work.title.right_adjust );
    layout_work.title.page_position = pos_right;
    layout_work.title.font = 2;
    p = &n15;
    to_internal_SU( &p, &layout_work.title.pre_top_skip );
    p = &n2;
    to_internal_SU( &p, &layout_work.title.skip );

    /***********************************************************************/
    /*  :DOCNUM     values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.docnum.left_adjust );
    p = &i1;
    to_internal_SU( &p, &layout_work.docnum.right_adjust );
    layout_work.docnum.page_position = pos_right;
    layout_work.docnum.font = 0;
    p = &n2;
    to_internal_SU( &p, &layout_work.docnum.pre_skip );
    strcpy( layout_work.docnum.string, "Document Number " );

    /***********************************************************************/
    /*  :DATE       values                                                 */
    /***********************************************************************/
    strcpy( layout_work.date.date_form, "$ml $dsn, $yl" );
    p = &z0;
    to_internal_SU( &p, &layout_work.date.left_adjust );
    p = &i1;
    to_internal_SU( &p, &layout_work.date.right_adjust );
    layout_work.date.page_position = pos_right;
    layout_work.date.font = 0;
    p = &n2;
    to_internal_SU( &p, &layout_work.date.pre_skip );

    /***********************************************************************/
    /*  :AUTHOR     values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.author.left_adjust );
    p = &i1;
    to_internal_SU( &p, &layout_work.author.right_adjust );
    layout_work.author.page_position = pos_right;
    layout_work.author.font = 0;
    p = &n25;
    to_internal_SU( &p, &layout_work.author.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.author.skip );

    /***********************************************************************/
    /*  :ADDRESS     values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.address.left_adjust );
    p = &i1;
    to_internal_SU( &p, &layout_work.address.right_adjust );
    layout_work.address.page_position = pos_right;
    layout_work.address.font = 0;
    p = &n2;
    to_internal_SU( &p, &layout_work.address.pre_skip );

    /***********************************************************************/
    /*  :ALINE       values                                                 */
    /***********************************************************************/
    p = &n1;
    to_internal_SU( &p, &layout_work.aline.skip );

    /***********************************************************************/
    /*  :APPENDIX   values                                                 */
    /***********************************************************************/
    p = &z0;
    to_internal_SU( &p, &layout_work.appendix.indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.appendix.pre_top_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.appendix.pre_skip );
    p = &n3;
    to_internal_SU( &p, &layout_work.appendix.post_skip );

    layout_work.appendix.spacing = 1;
    layout_work.appendix.font = 3;
    layout_work.appendix.number_font = 3;
    layout_work.appendix.number_form = num_new;
    layout_work.appendix.page_position = pos_left;
    layout_work.appendix.number_style = b_style;
    layout_work.appendix.page_eject = ej_yes;
    layout_work.appendix.line_break = true;
    layout_work.appendix.display_heading = true;
    layout_work.appendix.number_reset = true;
    layout_work.appendix.cases = case_mixed;
    p = &z0;
    to_internal_SU( &p, &layout_work.appendix.align );
    layout_work.appendix.header = true;
    strcpy( layout_work.appendix.string, "APPENDIX " );
    layout_work.appendix.page_reset = false;
    layout_work.appendix.section_eject = ej_yes;
    layout_work.appendix.columns = 1;

    /***********************************************************************/
    /*  :SL         values                                                 */
    /***********************************************************************/
    layout_work.sl.level = 1;
    p = &z0;
    to_internal_SU( &p, &layout_work.sl.left_indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.sl.right_indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.sl.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.sl.skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.sl.post_skip );

    layout_work.sl.spacing = 1;
    layout_work.sl.font = 0;

    /***********************************************************************/
    /*  :OL         values                                                 */
    /***********************************************************************/
    layout_work.ol.level = 1;
    p = &z0;
    to_internal_SU( &p, &layout_work.ol.left_indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.ol.right_indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.ol.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.ol.skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.ol.post_skip );
    p = &i04;
    to_internal_SU( &p, &layout_work.ol.align );

    layout_work.ol.spacing = 1;
    layout_work.ol.font = 0;
    layout_work.ol.number_style = h_style | xd_style;;
    layout_work.ol.number_font = 0;

    /***********************************************************************/
    /*  :UL         values                                                 */
    /***********************************************************************/
    layout_work.ul.level = 1;
    p = &z0;
    to_internal_SU( &p, &layout_work.ul.left_indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.ul.right_indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.ul.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.ul.skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.ul.post_skip );
    p = &i04;
    to_internal_SU( &p, &layout_work.ul.align );

    layout_work.ul.spacing = 1;
    layout_work.ul.font = 0;
    layout_work.ul.bullet = '*';
    layout_work.ul.bullet_translate = true;
    layout_work.ul.bullet_font = 0;

    /***********************************************************************/
    /*  :DL         values                                                 */
    /***********************************************************************/
    layout_work.dl.level = 1;
    p = &z0;
    to_internal_SU( &p, &layout_work.dl.left_indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.dl.right_indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.dl.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.dl.skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.dl.post_skip );
    p = &i1;
    to_internal_SU( &p, &layout_work.dl.align );

    layout_work.dl.spacing = 1;
    layout_work.dl.line_break = false;

    /***********************************************************************/
    /*  :GL         values                                                 */
    /***********************************************************************/
    layout_work.gl.level = 1;
    p = &z0;
    to_internal_SU( &p, &layout_work.gl.left_indent );
    p = &z0;
    to_internal_SU( &p, &layout_work.gl.right_indent );
    p = &n1;
    to_internal_SU( &p, &layout_work.gl.pre_skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.gl.skip );
    p = &n1;
    to_internal_SU( &p, &layout_work.gl.post_skip );
    p = &z0;
    to_internal_SU( &p, &layout_work.gl.align );

    layout_work.gl.spacing = 1;
    layout_work.gl.delim = ':';

    /***********************************************************************/
    /*  :BANNER     values                                                 */
    /***********************************************************************/

    banner_defaults();

}





void    init_page_geometry( void )
{
    int         k;
    uint32_t    tm;
    uint32_t    lm;
    uint32_t    rm;
    uint32_t    offset;


    g_max_char_width = 0;
    g_max_line_height = 0;

    for( k = 0; k < wgml_font_cnt; k++ ) {
        if( g_max_char_width < wgml_fonts[k].default_width ) \
            g_max_char_width = wgml_fonts[k].default_width;
        if( g_max_line_height < wgml_fonts[k].line_height ) \
            g_max_line_height = wgml_fonts[k].line_height;
    }


    tm = conv_vert_unit( &layout_work.page.top_margin );
    lm = conv_hor_unit( &layout_work.page.left_margin );

    rm = conv_hor_unit( &layout_work.page.right_margin )
         - bin_device->x_offset;

    g_page_depth = conv_vert_unit( &layout_work.page.depth )
                   - bin_device->y_offset;

    if( GlobalFlags.firstpass && GlobalFlags.research ) {
        out_msg( "\ntm:%d lm:%d rm:%d depth:%d\n", tm, lm, rm,
                 g_page_depth );
    }

    g_page_left = max( lm, bin_device->x_start );
    g_page_right = min( rm, bin_device->page_width - bin_device->x_offset );

    if( bin_driver->y_positive == 0 ) {
        g_page_top = min( bin_device->page_depth - tm,
                          bin_device->y_start );
        g_page_bottom = max( g_page_top - g_page_depth, bin_device->y_offset );
        g_net_page_height = g_page_top - g_page_bottom;

    } else {
        g_page_top = max( tm, bin_device->y_start );
        g_page_bottom = min( g_page_top + g_page_depth, bin_device->y_offset );
        g_net_page_height = g_page_bottom - g_page_top;
    }
    g_net_page_width = g_page_right - g_page_left;

    g_ll = g_page_right - g_page_left;  // line length
    g_cd = layout_work.defaults.columns;// no of columns
    g_gutter = conv_hor_unit( &layout_work.defaults.gutter );

    if( g_cd > 1 ) {                    // multi column layout
        if( g_cd > 9 ) {
                                        // no more than 9 columns
            g_cd = 9;                   // this limit is found in script_tso.txt
                                        // for .cd control word
        }
        g_cl = (g_page_right - g_page_left - (g_cd -1) * g_gutter )
                / (g_cd - 1);           // column length
        offset = g_page_left;
        for( k = 0; k < 9; ++k ) {
            g_offset[k] = offset;       // start of each column
            offset += g_cl + g_gutter;
        }
    } else {
        g_cl = g_ll;
    }
}



void    init_def_margins( void )
{
    char        buf[BUF_SIZE];

    g_cur_h_start = g_page_left;
    if( bin_driver->y_positive == 0x00 ) {
        g_cur_v_start = g_page_top - (1 * g_max_line_height);
    } else {
        g_cur_v_start = g_page_top + (1 * g_max_line_height);
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
                  "page top:%d bottom:%d left:%d right:%d\n",
                  g_page_top, g_page_bottom, g_page_left, g_page_right );
        out_msg( buf );
        snprintf( buf, buf_size,
           "page net heigth:%d width:%d line height:%d char width:%d\n\n",
                  g_net_page_height, g_net_page_width, g_max_line_height,
                  g_max_char_width );
        out_msg( buf );
    }
}


/***************************************************************************/
/*  Layout end processing / document start processing                      */
/*  will be call either before a non LAYOUT tag is processed, or when the  */
/*  first line without tags is found                                       */
/***************************************************************************/

void    do_layout_end_processing( void )
{

    if( !ProcFlags.fb_document_done ) {
        out_msg( "Formatting document\n" );
    }

    init_page_geometry();


    /* fb_document() needs to be done on the first pass only, but
     * also needs to be done immediately after the :ELAYOUT. tag.
     * This means that it may need to be relocated when layout
     * processing is implemented.
     */

    if( GlobalFlags.firstpass == 1) {
        fb_document();                 // DOCUMENT :PAUSE & :INIT processing.
    }

    ProcFlags.fb_document_done = true;

    init_def_margins();

    fb_position( g_cur_h_start, g_cur_v_start );

}
