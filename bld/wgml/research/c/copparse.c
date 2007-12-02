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
* Description:  Parses .COP files based on the defined name.
*               In addition to main(), these functions are implemented:
*                   cop_initialize_globals()
*                   display_device()
*                   display_driver()
*                   display_font()
*                   parse_defined_name()
*                   print_banner()
*                   print_usage()
*               These items from wgml.c are implemented here because the
*               dual main() functions that result from linking wgml.c do not
*               prevent linking but do prevent debugging:
*                   Pathes
*                   GMLlibs
*                   GMLincs
*                   get_env_vars()
*                   GML_get_env()
*                   mem_alloc()
*                   mem_realloc()
*                   mem_free()
*                   out_msg()
*
*                   check_directory()
*                   verify_device()
*                   verify_driver()
*                   verify_font()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <process.h>
#include <stdio.h>
#include <stdlib.h>

#include "banner.h"
#include "common.h"
#include "copfiles.h"
#include "research.h"

/* Done here because gvars.c includes wgml.h, and wgml.c cannot be linked in. */

char * Pathes;        // content of PATH Envvar
char * GMLlibs;       // content of GMMLIB Envvar
char * GMLincs;       // content of GMLINC Envvar

/* Capture the environmental variables */

char * GML_get_env( char * name )
{
    errno_t     rc;
    size_t      len;
    char    *   value;
    size_t      maxsize;

    maxsize = 128;
    value = mem_alloc( maxsize );
    rc = getenv_s( &len, value, maxsize, name );
    if( rc ) {
        mem_free( value );
        value = NULL;
        if( len ) {   /*  we need more space */
            maxsize = len + 1;
            value = mem_alloc( maxsize );
            rc = getenv_s( &len, value, maxsize, name );
        }
    }
    if( len == 0 ) {
        if( value != NULL ) {
            mem_free( value );
        }
        value = NULL;
    }
    return( value );
}


void get_env_vars( void )
{
    Pathes  = GML_get_env( "PATH" );
    GMLlibs = GML_get_env( "GMLLIB" );
    GMLincs = GML_get_env( "GMLINC" );
}

/* The memory allocation functions. These have been simplified. */

void * mem_alloc( size_t size )
{
    void    *   p;

    p = malloc( size );
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL" );
        exit(EXIT_FAILURE);
    }
    return( p );
}

void * mem_realloc( void * p, size_t size )
{
    p = realloc( p, size );
    if( p == NULL ) {
        out_msg( "ERR_NOMEM_AVAIL" );
        exit(EXIT_FAILURE);
    }
    return( p );
}

void mem_free( void * p )
{
    free( p );
    p = NULL;
}

/* Error message centralized output */

#include <stdarg.h>

void out_msg( char * msg, ... )
{
    va_list args;

    va_start( args, msg );
    vprintf_s( msg, args );
    va_end( args );
}

/*  Local variables */

/*  Load the usage text array */

static  char const *    usage_text[] = {
#include "copusage.h"
NULL
};

/* Local function definitions. */

/* Function display_device().
 * Displays the contents of a cop_device instance.
 *
 * Parameter:
 *      in_device is a pointer to the cop_device instance.
 */

