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
* Description:  IBM HLL and pre-CV4 CodeView debugging information.
*
****************************************************************************/


/* Note: The following is conceptually similar to cv4.h, but significantly
 * different in detail. Why lump together old CodeView with IBM HLL instead
 * of keeping all CodeView information in one place? Firstly, the IBM HLL
 * format is an outgrowth of old CodeView style debug data, and therefore old
 * CodeView format is much closer to HLL than it is to CV4. Secondly, and more
 * importantly, it is possible to have HLL and old CodeView debug information
 * mixed in a single executable image, hence the need to support HLL and old
 * CodeView in a single package.
 */

#define HLL_FIRST_USER_TYPE 0x200

#include "watcom.h"
typedef unsigned_8      u1;     /* short forms for read/written types */
typedef unsigned_16     u2;
typedef unsigned_32     u4;
typedef unsigned_64     u8;
typedef signed_8        i1;
typedef signed_16       i2;
typedef signed_32       i4;
typedef signed_64       i8;

/*
 * here are structures used in the executables.
*/


#define HLL_SIG_SIZE    4
#define HLL_NB02        "NB02"
#define HLL_NB04        "NB04"

#if 0
/* leaf type codes as defined in CV4 doc 3.1 */

typedef enum  lf_values {   /* type enumerations */
/* predefined numeric types  add as needed */
    LF_TNOTYPE       =0x0000,
    LF_TSEGMENT      =0x0002,
    LF_TVOID         =0x0003,
    LF_TCHAR         =0x0010,
    LF_TSHORT        =0x0011,
    LF_TRCHAR        =0x0070,
    LF_TINT4         =0x0074,
#define _LFMAC( n, N, c )    LF_##N = c,
    #include "cv4types.h"
#undef _LFMAC
#define _LFNUM( n, N, c )    LF_##N = c,
    #include "cv4nums.h"
#undef _LFNUM
#undef _LFMAC
#define _LFPAD( n, N, c )    LF_##N = c,
    #include "cv4pads.h"
#undef _LFPAD
} lf_values;

/* leaf symbol codes as defined in CV4 doc 3.1 */

typedef enum s_values { /* symbol enumerations */
    #define SLMAC( n, N, c )    S_##N = c,
    #include "cv4syms.h"
    #undef SLMAC
} s_values;
#endif

#pragma pack( 1 )
typedef enum {
    hll_sstModules = 0x101,
    hll_sstPublics,
    hll_sstTypes,
    hll_sstSymbols,
    hll_sstSrcLines,
    hll_sstLibraries,
    hll_sstImports,
    hll_sstCompacted,
    hll_sstSrcLnSeg,
    hll_sstHLLSrc = 0x10B
} hll_sst;

typedef struct {
    char        sig[HLL_SIG_SIZE];
    signed_32   offset;
} hll_trailer;

typedef struct {
    unsigned_16 cbDirHeader;
    unsigned_16 cbDirEntry;
    unsigned_32 cDir;
} hll_debug_dirinfo;

typedef struct {
    unsigned_16 subsection;
    unsigned_16 iMod;
    unsigned_32 lfo;
    unsigned_32 cb;
} hll_debug_dir;

typedef struct {
    unsigned_16 Seg;
    unsigned_32 offset;
    unsigned_32 cbSeg;
} hll_debug_seg_32;

#define HLL_DEBUG_STYLE_CV ('C' << 8 | 'V')
#define HLL_DEBUG_STYLE_HL ('H' << 8 | 'L')

typedef struct {
    hll_debug_seg_32    SegInfo;
    unsigned_16         ovlNumber;
    unsigned_16         iLib;
    unsigned_16         cSeg;
    unsigned_16         Style;
    unsigned_16         Version;
    char                name[];
} hll_debug_mod_32;

typedef struct {
    unsigned_32 offset;
    unsigned_16 seg;
    unsigned_16 type;
    char        name[1];
} hll_public_32;

typedef struct {
    unsigned_16 offset;
    unsigned_16 seg;
    unsigned_16 type;
    char        name[1];
} hll_public_16;

typedef struct {
//    char      name[1];
    unsigned_16 seg;
    unsigned_16 cPair;
//      line_offset_parms[1];
} cv_linnum_seg;

typedef struct {
    unsigned_16     line;
    unsigned_16     offset;
} cv_srcln_off_16;

typedef struct {
    unsigned_16 lineNum;
    unsigned_16 sfi;
    unsigned_32 offset;
} hll_linnum_entry;

/* HLL version 4 (HL04) specific line number information */

typedef struct {
    unsigned_16 lineNum;
    unsigned_8  entryType;
    unsigned_8  reserved;
    unsigned_16 numEntries;
    unsigned_16 segNum;
    unsigned_32 offset;
} hl4_linnum_first;

typedef struct {
    unsigned_32 firstChar;
    unsigned_32 numChars;
    unsigned_32 numFiles;
} hl4_filetab_entry;


/* HLL version 3 (HL03) specific line number information */

typedef struct {
    unsigned_16 lineNum;
    unsigned_8  entryType;
    unsigned_8  reserved;
    unsigned_16 numEntries;
    unsigned_16 segNum;
    unsigned_32 tableSize;
} hl3_linnum_first;

typedef struct {
    unsigned_32 srcStart;
    unsigned_32 numRecords;
    unsigned_32 numFiles;
} hl3_filetab_entry;


/* Old pre-CV4 types and definitions (NB02 signature) */

typedef struct {
    unsigned_16 subsection;
    unsigned_16 iMod;
    signed_32   lfo;
    unsigned_16 cb;
} cv3_directory_entry;

typedef struct {
    unsigned_16 Seg;
    unsigned_16 offset;
    unsigned_16 cbSeg;
} cv3_seginfo;

typedef struct {
    cv3_seginfo SegInfo;
    unsigned_16 ovlNumber;
    unsigned_16 iLib;
    unsigned_8  cSeg;
    unsigned_8  reserved;
//  char        name[];
//  cv3_seginfo arnsg[];
} cv3_sst_module;

#pragma pack()
