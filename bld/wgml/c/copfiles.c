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
*                   cop_setup()
*                   cop_teardown()
*                   fb_start()
*               plus these local functions:
*                   find_cop_font()
*                   find_dev_font()
*                   find_style()
*                   find_switch()
*                   free_opt_fonts()
*                   get_cop_device()
*                   get_cop_driver()
*                   get_cop_font()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <setjmp.h> // Required (but not included) by gvars.h.
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
#include "findfile.h"
#include "gtype.h" // Required (but not included) by gvars.h.
#include "gvars.h"
#include "outbuff.h"
#include "wgml.h"

/* Local function definitions. */

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
        out_msg( "The file for the font %s was not found\n", in_name );
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
 */

static cop_font * find_cop_font( char const * in_name )
{
    cop_font    *   current = NULL;
    cop_font    *   retval  = NULL;

    current = bin_fonts;
    while( current != NULL ) {
        if( !stricmp( in_name, current->defined_name ) ) {
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
 */

static device_font * find_dev_font( char const * in_name )
{
    devicefont_block    *   current = NULL;
    device_font         *   retval  = NULL;
    int                     i;

    current = &bin_device->devicefonts;
    for( i = 0; i < current->font_count; i++ ) {
        if( !stricmp( in_name, current->fonts[i].font_name ) ) {
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

/* Extern function definitions */

/* Function cop_setup().
 * Initializes the various globals specific to the binary device library subsystem.
 */

extern void cop_setup( void )
{
    default_font    *   cur_def_fonts   = NULL;
    device_font     *   cur_dev_font    = NULL;
    int                 font_base       = 0;
    int                 gen_cnt         = 0;
    int                 i;
    opt_font        *   cur_opt         = NULL;
    wgml_font           def_font;

    out_msg( "Processing device information\n" );

    /* Set the globals to known values. */

    bin_device = NULL;
    bin_driver = NULL;
    bin_fonts = NULL;
    wgml_font_cnt = 0;
    wgml_fonts = NULL;

    /* A "device" option must have been processed,
     * and it must have provided a device name.
     */

    if( (dev_name == NULL) || !strcmp(dev_name, "''") ) {
        out_msg( "A device name is required!\n" );
        err_count++;
        g_suicide();
    }

    bin_device = get_cop_device( dev_name );

    if( bin_device == NULL ) {
        out_msg( "Designated device not found: %s\n", dev_name );
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

    /* Get the highest font_number. */

    wgml_font_cnt = bin_device->defaultfonts.font_count;

    cur_opt = opt_fonts;
    while( cur_opt != NULL ) {
        if( cur_opt->font > wgml_font_cnt ) {
           wgml_font_cnt = cur_opt->font;
        }
        cur_opt = cur_opt->nxt;
    }

    /* The value needed for the upper bound of the zero-based array is the
     * highest font number plus 1, as stated in the Wiki.
     */

     wgml_font_cnt++;

    /* If either :BOX or :UNDERSCORE provided a font name, increment the count.
     * If both :BOX and :UNDERSCORE provided a font name, increment the count
     * once if the font names were identical, twice if they were different.
     */

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
        if( cur_def_fonts[i].font_name == NULL ) {
            continue; /* Do not initialize skipped font numbers. */
        } else {
            wgml_fonts[i].bin_font = find_cop_font( cur_def_fonts[i].font_name );
        }
        if( cur_def_fonts[i].font_style == NULL ) {
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
        cur_opt = cur_opt->nxt;
    }
    free_opt_fonts();    

    /* Generate any entries required by the :BOX and/or :UNDERSCORE blocks.
     * Note that the font_number will become non-zero and will be used in
     * document processing instead of the font name.
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
    def_font.font_height = wgml_fonts[0].font_height;
    def_font.font_space = wgml_fonts[0].font_space;

    for( i = 0; i < wgml_font_cnt; i++ ) {
        if( wgml_fonts[i].bin_font == NULL ) {
            wgml_fonts[i].bin_font = def_font.bin_font;
            wgml_fonts[i].font_switch = def_font.font_switch;
            wgml_fonts[i].font_pause = def_font.font_pause;
            wgml_fonts[i].font_style = def_font.font_style;
            wgml_fonts[i].font_height = def_font.font_height;
            wgml_fonts[i].font_space = def_font.font_space;
        }
    }

    /* Ensure that at least one binary :FONT block was processed. */

    if( bin_fonts == NULL ) {
        out_msg( "Device Library Error: No :FONT blocks loaded\n" );
        err_count++;
        g_suicide;
    }

    /* Initialize the dependent module. */

    ob_setup();

    return;
}

/* Function cop_teardown().
 * Frees all memory allocated by cop_setup().
 * This is where making cop_device, cop_driver and cop_font monolithic blocks of
 * allocated memory pays off!
 */

extern void cop_teardown( void )
{

    if( bin_device != NULL ) {
        mem_free( bin_device );
        bin_device = NULL;
    }
    
    if( bin_driver  != NULL ) {
        mem_free( bin_driver );
        bin_driver = NULL;
    }

    if( bin_fonts != NULL) {

        cop_font    *   old;

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

    /* Release any memory allocated by the dependent module. */

    ob_teardown();

    return;
}

/* Function fb_start().
 * Performs the processing which occurs before document processing starts. Indeed,
 * wgml 4.0 does not even look for the document specification file until the
 * actions taken here have been completed!
 */

extern void fb_start( void )
{
    /* Interpret the START :PAUSE block. */

    /* Interpret the START :INIT block. */

    return;
}
