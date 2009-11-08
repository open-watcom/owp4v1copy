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
* Description: WGML implement processing for banner output
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

static  text_line       b_line;         // for constructing banner line
static  text_chars  *   reg_text[3];    // 3 possible ban region parts

/***************************************************************************/
/*  calc banner top position                                               */
/***************************************************************************/

extern  uint32_t    ban_top_pos( banner_lay_tag * ban )
{
    uint32_t            ban_depth;
    uint32_t            v_start;
    uint32_t            reg_off;

    ban_depth = conv_vert_unit( &(ban->depth) );
    reg_off = conv_vert_unit( &(ban->region->voffset) );

    if( bin_driver->y_positive == 0 ) {
        v_start = bin_device->y_start - g_max_line_height - reg_off;
    } else {
        v_start = bin_device->y_start + reg_off;
    }
    return( v_start );
}

/***************************************************************************/
/*  calc banner bottom position                                            */
/***************************************************************************/

extern  uint32_t    ban_bot_pos( banner_lay_tag * ban )
{
    uint32_t    vpos;
    uint32_t    ban_depth;

    ban_depth = conv_vert_unit( &ban->depth );

    if( bin_driver->y_positive == 0 ) {
        vpos = bin_device->y_start - g_page_depth + ban_depth
               - conv_vert_unit( &(ban->region->voffset) )
               - wgml_fonts[ban->region->font].line_height;
    } else {
//        vpos = g_page_bottom_org - ban_depth; // TBD

        vpos = bin_device->y_start + g_page_depth - ban_depth
               + conv_vert_unit( &(ban->region->voffset) );
//               - wgml_fonts[ban->region->font].line_height;
    }
    return( vpos );
}

/***************************************************************************/
/*  substitute a variable in ban region text                               */
/***************************************************************************/

static char * subst_1var( char * pout, char * pvar, size_t len )
{
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;
    char            *   pchar;

    ProcFlags.suppress_msg = true;
    scan_err = false;

    pchar = scan_sym( pvar, &symvar_entry, &var_ind );
    ProcFlags.suppress_msg = false;
    if( !scan_err ) {
        if( symvar_entry.flags & local_var ) {  // lookup var in dict
            rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                              var_ind, &symsubval );
        } else {
            rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                              &symsubval );
        }
        if( rc == 2 ) {
            pchar = symsubval->value;
            while( *pchar ) {
                *pout++ = *pchar++;
            }
            *pout = '\0';
        }
    }
    return( pout );
}

/***************************************************************************/
/*  substitute vars in ban region                                          */
/***************************************************************************/

static void substitute_vars( char * pbuf, char * pin, size_t length )
{
    char    *   p;
    char    *   pvar;
    char    *   pout;
    int         k;
    int         len;

    p = pin;
    pout = pbuf;
    for( len = 0; len < length; ++len ) {
        if( *p != '&' ) {
            *pout++ = *p++;
        } else {
            pvar = p + 1;
            for( k = len; k < length; ++k ) {
                if( *p == '.' ) {
                    break;          // var end found
                } else {
                    p++;
                }
            }
            if( *p == '.' ) {
                *p = '\0';
                pout = subst_1var( pout, pvar, p - pvar );
                *p++ = '.';         // restore .
            } else {
                p = pvar - 1;       // no variable treat as text
                *pout++ = *p++;
                continue;
            }
        }
    }
    return;
}


/***************************************************************************/
/*  prepare 1 or more text_chars with region content                       */
/***************************************************************************/

