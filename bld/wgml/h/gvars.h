/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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

#include <setjmp.h>
#include <time.h>

#include "gtype.h"
#include "gtypelay.h"

#if defined( __WATCOMC__ )
#pragma enable_message( 128 ); // reenable: Warning! W128: 3 padding byte(s) added
#endif

typedef struct global_flags {
    unsigned        quiet         : 1;  // suppress product info
    unsigned        bannerprinted : 1;  // product info shown
    unsigned        wscript       : 1;  // enable WATCOM script extension
    unsigned        firstpass     : 1;  // first or only pass
    unsigned        lastpass      : 1;  // last or only pass
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
    unsigned        research      : 1;  // -r global research mode output
} global_flags;                         // Global flags

typedef struct proc_flags {
    doc_section     doc_sect;           // which part are we in (FRONTM, BODY, ...
    doc_section     doc_sect_nxt;       // next section (tag already seen)
    unsigned        frontm_seen    : 1; // FRONTM tag seen
    unsigned        start_section  : 1; // start section call done

    unsigned        researchfile   : 1; // research for one file ( -r filename )

    unsigned        fb_document_done    : 1;// true if fb_document() called
    unsigned        fb_position_done    : 1;// first positioning on new page done
    unsigned        page_ejected        : 1;// a page was deliberately ejected (headings)
    unsigned        page_started        : 1;// we have something for the curr page
    unsigned        col_started         : 1;// we have something for the curr page
    unsigned        line_started        : 1;// we have something for current line
    unsigned        just_override       : 1;// current line is to be justified

    unsigned        author_tag_seen     : 1;// remember first :AUTHOR tag
    unsigned        date_tag_seen       : 1;// :DATE is allowed only once
    unsigned        docnum_tag_seen     : 1;// :DOCNUM is allowed only once
    unsigned        index_tag_cw_seen   : 1;// .IX, :I1-3, :IH1-3, :IREF seen
    unsigned        stitle_seen         : 1;// remember first stitle value
    unsigned        title_tag_top       : 1;// :TITLE pre_top_skip used
    unsigned        title_text_seen     : 1;// remember first :TITLE tag text

    unsigned        heading_banner      : 1;// banner replaced for heading (Hn)
    unsigned        goto_active         : 1;// processing .go label
    unsigned        newLevelFile        : 1;// start new include Level (file)
    unsigned        gml_tag             : 1;// input buf starts with GML_char
    unsigned        scr_cw              : 1;// input buf starts with SCR_char
    unsigned        if_cond             : 1;// symbol substitution in if condition
    unsigned        macro_ignore        : 1;// .. in col 1-2
    unsigned        CW_force_sep        : 1;// scr cw line was indented and separator must be recognized
    unsigned        CW_noblank          : 1;// no blank between CW/macro and first operand
    unsigned        CW_sep_ignore       : 1;// ignore scr cw separator
    unsigned        indented_text       : 1;// text was indented      
    unsigned        in_macro_define     : 1;// macro definition active
    unsigned        in_figcap           : 1;// FIGCAP in progress
    unsigned        suppress_msg        : 1;// suppress error msg (during scanning)
    unsigned        blanks_allowed      : 1;// blanks allowed (during scanning)
    unsigned        keep_ifstate        : 1;// leave ifstack unchanged for next line
    unsigned        substituted         : 1;// variable substituted in current line
    unsigned        unresolved          : 1;// variable found, but not yet resolved
    unsigned        literal             : 1;// .li is active
    unsigned        concat              : 1;// .co ON if set
    unsigned        ct                  : 1;// .ct continue text is active
    unsigned        fsp                 : 1;// force space in spite of .ct
    unsigned        zsp                 : 1;// force no space (used when start position of next text_chars is already set)
    unsigned        as_text_line        : 1;// process text as <text line>
    unsigned        in_figlist_toc      : 1;// process FIGLIST/TOC text as <text line>
    unsigned        force_pc            : 1;// use PC tag processing on text not preceded by a tag or control word
    unsigned        utc                 : 1;// user tag with "continue" is active
    unsigned        in_trans            : 1;// esc char is specified (.ti set x)
    unsigned        reprocess_line      : 1;// unget for current input line
    unsigned        sk_2nd              : 1;// .sk follows blank lines of .sp
    unsigned        sk_co               : 1;// .sk -1/.sk n, n >0 processed with CO OFF
    unsigned        overprint           : 1;// .sk -1 active or not
    unsigned        tag_end_found       : 1;// '.' ending tag found
    unsigned        skips_valid         : 1;// controls set_skip_vars() useage
    unsigned        new_pagenr          : 1;// FIG/heading page number changed
    unsigned        first_hdr           : 1;// first header done
    unsigned        box_cols_cur        : 1;// current BX line had column list
    unsigned        bx_set_done         : 1;// BX SET was done last before current BX line
    unsigned        draw_v_line         : 1;// vertical lines are to be drawn for this BX line
    unsigned        force_op            : 1;// force overprint (used with BX CAN/BX DEL)
    unsigned        in_bx_box           : 1;// identifies first BX line
    unsigned        no_bx_hline         : 1;// determines if a horizontal line is to be emitted or not
    unsigned        top_line            : 1;// determines if current line is at top of page
    unsigned        vline_done          : 1;// determines if a vertical line was done
    unsigned        keep_left_margin    : 1;// for indent NOTE tag paragraph
    unsigned        skip_blank_line     : 1;// for XMP/eXMP blocks in macros -- scope TBD
    unsigned        in_reduced          : 1;// position resulting from IN reduced to left edge of device page
    unsigned        dd_starting         : 1;// DD after break had no text (in next scr_process_break())
    unsigned        para_starting       : 1;// :LP, :P or :PC had no text (in scr_process_break())
    unsigned        para_has_text       : 1;// :LP, :P, :PB or :PC had text (used by PB)
    unsigned        titlep_starting     : 1;// AUTHOR or TITLE had no text (in scr_process_break())

    unsigned        ix_in_block         : 1;// index tag/cw term attaches to following text
    unsigned        post_ix             : 1;// index tag/cw preceded current text

    unsigned        cc_cp_done          : 1;// CC or CP done; apply current inset to first line only
    unsigned        dd_break_done       : 1;// DD break done (first line of text only)
    unsigned        dd_macro            : 1;// DT/DD were invoked inside a macro
    unsigned        dt_space            : 1;// insert one space after DT text
    unsigned        null_value          : 1;// current symbol has "" or equivalent as its value

    unsigned        dd_space            : 1;// insert one space before DD text
    unsigned        need_dd             : 1;// DT seen; DD must be next tag
    unsigned        need_ddhd           : 1;// DTHD seen; DDHD must be next tag
    unsigned        need_gd             : 1;// GT seen; GD must be next tag
    unsigned        need_li_lp          : 1;// top of list/need LI/LP (OL,SL,UL)
    unsigned        need_tag            : 1;// need tag now, not text
    unsigned        need_text           : 1;// need text now, not tag or cw/macro
    unsigned        no_var_impl_err     : 1;// suppress err_var_not_impl msg
    unsigned        tophead_done        : 1;// tophead symbol set
    unsigned        wrap_indent         : 1;// for index item/reference indent when line breaks

    unsigned        has_aa_block        : 1;// true if device defined :ABSOLUTEADDRESS
    unsigned        ps_device           : 1;// true if device is PS (PostScript)
    unsigned        wh_device           : 1;// true if device is WHELP (help file)

    unsigned        layout              : 1;// within :layout tag and sub tags
    unsigned        lay_specified       : 1;// LAYOUT option or :LAYOUT tag seen
    unsigned        banner              : 1;// within layout banner definition
    unsigned        banregion           : 1;// within layout banregion definition
    unsigned        hx_level            : 3;// 0 - 6  active Hx :layout sub tag
    lay_sub         lay_xxx             : 8;// active :layout sub tag

    ju_enum         justify             : 8;// .ju on half off ...

} proc_flags;                           // processing flags

