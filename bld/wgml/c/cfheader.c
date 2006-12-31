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
* Description:  Implements function parse_header(), which verifies the
*               correctness of a .COP file header.
*
****************************************************************************/

/*
 *  Verifies that a given file is, in fact, a valid .COP file and, if it is,
 *  returns the file type.
 *
 *  Parameter:
 *      inFile -- the FILE to check
 *
 *  Returns:
 *      FAILURE if the header is invalid
 *      The file type if the header is valid
 *   
*/

#include <stdio.h>
#include <string.h>
#include "common.h"

/*
 *  Determine if the current position of the input stream points to the
 *  start of a valid .COP file and, if it does, advance the stream to the
 *  first byte following the header.
 *
 *  Parameter:
 *      in_file contains the input stream
 *
 *  Returns:
 *      FAILURE if in_file does not point to the start of a valid .COP
 *          file header
 *      FAILURE on any file error, since a valid .COP file has data after
 *          the header
 *      the file type byte if in_file points to the start of a valid .COP
 *          file header and the file contains data following the header
 */

int parse_header( FILE * in_file )
{
    char magic[4];
    int  type;

    /* Get the "magic number" and the version length byte */

    fread( &magic, 4, 1, in_file);
    if( ferror( in_file ) ) return( FAILURE );

    /* If neither a version 3 or a version 4 header, it is not a COP file */
    /* Check for a version 3 header */

    if( memcmp( magic, "\x02\x0a\x00", 3 ) ) {

        /* Check for a version 4 header */
        
        if( memcmp( magic, "\x02\x0c\x00", 3 ) ) return( FAILURE );
    }

    /* Skip the version text */
    
    fseek( in_file, magic[3], SEEK_CUR );
    if( ferror( in_file ) ) return( FAILURE );

    /* Get the file type byte */

    type = fgetc( in_file );

    /* If there is no more data, this is not a valid .COP file */
    
    if( ferror( in_file ) ) return( FAILURE );

    /* Valid header, more data exists, return the file type byte */
    
    return( type );
}

