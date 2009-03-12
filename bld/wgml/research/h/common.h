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
* Description:  Declares items common to all WGML files.
*
*               These functions must be implemented by each program
*               (or group of programs) that wishes to use them:
*                   parse_cmdline()
*                   print_banner()
*                   print_usage()
*
*               The remaining functions are implemented in common.c. These
*               are part of the research framework:
*                   initialize_globals()
*                   skip_spaces()
*
*               These are needed to reproduce enough of the wgml context for
*               research programs that use parts of wgml to work:               
*                   add_symvar()
*                   err_cnt
*                   find_symvar()
*                   free_resources()
*                   g_suicide()
*                   global_dict
*                   master_fname
*                   mem_alloc()
*                   mem_free()
*                   mem_realloc()
*                   out_file
*                   out_file_attr
*                   out_msg()
*                   wng_cnt
*
****************************************************************************/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "heapchk.h"    // Ensure is always available.

/* Function return values. */

#define FAILURE 0
#define SUCCESS 1

/* This macro is part of the wgml context. */

#define SYM_NAME_LENGTH 10

/* These enums are part of the wgml context. */

typedef enum {
    min_subscript = -1000000L,
    max_subscript =  1000000L,
    no_subscript  = 0x11223344
} sub_index;

typedef enum {
    local_var   = 1,
    subscripted = 2,
    auto_inc    = 4,
    deleted     = 0x100
} sym_flags;

/* These structs are part of the wgml context. */

typedef struct opt_font {
    struct opt_font *   nxt;
    uint8_t             font;
    char *              name;
    char *              style;
    uint32_t            space;
    uint32_t            height;
} opt_font;

typedef struct symsub {
    char            *   value;
} symsub;

typedef struct symvar {
    struct symvar   *   next;
    char                name[SYM_NAME_LENGTH + 1];
    symsub          *   sub_0;
} symvar;

/* Global variable declarations. */

/* This allows the same declarations to function as definitions.
 * Just #define global before including this file.
 */

#ifndef global
    #define global  extern
#endif

global char         switch_char;    // Either '\', '/', or whatever DOS is using.
global char     *   dev_name;       // Part of the wgml context.
global char     *   master_fname;   // Part of the wgml context.
global char     *   out_file;       // Part of the wgml context.
global char     *   out_file_attr;  // Part of the wgml context.
global int          err_count;      // Part of the wgml context.
global int          wng_count;      // Part of the wgml context.
global opt_font *   opt_fonts;      // Part of the wgml context.
global symvar   *   global_dict;    // Part of the wgml context.

/* Reset so can be reused with other headers. */

#undef global

/* Function declarations. */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode. */
#endif

/* These functions must be defined by each program using them. */

extern  int         parse_cmdline( char * );
extern  void        print_banner( void );
extern  void        print_usage( void );

/* These functions are defined in common.c. */

/* These functions are part of the research framework. */

extern  void        initialize_globals( void );
extern  char    *   skip_spaces( char * start );

/* These are part of the wgml context. */

extern int          add_symvar( symvar * * dict, char * name, char * val, sub_index subscript, sym_flags f );
extern int          find_symvar( symvar * * dict, char * name, sub_index subscript, symsub * * symsubval );
extern  bool        free_resources( errno_t in_errno ); 
extern  void        g_suicide( void );
extern  void        mem_free( void *p );
extern  void    *   mem_alloc( size_t size );
extern  void    *   mem_realloc( void *p, size_t size );
extern  void        out_msg( char *fmt, ... );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++. */
#endif

#endif  /* COMMON_H_INCLUDED */

