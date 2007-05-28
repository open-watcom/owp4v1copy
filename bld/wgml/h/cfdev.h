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
* Description:  Declares structs and functions used to manipulate .COP
*               device files:
*                   cop_device
*                       box_block
*                       underscore_block
*                       intrans_block
*                       outtrans_block
*                           translation
*                       defaultfont_block
*                           default_font
*                       pause_block
*                       devicefont_block
*                           device_font
*                   is_dev_file()
*                   parse_device()
*
****************************************************************************/

#ifndef CFDEV_H_INCLUDED
#define CFDEV_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

/* Structure declarations */

/* These structs are based on the discussion in the Wiki, which should be
 * consulted for further information on how the data is structured.
 */

/* Fonts In box_block and underscore_block.
 *
 * These structs have two font fields: font_name and font_number.
 *
 * If font_name is NULL, then font_name is to be ignored and font_number
 * is to be used.
 *
 * If font_name is not NULL, then font_name is to be used and font_number is
 * to be ignored.
 *
 * If the font_name is not NULL but the font indicated cannot be found, then
 * font number "0" should be used. There is always a font numbered "0".
 */

typedef struct box_block_struct
{
    char *          font_name;
    uint8_t         font_number;
    char            horizontal_line;
    char            vertical_line;
    char            top_left;
    char            top_right;
    char            bottom_left;
    char            bottom_right;
    char            top_join;
    char            bottom_join;
    char            left_join;
    char            right_join;
    char            inside_join;
} box_block;

typedef struct underscore_block_struct
{
    char *          font_name;
    uint8_t         font_number;
    char            underscore_char;
} underscore_block;

/* intrans_block is a struct for consistency with outtrans_block. */

typedef struct intrans_block_struct
{
    uint8_t         table[0x100];
} intrans_block;

/* Field "data" points to a buffer containing "count" bytes.
 * This is not a string: $00 is a valid embedded value.
 */

typedef struct translation_struct
{
    uint8_t         count;
    uint8_t *       data;
} translation;

/* Each entry in table will be NULL if no out-translation is needed */

typedef struct outtrans_block_struct
{
    translation *   table[0x100];
} outtrans_block;

typedef struct default_font_struct
{
    char *          font_name;
    char *          font_style;
    uint16_t        font_height;
    uint16_t        font_space;    
} default_font;

/* Field "font" points to an array of "count" default_font instances. */

typedef struct defaultfont_block_struct
{
    uint16_t        count;
    default_font *  font;
} defaultfont_block;

/* For each type of :PAUSE, the pointer points to an array of bytes whose
 * length is given in the corresponding "_count" field.
 */

typedef struct pause_block_struct
{
    uint16_t        startpause_count;
    uint8_t *       startpause;
    uint16_t        documentpause_count;
    uint8_t *       documentpause;
    uint16_t        docpagepause_count;
    uint8_t *       docpagepause;
    uint16_t        devpagepause_count;
    uint8_t *       devpagepause;
} pause_block;

/* Field "fontpause" points to an array of "fontpause_count" bytes */

typedef struct device_font_struct
{
    char *          font_name;
    char *          font_switch;
    uint8_t         resident;
    uint16_t        fontpause_count;
    uint8_t *       fontpause;
} device_font;

/* Field "font" points to an array of "count" device_font instances. */

typedef struct devicefont_block_struct
{
    uint16_t            count;
    device_font *       font;
} devicefont_block;

/*  The comments refer to the "blocks" discussed in the Wiki. 
 *
 *  Note that the "FunctionsBlock" is not mentioned. The various "CodeBlock"s
 *  are instead provided as part of PauseBlock and DevicefontBlock.
 *
 *  The first two fields are used internally and were used for sizing during
 *  development
 *
 *  The instance returned will be allocated as a single block and so can be
 *  freed in one statement.
 */

typedef struct cop_device_struct
{
    size_t              allocated_size;
    size_t              next_offset;
    /* The Attributes */
    char *              driver_name;
    char *              output_name;
    char *              output_extension;
    uint32_t            page_width;
    uint32_t            page_depth;
    uint32_t            horizontal_base_units;
    uint32_t            vertical_base_units;
    /* PagegeometryBlock */
    uint32_t            x_start;
    uint32_t            y_start;
    uint32_t            x_offset;
    uint32_t            y_offset;
    /* BoxBlock */
    box_block           box;
    /* UnderscoreBlock */
    underscore_block    underscore;
    /* TranslationBlock */
    intrans_block *     intrans;
    outtrans_block  *   outtrans;
    /* DefaultfontBlock */
    defaultfont_block   defaultfonts;
    /* PauseBlock */
    pause_block         pauses;
    /* DevicefontBlock */
    devicefont_block    devicefonts;
} cop_device;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

bool is_dev_file( FILE * );
cop_device * parse_device( FILE *);

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* CFDEV_H_INCLUDED */
