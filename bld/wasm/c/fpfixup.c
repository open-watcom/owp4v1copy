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

#include "directiv.h"
#include "asmfixup.h"
#include "mangle.h"
#include "fppatch.h"

#include "myassert.h"

static char *FPPatchName[] = {
#define pick_fp(enum,name,alt_name) name,
#include "fppatche.h"
#undef pick_fp
};

static char *FPPatchAltName[] = {
#define pick_fp(enum,name,alt_name) alt_name,
#include "fppatche.h"
#undef pick_fp
};

int AddFloatingPointEmulationFixup( const struct asm_ins ASMFAR *ins, bool secondary )
/************************************************************************************/
{
    fp_patches      patch;
    dir_node        *dir;
    char            **patch_name_array;

    patch_name_array = ( secondary ? FPPatchAltName : FPPatchName );

    if( ins->token == T_FWAIT ) {
        patch = FPP_WAIT;
    } else {
        switch( Code->prefix.seg ) {
        case EMPTY:
            patch = FPP_NORMAL;
            break;
        case PREFIX_ES:
            patch = FPP_ES;
            break;
        case PREFIX_CS:
            patch = FPP_CS;
            break;
        case PREFIX_SS:
            patch = FPP_SS;
            break;
        case PREFIX_DS:
            patch = FPP_DS;
            break;
        case PREFIX_FS:
            patch = FPP_FS;
            break;
        case PREFIX_GS:
            patch = FPP_GS;
            break;
        default:
            never_reach();
        }
    }

    /* put out an extern def for the the patch */
    if( patch_name_array[patch] == NULL )
        return( NOT_ERROR );
    dir = (dir_node *)AsmGetSymbol( patch_name_array[patch] );
    if( dir == NULL ) {
        dir = dir_insert( patch_name_array[patch], TAB_EXT );
        if( dir == NULL )
            return( ERROR );
        GetSymInfo( &dir->sym );
        dir->sym.offset = 0;
        dir->sym.referenced = TRUE;
        dir->sym.mem_type = MT_FAR;
        SetMangler( &dir->sym, "N", LANG_NONE );
    }
    if( MakeFpFixup( &dir->sym ) == ERROR )
        return( ERROR );
    return( NOT_ERROR );
}
