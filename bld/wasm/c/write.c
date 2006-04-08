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


#include "asmglob.h"
#include <errno.h>
#include <ctype.h>
#include <time.h>

#include "asmsym.h"
#include "asmins.h"
#include "asmalloc.h"
#include "fatal.h"
#include "asmdefs.h"
#include "asmeval.h"
#include "objio.h"
#include "objprs.h"
#include "fixup.h"
#include "autodept.h"
#include "mangle.h"
#include "namemgr.h"  // WOMP callback NameGet routine declaration
#include "directiv.h"
#include "queues.h"
#include "womputil.h"
#include "asmlabel.h"

#include "myassert.h"


// use separate fixupp and fixupp32 records
// fixupp32 record is used only for FIX_OFFSET386 and FIX_POINTER386 fixup
// it is for better compatibility with MASM
#define SEPARATE_FIXUPP_16_32 1

extern char             *ScanLine( char *, int );
extern void             FreeIncludePath( void );
extern void             CheckForOpenConditionals();
extern bool             PopLineQueue();
extern void             set_cpu_parameters( void );
extern void             set_fpu_parameters( void );
extern void             CheckProcOpen( void );

extern symbol_queue     Tables[];       // tables of definitions
extern struct fixup     *FixupListHead; // head of list of fixups ( from WOMP )
extern struct fixup     *FixupListTail;
extern uint_32          BufSize;
extern obj_rec          *ModendRec;     // Record for Modend

extern int              MacroExitState;
extern int              in_prologue;

int                     MacroLocalVarCounter = 0; // counter for temp. var names
char                    Parse_Pass;     // phase of parsing
char                    write_to_file;  // write if there is no error
uint                    LineNumber;
char                    Modend;         // end of module is reached
int_8                   DefineProc;     // TRUE if the definition of procedure
                                        // has not ended
dir_node                *CurrProc;      // current procedure
int_8                   Use32;          // if 32-bit code is use
unsigned long           PassTotal;      // Total number of ledata bytes generated
int_8                   PhaseError;
char                    EndDirectiveFound = FALSE;

extern uint             segdefidx;      // Number of Segment definition
extern uint             extdefidx;      // Number of Extern definition

static char             **NameArray;

typedef struct  fname_list {
        struct  fname_list *next;
        time_t  mtime;
        char    *name;
} FNAME, *FNAMEPTR;

global_vars     Globals = { 0, 0, 0, 0, 0, 0, 0 };

static FNAMEPTR FNames = NULL;
static uint     lastLineNumber;

void AddFlist( char const *filename )
/***********************************/
{
    FNAMEPTR    flist;
    FNAMEPTR    last;
    int         index;
    char        *fname;
    char        buff[2*_MAX_PATH];
        
    if( !Options.emit_dependencies )
        return;

    index = 0;
    last = FNames;
    for( flist = last; flist != NULL; flist = flist->next ) {
        if( strcmp( filename, flist->name ) == 0 )
            return;
        index++;
        last = flist;
    }
    fname = _getFilenameFullPath( buff, filename, sizeof(buff) );
    flist = (FNAMEPTR)AsmAlloc( sizeof( FNAME ) );
    flist->name = (char *)AsmAlloc( strlen( fname ) + 1 );
    strcpy( flist->name, fname );
    flist->mtime = _getFilenameTimeStamp( fname );
    flist->next = NULL;
    if( FNames == NULL ) {
        FNames = flist;
    } else {
        last->next = flist;
    }
    return;
}

static void FreeFlist( void )
/***************************/
{
    FNAMEPTR      curr;
    FNAMEPTR      last;

    if( !Options.emit_dependencies )
        return;

    for( curr = FNames; curr != NULL; ) {
        AsmFree(curr->name);
        last = curr;
        curr = curr->next;
        AsmFree(last);
    }
    FNames = NULL;
    return;
}

static void write_init( void )
/****************************/
{
    BufSize     = 0;
    FixupListHead = NULL;
    FixupListTail = NULL;
    ModendRec   = NULL;
    CurrProc    = NULL;
    DefineProc  = FALSE;
    Use32       = FALSE;
    write_to_file = TRUE;

    IdxInit();
    LnameInsert( "" );

    ModuleInit();
    FixInit();
}

