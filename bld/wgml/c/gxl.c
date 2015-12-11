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
* Description:  WGML tags :OL, :SL, :UL  :DL, :GL  and corresponding
*                         :eXX processing
*                         :LI and :LP processing
*               only some of the attributes are supported   TBD
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static  uint8_t     dl_cur_level    = 1;    // current DL list level
static  uint8_t     gl_cur_level    = 1;    // current GL list level
static  uint8_t     ol_cur_level    = 1;    // current OL list level
static  uint8_t     sl_cur_level    = 1;    // current SL list level
static  uint8_t     ul_cur_level    = 1;    // current UL list level


/***************************************************************************/
/*  end_lp  processing as if the non existant :eLP tag was seen            */
/***************************************************************************/
static void end_lp( void )
{
    tag_cb  *   wk;

    if( nest_cb->c_tag == t_LP ) {      // terminate current :LP
        wk = nest_cb;
        nest_cb = nest_cb->prev;
        add_tag_cb_to_pool( wk );
    }
}


/***************************************************************************/
/*  :SL, ... common processing                                             */
/***************************************************************************/

static void gml_xl_lp_common( e_tags t )
{
    char        *   p;

    end_lp();                           // terminate :LP if active

    init_nest_cb();
    nest_cb->p_stack = copy_to_nest_stack();

    nest_cb->c_tag = t;

    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                    // possible tag end
    if( t != t_LP ) {                       // text only allowed for :LP
        if( t != t_DL && t != t_GL ) {      // DL/GL don't require LI/LP
            ProcFlags.need_li_lp = true;    // :LI or :LP  next
        }
        start_doc_sect();                   // if not already done
        if( g_line_indent == 0 ) {
            ProcFlags.para_starting = false;    // clear for this tag's first break
        }
        scr_process_break();
        if( *p ) {
            process_text( p, g_curr_font );
        }
    }

    return;
}


/***************************************************************************/
/* Format:  :DL [compact]                                                  */
/*              [break]                                                    */
/*              [headhi=head-highlight]                                    */
/*              [termhi=term-highlight]                                    */
/*              [tsize='hor-space-unit'].                                  */
/*                                                                         */
/* The definition list tag signals the start of a definition list.  Each   */
/* list item in a definition list has two parts.  The first part is the    */
/* definition term and is defined with the :dt tag.  The second part is    */
/* the definition description and is defined with the :dd tag.  A          */
/* corresponding :edl tag must be specified for each :dl tag.  The compact */
/* corresponding :edl tag must be specified for each :dl tag.  The compact */
/* attribute indicates that the list items should be compacted.  Blank     */
/* lines that are normally placed between the list items will be           */
/* suppressed.  The compact attribute is one of the few WATCOM Script/GML  */
/* attributes which does not have an attribute value associated with it.   */
/* The break attribute indicates that the definition description should be */
/* started on a new output line if the size of the definition term exceeds */
/* the maximum horizontal space normally allowed for it.  If this          */
/* attribute is not specified, the definition description will be placed   */
/* after the definition term.  The break attribute is one of the few       */
/* WATCOM Script/GML attributes which does not have an attribute value     */
/* associated with it.  The headhi attribute allows you to set the         */
/* highlighting level of the definition list headings.  Non-negative       */
/* integer numbers are valid highlighting values.  The termhi attribute    */
/* allows you to set the highlighting level of the definition term.        */
/* Non-negative integer numbers are valid highlighting values.  The tsize  */
/* attribute allows you to set the minimum horizontal space taken by the   */
/* definition term.  Any valid horizontal space unit may be specified.     */
/* The attribute value is linked to the font of the :DT tag if the termhi  */
/* attribute is not specified.                                             */
/***************************************************************************/

