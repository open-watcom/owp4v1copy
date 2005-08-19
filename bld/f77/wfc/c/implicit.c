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
// IMPLICIT  : IMPLICIT statement processor
//

#include "ftnstd.h"
#include "opr.h"
#include "errcod.h"
#include "global.h"
#include "prdefn.h"
#include "segsw.h"
#include "csetinfo.h"

#include <string.h>
#include <limits.h>

extern  void            Error(uint,...);
extern  void            TypeErr(uint,uint);
extern  void            Extension(uint,...);
extern  void            StmtExtension(uint);
extern  void            AdvanceITPtr(void);
extern  bool            CIntExpr(void);
extern  bool            CmpNode2Str(itnode *,char *);
extern  bool            RecComma(void);
extern  bool            RecName(void);
extern  bool            RecNumber(void);
extern  bool            RecMin(void);
extern  bool            RecMul(void);
extern  bool            RecNextOpr(byte);
extern  bool            ReqOpenParen(void);
extern  bool            ReqCloseParen(void);
extern  bool            ReqNOpn(void);
extern  bool            RecNOpn(void);
extern  bool            ReqEOS(void);
extern  bool            SetImplType(char,char,uint,uint);
extern  int             StorageSize(uint);
extern  uint            MapTypes(uint,uint);
extern  int             FmtS2I(char *,int,bool,intstar4 *,bool,int *);
extern  intstar4        ITIntValue(itnode *);

extern  char            *TypeKW[];
extern  character_set   CharSetInfo;


static  bool    ReqChar() {
//=========================

// Recognize one character operand.

    if( RecName() && (CITNode->opnd_size == 1) &&
        !CharSetInfo.is_foreign( *CITNode->opnd ) ) return( TRUE );
    Error( IM_ILLEGAL_RANGE );
    return( FALSE );
}


static  bool            StarStar( byte typ ) {
//============================================

    if( typ != TY_CHAR ) return( FALSE );
    if( RecNOpn() == FALSE ) return( FALSE );
    if( RecNextOpr( OPR_LBR ) == FALSE ) return( FALSE );
    AdvanceITPtr();
    if( RecNOpn() == FALSE ) return( FALSE );
    if( RecNextOpr( OPR_MUL ) == FALSE ) return( FALSE );
    AdvanceITPtr();
    if( RecNOpn() == FALSE ) return( FALSE );
    if( RecNextOpr( OPR_RBR ) == FALSE ) return( FALSE );
    return( TRUE );
}


static  bool    CheckSize( byte typ, intstar4 size, itnode *start ) {
//===================================================================

// Ensure that the length specification (SIZE) is valid for the
// given data type (TYP).

    itnode      *temp;

    if( (typ == TY_DOUBLE) || (typ == TY_DCOMPLEX) ||
        (typ == TY_EXTENDED) || (typ == TY_XCOMPLEX) ) {
        temp = CITNode;
        CITNode = start; // get the caret in proper place
        TypeErr( TY_NOT_DBL_PREC, typ );
        CITNode = temp;
        return( FALSE );
    }
    if( typ == TY_LOGICAL ) {
        if( size == sizeof( logstar1 ) ) return( TRUE );
        if( size == sizeof( logstar4 ) ) return( TRUE );
    } else if( typ == TY_INTEGER ) {
        if( size == sizeof( intstar1 ) ) return( TRUE );
        if( size == sizeof( intstar2 ) ) return( TRUE );
        if( size == sizeof( intstar4 ) ) return( TRUE );
    } else if( typ == TY_REAL ) {
        if( size == sizeof( single ) ) return( TRUE );
        if( size == sizeof( double ) ) return( TRUE );
        if( size == sizeof( extended ) ) return( TRUE );
    } else if( typ == TY_COMPLEX ) {
        if( size == sizeof( complex ) ) return( TRUE );
        if( size == sizeof( dcomplex ) ) return( TRUE );
        if( size == sizeof( xcomplex ) ) return( TRUE );
    } else if( typ == TY_CHAR ) {
#if _CPU == 8086
        if( (size > 0) && (size <= USHRT_MAX) ) return( TRUE );
#else
        if( size > 0 ) return( TRUE );
#endif
    }
    temp = CITNode;
    CITNode = start; // get the caret in proper place
    Error( TY_ILL_TYP_SIZE, size, TypeKW[ typ ] );
    CITNode = temp;
    return( FALSE );
}


