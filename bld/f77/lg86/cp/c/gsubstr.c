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
// GSUBSTR      : Generate substringing code
//

#include "ftnstd.h"
#include "global.h"
#include "opn.h"
#include "fcodes.h"
#include "objutil.h"

extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  void            OpnRef(itnode *);
extern  void            SetOpn(itnode *,int);
extern  void            ReqStkDepth(int);
extern  void            ModStkDepth(int);
extern  void            IncStkDepth(int);
extern  uint            GetStkEntries(uint);


void    GBegSSStr() {
//===================

// Start a substring operation on an array element.

}


#pragma off (unreferenced);
void    GBegFSS( itnode *func_node ) {
#pragma on (unreferenced);
//====================================

// Start a substring operation on a function return value.

}


#pragma off (unreferenced);
void    GBegSFSS( itnode *sf_node ) {
#pragma on (unreferenced);
//===================================

// Start a substring operation on a statement function.

}


void    GInitSS( itnode *itptr ) {
//================================

// Start a substring operation.

    PushOpn( itptr );
}


void    GSubStr( itnode *char_node ) {
//====================================

// Generate a substring element.

    if( ( CITNode->opn & OPN_WHAT ) == OPN_SSR ) {
        IncStkDepth( TY_INTEGER );
        if( ( char_node->opn & OPN_WHERE ) == OPN_SAFE ) {
            // For the case:
            //    SUBROUTINE SAM( CARRAY )
            //    CHARACTER*(*) CARRAY(3)
            //    PRINT *, CARRAY(2)(3:)
            //    END
            EmitOp( PUSH_SCB_LEN_SAFE );
        } else {
            EmitOp( PUSH_SCB_LEN );
            OpnRef( char_node );
        }
    } else {
        PushOpn( CITNode );
    }
}


#pragma off (unreferenced);
void    GFiniSS( itnode *itptr, itnode *ss_node ) {
#pragma on (unreferenced);
//=================================================

// Finish a substring operation.

    int depth;

    depth = 2 * GetStkEntries( TY_INTEGER );
    ReqStkDepth( depth );
    if( itptr->opn & OPN_FLD ) {
        EmitOp( FIELD_SUBSTRING );
        OutInt( itptr->size );
    } else {
        EmitOp( RT_SUBSTRING );
    }
    SetOpn( itptr, OPN_SAFE );
    ModStkDepth( depth );
}
