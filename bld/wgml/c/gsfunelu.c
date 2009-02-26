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
* Description:  WGML single letter functions &e'    &l'     &u'
*                                             exist, length, upper
*               Dummy for   &s'        &S'                         TBD
*                            subscript, Superscript
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*  script single letter function &e'         exist                        */
/*              returns   0 or 1 in result                                 */
/***************************************************************************/

char    *scr_single_func_e( char * in, char * end, char * * result )
{
    char            *   pchar;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;

    end   = end;

    pchar = scan_sym( in + 3 + (*(in + 3) == '&'), &symvar_entry, &var_ind );

    if( symvar_entry.flags & local_var ) {  // lookup var in dict
        rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                          var_ind, &symsubval );
    } else {
        rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                          &symsubval );
    }
    if( rc == 2 ) {
        **result = '1';                 // exists
    } else {
        **result = '0';                 // does not exist
    }
    *result  += 1;
    **result = '\0';

    if( *pchar == '.' ) {
        pchar++;                        // skip optional terminating dot
    }
    ProcFlags.substituted = true;       // something changed
    return( pchar );
}


/***************************************************************************/
/*  script single letter function &l'         length                       */
/*              returns   length of value or length of name in result      */
/***************************************************************************/

char    *scr_single_func_l( char * in, char * end, char * * result )
{
    char            *   pchar;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;

    end  = end;

    pchar = scan_sym( in + 3 + (*(in + 3) == '&'), &symvar_entry, &var_ind );

    if( symvar_entry.flags & local_var ) {  // lookup var in dict
        rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                          var_ind, &symsubval );
    } else {
        rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                          &symsubval );
    }
    if( rc == 2 ) {
        sprintf( *result, "%d", strlen( symsubval->value ) );
    } else {
        sprintf( *result, "%d", strlen( symvar_entry.name ) );
    }
    *result  += strlen( *result );
    **result = '\0';
    if( *pchar == '.' ) {
        pchar++;                        // skip optional terminating dot
    }
    ProcFlags.substituted = true;       // something changed
    return( pchar );
}


/***************************************************************************/
/*  script single letter function &u'         upper                        */
/*                                                                         */
/***************************************************************************/

char    *scr_single_func_u( char * in, char * end, char * * result )
{
    char            *   pchar;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;
    char            *   pval;

    end   = end;


    pchar = scan_sym( in + 3 + (*(in + 3) == '&'), &symvar_entry, &var_ind );

    if( symvar_entry.flags & local_var ) {  // lookup var in dict
        rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                          var_ind, &symsubval );
    } else {
        rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                          &symsubval );
    }
    if( rc == 2 ) {
        pval = symsubval->value;        // upper content of variable
    } else {
        pval =  symvar_entry.name;      // upper variable name
    }
    while( *pval ) {
        **result = toupper( *pval++ );
        *result += 1;
    }
    **result = '\0';

    if( *pchar == '.' ) {
        pchar++;                        // skip optional terminating dot
    }
    ProcFlags.substituted = true;       // something changed
    return( pchar );
}


/***************************************************************************/
/*  script single letter functions unsupported   process to comsume        */
/*                                               variable for scanning     */
/***************************************************************************/

static  char    *scr_single_func_unsupport( char * in, char * * result )
{

    if( input_cbs->fmflags & II_macro ) {
        out_msg( "WNG_FUNCTION invalid / unsupported &%c'\n"
                 "\t\t\tLine %d of macro '%s'\n", *(in + 1),
                 input_cbs->s.m->lineno,
                 input_cbs->s.m->mac->name );
        show_include_stack();
    } else {
        out_msg( "WNG_FUNCTION invalid / unsupported &%c'\n"
                 "\t\t\tLine %d of file '%s'\n", *(in + 1),
                 input_cbs->s.f->lineno,
                 input_cbs->s.f->filename );
    }

    // do nothing
    return( in + 3 );
}


/***********************************************************************/
/*  Some single letter functions are implemeted here                   */
/*                                                                     */
/*  functions used within the OW doc build system:                     */
/*   &e'  existance of variable 0 or 1                                 */
/*   &l'  length of variable content or if undefined length of name    */
/*   &u'  upper                                                        */
/*                                                                     */
/*   &s'  subscript    These are recognized, but processed as &u'      */
/*   &S'  superscript           TBD                                    */
/*                                                                     */
/*   other single letter functions are not used AFAIK                  */
/*                                                                     */
/***********************************************************************/

char    *scr_single_funcs( char * in, char * end, char * * result )
{
    char            *   pw;

    if( *(in + 2) == '\'' ) {
        switch( *(in + 1) ) {
        case  'e' :             // exist function
            pw = scr_single_func_e( in, end, result );
            break;
        case  'l' :             // length function
            pw = scr_single_func_l( in, end, result );
            break;
        case  's' :             // subscript
        case  'S' :             // superscript
            if( input_cbs->fmflags & II_macro ) {
                out_msg( "WNG_FUNCTION not yet implemented &%c'\n"
                         "\t\t\tLine %d of macro '%s'\n", *(in + 1),
                         input_cbs->s.m->lineno,
                         input_cbs->s.m->mac->name );
                show_include_stack();
            } else {
                out_msg( "WNG_FUNCTION not yet implemented &%c'\n"
                         "\t\t\tLine %d of file '%s'\n", *(in + 1),
                         input_cbs->s.f->lineno,
                         input_cbs->s.f->filename );
            }
            wng_count++;

            // fallthrough treat as upper for now       TDB

        case  'u' :             // upper function
            pw = scr_single_func_u( in, end, result );
            break;
        default:
            pw = scr_single_func_unsupport( in, result );
            wng_count++;
            break;
        }
    } else {
        out_msg( "ERR_Logic error in gsfunelu.c\n" );
        err_count++;
        g_suicide();
    }
    ProcFlags.substituted = true;
    return( pw );
}

