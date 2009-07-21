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
/*  definitions for default frame for :FIG tag                             */
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
} layout_data;


/***************************************************************************/
/*  parameter structure for parsing layout attributes name + value         */
/***************************************************************************/

typedef struct att_args {
    char    *   start[2];
    int         len[2];
} att_args;


#endif  /* GTYPELAY_H_INCLUDED */
