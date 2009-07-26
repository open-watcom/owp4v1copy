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
* Description:  wgml type definitions for layout data
*
****************************************************************************/

#ifndef GTYPELAY_H_INCLUDED
#define GTYPELAY_H_INCLUDED

/***************************************************************************/
/*  Layout attributes as enum list                                         */
/*    these are used in LAYOUT tag processing to control the attributes    */
/***************************************************************************/

#define pick( name, funci, funco, result ) e_##name,
typedef enum lay_att {
    e_dummy_zero = 0,
#include "glayutil.h"
    e_dummy_max
} lay_att;

#undef pick

/***************************************************************************/
/*  definitions for number style                                           */
/***************************************************************************/

typedef enum num_style {
    h_style     = 0x0001,               // hindu arabic
    a_style     = 0x0002,               // lowercase alphabetic
    b_style     = 0x0004,               // uppercase alphabetic
    c_style     = 0x0080,               // uppercase roman
    r_style     = 0x0010,               // lowercase roman
    xd_style    = 0x0100,               // decimal point follows
    xp_style    = 0x0600,               // in parenthesis
    xpa_style   = 0x0200,               // only left parenthesis
    xpb_style   = 0x0400                // only right parenthesis
} num_style;


/***************************************************************************/
/*  definitions for default place for :FIG tag                             */
/***************************************************************************/

typedef enum def_place {
    top_place    = 1,
    bottom_place,
    inline_place
} def_place;

/***************************************************************************/
/*  definitions for frame   :FIG tag and others                            */
/***************************************************************************/

typedef enum def_frame_type {
    none,
    box_frame,
    rule_frame,
    char_frame
} def_frame_type;

typedef struct def_frame {
    def_frame_type      type;
    char                string[60];
} def_frame;

/***************************************************************************/
/*  definition for note_string for :NOTE tag and others                    */
/***************************************************************************/
#define xx_str      char

/***************************************************************************/
/*  definitions for :Hx tag number form and page position                  */
/*                                                                         */
/***************************************************************************/

typedef enum num_form {
    num_none,
    num_prop,
    num_new
} num_form;

typedef enum page_pos {
    pos_left,
    pos_right,
    pos_center
} page_pos;

typedef enum page_ej {
    ej_no,
    ej_yes,
    ej_odd,
    ej_even
} page_ej;

typedef enum case_t {
    case_mixed,
    case_lower,
    case_upper
} case_t;


/***************************************************************************/
/*  :PAGE    layout tag data                                               */
/***************************************************************************/
typedef struct page_lay_tag {
    su              top_margin;         // vertical space unit
    su              left_margin;        // horizontal space unit
    su              right_margin;       // horizontal space unit
    su              depth;              // vertical space unit
} page_lay_tag;


/***************************************************************************/
/*  :DEFAULT layout tag data                                               */
/***************************************************************************/

typedef struct default_lay_tag {
    int8_t          spacing;            // positive integer
    int8_t          columns;            // positive integer
    int8_t          font;               // non-negative integer
    bool            justify;            // yes / no  -> bool
    uint8_t         input_esc;          // none or quoted char
    su              gutter;             // horizontal space unit
    su              binding;            // horizontal space unit
} default_lay_tag;


/***************************************************************************/
/*  :WIDOW   layout tag data                                               */
/***************************************************************************/

typedef struct widow_lay_tag {
    int8_t          threshold;          // non-negative integer
} widow_lay_tag;


/***************************************************************************/
/*  :FN       Layout tag data                                              */
/***************************************************************************/

typedef struct fn_lay_tag {
    su              line_indent;        // horizontal space unit
    su              align;              // horizontal space unit
    su              pre_lines;          // vertical space unit
    su              skip;               // vertical space unit
    int8_t          spacing;            // positive integer
    int8_t          font;               // non-negative integer
    int8_t          number_font;        // non-negative integer
    num_style       number_style;       // special enum
    bool            frame;              // rule=1  none=0 bool
} fn_lay_tag;