void gml_dl( const gmltag * entry )  // not tested TBD
{
    bool                compact;
    char            *   p;
    dl_lay_level    *   dl_layout   = NULL;

    p = scan_start;
    p++;
    while( *p == ' ' ) {
        p++;
    }
    scan_start = p;                     // over spaces
    if( !strnicmp( "compact", p, 7 ) ) {
        compact = true;
        scan_start = p + 7;
    } else {
        compact = false;
    }
    gml_xl_lp_common( t_DL );

    dl_layout = layout_work.dl.first;
    while( (dl_layout != NULL) && (dl_cur_level < dl_layout->level) ) {
        dl_layout = dl_layout->next;
    }

    if( dl_layout == NULL ) {
        internal_err( __FILE__, __LINE__ );
    }

    if( dl_cur_level < layout_work.dl.max_level ) {
        dl_cur_level++;
    } else {
        dl_cur_level = 1;
    }

    nest_cb->compact = compact;

    nest_cb->li_number = 0;
    nest_cb->align = conv_hor_unit( &dl_layout->align );
    nest_cb->left_indent = conv_hor_unit( &dl_layout->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    nest_cb->right_indent = -1 * conv_hor_unit( &dl_layout->right_indent )
                            + nest_cb->prev->right_indent;
    nest_cb->xl_pre_skip = dl_layout->pre_skip;
    nest_cb->lay_tag = dl_layout;

    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;

    spacing = (int8_t) dl_layout->spacing;

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/* Format:  :GL [compact] [termhi=term-highlight].                         */
/*                                                                         */
/* The glossary list tag signals the start of a glossary list, and is      */
/* usually used in the back material section.  Each list item in a         */
/* glossary list has two parts.  The first part is the glossary term and   */
/* is defined with the :gt tag.  The second part is the glossary           */
/* description and is defined with the :gd tag.  A corresponding :egl tag  */
/* must be specified for each :gl tag.  The compact attribute indicates    */
/* that the list items should be compacted.  Blank lines that are normally */
/* placed between the list items will be suppressed.  The compact          */
/* attribute is one of the few WATCOM Script/GML attributes which does not */
/* have an attribute value associated with it.  The termhi attribute       */
/* allows you to set the highlighting level of the glossary term.          */
/* Non-negative integer numbers are valid highlighting values.             */
/***************************************************************************/

void gml_gl( const gmltag * entry )  // not tested TBD
{
    bool                compact;
    char            *   p;
    gl_lay_level    *   gl_layout   = NULL;

    p = scan_start;
    p++;
    while( *p == ' ' ) {
        p++;
    }
    scan_start = p;                     // over spaces
    if( !strnicmp( "compact", p, 7 ) ) {
        compact = true;
        scan_start = p + 7;
    } else {
        compact = false;
    }
    gml_xl_lp_common( t_GL );

    gl_layout = layout_work.gl.first;
    while( (gl_layout != NULL) && (gl_cur_level < gl_layout->level) ) {
        gl_layout = gl_layout->next;
    }

    if( gl_layout == NULL ) {
        internal_err( __FILE__, __LINE__ );
    }

    if( gl_cur_level < layout_work.gl.max_level ) {
        gl_cur_level++;
    } else {
        gl_cur_level = 1;
    }

    nest_cb->compact = compact;

    nest_cb->li_number = 0;
    nest_cb->align = conv_hor_unit( &gl_layout->align );
    nest_cb->left_indent = conv_hor_unit( &gl_layout->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    nest_cb->right_indent = -1 * conv_hor_unit( &gl_layout->right_indent )
                            + nest_cb->prev->right_indent;
    nest_cb->xl_pre_skip = gl_layout->pre_skip;
    nest_cb->lay_tag = gl_layout;

    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;

    spacing = (int8_t) gl_layout->spacing;

    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*Format: :OL [compact].                                                   */
/*                                                                         */
/*This tag signals the start of an ordered list. Items in the list are     */
/*specified using the :li tag. The list items are preceded by the number   */
/*of the list item. The layout determines the style of the number. An      */
/*ordered list may be used wherever a basic document element is permitted  */
/*to appear. A corresponding :eol tag must be specified for each :ol tag.  */
/*                                                                         */
/*The compact attribute indicates that the list items should be compacted. */
/*Blank lines that are normally placed between the list items will be      */
/*suppressed. The compact attribute is one of the few WATCOM Script/GML    */
/*attributes which does not have an attribute value associated with it.    */
/***************************************************************************/

void gml_ol( const gmltag * entry )
{
    bool                compact;
    char            *   p;
    ol_lay_level    *   ol_layout   = NULL;

    p = scan_start + 1;
    while( *p == ' ' ) {
        p++;
    }
    scan_start = p;
    if( !strnicmp( "compact", p, 7 ) ) {
        compact = true;
        scan_start = p + 7;
    } else {
        compact = false;
    }
    if( ProcFlags.need_li_lp ) {
        xx_nest_err( err_no_li_lp );
    }
    gml_xl_lp_common( t_OL );

    ol_layout = layout_work.ol.first;
    while( (ol_layout != NULL) && (ol_cur_level < ol_layout->level) ) {
        ol_layout = ol_layout->next;
    }

    if( ol_layout == NULL ) {
        internal_err( __FILE__, __LINE__ );
    }

    if( ol_cur_level < layout_work.ol.max_level ) {
        ol_cur_level++;
    } else {
        ol_cur_level = 1;
    }

    nest_cb->compact = compact;

    nest_cb->li_number = 0;
    nest_cb->left_indent = conv_hor_unit( &ol_layout->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    nest_cb->right_indent = -1 * conv_hor_unit( &ol_layout->right_indent )
                            + nest_cb->prev->right_indent;
    nest_cb->xl_pre_skip = ol_layout->pre_skip;
    nest_cb->lay_tag = ol_layout;

    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;

    spacing = (int8_t) ol_layout->spacing;

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/* Format:  :SL [compact].                                                 */
/*                                                                         */
/* This tag signals the start of a simple list.                            */
/* Items in the list are specified using the :li tag.  A simple list may   */
/* occur wherever a basic document element is permitted to appear.  A      */
/* corresponding :esl tag must be specified for each :sl tag.  The compact */
/* attribute indicates that the list items should be compacted.  Blank     */
/* lines that are normally placed between the list items will be           */
/* suppressed.  The compact attribute is one of the few WATCOM Script/GML  */
/* attributes which does not have an attribute value associated with it.   */
/*                                                                         */
/***************************************************************************/

void gml_sl( const gmltag * entry )
{
    bool                compact;
    char            *   p;
    sl_lay_level    *   sl_layout   = NULL;

    p = scan_start + 1;
    while( *p == ' ' ) {
        p++;
    }
    scan_start = p;                     // over spaces
    if( !strnicmp( "compact", p, 7 ) ) {
        compact = true;
        scan_start = p + 7;
    } else {
        compact = false;
    }
    if( ProcFlags.need_li_lp ) {
        xx_nest_err( err_no_li_lp );
    }
    gml_xl_lp_common( t_SL );

    sl_layout = layout_work.sl.first;
    while( (sl_layout != NULL) && (sl_cur_level < sl_layout->level) ) {
        sl_layout = sl_layout->next;
    }

    if( sl_layout == NULL ) {
        internal_err( __FILE__, __LINE__ );
    }

    if( sl_cur_level < layout_work.sl.max_level ) {
        sl_cur_level++;
    } else {
        sl_cur_level = 1;
    }

    nest_cb->compact = compact;

    nest_cb->li_number = 0;
    nest_cb->left_indent = conv_hor_unit( &sl_layout->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    nest_cb->right_indent = -1 * conv_hor_unit( &sl_layout->right_indent )
                            + nest_cb->prev->right_indent;
    nest_cb->xl_pre_skip = sl_layout->pre_skip;
    nest_cb->lay_tag = sl_layout;

    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;

    spacing = (int8_t) sl_layout->spacing;

    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/* Format:  :UL [compact].                                                 */
/*                                                                         */
/* This tag signals the start of an unordered                              */
/* list.  Items in the list are specified using the :li tag.  The list     */
/* items are preceded by a symbol such as an asterisk or a bullet.  This   */
/* tag may be used wherever a basic document element is permitted to       */
/* appear.  A corresponding :eul tag must be specified for each :ul tag.   */
/*                                                                         */
/* The compact attribute indicates that the list items should be           */
/* compacted.  Blank lines that are normally placed between the list items */
/* will be suppressed.  The compact attribute is one of the few WATCOM     */
/* Script/GML attributes which does not have an attribute value associated */
/* with it.                                                                */
/*                                                                         */
/***************************************************************************/

void gml_ul( const gmltag * entry )
{
    bool                compact;
    char            *   p;
    ul_lay_level    *   ul_layout   = NULL;

    p = scan_start + 1;
    while( *p == ' ' ) {
        p++;
    }
    scan_start = p;                     // over spaces
    if( !strnicmp( "compact", p, 7 ) ) {
        compact = true;
        scan_start = p + 7;
    } else {
        compact = false;
    }
    if( ProcFlags.need_li_lp ) {
        xx_nest_err( err_no_li_lp );
    }
    gml_xl_lp_common( t_UL );

    ul_layout = layout_work.ul.first;
    while( (ul_layout != NULL) && (ul_cur_level < ul_layout->level) ) {
        ul_layout = ul_layout->next;
    }

    if( ul_layout == NULL ) {
        internal_err( __FILE__, __LINE__ );
    }

    if( ul_cur_level < layout_work.ul.max_level ) {
        ul_cur_level++;
    } else {
        ul_cur_level = 1;
    }

    nest_cb->compact = compact;

    nest_cb->li_number = 0;
    nest_cb->left_indent = conv_hor_unit( &ul_layout->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    nest_cb->right_indent = -1 * conv_hor_unit( &ul_layout->right_indent )
                            + nest_cb->prev->right_indent;
    nest_cb->xl_pre_skip = ul_layout->pre_skip;
    nest_cb->lay_tag = ul_layout;

    nest_cb->lm = g_cur_left;
    nest_cb->rm = g_page_right;

    spacing = (int8_t) ul_layout->spacing;

    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*  common :eXXX processing                                                */
/***************************************************************************/

void    gml_exl_common( const gmltag * entry, e_tags t )
{
    char    *   p;
    tag_cb  *   wk;

    if( nest_cb->c_tag == t_LP ) {      // terminate :LP if active
        end_lp();
    }

    if( nest_cb->c_tag != t ) {         // unexpected exxx tag
        if( nest_cb->c_tag == t_NONE ) {
            g_err_tag_no( str_tags[t + 1] );// no exxx expected, no tag active
        } else {
            g_err_tag_nest( str_tags[nest_cb->c_tag + 1] ); // exxx expected
        }
    } else {
        g_cur_left = nest_cb->lm;
        g_page_right = nest_cb->rm;

        wk = nest_cb;
        nest_cb = nest_cb->prev;
        add_tag_cb_to_pool( wk );
        g_curr_font = nest_cb->font;

        g_cur_h_start = g_cur_left;
        scan_err = false;
        p = scan_start;
        if( *p == '.' ) p++;            // over '.'
        if( *p ) {
            gml_pc( NULL );
        } else {
            ProcFlags.skips_valid = false;  // force use of post_skip with following text element
        }
    }

    ProcFlags.need_li_lp = false;       // :LI or :LP no longer needed
    scan_start = scan_stop + 1;
}


/***************************************************************************/
/* Format:  :eDL.                                                          */
/*                                                                         */
/* This tag signals the end of a definition list.  A                       */
/* corresponding :DL tag must be previously specified for each :eDL tag.   */
/*                                                                         */
/*                                                                         */
/* Format:  :eGL.                                                          */
/*                                                                         */
/* This tag signals the end of a glossary list.  A                         */
/* corresponding :GL tag must be previously specified for each :eGL tag.   */
/*                                                                         */
/*                                                                         */
/* Format:  :eOL.                                                          */
/*                                                                         */
/* This tag signals the end of an ordered list.  A                         */
/* corresponding :OL tag must be previously specified for each :eOL tag.   */
/*                                                                         */
/*                                                                         */
/* Format:  :eSL.                                                          */
/*                                                                         */
/* This tag signals the end of a simple list.  A                           */
/* corresponding :SL tag must be previously specified for each :eSL tag.   */
/*                                                                         */
/*                                                                         */
/* Format:  :eUL.                                                          */
/*                                                                         */
/* This tag signals the end of a unordered list.  A                        */
/* corresponding :UL tag must be previously specified for each :eUL tag.   */
/*                                                                         */
/***************************************************************************/

void    gml_edl( const gmltag * entry ) // not tested TBD
{
    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's break
    }
    scr_process_break();
    set_skip_vars( NULL, NULL, &((dl_lay_level *)(nest_cb->lay_tag))->post_skip, 1, g_curr_font );
    gml_exl_common( entry, t_DL );
    if( dl_cur_level == 1 ) {
        dl_cur_level = layout_work.dl.max_level;
    } else {
        dl_cur_level++;
    }
}

void    gml_egl( const gmltag * entry ) // not tested TBD
{
    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's break
    }
    scr_process_break();
    set_skip_vars( NULL, NULL, &((gl_lay_level *)(nest_cb->lay_tag))->post_skip, 1, g_curr_font );
    gml_exl_common( entry, t_GL );
    if( gl_cur_level == 1 ) {
        gl_cur_level = layout_work.gl.max_level;
    } else {
        gl_cur_level++;
    }
}

void    gml_eol( const gmltag * entry )
{
    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's break
    }
    scr_process_break();
    set_skip_vars( NULL, NULL, &((ol_lay_level *)(nest_cb->lay_tag))->post_skip, 1, g_curr_font );
    gml_exl_common( entry, t_OL );
    if( ol_cur_level == 1 ) {
        ol_cur_level = layout_work.ol.max_level;
    } else {
        ol_cur_level++;
    }
}

void    gml_esl( const gmltag * entry )
{
    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's break
    }
    scr_process_break();
    set_skip_vars( NULL, NULL, &((sl_lay_level *)(nest_cb->lay_tag))->post_skip, 1, g_curr_font );
    gml_exl_common( entry, t_SL );
    if( sl_cur_level == 1 ) {
        sl_cur_level = layout_work.sl.max_level;
    } else {
        sl_cur_level++;
    }
}

void    gml_eul( const gmltag * entry )
{
    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's break
    }
    scr_process_break();
    set_skip_vars( NULL, NULL, &((ul_lay_level *)(nest_cb->lay_tag))->post_skip, 1, g_curr_font );
    gml_exl_common( entry, t_UL );
    if( ul_cur_level == 1 ) {
        ul_cur_level = layout_work.ul.max_level;
    } else {
        ul_cur_level++;
    }
}


/***************************************************************************/
/* :LI within :OL tag                                                      */
/***************************************************************************/

static  void    gml_li_ol( const gmltag * entry )
{
    char            charnumber[MAX_L_AS_STR];
    char        *   p;
    char        *   pn;
    int32_t         t_align         = 0;
    int32_t         t_left_indent_1 = 0;
    int32_t         t_left_indent_2 = 0;
    int32_t         t_right_indent  = 0;
    uint32_t        num_len;

    scan_err = false;
    p = scan_start;

    if( nest_cb == NULL ) {
        xx_nest_err( err_li_lp_no_list );   // tag must be in a list
        scan_start = scan_stop + 1;
        return;
    }

    t_align = conv_hor_unit( &((ol_lay_level *)(nest_cb->lay_tag))->align );

    nest_cb->li_number++;
    pn = format_num( nest_cb->li_number, charnumber, MAX_L_AS_STR,
                     ((ol_lay_level *)(nest_cb->lay_tag))->number_style );
    if( pn != NULL ) {
        num_len = strlen( pn );
        *(pn + num_len) = ' ';          // trailing space like wgml4 does
        *(pn + num_len + 1) = '\0';
        num_len++;
    } else {
        pn = charnumber;
        *pn = '?';
        *(pn + 1) = 0;
        num_len = 1;
    }

    scr_process_break();

    g_curr_font = ((ol_lay_level *)(nest_cb->lay_tag))->number_font;

    if( ProcFlags.need_li_lp ) {        // first :li for this list
        set_skip_vars( &nest_cb->xl_pre_skip, NULL, NULL, 1, g_curr_font );
    } else if( !nest_cb->compact ) {
        set_skip_vars( &((ol_lay_level *)(nest_cb->lay_tag))->skip, NULL,
                       NULL, 1, g_curr_font );
    } else {                            // compact
        set_skip_vars( NULL, NULL, NULL, 1, g_curr_font );
    }

    post_space = 0;

    /* Use original indents, not values possibly modified by IN */

    if( g_indent > 0 ) {
        t_left_indent_1 = conv_hor_unit( &((ol_lay_level *)(nest_cb->lay_tag))->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    } else {
        t_left_indent_1 = conv_hor_unit( &((ol_lay_level *)(nest_cb->lay_tag))->left_indent )
                            + nest_cb->prev->left_indent;
    }
    t_left_indent_2 = conv_hor_unit( &((ol_lay_level *)(nest_cb->lay_tag))->left_indent );
    t_right_indent = -1 * conv_hor_unit( &((ol_lay_level *)(nest_cb->lay_tag))->right_indent )
                            + nest_cb->prev->right_indent;
    g_cur_left = nest_cb->lm + t_left_indent_2;
    g_page_right = nest_cb->rm + t_right_indent;

    g_cur_h_start = g_cur_left;
    ProcFlags.keep_left_margin = true;  // keep special Note indent
    start_line_with_string( charnumber, g_curr_font, true );

    g_cur_h_start = g_cur_left + t_align;

    if( t_line != NULL ) {
        if( t_line->last != NULL ) {
            g_cur_left += t_line->last->width + post_space;
        }
    }
    post_space = 0;
    if( g_cur_h_start > g_cur_left ) {
        g_cur_left = g_cur_h_start;
    }
    g_cur_h_start = g_cur_left;
    ju_x_start = g_cur_h_start;

    g_curr_font = ((ol_lay_level *)(nest_cb->lay_tag))->font;
    if( *p == '.' ) p++;                // over '.'
    while( *p == ' ' ) p++;             // skip initial spaces
    ProcFlags.need_li_lp = false;       // 1. item in list processed
    if( *p ) {
        process_text( p, g_curr_font ); // if text follows
    }

    nest_cb->align = t_align;

    /* Set indents to their original values for the next LI */

    if( g_indent > 0 ) {
        nest_cb->left_indent = t_left_indent_1;
    } else {
        nest_cb->left_indent = t_left_indent_1 + nest_cb->prev->align;
    }
    nest_cb->right_indent = t_right_indent;
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/* :LI within :SL tag                                                      */
/***************************************************************************/

static  void    gml_li_sl( const gmltag * entry )
{
    char        *   p;
    int32_t         t_left_indent_1 = 0;
    int32_t         t_left_indent_2 = 0;
    int32_t         t_right_indent  = 0;

    scan_err = false;
    p = scan_start;

    if( nest_cb == NULL ) {
        xx_nest_err( err_li_lp_no_list );   // tag must be in a list
        scan_start = scan_stop + 1;
        return;
    }

    scr_process_break();

    if( ProcFlags.need_li_lp ) {        // first :li for this list
        set_skip_vars( &nest_cb->xl_pre_skip, NULL, NULL, 1, g_curr_font );
    } else if( !nest_cb->compact ) {
        set_skip_vars( &((sl_lay_level *)(nest_cb->lay_tag))->skip, NULL,
                       NULL, 1, g_curr_font );
    } else {                            // compact
        set_skip_vars( NULL, NULL, NULL, 1, g_curr_font );
    }

    ProcFlags.keep_left_margin = true;  // keep special Note indent

    /* Use original indents, not values possibly modified by IN */

    t_left_indent_1 = conv_hor_unit( &((sl_lay_level *)(nest_cb->lay_tag))->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    t_left_indent_2 = conv_hor_unit( &((sl_lay_level *)(nest_cb->lay_tag))->left_indent );
    t_right_indent = -1 * conv_hor_unit( &((sl_lay_level *)(nest_cb->lay_tag))->right_indent )
                            + nest_cb->prev->right_indent;
    g_cur_left = nest_cb->lm + t_left_indent_2;
    g_page_right = nest_cb->rm + t_right_indent;

    g_cur_h_start = g_cur_left;
    post_space = 0;
    ju_x_start = g_cur_h_start;

    g_curr_font = ((sl_lay_level *)(nest_cb->lay_tag))->font;
    if( *p == '.' ) p++;                // over '.'
    while( *p == ' ' ) p++;             // skip initial spaces
    ProcFlags.need_li_lp = false;
    if( *p ) {
        process_text( p, g_curr_font ); // if text follows
    }

    /* Set indents to their original values for the next LI */

    nest_cb->left_indent  = t_left_indent_1;
    nest_cb->right_indent = t_right_indent;

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/* :LI within :UL tag                                                      */
/***************************************************************************/

static  void    gml_li_ul( const gmltag * entry )
{
    char        *   p;
    char            bullet[3];
    int32_t         t_align         = 0;
    int32_t         t_left_indent_1 = 0;
    int32_t         t_left_indent_2 = 0;
    int32_t         t_right_indent  = 0;

    scan_err = false;
    p = scan_start;

    if( nest_cb == NULL ) {
        xx_nest_err( err_li_lp_no_list );   // tag must be in a list
        scan_start = scan_stop + 1;
        return;
    }

    if( ((ul_lay_level *)(nest_cb->lay_tag))->bullet_translate ) {
        bullet[0] = cop_in_trans( ((ul_lay_level *)(nest_cb->lay_tag))->bullet,
                            ((ul_lay_level *)(nest_cb->lay_tag))->bullet_font );
    } else {
        bullet[0] = ((ul_lay_level *)(nest_cb->lay_tag))->bullet;
    }
    bullet[1] = ' ';                    // 1 trailing space as wgml4 does
    bullet[2] = 0;

    scr_process_break();

    if( ProcFlags.need_li_lp ) {        // first :li for this list
        set_skip_vars( &nest_cb->xl_pre_skip, NULL, NULL, 1, g_curr_font );
    } else if( !nest_cb->compact ) {
        set_skip_vars( &((ul_lay_level *)(nest_cb->lay_tag))->skip, NULL,
                       NULL, 1, g_curr_font );
    } else {                            // compact
        set_skip_vars( NULL, NULL, NULL, 1, g_curr_font );
    }

    g_curr_font = ((ul_lay_level *)(nest_cb->lay_tag))->bullet_font;
    post_space = 0;

    /* Use original indents, not values possibly modified by IN */

    if( g_indent > 0 ) {
        t_left_indent_1  = conv_hor_unit( &((ul_lay_level *)(nest_cb->lay_tag))->left_indent )
                            + nest_cb->prev->left_indent + nest_cb->prev->align;
    } else {
        t_left_indent_1  = conv_hor_unit( &((ul_lay_level *)(nest_cb->lay_tag))->left_indent )
                            + nest_cb->prev->left_indent;
    }
    t_left_indent_2  = conv_hor_unit( &((ul_lay_level *)(nest_cb->lay_tag))->left_indent );
    t_right_indent = -1 * conv_hor_unit( &((ul_lay_level *)(nest_cb->lay_tag))->right_indent )
                            + nest_cb->prev->right_indent;
    g_cur_left = nest_cb->lm + t_left_indent_2;
    g_page_right = nest_cb->rm + t_right_indent;

    g_cur_h_start = g_cur_left;
    ProcFlags.keep_left_margin = true;  // keep special Note indent
    start_line_with_string( bullet, g_curr_font, true );

    t_align = conv_hor_unit( &((ul_lay_level *)(nest_cb->lay_tag))->align );
    g_cur_h_start = g_cur_left + t_align;

    if( t_line != NULL ) {
        if( t_line->last != NULL ) {
            g_cur_left += t_line->last->width + post_space;
        }
    }
    post_space = 0;
    if( g_cur_h_start > g_cur_left ) {
        g_cur_left = g_cur_h_start;
    }
    g_cur_h_start = g_cur_left;
    ju_x_start = g_cur_h_start;

    g_curr_font = ((ul_lay_level *)(nest_cb->lay_tag))->font;
    if( *p == '.' ) p++;                // over '.'
    while( *p == ' ' ) p++;             // skip initial spaces
    ProcFlags.need_li_lp = false;
    if( *p ) {
        process_text( p, g_curr_font ); // if text fullows
    }
    nest_cb->align = t_align;

    g_cur_left = nest_cb->lm + t_left_indent_2 + nest_cb->align;

    /* Set indents to their original values for the next LI */

    if( g_indent > 0 ) {
        nest_cb->left_indent = t_left_indent_1;
    } else {
        nest_cb->left_indent = t_left_indent_1 + nest_cb->prev->align;
    }
    nest_cb->right_indent = t_right_indent;

    scan_start = scan_stop + 1;
    return;
}



/****************************************************************************/
/*Format: :LI [id='id-name'].<paragraph elements>                           */
/*                           <basic document elements>                      */
/*                                                                          */
/*This tag signals the start of an item in a simple, ordered, or unordered  */
/*list. The unordered list items are preceded by an annotation symbol, such */
/*as an asterisk. The ordered list items are annotated by an ordered        */
/*sequence. The id attribute associates an identifier name with the list    */
/*item, and may only be used when the list item is in an ordered list. The  */
/*identifier name is used when processing a list item reference, and must   */
/*be unique within the document.                                            */
/*wgml 4.0 does not allow LI inside a DL or GL, but does produce an error   */
/****************************************************************************/

void    gml_li( const gmltag * entry )
{
    if( nest_cb->c_tag == t_LP ) {      // terminate :LP if active
        end_lp();
    }

    switch( nest_cb->c_tag ) {
    case t_OL :
        gml_li_ol( entry );
        break;
    case t_SL :
        gml_li_sl( entry );
        break;
    case t_UL :
        gml_li_ul( entry );
        break;
#if 0
    case t_DL :
        gml_li_dl( entry );             // error message here?
        break;
    case t_GL :
        gml_li_gl( entry );             // error message here?
        break;
#endif
    default:
        break;
    }
    return;
}


/***************************************************************************/
/* :LP                                                                     */
/***************************************************************************/

void    gml_lp( const gmltag * entry )
{
    char        *   p;
    su          *   list_skip_su;
    su          *   lp_skip_su;
    su          *   pre_skip_su;

    scan_err = false;
    p = scan_start;

    if( nest_cb == NULL ) {
        xx_nest_err( err_li_lp_no_list );   // tag must be in a list
        scan_start = scan_stop + 1;
        return;
    }

    list_skip_su = &nest_cb->xl_pre_skip;   // nest_cb on entry, not LP nest_cb
    lp_skip_su = &layout_work.lp.pre_skip;

    gml_xl_lp_common( t_LP );

    if( g_line_indent == 0 ) {
        ProcFlags.para_starting = false;    // clear for this tag's first break
    }
    scr_process_break();

    nest_cb->compact = false;

    nest_cb->li_number    = 0;
    nest_cb->left_indent  = conv_hor_unit( &layout_work.lp.left_indent );
    nest_cb->right_indent = -1 * conv_hor_unit( &layout_work.lp.right_indent );
    nest_cb->lay_tag      = &layout_work.lp;

    nest_cb->lm = nest_cb->prev->lm + nest_cb->prev->left_indent;
    nest_cb->rm = nest_cb->prev->rm + nest_cb->prev->right_indent;

    spacing = (int8_t) layout_work.lp.spacing;

    ProcFlags.keep_left_margin = true;  // keep special Note indent
    post_space = 0;

    if( ProcFlags.need_li_lp ) {        // :LP first tag in list
        pre_skip_su = greater_su( lp_skip_su, list_skip_su, spacing );
    } else {
        pre_skip_su = lp_skip_su;
    }
    set_skip_vars( pre_skip_su, NULL, &layout_work.lp.post_skip, 1, g_curr_font );

    g_cur_left = nest_cb->lm + nest_cb->left_indent;// left start
                                        // possibly indent first line
    g_line_indent = conv_hor_unit( &(layout_work.lp.line_indent) );
    g_cur_h_start = g_cur_left + g_line_indent;

    g_page_right = nest_cb->rm + nest_cb->right_indent;

    nest_cb->left_indent += nest_cb->prev->left_indent;
    nest_cb->right_indent += nest_cb->prev->right_indent;

    ju_x_start = g_cur_h_start;

    ProcFlags.need_li_lp = false;       // :LI or :LP seen
    ProcFlags.para_starting = true;     // for next break, not this tag's break

    if( *p == '.' ) p++;                // possible tag end
    while( *p == ' ' ) p++;             // skip initial spaces
    if( *p ) {
        process_text( p, g_curr_font ); // if text follows
    }

    scan_start = scan_stop + 1;
    return;
}

