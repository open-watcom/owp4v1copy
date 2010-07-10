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
* Description:  WGML tags :SF and :eSF processing
*
****************************************************************************/
#include    "wgml.h"
//#include    "findfile.h"
#include    "gvars.h"

/***************************************************************************/
/*  allow 20 active hilighting levels                                      */
/***************************************************************************/

static  int sf[20] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
#define SFMAXIND  (int)(sizeof( sf ) / sizeof( sf[0] ) - 1)


/***************************************************************************/
/*  :SF  processing                                                        */
/***************************************************************************/

void    gml_sf( const gmltag * entry )
{
    char    *   p;
    char    *   pe;
    char        tagn[TAG_NAME_LENGTH + 1];
    long        font;

    if( sfcount >= SFMAXIND ) {
        strcpy_s( tagn, sizeof( tagn ), entry->tagname );
        g_err_tag( tagn );              // all levels active
        return;
    }
    p = scan_start;
    p++;
    while( *p == ' ' ) {
        p++;
    }
    font = strtol( p, &pe, 10 );

    if( font >= wgml_font_cnt ) {       // invalid font use default
        font = 0;
    }
    sfcount++;
    sf[sfcount] = font;
    g_curr_font_num = font;
    scan_err = false;
    p = pe;
    if( *p == '.' ) p++;                // over '.'
    if( *p ) {
        process_text( p, g_curr_font_num );
    }
    scan_start = scan_stop + 1;
    return;
}

/***************************************************************************/
/*  :esf tag                                                               */
/***************************************************************************/

void    gml_esf( const gmltag * entry )
{
    char    *   p;

    if( sfcount < 0 ) {
        g_err_tag( " no :esf" );
    } else {
        if( sfcount > 0 ) {
           g_curr_font_num = sf[sfcount - 1];
        } else {
           g_curr_font_num = layout_work.defaults.font;
        }
        sfcount--;
        scan_err = false;
        p = scan_start;
        if( *p == '.' ) p++;                // over '.'
        if( *p ) {
            process_text( p, g_curr_font_num );
        }
    }
    scan_start = scan_stop + 1;
    return;
}
