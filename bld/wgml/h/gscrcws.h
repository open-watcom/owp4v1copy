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

  pick( "ap",    scr_ap,       0           )
//pick( "bd",    scr_bd,       0           )
//pick( "bi",    scr_bi,       0           )
//pick( "br",    scr_br,       0           )
//pick( "bx",    scr_bx,       0           )
//pick( "cc",    scr_cc,       0           )
//pick( "cd",    scr_cd,       0           )
//pick( "co",    scr_co,       0           )
//pick( "cp",    scr_cp,       0           )
//pick( "cs",    scr_cs,       0           )
//pick( "ct",    scr_ct,       0           )
  pick( "cw",    scr_cw,       0           )
  pick( "dc",    scr_dc,       0           )
//pick( "df",    scr_df,       0           )
  pick( "dm",    scr_dm,       0           )
  pick( "do",    scr_do,       0           )
  pick( "el",    scr_el,       0           )
  pick( "em",    scr_em,       0           )
//pick( "eq",    scr_eq,       0           )
//pick( "fi",    scr_fi,       0           )
//pick( "fk",    scr_fk,       0           )
//pick( "fn",    scr_fn,       0           )
//pick( "fo",    scr_fo,       0           )
  pick( "ga",    scr_ga,       0           )
  pick( "go",    scr_go,       0           )
  pick( "gt",    scr_gt,       0           )
//pick( "h1",    scr_h1,       0           )
//pick( "h2",    scr_h2,       0           )
  pick( "if",    scr_if,       0           )
  pick( "im",    scr_im,       0           )
//pick( "in",    scr_in,       0           )
//pick( "ix",    scr_ix,       0           )
//pick( "ju",    scr_ju,       0           )
//pick( "le",    scr_le,       0           )
//pick( "li",    scr_li,       0           )
//pick( "ll",    scr_ll,       0           )
//pick( "lt",    scr_lt,       0           )
  pick( "me",    scr_me,       0           )
//pick( "oc",    scr_oc,       0           )
//pick( "pa",    scr_pa,       0           )
//pick( "pe",    scr_pe,       0           )
//pick( "pl",    scr_pl,       0           )
//pick( "pp",    scr_pp,       0           )
  pick( "pu",    scr_pu,       0           )
//pick( "ra",    scr_ra,       0           )
//pick( "ri",    scr_ri,       0           )
  pick( "se",    scr_se,       0           )
//pick( "sk",    scr_sk,       0           )
//pick( "sp",    scr_sp,       0           )
  pick( "sr",    scr_se,       0           ) // treat as .se as substitute is always on
//pick( "sy",    scr_sy,       0           )
//pick( "tb",    scr_tb,       0           )
//pick( "ti",    scr_ti,       0           )
  pick( "th",    scr_th,       0           )
//pick( "tm",    scr_tm,       0           )
//pick( "tr",    scr_tr,       0           )
  pick( "ty",    scr_ty,       0           )
//pick( "ul",    scr_ul,       0           )
//pick( "us",    scr_us,       0           )
  pick( "..",    scr_label,    0           )

#undef pick
