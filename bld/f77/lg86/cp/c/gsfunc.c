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
// GSFUNC       : Generate code for statement functions
//

#include "ftnstd.h"
#include "global.h"
#include "fcodes.h"
#include "objutil.h"

extern  void            SymRef(sym_id);
extern  void            PushOpn(itnode *);
extern  void            PushTmp(sym_id,int);
extern  void            PopSym(sym_id);
extern  void            PushSym(sym_id);
extern  sym_id          GTempString(itnode *);
extern  void            EmitOp(unsigned_16);
extern  void            FlushStk(void);
extern  uint            GetStkEntries(uint);
extern  void            ReqStkDepth(uint);
extern  void            ReverseList(void **);


extern  void    GSFArg( sym_id sym_ptr ) {
//========================================

// Generate a statement function argument.

    sym_ptr = sym_ptr;
    PushOpn( CITNode );
}


extern  void    GSFCall( itnode *sfunc ) {
//========================================

// Generate a statement function call.

    sf_parm     *arg;
    sf_parm     *stack;
    sf_parm     *cur;

    // we have to flip the order of the args so we can pop them off
    arg = sfunc->sym_ptr->ns.si.sf.header->parm_list;
    ReverseList( &arg );

    // now we can pop the args and restore the links
    stack = NULL;
    while( arg != NULL ) {
        cur = arg;
        arg = arg->link;
        ReqStkDepth( GetStkEntries( cur->shadow->ns.typ ) );
        if( cur->shadow->ns.typ != TY_CHAR ) {
            PopSym( cur->shadow );
        } else {
            // we have to do the equivalent of character assignment here
            // otherwise we run into problems with code such as:
            //  CHARACTER F*8,X
            //  F(X) = X
            //  PRINT *,F('12345678')
            EmitOp( PUSH_SCB );
            SymRef( cur->shadow );
            EmitOp( RT_CAT );
            OutU16( 1 );
        }
        cur->link = stack;
        stack = cur;
    }
    /* assert( stack == sfunc->sym_ptr->ns.si.sf.header->parm_list ); */
    if( sfunc->typ == TY_CHAR ) {
        PushTmp( GTempString( sfunc ), TY_CHAR );
        PopSym( sfunc->sym_ptr );
    }
    FlushStk();
    EmitOp( SF_CALL );
    OutLocalRef( sfunc->sym_ptr->ns.address.la );
}


extern  void    GSFRetVal( itnode *sfunc ) {
//==========================================

// Generate a statement function return value.

    unsigned_16 save_flags;

    save_flags = sfunc->flags;
    // set SY_DATA_INIT to prevent undefined variable checking
    sfunc->flags = SY_TYPE | SY_USAGE | SY_VARIABLE | SY_DATA_INIT;
    PushOpn( sfunc );
    sfunc->flags = save_flags;
}


extern  void    AdjSFList( itnode *sfunc ) {
//==========================================

// Process statement function argument list.
    sfunc = sfunc;
}