#if defined( __WATCOMC__ )
#pragma enable_message( 128 ); // reenable: Warning! W128: 3 padding byte(s) added
#endif

#endif  /* GVARS_H_INCLUDED */

#if !defined( GVARS_H_INCLUDED ) || defined( global )
#define GVARS_H_INCLUDED

#ifndef global
    #define global  extern
#endif

global struct tm        doc_tm;         // document time/date

global  jmp_buf     *   environment;    // var for GSuicide()

global  char        *   scan_start;
global  char        *   scan_stop;
global  char        *   new_file_parms; // command tail for IM/AP
global  char        *   scan_char_ptr;  // used by character scanning routines
global  char        *   scan_restart;   // used by character scanning routines
global  bool            scan_err;       // used by character scanning routines
global  char        *   tok_start;      // start of scanned token
global  size_t          arg_flen;       // arg length
global  char        *   att_start;      // (potential) attribute start
global  size_t          val_len;        // attribute value length
global  char        *   val_start;      // attribute value start
global  char            quote_char;     // value is quoted by this char or \0
global  locflags        rs_loc;         // restricted location

global  int             switch_char;    // DOS switch character
global  char        *   alt_ext;        // alternate extension
global  char        *   def_ext;        // default extension
global  char            ampchar;        // symbol substitution/attribute/function marker

