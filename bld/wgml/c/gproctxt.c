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
*               document_new_page       start a new page
*               document_top_banner     output top banner
*               do_justify              distribute the words on the line
*               intrans                 perform input translation
*               process_line_full
*               set_h_start             set horizontal start postion
*               set_v_start             set vertical start position
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "outbuff.h"

void set_v_start( int8_t spacing );


/***************************************************************************/
/*  .sk adjust                                                             */
/*                                                                         */
/*   g_skip_wgml4 contains  0 or 1                                    TBD  */
/*    1 for  .sk n  with n > 0                                             */
/***************************************************************************/
static void calc_skip( void )
{

    if( g_skip == 0 ) {
        ProcFlags.sk_cond = false;
        return;
    }
    if( g_skip < 0 ) {                  // overprint
       if( bin_driver->y_positive == 0x00 ) {
           g_cur_v_start += g_max_line_height;
       } else {
           g_cur_v_start -= g_max_line_height;
       }
    } else {
       if( bin_driver->y_positive == 0x00 ) {
           g_cur_v_start -= g_skip * g_max_line_height - g_skip_wgml4;
       } else {
           g_cur_v_start += g_skip * g_max_line_height;
       }
    }

    g_skip = 0;
    g_skip_wgml4 = 0;
    ProcFlags.sk_cond = false;
}


/***************************************************************************/
/*  puncadj modelled after the host ASM sources to get the same spacing    */
/*  as wgml4, but still no luck                                       TBD  */
/***************************************************************************/

static  void    puncadj( text_chars * tc, int32_t * delta0, int32_t rem, int32_t cnt )
{
    text_chars  *   tn;
    text_chars  *   tw;
    text_chars  *   tp[30];
    const size_t    tp_max = sizeof( tp ) / sizeof( tp[0] ) - 1;
    int32_t         tp_ind;
    int32_t         tp_start;
    int32_t         delta;
    int32_t         loop_cnt;
    int32_t         space = wgml_fonts[0].spc_width;
    int32_t         spacew;
    int32_t         remw = rem / cnt;
    char            ch;
    bool            changed;

    changed = false;
    tp_ind = tp_max;
    tw = tc;
    while( tw != NULL && tp_ind >= 0) { // construct back chain
        tp[tp_ind] = tw;
        tp_ind--;
        tw = tw->next;
    }
    tp_start = tp_ind + 2;
    delta = *delta0;
    loop_cnt = 3;                       // 3 passes
    while( loop_cnt > 0 && delta >= space ) {
        tp_ind = tp_start;
        while( tp_ind < tp_max ) {
            tw = tp[tp_ind++];
            tn = tw->next;
            ch = tw->text[tw->count - 1];
            switch( loop_cnt ) {
            case   3:                   // test full stop
                if( ch == '.' || ch == '!' || ch == '?' ) {
                    spacew = space;
                    if( remw > 0 ) {
                        spacew++;
                        remw--;
                    }
                    changed = true;
                    tn->x_address += spacew;
                    delta -= spacew;
                    while( tn->next != NULL ) {
                        tn = tn->next;
                        tn->x_address += spacew;
                    }
                }
                break;
            case   2:                   // test half stop
                if( ch == ':' || ch == ';' ) {
                    changed = true;
                    tn->x_address += space;
                    delta -= space;
                    while( tn->next != NULL ) {
                        tn = tn->next;
                        tn->x_address += space;
                    }
                }
                break;
            case   1:
                if( ch == ',' || ch == ')' ) {
                    changed = true;
                    tn->x_address += space;
                    delta -= space;
                    while( tn->next != NULL ) {
                        tn = tn->next;
                        tn->x_address += space;
                    }
                }
                break;
            default:
                break;
            }
        }
        loop_cnt--;
    }
    *delta0 = delta;
}


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
    int32_t     delta1;
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
        if( tw->x_address >= lm ) {  // no justify for words left of ju start
            if( cnt == 0 ) {
                tc = tw;               // remember first text_char to justify
            }
            cnt++;                      // number of 'words'
            sum_w += tw->width;         // sum of 'words' widths
        }
        if( tw->next == NULL ) {        // last element
            hor_end = tw->x_address + tw->width;// hor end position
            break;
        }
        tw = tw->next;
    } while( tw != NULL );

    line_width = rm - lm;

    if( (sum_w <= 0) || (hor_end >= rm) || (line_width < 1) ) {
        return;                         // no justify needed / possible
    }
    delta0 = rm - hor_end;              // TBD
