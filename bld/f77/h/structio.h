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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


//
// Definitions for the load'n go compiler for structure i/o and comparisons.
//
// There is code in RSTDUMP that outputs a structure defn.
// You must ensure it is up to date with this file.

// sdefn's refer to other sdefn's.  Rather than put a PGM ptr for every
// reference on the 8086, we simply put an offset.  Then SDEFN_REF_PGM
// turns it into a PGM ptr.
typedef obj_ptr sdefn_ref;
#if _CPU == 8086
    #include <i86.h>
    #define SDEFN_REF_PGM( seg, sd_ref )        MK_FP( (seg), (sd_ref) )
    #define PGM_SDEFN_REF( sd_ref )             FP_OFF( sd_ref )

#elif _CPU == 386
    #define SDEFN_REF_PGM( seg, sd_ref )        (void PGM *)( sd_ref )
    #define PGM_SDEFN_REF( sd_ref )             (sdefn_ref)( sd_ref )

#else
    #error Need to define sdefn_ref and SDEFN_REF_PGM
#endif

#define SDEFN_ARRAY     0x80    // structure field is an array
#define SDEFN_UNION     0x40    // structure field is a union
#define SDEFN_TYPE      0x0f    // type field

typedef struct {
    byte                info;   // low nibble is type
} sdefn_simple;

typedef struct {
    byte                info;   // low nibble is TY_CHARACTER
    inttarg             size;
} sdefn_char;

typedef struct {
    byte                info;   // low nibble is TY_STRUCTURE
    sdefn_ref           sd_ref; // reference to alternate struct def'n
} sdefn_struct;

typedef struct {
    byte                info;   // low nibble is TY_UNION
    sdefn_ref           map;    // reference to map sdefn
} sdefn_union;

typedef struct {
    inttarg             total_size;// total size of structure
    sdefn_ref           back_link; // for i/o, stack of structures to output
    sdefn_ref           back_offs; // for i/o, offset into last struct
    unsigned_32         elts_left; // for doing array i/o
} sdefn_header;

typedef union {
    byte                info;
    sdefn_simple        sm;
    sdefn_char          ch;
    sdefn_struct        st;
    sdefn_union         un;
    sdefn_header        hd;
} sdefn;

#define NEXT_SDEFN( sd, amt )   (sdefn PGM *)(((char PGM *)sd) + (amt))


typedef struct {
    char HPGM           *origin;        // pointer to storage
    sdefn_ref           cur_sdefn;      // current structure definition
    sdefn_ref           cur_offs;       // offset into current structure defn
} sio_cntrl_blk;
