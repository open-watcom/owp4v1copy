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
* Description:  Implements the external functions declared in cfdir.h:
*                   get_compact_entry()
*                   get_extended_entry()
*
****************************************************************************/

#include <stdlib.h>

#include "copdir.h"

/*  Function get_compact_entry().
 *  Reads and returns the current (compact) entry.
 *
 *  Parameters:
 *      in_file points to the length-byte of the defined name
 *      entry is intended to contain the current entry
 *
 *  Modified Parameter:
 *      entry will be modified as indicated below
 *
 *  Returns:
 *      valid_entry if both values of entry were updated
 *      not_valid_entry if only one of the values or neither of the values
 *          of entry was updated.
 *
 *  Notes:
 *      The defined_name and the embedded_name fields will be terminated 
 *          with '\0' if updated.
 *      A file error may have occurred if not_valid_entry is returned.
 */

entry_found get_compact_entry( FILE * in_file, directory_entry * entry )
{
    uint8_t count;

    /* Get the length of the defined name */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );

    /* Ensure the length is not zero or too long for the buffer */

    if( (count == 0) || (count > DEFINED_NAME_MAX) ) {
        return( not_valid_entry );
    }

    /* Get the defined name */

    fread( entry->defined_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );
    entry->defined_name[ count ] = '\0';

    /* Get the length of the member name */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );

    /* Ensure the length is not zero or too long for the buffer */

    if( (count == 0) || ((uint16_t) count > _MAX_PATH) ) {
        return( not_valid_entry );
    }

    /* Get the member name */

    fread( entry->member_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );
    entry->member_name[ count ] = '\0';

    return( valid_entry );
}

/*  Function get_extended_entry().
 *  Reads and returns the current (extended) entry.
 *
 *  Parameters:
 *      in_file points to the length-byte of the defined name
 *      entry is intended to contain the current entry
 *
 *  Modified Parameter:
 *      entry will be modified as indicated below
 *
 *  Returns:
 *      valid_entry if both values of entry were updated.
 *      not_valid_entry if only one of the values or neither of the values
 *          of entry was updated.
 *
 *  Notes:
 *      The defined_name and the embedded_name fields will be terminated 
 *          with '\0' if updated.
 *      A file error may have occurred even if valid_entry is returned.
 */

entry_found get_extended_entry( FILE * in_file, directory_entry * entry )
{
    uint8_t count;

    /* Get the length of the defined name */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );

    /* Ensure the length is not zero or too long for the buffer */

    if( (count == 0) || (count > DEFINED_NAME_MAX) ) {
        return( not_valid_entry );
    }

    /* Get the defined name */

    fread( entry->defined_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );
    entry->defined_name[ count ] = '\0';

    /* skip the marker */
    fseek( in_file, sizeof( uint16_t ), SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );

    /* Get the length of the member name */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );

    /* Ensure the length is not zero or too long for the buffer */

    if( (count == 0) || ((uint16_t) count > _MAX_PATH) ) {
        return( not_valid_entry );
    }

    /* Get the member name */

    fread( entry->member_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( not_valid_entry );
    entry->member_name[ count ] = '\0';

    /* skip the preview */
    fseek( in_file, sizeof( uint16_t ), SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( valid_entry );

    return( valid_entry );
}

