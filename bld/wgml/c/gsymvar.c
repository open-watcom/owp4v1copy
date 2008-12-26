/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Implements symbolic variables (tables and access routines)
*               still incomplete
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"



/***************************************************************************/
/*  init_dict      initialize dictionary pointer                           */
/***************************************************************************/

void    init_dict( symvar * * dict )
{

    *dict = NULL;
    return;
}

/***************************************************************************/
/*  free_dict   free all symbol dictionary entries                         */
/***************************************************************************/

void    free_dict( symvar * * dict )
{
    symvar  *   wk;
    symvar  *   wkn;
    symsub  *   ws;
    symsub  *   wsn;

    wk = *dict;
    while( wk != NULL ) {
        ws = wk->subscripts;
        while( ws != NULL ) {
            mem_free( ws->value );
            wsn = ws->next;
            mem_free( ws );
            ws = wsn;
        }
        wkn = wk->next;
        mem_free( wk );
        wk = wkn;
    }
    *dict = NULL;
    return;
}


/***************************************************************************/
/*  search symbol and subscript entry in specified  dictionary             */
/*  fills symsub structure pointer if found                                */
/*                                                                         */
/***************************************************************************/

int find_symvar( symvar * * dict, char * name, sub_index sub, symsub * * symsubval )
{
    symvar  *   wk;
    symsub  *   ws;
    int         rc = 0;

    *symsubval = NULL;
    wk = *dict;
    while( wk != NULL) {
        if( !strcmp( wk->name, name ) ) {
            if( wk->flags & deleted ) {
                break;                  // symbol name is deleted
            }
            rc = 1;                     // symbol name found
            break;
        }
        wk = wk->next;
    }
    if( rc ) {
        *symsubval = wk->subscripts;    // return at least first subscript
        if( wk->flags & subscripted ) {
            rc = 1;                     // name is found
            ws = wk->subscripts;
            while( ws != NULL ) {
                if( sub == ws->subscript ) {
                    *symsubval = ws;    // subscript found
                    rc = 2;
                    break;
                }
                ws = ws->next;
            }
        } else {
            rc = 2;                     // not subscripted -> all found
        }
    }
    return( rc );
}

/***************************************************************************/
/*  check subscript for allowed range                                      */
/***************************************************************************/

bool    check_subscript( sub_index sub )
{

    if( sub != no_subscript ) {
        if( (sub < min_subscript) || (sub > max_subscript) ) {
            // SC--076 Subscript index must be between -1000000 and 1000000
            out_msg( "ERR_SUBSCRIPT_OUT_OF_RANGE (-1000000 - +1000000) %d\n",
                     sub);
            err_count++;
            show_include_stack();
            return( false );
        }
    }
    return( true );
}

/***************************************************************************/
/*  add_symvar_sub add value and subscript to base symbol entry            */
/*           the subscripts are added in ascending order                   */
/***************************************************************************/

static bool add_symvar_sub( symvar * var, char * val, sub_index sub )
{
    symsub  *   newsub;
    symsub  *   ws;
    symsub  *   wsv;

    if( sub != no_subscript ) {
        if( !check_subscript( sub ) ) {
            return( false );
        } else {
            var->subscript_used++;
            var->flags |= subscripted;
        }
    }
    newsub            = mem_alloc( sizeof( symsub ) );
    newsub->next      = NULL;
    newsub->base      = var;
    newsub->subscript = sub;
    newsub->value     = mem_alloc( strlen( val ) + 1 );
    strcpy_s( newsub->value, strlen( val ) + 1, val );

/*
 * insert subscript in ascending sort order
 */
    ws  = var->subscripts;
    if( ws == NULL || (sub < ws->subscript) ) {
        newsub->next    = var->subscripts;
        var->subscripts = newsub;
    } else {
        while( (ws != NULL) ) {
            if( sub > ws->subscript ) {
                wsv = ws;
                ws  = ws->next;
            } else {
                break;
            }
        }
        newsub->next = ws;
        wsv->next    = newsub;
    }
    return( true );
}

/***************************************************************************/
/*  add_symsym  add symbol base entry                                      */
/***************************************************************************/

static void add_symsym( symvar * * dict, char * name, sym_flags f, symvar * * n )
{
    symvar  *   new;
    int         k;

    new = mem_alloc( sizeof( symvar ) );

    for( k = 0; k < SYM_NAME_LENGTH; k++ ) {
       new->name[k] = name[k];
       if( !name[k] ) {
          break;
       }
    }
    for( ; k <= SYM_NAME_LENGTH; k++ ) {
       new->name[k] = '\0';
    }
    new->next = NULL;
    new->subscript_used = 0;
    new->subscripts = NULL;
    new->flags = f & ~(deleted | subscripted);

    *n = new;
    new->next = *dict;
    *dict = new;
    return;
}


/***************************************************************************/
/*  add_symvar  add symbol with subscript and value                        */
/***************************************************************************/

int add_symvar( symvar * * dict, char * name, char * val, sub_index subscript, sym_flags f )
{
    symvar  *   new = NULL;
    symsub  *   newsub = NULL;
    int     rc;
    bool    ok;

    if( !check_subscript( subscript ) ) {
        rc = 3;
    } else {
        rc = find_symvar( dict, name, subscript, &newsub );
        switch ( rc ) {
        case 0 :                        // nothing found
            add_symsym( dict, name, f, &new );
            ok = add_symvar_sub( new, val, subscript );
            if( !ok ) {
                rc = 3;
            }
            break;
        case 1 :                        // symbol found, but not subscript
            ok = add_symvar_sub( newsub->base, val, subscript );
            if( !ok ) {
                rc = 3;
            }
            break;
        case 2 :              // symbol + subscript found, or not subscripted
            if( !strcmp( newsub->value, val ) ) {
                ;                       // nothing to do value is unchanged
            } else {
                if( strlen( newsub->value ) < strlen( val ) ) {
                    mem_free( newsub->value );  // need more room
                    newsub->value = mem_alloc( strlen( val ) + 1 );
                }
                strcpy_s( newsub->value, strlen( val ) + 1, val );
            }
            break;
        default:
            break;
        }
    }
    return( rc );
}

/***************************************************************************/
/*  print_sym_dict  output all of the symbol dictionary                    */
/***************************************************************************/

void    print_sym_dict( symvar * dict )
{
    symvar          *   wk;
    symsub          *   ws;
    int                 symcnt;
    int                 symsubcnt;
    int                 len;
    static const char   fill[ 11 ] = "          ";

    symcnt      = 0;
    symsubcnt   = 0;
    wk          = dict;
    out_msg( "\nList of symbolic variables:\n" );
    while( wk != NULL ) {
        len = strlen( wk->name );

        out_msg( "Variable='%s'%s flags=%s%s subscript_used=%d", wk->name,
                 &fill[ len ], wk->flags & deleted ? "deleted " : "",
                 wk->flags & local_var ? "local " : "",
                 wk->subscript_used );
        ws = wk->subscripts;
        if( wk->flags & subscripted ) {
            symsubcnt++;
            out_msg( "\n" );
        } else {
            symcnt++;
        }
        while( ws != NULL ) {
            if( wk->flags & subscripted ) {
                out_msg( "   subscript= %8ld value='%s'\n",
                        ws->subscript, ws->value );
            } else {
                out_msg(" value='%s'\n", ws->value );
            }
            ws = ws->next;
        }
        wk = wk->next;
    }
    out_msg( "\nUnsubscripted symbols defined: %d\n", symcnt );
    out_msg( "Subscripted   symbols defined: %d\n", symsubcnt );
    return;
}

