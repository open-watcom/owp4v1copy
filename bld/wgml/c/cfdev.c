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
* Description:  Implements the functions declared in cfdev.h:
*                   is_dev_file()
*                   parse_device()
*                   resize_device()
*
****************************************************************************/

#include <stdlib.h>
#define __STDC_WANT_LIB_EXT1__  1
#include <string.h>
#include "cfdev.h"

/*  Local macros */

#define START_SIZE 2048
#define INC_SIZE   1024

/*  Local function declaration */

static cop_device * resize_device( cop_device *, size_t );

/*  Function definitions */

/*  Function is_dev_file().
 *  Determines whether or not in_file points to the start of a .COP device
 *  file (the first byte after the header).
 *
 *  Parameter:
 *      in_file points to the presumed start of a .COP device file
 *
 *  Returns:
 *      true if this has the correct designator
 *      false otherwise
 *
 */

bool is_dev_file( FILE * in_file )
{
    char designator[3];

    /* Get the designator */

    fread( &designator, 3, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( false );

    /* Verify that the designator is for a .COP device file */

    if( memcmp( designator, "DEV", 3 ) ) return( false );
    
    return( true );
}

/*  Function parse_device().
 *  Constructs a cop_device instance from the given input stream.
 *  
 *  Parameters:
 *      in_file points to the first byte of a .COP file encoding a :DEVICE
 *          struct after the "DEV" descriminator.
 *
 *  Returns:
 *      A pointer to a cop_device struct containing the data from in_file
 *          on success.
 *      A NULL pointer on failure.
 */

cop_device * parse_device( FILE * in_file )
{
    /* The out_device instance */
    
    cop_device *    out_device = NULL;

    /* Used to acquire string attributes */

    uint8_t         length;
    char *          string_ptr;

    /* Used to acquire numeric attributes */

    uint16_t        designator;
    uint16_t        numeric_16;

    /* Used to acquire the TranslationBlock */

    uint8_t         data_count;
    uint16_t        i;
    uint8_t         intrans_flag;
    uint16_t        outtrans_array[0x100];    
    uint8_t *       outtrans_data = NULL;
    uint8_t         outtrans_flag;
    uint8_t *       translation_start = NULL;

    /* Used for count and other values */

    uint8_t         count8;
    uint16_t        next_codeblock;

    /* Initialize the out_device instance */
        
    out_device = (cop_device *) malloc( START_SIZE );
    if( out_device == NULL ) return( out_device );

    out_device->allocated_size = START_SIZE;
    out_device->next_offset = sizeof( cop_device );

    /* Get the driver name */

    fread( &length, sizeof( length ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( length > 0 ) {

        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
        out_device->driver_name = string_ptr;
        out_device->next_offset += length;
        string_ptr[out_device->next_offset] = 0x00;
        ++out_device->next_offset;
    } else {
        out_device->driver_name = NULL;
    }

    /* Get the output file name */

    fread( &length, sizeof( length ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( length > 0 ) {
        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        out_device->output_name = string_ptr;
        out_device->next_offset += length;
        string_ptr[out_device->next_offset] = 0x00;
        ++out_device->next_offset;
    } else {
        out_device->output_name = NULL;
    }

    /* Get the output file extension */

    fread( &length, sizeof( length ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( length > 0 ) {
        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        out_device->output_extension = string_ptr;
        out_device->next_offset += length;
        string_ptr[out_device->next_offset] = 0x00;
        ++out_device->next_offset;
    } else {
        out_device->output_extension = NULL;
    }
    
    /* Get the numeric attributes, the "next codeblock" value, and the
     * PagegeometryBlock attributes 
     */

    /* The designator shows if the numeric attributes and the Pagegeometry
     * attributes are uint16_t or uint32_t (the "next codeblock" value is
     * always 16-bit)
     */

    fread( &designator, sizeof( designator ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    switch( designator ) {
    case 0x1200:

        /* Get the 16-bit Page Width */

        fread( &numeric_16, sizeof(numeric_16), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->page_width = numeric_16;

        /* Get the 16-bit Page Depth */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->page_depth = numeric_16;

        /* Get the 16-bit Horizontal Base Units */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->horizontal_base_units = numeric_16;

        /* Get the 16-bit Vertical Base Units */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->vertical_base_units = numeric_16;

        /* Get the "next codeblock" value */

        fread( &next_codeblock, sizeof( next_codeblock ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
        
        /* Get the 16-bit Page Start X Value */

        fread( &numeric_16, sizeof(numeric_16), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->x_start = numeric_16;

        /* Get the 16-bit Page Start Y Value */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->y_start = numeric_16;

        /* Get the 16-bit Page Offset X Value */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->x_offset = numeric_16;

        /* Get the 16-bit Page Offset Y Value */

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->y_offset = numeric_16;

        break;
    case 0x2200:

        /* Get the 32-bit Page Width */

        fread( &out_device->page_width, sizeof( out_device->page_width ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Page Depth */

        fread( &out_device->page_depth, sizeof( out_device->page_depth ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Horizontal Base Units */

        fread( &out_device->horizontal_base_units, sizeof( out_device->horizontal_base_units ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Vertical Base Units */

        fread( &out_device->vertical_base_units, sizeof( out_device->vertical_base_units ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the "next codeblock" value */

        fread( &next_codeblock, sizeof( next_codeblock ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
        
        /* Get the 32-bit Page Start X Value */

        fread( &out_device->x_start, sizeof( out_device->x_start ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Page Start Y Value */

        fread( &out_device->y_start, sizeof( out_device->y_start ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Page Offset X Value */

        fread( &out_device->x_offset, sizeof( out_device->x_offset ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the 32-bit Page Offset Y Value */

        fread( &out_device->y_offset, sizeof( out_device->y_offset ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        break;
    default:
        printf_s( "Bad numeric parameters designator: %i\n", designator );
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the BoxBlock values */

    /* Get the FontAttribute, which can be a string or a number */

    fread( &designator, sizeof( designator ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    switch( designator ) {
    case 0x0101:

        /* The Font is numeric */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        fread( &out_device->box.font_number, sizeof( out_device->box.font_number ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->box.font_name = NULL;
        break;
    case 0x0201:

        /* The Font is a character string */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( length > 0 ) {
            if( out_device->allocated_size < (out_device->next_offset + length) ) {
                out_device = resize_device( out_device, length );
                if( out_device == NULL ) return( out_device );
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
    
            out_device->box.font_name = string_ptr;
            out_device->next_offset += length;
            string_ptr[out_device->next_offset] = 0x00;
            ++out_device->next_offset;
        } else {
            out_device->box.font_name = NULL;
        }
    
        out_device->box.font_number = 0;
        break;
    default:
        printf_s( "Bad font type designator: %i\n", designator );
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the count field and verify that it contains 0x0F */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x0F ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    /* There are 0x0F bytes in the file but only 11 values */

    fread( &out_device->box.top_line, sizeof( out_device->box.top_line ), 11, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Skip the 4 unused bytes */

    fseek( in_file, 4, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    /* Get the UnderscoreBlock values */

    /* Get the FontAttribute, which can be a string or a number */

    fread( &designator, sizeof( designator ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    switch( designator ) {
    case 0x0101:

        /* The Font is numeric */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        fread( &out_device->box.font_number, sizeof( out_device->box.font_number ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->box.font_name = NULL;
        break;
    case 0x0201:

        /* The Font is a character string */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( length > 0 ) {
            if( out_device->allocated_size < (out_device->next_offset + length) ) {
                out_device = resize_device( out_device, length );
                if( out_device == NULL ) return( out_device );
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
    
            out_device->box.font_name = string_ptr;
            out_device->next_offset += length;
            string_ptr[out_device->next_offset] = 0x00;
            ++out_device->next_offset;
        } else {
            out_device->box.font_name = NULL;
        }
    
        out_device->box.font_number = 0;
        break;
    default:
        printf_s( "Bad font type designator: %i\n", designator );
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the count byte and verify that it contains 0x05 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x05 ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    /* There are 0x05 bytes in the file but only one value */

    fread( &out_device->underscore.underscore_char, sizeof( out_device->underscore.underscore_char ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Skip the 4 unused bytes */

    fseek( in_file, 4, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the TranslationBlock */

    /* Get the count and verify that it containss 0x03 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x03 ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    /* Now get the data_count and flags */

    fread( &data_count, sizeof( data_count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    fread( &outtrans_flag, sizeof( outtrans_flag ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    fread( &intrans_flag, sizeof( intrans_flag ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the IntransBlock, if present */

    if(intrans_flag == 0) {
        out_device->intrans = NULL;
    } else {

        /* Verify that the next byte is 0x81 */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        if( count8 != 0x81 ) {
           printf_s( "Incorrect Intrans Block designator: %i\n", count8 );
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Get the count byte and verify that it contains 0x00 */
        
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

            if( count8 != 0x00 ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        /* Initialize the pointer and reset next_offset */

        if( out_device->allocated_size < (out_device->next_offset + sizeof( out_device->intrans->table )) ) {
            out_device = resize_device( out_device, sizeof( out_device->intrans->table ) );
            if( out_device == NULL ) return( out_device );
        }

        out_device->intrans = (intrans_block *) ((char *) out_device + out_device->next_offset);
        out_device->next_offset += sizeof( out_device->intrans->table );

        /* Get the data into the array */

        fread( out_device->intrans->table, sizeof( out_device->intrans->table ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }
    }  

    /* Get the OuttransBlock, if present */

    if(outtrans_flag == 0) {
        out_device->outtrans = NULL;
    } else {

        /* Verify that the next byte is 0x82 */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        if( count8 != 0x82 ) {
           printf_s( "Incorrect Outtrans Block designator: %i\n", count8 );
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Read the count byte, and verify that it is equal to data_count */
        
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        if( count8 != data_count ) {
           printf_s( "Incorrect Outtrans Block data_count: %i instead of %i\n", data_count, count8 );
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Initialize the pointer and reset next_offset */

        if( out_device->allocated_size < (out_device->next_offset + sizeof( out_device->outtrans->table )) ) {
            out_device = resize_device( out_device, sizeof( out_device->outtrans->table ) );
            if( out_device == NULL ) return( out_device );
        }

        out_device->outtrans = (outtrans_block *) ((char *) out_device + out_device->next_offset);
        out_device->next_offset += sizeof( out_device->outtrans->table );

        /* Get the outtrans array into the local array */

        fread( &outtrans_array, sizeof( outtrans_array ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Allocate a buffer and read the translation characters into it */

        outtrans_data = (uint8_t *) malloc(data_count);

        fread( outtrans_data, sizeof( *outtrans_data ), data_count, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Convert the data to our format */

        for( i = 0; i < 0x100; i++ ) {

            /* If the first byte matches the index, there is no translation */

            if( outtrans_array[i] == i) {
                out_device->outtrans->table[i] = NULL;
            } else {

                /* Reserve space for the translation and adjust next_offset */

                if( out_device->allocated_size < (out_device->next_offset + sizeof( translation )) ) {
                    out_device = resize_device( out_device, sizeof( translation ) );
                    if( out_device == NULL ) return( out_device );
                }

                out_device->outtrans->table[i] = (translation *) ((char *) out_device + out_device->next_offset);
                out_device->next_offset += sizeof( translation );

                /* Get the translation for the current character */

                translation_start = outtrans_data + (outtrans_array[i] & 0x00ff);        
                out_device->outtrans->table[i]->count = *translation_start;
                ++translation_start;

                if( out_device->allocated_size < (out_device->next_offset + out_device->outtrans->table[i]->count) ) {
                    out_device = resize_device( out_device, out_device->outtrans->table[i]->count );
                    if( out_device == NULL ) return( out_device );
                }

                out_device->outtrans->table[i]->data = (uint8_t *) out_device + out_device->next_offset;
                out_device->next_offset += out_device->outtrans->table[i]->count;

                memcpy_s(out_device->outtrans->table[i]->data, out_device->outtrans->table[i]->count, translation_start, out_device->outtrans->table[i]->count );
            }
        }
    }  

    return( out_device );
}

/*  Function resize_device().
 *  Resizes a cop_device instance.
 *
 *  Parameters:
 *      in_device is a pointer to the cop_device to be resized
 *      in_size is the minimum acceptable increase in size
 *
 *  Warning:
 *      The memory pointed to by in_device will be freed whether the function
 *      succeeds or fails. The intended use is for the pointer passed as
 *      in_device to be used to store the return value. 
 *
 *  Returns:
 *      A pointer to a cop_device instance at least in_size larger with the same
 *          data (except for the allocated_size field, which reflects the new size)
 *          on success.
 *      A NULL pointer on failure.
 */

cop_device * resize_device( cop_device * in_device, size_t in_size )
{
    cop_device *    local_device = NULL;
    size_t          increment = INC_SIZE;
    size_t          new_size;
    size_t          scale;

    /* Compute how much larger to make the cop_device struct */
    if( in_size > INC_SIZE ) {
        scale = in_size % INC_SIZE;
        ++scale;
        increment = scale * INC_SIZE;
    }
    new_size = in_device->allocated_size + increment;

    /* Allocate a cop_device of the new size */
    local_device = (cop_device *) malloc( new_size );
    if( local_device == NULL) return( local_device );

    /* Copy the data, record the new size, free the old memory */    
    memcpy_s( local_device, new_size, in_device, in_device->next_offset );
    local_device->allocated_size = new_size;
    free( in_device );

    return( local_device );
}
        
