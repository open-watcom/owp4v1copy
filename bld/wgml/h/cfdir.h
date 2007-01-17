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
* Description:  Declares functions used to manipulate .COP directory files:
*                   get_entry_count()
*                   get_entry_type()
*                   get_compact_entry()
*                   get_extended_entry()
*                   skip_compact_entry()
*                   skip_extended_entry()
*
****************************************************************************/

#ifndef CFDIR_H_INCLUDED
#define CFDIR_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include "dowlinux.h"

#define ITEM_MAX 78     /* Per documentation, max length of item name */

/* Structure declarations */

typedef struct compact_entry_struct {
    char    item_name[ITEM_MAX+1];
    char    file_name[NAME_MAX+1];
} compact_entry_t;

typedef struct extended_entry_struct {
    char        item_name[ITEM_MAX+1];
    uint16_t    marker;
    char        file_name[NAME_MAX+1];
    uint16_t    preview;
} extended_entry_t;

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

int get_entry_count( FILE *, uint16_t *);
int get_entry_type( FILE *, uint16_t * );
int get_compact_entry( FILE *, compact_entry_t * );
int get_extended_entry( FILE *, extended_entry_t * );
int skip_compact_entry( FILE * );
int skip_extended_entry( FILE * );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* CFDIR_H_INCLUDED */
