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


//
// BLDOBJ.C     : build object name from auxiliary information
//

#include "ftnstd.h"
#include "symbol.h"
#include "intcnv.h"
#include "wf77defs.h"
#include "wf77aux.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//=================== Back End Code Generation Routines ====================

extern  unsigned long           BETypeLength(cg_type);

//=========================================================================

extern  aux_info                *AuxLookupName(char *,uint);
extern  aux_info                *AuxLookup(sym_id);

extern  aux_info                FortranInfo;


static uint BldObjString( aux_info *aux, char *name, uint len,
                          char *buff, uint buff_size ) {
//============================================================

    char        *pattern;
    uint        idx;
    uint        count;
    uint        i;

    pattern = aux->object_name;
    idx = 0;
    for(;;) {
        if( idx >= buff_size - 1 ) break;
        if( *pattern == NULLCHAR ) break;
        if( *pattern == '^' ) {
            count = len;
            if( idx + count > buff_size - 1 ) {
                count = buff_size - idx - 1;
            }
            memcpy( &buff[idx], name, count );
            idx += count;
        } else if( *pattern == '*' ) {
            count = len;
            if( idx + count > buff_size - 1 ) {
                count = buff_size - idx - 1;
            }

            if( aux->sym_len > 0 ) {
                memcpy( &buff[idx], aux->sym_name, aux->sym_len );
            } else {
                memcpy( &buff[idx], name, count );
            }

            idx += count;
        } else if( *pattern == '!' ) {
            count = len;
            if( idx + count > buff_size - 1 ) {
                count = buff_size - idx - 1;
            }
            for( i = 0; i < count; ++i ) {
                buff[idx+i] = tolower( name[i] );
            }
            idx += count;
        } else if( *pattern == '#' ) {

            int         args_size;
            pass_by     *arg;
            char        ibuff[MAX_INT_SIZE];

            args_size = 0;
            for( arg = aux->arg_info; arg != NULL; arg = arg->link ) {
                if( arg->info & ARG_SIZE_1 ) {
                    args_size += 1;
                } else if( arg->info & ARG_SIZE_2 ) {
                    args_size += 2;
                } else if( arg->info & ARG_SIZE_4 ) {
                    args_size += 4;
                } else if( arg->info & ARG_SIZE_8 ) {
                    args_size += 8;
                } else if( arg->info & ARG_SIZE_16 ) {
                    args_size += 16;
                } else if( arg->info & PASS_BY_REFERENCE ) {
                    args_size += BETypeLength( T_POINTER );
                }
            }
            utoa( args_size, ibuff, 10 );
            for( i = 0; ibuff[i] != '\0'; ++i ) {
                if( idx + i == buff_size ) break;
                buff[idx+i] = ibuff[i];
            }
            idx += i;
        } else {
            buff[idx] = *pattern;
            ++idx;
        }
        ++pattern;
    }
    buff[idx] = NULLCHAR;
    return( idx );
}


uint    BldObjName( sym_id sym, char *buff, uint size ) {
//=======================================================

    aux_info    *aux;

    aux = AuxLookupName( sym->ns.name, sym->ns.name_len );
    if( aux == NULL ) {
        aux = &FortranInfo;
    }
    return( BldObjString( aux, sym->ns.name, sym->ns.name_len, buff, size ) );
}


uint    BldObjSym( sym_id sym, char *buff, uint size ) {
//=======================================================

    aux_info    *aux;

    aux = AuxLookup( sym );
    return( BldObjString( aux, sym->ns.name, sym->ns.name_len, buff, size ) );
}
