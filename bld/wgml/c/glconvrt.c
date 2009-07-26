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
/*   :XMP       output example attribute values                            */
/***************************************************************************/
static  void    put_lay_xmp( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":XMP\n" );

    for( k = 0, curr = xmp_att[k]; curr > 0; k++, curr = xmp_att[k] ) {

        switch( curr ) {
        case   e_left_indent:
            o_space_unit( layfile, curr, &lay->xmp.left_indent );
            break;
        case   e_right_indent:
            o_space_unit( layfile, curr, &lay->xmp.right_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->xmp.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->xmp.post_skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->xmp.spacing );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->xmp.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :NOTE      output note attribute values                               */
/***************************************************************************/
static  void    put_lay_note( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":NOTE\n" );

    for( k = 0, curr = note_att[k]; curr > 0; k++, curr = note_att[k] ) {

        switch( curr ) {
        case   e_left_indent:
            o_space_unit( layfile, curr, &lay->note.left_indent );
            break;
        case   e_right_indent:
            o_space_unit( layfile, curr, &lay->note.right_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->note.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->note.post_skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->note.spacing );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->note.font );
            break;
        case   e_note_string:
            o_xx_string( layfile, curr, &lay->note.string );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :Hx        output header attribute values for :H0 - :H6               */
/***************************************************************************/
static  void    put_lay_hx( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;
    int                 lvl;

    for( lvl = 0; lvl < 7; ++lvl ) {


        fprintf_s( layfile, ":H%c\n", '0' + lvl );

        for( k = 0, curr = hx_att[k]; curr > 0; k++, curr = hx_att[k] ) {

            switch( curr ) {
            case   e_group:
                o_int8( layfile, curr, &lay->hx[lvl].group );
                break;
            case   e_indent:
                o_space_unit( layfile, curr, &lay->hx[lvl].indent );
                break;
            case   e_pre_top_skip:
                o_space_unit( layfile, curr, &lay->hx[lvl].pre_top_skip );
                break;
            case   e_pre_skip:
                o_space_unit( layfile, curr, &lay->hx[lvl].pre_skip );
                break;
            case   e_post_skip:
                o_space_unit( layfile, curr, &lay->hx[lvl].post_skip );
                break;
            case   e_spacing:
                o_int8( layfile, curr, &lay->hx[lvl].spacing );
                break;
            case   e_font:
                o_int8( layfile, curr, &lay->hx[lvl].font );
                break;
            case   e_number_font:
                o_int8( layfile, curr, &lay->hx[lvl].number_font );
                break;
            case   e_number_form:
                o_number_form( layfile, curr, &lay->hx[lvl].number_form );
                break;
            case   e_page_position:
                o_page_position( layfile, curr, &lay->hx[lvl].page_position );
                break;
            case   e_number_style:
                o_number_style( layfile, curr, &lay->hx[lvl].number_style );
                break;
            case   e_page_eject:
                o_page_eject( layfile, curr, &lay->hx[lvl].page_eject );
                break;
            case   e_line_break:
                o_yes_no( layfile, curr, &lay->hx[lvl].line_break );
                break;
            case   e_display_heading:
                o_yes_no( layfile, curr, &lay->hx[lvl].display_heading );
                break;
            case   e_number_reset:
                o_yes_no( layfile, curr, &lay->hx[lvl].number_reset );
                break;
            case   e_case:
                o_case( layfile, curr, &lay->hx[lvl].cases );
                break;
            case   e_align:
                o_space_unit( layfile, curr, &lay->hx[lvl].align );
                break;
            default:
                out_msg( "WGML logic error.\n");
                break;
            }
        }
    }
}


/***************************************************************************/
/*   :HEADING   output header attribute values                             */
/***************************************************************************/
static  void    put_lay_heading( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;


    fprintf_s( layfile, ":HEADING\n" );

    for( k = 0, curr = heading_att[k]; curr > 0; k++, curr = heading_att[k] ) {

        switch( curr ) {
        case   e_delim:
            o_char( layfile, curr, &lay->heading.delim );
            break;
        case   e_stop_eject:
            o_yes_no( layfile, curr, &lay->heading.stop_eject );
            break;
        case   e_para_indent:
            o_yes_no( layfile, curr, &lay->heading.para_indent );
            break;
        case   e_threshold:
            o_int8( layfile, curr, &lay->heading.threshold );
            break;
        case   e_max_group:
            o_int8( layfile, curr, &lay->heading.max_group );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :Ix        output index  attribute values for :I1 - :I3               */
/***************************************************************************/
static  void    put_lay_ix( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;
    int                 lvl;

    for( lvl = 0; lvl < 3; ++lvl ) {


        fprintf_s( layfile, ":I%c\n", '1' + lvl );

        for( k = 0, curr = ix_att[k]; curr > 0; k++, curr = ix_att[k] ) {

            switch( curr ) {
            case   e_pre_skip:
                o_space_unit( layfile, curr, &lay->ix[lvl].pre_skip );
                break;
            case   e_post_skip:
                o_space_unit( layfile, curr, &lay->ix[lvl].post_skip );
                break;
            case   e_skip:
                o_space_unit( layfile, curr, &lay->ix[lvl].skip );
                break;
            case   e_font:
                o_int8( layfile, curr, &lay->ix[lvl].font );
                break;
            case   e_indent:
                o_space_unit( layfile, curr, &lay->ix[lvl].indent );
                break;
            case   e_wrap_indent:
                o_space_unit( layfile, curr, &lay->ix[lvl].wrap_indent );
                break;
            case   e_index_delim:
                o_xx_string( layfile, curr, &lay->ix[lvl].index_delim );
                break;
            case   e_string_font:
                if( lvl < 2 ) {         // :I3 has no string font
                    o_int8( layfile, curr, &lay->ix[lvl].string_font );
                }
                break;
            default:
                out_msg( "WGML logic error.\n");
                break;
            }
        }
    }
}

/***************************************************************************/
/*   :LQ        output long quotation attribute values                     */
/***************************************************************************/
static  void    put_lay_lq( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":LQ\n" );

    for( k = 0, curr = lq_att[k]; curr > 0; k++, curr = lq_att[k] ) {

        switch( curr ) {
        case   e_left_indent:
            o_space_unit( layfile, curr, &lay->lq.left_indent );
            break;
        case   e_right_indent:
            o_space_unit( layfile, curr, &lay->lq.right_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->lq.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->lq.post_skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->lq.spacing );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->lq.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :xx        output for font only value                                 */
/***************************************************************************/
static  void    put_lay_xx( FILE * layfile, int8_t * font, char * name )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":%s\n", name );

    for( k = 0, curr = xx_att[k]; curr > 0; k++, curr = xx_att[k] ) {

        switch( curr ) {
        case   e_font:
            o_int8( layfile, curr, font );
            break;
        default:
            out_msg( "WGML logic error glconvrt.c.\n");
            err_count++;
            break;
        }
    }
}

static  void    put_lay_dt( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->dt.font), "DT" );
}

static  void    put_lay_gt( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->gt.font), "GT" );
}

static  void    put_lay_dthd( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->dthd.font), "DTHD" );
}

