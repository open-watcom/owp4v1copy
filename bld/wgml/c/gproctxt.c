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
*               alloc_text_char     create a text_chars instance
*               intrans             perform input translation
*               process_text
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "outbuff.h"


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
/*  allocate and init a text_chars instance                                */
/***************************************************************************/

static text_chars * alloc_text_char( char * p, uint8_t font_num )
{
    text_chars  *   curr_t;

    if( text_list.next != NULL ) {
        curr_t = text_list.next;
        text_list.next = curr_t->next;
    } else {
        curr_t = mem_alloc( sizeof( *curr_t ) );
    }

    curr_t->next = NULL;
    curr_t->font_number = font_num;
    curr_t->width = 0;
    curr_t->count = 0;
    curr_t->text = p;

    return( curr_t );
}



/***************************************************************************/
/*  process text                                                           */
/*      allocate (or reuse) buffer                                         */
/*      split into 'words'                                                 */
/*      translate if input translation active                              */
/*      calculate width with current font                                  */
/*      add text to output line                                 TBD        */
/***************************************************************************/

void    process_text( void )
{
    static  text_chars  *   text_work;
    static  bool            first = true;
    text_chars          *   curr_t;
    char                *   pb;
    char                *   p;

    if( first ) {                       // for testing
        first = false;
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
    }


    if( words.first == NULL ) {         // no output yet
        unused_buflist = used_buflist;  // reset buffer list to all unused
        used_buflist = 0;
    }
    if( used_buflist + 1 > unused_buflist ) {  // all used or no one to reuse
        if( used_buflist >= max_buflist ) {
            err_count++;
            out_msg( "wgml internal error: gproctxt too may buffers\n" );
            g_suicide();
        }
        buflist[used_buflist].buf = mem_alloc( buf_size );
    }
    pb = buflist[used_buflist].buf;     // (re) use buffer
    used_buflist++;

    memcpy_s( pb, buf_size, buff2, buf_size );  // copy input buffer

    p = pb;
    while( *p++ ) {
        if( *p != ' ' ) {       // no space no word end
            continue;
        }
        if( ProcFlags.in_trans && *(p - 1) == in_esc ) {
            continue;                   // guarded space no word end
        } else {
            if( *(p + 1) == ' ' ) {     // 2 or more spaces no word end
                while( *p == ' ' ) {
                    p++;
                }
                p--;
            } else {                    // word end
                curr_t = alloc_text_char( pb, curr_font_num );

                if( words.first == NULL ) {
                    words.first = curr_t;
                } else {
                    text_work->next = curr_t;
                }
                text_work = curr_t;

                curr_t->count = p - curr_t->text;   // no of bytes

                intrans( curr_t->text, &curr_t->count, curr_font_num );

                curr_t->width = cop_text_width( curr_t->text,
                                                curr_t->count,
                                                curr_font_num );
                pb = p + 1;             // new word start
            }
        }
    }
    if( p > pb ) {                      // last word
        curr_t = alloc_text_char( pb, curr_font_num );

        if( words.first == NULL ) {
            words.first = curr_t;
        } else {
            text_work->next = curr_t;
        }
        text_work = curr_t;

        curr_t->count = p - curr_t->text - (*p == '\0');// no of bytes

        intrans( curr_t->text, &curr_t->count, curr_font_num );

        curr_t->width = cop_text_width( curr_t->text, curr_t->count,
                                        curr_font_num );
    }
    /* no justification or concatenation            TBD */

    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        text_chars  *   tw;

        tw = words.first;
        while( tw != NULL ) {
            snprintf( buff2, buf_size,"font:%d width:%d count:%d text:'%.*s'\n",
                      tw->font_number, tw->width, tw->count, tw->count, tw->text );
            out_msg( buff2 );
            tw = tw->next;
        }
    }
    if( text_list.next == NULL ) {  // remember text_char instances for reuse
        text_list.next = words.first;
    } else {
        text_chars  *   tw = text_list.next;
        while( tw ) {
            if( tw->next == NULL ) {
                break;
            } else {
                tw = tw->next;
            }
        }
        tw->next = words.first;
    }
    words.first = NULL;                 // line now empty

#if 0
    if( words.y_address == 0 ) {
        words.y_address = bin_device->y_start - wgml_fonts[0].line_height;
    } else {
        words.y_address -= wgml_fonts[0].line_height;
    }
    fb_position( bin_device->x_start, words.y_address );
#endif
}

