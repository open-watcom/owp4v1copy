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
// GENOBJ       : Generate an object file (interface to WOMP code)
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <io.h>

#include "ftnstd.h"
#include "global.h"
#include "bglobal.h"
#include "fcgbls.h"
#include "sdfile.h"
#include "progsw.h"
#include "genobj.h"
#include "cpopt.h"
#include "ifargs.h"
#include "ifnames.h"
#include "saopt.h"
#include "wf77aux.h"
#include "csetinfo.h"
#include <pgmacc.h>
#include "genfixup.h"

#if ( _TARGET == _8086 )
  #define MK_OBJPTR( offs )     MK_FP( CurrSeg, offs )
#else
  #define MK_OBJPTR( offs )     (void *)( offs )
#endif

// At the current time these are all the 8086 types supported

#define hash_entry lib_hash_entry
#include "womp.h"
#include "genmsomf.h"
#include "objprs.h"
#include "objio.h"
#include "errcod.h"

enum which_seg {
    IN_SUBPROGRAM,
    IN_GLOBAL_DATA,
    IN_COMMON_BLOCK
};

extern  int             Spawn(void (*)());
extern  void            Suicide(void);
extern  void            FreeChain(void **);
extern  void            *FreeLink(void *);
extern  void            InfoError(int,...);
extern  int             MakeName(char *,char *,char *);
extern  char            *SDFName(char *);
extern  void            *FMemAlloc(size_t);
extern  void            FMemFree(void *);
extern  void            OutU32(unsigned_32);
extern  void            MsgFormat(char *,char *,...);
extern  void            GOFixSetData( obj_ptr *, my_fixup **, uint * );

#if _TARGET == _8086
extern  void            OutU16(unsigned_16);
#endif

extern  uint            BldObjName(sym_id,char *,uint);
extern  char            *ErrorInitializer(void);

extern  char            ObjExtn[];
extern  const byte      __FAR IFArgCt[];
extern  const char      *IFNames[];
extern  default_lib     *DefaultLibs;
extern  character_set   CharSetInfo;

static  qdesc           defnRecs;       // LNAMES, SEGDEF, EXTDEF, PUBDEF, etc.
static  qdesc           dataRecs;       // LEDATA, FIXUPP, COMENT
static  uint_16         segdefIdx;
static  uint_16         grpdefIdx;
static  uint_16         extdefIdx;
static  uint_16         lnamesIdx;
static  uint            numberOfSegsInDGROUP;
static  uint            numberOfCommonBlocks;
#if _TARGET == _8086
static  uint            litSegs;        // number of litsegs per pgm;
static  uint            gblSegs;        // number of gblsegs per pgm;
static  uint            comSegs;        // number of comsegs per coomon block
#endif
static  pobj_state      pobjState;      // for WOMP interface
static  pobj_filter     jumpTable[ CMD_MAX_CMD - CMD_POBJ_MIN_CMD + 1 ];
#define JUMP_OFFSET(cmd)    ((cmd)-CMD_POBJ_MIN_CMD)

static  obj_rec         *dataSEGDEF;    // SEGDEF record for _DATA seg
static  obj_rec         *curSP_LNAMES;  // LNAMES record for current SP
static  obj_rec         *curSP_SEG;     // current subprogram segment
static  obj_rec         *commonSEGDEF;  // current COMMON block segdef
static  obj_ptr         baseOfLEDATA;   // lowest byte belonging to LEDATA
#if _TARGET == _8086
 static obj_ptr         endOfSPSeg;     // end of SP seg ptr.
 static obj_ptr         gblPtr;         // Virtual global data offset
 static char            *commonName;    // name of current common block
 static size_t          commonNameLen;  // name length of current common block
 static unsigned_32     commonBlockSize;// Size of common segment
 static obj_rec         *commonClassLNAME;// LNAMES record for current class
 static obj_rec         *commonBlockDef;// current COMMON block's 1st segment
 static obj_rec         *curLIT_SEG;    // current addition segment for SP
#endif
static  enum which_seg  inWhichSeg;     // which segment we're in
static  my_fixup        *curFixups;     // linked list of fixups for cur LEDATA
static  uint            numFixups;      // number of fixups in list

typedef struct {
    obj_ptr     base;                   // base in memory of the segment
    int         idx;                    // segment index
    uint        offset;                 // offset from beginning of segment
#if _TARGET == _8086
    unsigned_16 seg;                    // the segment
#endif
} seg_base;

#define SEGBASE_INC     128             // amount to increase sizeof segBases
static  seg_base        *segBase;       // sorted array of segment base info
static  uint            numSegBase;     // size of the above array
#define OFFS_INTO_SEG( __base_idx, __optr )     \
    ( (__optr) - segBase[__base_idx].base + segBase[__base_idx].offset )

#define FCODES_EXTERNAL         "__fcodes__"
#define FCODES_EXTERNAL_LEN     sizeof( FCODES_EXTERNAL ) - 1
#define DEBUG_FCODES_EXTERNAL   "__debug_fcodes__"
#define DEBUG_FCODES_LEN        sizeof( DEBUG_FCODES_EXTERNAL ) - 1

enum {
    GROUP_DGROUP = 1,
    GROUP_FLAT
};

// this enum and defaultLNAMES must be kept in sync
enum {
    LNAME_NULL = 1,
    LNAME_CODE,
    LNAME_DATA,
    LNAME_BSS,
#if _TARGET == _80386
    LNAME_FLAT,
#endif
    LNAME_DGROUP,
    LNAME__WFDATA_B,            // the triplet for global data
    LNAME__WFDATA,
    LNAME__WFDATA_E,
    LNAME__DATA_INIT_B,         // the triplet for data initializers
    LNAME__DATA_INIT,
    LNAME__DATA_INIT_E,
    LNAME__CB_DEFN_B,           // the triplet for common block defns
    LNAME__CB_DEFN,
    LNAME__CB_DEFN_E,
#if _TARGET == _8086
    LNAME_FCODE,
    LNAME_FGLOBAL,
    LNAME_FGLOBAL_END,
    LNAME_MAX = LNAME_FGLOBAL_END
#else
    LNAME_MAX = LNAME__CB_DEFN_E
#endif
};