//    delta0 = line_width + tc->x_address - hor_end;
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


        puncadj( tc, &delta0, rem, cnt - 1 );
        tw = tc;
        do {
            if( tw->next == NULL ) {    // last element
                hor_end = tw->x_address + tw->width;// hor end position
                break;
            }
            tw = tw->next;
        } while( tw != NULL );
        delta0 = rm - hor_end;          // TBD
        if( cnt < 2 ) {
            delta = delta0;             // one text_chars gets all space
            rem   = 0;
        } else {
            delta = delta0 / (cnt - 1);
            rem   = delta0 % (cnt - 1);
        }

        if( input_cbs->fmflags & II_research && GlobalFlags.lastpass ) {
            find_symvar( &sys_dict, "$ju", no_subscript, &symjusub);
            out_msg( "\n  ju_%s lm:%d rm:%d line_width:%d sum_w:%d hor_end:%d"
                     " delta:%d rem:%d delta0:%d cnt:%d\n", symjusub->value,
                     lm, rm, line_width, sum_w, hor_end, delta, rem, delta0,
                     cnt );
        }
        if( delta < 1 && rem < 1 ) {    // nothing to distribute
            return;
        }


        delta1 = delta;
        tw = tc->next;
        while( tw != NULL ) {
            if( rem > 0 ) {             // distribute remainder, too
                tw->x_address += delta + 1;
                delta += delta1 + 1;
                rem--;
            } else {
                tw->x_address += delta;
                delta += delta1;
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


/***************************************************************************/
/*  start a new document page                                              */
/***************************************************************************/

void document_new_page( void )
{
    if( GlobalFlags.lastpass ) {
       fb_document_page();
    }
    page++;
    ProcFlags.page_started = false;     // ? TBD
    set_v_start( 1 );
}


/***************************************************************************/
/*  output top banner                                                      */
/***************************************************************************/

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
/*      look for input escape char and translate the following char,       */
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
/*  allocate / reuse and init a text_chars instance                        */
/*      optionally fill in text                                            */
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
    if( p != NULL ) {                   // text supplied
        memcpy_s(curr->text, cnt + 1, p, cnt ); // yes copy text
        curr->count = cnt;              // set current size
    } else {
        curr->count = 0;                // init current size
        curr->text[0] = 0;
    }
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
void set_v_start( int8_t spacing )
{
    if( bin_driver->y_positive == 0x00 ) {
        if( !ProcFlags.page_started ) {   // TBD
            g_cur_v_start = bin_device->y_start - g_max_line_height;
        } else {
            g_cur_v_start -= spacing * g_max_line_height;
        }
    } else {
        if( !ProcFlags.page_started ) {   // TBD
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
            do_justify( para_line.ju_x_start, g_page_right,
                        para_line.first );
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( &para_line );
            }
        }
        if( GlobalFlags.lastpass ) {
            fb_output_textline( &para_line );
        }
        add_text_chars_to_pool( &para_line );
        para_line.first = NULL;
    }

    if( GlobalFlags.lastpass ) {
        if( ProcFlags.para_line1 ) { // first line of paragraph
//          para_line.next        = a_line->next;   // TBD
            para_line.line_height = a_line->line_height;
            para_line.y_address   = a_line->y_address;
            para_line.ju_x_start  = a_line->ju_x_start;
            para_line.first       = a_line->first;
            a_line->first = NULL;
        } else {
            if( input_cbs->fmflags & II_research ) {
                test_out_t_line( a_line );
            }
            if( justify && !ProcFlags.literal && ProcFlags.justify > ju_off ) {

                do_justify( a_line->ju_x_start, g_page_right, a_line->first );
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
    set_v_start( spacing );             // not always OK TBD
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
/*      if section start processing not yet done do it now                 */
/*      split into 'words'                                                 */
/*      translate if input translation active                              */
/*      calculate width with current font                                  */
/*      add text to output line                                            */
/*      handle line and page overflow condition                            */
/***************************************************************************/

void    process_text( char * text, uint8_t font_num )
{
    text_chars          *   n_char;     // new text char
    size_t                  count;
    char                *   pword;
    char                *   p;


    if( !ProcFlags.prep_section ) {
        prepare_doc_sect( ProcFlags.doc_sect );
    }

    if( ProcFlags.page_started ) {
        if( bin_driver->y_positive == 0x00 ) {
            if( g_cur_v_start <= g_page_bottom ) {
                finish_page();
            }
        } else {
            if( g_cur_v_start >= g_page_bottom ) {
                finish_page();
            }
        }
    }



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
                    t_line.ju_x_start = g_cur_h_start;
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
                    process_line_full( &t_line, ProcFlags.concat );
                    if( !ProcFlags.page_started ) {
                        document_new_page();
                        document_top_banner();
                    }
                    set_h_start();
                    n_char->x_address = g_cur_h_start;
                }

                if( t_line.first == NULL ) {
                    pre_space = 0;
                    if( !ProcFlags.top_ban_proc ) {
                        document_new_page();
                        document_top_banner();
                    }
                    calc_skip();
                    t_line.first = n_char;
                    t_line.y_address = g_cur_v_start;
                    t_line.ju_x_start = n_char->x_address;
                } else {
                    p_char->next = n_char;
                }
                p_char = n_char;

                g_cur_h_start = n_char->x_address + n_char->width + pre_space;
                ProcFlags.page_started = true;
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
            pre_space = 0;
            n_char->x_address = g_cur_h_start;
            if( !ProcFlags.top_ban_proc ) {
                document_new_page();
                document_top_banner();
            }
            calc_skip();
            t_line.y_address = g_cur_v_start;
            t_line.ju_x_start = n_char->x_address;
            pre_space = 0;
        } else {
            n_char->x_address = g_cur_h_start + pre_space;
        }
        input_cbs->fmflags &= ~II_sol;
        n_char->width = cop_text_width( n_char->text, n_char->count,
                                        font_num );
        if( pre_space + g_cur_h_start + n_char->width > g_page_right ) {
            pre_space = 0;
            process_line_full( &t_line, ProcFlags.concat ); // TBD
            if( !ProcFlags.page_started ) {
                document_new_page();
                document_top_banner();
            }
            set_h_start();
            n_char->x_address = g_cur_h_start;
        }

        if( t_line.first == NULL ) {
            pre_space = 0;
            if( !ProcFlags.top_ban_proc ) {
                document_new_page();
                document_top_banner();
            }
            calc_skip();
            t_line.y_address = g_cur_v_start;
            t_line.first = n_char;
            t_line.ju_x_start = n_char->x_address;
        } else {
            p_char->next = n_char;
        }
        p_char = n_char;

        g_cur_h_start = n_char->x_address + n_char->width + pre_space;
        ProcFlags.page_started = true;

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
        ProcFlags.page_started = true;

        if( !ProcFlags.concat ) {
            post_space = 0;
            post_space_save = 0;
            if( input_cbs->fmflags & II_eol ) {
                process_line_full( &t_line, false );
                if( !ProcFlags.page_started ) {
                    document_new_page();
                    document_top_banner();
                }
                set_h_start();
            }
        }
    }
}

