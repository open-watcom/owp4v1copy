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
* Description:  Output OBJ queues routines
*
****************************************************************************/

#include "asmglob.h"

#include "asmalloc.h"
#include "mangle.h"
#include "asmins.h"
#include "directiv.h"
#include "queue.h"
#include "queues.h"

#include "myassert.h"

typedef struct queuenode {
    void *next;
    void *data;
} queuenode;

static qdesc   *LnameQueue  = NULL;   // queue of LNAME structs
static qdesc   *PubQueue    = NULL;   // queue of pubdefs
static qdesc   *GlobalQueue = NULL;   // queue of global / externdefs
static qdesc   *AliasQueue  = NULL;   // queue of aliases
static qdesc   *LinnumQueue = NULL;   // queue of linnum_data structs

static void QAddQItem( qdesc **queue, queuenode *node )
/*****************************************************/
{
    if( *queue == NULL ) {
        *queue = AsmAlloc( sizeof( qdesc ) );
        QInit( *queue );
    }
    QEnqueue( *queue, node );
}

static void QAddItem( qdesc **queue, void *data )
/***********************************************/
{
    struct queuenode    *node;

    node = AsmAlloc( sizeof( queuenode ) );
    node->data = data;
    if( *queue == NULL ) {
        *queue = AsmAlloc( sizeof( qdesc ) );
        QInit( *queue );
    }
    QEnqueue( *queue, node );
}

static long QCount( qdesc *q )
/****************************/
/* count the # of entries in the queue, if the retval is -ve we have an error */
{
    long        count = 0;
    queuenode   *node;

    if( q == NULL )
        return( 0 );
    for( node = q->head; node != NULL; node = node->next ) {
        if( ++count < 0 ) {
            return( -1 );
        }
    }
    return( count );
}

void AddPublicData( dir_node *dir )
/*********************************/
{
    QAddItem( &PubQueue, dir );
}

uint GetPublicData(
    uint *seg,
    uint *grp,
    char *cmd,
    char ***NameArray,
    struct pubdef_data **data,
    bool *need32,
    bool first )
/****************************/
{
    static struct queuenode    *start;
    struct queuenode           *curr;
    struct asm_sym             *sym;
    dir_node                   *pub;
    uint                       count;
    uint                       i;
    struct pubdef_data         *d;
    struct asm_sym             *curr_seg;

    if( PubQueue == NULL )
        return( 0 );
    if( first )
        start = PubQueue->head;
    if( start == NULL )
        return( 0 );

    *need32 = FALSE;
    *cmd = CMD_PUBDEF;
    sym = (asm_sym *)start->data;
    if( sym->state == SYM_UNDEFINED ) {
        AsmErr( SYMBOL_NOT_DEFINED, sym->name );
        return( 0 );
    }
    curr_seg = sym->segment;
    if( curr_seg == NULL ) {      // absolute symbol ( without segment )
        *seg = 0;
        *grp = 0;
    } else {
        *seg = GetSegIdx( curr_seg );
        *grp = GetGrpIdx( GetGrp( curr_seg ) );
    }
    curr = start;
    for( count = 0; curr != NULL; curr = curr->next ) {
        if( count == MAX_PUB_SIZE )  // don't let the records get too big
            break;
        sym = (asm_sym *)curr->data;
        if( sym->segment != curr_seg )
            break;
        if( sym->state == SYM_PROC ) {
            if( !sym->public ) {
                if( *cmd == CMD_PUBDEF ) {
                    if( curr != start )
                        break;
                    *cmd = CMD_STATIC_PUBDEF;
                } /* else we are just continuing a static def. */
            } else {
                if( *cmd == CMD_STATIC_PUBDEF ) {
                    break;
                }
            }
        }
        count++;
        /* if we don't get to here, this entry is part of the next pubdef */
    }
    *NameArray = AsmAlloc( count * sizeof( char * ) );
    for( i = 0; i < count; i++ ) {
        (*NameArray)[i] = NULL;
    }

    *data = d = AsmAlloc( count * sizeof( struct pubdef_data ) );

    for( curr = start, i = 0; i < count; i++, curr = curr->next ) {
        sym = (asm_sym *)curr->data;
        if( sym->segment != curr_seg )
            break;
        if( sym->offset > 0xffffUL )
            *need32 = TRUE;

        (*NameArray)[i] = Mangle( sym, NULL );

        d[i].name = i;
        /* No namecheck is needed by name manager */
        if( sym->state != SYM_CONST ) {
            d[i].offset = sym->offset;
        } else {
            pub = (dir_node *)sym;
            if( pub->e.constinfo->data[0].token != T_NUM  ) {
                AsmWarn( 2, PUBLIC_CONSTANT_NOT_NUMERIC );
                d[i].offset = 0;
            } else {
                d[i].offset = pub->e.constinfo->data[0].value;
            }
        }
        d[i].type.idx = 0;
    }
    start = curr;
    return( count );
}

static void FreePubQueue( void )
/******************************/
{
    if( PubQueue != NULL ) {
        while( PubQueue->head != NULL ) {
            AsmFree( QDequeue( PubQueue ) );
        }
        AsmFree( PubQueue );
    }
}

void AddAliasData( char *data )
/*****************************/
{
    QAddItem( &AliasQueue, data );
}

