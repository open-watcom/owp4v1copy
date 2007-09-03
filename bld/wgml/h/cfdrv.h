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
*               driver files:
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
*                   is_drv_file()
*                   parse_driver()
*
****************************************************************************/

#ifndef CFDRV_H_INCLUDED
#define CFDRV_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

/* Structure declarations */

/* These structs are based on the discussion in the Wiki, which should be
 * consulted for further information on how the data is structured.
 */

/* In all cases: a count of "0" or a NULL pointer indicates that the
 * corresponding item did not exist in the .COP file
 */

/* The :INIT block is unique in allowing multiple intermixed :VALUE and
 * :FONTVALUE blocks. Testing shows wgml invoking each :FONTVALUE block
 * four times but each :VALUE block only once, so is_fontvalue is provided
 * to allow wgml to determine if a init_text instance came from a :FONTVALUE
 * block or not.
 */

typedef struct init_text_struct
{
    bool                is_fontvalue;
    uint16_t            count;
    uint8_t *           text;
} init_text;

typedef struct init_block_struct
{
    uint16_t            count;
    init_text *         codetext;
} init_block;

/* There can be at most two :INIT blocks. */

typedef struct init_funcs_struct
{
    init_block *        start_initblock;
    init_block *        document_initblock;
} init_funcs;

/* This struct is used for all blocks where at most one CodeBlock is allowed */

typedef struct code_text_struct
{
    uint16_t            count;
    uint8_t *           text;
} code_text;

/* There can be at most two :FINISH blocks. code_text is used because, although
 * gendev will accept multiple :VALUE blocks in a :FINISH block and even encode
 * them in the .COP file, wgml will only use the first, so only that one needs
 * to be presented here.
 */

typedef struct finish_funcs_struct
{
    code_text *         end_finishblock;
    code_text *         document_finishblock;
} finish_funcs;

/* There will be as many newline_blocks as there are distinct values of
 * "advance". "count" contains the number of bytes in "text".
 */

typedef struct newline_block_struct
{
    uint16_t            advance;
    uint16_t            count;
    uint8_t *           text;
} newline_block;

/* "count" contains the number of newline_block instances */

typedef struct newline_funcs_struct
{
    uint16_t            count;
    newline_block *     newlineblock;
} newline_funcs;

/* There will be as many fontswitch_blocks as there are distinct values of
 * "type"
 */

typedef struct fontswitch_block_struct
{
    char *              type;
    code_text *         startvalue;
    code_text *         endvalue;
} fontswitch_block;

/* "count" contains the number of fontswitch_block instances */

typedef struct fontswitch_funcs_struct
{
    uint16_t            count;
    fontswitch_block *  fontswitchblock;
} fontswitch_funcs;

typedef struct line_proc_struct
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

typedef struct font_style_struct
{
    uint16_t            passes;
    char *              type;
    code_text *         startvalue;
    code_text *         endvalue;
    line_proc *         lineprocs;
} font_style;

/* "count" contains the number of font_style instances */

typedef struct fontstyle_block_struct
{
    uint16_t            count;
    font_style *        fontstyle;
} fontstyle_block;

/* "thickness" is the value of attribute "thickness"; "count" is the number
 * of bytes pointed to by "text"
 */

typedef struct line_block_struct
{
    uint32_t            thickness;
    uint16_t            count;
    uint8_t *           text;
} line_block;

/* The order of fields follows the order in the .COP file, except for "unknown",
 * which is omitted as it never has any data in it.
 */

typedef struct cop_driver_struct
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
    init_funcs          init;
    /* FinishFuncs */
    finish_funcs        finish;
    /* NewlineFuncs */
    newline_funcs       newline;
    /* Variant A FunctionsBlocks */
    code_text           newpage;
    code_text           htab;
    /* FontswitchFuncs */
    fontswitch_funcs    fontswitch;
    /* FontstyleBlock */
    fontstyle_block     fontstyle;
    /* Variant A FunctionsBlock */
    code_text           absoluteaddress;
    /* HlineBlock */
    line_block         hline;
    /* VlineBlock */
    line_block         vline;
    /* DboxBlock */
    line_block          dbox;
} cop_driver;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

bool is_drv_file( FILE * );
cop_driver * parse_driver( FILE *);

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* CFDRV_H_INCLUDED */
