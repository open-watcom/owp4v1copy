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
* Description:  Implements the functions declared in devfuncs.h:
*                   df_initialize_pages()
*                   df_increment_pages()
*                   df_interpret_device_functions()
*                   df_interpret_driver_functions()
*                   df_new_section()
*                   df_populate_device_table()
*                   df_populate_driver_table()
*                   df_set_horizontal()
*                   df_set_vertical()
*                   df_setup()
*                   df_teardown()
*                   fb_enterfont()
*                   fb_first_text_pass()
*                   fb_init()
*                   fb_line_block()
*                   fb_lineproc_endvalue()
*               as well as a macro
*                   MAX_FUNC_INDEX
*               some local typedefs and structs:
*                   df_data
*                   df_function
*                   page_state
*                   parameters
*               quite a few local variables:
*                   active_font
*                   at_start
*                   current_df_data
*                   current_function
*                   current_function_table
*                   current_state
*                   date_val
*                   desired_state
*                   device_function_table
*                   driver_function_table
*                   font_number
*                   htab_done
*                   line_pass_number
*                   page_to
*                   pages
*                   set_margin
*                   staging
*                   tab_width
*                   text_out_open
*                   text_output
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
*                   df_image_driver()
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
*                   fb_absoluteaddress()
*                   fb_first_text_chars()
*                   fb_firstword()
*                   fb_font_switch()
*                   fb_htab()
*                   fb_initial_horizontal_positioning()
*                   fb_internal_horizontal_positioning()
*                   fb_new_font_text_chars()
*                   fb_newline()
*                   fb_normal_vertical_positioning()
*                   fb_subsequent_text_chars()
*                   format_error()
*                   get_parameters()
*                   interpret_functions()
*                   numeric_literal()
*                   out_text_driver()
*                   output_spaces()
*                   post_text_output()
*                   pre_text_output()
*                   process_parameter()
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
#include "gvars.h"
#include "outbuff.h"
#include "wgml.h"

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

/* This is used to record the state of the current page. */

typedef struct {
    uint32_t         font_number;
    uint32_t         x_address;
    uint32_t         y_address;
} page_state;

/* Holds the offsets to the parameter(s). */

typedef struct {
    uint16_t    first;
    uint16_t    second;
} parameters;

/* Local typedef. */

/* All functions used in the function tables will use this signature. */

typedef void * (*df_function) (void);

/* Local data declarations. */

/* These are set by device functions and for use in outputting text. */

static bool             textpass                = false;
static bool             uline                   = false;

/* These are used to control some aspects of device function operation. */

static bool             at_start                = true;
static bool             htab_done               = false;
static bool             set_margin              = false;
static bool             text_output             = false;
static page_state       current_state           = { 0, 0, 0 };
static page_state       desired_state           = { 0, 0, 0 };
static uint32_t         line_pass_number        = 0;
static uint32_t         page_top                = 0;

/* These are used to hold values returned by device functions. */

static char         *   date_val                = NULL;
static char         *   time_val                = NULL;
static char             wgml_header[]           = "V4.0 PC/DOS";
static uint32_t         font_number             = 0;
static uint32_t         pages                   = 0;
static uint32_t         tab_width               = 0;
static uint32_t         thickness               = 0;
static uint32_t         x_address               = 0;
static uint32_t         x_size                  = 0;
static uint32_t         y_address               = 0;
static uint32_t         y_size                  = 0;

/* These are used by the interpreter. */
static bool             text_out_open           = false;
static uint32_t         active_font             = 0;
static df_data          current_df_data;
static df_function      device_function_table[MAX_FUNC_INDEX + 1];
static df_function      driver_function_table[MAX_FUNC_INDEX + 1];
static df_function  *   current_function_table  = NULL;
static uint8_t      *   current_function        = NULL;

/* This is used to create text for output when necessary. */

static record_buffer    staging = { 0, 0, NULL };

/* Local function definitions. */

/* Function fb_absoluteaddress().
 * Uses the :ABSOLUTEADDRESS block to actually position the print head to the
 * desired position. 
 *
 * Prerequisite:
 *      The :ABSOLUTEADDRESS block must be defined.
 */

static void fb_absoluteaddress( void )
{
    fb_lineproc_endvalue();

    df_interpret_driver_functions( bin_driver->absoluteaddress.text );

    current_state.x_address = desired_state.x_address;
    current_state.y_address = desired_state.y_address;    
    return;
}

/* Function fb_newline().
 * Uses the various :NEWLINE blocks to actually position the device to the
 * desired vertical position. 
 *
 * Prerequisite:
 *      The :ABSOLUTEADDRESS block must not be defined.
 */

static void fb_newline( void )
{
    int             i;
    newline_block * current_block   = NULL;
    int16_t         desired_units;
    int16_t         desired_lines;
    uint16_t        remainder;
    uint16_t        max_advance;

    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* desired_units holds the number of vertical base units to be moved. */

    if( bin_driver->y_positive == 0x00 ) {

        /* desired_state.y_address was formed by subtraction. */

        desired_units = current_state.y_address - desired_state.y_address;
    } else {

        /* desired_state.y_address was formed by addition. */

        desired_units = desired_state.y_address - current_state.y_address;
    }

    /* Devices using :ABSOLUTEADDRESS may be able to move backwards, but
     * :NEWLINE blocks with negative values for advance are not accepted
     * by gendev and so negative values of lines must be an error.
     */

    if( desired_units < 0 ) {
        out_msg( "wgml internal error: negative carriage movement\n" );
        err_count++;
        g_suicide();
    }

    /* desired_lines contains the number of lines, rounded up. Note: the
     * actual rounding algorithm used by wgml 4.0 has yet to be determined;
     * the rounding used here may require adjustment in the future.
     */

    desired_lines = desired_units / wgml_fonts[active_font].line_height;
    remainder = desired_lines % wgml_fonts[active_font].line_height;
    if ( 2 * remainder >= wgml_fonts[active_font].line_height ) desired_lines++;

    /* Devices using :ABSOLUTEADDRESS may be able to use partial line heights,
     * but devices using :NEWLINE blocks must advance at least one whole line
     * if any advance was called for. This is treated as an error because the
     * page layout code should not be cutting things this close for devices
     * which rely on :NEWLINE blocks.
     */

    if( (desired_units > 0 ) && (desired_lines == 0) ) {
        out_msg( "wgml internal error: vertical movement to small for device\n" );
        err_count++;
        g_suicide();
    }

    /* lines might equal 0, in which case no action is needed. */

    max_advance = 0;
    while( desired_lines > 0 ) {
        for( i = 0; i < bin_driver->newlines.count; i++ ) {
            if( bin_driver->newlines.newlineblocks[i].advance <= desired_lines ) {
                if( max_advance < \
                            bin_driver->newlines.newlineblocks[i].advance ) {
                    max_advance = bin_driver->newlines.newlineblocks[i].advance;
                    current_block = &bin_driver->newlines.newlineblocks[i];
                }
            }
        }
        df_interpret_driver_functions( current_block->text );
        desired_lines -= max_advance;
    }

    /* If this is the Initial Vertical Positioning, interpret the :LINEPROC
     * :ENDVALUE block for line pass 1 of available font 0, unless it has
     * already been done. Note: this places the block at the proper location
     * when one or more :LINEPROC blocks have been interpreted.
     */

    if( at_start ) {
        if( wgml_fonts[0].font_style->lineprocs != NULL ) {       
            if( wgml_fonts[0].font_style->lineprocs[0].endvalue != NULL ) {
                df_interpret_driver_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].endvalue->text );
            }
        }
        at_start = false;
    }

    /* Update the state to reflect the new position. */

    current_state.y_address = desired_state.y_address;

    return;
}

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

