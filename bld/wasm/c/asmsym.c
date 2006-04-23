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
* Description:  symbol manipulation routines
*
****************************************************************************/


#include "asmglob.h"

#include "asmsym.h"
#include "asmalloc.h"
#include "asmdefs.h"

#ifdef __USE_BSD
#define stricmp strcasecmp
#endif

#if defined( _STANDALONE_ )

#include "directiv.h"
#include "queues.h"
#include "hash.h"

static struct asm_sym *sym_table[ HASH_TABLE_SIZE ] = { NULL };
/* initialize the whole table to null pointers */

#else

static struct asm_sym  *AsmSymHead;

static unsigned short CvtTable[] = {
    MT_BYTE,   // INT1
    MT_WORD,   // INT2
    MT_DWORD,  // INT4
    MT_FWORD,  // INT6
    MT_QWORD,  // INT8
    MT_DWORD,  // FLOAT4
    MT_QWORD,  // FLOAT8
    MT_TBYTE,  // FLOAT10
    MT_NEAR,   // NEAR2
    MT_NEAR,   // NEAR4
    MT_FAR,    // FAR2
    MT_FAR     // FAR4
};

#endif

static char *InitAsmSym( struct asm_sym *sym, char *name )
/************************************************/
{
    sym->name = AsmAlloc( strlen( name ) + 1 );
    if( sym->name != NULL ) {
        strcpy( sym->name, name );
        sym->next = NULL;
        sym->fixup = NULL;
#if defined( _STANDALONE_ )
        sym->segment = NULL;
        sym->offset = 0;
        sym->public = FALSE;
        sym->langtype = ModuleInfo.langtype;
        sym->first_size = 0;
        sym->first_length = 0;
        sym->total_size = 0;
        sym->total_length = 0;
        sym->count = 0;
        sym->mangler = NULL;
        sym->state = SYM_UNDEFINED;
        sym->mem_type = MT_EMPTY;
#else
        sym->addr = 0;
        sym->state = AsmQueryExternal( sym->name );
        if( sym->state == SYM_UNDEFINED ) {
            sym->mem_type = MT_EMPTY;
        } else {
            sym->mem_type = CvtTable[ AsmQueryType( sym->name ) ];
        }
#endif
    }
    return( sym->name );
}

static struct asm_sym *AllocASym( char *name )
/************************************************/
{
    struct asm_sym      *sym;

#if defined( _STANDALONE_ )
    sym = AsmAlloc( sizeof( dir_node ) );
#else
    sym = AsmAlloc( sizeof( struct asm_sym ) );
#endif
    if( sym != NULL ) {
        if( InitAsmSym( sym, name ) == NULL ) {
            AsmFree( sym );
            return( NULL );
        }
#if defined( _STANDALONE_ )
        ((dir_node *)sym)->next = NULL;
        ((dir_node *)sym)->prev = NULL;
        ((dir_node *)sym)->line = 0;
        ((dir_node *)sym)->e.seginfo = NULL;
#endif
    }
    return sym;
}

static struct asm_sym **AsmFind( char *name )
/***********************************/
/* find a symbol in the symbol table, return NULL if not found */
{
    struct asm_sym      **sym;

#if defined( _STANDALONE_ )
    sym = &sym_table[ hashpjw( name ) ];
#else
    sym = &AsmSymHead;
#endif
    for( ; *sym; sym = &((*sym)->next) ) {
        if( stricmp( name, (*sym)->name ) == 0 ) {
            return( sym );
        }
    }
    return( sym );
}

struct asm_sym *AsmLookup( char *name )
/*************************************/
{
    struct asm_sym      **sym_ptr;
    struct asm_sym      *sym;

    if( strlen( name ) > MAX_ID_LEN ) {
        AsmError( LABEL_TOO_LONG );
        return NULL;
    }

    sym_ptr = AsmFind( name );
    sym = *sym_ptr;
    if( sym != NULL ) {
#if defined( _STANDALONE_ )
        /* current address operator */
        if( IS_SYM_COUNTER( name ) )
            GetSymInfo( sym );
#endif
        return( sym );
    }

    sym = AllocASym( name );
    if( sym != NULL ) {
        sym->next = *sym_ptr;
        *sym_ptr = sym;

#if defined( _STANDALONE_ )
        if( IS_SYM_COUNTER( name ) ) {
            GetSymInfo( sym );
            sym->state = SYM_INTERNAL;
            sym->mem_type = MT_NEAR;
            return( sym );
        }
#else
        sym->addr = AsmCodeAddress;
#endif
    } else {
        AsmError( NO_MEMORY );
    }
    return( sym );
}

