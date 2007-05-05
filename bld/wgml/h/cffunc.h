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
*                   functions_block
*                       code_block
*                   get_functions()
*
****************************************************************************/

#ifndef CFDEV_H_INCLUDED
#define CFDEV_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

/* Structure declarations */

// This section is under development!!!

/* These structs are based on the discussion in the Wiki, which should be
 * consulted for further information on how the data is structured.
 */

/* function points to an array of block_length bytes */

typedef struct code_block_struct
{
    uint16_t    block_length;
    uint8_t *   function;
} code_block;

/* code_blocks points to an array of count code_block structs */

typedef struct functions_block_struct
{
    uint8_t         count;
    code_block *    code_blocks;
} functions_block;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

functions_block * get_functions( FILE *);

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* CFDEV_H_INCLUDED */
