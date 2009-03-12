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
* Description:  Implements the functions declared in dfinterp.h:
*                   df_interpret_device_functions()
*                   df_interpret_driver_functions()
*                   df_populate_device_table()
*                   df_setup()
*                   df_teardown()
*                   fb_enterfont()
*                   fb_init()
*               as well as a macro
*                   MAX_FUNC_INDEX
*               some local typedefs and structs:
*                   df_data
*                   df_function
*                   parameters
*               some local variables:
*                   current_df_data
*                   current_function_table
*                   date_val
*                   device_function_table
*                   driver_function_table
*                   font_number
*                   pages
*                   staging
*                   tab_width
*                   textpass
*                   thickness
*                   time_val
*                   uline
*                   wgml_header
*                   x_address
*                   x_size
*                   y_address
*                   y_size
*               and a large number of local functions:
*                   char_convert()
*                   char_literal()
*                   df_add()
*                   df_bad_code()
*                   df_binary()
*                   df_cancel()
*                   df_clearPC()
*                   df_date()
*                   df_decimal()
*                   df_default_width()
*                   df_divide()
*                   df_do_nothing_char()
*                   df_do_nothing_num()
*                   df_dotab()
*                   df_endif()
*                   df_enterfont()
*                   df_font_height()
*                   df_font_number()
*                   df_font_outname1()
*                   df_font_outname2()
*                   df_font_resident()
*                   df_font_space()
*                   df_flushpage()
*                   df_getstrsymbol()
*                   df_getnumsymbol()
*                   df_hex()
*                   df_ifeqn()
*                   df_ifnen()
*                   df_ifeqs()
*                   df_ifnes()
*                   df_line_height()
*                   df_line_space()
*                   df_lower()
*                   df_out_text_device()
*                   df_page_depth()
*                   df_page_width()
*                   df_pages()
*                   df_recordbreak_device()
*                   df_recordbreak_driver()
*                   df_remainder()
*                   df_setsymbol()
*                   df_sleep()
*                   df_subtract()
*                   df_tab_width()
*                   df_text_device()
*                   df_text_driver()
*                   df_textpass()
*                   df_thickness()
*                   df_time()
*                   df_ulineoff()
*                   df_ulineon()
*                   df_wait()
*                   df_wgml_header()
*                   df_x_address()
*                   df_x_size()
*                   df_y_address()
*                   df_y_size()
*                   format_error()
*                   get_parameters()
*                   interpret_functions()
*                   numeric_literal()
*                   out_text_driver()
*                   process_parameter()
*                   resize_staging()
*                   skip_functions()
*
* Notes:        The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
*
*               Information on the device functions can be found in the
*               Wiki.
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1
#include <conio.h>
#include <dos.h>
#include <stdbool.h>
#include <string.h>

#include "devfuncs.h"
#include "outbuff.h"
#include "wgml.h"
#include "gvars.h"

/* Macros. */

/* Maximum index value for the function tables. */

#define MAX_FUNC_INDEX 0x3c

/* Local struct definitions. */

/* This keeps track of where we are in the compiled function block. */

typedef struct {
    bool        last_function_done;
    uint8_t *   base;
    uint8_t *   current;
    uint8_t     parameter_type;
    uint8_t     df_code;
} df_data;

/* Holds the offsets to the parameter(s). */

typedef struct {
    uint16_t    first;
    uint16_t    second;
} parameters;

/* Local typedef. */

/* All functions used in the function tables will use this signature. */

typedef void * (*df_function) (void);

/* Local data declarations. */

/* These are used to hold values returned by device functions. */

static bool             textpass        = false;
static bool             uline           = false;
static char         *   date_val        = NULL;
static char         *   time_val        = NULL;
static char             wgml_header[]   = "V4.0 PC/DOS";
static uint32_t         font_number     = 0;
static uint32_t         pages           = 0;
static uint32_t         tab_width       = 0;
static uint32_t         thickness       = 0;
static uint32_t         x_address       = 0;
static uint32_t         x_size          = 0;
static uint32_t         y_address       = 0;
static uint32_t         y_size          = 0;

/* These are used by the interpreter. */

static df_data          current_df_data;
static df_function      device_function_table[MAX_FUNC_INDEX + 1];
static df_function      driver_function_table[MAX_FUNC_INDEX + 1];
static df_function  *   current_function_table = NULL;

/* This is used to create text for output when necessary. */

static record_buffer    staging = { 0, 0, NULL };

/* Local function declaration (used below before it is defined). */

static void interpret_functions( uint8_t * in_function );

/* Local function definitions. */

/* Function char_convert().
 * This function returns a dynamically-allocated copy of the parameter. If the
 * parameter is a NULL pointer, then it returns a pointer to an empty string.
 *
 * Parameter:
 *      a pointer to the alleged character value.
 *
 * Returns:
 *      a pointer to a dynamically-allocated string.
 */

static char * char_convert( char * in_val )
{
    char *  ret_val = NULL;

    if( in_val == NULL ) {
        ret_val = (char *) mem_alloc( 1 );
        ret_val[0] = '\0';
    } else {
        ret_val = (char *) mem_alloc( strlen( in_val ) + 1 );
        strcpy_s( ret_val, strlen( in_val ) + 1, in_val );
    }

    return( ret_val );
}

/* Function format_error().
 * This function handles all format errors found when interpreting a function
 * block. It never returns to the caller. It should never actually run. 
 */

void format_error( void )
{
    out_msg( "Device library error: bad format\n" );
    err_count++;
    g_suicide();
}

/* Function resize_staging().
 * This function resizes the buffer used to construct the output text
 * safely.
 *
 * Parameter:
 *      count contains the minimum required size.
 *
 * Note:
 *      mem_realloc() exits the program if it fails. Thus, this function
 *      will either succeed or the program will end.
 */

static void resize_staging( size_t count )
{
    if( staging.length < count ) {
        staging.data = (uint8_t *) mem_realloc( staging.data, count );
        staging.length = count;
    }

    return;
}