/***************************************************************************/
/*  :FNREF    Layout tag data                                              */
/***************************************************************************/

typedef struct fnref_lay_tag {
    int8_t          font;               // non-negative integer
    num_style       number_style;       // special enum
} fnref_lay_tag;


/***************************************************************************/
/*  :P        Layout tag data                                              */
/***************************************************************************/

typedef struct p_lay_tag {
    su              line_indent;        // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
} p_lay_tag;


/***************************************************************************/
/*  :PC       Layout tag data                                              */
/***************************************************************************/

typedef struct pc_lay_tag {
    su              line_indent;        // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
} pc_lay_tag;


/***************************************************************************/
/*  :FIG      Layout tag data                                              */
/***************************************************************************/

typedef struct fig_lay_tag {
    su              left_adjust;        // horizontal space unit
    su              right_adjust;       // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    int8_t          spacing;            // positive integer
    int8_t          font;               // non-negative integer
    def_place       default_place;      // special enum
    def_frame       default_frame;      // special
} fig_lay_tag;


/***************************************************************************/
/*  :XMP      Layout tag data                                              */
/***************************************************************************/

typedef struct xmp_lay_tag {
    su              left_indent;        // horizontal space unit
    su              right_indent;       // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    int8_t          spacing;            // positive integer
    int8_t          font;               // non-negative integer
} xmp_lay_tag;


/***************************************************************************/
/*  :NOTE     Layout tag data                                              */
/***************************************************************************/
#define str_size   40                   // no value shown in documentation

typedef struct note_lay_tag {
    su              left_indent;        // horizontal space unit
    su              right_indent;       // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    int8_t          font;               // non-negative integer
    int8_t          spacing;            // positive integer
    xx_str          string[str_size];   // special string
} note_lay_tag;

/***************************************************************************/
/*  :H0 - :H6 Layout tag data                                              */
/***************************************************************************/

typedef struct hx_lay_tag {
    int8_t          group;              // 0 - 9
    su              indent;             // horizontal space unit
    su              pre_top_skip;       // vertical space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    su              align;              // horizontal space unit
    int8_t          spacing;            // positive integer
    int8_t          font;               // non-negative integer
    int8_t          number_font;        // non-negative integer
    num_form        number_form;        // special enum ( none, prop, new )
    page_pos        page_position;      // special enum (left, right, center)
    num_style       number_style;       // special enum
    page_ej         page_eject;         // enum for yes, no, odd, even
    bool            line_break;         // yes, no -> bool
    bool            display_heading;    // yes, no -> bool
    bool            number_reset;       // yes, no -> bool
    case_t          cases;              // lower, upper, mixed
} hx_lay_tag;


/***************************************************************************/
/*  :HEADING Layout tag data                                               */
/***************************************************************************/

typedef struct heading_lay_tag {
    char            delim;
    bool            stop_eject;         // yes, no -> bool
    bool            para_indent;        // yes, no -> bool
    int8_t          threshold;          // non-negative integer
    int8_t          max_group;          // non-negative integer
} heading_lay_tag;

/***************************************************************************/
/*  :LQ      Layout tag data                                               */
/***************************************************************************/

typedef struct lq_lay_tag {
    su              left_indent;        // horizontal space unit
    su              right_indent;       // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    int8_t          spacing;            // positive integer
    int8_t          font;               // non-negative integer
} lq_lay_tag;

/***************************************************************************/
/* :DT :GT :DTHD :CIT :GD :DDHD :IXPGNUM :IXMAJOR                          */
/*            Layout tag data  these only have a font value                */
/***************************************************************************/

typedef struct xx_lay_tag {
    int8_t          font;               // non-negative integer
} xx_lay_tag;

/***************************************************************************/
/*  :FIGCAP  Layout tag data                                               */
/***************************************************************************/

