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
* Description:  Implements the functions declared in cfdrv.h:
*                   is_drv_file()
*                   parse_driver()
*               and contains these local functions:
*                   parse_finish_block()
*                   parse_font_style()
*                   parse_init_block()
*                   resize_cop_driver()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfdrv.h"
#include "cffunc.h"

/*  Local macros */

#define START_SIZE 2048
#define INC_SIZE   1024

/*  Local function declarations */

static cop_driver * parse_finish_block( cop_driver *, uint8_t * * );
static cop_driver * parse_font_style( FILE *, cop_driver *, font_style *, p_buffer * *, uint8_t * *, uint8_t );
static cop_driver * parse_init_block( cop_driver *, uint8_t * * );
static cop_driver * resize_cop_driver( cop_driver *, size_t );

/*  Function definitions */

/*  Function is_drv_file().
 *  Determines whether or not in_file points to the start of a .COP driver
 *  file (the first byte after the header).
 *
 *  Parameter:
 *      in_file points to the presumed start of a .COP driver file
 *
 *  Returns:
 *      true if this has the correct designator
 *      false otherwise
 */

bool is_drv_file( FILE * in_file)
{
    char designator[3];

    /* Get the designator */

    fread( &designator, 3, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( false );

    /* Verify that the designator is for a .COP driver file */

    if( memcmp( designator, "DRV", 3 ) ) return( false );
    
    return( true );
}

/* Function parse_driver().
 *  Constructs a cop_driver instance from the given input stream.
 *  
 *  Parameters:
 *      in_file points to the first byte of a .COP file encoding a :DRIVER
 *          struct after the "DEV" descriminator.
 *
 *  Returns:
 *      A pointer to a cop_driver struct containing the data from in_file
 *          on success.
 *      A NULL pointer on failure.
 */

cop_driver * parse_driver( FILE * in_file )
{
    /* The out_driver instance */
    
    cop_driver *        out_driver              = NULL;

    /* Used to acquire string attributes */

    uint8_t             length;
    char *              string_ptr              = NULL;

    /* Used to acquire and work with the P-buffers */

    code_block *        cop_codeblocks          = NULL;
    code_text *         code_text_ptr           = NULL;
    font_style *        font_style_ptr          = NULL;
    fontswitch_block *  fontswitch_block_ptr    = NULL;
    functions_block *   cop_functions           = NULL;
    int                 factor;
    newline_block *     newline_block_ptr       = NULL;
    p_buffer *          p_buffer_set            = NULL;
    size_t              span;
    uint8_t *           byte_ptr                = NULL;
    uint8_t *           current                 = NULL;
    uint8_t *           text_ptr                = NULL;

    /* Used for counts and related values */

    uint8_t             count8;
    uint16_t            count16;

    /* Used in for loops */

    int                 i;
    int                 j;

    /* Initialize the out_driver instance */
        
    out_driver = (cop_driver *) malloc( START_SIZE );
    if( out_driver == NULL ) return( out_driver );

    out_driver->allocated_size = START_SIZE;
    out_driver->next_offset = sizeof( cop_driver );

    /* Note: The various pointers must be entered, initially, as offsets
     * and then converted to pointers before returning because out_driver
     * may be reallocated at any point and that invalidates actual pointers.
     */

    /* Get the record specification */

    fread( &length, sizeof( length ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( length > 0 ) {

        if( out_driver->allocated_size < (out_driver->next_offset + length) ) {
            out_driver = resize_cop_driver( out_driver, length );
            if( out_driver == NULL ) return( out_driver );
        }

        string_ptr = (char *) out_driver + out_driver->next_offset;

        fread( string_ptr, length, 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
        out_driver->rec_spec = (char *) out_driver->next_offset;
        out_driver->next_offset += length;
        string_ptr[length] = 0x00;
        ++out_driver->next_offset;
    } else {
        out_driver->rec_spec = NULL;
    }

    /* Get the count field and verify that it contains 0x04 */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( count8 != 0x04 ) {
        printf_s( "Bad count field for Attributes: %i\n", count8 );
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the fill character */

    fread( &out_driver->fill_char, sizeof( out_driver->fill_char ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the x_positive flag */

    fread( &out_driver->x_positive, sizeof( out_driver->x_positive ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the y_positive flag */

    fread( &out_driver->y_positive, sizeof( out_driver->y_positive ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the null byte and verify that it is, in fact, null */

    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( count8 != 0x00 ) {
        printf_s( "Byte after y_positive should be null, not %i\n", count8 );
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the first set of P-buffers */

    p_buffer_set = get_p_buffer( in_file );
    if( p_buffer_set == NULL) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( p_buffer_set->buffer == NULL) {
        free( p_buffer_set );
        p_buffer_set = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    current = p_buffer_set->buffer;

    /* Parse the Init Block */

    /* Get the number of :INIT blocks */

    memcpy_s( &count16, sizeof(count16), current, sizeof(count16) );
    current += sizeof(count16);

    out_driver->init.start_initblock = NULL;
    out_driver->init.document_initblock = NULL;

    switch( count16 ) {
    case 0x0000 :
        break;
    case 0x0001 :
        out_driver = parse_init_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }
        break;
    case 0x0002 :
        out_driver = parse_init_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }

        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }

        out_driver = parse_init_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }
        break;
    default:
        free( p_buffer_set );
        p_buffer_set = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }
    
    /* Parse the Finish Block */

    /* Get the number of :FINISH blocks */

    memcpy_s( &count16, sizeof(count16), current, sizeof(count16) );
    current += sizeof(count16);

    out_driver->finish.end_finishblock = NULL;
    out_driver->finish.document_finishblock = NULL;

    switch( count16 ) {
    case 0x0000 :
        break;
    case 0x0001 :
        out_driver = parse_finish_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }
        break;
    case 0x0002 :
        out_driver = parse_finish_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }

        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }

        out_driver = parse_finish_block( out_driver, &current );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }
        break;
    default:
        free( p_buffer_set );
        p_buffer_set = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }
    
    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Newline Block */

    /* Get the number of :NEWLINE blocks */

    memcpy_s( &out_driver->newline.count, sizeof(out_driver->newline.count), current, sizeof(out_driver->newline.count) );
    current += sizeof(out_driver->newline.count);

    /* Add the newline_block structs */

    if( out_driver->allocated_size < (out_driver->next_offset + out_driver->newline.count * sizeof( newline_block )) ) {
        out_driver = resize_cop_driver( out_driver, out_driver->newline.count * sizeof( newline_block ) );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            return( out_driver );
        }
    }
    newline_block_ptr = (newline_block *) ((uint8_t *) out_driver + out_driver->next_offset);

    out_driver->newline.newlineblock = (newline_block *) out_driver->next_offset;
    out_driver->next_offset += out_driver->newline.count * sizeof( newline_block );

    /* Initialize the newline_block structs */

    for(i = 0; i < out_driver->newline.count; i++ ) {

        /* Get the advance for the current :NEWLINE block */

        memcpy_s( &newline_block_ptr[i].advance, sizeof(newline_block_ptr[i].advance), current, sizeof(newline_block_ptr[i].advance) );
        current += sizeof(newline_block_ptr[i].advance);

        /* Get the number of CodeBlocks, and verify that it is 0x01 */

        memcpy_s( &count16, sizeof(count16), current, sizeof(count16) );
        current += sizeof(count16);

        if( count16 != 0x0001 ) {
            printf_s( ":NEWLINE block for advance %i has unexpected number of CodeBlocks: %i\n", newline_block_ptr[i].advance, count16);
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        /* Get the CodeBlock */
        
        cop_codeblocks = get_code_blocks( &current, count16 );
        if( cop_codeblocks == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        if( cop_codeblocks->designator != 0x00 ) {
            printf_s( ":NEWLINE block for advance %i has unexpected CodeBlock designator: %i\n", newline_block_ptr[i].advance, cop_codeblocks[i].designator );
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( cop_codeblocks );
            cop_codeblocks = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        newline_block_ptr[i].count = cop_codeblocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + newline_block_ptr[i].count) ) {
            out_driver= resize_cop_driver( out_driver, newline_block_ptr[i].count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                return( out_driver );
            }
            newline_block_ptr = (newline_block *) ((uint8_t *) out_driver + (size_t) out_driver->newline.newlineblock);
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, newline_block_ptr[i].count, cop_codeblocks->function, newline_block_ptr[i].count );
        newline_block_ptr[i].text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += newline_block_ptr[i].count;

        free(cop_codeblocks);
        cop_codeblocks = NULL;

        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }
    }    
        
    /* Skip the unknown (empty) P-buffer */

    /* Get the count and verify that it is 0 */

    memcpy_s( &count16, sizeof(count16), current, sizeof(count16) );
    current += sizeof(count16);

    if( count16 != 0x0000 ) {
        printf_s( "Unknown block has non-zero count: %i\n", count16 );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Newpage Block, a Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* Verify that the number of CodeBlocks is 1 */

    if( cop_functions->count != 0x0001 ) {
        printf_s( ":NEWPAGE block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    out_driver->newpage.count = cop_functions->code_blocks->count;

    if( out_driver->allocated_size < (out_driver->next_offset + out_driver->newpage.count) ) {
        out_driver= resize_cop_driver( out_driver, out_driver->newpage.count );
        if( out_driver == NULL ) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( cop_functions );
            cop_functions = NULL;
            return( out_driver );
        }
    }
    text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

    memcpy_s( text_ptr, out_driver->newpage.count, cop_functions->code_blocks->function, out_driver->newpage.count );
    out_driver->newpage.text = (uint8_t *) out_driver->next_offset;
    out_driver->next_offset += out_driver->newpage.count;

    free(cop_functions);
    cop_functions = NULL;

    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Htab Block, a Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* This block is optional: a count of 0 is allowed */

    switch( cop_functions->count ) {
    case 0x0000 :
        out_driver->htab.count = 0;
        out_driver->htab.text = NULL;
        break;
    case 0x0001 :
        out_driver->htab.count = cop_functions->code_blocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->htab.count) ) {
            out_driver= resize_cop_driver( out_driver, out_driver->htab.count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_driver );
            }
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, out_driver->htab.count, cop_functions->code_blocks->function, out_driver->htab.count );
        out_driver->htab.text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += out_driver->htab.count;
        break;
    default :
        printf_s( ":NEWPAGE block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    free(cop_functions);
    cop_functions = NULL;

    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Fontswitch Block */

    /* This block is optional: a count of 0 is allowed */

    memcpy_s( &out_driver->fontswitch.count, sizeof(out_driver->fontswitch.count), current, sizeof(out_driver->fontswitch.count) );
    current += sizeof(out_driver->fontswitch.count);

    if( out_driver->fontswitch.count == 0x0000 ) {
        out_driver->fontswitch.fontswitchblock = NULL;

        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }

    } else {
        
        /* Add the fontswitch_block structs */

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->fontswitch.count * sizeof( fontswitch_block )) ) {
            out_driver = resize_cop_driver( out_driver, out_driver->fontswitch.count * sizeof( fontswitch_block ) );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                return( out_driver );
            }
        }
        fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + out_driver->next_offset);

        out_driver->fontswitch.fontswitchblock = (fontswitch_block *) out_driver->next_offset;
        out_driver->next_offset += out_driver->fontswitch.count * sizeof( fontswitch_block );

        /* Now get the fontswitch_block instances */

        for( i = 0; i < out_driver->fontswitch.count; i++ ) {

            /* The "type" is a null-terminated character string */
            
            length = strlen(current);
            length++;
            
            if( length == 0 ) {
                printf_s( ":FONTSWITCH block %i has no 'type'\n", i );
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            if( out_driver->allocated_size < (out_driver->next_offset + length) ) {
                out_driver = resize_cop_driver( out_driver, out_driver->next_offset + length );
                if( out_driver == NULL ) {
                    free( p_buffer_set );
                    p_buffer_set = NULL;
                    return( out_driver );
                }
                fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock);
            }

            string_ptr = (char *) out_driver + out_driver->next_offset;
            strcpy_s( string_ptr, length, current );
            current += length;

            fontswitch_block_ptr[i].type = (char *) out_driver->next_offset;
            out_driver->next_offset += length;

            /* Skip the next 20 or 21 bytes */

            if( length == 78 ) {
                current += 20;
            } else {
                current += 21;
            }

            /* Get the number of CodeBlocks; only 1 or 2 is valid */

            memcpy_s( &count16, sizeof(count16), current, sizeof(count16) );
            current += sizeof(count16);

            if( (count16 == 0x00) || (count16 > 0x02) ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            } else {
            
                /* Get the CodeBlock(s) */
        
                cop_codeblocks = get_code_blocks( &current, count16 );
                if( cop_codeblocks == NULL ) {
                    free( p_buffer_set );
                    p_buffer_set = NULL;
                    free( out_driver );
                    out_driver = NULL;
                    return( out_driver );
                }

                /* Process the CodeBlocks */
            
                fontswitch_block_ptr[i].startvalue = NULL;
                fontswitch_block_ptr[i].endvalue = NULL;

                for( j = 0; j < count16; j++ ) {
                    switch( cop_codeblocks[j].designator) {
                    case 0x04:

                        /* Add the code_text struct for endvalue */

                        if( out_driver->allocated_size < (out_driver->next_offset + sizeof( code_text )) ) {
                            out_driver= resize_cop_driver( out_driver, sizeof( code_text ) );
                            if( out_driver == NULL ) {
                                free( p_buffer_set );
                                p_buffer_set = NULL;
                                free( cop_codeblocks );
                                cop_codeblocks = NULL;
                                return( out_driver );
                            }
                            fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock);
                        }
                        code_text_ptr = (code_text *) ((uint8_t *) out_driver + out_driver->next_offset);

                        fontswitch_block_ptr[i].endvalue = (code_text *) ((uint8_t *) out_driver->next_offset);
                        out_driver->next_offset += sizeof( code_text );

                        /* Now get the CodeBlock */

                        code_text_ptr->count = cop_codeblocks[j].count;
                
                        if( out_driver->allocated_size < (out_driver->next_offset + code_text_ptr->count) ) {
                            out_driver = resize_cop_driver( out_driver, code_text_ptr->count );
                            if( out_driver == NULL ) {
                                free( p_buffer_set );
                                p_buffer_set = NULL;
                                free( cop_codeblocks );
                                cop_codeblocks = NULL;
                                return( out_driver );
                            }
                            fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock);
                            code_text_ptr = (code_text *) ((uint8_t *) out_driver + (size_t) fontswitch_block_ptr[i].endvalue);
                        }
                        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

                        memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[j].function, code_text_ptr->count );
                        code_text_ptr->text = (uint8_t *) out_driver->next_offset;
                        out_driver->next_offset += code_text_ptr->count;

                        break;
                    case 0x05:

                        /* Add the code_text struct for startvalue */

                        if( out_driver->allocated_size < (out_driver->next_offset + sizeof( code_text )) ) {
                            out_driver= resize_cop_driver( out_driver, sizeof( code_text ) );
                            if( out_driver == NULL ) {
                                free( p_buffer_set );
                                p_buffer_set = NULL;
                                free( cop_codeblocks );
                                cop_codeblocks = NULL;
                                return( out_driver );
                            }
                            fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock);
                        }
                        code_text_ptr = (code_text *) ((uint8_t *) out_driver + out_driver->next_offset);

                        fontswitch_block_ptr[i].startvalue = (code_text *) ((uint8_t *) out_driver->next_offset);
                        out_driver->next_offset += sizeof( code_text );

                        /* Now get the CodeBlock */

                        code_text_ptr->count = cop_codeblocks[j].count;
                
                        if( out_driver->allocated_size < (out_driver->next_offset + code_text_ptr->count) ) {
                            out_driver = resize_cop_driver( out_driver, code_text_ptr->count );
                            if( out_driver == NULL ) {
                                free( p_buffer_set );
                                p_buffer_set = NULL;
                                free( cop_codeblocks );
                                cop_codeblocks = NULL;
                                return( out_driver );
                            }
                            fontswitch_block_ptr = (fontswitch_block *) ((uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock);
                            code_text_ptr = (code_text *) ((uint8_t *) out_driver + (size_t) fontswitch_block_ptr[i].startvalue);
                        }
                        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

                        memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[j].function, code_text_ptr->count );
                        code_text_ptr->text = (uint8_t *) out_driver->next_offset;
                        out_driver->next_offset += code_text_ptr->count;

                        break;
                    default:
                        printf_s( ":FONTSWITCH block %s has unexpected CodeBlock designator: %i\n", fontswitch_block_ptr[i].type, cop_codeblocks[i].designator );
                        free( p_buffer_set );
                        p_buffer_set = NULL;
                        free( cop_codeblocks );
                        cop_codeblocks = NULL;
                        free( out_driver );
                        out_driver = NULL;
                        return( out_driver );
                    }
                }
            }

            free(cop_codeblocks);
            cop_codeblocks = NULL;

            /* Reset to the start of the next P-buffer's data */

            if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
                factor = (current - p_buffer_set->buffer) / 80;
                factor++;
                current = p_buffer_set->buffer + factor * 80;
            }

        }
    }

    /* Verify that current is pointing to the end of the p_buffer_set */

    factor = (current - p_buffer_set->buffer) / 80;
    if( factor * 80 < p_buffer_set->count ) {

        /* Rewind the file to the count byte of the FontstyleBlock */
        /* The number of false P-buffers must be added to the span */

        span = (p_buffer_set->count - (factor * 80));
        fseek( in_file, -1 * (span + span / 80), SEEK_CUR );
        if( ferror( in_file ) || feof( in_file ) ) {
            puts( "Problem rewinding file before processing :FONTSTYLE block\n" );
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    } 

    free( p_buffer_set );
    p_buffer_set = NULL;
    
    /* Parse the Fontstyle Block */

    /* Get the initial data length byte and ensure it is not 0 */
    
    fread( &count8, sizeof( count8 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( count8 == 0x00 ) {
        puts( "First :FONTSTYLE data length was 0: invalid file" );
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* Get the number of Blocks and ensure it is not 0 */

    fread( &out_driver->fontstyle.count, sizeof( out_driver->fontstyle.count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    if( out_driver->fontstyle.count == 0x00 ) {
        puts( "No :FONTSTYLE blocks found: at least one ('plain') must exist" );
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }
    count8 -= sizeof( out_driver->fontstyle.count );

    /* Add the font_style instances */
    
    if( out_driver->allocated_size < (out_driver->next_offset + out_driver->fontstyle.count * sizeof( font_style )) ) {
        out_driver = resize_cop_driver( out_driver, out_driver->fontstyle.count * sizeof( font_style ) );
        if( out_driver == NULL ) return( out_driver );
    }
    font_style_ptr = (font_style *) ((uint8_t *) out_driver + out_driver->next_offset);

    out_driver->fontstyle.fontstyle = (font_style *) out_driver->next_offset;
    out_driver->next_offset += out_driver->fontstyle.count * sizeof( font_style );

    /* Initialize the fonst_style instances */

    /* Set the pointers in the font_style instances to NULL */

    for( i = 0; i < out_driver->fontstyle.count; i++ ) {
        font_style_ptr[i].startvalue = NULL;
        font_style_ptr[i].endvalue = NULL;
        font_style_ptr[i].lineprocs = NULL;
    }

    /* The first must be done separately -- see the Wiki for the file structure */
    
    out_driver = parse_font_style( in_file, out_driver, font_style_ptr, &p_buffer_set, &current, count8 );
    if( out_driver == NULL ) return( out_driver );
    font_style_ptr = (font_style *) ((uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle);


    /* The rest, if any, can be done in a loop */

    for( i = 1; i < out_driver->fontstyle.count; i++ ) {

        /* Locate the start of the "next" P-buffer & verify that it is not present */

        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        if( factor * 80 < p_buffer_set->count ) {

            /* Rewind the file to the count byte of the FontstyleBlock */
            /* The number of false P-buffers must be added to the span */

            span = (p_buffer_set->count - (factor * 80));
            fseek( in_file, -1 * (span + span / 80), SEEK_CUR );
            if( ferror( in_file ) || feof( in_file ) ) {
                puts( "Problem rewinding file before processing :FONTSTYLE block\n" );
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }
        } 
        free(p_buffer_set);
        p_buffer_set = NULL;
        
        /* Get the initial data length byte and ensure it is not 0 */
    
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        if( count8 == 0x00 ) {
            printf_s( ":FONTSTYLE %i data length was 0: invalid file\n", i+1 );
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        out_driver = parse_font_style( in_file, out_driver, &font_style_ptr[i], &p_buffer_set, &current, count8 );
        if( out_driver == NULL ) return( out_driver );
        font_style_ptr = (font_style *) ((uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle);
    }
    
    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Absoluteaddress Block, a Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* The number of CodeBlocks may be 0 or 1 */

    switch( cop_functions->count ) {
    case 0x0000 :
        out_driver->absoluteaddress.count = 0;
        out_driver->absoluteaddress.text = NULL;
        break;
    case 0x0001 :
        out_driver->absoluteaddress.count = cop_functions->code_blocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->absoluteaddress.count) ) {
            out_driver= resize_cop_driver( out_driver, out_driver->absoluteaddress.count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_driver );
            }
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, out_driver->absoluteaddress.count, cop_functions->code_blocks->function, out_driver->absoluteaddress.count );
        out_driver->absoluteaddress.text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += out_driver->absoluteaddress.count;
        break;
    default:
        printf_s( ":ABSOLUTEADDRESS block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    free(cop_functions);
    cop_functions = NULL;

    /* Reset to the start of the next P-buffer's data */

    if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
        factor = (current - p_buffer_set->buffer) / 80;
        factor++;
        current = p_buffer_set->buffer + factor * 80;
    }

    /* Parse the Hline Block */

    /* First parse the Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* The number of CodeBlocks may be 0 or 1 */

    switch( cop_functions->count ) {
    case 0x0000 :
        out_driver->hline.count = 0;
        out_driver->hline.text = NULL;
        break;
    case 0x0001 :
        out_driver->hline.count = cop_functions->code_blocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->hline.count) ) {
            out_driver= resize_cop_driver( out_driver, out_driver->hline.count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_driver );
            }
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, out_driver->hline.count, cop_functions->code_blocks->function, out_driver->hline.count );
        out_driver->hline.text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += out_driver->hline.count;
        break;
    default:
        printf_s( ":HLINE block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    free(cop_functions);
    cop_functions = NULL;

    /* The thickness is present only if :HLINE was present */

    if( out_driver->hline.text == NULL) {
        
        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }
    } else {

        /* The thickness halted the set of P-buffers so it is exhausted */
        
        free( p_buffer_set );
        p_buffer_set = NULL;

        /* Get the thickness */
    
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    
        switch( count8 ) {
        case 0x02:

            /* Get the 16-bit Page Width */

            fread( &count16, sizeof( count16 ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            out_driver->hline.thickness  = count16;

            break;
        case 0x04:

            /* Get the 32-bit Page Width */

            fread( &out_driver->hline.thickness, sizeof( out_driver->hline.thickness ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            break;
        default:
            printf_s( "Bad size for :HLINE thickness value: %i\n", count8 );
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    }
    
    /* Parse the Vline Block */

    /* If :HLINE existed, then get the set of P-buffers for :VLINE */

    if( out_driver->hline.text != NULL) {
        p_buffer_set = get_p_buffer( in_file );
        if( p_buffer_set == NULL) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        if( p_buffer_set->buffer == NULL) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        current = p_buffer_set->buffer;
    }

    /* Now parse the Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* The number of CodeBlocks may be 0 or 1 */

    switch( cop_functions->count ) {
    case 0x0000 :
        out_driver->vline.count = 0;
        out_driver->vline.text = NULL;
        break;
    case 0x0001 :
        out_driver->vline.count = cop_functions->code_blocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->vline.count) ) {
            out_driver= resize_cop_driver( out_driver, out_driver->vline.count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_driver );
            }
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, out_driver->vline.count, cop_functions->code_blocks->function, out_driver->vline.count );
        out_driver->vline.text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += out_driver->vline.count;
        break;
    default:
        printf_s( ":VLINE block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    free(cop_functions);
    cop_functions = NULL;

    /* The thickness is present only if :VLINE was present */

    if( out_driver->vline.text == NULL) {
        
        /* Reset to the start of the next P-buffer's data */

        if( (current - p_buffer_set->buffer) % 80 != 0 ) { 
            factor = (current - p_buffer_set->buffer) / 80;
            factor++;
            current = p_buffer_set->buffer + factor * 80;
        }
    } else {

        /* The thickness halted the set of P-buffers so it is exhausted */
        
        free( p_buffer_set );
        p_buffer_set = NULL;

        /* Get the thickness */
    
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    
        switch( count8 ) {
        case 0x02:

            /* Get the 16-bit Page Width */

            fread( &count16, sizeof( count16 ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            out_driver->vline.thickness  = count16;

            break;
        case 0x04:

            /* Get the 32-bit Page Width */

            fread( &out_driver->vline.thickness, sizeof( out_driver->vline.thickness ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            break;
        default:
            printf_s( "Bad size for :VLINE thickness value: %i\n", count8 );
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    }
    
    /* Parse the Dbox Block */

    /* If :VLINE existed, then get the set of P-buffers for :DBOX */

    if( out_driver->vline.text != NULL) {

        /* Get the set of P-buffers */

        p_buffer_set = get_p_buffer( in_file );
        if( p_buffer_set == NULL) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        if( p_buffer_set->buffer == NULL) {
            free( p_buffer_set );
            p_buffer_set = NULL;
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }

        current = p_buffer_set->buffer;
    }
    
    /* Now parse the Variant A FunctionsBlock */

    cop_functions = parse_functions_block( &current );

    /* The number of CodeBlocks may be 0 or 1 */

    switch( cop_functions->count ) {
    case 0x0000 :
        out_driver->dbox.count = 0;
        out_driver->dbox.text = NULL;
        break;
    case 0x0001 :
        out_driver->dbox.count = cop_functions->code_blocks->count;

        if( out_driver->allocated_size < (out_driver->next_offset + out_driver->dbox.count) ) {
            out_driver= resize_cop_driver( out_driver, out_driver->dbox.count );
            if( out_driver == NULL ) {
                free( p_buffer_set );
                p_buffer_set = NULL;
                free( cop_functions );
                cop_functions = NULL;
                return( out_driver );
            }
        }
        text_ptr = (uint8_t *) out_driver + out_driver->next_offset;

        memcpy_s( text_ptr, out_driver->dbox.count, cop_functions->code_blocks->function, out_driver->dbox.count );
        out_driver->dbox.text = (uint8_t *) out_driver->next_offset;
        out_driver->next_offset += out_driver->dbox.count;
        break;
    default:
        printf_s( ":DBOX block has unexpected number of CodeBlocks: %i\n", cop_functions->count );
        free( p_buffer_set );
        p_buffer_set = NULL;
        free(cop_functions);
        cop_functions = NULL;
        free( out_driver );
        out_driver = NULL;
        return( out_driver );
    }

    /* :DBOX is the last item in the .COP file and so the P-buffers are ended */

    free( p_buffer_set );
    p_buffer_set = NULL;
    free(cop_functions);
    cop_functions = NULL;

    /* The thickness is present only if :DBOX was present */

    if( out_driver->dbox.text != NULL) {
        
        /* Get the thickness */
    
        fread( &count8, sizeof( count8 ), 1, in_file );
        if( ferror( in_file ) || feof( in_file ) ) {
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    
        switch( count8 ) {
        case 0x02:

            /* Get the 16-bit Page Width */

            fread( &count16, sizeof( count16 ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            out_driver->dbox.thickness  = count16;

            break;
        case 0x04:

            /* Get the 32-bit Page Width */

            fread( &out_driver->dbox.thickness, sizeof( out_driver->dbox.thickness ), 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_driver );
                out_driver = NULL;
                return( out_driver );
            }

            break;
        default:
            printf_s( "Bad size for :DBOX thickness value: %i\n", count8 );
            free( out_driver );
            out_driver = NULL;
            return( out_driver );
        }
    }

    /* Convert non-NULL offsets to pointers */

    if( out_driver->rec_spec != NULL ) {
        string_ptr = (char *) out_driver + (size_t) out_driver->rec_spec;
        out_driver->rec_spec = string_ptr;
    }

    if( out_driver->init.start_initblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.start_initblock;
        out_driver->init.start_initblock = (init_block *) byte_ptr;
        if(out_driver->init.start_initblock->codetext != NULL ) {
            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.start_initblock->codetext;
            out_driver->init.start_initblock->codetext = (init_text *) byte_ptr;
            for( i = 0; i < out_driver->init.start_initblock->count; i++ ) {
                if( out_driver->init.start_initblock->codetext[i].text != NULL )
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.start_initblock->codetext[i].text;
                out_driver->init.start_initblock->codetext[i].text = byte_ptr;
            }
        }
    }
    
    if( out_driver->init.document_initblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.document_initblock;
        out_driver->init.document_initblock = (init_block *) byte_ptr;
        if(out_driver->init.document_initblock->codetext != NULL ) {
            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.document_initblock->codetext;
            out_driver->init.document_initblock->codetext = (init_text *) byte_ptr;
            for( i = 0; i < out_driver->init.document_initblock->count; i++ ) {
                if( out_driver->init.document_initblock->codetext[i].text != NULL )
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->init.document_initblock->codetext[i].text;
                out_driver->init.document_initblock->codetext[i].text = byte_ptr;
            }
        }
    }
    
    if( out_driver->finish.end_finishblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->finish.end_finishblock;
        out_driver->finish.end_finishblock = (code_text *) byte_ptr;
        if(out_driver->finish.end_finishblock->text != NULL ) {
            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->finish.end_finishblock->text;
            out_driver->finish.end_finishblock->text = byte_ptr;
        }
    }
    
    if( out_driver->finish.document_finishblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->finish.document_finishblock;
        out_driver->finish.document_finishblock = (code_text *) byte_ptr;
        if(out_driver->finish.document_finishblock->text != NULL ) {
            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->finish.document_finishblock->text;
            out_driver->finish.document_finishblock->text = byte_ptr;
        }
    }
    
    if( out_driver->newline.newlineblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->newline.newlineblock;
        out_driver->newline.newlineblock = (newline_block *) byte_ptr;
        for( i = 0; i < out_driver->newline.count; i++ ) {
            if(out_driver->newline.newlineblock[i].text != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->newline.newlineblock[i].text;
                out_driver->newline.newlineblock[i].text = byte_ptr;
            }
        }
    }

    if( out_driver->newpage.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->newpage.text;
        out_driver->newpage.text = byte_ptr;
    }

    if( out_driver->htab.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->htab.text;
        out_driver->htab.text = byte_ptr;
    }

    if( out_driver->fontswitch.fontswitchblock != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock;
        out_driver->fontswitch.fontswitchblock = (fontswitch_block *) byte_ptr;
        for( i = 0; i < out_driver->fontswitch.count; i++ ) {
            if( out_driver->fontswitch.fontswitchblock[i].type != NULL ) {
                string_ptr = (char *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock[i].type;
                out_driver->fontswitch.fontswitchblock[i].type = string_ptr;
            }
            if( out_driver->fontswitch.fontswitchblock[i].startvalue != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock[i].startvalue;
                out_driver->fontswitch.fontswitchblock[i].startvalue = (code_text *) byte_ptr;
                if( out_driver->fontswitch.fontswitchblock[i].startvalue->text != NULL ) {
                    byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock[i].startvalue->text;
                    out_driver->fontswitch.fontswitchblock[i].startvalue->text = byte_ptr;
                }
            }
            if( out_driver->fontswitch.fontswitchblock[i].endvalue != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock[i].endvalue;
                out_driver->fontswitch.fontswitchblock[i].endvalue = (code_text *) byte_ptr;
                if( out_driver->fontswitch.fontswitchblock[i].endvalue->text != NULL ) {
                    byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontswitch.fontswitchblock[i].endvalue->text;
                    out_driver->fontswitch.fontswitchblock[i].endvalue->text = byte_ptr;
                }
            }
        }
    }

    if( out_driver->fontstyle.fontstyle != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle;
        out_driver->fontstyle.fontstyle = (font_style *) byte_ptr;
        for( i = 0; i < out_driver->fontstyle.count; i++ ) {
            if( out_driver->fontstyle.fontstyle[i].type != NULL ) {
                string_ptr = (char *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].type;
                out_driver->fontstyle.fontstyle[i].type = string_ptr;
            }
            if( out_driver->fontstyle.fontstyle[i].startvalue != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].startvalue;
                out_driver->fontstyle.fontstyle[i].startvalue = (code_text *) byte_ptr;
                if( out_driver->fontstyle.fontstyle[i].startvalue->text != NULL ) {
                    byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].startvalue->text;
                    out_driver->fontstyle.fontstyle[i].startvalue->text = byte_ptr;
                }
            }
            if( out_driver->fontstyle.fontstyle[i].endvalue != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].endvalue;
                out_driver->fontstyle.fontstyle[i].endvalue = (code_text *) byte_ptr;
                if( out_driver->fontstyle.fontstyle[i].endvalue->text != NULL ) {
                    byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].endvalue->text;
                    out_driver->fontstyle.fontstyle[i].endvalue->text = byte_ptr;
                }
            }
            if( out_driver->fontstyle.fontstyle[i].lineprocs != NULL ) {
                byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs;
                out_driver->fontstyle.fontstyle[i].lineprocs = (line_proc *) byte_ptr;
                for( j = 0; j < out_driver->fontstyle.fontstyle[i].passes; j++ ) {
                    if( out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue != NULL ) {
                        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue;
                        out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue = (code_text *) byte_ptr;
                        if( out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue->text != NULL ) {
                            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue->text;
                            out_driver->fontstyle.fontstyle[i].lineprocs[j].startvalue->text = byte_ptr;
                        }
                    }
                    if( out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword != NULL ) {
                        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword;
                        out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword = (code_text *) byte_ptr;
                        if( out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword->text != NULL ) {
                            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword->text;
                            out_driver->fontstyle.fontstyle[i].lineprocs[j].firstword->text = byte_ptr;
                        }
                    }
                    if( out_driver->fontstyle.fontstyle[i].lineprocs[j].startword != NULL ) {
                        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].startword;
                        out_driver->fontstyle.fontstyle[i].lineprocs[j].startword = (code_text *) byte_ptr;
                        if( out_driver->fontstyle.fontstyle[i].lineprocs[j].startword->text != NULL ) {
                            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].startword->text;
                            out_driver->fontstyle.fontstyle[i].lineprocs[j].startword->text = byte_ptr;
                        }
                    }
                    if( out_driver->fontstyle.fontstyle[i].lineprocs[j].endword != NULL ) {
                        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].endword;
                        out_driver->fontstyle.fontstyle[i].lineprocs[j].endword = (code_text *) byte_ptr;
                        if( out_driver->fontstyle.fontstyle[i].lineprocs[j].endword->text != NULL ) {
                            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].endword->text;
                            out_driver->fontstyle.fontstyle[i].lineprocs[j].endword->text = byte_ptr;
                        }
                    }
                    if( out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue != NULL ) {
                        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue;
                        out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue = (code_text *) byte_ptr;
                        if( out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue->text != NULL ) {
                            byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue->text;
                            out_driver->fontstyle.fontstyle[i].lineprocs[j].endvalue->text = byte_ptr;
                        }
                    }
                }
            }
        }
    }

    if( out_driver->absoluteaddress.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->absoluteaddress.text;
        out_driver->absoluteaddress.text = byte_ptr;
    }

    if( out_driver->hline.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->hline.text;
        out_driver->hline.text = byte_ptr;
    }

    if( out_driver->vline.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->vline.text;
        out_driver->vline.text = byte_ptr;
    }

    if( out_driver->dbox.text != NULL ) {
        byte_ptr = (uint8_t *) out_driver + (size_t) out_driver->dbox.text;
        out_driver->dbox.text = byte_ptr;
    }

    return( out_driver );
}

/*  Function parse_finish_block().
 *  Processes a single :FINISH block. This may be called up to two times when
 *  parsing a .COP file encoding a :DRIVER block. 
 *
 *  Note: although a :FINISH block can contain more than one :VALUE block, and
 *  gendev will put more than one CodeBlock into the .COP file, wgml only
 *  invokes the first one. Thus, this function only parses the first CodeBlock.
 *
 *  Parameters:
 *      in_driver contains the cop_driver being initialized
 *      *current contains the current position in a p_buffer::buffer
 *
 *  Parameter modified:
 *      if the function succeeds, then *current will point to the first junk
 *          byte following the last CodeBlock for this :FINISH block.
 *      if the function fails, the value of *current should be regarded as
 *          invalid
 *
 *  Returns:
 *      in_driver, possibly relocated, on success
 *      NULL on failure
 */

cop_driver * parse_finish_block( cop_driver * in_driver, uint8_t * * current ) {

    code_block *    cop_codeblocks  = NULL;
    code_text *     code_text_ptr   = NULL;
    uint8_t         designator;    
    uint8_t *       text_ptr        = NULL;
    uint16_t        count;

    /* Get the designator and validate it */

    memcpy_s( &designator, 1, *current, 1 );
    (*current)++;

    /* Process the :FINISH block */

    switch( designator ) {
    case 0x01 :
        memcpy_s( &count, sizeof(count), *current, sizeof(count) );
        *current += sizeof(count);
        if( count == 0x0000 ) {
            puts( "End :FINISH block has no code!" );
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        if( count > 0x0001 ) {
            printf_s( "End :Finish block has unexpected number of CodeBlocks: %i\n", count);
        }

        /* Add the code_text struct itself */

        if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
            in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
            if( in_driver == NULL ) return( in_driver );
        }
        code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

        in_driver->finish.end_finishblock = (code_text *) in_driver->next_offset;
        in_driver->next_offset += sizeof( code_text );

        /* Get the CodeBlocks */
        
        cop_codeblocks = get_code_blocks( current, count );
        if( cop_codeblocks == NULL ) {
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }
        
        /* Initialize the code_text struct */

        if( cop_codeblocks[0].designator != 0x00 ) {
            printf_s( "End :FINISH block has unexpected CodeBlock designator: %i\n", designator );
            free( cop_codeblocks );
            cop_codeblocks = NULL;
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        code_text_ptr->count = cop_codeblocks[0].count;

        if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
            in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
            if( in_driver == NULL ) {
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                return( in_driver );
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) in_driver->finish.end_finishblock);
        }

        text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

        memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[0].function, code_text_ptr->count );
        code_text_ptr->text = (uint8_t *) in_driver->next_offset;
        in_driver->next_offset += code_text_ptr->count;

        free( cop_codeblocks );
        cop_codeblocks = NULL;

        break;
    case 0x02 :
        memcpy_s( &count, sizeof(count), *current, sizeof(count) );
        *current += sizeof(count);
        if( count == 0 ) {
            puts( "Document :FINISH block has no code!" );
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        if( count > 0x0001 ) {
            printf_s( "Document :Finish block has unexpected number of CodeBlocks: %i\n", count);
        }

        /* Add the code_text struct itself */

        if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
            in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
            if( in_driver == NULL ) return( in_driver );
        }
        code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

        in_driver->finish.document_finishblock = (code_text *) in_driver->next_offset;
        in_driver->next_offset += sizeof( code_text );

        /* Get the CodeBlocks */
        
        cop_codeblocks = get_code_blocks( current, count );
        if( cop_codeblocks == NULL ) {
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }
        
        /* Initialize the code_text struct */

        if( cop_codeblocks[0].designator != 0x00 ) {
            printf_s( "Document :FINISH block has unexpected CodeBlock designator: %i\n", designator );
            free( cop_codeblocks );
            cop_codeblocks = NULL;
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        code_text_ptr->count = cop_codeblocks[0].count;

        if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
            in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
            if( in_driver == NULL ) {
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                return( in_driver );
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) in_driver->finish.document_finishblock);
        }
        text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

        memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[0].function, code_text_ptr->count );
        code_text_ptr->text = (uint8_t *) in_driver->next_offset;
        in_driver->next_offset += code_text_ptr->count;

        free( cop_codeblocks );
        cop_codeblocks = NULL;

        break;
    default :
        printf_s( ":FINISH block designator incorrect: %i\n", designator );
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    return( in_driver );
}

/*  Function parse_font_style().
 *  Processes a single :FONTSTYLE block. This may be called any number of times
 *  when parsing a .COP file encoding a :DRIVER block.
 *
 *  Note: The Wiki should be consulted if the treatment of p_buffer_set and
 *        count appears inexplicable. The structure in the .COP file is very
 *        odd and something like the contortions shown here are unavoidable.
 *
 *  Parameters:
 *      in_file is the file being parsed
 *      in_driver contains the cop_drivr being initialized
 *      font_style_ptr is the current font_style instance
 *      p_buffer_set should be NULL and any memory formerly pointed to freed
 *      count contains the total length of the pre-P-buffer data
 *
 *  Parameter modified:
 *      if the function succeeds, then *p_buffer_set will point to the
 *          current set of P-buffers
 *      if the function fails, then *p_buffer_set will be NULL
 *
 *  Returns:
 *      in_driver, possibly relocated, on success
 *      NULL on failure
 */

cop_driver * parse_font_style( FILE * in_file, cop_driver * in_driver, font_style * font_style_ptr, p_buffer * * p_buffer_set, uint8_t * * current, uint8_t count )
{

    char *          string_ptr          = NULL;
    code_block *    cop_codeblocks      = NULL;
    code_text *     code_text_ptr       = NULL;
    int             i;
    line_proc *     line_proc_ptr       = NULL;
    ptrdiff_t       font_style_offset;
    uint8_t *       text_ptr            = NULL;
    uint16_t        count16;

    /* Get the number of passes, which can be 0 */

    fread( &font_style_ptr->passes, sizeof( font_style_ptr->passes ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }
    count -= sizeof( font_style_ptr->passes );

    /* Get the unknown count, and verify that it is 1 */

    fread( &count16, sizeof( count16 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    if( count16 != 0x0001 ) {
        printf_s( ":FONTSTYLE block has incorrect 'unknown count' value: %i\n", count16 );
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }
    count -= sizeof( count16 );

    /* Get the two nulls, and verify they contain the value 0 */

    fread( &count16, sizeof( count16 ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    if( count16 != 0x0000 ) {
        printf_s( ":FONTSTYLE block %i has incorrect 'two-byte nulls' value: %i\n", i, count16 );
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }
    count -= sizeof( count16 );

    /* count should contain the length of type, including the terminal null */

    if( count == 0 ) {
        puts( ":FONTSTYLE block has no type");
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    /* Add the space for the type */

    if( in_driver->allocated_size < (in_driver->next_offset + count) ) {
        font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
        in_driver = resize_cop_driver( in_driver, count );
        if( in_driver == NULL ) {
            free( *p_buffer_set );
            *p_buffer_set = NULL;
            return( in_driver );
        }
        font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
    }

    string_ptr = (char *) in_driver + in_driver->next_offset;

    font_style_ptr->type = (char *) in_driver->next_offset;
    in_driver->next_offset += count;

    /* Acquire the type */
        
    fread( string_ptr, count, 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    /* Done here so lineprocs can be used as an array */

    if( font_style_ptr->passes == 0 ) {
        font_style_ptr->lineprocs = NULL;
    } else {

        /* Add the space for the lineproc instances */

        if( in_driver->allocated_size < (in_driver->next_offset + (font_style_ptr->passes * sizeof(line_proc)) ) ) {
            font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
            in_driver = resize_cop_driver( in_driver, font_style_ptr->passes * sizeof(line_proc) );
            if( in_driver == NULL ) {
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                return( in_driver );
            }
            font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
        }
        line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + in_driver->next_offset);

        font_style_ptr->lineprocs = (line_proc *) in_driver->next_offset;
        in_driver->next_offset += font_style_ptr->passes * sizeof(line_proc);

        /* Set the line_proc instance pointers to null */

        for( i = 0; i < font_style_ptr->passes; i++ ) {
            line_proc_ptr[i].startvalue = NULL;
            line_proc_ptr[i].firstword = NULL;
            line_proc_ptr[i].startword = NULL;
            line_proc_ptr[i].endword = NULL;
            line_proc_ptr[i].endvalue = NULL;
        }
    }

    /* Now get the P-buffers */
        
    *p_buffer_set = get_p_buffer( in_file );
    if( *p_buffer_set == NULL) {
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    *current = (*p_buffer_set)->buffer;

    /* Skip the first 21 bytes */

    *current += 21;

    /* Get the number of codeblocks, which can be 0 */

    memcpy_s( &count16, sizeof( count16 ), *current, sizeof( count16 ) );
    *current += sizeof( count16 );
    
    /* If the count is 0, we are done: the P-buffer is empty */

    if( count16 == 0 ) return( in_driver );

    /* Get the CodeBlocks */
        
    cop_codeblocks = get_code_blocks( current, count16 );
    if( cop_codeblocks == NULL ) {
        free( *p_buffer_set );
        *p_buffer_set = NULL;
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }

    /* Process the CodeBlocks into the font_style instance */

    for( i = 0; i < count16; i++ ) {

        /* Trap zero-length CodeBlocks */
        if( cop_codeblocks[i].count == 0 ) continue;
        
        switch( cop_codeblocks[i].designator ) {
        case 0x04 :

            /* The :FONTSTYLE :ENDVALUE block */

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            font_style_ptr->endvalue = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) font_style_ptr->endvalue);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x05 :

            /* The :FONTSTYLE :STARTVALUE block */

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            font_style_ptr->startvalue = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) font_style_ptr->startvalue);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x08 :

            /* A :FONTSTYLE :LINEPROC :ENDVALUE block */

            /* Ensure that the pass is within range */

            if( cop_codeblocks[i].pass > font_style_ptr->passes ) {
                printf_s( ":FONTSTYLE :LINEPROC :ENDVALUE block has incorrect pass value: %i\n", cop_codeblocks[i].pass );
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                free( in_driver );
                in_driver = NULL;
                return( in_driver );
            }

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            line_proc_ptr[cop_codeblocks[i].pass - 1].endvalue = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) line_proc_ptr[cop_codeblocks[i].pass - 1].endvalue);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x09 :

            /* A :FONTSTYLE :LINEPROC :STARTVALUE block */

            /* Ensure that the pass is within range */

            if( cop_codeblocks[i].pass > font_style_ptr->passes ) {
                printf_s( ":FONTSTYLE :LINEPROC :STARTVALUE block has incorrect pass value: %i\n", cop_codeblocks[i].pass );
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                free( in_driver );
                in_driver = NULL;
                return( in_driver );
            }

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            line_proc_ptr[cop_codeblocks[i].pass - 1].startvalue = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) line_proc_ptr[cop_codeblocks[i].pass - 1].startvalue);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x28 :

            /* A :FONTSTYLE :LINEPROC :ENDWORD block */

            /* Ensure that the pass is within range */

            if( cop_codeblocks[i].pass > font_style_ptr->passes ) {
                printf_s( ":FONTSTYLE :LINEPROC :ENDWORD block has incorrect pass value: %i\n", cop_codeblocks[i].pass );
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                free( in_driver );
                in_driver = NULL;
                return( in_driver );
            }

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            line_proc_ptr[cop_codeblocks[i].pass - 1].endword = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) line_proc_ptr[cop_codeblocks[i].pass - 1].endword);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x29 :

            /* A :FONTSTYLE :LINEPROC :STARTWORD block */

            /* Ensure that the pass is within range */

            if( cop_codeblocks[i].pass > font_style_ptr->passes ) {
                printf_s( ":FONTSTYLE :LINEPROC :STARTWORD block has incorrect pass value: %i\n", cop_codeblocks[i].pass );
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                free( in_driver );
                in_driver = NULL;
                return( in_driver );
            }

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            line_proc_ptr[cop_codeblocks[i].pass - 1].startword = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) line_proc_ptr[cop_codeblocks[i].pass - 1].startword);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        case 0x49 :

            /* A :FONTSTYLE :LINEPROC :FIRSTWORD block */

            /* Ensure that the pass is within range */

            if( cop_codeblocks[i].pass > font_style_ptr->passes ) {
                printf_s( ":FONTSTYLE :LINEPROC :FIRSTWORD block has incorrect pass value: %i\n", cop_codeblocks[i].pass );
                free( *p_buffer_set );
                *p_buffer_set = NULL;
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                free( in_driver );
                in_driver = NULL;
                return( in_driver );
            }

            /* Add the code_text struct */

            if( in_driver->allocated_size < (in_driver->next_offset + sizeof( code_text )) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, sizeof( code_text ) );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
            }
            code_text_ptr = (code_text *) ((uint8_t *) in_driver + in_driver->next_offset);

            line_proc_ptr[cop_codeblocks[i].pass - 1].firstword = (code_text *) in_driver->next_offset;
            in_driver->next_offset += sizeof( code_text );

            /* Initialize the code_text struct */

            code_text_ptr->count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + code_text_ptr->count) ) {
                font_style_offset = ((uint8_t *) font_style_ptr - (uint8_t *) in_driver);
                in_driver = resize_cop_driver( in_driver, code_text_ptr->count );
                if( in_driver == NULL ) {
                    free( *p_buffer_set );
                    *p_buffer_set = NULL;
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                font_style_ptr = (font_style *) ((uint8_t *) in_driver + font_style_offset);
                line_proc_ptr = (line_proc *) ((uint8_t *) in_driver + (size_t) font_style_ptr->lineprocs);
                code_text_ptr = (code_text *) ((uint8_t *) in_driver + (size_t) line_proc_ptr[cop_codeblocks[i].pass - 1].firstword);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, code_text_ptr->count, cop_codeblocks[i].function, code_text_ptr->count );
            code_text_ptr->text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += code_text_ptr->count;

            break;
        default :
            printf_s( ":FONTSTYLE block %i has unknown designator: %i\n", i, cop_codeblocks[i].designator );
            free( *p_buffer_set );
            *p_buffer_set = NULL;
            free(cop_codeblocks);
            cop_codeblocks = NULL;
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }
    }

    /* Both in_driver and *p_buffer_set must be returned pointing to data */

    free(cop_codeblocks);
    cop_codeblocks = NULL;

    return( in_driver );
}

