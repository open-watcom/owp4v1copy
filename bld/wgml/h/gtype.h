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

typedef void                *VOIDPTR;
typedef enum {FALSE, TRUE}   bool;


/***************************************************************************/
/*  search sequence for gml, opt or layout files                           */
/***************************************************************************/

typedef enum {
    DS_cur_lib_inc_path = 1,            // curdir, gmllib, gmlinc, path
    DS_cur_inc_lib_path,                // curdir, gmlinc, gmllib, path
    DS_cur_lib_path                     // curdir,         gmllib, path
} DIRSEQ;


/***************************************************************************/
/*  Space units Horiz + Vert                                               */
/***************************************************************************/

typedef enum space_units {
    SU_undefined,                       // don't care = value zero
    SU_chars = 1,                       // chars per inch horizontal
    SU_lines = 1,                       // lines per inch vertical
    SU_inch,                            // inch
    SU_cm,                              // centimeter
    SU_mm,                              // millimeter
    SU_cicero,                          // Cicero
    SU_dev_unit,                        // device units
    SU_ems,                             // Ems
    SU_pica                             // pica
} space_units;

typedef struct filelist {
    struct filelist     *next;
    size_t              len;            // length of name
    char                name[1];        // var length
} FILELIST;


typedef enum fflags {
    FF_clear          = 0x0000,         // clear all flags
    FF_startofline    = 0x0001,         // at start of physical line
    FF_eof            = 0x0002,         // file eof
    FF_open           = 0x8000          // file is open
} fflags;

typedef struct filecb {
    struct filecb  *prev;
    int         level;                  // include level
    int         lineno;                 // line number
    size_t      buflen;                 // length of filebuf
    size_t      recsize;                // size from (x:nnnn)
    char        *filebuf;
    char        *scanPtr;               // ptr into filebuf
    char        currchar;
    FILE        *fp;                   // FILE ptr
    fpos_t      pos;                    // position for reopen
    fflags      flags;
    char        filename[1];            // full filename var length
} FILECB;


#if defined(__QNX__) || defined(__LINUX__)
 #define PATH_SEP       "/"
 #define INCLUDE_SEP    ':'
#elif defined(__DOS__) || defined(__OS2__) || defined(__NT__) || defined(__OSI__)
 #define PATH_SEP       "\\"
 #define INCLUDE_SEP    ';'
#else
 #error gtype.h not configured for system
#endif

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


#endif                                 // GTYPE_H_INCLUDED