/* Function df_do_nothing_char().
 * Returns an empty string. Used in the function tables for device functions
 * that are not allowed to do anything when used in the blocks with which that
 * function table is used.
 */

static void * df_do_nothing_char( void )
{
    return( (void *) char_convert( NULL ) );
}

/* Function df_do_nothing_num().
 * Returns the number 0. Used in the function tables for device functions that
 * are not allowed to do anything when used in the blocks with which that
 * function table is used.
 */

static void * df_do_nothing_num( void )
{
    return( (void *) 0 );
}

/* Function df_bad_code().
 * Reports byte codes not known to exist; should never be called.
 */

static void * df_bad_code( void )
{
    out_msg( "\nUnknown byte code: 0x%02x -- parameter type: 0x%02x\n", \
                    current_df_data.df_code, current_df_data.parameter_type);
    return( NULL );
}

/* These functions are for device functions which take no parameters. */

/* Type I device functions have an associated parameter type byte, which should
 * always be "0x00" for the functions in this section. 
 */

/* Function df_clearPC().
 * Implements device function %clearPC(). This is one of several
 * solutions suggested on the developer's newsgroup. Better solutions
 * may exist. The Linux solution is based on information on what does
 * and does not work in the developer's newsgroup.
 */

static void * df_clearPC( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

#ifdef __UNIX__
    out_msg( "\033[2J" );
#else
    system( "cls" );
#endif
    return( NULL );
}

/* Function df_dotab().
 * Implements device function %dotab().
 */
 
static void * df_dotab( void )
{
    int i;

    if( current_df_data.parameter_type != 0x00 ) format_error();

    if( bin_driver->absoluteaddress.text != NULL ) \
        interpret_functions( bin_driver->absoluteaddress.text );
    else {
        char    ps_suffix[] = ")|shwd|";
        size_t  count;
        size_t  ps_suffix_size;

        count = (size_t) tab_width;
        ps_suffix_size = sizeof( ps_suffix );

        resize_staging( count );
        if( ps_device ) {
            staging.data[staging.current] = '(';
            staging.current++;
        }

        for( i = 0; i < count; i++) {
            staging.data[staging.current] = ' ';
            staging.current++;
        }

        if( ps_device ) {
            memcpy_s( &staging.data[staging.current], ps_suffix_size, \
                                            ps_suffix, ps_suffix_size  );
            staging.current += ps_suffix_size;
        }

        ob_insert_block( staging.data, staging.current, true, true );
        staging.current = 0;
    }

    return( NULL );
}

/* Function df_endif().
 * Implements device function %endif().
 */
 
static void * df_endif( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    return( NULL );
}

/* Function df_flushpage().
 * Implements device function %flushpage().
 */
 
static void * df_flushpage( void )
{
    int i;

    /* Simply emits lines equal to the value of page_depth.
     * This will need to be altered in at least two ways:
     *   the number of lines should be enough to get to the end
     *   the use of :NEWLINE can be improved to skip more than one line
     *   this is not supposed to happen on the first page
     * but much of these details may be incorporated into a function which
     * uses :NEWLINE blocks to page down, which this function will then call.
     */
     
    if( current_df_data.parameter_type != 0x00 ) format_error();

    if( bin_driver->absoluteaddress.text == NULL ) {
        for( i = 0; i < bin_device->page_depth; i++ ) \
            interpret_functions( bin_driver->newlines.newlineblocks[0].text );
    }

    return( NULL );
}

/* Function df_recordbreak_device().
 * Implements device function %recordbreak() for the :DEVICE block.
 */
 
static void * df_recordbreak_device( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    out_msg( "\n" );
    return( NULL );
}

/* Function df_recordbreak_driver().
 * Implements device function %recordbreak() for the :DRIVER block.
 */
 
static void * df_recordbreak_driver( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    ob_flush();
    return( NULL );
}

/* Function df_textpass().
 * Implements device function %textpass().
 */
 
static void * df_textpass( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    textpass = true;
    return( NULL );
}

/* Function df_ulineoff().
 * Implements device function %ulineoff().
 */
 
static void * df_ulineoff( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    uline = false;
    return( NULL );
}

/* Function df_ulineon().
 * Implements device function %ulineon().
 */
 
static void * df_ulineon( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    uline = true;
    return( NULL );
}

/* Function df_wait().
 * Implements device function %wait().
 */
 
static void * df_wait( void )
{
    if( current_df_data.parameter_type != 0x00 ) format_error();

    getch();
    return( df_recordbreak_device() );
}

/* Type II device functions have no parameter type byte. */

/* Function df_date().
 * Implements device function %date().
 */
 
static void * df_date( void )
{
    return( (void *) char_convert( date_val ) );
}

/* Function df_default_width().
 * Implements device function %default_width().
 */
 
static void * df_default_width( void )
{
    return( (void *) wgml_fonts[font_number].bin_font->char_width );
}

/* Function df_font_height().
 * Implements device function %font_height().
 */
 
static void * df_font_height( void )
{
    return( (void *) wgml_fonts[font_number].font_height );
}

/* Function df_font_number().
 * Implements device function %font_number().
 */
 
static void * df_font_number( void )
{
    return( (void *) font_number );
}

/* Function df_font_outname1().
 * Implements device function %font_outname1().
 */
 
static void * df_font_outname1( void )
{
    char    *   ret_val;

    ret_val = char_convert( wgml_fonts[font_number].bin_font->font_out_name1 );

    return( (void *) ret_val );
}

/* Function df_font_outname2().
 * Implements device function %font_outname2().
 */
 
static void * df_font_outname2( void )
{
    char    *   ret_val;

    ret_val = char_convert( wgml_fonts[font_number].bin_font->font_out_name2 );

    return( (void *) ret_val );
}

/* Function df_font_resident().
 * Implements device function %font_resident().
 */
 
static void * df_font_resident( void )
{
    char    *   ret_val;

    ret_val = (char *) mem_alloc( sizeof(char) + 1 );
    ret_val[0] = (char) wgml_fonts[font_number].font_resident;
    ret_val[1] = '\0';
    
    return( (void *) ret_val );
}

