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
* Description:  WGML Process text not tags / controlwords
*
*               do_c_chars              generate c_chars for wgml_tabs
*               do_fc_comp              compute data used by fill_chars
*               do_justify              insert spaces between words
*               intrans                 perform input translation
*               next_tab                finds next tab stop
*               process_line_full       process text line into t_element
*               process_text            form input text into output text lines
*               puncadj                 insert spaces after .:?
*               redo_tabs()             redo tabbing of preprocessed line
*               set_h_start             set horizontal start postion
*               split_text              split text at given position
*               text_chars_width        width to first wgml tab
*               wgml_tabs               expands wgml tabs
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"

static  bool            def_tab         = false;    // true when a default tab has been selected
static  bool            phrase_start    = false;    // current token started new phrase
static  bool            tabbing         = false;    // current tab exists
static  bool            user_tab_skip   = false;    // true when a user tab has been skipped
static  uint32_t        fill_count      = 0;        // fill char count
static  uint32_t        fill_start      = 0;        // fill string starting position
static  uint32_t        fill_width      = 0;        // fill string width
static  uint32_t        gap_start       = 0;        // tab gap start
static  uint32_t        tab_space       = 0;        // space count between text and tab char, whether pre-tab or post-tab

/***************************************************************************/
/*  puncadj modelled after the host ASM sources to get the same horizontal */
/*  spacing as wgml4, but still no luck                               TBD  */
/*  further testing showed that .:!? are used as full stop characters      */
/*  the others ;,) have no special effect                                  */
/***************************************************************************/

static void puncadj( text_line * line, int32_t * delta0, int32_t rem,
                     int32_t cnt, uint32_t lm )
{

/***************************************************************************/
/*  puncadj has been disabled because, while the idea is correct, the      */
/*  details are wrong                                                      */
/*  further testing showed that at least "," is treated specially and      */
/*  that "." does not use a half-space-width                               */
/*  "," has either 1 or 2 hbu added to it, but only in PS                  */
/*  "." has approximately 1/4 of a space-width added to it (in PS)         */
/*  and so it depends on the font, but it is sometimes rounded up and      */
/*  sometimes not, in a pattern that remains to be explored                */
/*  Note: wgml 4.0 appears to be doing something strange with the end      */
/*  positions reported following "," and especially following ".", making  */
/*  the width uncomputable using the before & after positions in the       */
/*  output file produced by the test device PSSPEC; the widths need to be  */
/*  obtained using ".ty &'width(<text>,U)"                                 */
/*  A spreadsheet is heartily recommended for crunching the data!          */
/***************************************************************************/

#if 0
    text_chars  *   tleft;              // first text_char to justify
    text_chars  *   tn;
    text_chars  *   tw;
    int32_t         delta;
    int32_t         loop_cnt;
    int32_t         space = wgml_fonts[FONT0].spc_width;// TBD
    int32_t         spacew;
    int32_t         remw = rem - rem;   // TBD
    char            ch;
    bool            changed;

    for( tleft = line->first; tleft->x_address < lm ; tleft = tleft->next ) {
        if( tleft == NULL ) {
            return;                     // no justify due to left margin
        }
    }
    if( tleft->next == NULL ) {
        return;                         // only 1 text_chars no justify
    }

    if( ProcFlags.has_aa_block ) {
        space /= 2;                     // TBD
//      space -= 3;                     // TBD
    }

    changed = false;
    delta = *delta0;
    loop_cnt = 3;                       // 3 passes
    while( loop_cnt > 2 && delta >= space ) {   // only 1 pass TBD
        if( ProcFlags.has_aa_block ) {
            space = wgml_fonts[FONT0].spc_width / 2;// TBD
//          space += loop_cnt - 1;      // TBD
        }

        /* from right to left search for stop chars */
        for( tw = line->last->prev; tw != tleft; tw = tw->prev ) {

            tn = tw->next;
            ch = tw->text[tw->count - 1];
            switch( loop_cnt ) {
            case   3:                   // test full stop
                if( ch == '.' || ch == '!' || ch == '?' || ch == ':' ) {
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
                    changed = true;
                }
                break;
            case   2:                   // test half stop
                if( ch == ':' || ch == ';' ) {
#if 0
                    delta -= space;
                    while( tn != NULL ) {
                        tn->x_address += space;
                        tn = tn->next;
                    }
#else
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
#endif
                    changed = true;
                }
                break;
            case   1:
                if( ch == ',' || ch == ')' ) {
#if 0
                    delta -= space;
                    while( tn != NULL ) {
                        tn->x_address += space;
                        tn = tn->next;
                    }
#else
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    delta -= spacew;
                    while( tn != NULL ) {
                        tn->x_address += spacew;
                        tn = tn->next;
                    }
#endif
                    changed = true;
                }
                break;
            default:
                break;
            }
        }
        loop_cnt--;
    }
    *delta0 = delta;
#endif
}

/***************************************************************************/
/*  return the width of text up to the first tab stop                      */
/***************************************************************************/

static uint32_t text_chars_width( const char *text, size_t count, font_number font )
{
    size_t      i;
    size_t      cur_count   = 0;
    uint32_t    retval      = 0;

    for( i = 0; i < count; i++ ) {
        if( (text[i] == '\t') || (text[i] == tab_char) ) {
            break;
        }
        cur_count++;
    }
    // if there are no tabs, cur_count will equal count
    if( cur_count != 0 ) {
        retval = cop_text_width( text, cur_count, font );
    }
    return( retval );
}


/***************************************************************************/
/*  set c_stop to the next tab stop to be used                             */
/***************************************************************************/

static void next_tab( void )
{
    int                 i;
    int                 last_i;
    tab_stop            l_tab;
    tab_stop    *       c_tab       = NULL;
    uint32_t            cur_h;
    uint32_t            r_count;
    uint32_t            r_length;
    uint32_t            r_width;

    /* Note: use of g_cur_left & g_page_left appears to be correct, even in */
    /* the presence of .in, but this may not be entirely true               */

    if( g_cur_left > g_cur_h_start ) {
        g_cur_h_start = g_cur_left;
    }
    cur_h = g_cur_h_start - g_page_left;

    if( user_tabs.current > 0 ) {   // try user tabs first
        i = 0;
        if( c_stop == NULL ) {
            last_i = -1;
        } else {
            for( i = 0; i < user_tabs.current; i++ ) {  // find "i" corresponding to tab after c_stop
                if( c_stop->column == user_tabs.tabs[i].column ) {
                    break;
                }
            }
            last_i = i;
        }
        for( i; i < user_tabs.current; i++ ) {
            if( cur_h < user_tabs.tabs[i].column ) {
                c_tab = &user_tabs.tabs[i];
                break;
            }
        }
        if( last_i < i - 1 ) {
            user_tab_skip = true;
        }
    }
    if( c_tab == NULL ) {   // no user tab or none that works, use default tab
        def_tab = true;     // record use of default tab
        l_tab = def_tabs.tabs[def_tabs.current - 1];
        if( cur_h >= l_tab.column ) {   // initialize more tabs
            r_width = cur_h - l_tab.column;
            r_count = (r_width / inter_tab) + 1;
            if( (def_tabs.current + r_count) > def_tabs.length ) {
                // add enough tabs to ensure this isn't done every time
                r_count /= TAB_COUNT;
                r_count++;
                r_length = def_tabs.length + (r_count * TAB_COUNT);
                def_tabs.tabs = mem_realloc( def_tabs.tabs,
                                            r_length * sizeof( tab_stop ) );
                for( i = def_tabs.length; i < r_length; i++ ) {
                    def_tabs.tabs[i].column = def_tabs.tabs[i - 1].column + inter_tab;
                    def_tabs.tabs[i].fill_char = ' ';
                    def_tabs.tabs[i].alignment = al_left;
                }

                def_tabs.length = r_length;
            }
            def_tabs.current = def_tabs.length;
        }

        // get the tab stop to use
        for( i = 0; i < def_tabs.current; i++ ) {
            if( cur_h < def_tabs.tabs[i].column ) {
                c_tab = &def_tabs.tabs[i];
                break;
            }
        }
    }

    c_stop = c_tab;                 // set c_stop to next tab stop to use
    ju_x_start = c_stop->column;    // set left boundary for justification

    return;
}


/***************************************************************************/
/*  create c_chars instance and link it in correctly                       */
/***************************************************************************/

static text_chars * do_c_chars( text_chars *c_chars, text_chars *in_chars,
                                 const char *in_text, size_t count,
                                 uint32_t in_x_address, uint32_t width,
                                 font_number font, text_type type )
{
    if( c_chars == NULL ) {
        c_chars = alloc_text_chars( in_text, count, font );
        if( t_line->first == NULL ) {
            t_line->first = c_chars;
        } else {
            c_chars->prev = in_chars->prev;
            c_chars->prev->next = c_chars;
        }
    } else {
        c_chars->next = alloc_text_chars( in_text, count, font );
        c_chars->next->prev = c_chars;
        c_chars = c_chars->next;
    }
    c_chars->type = type;
    c_chars->x_address = in_x_address;
    c_chars->width = width;

    return( c_chars );
}


