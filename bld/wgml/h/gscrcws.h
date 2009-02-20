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
//       control     tagname  routinename     flags
//       word        length
 
  pick( "ap",         2,      scr_ap,       0           )
//pick( "bd",         2,      scr_bd,       0           )
//pick( "bi",         2,      scr_bi,       0           )
//pick( "br",         2,      scr_br,       0           )
//pick( "bx",         2,      scr_bx,       0           )
//pick( "cc",         2,      scr_cc,       0           )
//pick( "cd",         2,      scr_cd,       0           )
//pick( "co",         2,      scr_co,       0           )
//pick( "cp",         2,      scr_cp,       0           )
//pick( "cs",         2,      scr_cs,       0           )
//pick( "ct",         2,      scr_ct,       0           )
//pick( "cw",         2,      scr_cw,       0           )
//pick( "dc",         2,      scr_dc,       0           )
//pick( "df",         2,      scr_df,       0           )
  pick( "dm",         2,      scr_dm,       0           )
  pick( "do",         2,      scr_do,       0           )
  pick( "el",         2,      scr_el,       0           )
  pick( "em",         2,      scr_em,       0           )
//pick( "eq",         2,      scr_eq,       0           )
//pick( "fi",         2,      scr_fi,       0           )
//pick( "fk",         2,      scr_fk,       0           )
//pick( "fn",         2,      scr_fn,       0           )
//pick( "fo",         2,      scr_fo,       0           )
//pick( "ga",         2,      scr_ga,       0           )
  pick( "go",         2,      scr_go,       0           )
//pick( "gt",         2,      scr_gt,       0           )
//pick( "h1",         2,      scr_h1,       0           )
//pick( "h2",         2,      scr_h2,       0           )
  pick( "if",         2,      scr_if,       0           )
  pick( "im",         2,      scr_im,       0           )
//pick( "in",         2,      scr_in,       0           )
//pick( "ix",         2,      scr_ix,       0           )
//pick( "ju",         2,      scr_ju,       0           )
//pick( "le",         2,      scr_le,       0           )
//pick( "li",         2,      scr_li,       0           )
//pick( "ll",         2,      scr_ll,       0           )
//pick( "lt",         2,      scr_lt,       0           )
  pick( "me",         2,      scr_me,       0           )
//pick( "oc",         2,      scr_oc,       0           )
//pick( "pa",         2,      scr_pa,       0           )
//pick( "pe",         2,      scr_pe,       0           )
//pick( "pl",         2,      scr_pl,       0           )
//pick( "pp",         2,      scr_pp,       0           )
//pick( "pu",         2,      scr_pu,       0           )
//pick( "ra",         2,      scr_ra,       0           )
//pick( "ri",         2,      scr_ri,       0           )
  pick( "se",         2,      scr_se,       0           )
//pick( "sk",         2,      scr_sk,       0           )
//pick( "sp",         2,      scr_sp,       0           )
  pick( "sr",         2,      scr_se,       0           ) // treat as .se as substitute is always on
//pick( "sy",         2,      scr_sy,       0           )
//pick( "tb",         2,      scr_tb,       0           )
//pick( "ti",         2,      scr_ti,       0           )
  pick( "th",         2,      scr_th,       0           )
//pick( "tm",         2,      scr_tm,       0           )
//pick( "tr",         2,      scr_tr,       0           )
//pick( "ty",         2,      scr_ty,       0           )
//pick( "ul",         2,      scr_ul,       0           )
//pick( "us",         2,      scr_us,       0           )
  pick( "..",         2,      scr_label,    0           )
 
#undef pick
