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
* Description:  upscan constant folding routines
*
****************************************************************************/


#include "ftnstd.h"
#include "opr.h"
#include "optr.h"
#include "astype.h"
#include "opn.h"
#include "global.h"
#include "fmemmgr.h"
#include "types.h"
#include "utility.h"

#include <string.h>

extern  void            BadEqual(void);
extern  void            AddConst(itnode *);
extern  void            AddI(ftn_type *,ftn_type *);
extern  void            SubI(ftn_type *,ftn_type *);
extern  void            DivI(ftn_type *,ftn_type *);
extern  void            MulI(ftn_type *,ftn_type *);
extern  void            AddR(ftn_type *,ftn_type *);
extern  void            SubR(ftn_type *,ftn_type *);
extern  void            DivR(ftn_type *,ftn_type *);
extern  void            MulR(ftn_type *,ftn_type *);
extern  void            AddD(ftn_type *,ftn_type *);
extern  void            SubD(ftn_type *,ftn_type *);
extern  void            DivD(ftn_type *,ftn_type *);
extern  void            MulD(ftn_type *,ftn_type *);
extern  void            AddE(ftn_type *,ftn_type *);
extern  void            SubE(ftn_type *,ftn_type *);
extern  void            DivE(ftn_type *,ftn_type *);
extern  void            MulE(ftn_type *,ftn_type *);
extern  void            AddC(complex*,complex *);
extern  void            SubC(complex *,complex *);
extern  void            DivC(complex *,complex *);
extern  void            MulC(complex *,complex *);
extern  void            AddQ(dcomplex *,dcomplex *);
extern  void            SubQ(dcomplex *,dcomplex *);
extern  void            DivQ(dcomplex *,dcomplex *);
extern  void            MulQ(dcomplex *,dcomplex *);
extern  void            AddX(xcomplex *,xcomplex *);
extern  void            SubX(xcomplex *,xcomplex *);
extern  void            DivX(xcomplex *,xcomplex *);
extern  void            MulX(xcomplex *,xcomplex *);
extern  void            GenExp(TYPE);
extern  void            ExpI(byte,ftn_type *,intstar4);
extern  void            XINeg(ftn_type *,ftn_type *);
extern  void            XRNeg(ftn_type *,ftn_type *);
extern  void            XDNeg(ftn_type *,ftn_type *);
extern  void            XENeg(ftn_type *,ftn_type *);
extern  void            XCNeg(ftn_type *,ftn_type *);
extern  void            XQNeg(ftn_type *,ftn_type *);
extern  void            XXNeg(ftn_type *,ftn_type *);
extern  void            XIPlus(ftn_type *,ftn_type *);
extern  void            XRPlus(ftn_type *,ftn_type *);
extern  void            XDPlus(ftn_type *,ftn_type *);
extern  void            XEPlus(ftn_type *,ftn_type *);
extern  void            XCPlus(ftn_type *,ftn_type *);
extern  void            XQPlus(ftn_type *,ftn_type *);
extern  void            XXPlus(ftn_type *,ftn_type *);
extern  void            XLEqv(ftn_type *,ftn_type *);
extern  void            XLNeqv(ftn_type *,ftn_type *);
extern  void            XLOr(ftn_type *,ftn_type *);
extern  void            XLAnd(ftn_type *,ftn_type *);
extern  void            XLNot(ftn_type *,ftn_type *);
extern  void            XBitEqv(ftn_type *,ftn_type *);
extern  void            XBitNeqv(ftn_type *,ftn_type *);
extern  void            XBitOr(ftn_type *,ftn_type *);
extern  void            XBitAnd(ftn_type *,ftn_type *);
extern  void            XBitNot(ftn_type *,ftn_type *);
extern  void            XICmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XRCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XDCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XECmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XCCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XQCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XXCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            XChCmp(ftn_type *,ftn_type *,const logstar1 __FAR *);
extern  void            CnvTo(itnode *,TYPE,uint);
extern  void            CnI2R(void *);
extern  void            CnI2D(void *);
extern  void            CnI2C(void *);
extern  void            CnI2Q(void *);
extern  void            CnR2D(void *);
extern  void            CnR2C(void *);
extern  void            CnR2Q(void *);
extern  void            CnD2C(void *);
extern  void            CnD2Q(void *);
extern  void            CnC2Q(void *);

#define UAR_TAB_ROWS    9
#define UAR_TAB_COLS    2
#define UAR_TAB_SIZE    18

static  void    (* const __FAR XUArithTab[])() = {
                                        // operator/
                                        //     /
                    // PLUS    MINUS    //  /result
                    //==============================
                                        //
                      &XIPlus, &XINeg,  // integer_1
                      &XIPlus, &XINeg,  // integer_2
                      &XIPlus, &XINeg,  // integer
                      &XRPlus, &XRNeg,  // real
                      &XDPlus, &XDNeg,  // double
                      &XEPlus, &XENeg,  // extended
                      &XCPlus, &XCNeg,  // complex
                      &XQPlus, &XQNeg,  // dcomplex
                      &XXPlus, &XXNeg   // xcomplex
                                        };

//
// XArithTab -- called by BinOp. Indexed by the result type and opr
//

#define AR_TAB_ROWS     9
#define AR_TAB_COLS     4
#define AR_TAB_SIZE     36