/***************************************************************************/
/*  compute data used by fill_chars                                        */
/***************************************************************************/

static void do_fc_comp( void )
{
    fill_width = wgml_fonts[g_curr_font].width_table[c_stop->fill_char];
    fill_count = (g_cur_h_start - g_cur_left) / fill_width;

    fill_start = (gap_start - g_cur_left) / fill_width;
    if( ((gap_start - g_cur_left) % fill_width) != 0 ) {
        fill_start++;
    }

    fill_count -= fill_start;

    fill_start *= fill_width;
    fill_start += g_cur_left;

    if( (fill_count == 0) && (g_cur_h_start < fill_start) ) {
        g_cur_h_start = fill_start;
    }

    return;
}

/***************************************************************************/
/*  expand any wgml tabs infesting t_line->last                            */
/*  g_cur_h_start is correct when the function returns                     */
/*  Note: g_cur_left reflects the left indent of .in                       */
/*        g_page_left does not                                             */
/*        both appear to be needed, in different places                    */
/***************************************************************************/

static void wgml_tabs( void )
{
    bool                        center_end  = false;    // spaces at end of al_center scope
    bool                        skip_tab    = false;    // skip current tab
    char                    *   in_text;                // in_chars->text
    int                         i;
    int32_t                     t_align     = 0;        // current align value, or 0 if none found
    int32_t                     offset      = 0;        // offset for position adjustment
    size_t                      t_count     = 0;        // text count
    tag_cb                  *   t_cb        = NULL;
    text_chars              *   c_chars     = NULL;     // current text_chars
    text_chars              *   c_multi;                // used to traverse parts of multipart word
    text_chars              *   in_chars;               // text_chars being processed
    text_chars              *   s_chars     = NULL;     // source text_chars
    uint32_t                    in_count;               // in_chars->count
    uint32_t                    m_width;                // multi-part word width
    uint32_t                    pre_space   = 0;        // space before current word
    uint32_t                    pre_width;              // tab_space in hbus & adjusted for alignment
    uint32_t                    t_start;                // text start
    uint32_t                    t_width;                // text width

    static  bool                text_found  = false;    // text found after tab character
    static  text_chars      *   s_multi     = NULL;     // first part of multipart word
    static  text_line           tab_chars   = { NULL, 0, 0, NULL, NULL };   // current tab markers/fill chars
    static  text_type           c_type      = norm;     // type for current tab character
    static  font_number         c_font      = 0;        // font for current tab character
    static  uint32_t            s_width     = 0;        // space width (from tab_space)

    in_chars = t_line->last;
    in_text  = in_chars->text;
    in_count = in_chars->count;

    /* Set t_align to closest enclosing nonzero value */

    t_cb = nest_cb;
    while( t_cb != NULL ) {
        if( t_cb->align != 0 ) {
            t_align = t_cb->align;
            break;
        }
        t_cb = t_cb->prev;
    }
    for( i = 0; i < in_count; i++) {    // locate the first wgml tab, if any
        if( (in_text[i] == '\t') || (in_text[i] == tab_char) ) {
            break;
        }
        t_count++;
    }
    if( !tabbing && (t_count == in_count) ) {
        g_cur_h_start = t_line->last->x_address + t_line->last->width;
        phrase_start = false;
        post_space = 0;
        return; // no current tab stop, no new tab stop: nothing to do
    }
    if( t_line->first == t_line->last ) {
        t_line->first = NULL;       // in_chars is only text_chars in t_line
        t_line->last = NULL;
    } else {
        t_line->last = t_line->last->prev;  // unhook in_chars
        t_line->last->next = NULL;
    }
    if( !tabbing && (t_count > 0) ) {   // no current tab stop: reset state
        c_font = 0;
        c_stop = NULL;
        c_type = norm;
        tab_chars.first = NULL;
        tab_chars.last = NULL;
        tab_space = 0;
        text_found = false;
    }
    if( t_count == 0 ) {            // first character is tab character
        tab_chars.first = NULL;
        tab_chars.last = NULL;

        if( t_line->last == NULL ) {
            if( tab_space > 0 ) {                       // spaces preceed tab char
                c_chars = do_c_chars( c_chars, in_chars, in_text, t_count,
                            g_cur_h_start, in_chars->width,
                            in_chars->font, in_chars->type );
                if( s_multi == NULL ) {
                    s_multi = c_chars;
                }
            }
        } else {
            if( g_cur_h_start > (t_line->last->x_address + t_line->last->width) ) {
                if( t_line->last->count == 0 ) {
                    if( phrase_start ) {    // add new marker
                        g_cur_h_start = t_line->last->x_address;
                        c_chars = do_c_chars( c_chars, in_chars, in_text,
                            0, g_cur_h_start, 0, in_chars->font,
                            in_chars->type );
                    } else {                // adjust prior marker
                        pre_width = tab_space *
                                        wgml_fonts[in_chars->font].spc_width;
                        if( c_stop->alignment == al_center ) {
                            pre_width /= 2;
                            if( (pre_width % 2) > 0 ) {
                                pre_width++;
                            }
                        }
                        t_line->last->x_address += pre_width;
                        g_cur_h_start = t_line->last->x_address;
                        tab_space = 0;
                    }
                } else {    // spaces precede tab in mid-line; c_stop may be NULL if the tab is from input translation
                    if( (c_stop != NULL) && (c_stop->alignment == al_center) ) {
                        center_end = true;
                    } else {
                        c_chars = do_c_chars( c_chars, in_chars, in_text,
                            0, g_cur_h_start, 0, in_chars->font,
                            in_chars->type );
                    }
                }
            }
        }
        if( !center_end ) {
            next_tab();
            c_font = in_chars->font;
            c_type = in_chars->type;
            gap_start = g_cur_h_start;
            s_multi = NULL;
            s_width = 0;
            tabbing = true;
            tab_space = 0;
            text_found = false;
            i = 1;                      // skip initial tab character
        }
    } else if( tabbing ) {          // text belongs to current tab stop
        if( tab_chars.last != NULL ) {
            if( !(input_cbs->fmflags & II_sol) ) {   // not if at start of input line
                if( (tab_chars.first != NULL) && ((c_stop->alignment != al_left) ||
                    !(input_cbs->fmflags & II_tag_mac)) ) {
                    // remove all markers/fill chars
                    if( tab_chars.first->prev !=NULL) {
                        tab_chars.first->prev->next = tab_chars.last->next;
                    }
                    if( tab_chars.last->next != NULL) {
                        tab_chars.last->next->prev = tab_chars.first->prev;
                    }
                    if( in_chars->prev == tab_chars.last ) {
                        in_chars->prev = tab_chars.first->prev;
                    }
                    tab_chars.first->prev = NULL;
                    tab_chars.last->next = NULL;
                    add_text_chars_to_pool( &tab_chars );
                    tab_chars.first = NULL;
                    tab_chars.last = NULL;
                    t_line->last = in_chars->prev;
                    if( t_line->last == NULL ) {
                        t_line->first = NULL;
                    }
                } else {
                    tab_chars.first = NULL;
                    tab_chars.last = NULL;
                }
            }
            if( tab_space > 0 ) {   // tab followed by spaces then text
                if( c_stop->alignment == al_left ) {   // alignment left
                    s_width = 0;
                    if( input_cbs->fmflags & II_tag_mac ) {   // inside macro
                        g_cur_h_start = g_cur_left + c_stop->column + post_space - t_align;
                    } else {                                // not inside macro
                        g_cur_h_start = g_cur_left + c_stop->column + tab_space *
                                        wgml_fonts[in_chars->font].spc_width;
                    }
                    c_chars = do_c_chars( c_chars, in_chars, in_text, t_count,
                            g_cur_h_start, in_chars->width,
                            in_chars->font, in_chars->type );
                    if( t_count == in_count ) { // no tab character in text
                        t_line->last = c_chars;
                        g_cur_h_start = c_chars->x_address + c_chars->width;
                        in_chars->next = text_pool;
                        text_pool = in_chars;
                        tabbing = false;    // allow for at most one word
                        phrase_start = false;
                        post_space = 0;
                        return;
                    }
                    i = t_count + 1;
                    g_cur_h_start += c_chars->width;
                    next_tab();
                    c_font = in_chars->font;
                    c_type = in_chars->type;
                    gap_start = g_cur_h_start;
                    s_multi = NULL;
                    s_width = 0;
                    tabbing = true;
                    tab_space = 0;
                    text_found = false;
                } else {    // alignment right or center
                    if( s_multi == NULL ) {    // first time, set s_width
                        s_width = tab_space *
                                        wgml_fonts[in_chars->font].spc_width;
                    }
                    i = 0;
                }
            } else {
                i = 0;
            }
        } else {
            i = 0;
        }
    } else {                        // text found before first tab character
        tab_chars.first = NULL;
        tab_chars.last = NULL;
        c_chars = do_c_chars( c_chars, in_chars, in_text, t_count,
                        g_cur_h_start, in_chars->width,
                        in_chars->font, in_chars->type );
        i++;
        g_cur_h_start += c_chars->width;
        next_tab();
        c_font = in_chars->font;
        c_type = in_chars->type;
        gap_start = g_cur_h_start;
        s_multi = NULL;
        s_width = 0;
        tabbing = true;
        tab_space = 0;
        text_found = false;
    }
    input_cbs->fmflags &= ~II_sol;   // clear flag
    phrase_start = false;

    /************************************************************************/
    /*  These conditions hold at this point:                                */
    /*  in_text[i] is the first character after the current tab character   */
    /*  tab_chars controls no markers or fill chars                         */
    /*  s_multi is set correctly                                            */
    /*  c_font contains the font of the current tab character               */
    /*  c_type contains the type of the prior input text                    */
    /************************************************************************/

    for( i; i < in_count; i++ ) {
        t_start = i;
        for( i; i < in_count; i++ ) {   // get the text up to the next tab
            if( (in_text[i] == '\t') || (in_text[i] == tab_char) ) {
                break;
            }
        }
        t_count = i - t_start;
        t_width = cop_text_width( in_text + t_start, t_count, in_chars->font );
        if( t_count > 0 ) {             // text found after tab char
            text_found = true;
        }
        if( s_multi == NULL ) {
            m_width = t_width;
        } else {                        // get sizing for total word
            m_width = (g_cur_h_start + t_width) - s_multi->x_address;
            pre_space = in_chars->x_address - (in_chars->prev->x_address +
                                               in_chars->prev->width);
        }

        /* Set text start position and accept/reject current tab stop */

        switch( c_stop->alignment ) {
        case al_left:
            if( !tabbing || (s_multi == NULL) ) {
                g_cur_h_start = g_page_left + c_stop->column;

                /* II_macro immediately after a tab character */

                if( (input_cbs->fmflags & II_macro)
                        && (c_font != in_chars->phrase_font)
                        ) {
                    g_cur_h_start += post_space;
                }
                tabbing = false;
            }
            break;
        case al_center:
            if( gap_start < (g_page_left + c_stop->column - ((m_width + s_width) / 2)) ) {
                // split the width as evenly as possible
                g_cur_h_start = g_page_left + c_stop->column + (s_width / 2 ) - (m_width / 2);
                if( (s_width % 2) > 0 ) {
                    g_cur_h_start++;
                }
            } else {    // find the next tab stop; this one won't do
                skip_tab = true;
            }
            break;
        case al_right:
            if( gap_start < (g_page_left + c_stop->column + tab_col -
                                                            (m_width + s_width)) ) {
                g_cur_h_start = g_page_left + c_stop->column + tab_col - m_width;
            } else {    // find the next tab stop; this one won't do
                skip_tab = true;
            }
            break;
        default:
            internal_err( __FILE__, __LINE__ );
        }

        if( skip_tab ) {    // never true for al_left
            if( !def_tab ) {
                user_tab_skip = true;       // user tab skipped
            }
            if( tab_chars.first != NULL ) {  // remove all markers/fill chars
                if( tab_chars.first->prev !=NULL ) {
                    tab_chars.first->prev->next = tab_chars.last->next;
                }
                if( tab_chars.last->next != NULL ) {
                    tab_chars.last->next->prev = tab_chars.first->prev;
                }
                if( in_chars->prev == tab_chars.last ) {
                    in_chars->prev = tab_chars.first->prev;
                }
                tab_chars.first->prev = NULL;
                tab_chars.last->next = NULL;
                add_text_chars_to_pool( &tab_chars );
                tab_chars.first = NULL;
                tab_chars.last = NULL;
            }
            i = t_start;
            i--;
            next_tab();
            g_cur_h_start = g_cur_left + c_stop->column;

            /***********************************************/
            /*  gap_start is not reset because the gap     */
            /*  still starts at the same position          */
            /*  the others not reset are not reset         */
            /*  because the same tab character is being    */
            /*  processed; only the tab stop has changed   */
            /***********************************************/

            tabbing = true;
            text_found = false;
            skip_tab = false;
        } else {
            if( s_multi == NULL ) {
                s_chars = in_chars;
            } else {

            /***********************************************/
            /*  s_multi is used instead of in_chars        */
            /*  because this code is used to recreate the  */
            /*  tab markers and fill chars for a mulit-    */
            /*  part word as well as for a one-part word   */
            /***********************************************/

                s_chars = s_multi;
            }

            do_fc_comp();

            if( s_chars->prev != NULL ) {
                if( ((s_chars->prev->type == norm) && (c_type != norm)) ||
                     (s_chars->prev->type != norm) && (c_type == norm)) {
                    c_chars = do_c_chars( c_chars, in_chars, NULL, 0, gap_start,
                                                        0, c_font, c_type );
                    if( tab_chars.first == NULL) {
                        tab_chars.first = c_chars;
                    }
                    tab_chars.last = c_chars;
                }
            } else {
                if( c_type != norm ) {
                    c_chars = do_c_chars( c_chars, in_chars, NULL, 0,
                                          g_cur_h_start, 0, c_font, c_type );
                    if( tab_chars.first == NULL) {
                        tab_chars.first = c_chars;
                    }
                    tab_chars.last = c_chars;
                }
            }

            if( c_stop->fill_char != ' ' ) {
                c_chars = do_c_chars( c_chars, in_chars, NULL, fill_count,
                                        fill_start, fill_width * fill_count,
                                        c_font, c_type );
                c_chars->count = fill_count;
                memset( c_chars->text, c_stop->fill_char, fill_count );
                if( tab_chars.first == NULL) {
                    tab_chars.first = c_chars;
                }
                tab_chars.last = c_chars;
            }

            // Not for text from macros, even if font number changes

            if( (c_font != s_chars->font) && !(input_cbs->fmflags & II_macro) ) {
                c_chars = do_c_chars( c_chars, in_chars, NULL, 0,
                                g_cur_h_start, 0, c_font, c_type );
                if( tab_chars.first == NULL) {
                    tab_chars.first = c_chars;
                }
                tab_chars.last = c_chars;
            }

            if( s_chars->prev != NULL ) {
                if( (s_chars->type == norm) && (c_type != norm) ) {
                    c_chars = do_c_chars( c_chars, in_chars, NULL, 0,
                                        g_cur_h_start, 0, c_font, c_type );
                    if( tab_chars.first == NULL) {
                        tab_chars.first = c_chars;
                    }
                    tab_chars.last = c_chars;
                }
            }

            if( s_multi != NULL ) { // multi-part word
                if( tab_chars.first != NULL ) { // reposition markers/fill chars
                    t_line->last->next = NULL;
                    tab_chars.first->prev = s_multi->prev;
                    s_multi->prev->next = tab_chars.first;
                    s_multi->prev = tab_chars.last;
                    tab_chars.last->next = s_multi;
                    c_chars = NULL;
                }

                if( c_stop->alignment != al_left ) {  // no adjustment for left alignment
                    offset = g_cur_h_start - s_multi->x_address;
                    s_multi->x_address = g_cur_h_start;
                    g_cur_h_start = s_multi->x_address + s_multi->width;
                    c_multi = s_multi->next;
                    while( c_multi != NULL ) {  // adjust starting positions
                        c_multi->x_address += offset;
                        g_cur_h_start = c_multi->x_address + c_multi->width;
                        c_multi = c_multi->next;
                    }
                    g_cur_h_start += pre_space;
                }
            }

            // the text positioned by the tab stop
            c_chars = do_c_chars( c_chars, in_chars, in_text + t_start,
                                    t_count, g_cur_h_start, t_width,
                                    in_chars->font, in_chars->type );
            if( def_tab ) {
                c_chars->tab_pos = tt_def;
            } else {
                c_chars->tab_pos = tt_user;
            }
            if( t_width == 0 ) {    // no text: position marker
                if( tab_chars.first == NULL) {
                    tab_chars.first = c_chars;
                }
                tab_chars.last = c_chars;
                if( (s_chars->type == norm) && (c_type != norm) ) {
                    c_chars = do_c_chars( c_chars, in_chars, NULL, 0, gap_start,
                                                        0, c_font, c_type );
                    if( tab_chars.first == NULL) {
                        tab_chars.first = c_chars;
                    }
                    tab_chars.last = c_chars;
                }
            } else {
                g_cur_h_start += t_width;
                if( s_multi == NULL ) {
                    s_multi = c_chars;
                }
            }

            if( (in_text[i] == '\t') || (in_text[i] == tab_char) ) {
                // set up for new tab character
                tab_chars.first = NULL;
                tab_chars.last = NULL;
                next_tab();
                c_font = in_chars->font;
                c_type = in_chars->type;
                gap_start = g_cur_h_start;
                s_multi = NULL;
                s_width = 0;
                tabbing = true;
                tab_space = 0;
                text_found = false;
            }
        }
    }

    i = in_count - 1;                       // reset to check last character
    if( (in_text[i] == '\t') || (in_text[i] == tab_char) ) {
        gap_start = g_cur_h_start;
        g_cur_h_start = g_cur_left + c_stop->column - t_align;
        if( c_stop->alignment == al_right ) {
            g_cur_h_start += tab_col;
        }

        if( s_multi == NULL ) {
            s_chars = in_chars;
        } else {

            /***********************************************/
            /*  s_multi is used instead of in_chars        */
            /*  because this code is used to recreate the  */
            /*  tab markers and fill chars for a mulit-    */
            /*  part word as well as for a one-part word   */
            /***********************************************/

            s_chars = s_multi;
        }

        if( s_chars->prev != NULL ) {
            if( ((s_chars->prev->type == norm) && (c_type != norm)) ||
                 (s_chars->prev->type != norm) && (c_type == norm)) {
                c_chars = do_c_chars( c_chars, in_chars, NULL, 0, gap_start,
                                        0, c_font, c_type );
                if( tab_chars.first == NULL) {
                    tab_chars.first = c_chars;
                }
                tab_chars.last = c_chars;
            }
        }

        if( c_stop->fill_char != ' ' ) {
            do_fc_comp();
            c_chars = do_c_chars( c_chars, in_chars, NULL, fill_count,
                                        fill_start, fill_width * fill_count,
                                        c_font, c_type );
            c_chars->count = fill_count;
            memset( c_chars->text, c_stop->fill_char, fill_count );
            if( tab_chars.first == NULL) {
                tab_chars.first = c_chars;
            }
            tab_chars.last = c_chars;
        }

        // the tab marker for a tab character at the end of the input
        c_chars = do_c_chars( c_chars, in_chars, NULL, 0, g_cur_h_start, 0,
                                        in_chars->font, in_chars->type );
        if( tab_chars.first == NULL) {
            tab_chars.first = c_chars;
        }
        tab_chars.last = c_chars;
    }

    if( c_chars != NULL ) {
        t_line->last = c_chars;
    }
    in_chars->next = text_pool;
    text_pool = in_chars;

    post_space = 0;

    return;
}


