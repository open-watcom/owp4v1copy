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
* Description:  WGML layout attribute definitions
*                    and corresponding utility function definitions
*                    to be completed                                TBD
****************************************************************************/

#ifndef pick
    #error macro pick not defined
#endif
//      attribute                 routinename           result type
//      name               input            output

//pick( dummy_zero,        NULL,            NULL,            int )// dummy entry

  pick( align,             i_space_unit,    o_space_unit,    su        )
  pick( binding,           i_space_unit,    o_space_unit,    su        )
  pick( columns,           i_int8,          o_int8,          int8_t    )
  pick( default_frame,     i_default_frame, o_default_frame, def_frame )
  pick( default_place,     i_default_place, o_default_place, def_place )
  pick( depth,             i_space_unit,    o_space_unit,    su        )
  pick( font,              i_int8,          o_int8,          int8_t    )
  pick( frame,             i_frame,         o_frame,         bool      )
  pick( gutter,            i_space_unit,    o_space_unit,    su        )
  pick( input_esc,         i_char,          o_char,          char      )
  pick( justify,           i_yes_no,        o_yes_no,        bool      )
  pick( line_indent,       i_space_unit,    o_space_unit,    su        )
  pick( left_adjust,       i_space_unit,    o_space_unit,    su        )
  pick( left_indent,       i_space_unit,    o_space_unit,    su        )
  pick( left_margin,       i_space_unit,    o_space_unit,    su        )
  pick( number_font,       i_int8,          o_int8,          int8_t    )
  pick( number_style,      i_number_style,  o_number_style,  num_style )
  pick( pre_lines,         i_space_unit,    o_space_unit,    su        )
  pick( pre_skip,          i_space_unit,    o_space_unit,    su        )
  pick( post_skip,         i_space_unit,    o_space_unit,    su        )
  pick( right_adjust,      i_space_unit,    o_space_unit,    su        )
  pick( right_indent,      i_space_unit,    o_space_unit,    su        )
  pick( right_margin,      i_space_unit,    o_space_unit,    su        )
  pick( skip,              i_space_unit,    o_space_unit,    su        )
  pick( spacing,           i_int8,          o_int8,          int8_t    )
  pick( threshold,         i_int8,          o_int8,          int8_t    )
  pick( top_margin,        i_space_unit,    o_space_unit,    su        )

//pick( dummy_max,         NULL,            NULL,            int )// dummy entry

#undef  pick