static void write_fini( void )
/****************************/
{
    FixFini();
}

void OutSelect( bool starts )
/***************************/
{
    obj_rec             *objr;
    unsigned long       curr;

    if( starts ) {
        if( !Options.output_comment_data_in_code_records || Globals.data_in_code 
            || !Globals.code_seg )
            return;
        Globals.sel_start = GetCurrAddr();
        Globals.data_in_code = TRUE;
    } else {
        if( !Options.output_comment_data_in_code_records || !Globals.data_in_code )
            return;
        Globals.sel_idx = GetSegIdx( &CurrSeg->seg->sym );
        Globals.data_in_code = FALSE;

        if( (Parse_Pass > PASS_1) && !PhaseError ) {
            objr = ObjNewRec( CMD_COMENT );
            objr->d.coment.attr = 0x80;
            objr->d.coment.class = CMT_DISASM_DIRECTIVE;

            ObjAllocData( objr, 11 );
            curr = GetCurrAddr();
            if( (Globals.sel_start > 0xffffUL) || (curr > 0xffffUL) ) {
                ObjPut8( objr, DDIR_SCAN_TABLE_32 );
                ObjPutIndex( objr, Globals.sel_idx );
                ObjPut32( objr, Globals.sel_start );
                ObjPut32( objr, GetCurrAddr() );
            } else {
                ObjPut8( objr, DDIR_SCAN_TABLE );
                ObjPutIndex( objr, Globals.sel_idx );
                ObjPut16( objr, Globals.sel_start );
                ObjPut16( objr, GetCurrAddr() );
            }
            ObjTruncRec( objr );
            write_record( objr, TRUE );
        }
        Globals.sel_idx = 0;
        Globals.sel_start = 0;
    }
}

static void write_end_of_pass1( void )
/************************************/
{
    obj_rec     *objr;

    objr = ObjNewRec( CMD_COMENT );
    objr->d.coment.attr = 0x00;
    objr->d.coment.class = CMT_MS_END_PASS_1;
    ObjAttachData( objr, "\x001", 1 );
    write_record( objr, TRUE );
}

static void write_dosseg( void )
/******************************/
{
    obj_rec     *objr;

    objr = ObjNewRec( CMD_COMENT );
    objr->d.coment.attr = 0x80;
    objr->d.coment.class = CMT_DOSSEG;
    ObjAttachData( objr, "", 0 );
    write_record( objr, TRUE );
}

static void write_lib( void )
/***************************/
{
    obj_rec             *objr;
    struct dir_node     *curr;
    char                *name;

    for( curr = Tables[TAB_LIB].head; curr; curr = curr->next ) {
        name = curr->sym.name;
        objr = ObjNewRec( CMD_COMENT );
        objr->d.coment.attr = 0x80;
        objr->d.coment.class = CMT_DEFAULT_LIBRARY;
        ObjAttachData( objr, name, strlen( name ) );
        write_record( objr, TRUE );
    }
}

static void write_one_export( dir_node *dir )
/*******************************************/
{
    obj_rec     *objr;
    char        *name;
    proc_info   *info;

    info = dir->e.procinfo;
    if( info->visibility == VIS_EXPORT ) {
        objr = ObjNewRec( CMD_COMENT );
        objr->d.coment.attr = 0x00;
        objr->d.coment.class = CMT_DLL_ENTRY;

        name = Mangle( &dir->sym, NULL );

        ObjAllocData( objr, 4 + strlen( name )  );
        ObjPut8( objr, 2 );
        ObjPut8( objr, 0 );             // temporary
        ObjPutName( objr, name, strlen( name ) );
        ObjPut8( objr, 0 );
        write_record( objr, TRUE );

        AsmFree( name );
    }
}

static void write_export( void )
/******************************/
{
    dir_node    *dir;

    for( dir = Tables[TAB_PROC].head; dir != NULL; dir = dir->next ) {
        write_one_export( dir );
    }
}