void display_device( cop_device * in_device)
{
    char        font_character[2];
    int         i;
    int         j;
    char        translation[2];

    printf_s( "Allocated size:            %i\n", in_device->allocated_size );
    printf_s( "Bytes used:                %i\n", in_device->next_offset );
    if( in_device->driver_name == NULL ) puts( "Driver Name:");
    else printf_s( "Driver Name:               %s\n", in_device->driver_name );
    if( in_device->output_name == NULL ) puts( "Output File Name:" );
    else printf_s( "Output File Name:          %s\n", in_device->output_name );
    if( in_device->output_extension == NULL ) puts( "Output File Extension:" );
    else printf_s( "Output File Extension:     %s\n", in_device->output_extension );
    printf_s( "Page Width:                %i\n", in_device->page_width );
    printf_s( "Page Depth:                %i\n", in_device->page_depth );
    printf_s( "Horizontal Base Units:     %i\n", in_device->horizontal_base_units );
    printf_s( "Vertical Base Units:       %i\n", in_device->vertical_base_units );
    printf_s( "Page Start X Value:        %i\n", in_device->x_start );
    printf_s( "Page Start Y Value:        %i\n", in_device->y_start );
    printf_s( "Page Offset X Value:       %i\n", in_device->x_offset );
    printf_s( "Page Offset Y Value:       %i\n", in_device->y_offset );
    if( in_device->box.font_name == NULL ) printf_s( "Box Font Number:           %i\n", in_device->box.font_number);
    else printf_s( "Box Font Name:             %s\n", in_device->box.font_name );
    printf_s( "Horizontal line character: %c\n", in_device->box.horizontal_line );
    printf_s( "Vertical line character:   %c\n", in_device->box.vertical_line );
    printf_s( "Top left character:        %c\n", in_device->box.top_left );
    printf_s( "Top right character:       %c\n", in_device->box.top_right );
    printf_s( "Bottom left character:     %c\n", in_device->box.bottom_left );
    printf_s( "Bottom right character:    %c\n", in_device->box.bottom_right );
    printf_s( "Top join character:        %c\n", in_device->box.top_join );
    printf_s( "Bottom join character:     %c\n", in_device->box.bottom_join );
    printf_s( "Left join character:       %c\n", in_device->box.left_join );
    printf_s( "Right join character:      %c\n", in_device->box.right_join );
    printf_s( "Inside join character:     %c\n", in_device->box.inside_join );
    if( in_device->underscore.font_name == NULL ) printf_s( "Underscore Font Number:    %i\n", in_device->underscore.font_number);
    else printf_s( "Underscore Font Name:      %s\n", in_device->underscore.font_name );
    printf_s( "Underscore character:      %c\n", in_device->underscore.underscore_char );
    if( in_device->intrans == NULL) {
        puts( "No Intrans Table");
    } else {
        puts( "Intrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_device->intrans->table[i] != i ) {
                display_char( font_character, (char) i );
                display_char( translation, in_device->intrans->table[i] );
                printf_s( "%c%c %c%c\n", font_character[0], font_character[1], translation[0], translation[1] );
            }
        }
    }
    if( in_device->outtrans == NULL) {
        puts( "No Outtrans Table");
    } else {
        puts( "Outtrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_device->outtrans->table[i] != NULL ) {
                display_char( font_character, (char) i );
                printf_s( "%c%c ", font_character[0], font_character[1] );
                for( j = 0; j < in_device->outtrans->table[i]->count; j++ ) {
                    display_char( translation, in_device->outtrans->table[i]->data[j] );
                    printf_s( "%c%c ", translation[0], translation[1] );
                }
                puts( "" );
            }
        }
    }
    printf_s( "Number of Default Fonts: %i\n", in_device->defaultfonts.count );
    for( i = 0; i < in_device->defaultfonts.count; i++ ) {
        printf_s( "  Default Font Number  %i:\n", i );
        if( in_device->defaultfonts.font[i].font_name == NULL ) puts( "    Font Name:");
        else printf_s( "    Font Name:         %s\n", in_device->defaultfonts.font[i].font_name );
        if( in_device->defaultfonts.font[i].font_style == NULL ) puts( "    FontStyle:");
        else printf_s( "    Font Style:        %s\n", in_device->defaultfonts.font[i].font_style );
        printf_s( "    Font Height:       %i\n", in_device->defaultfonts.font[i].font_height );
        printf_s( "    Font Space:        %i\n", in_device->defaultfonts.font[i].font_space );
    }
    if( in_device->pauses.start_pause == NULL ) puts( "No START Pause" );
    else {
        puts( "START Pause:" );
        display_hex_block( in_device->pauses.start_pause->text, in_device->pauses.start_pause->count );
    }
    if( in_device->pauses.document_pause == NULL ) puts( "No DOCUMENT Pause" );
    else {
        puts( "DOCUMENT Pause:" );
        display_hex_block( in_device->pauses.document_pause->text, in_device->pauses.document_pause->count );
    }
    if( in_device->pauses.document_page_pause == NULL ) puts( "No DOCUMENT_PAGE Pause" );
    else {
        puts( "DOCUMENT_PAGE Pause:" );
        display_hex_block( in_device->pauses.document_page_pause->text, in_device->pauses.document_page_pause->count );
    }
    if( in_device->pauses.device_page_pause == NULL ) puts( "No DEVICE_PAGE Pause" );
    else {
        puts( "DEVICE_PAGE Pause:" );
        display_hex_block( in_device->pauses.device_page_pause->text, in_device->pauses.device_page_pause->count );
    }
    printf_s( "Number of Device Fonts: %i\n", in_device->devicefonts.count );
    for( i = 0; i < in_device->devicefonts.count; i++ ) {
        printf_s( "  Device Font Index:   %i:\n", i );
        if( in_device->devicefonts.font[i].font_name == NULL ) puts( "    Font Name:");
        else printf_s( "    Font Name:         %s\n", in_device->devicefonts.font[i].font_name );
        if( in_device->devicefonts.font[i].font_switch == NULL ) puts( "    Font Switch:");
        else printf_s( "    Font Switch:       %s\n", in_device->devicefonts.font[i].font_switch );
        printf_s( "    Resident Font:     %i\n", in_device->devicefonts.font[i].resident );
        if( in_device->devicefonts.font[i].font_pause == NULL ) puts( "    No Font Pause" );
        else {
            puts( "    Font Pause:" );
            display_hex_block( in_device->devicefonts.font[i].font_pause->text, in_device->devicefonts.font[i].font_pause->count );
        }
    }

    return;
}