/***************************************************************************/
/*  recompute tab stops when a line has been split                         */
/*  uses the tab_pos field to find text_chars positioned by tabs           */
/*  Note: g_cur_left reflects the left indent of .in                       */
/*        g_page_left does not                                             */
/*        both appear to be needed, in different places                    */
/***************************************************************************/

static void redo_tabs( text_line * a_line )
{
    bool            skip_tab    = false;    // skip current tab
    text_chars  *   cur_chars;
    text_line       tab_chars   = { NULL, 0, 0, NULL, NULL };   // current tab markers/fill chars
    uint32_t        cur_left;
    uint32_t        offset;                 // to adjust subsequen text_chars in tab scope
    uint32_t        scope_width = 0;        // total width of scope of current tab

    if( a_line == NULL ) {          // why are we here?
        return;
    }

    cur_chars = a_line->first;

    /* First skip the part of the line which has been shifted to the left margin */

    while( (cur_chars != NULL) && (cur_chars->tab_pos == tt_none) ) {
        cur_chars = cur_chars->next;
    }

    if( cur_chars == NULL ) {   // why are we here?
        return;
    }

    /* Now reprocess each tab stop in turn */

    while( cur_chars != NULL ) {
        tab_chars.first = cur_chars;
        scope_width += cur_chars->width;
        cur_left = cur_chars->prev->x_address + cur_chars->prev->width;
        g_cur_h_start = cur_left;
        cur_chars = cur_chars->next;
        while( (cur_chars != NULL) && (cur_chars->tab_pos == tt_none) ) {
            scope_width += cur_chars->x_address - cur_left;
            scope_width += cur_chars->width;
            cur_left = cur_chars->x_address + cur_chars->width;
            cur_chars = cur_chars->next;
        }
        if( cur_chars == NULL ) {
            tab_chars.last = a_line->last;
        } else {
            tab_chars.last = cur_chars->prev;
        }
        next_tab();

        /* Set text start position and accept/reject current tab stop */

        switch( c_stop->alignment ) {
        case al_left:
            if( !tabbing || (tab_chars.first == NULL) ) {
                g_cur_h_start = g_page_left + c_stop->column;

                /* II_macro immediately after a tab character */

                if( (tab_chars.first->fmflags & II_macro)
                        && (tab_chars.first->font != cur_chars->phrase_font)
                        ) {
                    g_cur_h_start += post_space;
                }
                tabbing = false;
            }
            break;
        case al_center:
            if( gap_start < (g_page_left + c_stop->column - ((scope_width + tab_chars.first->ts_width) / 2)) ) {
                // split the width as evenly as possible
                g_cur_h_start = g_page_left + c_stop->column + (tab_chars.first->ts_width / 2 ) - (scope_width / 2);
                if( (tab_chars.first->ts_width % 2) > 0 ) {
                    g_cur_h_start++;
                }
            } else {    // find the next tab stop; this one won't do
                skip_tab = true;
            }
            break;
        case al_right:
            if( gap_start < (g_page_left + c_stop->column + tab_col -
                                                            (scope_width + tab_chars.first->ts_width)) ) {
                g_cur_h_start = g_page_left + c_stop->column + tab_col - scope_width;
            } else {    // find the next tab stop; this one won't do
                skip_tab = true;
            }
            break;
        default:
            internal_err( __FILE__, __LINE__ );
        }

        if( skip_tab ) {    // never true for al_left
            if( !def_tab ) {
                user_tab_skip = true;       // user tab skipped
            }
            next_tab();
            g_cur_h_start = g_cur_left + c_stop->column;
            tabbing = true;
            skip_tab = false;
        } else {
            offset = tab_chars.first->x_address - g_cur_h_start;
            tab_chars.first->x_address = g_cur_h_start;
            if( def_tab ) {
                tab_chars.first->tab_pos = tt_def;
            } else {
                tab_chars.first->tab_pos = tt_user;
            }
            cur_chars = tab_chars.first->next;        // position remaining text_chars in tab scope
            while( (cur_chars != NULL) && (cur_chars->tab_pos == tt_none) ) {
                cur_chars->x_address -= offset;
                cur_chars = cur_chars->next;
            }
            if( cur_chars != NULL ) {
                if( cur_chars->tab_pos != tt_none ) {
                    scope_width = 0;
                }
            }
        }
    }

    return;
}


