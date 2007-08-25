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

#include "asmalloc.h"

#if defined( _STANDALONE_ )
  #include "directiv.h"
  #include "myassert.h"
#endif

struct asmfixup         *InsFixups[3];

#if !defined( _STANDALONE_ )

struct asmfixup         *FixupHead;

#else

struct fixup            *FixupListHead; // head of list of fixups
struct fixup            *FixupListTail;

void add_frame( void )
/********************/
/* determine the frame and frame datum for the fixup */
{
    struct asmfixup     *fixup;

    if( Parse_Pass != PASS_1 ) {
        fixup = InsFixups[Opnd_Count];
        if( fixup == NULL )
            return;
        fixup->frame = Frame;
        fixup->frame_datum = Frame_Datum;
    }
}

#endif

struct asmfixup *AddFixup( struct asm_sym *sym, enum fixup_types fixup_type, enum fixup_options fixup_option )
/************************************************************************************************************/
/*
  put the correct target offset into the link list when forward reference of
  relocatable is resolved;
*/
{
    struct asmfixup     *fixup;

    switch( Code->info.token ) {
    case T_CMPS:
    case T_LODS:
    case T_MOVS:
    case T_OUTS:
    case T_INS:
    case T_SCAS:
    case T_STOS:
    case T_XLAT:
        return( NULL );
    }

    fixup = AsmAlloc( sizeof( struct asmfixup ) );
    if( fixup != NULL ) {
        fixup->external = 0;
#if defined( _STANDALONE_ )
        fixup->sym = sym;
        fixup->offset = sym->offset;
        fixup->def_seg = (CurrSeg != NULL) ? CurrSeg->seg : NULL;
        fixup->frame = Frame;                   // this is just a guess
        fixup->frame_datum = Frame_Datum;
        fixup->next = sym->fixup;
        sym->fixup = fixup;
#else
        fixup->name = sym->name;
        fixup->next = FixupHead;
        FixupHead = fixup;
        fixup->offset = 0;
#endif
        fixup->fixup_type = fixup_type;
        fixup->fixup_option = fixup_option;
        InsFixups[Opnd_Count] = fixup;
    }
    return( fixup );
}

#if defined( _STANDALONE_ )

#define SkipFixup() \
    fixup->next = sym->fixup; \
    sym->fixup = fixup

#else

#define SkipFixup() \
    fixup->next = FixupHead; \
    FixupHead = fixup

static void PatchCodeBuffer( struct asmfixup *fixup, unsigned size )
/******************************************************************/
{
    long    disp;
    uint_8  *dst;

    dst = fixup->fixup_loc + AsmCodeBuffer;
    disp = fixup->offset + AsmCodeAddress - fixup->fixup_loc - size;
    for( ; size > 0; size-- ) {
        *(dst++) = disp;
        disp >>= 8;
    }
}

#endif

static int DoPatch( struct asm_sym *sym, struct asmfixup *fixup )
/***************************************************************/
{
    long                disp;
    long                max_disp;
    unsigned            size;
#if defined( _STANDALONE_ )
    dir_node            *seg;

    // all relative fixups should occure only at first pass and they signal forward references
    // they must be removed after patching or skiped ( next processed as normal fixup )
    seg = GetSeg( sym );
    if( seg == NULL || fixup->def_seg != seg ) {
        /* can't backpatch if fixup location is in diff seg than symbol */
        SkipFixup();
        return( NOT_ERROR );
    } else if( Parse_Pass != PASS_1 ) {
    } else if( sym->mem_type == MT_FAR && fixup->fixup_option == OPTJ_CALL ) {
        // convert far call to near, only at first pass
        PhaseError = TRUE;
        sym->offset++;
        AsmByte( 0 );
        AsmFree( fixup );
        return( NOT_ERROR );
    } else if( sym->mem_type == MT_NEAR ) {
        // near forward reference, only at first pass
        switch( fixup->fixup_type ) {
        case FIX_RELOFF32:
        case FIX_RELOFF16:
            AsmFree( fixup );
            return( NOT_ERROR );
        }
    }
#else
    if( fixup->name != sym->name ) {
        SkipFixup();
        return( NOT_ERROR );
    }
#endif
    size = 0;
    switch( fixup->fixup_type ) {
    case FIX_RELOFF32:
        size = 2;
        /* fall through */
    case FIX_RELOFF16:
        size++;
        /* fall through */
    case FIX_RELOFF8:
        size++;
        // calculate the displacement
        disp = fixup->offset + AsmCodeAddress - fixup->fixup_loc - size;
        max_disp = (1UL << ((size * 8)-1)) - 1;
        if( disp > max_disp || disp < (-max_disp-1) ) {
#if !defined( _STANDALONE_ )
            AsmError( JUMP_OUT_OF_RANGE );
            FixupHead = NULL;
            return( ERROR );
        } else {
            PatchCodeBuffer( fixup, size );
#else
            PhaseError = TRUE;
            switch( size ) {
            case 1:
                switch( fixup->fixup_option ) {
                case OPTJ_EXPLICIT:
                    AsmError( JUMP_OUT_OF_RANGE );
                    sym->fixup = NULL;
                    return( ERROR );
                case OPTJ_EXTEND:
                    sym->offset++;
                    AsmByte( 0 );
                    /* fall through */
                case OPTJ_JXX:
                    sym->offset++;
                    AsmByte( 0 );
                    /* fall through */
                default:
                    if( Code->use32 ) {
                        sym->offset += 2;
                        AsmByte( 0 );
                        AsmByte( 0 );
                    }
                    sym->offset++;
                    AsmByte( 0 );
                    break;
                }
                break;
            case 2:
            case 4:
                AsmWarn( 4, JUMP_OUT_OF_RANGE );
                break;
            }
#endif
        }
        AsmFree( fixup );
        break;
    default:
        SkipFixup();
        break;
    }
    return( NOT_ERROR );
}

