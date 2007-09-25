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
* Description:  Declares the structs for the intrans and outtrans blocks
*                   intrans_block
*                   outtrans_block
*                       translation
*
****************************************************************************/

#ifndef CFTRANS_H_INCLUDED
#define CFTRANS_H_INCLUDED

//#include <stdbool.h>
#include <stdint.h>

/* Structure declarations */

/* These structs are based on the discussion in the Wiki, which should be
 * consulted for further information on how the data is structured.
 */

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

#endif  /* CFTRANS_H_INCLUDED */
