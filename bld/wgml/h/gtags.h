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
*               only tags with a routinename other than gml_dummy
*               are processed so far (not always complete)
*
*    layout tags are defined in gtagslay.h
*
*    :cmt :imbed :include   are defined here and in layout tags identically
*    other tags for example :abstract are defined differently
****************************************************************************/

#ifndef pickg
    #error macro pickg not defined
#endif
//       tagname  tagname  routinename     flags
//                length

  pickg( ABSTRACT, 8,      gml_abstract,   tag_out_txt )
  pickg( ADDRESS,  7,      gml_address,    tag_out_txt )
  pickg( ALINE,    5,      gml_aline,      tag_out_txt )
  pickg( APPENDIX, 8,      gml_appendix,   tag_out_txt )
  pickg( AUTHOR,   6,      gml_author,     tag_out_txt )
  pickg( BACKM,    5,      gml_backm,      tag_out_txt )
  pickg( BINCLUDE, 8,      gml_dummy,      0 )
  pickg( BODY,     4,      gml_body,       tag_out_txt )
  pickg( CIT,      3,      gml_dummy,      tag_out_txt )
  pickg( CMT,      3,      gml_cmt,        tag_only )
  pickg( DATE,     4,      gml_date,       tag_out_txt )
  pickg( DD,       2,      gml_dummy,      tag_out_txt )
  pickg( DDHD,     4,      gml_dummy,      tag_out_txt )
  pickg( DL,       2,      gml_dummy,      tag_out_txt )
  pickg( DOCNUM,   6,      gml_docnum,     tag_out_txt )
  pickg( DT,       2,      gml_dummy,      tag_out_txt )
  pickg( DTHD,     4,      gml_dummy,      tag_out_txt )
  pickg( EADDRESS, 8,      gml_eaddress,   tag_out_txt )
  pickg( ECIT,     4,      gml_dummy,      tag_out_txt )
  pickg( EDL,      3,      gml_dummy,      tag_out_txt )
  pickg( EFIG,     4,      gml_dummy,      tag_out_txt )
  pickg( EFN,      3,      gml_dummy,      tag_out_txt )
  pickg( EGDOC,    5,      gml_egdoc,      tag_out_txt )
  pickg( EGL,      3,      gml_dummy,      tag_out_txt )
  pickg( EHP0,     4,      gml_ehpx,       tag_out_txt )
  pickg( EHP1,     4,      gml_ehpx,       tag_out_txt )
  pickg( EHP2,     4,      gml_ehpx,       tag_out_txt )
  pickg( EHP3,     4,      gml_ehpx,       tag_out_txt )
  pickg( ELQ,      3,      gml_dummy,      tag_out_txt )
  pickg( EOL,      3,      gml_dummy,      tag_out_txt )
  pickg( EPSC,     4,      gml_dummy,      tag_out_txt )
  pickg( EQ,       2,      gml_dummy,      tag_out_txt )
  pickg( ESF,      3,      gml_esf,        tag_out_txt )
  pickg( ESL,      3,      gml_dummy,      tag_out_txt )
  pickg( ETITLEP,  7,      gml_etitlep,    tag_out_txt )
  pickg( EUL,      3,      gml_dummy,      tag_out_txt )
  pickg( EXMP,     4,      gml_dummy,      tag_out_txt )
  pickg( FIG,      3,      gml_dummy,      tag_out_txt )
  pickg( FIGCAP,   6,      gml_dummy,      tag_out_txt )
  pickg( FIGDESC,  7,      gml_dummy,      tag_out_txt )
  pickg( FIGLIST,  7,      gml_figlist,    tag_out_txt )
  pickg( FIGREF,   6,      gml_dummy,      tag_out_txt )
  pickg( FN,       2,      gml_dummy,      tag_out_txt )
  pickg( FNREF,    5,      gml_dummy,      tag_out_txt )
  pickg( FRONTM,   6,      gml_frontm,     tag_out_txt )
  pickg( GDOC,     4,      gml_gdoc,       tag_out_txt )
  pickg( GL,       2,      gml_dummy,      tag_out_txt )
  pickg( GD,       2,      gml_dummy,      tag_out_txt )
  pickg( GRAPHIC,  7,      gml_dummy,      tag_out_txt )
  pickg( GT,       2,      gml_dummy,      tag_out_txt )
  pickg( H0,       2,      gml_dummy,      tag_out_txt )
  pickg( H1,       2,      gml_dummy,      tag_out_txt )
  pickg( H2,       2,      gml_dummy,      tag_out_txt )
  pickg( H3,       2,      gml_dummy,      tag_out_txt )
  pickg( H4,       2,      gml_dummy,      tag_out_txt )
  pickg( H5,       2,      gml_dummy,      tag_out_txt )
  pickg( H6,       2,      gml_dummy,      tag_out_txt )
  pickg( HDREF,    5,      gml_dummy,      tag_out_txt )
  pickg( HP0,      3,      gml_hp0,        tag_out_txt )
  pickg( HP1,      3,      gml_hp1,        tag_out_txt )
  pickg( HP2,      3,      gml_hp2,        tag_out_txt )
  pickg( HP3,      3,      gml_hp3,        tag_out_txt )
  pickg( I1,       2,      gml_dummy,      tag_out_txt )
  pickg( I2,       2,      gml_dummy,      tag_out_txt )
  pickg( I3,       2,      gml_dummy,      tag_out_txt )
  pickg( IH1,      3,      gml_dummy,      tag_out_txt )
  pickg( IH2,      3,      gml_dummy,      tag_out_txt )
  pickg( IH3,      3,      gml_dummy,      tag_out_txt )
  pickg( IMBED,    5,      gml_include,    tag_is_basic    )
  pickg( INCLUDE,  7,      gml_include,    tag_is_basic    )
  pickg( INDEX,    5,      gml_index,      tag_out_txt )
  pickg( IREF,     4,      gml_dummy,      tag_out_txt )
  pickg( LAYOUT,   6,      gml_layout,     0 )
  pickg( LI,       2,      gml_dummy,      tag_out_txt )
  pickg( LIREF,    5,      gml_dummy,      tag_out_txt )
  pickg( LP,       2,      gml_dummy,      tag_out_txt )
  pickg( LQ,       2,      gml_dummy,      tag_out_txt )
  pickg( NOTE,     4,      gml_note,       tag_out_txt )
  pickg( OL,       2,      gml_dummy,      tag_out_txt )
  pickg( P,        1,      gml_p,          tag_out_txt )
  pickg( PC,       2,      gml_pc,         tag_out_txt )
  pickg( PREFACE,  7,      gml_preface,    tag_out_txt )
  pickg( PSC,      3,      gml_dummy,      tag_out_txt )
  pickg( Q,        1,      gml_dummy,      tag_out_txt )
  pickg( SET,      3,      gml_set,        tag_is_basic    )
  pickg( SF,       2,      gml_sf,         tag_out_txt )
  pickg( SL,       2,      gml_dummy,      tag_out_txt )
  pickg( TITLE,    5,      gml_title,      tag_out_txt )
  pickg( TITLEP,   6,      gml_titlep,     tag_out_txt )
  pickg( TOC,      3,      gml_toc,        tag_out_txt )
  pickg( UL,       2,      gml_dummy,      tag_out_txt )
  pickg( XMP,      3,      gml_dummy,      tag_out_txt )

#undef  pickg
