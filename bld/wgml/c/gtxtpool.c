/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML utility functions for alloc / free / reuse of
*                           different structs
*
*               add_text_chars_to_pool  prepare reuse of text_chars instance(s)
*               alloc_text_chars        create a text_chars instance
*               alloc_text_line         create a text_line instance
*               add_text_line_to_pool   prepare reuse of a text_line instance
*               add_tag_cb_to_pool      nested tag cb
*               alloc_tag_cb            nested tag cb
*               free_pool_storage       do free for all pools
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*  allocate / reuse and init a text_chars instance                        */
/*      optionally fill in text                                            */
/***************************************************************************/

text_chars  * alloc_text_chars( char * p, size_t cnt, uint8_t font_num )
{
    text_chars   *   curr;
    text_chars   *   prev;

    curr = text_pool;
    while( (curr != NULL) && (curr->length <= cnt) ) {
        prev = curr;
        curr = curr->next;
    }
    if( curr != NULL ) {                // we found one large enough
        if( curr == text_pool ) {       // first is large enough
            text_pool = curr->next;
        } else {
            prev->next = curr->next;    // unchain curr
        }
    } else {                            // no one large enough found
        curr = mem_alloc( sizeof( *curr ) + cnt );
        curr->length = cnt;             // set max text size
    }

    curr->prev = NULL;
    curr->next = NULL;
    curr->type = norm;
    curr->font_number = font_num;
    curr->width = 0;
    if( p != NULL ) {                   // text supplied
        memcpy_s(curr->text, cnt + 1, p, cnt ); // yes copy text
        curr->count = cnt;              // set current size
    } else {
        curr->count = 0;                // init current size
        curr->text[0] = 0;
    }
    curr->text[cnt] = 0;

    return( curr );
}


/***************************************************************************/
/*  add text_chars instance(s) to free pool for reuse                      */
/***************************************************************************/

void    add_text_chars_to_pool( text_line * a_line )
{
    text_chars      *   tw;

    if( (a_line == NULL) || (a_line->first == NULL) ) {
        return;
    }
    // free text_chars in pool only have a valid next ptr
    if( text_pool == NULL ) {
        text_pool = a_line->first;
    } else {
        for( tw = text_pool; tw->next != NULL; tw = tw->next )
            ;                           // empty
        tw->next = a_line->first;
    }
}


/***************************************************************************/
/*  allocate / reuse a text_line  instance                                 */
/***************************************************************************/

text_line   * alloc_text_line( void )
{
    text_line    *   curr;

    curr = line_pool;
    if( curr != NULL ) {                // there is one to use
        line_pool = curr->next;
    } else {                            // pool is empty
        curr = mem_alloc( sizeof( text_line ) );
    }

    curr->next = NULL;
    curr->first = NULL;
    curr->last = NULL;

    return( curr );
}


/***************************************************************************/
/*  add a text_line instance to free pool for reuse                        */
/***************************************************************************/

void    add_text_line_to_pool( text_line * a_line )
{

    if( a_line == NULL ) {
        return;
    }

    a_line->next = line_pool;
    line_pool = a_line;
}


/***************************************************************************/
/*  allocate / reuse a tag_cb     instance                                 */
/***************************************************************************/

tag_cb  * alloc_tag_cb( void )
{
    tag_cb  *   curr;

    curr = tag_pool;
    if( curr != NULL ) {                // there is one to use
        tag_pool = curr->prev;
    } else {                            // pool is empty
        curr = mem_alloc( sizeof( tag_cb ) );
    }

    curr->prev = NULL;
    curr->c_tag = t_NONE;

    return( curr );
}


/***************************************************************************/
/*  add a tag_cb    instance to free pool for reuse                        */
/***************************************************************************/

void    add_tag_cb_to_pool( tag_cb * a_cb )
{

    if( a_cb == NULL ) {
        return;
    }

    a_cb->prev = tag_pool;
    tag_pool = a_cb;
}

/***************************************************************************/
/*  free all elements of our storage pools                                 */
/***************************************************************************/

void    free_pool_storage( void )
{
    tag_cb      *cwk;
    tag_cb      *cw = tag_pool;
    text_line   *lwk;
    text_line   *lw = line_pool;
    text_chars  *wk;
    text_chars  *w = text_pool;

    while( w != NULL ) {
       wk = w->next;
       mem_free( w );
       w = wk;
    }

    w = t_line.first;
    while( w != NULL ) {
       wk = w->next;
       mem_free( w );
       w = wk;
    }

    while( lw != NULL ) {
       lwk = lw->next;
       mem_free( lw );
       lw = lwk;
    }
    while( cw != NULL ) {
       cwk = cw->prev;
       mem_free( cw );
       cw = cwk;
    }
}

