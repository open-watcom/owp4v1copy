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
* Description: WGML implement :CONVERT LAYOUT tag
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/* Layout attributes as character strings                                                                         */
/***************************************************************************/

#define pick( name, funci, funco, result ) { #name },

const   char    att_names[e_dummy_max + 1][18] = {
//   18 is enough for longest attribute name  ( extract_threshold )
     { "DUMMY"},                        // enum zero not used
#include "glayutil.h"
};


/***************************************************************************/
/*                                                                         */
/*  Format: :CONVERT file='file name'.                                     */
/*                                                                         */
/*  alternate, undocumented but used format:                               */
/*                                                                         */
/*          :CONVERT.FILE.EXT                                              */
/*                                                                         */
/* Convert the current layout into the specified file name. The resulting  */
/* file will contain the entire layout in a readable form.                 */
/***************************************************************************/

/***************************************************************************/
/*   :PAGE   output  page attribute values                                 */
/***************************************************************************/
static  void    put_lay_page( FILE * layfile, layout_data * lay )
{
    lay_att         curr;
    int             k;
    su          *   units;

    fprintf_s( layfile, ":PAGE\n" );

    for( k = 0, curr = page_att[k]; curr > 0; k++, curr = page_att[k] ) {

        switch( curr ) {
        case   e_top_margin:
            units = &(lay->page.top_margin);
            break;
        case   e_left_margin:
            units = &(lay->page.left_margin);
            break;
        case   e_right_margin:
            units = &(lay->page.right_margin);
            break;
        case   e_depth:
            units = &(lay->page.depth);
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
        o_space_unit( layfile, curr, units );
    }
}

/***************************************************************************/
/*   :DEFAULT   output default attribute values                            */
/***************************************************************************/
static  void    put_lay_default( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":DEFAULT\n" );

    for( k = 0, curr = default_att[k]; curr > 0; k++, curr = default_att[k] ) {

        switch( curr ) {
        case   e_spacing:
            o_int8( layfile, curr, &lay->defaults.spacing );
            break;
        case   e_columns:
            o_int8( layfile, curr, &lay->defaults.columns );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->defaults.font );
            break;
        case   e_justify:
            o_yes_no( layfile, curr, &lay->defaults.justify );
            break;
        case   e_input_esc:
            o_char( layfile, curr, &lay->defaults.input_esc );
            break;
        case   e_gutter:
            o_space_unit( layfile, curr, &lay->defaults.gutter );
            break;
        case   e_binding:
            o_space_unit( layfile, curr, &lay->defaults.binding );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :WIDOW    output widow attribute value                                */
/***************************************************************************/
static  void    put_lay_widow( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":WIDOW\n" );

    for( k = 0, curr = widow_att[k]; curr > 0; k++, curr = widow_att[k] ) {

        switch( curr ) {
        case   e_threshold:
            o_int8( layfile, curr, &lay->widow.threshold );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :FN        output footnote attribute values                            */
/***************************************************************************/
static  void    put_lay_fn( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FN\n" );

    for( k = 0, curr = fn_att[k]; curr > 0; k++, curr = fn_att[k] ) {

        switch( curr ) {
        case   e_line_indent:
            o_space_unit( layfile, curr, &lay->fn.line_indent );
            break;
        case   e_align:
            o_space_unit( layfile, curr, &lay->fn.align );
            break;
        case   e_pre_lines:
            o_space_unit( layfile, curr, &lay->fn.pre_lines );
            break;
        case   e_skip:
            o_space_unit( layfile, curr, &lay->fn.skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->fn.spacing );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->fn.font );
            break;
        case   e_number_font:
            o_int8( layfile, curr, &lay->fn.number_font );
            break;
        case   e_number_style:
            o_number_style( layfile, curr, &lay->fn.number_style );
            break;
        case   e_frame:
            o_frame( layfile, curr, &lay->fn.frame );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :FNREF     output footnote attribute values                            */
/***************************************************************************/
static  void    put_lay_fnref( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FNREF\n" );

    for( k = 0, curr = fnref_att[k]; curr > 0; k++, curr = fnref_att[k] ) {

        switch( curr ) {
        case   e_font:
            o_int8( layfile, curr, &lay->fnref.font );
            break;
        case   e_number_style:
            o_number_style( layfile, curr, &lay->fnref.number_style );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :P         output paragraph attribute values                          */
/***************************************************************************/
static  void    put_lay_p( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":P\n" );

    for( k = 0, curr = p_att[k]; curr > 0; k++, curr = p_att[k] ) {

        switch( curr ) {
        case   e_line_indent:
            o_space_unit( layfile, curr, &lay->p.line_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->p.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->p.post_skip );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :PC        output paragraph continue attribute values                 */
/***************************************************************************/
static  void    put_lay_pc( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":PC\n" );

    for( k = 0, curr = pc_att[k]; curr > 0; k++, curr = pc_att[k] ) {

        switch( curr ) {
        case   e_line_indent:
            o_space_unit( layfile, curr, &lay->pc.line_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->pc.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->pc.post_skip );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :FIG       output figur attribute values                              */
/***************************************************************************/
static  void    put_lay_fig( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FIG\n" );

    for( k = 0, curr = fig_att[k]; curr > 0; k++, curr = fig_att[k] ) {

        switch( curr ) {
        case   e_left_adjust:
            o_space_unit( layfile, curr, &lay->fig.left_adjust );
            break;
        case   e_right_adjust:
            o_space_unit( layfile, curr, &lay->fig.right_adjust );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->fig.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->fig.post_skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->fig.spacing );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->fig.font );
            break;
        case   e_default_place:
            o_default_place( layfile, curr, &lay->fig.default_place );
            break;
        case   e_default_frame:
            o_default_frame( layfile, curr, &lay->fig.default_frame );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}



/***************************************************************************/
/*   output layout data to file                                            */
/***************************************************************************/
static  void    put_layout( char * name, layout_data * lay )
{
    static  FILE    *   layfile;
    symsub          *   sversion;

    fopen_s( &layfile, name, "uwb" );
    if( layfile == NULL ) {
        out_msg( "open error %s\n", name );
        return;
    }

    fprintf_s( layfile, ":LAYOUT\n" );
    if( GlobalFlags.research ) {
        find_symvar( &global_dict, "$version", no_subscript, &sversion );
        fprintf_s( layfile, ":cmt. Created with %s\n", sversion->value );
    }

    put_lay_page( layfile, lay );
    put_lay_default( layfile, lay );
    put_lay_widow( layfile, lay );
    put_lay_fn( layfile, lay );
    put_lay_fnref( layfile, lay );
    put_lay_p( layfile, lay );
    put_lay_pc( layfile, lay );
    put_lay_fig( layfile, lay );

    fprintf_s( layfile, ":eLAYOUT\n" );

    fclose( layfile );
}


/***************************************************************************/
/*  lay_convert   process :CONVERT tag                                     */
/***************************************************************************/

void    lay_convert( const gmltag * entry )
{
    char        *   p;

    p = scan_start;
    scan_start = scan_stop + 1;

    if( !GlobalFlags.firstpass ) {
        ProcFlags.layout = true;
        return;                         // process during first pass only
    }

    while( *p == ' ' ) {
        p++;
    }
    *token_buf = '\0';
    if( !strnicmp( "file=", p, 5 ) ) {  // file attribute?
        char    quote;
        char    *fnstart;

        p += 5;
        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = '.';                // error?? filename without quotes
        }
        fnstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';
        strcpy_s( token_buf, buf_size, fnstart );
    } else {                            // try undocumented format
        if( *p && *p == '.' ) {
            strcpy_s( token_buf, buf_size, p + 1 );
        }
    }
    if( *token_buf == '\0' ) {           // file name missing
        err_count++;
        g_err( err_att_missing );
        show_include_stack();
    } else {
        put_layout( token_buf, &layout_work );
    }
    return;
}
