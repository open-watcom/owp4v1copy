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
* Description:  wgml misc error messages
*
****************************************************************************/
 
#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */
 
#include "wgml.h"
#include "gvars.h"
 
 
/***************************************************************************/
/*  display lineno of file/macro and include stack                         */
/***************************************************************************/
 
void    file_mac_info( void )
{
    char        linestr[MAX_L_AS_STR];
 
    if( input_cbs->fmflags & II_macro ) {
//        utoa( input_cbs->s.m->lineno, linestr, 10 );
//        g_info( inf_mac_line, linestr, input_cbs->s.m->mac->name );
    } else {
        utoa( input_cbs->s.f->lineno, linestr, 10 );
        g_info( inf_file_line, linestr, input_cbs->s.f->filename );
    }
    show_include_stack();
    return;
}
 
 
void    att_val_err( char * attname )
{
//****ERROR**** SC--045: Value 'xxx' for the 'yyy' attribute is not defined
    err_count++;
    g_err( err_att_val, token_buf, attname );
    file_mac_info();
    return;
}
 
 
void    auto_att_err( void )
{
//****ERROR**** SC--041: Cannot specify the automatic attribute 'xxx'
    err_count++;
    g_err( err_auto_att, token_buf );
    file_mac_info();
    return;
}
 
 
void    cw_err( void )
{
// SC--006: Unrecognized control word
    err_count++;
    g_err( err_cw_unrecognized, token_buf );
    file_mac_info();
    return;
}
 
 
void    dc_opt_err( char *pa )
{
    err_count++;
    g_err( err_dc_opt, pa );
    file_mac_info();
    return;
}
 
 
void    numb_err( void )
{
    char    linestr[MAX_L_AS_STR];
 
    err_count++;
    if( input_cbs->fmflags & II_macro ) {
        utoa( input_cbs->s.m->lineno, linestr, 10 );
        g_err( ERR_PU_NUM, linestr, "macro", input_cbs->s.m->mac->name );
    } else {
        utoa( input_cbs->s.f->lineno, linestr, 10 );
        g_err( ERR_PU_NUM, linestr, "file", input_cbs->s.f->filename );
    }
    show_include_stack();
    return;
}
 
 
void    nottag_err( void )
{
    err_count++;
    g_err( err_user_tag, tagname );
    file_mac_info();
    return;
}
 
 
void    tag_name_missing_err( void )
{
    err_count++;
    g_err( err_missing_name, "" );
    file_mac_info();
    return;
}
 
 
void    tag_text_err( char * tagname )
{
//****ERROR**** SC--038: Tag text may not be specified for the 'xxx' tag
    err_count++;
    g_err( err_att_text, tagname );
    file_mac_info();
    return;
}
 
 
void    tag_text_req_err( char * tagname )
{
//****ERROR**** SC--039: Tag text must be specified with the 'xxx' tag
    err_count++;
    g_err( err_att_text_req, tagname );
    file_mac_info();
    return;
}
 
 
void    xx_opt_err( char *cw, char *pa )
{
    err_count++;
    g_err( err_xx_opt, cw, pa );
    file_mac_info();
    return;
}
 
 
void    xx_err( const msg_ids errid )
{
    err_count++;
    g_err( errid );
    file_mac_info();
    return;
}
 