static  void    put_lay_cit( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->cit.font), "CIT" );
}

static  void    put_lay_gd( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->gd.font), "GD" );
}

static  void    put_lay_ddhd( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->ddhd.font), "DDHD" );
}

static  void    put_lay_ixpgnum( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->ixpgnum.font), "IXPGNUM" );
}

static  void    put_lay_ixmajor( FILE * layfile, layout_data * lay )
{
    put_lay_xx( layfile, &(lay->ixmajor.font), "IXMAJOR" );
}


/***************************************************************************/
/*   :FIGCAP    output figure caption attribute values                     */
/***************************************************************************/
static  void    put_lay_figcap( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FIGCAP\n" );

    for( k = 0, curr = figcap_att[k]; curr > 0; k++, curr = figcap_att[k] ) {

        switch( curr ) {
        case   e_pre_lines:
            o_space_unit( layfile, curr, &lay->figcap.pre_lines );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->figcap.font );
            break;
        case   e_figcap_string:
            o_xx_string( layfile, curr, &lay->figcap.string );
            break;
        case   e_string_font:
            o_int8( layfile, curr, &lay->figcap.string_font );
            break;
        case   e_delim:
            o_char( layfile, curr, &lay->figcap.delim );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :FIGDESC   output figure description attribute values                 */
/***************************************************************************/
static  void    put_lay_figdesc( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FIGDESC\n" );

    for( k = 0, curr = figdesc_att[k]; curr > 0; k++, curr = figdesc_att[k] ) {

        switch( curr ) {
        case   e_pre_lines:
            o_space_unit( layfile, curr, &lay->figdesc.pre_lines );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->figdesc.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   :FIGLIST   output figure list attribute values                        */
/***************************************************************************/
static  void    put_lay_figlist( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":FIGLIST\n" );

    for( k = 0, curr = figlist_att[k]; curr > 0; k++, curr = figlist_att[k] ) {

        switch( curr ) {
        case   e_left_adjust:
            o_space_unit( layfile, curr, &lay->figlist.left_adjust );
            break;
        case   e_right_adjust:
            o_space_unit( layfile, curr, &lay->figlist.right_adjust );
            break;
        case   e_skip:
            o_space_unit( layfile, curr, &lay->figlist.skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->figlist.spacing );
            break;
        case   e_columns:
            o_int8( layfile, curr, &lay->figlist.columns );
            break;
        case   e_fill_string:
            o_xx_string( layfile, curr, &lay->figlist.fill_string );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   :DD        output definition data attribute values                    */
/***************************************************************************/
static  void    put_lay_dd( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":DD\n" );

    for( k = 0, curr = dd_att[k]; curr > 0; k++, curr = dd_att[k] ) {

        switch( curr ) {
        case   e_line_left:
            o_space_unit( layfile, curr, &lay->dd.line_left );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->dd.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   output for :ABSTRACT or :PREFACE values                               */
/***************************************************************************/
static  void    put_lay_abspref( FILE * layfile, abspref_lay_tag * ap,
                                 char * name )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":%s\n", name );

    for( k = 0, curr = abspref_att[k]; curr > 0; k++, curr = abspref_att[k] ) {

        switch( curr ) {
        case   e_post_skip:
            o_space_unit( layfile, curr, &ap->post_skip );
            break;
        case   e_pre_top_skip:
            o_space_unit( layfile, curr, &ap->pre_top_skip );
            break;
        case   e_font:
            o_int8( layfile, curr, &ap->font );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &ap->spacing );
            break;
        case   e_header:
            o_yes_no( layfile, curr, &ap->header );
            break;
        case   e_abstract_string:
            if( *name == 'A' ) {
                o_xx_string( layfile, curr, &ap->string );
            }
            break;
        case   e_preface_string:
            if( *name == 'P' ) {
                o_xx_string( layfile, curr, &ap->string );
            }
            break;
        case   e_page_eject:
            o_page_eject( layfile, curr, &ap->page_eject );
            break;
        case   e_page_reset:
            o_yes_no( layfile, curr, &ap->page_reset );
            break;
        case   e_columns:
            o_int8( layfile, curr, &ap->columns );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

static  void    put_lay_abstract( FILE * layfile, layout_data * lay )
{
    put_lay_abspref( layfile, &(lay->abstract), "ABSTRACT" );
}

static  void    put_lay_preface( FILE * layfile, layout_data * lay )
{
    put_lay_abspref( layfile, &(lay->preface), "PREFACE" );
}


/***************************************************************************/
/*   output for :BACKM or :BODY values                                     */
/***************************************************************************/
static  void    put_lay_backbod( FILE * layfile, backbod_lay_tag * ap,
                                 char * name )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":%s\n", name );

    for( k = 0, curr = backbod_att[k]; curr > 0; k++, curr = backbod_att[k] ) {

        switch( curr ) {
        case   e_post_skip:
            o_space_unit( layfile, curr, &ap->post_skip );
            break;
        case   e_pre_top_skip:
            o_space_unit( layfile, curr, &ap->pre_top_skip );
            break;
        case   e_header:
            o_yes_no( layfile, curr, &ap->header );
            break;
        case   e_body_string:
            if( *(name + 1) == 'O') {   // BODY tag
                o_xx_string( layfile, curr, &ap->string );
            }
            break;
        case   e_backm_string:
            if( *(name + 1) == 'A') {   // BACKM tag
                o_xx_string( layfile, curr, &ap->string );
            }
            break;
        case   e_page_eject:
            o_page_eject( layfile, curr, &ap->page_eject );
            break;
        case   e_page_reset:
            o_yes_no( layfile, curr, &ap->page_reset );
            break;
        case   e_columns:
            if( *(name + 1) == 'A') {   // BACKM tag
                o_int8( layfile, curr, &ap->columns );
            }
            break;
        case   e_font:
            o_int8( layfile, curr, &ap->font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

static  void    put_lay_backm( FILE * layfile, layout_data * lay )
{
    put_lay_backbod( layfile, &(lay->backm), "BACKM" );
}

static  void    put_lay_body( FILE * layfile, layout_data * lay )
{
    put_lay_backbod( layfile, &(lay->body), "BODY" );
}


/***************************************************************************/
/*   :LP        output list part attribute values                          */
/***************************************************************************/
static  void    put_lay_lp( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":LP\n" );

    for( k = 0, curr = lp_att[k]; curr > 0; k++, curr = lp_att[k] ) {

        switch( curr ) {
        case   e_left_indent:
            o_space_unit( layfile, curr, &lay->lp.left_indent );
            break;
        case   e_right_indent:
            o_space_unit( layfile, curr, &lay->lp.right_indent );
            break;
        case   e_line_indent:
            o_space_unit( layfile, curr, &lay->lp.line_indent );
            break;
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->lp.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->lp.post_skip );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->lp.spacing );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :INDEX     output index attribute values                              */
/***************************************************************************/
static  void    put_lay_index( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":INDEX\n" );

    for( k = 0, curr = index_att[k]; curr > 0; k++, curr = index_att[k] ) {

        switch( curr ) {
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->index.post_skip );
            break;
        case   e_pre_top_skip:
            o_space_unit( layfile, curr, &lay->index.pre_top_skip );
            break;
        case   e_left_adjust:
            o_space_unit( layfile, curr, &lay->index.left_adjust );
            break;
        case   e_right_adjust:
            o_space_unit( layfile, curr, &lay->index.right_adjust );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->index.spacing );
            break;
        case   e_columns:
            o_int8( layfile, curr, &lay->index.columns );
            break;
        case   e_see_string:
            o_xx_string( layfile, curr, &lay->index.see_string );
            break;
        case   e_see_also_string:
            o_xx_string( layfile, curr, &lay->index.see_also_string );
            break;
        case   e_header:
            o_yes_no( layfile, curr, &lay->index.header );
            break;
        case   e_index_string:
            o_xx_string( layfile, curr, &lay->index.index_string );
            break;
        case   e_page_eject:
            o_page_eject( layfile, curr, &lay->index.page_eject );
            break;
        case   e_page_reset:
            o_yes_no( layfile, curr, &lay->index.page_reset );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->index.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}


/***************************************************************************/
/*   :IXHEAD    output index header attribute values                       */
/***************************************************************************/
static  void    put_lay_ixhead( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":IXHEAD\n" );

    for( k = 0, curr = ixhead_att[k]; curr > 0; k++, curr = ixhead_att[k] ) {

        switch( curr ) {
        case   e_pre_skip:
            o_space_unit( layfile, curr, &lay->ixhead.pre_skip );
            break;
        case   e_post_skip:
            o_space_unit( layfile, curr, &lay->ixhead.post_skip );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->ixhead.font );
            break;
        case   e_indent:
            o_space_unit( layfile, curr, &lay->ixhead.indent );
            break;
        case   e_ixhead_frame:
            curr = e_frame;             // frame = instead of ixhead_frame =
            o_default_frame( layfile, curr, &lay->ixhead.frame );
            break;
        case   e_header:
            o_yes_no( layfile, curr, &lay->ixhead.header );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   :TOC       output table of contents attribute values                  */
/***************************************************************************/
static  void    put_lay_toc( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":TOC\n" );

    for( k = 0, curr = toc_att[k]; curr > 0; k++, curr = toc_att[k] ) {

        switch( curr ) {
        case   e_left_adjust:
            o_space_unit( layfile, curr, &lay->toc.left_adjust );
            break;
        case   e_right_adjust:
            o_space_unit( layfile, curr, &lay->toc.right_adjust );
            break;
        case   e_spacing:
            o_int8( layfile, curr, &lay->toc.spacing );
            break;
        case   e_columns:
            o_int8( layfile, curr, &lay->toc.columns );
            break;
        case   e_toc_levels:
            o_int8( layfile, curr, &lay->toc.toc_levels );
            break;
        case   e_fill_string:
            o_xx_string( layfile, curr, &lay->toc.fill_string );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   :TOCPGNUM  output table of contents number attribute values           */
/***************************************************************************/
static  void    put_lay_tocpgnum( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;

    fprintf_s( layfile, ":TOCPGNUM\n" );

    for( k = 0, curr = tocpgnum_att[k]; curr > 0; k++, curr = tocpgnum_att[k] ) {

        switch( curr ) {
        case   e_size:
            o_space_unit( layfile, curr, &lay->tocpgnum.size );
            break;
        case   e_font:
            o_int8( layfile, curr, &lay->tocpgnum.font );
            break;
        default:
            out_msg( "WGML logic error.\n");
            break;
        }
    }
}

/***************************************************************************/
/*   :TOCHx     output TOC header attribute values for :TOCH0 - :TOCH6     */
/***************************************************************************/
static  void    put_lay_tochx( FILE * layfile, layout_data * lay )
{
    lay_att             curr;
    int                 k;
    int                 lvl;

    for( lvl = 0; lvl < 7; ++lvl ) {


        fprintf_s( layfile, ":TOCH%c\n", '0' + lvl );

        for( k = 0, curr = tochx_att[k]; curr > 0; k++, curr = tochx_att[k] ) {

            switch( curr ) {
            case   e_group:
                o_int8( layfile, curr, &lay->tochx[lvl].group );
                break;
            case   e_indent:
                o_space_unit( layfile, curr, &lay->tochx[lvl].indent );
                break;
            case   e_skip:
                o_space_unit( layfile, curr, &lay->tochx[lvl].skip );
                break;
            case   e_pre_skip:
                o_space_unit( layfile, curr, &lay->tochx[lvl].pre_skip );
                break;
            case   e_post_skip:
                o_space_unit( layfile, curr, &lay->tochx[lvl].post_skip );
                break;
            case   e_font:
                o_int8( layfile, curr, &lay->tochx[lvl].font );
                break;
            case   e_align:
                o_space_unit( layfile, curr, &lay->tochx[lvl].align );
                break;
            case   e_display_in_toc:
                o_yes_no( layfile, curr, &lay->tochx[lvl].display_in_toc );
                break;
                break;
            default:
                out_msg( "WGML logic error.\n");
                break;
            }
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

    fopen_s( &layfile, name, "uwt" );
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
    put_lay_xmp( layfile, lay );
    put_lay_note( layfile, lay );
    put_lay_hx( layfile, lay );
    put_lay_heading( layfile, lay );
    put_lay_lq( layfile, lay );
    put_lay_dt( layfile, lay );
    put_lay_gt( layfile, lay );
    put_lay_dthd( layfile, lay );
    put_lay_cit( layfile, lay );
    put_lay_figcap( layfile, lay );
    put_lay_figdesc( layfile, lay );
    put_lay_dd( layfile, lay );
    put_lay_gd( layfile, lay );
    put_lay_ddhd( layfile, lay );
    put_lay_abstract( layfile, lay );
    put_lay_preface( layfile, lay );
    put_lay_body( layfile, lay );
    put_lay_backm( layfile, lay );
    put_lay_lp( layfile, lay );
    put_lay_index( layfile, lay );
    put_lay_ixpgnum( layfile, lay );
    put_lay_ixmajor( layfile, lay );
    put_lay_ixhead( layfile, lay );
    put_lay_ix( layfile, lay );
    put_lay_toc( layfile, lay );
    put_lay_tocpgnum( layfile, lay );
    put_lay_tochx( layfile, lay );
    put_lay_figlist( layfile, lay );

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
        file_mac_info();
    } else {
        put_layout( token_buf, &layout_work );
    }
    return;
}