/* Function output_spaces().
 * This function inserts the spaces, if any, needed to move the print head
 * from its current position to its desired position into the output buffer.
 *
 * Parameter:
 *      count contains the number of spaces to emit.
 * Global Modified:
 *      current_state.x_address is set equal to desired_state.x_address.
 *
 * Note:
 *      The calling function is responsible for detecting and handling
 *          cases in which the desired horizontal space cannot be treated
 *          as an exact multiple of the width of a space character.
 */

static void output_spaces( uint32_t count )
{
    int     i;

    if( staging.length < count ) {
        staging.text = (uint8_t *) mem_realloc( staging.text, count );
        staging.length = count;
    }

    for( i = 0; i < count; i++) {
        staging.text[staging.current] = ' ';
        staging.current++;
    }

    ob_insert_block( staging.text, staging.current, true, true, active_font );
    current_state.x_address = desired_state.x_address;
    staging.current = 0;

    return;
}

/* Function post_text_output().
 * Performs any processing needed to terminate text output.
 *
 * Global Changed:
 *      htab_done is set to false.
 *      text_out_open is set to false.
 */

static void post_text_output( void )
{
    char    shwd_suffix[] = ") shwd ";
    char    sd_suffix[] = ") sd ";
    size_t  ps_suffix_size;

    if( ps_device ) {
        if( htab_done ) {
            ps_suffix_size = strlen( sd_suffix );
            ob_insert_block( sd_suffix, ps_suffix_size, false, false, \
                                                                active_font );
        } else {
            ps_suffix_size = strlen( shwd_suffix );
            ob_insert_block( shwd_suffix, ps_suffix_size, false, false, \
                                                                active_font );
        }
    }

    htab_done = false;
    text_out_open = false;

    return;
}

/* Function pre_text_output().
 * Performs any processing needed to start text output.
 *
 * Global Changed:
 *      text_out_open is set to true.
 */

static void pre_text_output( void )
{
    if( ps_device ) ob_insert_byte( '(' );
    text_out_open = true;

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
 * Reports byte codes not known to exist but nonetheless found. It never
 * returns to the caller. It should never actually run. Since it is used
 * in the function tables, it must conform to the function typedef.
 */

static void * df_bad_code( void )
{
    out_msg( "\nUnknown byte code: 0x%02x -- parameter type: 0x%02x\n", \
                    current_df_data.df_code, current_df_data.parameter_type);
    err_count++;
    g_suicide();
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
 *
 * Note: mem_realloc() will not return unless it succeeds.
 */
 
static void * df_dotab( void )
{
    static  int         instance = 0;
            uint32_t    spaces;

    /* Recursion is an error. */

    instance++;
    if( instance > 1 ) {
        out_msg( "Recursive use of %%dotab(), check device library." );
        err_count++;
        g_suicide();
    }

    if( current_df_data.parameter_type != 0x00 ) format_error();

    if( has_aa_block ) {
        fb_absoluteaddress();
    } else {

        /* Spaces cannot be emitted "backwards". */

        if( current_state.x_address > desired_state.x_address) {
            out_msg( "Cannot move backwards within a text line!\n" );
            err_count++;
            g_suicide();
        }

        /* Set tab_width and spaces. */

        tab_width = desired_state.x_address - current_state.x_address;
        spaces = tab_width / wgml_fonts[active_font].spc_width;

        /* Warn of apparent use of justification. */

        if( (tab_width % wgml_fonts[active_font].spc_width) > 0 ) {
            out_msg( "Warning: justification not supported (dotab).\n" );
            wng_count++;
        }

        /* Perform the %dotab() horizontal positioning. */

        if( !text_out_open ) pre_text_output();
        output_spaces( spaces );
        if( text_out_open ) post_text_output();
        tab_width = 0;
    }

    instance--;

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
    static  int         instance    = 0;
            uint16_t    current_pages;

    /* Recursion is an error. */

    instance++;
    if( instance > 1 ) {
        out_msg( "Recursive use of %%flushpage(), check device library." );
        err_count++;
        g_suicide();
    }

    if( current_df_data.parameter_type != 0x00 ) format_error();

    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* current_pages contains the number of device pages needed to reach
     * current_state.y_address; however, current_state.y_address is 0-based
     * and must be 1-based for the correct value to be obtained if it
     * designates the last line of the page.
     */

    current_pages = (current_state.y_address + 1) / bin_device->page_depth;

    /* The value needed in desired_state.y_address must reflect the
     * number of device pages included in currrent_state.y_address so
     * that fb_newlines() will have comparable values to work with.
     */

    if( bin_driver->y_positive == 0x00 ) {

        /* desired_state.y_address is to be formed by subtraction. */

        desired_state.y_address = current_pages * bin_device->page_depth;
    } else {

        /* desired_state.y_address is to be formed by addition. */

        desired_state.y_address = (current_pages + 1) * bin_device->page_depth;
    }

    /* The print head position is the start of the bottom line. */

    x_address = bin_device->x_start;
    y_address = bin_device->page_depth;

    /* If :ABSOLUTEADDRESS is not available, do the vertical positioning. */

    if( !has_aa_block ) fb_newline();

    /* If this is the Initial Vertical Positioning, interpret the :LINEPROC
     * :ENDVALUE block for line pass 1 of available font 0, unless it has
     * already been done. Note: this places the block at the proper location
     * when the :ABSOLUTEADDRESS block is defined and %flushpage() is invoked.
     */

    if( at_start ) {
        if( wgml_fonts[0].font_style->lineprocs != NULL ) {       
            if( wgml_fonts[0].font_style->lineprocs[0].endvalue != NULL ) \
                df_interpret_driver_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].endvalue->text );
        }
        at_start = false;
    }

    /* The print head position is now the start of the line before the first. */

    y_address = bin_device->y_start;

    instance--;

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
 * Global Variable Prerequisite:
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
 * Global Variable Prerequisite:
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
 * Notes:
 *      current_df_data.current will point to the first byte after
 *          the byte code for the function being processed when the handler in
 *          function_table[] is invoked. This occurs when a parameter block is
 *          present.
 *      current_df_data.current will point to the first byte after
 *          the character data found by %image() and %text() when they have a
 *          parameter which is not in a parameter block.
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
    char    *   first;
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
        first = (char *) process_parameter();
        out_msg( first );

        /* Free the memory allocated to the parameter. */

        mem_free( first );

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
        ob_insert_block( current_df_data.current, count, out_trans, out_text, \
                                                                active_font );
        break;

    case 0x10:

        /* Parameter block. */

        /* Ensure that this is either a ShortHeader or a LongHeader. */

        get_parameters( &my_parameters );
        if( (my_parameters.first != 0x0009) && \
                            (my_parameters.first != 0x000d) ) format_error();

        /* Now get and insert the parameter. */

        current_df_data.current = current_df_data.base + my_parameters.first;
        first = (char *) process_parameter();
        count = strlen( first );
        ob_insert_block( first, count, out_trans, out_text, active_font );

        /* Free the memory allocated to the parameter. */

        mem_free( first );

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
        out_msg( "Recursive use of %%cancel(), check device library." );
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
                df_interpret_driver_functions( \
                    wgml_fonts[font_number].font_style->startvalue->text );
            }
        }
    }

    if( wgml_fonts[font_number].font_switch != NULL ) {
        if( !stricmp( first, wgml_fonts[font_number].font_switch->type ) ) {
            if( wgml_fonts[font_number].font_switch->startvalue != NULL ) {
                df_interpret_driver_functions( \
                    wgml_fonts[font_number].font_switch->startvalue->text );
            }
        }
    }

    /* Free the buffer and reset the recursion check. */

    mem_free( first );
    instance--;

    return( NULL );
}

