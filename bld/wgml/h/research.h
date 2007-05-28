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
* Description:  Declares items specific to the research programs:
*                   display_char()
*                   display_hex_block()
*                   display_hex_line()
*                   res_initialize_globals()
*
****************************************************************************/

#ifndef RESEARCH_H_INCLUDED
#define RESEARCH_H_INCLUDED

#include <stdint.h>

/* In theory, this should go into banner.h */
/* For gendev and wgml, the equivalent macros will */

#define _RESEARCH_VERSION_ BAN_VER_STR

/* Global variable declarations */

/*
 * This allows the same declarations to function as definitions.
 * Just #define global before including this file.
 */

#ifndef global
    #define global  extern
#endif

global  char *  tgt_path;   /* path of directory to be checked */

#undef global   /* reset so can be reused with other headers */

/* Function declarations */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

void    display_char( char *, char );
void    display_hex_block( uint8_t *, uint16_t );
void    display_hex_line( char *, char * );
void    res_initialize_globals( void );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* RESEARCH_H_INCLUDED */
