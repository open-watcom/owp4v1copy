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
* Description:  WGML Process text not tags / controlwords
*
*
*               process_text            control routine
*               add_text_chars_to_pool  prepare reuse of a text_chars instance
*               alloc_text_chars        create a text_chars instance
*               do_justify              distribute the words on the line
*               intrans                 perform input translation
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "outbuff.h"

static void set_v_start( int8_t spacing );  // local prototype

/***************************************************************************/
/*  justification  experimental    treat half as left               TBD    */
/***************************************************************************/

void    do_justify( uint32_t lm, uint32_t rm, text_chars * tc )
{
    text_chars  *tw;
    int32_t     sum_w;
    int32_t     hor_end;
    int32_t     cnt;
    int32_t     line_width;
    int32_t     delta;
    int32_t     delta0;
    int32_t     rem;
    ju_enum     just;
    symsub  *   symjusub;

    if( ProcFlags.justify == ju_off || ProcFlags.literal || tc == NULL ) {
        return;
    }
    sum_w = 0;
    hor_end = 0;
    cnt = 0;
    tw = tc;
    do {                                // calculate used width
        cnt++;                          // number of 'words'
        sum_w += tw->width;             // sum of 'words' widths
        if( tw->next == NULL ) {        // last element
            hor_end = tw->x_address + tw->width;// hor end position
            break;
        }
        tw = tw->next;
    } while( tw != NULL );
//  line_width = rm - lm;
    line_width = rm - tc->x_address;    // TBD

    if( (sum_w <= 0) || (hor_end >= rm) || (line_width < 1) ) {
        return;                         // no justify needed / possible
    }
    delta0 = line_width + tc->x_address - hor_end;
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
        out_msg( "\n  ju_%s lm:%d rm:%d line_width:%d sum_w:%d hor_end:%d"
                 " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                 lm, rm, line_width, sum_w, hor_end, delta, rem, delta0, cnt );
    }
    if( delta < 1 && rem < 1 ) {        // nothing to distribute
        return;
    }
    switch( ProcFlags.justify ) { // convert inside / outside to left / right
    case ju_inside :                    // depending on odd / even page
        if( page & 1 ) {
            just = ju_left;
        } else {
            just = ju_right;
        }
        break;
    case ju_outside :
        if( page & 1 ) {
            just = ju_right;
        } else {
            just = ju_left;
        }
        break;
    default :
        just = ProcFlags.justify;
        break;
    }

    switch( just ) {
/*************************************
    case  ju_half :
        delta /= 2;
        if( delta < 1 && rem < 1 ) {
            break;
        }
        // falltrough
************************************** */
    case  ju_on :
        if( tc->x_address < lm ) {
            break;                      // left of left margin no justify
        }
        delta0 = delta;
        tw = tc->next;
        while( tw != NULL ) {
            if( rem > 0 ) {             // distribute remainder, too
                tw->x_address += delta + 1;
                delta += delta0 + 1;
                rem--;
            } else {
                tw->x_address += delta;
                delta += delta0;
            }
            tw = tw->next;
        }
        break;
    case  ju_half :                     // treat as left
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


void document_new_page( void )
{
    if( GlobalFlags.lastpass ) {
       fb_document_page();
    }
    page++;
    set_v_start( 1 );
}


void document_top_banner( void )
{
    int     ind = page & 1;

    if( sect_ban_top[ind] != NULL ) {
        g_cur_v_start = ban_top_pos( sect_ban_top[ind] );
    } else {
        g_cur_v_start = g_page_top;
    }

    g_cur_h_start = g_page_left_org;
    out_ban_top( sect_ban_top[ind] );
}

/***************************************************************************/
/*  if input translation is active                                         */
/*      look for input escape char and translate the follwoing char,       */
/*      delete the escape char                                             */
/***************************************************************************/

static  void    intrans( char * data, uint16_t * len, uint8_t font )
{
    char    *   ps;                     // source ptr
    char    *   pt;                     // target ptr
    uint32_t    k;

    if( !ProcFlags.in_trans ) {
        return;                         // input translation not active
    }
    k = *len;
    ps = data;
    pt = data;
    for( k = 0; k <= *len; k++ ) {
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
        *len -= (ps - pt);              // new length
        *pt = ' ';
    }
}

/***************************************************************************/
/*  allocate / reuse and init a text_chars  instance                       */
/***************************************************************************/

text_chars    * alloc_text_chars( char * p, size_t cnt, uint8_t font_num )
{
    text_chars   *   curr;
    text_chars   *   prev;

    curr = text_pool;
    while( (curr != NULL) && (curr->length <= cnt) ) {
        prev = curr;
        curr = curr->next;
    }
    if( curr != NULL ) {                // we found one large enough
        if( curr == text_pool ) {       // first is large enough
            text_pool = curr->next;
        } else {
            prev->next = curr->next;    // unchain curr
        }
    } else {                            // no one large enough found
        curr = mem_alloc( sizeof( *curr ) + cnt );
        curr->length = cnt;             // set max text size
    }

    curr->next = NULL;
    curr->font_number = font_num;
    curr->width = 0;
    curr->count = cnt;                  // set current size
    memcpy_s(curr->text, cnt + 1, p, cnt ); // and copy text
    curr->text[cnt] = 0;

    return( curr );
}


/***************************************************************************/
/*  add text_chars instance(s) to free pool for reuse                      */
/***************************************************************************/

void    add_text_chars_to_pool( text_line * a_line )
{
    text_chars      *   tw;

    if( text_pool == NULL ) {
        text_pool = a_line->first;
    } else {
        tw = text_pool;
        while( tw->next != NULL ) {
            tw = tw->next;
        }
        tw->next = a_line->first;
    }
}

/***************************************************************************/
/*  set position                                                           */
/***************************************************************************/
static void set_v_start( int8_t spacing )
{
    if( bin_driver->y_positive == 0x00 ) {
        if( !ProcFlags.output_started ) {   // TBD
            g_cur_v_start = bin_device->y_start - g_max_line_height;
        } else {
            g_cur_v_start -= spacing * g_max_line_height;
        }
    } else {
        if( !ProcFlags.output_started ) {   // TBD
            g_cur_v_start = bin_device->y_start;
        } else {
            g_cur_v_start += (spacing * g_max_line_height);
        }
    }
}

void set_h_start( void )
{
    g_cur_h_start = g_page_left = g_page_left_org;
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/

void    process_line_full( text_line * a_line, bool justify )
{
    if( a_line->first == NULL ) {       // why are we called?
        return;
    }
    if( para_line.first != NULL ) {     // buffered first line of paragraph
        if( justify && ProcFlags.justify > ju_off ) {
            do_justify( para_line.first->x_address, g_page_right,
                        para_line.first );
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( &para_line );
            }
        }
        fb_output_textline( &para_line );
        add_text_chars_to_pool( &para_line );
        para_line.first = NULL;
    }

    if( GlobalFlags.lastpass ) {
        if( ProcFlags.para_line1 ) { // first line of paragraph
//          para_line.next        = a_line->next;   // TBD
            para_line.line_height = a_line->line_height;
            para_line.y_address   = a_line->y_address;
            para_line.first       = a_line->first;
            a_line->first = NULL;
        } else {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( a_line );
            }
            if( justify && !ProcFlags.literal && ProcFlags.justify > ju_off ) {

                do_justify( g_page_left, g_page_right, a_line->first );
                if( input_cbs->fmflags & II_research ) {
                    test_out_t_line( a_line );
                }
            }
            fb_output_textline( a_line );
        }
    } else {
        ProcFlags.para_line1 = false;
    }
    if( ProcFlags.para_line1 ) {        // first line of paragraph
        ProcFlags.para_line1 = false;
    } else {
       add_text_chars_to_pool( a_line );
       a_line->first = NULL;
    }
    p_char = NULL;
    ProcFlags.just_override = true;     // justify for following lines
    set_v_start( layout_work.defaults.spacing );// not always OK TBD
    set_h_start();

    if( bin_driver->y_positive == 0 ) {
        if( g_cur_v_start <= g_page_bottom ) {
            finish_page();
        }
    } else {
        if( g_cur_v_start >= g_page_bottom ) {
            finish_page();
        }
    }
}


/***************************************************************************/
/*  process text  (input line or part thereof)                             */
/*      split into 'words'                                                 */
/*      translate if input translation active                              */
/*      calculate width with current font                                  */
/*      add text to output line                                            */
/*      handle line overflow condition                                     */
/***************************************************************************/

void    process_text( char * text, uint8_t font_num )
{
    text_chars          *   n_char;     // new text char
    size_t                  count;
    char                *   pword;
    char                *   p;

    p = text;
    if( ProcFlags.concat ) {            // experimental TBD
        while( *p == ' ' ) {
            p++;
        }
    }
    pword = p;                          // remember word start
    while( *p ) {
        p++;
        if( *p != ' ' ) {               // no space no word end
            continue;
        }
        if( ProcFlags.in_trans && *(p - 1) == in_esc ) {
            continue;                   // guarded space no word end
        } else {
            if( !ProcFlags.concat && ((*(p - 1) == ' ') || (*(p + 1) == ' ')) ) {
                while( *p == ' ' ) {    // more than 1 space no word end
                    p++;                // if .co off
                }
                p--;
            } else {                    // 'word' end
                count = p - pword;      // no of bytes
                n_char = alloc_text_chars( pword, count, font_num );

                intrans( n_char->text, &n_char->count, font_num );

                if( post_space_save > 0 ) {
                    pre_space  = post_space_save;
                    post_space = 0;
                    post_space_save = 0;
                } else {
                    pre_space = post_space;
                    post_space = 0;
                }
                if( t_line.first == NULL ) {
                    n_char->x_address = g_cur_h_start;
                    pre_space = 0;
                } else {
                    n_char->x_address = g_cur_h_start + pre_space;
                }
                input_cbs->fmflags &= ~II_sol;

                n_char->width = cop_text_width( n_char->text,
                                                n_char->count,
                                                font_num );
                if( pre_space + g_cur_h_start + n_char->width > g_page_right ) {
                    pre_space = 0;
                    process_line_full( &t_line, ProcFlags.just_override );
                    if( !ProcFlags.output_started ) {
                        document_new_page();
                        document_top_banner();
                    }
                    set_h_start();
                    n_char->x_address = g_cur_h_start;
                }
                ProcFlags.output_started = true;

                if( t_line.first == NULL ) {
                    t_line.first = n_char;
                    t_line.y_address = g_cur_v_start;
                } else {
                    p_char->next = n_char;
                }
                p_char = n_char;
                g_cur_h_start += n_char->width + pre_space;
                post_space = wgml_fonts[font_num].spc_width;
                if( is_stop_char( n_char->text[n_char->count - 1] ) ) {
                     post_space += wgml_fonts[font_num].spc_width;
                }

                if( ProcFlags.concat ) {// ignore multiple blanks in concat mode
                    while( *p == ' ' ) {
                        p++;
                    }
                    p--;
                }
                pword = p + 1;          // new word start or end of record
            }
        }
    }
    while( *p == ' ' ) {                // ??? TBD
        p--;
    }

    if( p > pword ) {                   // last word
        count = p - pword;              // no of bytes
        n_char = alloc_text_chars( pword, count, font_num );

        intrans( n_char->text, &n_char->count, g_curr_font_num );
        if( pword != text ) {           // last is not only word
            pre_space = post_space;
            post_space = 0;
        } else {
            if( post_space_save > 0 ) {
                pre_space  = post_space_save;
                post_space = 0;
                post_space_save = 0;
            } else {
                pre_space = post_space;
                post_space = 0;
            }
        }
        if( t_line.first == NULL ) {
            t_line.y_address = g_cur_v_start;
            n_char->x_address = g_cur_h_start;
            pre_space = 0;
        } else {
            n_char->x_address = g_cur_h_start + pre_space;
        }
        input_cbs->fmflags &= ~II_sol;
        n_char->width = cop_text_width( n_char->text, n_char->count,
                                        font_num );
        if( pre_space + g_cur_h_start + n_char->width > g_page_right ) {
            pre_space = 0;
            process_line_full( &t_line, ProcFlags.just_override );
            if( !ProcFlags.output_started ) {
                document_new_page();
                document_top_banner();
            }
            set_h_start();
            n_char->x_address = g_cur_h_start;
        }
        ProcFlags.output_started = true;

        if( t_line.first == NULL ) {
            t_line.first = n_char;
            t_line.y_address = g_cur_v_start;
        } else {
            p_char->next = n_char;
        }
        p_char = n_char;
        g_cur_h_start += n_char->width + pre_space;

        post_space = wgml_fonts[font_num].spc_width;
        if( is_stop_char( n_char->text[n_char->count - 1])) {
             post_space += wgml_fonts[font_num].spc_width;
        }
    } else {
        if( p_char != NULL ) {
            post_space_save = post_space;
        }
    }

    if( t_line.first != NULL ) {        // something in the line
        ProcFlags.output_started = true;

        if( !ProcFlags.concat && GlobalFlags.lastpass) {
            post_space = 0;
            post_space_save = 0;
            process_line_full( &t_line, ProcFlags.just_override );
            if( !ProcFlags.output_started ) {
                document_new_page();
                document_top_banner();
            }
            set_h_start();
        }
    }
}