typedef struct figcap_lay_tag {
    su              pre_lines;          // vertical space unit
    int8_t          font;               // non-negative integer
    xx_str          string[str_size];   // special string
    int8_t          string_font;        // non-negative integer
    char            delim;
} figcap_lay_tag;

/***************************************************************************/
/*  :FIGDESC Layout tag data                                               */
/***************************************************************************/

typedef struct figdesc_lay_tag {
    su              pre_lines;          // vertical space unit
    int8_t          font;               // non-negative integer
} figdesc_lay_tag;

/***************************************************************************/
/*  :FIGLIST        Layout tag data                                        */
/***************************************************************************/

typedef struct figlist_lay_tag {
    su              left_adjust;        // horizontal space unit
    su              right_adjust;       // horizontal space unit
    su              skip;               // vertical space unit
    int8_t          spacing;            // positive integer
    int8_t          columns;            // positive integer
    int8_t          toc_levels;         // non-negative integer
    xx_str          fill_string[str_size];  // special string
} figlist_lay_tag;

/***************************************************************************/
/*  :FLPGNUM        Layout tag data                                        */
/***************************************************************************/

typedef struct flpgnum_lay_tag {
    su              size;               // horizontal space unit
    int8_t          font;               // non-negative integer
    xx_str          fill_string[str_size];  // special string
} flpgnum_lay_tag;

/***************************************************************************/
/*  :DD      Layout tag data                                               */
/***************************************************************************/

typedef struct dd_lay_tag {
    su              line_left;          // horizontal space unit
    int8_t          font;               // non-negative integer
} dd_lay_tag;

/***************************************************************************/
/*  :ABSTRACT and :PREFACE  Layout tag data                                */
/***************************************************************************/

typedef struct abspref_lay_tag {
    su              post_skip;          // vertical space unit
    su              pre_top_skip;       // vertical space unit
    int8_t          font;               // non-negative integer
    int8_t          spacing;            // positive integer
    bool            header;             // yes, no -> bool
    xx_str          string[str_size];   // special string
    page_ej         page_eject;         // enum for yes, no, odd, even
    bool            page_reset;         // yes, no -> bool
    int8_t          columns;            // positive integer
} abspref_lay_tag;

/***************************************************************************/
/*  :BACKM and :BODY  Layout tag data                                      */
/***************************************************************************/

typedef struct backbod_lay_tag {
    su              post_skip;          // vertical space unit
    su              pre_top_skip;       // vertical space unit
    bool            header;             // yes, no -> bool
    xx_str          string[str_size];   // special string
    page_ej         page_eject;         // enum for yes, no, odd, even
    bool            page_reset;         // yes, no -> bool
    int8_t          font;               // non-negative integer
                            // columns leave last
    int8_t          columns;            // positive integer
} backbod_lay_tag;

/***************************************************************************/
/*  :LP             Layout tag data                                        */
/***************************************************************************/

typedef struct lp_lay_tag {
    su              left_indent;        // horizontal space unit
    su              right_indent;       // horizontal space unit
    su              line_indent;        // horizontal space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    int8_t          spacing;            // positive integer
} lp_lay_tag;

/***************************************************************************/
/*  :INDEX          Layout tag data                                        */
/***************************************************************************/

typedef struct index_lay_tag {
    su              post_skip;          // vertical space unit
    su              pre_top_skip;       // vertical space unit
    su              left_adjust;        // horizontal space unit
    su              right_adjust;       // horizontal space unit
    int8_t          spacing;            // positive integer
    int8_t          columns;            // positive integer
    xx_str          see_string[str_size];// special string
    xx_str          see_also_string[str_size];  // special string
    xx_str          index_string[str_size];// special string
    bool            header;             // yes, no -> bool
    page_ej         page_eject;         // enum for yes, no, odd, even
    bool            page_reset;         // yes, no -> bool
    int8_t          font;               // non-negative integer
} index_lay_tag;