static void write_grp( void )
/***************************/
{
    dir_node    *curr;
    dir_node    *segminfo;
    seg_list    *seg;
    obj_rec     *grp;
    uint        line;
    char        writeseg;
    unsigned    i = 1;

    line = LineNumber;

    for( curr = Tables[TAB_GRP].head; curr; curr = curr->next, i++ ) {

        grp = ObjNewRec( CMD_GRPDEF );
        /**/myassert( grp != NULL );

        grp->d.grpdef.idx = curr->e.grpinfo->idx;

        /* we might need up to 3 bytes for each seg in dgroup and 1 byte for
           the group name index */
        ObjAllocData( grp, 1 + 3 * curr->e.grpinfo->numseg );
        ObjPut8( grp, GetLnameIdx( curr->sym.name ) );

        for( seg = curr->e.grpinfo->seglist; seg; seg = seg->next ) {
            writeseg = TRUE;
            segminfo = (dir_node *)(seg->seg);
            if( ( segminfo->sym.state != SYM_SEG ) || ( segminfo->sym.segment == NULL ) ) {
                LineNumber = curr->line;
                AsmErr( SEG_NOT_DEFINED, segminfo->sym.name );
                write_to_file = FALSE;
                LineNumber = line;
            } else {
                ObjPut8( grp, GRP_SEGIDX );
                ObjPutIndex( grp, segminfo->e.seginfo->segrec->d.segdef.idx);
            }
        }
        if( write_to_file ) {
            ObjTruncRec( grp );
            write_record( grp, TRUE );
        } else {
            ObjKillRec( grp );
        }
    }
}

static void write_seg( void )
/***************************/
{
    dir_node    *curr;
    obj_rec     *objr;
    uint        seg_index;
    uint        total_segs = 0;

    for( curr = Tables[TAB_SEG].head; curr; curr = curr->next ) {
        if( ( curr->sym.segment == NULL ) 
          && ( curr->e.seginfo->group == NULL ) )
            AsmErr( SEG_NOT_DEFINED, curr->sym.name );
        total_segs++;
    }

    for( seg_index = 1; seg_index <= total_segs; seg_index++ ) {
        /* find segment by index */
        for( curr = Tables[TAB_SEG].head; curr; curr = curr->next ) {
            if( GetSegIdx( curr->sym.segment ) == seg_index ) {
                break;
            }
        }
        if( curr == NULL )
            continue;
        if( curr->sym.state != SYM_SEG ) {
            AsmErr( SEG_NOT_DEFINED, curr->sym.name );
            continue;
        }
        objr = curr->e.seginfo->segrec;
        objr->is_32 = TRUE;
        objr->d.segdef.ovl_name_idx = 1;
        objr->d.segdef.seg_name_idx = GetLnameIdx( curr->sym.name );
        write_record( objr, FALSE );
        if( curr->e.seginfo->iscode == SEGTYPE_ISCODE ) {
            obj_rec     *rec;

            rec = ObjNewRec( CMD_COMENT );
            rec->d.coment.attr = CMT_TNP;
            rec->d.coment.class = CMT_LINKER_DIRECTIVE;
            ObjAllocData( rec, 3  );
            ObjPut8( rec, LDIR_OPT_FAR_CALLS );
            ObjPutIndex( rec, seg_index );
            write_record( rec, TRUE );
        }
    }
}

static void write_lnames( void )
/******************************/
{
    obj_rec     *objr;
    uint        total_size = 0;
    char        *lname = NULL;

    objr = ObjNewRec( CMD_LNAMES );
    objr->d.lnames.first_idx = 1;
    objr->d.lnames.num_names = LnamesIdx;
    total_size = GetLnameData( &lname );
    if( total_size > 0 ) {
        ObjAttachData( objr, lname, total_size );
    }
    ObjCanFree( objr );
    write_record( objr, TRUE );
}

static void write_global( void )
/******************************/
/* turn the globals into either externs or publics as appropriate */
{
    GetGlobalData();
}