int BackPatch( struct asm_sym *sym )
/**********************************/
/*
- patching for forward reference labels in Jmp/Call instructions;
- call only when a new label appears;
*/
{
    struct asmfixup     *fixup;
    struct asmfixup     *next;

#if defined( _STANDALONE_ )
    fixup = sym->fixup;
    sym->fixup = NULL;
#else
    fixup = FixupHead;
    FixupHead = NULL;
#endif
    for( ; fixup != NULL; fixup = next ) {
        next = fixup->next;
        if( DoPatch( sym, fixup ) == ERROR ) {
            return( ERROR );
        }
    }
    return( NOT_ERROR );
}

void mark_fixupp( OPNDTYPE determinant, int index )
/*************************************************/
/*
  this routine marks the correct target offset and data record address for
  FIXUPP record;
*/
{
    struct asmfixup     *fixup;

    fixup = InsFixups[index];
    if( fixup != NULL ) {
        fixup->fixup_loc = AsmCodeAddress;
#if defined( _STANDALONE_ )
        // fixup->offset = Code->data[index];
        // Code->data[index] = 0; // fixme
        if( fixup->fixup_type != FIX_SEG ) {
            Code->data[index] += fixup->offset;
        }
        /*
        20-Aug-92: put the offset in the location instead of attaching it
        to the fixup
        */
#else
        fixup->offset = Code->data[index];
        Code->data[index] = 0;
#endif

        switch( determinant ) {
        case OP_I16:
        case OP_J32:
            switch( fixup->fixup_type ) {
            case FIX_OFF32:
                fixup->fixup_type = FIX_OFF16;
                break;
            case FIX_PTR32:
                fixup->fixup_type = FIX_PTR16;
                break;
            }
            break;
        case OP_I32:
        case OP_J48:
            switch( fixup->fixup_type ) {
            case FIX_OFF16:
                fixup->fixup_type = FIX_OFF32;
                break;
            case FIX_PTR16:
                fixup->fixup_type = FIX_PTR32;
                break;
            }
            break;
        }
    }
}

#if defined( _STANDALONE_ )

