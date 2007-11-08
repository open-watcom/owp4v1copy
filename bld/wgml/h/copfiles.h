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
* Description:  Declares structs and functions used by wgml as such to
*               parse and interpret the information from .COP files:
*                   cop_device
*                       box_block
*                       underscore_block
*                       intrans_block
*                       outtrans_block
*                           translation
*                       defaultfont_block
*                           default_font
*                       pause_block
*                           code_text
*                       devicefont_block
*                           device_font
*                               code_text
*                   cop_driver
*                       init_funcs
*                           init_block
*                               init_text
*                       finish_funcs
*                           code_text
*                       newline_funcs
*                           newline_block
*                       fontswitch_funcs
*                           fontswitch_block
*                               code_text
*                       fontstyle_block
*                           font_style
*                               code_text
*                               line_proc
*                                   code_text
*                       line_block
*                   cop_font
*                       intrans_block
*                       outtrans_block
*                           translation
*                       width_block
*                   get_cop_device()
*                   get_cop_driver()
*                   get_cop_font()
*
****************************************************************************/

#ifndef COPFILE_H_INCLUDED
#define COPFILE_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#include "wgml.h"

/* Structure declarations */

/* These structs are based on the discussion in the Wiki, which should be
 * consulted for further information on how the data is structured.
 */

/* These structs are used by more than one of the top-level structs:
 *      intrans_block is used by both cop_device and cop_font
 *      outtrans_block is used by both cop_device and cop_font
 *      code_text is used by both cop_device and cop_driver
 */
 
/* intrans_block is a struct for consistency with outtrans_block. */

typedef struct
{
    uint8_t         table[0x100];
} intrans_block;

/* Field "data" points to a buffer containing "count" bytes.
 * This is not a string: $00 is a valid embedded value.
 */

typedef struct
{
    uint8_t         count;
    uint8_t *       data;
} translation;

/* Each entry in table will be NULL if no out-translation is needed. */

typedef struct
{
    translation *   table[0x100];
} outtrans_block;

/* This struct represents a single CodeBlock. */

typedef struct
{
    uint16_t            count;
    uint8_t *           text;
} code_text;

/* These structs are unique to the top-level struct cop_device. */

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

typedef struct
{
    char *              font_name;
    uint8_t             font_number;
    char                horizontal_line;
    char                vertical_line;
    char                top_left;
    char                top_right;
    char                bottom_left;
    char                bottom_right;
    char                top_join;
    char                bottom_join;
    char                left_join;
    char                right_join;
    char                inside_join;
} box_block;

typedef struct
{
    char *              font_name;
    uint8_t             font_number;
    char                underscore_char;
} underscore_block;

typedef struct
{
    char *              font_name;
    char *              font_style;
    uint16_t            font_height;
    uint16_t            font_space;    
} default_font;

/* Field "font" points to an array of "count" default_font instances. */

typedef struct
{
    uint16_t            count;
    default_font *      font;
} defaultfont_block;

typedef struct
{
    code_text *         start_pause;
    code_text *         document_pause;
    code_text *         document_page_pause;
    code_text *         device_page_pause;
} pause_block;

typedef struct
{
    char *              font_name;
    char *              font_switch;
    uint8_t             resident;
    code_text *         font_pause;
} device_font;

/* Field "font" points to an array of "count" device_font instances. */

typedef struct
{
    uint16_t            count;
    device_font *       font;
} devicefont_block;

/* These structs are unique to the top-level struct cop_driver. */

/* The :INIT block is unique in allowing multiple intermixed :VALUE and
 * :FONTVALUE blocks. Testing shows wgml invoking each :FONTVALUE block
 * multiple times, as discussed in the Wiki. The field is_fontvalue is
 * provided to allow wgml to determine if a init_text instance came from
 * a :FONTVALUE block or not.
 */

typedef struct
{
    bool                is_fontvalue;
    uint16_t            count;
    uint8_t *           text;
} init_text;

typedef struct
{
    uint16_t            count;
    init_text *         codetext;
} init_block;

/* There can be at most two :INIT blocks. */

typedef struct
{
    init_block *        start;
    init_block *        document;
} init_funcs;

/* There can be at most two :FINISH blocks. A single code_text is used
 * for each because, although gendev will accept multiple :VALUE blocks
 * in a :FINISH block and even encode them in the .COP file, wgml will
 * only use the first, so only that one needs to be present.
 */

typedef struct
{
    code_text *         end;
    code_text *         document;
} finish_funcs;

/* There will be as many newline_blocks as there are distinct values of
 * "advance". "count" contains the number of bytes in "text".
 */