/* Function df_font_space().
 * Implements device function %font_space().
 */
 
static void * df_font_space( void )
{
    return( (void *) wgml_fonts[font_number].font_space );
}

/* Function df_line_height().
 * Implements device function %line_height().
 */
 
static void * df_line_height( void )
{
    return( (void *) wgml_fonts[font_number].line_height );
}

/* Function df_line_space().
 * Implements device function %line_space().
 */
 
static void * df_line_space( void )
{
    return( (void *) wgml_fonts[font_number].line_space );
}

/* Function df_page_depth().
 * Implements device function %page_depth().
 */
 
static void * df_page_depth( void )
{
    return( (void *) bin_device->page_depth );
}

/* Function df_page_width().
 * Implements device function %page_width().
 */
 
static void * df_page_width( void )
{
    return( (void *) bin_device->page_width );
}

/* Function df_pages().
 * Implements device function %pages().
 */
 
static void * df_pages( void )
{
    return( (void *) pages );
}

/* Function df_tab_width().
 * Implements device function %tab_width().
 */
 
static void * df_tab_width( void )
{
    return( (void *) tab_width );
}

/* Function df_thickness().
 * Implements device function %thickness().
 */
 
static void * df_thickness( void )
{
    return( (void *) thickness );
}

/* Function df_time().
 * Implements device function %time().
 */
 
static void * df_time( void )
{
    return( (void *) char_convert( time_val ) );
}

/* Function df_wgml_header().
 * Implements device function %wgml_header().
 */
 
static void * df_wgml_header( void )
{
    return( (void *) char_convert( wgml_header ) );
}

/* Function df_x_address().
 * Implements device function %x_address().
 */
 
static void * df_x_address( void )
{
    return( (void *) x_address );
}

/* Function df_x_size().
 * Implements device function %x_size().
 */
 
static void * df_x_size( void )
{
    return( (void *) x_size );
}

/* Function df_y_address().
 * Implements device function %y_address().
 */
 
static void * df_y_address( void )
{
    return( (void *) y_address );
}

/* Function df_y_size().
 * Implements device function %y_size().
 */
 
static void * df_y_size( void )
{
    return( (void *) y_size );
}

/* Parameter block parsing functions. */

/* Function get_parameters().
 * Place offset2 and offset3 into in_parameters->first and
 * in_parameters->second. This function also skips both offset1 and offset4.
 *
 * Parameter:
 *      in_parameters points to the parameters instance to be initialized.
 *
 * Global Variable Used:
 *      current_df_data.current must point to the first byte of 
 *      offset1 on entry.
 *
 * Global Variable Modified:
 *      current_df_data.current will point to the first byte after
 *      offset4 on exit.
 *
*/

static void * get_parameters ( parameters * in_parameters )
{
    uint16_t    offset;

    /* Skip the offset1 value. */

    memcpy_s( &offset, sizeof( offset ), current_df_data.current, \
                                                            sizeof( offset ) );
    current_df_data.current += sizeof( offset );

    /* Get the first parameter offset (offset2). */

    memcpy_s( &in_parameters->first, sizeof( in_parameters->first ), \
                        current_df_data.current, sizeof( in_parameters->first ) );
    current_df_data.current += sizeof( in_parameters->first );

    /* Get the second parameter offset (offset3). */

    memcpy_s( &in_parameters->second, sizeof( in_parameters->second ), \
                    current_df_data.current, sizeof( in_parameters->second ) );
    current_df_data.current += sizeof( in_parameters->second );

    /* Skip the offset4 value. */

    memcpy_s( &offset, sizeof( offset ), current_df_data.current, \
                                                            sizeof( offset ) );
    current_df_data.current += sizeof( offset );

    return( NULL );    
}

/* Function process_parameter().
 * Extracts the byte code from the Directive instance, updates
 * current_df_data.current, and invokes the function which handles
 * that byte code.
 *
 * Global Variable Used:
 *      current_df_data.current must point to the first byte in the
 *      Directive instance. 
 *
 * Global Variable Altered:
 *      current_df_data will be altered as needed to parse the rest of the
 *      function sequence.
 *
 * Returns:
 *      the value returned by the device function invoked.
*/

static void * process_parameter( void )
{
    /* Reset current_df_data for the parameter. */

    memcpy_s( &current_df_data.df_code, sizeof( current_df_data.df_code ), \
                    current_df_data.current, sizeof( current_df_data.df_code ) );
    current_df_data.current += sizeof( current_df_data.df_code );

    /* Invoke parameter function. */

    if( current_df_data.df_code > MAX_FUNC_INDEX ) format_error();

    return( current_function_table[current_df_data.df_code]() );
}

/* Functions which use parameters.
 * These functions all take parameters and so have common effects on the global
 * current_df_data. 
 *
 * Global Variables Used:
 *      current_df_data is updated to reflect the current function.
 *      function_table[] is used to invoke the handler for each device function.
 *
 * Global Variable Modified:
 *      current_df_data.current will point to the first byte after
 *      the byte code for the function being processed when the handler in
 *      function_table[] is invoked. This occurs when a parameter block is
 *      present.
 *
 *      current_df_data.current will point to the first byte after
 *      the character data found by %image() and %text() when they have a
 *      parameter which is not in a parameter block.
 */

/* Device functions %image() and %text() have an associated parameter type byte
 * which can be either "0x00" (for a literal parameter) or "0x10" (for a
 * parameter block).
 */

/* Function df_out_text_device().
 * Implements device functions %image() and %text() for the :DEVICE block.
 */
 
