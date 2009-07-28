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
* Description:  Implements the functions used to parse .COP files:
*                   cop_in_trans()
*                   cop_out_trans()
*                   cop_setup()
*                   cop_teardown()
*                   cop_text_width()
*                   cop_ti_table()
*                   cop_tr_table()
*                   fb_dbox()
*                   fb_document()
*                   fb_document_page()
*                   fb_finish()
*                   fb_hline()
*                   fb_new_section()
*                   fb_output_textline()
*                   fb_position()
*                   fb_start()
*                   fb_vline()
*               plus these local variables:
*                   bin_fonts
*                   cur_token
*                   ti_table
*                   tr_table
*               and these local functions:
*                   compute_metrics()
*                   find_cop_font()
*                   find_dev_font()
*                   find_style()
*                   find_switch()
*                   free_opt_fonts()
*                   get_cop_device()
*                   get_cop_driver()
*                   get_cop_font()
*                   get_next_token()
*                   scale_basis_to_horizontal_base_units()
*                   update_translate_table()
*                   validate_token()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent and for how the various function blocks are used.
*               This should help in most cases.
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* Allocate storage for global variables. */

#define global
#include "copfiles.h"
#undef  global

/* copfiles.h is included by several of these headers. If it is not included
 * first, its guard will prevent the global variables from being allocated.
 */

#include "copdev.h"
#include "copdrv.h"
#include "copfon.h"
#include "cophdr.h"
#include "devfuncs.h"
#include "findfile.h"
#include "gvars.h"
#include "outbuff.h"
#include "wgml.h"

/* Local data. */

static cop_font        *   bin_fonts;       // binary fonts being used (linked list)
static record_buffer   *   cur_token;       // Current token.
static uint8_t             ti_table[0x100]; // .TI-controlled translation table
static uint8_t             tr_table[0x100]; // .TR-controlled translation table

/* Local function definitions. */

/* Function scale_basis_to_horizontal_base_units().
 * Converts a length expressed in scale_basis units to the same length
 * expressed in horizontal_base_units.
 *
 * Parameter:
 *      in_units contains the length in scale_basis units.
 *      in_font points to the font to use.
 *
 * Returns:
 *      the same length in horizontal_base_units.
 */
 
static uint32_t scale_basis_to_horizontal_base_units( uint32_t in_units, \
                                                       wgml_font * in_font )
{
    uint32_t    divisor;
    uint64_t    units;
    uint32_t    width;

    /* The conversion is done using this formula: 
     * horizontal_base_units * font_height/100 * in_units
     * ------------------------------------------------
     *                     scale_basis
     * font_height is reduced from centipoints to points. This is done by
     * adjusting the divisor to avoid loss-of-precision problems with
     * integer arithmetic.
     */

    divisor = (in_font->bin_font->scale_basis * 100);
    units = (bin_device->horizontal_base_units * in_font->font_height);
    units *= in_units;
    width = units / divisor;

    /* This rounds width up if the division did not produce an integer and
     * the decimal part, if computed, would be 0.1 or greater. This produces
     * correct results when the tested with multiple copies of the same character,
     * but may need to be modified (or the entire algorithm reconsidered) when
     * side-by-side comparisons of wgml 4.0 and our wgml become possible.
     */

    if( ((units % divisor) * 10) >= divisor ) width++;

    return( width );
}

/* Function compute_metrics().
 * Computes and initializes the default_width, line_height and line_space values
 * for the given wgml_font instance.
 *
 * Parameter:
 *      in_font points to the given wgml_font instance.
 */

static void compute_metrics( wgml_font * in_font )
{
    uint32_t    glyph_height;
    uint32_t    height;

    /* Compute the default character width. */

    if( in_font->bin_font->scale_basis == 0 ) {

        /* The default_width is the char_width. */

        in_font->default_width = in_font->bin_font->char_width;

    } else {

        /* The default_width is the char_width properly scaled. */

        in_font->default_width = scale_basis_to_horizontal_base_units( \
                                        in_font->bin_font->char_width, in_font );
    }

    /* Compute the line height and space values. */

    if( in_font->font_height == 0 ) {

        /* Use the line_height and line_space values from the :FONT block. */

        in_font->line_space = in_font->bin_font->line_space;
            in_font->line_height = in_font->bin_font->line_height + \
                                                            in_font->line_space;
    } else {

        /* Use the font_height, font_space, and vertical_base_units values.
         * The formula is:
         *    (font_height + font_space) * vertical_base_units
         *    ------------------------------------------------
         *                         7200
         * where the divisor is 72 points/inch times the correction of
         * (font_height + font_space) from centipoints to points.
         */


        height = (in_font->font_height + in_font->font_space) * \
                                                bin_device->vertical_base_units;

        in_font->line_height = height / 7200;

        /* The rounding criteria may need to be tweaked when side-by-side
         * comparisons of wgml 4.0 and our wgml become possible.
         */

        if( (height % 7200) > 0 ) in_font->line_height++;

        /* If font_space is "0", then line_space will be "0". */

        in_font->line_space = 0;
        if( in_font->font_space > 0) {

            /* glyph_height is set to the number of vertical_base_units
             * corresponding to the value of font_height (and so, presumably,
             * to the amount of vertical space occupied by the glyphs).
             */

            height = in_font->font_height * bin_device->vertical_base_units;
            glyph_height = height / 7200;

            /* The rounding criteria may need to be tweaked when side-by-side
             * comparisons of wgml 4.0 and our wgml become possible.
             */

            if( (height % 7200) >= 3600 ) glyph_height++;

            /* The value of font_space is the difference between the computed
             * value of font_height and the value of glyph_height. 
             */

            in_font->line_space = in_font->line_height - glyph_height;
        }
    }

    return;
}

/* Function get_cop_device().
 * Converts the defined name of a :DEVICE block into a cop_device struct
 * containing the information in that :DEVICE block.
 *
 * Parameter:
 *      in_name points to the defined name of the device.
 *
 * Globals Used:
 *      try_file_name contains the name of the device file, if found.
 *      try_fp contains the FILE * for the device file, if found.
 *
 * Return:
 *      on success, a cop_device instance containing the data.
 *      on failure, a NULL pointer.
 */