/***************************************************************************/
/*  split in_chars->text at limit                                          */
/*  returns number of characters which will not fit on the current line    */
/***************************************************************************/

static uint32_t split_text( text_chars *in_chars, uint32_t limit )
{
    uint32_t    retval;
    size_t      t_count;
    uint32_t    t_limit;
    uint32_t    t_width;

    if( limit < in_chars->x_address ) {     // no characters will fit
        retval = in_chars->count;
    } else {                                // some characters might fit
        t_limit = limit - in_chars->x_address;
        if( in_chars->width > t_limit ) {   // more exact computation is needed
            t_count = t_limit / wgml_fonts[in_chars->font].default_width;
            t_width = cop_text_width( in_chars->text, t_count, in_chars->font );
            if( t_width != t_limit ) {      // not all characters will fit
                if( t_width < t_limit ) {
                    while( t_width < t_limit ) {
                        t_count++;
                        t_width = cop_text_width( in_chars->text, t_count, in_chars->font );
                    }
                } else {
                    while( t_width > t_limit ) {
                        t_count--;
                        t_width = cop_text_width( in_chars->text, t_count, in_chars->font );
                    }
                }
                if( t_count == 0 ) { // no characters will fit
                    retval = in_chars->count;
                } else if( t_count == in_chars->count ) {
                    retval = 0;         // all characters will fit
                } else {                // some characters will fit
                    retval = in_chars->count - t_count;
                    in_chars->count = t_count;
                    in_chars->width = t_width;
                }
            } else {                    // t_count chars will fit exactly
                retval = in_chars->count - t_count;
                in_chars->count = t_count;
                in_chars->width = t_width;
            }
        } else { // the entire in_chars->text will fit, with room to spare
            retval = 0;
        }
    }

    return( retval );
}


/***************************************************************************/
/*  justification and alignment                                            */
/*  there are several differences between how justification and alignment  */
/*  are done here and how they are done by wgml 4.0                        */  
/*  Note: the description of the function in the TSO is different from     */
/*  both how wgml 4.0 actually implements JU and this implementation       */
/***************************************************************************/