global  char        *   master_fname;   // Primary input file name
global  char        *   master_fname_attr;// Primary input file name attributes
global  ulong           print_from;     // first page to print
global  ulong           print_to;       // last page to print
global  char        *   dev_name;       // device defined_name
global  opt_font    *   opt_fonts;      // option FONT parameters (linked list)

global  inputcb     *   input_cbs;      // GML input stack (files + macros)
global  fnstack     *   fn_stack;       // input filename stack
global  char        *   out_file;       // output file name
global  char        *   out_file_attr;  // output file attributes (T:2222)
global  unsigned        inc_level;      // include nesting level 1 = MasterFname
global  unsigned        max_inc_level;  // maximum include level depth
global  line_number     line_from;      // starting lineno to process
global  line_number     line_to;        // ending lineno to process
#define LINEFROM_DEFAULT    1
#define LINETO_DEFAULT      (0x1000000) // 16 MiB lines should be enough

global  char            gotarget[MAC_NAME_LENGTH +1];   // .go to target name
global  int32_t         gotargetno;     // .go to line no

global  int             err_count;      // Overall Errorcount
global  int             wng_count;      // Overall warning count

global  char            GML_char;       // GML Keywword start char (normally ":")
global  char            SCR_char;       // SCRIPT keywword start char (normally ".")
global  char            CW_sep_char;    // Control Word separator char (normally ";")

global  int             CPI;            // chars per inch
global  space_units     CPI_units;      // unit for chars per inch
global  int             LPI;            // lines per inch
global  space_units     LPI_units;      // unit for lines per inch

global  su              bind_odd;       // Bind value for odd pages

global  su              bind_even;      // Bind value for even pages

global  int             passes;         // Max no of document passes
global  int             pass;           // current document pass no

global  uint32_t        apage;          // current absolute pageno &$apage
global  uint32_t        page;           // current document pageno &$page
global  line_number     line;           // current output lineno   &$line
global  int32_t         lcmax;          // remaining lines on page initial

global  int32_t         hm;             // heading margin          &$hm
global  int32_t         tm;             // top margin              &$tm

global  int32_t         bm;             // bottom margin           &$bm
global  int32_t         fm;             // footing margin          &$fm

global  int32_t         lm;             // left margin             &$pagelm
global  int32_t         rm;             // right margin            &$pagerm

global  symvar      *   global_dict;    // global symbol dictionary
global  symvar      *   sys_dict;       // global system symbol dictionary
global  mac_entry   *   macro_dict;     // macro dictionary
global  gtentry     *   tag_dict;       // user tag dictionary

global  char            research_file_name[48]; // filename for research
global  line_number     research_from;  // line no start for research output
global  line_number     research_to;    // line no end   for research output

global  global_flags    GlobalFlags;    // Global flags

global  proc_flags      ProcFlags;      // processing flags

global  size_t          buf_size;       // default buffer size
global  char        *   token_buf;

global char         *   workbuf;        // work for input buffer
global char         *   buff2;          // input buffer
global size_t           buff2_lg;       // input buffer used length

// the following to manage .gt * and .ga * * syntax
global char         tagname[TAG_NAME_LENGTH + 1];// last defined GML tag name
global gtentry  *   tag_entry;          // ... entry in tag_dict
global char         attname[ATT_NAME_LENGTH + 1];// last defined GML attribute
global gaentry  *   att_entry;          // ... entry in tag_dict

global  long        li_cnt;             // remaining count for .li processing

global  uint8_t     in_esc;             // input escape char from .ti


// file block support
global doc_el_group     *   block_queue;        // queue of FB blocks; blocks removed from here
global doc_el_group     *   block_queue_end;    // add point for new FB blocks

