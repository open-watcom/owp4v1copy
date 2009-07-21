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
* Description:  wgml global variables.
*
****************************************************************************/
 
 
#ifndef GVARS_H_INCLUDED
#define GVARS_H_INCLUDED
 
#include <setjmp.h>
#include <time.h>
 
#ifndef global
    #define global  extern
#endif
 
#include "gtype.h"
#include "gtypelay.h"
 
global struct tm        doc_tm;         // document time/date
 
global  jmp_buf     *   environment;    // var for GSuicide()
 
global  char        *   scan_start;
global  char        *   scan_stop;
global  char        *   scan_char_ptr;  // used by character scanning routines
global  char        *   scan_restart;   // used by character scanning routines
global  bool            scan_err;       // used by character scanning routines
global char         *   tok_start;      // start of scanned token
global size_t           arg_flen;       // arg length
 
global  int             switch_char;    // DOS switch character
global  char        *   alt_ext;        // alternate extension
global  char        *   def_ext;        // default extension
 
global  char        *   master_fname;   // Primary input file name
global  char        *   master_fname_attr;// Primary input file name attributes
global  ulong           print_from;     // first page to print
global  ulong           print_to;       // last page to print
global  char        *   dev_name;       // device defined_name
global  opt_font    *   opt_fonts;      // option FONT parameters (linked list)
 
global  inputcb     *   input_cbs;      // GML input stack (files + macros)
global  char        *   out_file;       // output file name
global  char        *   out_file_attr;  // output file attributes (T:2222)
global  unsigned        inc_level;   // include nesting level 1 = MasterFname
global  unsigned        max_inc_level;  // maximum include level depth
global  ulong           line_from;      // starting lineno to process
global  ulong           line_to;        // ending lineno to process
#define LINEFROM_DEFAULT    1
#define LINETO_DEFAULT      (0x1000000) // 16 MiB lines should be enough
 
global  char            gotarget[MAC_NAME_LENGTH +1];   // .go to target name
global  int32_t         gotargetno;     // .go to line no
 
global  int             err_count;      // Overall Errorcount
global  int             wng_count;      // Overall warning count
 
global  char            GML_char;       // GML Keywword start char :
global  char            SCR_char;       // SCRIPT keywword start char .
global  char            CW_sep_char;    // Control Word separator char ;
 
global  int             CPI;            // chars per inch
global  space_units     CPI_units;      // unit for chars per inch
global  int             LPI;            // lines per inch
global  space_units     LPI_units;      // unit for lines per inch
 
global  su              bind_odd;       // Bind value for odd pages
 
global  su              bind_even;      // Bind value for even pages
 
global  int             passes;         // Max no of document passes
global  int             pass;           // current document pass no
 
global  int             apage;          // current absolute pageno 1 - n
global  int             page;           // current pageno (in body 1 - n)
global  int             line;           // current output lineno on page
global  int             lc;             // remaining lines on page
 
global  ix_h_blk    *   index_dict;     // the index structure dict
 
global  symvar      *   global_dict;    // global symbol dictionary
global  symvar      *   sys_dict;       // global system symbol dictionary
global  mac_entry   *   macro_dict;     // macro dictionary
global  gtentry     *   tag_dict;       // User tag dictionary
 
 
global  struct GlobalFlags {
    unsigned        quiet         : 1;  // show Productinfo?
    unsigned        bannerprinted : 1;  // Productinfo shown
    unsigned        wscript       : 1;  // enable WATCOM script extension
    unsigned        firstpass     : 1;  // first or only pass
    unsigned        lastpass      : 1;  // last pass
    unsigned        inclist       : 1;  // show included files
    unsigned        warning       : 1;  // show warnings
    unsigned        statistics    : 1;  // output statistics at end
 
    unsigned        index         : 1;  // index option
    unsigned        free9         : 1;
    unsigned        freea         : 1;
    unsigned        freeb         : 1;
    unsigned        freec         : 1;
    unsigned        freed         : 1;
    unsigned        freee         : 1;
    unsigned        research      : 1;  // research mode, minimal formatting
} GlobalFlags;                          // Global flags
 
 
typedef enum ju_enum {                  // for .ju(stify)
    ju_off,                             // ju_off must have lowest value
    ju_on,                              // ju_on next
    ju_half,
    ju_left,
    ju_right,
    ju_centre,
    ju_inside,
    ju_outside
} ju_enum;
 
/***************************************************************************/
/*  enums for layout tags with attributes                                  */
/***************************************************************************/
 
typedef enum lay_sub {
    el_zero     = 0,                    // dummy to make 0 invalid
    el_page     = 1,
    el_default,
    el_widow,
    el_fn,
    el_fnref,
    el_p,
    el_pc,
    el_fig,
    el_xmp,
    el_note,
    el_h0,
    el_h1,
    el_h2,
    el_h3,
    el_h4,
    el_h5,
    el_h6,
    el_heading,
    el_lq,
    el_dt,
    el_gt,
    el_dthd,
    el_cit,
    el_figcap,
    el_figdesc,
    el_dd,
    el_gd,
    el_ddhd,
    el_abstract,
    el_preface,
    el_body,
    el_backm,
    el_lp,
    el_index,
    el_ixpgnum,
    el_ixmajor,
    el_ixhead,
    el_i1,
    el_i2,
    el_i3,
    el_toc,
    el_tocpgnum,
    el_toch0,
    el_toch1,
    el_toch2,
    el_toch3,
    el_toch4,
    el_toch5,
    el_toch6,
    el_figlist,
    el_flpgnum,
    el_titlep,
    el_title,
    el_docnum,
    el_date,
    el_author,
    el_address,
    el_aline,
    el_from,
    el_to,
    el_attn,
    el_subject,
    el_letdate,
    el_open,
    el_close,
    el_eclose,
    el_distrib,
    el_appendix,
    el_sl,
    el_ol,
    el_ul,
    el_dl,
    el_gl,
    el_banner,
    el_banregion
} lay_sub;
 