static dir_node *write_extdef( dir_node *start )
/**********************************************/
{
    obj_rec     *objr;
    dir_node    *curr;
    uint        num;
    uint        total_size;
    uint        i;
    char        name[MAX_EXT_LENGTH];
    char        buffer[MAX_LINE_LEN];
    uint        len;

    num = 0;
    total_size = 0;
    i = 0;

    objr = ObjNewRec( CMD_EXTDEF );
    objr->d.extdef.first_idx = 0;

    for( curr = start;
        ( curr != NULL ) && ( curr->e.extinfo->comm == start->e.extinfo->comm );
        curr = curr->next ) {
        Mangle( &curr->sym, buffer );
        len = strlen( buffer );

        if( total_size + len >= MAX_EXT_LENGTH )
            break;
        total_size += len + 2;
        num++;

        name[i] = (char)len;
        i++;
        memcpy( name+i, buffer, len );
        i += len;
        name[i++] = 0;      // for the type index
    }
    ObjAttachData( objr, name, total_size );
    if( num != 0 ) {
        objr->d.extdef.num_names = num;
        write_record( objr, TRUE );
    } else {
        ObjKillRec( objr );
    }
    return( curr );
}

static int opsize( memtype mem_type )
/************************************/
{
    switch( mem_type ) {
    case MT_EMPTY:  return( 0 );
    case MT_SBYTE:
    case MT_BYTE:   return( 1 );
    case MT_SWORD:
    case MT_WORD:   return( 2 );
    case MT_SDWORD:
    case MT_DWORD:  return( 4 );
    case MT_FWORD:  return( 6 );
    case MT_QWORD:  return( 8 );
    case MT_TBYTE:  return( 10 );
    case MT_OWORD:  return( 16 );
    default:        return( 0 );
    }
}

#define THREE_BYTE_MAX ( (1UL << 24) - 1 )

static int get_number_of_bytes_for_size_in_commdef( unsigned long value )
/****************************************************************/
{
    /* The spec allows up to 128 in a one byte size field, but lots
       of software has problems with that, so we'll restrict ourselves
       to 127.
    */
    if( value < 128 ) {
        return( 1 );    /* 1 byte value */
    } else if( value <= USHRT_MAX ) {
        return( 3 );    /* 1 byte flag + 2 byte value */
    } else if( value <= THREE_BYTE_MAX ) {
        return( 4 );    /* 1 byte flag + 3 byte value */
    } else { // if( value <= ULONG_MAX )
        return( 5 );    /* 1 byte flag + 4 byte value */
    }
}

static dir_node *write_comdef( dir_node *start )
/**********************************************/
{
    obj_rec     *objr;
    dir_node    *curr;
    uint        num = 0;
    uint        total_size = 0;
    uint        varsize = 0;
    uint        symsize;
    uint        i = 0;
    uint        j = 0;
    char        *name;
    uint        len;
    unsigned long value;
    char        buffer[MAX_LINE_LEN];
    char        *ptr;


    objr = ObjNewRec( CMD_COMDEF );
    objr->d.comdef.first_idx = 0;

    for( curr = start;
        ( curr != NULL ) && ( curr->e.extinfo->comm == start->e.extinfo->comm );
        curr = curr->next ) {
        ptr = Mangle( &curr->sym, buffer );
        total_size += 3 + strlen( ptr );
        /* 3 = 1 for string len + 1 for type index + 1 for data type */

        varsize = opsize( curr->sym.mem_type );
        if( curr->e.comminfo->distance == T_FAR ) {
            total_size += get_number_of_bytes_for_size_in_commdef( varsize );
            total_size += get_number_of_bytes_for_size_in_commdef( curr->e.comminfo->size );
        } else {
            total_size += get_number_of_bytes_for_size_in_commdef( curr->e.comminfo->size );
        }
        num++;
    }

    if( total_size > 0 ) {
        name = AsmAlloc( total_size * sizeof( char ) );
        for( curr = start;
            ( curr != NULL ) && ( curr->e.extinfo->comm == start->e.extinfo->comm );
            curr = curr->next ) {
            ptr = Mangle( &curr->sym, buffer );
            len = strlen( ptr );
            name[i] = (char)len;
            i++;
            memcpy( name+i, ptr, len );
            i += len;
            name[i++] = 0;      // for the type index

            /* now add the data type & communal length */
            if( curr->e.comminfo->distance == T_FAR ) {
                name[i++] = COMDEF_FAR;
            } else {
                name[i++] = COMDEF_NEAR;
            }

            value = curr->e.comminfo->size;
            varsize = get_number_of_bytes_for_size_in_commdef( value );
            switch( varsize ) {
            case 1:
                break;
            case 3:
                name[i++] = COMDEF_LEAF_2;
                break;
            case 4:
                name[i++] = COMDEF_LEAF_3;
                break;
            case 5:
                name[i++] = COMDEF_LEAF_4;
                break;
            }
            if( varsize > 1 )
                varsize--; /* we already output 1 byte */

            symsize = opsize( curr->sym.mem_type );
            if( curr->e.comminfo->distance != T_FAR ) {
                value *= symsize;
            }

            for( j=0; j < varsize; j++ ) {
                name[i++] = value % ( UCHAR_MAX + 1 );
                value >>= 8;
            }

            if( curr->e.comminfo->distance == T_FAR ) {
                /* mem type always needs <= 1 byte */
                myassert( symsize < UCHAR_MAX );
                name[i++] = symsize;
            }
        }
        ObjAttachData( objr, name, total_size );
    }
    ObjCanFree( objr );
    if( num != 0 ) {
        objr->d.comdef.num_names = num;
        write_record( objr, TRUE );
    } else {
        ObjKillRec( objr );
    }
    return( curr );
}