static void content_reg( banner_lay_tag * ban )
{
    text_chars  *   curr_t;
    char        *   pbuf;
    char        *   pl;
    symsub      *   symsubval;
    int             k;
    int             rc;
    char            sep;

    switch( ban->region->contents.content_type ) {
    case   string_content:
        pbuf = mem_alloc( buf_size );
        if( ban->region->script_format ) {

            /***************************************************************/
            /*  preprocess script format banner region for speed           */
            /*  if not already done                                        */
            /***************************************************************/
            if( ban->region->script_region[0].len +
                ban->region->script_region[1].len +
                ban->region->script_region[2].len == 0 ) {

                pl = ban->region->contents.string;
                sep = *pl;
                                            // isolate region parts
                for( k = 0; k < 3; ++k ) {  // left, center, right
                    pl++;
                    ban->region->script_region[k].string = pl;
                    while( *pl &&  *pl != sep ) {
                        pl++;
                    }
                    ban->region->script_region[k].len =  pl -
                        ban->region->script_region[k].string ;

                    if( ban->region->script_region[k].len == 0 ) {
                        ban->region->script_region[k].string = NULL;
                    } else {
                        *pl = '\0';     // null terminate
                    }
                }
            }

            /***************************************************************/
            /*  substitute variables and create text_chars instances       */
            /***************************************************************/
            for( k = 0; k < 3; ++k ) {

                if( ban->region->script_region[k].string != NULL ) {
                    substitute_vars( pbuf, ban->region->script_region[k].string,
                                     ban->region->script_region[k].len );
                    if( *pbuf ) {
                        curr_t = alloc_text_chars( pbuf, strlen( pbuf ),
                                                   ban->region->font );
                        curr_t->width = cop_text_width( curr_t->text,
                                                        curr_t->count,
                                                        ban->region->font );
                        reg_text[k] = curr_t;
                    }
                }
            }
        } else {    // not script format only normal string with perhaps vars
            substitute_vars( pbuf, ban->region->contents.string,
                             strlen( ban->region->contents.string ) );
            if( *pbuf ) {
                curr_t = alloc_text_chars( pbuf, strlen( pbuf ),
                                    ban->region->font );
                curr_t->width = cop_text_width( curr_t->text, curr_t->count,
                                                ban->region->font );

                if( ban->region->region_position == pos_left ) {
                    reg_text[0] = curr_t;
                } else if( ban->region->region_position == pos_center ) {
                    reg_text[1] = curr_t;
                } else if( ban->region->region_position == pos_right ) {
                    reg_text[2] = curr_t;
                } else {
                    reg_text[0] = curr_t;   // position left if unknown
                }
            }
        }

        mem_free( pbuf );
        break;

    case author_content :
        rc = find_symvar( &global_dict, "$author", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "author", 6, ban->region->font );
        }
        break;
    case bothead_content :
        rc = find_symvar( &global_dict, "$bothead", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "bothead", 7, ban->region->font );
        }
        break;
    case date_content :
        rc = find_symvar( &global_dict, "$date", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "date", 4, ban->region->font );
        }
        break;
    case docnum_content :
        rc = find_symvar( &global_dict, "$docnum", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "docnum", 6, ban->region->font );
        }
        break;
    case head0_content :
        rc = find_symvar( &global_dict, "$head0", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head0", 5, ban->region->font );
        }
        break;
    case head1_content :
        rc = find_symvar( &global_dict, "$head1", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head1", 5, ban->region->font );
        }
        break;
    case head2_content :
        rc = find_symvar( &global_dict, "$head2", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head2", 5, ban->region->font );
        }
        break;
    case head3_content :
        rc = find_symvar( &global_dict, "$head3", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head3", 5, ban->region->font );
        }
        break;
    case head4_content :
        rc = find_symvar( &global_dict, "$head4", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head4", 5, ban->region->font );
        }
        break;
    case head5_content :
        rc = find_symvar( &global_dict, "$head5", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head5", 5, ban->region->font );
        }
        break;
    case head6_content :
        rc = find_symvar( &global_dict, "$head6", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "head6", 5, ban->region->font );
        }
        break;
    case headnum0_content :
        rc = find_symvar( &global_dict, "$hnum0", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum0", 5, ban->region->font );
        }
        break;
    case headnum1_content :
        rc = find_symvar( &global_dict, "$hnum1", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum1", 5, ban->region->font );
        }
        break;
    case headnum2_content :
        rc = find_symvar( &global_dict, "$hnum2", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum2", 5, ban->region->font );
        }
        break;
    case headnum3_content :
        rc = find_symvar( &global_dict, "$hnum3", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum3", 5, ban->region->font );
        }
        break;
    case headnum4_content :
        rc = find_symvar( &global_dict, "$hnum4", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum4", 5, ban->region->font );
        }
        break;
    case headnum5_content :
        rc = find_symvar( &global_dict, "$hnum5", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum5", 5, ban->region->font );
        }
        break;
    case headnum6_content :
        rc = find_symvar( &global_dict, "$hnum6", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "hnum6", 5, ban->region->font );
        }
        break;
    case headtext0_content :
        rc = find_symvar( &global_dict, "$htext0", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext0", 5, ban->region->font );
        }
        break;
    case headtext1_content :
        rc = find_symvar( &global_dict, "$htext1", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext1", 5, ban->region->font );
        }
        break;
    case headtext2_content :
        rc = find_symvar( &global_dict, "$htext2", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext2", 5, ban->region->font );
        }
        break;
    case headtext3_content :
        rc = find_symvar( &global_dict, "$htext3", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext3", 5, ban->region->font );
        }
        break;
    case headtext4_content :
        rc = find_symvar( &global_dict, "$htext4", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext4", 5, ban->region->font );
        }
        break;
    case headtext5_content :
        rc = find_symvar( &global_dict, "$htext5", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext5", 5, ban->region->font );
        }
        break;
    case headtext6_content :
        rc = find_symvar( &global_dict, "$htext6", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "htext6", 5, ban->region->font );
        }
        break;
    case pgnuma_content :
        rc = find_symvar( &global_dict, "$pgnuma", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnuma", 6, ban->region->font );
        }
        break;
    case pgnumad_content :
        rc = find_symvar( &global_dict, "$pgnumad", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnumad", 7, ban->region->font );
        }
        break;
    case pgnumc_content :
        rc = find_symvar( &global_dict, "$pgnumc", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnumc", 6, ban->region->font );
        }
        break;
    case pgnumcd_content :
        rc = find_symvar( &global_dict, "$pgnumcd", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnumcd", 7, ban->region->font );
        }
        break;
    case pgnumr_content :
        rc = find_symvar( &global_dict, "$pgnumr", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnumr", 6, ban->region->font );
        }
        break;
    case pgnumrd_content :
        rc = find_symvar( &global_dict, "$pgnumrd", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "pgnumrd", 7, ban->region->font );
        }
        break;
    case sec_content :
        rc = find_symvar( &global_dict, "$sec", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "sec", 3, ban->region->font );
        }
        break;
    case stitle_content :
        rc = find_symvar( &global_dict, "$stitle", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "stitle", 6, ban->region->font );
        }
        break;
    case title_content :
        rc = find_symvar( &global_dict, "$title", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "title", 6, ban->region->font );
        }
        break;
    case time_content :
        rc = find_symvar( &global_dict, "$time", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "time", 4, ban->region->font );
        }
        break;
    case tophead_content :
        rc = find_symvar( &global_dict, "$tophead", no_subscript, &symsubval );
        if( rc == 2 ) {
            curr_t = alloc_text_chars( symsubval->value, strlen( symsubval->value ),
                                       ban->region->font );
        } else {
            curr_t = alloc_text_chars( "tophead", 7, ban->region->font );
        }
        break;
    default:
        // the other possible banner region values are TBD

        curr_t = alloc_text_chars( "Dummy region", 12, ban->region->font );
        break;
    }
    if( ban->region->contents.content_type != string_content ) {
        if( ban->region->region_position == pos_left ) {
            reg_text[0] = curr_t;
        } else if( ban->region->region_position == pos_center ) {
            reg_text[1] = curr_t;
        } else if( ban->region->region_position == pos_right ) {
            reg_text[2] = curr_t;
        } else {
            reg_text[0] = curr_t;  // position left if invalid
        }
    }
    return;
}

