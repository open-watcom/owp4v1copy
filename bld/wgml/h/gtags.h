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
****************************************************************************/

#ifndef pick
    #error macro pick not defined
#endif
//       tagname    tagname  routinename     flags
//                  length

  pick( "ABSTRACT", 8,      gml_dummy,      0 )
  pick( "ADDRESS",  7,      gml_dummy,      0 )
  pick( "ALINE",    5,      gml_dummy,      0 )
  pick( "APPENDIX", 8,      gml_dummy,      0 )
  pick( "AUTHOR",   6,      gml_dummy,      0 )
  pick( "BACKM",    5,      gml_dummy,      0 )
  pick( "BINCLUDE", 8,      gml_dummy,      0 )
  pick( "BODY",     4,      gml_dummy,      0 )
  pick( "CIT",      3,      gml_dummy,      0 )
  pick( "CMT",      3,      gml_cmt,        tag_only )
  pick( "DATE",     4,      gml_dummy,      0 )
  pick( "DD",       2,      gml_dummy,      0 )
  pick( "DDHD",     4,      gml_dummy,      0 )
  pick( "DL",       2,      gml_dummy,      0 )
  pick( "DOCNUM",   6,      gml_dummy,      0 )
  pick( "DT",       2,      gml_dummy,      0 )
  pick( "DTHD",     4,      gml_dummy,      0 )
  pick( "EADDRESS", 8,      gml_dummy,      0 )
  pick( "ECIT",     4,      gml_dummy,      0 )
  pick( "EDL",      3,      gml_dummy,      0 )
  pick( "EFIG",     4,      gml_dummy,      0 )
  pick( "EFN",      3,      gml_dummy,      0 )
  pick( "EGDOC",    5,      gml_dummy,      0 )
  pick( "EGL",      3,      gml_dummy,      0 )
  pick( "EHP0",     4,      gml_dummy,      0 )
  pick( "EHP1",     4,      gml_dummy,      0 )
  pick( "EHP2",     4,      gml_dummy,      0 )
  pick( "EHP3",     4,      gml_dummy,      0 )
  pick( "ELAYOUT",  7,      gml_dummy,      0 )
  pick( "ELQ",      3,      gml_dummy,      0 )
  pick( "EOL",      3,      gml_dummy,      0 )
  pick( "EPSC",     4,      gml_dummy,      0 )
  pick( "EQ",       2,      gml_dummy,      0 )
  pick( "ESF",      3,      gml_dummy,      0 )
  pick( "ESL",      3,      gml_dummy,      0 )
  pick( "ETITLEP",  7,      gml_dummy,      0 )
  pick( "EUL",      3,      gml_dummy,      0 )
  pick( "EXMP",     4,      gml_dummy,      0 )
  pick( "FIG",      3,      gml_dummy,      0 )
  pick( "FIGCAP",   6,      gml_dummy,      0 )
  pick( "FIGDESC",  7,      gml_dummy,      0 )
  pick( "FIGLIST",  7,      gml_dummy,      0 )
  pick( "FIGREF",   6,      gml_dummy,      0 )
  pick( "FN",       2,      gml_dummy,      0 )
  pick( "FNREF",    5,      gml_dummy,      0 )
  pick( "FRONTM",   6,      gml_dummy,      0 )
  pick( "GDOC",     4,      gml_dummy,      0 )
  pick( "GL",       2,      gml_dummy,      0 )
  pick( "GD",       2,      gml_dummy,      0 )
  pick( "GRAPHIC",  7,      gml_dummy,      0 )
  pick( "GT",       2,      gml_dummy,      0 )
  pick( "H0",       2,      gml_dummy,      0 )
  pick( "H1",       2,      gml_dummy,      0 )
  pick( "H2",       2,      gml_dummy,      0 )
  pick( "H3",       2,      gml_dummy,      0 )
  pick( "H4",       2,      gml_dummy,      0 )
  pick( "H5",       2,      gml_dummy,      0 )
  pick( "H6",       2,      gml_dummy,      0 )
  pick( "HDREF",    5,      gml_dummy,      0 )
  pick( "HP0",      3,      gml_dummy,      0 )
  pick( "HP1",      3,      gml_dummy,      0 )
  pick( "HP2",      3,      gml_dummy,      0 )
  pick( "HP3",      3,      gml_dummy,      0 )
  pick( "I1",       2,      gml_dummy,      0 )
  pick( "I2",       2,      gml_dummy,      0 )
  pick( "I3",       2,      gml_dummy,      0 )
  pick( "IH1",      3,      gml_dummy,      0 )
  pick( "IH2",      3,      gml_dummy,      0 )
  pick( "IH3",      3,      gml_dummy,      0 )
  pick( "IMBED",    5,      gml_include,    tag_is_basic    )
  pick( "INCLUDE",  7,      gml_include,    tag_is_basic    )
  pick( "INDEX",    5,      gml_dummy,      0 )
  pick( "IREF",     4,      gml_dummy,      0 )
  pick( "LAYOUT",   6,      gml_dummy,      0 )
  pick( "LI",       2,      gml_dummy,      0 )
  pick( "LIREF",    5,      gml_dummy,      0 )
  pick( "LP",       2,      gml_dummy,      0 )
  pick( "LQ",       2,      gml_dummy,      0 )
  pick( "NOTE",     4,      gml_dummy,      0 )
  pick( "OL",       2,      gml_dummy,      0 )
  pick( "P",        1,      gml_dummy,      0 )
  pick( "PC",       2,      gml_dummy,      0 )
  pick( "PREFACE",  8,      gml_dummy,      0 )
  pick( "PSC",      3,      gml_dummy,      0 )
  pick( "Q",        1,      gml_dummy,      0 )
  pick( "SET",      3,      gml_set,        tag_is_basic    )
  pick( "SF",       2,      gml_dummy,      0 )
  pick( "SL",       2,      gml_dummy,      0 )
  pick( "TITLE",    5,      gml_dummy,      0 )
  pick( "TITLEP",   6,      gml_dummy,      0 )
  pick( "TOC",      3,      gml_dummy,      0 )
  pick( "UL",       2,      gml_dummy,      0 )
  pick( "XMP",      3,      gml_dummy,      0 )

#undef  pick
