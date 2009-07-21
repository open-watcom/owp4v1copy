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
* Description: WGML implement :FIG tag for LAYOUT processing
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <errno.h>
#include <stdarg.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*   :FIG   attributes                                                     */
/***************************************************************************/
const   lay_att     fig_att[9] =
    { e_left_adjust, e_right_adjust, e_pre_skip, e_post_skip, e_spacing,
      e_font, e_default_place, e_default_frame, e_dummy_zero };



/***************************************************************************/
/*  lay_fig                                                                */
/***************************************************************************/

void    lay_fig( const gmltag * entry )
{
    char        *   p;
    condcode        cc;
    int             k;
    lay_att         curr;
    att_args        l_args;
    bool            cvterr;

    p = scan_start;

    if( !GlobalFlags.firstpass ) {
        scan_start = scan_stop + 1;
        eat_lay_sub_tag();
        return;                         // process during first pass only
    }
    if( ProcFlags.lay_xxx != el_fig ) {
        ProcFlags.lay_xxx = el_fig;
        out_msg( ":FIG nearly dummy\n" );
    }
    cc = get_lay_sub_and_value( &l_args );  // get one with value
    if( cc != pos ) {
        scan_start = scan_stop + 1;
        return;
    }
    cvterr = true;
    for( k = 0, curr = fig_att[k]; curr > 0; k++, curr = fig_att[k] ) {

        if( !strnicmp( att_names[curr], l_args.start[0], l_args.len[0] ) ) {
            p = l_args.start[1];

            switch( curr ) {
            case   e_left_adjust:
                cvterr = i_space_unit( p, curr, &layout_work.fig.left_adjust );
                break;
            case   e_right_adjust:
                cvterr = i_space_unit( p, curr, &layout_work.fig.right_adjust );
                break;
            case   e_pre_skip:
                cvterr = i_space_unit( p, curr, &layout_work.fig.pre_skip );
                break;
            case   e_post_skip:
                cvterr = i_space_unit( p, curr, &layout_work.fig.post_skip );
                break;
            case   e_spacing:
                cvterr = i_int8( p, curr, &layout_work.fig.spacing );
                break;
            case   e_font:
                cvterr = i_int8( p, curr, &layout_work.fig.font );
                break;
            case   e_default_place:
                cvterr = i_default_place( p, curr, &layout_work.fig.default_place );
                break;
            case   e_default_frame:
                cvterr = i_default_frame( p, curr, &layout_work.fig.default_frame );
                break;
            default:
                out_msg( "WGML logic error.\n");
                break;
            }
            break;                  // break out of for loop
        }
    }
    if( cvterr ) {                  // there was an error
        err_count++;
        g_err( err_att_val_inv );
        show_include_stack();
    }
    scan_start = scan_stop + 1;
    return;
}