/*  Function display_driver().
 *  Displays the contents of a cop_driver instance.
 *
 *  Parameter:
 *      in_driver is a pointer to the cop_driver instance.
 */

void display_driver( cop_driver * in_driver )
{
    int i;
    int j;
    
    printf_s( "Allocated size:            %i\n", in_driver->allocated_size );
    printf_s( "Bytes used:                %i\n", in_driver->next_offset );
    if( in_driver->rec_spec == NULL ) puts( "Record Specification:");
    else printf_s( "Record Specification:      %s\n", in_driver->rec_spec );
    printf_s( "Fill Character:            %c\n", in_driver->fill_char );
    puts( "Page Address Flags:");
    printf_s( "  x_positive:              %i\n", in_driver->x_positive );
    printf_s( "  y_positive:              %i\n", in_driver->y_positive );
    puts( "Start :INIT Block:" );
    if( in_driver->inits.start != NULL ) {
        for( i = 0; i < in_driver->inits.start->count; i++ ) {
            if( in_driver->inits.start->codetext[i].is_fontvalue ) puts( ":FONTVALUE Block:");
            else puts( ":VALUE Block:");
            display_hex_block( in_driver->inits.start->codetext[i].text, in_driver->inits.start->codetext[i].count );
        }
    }
    puts( "Document :INIT Block:" );
    if( in_driver->inits.document != NULL ) {
        for( i = 0; i < in_driver->inits.document->count; i++ ) {
            if( in_driver->inits.document->codetext[i].is_fontvalue ) puts( ":FONTVALUE Block:");
            else puts( ":VALUE Block:");
            display_hex_block( in_driver->inits.document->codetext[i].text, in_driver->inits.document->codetext[i].count );
        }
    }
    puts( "End :FINISH Block:" );
    if( in_driver->finishes.end != NULL ) {
        display_hex_block( in_driver->finishes.end->text, in_driver->finishes.end->count );
    }
    puts( "Document :FINISH Block:" );
    if( in_driver->finishes.document != NULL ) {
        display_hex_block( in_driver->finishes.document->text, in_driver->finishes.document->count );
    }
    if( in_driver->newlines.newlineblocks == NULL ) puts( ":NEWLINE Block:");
    else {
        puts( ":NEWLINE Block(s):" );
        for( i = 0; i < in_driver->newlines.count; i++ ) {
            printf_s( "  Advance: %i\n", in_driver->newlines.newlineblocks[i].advance );
            if( in_driver->newlines.newlineblocks[i].text != NULL ) {
                display_hex_block( in_driver->newlines.newlineblocks[i].text, in_driver->newlines.newlineblocks[i].count );
            }
        }
    }
    puts( ":NEWPAGE Block:" );
    if( in_driver->newpage.text != NULL ) {
        display_hex_block( in_driver->newpage.text, in_driver->newpage.count );
    }
    puts( ":HTAB Block:" );
    if( in_driver->htab.text != NULL ) {
        display_hex_block( in_driver->htab.text, in_driver->htab.count );
    }
    if( in_driver->fontswitches.fontswitchblocks == NULL ) puts( ":FONTSWITCH Block:");
    else {
        puts( ":FONTSWITCH Block(s):" );    
        for( i = 0; i < in_driver->fontswitches.count; i++ ) {
            printf_s( "  Type: %s\n", in_driver->fontswitches.fontswitchblocks[i].type );
            if( in_driver->fontswitches.fontswitchblocks[i].startvalue != NULL ) {
                puts( "  :STARTVALUE Block:");
                display_hex_block( in_driver->fontswitches.fontswitchblocks[i].startvalue->text, in_driver->fontswitches.fontswitchblocks[i].startvalue->count );
            }
            if( in_driver->fontswitches.fontswitchblocks[i].endvalue != NULL ) {
                puts( "  :ENDVALUE Block:");
                display_hex_block( in_driver->fontswitches.fontswitchblocks[i].endvalue->text, in_driver->fontswitches.fontswitchblocks[i].endvalue->count );
            }
        }
    }
    if( in_driver->fontstyles.fontstyle_list == NULL ) puts( ":FONTSTYLE Block:");
    else {
        puts( ":FONTSTYLE Block(s):" );    
        for( i = 0; i < in_driver->fontstyles.count; i++ ) {
            printf_s( "  Type: %s\n", in_driver->fontstyles.fontstyle_list[i].type );
            if( in_driver->fontstyles.fontstyle_list[i].startvalue == NULL ) {
                puts( "  No :STARTVALUE Block");
            } else {
                puts( "  :STARTVALUE Block:");
                display_hex_block( in_driver->fontstyles.fontstyle_list[i].startvalue->text, in_driver->fontstyles.fontstyle_list[i].startvalue->count );
            }
            if( in_driver->fontstyles.fontstyle_list[i].endvalue == NULL ) {
                puts( "  No :ENDVALUE Block");
            } else {
                puts( "  :ENDVALUE Block:");
                display_hex_block( in_driver->fontstyles.fontstyle_list[i].endvalue->text, in_driver->fontstyles.fontstyle_list[i].endvalue->count );
            }
            if(in_driver->fontstyles.fontstyle_list[i].lineprocs == NULL ) {
                puts( "  No :LINEPROC Blocks");
            } else {
                puts( "  :LINEPROC Block(s):");
                for( j = 0; j < in_driver->fontstyles.fontstyle_list[i].passes; j++ ) { 
                    printf_s( "  Pass: %i\n", j+1 );
                    if( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startvalue == NULL ) {
                        puts( "  No :STARTVALUE Block");
                    } else {
                        puts( "  :STARTVALUE Block:");
                        display_hex_block( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startvalue->text, in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startvalue->count );
                    }
                    if( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].firstword == NULL ) {
                        puts( "  No :FIRSTWORD Block");
                    } else {
                        puts( "  :FIRSTWORD Block:");
                        display_hex_block( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].firstword->text, in_driver->fontstyles.fontstyle_list[i].lineprocs[j].firstword->count );
                    }
                    if( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startword == NULL ) {
                        puts( "  No :STARTWORD Block");
                    } else {
                        puts( "  :STARTWORD Block");
                        display_hex_block( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startword->text, in_driver->fontstyles.fontstyle_list[i].lineprocs[j].startword->count );
                    }
                    if( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endword == NULL ) {
                        puts( "  No :ENDWORD Block");
                    } else {
                        puts( "  :ENDWORD Block:");
                        display_hex_block( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endword->text, in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endword->count );
                    }
                    if( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endvalue == NULL ) {
                        puts( "  No :ENDVALUE Block");
                    } else {
                        puts( "  :ENDVALUE Block:");
                        display_hex_block( in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endvalue->text, in_driver->fontstyles.fontstyle_list[i].lineprocs[j].endvalue->count );
                    }
                }
            }
        }
    }
    if( in_driver->absoluteaddress.text == NULL ) {
        puts( "No :ABSOLUTEADDRESS Block" );
    } else {
    puts( ":ABSOLUTEADDRESS Block:" );
        display_hex_block( in_driver->absoluteaddress.text, in_driver->absoluteaddress.count );
    }
    if( in_driver->hline.text == NULL ) {
        puts( "No :HLINE Block" );
    } else {
    puts( ":HLINE Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->hline.thickness );
        display_hex_block( in_driver->hline.text, in_driver->hline.count );
    }
    if( in_driver->vline.text == NULL ) {
        puts( "No :VLINE Block" );
    } else {
    puts( ":VLINE Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->vline.thickness );
        display_hex_block( in_driver->vline.text, in_driver->vline.count );
    }
    if( in_driver->dbox.text == NULL ) {
        puts( "No :DBOX Block" );
    } else {
    puts( ":DBOX Block:" );
        printf_s( "  Thickness:               %i\n", in_driver->dbox.thickness );
        display_hex_block( in_driver->dbox.text, in_driver->dbox.count );
    }

    return;
}

/*  Function display_font().
 *  Displays the contents of a cop_font instance.
 *
 *  Parameter:
 *      in_font is a pointer to the cop_font instance.
 */

void display_font( cop_font * in_font )
{

    char        font_character[2];
    int         i;
    int         j;
    char        translation[2];

    printf_s( "Allocated size:            %i\n", in_font->allocated_size );
    printf_s( "Bytes used:                %i\n", in_font->next_offset );
    if( in_font->font_out_name1 == NULL ) puts( "Font Output Name 1:");
    else printf_s( "Font Output Name 1:        %s\n", in_font->font_out_name1 );
    if( in_font->font_out_name2 == NULL ) puts( "Font Output Name 2:" );
    else printf_s( "Font Output Name 2:        %s\n", in_font->font_out_name2 );
    printf_s( "Line Height:               %i\n", in_font->line_height );
    printf_s( "Line Space:                %i\n", in_font->line_space );
    printf_s( "Scale Basis:               %i\n", in_font->scale_basis );
    printf_s( "Scale Minimum:             %i\n", in_font->scale_min );
    printf_s( "Scale Maximum:             %i\n", in_font->scale_max );
    printf_s( "Character Width:           %i\n", in_font->char_width );
    if( in_font->intrans == NULL) {
        puts( "No Intrans Table");
    } else {
        puts( "Intrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_font->intrans->table[i] != i ) {
                display_char( font_character, (char) i );
                display_char( translation, in_font->intrans->table[i] );
                printf_s( "%c%c %c%c\n", font_character[0], font_character[1], translation[0], translation[1] );
            }
        }
    }
    if( in_font->outtrans == NULL) {
        puts( "No Outtrans Table");
    } else {
        puts( "Outtrans Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_font->outtrans->table[i] != NULL ) {
                display_char( font_character, (char) i );
                printf_s( "%c%c ", font_character[0], font_character[1] );
                for( j = 0; j < in_font->outtrans->table[i]->count; j++ ) {
                    display_char( translation, in_font->outtrans->table[i]->data[j] );
                    printf_s( "%c%c ", translation[0], translation[1] );
                }
                puts( "" );
            }
        }
    }
    if( in_font->width == NULL) {
        puts( "No Width Table");
    } else {
        puts( "Width Table:" );
        for( i = 0; i < 0x100; i++ ) {
            if( in_font->width->table[i] != in_font->char_width ) {
                display_char( font_character, (char) i );
                printf_s( "%c%c %lu\n", font_character[0], font_character[1], in_font->width->table[i] );
            }
        }
    }

    return;
}

