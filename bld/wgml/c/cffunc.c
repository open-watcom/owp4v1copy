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
* Description:  Implements the functions declared in cffunc.h:
*                   find_cumulative_index();
*                   get_functions()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1
#include <stdlib.h>
#include <string.h>
#include "cffunc.h"
#include "common.h"

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

/*  Function get_functions().
 *  Construct a functions_block containing the data in the P-buffer.
 *
 *  Parameter:
 *      in_file points to the start of a P-buffer
 *
 *  Parameter is changed:
 *      in_file will point to the start of the PausesBlock on success
 *      the status of in_file is unpredictable on failure
 *
 *  Returns:
 *      a pointer to a functions_block containing the processed data on success
 *      NULL on failure
 */

functions_block * get_functions( FILE * in_file)
{
    uint8_t             code_count;
    uint8_t *           current     = NULL;
    uint8_t             i;
    uint8_t             nulls[2];
    functions_block *   out_block   = NULL;
    uint8_t             p_count;
    char                test_char;

    p_count = 0;
    test_char = fgetc( in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( out_block );
    while( test_char == 80 ) {
        ++p_count;
        fseek( in_file, 80, SEEK_CUR );
        if( ferror( in_file ) || feof( in_file ) ) return( out_block );
        test_char = fgetc( in_file );
        if( ferror( in_file ) || feof( in_file ) ) return( out_block );
    }

    /* There should always be at least one P-buffer */

    if( p_count == 0 ) {
        puts( "No P-buffer found." );
        return( out_block );
    }

    /* Rewind the file by 81 bytes per P-buffer plus 1 */

    fseek( in_file, -81 * p_count - 1, SEEK_CUR );
    if( ferror( in_file ) || feof( in_file ) ) return( out_block );

    /* Get the first entry in the first P-buffer */

    test_char = fgetc( in_file );    
    if( ferror( in_file ) || feof( in_file ) ) return( out_block );
    if( test_char != 80 ) {
        puts( "P-buffer rewind failed." );
        return( out_block );
    }

    fread( &code_count, sizeof( code_count ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( out_block );

    /* Skip the nulls before the first CodeBlock, and ensure they are nulls */
    
    fread( &nulls, sizeof( nulls ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) return( out_block );

    if( ferror( in_file ) || feof( in_file ) ) {
        printf_s( "FunctionsBlock count not followed by nulls: %i\n", nulls );
        return( out_block );
    }

    /* Allocate out_block so that it has room for:
     *   the functions_block as such
     *   code_count * code_blocks
     *   p_block * 80 data bytes
     *   minus the size of out_block->count, which is stored separately
     *   and the size of nulls, which is not part of out_block
     */

    out_block = (functions_block *) malloc( sizeof( functions_block ) + code_count * sizeof( code_block ) + 80 * p_count - sizeof( out_block->count ) - sizeof( nulls ) );
    if( out_block == NULL) return( out_block );

    out_block->count = code_count;
    out_block->code_blocks = (code_block * ) ((char *) out_block + sizeof( functions_block )); 
    current = (uint8_t *) out_block->code_blocks + code_count * sizeof( code_block );

    /* Now get the data into the buffer */

    fread( current, 80 - sizeof( out_block->count ) - sizeof( nulls ), 1, in_file );
    if( ferror( in_file ) || feof( in_file ) ) {
        free( out_block );
        out_block = NULL;
        return( out_block );
    }

    current = current + 80 - sizeof( out_block->count ) - sizeof( nulls );

    if( p_count > 1 ) {
        for( i = 1; i < p_count; i++ ) {
            test_char = fgetc( in_file );    
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_block );
                out_block = NULL;
                return( out_block );
            }

            if( test_char != 80 ) {
                puts( "P-buffer terminated prematurely." );
                return( out_block );
            }

            fread( current, 80, 1, in_file );
            if( ferror( in_file ) || feof( in_file ) ) {
                free( out_block );
                out_block = NULL;
                return( out_block );
            }

            current = current + 80;
        }
    }

    /* Now extract the CodeBlocks, if any */

    if( out_block->count == 0 ) {
        out_block->code_blocks = NULL;
    } else {

       /* Set current to the start of the data */ 
             
        current = (uint8_t *) out_block->code_blocks + code_count * sizeof( code_block );
        for( i = 0; i < out_block->count; i++ ) {

            /* Skip the nulls */

            if( memcmp( current, "\0\0\0\0", 4) ) {
                printf_s( "CodeBlock %i doesn't start with four nulls\n", i );
                free( out_block );
                out_block = NULL;
                return( out_block );
            }
            current += 4;

            /* Get the number of bytes to copy */
            
            memcpy_s( &out_block->code_blocks[i].max_index, 2, current, 2 );
            current += 2;

            /* Adjust current: max_index is 1 less than the number of bytes */
            
            out_block->code_blocks[i].function = current;
            current += out_block->code_blocks[i].max_index + 1;
        }    

        /* Now set the cumulative_index fields */

        out_block->code_blocks[0].cumulative_index = 0x0000;
        if( out_block->count > 1 ) {
            for( i = 1; i < out_block->count; i++ ) {
                out_block->code_blocks[i].cumulative_index = out_block->code_blocks[i-1].cumulative_index + out_block->code_blocks[i-1].max_index;
            }
        }
    }

    return( out_block );
}
