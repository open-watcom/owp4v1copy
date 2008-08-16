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
*               FunctionsBlocks:
*                   p_buffer
*                   functions_block
*                       code_block
*                   get_code_blocks()
*                   get_p_buffer()
*                   parse_functions_block()
*
****************************************************************************/

#ifndef CFFUNC_H_INCLUDED
#define CFFUNC_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

/*  Structure declarations */

/*  These structs are based on the discussion in the Wiki, which should be
 *  consulted for further information on how the data is structured.
 */

/*  This holds the raw contents of one or more contiguous P-buffers. The
 *  buffer is to be interpreted as an array of count uint8_t length. The
 *  value of count should always be a multiple of 80.
 */

typedef struct p_buffer_struct
{
    uint16_t    count;
    uint8_t *   buffer;
} p_buffer;

/*  This is the CodeBlock discussed in the Wiki */

typedef struct code_block_struct
{
    uint8_t     designator;
    uint8_t     cb05_flag;
    uint8_t     lp_flag;
    uint16_t    pass;
    uint16_t    count;
    uint16_t    cumulative_index;
    uint8_t *   function;
} code_block;

/* This is the Variant A FunctionsBlock discussed in the Wiki */

typedef struct functions_block_struct
{
    uint16_t        count;
    code_block *    code_blocks;
} functions_block;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

code_block *        get_code_blocks( uint8_t * *, uint16_t, uint8_t *, char * );
p_buffer *          get_p_buffer( FILE * );
functions_block *   parse_functions_block( uint8_t * *, uint8_t *, char * );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* CFFUNC_H_INCLUDED */
