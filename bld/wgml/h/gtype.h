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
* Description:  wgml path and separator char definitions, Environment variables
*               for includes, ...
*
****************************************************************************/

#ifndef GTYPE_H_INCLUDED
#define GTYPE_H_INCLUDED


#if defined(__QNX__) || defined(__LINUX__) // try to be nice to linux
    #define PATH_SEP        "/"
    #define INCLUDE_SEP     ':'
#elif defined(__DOS__) || defined(__OS2__) || defined(__NT__) || defined(__OSI__)
    #define PATH_SEP        "\\"
    #define INCLUDE_SEP     ';'
#else
    #error gtype.h not configured for system
#endif

#define ulong           unsigned long

//================= Some global defines ========================
#define MAX_NESTING     32              // max nesting of option files
#define MAX_PASSES      10              // max no of passes
#define MAX_INC_DEPTH   255             // max include level depth
#define BUF_SIZE        512             // default buffersize for filecb e.a.
#define MAX_FILE_ATTR   15              // max size for fileattr (T:xxxx)
#define SCR_KW_LENGTH   2               // script control word length
#define TAG_NAME_LENGTH 15              // :tag name length
#define ATT_NAME_LENGTH 9               // :tag attr name len
#define SYM_NAME_LENGTH 10              // symbol name length
#define MAC_NAME_LENGTH 8               // macro name length
#define MAX_MAC_PARMS   32              // maximum macro parm count
                                        // arbitrary value, not found in docu!!!
#define MAC_STAR_NAME   "_"             // local variable name for &*


/* default filename extensions */
#define DEF_EXT         ".def"
#define ERR_EXT         ".err"
#define GML_EXT         ".gml"
#define LAY_EXT         ".lay"
#define OPT_EXT         ".opt"

#define GML_CHAR_DEFAULT    ':'         // start of GML tag
#define SCR_CHAR_DEFAULT    '.'         // start of Script keyword
#define CW_SEP_CHAR_DEFAULT ';'         // script controlline seperator


/***************************************************************************/
/*  Space units Horiz + Vert              to be redesigned                 */
/***************************************************************************/

typedef enum space_units {
    SU_undefined,                       // don't care = value zero
    SU_chars_lines = 10,                // undimensioned value
    SU_chars = 1,                       // chars horizontal
    SU_lines = 1,                       // lines vertical
    SU_cicero,                          // Cicero
    SU_cm,                              // centimeter
    SU_dv,                              // device units
    SU_ems,                             // Ems
    SU_inch,                            // inch
    SU_mm,                              // millimeter
    SU_pica,                            // pica
    SU_absolute                         // absolute mm with 4 decimals
} space_units;

#define MAX_SU_CHAR     12            // length of space units in char format

typedef struct {
    char        su_txt[ MAX_SU_CHAR ];
    long        su_whole;               // integer part
    long        su_dec;                 // decimal part (if any)
    long        su_conv;                // abs. value in mm with 4 decimals
    space_units su_u;                   // unit
    bool        su_relative;            // + - sign found
} su;

/***************************************************************************/
/*  Symbolic variables related                                             */
/***************************************************************************/

typedef enum {
    min_subscript = -1000000L,          // smallest valid subscript
    max_subscript =  1000000L,          // largest  valid subscript
    no_subscript  = 0x11223344          // value if not subscripted
                // must be outside of range min_subscript,max_subscript
} sub_index;

typedef enum {
    local_var   = 1,
    subscripted = 2,
    auto_inc    = 4,
    deleted     = 0x100
} sym_flags;


/***************************************************************************/
/*  entry for a subscripted symbolic variable                              */
/***************************************************************************/
typedef struct symsub {
    struct symsub   *   next;           // next subscript entry
    struct symvar   *   base;           // the base symvar
    sub_index           subscript;      // the subscript
    char            *   value;          // the value
} symsub;


/***************************************************************************/
/*  Symbolic variable base entry                                           */
/***************************************************************************/
typedef struct symvar {
    struct symvar   *   next;           // next base entry
    char                name[ SYM_NAME_LENGTH + 1];
    long                last_auto_inc;// last autoincremented subscript value
    long                subscript_used; // count of used subscript
    symsub          *   subscripts;     // subscript entries
    sym_flags           flags;
} symvar;


/***************************************************************************/
/*  Flags for filecb and macrocb                                           */
/***************************************************************************/

typedef enum {
    FF_clear        = 0x0000,           // clear all flags
    FF_startofline  = 0x0001,           // at start of physical line
    FF_eof          = 0x0002,           // eof
    FF_err          = 0x0004,           // file error
    FF_crlf         = 0x0008,           // delete trailing CR and / or LF
    FF_macro        = 0x0010,           // entry is macro not file
    FF_open         = 0x8000            // file is open
} fflags;

/***************************************************************************/
/*  List of (defined macro / input) lines                                  */
/***************************************************************************/
typedef struct inp_line {
    struct inp_line *   next;           // next line
    char                value[ 1 ];     // line content variable length
} inp_line;

