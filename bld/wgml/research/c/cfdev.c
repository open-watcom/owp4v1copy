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
*               and contains these local functions:
*                   find_cumulative_index()
*                   resize_cop_device()
*                   set_cumulative_index()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1
#include <stdlib.h>
#include <string.h>
#include "cfdev.h"
#include "cffunc.h"
#include "common.h"

/*  Local macros */

#define START_SIZE 2048
#define INC_SIZE   1024

/*  Local function declaration */

static int find_cumulative_index( functions_block *, uint16_t, uint8_t * );
static cop_device * resize_cop_device( cop_device *, size_t );
static void set_cumulative_index( functions_block * );

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
    
    cop_device *        out_device          = NULL;

    /* Used to acquire string attributes */

    uint8_t             length;
    char *              string_ptr          = NULL;

    /* Used to acquire numeric attributes */

    uint16_t            designator;
    uint16_t            numeric_16;

    /* Used to acquire the TranslationBlock */

    uint8_t *           byte_ptr            = NULL;
    uint8_t             data_count;
    uint16_t            i;
    uint8_t             intrans_flag;
    uint8_t *           outtrans_data       = NULL;
    uint8_t             outtrans_data_size;
    uint8_t             outtrans_flag;
    outtrans_block *    outtrans_ptr        = NULL;
    translation *       translation_ptr     = NULL;
    uint8_t *           translation_start   = NULL;
    uint8_t             uint8_array[0x100];    
    uint16_t            uint16_array[0x100];    

    /* Used to acquire the DefaultFonts and DeviceFonts */

    default_font *      defaultfont_ptr     = NULL;
    device_font  *      devicefont_ptr      = NULL;

    /* Used to acquire the PauseBlock and DevicefontBlock CodeBlocks */

    functions_block *   cop_functions       = NULL;
    p_buffer *          raw_functions       = NULL;
    uint16_t            cumulative_index;
    uint8_t *           current;
    uint8_t             j;
    uint8_t             nulls[2];
    int                 return_value;

    /* Used for count and other values */

    uint8_t             count8;

    /* Used for the next_codeblock value */

    uint16_t            next_codeblock;

    /* Initialize the out_device instance */
        
    out_device = (cop_device *) malloc( START_SIZE );
    if( out_device == NULL ) return( out_device );

    out_device->allocated_size = START_SIZE;
    out_device->next_offset = sizeof( cop_device );

    /* Note: The various pointers must be entered, initially, as offsets
     * and then converted to pointers before returning because out_device
     * may be reallocated at any point and that invalidates actual pointers.
     */

    /* Get the driver name */

    fread( &length, sizeof( length ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( length > 0 ) {

        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_cop_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
        out_device->driver_name = (char *) out_device->next_offset;
        out_device->next_offset += length;
        string_ptr[length] = 0x00;
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
            out_device = resize_cop_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        out_device->output_name = (char *) out_device->next_offset;
        out_device->next_offset += length;
        string_ptr[length] = 0x00;
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
            out_device = resize_cop_device( out_device, length );
            if( out_device == NULL ) return( out_device );
        }

        string_ptr = (char *) out_device + out_device->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        out_device->output_extension = (char *) out_device->next_offset;
        out_device->next_offset += length;
        string_ptr[length] = 0x00;
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

        fread( &numeric_16, sizeof( numeric_16 ), 1, in_file );
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
                out_device = resize_cop_device( out_device, length );
                if( out_device == NULL ) return( out_device );
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
    
            out_device->box.font_name = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
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
        printf_s( "Bad count field for BoxBlock: %i\n", count8 );
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    /* There are 0x0F bytes in the file but only 11 values */

    fread( &out_device->box.horizontal_line, sizeof( out_device->box.horizontal_line ), 11, in_file );
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

        fread( &out_device->underscore.font_number, sizeof( out_device->underscore.font_number ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->underscore.font_name = NULL;
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
                out_device = resize_cop_device( out_device, length );
                if( out_device == NULL ) return( out_device );
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
    
            out_device->underscore.font_name = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
            ++out_device->next_offset;
        } else {
            out_device->underscore.font_name = NULL;
        }
    
        out_device->underscore.font_number = 0;
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
        printf_s( "Bad count field for UnderscoreBlock: %i\n", count8 );
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
        printf_s( "Bad count field for TranslationBlock: %i\n", count8 );
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
           printf_s( "Incorrect IntransBlock designator: %i\n", count8 );
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
            printf_s( "Incorrect IntransBlock count: %i\n", count8 );
            free( out_device );
            out_device = NULL;
            return( out_device );
        }
    
        /* Get the data into the array */

        if( out_device->allocated_size < (out_device->next_offset + sizeof( out_device->intrans->table )) ) {
            out_device = resize_cop_device( out_device, sizeof( out_device->intrans->table ) );
            if( out_device == NULL ) return( out_device );
        }

        byte_ptr = (uint8_t *) out_device + out_device->next_offset;
        
        fread( byte_ptr, sizeof( out_device->intrans->table ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        out_device->intrans = (intrans_block *) out_device->next_offset;
        out_device->next_offset += sizeof( out_device->intrans->table );

    }  

    /* Get the OuttransBlock, if present */

    if(outtrans_flag == 0) {
        out_device->outtrans = NULL;
    } else {

        /* Get the next byte, which indicates the OuttransBlock data size */

        fread( &outtrans_data_size, sizeof( outtrans_data_size ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
           free( out_device );
           out_device = NULL;
           return( out_device );
        }

        /* Read the count byte */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* The file is positioned at the start of the data */

        switch( outtrans_data_size) {
        case 0x81:

            /* The count byte should be 0x00 */

            if( count8 != 0x00 ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            /* Get the data into the local buffer */

            fread( &uint8_array, sizeof( uint8_array ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
            
            /* Reserve space for the outtrans_block */

            if( out_device->allocated_size < (out_device->next_offset + sizeof( out_device->outtrans->table )) ) {
                out_device = resize_cop_device( out_device, sizeof( out_device->outtrans->table ) );
                if( out_device == NULL ) {
                    free( outtrans_data );
                    outtrans_data = NULL;
                    return( out_device );
                }
            }

            out_device->outtrans = (outtrans_block *) out_device->next_offset;
            out_device->next_offset += sizeof( out_device->outtrans->table );

            outtrans_ptr = (outtrans_block *) ((char *) out_device + (size_t) out_device->outtrans);

            /* Build the actual table, which requires actual pointers in
             * place of the offsets recorded in *out_device:
             *   outtrans_ptr is the pointer version of out_device->outtrans
             *   for each iteration,
             *     translation_ptr is the pointer version of outtrans->ptr->table[i]
             *     byte_ptr is the pointer version of translation->ptr.data
             */

            for( i = 0; i < 0x100; i++ ) {

                /* If the first byte matches the index, there is no translation */

                if( uint8_array[i] == i) {
                    outtrans_ptr->table[i] = NULL;
                } else {

                    /* Reserve space for the translation */

                    if( out_device->allocated_size < (out_device->next_offset + sizeof( translation )) ) {
                        out_device = resize_cop_device( out_device, sizeof( translation ) );
                        if( out_device == NULL ) {
                            free( outtrans_data );
                            outtrans_data = NULL;
                            return( out_device );
                        }
                        outtrans_ptr = (outtrans_block *) ((uint8_t *) out_device + (size_t) out_device->outtrans);
                    }

                    outtrans_ptr->table[i] = (translation *) out_device->next_offset;
                    out_device->next_offset += sizeof( translation );

                    /* Get the translation for the current character */

                    translation_ptr = (translation *) ((char *) out_device + (size_t) outtrans_ptr->table[i] );

                    /* The translation always contains exactly one character */
                    
                    translation_ptr->count = 1;

                    if( out_device->allocated_size < (out_device->next_offset + translation_ptr->count ) ) {
                        out_device = resize_cop_device( out_device, translation_ptr->count  );
                        if( out_device == NULL ) {
                            free( outtrans_data );
                            outtrans_data = NULL;
                            return( out_device );
                        }
                        outtrans_ptr = (outtrans_block *) ((uint8_t *) out_device + (size_t) out_device->outtrans);
                        translation_ptr = (translation *) ((uint8_t *) out_device + (size_t) outtrans_ptr->table[i] );
                    }

                    translation_ptr->data = (uint8_t *) out_device->next_offset;
                    out_device->next_offset += translation_ptr->count;

                    byte_ptr = (uint8_t *) out_device + (size_t) translation_ptr->data;

                    /* The translation character is the value in the input array */
                    
                    *byte_ptr = uint8_array[i];
                }
            }
            break;
        case 0x82:

            /* The count byte should be equal to data_count */
        
            if( count8 != data_count ) {
                printf_s( "Incorrect OuttransBlock data_count: %i instead of %i\n", data_count, count8 );
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            /* Get the outtrans array into the local array */

            fread( &uint16_array, sizeof( uint16_array ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            /* Allocate a buffer and read the translation characters into it */

            outtrans_data = (uint8_t *) malloc( data_count );

            fread( outtrans_data, sizeof( *outtrans_data ), data_count, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( outtrans_data );
                outtrans_data = NULL;
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            /* Initialize outtrans_ptr and the outtrans pointer in out_device */

            if( out_device->allocated_size < (out_device->next_offset + sizeof( out_device->outtrans->table )) ) {
                out_device = resize_cop_device( out_device, sizeof( out_device->outtrans->table ) );
                if( out_device == NULL ) {
                    free( outtrans_data );
                    outtrans_data = NULL;
                    return( out_device );
                }
            }

            out_device->outtrans = (outtrans_block *) out_device->next_offset;
            out_device->next_offset += sizeof( out_device->outtrans->table );

            outtrans_ptr = (outtrans_block *) ((char *) out_device + (size_t) out_device->outtrans);

            /* Convert the data in uint16_array to our format, which requires
             * actual pointers in place of the offsets recorded in *out_device:
             *   outtrans_ptr is the pointer version of out_device->outtrans
             *   for each iteration,
             *     translation_ptr is the pointer version of outtrans->ptr->table[i]
             *     byte_ptr is the pointer version of translation->ptr.data
             */

            for( i = 0; i < 0x100; i++ ) {

                /* If the first byte matches the index, there is no translation */

                if( uint16_array[i] == i) {
                    outtrans_ptr->table[i] = NULL;
                } else {

                    /* Reserve space for the translation */

                    if( out_device->allocated_size < (out_device->next_offset + sizeof( translation )) ) {
                        out_device = resize_cop_device( out_device, sizeof( translation ) );
                        if( out_device == NULL ) {
                            free( outtrans_data );
                            outtrans_data = NULL;
                            return( out_device );
                        }
                        outtrans_ptr = (outtrans_block *) ((uint8_t *) out_device + (size_t) out_device->outtrans);
                    }

                    outtrans_ptr->table[i] = (translation *) out_device->next_offset;
                    out_device->next_offset += sizeof( translation );

                    /* Get the translation for the current character */

                    translation_ptr = (translation *) ((char *) out_device + (size_t) outtrans_ptr->table[i] );

                    translation_start = outtrans_data + (uint16_array[i] & 0x00ff);        
                    translation_ptr->count = *translation_start;
                    ++translation_start;

                    if( out_device->allocated_size < (out_device->next_offset + translation_ptr->count ) ) {
                        out_device = resize_cop_device( out_device, translation_ptr->count  );
                        if( out_device == NULL ) {
                            free( outtrans_data );
                            outtrans_data = NULL;
                            return( out_device );
                        }
                        outtrans_ptr = (outtrans_block *) ((uint8_t *) out_device + (size_t) out_device->outtrans);
                        translation_ptr = (translation *) ((uint8_t *) out_device + (size_t) outtrans_ptr->table[i] );
                    }

                    translation_ptr->data = (uint8_t *) out_device->next_offset;
                    out_device->next_offset += translation_ptr->count;

                    byte_ptr = (uint8_t *) out_device + (size_t) translation_ptr->data;
                    memcpy_s(byte_ptr, translation_ptr->count, translation_start, translation_ptr->count );
                }
            }
            break;
        default:
           printf_s( "Incorrect OuttransBlock designator: %i\n", count8 );
           free( out_device );
           out_device = NULL;
           return( out_device );
        }
        free( outtrans_data );
        outtrans_data = NULL;
    }  

    /* Get the DefaultfontBlock */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect DefaultfontBlock designator: %i\n", count8 );
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the number of DefaultFonts */
    
    fread( &out_device->defaultfonts.count, sizeof( out_device->defaultfonts.count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Set defaultfont_ptr and defaultfonts.font and adjust next_offset */
    
    if( out_device->allocated_size < (out_device->next_offset + sizeof( *out_device->defaultfonts.font) * out_device->defaultfonts.count) ) {
        out_device = resize_cop_device( out_device, sizeof( *out_device->defaultfonts.font) * out_device->defaultfonts.count );
        if( out_device == NULL ) return( out_device );
   }

    out_device->defaultfonts.font = (default_font *) out_device->next_offset;
    out_device->next_offset += sizeof( *out_device->defaultfonts.font) * out_device->defaultfonts.count;

    defaultfont_ptr = (default_font *) ((uint8_t *) out_device + (size_t) out_device->defaultfonts.font);

    /* Get the DefaultFonts */

    for( i = 0; i < out_device->defaultfonts.count; i++ ){

        /* Get the Font Style */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_cop_device( out_device, length );
            if( out_device == NULL ) return( out_device );
            defaultfont_ptr = (default_font *) ((uint8_t *) out_device + (size_t) out_device->defaultfonts.font);
        }

        if( length > 0 ) {
            string_ptr = (char *) out_device + out_device->next_offset;
            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            defaultfont_ptr[i].font_style = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
            ++out_device->next_offset;
        } else {
            defaultfont_ptr[i].font_style = NULL;
        }
        
        /* Get the count and verify that it is 0x04 */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( count8 != 0x04 ) {
            printf_s( "Incorrect DefaultFont count: %i\n", count8 );
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the Font Height */

        fread( &defaultfont_ptr[i].font_height, sizeof( defaultfont_ptr[i].font_height ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the Font Space */

        fread( &defaultfont_ptr[i].font_space, sizeof( defaultfont_ptr[i].font_space ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the Font Name */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( out_device->allocated_size < (out_device->next_offset + length) ) {
            out_device = resize_cop_device( out_device, length );
            if( out_device == NULL ) return( out_device );
            defaultfont_ptr = (default_font *) ((uint8_t *) out_device + (size_t) out_device->defaultfonts.font);
        }

        if( length > 0 ) {
            string_ptr = (char *) out_device + out_device->next_offset;
            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            defaultfont_ptr[i].font_name = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
            ++out_device->next_offset;
        } else {
            defaultfont_ptr[i].font_style = NULL;
        }
    }

    /* Now get the FunctionsBlock and position in_file to the start of 
     * the PausesBlock. This must be done even if no functions are present.
     */

    raw_functions = get_p_buffer( in_file );
    if( raw_functions == NULL) {
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( raw_functions->buffer == NULL) {
        free( raw_functions );
        raw_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    current = raw_functions->buffer;
    cop_functions = parse_functions_block( &current );

    if( cop_functions == NULL) {
        free( raw_functions );
        raw_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }
    
    if( cop_functions->count > 0) set_cumulative_index( cop_functions );

    /* Get the PauseBlock */

    /* Get the START Pause */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect START Pause count: %i\n", count8 );
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the value to use to find the CodeBlock */

    fread( &cumulative_index, sizeof( cumulative_index ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* The value 0xFFFF indicates that no such pause exists */

    if( cumulative_index == 0xFFFF ) {
        out_device->pauses.startpause_count = 0;
        out_device->pauses.startpause = NULL;
    } else {
        return_value = find_cumulative_index( cop_functions, cumulative_index, &j );
        if( return_value == FAILURE ) {
            puts( "START Pause CodeBlock not found!" );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->pauses.startpause_count = cop_functions->code_blocks[j].count;

        if( out_device->allocated_size < (out_device->next_offset + out_device->pauses.startpause_count) ) {
            out_device = resize_cop_device( out_device, out_device->pauses.startpause_count );
            if( out_device == NULL ) {
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_device );
            }
        }

        out_device->pauses.startpause = (uint8_t *) out_device->next_offset;
        out_device->next_offset += out_device->pauses.startpause_count;

        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.startpause;
        memcpy_s(byte_ptr, out_device->pauses.startpause_count, cop_functions->code_blocks[j].function, out_device->pauses.startpause_count );
    }
    
    /* Get the DOCUMENT Pause */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect DOCUMENT Pause count: %i\n", count8 );
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the value to use to find the CodeBlock */

    fread( &cumulative_index, sizeof( cumulative_index ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* The value 0xFFFF indicates that no such pause exists */

    if( cumulative_index == 0xFFFF ) {
        out_device->pauses.documentpause_count = 0;
        out_device->pauses.documentpause = NULL;
    } else {
        return_value = find_cumulative_index( cop_functions, cumulative_index, &j );
        if( return_value == FAILURE ) {
            puts( "DOCUMENT Pause CodeBlock not found!" );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->pauses.documentpause_count = cop_functions->code_blocks[j].count;

        if( out_device->allocated_size < (out_device->next_offset + out_device->pauses.documentpause_count) ) {
            out_device = resize_cop_device( out_device, out_device->pauses.documentpause_count );
            if( out_device == NULL ) {
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_device );
            }
        }

        out_device->pauses.documentpause = (uint8_t *) out_device->next_offset;
        out_device->next_offset += out_device->pauses.documentpause_count;

        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.documentpause;
        memcpy_s(byte_ptr, out_device->pauses.documentpause_count, cop_functions->code_blocks[j].function, out_device->pauses.documentpause_count );
    }
    
    /* Get the DOCUMENT_PAGE Pause */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect DOCUMENT_PAGE Pause count: %i\n", count8 );
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the value to use to find the CodeBlock */

    fread( &cumulative_index, sizeof( cumulative_index ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* The value 0xFFFF indicates that no such pause exists */

    if( cumulative_index == 0xFFFF ) {
        out_device->pauses.docpagepause_count = 0;
        out_device->pauses.docpagepause = NULL;
    } else {
        return_value = find_cumulative_index( cop_functions, cumulative_index, &j );
        if( return_value == FAILURE ) {
            puts( "DOCUMENT_PAGE Pause CodeBlock not found!" );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->pauses.docpagepause_count = cop_functions->code_blocks[j].count;

        if( out_device->allocated_size < (out_device->next_offset + out_device->pauses.docpagepause_count) ) {
            out_device = resize_cop_device( out_device, out_device->pauses.docpagepause_count );
            if( out_device == NULL ) {
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_device );
            }
        }

        out_device->pauses.docpagepause = (uint8_t *) out_device->next_offset;
        out_device->next_offset += out_device->pauses.docpagepause_count;

        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.docpagepause;
        memcpy_s(byte_ptr, out_device->pauses.docpagepause_count, cop_functions->code_blocks[j].function, out_device->pauses.docpagepause_count );
    }
    
    /* Get the DEVICE_PAGE Pause */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect DEVICE_PAGE Pause count: %i\n", count8 );
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the value to use to find the CodeBlock */

    fread( &cumulative_index, sizeof( cumulative_index ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* The value 0xFFFF indicates that no such pause exists */

    if( cumulative_index == 0xFFFF ) {
        out_device->pauses.devpagepause_count = 0;
        out_device->pauses.devpagepause = NULL;
    } else {
        return_value = find_cumulative_index( cop_functions, cumulative_index, &j );
        if( return_value == FAILURE ) {
            puts( "DEVICE_PAGE Pause CodeBlock not found!" );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        out_device->pauses.devpagepause_count = cop_functions->code_blocks[j].count;

        if( out_device->allocated_size < (out_device->next_offset + out_device->pauses.devpagepause_count) ) {
            out_device = resize_cop_device( out_device, out_device->pauses.devpagepause_count );
            if( out_device == NULL ) {
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_device );
            }
        }

        out_device->pauses.devpagepause = (uint8_t *) out_device->next_offset;
        out_device->next_offset += out_device->pauses.devpagepause_count;

        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.devpagepause;
        memcpy_s(byte_ptr, out_device->pauses.devpagepause_count, cop_functions->code_blocks[j].function, out_device->pauses.devpagepause_count );
    }

    /* Get the DevicefontsBlock */

    /* Get the count and verify that it is 0x02 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    if( count8 != 0x02 ) {
        printf_s( "Incorrect Device Font count: %i\n", count8 );
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the number of Devicefonts */

    fread( &out_device->devicefonts.count, sizeof( out_device->devicefonts.count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( raw_functions );
        raw_functions = NULL;
        free( cop_functions->code_blocks );
        cop_functions->code_blocks = NULL;
        free( cop_functions );
        cop_functions = NULL;
        free( out_device );
        out_device = NULL;
        return( out_device );
    }

    /* Get the Devicefonts */

    if( out_device->allocated_size < (out_device->next_offset + out_device->devicefonts.count * sizeof( *out_device->devicefonts.font ) ) ) {
        out_device = resize_cop_device( out_device, out_device->devicefonts.count * sizeof( *out_device->devicefonts.font ) );
        if( out_device == NULL ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            return( out_device );
        }
    }

    out_device->devicefonts.font = (device_font *) out_device->next_offset;
    out_device->next_offset += out_device->devicefonts.count * sizeof( *out_device->devicefonts.font );
    
    devicefont_ptr = (device_font *) ((uint8_t *) out_device + (size_t) out_device->devicefonts.font);

    for( i = 0; i < out_device->devicefonts.count; i++ ) {

        /* Get the font_name */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( length > 0 ) {

            if( out_device->allocated_size < (out_device->next_offset + length) ) {
                out_device = resize_cop_device( out_device, length );
                if( out_device == NULL ) {
                    free( raw_functions );
                    raw_functions = NULL;
                    free( cop_functions->code_blocks );
                    cop_functions->code_blocks = NULL;
                    free( cop_functions );
                    cop_functions = NULL;
                    return( out_device );
                }
                devicefont_ptr = (device_font *) ((uint8_t *) out_device + (size_t) out_device->devicefonts.font);
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                free( out_device );
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
            devicefont_ptr[i].font_name = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
            ++out_device->next_offset;
        } else {
            devicefont_ptr[i].font_name = NULL;
        }

        /* Get the font_switch */

        fread( &length, sizeof( length ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( length > 0 ) {

            if( out_device->allocated_size < (out_device->next_offset + length) ) {
                out_device = resize_cop_device( out_device, length );
                if( out_device == NULL ) {
                    free( raw_functions );
                    raw_functions = NULL;
                    free( cop_functions->code_blocks );
                    cop_functions->code_blocks = NULL;
                    free( cop_functions );
                    cop_functions = NULL;
                    return( out_device );
                }
                devicefont_ptr = (device_font *) ((uint8_t *) out_device + (size_t) out_device->devicefonts.font);
            }

            string_ptr = (char *) out_device + out_device->next_offset;

            fread( string_ptr, length, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                free( out_device );
                out_device = NULL;
                return( out_device );
            }
            devicefont_ptr[i].font_switch = (char *) out_device->next_offset;
            out_device->next_offset += length;
            string_ptr[length] = 0x00;
            ++out_device->next_offset;
        } else {
            devicefont_ptr[i].font_switch = NULL;
        }

        /* Get the nulls and verify that they are, in fact, nulls */

        fread( &nulls, sizeof( nulls ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        if( memcmp( nulls, "\0\0", 2 ) ) {
            printf_s( "DeviceFont %i has this for the nulls: %i\n", i, nulls );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the count and verify that it is 0x03 */

        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }   

        if( count8 != 0x03 ) {
            printf_s( "Incorrect Device Font count: %i\n", count8 );
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the resident font indicator */

        fread( &devicefont_ptr[i].resident, sizeof( devicefont_ptr[i].resident ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* Get the fontpause */
        /* Get the value to use to find the CodeBlock */
 
        fread( &cumulative_index, sizeof( cumulative_index ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( raw_functions );
            raw_functions = NULL;
            free( cop_functions->code_blocks );
            cop_functions->code_blocks = NULL;
            free( cop_functions );
            cop_functions = NULL;
            free( out_device );
            out_device = NULL;
            return( out_device );
        }

        /* The value 0xFFFF indicates that no such pause exists */

        if( cumulative_index == 0xFFFF ) {
            devicefont_ptr[i].fontpause_count = 0;
            devicefont_ptr[i].fontpause = NULL;
        } else {
            return_value = find_cumulative_index( cop_functions, cumulative_index, &j );
            if( return_value == FAILURE ) {
                puts( "Device Font %i Font Pause CodeBlock not found!" );
                free( raw_functions );
                raw_functions = NULL;
                free( cop_functions->code_blocks );
                cop_functions->code_blocks = NULL;
                free( cop_functions );
                cop_functions = NULL;
                free( out_device );
                out_device = NULL;
                return( out_device );
            }

            devicefont_ptr[i].fontpause_count = cop_functions->code_blocks[j].count;

            if( out_device->allocated_size < (out_device->next_offset + devicefont_ptr[i].fontpause_count) ) {
                out_device = resize_cop_device( out_device, devicefont_ptr[i].fontpause_count );
                if( out_device == NULL ) {
                    free( raw_functions );
                    raw_functions = NULL;
                    free( cop_functions->code_blocks );
                    cop_functions->code_blocks = NULL;
                    free( cop_functions );
                    cop_functions = NULL;
                    return( out_device );
                }
                devicefont_ptr = (device_font *) ((uint8_t *) out_device + (size_t) out_device->devicefonts.font);
            }

            devicefont_ptr[i].fontpause = (uint8_t *) out_device->next_offset;
            out_device->next_offset += devicefont_ptr[i].fontpause_count;

            byte_ptr = (uint8_t *) out_device + (size_t) devicefont_ptr[i].fontpause;
            memcpy_s(byte_ptr, devicefont_ptr[i].fontpause_count, cop_functions->code_blocks[j].function, devicefont_ptr[i].fontpause_count );
        }
    }

    /*  Free the memory used in parsing the CodeBlocks */

    free( raw_functions );
    raw_functions = NULL;
    free( cop_functions->code_blocks );
    cop_functions->code_blocks = NULL;
    free( cop_functions );
    cop_functions = NULL;

    /* Convert non-NULL offsets to pointers */

    if( out_device->driver_name != NULL ) {
        string_ptr = (char *) out_device + (size_t) out_device->driver_name;
        out_device->driver_name = string_ptr;
    }
    
    if( out_device->output_name != NULL ) {
        string_ptr = (char *) out_device + (size_t) out_device->output_name;
        out_device->output_name = string_ptr;
    }
    
    if( out_device->output_extension != NULL ) {
        string_ptr = (char *) out_device + (size_t) out_device->output_extension;
        out_device->output_extension = string_ptr;
    }
    
    if( out_device->box.font_name  != NULL ) {
        string_ptr = (char *) out_device + (size_t) out_device->box.font_name;
        out_device->box.font_name = string_ptr;
    }
    
    if( out_device->underscore.font_name != NULL ) {
        string_ptr = (char *) out_device + (size_t) out_device->underscore.font_name;
        out_device->underscore.font_name = string_ptr;
    }
    
    if( out_device->intrans != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->intrans;
        out_device->intrans = (intrans_block *) byte_ptr;
    }
    
    if( out_device->outtrans != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->outtrans;
        out_device->outtrans = (outtrans_block *) byte_ptr;
    
        for( i = 0; i < sizeof( outtrans_block ) / sizeof( translation * ); i++ ) {
            if( out_device->outtrans->table[i] != NULL ) {
                byte_ptr = (uint8_t *) out_device + (size_t) out_device->outtrans->table[i];
                out_device->outtrans->table[i] = (translation *) byte_ptr;
                if( out_device->outtrans->table[i]->data != NULL ) {
                    byte_ptr = (uint8_t *) out_device + (size_t) out_device->outtrans->table[i]->data;
                    out_device->outtrans->table[i]->data = byte_ptr;
                }
            }
        }
    }
    
    if( out_device->defaultfonts.font != NULL ) {
       byte_ptr = (uint8_t *) out_device + (size_t) out_device->defaultfonts.font;
       out_device->defaultfonts.font = (default_font *) byte_ptr;
        for( i = 0; i < out_device->defaultfonts.count; i++ ) {
            if( out_device->defaultfonts.font[i].font_name != NULL ) {
                string_ptr = (char *) out_device + (size_t) out_device->defaultfonts.font[i].font_name;
                out_device->defaultfonts.font[i].font_name = string_ptr;
            }
            if( out_device->defaultfonts.font[i].font_style != NULL ) {
                string_ptr = (char *) out_device + (size_t) out_device->defaultfonts.font[i].font_style;
                out_device->defaultfonts.font[i].font_style = string_ptr;
            }
        }
    }
    
    if( out_device->pauses.startpause != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.startpause;
        out_device->pauses.startpause = byte_ptr;
    }
    
    if( out_device->pauses.documentpause != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.documentpause;
        out_device->pauses.documentpause = byte_ptr;
    }
    
    if( out_device->pauses.docpagepause != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.docpagepause;
        out_device->pauses.docpagepause = byte_ptr;
    }
    
    if( out_device->pauses.devpagepause != NULL ) {
        byte_ptr = (uint8_t *) out_device + (size_t) out_device->pauses.devpagepause;
        out_device->pauses.devpagepause = byte_ptr;
    }
    
    if( out_device->devicefonts.font != NULL ) {
       byte_ptr = (uint8_t *) out_device + (size_t) out_device->devicefonts.font;
       out_device->devicefonts.font = (device_font *) byte_ptr;
        for( i = 0; i < out_device->devicefonts.count; i++ ) {
            if( out_device->devicefonts.font[i].font_name != NULL ) {
                string_ptr = (char *) out_device + (size_t) out_device->devicefonts.font[i].font_name;
                out_device->devicefonts.font[i].font_name = string_ptr;
            }
            if( out_device->devicefonts.font[i].font_switch != NULL ) {
                string_ptr = (char *) out_device + (size_t) out_device->devicefonts.font[i].font_switch;
                out_device->devicefonts.font[i].font_switch= string_ptr;
            }
            if( out_device->devicefonts.font[i].fontpause != NULL ) {
                byte_ptr = (uint8_t *) out_device + (size_t) out_device->devicefonts.font[i].fontpause;
                out_device->devicefonts.font[i].fontpause = byte_ptr;
            }
        }
    }
    
    return( out_device );
}

/*  Local functions definitions */

/*  Function find_cumulative_index().
 *  Finds the index the cumulative_max value.
 *
 *  Parameter:
 *      in_block contains the functions_block to search
 *      in_max contains the value sought
 *      out_index will contain the corresponding index
 *
 *  Returns:
 *      SUCCESS if in_max is found in in_block
 *      FAILURE otherwise
 */

int find_cumulative_index( functions_block * in_block, uint16_t in_max, uint8_t * out_index )
{
    uint8_t i;

    for( i = 0; i < in_block->count; i++ ) {
        if( in_block->code_blocks[i].cumulative_index == in_max ) {
            *out_index = i;
            return(SUCCESS);
        }
    }

    return(FAILURE);
}

/*  Function resize_cop_device().
 *  Resizes a cop_device instance.
 *
 *  Parameters:
 *      in_device is a pointer to the cop_device to be resized
 *      in_size is the minimum acceptable increase in size
 *
 *  Warning:
 *      If realloc() returns a different value from in_device, then the
 *      memory pointed to by in_device will be freed whether the function
 *      succeeds or fails. The intended use is for the pointer passed as
 *      in_device to be used to store the return value. 
 *
 *  Returns:
 *      A pointer to a cop_device instance at least in_size larger with the same
 *          data (except for the allocated_size field, which reflects the new size)
 *          on success.
 *      A NULL pointer on failure.
 */

cop_device * resize_cop_device( cop_device * in_device, size_t in_size )
{
    cop_device *    local_device = NULL;
    size_t          increment = INC_SIZE;
    size_t          new_size;
    size_t          scale;

    /* Compute how much larger to make the cop_device struct */

    if( in_size > INC_SIZE ) {
        scale = in_size / INC_SIZE;
        ++scale;
        increment = scale * INC_SIZE;
    }
    new_size = in_device->allocated_size + increment;

    /* Reallocate the cop_device */

    local_device = (cop_device *) realloc( in_device, new_size );
    if( local_device != in_device ) free( in_device );
    if( local_device != NULL ) local_device->allocated_size = new_size;

    return( local_device );
}

/*  Function set_cumulative_index().
 *  Sets the cumulative_index field in each code_block of a functions_block.
 *
 *  Parameter:
 *      in_block contains a pointer to functions_block to index
 */

void set_cumulative_index( functions_block * in_block )
{
    uint8_t         i;

    in_block->code_blocks[0].cumulative_index = 0x0000;

    if( in_block->count > 1 ) {
        for( i = 1; i < in_block->count; i++ ) {
            in_block->code_blocks[i].cumulative_index = in_block->code_blocks[i-1].cumulative_index + in_block->code_blocks[i-1].count;
        }
    }

    return;
}