typedef struct
{
    uint16_t            advance;
    uint16_t            count;
    uint8_t *           text;
} newline_block;

/* "count" contains the number of newline_block instances */

typedef struct
{
    uint16_t            count;
    newline_block *     newlineblocks;
} newline_funcs;

/* There will be as many fontswitch_blocks as there are distinct values of
 * "type"
 */

typedef struct
{
    char *              type;
    code_text *         startvalue;
    code_text *         endvalue;
} fontswitch_block;

/* "count" contains the number of fontswitch_block instances */

typedef struct
{
    uint16_t            count;
    fontswitch_block *  fontswitchblocks;
} fontswitch_funcs;

/* the order shown is the order enforced on the source file by gendev */

typedef struct
{
    code_text *         startvalue;
    code_text *         firstword;
    code_text *         startword;
    code_text *         endword;
    code_text *         endvalue;
} line_proc;

/* "passes" contains the number of line_proc instances; "type" is the style
 * name, a null-terminated string
 */

typedef struct
{
    uint16_t            passes;
    char *              type;
    code_text *         startvalue;
    code_text *         endvalue;
    line_proc *         lineprocs;
} font_style;

/* "count" contains the number of font_style instances */

typedef struct
{
    uint16_t            count;
    font_style *        fontstyle_list;
} fontstyle_block;

/* "thickness" is the value of attribute "thickness"; "count" is the number
 * of bytes pointed to by "text"
 */

typedef struct
{
    uint32_t            thickness;
    uint16_t            count;
    uint8_t *           text;
} line_block;


/* This struct is unique to the top-level struct cop_font. */

/* width_block is a struct for consistency with outtrans_block. */

typedef struct
{
    uint32_t            table[0x100];
} width_block;

/* These are the top-level structs. These are the only structs intended to
 * be created and passed around independently.
 *
 * The comments within the structs refer to the "blocks" discussed in the Wiki. 
 *
 * The first two fields are used internally and were used for sizing during
 * development
 *
 * The instance returned will be allocated as a single block and so can be
 * freed in one statement.
 */

/* This struct contains the data in a .COP file encoding a :DEVICE block.
 *
 * Note that the "FunctionsBlock" is not mentioned. The various "CodeBlock"s
 * are instead provided as part of PauseBlock and DevicefontBlock.
 */

typedef struct
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
    intrans_block  *    intrans;
    outtrans_block *    outtrans;
    /* DefaultfontBlock */
    defaultfont_block   defaultfonts;
    /* PauseBlock */
    pause_block         pauses;
    /* DevicefontBlock */
    devicefont_block    devicefonts;
} cop_device;

/* This struct contains the data in a .COP file encoding a :DRIVER block.
 *
 * Note that the "unknown" block is not mentioned. This is because it
 * never has any data in it.
 */

typedef struct
{
    size_t              allocated_size;
    size_t              next_offset;
    /* The Attributes */
    char *              rec_spec;
    char                fill_char;
    /* PageAddressBlock */
    uint8_t             x_positive;
    uint8_t             y_positive;
    /* InitFuncs */
    init_funcs          inits;
    /* FinishFuncs */
    finish_funcs        finishes;
    /* NewlineFuncs */
    newline_funcs       newlines;
    /* Variant A FunctionsBlocks */
    code_text           newpage;
    code_text           htab;
    /* FontswitchFuncs */
    fontswitch_funcs    fontswitches;
    /* FontstyleBlock */
    fontstyle_block     fontstyles;
    /* Variant A FunctionsBlock */
    code_text           absoluteaddress;
    /* HlineBlock */
    line_block          hline;
    /* VlineBlock */
    line_block          vline;
    /* DboxBlock */
    line_block          dbox;
} cop_driver;

/* This struct contains the data in a .COP file encoding a :FONT block. */

typedef struct
{
    size_t              allocated_size;
    size_t              next_offset;
    /* The Attributes */
    char *              font_out_name1;
    char *              font_out_name2;
    uint32_t            line_height;
    uint32_t            line_space;
    uint32_t            scale_basis;
    uint32_t            scale_min;
    uint32_t            scale_max;
    uint32_t            char_width;
    /* CharacterDescriptionBlock */
    intrans_block *     intrans;
    outtrans_block *    outtrans;
    width_block *       width;
} cop_font;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

cop_device * get_cop_device( char const * defined_name );
cop_driver * get_cop_driver( char const * defined_name );
cop_font   * get_cop_font( char const * defined_name );

#ifdef  __cplusplus
}               /* End of "C" linkage for C++ */
#endif

#endif          /* COPFILE_H_INCLUDED */