static void FreeASym( struct asm_sym *sym )
/*****************************************/
{
#if defined( _STANDALONE_ )
    struct asmfixup     *fixup;

    for( ;; ) {
        fixup = sym->fixup;
        if( fixup == NULL )
            break;
        sym->fixup = fixup->next;
        AsmFree( fixup );
    }
#endif
    AsmFree( sym->name );
    AsmFree( sym );
}

#if defined( _STANDALONE_ )

int AsmChangeName( char *old, char *new )
/***************************************/
{
    struct asm_sym      **sym_ptr;
    struct asm_sym      *sym;

    sym_ptr = AsmFind( old );
    if( *sym_ptr != NULL ) {
        sym = *sym_ptr;
        *sym_ptr = sym->next;
        AsmFree( sym->name );
        sym->name = AsmAlloc( strlen( new ) + 1 );
        strcpy( sym->name, new );

        sym_ptr = AsmFind( new );
        if( *sym_ptr != NULL )
            return( ERROR );

        sym->next = *sym_ptr;
        *sym_ptr = sym;
    }
    return( NOT_ERROR );
}

void AsmTakeOut( char *name )
/***************************/
{
    struct asm_sym      *sym;
    struct asm_sym      **sym_ptr;

    sym_ptr = AsmFind( name );
    if( *sym_ptr != NULL ) {
        /* found it -- so take it out */
        sym = *sym_ptr;
        *sym_ptr = sym->next;
        FreeInfo( (dir_node *)sym );
        FreeASym( sym );
    }
    return;
}

static struct asm_sym *AsmAdd( struct asm_sym *sym )
/*******************************************/
{
    struct asm_sym  **location;

    location = AsmFind( sym->name );

    if( *location != NULL ) {
        /* we already have one */
        AsmError( SYMBOL_ALREADY_DEFINED );
        return( NULL );
    }

    sym->next = *location;
    *location = sym;
    return( sym );
}

struct asm_sym *AllocDSym( char *name, int add_symbol )
/*****************************************************/
/* Create directive symbol and insert it into the symbol table */
{
    struct asm_sym      *new;

    new = AllocASym( name );
    if( new == NULL ) {
        AsmError( NO_MEMORY );
        return( NULL );
    }
    /* add it into the symbol table */
    if( add_symbol ) {
        return( AsmAdd( new ) );
    } else {
        return( new );
    }
}

struct asm_sym *AsmGetSymbol( char *name )
/****************************************/
{
    struct asm_sym  **sym_ptr;

    sym_ptr = AsmFind( name );
#if defined( _STANDALONE_ )
    if( ( *sym_ptr != NULL ) && IS_SYM_COUNTER( name ) )
        GetSymInfo( *sym_ptr );
#endif
    return( *sym_ptr );
}
#endif

void AsmSymFini( void )
/*********************/
{
    struct asm_sym      *sym;
#if defined( _STANDALONE_ )
    dir_node            *dir;
    unsigned            i;

    FreeAllQueues();

#if defined( DEBUG_OUT )
    DumpASym();
#endif

    /* now free the symbol table */
    for( i = 0; i < HASH_TABLE_SIZE; i++ ) {
        struct asm_sym  *next;
        next = sym_table[i];
        for( ;; ) {
            sym = next;
            if( sym == NULL )
                break;
            dir = (dir_node *)sym;
            next = sym->next;
            FreeInfo( dir );
            FreeASym( sym );
        }
    }

#else
    struct asmfixup     *fixup;

    for( ;; ) {
        sym = AsmSymHead;
        if( sym == NULL )
            break;
        AsmSymHead = sym->next;
        FreeASym( sym );
    }
    for( ;; ) {
        fixup = FixupHead;
        if( fixup == NULL )
            break;
        FixupHead = fixup->next;
        AsmFree( fixup );
    }
#endif
}

#if defined( _STANDALONE_ ) && defined( DEBUG_OUT )

