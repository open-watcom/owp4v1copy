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


#include <stdlib.h>
#include <string.h>
#include "asmsym.h"
#include "asmins.h"
#include "asmalloc.h"
#include "asmerr.h"
#include "asmglob.h"
#include "asmdefs.h"

#ifdef __USE_BSD
#define stricmp strcasecmp
#endif

#ifdef _WASM_

#include "myassert.h"
#include "directiv.h"

#include "hash.h"
struct asm_sym *sym_table[ HASH_TABLE_SIZE ] = { NULL };
/* initialize the whole table to null pointers */

#else

struct asm_sym  *AsmSymHead;

static unsigned short CvtTable[] = {
    T_BYTE,   // INT1
    T_WORD,   // INT2
    T_DWORD,  // INT4
    T_FWORD,  // INT6
    T_QWORD,  // INT8
    T_DWORD,  // FLOAT4
    T_QWORD,  // FLOAT8
    T_TBYTE,  // FLOAT10
    T_NEAR,   // NEAR2
    T_NEAR,   // NEAR4
    T_FAR,    // FAR2
    T_FAR,    // FAR4
};

#endif

extern void     AsmError( int );

char *InitAsmSym( struct asm_sym *sym, char *name )
/************************************************/
{
    sym->name = AsmAlloc( strlen( name ) + 1 );
    if( sym->name != NULL ) {
        strcpy( sym->name, name );
        sym->next = NULL;
        sym->fixup = NULL;
#ifdef _WASM_
        sym->grpidx = 0;
        sym->segidx = 0;
        sym->offset = 0;
        sym->public = FALSE;
        sym->first_size = 0;
        sym->first_length = 0;
        sym->total_size = 0;
        sym->total_length = 0;
        sym->mangler = NULL;
        sym->state = SYM_UNDEFINED;
        sym->mem_type = EMPTY;
#else
        sym->addr = 0;
        sym->state = AsmQueryExternal( sym->name );
        if( sym->state == SYM_UNDEFINED ) {
            sym->mem_type = EMPTY;
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

#ifdef _WASM_
    sym = AsmAlloc( sizeof( dir_node ) );
#else
    sym = AsmAlloc( sizeof( struct asm_sym ) );
#endif
    if( sym != NULL ) {
        if( InitAsmSym( sym, name ) == NULL ) {
            AsmFree( sym );
            return( NULL );
        }
#ifdef _WASM_
        ((dir_node *)sym)->next = NULL;
        ((dir_node *)sym)->prev = NULL;
        ((dir_node *)sym)->line = 0;
        ((dir_node *)sym)->e.seginfo = NULL;
#endif
    }
    return sym;
}

struct asm_sym **AsmFind( char *name )
/***********************************/
/* find a symbol in the symbol table, return NULL if not found */
{
    struct asm_sym      **sym;

#ifdef _WASM_
    sym = &sym_table[ hashpjw( name ) ];
#else
    sym = &AsmSymHead;
#endif
    for( ; *sym; sym = &((*sym)->next) ) {
        if( stricmp( name, (*sym)->name ) == 0 ) return( sym );
    }
    return( sym );
}

struct asm_sym *AsmLookup( char *name )
/*************************************/
{
    struct asm_sym      **sym_ptr;
    struct asm_sym      *sym;
#ifdef _WASM_
    char                is_current_loc = FALSE;
#endif

    if( strlen( name ) > MAX_ID_LEN ) {
        AsmError( LABEL_TOO_LONG );
        return NULL;
    }

    sym_ptr = AsmFind( name );
#ifdef _WASM_
    if( strcmp( name, "$" ) ==  0 ) {
        is_current_loc = TRUE;
        /* current address operator */
        if( *sym_ptr != NULL ) {
            GetSymInfo( *sym_ptr );
        }
    }
#endif
    if( *sym_ptr != NULL )
        return( *sym_ptr );

    sym = AllocASym( name );
    if( sym != NULL ) {
        sym->next = *sym_ptr;
        *sym_ptr = sym;

#ifdef _WASM_
        if( is_current_loc ) {
            GetSymInfo( sym );
            sym->state = SYM_INTERNAL;
            sym->mem_type = T_NEAR;
            return( sym );
        }
#else
        sym->addr = Address;
#endif
    } else {
        AsmError( NO_MEMORY );
    }
    return( sym );
}

static void FreeASym( struct asm_sym *sym )
{
#ifdef _WASM_
    struct asmfixup     *fix;

    for( ;; ) {
        fix = sym->fixup;
        if( fix == NULL ) break;
        sym->fixup = fix->next;
        AsmFree( fix );
    }
#endif
    AsmFree( sym->name );
    AsmFree( sym );
}

#ifdef _WASM_

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
        if( *sym_ptr != NULL ) return( ERROR );

        sym->next = *sym_ptr;
        *sym_ptr = sym;
    }
    return( NOT_ERROR );
}

struct asm_sym *AsmAdd( struct asm_sym *sym )
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

#endif

struct asm_sym *AsmGetSymbol( char *name )
/****************************************/
{
    struct asm_sym  **sym_ptr;

    sym_ptr = AsmFind( name );
    return( *sym_ptr );
}

void AsmSymFini()
{
    struct asm_sym      *sym;
#ifdef _WASM_
    dir_node            *dir;
    unsigned            i;

    FreePubQueue();
    FreeAliasQueue();
    FreeLnameQueue();

    /* now free the symbol table */
    for( i = 0; i < HASH_TABLE_SIZE; i++ ) {
        struct asm_sym  *next;
        next = sym_table[i];
        for( ;; ) {
            sym = next;
            if( sym == NULL ) break;
            dir = (dir_node *)sym;
            next = sym->next;
            FreeInfo( dir );
            FreeASym( sym );
        }
    }

#else
    struct asmfixup     *fix;

    for( ;; ) {
        sym = AsmSymHead;
        if( sym == NULL ) break;
        AsmSymHead = sym->next;
        FreeASym( sym );
    }
    for( ;; ) {
        fix = FixupHead;
        if( fix == NULL ) break;
        FixupHead = fix->next;
        AsmFree( fix );
    }
#endif
}