/*  Function parse_init_block().
 *  Processes a single :INIT block. This may be called up to two times when
 *  parsing a .COP file encoding a :DRIVER block. 
 *
 *  Parameters:
 *      in_driver contains the cop_driver being initialized
 *      *current contains the current position in a p_buffer::buffer
 *
 *  Parameter modified:
 *      if the function succeeds, then *current will point to the first junk
 *          byte following the last CodeBlock for this init_block
 *      if the function fails, the value of *current should be regarded as
 *          invalid
 *
 *  Returns:
 *      in_driver, possibly relocated, on success
 *      NULL on failure
 */
cop_driver * parse_init_block( cop_driver * in_driver, uint8_t * * current ) {

    code_block *    cop_codeblocks  = NULL;
    init_block *    init_block_ptr  = NULL;
    init_text *     init_text_ptr   = NULL;
    int             i;
    uint8_t         designator;    
    uint8_t *       text_ptr        = NULL;
    uint16_t        count;

    /* Get the designator and validate it */

    memcpy_s( &designator, 1, *current, 1 );
    (*current)++;

    /* Process the init_block */

    switch( designator ) {
    case 0x01 :
        memcpy_s( &count, sizeof(count), *current, sizeof(count) );
        *current += sizeof(count);
        if( count == 0 ) {
            puts( "Start :INIT block has no code!" );
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        /* Add the init_block structure itself */

        if( in_driver->allocated_size < (in_driver->next_offset + sizeof( init_block )) ) {
            in_driver = resize_cop_driver( in_driver, sizeof( init_block ) );
            if( in_driver == NULL ) return( in_driver );
        }
        init_block_ptr = (init_block *) ((uint8_t *) in_driver + in_driver->next_offset);

        in_driver->init.start_initblock = (init_block *) in_driver->next_offset;
        in_driver->next_offset += sizeof( init_block );

        /* Get the CodeBlocks */
        
        cop_codeblocks = get_code_blocks( current, count );
        if( cop_codeblocks == NULL ) {
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }
        
        /* Add the init_text blocks */
        
        if( in_driver->allocated_size < (in_driver->next_offset + count * sizeof( init_text )) ) {
            in_driver = resize_cop_driver( in_driver, count * sizeof( init_text ) );
            if( in_driver == NULL ) {
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                return( in_driver );
            }
            init_block_ptr = (init_block *) ((uint8_t *) in_driver + (size_t) in_driver->init.start_initblock);
        }
        init_text_ptr = (init_text *) ((uint8_t *) in_driver + in_driver->next_offset);

        init_block_ptr->count = count;
        init_block_ptr->codetext = (init_text *) in_driver->next_offset;
        in_driver->next_offset += count * sizeof( init_text );

        /* Initialize the init_text blocks */

        for(i = 0; i < init_block_ptr->count; i++ ) {

            if( cop_codeblocks[i].designator == 0x02 ) init_text_ptr[i].is_fontvalue = true;
            else {
                if( cop_codeblocks[i].designator == 0x00 ) init_text_ptr[i].is_fontvalue = false;
                else {
                    printf_s( "Start :INIT block has unexpected CodeBlock designator: %i\n", designator );
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    free( in_driver );
                    in_driver = NULL;
                    return( in_driver );

                }
            }

            init_text_ptr[i].count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + init_text_ptr[i].count) ) {
                in_driver = resize_cop_driver( in_driver, init_text_ptr[i].count );
                if( in_driver == NULL ) {
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                init_block_ptr = (init_block *) ((uint8_t *) in_driver + (size_t) in_driver->init.start_initblock);
                init_text_ptr = (init_text *) ((uint8_t *) in_driver + (size_t) init_block_ptr->codetext);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, init_text_ptr[i].count, cop_codeblocks[i].function, init_text_ptr[i].count );
            init_text_ptr[i].text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += init_text_ptr[i].count;

        }

        free( cop_codeblocks );
        cop_codeblocks = NULL;

        break;
    case 0x02 :
        memcpy_s( &count, sizeof(count), *current, sizeof(count) );
        *current += sizeof(count);
        if( count == 0 ) {
            puts( "Start :INIT block has no code!" );
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }

        /* Add the init_block structure itself */

        if( in_driver->allocated_size < (in_driver->next_offset + sizeof( init_block )) ) {
            in_driver = resize_cop_driver( in_driver, sizeof( init_block ) );
            if( in_driver == NULL ) return( in_driver );
        }
        init_block_ptr = (init_block *) ((uint8_t *) in_driver + in_driver->next_offset);

        in_driver->init.document_initblock = (init_block *) in_driver->next_offset;
        in_driver->next_offset += sizeof( init_block );

        /* Get the CodeBlocks */
        
        cop_codeblocks = get_code_blocks( current, count );
        if( cop_codeblocks == NULL ) {
            free( in_driver );
            in_driver = NULL;
            return( in_driver );
        }
        
        /* Add the init_text blocks */
        
        if( in_driver->allocated_size < (in_driver->next_offset + count * sizeof( init_text )) ) {
            in_driver = resize_cop_driver( in_driver, count * sizeof( init_text ) );
            if( in_driver == NULL ) {
                free( cop_codeblocks );
                cop_codeblocks = NULL;
                return( in_driver );
            }
            init_block_ptr = (init_block *) ((uint8_t *) in_driver + (size_t) in_driver->init.document_initblock);
        }

        init_text_ptr = (init_text *) ((uint8_t *) in_driver + in_driver->next_offset);

        init_block_ptr->count = count;
        init_block_ptr->codetext = (init_text *) in_driver->next_offset;
        in_driver->next_offset += count * sizeof( init_text );

        /* Initialize the init_text blocks */

        for(i = 0; i < init_block_ptr->count; i++ ) {

            if( cop_codeblocks[i].designator == 0x02 ) init_text_ptr[i].is_fontvalue = true;
            else {
                if( cop_codeblocks[i].designator == 0x00 ) init_text_ptr[i].is_fontvalue = false;
                else {
                    printf_s( "Document :INIT block has unexpected CodeBlock designator: %i\n", designator );
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    free( in_driver );
                    in_driver = NULL;
                    return( in_driver );

                }
            }

            init_text_ptr[i].count = cop_codeblocks[i].count;

            if( in_driver->allocated_size < (in_driver->next_offset + init_text_ptr[i].count) ) {
                in_driver = resize_cop_driver( in_driver, init_text_ptr[i].count );
                if( in_driver == NULL ) {
                    free( cop_codeblocks );
                    cop_codeblocks = NULL;
                    return( in_driver );
                }
                init_block_ptr = (init_block *) ((uint8_t *) in_driver + (size_t) in_driver->init.document_initblock);
                init_text_ptr = (init_text *) ((uint8_t *) in_driver + (size_t) init_block_ptr->codetext);
            }
            text_ptr = (uint8_t *) in_driver + in_driver->next_offset;

            memcpy_s( text_ptr, init_text_ptr[i].count, cop_codeblocks[i].function, init_text_ptr[i].count );
            init_text_ptr[i].text = (uint8_t *) in_driver->next_offset;
            in_driver->next_offset += init_text_ptr[i].count;

        }

        free( cop_codeblocks );
        cop_codeblocks = NULL;

        break;
    default :
        printf_s( ":INIT block designator incorrect: %i\n", designator );
        free( in_driver );
        in_driver = NULL;
        return( in_driver );
    }    

    return( in_driver );
}

