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
* Description:  WGML tag :GRAPHIC processing
*
****************************************************************************/
#include    "wgml.h"
//#include    "findfile.h"
#include    "gvars.h"


/***************************************************************************/
/*  :GRAPHIC tag                                                           */
/***************************************************************************/

void    gml_graphic( const gmltag * entry )
{
//    char        *   p;
//    text_line       p_line;
//    int8_t          t_spacing;
//    int8_t          font_save;

    if( (ProcFlags.doc_sect < doc_sect_gdoc) ) {
        if( (ProcFlags.doc_sect_nxt < doc_sect_gdoc) ) {
            xx_tag_err( err_tag_before_gdoc, entry->tagname );
        }
    }
//    p = scan_start;
//    if( *p && *p != '.' ) p++;

//    while( *p == ' ' ) {                // over WS to attribute
//        p++;
//    }
//    if( *p &&
//        ! (strnicmp( "stitle ", p, 7 ) &&   // look for stitle
//           strnicmp( "stitle=", p, 7 )) ) {
//        char        quote;
//        char    *   valstart;

//        p += 6;
//        while( *p == ' ' ) {
//            p++;
//        }
//        if( *p == '=' ) {
//            p++;
//            while( *p == ' ' ) {
//                p++;
//            }
//        }
//        if( *p == '"' || *p == '\'' ) {
//            quote = *p;
//            ++p;
//        } else {
//            quote = ' ';
//        }
//        valstart = p;
//        while( *p && *p != quote ) {
//            ++p;
//        }
//        *p = '\0';
//        if( !ProcFlags.title_tag_seen ) {// first stitle goes into dictionary
//            add_symvar( &global_dict, "$stitle", valstart, no_subscript, 0 );
//        }
//        p++;
//    } else {
//        if( !ProcFlags.title_tag_seen ) {
//            add_symvar( &global_dict, "$stitle", "", no_subscript, 0 );// set nullstring
//        }
//    }

//    if( *p == '.' ) p++;                // over '.'
//    if( !ProcFlags.title_tag_seen ) {
//        if( *p ) {                      // first title goes into dictionary
//            add_symvar( &global_dict, "$title", p, no_subscript, 0 );
//        }
//    }

    start_doc_sect();                   // if not already done

//    p_line.first = NULL;
//    p_line.next  = NULL;
//    p_line.last  = NULL;

//    t_spacing = layout_work.titlep.spacing;

//    font_save = g_curr_font_num;
//    g_curr_font_num = layout_work.title.font;

//    p_line.line_height = wgml_fonts[g_curr_font_num].line_height;

//    calc_title_pos( g_curr_font_num, t_spacing, !ProcFlags.title_tag_seen );
//    p_line.y_address = g_cur_v_start;

//    prep_title_line( &p_line, p );

//    ProcFlags.page_started = true;
//    process_line_full( &p_line, false );
//    g_curr_font_num = font_save;

//    if( p_line.first != NULL) {
//        add_text_chars_to_pool( &p_line );
//    }
//    ProcFlags.page_started = true;

//    ProcFlags.title_tag_seen = true;
    scan_start = scan_stop + 1;
//out_msg( "gml_graphic() invoked!\n");
}