static void write_ext_comm( void )
/********************************/
{
    dir_node    *next;

    for( next = Tables[TAB_EXT].head; next != NULL; ) {
        if( next->e.extinfo->comm == 0 ) {
            // extdef
            next = write_extdef( next );
        } else {
            // comdef
            next = write_comdef( next );
        }
    }
}

static void write_header( void )
/******************************/
{
    obj_rec     *objr;
    unsigned    len;
    char        *name;
    char        full_name[_MAX_PATH];

    objr = ObjNewRec( CMD_THEADR );
    if( Options.module_name != NULL ) {
        name = Options.module_name;
    } else {
        name = _getFilenameFullPath( full_name, AsmFiles.fname[ASM], sizeof( full_name ) );
    }
    len = strlen( name );
    ObjAllocData( objr, len + 1 );
    ObjPutName( objr, name, len );
    ObjTruncRec( objr );
    write_record( objr, TRUE );
}

static int write_modend( void )
/*****************************/
{
    if( ModendRec == NULL ) {
        AsmError( UNEXPECTED_END_OF_FILE );
        return ERROR;
    }
    write_record( ModendRec, TRUE );
    return NOT_ERROR;
}

static int write_autodep( void )
/******************************/
{
    obj_rec       *objr;
    char          buff[2*PATH_MAX + 5];
    unsigned int len;
    FNAMEPTR      curr;
    FNAMEPTR      last;

    if( !Options.emit_dependencies )
        return NOT_ERROR;

    // add source file to autodependency list
    AddFlist( AsmFiles.fname[ASM] );

    for( curr = FNames; curr != NULL; ) {
        objr = ObjNewRec( CMD_COMENT );
        objr->d.coment.attr = 0x80;
        objr->d.coment.class = CMT_DEPENDENCY;

        len = strlen(curr->name);
        *((time_t *)buff) = _timet2dos(curr->mtime);
        *(buff + 4) = (unsigned char)len;
        strcpy(buff + 5, curr->name);
        len += 5;

        ObjAttachData( objr, buff, len );

        write_record( objr, TRUE );

        AsmFree(curr->name);
        last = curr;
        curr = curr->next;
        AsmFree(last);
    }
    FNames = NULL;
    // one NULL dependency record must be on the end
    objr = ObjNewRec( CMD_COMENT );
    objr->d.coment.attr = 0x80;
    objr->d.coment.class = CMT_DEPENDENCY;
    ObjAttachData( objr, "", 0 );
    write_record( objr, TRUE );
    return NOT_ERROR;
}

void AddLinnumDataRef( void )
/***************************/
/* store a reference for the current line at the current address */
{
    struct linnum_data  *curr;
    uint                line_number;

    if( in_prologue ) {
        line_number = CurrProc->line;
    } else {
        line_number = LineNumber;
    }
    if( line_number < 0x8000 )  {
        if( lastLineNumber != line_number ) {
            curr = AsmAlloc( sizeof( struct linnum_data ) );
            curr->number = line_number;
            curr->offset = AsmCodeAddress;

            AddLinnumData( curr );
            lastLineNumber = line_number;
        }
    }
}