static cop_device * get_cop_device( char const * in_name )
{
    cop_device      *   out_device  = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the device %s was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :DEVICE block. */

    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:

        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_device );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_dev_file( try_fp ) ) {
            out_msg( "Device library problem: file given for device %s does not" \
                            " encode a device:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_device = parse_device( try_fp );
        if( out_device == NULL ) \
            out_msg( "Device library problem: file given for device %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_device );
}

/* Function get_cop_driver().
 * Converts the defined name of a :DRIVER block into a cop_driver struct
 * containing the information in that :DRIVER block.
 *
 * Parameter:
 *      in_name points to the defined name of the device.
 *
 * Returns:
 *      on success, a cop_driver instance containing the data.
 *      on failure, a NULL pointer.
 */

static cop_driver * get_cop_driver( char const * in_name )
{
    cop_driver      *   out_driver  = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the driver %s was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :DRIVER block. */

    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:

        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_driver );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_drv_file( try_fp ) ) {
            out_msg( "Device library problem: file given for driver %s does not" \
                            " encode a driver:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_driver = parse_driver( try_fp );
        if( out_driver == NULL ) \
            out_msg( "Device library problem: file given for driver %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_driver );
}

/* Function get_cop_font().
 * Converts the defined name of a :FONT block into a cop_font struct
 * containing the information in that :FONT block.
 *
 * Parameter:
 *      in_name points to the defined name of the font.
 *
 * Returns:
 *      on success, a cop_font instance containing the data.
 *      on failure, a NULL pointer.
 */

static cop_font * get_cop_font( char const * in_name )
{
    cop_font        *   out_font    = NULL;
    cop_file_type       file_type;

    /* Acquire the file, if it exists. */

    if( !search_file_in_dirs( (char *) in_name, "", "", ds_bin_lib ) ) {
        out_msg( "The file for the font '%s' was not found\n", in_name );
        err_count++;
        g_suicide();
    }

    /* Determine if the file encodes a :FONT block. */

    file_type = parse_header( try_fp );

    switch( file_type ) {
    case file_error:

        /* File error, including premature eof. */

        out_msg( "ERR_FILE_IO %d %s\n", errno, try_file_name );
        err_count++;
        g_suicide();

    case not_bin_dev:
    case not_se_v4_1:
    case dir_v4_1_se:

        /* Wrong type of file: something is wrong with the device library. */

        out_msg( "Device library corrupt or wrong version: %s\n", try_file_name );
        return( out_font );

    case se_v4_1_not_dir:

        /* try_fp was a same-endian version 4.1 file, but not a directory file. */

        if( !is_fon_file( try_fp ) ) {
            out_msg( "Device library problem: file given for font %s does not" \
                            " encode a font:\n  %s\n", in_name, try_file_name );
            break;
        }

        out_font = parse_font( try_fp, in_name );
        if( out_font == NULL ) \
            out_msg( "Device library problem: file given for font %s appears" \
                            " to be corrupted:\n  %s\n", in_name, try_file_name );
        break;

    default:

        /* parse_header() returned an unknown value. */

        out_msg("wgml internal error\n");
        err_count++;
        g_suicide();
    }

    return( out_font );
}

/* Function find_cop_font().
 * Finds the cop_font instance for the requested font, creating and adding it
 * to the bin_fonts list if necessary.
 *
 * Parameter:
 *      in_name is the defined name of the desired binary :FONT block.
 *
 * Returns:
 *      a pointer to the cop_font instance on success.
 *      a NULL pointer on failure.
 *
 * Note:
 *      the comparison is case-sensitive for compatability with wgml 4.0.
 */

static cop_font * find_cop_font( char const * in_name )
{
    cop_font    *   current = NULL;
    cop_font    *   retval  = NULL;

    current = bin_fonts;
    while( current != NULL ) {
        if( !strcmp( in_name, current->defined_name ) ) {
            retval = current;
            break;
        }
        current = current->next_font;
    }

    if( retval == NULL ) {
        retval = get_cop_font( in_name );
        if( retval == NULL ) {
            out_msg( "Designated font not found: %s\n", in_name );
            err_count++;
            g_suicide();
        }
        retval->next_font = bin_fonts;
        bin_fonts = retval;
    }

    return( retval );
}

/* Function find_dev_font().
 * Finds the device_font instance for the requested font.
 *
 * Parameter:
 *      in_name is the defined name of the desired binary :DEVICEFONT block.
 *
 * Returns:
 *      a pointer to the device_font instance on success.
 *      a NULL pointer on failure.
 *
 * Note:
 *      the comparison is case-sensitive for compatability with wgml 4.0.
 */

static device_font * find_dev_font( char const * in_name )
{
    devicefont_block    *   current = NULL;
    device_font         *   retval  = NULL;
    int                     i;

    current = &bin_device->devicefonts;
    for( i = 0; i < current->font_count; i++ ) {
        if( !strcmp( in_name, current->fonts[i].font_name ) ) {
            retval = &current->fonts[i];
            break;
        }
    }

    if( retval == NULL ) {
        out_msg( "Designated :DEVICEFONT block not found: %s\n", in_name );
        err_count++;
        g_suicide();
    }

    return( retval );
}

/* Function find_style().
 * Finds the fonstyle_block instance for the requested font style.
 *
 * Parameter:
 *      in_name is the defined name of the desired binary :FONTSTYLE block.
 *
 * Returns:
 *      a pointer to the fonstyle_block instance on success.
 *      a NULL pointer on failure.
 *
 * Note:
 *      the comparison is case-insensitive for compatability with wgml 4.0.
 */

static fontstyle_block * find_style( char const * in_name )
{
    fontstyle_group  *   current = NULL;
    fontstyle_block  *   retval  = NULL;
    int                 i;

    current = &bin_driver->fontstyles;
    for( i = 0; i < current->count; i++ ) {
        if( !stricmp( in_name, current->fontstyleblocks[i].type ) ) {
            retval = &current->fontstyleblocks[i];
            break;
        }
    }

    if( retval == NULL ) {
        out_msg( "Designated :FONTSTYLE block not found: %s\n", in_name );
        err_count++;
        g_suicide();
    }

    return( retval );
}

/* Function find_switch().
 * Finds the fontswitch_block instance for the requested font switch.
 *
 * Parameter:
 *      in_name is the defined name of the desired binary :FONTSWITCH block.
 *
 * Returns:
 *      a pointer to the fontswitch_block instance on success.
 *      a NULL pointer on failure.
 *
 * Note:
 *      the comparison is case-insensitive for compatability with wgml 4.0.
 */

static fontswitch_block * find_switch( char const * in_name )
{
    fontswitch_funcs    *   current = NULL;
    fontswitch_block    *   retval  = NULL;
    int                     i;

    current = &bin_driver->fontswitches;
    for( i = 0; i < current->count; i++ ) {
        if( !stricmp( in_name, current->fontswitchblocks[i].type ) ) {
            retval = &current->fontswitchblocks[i];
            break;
        }
    }

    if( retval == NULL ) {
        out_msg( "Designated :FONTSWITCH block not found: %s\n", in_name );
        err_count++;
        g_suicide();
    }

    return( retval );
}

/* Function free_opt_fonts().
 * Frees all memory allocated in setting up the opt_fonts global variable.
 */

static void free_opt_fonts( void )
{
    opt_font    *   old;

    while( opt_fonts != NULL) {
        old = opt_fonts;

        if( old->name != NULL) {
            mem_free( old->name );
            old->name = NULL;
        }

        if( old->style != NULL) {
            mem_free( old->style );
            old->style = NULL;
        }
        opt_fonts = opt_fonts->nxt;
        mem_free( old );
    }

    return;
}

/* Function get_next_token().
 * Extracts the next token from the buffer and returns in the supplied
 * record_buffer *.
 *
 * Parameters:
 *      buffer points to the first character to be processed.
 *      count contains the number of characters in the buffer.
 *      in_out points to the record_buffer to be loaded with the token.
 *
 * Parameter Modified:
 *      if a token is found, then in_out.current will contain its length
 *          and in_out.text will point to its first character.
 *      if a token is not found, then in_out.current will be "0".
 *
 * Returns:
 *      The number of characters processed, including whitespace.
 *
 * Notes:
 *      mem_realloc() will not return unless it succeeds.
 *      in_out.text will have null terminator (for use in error messages),
 *          but in_out.current does not include it.
 */

static uint32_t get_next_token( uint8_t * buffer, uint32_t count, \
                                        record_buffer * in_out )
{
    int     i;
    int     j;
    size_t  token_length;

    in_out->current = 0;

    for( i = 0; i < count; i++ ) {
        if( isspace( buffer[i] ) ) continue;

        /* A token! */

        token_length = 0;
        for( j = i; j < count; j++) {
            if( isspace( buffer[j] ) ) break;
            token_length += 1;
        }

        if( in_out->length < (token_length + 1) ) {
            mem_realloc( in_out->text, token_length + 1 );
            in_out->length = token_length + 1;
        }
        memcpy_s( in_out->text, token_length, &buffer[i], token_length );
        in_out->current = token_length;
        in_out->text[in_out->current] = '\0';
        break;
    }

    return( j );
}

/* Function validate_token().
 * Ensures the token is valid, that is, is either a single character or a
 * two-character hexadecimal value and returns the character. 
 *
 * Parameter:
 *      token contains a pointer to the token. 
 *
 * Returns:
 *      The appropriate character value.
 *
 * Notes:
 *      This will treat "set" as an error; .ti processing must identify "set"
 *          through other means.
 *      Several errors are trapped and reported, after which the program quits.
 */

static uint8_t validate_token( record_buffer * token )
{
    uint8_t retval;
    uint8_t hex_string[3];

    if( token->current > 2 ) {

        /* The maximum allowed token length is 2. */

        out_msg( "A single character or a two character hexadecimal value "\
                                    "must be specified: '%s'\n", token->text );
        err_count++;
        g_suicide();
    }

    /* If the token length is 2, it must be a hexadecimal number. */

    if( token->current == 2 ) {

        if( !isxdigit( token->text[0] ) || !isxdigit( token->text[1] ) ) {
            out_msg( "A single character or a two character hexadecimal value "\
                                    "must be specified: '%s'\n", token->text );
            err_count++;
            g_suicide();
        }

        hex_string[0] = token->text[0];
        hex_string[1] = token->text[1];
        hex_string[2] = '\0';

        retval = (uint8_t) strtol( hex_string, NULL, 16 );
    } else {
        retval = *token->text;
    }

    return( retval );
}

/* Function update_translate_table().
 * Use the given data to update the desired table.
 *
 * Parameters
 *      is_ti_table is "true" if the ti_table is to be updated, "false"
 *          if the tr_table is to be updated.
 *      data contains any data associated with the .TI or .TR control word.
 *      count contains the number of bytes to process.
 */

static void update_translate_table( bool is_ti_table, uint8_t * data, \
                                     uint32_t count )
{
    bool        first_found = false;
    bool        no_data     = true;
    int         i           = 0;
    uint8_t *   table       = NULL;
    uint8_t     token_char;
    uint8_t     first_char;

    /* Set up the variables. count must be "0" if there is no data. no_data
     * will cause the table to be reset if it is still "true" after the loop.
     */

    if( is_ti_table ) table = ti_table;
    else table = tr_table;
    if( data == NULL ) count = 0;

    if( count != 0 ) {

        /* Determine if this is a ".ti set" instance. */

        if( is_ti_table ) {

            /* Get the first token, if any. */

            i = get_next_token( data, count, cur_token );

            /* If the length of the token is "3", see if it is "set". */

            if( cur_token->current == 3 ) {
                if( !memicmp( cur_token->text, "set", 3 ) ) {

                    /* Get the next token, if any. */            

                    i += get_next_token( &data[i], count - i, cur_token );

                    if( cur_token->current == 0 ) {

                        /* If there was no token, clear the input escape. */

                        ProcFlags.in_trans = false;
                        in_esc = ' ';
                    } else {

                        /* Only a single character is valid; hex digits are
                         * ignored by wgml 4.0.
                         */

                         if( cur_token->current > 1 ) {
                            out_msg( ".ti set can only be used with a single "\
                                     "character: '%s'\n", cur_token->text );
                            err_count++;
                            g_suicide();
                        }

                        /* Set the escape character. */

                        ProcFlags.in_trans = true;
                        in_esc = *cur_token->text;

                        /* wgml 4.0 ignores any additional tokens. */

                        get_next_token( &data[i], count - i, cur_token );
                        if( cur_token->current != 0 ) {
                            uint8_t * tail;

                            tail = (uint8_t *) mem_alloc( count - i + 1);
                            memcpy_s( tail, count - i, &data[i], count - i );
                            tail[count - i] = '\0';
                            out_msg( \
    ".ti set cannot be used to set a translation: '%s'\n", tail );
                            mem_free( tail );
                            err_count++;
                            g_suicide();
                        }
                    }

                    /* Processing is done if this was ".ti set". */

                    return; 
                }
            }

            /* This was not ".ti set", and so, if not empty, the token found
             * is either a first_char or invalid.
             */

            if( cur_token->current != 0 ) {
                first_char = validate_token( cur_token );
                first_found = true;
                no_data = false;
            }
        }
            
        /* This loop only deals with the non-".ti set" forms. */

        for( ; i < count; i++ ) {

            /* Get the next token, if any. */

            i += get_next_token( &data[i], count - i, cur_token );

            /* If there was no token, then we are done. */

            if( cur_token->current == 0 ) break;

            /* Validate the token and note that valid data was found. */

            token_char = validate_token( cur_token );
            no_data = false;

            /* If we have an unused first_char, then we just found the char it
             * is to be converted into. Otherwise, we found a new first_char.
             */

            if( first_found ) {
                table[first_char] = token_char;
                first_found = false;
            } else {
                first_char = token_char;
                first_found = true;
            }
        }
    }

    /* If first_found is true at this point, then a single character was
     * found at the end of the data and the table must be updated to return
     * the same character.
     */

    if( first_found ) table[first_char] = first_char;

    /* If there was no data, reset the table. */

    if( no_data ) for( i = 0; i < 0x100; i++ ) table[i] = i;

    return;
}

/* Extern function definitions */

/* Function cop_in_trans().
 * Translates the given character per the tables associated with the .TI
 * command word and the various :INTRANS blocks.
 *
 * Parameter:
 *      in_char contains the character to be translated.
 *
 * Returns:
 *      The appropriate character, which may be the same as in_char.
 */

uint8_t cop_in_trans( uint8_t in_char, uint8_t font )
{
    intrans_block   *   block   = NULL;
    uint8_t             retval;

    if( font > wgml_font_cnt ) font = 0;
    retval = ti_table[in_char];

    block = wgml_fonts[font].bin_font->intrans;
    if( retval == in_char ) if( block != NULL ) retval = block->table[in_char];

    block = bin_device->intrans;
    if( retval == in_char ) if( block != NULL ) retval = block->table[in_char];

    return( retval );
}

/* Function cop_out_trans().
 * Copies text to in_out.text, translating each character per the tables
 * associated with the .TR command word and any active :OUTTRANS block.
 *
 * Parameters:
 *      text is a pointer to the text to be translated.
 *      count contains the number of bytes pointed to by text.
 *      in_out is the record_buffer to use to return the translated text.
 *      font contains the font whose :OUTTRANS table, if any, is to be used. 
 *
 * Returns:
 *      in_out, with in_out.text modified to contain the translated text.
 *
 * Note:
 *      in_out.text may be reallocated; however, since mem_realloc() halts
 *      program execution on error, in_out.text will be correct if this
 *      function returns.
 */

record_buffer * cop_out_trans( uint8_t * text, uint32_t count, \
                               record_buffer * in_out, uint8_t font )
{
    outtrans_block  *   block   = NULL;
    uint8_t             byte;
    uint32_t            i;
    uint32_t            j;
    uint32_t            k;

    if( font > wgml_font_cnt ) font = 0;

    k = 0;
    for( i = 0; i < count; i++ ) {
        byte = tr_table[text[i]];
        if( byte == text[i] ) {
            block = wgml_fonts[font].bin_font->outtrans;
            if( block == NULL ) block = bin_device->outtrans;
            if( block == NULL ) {
                if( k >= in_out->length ) {
                    in_out->length *= 2;
                    in_out->text = (uint8_t *) mem_realloc( in_out->text, \
                                                            in_out->length );
                }
                in_out->text[k] = byte;
                k++;
            } else {
                if( block->table[text[i]] == NULL ) {
                    if( k >= in_out->length ) {
                        in_out->length *= 2;
                        in_out->text = (uint8_t *) mem_realloc( in_out->text, \
                                                                in_out->length );
                    }
                    in_out->text[k] = byte;
                    k++;
                } else {
                    for( j = 0; j < block->table[text[i]]->count; j++ ) {
                        if( k >= in_out->length ) {
                            in_out->length *= 2;
                            in_out->text = (uint8_t *) mem_realloc( \
                                                in_out->text, in_out->length );
                        }
                        in_out->text[k] = block->table[text[i]]->data[j];
                        k++;
                    }
                }
            }
        } else {
            if( k >= in_out->length ) {
                in_out->length *= 2;
                in_out->text = (uint8_t *) mem_realloc( in_out->text, \
                                                        in_out->length );
            }
            in_out->text[k] = byte;
            k++;
        }
    }
    in_out->current = k;

    return( in_out );
}

/* Function cop_setup().
 * Initializes the various globals specific to the binary device library
 * subsystem. And the local.
 */

void cop_setup( void )
{
    default_font    *   cur_def_fonts   = NULL;
    device_font     *   cur_dev_font    = NULL;
    int                 font_base       = 0;
    int                 gen_cnt         = 0;
    int                 i;
    int                 j;
    opt_font        *   cur_opt         = NULL;
    wgml_font           def_font;

    /* A "device" option must have been processed,
     * and it must have provided a device name.
     */

    if( (dev_name == NULL) || !strcmp(dev_name, "''") ) {
        out_msg( "A device name is required!\n" );
        err_count++;
        g_suicide();
    }

    /* Set the globals to known values. */

    bin_device = NULL;
    bin_driver = NULL;
    has_aa_block = false;
    ps_device = false;
    wgml_font_cnt = 0;
    wgml_fonts = NULL;

    /* Initialize the local variables. */

    bin_fonts = NULL;

    cur_token = (record_buffer *) mem_alloc( sizeof( record_buffer ) );
    cur_token->current = 0;
    cur_token->length = 10;
    cur_token->text = (uint8_t *) mem_alloc( cur_token->length );

    for( i = 0; i < 0x100; i++) {
        ti_table[i] = i;
        tr_table[i] = i;
    }

    /* Emit the expected message. */

    out_msg( "Processing device information\n" );

    /* Process the device. */

    bin_device = get_cop_device( dev_name );

    if( bin_device == NULL ) {
        out_msg( "Designated device not found: %s\n", dev_name );
        err_count++;
        g_suicide();
    }

    /* The value of horizontal_base_units cannot be "0". */

    if( bin_device->horizontal_base_units == 0 ) {
        out_msg( \
        "Device library error: horizontal_base_units cannot have value '0'\n" );
        err_count++;
        g_suicide();
    }

    /* The value of vertical_base_units cannot be "0". */

    if( bin_device->vertical_base_units == 0 ) {
        out_msg( \
            "Device library error: vertical_base_units cannot have value '0'\n" );
        err_count++;
        g_suicide();
    }

    /* A driver name must exist. */

    if( bin_device->driver_name == NULL ) {
        out_msg( "Device library error: driver name not provided\n" );
        err_count++;
        g_suicide();
    }
    bin_driver = get_cop_driver( bin_device->driver_name );

    if( bin_driver == NULL ) {
        out_msg( "Designated driver not found: %s\n", bin_device->driver_name );
        err_count++;
        g_suicide();
    }

    /* Attribute x_positive in :PAGEADDRESS cannot be "no", since horizontal
     * positioning does not check this value or react to it. This prevents
     * the generation of negative values for horizontal positions.
     */

    if( bin_driver->x_positive == 0 ) {
        out_msg( "The value 'no' is not supported for attribute 'x_positive'\n" );
        err_count++;
        g_suicide();
    }

    /* If attribute y_positive in :PAGEADDRESS was "no", then attribute
     * y_start in :PAGESTART must not be "0". This prevents the generation
     * of negative values for vertical positions.
     */

    if( bin_driver->y_positive == 0 ) {
        if( bin_device->y_start == 0 ) {
            out_msg( "Vertical start position cannot be 0\n" );
            err_count++;
            g_suicide();
        }
    }

    /* Set has_aa_block to "true" if the driver defines the
     * :ABSOLUTEADDRESS block.
     */

    if( bin_driver->absoluteaddress.text != NULL ) has_aa_block = true;

    /* Set ps_device to "true" if the driver name begins with "ps" or "PS". */

    if( !_strnicmp( bin_device->driver_name, "ps", 2 ) ) ps_device = true;

    /* Get the highest font_number and reduce it by one so it contains the
     * highest valid array index.
     */

    wgml_font_cnt = bin_device->defaultfonts.font_count;
    wgml_font_cnt--;

    /* Adjust the highest valid array index as needed to accomodate the font
     * numbers given with the FONT command-line option.
     */

    cur_opt = opt_fonts;
    while( cur_opt != NULL ) {
        if( cur_opt->font > wgml_font_cnt ) {
           wgml_font_cnt = cur_opt->font;
        }
        cur_opt = cur_opt->nxt;
    }

    /* The value needed for the upper bound of the zero-based array is the
     * highest valid array index plus 1.
     */

     wgml_font_cnt++;

    /* If either :BOX or :UNDERSCORE provided a font name, increment the count.
     * If both :BOX and :UNDERSCORE provided a font name, increment the count
     * once if the font names were identical, twice if they were different.
     * But not if the device is PS: for PS, such fonts are never created.
     */

    if( !ps_device ) {
        if( bin_device->box.font_name == NULL ) {
            if( bin_device->underscore.specified_font && \
                                (bin_device->underscore.font_name != NULL) ) {
                gen_cnt++;
            }
        } else {
            gen_cnt++;
            if( bin_device->underscore.specified_font && \
                                (bin_device->underscore.font_name != NULL) ) {
                if( strcmp( bin_device->box.font_name, \
                                        bin_device->underscore.font_name ) ) {
                    gen_cnt++;
                }
            }
        }
    }

    font_base = wgml_font_cnt;
    wgml_font_cnt += gen_cnt;

    /* Initialize wgml_fonts, which is an array */

    wgml_fonts = (wgml_font * ) mem_alloc( wgml_font_cnt * sizeof( wgml_font ) );
    for( i = 0; i < wgml_font_cnt; i++ ) {
        wgml_fonts[i].bin_font = NULL;
        wgml_fonts[i].font_switch = NULL;
        wgml_fonts[i].font_pause = NULL;
        wgml_fonts[i].font_style = NULL;
        wgml_fonts[i].font_height = 0;
        wgml_fonts[i].font_space = 0;
    }

    /* Process the :DEFAULTFONT Blocks. */

    cur_def_fonts = bin_device->defaultfonts.fonts;
    for( i = 0; i < bin_device->defaultfonts.font_count; i++ ) {
        if( (cur_def_fonts[i].font_name == NULL) \
                            || (strlen( cur_def_fonts[i].font_name ) == 0) ) {
            continue; /* Do not initialize skipped font numbers. */
        } else {
            wgml_fonts[i].bin_font = find_cop_font( cur_def_fonts[i].font_name );
        }
        if( (cur_def_fonts[i].font_style == NULL) \
                            || (strlen( cur_def_fonts[i].font_style ) == 0) ) {
            wgml_fonts[i].font_style = find_style( "plain" );
        } else {
            wgml_fonts[i].font_style = find_style( cur_def_fonts[i].font_style );
        }
        wgml_fonts[i].font_height = cur_def_fonts[i].font_height;
        wgml_fonts[i].font_space = cur_def_fonts[i].font_space;

        if( cur_def_fonts[i].font_name == NULL ) {
            cur_dev_font = find_dev_font( cur_def_fonts[0].font_name );
        } else {
            cur_dev_font = find_dev_font( cur_def_fonts[i].font_name );
        }
        if( cur_dev_font->font_switch == NULL ) {
            wgml_fonts[i].font_switch = NULL;
        } else {
            wgml_fonts[i].font_switch = find_switch( cur_dev_font->font_switch );
        }
        wgml_fonts[i].font_pause = cur_dev_font->font_pause;
        if( cur_dev_font->resident == 0x00 ) wgml_fonts[i].font_resident = 'n';
        else wgml_fonts[i].font_resident = 'y';

        /* If scale_basis is not "0", then font_height must not be "0". */

        if( (wgml_fonts[i].bin_font->scale_basis != 0) && \
                                        (wgml_fonts[i].font_height == 0)) {
            out_msg( "For default font %i, the font_height attribute\n", i );
            out_msg( "must be specified when the font is scaled.\n" );
            err_count++;
            g_suicide();
        }

        compute_metrics( &wgml_fonts[i] );
    }

    /* Process the FONT command-line option instances. */

    cur_opt = opt_fonts;
    while( cur_opt != NULL ) {
        i = cur_opt->font;
        wgml_fonts[i].bin_font = find_cop_font( cur_opt->name );
        if( cur_opt->style == NULL ) {
            wgml_fonts[i].font_style = find_style( "plain" );
        } else {
            wgml_fonts[i].font_style = find_style( cur_opt->style );
        }
        wgml_fonts[i].font_height = cur_opt->height;
        wgml_fonts[i].font_space = cur_opt->space;
        cur_dev_font = find_dev_font( cur_opt->name );
        if( cur_dev_font->font_switch == NULL ) {
            wgml_fonts[i].font_switch = NULL;
        } else {
            wgml_fonts[i].font_switch = find_switch( cur_dev_font->font_switch );
        }
        wgml_fonts[i].font_pause = cur_dev_font->font_pause;
        if( cur_dev_font->resident == 0x00 ) wgml_fonts[i].font_resident = 'n';
        else wgml_fonts[i].font_resident = 'y';

        /* If scale_basis is not "0", then font_height must not be "0". */

        if( (wgml_fonts[i].bin_font->scale_basis != 0) && \
                                        (wgml_fonts[i].font_height == 0)) {
            out_msg( "For the FONT option with font %i, the font_height\n", i );
            out_msg( "attribute must be specified when the font is scaled.\n" );
            err_count++;
            g_suicide();
        }

        compute_metrics( &wgml_fonts[i] );
        cur_opt = cur_opt->nxt;
    }
    free_opt_fonts();

    /* Generate any entries required by the :BOX and/or :UNDERSCORE blocks.
     * Note that the font_number will become non-zero and will be used in
     * document processing instead of the font name. If the device is PS,
     * then gen_cnt will be "0" and no fonts will be generated.
     */

    switch( gen_cnt ) {
    case 0:
        break;
    case 1:
        if( bin_device->underscore.font_name != NULL ) {
            i = font_base;
            bin_device->underscore.font_number = font_base;
            wgml_fonts[i].bin_font = \
                                find_cop_font( bin_device->underscore.font_name );
            wgml_fonts[i].font_style = find_style( "plain" );
            wgml_fonts[i].font_height = 0;
            wgml_fonts[i].font_space = 0;
            cur_dev_font = find_dev_font( bin_device->underscore.font_name );
            if( cur_dev_font->font_switch == NULL ) {
                wgml_fonts[i].font_switch = NULL;
            } else {
                wgml_fonts[i].font_switch = \
                                        find_switch( cur_dev_font->font_switch );
            }
            wgml_fonts[i].font_pause = cur_dev_font->font_pause;
            wgml_fonts[i].font_resident = 'n';
            wgml_fonts[i].default_width = 1;
            wgml_fonts[i].line_height = 1;
            wgml_fonts[i].line_space = 0;

            /* The font used with the :UNDERSCORE block cannot be scaled. */

            if( wgml_fonts[i].bin_font->scale_basis != 0 ) {
                out_msg( "The UNDERSCORE block cannot specify a font which "\
                                                            "is scaled.\n" );
                err_count++;
                g_suicide();
            }

            break;
        }
        if( bin_device->box.font_name != NULL ) {
            i = font_base;
            bin_device->box.font_number = font_base;
            wgml_fonts[i].bin_font = find_cop_font( bin_device->box.font_name );
            wgml_fonts[i].font_style = find_style( "plain" );
            wgml_fonts[i].font_height = 0;
            wgml_fonts[i].font_space = 0;
            cur_dev_font = find_dev_font( bin_device->box.font_name );
            if( cur_dev_font->font_switch == NULL ) {
                wgml_fonts[i].font_switch = NULL;
            } else {
                wgml_fonts[i].font_switch = \
                                        find_switch( cur_dev_font->font_switch );
            }
            wgml_fonts[i].font_pause = cur_dev_font->font_pause;
            wgml_fonts[i].font_resident = 'n';
            wgml_fonts[i].default_width = 1;
            wgml_fonts[i].line_height = 1;
            wgml_fonts[i].line_space = 0;

            /* The font used with the :BOX block cannot be scaled. */

            if( wgml_fonts[i].bin_font->scale_basis != 0 ) {
                out_msg( "The BOX block cannot specify a font which is scaled.\n" );
                err_count++;
                g_suicide();
            }

            break;
        }
        break;
    case 2:
        if( bin_device->underscore.font_name != NULL ) {
            i = font_base;
            bin_device->underscore.font_number = font_base;
            wgml_fonts[i].bin_font = \
                                find_cop_font( bin_device->underscore.font_name );
            wgml_fonts[i].font_style = find_style( "plain" );
            wgml_fonts[i].font_height = 0;
            wgml_fonts[i].font_space = 0;
            cur_dev_font = find_dev_font( bin_device->underscore.font_name );
            if( cur_dev_font->font_switch == NULL ) {
                wgml_fonts[i].font_switch = NULL;
            } else {
                wgml_fonts[i].font_switch = \
                                        find_switch( cur_dev_font->font_switch );
            }
            wgml_fonts[i].font_pause = cur_dev_font->font_pause;
            wgml_fonts[i].font_resident = 'n';
            wgml_fonts[i].default_width = 1;
            wgml_fonts[i].line_height = 1;
            wgml_fonts[i].line_space = 0;

            /* The font used with the :UNDERSCORE block cannot be scaled. */

            if( wgml_fonts[i].bin_font->scale_basis != 0 ) {
                out_msg( "The UNDERSCORE block cannot specify a font which "\
                                                            "is scaled.\n" );
                err_count++;
                g_suicide();
            }

        }
        if( bin_device->box.font_name != NULL ) {
            font_base++;
            i = font_base;
            bin_device->box.font_number = font_base;
            wgml_fonts[i].bin_font = find_cop_font( bin_device->box.font_name );
            wgml_fonts[i].font_style = find_style( "plain" );
            wgml_fonts[i].font_height = 0;
            wgml_fonts[i].font_space = 0;
            cur_dev_font = find_dev_font( bin_device->box.font_name );
            if( cur_dev_font->font_switch == NULL ) {
                wgml_fonts[i].font_switch = NULL;
            } else {
                wgml_fonts[i].font_switch = \
                                        find_switch( cur_dev_font->font_switch );
            }
            wgml_fonts[i].font_pause = cur_dev_font->font_pause;
            wgml_fonts[i].font_resident = 'n';
            wgml_fonts[i].default_width = 1;
            wgml_fonts[i].line_height = 1;
            wgml_fonts[i].line_space = 0;

            /* The font used with the :BOX block cannot be scaled. */

            if( wgml_fonts[i].bin_font->scale_basis != 0 ) {
                out_msg( "The BOX block cannot specify a font which is scaled.\n" );
                err_count++;
                g_suicide();
            }

        }
        break;
    default:
        out_msg( "wgml internal error\n" );
        err_count++;
        g_suicide;
    }

    /* Ensure that font 0 was initialized */

    if( wgml_fonts[0].bin_font == NULL ) {
        out_msg( "Device Library Error: Font 0 not defined\n" );
        err_count++;
        g_suicide;
    }

    /* Fill in any skipped entries with the values used for wgml_font 0 */

    def_font.bin_font = wgml_fonts[0].bin_font;
    def_font.font_switch = wgml_fonts[0].font_switch;
    def_font.font_pause = wgml_fonts[0].font_pause;
    def_font.font_style = wgml_fonts[0].font_style;
    def_font.font_resident = wgml_fonts[0].font_resident;
    def_font.font_height = wgml_fonts[0].font_height;
    def_font.font_space = wgml_fonts[0].font_space;
    def_font.default_width = wgml_fonts[0].default_width;
    def_font.line_height = wgml_fonts[0].line_height;
    def_font.line_space = wgml_fonts[0].line_space;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( wgml_fonts[i].bin_font == NULL ) {
            wgml_fonts[i].bin_font = def_font.bin_font;
            wgml_fonts[i].font_switch = def_font.font_switch;
            wgml_fonts[i].font_pause = def_font.font_pause;
            wgml_fonts[i].font_style = def_font.font_style;
            wgml_fonts[i].font_resident = def_font.font_resident;
            wgml_fonts[i].font_height = def_font.font_height;
            wgml_fonts[i].font_space = def_font.font_space;
            wgml_fonts[i].default_width = def_font.default_width;
            wgml_fonts[i].line_height = def_font.line_height;
            wgml_fonts[i].line_space = def_font.line_space;
        }
    }

    /* Ensure that at least one binary :FONT block was processed. */

    if( bin_fonts == NULL ) {
        out_msg( "Device Library Error: No :FONT blocks loaded\n" );
        err_count++;
        g_suicide;
    }

    if( !has_aa_block ) {
        uint32_t    test_height;

        /* Verify that all line_height fields have the same value. */

        test_height = wgml_fonts[0].line_height;
        for( i = 1; i < wgml_font_cnt; i++ ) {
            if( test_height != wgml_fonts[i].line_height ) {
                out_msg( "     Computed line height values for devices\n" );
                out_msg( "     which rely on :NEWLINE blocks must be the\n" );
                out_msg( "     same for all fonts\n" );
                err_count++;
                g_suicide();
            }
        }
    }

    /* Initialize the width_table entries. This is done here because it is
     * not clear whether or not a more efficient method is needed. Note that
     * all wgml_font instances will have a valid table, and that different
     * wgml_font instances may have identical tables. However, since the
     * conversion takes font_height into account, tables based on the same
     * cop_font may still differ. Also set the base values for the "Em" and
     * "Device Unit" ("DV") Horizontal Space Units, and record the width of
     * the space character for quick reference. It is not clear if this is
     * actually necessary, but it is a bit faster than using the width_table
     * directly. 
     */

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( wgml_fonts[i].bin_font->width == NULL ) {
            for( j = 0; j < 0x100; j++ ) {
                wgml_fonts[i].width_table[j] = wgml_fonts[i].default_width;
            }
        } else {
            if( wgml_fonts[i].bin_font->scale_basis == 0 ) {
                for( j = 0; j < 0x100; j++ ) {
                    wgml_fonts[i].width_table[j] = \
                                        wgml_fonts[i].bin_font->width->table[j];
                }
            } else {
                for( j = 0; j < 0x100; j++ ) {
                    wgml_fonts[i].width_table[j] = \
                                    scale_basis_to_horizontal_base_units( \
                        wgml_fonts[i].bin_font->width->table[j], &wgml_fonts[i] );
                }
            }                
        }
        wgml_fonts[i].dv_base = wgml_fonts[i].width_table['0'];
        wgml_fonts[i].em_base = wgml_fonts[i].width_table['M'];
        wgml_fonts[i].spc_width = wgml_fonts[i].width_table[' '];
    }

    /* Initialize the dependent modules. */

    df_setup();
    ob_setup();

    return;
}

/* Function cop_teardown().
 * Frees all memory allocated by cop_setup().
 * This is where making cop_device, cop_driver and cop_font monolithic blocks of
 * allocated memory pays off!
 */

void cop_teardown( void )
{
    cop_font    *   old;

    if( bin_device != NULL ) {
        mem_free( bin_device );
        bin_device = NULL;
    }

    if( bin_driver  != NULL ) {
        mem_free( bin_driver );
        bin_driver = NULL;
    }

    if( bin_fonts != NULL) {
        while( bin_fonts != NULL) {
            old = bin_fonts;
            bin_fonts = bin_fonts->next_font;
            mem_free( old );
        }
    }

    if( wgml_fonts != NULL) {
        mem_free( wgml_fonts );
        wgml_fonts = NULL;
    }

    if( cur_token != NULL ) {
        if( cur_token->text != NULL ) {
            mem_free( cur_token->text);
            cur_token->text = NULL;
        }
        mem_free( cur_token );
        cur_token = NULL;
    }

    /* Release any memory allocated by the dependent modules. */

    df_teardown();
    ob_teardown();

    return;
}

/* Function cop_text_width().
 * Returns the width, in horizontal_base_units, of the text given.
 *
 * Parameters:
 *      text is a pointer to the first character.
 *      count is the number of characters.
 *      font is the font number of the available font to use.
 *
 * Returns:
 *      The sum of the widths of the count characters starting with *text.
 *
 * Note:
 *      This version simply adds up the widths, in horizontal_base_units, of
 *          the first count characters in text. For :FONT blocks without
 *          a :WIDTH block, it might be more efficient to use the
 *          product of default_width and count. However, many counts will be
 *          quite small, and the cost of determining whether or not the
 *          bin_font contains a width table must be considered.
 */
 
uint32_t cop_text_width( uint8_t * text, uint32_t count, uint8_t font )
{
    int             i;
    uint32_t        width;

    if( font > wgml_font_cnt ) font = 0;

    /* Compute the width. */

    width = 0;
    for( i = 0; i < count; i++ ) {
        width += wgml_fonts[font].width_table[text[i]];
    }

    return( width );
}


/* Function cop_ti_table().
 * Updates ti_table as specified by the data.
 *
 * Parameters:
 *      data contains any data associated with the .TI control word.
 *      count contains the number of bytes to process.
 *
 * Notes:
 *      The SET operand will be treated as an error by this version.
 *      Whatever terminated the .TI record must not be part of the data.
 */

void cop_ti_table( uint8_t * data, uint32_t count )
{
    update_translate_table( true, data, count );
    return;
}

/* Function cop_tr_table().
 * Updates tr_table as specified by the data.
 *
 * Parameters:
 *      data contains any data associated with the .TR control word.
 *      count contains the number of bytes to process.
 *
 * Note:
 *      Whatever terminated the .TR record must not be part of the data.
 */

void cop_tr_table( uint8_t * data, uint32_t count )
{
    update_translate_table( false, data, count );
    return;
}

/* Function fb_dbox().
 * Interprets the :DBOX block.
 *
 * Parameters:
 *      h_start contains the horizontal position.
 *      v_start contains the vertical position.
 *      h_len contains the horizontal extent.
 *      v_len contains the vertical extent.
 *
 * Prerequisites:
 *      The :DBOX block must exist.
 *      The :ABSOLUTEADDRESS block must exist.
 *
 * Notes:
 *      The :ABSOLUTEADDRESS block is required to position the print to the
 *          start of the line or box.
 *      The :DBOX block must exist because the box-drawing code should be
 *          checking this and drawing the box using either :HLINE and :VLINE
 *          or the :BOX block characters instead.
 */

void fb_dbox( uint32_t h_start, uint32_t v_start, uint32_t h_len, uint32_t v_len )
{
    fb_line_block( &(bin_driver->dbox), h_start, v_start, h_len, v_len, "DBOX" );
    return;
}

/* Function fb_document().
 * Performs the first document pass output seen when document processing begins.
 */

void fb_document( void )
{
    uint32_t    page_top;

    /* Interpret the DOCUMENT :PAUSE block. */

    if( bin_device->pauses.document_pause != NULL ) \
        df_interpret_device_functions( bin_device->pauses.document_pause->text );

    /* Interpret the DOCUMENT :INIT block. */

    if( bin_driver->inits.document != NULL ) \
                                        fb_init( bin_driver->inits.document );

    /* Perform the virtual %enterfont(0). */

    fb_enterfont();

    /* Perform the other actions needed at this point. */

    df_populate_driver_table();

    /* Initialize page_top.
     * Eventually, page_top (or its functionality) will become a global
     * variable initialized and used by the document layout code. The reason
     * for this is that, if y_positive is "0", then the correct value is the
     * top of the document page, which can only be determined from the page
     * depth given in the :LAYOUT section (:PAGE attribute depth). The value
     * computed here is the top of the device page.
     */

    page_top = bin_device->y_start;

    /* Set up for the first document page. */

    df_initialize_pages( page_top );

    return;
}

/* Function fb_document_page().
 * Interprets the :NEWPAGE block and increments the page number variable.
 *
 * Note:
 *      This function should be used for new document pages within a section,
 *      except as documented in the Wiki. The function fb_new_section() should
 *      be used at the start of each new section, and elsewhere as documented
 *      in the Wiki. 
 */

void fb_document_page( void )
{
    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* Interpret the DOCUMENT_PAGE :PAUSE block. */

    if( bin_device->pauses.docpage_pause != NULL ) \
        df_interpret_device_functions( bin_device->pauses.docpage_pause->text );

    /* Interpret the :NEWPAGE block. */

    df_interpret_driver_functions( bin_driver->newpage.text );

    /* Set up for a new document page. */

    df_increment_pages();

    return;
}

/* Function fb_finish().
 * Performs the processing which occurs after document processing ends.
 */

void fb_finish( void )
{
    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* If the END :FINISH block is present, interpret it. If the END
     * :FINISH block is not present, then, if the DOCUMENT :FINISH block
     * is present, interpret it.
     */

    if( bin_driver->finishes.end != NULL ) \
        df_interpret_driver_functions( bin_driver->finishes.end->text );
    else if( bin_driver->finishes.document != NULL ) \
        df_interpret_driver_functions( bin_driver->finishes.document->text );

    return;
}

/* Function fb_hline().
 * Interprets the :HLINE block.
 *
 * Parameters:
 *      h_start contains the horizontal position.
 *      v_start contains the vertical position.
 *      h_len contains the horizontal extent.
 *
 * Prerequisites:
 *      The :HLINE block must exist.
 *      The :ABSOLUTEADDRESS block must exist.
 *
 * Notes:
 *      The :ABSOLUTEADDRESS block is required to position the print to the
 *          start of the line or box.
 *      The :HLINE block must exist because the box-drawing code should be
 *          checking this and, in some cases, drawing the line using the :BOX
 *          block characters instead.
 */

void fb_hline( uint32_t h_start, uint32_t v_start, uint32_t h_len )
{
    fb_line_block( &(bin_driver->hline), h_start, v_start, h_len, 0, "HLINE" );
}

/* Function fb_new_section().
 * Performs the subsequent initial vertical positioning, as described in the
 * Wiki.
 *
 * Parameter:
 *      v_start contains the desired starting vertical position.
 *
 * Note:
 *      This function should not be invoked at the start of the file; instead,
 *      fb_position() should be used as it will do the first initial vertical
 *      positioning. This function should be used in place of fb_document_page()
 *      when a new section or other event occurs where its action is needed.
 */
 
void fb_new_section( uint32_t v_start )
{
    df_new_section( v_start );
    return;
}

/* Function fb_output_textline.
 * Sends the text_line passed to the device via the output buffer.
 *
 * Parameter:
 *      out_line points to a text_line instance specifying the text to be
 *          sent to the device.
 *
 * Note:
 *      This function deals with the normal output sequence only.
 *      It is expected that lines drawn with :BOX characters will require
 *          other, specialized fb_output_ functions.
 */

void fb_output_textline( text_line * out_line )
{
    int             i;
    text_chars  *   current;
    uint16_t        line_passes;

    /* Ensure out_line has at least one text_chars instance. */

    current = out_line->first;
    if( current == NULL ) {
        out_msg( "wgml internal error: output line with no text\n" );
        err_count++;
        g_suicide();
    }

    /* Determine the number of passes. */

    line_passes = 0;
    while( current != NULL ) {
        if( wgml_fonts[current->font_number].font_style->line_passes > \
                                                                line_passes ) {
                line_passes = \
                    wgml_fonts[current->font_number].font_style->line_passes;
        }
        current = current->next;
    }

    /* Do the first pass. */

    fb_first_text_pass( out_line );

    /* Do the subsequent passes */

    for( i = 1; i < line_passes; i ++ ) {
// still need to write this one!
    }

    return;
}

/* Function fb_position().
 * Initializes the print head position by calling two helper functions, which
 * allows them to use the internals of the interpreter module. This function
 * is extremely specialized.
 *
 * Parameters:
 *      h_start contains the horizontal position.
 *      v_start contains the vertical position.
 *
 * Notes:
 *      This function performs the first initial vertical positioning, as
 *          described in the Wiki.
 *      This is called at most twice: once to set the left margin at the
 *          start of document processing, and once if the first element is a
 *          paragraph (:P.) which has an indent.
 *      The initial vertical positioning should only be done the first time:
 *          v_start should have the same value on the second invocation as it
 *          did on the first.
 *      The initial horizontal positioning will behave slightly differently
 *          the first and second time this function is called.
 *      The internal state is updated without regard to whether any function
 *          blocks, or which function blocks, are interpreted.
 */

void fb_position( uint32_t h_start, uint32_t v_start )
{
    df_set_vertical( v_start );
    df_set_horizontal( h_start );
    return;
}

/* Function fb_start().
 * Performs the processing which occurs before document processing starts.
 * Indeed, wgml 4.0 does not even look for the document specification file
 * until the actions taken here have been completed!
 */

void fb_start( void )
{
    /* Interpret the START :PAUSE block and reset the function table. */

    if( bin_device->pauses.start_pause != NULL ) \
        df_interpret_device_functions( bin_device->pauses.start_pause->text );

    df_populate_device_table();

    /* Interpret the START :INIT block. */

    if( bin_driver->inits.start != NULL ) fb_init( bin_driver->inits.start );

    return;
}

/* Function fb_vline().
 * Interprets the :VLINE block.
 *
 * Parameters:
 *      h_start contains the horizontal position.
 *      v_start contains the vertical position.
 *      v_len contains the vertical extent.
 *
 * Prerequisites:
 *      The :VLINE block must exist.
 *      The :ABSOLUTEADDRESS block must exist.
 *
 * Notes:
 *      The :ABSOLUTEADDRESS block is required to position the print to the
 *          start of the line or box.
 *      The :VLINE block must exist because the box-drawing code should be
 *          checking this and, in some cases, drawing the line using the :BOX
 *          block characters instead.
 */

void fb_vline( uint32_t h_start, uint32_t v_start, uint32_t v_len )
{
    fb_line_block( &(bin_driver->vline), h_start, v_start, 0, v_len, "VLINE" );
    return;
}