global struct ProcFlags {
    unsigned        newLevelFile    : 1;// start new include Level (file)
    unsigned        macro_ignore    : 1;// .. in col 1-2
    unsigned        CW_sep_ignore   : 1;// .' in col 1-2
    unsigned        in_macro_define : 1;// macro definition active
    unsigned        suppress_msg    : 1;// suppress error msg (during scanning)
    unsigned        blanks_allowed  : 1;// blanks allowed (during scanning)
    unsigned        keep_ifstate    : 1;// leave ifstack unchanged for next line
    unsigned        goto_active     : 1;// processing .go label
 
    unsigned        substituted     : 1;// variabel substituted in current line
    unsigned        unresolved      : 1;// variable found, but not yet resolved
    unsigned        late_subst      : 1;// special variable found &gml, &amp,
    unsigned        literal         : 1;// .li is active
    unsigned        in_trans        : 1;// esc char is specified (.ti set x)
    unsigned        reprocess_line  : 1;// unget for current input line
    unsigned        layout          : 1;// :layout is active
    unsigned        concat          : 1;// .co ON if set
 
    ju_enum         justify         : 8;// .ju on half off ...
 
    lay_sub         lay_xxx         : 8;// :layout sub tag is active
 
} ProcFlags;                            // processing flags
 
 
global  size_t          buf_size;       // default buffer size
global  char        *   token_buf;
 
global char         *   buff2;          // input buffer
global size_t           buff2_lg;       // input buffer used length
global char         *   open_paren;     // ( in input
global char         *   clos_paren;     // ) in input
global char         *   var_unresolved; // first unresolved var in input
 
global char             srnm[SYM_NAME_LENGTH + 1];// symbol name for getsym()
global sub_index        srnmsub;        // subscript
 
// the following to manage .gt * and .ga * * syntax
global char         tagname[TAG_NAME_LENGTH + 1];// last defined GML tag name
global gtentry  *   tag_entry;          // ... entry in tag_dict
global char         attname[ATT_NAME_LENGTH + 1];// last defined GML attribute
global gaentry  *   att_entry;          // ... entry in tag_dict
 
global  long        li_cnt;             // remaining count for .li processing
 
global  uint8_t     in_esc;             // input char for .ti processing
 
#if 0
global word_line    w_line;             // for constructing output line
global text_word  * word_pool;          // for reuse of text_word  structs
global word_line    w_line;
 
global  uint8_t     cur_font_num;       // the font to use for current line
#endif
 
 
/***************************************************************************/
/*  some globals which are to be redesigned when the :LAYOUT tag is coded. */
/*  Defined here so some script control words can be prototyped            */
/***************************************************************************/
 
global  uint32_t    g_cur_h_start;
global  uint32_t    g_cur_v_start;
global  uint32_t    g_page_bottom;
global  uint32_t    g_page_left;
global  uint32_t    g_page_right;
global  uint32_t    g_page_top;
 
global  uint32_t    g_cl;               // column length
global  uint32_t    g_ll;               // line length
global  uint32_t    g_cd;               // no of columns
global  uint32_t    g_gutter;           // space between columns
global  uint32_t    g_offset[9];        // column start offset
 
/***************************************************************************/
/*  :LAYOUT  data                     incomplete  TBD                      */
/***************************************************************************/
global  int32_t         lay_ind;// index into lay_tab for attribute processing
global  layout_data     layout_save;    // layout used at pass start
global  layout_data     layout_work;    // layout used for formatting
 
 
/***************************************************************************/
/* Layout attributes as character strings                                                                         */
/***************************************************************************/
extern  const   char    att_names[e_dummy_max + 1][18];
 
 
/***************************************************************************/
/*   definitions for the sequence of attribute values                      */
/***************************************************************************/
 
/***************************************************************************/
/*   :PAGE attributes                                                      */
/***************************************************************************/
extern  const   lay_att     page_att[5];
 
/***************************************************************************/
/*   :DEFAULT attributes                                                   */
/***************************************************************************/
extern  const   lay_att     default_att[8];
 
/***************************************************************************/
/*   :WIDOW attributes                                                     */
/***************************************************************************/
extern  const   lay_att     widow_att[2];
 
/***************************************************************************/
/*   :FN    attributes                                                     */
/***************************************************************************/
extern  const   lay_att     fn_att[11];
 
/***************************************************************************/
/*   :FNREF    attributes                                                  */
/***************************************************************************/
extern  const   lay_att     fnref_att[3];
 
/***************************************************************************/
/*   :P     attributes                                                     */
/***************************************************************************/
extern  const   lay_att     p_att[4];
 
/***************************************************************************/
/*   :PC    attributes                                                     */
/***************************************************************************/
extern  const   lay_att     pc_att[4];
 
/***************************************************************************/
/*   :FIG   attributes                                                     */
/***************************************************************************/
extern  const   lay_att     fig_att[9];
 
 
 
/* Reset so can be reused with other headers. */
#undef global
 
#endif  /* GVARS_H_INCLUDED */
 
