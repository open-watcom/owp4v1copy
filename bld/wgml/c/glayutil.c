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
* Description: WGML implement utility functions for :LAYOUT processing
*                   eat_lay_sub_tag()
*                   get_lay_sub_and_value()
*                   i_xxxx               input routines
*                   o_xxxx               output routines
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

static char  const      stryes[] =  { "yes" };
static char  const      strno[]  =  { "no" };


/***************************************************************************/
/*  read lines until a tag starts in col 1                                 */
/*  then set reprocess switch  and return                                  */
/***************************************************************************/

void    eat_lay_sub_tag( void )
{
     while( get_line() ) {
         if( *buff2 == ':' ) {
             ProcFlags.reprocess_line = true;
             break;
         }
     }
}


/***************************************************************************/
/*  parse lines like right_margin = '7i'                                   */
/*              or   right_margin='7i'                                     */
/*  rc = pos if all ok                                                     */
/*  rc = no  in case of error                                              */
/***************************************************************************/

condcode    get_lay_sub_and_value( att_args * args )
{
    char        *   p;
    char            quote;
    condcode        rc;

    p = scan_start;
    rc = no;

    while( *p == ' ' ) {                // over WS to start of name
        p++;
    }
    args->start[0] = p;
    args->len[0] = -1;                  // switch for scanning error
    args->len[1] = -1;                  // switch for scanning error

    while( *p && is_lay_att_char( *p ) ) {
        p++;
    }
    if( *p == '\0' ) {
        if( p == scan_start ) {
            rc = omit;                  // nothing found
        }
        return( rc );                   // or parsing error
    }
    args->len[0] = p - args->start[0];
    if( args->len[0] < 4 ) {            // attribute name length
        err_count++;
        g_err( err_att_name_inv );
        file_mac_info();
        return( rc );
    }

    while( *p && *p == ' ' ) {          // over WS to =
        p++;
    }

    if(*p && *p == '=' ) {
        p++;
        while( *p == ' ' ) {            // over WS to attribute value
            p++;
        }
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        return( no );                   // parsing err '=' missing
    }

    args->start[1] = p;

    if( is_quote_char( *p ) ) {
        quote = *p;
        ++p;
    } else {
        quote = ' ';
    }
    while( *p && *p != quote ) {
        ++p;
    }
    if( *p == quote ) {
        p++;                            // over treminating quote
    }
    args->len[1] = p - args->start[1];

    if( args->len[1] < 1 ) {            // attribute value length
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
    } else {
        rc = pos;
    }
    scan_start = p;
    return( rc );
}


