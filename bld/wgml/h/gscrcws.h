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
* Description:  WGML script control words definition header.
*               only uncommented control words are processed so far
*
****************************************************************************/

#ifndef pick
    #error macro pick not defined
#endif
//     control  routinename  flags
//     word

//pick( "ad",    scr_ad,       cw_break    ) // adjust
  pick( "ap",    scr_ap,       0           ) // append
//pick( "bd",    scr_bd,       0           ) // bold
//pick( "bi",    scr_bi,       0           ) // bold italic
//pick( "br",    scr_br,       cw_break    ) // break
//pick( "bx",    scr_bx,       cw_break    ) // box
//pick( "cc",    scr_cc,       cw_break    ) // conditional column
//pick( "cd",    scr_cd,       cw_break    ) // column definition
  pick( "co",    scr_co,       cw_break    ) // concatenate
//pick( "cp",    scr_cp,       cw_break    ) // conditional page
//pick( "cs",    scr_cs,       0           ) // conditional section
//pick( "ct",    scr_ct,       0           ) // continued text
  pick( "cw",    scr_cw,       0           ) // control word separator
  pick( "dc",    scr_dc,       0           ) // define character
//pick( "df",    scr_df,       0           ) // define font
  pick( "dm",    scr_dm,       0           ) // define macro
  pick( "do",    scr_do,       0           ) // do
  pick( "el",    scr_el,       0           ) // else
  pick( "em",    scr_em,       0           ) // execute macro
//pick( "fi",    scr_fi,       0           ) // undocumented CW  ???
/***************************************************************************/
/* script_tso.txt shows .fi as archaic control word for fill and says to   */
/* use .fo (format) instead. BUT, the use of .fi in the OW doc build       */
/* system shows a different use. So, it looks WGML4.0 has the meaning of   */
/* .fi recycled.                                                           */
/***************************************************************************/

//pick( "fk",    scr_fk,       0           ) // floating keep
//pick( "fn",    scr_fn,       0           ) // footnote
  pick( "fo",    scr_fo,       cw_break    ) // format
  pick( "ga",    scr_ga,       0           ) // GML attribute
  pick( "go",    scr_go,       0           ) // go to
  pick( "gt",    scr_gt,       0           ) // GML tag
//pick( "h1",    scr_h1,       cw_break    ) // Heading level 1
//pick( "h2",    scr_h2,       cw_break    ) // heading level 2
  pick( "if",    scr_if,       0           ) // if
  pick( "im",    scr_im,       0           ) // imbed
//pick( "in",    scr_in,       cw_break    ) // indent
//pick( "ix",    scr_ix,       0           ) // index
  pick( "ju",    scr_ju,       cw_break    ) // justify
//pick( "le",    scr_le,       0           ) // leading space
  pick( "li",    scr_li,       0           ) // literal
//pick( "ll",    scr_ll,       cw_break    ) // line length
//pick( "lt",    scr_lt,       cw_break    ) // leading tab
  pick( "me",    scr_me,       0           ) // macro exit
//pick( "oc",    scr_oc,       0           ) // output comment
//pick( "pa",    scr_pa,       cw_break    ) // page eject
  pick( "pe",    scr_pe,       0           ) // perform
//pick( "pl",    scr_pl,       cw_break    ) // page length
//pick( "pp",    scr_pp,       cw_break    ) // paragraph
  pick( "pu",    scr_pu,       0           ) // put workfile
//pick( "ra",    scr_ra,       0           ) // old CW use .ri
//pick( "ri",    scr_ri,       cw_break    ) // right adjust
  pick( "se",    scr_se,       0           ) // set symbol
//pick( "sk",    scr_sk,       cw_break    ) // skip (generate blank line(s)
//pick( "sp",    scr_sp,       cw_break    ) // space same as skip
  pick( "sr",    scr_se,       0           ) // treat as .se as substitute is always on
//pick( "sy",    scr_sy,       0           ) // system command
//pick( "tb",    scr_tb,       cw_break    ) // define tab stops
//pick( "ti",    scr_ti,       0           ) // translate on input
  pick( "th",    scr_th,       0           ) // then
//pick( "tm",    scr_tm,       cw_break    ) // top margin
//pick( "tr",    scr_tr,       0           ) // translate table for output
  pick( "ty",    scr_ty,       0           ) // type
//pick( "ul",    scr_ul,       0           ) // underline
//pick( "us",    scr_us,       0           ) // underscore
  pick( "..",    scr_label,    0           )

#undef pick