#if _TARGET == _80386
  #define       _FCodeClass()   LNAME_DATA
  #define       _FCodeComb()    COMB_ADDOFF
  #define       _FGblStart()    COMB_ADDOFF
  #define       _FGblClass()    LNAME_BSS
  #define       _FGroupIdx()    GROUP_DGROUP
  #define       MAX_SEG         0xffffffff
#else
  #define       _FCodeClass()   LNAME_FCODE
  #define       _FCodeComb()    COMB_INVALID
  #define       _FGblStart()    COMB_COMMON
  #define       _FGblClass()    LNAME_FGLOBAL
  #define       _FGroupIdx()    0
  #define       MAX_SEG         0x10000L
#endif

static const char defaultLNAMES[] = {   // keep in sync with above enum
    0,
    4, 'C', 'O', 'D', 'E',
    4, 'D', 'A', 'T', 'A',
    3, 'B', 'S', 'S',
#if _TARGET == _80386
    4, 'F', 'L', 'A', 'T',
#endif
    6, 'D', 'G', 'R', 'O', 'U', 'P',
    9, '_', 'W', 'F', 'D', 'A', 'T', 'A', '_', 'B',
    7, '_', 'W', 'F', 'D', 'A', 'T', 'A',
    9, '_', 'W', 'F', 'D', 'A', 'T', 'A', '_', 'E',
    12, '_', 'D', 'A', 'T', 'A', '_', 'I', 'N', 'I', 'T', '_', 'B',
    10, '_', 'D', 'A', 'T', 'A', '_', 'I', 'N', 'I', 'T',
    12, '_', 'D', 'A', 'T', 'A', '_', 'I', 'N', 'I', 'T', '_', 'E',
    10, '_', 'C', 'B', '_', 'D', 'E', 'F', 'N', '_', 'B',
     8, '_', 'C', 'B', '_', 'D', 'E', 'F', 'N',
    10, '_', 'C', 'B', '_', 'D', 'E', 'F', 'N', '_', 'E'
#if _TARGET == _8086
   ,6, 'F', 'C', 'O', 'D', 'E', 'S',
    7, 'F', 'G', 'L', 'O', 'B', 'A', 'L',
    11, 'F', 'G', 'L', 'O', 'B', 'A', 'L', '_', 'E', 'N', 'D'
#endif
};

#define FMAIN_NAME              "FMAIN"
#define FMAIN_LEN               5
#define WFDATA_B_NAME           "__wfdata_start"
#define WFDATA_B_LEN            14
#define WFDATA_E_NAME           "__wfdata_end"
#define WFDATA_E_LEN            12

static const char fmainLNAMES[] = {
    10, 'F', 'M', 'A', 'I', 'N', '_', 'D', 'A', 'T', 'A'
};

#define OPTIONS_NAME            "LG@OPTIONS"
#define OPTIONS_LEN             10
// Marker for variable argument list functions
#define ELIPSE_MARKER           'X'
static const char optionsLNAMES[] = {
    8, '_', 'O', 'P', 'T', 'I', 'O', 'N', 'S'
};


static obj_rec *newDefnRec( uint_8 command ) {
//============================================

// create and link a new definition record
// includes THEADR, COMENT, LNAMES, SEGDEF, GRPDEF, EXTDEF

    obj_rec     *objr;

    objr = ObjNewRec( command );
    QEnqueue( &defnRecs, objr );
#if _TARGET == _8086
    objr->is_32 = 0;
#else
    objr->is_32 = 1;
#endif
    return( objr );
}


static  void    GObjSetLNAME( obj_rec *rec, char *name, size_t len, char *extn ) {
//================================================================================

// called when an LNAME is needed to be set with a designated extension

    char        buf[ 255 ];
    size_t      extlen;

    if( !rec || !name || !extn ) return;
    extlen = strlen( extn );
    if( len > ( 255 - extlen ) ) len = 255 - extlen;
    ObjAllocData( rec, len + extlen + 1 );
    memcpy( buf, name, len );
    memcpy( buf + len, extn, extlen );
    ObjPutName( rec, buf, len + extlen );
    ObjTruncRec( rec );
}


static obj_rec *newDataRec( uint_8 command ) {
//============================================

// create and link a new data record
// includes LEDATA, FIXUP, COMENT, PUBDEF

    obj_rec     *objr;

    objr = ObjNewRec( command );
    QEnqueue( &dataRecs, objr );
#if _TARGET == _8086
    objr->is_32 = 0;
#else
    objr->is_32 = 1;
#endif
    return( objr );
}


static obj_rec *newSEGDEF( int name, int class, int align, int comb ) {
//=====================================================================

    obj_rec     *segdef;

    /* now create the SEGDEF */
    segdef = newDefnRec( CMD_SEGDEF );
    ++segdefIdx;
    segdef->d.segdef.idx            = segdefIdx;
    segdef->d.segdef.align          = align;
    segdef->d.segdef.combine        = comb;
    segdef->d.segdef.seg_name_idx   = name;
    segdef->d.segdef.class_name_idx = class;
#if _TARGET != _8086
    if( comb != COMB_COMMON ) {
#endif
        if( class == LNAME_DATA || class == LNAME_BSS ) {
            ++numberOfSegsInDGROUP;
        }
#if _TARGET != _8086
    } else {
        /* for creating COMMON type segdefs */
        segdef->d.segdef.class_name_idx = name;
        ++numberOfSegsInDGROUP;
    }
#endif
    return( segdef );
}