/***************************************************************************/
/*  output top / bottom banner      incomplete    TBD                      */
/*  only the first banregion is output                                     */
/***************************************************************************/
void    out_ban_common( banner_lay_tag * ban, bool bottom )
{
    text_chars      *   curr_t;
    uint32_t            ban_left;
    uint32_t            h_left;
    uint32_t            ban_right;
    uint32_t            h_right;
    uint32_t            reg_indent;
    uint32_t            curr_x;
    int                 k;

    if( ban == NULL ) {
        return;
    }

    reg_text[0] = NULL;
    reg_text[1] = NULL;
    reg_text[2] = NULL;

    b_line.first = NULL;

    if( bottom ) {
        b_line.y_address = ban_bot_pos( ban );
    } else {
        b_line.y_address = ban_top_pos( ban );
    }

    /* calc banner horizontal margins */
    ban_left  = g_page_left + conv_hor_unit( &(ban->left_adjust) );
    ban_right = g_page_right - conv_hor_unit( &(ban->right_adjust) );

    content_reg( ban );
    curr_x = 0;

    for( k = 0; k < 3; ++k ) {          // for all region parts
        if( reg_text[k] == NULL ) {
            continue;                   // skip empty part
        }
        if( b_line.first == NULL ) {
            b_line.first = reg_text[k];
        } else {
            curr_t = b_line.first;
            while( curr_t->next != NULL ) {
                curr_t = curr_t->next;
            }
            curr_t->next = reg_text[k];
        }
        curr_t = reg_text[k];

        h_left  = ban_left;
        h_right = ban_right;
        reg_indent = conv_hor_unit( &(ban->region->indent) );
        if( ban->region->hoffset.su_u >= SU_lay_left  ) {   // symbolic
            if( ban->region->hoffset.su_u == SU_lay_left ) {
                h_left += reg_indent;
            } else if( ban->region->hoffset.su_u == SU_lay_right ) {
                h_right -= reg_indent;
            } else if( ban->region->hoffset.su_u == SU_lay_centre ) {
                h_left += reg_indent;
            }
        } else {                            // in horiz space units
            h_left = reg_indent + conv_hor_unit( &(ban->region->hoffset) );
        }

        if( ban->region->region_position == pos_center || k == 1) {
            if( h_left + curr_t->width < h_right ) {
                h_left += (h_right - h_left - curr_t->width) / 2;
                curr_x = h_left;
            }
        } else if( ban->region->region_position == pos_right || k == 2) {
            h_left = h_right - curr_t->width;
            curr_x = h_left;
        }
        if( curr_x == 0 ) {
            curr_x = h_left;
        }
        curr_t->x_address = curr_x;   // not checked for overlap of parts TBD
        curr_x += curr_t->width;      // not checked for overlap of parts TBD

    }
    if( GlobalFlags.lastpass && b_line.first != NULL) {
        fb_output_textline( &b_line );
    }

    if( b_line.first != NULL) {
        add_text_chars_to_pool( &b_line );
        b_line.first = NULL;
    }
    if( bin_driver->y_positive == 0 ) {
        g_cur_v_start -= g_max_line_height;
    } else {
        g_cur_v_start += g_max_line_height;
    }
}

/***************************************************************************/
/*  output top or bottom  banner                                           */
/*  only the first banregion is output      TBD                            */
/***************************************************************************/
void    out_ban_top( banner_lay_tag * ban )
{
    out_ban_common( ban, false );       // false for top banner
}

void    out_ban_bot( banner_lay_tag * ban )
{
    out_ban_common( ban, true );        // true for bottom banner
}

