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
*               are processed so far
*
*    layout tags are defined in gtagslay.h
*
*    :cmt :imbed :include   are defined here and in layout tags identically
*    other tags for example :abstract are defined differently
****************************************************************************/

#ifndef pick
    #error macro pick not defined
#endif
//       tagname    tagname  routinename     flags
//                  length

  pick( "ABSTRACT", 8,      gml_dummy,      tag_out_txt )
  pick( "ADDRESS",  7,      gml_dummy,      tag_out_txt )
  pick( "ALINE",    5,      gml_dummy,      tag_out_txt )
  pick( "APPENDIX", 8,      gml_dummy,      tag_out_txt )
  pick( "AUTHOR",   6,      gml_dummy,      tag_out_txt )
  pick( "BACKM",    5,      gml_dummy,      tag_out_txt )
  pick( "BINCLUDE", 8,      gml_dummy,      0 )
  pick( "BODY",     4,      gml_dummy,      tag_out_txt )
  pick( "CIT",      3,      gml_dummy,      tag_out_txt )
  pick( "CMT",      3,      gml_cmt,        tag_only )
  pick( "DATE",     4,      gml_dummy,      tag_out_txt )
  pick( "DD",       2,      gml_dummy,      tag_out_txt )
  pick( "DDHD",     4,      gml_dummy,      tag_out_txt )
  pick( "DL",       2,      gml_dummy,      tag_out_txt )
  pick( "DOCNUM",   6,      gml_dummy,      tag_out_txt )
  pick( "DT",       2,      gml_dummy,      tag_out_txt )
  pick( "DTHD",     4,      gml_dummy,      tag_out_txt )
  pick( "EADDRESS", 8,      gml_dummy,      tag_out_txt )
  pick( "ECIT",     4,      gml_dummy,      tag_out_txt )
  pick( "EDL",      3,      gml_dummy,      tag_out_txt )
  pick( "EFIG",     4,      gml_dummy,      tag_out_txt )
  pick( "EFN",      3,      gml_dummy,      tag_out_txt )
  pick( "EGDOC",    5,      gml_dummy,      tag_out_txt )
  pick( "EGL",      3,      gml_dummy,      tag_out_txt )
  pick( "EHP0",     4,      gml_dummy,      tag_out_txt )
  pick( "EHP1",     4,      gml_dummy,      tag_out_txt )
  pick( "EHP2",     4,      gml_dummy,      tag_out_txt )
  pick( "EHP3",     4,      gml_dummy,      tag_out_txt )
  pick( "ELQ",      3,      gml_dummy,      tag_out_txt )
  pick( "EOL",      3,      gml_dummy,      tag_out_txt )
  pick( "EPSC",     4,      gml_dummy,      tag_out_txt )
  pick( "EQ",       2,      gml_dummy,      tag_out_txt )
  pick( "ESF",      3,      gml_dummy,      tag_out_txt )
  pick( "ESL",      3,      gml_dummy,      tag_out_txt )
  pick( "ETITLEP",  7,      gml_dummy,      tag_out_txt )
  pick( "EUL",      3,      gml_dummy,      tag_out_txt )
  pick( "EXMP",     4,      gml_dummy,      tag_out_txt )
  pick( "FIG",      3,      gml_dummy,      tag_out_txt )
  pick( "FIGCAP",   6,      gml_dummy,      tag_out_txt )
  pick( "FIGDESC",  7,      gml_dummy,      tag_out_txt )
  pick( "FIGLIST",  7,      gml_dummy,      tag_out_txt )
  pick( "FIGREF",   6,      gml_dummy,      tag_out_txt )
  pick( "FN",       2,      gml_dummy,      tag_out_txt )
  pick( "FNREF",    5,      gml_dummy,      tag_out_txt )
  pick( "FRONTM",   6,      gml_dummy,      tag_out_txt )
  pick( "GDOC",     4,      gml_dummy,      tag_out_txt )
  pick( "GL",       2,      gml_dummy,      tag_out_txt )
  pick( "GD",       2,      gml_dummy,      tag_out_txt )
  pick( "GRAPHIC",  7,      gml_dummy,      tag_out_txt )
  pick( "GT",       2,      gml_dummy,      tag_out_txt )
  pick( "H0",       2,      gml_dummy,      tag_out_txt )
  pick( "H1",       2,      gml_dummy,      tag_out_txt )
  pick( "H2",       2,      gml_dummy,      tag_out_txt )
  pick( "H3",       2,      gml_dummy,      tag_out_txt )
  pick( "H4",       2,      gml_dummy,      tag_out_txt )
  pick( "H5",       2,      gml_dummy,      tag_out_txt )
  pick( "H6",       2,      gml_dummy,      tag_out_txt )
  pick( "HDREF",    5,      gml_dummy,      tag_out_txt )
  pick( "HP0",      3,      gml_dummy,      tag_out_txt )
  pick( "HP1",      3,      gml_dummy,      tag_out_txt )
  pick( "HP2",      3,      gml_dummy,      tag_out_txt )
  pick( "HP3",      3,      gml_dummy,      tag_out_txt )
  pick( "I1",       2,      gml_dummy,      tag_out_txt )
  pick( "I2",       2,      gml_dummy,      tag_out_txt )
  pick( "I3",       2,      gml_dummy,      tag_out_txt )
  pick( "IH1",      3,      gml_dummy,      tag_out_txt )
  pick( "IH2",      3,      gml_dummy,      tag_out_txt )
  pick( "IH3",      3,      gml_dummy,      tag_out_txt )
  pick( "IMBED",    5,      gml_include,    tag_is_basic    )
  pick( "INCLUDE",  7,      gml_include,    tag_is_basic    )
  pick( "INDEX",    5,      gml_dummy,      tag_out_txt )
  pick( "IREF",     4,      gml_dummy,      tag_out_txt )
  pick( "LAYOUT",   6,      gml_layout,     0 )
  pick( "LI",       2,      gml_dummy,      tag_out_txt )
  pick( "LIREF",    5,      gml_dummy,      tag_out_txt )
  pick( "LP",       2,      gml_dummy,      tag_out_txt )
  pick( "LQ",       2,      gml_dummy,      tag_out_txt )
  pick( "NOTE",     4,      gml_dummy,      tag_out_txt )
  pick( "OL",       2,      gml_dummy,      tag_out_txt )
  pick( "P",        1,      gml_dummy,      tag_out_txt )
  pick( "PC",       2,      gml_dummy,      tag_out_txt )
  pick( "PREFACE",  8,      gml_dummy,      tag_out_txt )
  pick( "PSC",      3,      gml_dummy,      tag_out_txt )
  pick( "Q",        1,      gml_dummy,      tag_out_txt )
  pick( "SET",      3,      gml_set,        tag_is_basic    )
  pick( "SF",       2,      gml_dummy,      tag_out_txt )
  pick( "SL",       2,      gml_dummy,      tag_out_txt )
  pick( "TITLE",    5,      gml_dummy,      tag_out_txt )
  pick( "TITLEP",   6,      gml_dummy,      tag_out_txt )
  pick( "TOC",      3,      gml_dummy,      tag_out_txt )
  pick( "UL",       2,      gml_dummy,      tag_out_txt )
  pick( "XMP",      3,      gml_dummy,      tag_out_txt )

#undef  pick