static void DumpSymbol( struct asm_sym *sym )
/*******************************************/
{
    dir_node    *dir;
    char        *type;
    char        value[512];
    char        *langtype;
    char        *public;

    dir = (dir_node *)sym;
    *value = 0;
    switch( sym->state ) {
    case SYM_SEG:
        type = "SEGMENT";
//        dir->e.seginfo->lname_idx = 0;
//        dir->e.seginfo->grpidx = 0;
//        dir->e.seginfo->segrec = NULL;
        break;
    case SYM_GRP:
        type = "GROUP";
//        dir->e.grpinfo = AsmAlloc( sizeof( grp_info ) );
//        dir->e.grpinfo->idx = grpdefidx;
//        dir->e.grpinfo->seglist = NULL;
//        dir->e.grpinfo->numseg = 0;
//        dir->e.grpinfo->lname_idx = 0;
        break;
    case SYM_EXTERNAL:
        type = "EXTERNAL";
//        dir->e.extinfo = AsmAlloc( sizeof( ext_info ) );
//        dir->e.extinfo->idx = ++extdefidx;
//        dir->e.extinfo->use32 = Use32;
//        dir->e.extinfo->comm = 0;
        break;
//    case TAB_COMM:
//        sym->state = SYM_EXTERNAL;
//        dir->e.comminfo = AsmAlloc( sizeof( comm_info ) );
//        dir->e.comminfo->idx = ++extdefidx;
//        dir->e.comminfo->use32 = Use32;
//        dir->e.comminfo->comm = 1;
//        break;
    case SYM_CONST:
        type = "CONSTANT";
//        dir->e.constinfo = AsmAlloc( sizeof( const_info ) );
//        dir->e.constinfo->data = NULL;
//        dir->e.constinfo->count = 0;
        break;
    case SYM_PROC:
        type = "PROCEDURE";
//        dir->e.procinfo = AsmAlloc( sizeof( proc_info ) );
//        dir->e.procinfo->regslist = NULL;
//        dir->e.procinfo->paralist = NULL;
//        dir->e.procinfo->locallist = NULL;
        break;
    case SYM_MACRO:
        type = "MACRO";
//        dir->e.macroinfo = AsmAlloc( sizeof( macro_info ) );
//        dir->e.macroinfo->parmlist = NULL;
//        dir->e.macroinfo->data = NULL;
//        dir->e.macroinfo->filename = NULL;
//        dir->e.macroinfo->start_line = LineNumber;
        break;
    case SYM_CLASS_LNAME:
        type = "CLASS";
        break;
    case SYM_LNAME:
        type = "LNAME";
//        dir->e.lnameinfo = AsmAlloc( sizeof( lname_info ) );
//        dir->e.lnameinfo->idx = ++LnamesIdx;
        break;
//    case TAB_PUB:
//        sym->public = TRUE;
//        return;
//    case TAB_GLOBAL:
//        break;
    case SYM_STRUCT:
        type = "STRUCTURE";
//        dir->e.structinfo = AsmAlloc( sizeof( struct_info ) );
//        dir->e.structinfo->size = 0;
//        dir->e.structinfo->alignment = 0;
//        dir->e.structinfo->head = NULL;
//        dir->e.structinfo->tail = NULL;
        break;
    case SYM_STRUCT_FIELD:
        type = "STRUCTURE FIELD";
        break;
    case SYM_LIB:
        type = "LIBRARY";
        break;
    case SYM_UNDEFINED:
        type = "UNDEFINED";
        break;
    case SYM_INTERNAL:
        type = "INTERNAL";
        break;
    default:
        type = "UNKNOWN";
        break;
    }
    if( sym->public ) {
        public = "PUBLIC ";
    } else {
        public = "";
    }
    switch( sym->langtype ) {
    case LANG_C:
        langtype = "C";
        break;
    case LANG_BASIC:
        langtype = "BASIC";
        break;
    case LANG_FORTRAN:
        langtype = "FORTRAN";
        break;
    case LANG_PASCAL:
        langtype = "PASCAL";
        break;
    case LANG_WATCOM_C:
        langtype = "WATCOM_C";
        break;
    case LANG_STDCALL:
        langtype = "STDCALL";
        break;
    case LANG_SYSCALL:
        langtype = "SYSCALL";
        break;
    default:
        langtype = "";
        break;
    }
    DoDebugMsg( "%-30s\t%s\t%s%s\t%8X\t%s\n", sym->name, type, public, langtype, sym->offset, value );
}

void DumpASym( void )
/*******************/
{
    struct asm_sym      *sym;
    unsigned            i;

    DoDebugMsg( "\n" );
    for( i = 0; i < HASH_TABLE_SIZE; i++ ) {
        struct asm_sym  *next;
        next = sym_table[i];
        for( ;; ) {
            sym = next;
            if( sym == NULL )
                break;
            next = sym->next;
            DumpSymbol( sym );
        }
    }
}

#endif