static void write_linnum( void )
/******************************/
{
    struct linnum_data  *ldata;
    int                 count;
    obj_rec             *objr;
    bool                need_32;

    count = GetLinnumData( &ldata, &need_32 );
    if( count == 0 )
        return;
    if( ldata == NULL ) {
        AsmError( NO_MEMORY );
    } else {
        objr = ObjNewRec( CMD_LINNUM );
        objr->is_32 = need_32;
        objr->d.linnum.num_lines = count;
        objr->d.linnum.lines = ldata;
        objr->d.linnum.d.base.grp_idx = GetGrpIdx( GetGrp( &GetCurrSeg()->sym ) ); // fixme ?
        objr->d.linnum.d.base.seg_idx = CurrSeg->seg->e.seginfo->segrec->d.segdef.idx;
        objr->d.linnum.d.base.frame = 0; // fixme ?

        write_record( objr, TRUE );
    }
}

#ifdef SEPARATE_FIXUPP_16_32

static void divide_fixup_list( struct fixup **fl16, struct fixup **fl32 ) {
/**********************************************/
/* divide fixup record list to the 16-bit or 32-bit list of a fixup record */

    struct fixup *fix;
    struct fixup *fix16;
    struct fixup *fix32;

    fix16 = NULL;
    fix32 = NULL;
    fix = FixupListHead;
    for( fix = FixupListHead; fix != NULL; fix = fix->next ) {
        switch( fix->loc_method ) {
        case FIX_OFFSET386:
        case FIX_POINTER386:
            if( fix32 == NULL ) {
                *fl32 = fix;
            } else {
                fix32->next = fix;
            }
            fix32 = fix;
            break;
        default:
            if( fix16 == NULL ) {
                *fl16 = fix;
            } else {
                fix16->next = fix;
            }
            fix16 = fix;
            break;
        }
    }
    if( fix32 != NULL ) {
        fix32->next = NULL;
    }
    if( fix16 != NULL ) {
        fix16->next = NULL;
    }
}

#else

static void check_need_32bit( obj_rec *objr ) {
/**********************************************/
/* figure out if we need the 16-bit or 32-bit form of a fixup record */

    struct fixup        *fix;

    fix = objr->d.fixup.fixup;
    for( ;; ) {
        if( fix == NULL )
            break;
        switch( fix->loc_method ) {
        case FIX_OFFSET386:
        case FIX_POINTER386:
            objr->is_32 = 1;
            break;
        }
        if( (unsigned_32)fix->lr.target_offset > 0xffffUL ) {
            objr->is_32 = 1;
        }
        if( objr->is_32 )
            break;
        fix = fix->next;
    }
}

#endif

static void write_ledata( void )
/******************************/
{
    obj_rec         *objr;
#ifdef SEPARATE_FIXUPP_16_32
    struct fixup    *fl16 = NULL;
    struct fixup    *fl32 = NULL;
#endif

    if( BufSize > 0 ) {
        objr = ObjNewRec( CMD_LEDATA );
        ObjAttachData( objr, AsmCodeBuffer, BufSize );
        objr->d.ledata.idx = CurrSeg->seg->e.seginfo->segrec->d.segdef.idx;
        objr->d.ledata.offset = CurrSeg->seg->e.seginfo->start_loc;
        if( objr->d.ledata.offset > 0xffffUL )
            objr->is_32 = TRUE;
        CurrSeg->seg->e.seginfo->start_loc = CurrSeg->seg->e.seginfo->current_loc;
        write_record( objr, TRUE );

        /* Process Fixup, if any */
        if( FixupListHead != NULL ) {
#ifdef SEPARATE_FIXUPP_16_32
            divide_fixup_list( &fl16, &fl32 );
            /* Process Fixup, if any */
            if( fl16 != NULL ) {
                objr = ObjNewRec( CMD_FIXUP );
                objr->is_32 = FALSE;
                objr->d.fixup.fixup = fl16;
                write_record( objr, TRUE );
            }
            if( fl32 != NULL ) {
                objr = ObjNewRec( CMD_FIXUP );
                objr->is_32 = TRUE;
                objr->d.fixup.fixup = fl32;
                write_record( objr, TRUE );
            }
#else
            objr = ObjNewRec( CMD_FIXUP );
            objr->d.fixup.fixup = FixupListHead;
            check_need_32bit( objr );
            write_record( objr, TRUE );
#endif
            FixupListHead = FixupListTail = NULL;
        }
        /* add line numbers if debugging info is desired */
        if( Options.debug_flag ) {
            write_linnum();
        }
    }
}