/* Function df_enterfont().
 * Implements device function %enterfont().
 */
 
static void * df_enterfont( void )
{
    static  int         instance = 0;

    /* Recursion is an error. */

    instance++;
    if( instance > 1 ) {
        out_msg( "Recursive use of %%enterfont(), check device library." );
        err_count++;
        g_suicide();
    }

    /* Device function %enterfont() ignores its parameter. */

    fb_enterfont();
    instance--;

    return( NULL );
}

/* Function df_sleep().
 * Implements device function %sleep().
 *
 * Note:
 *      Added for completeness. This device function causes gendev 4.1 to hang
 *      if used with a literal, and wgml 4.0 to hang if used with a non-literal.
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

    /* The existing gendev generates a ShortHeader but encodes the first
     * parameter as if it were a LongHeader. Unfortunately, if that
     * parameter takes a parameter, it's offset is also incorrect. This
     * function will fail in process_parameter() since the next binary code
     * will be read as 0xFF, which exceeds the maximum value for the code.
     */

    current_df_data.current = current_df_data.base + my_parameters.first;
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

    /* Free the memory allocated to the parameters. */

    mem_free(first);
    mem_free(second);

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
 *
 * Global Variables Used:
 *      current_df_data is updated to reflect the current function.
 *      current_function is used to record the start of the current function.
 *
 * Global Variables Modified:
 *      current_df_data.current will point to the first byte after
 *          the byte code for the function being processed when the handler
 *          in function_table[] is invoked.
 *      current_function will point to the %endif() or to the next-to-last
 *          function if current_df_data.last_function_done is set to true.
 *
 * Note:
 *      current_function is used to reset current_df_data.current
 *      on each pass through the loop. It is global so that it will do so
 *      when interpret_functions() resumes processing.
 */

