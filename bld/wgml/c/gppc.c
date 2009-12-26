/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML tags :P and :PC processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"





/***************************************************************************/
/*  :P.perhaps paragraph elements                                          */
/***************************************************************************/
static  void    proc_p_pc( p_lay_tag * p_pc )
{
    char        *   p;

    scan_err = false;
    p = scan_start;

    prepare_doc_sect( ProcFlags.doc_sect ); // if not already done

    scr_process_break();

    g_cur_h_start = g_page_left + g_indent + conv_hor_unit( &(p_pc->line_indent) );

    if( ProcFlags.page_started ) {      // TBD
        if( bin_driver->y_positive == 0x00 ) {  // TBD
            g_cur_v_start -= conv_vert_unit( &(p_pc->pre_skip), 0 );   // TBD
            if( post_skip != NULL ) {
                g_cur_v_start -= conv_vert_unit( post_skip, 0 );
            }
        } else {
            g_cur_v_start += conv_vert_unit( &(p_pc->pre_skip), 0 );   // TBD
            if( post_skip != NULL ) {
                g_cur_v_start += conv_vert_unit( post_skip, 0 );
            }
        }
    } else {
        if( bin_driver->y_positive == 0x00 ) {  // TBD
            if( post_skip != NULL ) {
                g_cur_v_start -= conv_vert_unit( post_skip, 0 );
            }
        } else {
            if( post_skip != NULL ) {
                g_cur_v_start += conv_vert_unit( post_skip, 0 );
            }
        }
    }
    post_skip = NULL;

//  ProcFlags.just_override = false;// perhaps no justify for first para line
//  ProcFlags.para_line1 = true;        // remember first paragraph line TBD

    post_space = 0;
    post_space_save = 0;
    p_char = NULL;

    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, g_curr_font_num );
    }

    post_skip = &(p_pc->post_skip);

    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*  :P.perhaps paragraph elements                                          */
/***************************************************************************/
extern  void    gml_p( const gmltag * entry )
{
    p_lay_tag   *   p_pc;

    p_pc = &layout_work.p;
    proc_p_pc( p_pc );
    return;
}

/***************************************************************************/
/*  :PC.perhaps paragraph elements                                         */
/***************************************************************************/
extern  void    gml_pc( const gmltag * entry )
{
    p_lay_tag   *   p_pc;

    p_pc = &layout_work.pc;
    proc_p_pc( p_pc );
    return;
}

