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
* Description:  WGML tags :HDREF :FIGREF :FNREF processing
*
*                         :FNREF not yet implemented
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"

static  bool    ref_page        = false;
static  bool    page_found      = false;
static  bool    refid_found     = false;
static  char    refid[ID_LEN];

/***************************************************************************/
/* Get attribute values for FIGREF, FNREF, and HDREF                       */
/***************************************************************************/

static void get_ref_attributes( void )
{ 
    char        *   p;
    int             k;

    scan_err = false;
    p = scan_start;
    if( *p == '.' ) p++;                // possible tag end
    while( *p == ' ' ) p++;             // skip initial spaces

    for( ;; ) {
        while( *p == ' ' ) {            // over WS to attribute
            p++;
        }
        if( *p == '\0' ) {              // end of line: get new line
            if( !(input_cbs->fmflags & II_eof) ) {
                if( get_line( true ) ) {// next line for missing attribute
 
                    process_line();
                    scan_start = buff2;
                    scan_stop  = buff2 + buff2_lg;
                    if( (*scan_start == SCR_char) ||    // cw found: end-of-tag
                        (*scan_start == GML_char) ) {   // tag found: end-of-tag
                        ProcFlags.tag_end_found = true; 
                        break;          
                    } else {
                        p = scan_start; // new line is part of current tag
                        continue;
                    }
                }
            }
        }
        if( !strnicmp( "page", p, 4 ) ) {
            page_found = true;
            p += 4;
            p = get_att_value( p );
            if( val_start == NULL ) {
                break;
            }
            if( !strnicmp( "yes", val_start, 3 ) ) {
                page = true;
            } else if( !strnicmp( "no", val_start, 2 ) ) {
                page = false;
            } else {
                xx_line_err( err_inv_att_val, val_start );
                scan_start = scan_stop + 1;
                return;
            }
            if( ProcFlags.tag_end_found ) {
                break;
            }
        } else if( !strnicmp( "refid", p, 5 ) ) {
            p += 5;

            p = get_refid_value( p );
            if( val_start == NULL ) {
                break;
            }

            refid_found = true;
            for( k = 0; k < val_len; k++ ) {
                (refid)[k] = tolower( *(val_start + k) );
            }
            if( ProcFlags.tag_end_found ) {
                break;
            }
        } else {    // no match = end-of-tag in wgml 4.0
            ProcFlags.tag_end_found = true;
            break;
        }
    }
    if( !refid_found ) {            // detect missing required attribute
        xx_err( err_att_missing );
        scan_start = scan_stop + 1;
        return;
    }
}

/***************************************************************************/
/*      :FIGREF refid=’id-name’                                            */
/*              [page=yes                                                  */
/*                    no].                                                 */
/* This tag causes a figure reference to be generated. The text "Figure"   */
/* followed by the figure number will be generated at the point where the  */
/* :figref tag is specified. The figure reference tag is a paragraph       */
/* element, and is used with text to create the content of a basic         */
/* document element. The figure being referenced must have a figure        */
/* caption specified.                                                      */
/***************************************************************************/

void gml_figref( const gmltag * entry )
{ 
    ref_entry   *   cur_re;  

    get_ref_attributes();

    cur_re = find_refid( fig_ref_dict, refid );
/// now format output!
/// concat is on!

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*         :HDREF refid='id-name'                                          */
/*                [page=yes                                                */
/*                      no].                                               */
/*                                                                         */
/* This tag causes a heading reference to be generated.  The heading       */
/* reference tag is a paragraph element, and is used with text to create   */
/* the content of a basic document element.  The heading text from the     */
/* referenced heading is enclosed in double quotation marks and inserted   */
/* into the formatted document.                                            */
/***************************************************************************/

void gml_hdref( const gmltag * entry )
{
/// idp needs to be created below, may not need to be set to NULL///
    char    *   idp     = NULL; //***what value should it have?***//
    char        buf64[64];
    ref_entry   *   cur_re;
    static char undefid[]   = "\"Undefined Heading\" on page XXX";

    get_ref_attributes();

    if( refid ) {                      // id attribute was specified
        bool concatsave = ProcFlags.concat;

        ProcFlags.concat = true;        // make process_text add to line
        cur_re = find_refid( hx_ref_dict, refid );
        if( cur_re == NULL ) {              // undefined refid
            process_text( undefid, g_curr_font );
        } else {
            process_text( idp, g_curr_font );
            if( ref_page || (!page_found && (page != cur_re->pageno)) ) {
                sprintf_s( buf64, sizeof( buf64 ), "on page %d", cur_re->pageno );
                process_text( buf64, g_curr_font );
            }
            mem_free( idp );
        }
        ProcFlags.concat = concatsave;
    } else {
        g_err( err_att_missing );       // id attribute missing
        file_mac_info();
        err_count++;
    }

    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*      :FIGREF refid=’id-name’                                            */
/* This tag causes a footnote reference to be generated. The number of the */
/* referenced footnote will be generated at the point where the :fnref tag */
/* is specified. The footnote reference tag is a paragraph element, and is */
/* used with text to create the content of a basic document element.       */
/***************************************************************************/

void gml_fnref( const gmltag * entry )
{ 
    ref_entry   *   cur_re;  

    get_ref_attributes();

    cur_re = find_refid( fn_ref_dict, refid );
/// now format output!
/// concat is on!

    scan_start = scan_stop + 1;
    return;
}