char *GetAliasData( bool first )
/******************************/
{
    static queuenode    *node;
    char                *p;

    if( AliasQueue == NULL )
        return( NULL );
    if( first )
        node = AliasQueue->head;
    if( node == NULL )
        return( NULL );
    p = node->data ;
    node = node->next;
    return( p );
}

static void FreeAliasQueue( void )
/********************************/
{
    if( AliasQueue != NULL ) {
        while( AliasQueue->head != NULL ) {
            queuenode   *node;

            node = QDequeue( AliasQueue );
            AsmFree( node->data );
            AsmFree( node );
        }
        AsmFree( AliasQueue );
    }
}

void AddLnameData( dir_node *dir )
/********************************/
{
    QAddItem( &LnameQueue, dir );
}

direct_idx FindLnameIdx( char *name )
/***********************************/
{
    queuenode           *node;
    dir_node            *dir;

    if( LnameQueue == NULL )
        return( LNAME_NULL);

    for( node = LnameQueue->head; node != NULL; node = node->next ) {
        dir = (dir_node *)node->data;
        if( dir->sym.state != SYM_CLASS_LNAME )
            continue;
        if( stricmp( dir->sym.name, name ) == 0 ) {
            return( dir->e.lnameinfo->idx );
        }
    }
    return( LNAME_NULL );
}

char *GetLname( direct_idx idx )
/******************************/
{
    queuenode           *node;
    dir_node            *dir;

    if( LnameQueue == NULL )
        return( NULL);

    for( node = LnameQueue->head; node != NULL; node = node->next ) {
        dir = (dir_node *)node->data;
        if( dir->sym.state != SYM_CLASS_LNAME )
            continue;
        if( dir->e.lnameinfo->idx == idx ) {
            return( dir->sym.name );
        }
    }
    return( NULL );
}

unsigned GetLnameData( char **data )
/**********************************/
{
    char            *lname = NULL;
    unsigned        total_size = 0;
    queuenode       *curr;
    dir_node        *dir;
    int             len;

    if( LnameQueue == NULL )
        return( 0 );

    for( curr = LnameQueue->head; curr != NULL ; curr = curr->next ) {
        dir = (dir_node *)(curr->data);
        myassert( dir != NULL );
        total_size += 1 + strlen( dir->sym.name );
    }

    if( total_size > 0 ) {
        int     i = 0;

        lname = AsmAlloc( total_size * sizeof( char ) + 1 );
        for( curr = LnameQueue->head; curr != NULL ; curr = curr->next ) {
            dir = (dir_node *)(curr->data);

            len = strlen( dir->sym.name );
            lname[i] = (char)len;
            i++;
            strcpy( lname+i, dir->sym.name );
            //For the Q folks... strupr( lname+i );
            i += len; // overwrite the null char
        }
    }
    *data = lname;
    return( total_size );
}

static void FreeLnameQueue( void )
/********************************/
{
    dir_node *dir;
    queuenode *node;

    if( LnameQueue != NULL ) {
        while( LnameQueue->head != NULL ) {
            node = QDequeue( LnameQueue );
            dir = (dir_node *)node->data;
            if( dir->sym.state == SYM_CLASS_LNAME ) {
                AsmFree( dir->e.lnameinfo );
                AsmFree( dir->sym.name );
                AsmFree( dir );
            }
            AsmFree( node );
        }
        AsmFree( LnameQueue );
    }
}

void AddGlobalData( dir_node *dir )
/*********************************/
{
    QAddItem( &GlobalQueue, dir );
}

void GetGlobalData( void )
/************************/
/* turn the globals into either externs or publics as appropriate */
{
    queuenode           *curr;
    struct asm_sym      *sym;

    if( GlobalQueue == NULL )
        return;
    for( ; ; ) {
        curr = (queuenode *)QDequeue( GlobalQueue );
        if( curr == NULL )
            break;
        sym = (asm_sym *)curr->data;
        if( sym->state == SYM_UNDEFINED ) {
            dir_change( (dir_node *)curr->data, TAB_EXT );
            AsmFree( curr );
        } else {
            /* make this record a pubdef */
            sym->public = TRUE;
            QAddQItem( &PubQueue, curr );
        }
    }
    AsmFree( GlobalQueue );
    GlobalQueue = NULL;
}

void AddLinnumData( struct line_num_info *data )
/**********************************************/
{
    QAddItem( &LinnumQueue, data );
}

int GetLinnumData( struct linnum_data **ldata, bool *need32 )
/***********************************************************/
{
    queuenode               *node;
    struct line_num_info    *next;
    int                     count, i;

    count = QCount( LinnumQueue );
    if( count == 0 )
        return( count );
    *need32 = FALSE;
    *ldata = AsmAlloc( count * sizeof( struct linnum_data ) );
    for( i = 0; i < count; i++ ) {
        node = QDequeue( LinnumQueue );
        next = (struct line_num_info *)(node->data);
        if( *ldata != NULL ) {
            (*ldata)[i].number = next->number;
            (*ldata)[i].offset = next->offset;
            if( next->offset > 0xffffUL ) {
                *need32 = TRUE;
            }
        }
        AsmFree( next );
        AsmFree( node );
    }
    AsmFree( LinnumQueue );
    LinnumQueue = NULL;
    return( count );
}

void FreeAllQueues( void )
/************************/
{
    FreePubQueue();
    FreeAliasQueue();
    FreeLnameQueue();
}
