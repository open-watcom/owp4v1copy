/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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


#include "gtype.h"


#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif


//================= Function Prototypes ========================

/* wgml.c                              */
extern  bool    free_resources( errno_t in_errno );
extern  void    g_banner( void );
// extern  char  * get_filename_full_path( char * buff, char const * name, size_t max );
extern  bool    get_line( void );
extern  void    inc_inc_level( void );

#pragma aux     my_exit aborts;
extern  void    my_exit( int );
extern  void    show_include_stack( void );

/* gargutil.c                           */
extern  void        garginit( void );
extern  void        garginitdot( void );
extern  condcode    getarg( void );
extern  condcode    getqst( void );
extern  bool        is_quote_char( char c );
extern  bool        is_function_char( char c );
extern  bool        is_id_char( char c );
extern  bool        is_macro_char( char c );
extern  bool        is_symbol_char( char c );
extern  void        unquote_if_quoted( char * * a, char * * z );


/* gdata.c                              */
extern  void    init_global_vars( void );


/* gerror.c                             */
extern  void    out_msg( char * fmt, ... );

/* -------------------------------- TBD
extern  void    g_err( int, ... );
extern  void    g_warn( int, ... );
extern  void    g_info( int, ... );
----------------------------------*/

extern  void    g_suicide( void );

/* getnum.c                             */
extern condcode     getnum( getnum_block * gn );

/* gmacdict.c                         */
extern  void        add_macro_entry( mac_entry * * dict, mac_entry * me );
extern  void        init_macro_dict( mac_entry * * dict );
extern  void        free_macro_dict( mac_entry * * dict );
extern  void        free_macro_entry( mac_entry * * dict, mac_entry * me );
extern  void        print_macro_dict( mac_entry * dict, bool with_mac_lines );
extern  mac_entry * find_macro( mac_entry * dict, char const * name );


/* gmemory.c                          */
extern  void        mem_free( void * p );
extern  void    *   mem_alloc( size_t size );
extern  void    *   mem_realloc( void * p, size_t size );

extern  void        g_trmem_init( void );
extern  void        g_trmem_prt_list( void );
extern  void        g_trmem_close( void );


/* goptions.c                         */
extern  void    proc_options( char * cmdline );
extern  void    split_attr_file( char * filename, char * attr, size_t attrlen );

/* gprocess.c                         */
extern  void    process_line( void );
extern  void    split_input( char * buf, char * split_pos );
extern  void    split_input_LIFO( char * buf, char * split_pos );

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


/* gsfuncs.c                          */
extern  char    *   scr_multi_funcs( char * in, char * end, char * * ppval );


/* gsfunelu.c                         */
extern  char    *   scr_single_funcs( char * in, char * end, char * * result );
extern  char    *   scr_single_func_e( char * in, char * end, char * * result );
extern  char    *   scr_single_func_l( char * in, char * end, char * * result );
extern  char    *   scr_single_func_u( char * in, char * end, char * * result );


/* gsgoto.c                           */
extern  void    print_labels( labelcb * lb );
extern  bool    gotarget_reached( void );


/* gsgt.c                             */
extern  void    init_tag_att( void );


/* gsetvar.c                          */
extern char *   scan_sym( char * p, symvar * sym, sub_index * subscript );


/* gsmacro.c                          */
extern  void        add_macro_cb_entry( mac_entry * me, gtentry * ge );
extern  void        add_macro_parms( char * p );
extern  void        free_lines( inp_line * line );


/* gspu.c                             */
extern  void    close_pu_file( int numb );
extern  void    close_all_pu_files( void );


/* gsymvar.c                          */
extern void     init_dict( symvar * * dict );
extern void     free_dict( symvar * * dict );
extern int      find_symvar( symvar * * dict, char * name, sub_index subscript, symsub * * symsubval );
extern int      add_symvar( symvar * * dict, char * name, char * val, sub_index subscript, sym_flags f );
extern void     print_sym_dict( symvar * dict );
extern void     reset_auto_inc_dict( symvar * dict );

/* gtagdict.c                         */
extern  gtentry *   add_tag( gtentry * * dict, char const * name, char const * macro, const int flags );
extern  gtentry *   change_tag( gtentry * * dict, char const * name, char const * macro );
extern  void        init_tag_dict( gtentry * * dict );
extern  void        free_tag_dict( gtentry * * dict );
extern  gtentry *   free_tag( gtentry * * dict, gtentry * ge );
extern  void        print_tag_dict( gtentry * dict );
extern  void        print_tag_entry( gtentry * entry );
extern  gtentry *   find_tag( gtentry * * dict, char const * name );

/* gusertag.c                         */
extern  bool        process_tag( gtentry * ge, mac_entry * me );

/* gutils.c                           */
extern  bool    to_internal_SU( char * * scaninput, su * spaceunit );

/*
 * prototypes for the gml processing routines
 */

#define pick( name, length, routine, flags )  extern void routine( const gmltag * entry );

#include "gtags.h"

/*
 * prototypes for the script control word processing routines
 */

#define pick( name, routine, flags )  extern void routine( void );

#include "gscrcws.h"

/*
 * prototypes for the script string function routines , ie. &'substr( ,..
 */

#define pick( name, length, mand_parms, opt_parms, routine ) \
    extern condcode routine( parm parms[ MAX_FUN_PARMS ], size_t parm_count, char * * ppval );

#include "gsfuncs.h"


#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* WGML_H_INCLUDED */
