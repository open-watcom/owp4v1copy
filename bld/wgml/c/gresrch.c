/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  utility functions for wgml research related
*                                          i.e. non production formatting
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include "wgml.h"
#include "gvars.h"


typedef struct taglist {
    struct  taglist *   nxt;
    long                count;
    char                tagname[16];
} taglist;

static  taglist *   tags = NULL;        // list of found gml tags
static  taglist *   scrkws = NULL;      // list of found scr keywords



void printf_research( char * msg, ... )
{
    va_list args;

    va_start( args, msg );
    vfprintf_s( stdout, msg, args );
    va_end( args );
}




/***************************************************************************/
/*  Add and/or count GMLtag                                                */
/***************************************************************************/

void add_GML_tag_research( char * tag )
{
    taglist *   wk = tags;
    taglist *   new;

    while( wk ) {
        if( !stricmp( tag, wk->tagname ) ) {
            wk->count++;
            return;
        }
        if( !wk->nxt ) {
            break;
        }
        wk = wk->nxt;
    }
    new = mem_alloc( sizeof( taglist ) );
    if( wk == NULL ) {
        wk = new;
        tags = new;
    } else {
        wk->nxt = new;
    }
    new->nxt = NULL;
    strcpy_s( new->tagname, sizeof( new->tagname ), tag );
    strupr( new->tagname );
    new->count = 1;
}

/***************************************************************************/
/*  print found GMLtags with usagecount                                    */
/***************************************************************************/

void    print_GML_tags_research( void )
{
    taglist *   wk = tags;

    printf_research( "\nGML tag / macro list sorted by first occurrence\n\n" );
    while( wk ) {
        printf_research("%6ld  :%s\n", wk->count, wk->tagname );
        wk = wk->nxt;
    }
}

/***************************************************************************/
/*  free storage of GMLtaglist                                             */
/***************************************************************************/

void    free_GML_tags_research( void )
{
    taglist *   wk = tags;
    taglist *   wk1;

    while( wk ) {
        wk1 = wk;
        wk = wk->nxt;
        mem_free( wk1) ;
    }
    tags = NULL;
}

/***************************************************************************/
/*  add and/or count SCR keyword                                           */
/***************************************************************************/

void    add_SCR_tag_research( char * tag )
{
    taglist *   wk = scrkws;
    taglist *   new;

    while( wk ) {
        if( !stricmp( tag, wk->tagname ) ) {
            wk->count++;
            return;
        }
        if( !wk->nxt ) {
            break;
        }
        wk = wk->nxt;
    }
    new = mem_alloc( sizeof( taglist ) );
    if( wk == NULL ) {
        wk = new;
        scrkws = new;
    } else {
        wk->nxt = new;
    }
    new->nxt = NULL;
    strcpy_s( new->tagname, sizeof( new->tagname ), tag );
    strlwr( new->tagname );
    new->count = 1;
}

/***************************************************************************/
/*  print found SCR keywords and usage count                               */
/***************************************************************************/

void    print_SCR_tags_research( void )
{
    taglist *   wk = scrkws;

    printf_research(
        "\nScript controlword / macro list sorted by first occurrence\n\n" );
    while( wk ) {
        printf_research("%6ld  .%s\n", wk->count, wk->tagname );
        wk= wk->nxt;
    }
}

/***************************************************************************/
/*  free SCR keyword list                                                  */
/***************************************************************************/

void    free_SCR_tags_research( void )
{
    taglist *   wk = scrkws;
    taglist *   wk1;

    while( wk ) {
        wk1 = wk;
        wk = wk->nxt;
        mem_free( wk1) ;
    }
    tags = NULL;
}

/***************************************************************************/
/*  testoutput of words belonging to a line with positional info           */
/***************************************************************************/

void    test_out_t_line( text_line  * a_line )
{
    text_chars  *   tw;
    char            buf[BUF_SIZE];

    if( a_line == NULL ) {
        return;
    }

    tw = a_line->first;
    out_msg( "\n   y_address:%d\n", a_line->y_address );
    while( tw != NULL ) {
        snprintf( buf, buf_size,
                  "font:%d x:%d-%d width:%d count:%d text:'%.*s'\n",
                  tw->font_number, tw->x_address, tw->x_address + tw->width,
                  tw->width, tw->count, tw->count, tw->text );
        out_msg( buf );
        tw = tw->next;
    }
}