/*  Function resize_cop_driver().
 *  Resizes a cop_driver instance.
 *
 *  Parameters:
 *      in_driver is a pointer to the cop_driver to be resized
 *      in_size is the minimum acceptable increase in size
 *
 *  Warning:
 *      If realloc() returns a different value from in_driver, then the
 *      memory pointed to by in_driver will be freed whether the function
 *      succeeds or fails. The intended use is for the pointer passed as
 *      in_driver to be used to store the return value. 
 *
 *  Returns:
 *      A pointer to a cop_driver instance at least in_size larger with the same
 *          data (except for the allocated_size field, which reflects the new size)
 *          on success.
 *      A NULL pointer on failure.
 */

cop_driver * resize_cop_driver( cop_driver * in_driver, size_t in_size )
{
    cop_driver *    local_driver = NULL;
    size_t          increment = INC_SIZE;
    size_t          new_size;
    size_t          scale;

    /* Compute how much larger to make the cop_driver struct */

    if( in_size > INC_SIZE ) {
        scale = in_size / INC_SIZE;
        ++scale;
        increment = scale * INC_SIZE;
    }
    new_size = in_driver->allocated_size + increment;

    /* Reallocate the cop_driver */

    local_driver = (cop_driver *) realloc( in_driver, new_size );
    if( local_driver != in_driver ) free( in_driver );
    if( local_driver != NULL ) local_driver->allocated_size = new_size;

    return( local_driver );
}