static void buildGroups( void ) {
//===============================

    obj_rec     *dgroup;
    obj_rec     *objr;
    int         cbdefn_seg;
    obj_rec     *cbdefn_data;
    uint        num_cbdefn;     // number of cb_defns we've written
#if _TARGET == _80386
    obj_rec     *flat;
    static char flatData[] = { LNAME_FLAT };
#endif

#define CBDEFN_PER_LEDATA       (MAX_LEDATA_THRESHOLD/sizeof(cb_defn))

    cbdefn_data = NULL;
    // setup for cb_defn records
    if( numberOfCommonBlocks ) {
        objr = newSEGDEF( LNAME__CB_DEFN_B, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
        objr->d.segdef.seg_length = 0;
        objr = newSEGDEF( LNAME__CB_DEFN, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
        objr->d.segdef.seg_length = numberOfCommonBlocks*sizeof(cb_defn);
        cbdefn_seg = objr->d.segdef.idx;
        objr = newSEGDEF( LNAME__CB_DEFN_E, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
        objr->d.segdef.seg_length = 0;
        num_cbdefn = 0;
    }

    // create DGROUP
    dgroup = newDefnRec( CMD_GRPDEF );
    dgroup->d.grpdef.idx = GROUP_DGROUP;
    // we might need up to 3 bytes for each seg in dgroup and 1 byte for
    // the group name index
    ObjAllocData( dgroup, 1 + 3 * numberOfSegsInDGROUP );
    ObjPut8( dgroup, LNAME_DGROUP );

    // scan definition records for SEGDEFs
    objr = defnRecs.head;
    while( objr != NULL ) {
        if( objr->command == CMD_SEGDEF ) {
#if _TARGET == _8086
            if( ( objr->d.segdef.combine == COMB_COMMON ) &&
                ( objr->d.segdef.seg_name_idx == objr->d.segdef.class_name_idx ) ) {
#else
            if( objr->d.segdef.combine == COMB_COMMON ) {
#endif
                // create a cb_defn record

                my_fixup        *fix;
                uint            amt;
                void            *ptr;

                if( num_cbdefn % CBDEFN_PER_LEDATA == 0 ) {
                    // we need a new LEDATA
                    if( cbdefn_data ) {
                        ObjTruncRec( cbdefn_data );
                    }
                    cbdefn_data = newDataRec( CMD_LEDATA );
                    cbdefn_data->d.ledata.idx = cbdefn_seg;
                    cbdefn_data->d.ledata.offset = num_cbdefn*sizeof(cb_defn);
                    cbdefn_data->d.ledata.fixup = NULL;
                    amt = numberOfCommonBlocks - num_cbdefn;
                    if( amt > CBDEFN_PER_LEDATA ) {
                        amt = CBDEFN_PER_LEDATA;
                    }
                    cbdefn_data->d.ledata.num = amt;
                    ObjAllocData( cbdefn_data, amt * sizeof( cb_defn ) );
                }
                fix = FMemAlloc( sizeof( my_fixup ) );
                fix->next = cbdefn_data->d.ledata.fixup;
                cbdefn_data->d.ledata.fixup = fix;
                fix->idx = objr->d.segdef.idx;
                ptr = ObjGet( cbdefn_data, 0 );
                fix->value.offset = ( obj_ptr ) ptr;
                ObjPut32( cbdefn_data, 0 );                     // origin field
                ObjPut32( cbdefn_data, objr->d.segdef.seg_length ); // size fld
#if _TARGET == _8086
                // Adjust Segment size to max.
                if ( objr->d.segdef.seg_length > MAX_SEG ) {
                    objr->d.segdef.seg_length = MAX_SEG;
                }
                fix->value.seg = FP_SEG( ptr );
#endif
                ++num_cbdefn;
            }
            if( objr->d.segdef.class_name_idx == LNAME_DATA ||
                objr->d.segdef.class_name_idx == LNAME_BSS ||
#if _TARGET == _8086
                // We most definitely do not want it placed in the
                // DGROUP in the 16 bit model
                0 ) {
#else
                objr->d.segdef.combine == COMB_COMMON ) {
#endif
                // this segment belongs in DGROUP
                ObjPut8( dgroup, GRP_SEGIDX );
                ObjPutIndex( dgroup, objr->d.segdef.idx );
            }
        }
        objr = objr->next;
    }
    ObjTruncRec( dgroup );
    if( cbdefn_data ) {
        ObjTruncRec( cbdefn_data );
    }
#if _TARGET == _80386
    /* create the FLAT group */
    flat = newDefnRec( CMD_GRPDEF );
    dgroup->d.grpdef.idx = GROUP_FLAT;
    ObjAttachData( flat, flatData, sizeof( flatData ) );
#endif
}


static void finishQueues( void ) {
//================================

    obj_rec     *objr;
    default_lib *libs;
    char        *p;

    // emit some special records
    createEXTDEF( FCODES_EXTERNAL, FCODES_EXTERNAL_LEN );
    p = CharSetInfo.initializer;
    createEXTDEF( p, strlen( p ) );
    p = ErrorInitializer();
    createEXTDEF( p, strlen( p ) );

    for( libs = DefaultLibs; libs != NULL; libs = libs->link ) {
        objr = newDefnRec( CMD_COMENT );
        objr->d.coment.attr = 0x80;
        objr->d.coment.class = CMT_DEFAULT_LIBRARY;
        ObjAttachData( objr, libs->lib + 1, strlen( libs->lib + 1 ) );
    }

    // join the queues
    QJoinQueue( &defnRecs, &dataRecs );
    QInit( &dataRecs );

    // write the MODEND
    objr = ObjNewRec( CMD_MODEND );
    QEnqueue( &defnRecs, objr );
    objr->d.modend.main_module = 0;
}


static void processFixups( obj_rec *ledata ) {
//============================================

// given an ledata, create the fixup record for it

    my_fixup    *cur;
    uint        data_offs;
    uint_8      b;
    uint_8      b3;
    obj_rec     *fixrec;
    uint        written;
    obj_ptr     base;

    if( ledata->d.ledata.num == 0 ) return;     // no fixups
    fixrec = ObjNewRec( CMD_FIXUP );
#if _TARGET == _8086
    fixrec->is_32 = 0;
#else
    fixrec->is_32 = 1;
#endif
    cur = ledata->d.ledata.fixup;
    ObjAllocData( fixrec, 6 * ledata->d.ledata.num );
    written = 0;
    base = (obj_ptr)ObjGet( ledata, 0 );
    do {

#if _TARGET == _8086
        b = 0xc0 | ( LOC_BASE_OFFSET << 2 );
        b3 = ( FRAME_TARG << 4 );
#else
        b = 0xa4;
        b3 = ( FRAME_GRP << 4 );
#endif

        data_offs = cur->value.offset - base;
        if( cur->idx != 0 ) {
            if( cur->idx > 0 ) {
                b |= 0x40;
                b3 |= TARGET_SEG;
            } else {
                cur->idx = -cur->idx;
                // if it's not Ext_to_F77 then the relocation is seg relative
                if( cur->idx != EXTDEF_EXT_TO_F77 ) {
                    b |= 0x40;
                }
                b3 |= TARGET_EXT;
            }
            ObjPut8( fixrec, b | ( data_offs >> 8 ) );
            ObjPut8( fixrec, data_offs );
            ObjPut8( fixrec, b3 );

#if _TARGET == _80386
            ObjPutIndex( fixrec, GROUP_FLAT );
#endif
            ObjPutIndex( fixrec, cur->idx );
            ++written;
        }
        cur = FreeLink( cur );
    } while( cur != NULL );
    ledata->d.ledata.fixup = NULL;
    if( written ) {
        ObjTruncRec( fixrec );
        fixrec->next = ledata->next;    // insert record after ledata
        ledata->next = fixrec;
        ObjRSeek( fixrec, 0 );
    } else {
        // all the fixups had an index of 0... so don't output a fixup record
        ObjKillRec( fixrec );
    }
}


static obj_rec *freeObjRec( obj_rec *objr ) {
//===========================================

    obj_rec     *next;

    next = objr->next;
    if( objr->command == CMD_LEDATA && objr->d.ledata.fixup != NULL ) {
        FreeChain( &objr->d.ledata.fixup );
    }
    ObjKillRec( objr );
    return( next );
}


static void writeRecords( void ) {
//================================

    char        fn_buff[ MAX_FILE ];

    MakeName( SDFName( SrcName ), ObjExtn, fn_buff );
    pobjState.file_out = ObjWriteOpen( fn_buff );
    if( pobjState.file_out == NULL ) {
        InfoError( SM_OPENING_FILE, fn_buff, strerror( errno ) );
        unlink( fn_buff );
        Suicide();
    }
    while( defnRecs.head != NULL ) {
        ObjRSeek( (obj_rec *)defnRecs.head, 0 );
        if( ((obj_rec *)defnRecs.head)->command == CMD_LEDATA ) {
            processFixups( defnRecs.head );
        }
        jumpTable[ JUMP_OFFSET( ((obj_rec *)defnRecs.head)->command ) ]
                                        ( defnRecs.head, &pobjState );
        defnRecs.head = freeObjRec( defnRecs.head );
    }
    ObjWriteClose( pobjState.file_out );
}


static int createEXTDEF( const char *name, size_t len ) {
//=======================================================

    obj_rec     *objr;

    objr = newDefnRec( CMD_EXTDEF );
    ObjAllocData( objr, len + 2 );
    ObjPutName( objr, name, len );
    ObjPut8( objr, 0 );
    ObjTruncRec( objr );
    objr->d.extdef.num_names = 1;
    ++extdefIdx;
    objr->d.extdef.first_idx = extdefIdx;
    return( extdefIdx );
}


static obj_rec *newLNAMES( int num ) {
//====================================

// create an LNAMES record, and enqueue it, with room for num LNAMES in it

    obj_rec     *lnames;

    lnames = newDefnRec( CMD_LNAMES );
    lnames->d.lnames.first_idx = lnamesIdx + 1;
    lnames->d.lnames.num_names = num;
    lnamesIdx += num;
    return( lnames );
}


static obj_rec *newPUBDEF( const char *name, size_t len, int grp,
                                                int seg, unsigned_32 offs ) {
//===========================================================================

    obj_rec     *pubdef;

    pubdef = newDataRec( CMD_PUBDEF );
#if _TARGET == _8086
    pubdef->is_32 = 0;
#else
    pubdef->is_32 = 1;
#endif

    /* need 1 byte for dgroup index, 2 bytes for seg index,
        1 byte for name len, len bytes for name, 4 or 2 bytes for
        public offset, 1 byte for type index */
#if _TARGET == _8086
    ObjAllocData( pubdef, len + ( 1 + 2 + 1 + 2 + 1 ) );
#else
    ObjAllocData( pubdef, len + ( 1 + 2 + 1 + 4 + 1 ) );
#endif
    ObjPutIndex( pubdef, grp );
    ObjPutIndex( pubdef, seg );
    ObjPutName( pubdef, name, len );
#if _TARGET == _8086
    ObjPut16( pubdef, ( unsigned_16 )offs );    /* public offset */
#else
    ObjPut32( pubdef, offs );   /* public offset */
#endif
    ObjPut8( pubdef, 0 );       /* type index */
    ObjTruncRec( pubdef );
    return( pubdef );
}


/* these routines are part of the interface to the WOMP routines */

void PObjRegList( const pobj_list *list, size_t len ) {
//=====================================================

    size_t  i;

    for( i = 0; i < len; ++i ) {
        jumpTable[ JUMP_OFFSET( list[i].command ) ] = list[i].func;
    }
}


void PObjUnRegList( const pobj_list *list, size_t len ) {
//=======================================================

    list = list;
    len = len;
}


void    GObjInit( void ) {
//========================

    obj_rec     *objr;
    size_t      len;

    QInit( &defnRecs );
    QInit( &dataRecs );
    segdefIdx = 0;
    grpdefIdx = 0;
    extdefIdx = 0;
    lnamesIdx = 0;
    numberOfSegsInDGROUP = 0;
    numberOfCommonBlocks = 0;
    curFixups = NULL;
    numFixups = 0;
    GOFixSetData( &baseOfLEDATA, &curFixups, &numFixups );
    segBase = FMemAlloc( sizeof( *segBase ) * SEGBASE_INC );
    numSegBase = 0;

    ObjRecInit();
    pobjState.pass = POBJ_WRITE_PASS;
    GenMSOmfInit();

    /* write the THEADR */
    objr = newDefnRec( CMD_THEADR );
    len = strlen( SrcName );
    ObjAllocData( objr, len + 1 );
    ObjPutName( objr, SrcName, len );
    ObjTruncRec( objr );

    /* write the OS220 COMENT */
    objr = newDefnRec( CMD_COMENT );
    objr->d.coment.attr = 0x80;
    objr->d.coment.class = 0xaa;
    ObjAttachData( objr, "OS220", 5 );

    /* write the MS_OMF COMENT */
    objr = newDefnRec( CMD_COMENT );
    objr->d.coment.attr = 0x00;
    objr->d.coment.class = CMT_MS_OMF;

    /* write some LNAMES */
    objr = newLNAMES( LNAME_MAX );
    ObjAttachData( objr, (uint_8 *)defaultLNAMES, sizeof( defaultLNAMES ) );

    /* emit the extdef for Ext_to_F77 */
                 //1234567890
    createEXTDEF( "Ext_to_F77", 10 );
#if _TARGET == _8086
    litSegs = 0;
    gblSegs = 0;

    /* write the pack data = 0 COMENT */
    objr = newDefnRec( CMD_COMENT );
    objr->d.coment.attr = 0;
    objr->d.coment.class = CMT_LINKER_DIRECTIVE;
    ObjAttachData( objr, "7\0\0\0\0", 5 );
#endif
}


static void nextSegBase( int seg_idx, uint offset ) {
//===================================================

// ObjPtr is the base of the next segment

#if _TARGET == _80386
    baseOfLEDATA = ObjPtr;
#else
    baseOfLEDATA = 0;
#endif
    ++numSegBase;
    if( numSegBase % SEGBASE_INC == 1 && numSegBase > 1 ) {
        // we've run out of room in our segBase array
        seg_base        *new;

        new = FMemAlloc( sizeof( *segBase ) * ( numSegBase + (SEGBASE_INC-1) ) );
        memcpy( new, segBase, sizeof( *segBase ) * ( numSegBase - 1 ) );
        FMemFree( segBase );
        segBase = new;
    }
#if _TARGET == _80386
    segBase[ numSegBase - 1 ].base = ObjPtr;
#else
    segBase[ numSegBase - 1 ].base = 0;
    segBase[ numSegBase - 1 ].seg = CurrSeg;
#endif
    segBase[ numSegBase - 1 ].idx = seg_idx;
    segBase[ numSegBase - 1 ].offset = offset;
    segBase[ numSegBase ].base = ~0UL;          // sentinal
}


static void GObjCreateSegStruct( obj_rec **lnamerec, int num,
                                 obj_rec **segrec, int type, int seg ) {
//======================================================================

    int         combination;

    /* first create an LNAMES */
    *lnamerec = newLNAMES( num );

    /* now create the SEGDEFs */
    switch ( seg ) {
    case IN_COMMON_BLOCK:
        combination = COMB_COMMON;
        break;
    case IN_SUBPROGRAM:
    case IN_GLOBAL_DATA:
    default:
        combination = _FCodeComb();
        break;
    }

    *segrec = newSEGDEF( ( *lnamerec )->d.lnames.first_idx, type,
                           SEGDEF_ALIGN_PARA, combination );

    nextSegBase( ( *segrec )->d.segdef.idx, 0 );
    inWhichSeg = seg;
}


void GObjSubInit( void ) {
//========================

#if _TARGET == _8086
    litSegs = 0;
#endif
    GObjCreateSegStruct( &curSP_LNAMES, 1, &curSP_SEG, _FCodeClass(),
                        IN_SUBPROGRAM );
}


static void fillLEDATA( void ) {
//==============================

// make LEDATAs for data from baseOfLEDATA to ObjPtr

    unsigned_32         len;
    obj_rec             *ledata;

    if( inWhichSeg == IN_GLOBAL_DATA || inWhichSeg == IN_COMMON_BLOCK ) {
        // We never emit any LEDATAs for global data or common block
        // segments.
        return;
    }
    while( baseOfLEDATA < ObjPtr ) {
        ledata = newDataRec( CMD_LEDATA );
        len = ObjPtr - baseOfLEDATA;
        if( len > MAX_LEDATA_THRESHOLD ) {
            len = MAX_LEDATA_THRESHOLD;
        }
        ObjAttachData( ledata, ( void PGM * )MK_OBJPTR( baseOfLEDATA ), len );
        ledata->d.ledata.idx = segdefIdx;
        ledata->d.ledata.offset = OFFS_INTO_SEG( numSegBase-1, baseOfLEDATA );
        ledata->d.ledata.fixup = curFixups;
        ledata->d.ledata.num = numFixups;
        numFixups = 0;
        curFixups = 0;
        baseOfLEDATA += len;
    }
}


void GObjSubFini( void ) {
//========================

#if _TARGET == _80386
    fillLEDATA();

    // calculate the size of the data segment
    curSP_SEG->d.segdef.seg_length = ObjPtr - segBase[ numSegBase - 1 ].base;
#else
    if ( GObjExistLiteralSegment() ) {
        fillLEDATA();
        GObjSetLiteralSegmentLen();
    }
#endif
}

void    GObjFlushLEDATA( void ) {
//===============================

    fillLEDATA();
}


#if _TARGET == _8086

void    GObjEndOfSPSeg( void ) {
//==============================

    curSP_SEG->d.segdef.seg_length = ObjPtr - segBase[ numSegBase - 1 ].base;
    endOfSPSeg = ObjPtr;
}



void    GObjSetLiteralSegmentLen( void ) {
//========================================

    curLIT_SEG->d.segdef.seg_length = ObjPtr - segBase[ numSegBase - 1 ].base;
}


void    GObjSetGlobalSegmentLen( void ) {
//=======================================

    dataSEGDEF->d.segdef.seg_length = gblPtr;
}


void    GObjSetCommonSegmentLen( void ) {
//=======================================

    commonSEGDEF->d.segdef.seg_length = gblPtr;
}


void    GObjSetCommonBlockLen( void ) {
//=====================================

    commonBlockDef->d.segdef.seg_length = commonBlockSize;
}


void    GObjSetGlobalSegFull( void ) {
//====================================

    dataSEGDEF->d.segdef.seg_length = MAX_SEG;
}


void    GObjSetCommonSegFull( void ) {
//====================================

    commonSEGDEF->d.segdef.seg_length = MAX_SEG;
}


void    GObjSetCommonBlockSize( unsigned_32 size ) {
//==================================================

// Set the common block size

    commonBlockSize = size;
}


unsigned long   GObjGetCommonBlockSize( void ) {
//==============================================

// Get the common block size

    return( commonBlockSize );
}


uint    GObjCalculateNumOfGblSegs( unsigned long size ) {
//=======================================================

// Calculate the number of segments to create for datasize

    return( ( ( size - ( MAX_SEG - gblPtr ) ) / MAX_SEG ) + 1 );
}


unsigned long   GObjCalculatePrevSize( unsigned long offset ) {
//=============================================================

// Calculate the size of the previous item based on the current
// offset, ( its end ).

    return( offset - ( ( comSegs * MAX_SEG ) + gblPtr ) );
}


uint    GObjGetGlobalVirtualPtr( void ) {
//=======================================

    return( gblPtr );
}


void    GObjSetGlobalVirtualPtr( uint size ) {
//============================================

    gblPtr = size;
}


void    GObjIncGlobalVirtualPtr( uint size ) {
//============================================

    gblPtr += size;
}


int     GObjGlobalPackedSegment( uint size ) {
//============================================

// If segment is packed, return true.

     return( ( ( size + gblPtr ) < gblPtr ) || ( size + gblPtr ) > MAX_SEG );
}


int     GObjPackedSegment( uint size ) {
//======================================

// If segment is packed, return true.
     return( ( ( size + ObjPtr ) < ObjPtr ) || ( size + ObjPtr ) > MAX_SEG );
}


uint    GObjExistLiteralSegment( void ) {
//=======================================

// If literal segment exists, return true.

     return( litSegs );
}


void    GObjMakeNewLiteralSeg() {
//===============================

    char        root_ext[15];
    obj_rec     *litnames;

    litSegs++;
    MsgFormat( "_LITERAL%d1", root_ext, litSegs );
    GObjCreateSegStruct( &litnames, 1, &curLIT_SEG, _FCodeClass(),
                                IN_SUBPROGRAM );
    GObjSetLNAME( litnames, SubProgId->ns.name, SubProgId->ns.name_len,
                        root_ext );
}


void    GObjMakeNewGlobalSeg() {
//==============================

    char        root_ext[15];
    obj_rec     *gbllnames;

    gblSegs++;
    GObjSetGlobalVirtualPtr( 0 );
    MsgFormat( "_@%d1", root_ext, gblSegs );
    GObjCreateSegStruct( &gbllnames, 1, &dataSEGDEF, _FGblClass(),
                        IN_GLOBAL_DATA );
    GObjSetLNAME( gbllnames, "_WFDATA", 7, root_ext );
}


void    GObjMakeNewCommonSeg( void ) {
//====================================

    char        root_ext[15];
    obj_rec     *common_names;

    comSegs++;
    GObjSetGlobalVirtualPtr( 0 );
    MsgFormat( "_@%d1", root_ext, comSegs );
    GObjCreateSegStruct( &common_names, 1, &commonSEGDEF,
                        commonClassLNAME->d.lnames.first_idx,
                        IN_COMMON_BLOCK );
    GObjSetLNAME( common_names, commonName, commonNameLen, root_ext );
}

#endif


int     GObjFindSeg( targ_addr addr ) {
//=====================================

// Find which seg addr is in; but note that we don't return the seg index,
// we return an index into the segBase array.

    int         low;
    int         mid;
    int         high;

    // check if it's the current segment first
#if _TARGET == _8086
    if( addr.seg >= segBase[ numSegBase - 1 ].seg ) {
        return( numSegBase - 1 );
    }
#else
    if( addr.offset >= segBase[ numSegBase - 1 ].base ) {
        return( numSegBase - 1 );
    }
#endif
    low = 0;
    high = numSegBase - 2;
    // this loop will always terminate if a valid addr is passed in
    // because the addr must appear in one of the segments
    for(;;) {
        mid = low + ( high - low ) / 2;
#if _TARGET == _8086
        if( segBase[ mid ].seg <= addr.seg &&
            addr.seg < segBase[ mid+1 ].seg ) break;
        if( addr.seg < segBase[ mid ].seg ) {
#else
        if( segBase[ mid ].base <= addr.offset &&
            addr.offset < segBase[ mid+1 ].base ) break;
        if( addr.offset < segBase[ mid ].base ) {
#endif
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    // Note that the above search guarantees that if
    // segBase[ n ].base == segBase[ n+1 ].base, then mid >= n+1.
    // this is required in case a COMMON block is at the very beginning of
    // the global data area.
    return( mid );
}


static void doDataInit( void ) {
//==============================

// we handle DataChain in a manner similar to CLIB's initializer/finalizers
    obj_rec     *seg;
    obj_rec     *ledata;
    int         chain_idx;
    my_fixup    *fix;
    char        *ptr;

    if( DataChain.offset == 0 ) return; // no data statements

    chain_idx = GObjFindSeg( DataChain );

    // get a data record to do the fixup on
    ledata = newDataRec( CMD_LEDATA );
    ledata->d.ledata.offset = 0;
    ObjAllocData( ledata, sizeof( targ_addr ) );
    ptr = ObjGet( ledata, 0 );
    ObjPut32( ledata, OFFS_INTO_SEG( chain_idx, DataChain.offset ) );

    // define our segments
    seg = newSEGDEF( LNAME__DATA_INIT_B, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
    seg->d.segdef.seg_length = 0;
    seg = newSEGDEF( LNAME__DATA_INIT, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
    seg->d.segdef.seg_length = sizeof( targ_addr );
    ledata->d.ledata.idx = seg->d.segdef.idx;
    seg = newSEGDEF( LNAME__DATA_INIT_E, LNAME_DATA,
                                            SEGDEF_ALIGN_DWORD, COMB_ADDOFF );
    seg->d.segdef.seg_length = 0;

    // create the fixup
    fix = FMemAlloc( sizeof( my_fixup ) );
    fix->next = NULL;
    fix->value.offset = ( obj_ptr )ptr;
#if _TARGET == _8086
    fix->value.seg = FP_SEG( ptr );
#endif
    fix->idx = segBase[ chain_idx ].idx;
    ledata->d.ledata.fixup = fix;
    ledata->d.ledata.num = 1;
}


static void freeMemory( void ) {
//==============================

    if( curFixups != NULL ) {
        FreeChain( &curFixups );
    }
    while( defnRecs.head ) {
        defnRecs.head = freeObjRec( defnRecs.head );
    }
    while( dataRecs.head ) {
        dataRecs.head = freeObjRec( dataRecs.head );
    }
    if( segBase != NULL ) {
        FMemFree( segBase );
    }
}


void GObjFini( void ) {
//=====================

    if( ( ProgSw & PS_ERROR ) == 0 ) {
#if _TARGET == _8086
        GObjSetGlobalSegmentLen();
        GObjEndGD();
#else
        dataSEGDEF->d.segdef.seg_length += ObjPtr - segBase[ numSegBase - 1 ].base;
#endif

        // handle the DataChain
        doDataInit();
        FMemFree( segBase );
        segBase = NULL;

        buildGroups();
        finishQueues();
        if( Spawn( writeRecords ) ) {
            if( pobjState.file_out != NULL ) {
                ObjWriteClose( pobjState.file_out );
            }
            freeMemory();
        }
    } else {
        freeMemory();
    }
    ObjRecFini();
    GenMSOmfFini();
}


void GObjStartLD( void ) {
//==========================

// called when the code for the SP is done. i.e., just before the data for
// the SP is begun

    // get the name for our subprogram segdefs
    if( ( SubProgId->ns.flags & SY_SUBPROG_TYPE ) == SY_PROGRAM ) {
        ObjAttachData( curSP_LNAMES, (uint_8 *)fmainLNAMES,
                        sizeof(fmainLNAMES));
    } else {
        GObjSetLNAME( curSP_LNAMES, SubProgId->ns.name,
                        SubProgId->ns.name_len, "_DATA" );
    }
}


void GObjStartGD( void ) {
//========================

// begin emitting data into the global data segment

    obj_rec     *seg;

    seg = newSEGDEF( LNAME__WFDATA_B, _FGblClass(),
                                            SEGDEF_ALIGN_PARA, _FGblStart() );
    seg->d.segdef.seg_length = 0;
#if _TARGET == _8086
    newPUBDEF( WFDATA_B_NAME, WFDATA_B_LEN, 0, segdefIdx, 0 );
#endif

    dataSEGDEF = newSEGDEF( LNAME__WFDATA, _FGblClass(),
                                            SEGDEF_ALIGN_PARA, _FCodeComb() );
    dataSEGDEF->d.segdef.seg_length = 0;
    nextSegBase( dataSEGDEF->d.segdef.idx, 0 );

#if _TARGET == _80386
    seg = newSEGDEF( LNAME__WFDATA_E, LNAME_BSS,
                                            SEGDEF_ALIGN_BYTE, COMB_ADDOFF );
    seg->d.segdef.seg_length = 0;
#else
    GObjSetGlobalVirtualPtr( ObjPtr );
#endif

    inWhichSeg = IN_GLOBAL_DATA;
}


#if _TARGET == _8086

void    GObjEndGD( void ) {
//=========================

// end emitting data into the global data segment

    obj_rec     *seg;

    seg = newSEGDEF( LNAME__WFDATA_E, LNAME_FGLOBAL_END,
                                            SEGDEF_ALIGN_BYTE, COMB_COMMON );
    seg->d.segdef.seg_length = 0;
    newPUBDEF( WFDATA_E_NAME, WFDATA_E_LEN, 0, segdefIdx, 0 );
}

#endif

#if _TARGET == _80386

obj_ptr GObjPutPtr( obj_ptr ptr, obj_ptr val, int base_idx ) {
//============================================================

// Exchange pointer in object memory.

    obj_ptr             PGM *optr;
    obj_ptr             old_val;
    my_fixup            *fix;

    optr = ( obj_ptr PGM * )MK_OBJPTR( ptr );
    fix = *(my_fixup * PGM *)optr;
    if( !fix ) return( 0 );
    old_val = fix->value.offset;
    fix->value.offset = ptr;
    fix->idx = segBase[base_idx].idx;
    *( unsigned_32 *)optr = 0L;
    *optr = OFFS_INTO_SEG( base_idx, val );
    return( old_val );
}

#endif

void GObjPutLink( targ_addr *tloc, targ_addr *tlink ) {
//=====================================================

// link a value into a relocation chain

    void PGM * PGM      *ptr;
    void PGM *          *loc;
    void PGM *          *link;
    my_fixup            *fix;

    loc = (void PGM **)tloc;
    link = (void PGM **)tlink;
    ptr = *loc;
    fix = (my_fixup *)*ptr;
    *tloc = fix->value;
    fix->value = *( targ_addr * )link;
}


void GObjPutAddr( targ_addr *tloc, targ_addr *tlink, int base_idx ) {
//===================================================================

// If base_idx >= 0 then it refers to an entry in segBase.  Otherwise it
// is the negative of an EXTDEF index.

    void PGM * PGM      *ptr;
    void PGM *          *loc;
    void PGM *          *link;
    my_fixup            *fix;

    loc = (void PGM **)tloc;
    link = (void PGM **)tlink;
    ptr = *loc;
    fix = (my_fixup *)*ptr;
    *tloc = fix->value;
    fix->value.offset = ( obj_ptr )ptr;
#if _TARGET == _8086
    fix->value.seg = FP_SEG( ptr );
#endif
    if( base_idx >= 0 ) {
        fix->idx = segBase[ base_idx ].idx;
        *ptr = (void *)OFFS_INTO_SEG( base_idx, (obj_ptr)*link );
    } else {
        fix->idx = base_idx;
        *ptr = *link;
    }
#if _TARGET == _8086
// we only want the offset
    *(unsigned_32 *)ptr &= 0xffff;
#endif
}


void    GObjNoReloc( targ_addr loc ) {
//====================================

// the value at loc is not to be relocated; it should be fix->value

    my_fixup            *fix;
    void                PGM *optr;

#if _TARGET == _8086
    optr = MK_FP( loc.seg, loc.offset );
#else
    optr = (void *)loc.offset;
#endif
    fix = *(my_fixup **)optr;
    *( unsigned_32 *)optr = ( unsigned_32 )fix->value.offset;
    fix->idx = 0;               // indicate null fixup
}


void Sym2Obj( sym_id sym ) {
//==========================

    int         base_idx;
    int         flags;

    flags = sym->ns.flags;
    if( ( flags & SY_CLASS ) == SY_COMMON ) {
        // We've already created the PUBDEF for the common block, here we
        // just calc the block's size and restart the global data area.
        fillLEDATA();
        // This operation is done here for the 386 case, but is done
        // in CloseCommonBlock() ( tdreloc.c ) for the 8086 case
#if _TARGET == _80386
        commonSEGDEF->d.segdef.seg_length = ObjPtr - segBase[numSegBase-1].base;
        nextSegBase(dataSEGDEF->d.segdef.idx, dataSEGDEF->d.segdef.seg_length);
        inWhichSeg = IN_GLOBAL_DATA;
#endif
        return;
    }
    if( flags & SY_ADDR_ASSIGNED ) {
        // create a pubdef for this symbol
        base_idx = GObjFindSeg( sym->ns.address.ga );
        if( ( flags & SY_CLASS ) == SY_SUBPROGRAM &&
            ( flags & SY_SUBPROG_TYPE ) == SY_PROGRAM ) {
            obj_rec     *objr;
            obj_rec     *seg;

            newPUBDEF( FMAIN_NAME, FMAIN_LEN, 0, segBase[base_idx].idx,
                OFFS_INTO_SEG( base_idx, sym->ns.address.ga.offset ) );
            // now we process Options
            if( Options & OPT_DEBUG ) {
                // emit EXTDEF to haul in the debugger
                createEXTDEF( DEBUG_FCODES_EXTERNAL, DEBUG_FCODES_LEN );
            }
            objr = newLNAMES( 1 );
            ObjAttachData( objr, (uint_8 *)optionsLNAMES,sizeof(optionsLNAMES));
            seg = newSEGDEF( objr->d.lnames.first_idx, _FCodeClass(),
                                            SEGDEF_ALIGN_BYTE, _FCodeComb() );
            seg->d.segdef.seg_length = sizeof( sa_options );
            objr = newDataRec( CMD_LEDATA );
            objr->d.ledata.offset = 0;
            objr->d.ledata.idx = seg->d.segdef.idx;
            objr->d.ledata.fixup = NULL;
            objr->d.ledata.num = 0;
            ObjAllocData( objr, sizeof( sa_options ) );
            ObjPut32( objr, Options );
            ObjPut32( objr, StmtLimit );
#if _TARGET == _8086
            ObjPut16( objr, PageLimit );
            ObjPut16( objr, TimeLimit );
#else
            ObjPut32( objr, PageLimit );
            ObjPut32( objr, TimeLimit );
#endif
            ObjPut8( objr, MathOptions );
            ObjTruncRec( objr );
            newPUBDEF( OPTIONS_NAME, OPTIONS_LEN, 0, seg->d.segdef.idx, 0 );
        } else {
            newPUBDEF( SymBuff, BldObjName( sym, SymBuff, MAX_SYMLEN ),
                0, segBase[base_idx].idx,
                OFFS_INTO_SEG( base_idx, sym->ns.address.ga.offset ) );
        }
        return;
    }
    if( flags & SY_INTRINSIC ) {
        char    *buf;
        char    *p;
        size_t  len;

        len = strlen( IFNames[ sym->ns.si.fi.index ] );
        buf = alloca( 5 + len );
        p = buf + 3;
        buf[0] = 'L'; buf[1] = 'G'; buf[2] = '@';
        if( ( IFArgCt[ sym->ns.si.fi.index ] & IF_COUNT_MASK ) ==
            TWO_OR_MORE ) {
            *p++ = ELIPSE_MARKER;
        }
        memcpy( p, IFNames[ sym->ns.si.fi.index ], len );
        p += len;
        buf[ p-buf ] = '\0';
        sym->ns.address.la = createEXTDEF( buf, p - buf );
        return;
    }
    if( sym->ns.flags & SY_EXTERNAL ) {
        if( sym->ns.reloc_chain.gr.offset == 0 ) {
            if( sym->ns.flags & SY_RELAX_EXTERN ) {
                return;
            }
        }
    }
    // create an extdef for this symbol and store the index
    sym->ns.address.la = createEXTDEF( SymBuff,
                                BldObjName( sym, SymBuff, MAX_SYMLEN ) );
}


void GObjRelocExt( sym_id ste_ptr ) {
//===================================

    targ_addr   where;
    targ_addr   cur;
    int         ext;

    if( ( ProgSw & PS_ERROR ) == 0 ) {
        ext = -ste_ptr->ns.address.la;
        where.offset = 0;
        cur = ste_ptr->ns.reloc_chain.gr;
        while( cur.offset != 0 ) {
            GObjPutAddr( &cur, &where, ext );
        }
    }
}


void GObjComBlk( sym_id gbl ) {
//=============================

    obj_rec     *lnames;

    // create the LNAMES record for it
    lnames = newLNAMES( 1 );
    ObjAllocData( lnames, gbl->ns.name_len + 1 );
    ObjPutName( lnames, gbl->ns.name, gbl->ns.name_len );
#if _TARGET == _8086
    commonName = gbl->ns.name;
    commonNameLen = gbl->ns.name_len;
    commonClassLNAME = lnames;
#endif

    // close up the last ledata and adjust global data size
    fillLEDATA();
#if _TARGET == _8086
    GObjSetGlobalSegmentLen();
    comSegs = 0;
    GObjSetGlobalVirtualPtr( 0 );
#else
    dataSEGDEF->d.segdef.seg_length += ObjPtr - segBase[ numSegBase - 1 ].base;
#endif

    // create the segdef
    commonSEGDEF = newSEGDEF( lnames->d.lnames.first_idx,
                                lnames->d.lnames.first_idx,
                                SEGDEF_ALIGN_PARA, COMB_COMMON );
    nextSegBase( commonSEGDEF->d.segdef.idx, 0 );
#if _TARGET == _8086
    // We need to know the start of the common block so that we
    // can adjust its size later on.  This is because
    // commonSEGDEF will always point to the current segment of
    // the current common  block, since we can have multiple
    // segments for 1 common block in the 8086 model
    // we must know which is the first common block.
    commonBlockDef = commonSEGDEF;
#endif

    // create a PUBDEF so that C programs can access the common block
    newPUBDEF( gbl->ns.name, gbl->ns.name_len, _FGroupIdx(),
            commonSEGDEF->d.segdef.idx, 0 );

    ++numberOfCommonBlocks;
    inWhichSeg = IN_COMMON_BLOCK;
}


void GObjError( void ) {
//======================

    char        fn_buff[MAX_FILE];

    if( ProgSw & PS_ERROR ) return;
    Options |= OPT_TERM;
    TermCursor = 0;
    MakeName( SDFName( SrcName ), ObjExtn, fn_buff );
    InfoError( SM_IO_WRITE_ERR, fn_buff, strerror( errno ) );
    unlink( fn_buff );
    Suicide();
}
