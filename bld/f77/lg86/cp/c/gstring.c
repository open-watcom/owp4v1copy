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
// GSTRING      : Generate code for string operations
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "opn.h"
#include "objutil.h"

extern  void            AdvanceITPtr(void);
extern  int             AsgnCat(void);
extern  void            CatArgs(int);
extern  void            EmitOp(unsigned_16);
extern  void            PushOpn(itnode *);
extern  void            PushAddr(itnode *);
extern  bool            ChkUDef(itnode *);
extern  sym_id          StaticAlloc(int,int);


extern  sym_id  GTempString( itnode *itptr ) {
//============================================

// Generate a static temporary string.

    sym_id      sym_ptr;

    itptr->flags = SY_VARIABLE; // treat it like a variable
    sym_ptr = StaticAlloc( sizeof( string ), TY_CHAR );
    sym_ptr->ns.xt.size = itptr->size; // must come after StaticAlloc()
    return( sym_ptr );
}


#pragma off (unreferenced);
extern  sym_id  GStartCat( int num_args, int size ) {
#pragma on (unreferenced);
//===================================================

// Start cconcatenation into a temporary.

    return( NULL );
}


#pragma off (unreferenced);
extern  void    GStopCat( int num_args, sym_id result ) {
#pragma on (unreferenced);
//=======================================================

// Finish concatenation into a temporary.

    CITNode->sym_ptr = GTempString( CITNode );
    CITNode->opn = OPN_VAL;  // indicate a static temporary SCB
    // Push the address of a static SCB so that we can modify its
    // length to correspond to the length concatenated so that
    //      CHARACTER*5 WORD
    //      PRINT,LEN('1'//'WORD(1:3))
    // prints 4 and not 6. The static SCB in this case initially
    // contains the length 6 ( len('1') + len(word) ) since generally
    // we don't know the length concatenated at compile time if WORD
    // was indexed as WORD(I:J).
    PushAddr( CITNode );
    EmitOp( RT_CAT );
    OutU16( num_args | 0x8000 ); // indicate concatenating into a static temp
    CITNode->opn = OPN_VAL;  // indicate a static temporary SCB
}


extern  void    GCatArg( itnode *itptr ) {
//========================================

// Emit a character string to be concatenated.

    PushOpn( itptr );
}


extern  void    AsgnChar() {
//==========================

// Perform character assignment.

    itnode      *save_cit;
    int         num_args;
    int         opn;

    save_cit = CITNode;
    AdvanceITPtr();
    num_args = AsgnCat();
    CatArgs( num_args );
    CITNode = save_cit;
    opn = CITNode->opn;
    PushOpn( CITNode );
    switch( opn & OPN_WHAT ) {
    case OPN_NWL:
    case OPN_ASS:
        opn = OPN_ADR;
        break;
    }
    CITNode->opn = opn; // set opn for multiple assignment
    if( ChkUDef( CITNode ) &&
        ( ( num_args != 1 ) ||
          ( ( CITNode->link->opn & OPN_WHAT ) != OPN_CON ) ) ) {
        EmitOp( RT_UCAT );
    } else {
        EmitOp( RT_CAT );
    }
    OutU16( num_args );
}