struct fixup *CreateFixupRec( int index )
/***************************************/
/* Create a fixup record for WOMP;
   Note that if Modend is TRUE, it means the fixup is the starting address
   for the module.
*/
{
    struct asmfixup     *fixup;         // fixup structure from WASM
    struct fixup        *fixnode;       // fixup structure from WOMP
    struct asm_sym      *sym;

    fixup = InsFixups[index];

    if( fixup == NULL )
        return( NULL );

    fixnode = FixNew();
    fixnode->next = NULL;
    fixnode->self_relative = FALSE;
    fixnode->lr.target_offset = 0;
    if( !Modend ) {
        fixnode->lr.is_secondary = TRUE;
    } else {
        fixnode->lr.is_secondary = FALSE;
        fixnode->lr.target_offset = fixup->offset;
    }

    if( !Modend ) {
        switch( fixup->fixup_type ) {
        case FIX_LOBYTE:
            fixnode->loc_method = FIX_LO_BYTE;
            break;
        case FIX_RELOFF8:
            fixnode->self_relative = TRUE;
            fixnode->loc_method = FIX_LO_BYTE;
            break;
        case FIX_RELOFF16:
            fixnode->self_relative = TRUE;
        case FIX_OFF16:
            fixnode->loc_method = FIX_OFFSET;
            break;
        case FIX_RELOFF32:
            fixnode->self_relative = TRUE;
        case FIX_OFF32:
            fixnode->loc_method = FIX_OFFSET386;
            break;
        case FIX_SEG:
            fixnode->loc_method = FIX_BASE;
            break;
        case FIX_PTR16:
            fixnode->loc_method = FIX_POINTER;
            break;
        case FIX_PTR32:
            fixnode->loc_method = FIX_POINTER386;
            break;
        }
    }

    sym = fixup->sym;
    if( sym == NULL )
        return( NULL );

    fixnode->loader_resolved = FALSE;

    fixnode->loc_offset = AsmCodeAddress - GetCurrSegStart();

    /*------------------------------------*/
    /* Determine the Target and the Frame */
    /*------------------------------------*/

    if( sym->state == SYM_UNDEFINED ) {
        AsmErr( SYMBOL_NOT_DEFINED, sym->name );
        return( NULL );
    } else if( sym->state == SYM_GRP ) {

        if( Modend ) {
            AsmError( INVALID_START_ADDRESS );
            return( NULL );
        }

        fixnode->lr.target = TARGET_GRP;
        fixnode->lr.target_datum = GetGrpIdx( sym );
        if( fixup->frame != EMPTY ) {
            fixnode->lr.frame = fixup->frame;
            fixnode->lr.frame_datum = fixup->frame_datum;
        } else {
            fixnode->lr.frame = FRAME_GRP;
            fixnode->lr.frame_datum = fixnode->lr.target_datum;
        }

    } else if( sym->state == SYM_SEG ) {

        if( Modend ) {
            AsmError( INVALID_START_ADDRESS );
            return( NULL );
        }

        fixnode->lr.target = TARGET_SEG;
        fixnode->lr.target_datum = GetSegIdx( sym );
        if( fixup->frame != EMPTY ) {
            fixnode->lr.frame = fixup->frame;
            fixnode->lr.frame_datum = fixup->frame_datum;
        } else {
            fixnode->lr.frame = FRAME_SEG;
            fixnode->lr.frame_datum = fixnode->lr.target_datum;
        }

    } else {

        /* symbol is a label */

        if( sym->state == SYM_EXTERNAL ) {
            if( Modend ) {
                if( sym->mem_type == MT_BYTE ||
                    sym->mem_type == MT_SBYTE ||
                    sym->mem_type == MT_WORD ||
                    sym->mem_type == MT_SWORD ||
                    sym->mem_type == MT_DWORD ||
                    sym->mem_type == MT_SDWORD ||
                    sym->mem_type == MT_FWORD ||
                    sym->mem_type == MT_QWORD ||
                    sym->mem_type == MT_TBYTE ||
                    sym->mem_type == MT_OWORD ||
                    sym->mem_type == MT_ABS ) {

                    AsmError( MUST_BE_ASSOCIATED_WITH_CODE );
                    return( NULL );
                }
                fixnode->lr.target = TARGET_EXT & TARGET_WITH_DISPL;
            } else {
                fixnode->lr.target = TARGET_EXT;
            }
            fixnode->lr.target_datum = GetExtIdx( sym );

            if( fixup->frame == FRAME_GRP && fixup->frame_datum == 0 ) {
                /* set the frame to the frame of the corresponding segment */
                fixup->frame_datum = GetGrpIdx( sym );
            }
        } else {
            /**/myassert( sym->segment != NULL );
            if( Modend ) {
                fixnode->lr.target = TARGET_SEG & TARGET_WITH_DISPL;
                fixup->frame = FRAME_TARG;
            } else {
                fixnode->lr.target = TARGET_SEG;
            }
            fixnode->lr.target_datum = GetSegIdx( sym->segment );
        }

        /* HMMM .... what if fixup->frame is say -2 .... ie empty?
        * what should really be going on here?
        */
        // fixnode->lr.frame = (uint_8)fixup->frame;
        // fixnode->lr.frame_datum = fixup->frame_datum;

        if( fixup->frame != EMPTY ) {
            fixnode->lr.frame = (uint_8)fixup->frame;
        } else {
            fixnode->lr.frame = FRAME_TARG;
        }
        fixnode->lr.frame_datum = fixup->frame_datum;

        if( Modend )
            return( fixnode );
    }

    /*--------------------*/
    /* Optimize the fixup */
    /*--------------------*/

    if( fixnode->lr.frame == ( fixnode->lr.target - TARGET_SEG ) ) {
        fixnode->lr.frame = FRAME_TARG;
    }

    return( fixnode );
}

int store_fixup( int index )
/**************************/
/* Store the fixup information in a WOMP fixup record */
{
    struct fixup        *fixnode;

    if( Parse_Pass == PASS_1 )
        return( NOT_ERROR );
    if( InsFixups[index] == NULL )
        return( NOT_ERROR );
    fixnode = CreateFixupRec( index );
    if( fixnode == NULL )
        return( ERROR );

    if( FixupListHead == NULL ) {
        FixupListTail = FixupListHead = fixnode;
    } else {
        FixupListTail->next = fixnode;
        FixupListTail = fixnode;
    }
    return( NOT_ERROR );
}

int MakeFpFixup( struct asm_sym *sym )
/*************************************/
{
    int old_count;
    int_8 old_frame;

    old_count = Opnd_Count;
    old_frame = Frame;
    Opnd_Count = 2;
    Frame = FRAME_LOC;
    AddFixup( sym, FIX_OFF16, OPTJ_NONE );
    Frame = old_frame;
    Opnd_Count = old_count;
    if( store_fixup( 2 ) == ERROR )
        return( ERROR ); // extra entry in insfixups
    return( NOT_ERROR );
}

#endif
