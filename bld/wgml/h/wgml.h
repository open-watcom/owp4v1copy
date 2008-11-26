/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML global include header.
*
****************************************************************************/

#ifndef WGML_H_INCLUDED
#define WGML_H_INCLUDED

#ifndef __STDC_WANT_LIB_EXT1__
    #define __STDC_WANT_LIB_EXT1__  1  /* use safer C library              */
#endif

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <process.h>


//================= Some global defines ========================
#define MAX_NESTING     32              // max nesting of option files
#define MAX_PASSES      10              // max no of passes
#define MAX_INC_DEPTH   255             // max include level depth
#define BUF_SIZE        512             // buffersize for filecb e.a.
#define MAX_FILE_ATTR   16              // max size for fileattr (T:xxxx)
#define TAG_NAME_LENGTH 12              // :tag name length


#include "gtype.h"


#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif


//================= Function Prototypes ========================

/* wgml.c                              */
extern  void    g_banner( void );
extern  char    *get_filename_full_path( char *buff, char const *name, \
                                          size_t max );

#pragma aux     my_exit aborts;
extern  void    my_exit( int );

extern  int     search_file_in_dirs( char *filename, char *defext, \
                                      char *altext, DIRSEQ seq );
extern  int     try_open( char *prefix, char *separator, char *filename, \
                           char *suffix );



/* garginit.c                           */
extern  void    garg_init( void );


/* gdata.c                              */
extern  void    init_global_vars( void );
extern  void    get_env_vars( void );
extern  char    *GML_get_env( char *name );


/* gerror.c                             */
extern  void    out_msg( char *fmt, ... );

/* -------------------------------- TBD
extern  void    g_err( int, ... );
extern  void    g_warn( int, ... );
extern  void    g_info( int, ... );
----------------------------------*/

extern  void    g_suicide( void );


/* gmemory.c                            */
extern  void    mem_free( void *p );
extern  void    *mem_alloc( size_t size );
extern  void    *mem_realloc( void *p, size_t size );

extern  void    g_trmem_init( void );
extern  void    g_trmem_prt_list( void );
extern  void    g_trmem_close( void );


/* goptions.c                           */
extern  void    proc_options( char *cmdline );
extern  void    split_attr_file( char *filename, char *attr, size_t attrlen );


/* gresrch.c                          */
extern  void    add_GML_tag_research( char * tag );
extern  void    free_GML_tags_research( void );
extern  void    print_GML_tags_research( void );
extern  void    add_SCR_tag_research( char * tag );
extern  void    free_SCR_tags_research( void );
extern  void    print_SCR_tags_research( void );
extern  void    printf_research( char * msg, ... );


/* gscan.c                            */
extern  void    scan_line( void );


/* gsetvar.c                          */
extern condcode getnum( getnum_block *gn );
extern void     sc_se( void );


/* gutils.c                           */
extern  bool    to_internal_SU( char **scaninput, su *spaceunit );


#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* WGML_H_INCLUDED */