void do_justify( uint32_t lm, uint32_t rm, text_line * line )
{
    text_chars  *   tc;
    text_chars  *   tw;
    text_chars  *   tl;                 // last text_chars in line
//    int32_t         sum_w;              // sum of words widths
    int32_t         hor_end;
    int32_t         cnt;                // # of text_chars
    int32_t         line_width;         // usable line length
    int32_t         delta;              // space increment between words
    int32_t         delta0;             // total space to distribute
    int32_t         delta1;
    int32_t         rem;              // possible part of space to distribute
    int32_t         deltarem;
    ju_enum         just;
    symsub  *       symjusub;         // for debug output string value of .ju

    if( ProcFlags.justify == ju_off || ProcFlags.literal || line == NULL
        || line->first == NULL) {
        return;
    }
    if( lm >= rm ) { // with tabbing, ju_x_start can be past the right margin
        return;
    }

#if 0
    /***********************************************************************/
    /*  for PS device remainder decrement is treated differently      TBD  */
    /***********************************************************************/
    if( ProcFlags.has_aa_block ) {
        deltarem = 1;                   // TBD was 2
    } else {
        deltarem = 1;
    }
#endif
    deltarem = 1;

//    sum_w = 0;
    hor_end = 0;
    cnt = 0;
    tw = line->first;
    tl = line->last;
    tc = tw;
    hor_end = tl->x_address + tl->width;// hor end position

    do {                                // calculate used width
        if( tw->x_address >= lm ) {  // no justify for words left of ju start
            if( cnt == 0 ) {
                tc = tw;              // remember first text_char for justify
            }
            cnt++;                      // number of 'words'
//            sum_w += tw->width;         // sum of 'words' widths
        }
        tw = tw->next;
    } while( tw != NULL );

    line_width = rm - lm;

//    if( (sum_w <= 0) || (hor_end >= rm) || (line_width < 1) ) {
    if( (hor_end >= rm) || (line_width < 1) ) {
        return;                         // no justify needed / possible
    }
    delta0 = rm - hor_end;

    /* both here and below, it is not clear if this should be rounded up */
    /* when delta0 is odd                                                */
    if( ProcFlags.justify == ju_half ) {
        delta0 /= 2;
    }

    if( ProcFlags.justify == ju_on ) {
        if( cnt < 2 ) {      // one text_chars only, no full justify possible
            return;
        }
    }
    if( cnt < 2 ) {
        delta = delta0;                 // one text_chars gets all space
        rem   = 0;
    } else {
        delta = delta0 / (cnt - 1);
        rem   = delta0 % (cnt - 1);
    }

    if( input_cbs->fmflags & II_research && GlobalFlags.lastpass ) {
        find_symvar( &sys_dict, "$ju", no_subscript, &symjusub);// .ju as string
#if 0
        out_msg( "\n ju_%s lm:%d %d rm:%d sum_w:%d hor_end:%d"
                 " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                 lm, line_width, rm, sum_w, hor_end, delta, rem, delta0, cnt );
#endif
        out_msg( "\n ju_%s lm:%d %d rm:%d hor_end:%d"
                 " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                 lm, line_width, rm, hor_end, delta, rem, delta0, cnt );
    }
    if( delta < 1 && rem < 1 ) {        // nothing to distribute
        return;
    }
    switch( ProcFlags.justify ) { // convert inside / outside to left / right
    case ju_inside :                    // depending on odd / even page
        if( page & 1 ) {
            just = ju_right;
        } else {
            just = ju_left;
        }
        break;
    case ju_outside :
        if( page & 1 ) {
            just = ju_left;
        } else {
            just = ju_right;
        }
        break;
    default :
        just = ProcFlags.justify;
        break;
    }

    switch( just ) {                  // what type of justification is wanted
/*************************************
    case  ju_half :                   Treated as left ??? TBD
        delta /= 2;
        if( delta < 1 && rem < 1 ) {
            break;
        }
        // falltrough
************************************** */
    case  ju_half :
    case  ju_on :
//      if( tc->x_address < lm ) {
//          break;                      // left of left margin no justify
//      }

        /* presumably, puncadj required the recomputation of hor_end & delta0 */
        puncadj( line, &delta0, rem, cnt - 1, lm );

        hor_end = tl->x_address + tl->width;// hor end position
        delta0 = rm - hor_end;          // TBD

        /* if delta0 is recomputed, then it must also be re-halved */
        if( ProcFlags.justify == ju_half ) {
            delta0 /= 2;
        }

        if( cnt < 2 ) {
            delta = delta0;             // one text_chars gets all space
            rem   = 0;
        } else {
            delta = delta0 / (cnt - 1);
            rem   = delta0 % (cnt - 1);
        }

        if( input_cbs->fmflags & II_research && GlobalFlags.lastpass ) {
            test_out_t_line( line );
#if 0
            out_msg( "\n ju_%s lm:%d %d rm:%d sum_w:%d hor_end:%d"
                     " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                     lm, line_width, rm, sum_w, hor_end, delta, rem, delta0,
                     cnt );
#endif
            out_msg( "\n ju_%s lm:%d %d rm:%d hor_end:%d"
                     " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                     lm, line_width, rm, hor_end, delta, rem, delta0,
                     cnt );
        }
        if( delta < 1 && rem < 1 ) {    // nothing to distribute
            return;
        }


        delta1 = delta;
        tw = tc->next;
        while( tw != NULL ) {
            if( rem > 0 ) {             // distribute remainder, too
                tw->x_address += delta + deltarem;
                delta += delta1 + deltarem;
//              if( !ProcFlags.has_aa_block ) {      // TBD
                    rem -= deltarem;
//              }
            } else {
                tw->x_address += delta;
                delta += delta1;
            }
            tw = tw->next;
        }
        break;
    case  ju_left :
        delta = tc->x_address - lm;     // shift to the left
        if( delta < 1 ) {
            break;                      // already left
        }
        tw = tc;
        do {
            tw->x_address -= delta;
            tw = tw->next;
        } while( tw != NULL );
        break;
    case  ju_right :
        delta = rm - hor_end;           // shift right
        if( delta < 1 ) {
            break;                      // already at right margin
        }
        tw = tc;
        do {
           tw->x_address += delta;
           tw = tw->next;
        } while( tw != NULL );
        break;
    case  ju_centre :
        delta = (rm - hor_end) / 2;
        if( delta < 1 ) {
            break;                      // too wide no centre possible
        }
        tw = tc;
        do {
           tw->x_address += delta;
           tw = tw->next;
        } while( tw != NULL );
        break;
    default:
        break;
    }
}


/***************************************************************************/
/*  if input translation is active                                         */
/*      look for input escape char and translate the following char,       */
/*      delete the escape char                                             */
/***************************************************************************/

size_t intrans( char *data, size_t len, font_number font )
{
    char    *ps;                        // source ptr
    char    *pt;                        // target ptr
    size_t  k;

    if( ProcFlags.in_trans ) {
        ps = data;
        pt = data;
        for( k = 0; k <= len; k++ ) {
            if( *ps == in_esc ) {           // translate needed
                ps++;                       // skip escape char
                k++;                        // and count
                *pt = cop_in_trans( *ps, font );   // translate
                ps++;
                pt++;
            } else {
                *pt++ = *ps++;              // else copy byte
            }
        }
        if( pt < ps ) {                     // something translated
            len -= (ps - pt);               // new length
            *pt = ' ';
        }
    }
    return( len );
}


/***************************************************************************/
/*  set position                                                           */
/***************************************************************************/

void set_h_start( void )
{
    if( !ProcFlags.keep_left_margin ) {
        g_cur_left = g_page_left + g_indent + nest_cb->left_indent + nest_cb->align;
    }
    g_cur_h_start = g_cur_left;

}


/***************************************************************************/
/* finalize line and place into t_element                                  */
/* Note: this now presumes that a_line->next is NULL on entry              */
/***************************************************************************/

void process_line_full( text_line * a_line, bool justify )
{
    bool            do_retab;       // retab all tab stops on new line
    text_chars  *   split_chars;    // first text_chars of second line, if any
    text_chars  *   test_chars;     // used to set do_retab
    text_line   *   b_line  = NULL; // for second line, if any
    uint32_t        offset;         // to adjust second line x_address fields, if any

    if( (a_line == NULL) || (a_line->first == NULL) ) { // why are we called?
        return;
    }
    if( !ProcFlags.start_section ) {
        start_doc_sect();
    }

    /************************************************************************/
    /* Split a_line if all of these conditions are met:                     */
    /*   concatenation is on                                                */
    /*   user tabs were available                                           */
    /*   at least one user tab was skipped                                  */
    /*   a default tab was used                                             */
    /*   a_line will have at least one text_chars when the split is done    */
    /*   b_line will start with a text_chars whose position was not based   */
    /*     on a tab stop                                                    */
    /* The exact point at which the split occurs depends on whether or not  */
    /*   the first tab not at the start of the line was preceded by a       */
    /*   token or by more than one token.                                   */
    /* Note that this is what wgml 4.0 does, and how it does it             */
    /************************************************************************/

    if( user_tab_skip && def_tab && (user_tabs.current > 0) && ProcFlags.concat
                && (a_line->first->next != NULL) ) {
        split_chars = a_line->first->next;      // get second text_chars
        while( (split_chars != NULL) && (split_chars->width == 0) ) {
            split_chars = split_chars->next;
        }
        if( split_chars != NULL ) {                         // second token found
            if( split_chars->tab_pos == tt_none ) {         // multiple tokens before first internal tab
                while( (split_chars != NULL)
                        && (split_chars->tab_pos == tt_none) ) {    // find next text_chars positioned by a tab stop
                    split_chars = split_chars->next;
                }

                if( split_chars != NULL ) {         // there is a second tab stop

                    /* find first tt_def text_chars */

                    test_chars = split_chars;               // start from first text_chars set by a tab stop
                    if( test_chars->tab_pos == tt_def ) {
                        test_chars = test_chars->next;
                    } else {
                        while( (test_chars != NULL) && (test_chars->tab_pos != tt_def) ) {
                            test_chars = test_chars->next;
                        }
                    }

                    /* find second tt_def text_chars, if any */

                    while( (test_chars != NULL) && (test_chars->tab_pos != tt_def) ) {
                        test_chars = test_chars->next;
                    }
                    do_retab = (test_chars == NULL);        // true if only one default tab used

                    if( split_chars->prev != NULL ) {       // separate the line unless nothing would be left on a_line
                        split_chars = split_chars->prev;    // the text_chars before the next tab
                        b_line = alloc_text_line();
                        b_line->line_height = 0;            // will be set below
                        a_line->next = b_line;
                        b_line->first = split_chars;
                        b_line->last = a_line->last;
                        a_line->last = split_chars->prev;
                        b_line->first->prev = NULL;
                        a_line->last->next = NULL;
                        split_chars = b_line->first;
                        while( split_chars != NULL ) {      // set b_line height
                            if( b_line->line_height < wgml_fonts[split_chars->font].line_height ) {
                                b_line->line_height = wgml_fonts[split_chars->font].line_height;
                            }
                            split_chars = split_chars->next;
                        }
                        split_chars = b_line->first;
                        offset = split_chars->x_address - g_cur_left;
                        split_chars->x_address = g_cur_left;    // shift x_address to margin
                        split_chars->tab_pos = tt_none;         // not tabbed any longer if ever was
                        c_stop = NULL;                          // reset tabbing state
                        if( (split_chars->count == 0)
                                && (split_chars->x_address == split_chars->next->x_address) ) { // check for line starting with marker
                            g_cur_h_start = a_line->first->x_address + a_line->first->count;    // prior line first text_chars position
                        } else {
                            g_cur_h_start = split_chars->x_address + split_chars->count;  // current line pre-tab positioning posititon
                        }
                        if( !do_retab ) {
                            split_chars = split_chars->next;        // shift text_chars to first default tab
                            while( split_chars != NULL ) {
                                split_chars->x_address -= offset;
                                if( split_chars->tab_pos == tt_user ) {
                                    split_chars->tab_pos = tt_none;     // not tabbed any longer 
                                    g_cur_h_start = split_chars->x_address + split_chars->count;  // current line pre-tab positioning position
                                }
                                split_chars = split_chars->next;
                            }
                        }
                        redo_tabs( b_line );
                    }
                }
            } else {    // only one token before first internal tab
                while( (split_chars != NULL) && (split_chars->tab_pos != tt_def) ) {   // get first default tab stop
                    split_chars = split_chars->next;
                }
                if( split_chars != NULL ) { // will be NULL if line ends in a tab character which is also the only default tab
                    if( split_chars->prev->tab_pos == tt_none ) {
                        split_chars = split_chars->prev;            // multiple tokens: split at last
                    }
                    b_line = alloc_text_line();
                    b_line->line_height = 0;            // will be set below
                    a_line->next = b_line;
                    b_line->first = split_chars;
                    b_line->last = a_line->last;
                    a_line->last = split_chars->prev;
                    b_line->first->prev = NULL;
                    a_line->last->next = NULL;
                    split_chars = b_line->first;
                    while( split_chars != NULL ) {      // set b_line height
                        if( b_line->line_height < wgml_fonts[split_chars->font].line_height ) {
                            b_line->line_height = wgml_fonts[split_chars->font].line_height;
                        }
                        split_chars = split_chars->next;
                    }
                    split_chars = b_line->first;
                    offset = split_chars->x_address - g_cur_left;
                    split_chars->x_address = g_cur_left;    // shift x_address to margin
                    split_chars->tab_pos = tt_none;         // not tabbed any longer if ever was
                    c_stop = NULL;                          // reset tabbing state
                    if( (split_chars->count == 0)
                            && (split_chars->x_address == split_chars->next->x_address) ) { // check for line starting with marker
                        g_cur_h_start = a_line->first->x_address + a_line->first->count;    // prior line first text_chars position
                    } else {
                        g_cur_h_start = split_chars->x_address + split_chars->count;  // current line pre-tab positioning posititon
                    }
                    redo_tabs( b_line );
                }
            }
        }
    }
    def_tab = false;                        // reset for next a_line
    user_tab_skip = false;

    while( a_line != NULL ) {
        if( justify && GlobalFlags.lastpass && !ProcFlags.literal
                                         && ProcFlags.justify > ju_off ) {
            do_justify( ju_x_start, g_page_right, a_line );
        }

        if( t_element == NULL ) {
            t_element = alloc_doc_el( el_text );

            if( !ProcFlags.skips_valid) {
                set_skip_vars( NULL, NULL, NULL, 1, g_curr_font );
            }
            t_element->blank_lines = g_blank_lines;
            g_blank_lines = 0;
            t_element->subs_skip = g_subs_skip;
            t_element->top_skip = g_top_skip;
            t_element->depth = a_line->line_height + g_spacing;
            t_element->element.text.overprint = ProcFlags.overprint;
            ProcFlags.overprint = false;
            t_element->element.text.spacing = g_spacing;
            t_element->element.text.first = a_line;
            t_el_last = t_element->element.text.first;
            ProcFlags.skips_valid = false;
        } else {
            t_element->depth += a_line->line_height + t_element->element.text.spacing;
            t_el_last->next = a_line;
            t_el_last = t_el_last->next;
        }
        a_line = a_line->next;
    }
    ProcFlags.line_started = false;     // line is now empty
    ProcFlags.just_override = true;     // justify for following lines
    tabbing = false;                    // tabbing ends when line committed

    set_h_start();
    return;
}


/***************************************************************************/
/*  create a text_chars instance and fill it with a 'word'                 */
/***************************************************************************/

text_chars *process_word( const char *pword, size_t count, font_number font )
{
    text_chars  *   n_chars;

    n_chars = alloc_text_chars( pword, count, font );
    // remove end-of-line spaces if .co off before input translation
    if( !ProcFlags.concat && (input_cbs->fmflags & II_eol) ) {
        if( n_chars->text[n_chars->count - 1] == ' ' ) {
            while( n_chars->text[n_chars->count - 1] == ' ' ) {
                n_chars->count--;
            }
            n_chars->width = text_chars_width( n_chars->text, n_chars->count, font );
        }
    }
    n_chars->count = intrans( n_chars->text, n_chars->count, font );
    if( n_chars->count == 0 ) {
        n_chars->width = 0;
    } else {
        n_chars->width = text_chars_width( n_chars->text, n_chars->count, font );
    }

    return( n_chars );
}


/***************************************************************************/
/*  process text  (input line or part thereof)                             */
/*      if section start processing not yet done do it now                 */
/*      split into 'words'                                                 */
/*      translate if input translation active                              */
/*      calculate width with current font                                  */
/*      resolve tab characters                                             */
/*      add text to output line                                            */
/*      handle line and page overflow conditions                           */
/***************************************************************************/

void process_text( const char *text, font_number font )
{
    const char          *   pword;
    const char          *   p;

    font_number             temp_font       = 0;
    i_flags                 flags_x_eol;                // fmflags with II_eol removed
    size_t                  count;
    text_chars          *   fm_chars;                   // start text_chars for inserting fmflags
    text_chars          *   h_chars;                    // hyphen text_chars
    text_chars          *   n_chars;                    // new text_chars
    text_chars          *   s_chars;                    // save text_chars
    uint32_t                o_count         = 0;
    uint32_t                o_length        = 0;
    uint32_t                offset          = 0;
    // when hyph can be set, it will need to be used here & below
    uint32_t                hy_width        = wgml_fonts[FONT0].width_table['-'];

    static      text_type       typ             = norm;
    static      text_type       typn            = norm;
    static      macrocb     *   prev_mac        = NULL;

    /********************************************************************/
    /*  we need a started section for text output                       */
    /*  note: ProcFlags.doc_sect will be doc_sect_body                  */
    /********************************************************************/

    if( !ProcFlags.start_section ) {
        start_doc_sect();
    }

    /********************************************************************/
    /*  no text between :GDOC and the first of :ABSTRACT, :PREFACE,     */
    /*  :BODY, :APPENDIX, or :BACKM except between :TITLEP and :eTITLEP */
    /********************************************************************/

    if( ProcFlags.doc_sect < doc_sect_abstract ) {
        if( ProcFlags.doc_sect != doc_sect_titlep ) {
            xx_line_err( err_doc_sec_expected_2, text );
            return;
        }
    }

    /********************************************************************/
    /*  force a break in when certain conditions involving new input    */
    /*  lines, user tabs, and a tab character in the input              */
    /*  Note: this only works if the tab is in the first logical record */
    /********************************************************************/

    p = text;
    while( *p ) {                       // locate the first wgml tab, if any
        if( (*p == '\t') || (*p == tab_char) ) {
            break;
        }
        p++;
    }
    count = p - text;
    p = text;                               // restore p to start of text

    if( (tt_stack != NULL) 
            && (input_cbs->fmflags & II_sol)
            && ((*p == '\t') || (*p == tab_char)) ) {    // adjust parent font, see Wiki
        tt_stack->font = FONT0; // hard-wired: LAYOUT DEFAULT FONT does not affect this
        font = FONT0;
        g_curr_font = FONT0;
    }

    /* Check for script style font */

    if( script_style.style != SCT_none ) {
        font = scr_style_font( font );
    }

    phrase_start = true;
    if( !ProcFlags.ct ) {                   // not if text follows CT
        if( count != strlen( p ) ) {        // tab character found in input text
            if( input_cbs->fmflags & II_sol ) {   // at start of input line

                /****************************************************/
                /* the last line need not have contained a tab      */
                /* character but, if it did, all user tabs must     */
                /* have been used; if it did or didn't, an initial  */
                /* tab character also produces a break              */
                /****************************************************/

                if( ((c_stop != NULL) && (c_stop->column >=
                    user_tabs.tabs[user_tabs.current - 1].column)) ||
                    (*p == '\t') || (*p == tab_char) ) {    // the final test

                    /********************************************************/
                    /* scr_process_break() can produce a doc_element with a */
                    /* blank line; ensure that line has the line height of  */
                    /* font 0                                               */
                    /* Note: this is necessary after P and PC, but appears  */
                    /* to work more generally                               */
                    /********************************************************/

                    temp_font = g_curr_font;
                    g_curr_font = FONT0;
                    scr_process_break();        // treat new line as break
                    tab_space = 0;

                    /* Restore font for current text fragment */

                    g_curr_font = temp_font;

                    /* Remove any indent; needed for P and PC */

                    g_cur_h_start = g_cur_left;
                }
            }
        }
    }

    if( t_line == NULL ) {
        t_line = alloc_text_line();
    }
    if( t_line->first == NULL ) {    // first phrase in paragraph
        post_space = 0;
        tab_space = 0;
        if( ProcFlags.concat ) {    // ".co on": skip initial spaces
            while( *p == ' ' ) {
                p++;
                tab_space++;
            }
        } else {                    // ".co off": compute initial spacing
            while( *p == ' ' ) {
                post_space += wgml_fonts[font].spc_width;
                p++;
                tab_space++;
            }
        }
        ju_x_start = g_cur_h_start; // g_cur_h_start appears to be correct
    } else {                        // subsequent phrase in paragraph
        if( ProcFlags.concat ) {    // ".co on"
            if( post_space == 0 ) {
                // compute initial spacing if needed; .ct and some user tags affect this
                if( *p == ' ' ) {
                    post_space = wgml_fonts[font].spc_width;
                } else if( !ProcFlags.ct && (ju_x_start <= t_line->last->x_address) ) {
                    if( input_cbs->fmflags & II_sol ) {
                        post_space = wgml_fonts[font].spc_width;
                    } else if( (input_cbs->fmflags & II_tag) && !ProcFlags.utc ) {
                        post_space = wgml_fonts[font].spc_width;
                    } else if( input_cbs->fmflags & II_macro ) {
                        if( ((font != FONT0) || (g_prev_font == FONT0))
                                && (input_cbs->s.m != prev_mac) ) {
                            post_space = wgml_fonts[FONT0].spc_width;
                        } else if( (font != 0) && (g_prev_font != 0) ) {
                            post_space = wgml_fonts[font].spc_width;
                        }
                    }
                }
                if( post_space > 0 ) {
                    if( is_stop_char( t_line->last->text[t_line->last->count - 1] ) ) {
                        post_space += wgml_fonts[font].spc_width;
                    }
                    if( (c_stop != NULL) && (t_line->last->width == 0) ) {
                        while( *p ) {   // locate the first non-space character, if any
                            if( *p != ' ' ) {
                                break;
                            }
                            p++;
                        }
                        tab_space = p - text;
                        p = text;       // restore p to start of text
                        if( tab_space != strlen( p ) ) {  // something follows the spaces
                            if( count == tab_space ) {     // and it is a tab character
                                // insert a marker
                                g_cur_h_start += wgml_fonts[font].spc_width;
                                n_chars = alloc_text_chars( NULL, 0, font );
                                n_chars->x_address = g_cur_h_start;
                                t_line->last->next = n_chars;
                                n_chars->prev = t_line->last;
                                if( t_line->line_height < wgml_fonts[font].line_height ) {
                                    t_line->line_height = wgml_fonts[font].line_height;
                                }
                                t_line->last = n_chars;
                                n_chars = NULL;
                            }
                        }
                    }
                }
            }
            while( *p == ' ' ) {    // skip initial spaces
                p++;
            }
        } else {                    // ".co off": increment initial spacing
            while( *p == ' ' ) {
                post_space += wgml_fonts[font].spc_width;
                p++;
            }
            if( !*p ) { // text is entirely spaces
                n_chars = process_word( NULL, 0, font );
                n_chars->type = norm;
                g_cur_h_start += post_space;
                post_space = 0;
                n_chars->x_address = g_cur_h_start;
                t_line->last->next = n_chars;
                n_chars->prev = t_line->last;
                if( t_line->line_height < wgml_fonts[font].line_height ) {
                    t_line->line_height = wgml_fonts[font].line_height;
                }
                t_line->last = n_chars;
                n_chars = NULL;
                return;
            }
        }
    }
    h_chars = NULL;
    n_chars = NULL;
    pword = p;                          // remember word start
    while( *p ) {
        if( *p == function_escape ) {   // special sub/superscript...
            switch( *(p + 1) ) {
            case function_subscript :   // start of subscript
                typn = sub;
                break;
            case function_superscript : // start of superscript
                typn = sup;
                break;
            case function_end:
            case function_sub_end:      // perhaps different action TBD
            case function_sup_end:
                typn = norm;
                break;
            default:
                out_msg( "gproctxt.c unknown function escape %#.02x\n",
                         *(p + 1) );
                wng_count++;
                show_include_stack();
                typn = norm;            // set normal mode TBD
            }
            if( p > pword ) {
                count = p - pword;      // no of bytes

                n_chars = process_word( pword, count, font );
                n_chars->phrase_font = g_phrase_font;
                n_chars->type = typ;
            }
            typ = typn;
            p += 2;
            pword = p;

        } else {                        // no function escape
            p++;
            if( *p ) {                  // process last word inside loop
                if( *p != ' ' ) {       // no space no word end
                    continue;
                }
                if( ProcFlags.in_trans && (*(p - 1) == in_esc) ) {
                    if( ((p - 2) >= pword) && (*(p - 2) == in_esc) ) {
                        // two in_esc in a row do not guard a space
                    } else {
                        continue;           // guarded space no word end
                    }
                }
                if( !ProcFlags.concat && (cur_group_type != gt_xmp) ) { // .co off but not XMP: include internal spaces
                    continue;
                }
            }
        }
        if( n_chars == NULL ) {
            // can happen at end of phrase or after function_escape sequence
            if( p == pword ) { // avoid unwanted empty text_chars
                continue;
            }
            count = p - pword;          // no of bytes
            n_chars = process_word( pword, count, font );
            n_chars->phrase_font = g_phrase_font;
            n_chars->type = typ;
            typ = typn;
        }
        g_cur_h_start += post_space;
        n_chars->x_address = g_cur_h_start;
        o_count = n_chars->count;       // catches special case below
        o_length = n_chars->length;

        if( ProcFlags.concat || (cur_group_type == gt_xmp) ) { // .co on or XMP

            /***********************************************************/
            /* test if word exceeds right margin                       */
            /* this no longer excludes cases where tabbing == true     */
            /***********************************************************/

            while( (n_chars->x_address + n_chars->width) > g_page_right ) {
                s_chars = t_line->last; // find multipart words
                if( s_chars != NULL ) {
                    while( g_cur_h_start == (s_chars->x_address + s_chars->width) ) {
                        g_cur_h_start = s_chars->x_address;
                        s_chars = s_chars->prev;
                        if( s_chars == NULL ) break;
                    }
                }

                /* Identify when s_chars must be moved to a new line. */

                if( s_chars != NULL ) {
                    // t_line ends in a multi-part word or an empty text_chars
                    if( ((s_chars != t_line->last) &&
                        ((g_cur_left + n_chars->width) <= g_page_right)) ||
                                                (s_chars->count == 0) ) {
                        // s_chars itself belongs to t_line
                        t_line->last = s_chars;
                        s_chars = s_chars->next;
                        if( s_chars != NULL ) {
                            s_chars->prev = NULL;
                        }
                        t_line->last->next = NULL;
                    } else { // s_chars will not be used; this prevents a loop
                        s_chars = NULL;
                    }
                }

                if( s_chars == NULL ) { // append n_chars to t_line & split it
                    // these conditions determine if n_chars is to be split
                    if( (t_line->first == NULL) ||
                        ((t_line->last->x_address + t_line->last->width)
                                                == n_chars->x_address) ||
                            ((g_cur_left + n_chars->width) > g_page_right) ) {
                        // find the split position with a hyphen's width
                        count = split_text( n_chars, g_page_right - hy_width );
                        if( count == 0 ) { // n_chars fits entirely
                            break;
                        }
                        // if count == o_count, there is nothing to do
                        if( count != o_count ) { // split n_chars with hyphenation
                            // first attach n_chars to tline
                            if( t_line->first == NULL ) {
                                t_line->first = n_chars;
                            } else {
                                n_chars->prev = t_line->last;
                                t_line->last->next = n_chars;
                            }
                            t_line->last = n_chars;
                            // now do the split
                            n_chars = alloc_text_chars(
                                    &t_line->last->text[t_line->last->count],
                                            count, t_line->last->font );
                            n_chars->type = t_line->last->type;

                            if( t_line->first == t_line->last ) {
                                fm_chars = NULL;
                            } else {
                                fm_chars = t_line->last->prev;
                            }
                            wgml_tabs();
                            if( fm_chars == NULL ) {
                                fm_chars = t_line->first;
                            } else {
                                if( fm_chars->next != NULL ) {
                                    fm_chars = fm_chars->next;
                                }
                            }
                            if( t_line->line_height < wgml_fonts[font].line_height ) {
                                t_line->line_height = wgml_fonts[font].line_height;
                            }
                            // each line must have its own hyphen
                            h_chars = alloc_text_chars( "-", 1, 0 );
                            h_chars->type = norm;
                            h_chars->width = hy_width;

                            h_chars->x_address = t_line->last->x_address
                                                    + t_line->last->width;
                            h_chars->prev = t_line->last;
                            t_line->last->next = h_chars;
                            t_line->last = h_chars;
                            h_chars = NULL;

                            if( t_line->line_height < wgml_fonts[font].line_height ) {
                                t_line->line_height = wgml_fonts[font].line_height;
                            }

                            /* Now set fmflags inside the text_chars */

                            flags_x_eol = input_cbs->fmflags & ~II_eol;
                            fm_chars->fmflags = flags_x_eol;    
                            input_cbs->fmflags &= ~II_sol;   // clear flag
                            fm_chars = fm_chars->next;
                            while( fm_chars != NULL ) {
                                fm_chars->fmflags = flags_x_eol;
                                fm_chars = fm_chars->next;
                            }
                        }
                    }
                }

                if( t_line->first != NULL ) { // t_line is ready for output
                    process_line_full( t_line, ProcFlags.concat
                                      && (ProcFlags.justify > ju_off) );
                    t_line = NULL;
                }
                // s_chars processing
                if( s_chars == NULL ) {
                    n_chars->x_address = g_cur_left;
                } else {
                    if( t_line == NULL ) {
                        t_line = alloc_text_line();
                    }
                    t_line->first = s_chars;
                    t_line->last = s_chars;
                    // s_chars must be repositioned to the start of the line
                    offset = s_chars->x_address - g_cur_left;
                    s_chars->x_address = g_cur_left;
                    if( t_line->line_height < wgml_fonts[font].line_height ) {
                        t_line->line_height = wgml_fonts[font].line_height;
                    }
                    // now reposition the remainder of the list
                    s_chars = s_chars->next;
                    while( s_chars != NULL ) {
                        t_line->last = s_chars;
                        s_chars->x_address -= offset;
                        g_cur_h_start = s_chars->x_address;
                        if( t_line->line_height < wgml_fonts[font].line_height ) {
                            t_line->line_height = wgml_fonts[font].line_height;
                        }
                        s_chars = s_chars->next;
                    }
                    // n_chars must also be repositioned
                    n_chars->x_address -= offset;
                    g_cur_h_start = n_chars->x_address;
                }
                //reset n_chars for the next pass of the loop
                n_chars->width = text_chars_width( n_chars->text, n_chars->count,
                                                   n_chars->font );
            }
        } else { // concatenation is off
            count = split_text( n_chars, g_page_right );
            // split the text is split over as many lines as necessary
            while( count > 0 ) {
                if( t_line == NULL ) {
                    t_line = alloc_text_line();
                }
                if( t_line->first == NULL ) {
                    t_line->first = n_chars;
                    t_line->y_address = g_cur_v_start;
                    t_line->line_height = wgml_fonts[font].line_height;
                    ju_x_start = n_chars->x_address;
                    ProcFlags.line_started = true;
                } else {
                    t_line->last->next = n_chars;
                    n_chars->prev = t_line->last;
                    if( t_line->line_height < wgml_fonts[font].line_height ) {
                        t_line->line_height = wgml_fonts[font].line_height;
                    }
                    t_line->last = n_chars;
                    // reset n_chars to contain the rest of the split text
                    n_chars = alloc_text_chars(
                        &t_line->last->text[t_line->last->count], count,
                                                    t_line->last->font );
                }
                if( t_line->first == t_line->last ) {
                    fm_chars = NULL;
                } else {
                    fm_chars = t_line->last->prev;
                }
                wgml_tabs();
                if( fm_chars == NULL ) {
                    fm_chars = t_line->first;
                } else {
                    if( fm_chars->next != NULL ) {
                        fm_chars = fm_chars->next;
                    }
                }

                /* Now set fmflags inside the text_chars */

                flags_x_eol = input_cbs->fmflags & ~II_eol;
                fm_chars->fmflags = flags_x_eol;    
                input_cbs->fmflags &= ~II_sol;   // clear flag
                fm_chars = fm_chars->next;
                while( fm_chars != NULL ) {
                    fm_chars->fmflags = flags_x_eol;
                    fm_chars = fm_chars->next;
                }

                // process the full text_line
                process_line_full( t_line, false );
                t_line = NULL;

                // reset n_chars and count
                n_chars->x_address = g_cur_h_start;
                n_chars->width = text_chars_width( n_chars->text, n_chars->count,
                                                   n_chars->font );
                count = split_text( n_chars, g_page_right );
            }
        }
        // adding n_chars to t_line is always correct at this point
        if( t_line == NULL ) {
            t_line = alloc_text_line();
        }
        if( t_line->first == NULL ) {    // first element in output line
            t_line->first = n_chars;
            t_line->line_height = wgml_fonts[font].line_height;
            ju_x_start = n_chars->x_address;
            ProcFlags.line_started = true;
        } else {
            t_line->last->next = n_chars;
            n_chars->prev = t_line->last;
            if( t_line->line_height < wgml_fonts[font].line_height ) {
                t_line->line_height = wgml_fonts[font].line_height;
            }
        }
        t_line->last = n_chars;
        if( t_line->first == t_line->last ) {
            fm_chars = NULL;
        } else {
            fm_chars = t_line->last->prev;
        }
        wgml_tabs();
        if( fm_chars == NULL ) {
            fm_chars = t_line->first;
        } else {
            if( fm_chars->next != NULL ) {
                fm_chars = fm_chars->next;
            }
        }

        /* Now set fmflags inside the text_chars */

        if( fm_chars->next == NULL ) {
                fm_chars->fmflags = input_cbs->fmflags;  // allow II_eol in only text_chars
        } else {            
            flags_x_eol = input_cbs->fmflags & ~II_eol;
            fm_chars->fmflags = flags_x_eol;    
            input_cbs->fmflags &= ~II_sol;   // clear flag
            fm_chars = fm_chars->next;
            while( fm_chars != NULL ) {
                if( fm_chars->next == NULL ) {
                    fm_chars->fmflags = input_cbs->fmflags;  // allow II_eol in last text_chars
                } else {
                    fm_chars->fmflags = flags_x_eol;
                }
                fm_chars = fm_chars->next;
            }
        }

        // exit at end of text unless at end of input line
        if( !(input_cbs->fmflags & II_eol) && !*p ) {
            break;
        }

        if( *p == ' ' ) {                                       // spaces to process
            pword = p;
                post_space = wgml_fonts[font].spc_width;
                if( is_stop_char( t_line->last->text[t_line->last->count - 1] )
                        && (cur_group_type != gt_xmp) ) {   // exclude XMP 
                    post_space += wgml_fonts[font].spc_width;
                }
                p++;
                while( *p == ' ' ) {
                    if( (cur_group_type == gt_xmp) ) {   // multiple blanks
                        post_space += wgml_fonts[font].spc_width;
                    }                
                    p++;
                }
                p--;                    // back off non-space character, whatever it was
                tab_space = p - pword + 1;
                if( (input_cbs->fmflags & II_eol) && (tab_space == 0) ) {
                    tab_space = 1;
                }
                pword = p + 1;          // new word start or end of input record
        } else if( (input_cbs->fmflags & II_eol) && !*p ) { // insert spaces at actual end-of-line
            pword = p;
            post_space = wgml_fonts[font].spc_width;
            if( is_stop_char( t_line->last->text[t_line->last->count - 1] )
                    && (cur_group_type != gt_xmp) ) {   // exclude XMP 
                post_space += wgml_fonts[font].spc_width;
            }
        }
        n_chars = NULL;
    }

    /***********************************************************************/
    /*  for .co off and if the input line contains only spaces,            */
    /*  ensure an empty output line as wgml 4.0 does                       */
    /***********************************************************************/

    if( !ProcFlags.concat && (post_space > 0) && (t_line->first == NULL) ) {
        blank_lines++;
    }

    if( t_line->first != NULL ) {           // something in the line
        if( ProcFlags.need_li_lp ) {        // no text allowed!
            xx_err( err_tag_not_text );
        }

        if( !ProcFlags.concat ) {
            if( input_cbs->fmflags & II_eol ) {
                process_line_full( t_line, false );
                t_line = NULL;
            }
        }
    }
    if( input_cbs->fmflags & II_file ) {
        prev_mac = NULL;
    } else {
        prev_mac = input_cbs->s.m;
    }
    g_prev_font = font; // save font number for potential use with BX - TBD

    ProcFlags.ct = false;               // experimental TBD
    ProcFlags.fsp = false;              // experimental TBD
    ProcFlags.utc = false;              // experimental TBD
}