bool    LenSpec( uint typ, int *size_ptr ) {
//==========================================

// Process a length specification.

    itnode      *save_itptr;
    bool        len_spec;
    itnode      *temp;
    intstar4    size;

    len_spec = FALSE;
    if( RecMul() ) {
        save_itptr = CITNode;
        if( StarStar( typ ) ) {
            AdvanceITPtr();
            if( RecNOpn() ) {
                AdvanceITPtr();
            }
            *size_ptr = 0;
            if( StmtProc == PR_IMP ) {
                temp = CITNode;
                CITNode = save_itptr; // get the caret in proper place
                Extension( IM_CHAR_STAR_STAR );
                CITNode = temp;
            }
            len_spec = TRUE;
        } else {
            CITNode = save_itptr;
            if( RecNOpn() && RecNextOpr( OPR_LBR ) ) {
                AdvanceITPtr();
                CIntExpr();
                size = ITIntValue( CITNode );
                AdvanceITPtr();
                ReqCloseParen();
                if( RecNOpn() ) {
                    AdvanceITPtr();
                }
                len_spec = (AError == FALSE);
            } else if( RecNumber() ) {
                FmtS2I( CITNode->opnd, CITNode->opnd_size, FALSE, &size, FALSE,
                        NULL );
                AdvanceITPtr();
                len_spec = TRUE;
            } else {
                Error( SX_EXPECT_INT );
                AdvanceITPtr();
                len_spec = FALSE;
            }
            if( len_spec ) {
                len_spec = CheckSize( typ, size, save_itptr );
                if( len_spec ) {
                    *size_ptr = size;
                }
            }
        }
    }
    if( len_spec && ( typ != TY_CHAR ) ) {
        temp = CITNode;
        CITNode = save_itptr;
        StmtExtension( TY_LEN_SPEC );
        CITNode = temp;
    }
    return( len_spec );
}


void    CpImplicit() {
//====================

// Process the IMPLICIT statement.

    byte        chr1;
    byte        chr2;
    int         typ;
    bool        valid_range;
    int         size;

    if( (CITNode->opnd_size == 4) && (memcmp( CITNode->opnd, "NONE", 4 ) == 0) ) {
        AdvanceITPtr();
        Extension( SP_STRUCTURED_EXT, "IMPLICIT NONE" );
        if( SgmtSw & ( SG_IMPLICIT_STMT | SG_IMPLICIT_NONE ) ) {
            Error( IM_NONE_USED );
        }
        SgmtSw |= SG_IMPLICIT_NONE;
    } else {
        for(;;) {
            typ = RecTypeKW();
            AdvanceITPtr();
            if( ( typ != -1 ) && !LenSpec( typ, &size ) ) {
                size = StorageSize( typ );
            }
            ReqOpenParen();
            for(;;) {
                valid_range = ReqChar();
                chr1 = *CITNode->opnd;
                AdvanceITPtr();
                if( RecMin() ) {
                    valid_range = valid_range && ReqChar();
                    chr2 = *CITNode->opnd;
                    if( valid_range ) {
                        if( chr1 >= chr2 ) {
                            Error( IM_ILLEGAL_RANGE );
                        }
                        AdvanceITPtr();
                    }
                } else {
                    chr2 = chr1;
                }
                if( valid_range ) {
                    typ = MapTypes( typ, size );
                    if( !SetImplType( chr1, chr2, typ, size ) ) {
                        Error( IM_PREV_IMPLICIT );
                    }
                }
                if( !RecComma() ) break;
            }
            ReqCloseParen();
            ReqNOpn();
            AdvanceITPtr();
            if( !RecComma() ) break;
        }
        if( SgmtSw & SG_IMPLICIT_NONE ) {
            Error( IM_NONE_USED );
        }
        SgmtSw |= SG_IMPLICIT_STMT;
    }
    ReqEOS();
}


static  int     RecTypeKW() {
//===========================

// Recognize a type keyword (INTEGER, REAL, etc.).
// Assumptions: Types are consecutive starting at 0

    int         typ;

    if( RecName() ) {
        typ = 0;
        for(;;) {
            if( CmpNode2Str( CITNode, TypeKW[ typ ] ) ) return( typ );
            typ++;
            if( typ > TY_CHAR ) break;
        }
    }
    Error( IM_UNRECOG_TYPE );
    return( -1 );
}