/***************************************************************************/
/*  macro definition entry  for macro dictionary                           */
/***************************************************************************/
typedef struct mac_entry {
    struct mac_entry    *   next;
    char                    name[ MAC_NAME_LENGTH + 1 ];
    inp_line            *   macline;    // macro definition lines
    ulong                   lineno;     // lineno start of macro definition
    char                    mac_file_name[ 1 ]; // file name macro definition
                                            // var length
} mac_entry;


/***************************************************************************/
/*  entry for an included file                                             */
/***************************************************************************/

typedef struct filecb {
    fflags          flags;
    FILE        *   fp;                 // FILE ptr
    ulong           lineno;             // current line number
    ulong           linemin;            // first line number to process
    ulong           linemax;            // last line number to process
    size_t          usedlen;            // used data of filebuf
    fpos_t          pos;                // position for reopen
    char            fileattr[ MAX_FILE_ATTR + 1];  // T:xxxx
    char            filename[ 1 ];      // full filename var length
} filecb;

/***************************************************************************/
/*  parameter structure for macro call                                     */
/***************************************************************************/
typedef struct mac_parms {
    char        *   star;               // &*  complete parmline
    int             star0;              // &*0 parmcount
    inp_line    *   starx;              // &*1 - &*x parms

} mac_parms;

/***************************************************************************/
/*  Entry for an included macro                                            */
/***************************************************************************/

typedef struct  macrocb {
    fflags          flags;
    ulong           lineno;             // current macro line number
    inp_line    *   macline;            // list of macro lines
    mac_entry   *   mac;                // macro definition entry
} macrocb;

typedef enum {
    II_file     = 0x01,                 // inputcb is file
    II_macro    = 0x02,                 // inputcb is macro
    II_eof      = 0x04                  // end of file (input)
} i_flags;

/***************************************************************************/
/*  input stack for files and macros                                       */
/***************************************************************************/
typedef struct  inputcb {
    struct inputcb  *   prev;
    i_flags             fmflags;
    inp_line        *   hidden_head;    // manage split lines at ; or :
    inp_line        *   hidden_tail;    // manage split lines at ; or :
    symvar          *   local_dict;     // local symbol dictionary
    union  {
        filecb      *   f;              // used if input is from file
        macrocb     *   m;              // used if input is from macro
    } s;
} inputcb;


/***************************************************************************/
/*  scr keywords                                                           */
/***************************************************************************/

typedef struct scrtag {
    char            tagname[ SCR_KW_LENGTH + 1 ];
    void            (*tagproc)( void );
} scrtag;


/***************************************************************************/
/*  GML tags                                                               */
/***************************************************************************/

typedef enum {
    tag_only     = 1,                   // tag without any attribute
    tag_basic    = 2,                   // basic elements possible on tag line.
    tag_text     = 4,                   // text line possible
    etag_req     = 8,                   // eTAG required
    etag_opt     = 16,                  // eTAG optional
    tag_is_basic = 32                   // basic tag
} gmlflags;


typedef struct gmltag {
   char             tagname[ TAG_NAME_LENGTH + 1 ];
   size_t           taglen;
   void             (*gmlproc)( const struct gmltag * entry );
   gmlflags         tagflags;
} gmltag;

/***************************************************************************/
/*  condcode  returncode for several conditions during parameterchecking   */
/*            loosely adapted from wgml 88.1 on  IBM S/360 code            */
/***************************************************************************/

typedef enum condcode {            // return code for some scanning functions
    zero            = 0,
    omit            = 1,                // argument omitted
    pos             = 2,                // argument affirmative
    neg             = 4,                // argument negative
    quotes          = 6,                // argument within quotes
    no              = 8,                // argument undefined
    notnum          = 8                 // value not numeric / overflow
}  condcode;


/***************************************************************************/
/*  definitions for getnum routine  to be reworked  TBD                    */
/***************************************************************************/


typedef enum {
    selfdef     = 4,
    aritherr    = 8,
    ilorder     = 12,
    illchar     = 16,
    mnyerr      = 20,
    operr       = 24,
    parerr      = 28,
    enderr      = 32
} getnumrc;

typedef struct getnum_block {
    condcode    cc;
    int         ignore_blanks;          // 1 if blanks are ignored
    char    *   argstart;
    char    *   argstop;
    char    *   errstart;
    char    *   first;
    long        length;
    long        result;                 // result as long
    char        resultstr[12];          // result in char format
    getnumrc    error;
    char        num_sign;              // remember absolute or relative value
} getnum_block;

/***************************************************************************/
/*  struct used to hold parameters of option FONT                          */
/***************************************************************************/

typedef struct opt_font {
    struct opt_font *   nxt;
    uint8_t             font;
    char *              name;
    char *              style;
    uint32_t            space;
    uint32_t            height;
} opt_font;

#endif                                  // GTYPE_H_INCLUDED