static  void    (* const __FAR XArithTab[])() = {
                                                 // operator/
                                                 //     /
         // PLUS    MINUS     TIMES     DIVIDE   //  /result
         ////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                                                 //
           &AddI,   &SubI,    &MulI,    &DivI,   // integer
           &AddI,   &SubI,    &MulI,    &DivI,   // integer_1
           &AddI,   &SubI,    &MulI,    &DivI,   // integer_2
           &AddR,   &SubR,    &MulR,    &DivR,   // real
           &AddD,   &SubD,    &MulD,    &DivD,   // double
           &AddE,   &SubE,    &MulE,    &DivE,   // extended
           &AddC,   &SubC,    &MulC,    &DivC,   // complex
           &AddQ,   &SubQ,    &MulQ,    &DivQ,   // dcomplex
           &AddX,   &SubX,    &MulX,    &DivX    // xcomplex
                                                 };

static  void    (* const __FAR XCmpTab[])() = {

// int*1   int*2   integer real    double  extended complex dcomplex xcomplex character
   &XICmp, &XICmp, &XICmp, &XRCmp, &XDCmp, &XECmp,  &XCCmp, &XQCmp,  &XXCmp,  &XChCmp
          };

static  void    (* const __FAR XLogicalTab[])() = {

       // EQV        NEQV       OR         AND        NOT
         &XLEqv,    &XLNeqv,   &XLOr,     &XLAnd,    &XLNot
         };

static  void    (* const __FAR XBitWiseTab[])() = {

       // EQV        NEQV       OR         AND        NOT
         &XBitEqv,  &XBitNeqv, &XBitOr,   &XBitAnd,  &XBitNot
         };

static  const bool __FAR        CmpValue[] = {
                                     // result/
                                     //   /
         // <         =         >    ///operator
         //=====================================
                                     //
           FALSE,   TRUE,     FALSE, // .EQ.
           TRUE,    FALSE,    TRUE,  // .NE.
           TRUE,    FALSE,    FALSE, // .LT.
           FALSE,   TRUE,     TRUE,  // .GE.
           TRUE,    TRUE,     FALSE, // .LE.
           FALSE,   FALSE,    TRUE   // .GT.
           };


static  void    Convert() {
//=========================

    CnvTo( CITNode, ResultType, TypeSize( ResultType ) );
    CnvTo( CITNode->link , ResultType, TypeSize( ResultType ) );
}


static  void    LogOp( TYPE typ1, TYPE typ2, OPTR op ) {
//======================================================

    typ1 = typ1;
    op -= OPTR_EQV;
    if( _IsTypeInteger( typ2 ) ) {
        Convert();
        XBitWiseTab[ op ]( &CITNode->value, &CITNode->link->value );
    } else {
        XLogicalTab[ op ]( &CITNode->value, &CITNode->link->value );
    }
    CITNode->opn.us = USOPN_CON; // this is required for .not. operator
}


static  void    RelOp( TYPE typ1, TYPE typ2, OPTR op ) {
//======================================================

    typ1 = typ1; typ2 = typ2; op = op;
    if( ResultType != TY_CHAR ) {
        Convert();
    }
    XCmpTab[ ResultType - TY_INTEGER_1 ]( &CITNode->value,
                                        &CITNode->link->value,
          &CmpValue[ ( CITNode->link->opr - FIRST_RELOP ) * 3 ] );
    ResultType = TY_LOGICAL;
}


static  void    BinOp( TYPE typ1, TYPE typ2, OPTR op ) {
//===================================================

    byte        index;

    typ2 = typ2;
    op -= OPTR_ADD;
    index = ResultType - TY_INTEGER_1;
    if( typ1 != TY_NO_TYPE ) {
        Convert();
        XArithTab[ index * AR_TAB_COLS + op ]
                 ( &CITNode->value, &CITNode->link->value );
    } else {
        CnvTo( CITNode->link , ResultType, TypeSize( ResultType ) );
        XUArithTab[ index * UAR_TAB_COLS + op ]
                  ( &CITNode->value, &CITNode->link->value );
        CITNode->opn.us = USOPN_CON;
    }
}


static  void    ExpOp( TYPE typ1, TYPE typ2, OPTR op ) {
//======================================================

    op = op;
    if( !_IsTypeInteger( typ2 ) ) {
        Convert();
        GenExp( ResultType );
    } else {
        CnvTo( CITNode, ResultType, TypeSize( ResultType ) );
        ExpI( typ1, &CITNode->value, ITIntValue( CITNode->link ) );
    }
}


void    ConstCat( int size ) {
//============================

    itnode      *last_node;
    byte        *dest;
    int         opn_size;
    int         size_left;
    byte        *string;
    itnode      *link_node;

    last_node = CITNode;
    string = FMemAlloc( size );
    size_left = size;
    dest = string;
    for(;;) {
        opn_size = last_node->value.cstring.len;
        memcpy( dest, last_node->value.cstring.strptr, opn_size );
        size_left -= opn_size;
        if( size_left == 0 ) break;
        last_node = last_node->link;
        dest += opn_size;
    }
    CITNode->value.cstring.strptr = string;
    CITNode->value.cstring.len = size;
    CITNode->size = size;
    link_node = last_node->link;
    last_node->link = NULL;
    FreeITNodes( CITNode->link );
    CITNode->link = link_node;
    AddConst( CITNode );
    CITNode->value.cstring.strptr = &CITNode->sym_ptr->lt.value;
    FMemFree( string );
}

#ifdef pick
#undef pick
#endif
#define pick(id,const,gener) const,

void    (* const __FAR ConstTable[])() = {
#include "optrdefn.h"
};
