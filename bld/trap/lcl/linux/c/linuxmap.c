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
* Description:  Routines to keep track of loaded modules and address maps.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "trpimp.h"
#include "mad.h"
#include "exeelf.h"
#include "linuxcomm.h"

typedef struct lli {
    addr48_off  offset;
    addr48_off  dbg_dyn_sect;
    addr48_off  code_size;
    char        newly_unloaded : 1;
    char        newly_loaded : 1;
    char        filename[257]; // TODO: These should really be dynamic!
    char        modname[128];
} lib_load_info;

static lib_load_info    *moduleInfo;
static int              ModuleTop;

/* At this point, due to the way the Linux trap file is built, we don't have
 * any dynamic memory management routines available. So we just define a big
 * fat static array. This should be redone once the trap file becomes a real
 * shared library.
 */
#define NO_DYN_MEM
#ifdef NO_DYN_MEM
static lib_load_info    module_info_array[32];
#endif

/*
 * AddProcess - a new process has been created
 */
void AddProcess( void )
{
    lib_load_info       *lli;

#ifdef NO_DYN_MEM
    moduleInfo = &module_info_array[0];
#else    
    moduleInfo = malloc( sizeof( lib_load_info ) );
#endif    
    memset( moduleInfo, 0, sizeof( lib_load_info ) );
    ModuleTop = 1;

    lli = &moduleInfo[0];

    lli->offset = 0;    /* Assume that main executable was not relocated */
    lli->modname[0] = 0;
    lli->filename[0] = 0;
}

/*
 * AddLib - a new library has loaded
 */
void AddLib( struct link_map *lmap )
{
    lib_load_info       *lli;

    ModuleTop++;
#ifdef NO_DYN_MEM
    lli = &module_info_array[0];
#else
    /* This code is not terribly efficient */
    lli = malloc( ModuleTop * sizeof( lib_load_info ) );
    memset( lli, 0, ModuleTop * sizeof( lib_load_info ) );
    memcpy( lli, moduleInfo, ( ModuleTop - 1 ) *sizeof( lib_load_info ) );
    free( moduleInfo );
#endif
    moduleInfo = lli;
    lli = &moduleInfo[ModuleTop - 1];

    lli->offset = lmap->l_addr;
    lli->dbg_dyn_sect = (addr48_off)lmap->l_ld;
    lli->modname[0] = 0;
    strcpy( lli->filename, lmap->l_name );
    lli->newly_loaded = TRUE;
    lli->newly_unloaded = FALSE;
}

void DelLib( addr48_off dynsection )
{
    unsigned    i;

    for( i = 0; i < ModuleTop; ++i ) {
        if( moduleInfo[i].dbg_dyn_sect == dynsection ) {
            moduleInfo[i].newly_unloaded = TRUE;
            moduleInfo[i].offset = NULL;
	    moduleInfo[i].dbg_dyn_sect = NULL;
            moduleInfo[i].code_size = 0;
            break;
        }
    }
}

void DelProcess( void )
{
    unsigned    i;

    for( i = 0; i < ModuleTop; ++i ) {
        moduleInfo[i].offset = NULL;
        moduleInfo[i].dbg_dyn_sect = NULL;
        moduleInfo[i].code_size = 0;
    }
}

#if 0
unsigned ReqMap_addr( void )
{
    u_short             seg;
    map_addr_req        *acc;
    map_addr_ret        *ret;
    lib_load_info       *lli;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );

    seg = acc->in_addr.segment;
    switch( seg ) {
    case MAP_FLAT_CODE_SELECTOR:
    case MAP_FLAT_DATA_SELECTOR:
        seg = 0;
        break;
    default:
        --seg;
        break;
    }

    lli = &moduleInfo[acc->handle];

#if 0
    /*
     * for a 32-bit app, we get the PE header. We can look up the
     * object in the header and determine if it is code or data, and
     * use that to assign the appropriate selector (either FlatCS
     * or FlatDS).
     */
    if( hi.sig == EXE_PE ) {
        for( i = 0; i < hi.peh.num_objects; i++ ) {
            ReadFile( handle, &obj, sizeof( obj ), &bytes, NULL );
            if( i == seg ) {
                break;
            }
        }
        if( i == hi.peh.num_objects ) {
            return( 0 );
        }
        if( obj.flags & ( PE_OBJ_CODE | PE_OBJ_EXECUTABLE ) ) {
            ret->out_addr.segment = FlatCS;
        } else {
            ret->out_addr.segment = FlatDS;
        }
        ret->out_addr.offset = (addr48_off)lli->base + obj.rva;
    } else {
        return( 0 );
    }
#endif
    ret->out_addr.offset += acc->in_addr.offset;
    ret->lo_bound = 0;
    ret->hi_bound = ~(addr48_off)0;
    return( sizeof( *ret ) );
}
#endif

/*
 * AccGetLibName - get lib name of current module
 */
unsigned ReqGet_lib_name( void )
{
    get_lib_name_req    *acc;
    get_lib_name_ret    *ret;
    char                *name;
    unsigned            i;


    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    name = GetOutPtr( sizeof( *ret ) );

    ret->handle = 0;
    name[0] = '\0';

    for( i = 0; i < ModuleTop; ++i ) {
        if( moduleInfo[i].newly_unloaded ) {
            ret->handle = i;
            name[0] = '\0';
            moduleInfo[i].newly_unloaded = FALSE;
            return( sizeof( *ret ) );
        } else if( moduleInfo[i].newly_loaded ) {
            ret->handle = i;
            strcpy( name, moduleInfo[i].filename );
            moduleInfo[i].newly_loaded = FALSE;
            return( sizeof( *ret ) + strlen( name ) + 1 );
        }
    }
    return( sizeof( *ret ) );
}
