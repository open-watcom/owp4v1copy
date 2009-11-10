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
* Description:  WGML tags :TITLE processing
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"


/***************************************************************************/
/*  :TITLE tag                                                             */
/***************************************************************************/

void    gml_title( const gmltag * entry )
{
    char    *   p;

    if( ProcFlags.doc_sect != doc_sect_titlep ) {
        g_err( err_tag_wrong_sect, entry->tagname, ":TITLEP section" );
        err_count++;
        show_include_stack();
    }
    p = scan_start;
    if( *p && *p != '.' ) p++;

    while( *p == ' ' ) {                // over WS to attribute
        p++;
    }
    if( *p &&
        ! (strnicmp( "stitle ", p, 7 ) &&   // look for stitle
           strnicmp( "stitle=", p, 7 )) ) {
        char        quote;
        char    *   valstart;

        p += 6;
        while( *p == ' ' ) {
            p++;
        }
        if( *p == '=' ) {
            p++;
            while( *p == ' ' ) {
                p++;
            }
        }
        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = ' ';
        }
        valstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';
        if( !ProcFlags.title_tag_seen ) {
            add_symvar( &global_dict, "$stitle", valstart, no_subscript, 0 );
        }
        p++;
    } else {
        if( !ProcFlags.title_tag_seen ) {
            add_symvar( &global_dict, "$stitle", "", no_subscript, 0 );// set nullstring
        }
    }

    if( *p == '.' ) p++;                // over '.'
    if( !ProcFlags.title_tag_seen ) {
        if( *p ) {
            add_symvar( &global_dict, "$title", p, no_subscript, 0 );
        }
    }
    // process more than one title line TBD
    ProcFlags.title_tag_seen = true;
    scan_start = scan_stop + 1;
}
