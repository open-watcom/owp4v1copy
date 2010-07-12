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
* Description:  WGML tags :HP0 :HP1, :HP2, :HP3, :eHPx
*                         :SF  and :eSF processing
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"


/***************************************************************************/
/*  :HPx common processing                                                 */
/***************************************************************************/

static void gml_hpx_common( const gmltag * entry, int level )
{
    char    *   p;
    char        tagn[TAG_NAME_LENGTH + 1];

    if( hilcount >= HILMAXIND ) {
        strcpy_s( tagn, sizeof( tagn ), entry->tagname );
        g_err_tag( tagn );              // all levels active
        return;
    }

    hilcount++;
    hil[hilcount].tag = level;
    if( level >= wgml_font_cnt ) {      // invalid font use default
        level = 0;
    }
    hil[hilcount].font = g_curr_font_num;   // save current font
    g_curr_font_num = level;
    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, g_curr_font_num );
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  :HP0  :HP1  :HP2  :HP3                                                 */
/***************************************************************************/

void    gml_hp0( const gmltag * entry )
{
    gml_hpx_common( entry, 0 );
}

void    gml_hp1( const gmltag * entry )
{
    gml_hpx_common( entry, 1 );
}

void    gml_hp2( const gmltag * entry )
{
    gml_hpx_common( entry, 2 );
}

void    gml_hp3( const gmltag * entry )
{
    gml_hpx_common( entry, 3 );
}

/***************************************************************************/
/*  :eHPx tags                                                             */
/***************************************************************************/

void    gml_ehpx( const gmltag * entry )
{
    char    *   p;
    static char tagehpx[5] = { "eHPx" };

    if( hilcount < 0 ) {
        g_err_tag_no( "eHPx" );         // no eHPx expected
    } else {
        if( hil[hilcount].tag != entry->tagname[3] - '0' ) {
            if( hil[hilcount].tag < 0 ) {
                g_err_tag( "eSF" );     // :eSF expected
            } else {
                tagehpx[3] = '0' + hil[hilcount].tag;
                g_err_tag( tagehpx );   // :eHPx expected
            }
        } else {
            g_curr_font_num = hil[hilcount].font;
            hilcount--;
            scan_err = false;
            p = scan_start;
            if( *p == '.' ) p++;                // over '.'
            if( *p ) {
                process_text( p, g_curr_font_num );
            }
        }
    }
    scan_start = scan_stop + 1;
}


/***************************************************************************/
/*  :SF  processing                                                        */
/*                                                                         */
/*  Format: :SF font=number.                                               */
/*  The set font tag starts the highlighting of phrases at the level       */
/*  specified by the required attribute font. The actual highlighting      */
/*  to be performed is determined by the type of device for which the      */
/*  document is being formatted. Examples of highlighting include          */
/*  underlining, displaying in bold face, or using a different             */
/*  character shape (such as italics).                                     */
/*  The value of the font attribute is a non-negative integer number.      */
/*  If the specified number is larger than the last defined font for the   */
/*  document, font for zero is used.Highlighting may not be used when      */
/*  the GML layout explicitly determines the emphasis to be used, such     */
/*  as in the text of a heading.The set font tag is a paragraph element.   */
/*  It is used with text to create the content of a basic document element,*/
/*  such as a paragraph. A corresponding :ESF tag must be specified for    */
/*  each :SF tag.                                                          */
/***************************************************************************/

void    gml_sf( const gmltag * entry )
{
    char    *   p;
    char    *   pe;
    char        tagn[TAG_NAME_LENGTH + 1];
    long        font;

    if( hilcount >= HILMAXIND ) {       // all hilite levels active
        if( hil[hilcount].tag == -1 ) {
            g_err_tag( "eSF" );         // :eSF expected
        } else {
            strcpy_s( tagn, sizeof( tagn ), "eHPx" );
            tagn[3] = '0' +  hil[hilcount].tag;
            g_err_tag( tagn );
        }
        return;
    }
    p = scan_start;
    p++;
    while( *p == ' ' ) {
        p++;
    }
    if( !strnicmp( "font=", p, 5 ) ) {
        p += 5;
        font = strtol( p, &pe, 10 );

        if( (font < 0) || (font >= wgml_font_cnt) ) {// invalid font use default
            font = 0;
        }
        hilcount++;
        hil[hilcount].font = g_curr_font_num;
        hil[hilcount].tag = -1;         // :SF tag
        g_curr_font_num = font;
        scan_err = false;
        p = pe;
        if( *p == '.' ) p++;            // over '.'
        if( *p ) {
            process_text( p, g_curr_font_num );
        }
    } else {
        err_count++;
        // AT-001 Required attribute not found
        g_err( err_att_missing );
        file_mac_info();
    }
    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*  :esf tag processing                                                    */
/*                                                                         */
/*   Format: :eSF.                                                         */
/*                                                                         */
/*   This tag ends the highlighting of phrases started by the last :sf tag */
/***************************************************************************/

void    gml_esf( const gmltag * entry )
{
    char    *   p;

    if( hilcount < 0 ) {
        g_err_tag_no( "eSF" );          // no :eSF expected
    } else {
        if( hil[hilcount].tag != -1 ) {
            g_err_tag( "eSF" );         // :eSF expected
        } else {
            g_curr_font_num = hil[hilcount].font;
            hilcount--;
            scan_err = false;
            p = scan_start;
            if( *p == '.' ) p++;        // over '.'
            if( *p ) {
                process_text( p, g_curr_font_num );
            }
        }
    }
    scan_start = scan_stop + 1;
    return;
}