// box support
global  box_col_set     *   box_col_set_pool;   // pool of box_col_set instances
global  box_col_set     *   cur_line;           // the line from the current BX line
global  box_col_set     *   prev_line;          // the previously drawn line
global  box_col_stack   *   box_col_stack_pool; // pool of box_col_stack instances
global  box_col_stack   *   box_line;           // the current line to be drawn
global  uint32_t            box_col_width;      // width of one column, as used with BX
global  uint32_t            h_vl_offset;        // horizontal offset used to position VLINE output
global  uint32_t            max_depth;          // space left on page (used by BX)

// figure support
global  uint32_t            fig_count;      // figure number
global  ffh_entry       *   fig_entry;      // current fig_list entry
global  fwd_ref         *   fig_fwd_refs;   // forward reference/undefined id/page change
global  ffh_entry       *   fig_list;       // list of figures in order encountered
global  record_buffer       line_buff;      // used for some frame types
global  ref_entry       *   fig_ref_dict;   // reference dictionary :FIG tags

// footnote support
global  uint32_t        fn_count;       // footnote number
global  ffh_entry   *   fn_entry;       // current fn_list entry
global  fwd_ref     *   fn_fwd_refs;    // forward reference/undefined id
global  ref_entry   *   fn_ref_dict;    // reference dictionary :FN tags
global  ffh_entry   *   fn_list;        // list of footnotes in order encountered

// heading support
global  ffh_entry   *   hd_entry;       // current hd_list entry
global  fwd_ref     *   hd_fwd_refs;    // forward reference/undefined id/page change
global  hdsrc           hd_level;       // current heading level
global  ref_entry   *   hd_ref_dict;    // reference dictionary :Hx tags
global  ffh_entry   *   hd_list;        // list of headings in order encountered
global  hd_num_data     hd_nums[hds_appendix];  // heading hierarchy numbering

// index support
global  bool            ixhlvl[2];      // true for levels that exist
global  eol_ix      *   eol_ix_pool;    // eol_ix pool
global  eol_ix      *   g_eol_ix;       // global list of eol_ix instances
global  ix_h_blk    *   index_dict;     // index structure dictionary
global  ix_h_blk    *   ixhtag[3];      // current entry for each level in index
global  ref_entry   *   ix_ref_dict;    // reference id dictionary :Ix :IHx :IREF

// page number format
global  num_style       pgnum_style[pns_max];

// symbol support
global  sym_list_entry  *   sym_list_pool;  // sym_list_entry pool
//global  sym_list_entry  *   g_sym_list;     // global stack of sym_list_entry instances

// tab support
global  tab_stop    *   c_stop;         // current tab_stop
global  uint32_t        first_tab;      // first default tab position
global  uint32_t        inter_tab;      // distance between default tabs
global  char            tab_char;       // tab character from .tb
global  uint32_t        tab_col;        // width of one column, as used with tabs
global  tag_cb      *   tt_stack;       // font stack entry to modify for tab tables

// the document page and related items
global doc_element      *   doc_el_pool;        // for reuse of doc_element structs
global group_type           cur_group_type;     // current tag/cw in effect (gt_bx, gt_co not allowed)
global doc_el_group     *   cur_doc_el_group;   // current doc_el_group, if any
global doc_el_group     *   t_doc_el_group;     // stack of groups of doc_elements
global doc_el_group     *   doc_el_group_pool;  // for reuse of doc_el_group structs
global doc_element      *   t_element;          // the current element for main
global doc_page             t_page;             // for constructing output page
global doc_next_page        n_page;             // for deferred elements
global page_pos             line_position;      // left, center, right
global text_chars       *   text_pool;          // for reuse of text_chars structs
global text_line        *   t_el_last;          // attachment point to t_element
global text_line        *   t_line;             // for constructing output line
global text_line        *   line_pool;          // for reuse of text_line structs

// document section support

global gen_sect             figlist_toc;        // used with FIGLIST, TOC and eGDOC

/***************************************************************************/
/*  some globals which are to be redesigned when the :LAYOUT tag is coded. */
/*  Defined here so some script control words can be prototyped            */
/*  These are very slowly being refactored.                                */
/***************************************************************************/

global  uint32_t    g_cur_v_start;
global  uint32_t    g_page_bottom_org;
global  uint32_t    g_page_left_org;
global  uint32_t    g_page_right_org;
global  uint32_t    g_page_depth;
global  uint32_t    g_max_char_width;
global  uint32_t    g_max_line_height;
global  uint32_t    g_net_page_depth;
global  uint32_t    g_net_page_width;