/***************************************************************************/
/*  case                                                                   */
/***************************************************************************/
bool    i_case( char * p, lay_att curr, case_t * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( "mixed", p ) ) {
        *tm = case_mixed;
    } else if( !stricmp( "lower", p ) ) {
        *tm = case_lower;
    } else if( !stricmp( "upper", p ) ) {
        *tm = case_upper;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_case( FILE * f, lay_att curr, case_t * tm )
{
    char    * p;

    if( *tm == case_mixed ) {
        p = "mixed";
    } else if( *tm == case_lower ) {
        p = "lower";
    } else if( *tm == case_upper ) {
        p = "upper";
    } else {
        p = "???";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  single character                                                       */
/***************************************************************************/
bool    i_char( char * p, lay_att curr, char * tm )
{
    if( is_quote_char( *p ) && (*p == *(p + 2)) ) {
        *tm = *(p + 1);                 // 2. char if quoted
    } else {
        *tm = *p;                       // else 1.
    }
    return( false );
}

void    o_char( FILE * f, lay_att curr, char * tm )
{
    fprintf_s( f, "        %s = '%c'\n", att_names[curr], *tm );
    return;
}

/***************************************************************************/
/*  default frame                                                          */
/***************************************************************************/
bool    i_default_frame( char * p, lay_att curr, def_frame * tm )
{
    bool        cvterr;
    int         len;

    cvterr = false;
    if( !stricmp( "none", p ) ) {
        tm->type = none;
    } else if( !stricmp( "rule", p ) ) {
        tm->type = rule_frame;
    } else if( !stricmp( "box", p ) ) {
        tm->type = box_frame;
    } else if( !is_quote_char( *p ) ) {
        cvterr = true;
    } else {
        len = strlen( p );
        if( *p != *(p + len - 1) ) {
            cvterr = true;  // string not terminated
        } else {
            if( sizeof( tm->string ) > len - 2 ) {
                *(p + len - 1 ) = '\0';
                strcpy_s( tm->string, sizeof( tm->string ), p + 1 );
                tm->type = char_frame;
            } else {
                cvterr = true; // string too long;
            }
        }
    }
    if( cvterr ) {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
    }
    return( cvterr );

}

void    o_default_frame( FILE * f, lay_att curr, def_frame * tm )
{

    switch( tm->type ) {
    case   none:
        fprintf_s( f, "        %s = none\n", att_names[curr] );
        break;
    case   rule_frame:
        fprintf_s( f, "        %s = rule\n", att_names[curr] );
        break;
    case   box_frame:
        fprintf_s( f, "        %s = box\n", att_names[curr] );
        break;
    case   char_frame:
        fprintf_s( f, "        %s = '%s'\n", att_names[curr], tm->string );
        break;
    default:
        fprintf_s( f, "        %s = ???\n", att_names[curr] );
        break;
    }
    return;
}


/***************************************************************************/
/*  default place                                                          */
/***************************************************************************/
bool    i_default_place( char * p, lay_att curr, def_place * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( "top", p ) ) {
        *tm = top_place;
    } else if( !stricmp( "bottom", p ) ) {
        *tm = bottom_place;
    } else if( !stricmp( "inline", p ) ) {
        *tm = bottom_place;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_default_place( FILE * f, lay_att curr, def_place * tm )
{
    char    * p;

    if( *tm == top_place ) {
        p = "top";
    } else if( *tm == bottom_place ) {
        p = "bottom";
    } else if( *tm == inline_place ) {
        p = "inline";
    } else {
        p = "???";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  frame  rule or none                                                    */
/***************************************************************************/
bool    i_frame( char * p, lay_att curr, bool * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( "none", p ) ) {
        *tm = false;
    } else if( !stricmp( "rule", p ) ) {
        *tm = true;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );

}

void    o_frame( FILE * f, lay_att curr, bool * tm )
{
    char    * p;

    if( *tm ) {
        p = "rule";
    } else {
        p = "none";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  integer routines                                                       */
/***************************************************************************/
bool    i_int32( char * p, lay_att curr, int32_t * tm )
{

    *tm = strtol( p, NULL, 10 );
    return( false );
}

void    o_int32( FILE * f, lay_att curr, int32_t * tm )
{

    fprintf_s( f, "        %s = %ld\n", att_names[curr], *tm );
    return;
}

bool    i_int8( char * p, lay_att curr, int8_t * tm )
{
    int32_t     wk;

    wk = strtol( p, NULL, 10 );
    if( abs( wk ) > 255 ) {
        return( true );
    }
    *tm = wk;
    return( false );
}

void    o_int8( FILE * f, lay_att curr, int8_t * tm )
{
    int     wk = *tm;

    fprintf_s( f, "        %s = %d\n", att_names[curr], wk );
    return;
}


/***************************************************************************/
/*  number form                                                            */
/***************************************************************************/
bool    i_number_form( char * p, lay_att curr, num_form * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( "none", p ) ) {
        *tm = num_none;
    } else if( !stricmp( "prop", p ) ) {
        *tm = num_prop;
    } else if( !stricmp( "new", p ) ) {
        *tm = num_new;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_number_form( FILE * f, lay_att curr, num_form * tm )
{
    char    * p;

    if( *tm == num_none ) {
        p = "none";
    } else if( *tm == num_prop ) {
        p = "prop";
    } else if( *tm == num_new ) {
        p = "new";
    } else {
        p = "???";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  number style                                                           */
/***************************************************************************/
bool    i_number_style( char * p, lay_att curr, num_style * tm )
{
    bool        cvterr;
    num_style   wk = 0;
    char        c;

    cvterr = false;
    c = tolower( *p );
    switch( c ) {                       // first letter
    case   'a':
        wk |= a_style;
        break;
    case   'b':
        wk |= b_style;
        break;
    case   'c':
        wk |= c_style;
        break;
    case   'r':
        wk |= r_style;
        break;
    case   'h':
        wk |= h_style;
        break;
    default:
        cvterr = true;
        break;
    }

    p++;
    if( !cvterr && *p ) {               // second letter
        c = tolower( *p );
        switch( c ) {
        case   'd':
            wk |= xd_style;
            break;
        case   'p':
            p++;
            if( *p ) {                  // third letter
                c = tolower( *p );
                switch( c ) {
                case   'a':
                    wk |= xpa_style;    // only left parenthesis
                    break;
                case   'b':
                    wk |= xpb_style;    // only right parenthesis
                    break;
                default:
                    cvterr = true;
                    break;
                }
            } else {
                wk |= xp_style;         // left and right parenthesis
            }
            break;
        default:
            cvterr = true;
            break;
        }
    }
    if( !cvterr ) {
        *tm = wk;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
    }
    return( cvterr );
}

void    o_number_style( FILE * f, lay_att curr, num_style * tm )
{
    char        str[4];
    char    *    p;

    p = str;
    if( *tm & h_style ) {
        *p = 'h';
        p++;
    } else if( *tm & a_style ) {
        *p = 'a';
        p++;
    } else if( *tm & b_style ) {
        *p = 'b';
        p++;
    } else if( *tm & c_style ) {
        *p = 'c';
        p++;
    } else if( *tm & r_style ) {
        *p = 'r';
        p++;
    }
    *p = '\0';

    if( *tm & xd_style ) {
        *p = 'd';
        p++;
    } else if( (*tm & xp_style) == xp_style) {
        *p = 'p';
        p++;
    } else if( *tm & xpa_style ) {
        *p = 'p';
        p++;
        *p = 'a';
        p++;
    } else if( *tm & xpb_style ) {
        *p = 'p';
        p++;
        *p = 'b';
        p++;
    }
    *p = '\0';
    fprintf_s( f, "        %s = %s\n", att_names[curr], str );
    return;
}


/***************************************************************************/
/*  page eject                                                             */
/***************************************************************************/
bool    i_page_eject( char * p, lay_att curr, page_ej * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( strno, p ) ) {
        *tm = ej_no;
    } else if( !stricmp( stryes, p ) ) {
        *tm = ej_yes;
    } else if( !stricmp( "odd", p ) ) {
        *tm = ej_odd;
    } else if( !stricmp( "even", p ) ) {
        *tm = ej_even;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_page_eject( FILE * f, lay_att curr, page_ej * tm )
{
    const   char    *   p;

    if( *tm == ej_no ) {
        p = strno;
    } else if( *tm == ej_yes ) {
        p = stryes;
    } else if( *tm == ej_odd ) {
        p = "odd";
    } else if( *tm == ej_even ) {
        p = "even";
    } else {
        p = "???";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  page position                                                          */
/***************************************************************************/
bool    i_page_position( char * p, lay_att curr, page_pos * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( "left", p ) ) {
        *tm = pos_left;
    } else if( !stricmp( "right", p ) ) {
        *tm = pos_right;
    } else if( !(stricmp( "centre", p ) && stricmp( "center", p )) ) {
        *tm = pos_center;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_page_position( FILE * f, lay_att curr, page_pos * tm )
{
    char    * p;

    if( *tm == pos_left ) {
        p = "left";
    } else if( *tm == pos_right ) {
        p = "right";
    } else if( *tm == pos_center ) {
        p = "center";
    } else {
        p = "???";
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}


/***************************************************************************/
/*  Space unit                                                             */
/***************************************************************************/
bool    i_space_unit( char * p, lay_att curr, su * tm )
{

    return( to_internal_SU( &p, tm ) );
}

void    o_space_unit( FILE * f, lay_att curr, su * tm )
{

    if( tm->su_u == SU_chars_lines || tm->su_u == SU_undefined ) {
        fprintf_s( f, "        %s = %s\n", att_names[curr], tm->su_txt );
    } else {
        fprintf_s( f, "        %s = '%s'\n", att_names[curr], tm->su_txt );
    }
    return;
}


/***************************************************************************/
/*  xx_string  for :NOTE and others                                        */
/***************************************************************************/
bool    i_xx_string( char * p, lay_att curr, xx_str * tm )
{
    bool        cvterr;
    int         len;

    cvterr = false;
    len = strlen( p );
    if( *p != *(p + len - 1) ) {
        cvterr = true;                  // string not terminated
    } else {
        if( str_size > len - 2 ) {
            *(p + len - 1) = '\0';
            strcpy_s( tm, str_size, p + 1 );
        } else {
            cvterr = true;              // string too long;
        }
    }
    if( cvterr ) {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
    }
    return( cvterr );
}

void    o_xx_string( FILE * f, lay_att curr, xx_str * tm )
{

    fprintf_s( f, "        %s = \"%s\"\n", att_names[curr], tm );
    return;
}

/***************************************************************************/
/*  date_form      stored as string perhaps better other type    TBD       */
/***************************************************************************/
bool    i_date_form( char * p, lay_att curr, xx_str * tm )
{
    return( i_xx_string( p, curr, tm ) );
}

void    o_date_form( FILE * f, lay_att curr, xx_str * tm )
{
    o_xx_string( f, curr, tm );
}

/***************************************************************************/
/*  Yes or No  as bool result                                              */
/***************************************************************************/
bool    i_yes_no( char * p, lay_att curr, bool * tm )
{
    bool        cvterr;

    cvterr = false;
    if( !stricmp( strno, p ) ) {
        *tm = false;
    } else if( !stricmp( stryes, p ) ) {
        *tm = true;
    } else {
        err_count++;
        g_err( err_att_val_inv );
        file_mac_info();
        cvterr = true;
    }
    return( cvterr );
}

void    o_yes_no( FILE * f, lay_att curr, bool * tm )
{
    char    const   *   p;

    if( *tm == 0 ) {
        p = strno;
    } else {
        p = stryes;
    }
    fprintf_s( f, "        %s = %s\n", att_names[curr], p );
    return;
}