static void * df_out_text_device( void )
{
    int         i;
    parameters  my_parameters;
    uint16_t    count;

    switch( current_df_data.parameter_type) {
    case 0x00:

        /* Character literal parameter. */
        
        memcpy_s( &count, sizeof( count ), current_df_data.current, \
                                                            sizeof( count ) );
        current_df_data.current += sizeof( count );

        /* Emit parameter byte-by-byte since may contain nulls. */

        for( i = 0; i < count; i++ ) {
            out_msg( "%c", current_df_data.current[i] );
        }
        break;

    case 0x10:

        /* Parameter block. */

        /* Ensure that this is either a ShortHeader or a LongHeader. */

        get_parameters( &my_parameters );
        if( (my_parameters.first != 0x0009) && \
                            (my_parameters.first != 0x000d) ) format_error();

        /* Now get and emit the parameter. */

        current_df_data.current = current_df_data.base + my_parameters.first;
        out_msg( (char *) process_parameter() );

        break;

    default:
        format_error();
    }
    return( NULL );
}

/* Function out_text_driver().
 * Implements device functions %image() and %text() for the :DRIVER block.
 */
 
static void out_text_driver( bool out_trans, bool out_text )
{
    char        *   first;
    parameters      my_parameters;
    uint16_t        count;

    switch( current_df_data.parameter_type) {
    case 0x00:

        /* Character literal parameter. */
        
        memcpy_s( &count, sizeof( count ), current_df_data.current, \
                                                                sizeof( count ) );
        current_df_data.current += sizeof( count );
        ob_insert_block( current_df_data.current, count, out_trans, out_text );
        break;

    case 0x10:

        /* Parameter block. */

        /* Ensure that this is either a ShortHeader or a LongHeader. */

        get_parameters( &my_parameters );
        if( (my_parameters.first != 0x0009) && \
                            (my_parameters.first != 0x000d) ) format_error();

        /* Now get and insert the parameter. */

        current_df_data.current = current_df_data.base + my_parameters.first;
        first = process_parameter();
        count = strlen( first );
        ob_insert_block( first, count, out_trans, out_text );

        break;

    default:
        format_error();
    }
    return;
}

/* Function df_image_driver().
 * Implements device function %image() for the :DRIVER block.
 */
 
static void * df_image_driver( void )
{
    out_text_driver( false, false );
    return( NULL );
}

/* Function df_text_driver().
 * Implements device function %text() for the :DRIVER block.
 */
 
static void * df_text_driver( void )

{
    out_text_driver( true, false );
    return( NULL );
}

/* These functions handle parameter blocks containing literals. */

/* Function char_literal().
 * Returns a pointer to the character literal.
 */
 
static void * char_literal( void )
{
    char        *   ret_val = NULL;
    uint16_t        count;

    /* Skip the rest of the Directive. */

    current_df_data.current += 0x0c;

    /* Get the count. */

    memcpy_s( &count, sizeof( count ), current_df_data.current, \
                                                                sizeof( count ) );
    current_df_data.current += sizeof( count );

    /* Convert the character literal into a char *. */

    ret_val = char_convert( current_df_data.current );
    current_df_data.current += count + 1;

    return( (void *) ret_val );
}

/* Function numeric_literal().
 * Returns the value of a numeric literal.
 */
 
static void * numeric_literal( void )
{
    uint16_t    value;

    /* Skip the Offsets. */

    current_df_data.current += 0x08;

    /* Get and return the value. */

    memcpy_s( &value, sizeof( value ), current_df_data.current, \
                                                            sizeof( value ) );
    return( (void *) value );
}

/* These functions take parameters in parameter blocks only. */

/* Type I device functions have an associated parameter type byte, which should
 * always be "0x10" for the functions in this section.
 */

/* Function df_cancel().
 * Implements device function %cancel().
 */
 
static void * df_cancel( void )
{
            char *      first;
    static  int         instance = 0;
            parameters  my_parameters;

    /* Recursion is an error. */

    instance++;
    if( instance > 1 ) {
        out_msg( "Recursive use of %cancel(), check device library." );
        err_count++;
        g_suicide();
    }

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();

    /* Now invoke the parameter's handler. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (char *) process_parameter();

    if( wgml_fonts[font_number].font_style != NULL ) {
        if( !stricmp( first, wgml_fonts[font_number].font_style->type ) ) {
            if( wgml_fonts[font_number].font_style->startvalue != NULL ) {
               interpret_functions( \
                    wgml_fonts[font_number].font_style->startvalue->text );
            }
        }
    }

    if( wgml_fonts[font_number].font_style != NULL ) {
        if( !stricmp( first, wgml_fonts[font_number].font_switch->type ) ) {
            if( wgml_fonts[font_number].font_switch->startvalue != NULL ) {
               interpret_functions( \
                    wgml_fonts[font_number].font_switch->startvalue->text );
            }
        }
    }

    /* Free the buffer and reset the recursion check. */

    mem_free( first );
    instance++;

    return( NULL );
}

/* Function df_enterfont().
 * Implements device function %enterfont().
 */
 
static void * df_enterfont( void )
{
    /* Device function %enterfont() ignores its parameter. */

    fb_enterfont();
    return( NULL );
}

/* Function df_sleep().
 * Implements device function %sleep().
 *
 * Note:
 *      Added for completeness. This device function causes gendev to hang
 *      if used with a literal, and wgml to hang if used with a non-literal.
 *      It is used in device_function_table to keep the compiler happy.
 */
 
static void * df_sleep( void )
{
    parameters  my_parameters;
    uintptr_t   first;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();

    /* Get the parameter. */

    /* The problem with wgml appears to be related to the fact that gendev
     * generates a ShortHeader but encodes the first parameter as if it were
     * a LongHeader. Hence, the literal.
     * The original line:
     * current_df_data.current = current_df_data.base + my_parameters.first;
     */

    current_df_data.current = current_df_data.base + 0x09;
    first = (uintptr_t) process_parameter();

    sleep( (unsigned) first);

    return( NULL );
}

/* Function df_setsymbol().
 * Implements device function %setsymbol().
 */

static void * df_setsymbol( void )
{
    char        *   first;
    char        *   second;
    parameters      my_parameters;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();

    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (char *) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (char *) process_parameter();

    /* Insert the symbol into the global symbol table. */

    add_symvar( &global_dict, first, second, no_subscript, 0 );

    return( NULL );
}