global  int32_t     g_resh;             // horiz base units
global  int32_t     g_resv;             // vert base units

global  font_number g_curr_font;        // the font to use for current line
global  font_number g_phrase_font;      // the font used with SF, even if too large
global  font_number g_prev_font;        // the font used for the last text output

global  uint32_t    g_cl;               // column length
global  uint32_t    g_ll;               // line length
global  uint32_t    g_cd;               // no of columns
global  uint32_t    g_gutter;           // space between columns

global  uint32_t    g_blank_text_lines; // blank lines (line count)
global  units_space g_blank_units_lines;// blank lines (in vertical base units)
global  uint32_t    g_post_skip;        // post_skip
global  uint32_t    g_subs_skip;        // subs_skip
global  uint32_t    g_top_skip;         // top_skip
global  text_space  g_text_spacing;     // spacing between lines (line count)
global  units_space g_units_spacing;    // spacing (in vertical base units)
global  int32_t     g_skip;             // .sk skip value (in vbus)
global  int32_t     g_space;            // .sp space value (in vbus)

global  uint32_t    post_space;         // spacing within a line
global  uint32_t    ju_x_start;         // .. formatting

global  int32_t     g_indent;           // .in 1st value (left) default 0
global  int32_t     g_indentr;          // .in 2nd value (right) default 0
global  int32_t     g_line_indent;      // :LP, :P, :PC line indent
global  int32_t     wrap_indent;        // :I1/:I2/:I3 wrap_indent value

global  int32_t     g_cur_threshold;    // current widow threshold value
                                        // from layout (widow or heading)

global  fwd_ref *   fwd_ref_pool;       // pool of unused fwd_ref instances

global  tag_cb  *   nest_cb;            // infos about nested tags
global  tag_cb  *   tag_pool;           // list of reusable tag_cbs

global  banner_lay_tag  * sect_ban_top[2];// top even / odd banner for curr sect
global  banner_lay_tag  * sect_ban_bot[2];// bot even / odd banner for curr sect

global  uint32_t    msg_indent;         // indent for message output (to screen, not to device)

global script_style_info    script_style;   // BD/US etc scope control

/***************************************************************************/
/*  tagnames as strings for msg display                                    */
/***************************************************************************/
global char str_tags[t_MAX + 1][10]
#if defined(tag_strings)
  = {
    { "NONE" },
    #define pickg( name, length, routine, gmlflags, locflags, classflags )  { #name },
    #include "gtags.h"
//    #define picklab( name, routine, flags )  extern void routine( void );
//    #define picks( name, routine, flags )  extern void routine( void );
//    #include "gscrcws.h" TBD
    { "MAX" }
}
#endif
;

/***************************************************************************/
/* The tab lists.                                                          */
/***************************************************************************/

global  tab_list        def_tabs;   // tabs at columns 6, 11, 16, ..., 81
global  tab_list        user_tabs;  // for tabs defined by control word TB

/***************************************************************************/
/*  :LAYOUT  data                                                          */
/***************************************************************************/

global  int32_t         lay_ind;        // index into lay_tab for attribute processing
global  layout_data     layout_work;    // layout used for formatting
global  laystack    *   lay_files;      // layout file(s) specified on cmdline

/***************************************************************************/
/*  document sections for banner definition                                */
/***************************************************************************/

extern  const   ban_sections    doc_sections[max_ban];

/***************************************************************************/
/*  place names for fig and banner definition                              */
/***************************************************************************/

extern  const   ban_places    bf_places[max_place];

/***************************************************************************/
/* Layout attribute names as character strings                             */
/*  array initialized in glconvrt.c                                        */
/*  longest attribute name is extract_threshold  (=17)                     */
/*                            ....+....1....+..                            */
/***************************************************************************/
extern  const   char    att_names[e_dummy_max + 1][18];

/***************************************************************************/
/*   declarations for the sequence of LAYOUT attribute values              */
/*   definitions are in the layout tag processing source file              */
/*   sequence as seen by :convert output                                   */
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
/*   :P and :PC attributes                                                 */
/***************************************************************************/
extern  const   lay_att     p_att[4];

/***************************************************************************/
/*   :FIG   attributes                                                     */
/***************************************************************************/
extern  const   lay_att     fig_att[9];

/***************************************************************************/
/*   :XMP   attributes                                                     */
/***************************************************************************/
extern  const   lay_att     xmp_att[7];

