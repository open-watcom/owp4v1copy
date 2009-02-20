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
* Description:  WGML GML tags definition header.
*               only uncommented tags are processed so far
*
****************************************************************************/
 
#ifndef pick
    #error macro pick not defined
#endif
//       tagname     tagname  routinename     flags
//                   length
 
//pick( "ABSTRACT",   8,      gml_abstract,
//pick( "ADDRESS",    7,      gml_address,
//pick( "ALINE",      5,      gml_aline,
//pick( "APPENDIX",   8,      gml_appendix,
//pick( "AUTHOR",     6,      gml_author,
//pick( "BACKM",      5,      gml_backm,
//pick( "BINCLUDE",   8,      gml_binclude,
//pick( "BODY",       5,      gml_body,
//pick( "CIT",        3,      gml_cit,
  pick( "CMT",        3,      gml_cmt,        tag_only        )
//pick( "DATE",       4,      gml_date,
//pick( "DD",         2,      gml_dd,
//pick( "DDHD",       4,      gml_ddhd,
//pick( "DL",         2,      gml_dl,
//pick( "DOCNUM",     6,      gml_docnum,
//pick( "DT",         2,      gml_dt,
//pick( "DTHD",       4,      gml_dthd,
//pick( "EADDRESS",   8,      gml_eaddress,
//pick( "ECIT",       4,      gml_ecit,
//pick( "EDL",        3,      gml_edl,
//pick( "EFIG",       4,      gml_efig,
//pick( "EFN",        3,      gml_efn,
//pick( "EGDOC",      5,      gml_egdoc,
//pick( "EGL",        3,      gml_egl,
//pick( "EHP0",       4,      gml_ehp0,
//pick( "EHP1",       4,      gml_ehp1,
//pick( "EHP2",       4,      gml_ehp2,
//pick( "EHP3",       4,      gml_ehp3,
//pick( "ELAYOUT",    7,      gml_elayout,
//pick( "ELQ",        3,      gml_elq,
//pick( "EOL",        3,      gml_eol,
//pick( "EPSC",       4,      gml_epsc,
//pick( "EQ",         2,      gml_eq,
//pick( "ESF",        3,      gml_esf,
//pick( "ESL",        3,      gml_esl,
//pick( "ETITLEP",    7,      gml_etitlep,
//pick( "EUL",        3,      gml_eul,
//pick( "EXMP",       4,      gml_exmp,
//pick( "FIG",        3,      gml_fig,
//pick( "FIGCAP",     6,      gml_figcap,
//pick( "FIGDESC",    7,      gml_figdesc,
//pick( "FIGLIST",    7,      gml_figlist,
//pick( "FIGREF",     6,      gml_figref,
//pick( "FN",         2,      gml_fn,
//pick( "FNREF",      5,      gml_fnref,
//pick( "FRONTM",     6,      gml_frontm,
//pick( "GDOC",       4,      gml_gdoc,
//pick( "GL",         2,      gml_gl,
//pick( "GD",         2,      gml_gd,
//pick( "GRAPHIC",    7,      gml_graphic,
//pick( "GT",         2,      gml_gt,
//pick( "H0",         2,      gml_h0,
//pick( "H1",         2,      gml_h1,
//pick( "H2",         2,      gml_h2,
//pick( "H3",         2,      gml_h3,
//pick( "H4",         2,      gml_h4,
//pick( "H5",         2,      gml_h5,
//pick( "H6",         2,      gml_h6,
//pick( "HDREF",      5,      gml_hdref,
//pick( "HP0",        3,      gml_hp0,
//pick( "HP1",        3,      gml_hp1,
//pick( "HP2",        3,      gml_hp2,
//pick( "HP3",        3,      gml_hp3,
//pick( "I1",         2,      gml_i1,
//pick( "I2",         2,      gml_i2,
//pick( "I3",         2,      gml_i3,
//pick( "IH1",        3,      gml_ih1,
//pick( "IH2",        3,      gml_ih2,
//pick( "IH3",        3,      gml_ih3,
  pick( "IMBED",      5,      gml_include,    tag_is_basic    )
  pick( "INCLUDE",    7,      gml_include,    tag_is_basic    )
//pick( "INDEX",      5,      gml_index,
//pick( "IREF",       4,      gml_iref,
//pick( "LAYOUT",     6,      gml_layout,
//pick( "LI",         2,      gml_li,
//pick( "LIREF",      5,      gml_liref,
//pick( "LP",         2,      gml_lp,
//pick( "LQ",         2,      gml_lq,
//pick( "NOTE",       4,      gml_note,
//pick( "OL",         2,      gml_ol,
//pick( "P",          1,      gml_p,
//pick( "PC",         2,      gml_pc,
//pick( "PREFACE",    8,      gml_preface,
//pick( "PSC",        3,      gml_psc,
//pick( "Q",          1,      gml_q,
  pick( "SET",        3,      gml_set,        tag_is_basic    )
//pick( "SF",         2,      gml_sf,
//pick( "SL",         2,      gml_sl,
//pick( "TITLE",      5,      gml_title,
//pick( "TITLEP",     6,      gml_titlep,
//pick( "TOC",        3,      gml_toc,
//pick( "UL",         2,      gml_ul,
//pick( "XMP",        3,      gml_xmp,
 
#undef  pick 
