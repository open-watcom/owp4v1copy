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
* Description:  Implements the functions declared in cfdir.h:
*                   get_entry_count()
*                   get_entry_type()
*                   get_compact_entry()
*                   get_extended_entry()
*                   skip_compact_entry()
*                   skip_extended_entry()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
****************************************************************************/

#include <stdlib.h>
#include "cfdir.h"
#include "common.h"
#include "wstd.h"

/*  Function get_entry_count().
 *  Reads and returns the entry count.
 *
 *  Parameters:
 *      in_file points to the first byte of the entry count, which is the
 *          first byte after the .COP file header.
 *      entry_count is intended to contain the number of entries.
 *
 *  Modified Parameter:
 *      entry_count returns the entry count if SUCCESS is returned.
 *      entry_count contains its value on entry if FAILURE is returned.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 *
 */

int get_entry_count( FILE * in_file, uint16_t * entry_count )
{
    fread( entry_count, sizeof( *entry_count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file) ) return( FAILURE );
    return( SUCCESS );
}

/*  Function get_entry_type().
 *  Reads and returns the entry type.
 *  This is intended to be used for both meta-types and entry types as such.
 *  There are two meta-types:
 *      0x0000 indicates that the following entries are in compact format.
 *      0x0001 indicates that the following entry is in extended format.
 *  There are three values for the entry type as such:
 *      0x0101 for device files.
 *      0x0201 for driver files.
 *      0x0401 for font files.
 *
 *  Parameters:
 *      in_file points to the first byte of the entry type (either kind).
 *      entry_type is intended to contain the meta-type or the entry type
 *          as such.
 *
 *  Modified Parameter:
 *      entry_type contains the value of the meta-type or the entry type 
 *          as such if SUCCESS is returned.
 *      entry_type contains its value on entry if FAILURE is returned.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 *
 */

int get_entry_type( FILE * in_file, uint16_t * entry_type )
{
    fread( entry_type, sizeof( *entry_type ), 1, in_file );
    if( feof( in_file ) || ferror( in_file ) ) return( FAILURE );
    return( SUCCESS );
}

/*  Function get_compact_entry().
 *  Reads and returns the current entry.
 *
 *  Parameters:
 *      in_file points to the length-byte of the item name of the item to be
 *          skipped.
 *      entry is intended to contain the current entry.
 *
 *  Modified Parameter:
 *      entry will contain the values from the current entry on SUCCESS.
 *      entry will contain either the values on entry or a mixture of the
 *          values on entry with the values from the current entry on
 *          FAILURE, depending on where the failure occurs.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 *
 *  Note: the item_name and the file_name fields will be terminated with
 *        NULLCHAR if SUCCESS is returned.
 */

int get_compact_entry( FILE * in_file, compact_entry_t * entry )
{
    uint8_t count;

    /* Get the length of the item_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Ensure the length is not zero or too long for the buffer. */

    if( (count == 0) || (count > ITEM_MAX) ) {
        return( FAILURE );
    }

    /* Get the item_name. */

    fread( entry->item_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Make it a null-terminated string. */

    entry->item_name[count] = NULLCHAR;

    /* Get the length of the file_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Ensure the length is not zero or too long for the buffer. */

    if( (count == 0) || ((uint16_t) count > NAME_MAX) ) {
        return( FAILURE );
    }

    /* Get the file_name. */

    fread( entry->file_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Make it a null-terminated string. */

    entry->file_name[count] = NULLCHAR;

    return( SUCCESS );
}

/*  Function get_extended_entry().
 *  Reads and returns the current entry.
 *
 *  Parameters:
 *      in_file points to the length-byte of the item name of the item to be
 *          skipped.
 *      entry is intended to contain the current entry.
 *
 *  Modified Parameter:
 *      entry will contain the values from the current entry on SUCCESS.
 *      entry will contain either the values on entry or a mixture of the
 *          values on entry with the values from the current entry on
 *          FAILURE, depending on where the failure occurs.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 *
 *  Note: the item_name and the file_name fields will be terminated with
 *        NULLCHAR if SUCCESS is returned.
 */

int get_extended_entry( FILE * in_file, extended_entry_t * entry )
{
    uint8_t count;

    /* Get the length of the item_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Ensure the length is not zero or too long for the buffer. */

    if( (count == 0) || (count > ITEM_MAX) ) {
        return( FAILURE );
    }

    /* Get the item_name. */

    fread( entry->item_name, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Make it a null-terminated string. */

    entry->item_name[count] = NULLCHAR;

    /* Get the marker. */

    fread( &(entry->marker), sizeof( entry->marker ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Get the length of the file_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Ensure the length is not zero or too long for the buffer. */

    if( (count == 0) || ((uint16_t) count > NAME_MAX) ) {
        return( FAILURE );
    }

    /* Get the file_name. */

    fread( &(entry->file_name), count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Make it a null-terminated string. */

    entry->file_name[count] = NULLCHAR;

    /* Get the preview. */

    fread( &(entry->preview), sizeof( entry->preview ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    return( SUCCESS );
}

/*  Function skip_compact_entry().
 *  Skips the current entry.
 *
 *  Parameter:
 *      in_file points to the length-byte of the item name of the item to be
 *          skipped.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 */

int skip_compact_entry( FILE * in_file )
{
    uint8_t count;

    /* Get the length of the item_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the item_name. */

    fseek( in_file, count, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Get the length of the file_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the file_name. */

    fseek( in_file, count, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    return( SUCCESS );
}

/*  Function skip_extended_entry().
 *  Skips the current entry.
 *
 *  Parameter:
 *      in_file points to the length-byte of the item name of the item to be
 *          skipped.
 *
 *  Returns:
 *      FAILURE on a file error or on EOF.
 *      SUCCESS otherwise.
 */

int skip_extended_entry( FILE * in_file )
{
    uint8_t count;

    /* Get the length of the item_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the item_name. */

    fseek( in_file, count, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the marker. */

    fseek( in_file, sizeof( uint16_t ), SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Get the length of the file_name. */
    
    count = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the file_name. */

    fseek( in_file, count, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    /* Skip the preview. */

    fseek( in_file, sizeof( uint16_t ), SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( FAILURE );

    return( SUCCESS );
}