/* Function parse_defined_name().
 * Parses the file corresponding to the defined name provided on the command
 * line, if any, to include displaying its contents.
 *
 * The actual parsing is done using the functions declared in copfiles.h.
 * This function is, in fact, the test function for those functions.
 *
 * Note: since mem_alloc() is used to allocate these structs, mem_free()
 *      must be used to free them. 
 *
 * Global Used:
 *      tgt_path contains the defined name passed on the command line
 *
 * Returns:
 *      SUCCESS if the defined name was parsed without error 
 *      FAILURE if the any error occurred
 */

int parse_defined_name( void )
{
    cop_device *    current_device  = NULL;
    cop_driver *    current_driver  = NULL;
    cop_font   *    current_font    = NULL;

    printf_s( "Defined Name: %s\n", tgt_path );

    current_device = get_cop_device( tgt_path );
    if( current_device != NULL) {
        display_device( current_device );
        mem_free( current_device );
        return( SUCCESS );
    }

    current_driver = get_cop_driver( tgt_path );
    if( current_driver != NULL) {
        display_driver( current_driver );
        mem_free( current_driver );
        return( SUCCESS );
    }

    current_font = get_cop_font( tgt_path );
    if( current_font != NULL) {
        display_font( current_font );
        mem_free( current_font );
        return( SUCCESS );
    }

    return( FAILURE );
}