/***************************************************************************/
/*   :NOTE  attributes                                                     */
/***************************************************************************/
extern  const   lay_att     note_att[8];

/***************************************************************************/
/*   :H0 - :H6  attributes                                                 */
/***************************************************************************/
extern  const   lay_att     hx_att[18];

/***************************************************************************/
/*   :HEADING   attributes                                                 */
/***************************************************************************/
extern  const   lay_att     heading_att[6];

/***************************************************************************/
/*   :LQ        attributes                                                 */
/***************************************************************************/
extern  const   lay_att     lq_att[7];

/***************************************************************************/
/* :DT :GT :DTHD :CIT :GD :DDHD :IXPGNUM :IXMAJOR                          */
/*              attributes                                                 */
/***************************************************************************/
extern  const   lay_att     xx_att[2];

/***************************************************************************/
/*   :FIGCAP    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     figcap_att[6];

/***************************************************************************/
/*   :FIGDESC   attributes                                                 */
/***************************************************************************/
extern  const   lay_att     figdesc_att[3];

/***************************************************************************/
/*   :DD        attributes                                                 */
/***************************************************************************/
extern  const   lay_att     dd_att[3];

/***************************************************************************/
/*   :ABSTRACT and :PREFACE attributes                                     */
/***************************************************************************/
extern  const   lay_att     abspref_att[11];

/***************************************************************************/
/*   :BACKM and :BODY attributes                                           */
/***************************************************************************/
extern  const   lay_att     backbod_att[10];

/***************************************************************************/
/*   :LP        attributes                                                 */
/***************************************************************************/
extern  const   lay_att     lp_att[7];

/***************************************************************************/
/*   :INDEX     attributes                                                 */
/***************************************************************************/
extern  const   lay_att     index_att[14];

/***************************************************************************/
/*   :IXHEAD    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     ixhead_att[7];

/***************************************************************************/
/*   :I1 :I2 :I3 attributes   nearly identical :i3 without string_font     */
/***************************************************************************/
extern  const   lay_att     ix_att[9];

/***************************************************************************/
/*   :TOC       attributes                                                 */
/***************************************************************************/
extern  const   lay_att     toc_att[7];

/***************************************************************************/
/*   :TOCPGNUM  attributes                                                 */
/***************************************************************************/
extern  const   lay_att     tocpgnum_att[3];

/***************************************************************************/
/*   :TOCPHx    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     tochx_att[9];

/***************************************************************************/
/*   :FIGLIST   attributes                                                 */
/***************************************************************************/
extern  const   lay_att     figlist_att[7];

/***************************************************************************/
/*   :FLPGNUM   attributes                                                 */
/***************************************************************************/
extern  const   lay_att     flpgnum_att[3];

/***************************************************************************/
/*   :TITLEP    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     titlep_att[3];

/***************************************************************************/
/*   :TITLE     attributes                                                 */
/***************************************************************************/
extern  const   lay_att     title_att[7];

/***************************************************************************/
/*   :DOCNUM    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     docnum_att[7];

/***************************************************************************/
/*   :DATE      attributes                                                 */
/***************************************************************************/
extern  const   lay_att     date_att[7];

/***************************************************************************/
/*   :AUTHOR    attributes                                                 */
/***************************************************************************/
extern  const   lay_att     author_att[7];

/***************************************************************************/
/*   :ADDRESS   attributes                                                 */
/***************************************************************************/
extern  const   lay_att     address_att[6];

/***************************************************************************/
/*   :ALINE     attributes                                                 */
/***************************************************************************/
extern  const   lay_att     aline_att[2];

/***************************************************************************/
/*   :APPENDIX  attributes                                                 */
/***************************************************************************/
extern  const   lay_att     appendix_att[22];

/***************************************************************************/
/*   :DL :GL :OL :SL :UL attributes                                        */
/***************************************************************************/
extern  const   lay_att     dl_att[10];
extern  const   lay_att     gl_att[10];
extern  const   lay_att     ol_att[12];
extern  const   lay_att     sl_att[9];
extern  const   lay_att     ul_att[13];

/***************************************************************************/
/*   :BANNER and :BANREGION      attributes                                */
/***************************************************************************/
extern  const   lay_att     banner_att[8];
extern  const   lay_att     banregion_att[12];

/* Reset so can be reused with other headers. */
#undef global
#undef tag_strings

#endif  /* GVARS_H_INCLUDED */

