/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
    #define __STDC_WANT_LIB_EXT1__  1   /* use safer C library             */
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
#include "gtypelay.h"
#include "copfiles.h"                   // for text_line and text_vars


#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif


//================= Function Prototypes ========================

/* wgmlsupp.c                          */
extern  bool    free_resources( errno_t in_errno );
extern  void    free_some_mem( void );
extern  void    free_layout_banner( void );
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
extern  bool        is_lay_att_char( char c );
extern  bool        is_id_char( char c );
extern  bool        is_macro_char( char c );
extern  bool        is_stop_char( char c );
extern  bool        is_symbol_char( char c );
extern  void        unquote_if_quoted( char * * a, char * * z );


/* gbanner.c                            */
extern  uint32_t    ban_top_pos( banner_lay_tag * ban );
extern  uint32_t    ban_bot_pos( banner_lay_tag * ban );
extern  void        out_ban_bot( banner_lay_tag * ban );
extern  void        out_ban_top( banner_lay_tag * ban );


/* gdata.c                              */
extern  void    init_global_vars( void );
extern  void    init_pass_data( void );


/* gdeflay.c                            */
extern  void    init_def_lay( void );


/* gdocsect.c                           */
extern  void    set_section_banners( doc_section ds );
extern  void    finish_page( void );
extern  void    prepare_doc_sect( doc_section ds );


/* gerror.c                             */
extern  void    out_msg( const char * fmt, ... );
extern  void    g_err( const msg_ids err, ... );
extern  void    g_warn( const msg_ids err, ... );
extern  void    g_info( const msg_ids err, ... );
extern  void    g_suicide( void );


/* gerrorxx.c                           */
extern  void    att_val_err( char * attname );
extern  void    auto_att_err( void );
extern  void    cw_err( void );
extern  void    dc_opt_err( char * pa );
extern  void    file_mac_info( void );
extern  void    nottag_err( void );
extern  void    numb_err( void );
extern  void    parm_extra_err( char *cw, char *pa );
extern  void    parm_miss_err( char *cw );
extern  void    tag_name_missing_err( void );
extern  void    tag_text_err( char * tagname );
extern  void    tag_text_req_err( char * tagname );
extern  void    xx_err( const msg_ids errid );
extern  void    xx_opt_err( char *cw, char *pa );
extern  void    g_err_tag( char *tagname );
extern  void    g_err_tag_prec( char *tagname );


/* getnum.c                             */
extern condcode     getnum( getnum_block * gn );


/* glayutil.c                           */
extern void         eat_lay_sub_tag( void );
extern condcode     get_lay_sub_and_value( struct att_args * l_args );


/* glbanner.c                           */
extern void         lay_banner_end_prepare( void );


/* glbandef.c                           */
extern void         banner_defaults( void );


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
extern  unsigned long   g_trmem_peak_usage( void );


/* goptions.c                         */
extern  int     proc_options( char * cmdline );
extern  void    split_attr_file( char * filename, char * attr, size_t attrlen );


/* gpagegeo.c                           */
extern  void    do_layout_end_processing( void );
extern  void    init_def_margins( void );
extern  void    init_page_geometry( void );
extern  void    set_page_position( doc_section ds );


/* gprocess.c                         */
extern  void    process_late_subst( void );
extern  void    process_line( void );
extern  void    split_input( char * buf, char * split_pos );
extern  void    split_input_LIFO( char * buf, char * split_pos );


/* gproctxt.c              TBD        */
extern  void    add_text_chars_to_pool( text_line * a_line );
extern  text_chars * alloc_text_chars( char * p, size_t cnt, uint8_t font_num );
extern  void    do_justify( uint32_t left_m, uint32_t right_m, text_line * line );
extern  void    document_new_page( void );
extern  void    document_top_banner( void );
extern  void    intrans( char * data, uint16_t * len, uint8_t font );
extern  void    process_line_full( text_line * a_line, bool justify );
extern  void    process_text( char * text, uint8_t font_num );
extern  text_chars * process_word( char * text, size_t count, uint8_t font_num );
extern  void    set_h_start( void );
extern  void    shift_spaces( void );
extern  void    test_page_full( void );


/* gresrch.c                          */
extern  void    add_GML_tag_research( char * tag );
extern  void    free_GML_tags_research( void );
extern  void    print_GML_tags_research( void );
extern  void    add_SCR_tag_research( char * tag );
extern  void    free_SCR_tags_research( void );
extern  void    print_SCR_tags_research( void );
extern  void    printf_research( char * msg, ... );
extern  void    test_out_t_line( text_line  * a_line );


/* gsbr.c                             */
extern  void    scr_process_break( void );


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