/* Function print_banner().
 * Print the banner to the screen
 */

void print_banner( void )
{
    puts( banner1w( "Script/GML Defined Name Parser Program", _RESEARCH_VERSION_ ) );
    puts( banner2( "1983" ) );
    puts( banner3 );
    puts( banner3a );
}

/* Function print_usage().
 * Print the usage information to the screen
 */

void print_usage( void )
{
    char const * *  list;

    list = usage_text;
    while( *list ) {
        puts( *list );
        ++list;
    }
}

/* Function main().
 * Given a valid defined name, verify that it is a valid .COP file and parse 
 * it if it is.
 *
 * The actual parsing is performed in the function parse_defined_name(); main()
 * is concerned with overall program architecture, not details.
 *
 * Returns:
 *      EXIT_FAILURE or EXIT_SUCCESS, as appropriate
 */

int main()
{
    /* Declare automatic variables */

    size_t  cmdlen          = 0;
    char *  cmdline         = NULL;
    int     retval;

    /* Display the banner */

    print_banner();

    /* Display the usage information if the command line is empty */

    cmdlen = _bgetcmd( NULL, 0 );
    if( cmdlen == 0 ) {
        print_usage();
        return( EXIT_FAILURE );
    }

    /* Get the command line */

    cmdlen++; /* Include space for the terminating null character */
    cmdline = malloc( cmdlen );
    if( cmdline == NULL ) {
        return( EXIT_FAILURE );
    }

    cmdlen = _bgetcmd( cmdline, cmdlen );

    /* Initialize the globals */

    initialize_globals();
    res_initialize_globals();
    get_env_vars();
    
    /* Parse the command line: allocates and sets tgt_path */

    retval = parse_cmdline( cmdline );
    if( retval == FAILURE ) {
        free( cmdline );
        return( EXIT_FAILURE );
    }

    /* Free the memory held by cmdline and reset it */

    free( cmdline );
    cmdline = NULL;

    /* Adjust tgt_path if necessary; see the Wiki */

    if( !strcmp( tgt_path, "''" ) ) tgt_path[0] = '\0';

    /* Parse the alleged .COP file */

    retval = parse_defined_name();

    /* Done */

    if( retval == FAILURE) {
      print_usage();
      return( EXIT_FAILURE );
    }

    return( EXIT_SUCCESS );
}