static void skip_functions( void )
{
    uint16_t    current_offset;

    /* current_df_data.base points at the binary code for the conditional
     * function. current_function needs to point to the next top-level
     * function: the two-byte offset needed to do this starts three bytes
     * bytes in front of the position pointed to by current_df_data.base but
     * must be added to current_df_data.base.
     */

    current_function = current_df_data.base;
    current_function -= 3;
    memcpy_s( &current_offset, sizeof( current_offset ), current_function, \
                                                sizeof( current_offset ) );
    current_function = current_df_data.base + current_offset;

    for( ;; )
    {
        /* Start at the proper location. For current_df_data.base, this is
         * the byte before where the parameter block starts, if one is present.
         */

        current_df_data.base = current_function;
        current_df_data.base += 3;
        current_df_data.current = current_function;

        /* Get the offset to the next element in the linked list. */

        memcpy_s( &current_offset, sizeof( current_offset ), \
                        current_df_data.current, sizeof( current_offset ) );
        current_df_data.current += sizeof( current_offset );

        /* Get the parameter type for the current device function */

        memcpy_s( &current_df_data.parameter_type, \
                  sizeof( current_df_data.parameter_type ), \
                  current_df_data.current, \
                  sizeof( current_df_data.parameter_type ) );
        current_df_data.current += sizeof( current_df_data.parameter_type );

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
    char    *   value   = NULL;
    parameters  my_parameters;
    uintptr_t   first;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Convert and return the value. */

    value = (char *) mem_alloc( 12 );
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

/* Function df_getnumsymbol().
 * Implements device function %getnumsymbol().
 */
 
static void * df_getnumsymbol( void )
{
    char        *   name    = NULL;
    parameters      my_parameters;
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

    /* Free the memory allocated to the parameter. */

    mem_free( name );

    return( (void *) ret_val );
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

    /* Free the memory allocated to the parameter. */

    mem_free( name );

    return( (void *) ret_val );
}

/* Function df_hex().
 * Implements device function %hex().
 */
 
static void * df_hex( void )
{
    char    *   value   = NULL;
    parameters  my_parameters;
    uintptr_t   first;

    /* Extract parameter offset. */

    get_parameters( &my_parameters );

    /* Now get the parameter. */

    current_df_data.current = current_df_data.base + my_parameters.first;
    first = (uintptr_t) process_parameter();

    /* Convert and return a pointer to the parameter */

    value = (char *) mem_alloc( 9 );
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

    return( (void *) strlwr( first ) );
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
    &df_decimal,            // 0x0C %decimal()
    &df_hex,                // 0x0D %hex()
    &df_add,                // 0x0E %add()
    &df_subtract,           // 0x0F %subtract()
    &df_divide,             // 0x10 %divide()
    &df_remainder,          // 0x11 %remainder()
    &df_getnumsymbol,       // 0x12 %getnumsymbol()
    &df_getstrsymbol,       // 0x13 %getstrsymbol()
    &df_lower,              // 0x14 %lower()
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
    &df_font_number,        // 0x28 %font_number()
    &df_tab_width,          // 0x29 %tab_width()
    &df_do_nothing_num,     // 0x2A %page_depth()
    &df_do_nothing_num,     // 0x2B %page_width()
    &df_x_address,          // 0x2C %x_address()
    &df_y_address,          // 0x2D %y_address()
    &df_x_size,             // 0x2E %x_size()
    &df_y_size,             // 0x2F %y_size()
    &df_thickness,          // 0x30 %thickness()
    &df_do_nothing_num,     // 0x31 %font_height()
    &df_do_nothing_num,     // 0x32 %font_space()
    &df_do_nothing_num,     // 0x33 %line_height()
    &df_do_nothing_num,     // 0x34 %line_space()
    &df_pages,              // 0x35 %pages()
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
    &df_do_nothing_num,     // 0x1D %flushpage()
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
 *      current_function is used to record the start of the current function.
 *      current_function_table is used to invoke the handler for each device
 *          function.
 *
 * Global Variable Modified:
 *      current_df_data.current will point to the first byte after
 *          the byte code for the function being processed when the handler
 *          in function_table[] is invoked.
 *      current_function will point to the next-to-last function.
 *
 * Notes:
 *      current_function is used to reset current_df_data.current
 *          on each pass through the loop. It is not necessary that the
 *          functions invoked through function_table[] leave
 *          current_df_data.current in any particular state.
 *      This function is called recursively and so several globals must be
 *          restored to their value on entry on exit.
*/

static void interpret_functions( uint8_t * in_function )
{
    bool                old_last_done   = false;
    df_function     *   old_function_table  = NULL;
    uint8_t         *   old_function    = NULL;
    uint16_t            current_offset;

    /* An empty or missing block is not an error, but nothing happens. */

    if( in_function == NULL ) return;

    /* Save the interpreter state. */
    
    old_function = current_function;
    old_function_table = current_function_table;
    old_last_done = current_df_data.last_function_done;

    /* Initialize the loop parameters. */

    current_function = in_function;
    current_df_data.last_function_done = false;

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
            current_function = current_df_data.current + current_offset;
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

    /* Restore interpreter state. */

    current_function = old_function;
    current_function_table = old_function_table;
    current_df_data.last_function_done = old_last_done ;

    return;
}

/* Function fb_firstword().
 * Interprets the :FIRSTWORD block of the given :LINEPROC block. The "trick"
 * here is that, if the :FIRSTWORD block does not exist but the :STARTWORD
 * block does, then the :STARTWORD block is interpreted instead. 
 *
 * Parameter:
 *      in_block points to the :LINEPROC block. 
 */
 
static void fb_firstword( line_proc * in_block )
{
    if( in_block->firstword == NULL ) {
        if( in_block->startword != NULL ) {
            df_interpret_driver_functions( in_block->startword->text );
        }
    } else {
        df_interpret_driver_functions( in_block->firstword->text );
    }

    return;
}

/* Function fb_font_switch().
 * Performs the font switch sequence as described in the Wiki.
 */

static void fb_font_switch( void )
{
    bool                    do_now;
    char                *   from_string;
    char                *   to_string;
    fontswitch_block    *   font_switch;
    uint32_t                save_font_number;
    uintptr_t               from_numeric;
    uintptr_t               to_numeric;
    wgml_font           *   from_font;
    wgml_font           *   to_font;

    /* Acquire the "from" and "to" fonts and save the font_number. */

    from_font = &wgml_fonts[current_state.font_number];
    to_font = &wgml_fonts[desired_state.font_number];
    save_font_number = font_number;

    /* The first test: do the fonts use the same :FONTSWITCH block? */

    do_now = (from_font->font_switch != to_font->font_switch);

    /* font_switch will only be used if the "from" and "to" fonts both
     * use the same :FONTSWITCH block (do_now is false here in that case).
     */

    if( do_now ) {
        font_switch = NULL;
    } else {
        font_switch = from_font->font_switch;
    }

    /* The second test: given only one :FONTSWITCH block, is do_always true
     * or false? Note: that :FONTSWITCH block could be NULL.
     */

    if( !do_now && (font_switch != NULL) ) do_now = font_switch->do_always;

    /* The third test: evaluate the :FONTSWITCH block (if it exits) as
     * described in the Wiki. The trick here, of course, is to ensure that,
     * once set to "true", do_now is never reset to "false".
     */

    if( !do_now && (font_switch != NULL) ) {
        if( font_switch->default_width_flag ) {

            /* The default width is a numeric. */

            font_number = current_state.font_number;
            from_numeric = (uintptr_t) df_default_width();
            font_number = desired_state.font_number;
            to_numeric = (uintptr_t) df_default_width();
            if( !do_now ) do_now = ( from_numeric != to_numeric );
        }

        if( font_switch->font_height_flag ) {

            /* The font height is a numeric. */

            font_number = current_state.font_number;
            from_numeric = (uintptr_t) df_font_height();
            font_number = desired_state.font_number;
            to_numeric = (uintptr_t) df_font_height();
            if( !do_now ) do_now = ( from_numeric != to_numeric );
        }

        if( font_switch->font_outname1_flag ) {

            /* The font out name 1 is a string. */

            font_number = current_state.font_number;
            from_string = (char *) df_font_outname1();
            font_number = desired_state.font_number;
            to_string = (char *) df_font_outname1();
            if( !do_now ) do_now = ( strcmp( from_string, to_string ) );
            mem_free( from_string );
            mem_free( to_string );
        }

        if( font_switch->font_outname2_flag ) {

            /* The font out name 2 is a string. */

            font_number = current_state.font_number;
            from_string = (char *) df_font_outname2();
            font_number = desired_state.font_number;
            to_string = (char *) df_font_outname2();
            if( !do_now ) do_now = ( strcmp( from_string, to_string ) );
            mem_free( from_string );
            mem_free( to_string );
        }

        if( font_switch->font_resident_flag ) {

            /* The font resident flag is a string. */

            font_number = current_state.font_number;
            from_string = (char *) df_font_resident();
            font_number = desired_state.font_number;
            to_string = (char *) df_font_resident();
            if( !do_now ) do_now = ( strcmp( from_string, to_string ) );
            mem_free( from_string );
            mem_free( to_string );
        }

        if( font_switch->font_space_flag ) {

            /* The font space is a numeric. */

            font_number = current_state.font_number;
            from_numeric = (uintptr_t) df_font_space();
            font_number = desired_state.font_number;
            to_numeric = (uintptr_t) df_font_space();
            if( !do_now ) do_now = ( from_numeric != to_numeric );
        }

        if( font_switch->line_height_flag ) {

            /* The line height is a numeric. */

            font_number = current_state.font_number;
            from_numeric = (uintptr_t) df_line_height();
            font_number = desired_state.font_number;
            to_numeric = (uintptr_t) df_line_height();
            if( !do_now ) do_now = ( from_numeric != to_numeric );
        }

        if( font_switch->line_space_flag ) {

            /* The line space is a numeric. */

            font_number = current_state.font_number;
            from_numeric = (uintptr_t) df_line_space();
            font_number = desired_state.font_number;
            to_numeric = (uintptr_t) df_line_space();
            if( !do_now ) do_now = ( from_numeric != to_numeric );
        }
    }

    /* Restore the value of font_number. */

    font_number = save_font_number;

    /* Now for the font switch itself. */

    if( from_font->font_style != NULL ) {
        if( from_font->font_style->endvalue != NULL ) {
            df_interpret_driver_functions( \
                                    from_font->font_style->endvalue->text );
        }
    }

    if( do_now ) {
        if( from_font->font_switch != NULL ) {
            if( from_font->font_switch->endvalue != NULL ) {
                df_interpret_driver_functions( \
                                    from_font->font_switch->endvalue->text );
            }
        }
    }

    if( to_font->font_pause != NULL ) \
        df_interpret_device_functions( to_font->font_pause->text );

    if( do_now ) {
        if( to_font->font_switch != NULL ) {
            if( to_font->font_switch->startvalue != NULL ) {
                df_interpret_driver_functions( \
                                    to_font->font_switch->startvalue->text );
            }
        }
    }

    if( to_font->font_style != NULL ) {
        if( to_font->font_style->startvalue != NULL ) {
            df_interpret_driver_functions( \
                                    to_font->font_style->startvalue->text );
        }
    }

    /* This ensures that switches from the new font will be detected. */

    current_state.font_number = desired_state.font_number;

    return;
}

/* Function fb_htab().
 * Function fb_htab() inteprets the :HTAB block and sets htab_done to "true".
 *
 * Global Used:
 *      tab_width must be set to the appropriate value, as this is
 *          (presumably) what the :HTAB block will use to determine
 *          how much horizontal movement is needed.
 *
 * Prerequisite:
 *      The :HTAB block must have been defined for the current device.
 */

static void fb_htab( void )
{
    df_interpret_driver_functions( bin_driver->htab.text );
    htab_done = true;
    return;
}

/* Function fb_initial_horizontal_positioning().
 * Performs the initial horizontal positioning as described in the Wiki.
 */

static void fb_initial_horizontal_positioning( void )
{
    uint32_t    spaces;

    if( has_aa_block ) {
        fb_absoluteaddress();
    } else {

        /* Spaces cannot be emitted and tabs cannot be done "backwards". */

        if( current_state.x_address > desired_state.x_address) {
            out_msg( "Cannot move backwards within a text line!\n" );
            err_count++;
            g_suicide();
        }

        /* Set tab_width and spaces. */

        tab_width = desired_state.x_address - current_state.x_address;
        spaces = tab_width / wgml_fonts[active_font].spc_width;

        /* Warn of apparent use of justification. */

        if( (tab_width % wgml_fonts[active_font].spc_width) > 0 ) {
            out_msg( "Warning: justification not supported (initial).\n" );
            wng_count++;
        }

        /* Perform the initial horizontal positioning. */

        if( (bin_driver->htab.text != 0) && (tab_width > 8) ) {
            fb_htab();
        } else {
            if( !text_out_open ) pre_text_output();
            output_spaces( spaces );
            if( text_out_open ) post_text_output();
        }
        tab_width = 0;
    }
    return;
}

/* Function fb_internal_horizontal_positioning().
 * Performs the internal horizontal positioning as described in the Wiki.
 */

static void fb_internal_horizontal_positioning( void )
{
    uint32_t    spaces;

    /* Spaces cannot be emitted and tabs cannot be done "backwards". */

    if( current_state.x_address > desired_state.x_address) {
        out_msg( "Cannot move backwards within a text line!\n" );
        err_count++;
        g_suicide();
    }

    /* Set tab_width and spaces. */

    tab_width = desired_state.x_address - current_state.x_address;
    spaces = tab_width / wgml_fonts[active_font].spc_width;

    /* Warn of apparent use of justification. */

    if( (tab_width % wgml_fonts[active_font].spc_width) > 0 ) {
        out_msg( "Warning: justification not supported (internal).\n" );
        wng_count++;
    }

    /* Perform the internal horizontal positioning. */

    if( (bin_driver->htab.text != 0) && (spaces > 8) ) {
        fb_htab();
    } else {
        if( !text_out_open ) pre_text_output();
        output_spaces( spaces );
        if( text_out_open ) post_text_output();
    }
    tab_width = 0;

    return;
}

/* Function fb_first_text_chars().
 * Performs the "first text_chars instance" sequence per the Wiki.
 *
 * Parameter:
 *      in_chars points to the text_chars instance to be processed.
 *
 * Note:
 *      The first step in the Wiki is omitted because that step is only
 *      done on the first line pass, and this function will be used on
 *      subsequent passes as well.
 */

static void fb_first_text_chars( text_chars * in_chars )
{
    bool            font_switch_needed  = true;
    line_proc   *   cur_lineproc        = NULL;

    /* Ensure textpass starts the line pass with value "false". */

    textpass = false;

    /* Initialize the locals. */

    if( wgml_fonts[font_number].font_style != NULL ) {
        if( wgml_fonts[font_number].font_style->lineprocs != NULL ) \       
            cur_lineproc = &wgml_fonts[font_number].font_style->lineprocs[0];
    }
    if( current_state.font_number == desired_state.font_number ) \
                                                    font_switch_needed = false;

    /* Interpret the pre-font switch function blocks. */

    if( text_out_open ) post_text_output();

    /* Do the font switch, if needed. If a font switch is not needed,
     * interpret the :FONTSTYLE block :STARTVALUE block.
     */

    if( font_switch_needed ) {
        fb_font_switch();
    } else {
        if( wgml_fonts[font_number].font_style != NULL ) {
            if( wgml_fonts[font_number].font_style->startvalue != NULL ) {
                df_interpret_driver_functions( \
                        wgml_fonts[font_number].font_style->startvalue->text );
            }
        }
    }

    /* If there is no :LINEPROC block, then set textpass to "true"; if there
     * is a :LINEPROC block, set textpass only if device function
     * %textpass() is processed.
     */

    if( cur_lineproc == NULL ) {
        textpass = true;
    } else {

        if( cur_lineproc->startvalue != NULL ) \
            df_interpret_driver_functions( cur_lineproc->startvalue ->text );

        fb_firstword( cur_lineproc );

        if( !font_switch_needed ) {
            if( cur_lineproc->startword != NULL ) \
                df_interpret_driver_functions( cur_lineproc->startword->text );
        }
    }

    /* If textpass is "true", do the horizontal positioning and text or
     * underscore output. If textpass is not "true", nothing appears in
     * the document! The value of textpass is not changed: it will be used by
     * fb_subsequent_text_chars().
     */

    if( textpass ) {
        fb_initial_horizontal_positioning();

        if( uline ) {
            /* underscore output will go here when implemented */
        } else {
            if( !text_out_open ) pre_text_output();
            ob_insert_block( in_chars->text, in_chars->count, true, true, \
                                                        in_chars->font_number);
        }
    }

    /* Update variables and interpret the post-output function block. */

    current_state.x_address += in_chars->width;
    x_address = current_state.x_address;

    if( cur_lineproc != NULL ) {
        if( text_out_open ) post_text_output();
        if( cur_lineproc->endword != NULL ) {
            df_interpret_driver_functions( cur_lineproc->endword->text );
        }
    }

    return;
}

/* Function fb_new_font_text_chars().
 * Performs the "new font text_chars instance" sequence per the Wiki.
 *
 * Parameter:
 *      in_chars points to the text_chars instance to be processed.
 */

static void fb_new_font_text_chars( text_chars * in_chars )
{
    line_proc   *   cur_lineproc        = NULL;

    /* Ensure textpass has the value "false" at the start of a new font. */

    textpass = false;

    /* Initialize cur_linproc and some globals. */

    if( wgml_fonts[font_number].font_style != NULL ) {
        if( wgml_fonts[font_number].font_style->lineprocs != NULL ) \       
            cur_lineproc = &wgml_fonts[font_number].font_style->lineprocs[0];
    }
    desired_state.font_number = in_chars->font_number;
    desired_state.x_address = in_chars->x_address;
    font_number = desired_state.font_number;
    active_font = desired_state.font_number;
    
    /* Interpret the pre-font switch function blocks. */

    if( text_out_open ) post_text_output();
    fb_lineproc_endvalue();

    /* Do the font switch, which is needed by definition. */

    fb_font_switch();

    /* If there is no :LINEPROC block, then set textpass to "true"; if there
     * is a :LINEPROC block, set textpass only if device function
     * %textpass() is processed.
     */

    if( cur_lineproc == NULL ) {
        textpass = true;
    } else {
        if( cur_lineproc->startvalue != NULL ) \
            df_interpret_driver_functions( cur_lineproc->startvalue ->text );

        fb_firstword( cur_lineproc );
    }

    /* If textpass is "true", do the horizontal positioning and text or
     * underscore output. If textpass is not "true", nothing appears in
     * the document! The value of textpass is not changed: it will be used by
     * fb_subsequent_text_chars().
     */

    if( textpass ) {
        fb_internal_horizontal_positioning();

        if( uline ) {
            /* underscore output will go here when implemented */
        } else {
            if( !text_out_open ) pre_text_output();
            ob_insert_block( in_chars->text, in_chars->count, true, true, \
                                                        in_chars->font_number);
        }
    }

    /* Update variables and interpret the post-output function block. */

    current_state.x_address += in_chars->width;
    x_address = current_state.x_address;

    if( cur_lineproc != NULL ) {
        if( text_out_open ) post_text_output();
        if( cur_lineproc->endword != NULL ) {
            df_interpret_driver_functions( cur_lineproc->endword->text );
        }
    }

    return;
}

/* Function fb_normal_vertical_positioning().
 * Performs the normal vertical positioning as described in the Wiki. 
 */

static void fb_normal_vertical_positioning( void )
{
    uint32_t    current_lines;
    uint32_t    desired_lines;
    uint32_t    current_pages;
    uint32_t    desired_pages;
    uint32_t    device_pages;
    int         i;

    /* A device using :ABSOLUTEADDRESS may be able to move upwards on a given
     * device page, but it cannot go back to a prior device page. A device
     * using :NEWLINE blocks, of course, cannot move upwards at all.
     */

    if( bin_driver->y_positive == 0x00 ) {

        /* y_address is formed by subtraction. */

        if( current_state.y_address < desired_state.y_address ) {
            out_msg( "wgml internal error: cannot move to prior device page\n" );
            err_count++;
            g_suicide();
        }
    } else {

        /* y_address is formed by addition. */

        if( current_state.y_address > desired_state.y_address ) {
            out_msg( "wgml internal error: cannot move to prior device page\n" );
            err_count++;
            g_suicide();
        }
    }

    /* If there is no difference, there is nothing to do. */

    if( current_state.y_address != desired_state.y_address ) {

        /* Detect and process device pages. */

        current_lines = current_state.y_address;
        desired_lines = desired_state.y_address;
        current_pages = current_lines / bin_device->page_depth;
        desired_pages = desired_lines / bin_device->page_depth;
        device_pages = desired_pages - current_pages;

        /* Ensure that (current_pages + i) will contain the number of
         * device pages prior to the current device page.
         */

        current_pages++;

        /* device_pages contains the number of :NEWPAGE blocks needed. */

        if( device_pages > 0 ) {

            for( i=0; i < device_pages; i++ ) {

                /* Interpret the DOCUMENT_PAGE :PAUSE block. */

                if( bin_device->pauses.devpage_pause != NULL ) \
        df_interpret_device_functions( bin_device->pauses.devpage_pause->text );

                /* Interpret the :NEWPAGE block. */

                df_interpret_driver_functions( bin_driver->newpage.text );

                /* If the :ABSOLUTEADDRESS block is defined and this is the
                 * Initial Vertical Positioning, interpret the :LINEPROC
                 * :ENDVALUE block for line pass 1 of available font 0, unless
                 * it has already been done. Note: this places the block at the
                 * proper location when the :ABSOLUTEADDRESS block is defined
                 * but does not include %flushpage() and device paging has
                 * occurred.
                 */

                if( has_aa_block ) {
                    if( at_start ) {
                        if( wgml_fonts[0].font_style->lineprocs != NULL ) {       

                            /* Set the value of current_state.y_address and the
                             * value returned by %y_address() to the last line
                             * of the previous device page.
                             */

                            current_state.y_address = \
                                (current_pages + i) * bin_device->page_depth;
                            y_address = current_state.y_address;

                            if( wgml_fonts[0].font_style->lineprocs[0].endvalue \
                                != NULL ) df_interpret_driver_functions( \
                        wgml_fonts[0].font_style->lineprocs[0].endvalue->text );
                        }
                        at_start = false;
                    }
                }
            }
        }

        /* Set the value of current_state.x_address and the value returned by
         * device function %x_address() start of the line.
         */

        current_state.x_address = bin_device->x_start;
        x_address = current_state.x_address;

        /* Only reset y_address if one or more device pages was emitted. */

        if( device_pages > 0 ) {

            /* Set the value of current_state.y_address to the last line of the
             * previous device page and the value returned by device function
             * %y_address() to the correct value for the current page.
             */

            if( bin_driver->y_positive == 0x00 ) {

                /* y_address is formed by subtraction. */

                current_state.y_address = (page_top + 1) - (desired_pages * \
                                                    bin_device->page_depth);
            } else {

                /* y_address is formed by addition. */

                current_state.y_address = desired_pages * \
                                                        bin_device->page_depth;
            }

            y_address = desired_state.y_address % bin_device->page_depth;
        }

        /* If at_start is still "true", then no device paging occurred. */

        if( at_start ) {
            at_start = false;
            set_margin = true;
        }

        /* If :ABSOLUTEADDRESS is not available, do the vertical positioning. */

        if( !has_aa_block ) fb_newline();
    }

    return;
}

/* Function fb_subsequent_text_chars().
 * Performs the "subsequent text_chars instance" sequence per the Wiki.
 *
 * Parameter:
 *      in_chars points to the text_chars instance to be processed.
 */

static void fb_subsequent_text_chars( text_chars * in_chars )
{
    line_proc   *   cur_lineproc        = NULL;

    /* Initialize cur_lineproc and desired_state.x_address. */

    if( wgml_fonts[font_number].font_style != NULL ) {
        if( wgml_fonts[font_number].font_style->lineprocs != NULL ) \       
            cur_lineproc = &wgml_fonts[font_number].font_style->lineprocs[0];
    }
    desired_state.x_address = in_chars->x_address;
    
    /* Interpret the pre-font switch function blocks. */

    if( text_out_open ) post_text_output();

    /* Since only the :STARTWORD (if present) is interpreted, and since device
     * funtion %textpass() is not allowed in a :STARTWORD block, it cannot be
     * set here. Instead, it retains its setting from the last time a new line
     * started or a new font was encountered. 
     */

    if( cur_lineproc != NULL ) {
        if( cur_lineproc->startword != NULL ) {
            df_interpret_driver_functions( cur_lineproc->startword->text );
        }
    }

    /* If textpass is "true", do the horizontal positioning and text or
     * underscore output. If textpass is not "true", nothing appears in
     * the document! The value of textpass is not changed: it will be used by
     * the next invocation of fb_subsequent_text_chars().
     */

    if( textpass ) {
        fb_internal_horizontal_positioning();

        if( uline ) {
            /* underscore output will go here when implemented */
        } else {
            if( !text_out_open ) pre_text_output();
            ob_insert_block( in_chars->text, in_chars->count, true, true, \
                                                        in_chars->font_number);
        }
    }

    /* Update variables and interpret the post-output function block. */

    current_state.x_address += in_chars->width;
    x_address = current_state.x_address;

    if( cur_lineproc != NULL ) {
        if( text_out_open ) post_text_output();
        if( cur_lineproc->endword != NULL ) \
            df_interpret_driver_functions( cur_lineproc->endword->text );
    }

    return;
}

/*  Global function definitions. */

/* Function df_initialize_pages().
 * Ensures pages is 0, sets page_top to the parameter's value, and then
 * uses df_increment_pages() to onitializes pages and set the state
 * variables to the top of a new document page.
 *
 * Parameter:
 *      in_page_top contains the vertical location of the top of a
 *          document page.
 */

void df_initialize_pages( uint32_t in_page_top )
{
    page_top = in_page_top;
    pages = 0;
    df_increment_pages();
    return;
}


/* Function df_increment_pages().
 * Increments pages and sets the state variables to the top of a new
 * document page.
 */

void df_increment_pages( void )
{
    pages++;
    desired_state.x_address = bin_device->x_start;
    desired_state.y_address = page_top;
    current_state.x_address = bin_device->x_start;
    current_state.y_address = page_top;
    return;
}

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

/* Function df_new_section().
 * Performs the work of fb_new_section(). This function is very specialized.
 *
 * Parameter:
 *      v_start contains the desired starting vertical position.
 */
 
void df_new_section( uint32_t v_start )
{
    uint32_t    save_font;

    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* Save active_font and set it to 0 for the :NEWPAGE and :NEWLINE blocks. */

    save_font = active_font;
    active_font = 0;

    /* Interpret the DOCUMENT_PAGE :PAUSE block. */

    if( bin_device->pauses.docpage_pause != NULL ) \
        df_interpret_device_functions( bin_device->pauses.docpage_pause->text );

    /* Interpret the :NEWPAGE block. */

    df_interpret_driver_functions( bin_driver->newpage.text );

    /* Set up for a new document page. */

    df_increment_pages();

    /* Do the initial vertical positioning for the section. */

    desired_state.y_address = v_start;
    fb_normal_vertical_positioning();

    /* Restore the value of active_font. This ensures that the next font
     * switch decision and font switch, if any, will be done using the
     * correct fonts.
     */

    active_font = save_font;

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
    device_function_table[0x27] = &df_default_width;
    device_function_table[0x2a] = &df_page_depth;
    device_function_table[0x2b] = &df_page_width;
    device_function_table[0x31] = &df_font_height;
    device_function_table[0x32] = &df_font_space;
    device_function_table[0x33] = &df_line_height;
    device_function_table[0x34] = &df_line_space;
    device_function_table[0x36] = &df_wgml_header;
    device_function_table[0x37] = &df_font_outname1;
    device_function_table[0x38] = &df_font_outname2;
    device_function_table[0x39] = &df_font_resident;
    device_function_table[0x3a] = &df_time;
    device_function_table[0x3b] = &df_date;
    return;
}

/* Function df_populate_driver_table().
 * Modifies the entry for %flushpage() in driver_function_table so that this
 * device function will now, in fact work. This should be called after the
 * virtual %enterfont(0) is performed and before the initial vertical
 * positioning. 
 */
 
void df_populate_driver_table( void )
{
    driver_function_table[0x1D] = &df_flushpage;
    return;
}

/* Function df_set_horizontal().
 * Does the horizontal positioning for fb_position(). This function is extremely
 * specialized and should not be called by any function other than
 * fb_position(). 
 *
 * Parameter:
 *      h_start contains the new value for current_state.x_address
 *      and x_address.
 */

void df_set_horizontal( uint32_t h_start )
{
    bool    fontstyle = false;

    /* If set_margin is set to "true", then determine whether a :FONTSTYLE
     * block for line pass 1 for font 0 exists. If set_margin is set to "false",
     * then the :FONTSTYLE block will be treated as not existing whether it
     * exists or not.
     */

    if( set_margin ) {
        if( wgml_fonts[0].font_style != NULL ) {
            if( wgml_fonts[0].font_style->lineprocs !=NULL ) {
                fontstyle = true;
            }
        }
    }

    /* If the :FONTSTYLE block exists, interpret the :ENDVALUE block. */

    if( fontstyle ) df_interpret_driver_functions( \
                        wgml_fonts[0].font_style->lineprocs[0].endvalue->text );

    /* Set the variables whether the :FONTSTYLE block exists or not. */

    desired_state.x_address = h_start;
    x_address = h_start;

    /* If the :FONTSTYLE block exists, interpret the :STARTVALUE block and
     * the :FIRSTVALUE block.
     */

    if( fontstyle ) {
        df_interpret_driver_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].startvalue->text );
        fb_firstword( &wgml_fonts[0].font_style->lineprocs[0] );
    }

    return;
}

/* Function df_set_vertical().
 * Does the vertical positioning for fb_position(). This function is extremely
 * specialized and should not be called by any function other than
 * fb_position(). 
 *
 * Parameter:
 *      v_start contains the new value for current_state.y_address.
 *
 * Note:
 *      if the second call to fb_position() occurs, the value of v_start
 *      should be the same as it was on the first call, in which case
 *      nothing happens. This avoids resetting the value returned by
 *      %x_address() to bin_device->x_start on the second call.
 */

void df_set_vertical( uint32_t v_start )
{
    if( desired_state.y_address != v_start ) {
        desired_state.y_address = v_start;
        fb_normal_vertical_positioning();
    }
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

    staging.text = (uint8_t *) mem_alloc( 80 );
    staging.length = 80;
    staging.current = 0;

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
    
    if( staging.text != NULL ) {
        mem_free( staging.text);
        staging.current = 0;
        staging.length = 0;
        staging.text = NULL;
    }

    return;
}

/* Function fb_enterfont().
 * Performs the action of device function %enterfont(0), whether the device
 * function was invoked explicitly or implicitly. Parts of this function may
 * eventually be refactored if needed in other functions.
 *
 * Note:
 *      active_font and font_number are set to "0" to ensure that all function
 *      blocks will be done in the context of the default font. They are
 *      restored to their initial value on exit.
 */

void fb_enterfont( void )
{
    uint32_t    old_active_font;
    uint32_t    old_font_number;

    old_active_font = active_font;
    old_font_number = font_number;
    active_font = 0;
    font_number = 0;

    if( wgml_fonts[0].font_pause != NULL ) \
        df_interpret_device_functions( wgml_fonts[0].font_pause->text );

    if( wgml_fonts[0].font_switch != NULL ) {
        if( wgml_fonts[0].font_switch->startvalue != NULL ) \
            df_interpret_driver_functions( \
                                wgml_fonts[0].font_switch->startvalue->text );
    }

    if( wgml_fonts[0].font_style != NULL ) {       
        if( wgml_fonts[0].font_style->startvalue != NULL ) \
            df_interpret_driver_functions( \
                                wgml_fonts[0].font_style->startvalue->text );
        if( wgml_fonts[0].font_style->lineprocs != NULL ) {       
            if( wgml_fonts[0].font_style->lineprocs[0].startvalue != NULL ) \
                df_interpret_driver_functions( \
                    wgml_fonts[0].font_style->lineprocs[0].startvalue->text );
            fb_firstword( &wgml_fonts[0].font_style->lineprocs[0] );
        }
    }
    active_font = old_active_font;
    font_number = old_font_number;

    return;
}

/* Function fb_first_text_pass().
 * Performs the first pass for outputting ordinary text.
 *
 * Parameter:
 *      out_line points to a text_line instance specifying the text to be
 *          sent to the device.
 *
 * Note:
 *      This function is to be used only by fb_output_text(), which includes
 *          a test ensuring that out_line contains at least one text_chars
 *          instance (that is, that out_line->first is not NULL).
 *      Other functions are expected to be created for outputting lines
 *          created using :BOX characters.
 */

void fb_first_text_pass( text_line * out_line )
{
    text_chars  *   current;

    /* Update the internal state for the new text_line. */

    current = out_line->first;
    desired_state.font_number = current->font_number;
    desired_state.x_address   = current->x_address;
    desired_state.y_address   = out_line->y_address;

    /* Interpret a :LINEPROC :ENDVALUE block if appropriate. */

    fb_lineproc_endvalue();

    /* Perform the Normal Vertical Positioning. */

    fb_normal_vertical_positioning();

    /* Update the font number. */

    font_number = desired_state.font_number;
    active_font = desired_state.font_number;

    /* The First Line Pass Sequence. */ 

    /* The "first text_chars instance" sequence. */

    x_address = desired_state.x_address;
    fb_first_text_chars( current );

    /* Now do the remaining text_chars instances. */

    current = current->next;
    while( current != NULL ) {
        desired_state.x_address = current->x_address;
        if( current_state.font_number != current->font_number ) {
            fb_new_font_text_chars( current );
        } else {
            fb_subsequent_text_chars( current );
        }
        current = current->next;
    }

    /* Close text output if still open at end of line. */

    if( text_out_open ) post_text_output();

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
    uint32_t         old_active_font;
    uint32_t         old_font_number;

    /* An empty init_block is not an error. */

    if( in_block == NULL ) return;

    /* These should be zero, but save them just to be sure. */

    old_active_font = active_font;
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

    /* Restore the original values, which should be zero. */

    active_font = old_active_font;
    font_number = old_font_number;

    return;
}

/* Function fb_line_block().
 * Interprets any of the :HLINE, :VLINE, or :DBOX blocks, which define
 * attribute "thickness". Sets x_size, y_size, and thickness to match the
 * parameters. 
 *
 * Parameters:
 *      in_line_block points to the :HLINE, :VLINE, or :DBOX block.
 *      h_start contains the horizontal start position.
 *      v_start contains the vertical start position.
 *      h_len contains the horizontal extent for a :HLINE or :DBOX block.
 *      v_len contains the vertical extent for a :VLINE or :DBOX block.
 *      name contains the name (:HLINE, :VLINE, or :DBOX) of the block.
 *
 * Prerequisites:
 *      The block to be interpreted must exist.
 *      The :ABSOLUTEADDRESS block must exist.
 *
 * Notes:
 *      The :ABSOLUTEADDRESS block is required to position the print to the
 *          start of the line or box.
 *      The block must exist because the box-drawing code should be checking
 *          this and, in some cases, drawing the line or box using the :BOX
 *          block characters instead.
 */

void fb_line_block( line_block * in_line_block, uint32_t h_start, \
                    uint32_t v_start, uint32_t h_len, uint32_t v_len, \
                    char * name )
{
    /* An empty block is definitely an error. */

    if( in_line_block->text == NULL ) {
        out_msg("wgml internal error: The %s block must be defined if used\n", \
                                                                        name );
        err_count++;
        g_suicide();
    }

    /* As is a missing :ABSOLUTEADDRESS block. */
    
    if( !has_aa_block ) {
        out_msg("The :ABSOLUTEADDRESS must be defined if the %s is defined\n", \
                                                                        name );
        err_count++;
        g_suicide();
    }

    /* Set up for fb_absoluteaddress(). */

    desired_state.x_address = h_start;
    desired_state.y_address = v_start;
    x_address = h_start;
    y_address = v_start % bin_device->page_depth;

    fb_absoluteaddress();

    /* Set up for in_function; current_state has been updated by
     * fb_absoluteaddress().
     */

    x_size = h_len;
    y_size = v_len;
    thickness = in_line_block->thickness;
    df_interpret_driver_functions( in_line_block->text );

    /* Clear the values not needed outside this block. */

    x_size = 0;
    y_size = 0;
    thickness = 0;

    return;
}

/* Function fb_lineproc_endvalue()
 * Checks the value of the text_output flag and interprets the :LINEPROC
 * :ENDVALUE block for the current font if appropriate.
 */

void fb_lineproc_endvalue( void )
{
    if( text_output ) {
        post_text_output();
        if( wgml_fonts[font_number].font_style->lineprocs != NULL ) {       
            if( wgml_fonts[font_number].font_style->\
                                    lineprocs[line_pass_number].endvalue != NULL ) \
                df_interpret_driver_functions( \
                    wgml_fonts[font_number].font_style->lineprocs[line_pass_number].\
                                                            endvalue->text );
        }
    }

    return;
}