/* Function df_binary().
 * Implements device function %binary().
 *
 * Note:
 *      Device function %binary1() has the same byte code, and hence uses
 *      the same implementation, as device function %binary().
 */
 
static void * df_binary( void )
{
    parameters  my_parameters;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();

    /* Now invoke the parameter's handler. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    ob_insert_byte( (uint8_t) process_parameter() );
    
    return( NULL );
}

/* Function skip_functions().
 * Skips all function sequences until %endif() is encountered or the block ends.
 */

static void skip_functions( void )
{
    uint8_t *   current_function    = NULL;
    uint16_t    current_offset;

    for( ;; )
    {
        /* Start at the proper location. For current_df_data.base, this is
         * the byte before where the parameter block starts, if one is present.
         */

        current_df_data.base = current_function;
        current_df_data.base += 3;
        current_df_data.current = current_function;

        /* Get the offset to the next element in the linked list. */

        memcpy_s( &current_offset, sizeof( current_offset ),
            current_df_data.current, sizeof( current_offset ) );
        current_df_data.current += sizeof( current_offset );

        /* Either reset current_function to the next list element
         * or exit the loop. If this is the last function, it is either
         * %endif() or it must be skipped anyway, so we are done.
         */

        if( current_offset < 0xFFFF ) {
            current_function = current_df_data.current + current_offset;
        } else {

            /* The flag must be set to prevent interpret_functions from
             * processing the last function in error.
             */

            current_df_data.last_function_done = true;
            break;
        }

        /* Get the function code. */

        memcpy_s( &current_df_data.df_code, sizeof( current_df_data.df_code ), \
                  current_df_data.current, sizeof( current_df_data.df_code ) );
        current_df_data.current += sizeof( current_df_data.df_code );

        /* If the function code is for %endif(), exit the loop. */

        if( current_df_data.df_code == 0x1C ) break;

    }

    return;
}

/* Function df_ifeqn().
 * Implements device function %ifeqn().
 */
 
static void * df_ifeqn( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();
    
    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    /* if_eqn: skip the controlled functions if the values are not equal. */

    if( first != second ) skip_functions();

    return( NULL );
}

/* Function df_ifnen().
 * Implements device function %ifnen().
 */
 
static void * df_ifnen( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();
    
    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    /* if_nen: skip the controlled functions if the values are equal. */

    if( first == second ) skip_functions();

    return( NULL );
}

/* Function df_ifeqs().
 * Implements device function %ifeqs().
 */
 
static void * df_ifeqs( void )
{
    char        *   first;
    char        *   second;
    parameters      my_parameters;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();
    
    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (char *) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (char *) process_parameter();

    /* if_eqs: skip the controlled functions if the values are not equal. */

    if( strcmp( first, second ) ) skip_functions();

    /* Free the memory allocated to produce the parameters */

    mem_free( first );
    mem_free( second );

    return( NULL );
}

/* Function df_ifnes().
 * Implements device function %ifnes().
 */
 
static void * df_ifnes( void )
{
    char        *   first;
    char        *   second;
    parameters      my_parameters;

    /* Ensure that this is either a ShortHeader or a LongHeader. */

    get_parameters( &my_parameters );
    if( (my_parameters.first != 0x0009) && (my_parameters.first != 0x000d) ) \
                                                                format_error();

    /* Ensure the parameter_type is correct */

    if( current_df_data.parameter_type != 0x10 ) format_error();
    
    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (char *) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (char *) process_parameter();

    /* if_nes: skip the controlled functions if the values are equal. */

    if( !strcmp( first, second ) ) skip_functions();

    /* Free the memory allocated to produce the parameters */

    mem_free( first );
    mem_free( second );

    return( NULL );
}

/* Type II device functions have no parameter type byte or
 * ShortHeader/Longheader check.
 */

/* Function df_add().
 * Implements device function %add().
 */
 
static void * df_add( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Extract parameter offsets. */

    get_parameters( &my_parameters );

    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    return( (void *) (first + second) );
}

/* Function df_decimal().
 * Implements device function %decimal().
 */
 
static void * df_decimal( void )
{
    char        value[12];
    parameters  my_parameters;
    uintptr_t   first;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Convert and return the value. */

    return( (void *) _ltoa( first, value, 10 ) );
}

/* Function df_divide().
 * Implements device function %divide().
 */
 
static void * df_divide( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Extract parameter offsets. */

    get_parameters( &my_parameters );

    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    if( second == 0 ) {
        out_msg( "Zero divisor found in device function %%divide()!\n" );
        err_count++;
        g_suicide();
    }

    return( (void *) (first / second) );
}

/* Function df_getstrsymbol().
 * Implements device function %getstrsymbol().
 */
 
static void * df_getstrsymbol( void )
{
    char        *   name    = NULL;
    char        *   ret_val = NULL;
    parameters      my_parameters;
    symsub      *   sym_val = NULL;
    
    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    name = process_parameter();

    /* Now get the symbol's value. */

    find_symvar( &global_dict, name, no_subscript, &sym_val );
    if( sym_val == NULL ) ret_val = char_convert( NULL );
    else ret_val = char_convert( sym_val->value );

    return( (void *) ret_val );
}

/* Function df_getnumsymbol().
 * Implements device function %getnumsymbol().
 */
 
static void * df_getnumsymbol( void )
{
    char        *   name    = NULL;
    parameters  my_parameters;
    symsub      *   sym_val = NULL;
    uint32_t        ret_val = 0;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    name = process_parameter();

    /* Now get the symbol's value. */

    find_symvar( &global_dict, name, no_subscript, &sym_val );
    if( sym_val != NULL ) ret_val = atol( sym_val->value );

    return( (void *) ret_val );
}

/* Function df_hex().
 * Implements device function %hex().
 */
 
static void * df_hex( void )
{
    char        value[9];
    parameters  my_parameters;
    uintptr_t   first;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Convert and return a pointer to the parameter */

    return( (void *) _ltoa( first, value, 16 ) );
}

/* Function df_lower().
 * Implements device function %lower().
 */
 
static void * df_lower( void )
{
    char        *   first;
    parameters      my_parameters;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (char *) process_parameter();

    /* Convert and return the parameter. */

    return( (void *) _strlwr( first ) );
}

/* Function df_remainder().
 * Implements device function %remainder().
 */
 
static void * df_remainder( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Extract parameter offsets. */

    get_parameters( &my_parameters );

    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    if( second == 0 ) {
        out_msg( "Zero divisor found in device function %%remainder()!\n" );
        err_count++;
        g_suicide();
    }

    return( (void *) (first % second) );
}

/* Function df_subtract().
 * Implements device function %subtract().
 */
 
static void * df_subtract( void )
{
    parameters  my_parameters;
    uintptr_t   first;
    uintptr_t   second;

    /* Extract parameter offsets. */

    get_parameters( &my_parameters );

    /* Now get the first parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Now get the second parameter. */

    current_df_data.current = current_df_data.base + my_parameters.second;
    second = (uintptr_t) process_parameter();

    return( (void *) (first - second) );
}

/* The function table for function blocks defined in the :DEVICE block.
 * This is set up for the START :PAUSE block, hence the liberal use of
 * df_do_nothing_char() and df_do_nothing_num(). Many of them will be
 * replaced when the function populate_device_table() is invoked.
 */

static df_function device_function_table[0x3D] = {
    &char_literal,          // 0x00 (character parameter in parameter block)
    &df_recordbreak_device, // 0x01 %recordbreak()
    &df_bad_code,           // 0x02 (none)
    &df_bad_code,           // 0x03 (none)
    &df_bad_code,           // 0x04 (none)
    &df_bad_code,           // 0x05 (none)
    &df_do_nothing_num,     // 0x06 %enterfont()
    &df_bad_code,           // 0x07 (none)
    &df_bad_code,           // 0x08 (none)
    &df_do_nothing_num,     // 0x09 %binary() and %binary1()
    &df_do_nothing_num,     // 0x0A %binary2()
    &df_do_nothing_num,     // 0x0B %binary4()
    &df_do_nothing_char,    // 0x0C %decimal()
    &df_do_nothing_char,    // 0x0D %hex()
    &df_do_nothing_num,     // 0x0E %add()
    &df_do_nothing_num,     // 0x0F %subtract()
    &df_do_nothing_num,     // 0x10 %divide()
    &df_do_nothing_num,     // 0x11 %remainder()
    &df_getnumsymbol,       // 0x12 %getnumsymbol()
    &df_getstrsymbol,       // 0x13 %getstrsymbol()
    &df_do_nothing_char,    // 0x14 %lower()
    &df_out_text_device,    // 0x15 %image()
    &df_out_text_device,    // 0x16 %text()
    &df_setsymbol,          // 0x17 %setsymbol()
    &df_ifeqs,              // 0x18 %ifeqs()
    &df_ifnes,              // 0x19 %ifnes()
    &df_ifeqn,              // 0x1A %ifeqn()
    &df_ifnen,              // 0x1B %ifnen()
    &df_endif,              // 0x1C %endif()
    &df_do_nothing_num,     // 0x1D %flushpage()
    &df_clearPC,            // 0x1E %clearPC()
    &df_do_nothing_num,     // 0x1F %clear3270()
    &df_do_nothing_num,     // 0x20 %textpass()
    &df_do_nothing_num,     // 0x21 %ulineon()
    &df_do_nothing_num,     // 0x22 %ulineoff()
    &df_do_nothing_num,     // 0x23 %dotab()
    &df_do_nothing_num,     // 0x24 %cancel()
    &df_wait,               // 0x25 %wait()
    &df_sleep,              // 0x26 %sleep()
    &df_do_nothing_num,     // 0x27 %default_width()
    &df_do_nothing_num,     // 0x28 %font_number()
    &df_do_nothing_num,     // 0x29 %tab_width()
    &df_do_nothing_num,     // 0x2A %page_depth()
    &df_do_nothing_num,     // 0x2B %page_width()
    &df_do_nothing_num,     // 0x2C %x_address()
    &df_do_nothing_num,     // 0x2D %y_address()
    &df_do_nothing_num,     // 0x2E %x_size()
    &df_do_nothing_num,     // 0x2F %y_size()
    &df_do_nothing_num,     // 0x30 %thickness()
    &df_do_nothing_num,     // 0x31 %font_height()
    &df_do_nothing_num,     // 0x32 %font_space()
    &df_do_nothing_num,     // 0x33 %line_height()
    &df_do_nothing_num,     // 0x34 %line_space()
    &df_do_nothing_num,     // 0x35 %pages()
    &df_do_nothing_char,    // 0x36 %wgml_header()
    &df_do_nothing_char,    // 0x37 %font_outname1()
    &df_do_nothing_char,    // 0x38 %font_outname2()
    &df_do_nothing_char,    // 0x39 %font_resident()
    &df_do_nothing_char,    // 0x3A %time()
    &df_do_nothing_char,    // 0x3B %date()
    &numeric_literal,       // 0x3C (numeric parameter in parameter block)
};

/* The function table for function blocks defined in the :DRIVER block.
 * Function df_do_nothing_num() is only used for those functions documented
 * to have no effect in this context.
 */

static df_function driver_function_table[0x3D] = {
    &char_literal,          // 0x00 (character parameter in parameter block)
    &df_recordbreak_driver, // 0x01 %recordbreak()
    &df_bad_code,           // 0x02 (none)
    &df_bad_code,           // 0x03 (none)
    &df_bad_code,           // 0x04 (none)
    &df_bad_code,           // 0x05 (none)
    &df_enterfont,          // 0x06 %enterfont()
    &df_bad_code,           // 0x07 (none)
    &df_bad_code,           // 0x08 (none)
    &df_binary,             // 0x09 %binary() and %binary1()
    &df_do_nothing_num,     // 0x0A %binary2()
    &df_do_nothing_num,     // 0x0B %binary4()
    &df_decimal,            // 0x0C %decimal()
    &df_hex,                // 0x0D %hex()
    &df_add,                // 0x0E %add()
    &df_subtract,           // 0x0F %subtract()
    &df_divide,             // 0x10 %divide()
    &df_remainder,          // 0x11 %remainder()
    &df_getnumsymbol,       // 0x12 %getnumsymbol()
    &df_getstrsymbol,       // 0x13 %getstrsymbol()
    &df_lower,              // 0x14 %lower()
    &df_image_driver,       // 0x15 %image()
    &df_text_driver,        // 0x16 %text()
    &df_setsymbol,          // 0x17 %setsymbol()
    &df_ifeqs,              // 0x18 %ifeqs()
    &df_ifnes,              // 0x19 %ifnes()
    &df_ifeqn,              // 0x1A %ifeqn()
    &df_ifnen,              // 0x1B %ifnen()
    &df_endif,              // 0x1C %endif()
    &df_flushpage,          // 0x1D %flushpage()
    &df_do_nothing_num,     // 0x1E %clearPC()
    &df_do_nothing_num,     // 0x1F %clear3270()
    &df_textpass,           // 0x20 %textpass()
    &df_ulineon,            // 0x21 %ulineon()
    &df_ulineoff,           // 0x22 %ulineoff()
    &df_dotab,              // 0x23 %dotab()
    &df_cancel,             // 0x24 %cancel()
    &df_do_nothing_num,     // 0x25 %wait()
    &df_do_nothing_num,     // 0x26 %sleep()
    &df_default_width,      // 0x27 %default_width()
    &df_font_number,        // 0x28 %font_number()
    &df_tab_width,          // 0x29 %tab_width()
    &df_page_depth,         // 0x2A %page_depth()
    &df_page_width,         // 0x2B %page_width()
    &df_x_address,          // 0x2C %x_address()
    &df_y_address,          // 0x2D %y_address()
    &df_x_size,             // 0x2E %x_size()
    &df_y_size,             // 0x2F %y_size()
    &df_thickness,          // 0x30 %thickness()
    &df_font_height,        // 0x31 %font_height()
    &df_font_space,         // 0x32 %font_space()
    &df_line_height,        // 0x33 %line_height()
    &df_line_space,         // 0x34 %line_space()
    &df_pages,              // 0x35 %pages()
    &df_wgml_header,        // 0x36 %wgml_header()
    &df_font_outname1,      // 0x37 %font_outname1()
    &df_font_outname2,      // 0x38 %font_outname2()
    &df_font_resident,      // 0x39 %font_resident()
    &df_time,               // 0x3A %time()
    &df_date,               // 0x3B %date()
    &numeric_literal,       // 0x3C (numeric parameter in parameter block)
};

/* Function interpret_functions().
 * Interprets device functions for function blocks as selected by the value of
 * current_function_table. This function directly handles the linked list of
 * Type I device functions only; the individual functions in
 * current_function_table[] handle parameters, including parameter blocks and
 * Type II device functions, as appropriate.
 *
 * Parameter:
 *      in_function points to a compiled function block.
 *
 * Global Variables Used:
 *      current_df_data is updated to reflect the current function.
 *      current_function_table is used to invoke the handler for each device
 *          function.
 *
 * Global Variable Modified:
 *      current_df_data.current will point to the first byte after
 *      the byte code for the function being processed when the handler in
 *      function_table[] is invoked.
 *
 * Note:
 *      current_function is used to reset current_df_data.current
 *      on each pass through the loop. It is not necessary that the functions
 *      invoked through function_table[] leave current_df_data.current
 *      in any particular state.
*/

static void interpret_functions( uint8_t * in_function )
{
    uint8_t *   current_function    = NULL;
    uint16_t    current_offset;

    /* An empty or missing block is not an error, but nothing happens. */

    if( in_function == NULL ) return;

    /* Initialize the loop parameters. */

    current_df_data.last_function_done = false;
    current_function = in_function;

    while( !current_df_data.last_function_done )
    {

        /* Start at the proper location. For current_df_data.base, this is
         * the byte before where the parameter block starts, if one is present.
         */

        current_df_data.base = current_function;
        current_df_data.base += 3;
        current_df_data.current = current_function;

        /* Get the offset to the next element in the linked list. */

        memcpy_s( &current_offset, sizeof( current_offset ),
            current_df_data.current, sizeof( current_offset ) );
        current_df_data.current += sizeof( current_offset );

        /* Get the parameter type for the current device function */

        memcpy_s( &current_df_data.parameter_type, \
                  sizeof( current_df_data.parameter_type ), \
                  current_df_data.current, \
                  sizeof( current_df_data.parameter_type ) );
        current_df_data.current += sizeof( current_df_data.parameter_type );

        /* Either reset current_function to the next list element
         * or record that the last function will be done this iteration.
         */

        if( current_offset < 0xFFFF ) {
            current_function = current_df_data.current + \
                current_offset;
        } else {
            current_df_data.last_function_done = true;
        }

        /* Get the function code. */

        memcpy_s( &current_df_data.df_code, sizeof( current_df_data.df_code ), \
                  current_df_data.current, sizeof( current_df_data.df_code ) );
        current_df_data.current += sizeof( current_df_data.df_code );

        /* This is where the df_code processing occurs. */

        if( current_df_data.df_code > MAX_FUNC_INDEX ) format_error();

        current_function_table[current_df_data.df_code]();

    }
    return;
}

/*  Global function definition. */

/* Function df_interpret_device_functions().
 * Interprets device functions for function blocks in the :DEVICE block.
 *
 * Parameter:
 *      in_function points to a compiled function block.
 *
 * Global Variable Used:
 *      current_function_table is set to device_function_table.
*/

void df_interpret_device_functions( uint8_t * in_function )
{
    /* Select the table and invoke the interpreter. */

    current_function_table = device_function_table;
    interpret_functions( in_function );

    return;
}

/* Function df_interpret_driver_functions().
 * Interprets device functions for function blocks in the :DRIVER block. 
 *
 * Parameter:
 *      in_function points to a compiled function block.
 *
 * Global Variable Used:
 *      current_function_table is set to driver_function_table.
*/

void df_interpret_driver_functions( uint8_t * in_function )
{
    /* Select the table and invoke the interpreter. */

    current_function_table = driver_function_table;
    interpret_functions( in_function );

    return;
}

/* Function df_populate_device_table().
 * Modifies the entries in device_function_table so that all the device
 * functions which are supposed to work for function blocks in :DEVICE blocks
 * will, in fact work. This should be called after the START :PAUSE is
 * interpreted and before the DOCUMENT :PAUSE is interpreted.
 */
 
void df_populate_device_table( void )
{
    device_function_table[0x0c] = &df_decimal;
    device_function_table[0x0d] = &df_hex;
    device_function_table[0x0e] = &df_add;
    device_function_table[0x0f] = &df_subtract;
    device_function_table[0x10] = &df_divide;
    device_function_table[0x11] = &df_remainder;
    device_function_table[0x14] = &df_lower;
    device_function_table[0x27] = &df_default_width;
    device_function_table[0x28] = &df_font_number;
    device_function_table[0x29] = &df_tab_width;
    device_function_table[0x2a] = &df_page_depth;
    device_function_table[0x2b] = &df_page_width;
    device_function_table[0x2c] = &df_x_address;
    device_function_table[0x2d] = &df_y_address;
    device_function_table[0x2e] = &df_x_size;
    device_function_table[0x2f] = &df_y_size;
    device_function_table[0x30] = &df_thickness;
    device_function_table[0x31] = &df_font_height;
    device_function_table[0x32] = &df_font_space;
    device_function_table[0x33] = &df_line_height;
    device_function_table[0x34] = &df_line_space;
    device_function_table[0x35] = &df_pages;
    device_function_table[0x36] = &df_wgml_header;
    device_function_table[0x37] = &df_font_outname1;
    device_function_table[0x38] = &df_font_outname2;
    device_function_table[0x39] = &df_font_resident;
    device_function_table[0x3a] = &df_time;
    device_function_table[0x3b] = &df_date;
    return;
}

/* Function df_setup().
 * Initializes those local variables that need more than a simple assignment
 * statement.
 */

void df_setup( void )
{
    symsub  *   sym_val = NULL;

    /* When called, each of symbols "date" and "time" contains either of
     * -- the value set from the system clock; or
     * -- the value set from the command-line option SETSYMBOL
     * but have not been affected by anything in the document specification.
     */

    find_symvar( &global_dict, "date", no_subscript, &sym_val );
    if( sym_val == NULL ) date_val = char_convert( NULL );
    else date_val = char_convert( sym_val->value );

    sym_val = NULL;
    find_symvar( &global_dict, "time", no_subscript, &sym_val );
    if( sym_val == NULL ) time_val = char_convert( NULL );
    else time_val = char_convert( sym_val->value );

    /* Initialize staging to hold 80 characters. */     

    resize_staging( 80 );

    return;
}

/* Function df_teardown().
 * Releases the memory allocated by df_startup().
 */

void df_teardown( void )
{
    if( date_val != NULL ) {
        mem_free( date_val );
        date_val = NULL;
    }

    if( time_val != NULL ) {
        mem_free( time_val );
        time_val = NULL;
    }
    
    if( staging.data != NULL ) {
        mem_free( staging.data);
        staging.current = 0;
        staging.length = 0;
        staging.data = NULL;
    }

    return;
}

/* Function fb_enterfont().
 * Performs the action of device function %enterfont(0), whether the device
 * function was invoked explicitly or implicitly. Parts of this function may
 * eventually be refactored if needed in other functions.
 */

void fb_enterfont( void )
{

    if( wgml_fonts[0].font_pause != NULL ) \
                        interpret_functions( wgml_fonts[0].font_pause->text );

    if( wgml_fonts[0].font_switch != NULL ) {
        if( wgml_fonts[0].font_switch->startvalue != NULL ) \
            interpret_functions( wgml_fonts[0].font_switch->startvalue->text );
    }

    if( wgml_fonts[0].font_style != NULL ) {       
        if( wgml_fonts[0].font_style->startvalue != NULL ) \
            interpret_functions( wgml_fonts[0].font_style->startvalue->text );
        if( wgml_fonts[0].font_style->lineprocs != NULL ) {       
            if( wgml_fonts[0].font_style->lineprocs[0].startvalue != NULL ) \
                interpret_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].startvalue->text );
            if( wgml_fonts[0].font_style->lineprocs[0].firstword == NULL ) {
                if( wgml_fonts[0].font_style->lineprocs[0].startword != NULL ) \
                    interpret_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].startword->text );
            } else {
                interpret_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].firstword->text );
            }
        }
    }
    return;
}

/* Function fb_init().
 * Interprets the :INIT block, which can contain multiple function blocks of two
 * types, one of which is interpreted for each available font.
 *
 * Parameter:
 *      in_block points to either the START :INIT block or the DOCUMENT :INIT
 *          block.
 */
 
void fb_init( init_block * in_block )
{
    int i;
    int j;
    uint32_t         old_font_number;

    /* An empty init_block is not an error. */

    if( in_block == NULL ) return;

    /* This should be zero, but save it just to be sure. */

    old_font_number = font_number;

    /* :VALUE blocks are done once, :FONTVALUE blocks are done once for
     * each available font.
     */

    for( i = 0; i < in_block->count; i++ ) {
        if( in_block->codeblock[i].is_fontvalue == false ) \
            df_interpret_driver_functions( in_block->codeblock[i].text );
        else for( j = 0; j < wgml_font_cnt; j++ ) {
            font_number = j;
            df_interpret_driver_functions( in_block->codeblock[i].text );
        }
    }

    /* Restore the original font number, which should be zero. */

    font_number = old_font_number;

    return;
}

