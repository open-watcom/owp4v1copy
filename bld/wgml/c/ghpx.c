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
* Description:  WGML tags :HP0 :HP1, :HP2, :HP3 and :eHPx processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"

/***************************************************************************/
/*  allow 20 active hilighting levels                                      */
/***************************************************************************/

static  int hp[20] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
#define HPMAXIND  (int)(sizeof( hp ) / sizeof( hp[0] ) - 1)


/***************************************************************************/
/*  :HPx common processing                                                 */
/***************************************************************************/

static bool gml_hpx_common( const gmltag * entry, int font )
{
    char    *   p;
    bool        result = true;
    char        tagn[TAG_NAME_LENGTH + 1];

    if( hpcount >= HPMAXIND ) {
        strcpy_s( tagn, sizeof( tagn ), entry->tagname );
        g_err_tag( tagn );              // all levels active
        result = false;
        return( result );
    }
    if( font >= wgml_font_cnt ) {       // invalid font use default
        font = 0;
    }
#if 0
    if( (hpcount > -1) && (hp[hpcount] == font) ) {
        g_err_tag( " no :HPx" );        // same hilighting already active
        result = false;
        return( result );
    }
#endif
    hpcount++;
    hp[hpcount] = font;
    g_curr_font_num = font;
    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, font);
    }
    scan_start = scan_stop + 1;
    return( result );
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

    if( hpcount < 0 ) {
        g_err_tag( " no :eHPx" );
    } else {
        if( hpcount > 0 ) {
           g_curr_font_num = hp[hpcount - 1];
        } else {
           g_curr_font_num = layout_work.defaults.font;
        }
        hpcount--;
        scan_err = false;
        p = scan_start;
        if( *p == '.' ) p++;                // over '.'
        if( *p ) {
            process_text( p, g_curr_font_num );
        }
    }
    scan_start = scan_stop + 1;
}
