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



/* default extensions */
#define DEF_EXT         ".def"
#define ERR_EXT         ".err"
#define GML_EXT         ".gml"
#define LAY_EXT         ".lay"
#define OPT_EXT         ".opt"

/* environment variable names */
#define GMLLIB          "GMLLIB"
#define GMLINC          "GMLINC"


#define GML_CHAR_DEFAULT    ':'
#define SCR_CHAR_DEFAULT    '.'
#define CW_SEP_CHAR_DEFAULT ';'


/***************************************************************************/
/*  search sequence for gml, opt or layout files                           */
/***************************************************************************/

typedef enum {
    DS_cur_lib_inc_path = 1,            // curdir, gmllib, gmlinc, path
    DS_cur_inc_lib_path,                // curdir, gmlinc, gmllib, path
    DS_cur_lib_path                     // curdir,         gmllib, path
} DIRSEQ;


/***************************************************************************/
/*  Space units Horiz + Vert to be redesigned                              */
/***************************************************************************/

typedef enum {
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
    long        su_conv;              // absolute value in mm with 4 decimals
    space_units su_u;                   // unit
    bool        su_relative;            // + - sign found
} su;

typedef enum {
    FF_clear        = 0x0000,           // clear all flags
    FF_startofline  = 0x0001,           // at start of physical line
    FF_eof          = 0x0002,           // file eof
    FF_err          = 0x0004,           // file error
    FF_crlf         = 0x0008,           // delete trailing CR and or LF
    FF_open         = 0x8000            // file is open
} fflags;


typedef struct filecb {
    struct filecb   *prev;
    FILE            *fp;                // FILE ptr
    ulong           lineno;             // current line number
    ulong           linemin;            // first line number to process
    ulong           linemax;            // last line number to process
    size_t          buflen;             // length of filebuf
    char            fileattr[ MAX_FILE_ATTR ]; // T:xxxx
    char            *filebuf;
    char            *scanPtr;           // ptr into filebuf
    char            *scanStop;          // last position to scan
    size_t          usedlen;            // used data of filebuf
    fpos_t          pos;                // position for reopen
    fflags          flags;
    char            filename[1];        // full filename var length
} filecb;



typedef struct tag {
    struct tag  *next;
    char        tagname[ 16 ];
    unsigned    min_abbrev;
    void       (*tagproc)( void );
} tag;







/***************************************************************************/
/*  GML tags                                                               */
/***************************************************************************/

typedef enum {
    tagonly     = 1,                    // tag without any attr
    tagbasic    = 2,                    // basic elements possible on tag line.
    tagtext     = 4,                    // only text possible
    etagreq     = 8,                    // eTAG required
    etagopt     = 16                    // eTAG optional
} gmlflags;


typedef struct {
   char             tagname[ TAG_NAME_LENGTH ];
   gmlflags         tagflags;
   void             (*proctag)( void );
} gmltag;


/***************************************************************************/
/*  definitions for getnum routine                                         */
/***************************************************************************/

typedef enum {            // return code for some scanning functions
    omit    = 1,                        // parm(s) omitted
    pos     = 2,                        // value >= 0
    neg     = 4,                        // value < 0
    notnum  = 8                         // value not numeric / overflow
} condcode;

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

typedef struct {
    condcode    cc;
    int         ignore_blanks;          // 1 if blanks are ignored
    char        *argstart;
    char        *argstop;
    char        *errstart;
    char        *first;
    long        length;
    long        res;
    getnumrc    error;
    char        resc[5];
} getnum_block;


#endif                                 // GTYPE_H_INCLUDED
