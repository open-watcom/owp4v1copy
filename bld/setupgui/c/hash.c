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


/*
* Comments: Hash table to spped up install search
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gui.h"
#include "guiutil.h"
#include "hash.h"

typedef struct hash_element_struct {
    struct hash_element_struct  *next;
    hash_key                    key;
    hash_data                   data;
} hash_element;

typedef struct hash_table_struct {
    unsigned int        size;
    hash_key_cmp        cmp_func;
    hash_element        *table[1];
} hash_table;

static unsigned int     hashKey( unsigned int size, hash_key k );

hash_handle     HashInit( int size, hash_key_cmp func ) {
/*******************************************************/

    hash_table  *ht;

    assert( size > 0 );
    assert( func );

    ht = GUIAlloc( sizeof( hash_table ) + ( sizeof( hash_element * ) * size ) );
    if( !ht ) return( NULL );
    memset( ht, 0, sizeof( hash_table ) + ( sizeof( hash_element * ) * size ) );
    ht->size = size;
    ht->cmp_func = func;
    return( ht );
}

int             HashInsert( hash_handle h, hash_key k, hash_data d ) {
/********************************************************************/

    hash_table          *ht = (hash_table *)h;
    unsigned int        i;
    hash_element        *he;

    assert( h );
    assert( k );
    assert( d );

    he = GUIAlloc( sizeof( hash_element ) );
    if( !he ) return( 0 );
    i = hashKey( ht->size, k );
    he->data = d;
    GUIStrDup( k, &(he->key) );
    he->next = ht->table[i];
    ht->table[i] = he;
    return( 1 );
}


hash_data       HashFind( hash_handle h, hash_key k ) {
/*****************************************************/

    hash_table          *ht = (hash_table *)h;
    unsigned int        i;
    hash_element        *he;

    assert( h );
    assert( k );

    i = hashKey( ht->size, k );
    he = ht->table[i];

    while( he ) {
        if( !ht->cmp_func( k, he->key ) ) break;
        he = he->next;
    }

    if( he ) {
        return( he->data );
    } else {
        return( NULL );
    }
}

void            HashFini( hash_handle h ) {
/*****************************************/

    hash_table          *ht = (hash_table *)h;
    unsigned int        i;
    hash_element        *he;
    hash_element        *tmp;

    assert( h );

    for( i = 0; i < ht->size; i++ ) {
        he = ht->table[i];
        while( he ) {
            tmp = he;
            he = he->next;
            GUIFree( tmp->key );
            GUIFree( tmp );
        }
    }
    GUIFree( ht );
}


static unsigned int     hashKey( unsigned int size, hash_key k ) {
/****************************************************************/

    unsigned long       hash = 0;

    assert( k );
    assert( size > 0 );

    while( *k ) {
        hash = ( hash << 4 ) + *k;
        if ( hash & 0xffff0000 ) {
            hash += hash >> 24;
        }
        k++;
    }
    hash = hash % size;
    assert( hash < size );
    return( hash );
}