static void put_public_procs_in_public_table( void )
/**************************************************/
{
    dir_node            *proc;

    for( proc = Tables[TAB_PROC].head; proc != NULL; proc = proc->next ) {

        /* put it into the pub table */
        if( !proc->sym.public ) {
            proc->sym.public = TRUE;
            AddPublicData( proc );
        }
    }
}

static void write_alias()
/***********************/
{
    obj_rec             *objr;
    char                *alias;
    char                *subst;
    char                *new;
    char                len1;
    char                len2;
    bool                first = TRUE;

    while( ( alias = GetAliasData( first ) ) != NULL ) {
        /* output an alias record for this alias */
        subst = alias + strlen( alias ) + 1;

        len1 = strlen( alias );
        len2 = strlen( subst );

        new = AsmTmpAlloc( len1 + len2 + 2 );

        *new = len1;
        new++;
        strncpy( new, alias, len1 );
        new+=len1;
        *new = len2;
        new++;
        strncpy( new, subst, len2 );
        new -= len1 + 2;

        objr = ObjNewRec( CMD_ALIAS );
        ObjAttachData( objr, new, len1+len2+2);
        write_record( objr, TRUE );
        first = FALSE;
    }
}

static int write_pub()
/*********************/
/* note that procedures with public or export visibility are written out here */
{
    obj_rec             *objr;
    struct pubdef_data  *data;
    uint                i;
    uint                count = 0;
    uint                seg;
    uint                grp;
    char                cmd;
    bool                first = TRUE;
    bool                need32 = FALSE;

    put_public_procs_in_public_table();

    while( ( count = GetPublicData( &seg, &grp, &cmd, &NameArray, &data, &need32, first) ) > 0 ) {

        /* create a public record for this segment */

        objr = ObjNewRec( cmd );
        objr->is_32 = need32;
        objr->d.pubdef.base.grp_idx = grp;
        objr->d.pubdef.base.seg_idx = seg;
        objr->d.pubdef.base.frame = 0;
        objr->d.pubdef.num_pubs = count;
        objr->d.pubdef.pubs = data;
        objr->d.pubdef.free_pubs = TRUE;
        write_record( objr, TRUE );

        /* free the names table */
        for( i = 0; i < count; i++ ) {
            if( NameArray[i] != NULL ) {
                AsmFree( NameArray[i] );
            }
        }
        AsmFree( NameArray );
        first = FALSE;
    }
    return( NOT_ERROR );
}

const char *NameGet( uint_16 hdl )
/********************************/
// WOMP callback routine
{
    return( NameArray[hdl] );
}


void FlushCurrSeg( void )
/***************************/
{
    unsigned i;

    /* this IF is for code segments which contain entirely data up to the
     * point when they are flushed
     * outselect calls flushcurrseg right back
     */
    if( CurrSeg == NULL )
        return;

    /* first check if the last fixup overlaps the end of the ledata record
     * if so, wait until we get more bytes output so that it will not
     */

    if( FixupListTail != NULL ) {
        switch( FixupListTail->loc_method ) {
        case FIX_LO_BYTE:
        case FIX_HI_BYTE:
            i = 1;
            break;
        case FIX_OFFSET:
        case FIX_BASE:
            i = 2;
            break;
        case FIX_POINTER:
        case FIX_OFFSET386:
            i = 4;
            break;
        case FIX_POINTER386:
            i = 6;
            break;
        }
        if( FixupListTail->loc_offset + i > BufSize ) {
            return; // can't output the ledata record as is
        }
    }

    write_ledata();
    BufSize = 0;
    OutSelect( FALSE );
}

