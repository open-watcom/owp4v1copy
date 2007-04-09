/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
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
* Description:  Implements the functions declared in cffon.h:
*                   is_fon_file()
*
****************************************************************************/

#include <string.h>
#include "cffon.h"

/*  Function is_fon_file().
 *  Determines whether or not in_file points to the start of a .COP font
 *  file (the first byte after the header).
 *
 *  Parameter:
 *      in_file points to the presumed start of a .COP font file
 *
 *  Returns:
 *      true if this has the correct designator
 *      false otherwise
 *
 */

bool is_fon_file( FILE * in_file)
{
    char designator[3];

    /* Get the designator */

    fread( &designator, 3, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( false );

    /* Verify that the designator is for a .COP device file */

    if( memcmp( designator, "FON", 3 ) ) return( false );
    
    return( true );
}
