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
// UFNS : handle user functions for 386 version
//

#include <stddef.h>
#include <string.h>
#include <i86.h>

#include "ftnstd.h"
#include "global.h"
#include "progsw.h"
#include "cpopt.h"

extern  void    FMemFree(void *);
extern  int     KwLookUp(void **,int,char *,int,bool);
extern  sym_id  SearchGList(sym_id);
extern  sym_id  STAdd(char *,int);

// create external declarations for all the ufns
#define UFN( x )        extern void fortran x( void );
#include <ufns.inc>
#undef UFN

// get text names for all the ufns
#define UFN( x )        #x ,
static  char    *UFNames[] = {
#include <ufns.inc>
};
#undef UFN
#define NUM_UFNS        ( sizeof( UFNames ) / sizeof( char * ) )

// an array of addresses of the ufns
#define UFN( x )        &x,
static void PGM * UFPtrs[] = {
#include <ufns.inc>
};
#undef UFN


lib_handle      SearchUFNS( char *name ) {
//========================================

    int         num;

    if( _GenObjectFile() ) return( NULL );
    num = KwLookUp( UFNames, NUM_UFNS, name, strlen( name ), TRUE );
    if( num == 0 ) return( NULL );
    ProgSw |= PS_LIB_OBJECT;
    return( (lib_handle)num );
}


void    ProcObjFile( lib_handle lp ) {
//====================================

    int         num;
    sym_id      sym;
    sym_id      gbl;

    num = (int)lp - 1;
    sym = STAdd( UFNames[ num ], strlen( UFNames[ num ] ) );
    sym->ns.flags = SY_USAGE | SY_SUBPROGRAM | SY_FN_OR_SUB | SY_PS_ENTRY;
    gbl = SearchGList( sym );
    gbl->ns.address.ga.offset = FP_OFF( UFPtrs[ num ] );
#if _TARGET == _8086
    gbl->ns.address.ga.seg = FP_SEG( UFPtrs[ num ] );
#endif
    gbl->ns.flags |= SY_ADDR_ASSIGNED;
    FMemFree( sym );
}