static void reset_seg_len( void )
/*******************************/
/* Reset length of all segments to zero */
{
    dir_node    *curr;

    for( curr = Tables[TAB_SEG].head; curr; curr = curr->next ) {
        if( ( curr->sym.state != SYM_SEG ) || ( curr->sym.segment == NULL ) )
            continue;
        if( curr->e.seginfo->segrec->d.segdef.combine != COMB_STACK ) {
            curr->e.seginfo->segrec->d.segdef.seg_length = 0;
        }
        curr->e.seginfo->start_loc = 0; // fixme ?
        curr->e.seginfo->current_loc = 0;
    }
}

static void writepass1stuff( void )
/*********************************/
{
    if( CurrProc != NULL ) {
        AsmError( END_OF_PROCEDURE_NOT_FOUND );
        return;
    }
    write_header();
    write_autodep();
    if( Globals.dosseg )
        write_dosseg();
    write_lib();
    write_lnames();
    write_seg();
    write_grp();
    write_global();
    write_ext_comm();
    write_alias();
    if( write_pub() == ERROR )
        return;
    write_export();
    write_end_of_pass1();
}

static unsigned long OnePass( char *string )
/******************************************/
{
    set_cpu_parameters();
    set_fpu_parameters();

    AssumeInit();

    EndDirectiveFound = FALSE;
    PhaseError = FALSE;
    Modend = FALSE;
    PassTotal = 0;
    LineNumber = 0;
    lastLineNumber = 0;
    MacroExitState = 0;

    for(;;) {
        if( ScanLine( string, MAX_LINE_LEN ) == NULL )
            break; // EOF
        AsmLine( string );
        if( EndDirectiveFound ) {
            break;
        }
    }
    CheckProcOpen();
    return( PassTotal );
}

void WriteObjModule( void )
/**************************/
{
    char                codebuf[ MAX_LEDATA_THRESHOLD ];
    char                string[ MAX_LINE_LEN ];
    char                *p;
    unsigned long       prev_total;
    unsigned long       curr_total;

    AsmCodeBuffer = codebuf;

    write_init();

    Parse_Pass = PASS_1;
#ifdef DEBUG_OUT
    if( Options.debug )
        printf( "*************\npass %u\n*************\n", Parse_Pass + 1 );
#endif
    prev_total = OnePass( string );
    if( EndDirectiveFound ) {
        if( !Options.stop_at_end ) {
            for( ;; ) {
                if( ScanLine( string, MAX_LINE_LEN ) == NULL )
                    break;
                p = string;
                while( isspace( *p ) )
                    ++p;
                if( *p != '\0' ) {
                    AsmWarn( 1, END_DIRECTIVE_REQUIRED );
                    break;
                }
            }
        }
    } else {
        AsmError( END_DIRECTIVE_REQUIRED );
    }
    while( PopLineQueue() ) {
    }
    CheckForOpenConditionals();
    for( ;; ) {
        if( !write_to_file || Options.error_count > 0 )
            break;
        writepass1stuff();
        ++Parse_Pass;
        rewind( AsmFiles.file[ASM] );
        reset_seg_len();
        BufSize = 0;
        MacroLocalVarCounter = 0;
        Globals.data_in_code = FALSE;
        PrepAnonLabels();

#ifdef DEBUG_OUT
        if( Options.debug )
            printf( "*************\npass %u\n*************\n", Parse_Pass + 1 );
#endif
        curr_total = OnePass( string );
        // remove all remaining lines and deallocate corresponding memory
        while( ScanLine( string, MAX_LINE_LEN ) != NULL ) {
        }
        while( PopLineQueue() ) {
        }
        if( !PhaseError && prev_total == curr_total )
            break;
#ifdef DEBUG_OUT
        write_modend();
#endif
        ObjWriteClose( pobjState.file_out );
        /* This remove works around an NT networking bug */
        remove( AsmFiles.fname[OBJ] );
        pobjState.file_out = ObjWriteOpen( AsmFiles.fname[OBJ] );
        if( pobjState.file_out == NULL ) {
            Fatal( MSG_CANNOT_OPEN_FILE, AsmFiles.fname[OBJ] );
        }
        prev_total = curr_total;
    }
    if( write_to_file && Options.error_count == 0 )
        write_modend();

    FreeFlist();
    AsmSymFini();
    FreeIncludePath();
    write_fini();
}