/***************************************************************************/
/*  :IXHEAD         Layout tag data                                        */
/***************************************************************************/

typedef struct ixhead_lay_tag {
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    su              indent;             // horizontal space unit
    def_frame       frame;              // special
    int8_t          font;               // non-negative integer
    bool            header;             // yes, no -> bool
} ixhead_lay_tag;

/***************************************************************************/
/*  :I1 :I2 :I3     Layout tag data                                        */
/***************************************************************************/

typedef struct ix_lay_tag {
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    su              skip;               // vertical space unit
    su              indent;             // horizontal space unit
    su              wrap_indent;        // horizontal space unit
    xx_str          index_delim[str_size];  // special string
    int8_t          font;               // non-negative integer
    int8_t          string_font;        // non-negative integer
} ix_lay_tag;

/***************************************************************************/
/*  :TOC            Layout tag data                                        */
/***************************************************************************/

typedef struct toc_lay_tag {
    su              left_adjust;        // horizontal space unit
    su              right_adjust;       // horizontal space unit
    int8_t          spacing;            // positive integer
    int8_t          columns;            // positive integer
    int8_t          toc_levels;         // non-negative integer
    xx_str          fill_string[str_size];  // special string
} toc_lay_tag;

/***************************************************************************/
/*  :TOCPGNUM       Layout tag data                                        */
/***************************************************************************/

typedef struct tocpgnum_lay_tag {
    su              size;               // horizontal space unit
    int8_t          font;               // non-negative integer
} tocpgnum_lay_tag;

/***************************************************************************/
/*  :TOCH0 - TOCH6  Layout tag data                                        */
/***************************************************************************/

typedef struct tochx_lay_tag {
    int8_t          group;              // 0 - 9
    su              indent;             // horizontal space unit
    su              skip;               // vertical space unit
    su              pre_skip;           // vertical space unit
    su              post_skip;          // vertical space unit
    su              align;              // horizontal space unit
    int8_t          font;               // non-negative integer
    bool            display_in_toc;     // yes, no -> bool
} tochx_lay_tag;





/***************************************************************************/
/*  Layout data stored as default or overriden by input     TBD            */
/*                                                                         */
/***************************************************************************/

typedef struct layout_data {
    page_lay_tag        page;
    default_lay_tag     defaults;
    widow_lay_tag       widow;
    fn_lay_tag          fn;
    fnref_lay_tag       fnref;
    p_lay_tag           p;
    pc_lay_tag          pc;
    fig_lay_tag         fig;
    xmp_lay_tag         xmp;
    note_lay_tag        note;
    hx_lay_tag          hx[7];
    heading_lay_tag     heading;
    lq_lay_tag          lq;
    xx_lay_tag          dt;
    xx_lay_tag          gt;
    xx_lay_tag          dthd;
    xx_lay_tag          cit;
    figcap_lay_tag      figcap;
    figdesc_lay_tag     figdesc;
    dd_lay_tag          dd;
    xx_lay_tag          gd;
    xx_lay_tag          ddhd;
    abspref_lay_tag     abstract;
    abspref_lay_tag     preface;
    backbod_lay_tag     body;
    backbod_lay_tag     backm;
    lp_lay_tag          lp;
    index_lay_tag       index;
    xx_lay_tag          ixpgnum;
    xx_lay_tag          ixmajor;
    ixhead_lay_tag      ixhead;
    ix_lay_tag          ix[3];
    toc_lay_tag         toc;
    tocpgnum_lay_tag    tocpgnum;
    tochx_lay_tag       tochx[7];
    figlist_lay_tag     figlist;
} layout_data;


/***************************************************************************/
/*  parameter structure for parsing layout attributes name + value         */
/***************************************************************************/

typedef struct att_args {
    char    *   start[2];
    int         len[2];
} att_args;


#endif  /* GTYPELAY_H_INCLUDED */
