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
// FMEMMGR      : FORTRAN memory manager
//

#include "ftnstd.h"
#include "errcod.h"
#include "progsw.h"
#include "stmtsw.h"
#include "global.h"
#include "bglobal.h"

extern  void            Error(int,...);
extern  void            PurgeAll(void);
extern  void            FreeITNodes(itnode *);
extern  void            FrlFini(void **);
extern  void            CompErr(uint);
extern  void            *_SysMemAlloc(uint);
extern  void            _SysMemFree(void *);
extern  void            _SysMemInit(void);
extern  void            _SysMemFini(void);
extern  void            Suicide(void);


void    FMemInit() {
//==================

    UnFreeMem = 0;
    _SysMemInit();
}


void    FMemFini() {
//==================

    ProgSw &= ~PS_ERROR; // we always want to report memory problems
    if( UnFreeMem > 0 ) {
        CompErr( CP_MEMORY_NOT_FREED );
    } else if( UnFreeMem < 0 ) {
        CompErr( CP_FREEING_UNOWNED_MEMORY );
    }
    _SysMemFini();
}


void    *FMemAlloc( uint size ) {
//===============================

    void        *p;

    p = _SysMemAlloc( size );
    if( p == NULL ) {
        FrlFini( &ITPool );
        p = _SysMemAlloc( size );
        if( p == NULL ) {
            if( !(ProgSw & PS_STMT_TOO_BIG) &&
                 (StmtSw & SS_SCANNING) && (ITHead != NULL) ) {
                FreeITNodes( ITHead );
                ITHead = NULL;
                Error( MO_LIST_TOO_BIG );
                ProgSw |= PS_STMT_TOO_BIG;
            } else {
                ProgSw |= PS_FATAL_ERROR;
                PurgeAll(); // free up memory so we can process the error
                Error( MO_DYNAMIC_OUT );
                Suicide();
            }
        } else {
            UnFreeMem++;
        }
    } else {
        UnFreeMem++;
    }
    return( p );
}


void    FMemFree( void *p ) {
//===========================

    _SysMemFree( p );
    UnFreeMem--;
}