/* gsix.c                             */
extern  void    free_index_dict( ix_h_blk ** dict );


/* gsetvar.c                          */
extern char *   scan_sym( char * p, symvar * sym, sub_index * subscript );


/* gsmacro.c                          */
extern  void    add_macro_cb_entry( mac_entry * me, gtentry * ge );
extern  void    add_macro_parms( char * p );
extern  void    free_lines( inp_line * line );


/* gspe.c                             */
extern  void    reset_pe_cb( void );


/* gspu.c                             */
extern  void    close_pu_file( int numb );
extern  void    close_all_pu_files( void );


/* gssk.c                             */
extern  void    calc_skip( void );
extern  int32_t calc_skip_value( void );


/* gsymvar.c                          */
extern void     init_dict( symvar * * dict );
extern void     free_dict( symvar * * dict );
extern int      find_symvar( symvar * * dict, char * name, sub_index subscript, symsub * * symsubval );
extern int      add_symvar( symvar * * dict, char * name, char * val, sub_index subscript, symbol_flags f );
extern void     print_sym_dict( symvar * dict );
extern void     reset_auto_inc_dict( symvar * dict );


/* gsyssym.c                          */
extern  void    init_sys_dict( symvar * * dict );
extern  void    init_sysparm( char * cmdline, char * banner );


/* gtagdict.c                         */
extern  gtentry *   add_tag( gtentry * * dict, char const * name, char const * macro, const int flags );
extern  gtentry *   change_tag( gtentry * * dict, char const * name, char const * macro );
extern  void        init_tag_dict( gtentry * * dict );
extern  void        free_tag_dict( gtentry * * dict );
extern  gtentry *   free_tag( gtentry * * dict, gtentry * ge );
extern  void        print_tag_dict( gtentry * dict );
extern  void        print_tag_entry( gtentry * entry );
extern  gtentry *   find_tag( gtentry * * dict, char const * name );


/* gtxtpool.c                         */
extern  void        add_text_chars_to_pool( text_line * a_line );
extern  text_chars * alloc_text_chars( char * p, size_t cnt, uint8_t font_num );
extern  void        add_text_line_to_pool( text_line * a_line );
extern  text_line * alloc_text_line( void );


/* gtitlepo.c                         */
extern  void        titlep_output( void );


/* gusertag.c                         */
extern  bool        process_tag( gtentry * ge, mac_entry * me );


/* gutils.c                           */
extern  int32_t     conv_hor_unit( su * spaceunit );
extern  int32_t     conv_vert_unit( su * spaceunit, uint8_t spacing );
extern  char    *   int_to_roman( uint32_t n, char * r, size_t rsize );
extern  bool        to_internal_SU( char * * scaninput, su * spaceunit );


/* gwidowut.c                         */
extern  void        add_line_to_buf_lines( text_line * * lines, text_line * a_line );
extern  void        out_buf_lines( text_line * * lines, bool newpage );
extern  bool        widow_check( void );


/* wgmlmsg.c                          */
extern  int     init_msgs( void );
extern  void    fini_msgs( void );
extern  int     get_msg( msg_ids resourceid, char *buffer, size_t buflen );
//extern  void Msg_Do_Put_Args( char rc_buff[], MSG_ARG_LIST *arg_info, char *types, ... );
//extern  void Msg_Put_Args( char message[], MSG_ARG_LIST *arg_info, char *types, va_list *args );


/*
 * prototypes for the gml tag processing routines
 */

#ifdef pick
    #undef pick
#endif
#define pick( name, length, routine, flags )  extern void routine( const gmltag * entry );

#include "gtags.h"

/*
 * prototypes for the layout tag processing routines
 */

#define pick( name, length, routine, flags )  extern void routine( const gmltag * entry );

#include "gtagslay.h"

/*
 * prototypes for the layout tag attribute processing routines
 */

/*          for input scanning              */
#define pick( name, funci, funco, restype ) \
    extern  bool    funci( char * buf, lay_att attr, restype * result );

#include "glayutil.h"

/*          for output via :convert tag     */
#define pick( name, funci, funco, restype ) \
    extern  void    funco( FILE * f, lay_att attr, restype * in );

#include "glayutil.h"

/*
 * prototypes for the script control word processing routines
 */

#define pick( name, routine, flags )  extern void routine( void );

#include "gscrcws.h"

/*
 * prototypes for the script string function routines , ie. &'substr( ,..
 */

#define pick( name, length, mand_parms, opt_parms, routine ) \
    extern condcode routine( parm parms[MAX_FUN_PARMS], size_t parm_count, char * * ppval );

#include "gsfuncs.h"


#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* WGML_H_INCLUDED */
