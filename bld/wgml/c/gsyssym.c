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
* Description:  Implements predefined system symbols &gml, &amp, ...
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


#define  mystr(x)           # x
#define  xmystr(s)          mystr(s)

/***************************************************************************/
/*  init_predefined_symbols              incomplete TDB                    */
/***************************************************************************/

void    init_predefined_symbols( void )
{
    char    string[ 2 ] = { 0, 0 };

    add_symvar( &global_dict, "amp", "&", no_subscript, predefined + late_subst );

    string[ 0 ] = CW_SEP_CHAR_DEFAULT;
    add_symvar( &global_dict, "$cw", string, no_subscript, predefined );

    string[ 0 ] = GML_CHAR_DEFAULT;
    add_symvar( &global_dict, "gml", string, no_subscript, predefined + late_subst );
    add_symvar( &global_dict, "$gml", string, no_subscript, predefined + late_subst );

    string[ 0 ] = '.';
    add_symvar( &global_dict, "$per", string, no_subscript, predefined );
}

