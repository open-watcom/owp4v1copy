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
* Description:  Upscan phase of expression handling.
*
****************************************************************************/


#include "ftnstd.h"
#include "opr.h"
#include "opn.h"
#include "errcod.h"
#include "prdefn.h"
#include "astype.h"
#include "ifflags.h"
#include "ifnames.h"
#include "global.h"
#include "cpopt.h"
#include "parmtype.h"

extern  void            DetSubList(void);
extern  void            DetCallList(void);
extern  void            DetSStr(void);
extern  byte            IFSpecific(byte);
extern  byte            IFArgType(byte);
extern  bool            IFIsGeneric(byte);
extern  void            MarkIFUsed(int);
extern  void            GMakeCplx(void);
extern  void            GMakeDCplx(void);
extern  void            GMakeXCplx(void);
extern  void            FreeITNodes(itnode *);
extern  void            FreeOneNode(itnode *);
extern  bool            Subscripted(void);
extern  void            AdvanceITPtr(void);
extern  bool            BitOn(unsigned_16);
extern  bool            ClassIs(unsigned_16);
extern  void            GArg(void);
extern  int             TypeSize(uint);
extern  void            GILCnvTo(uint,uint);
extern  void            CnvTo(itnode*,uint,uint);
extern  void            UpdateNode(itnode *,itnode *);
extern  bool            RecNextOpr(byte);
extern  bool            RecNOpn(void);
extern  bool            RecColon(void);
extern  bool            RecCloseParen(void);
extern  bool            ReqNOpn(void);
extern  bool            RecOpenParen(void);
extern  bool            ReqCloseParen(void);
extern  void            BackTrack(void);
extern  void            MoveDown(void);
extern  void            KillOpnOpr(void);
extern  void            AdvError(int);
extern  void            Error(int,...);
extern  void            Extension(int,...);
extern  void            ClassErr(int,sym_id);
extern  void            TypeTypeErr(int,uint,uint);
extern  void            TypeErr(int,uint);
extern  void            OpndErr(int);
extern  void            PrmCodeErr(int,uint);
extern  void            EndExpr(void);
extern  sym_id          STConst(void *,int,int);
extern  sym_id          STLit(byte *,int);
extern  byte            ParmCode(itnode *);
extern  void            IFCntPrms(uint,byte);
extern  void            SetOpn(itnode *,byte);
extern  void            FiniCat(void);
extern  void            CatBack(void);
extern  void            CatAxeParens(void);
extern  void            CatOpn(void);
extern  void            ChkCatOpn(void);
extern  void            CatParen(void);
extern  void            ParenCat(void);
extern  intstar4        ITIntValue(itnode *);
extern  void            PushOpn(itnode *);
extern  void            ProcList(itnode *);
extern  bool            IsIFMax(uint);
extern  bool            IsIFMin(uint);
extern  void            GIChar(void);
extern  void            GModulus(void);
extern  void            GCharLen(void);
extern  void            GImag(void);
extern  void            GMax(int);
extern  void            GMin(int);
extern  void            GConjg(void);
extern  void            GDProd(void);
extern  void            GXProd(void);
extern  void            GSign(void);
extern  void            GBitTest(void);
extern  void            GBitSet(void);
extern  void            GBitClear(void);
extern  void            GBitOr(void);
extern  void            GBitAnd(void);
extern  void            GBitNot(void);
extern  void            GBitExclOr(void);
extern  void            GBitChange(void);
extern  void            GBitLShift(void);
extern  void            GBitRShift(void);
extern  void            GMod(void);
extern  void            GAbs(void);
extern  void            GASin(void);
extern  void            GACos(void);
extern  void            GATan(void);
extern  void            GATan2(void);
extern  void            GLog(void);
extern  void            GLog10(void);
extern  void            GCos(void);
extern  void            GSin(void);
extern  void            GTan(void);
extern  void            GSinh(void);
extern  void            GCosh(void);
extern  void            GTanh(void);
extern  void            GSqrt(void);
extern  void            GExp(void);
extern  void            GParenExpr(void);
extern  void            GVolatile(void);
extern  void            GLoc(void);
extern  void            GAllocated(void);
extern  sym_id          FindStruct(char *,int);
extern  void            SetDefinedStatus(void);
extern  bool            TypeIs(int);

extern  const unsigned_16 __FAR IFFlags[];
extern  char            *IFNames[];
extern  void            (* const __FAR GenOprTable[])();
extern  void            (* const __FAR ConstTable[])();

static  const byte __FAR        OprIndex[] = {
//     row/column         CITNode->opr
         0,              // 0x00 phi
         1,              // 0x01 start/terminate
         2,              // 0x02 fn/array bracket
         3,              // 0x03 (
         4,              // 0x04 comma
         5,              // 0x05 :
         6,              // 0x06 =
         16,             // 0x07 )
         17,             // 0x08 . field selection
         0,              // 0x09 * (statement number alternate return)
         11,             // 0x0A .EQ.
         11,             // 0x0B .NE.
         11,             // 0x0C .LT.
         11,             // 0x0D .GE.
         11,             // 0x0E .LE.
         11,             // 0x0F .GT.
         7,              // 0x10 .EQV.
         7,              // 0x11 .NEQV.
         8,              // 0x12 .OR.
         9,              // 0x13 .AND.
         10,             // 0x14 .NOT.
         17,             // 0x15 % (field selection)
         0,0,0,0,        // 0x16-0x19 empty spot in OPR codes
         12,             // 0x1A +
         12,             // 0x1B -
         13,             // 0x1C *
         13,             // 0x1D /
         14,             // 0x1E **
         15              // 0x1F //
};

typedef enum {
    BB,BC,BE,BR,BS,CA,EE,GC,GO,PA,RP,FC,MO,BT,CB,CR,CO,CP,EV,HC,LC,PC,PE,KO
} move;

static  const move    __FAR OprSeqMat[] = {
//                                                                       |o   /
//                                                                       |p  /
//                                                                       |r /
//                                                                       |2/
// phi |- [   (   ,   :   =   eqv OR  AND NOT rel +-  */  **  //  )   %  |/opr1
//                                                                       *-----
  MO, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, BT, GO, // phi
  MO, EE, CA, RP, BS, BS, GO, GO, GO, GO, GO, GO, GO, GO, GO, FC, BB, GO, // |-
  MO, BB, CA, RP, PA, LC, BE, GO, GO, GO, GO, GO, GO, GO, GO, FC, PA, GO, // [
  MO, BB, CA, RP, BT, GC, BE, GO, GO, GO, GO, GO, GO, GO, GO, CR, PE, GO, // (
  MO, BS, CA, RP, PA, BS, BE, GO, GO, GO, GO, GO, GO, GO, GO, FC, PA, GO, // ,
  MO, BS, CA, RP, BS, BS, BE, BC, BC, BC, BC, BC, GO, GO, GO, BC, HC, GO, // :
  MO, EV, CA, RP, BS, BS, GO, GO, GO, GO, GO, GO, GO, GO, GO, CB, BB, GO, // =
  MO, BT, CA, RP, BT, BC, BE, BT, GO, GO, GO, GO, GO, GO, GO, FC, BT, GO, // eqv
  MO, BT, CA, RP, BT, BC, BE, BT, GO, GO, GO, GO, GO, GO, GO, FC, BT, GO, // OR
  MO, BT, CA, RP, BT, BC, BE, BT, BT, GO, GO, GO, GO, GO, GO, FC, BT, GO, // AND
  MO, BT, CA, RP, BT, BC, BE, BT, BT, BT, BS, GO, GO, GO, GO, FC, BT, GO, // NOT
  MO, BT, CA, RP, BT, BC, BE, BT, BT, BT, BS, BR, GO, GO, GO, FC, BT, GO, // rel
  MO, BT, CA, RP, BT, BT, BE, BT, BT, BT, BS, BT, BT, GO, GO, CO, BT, GO, // +-
  MO, BT, CA, RP, BT, BT, BE, BT, BT, BT, BS, BT, BT, BT, GO, CO, BT, GO, // */
  MO, BT, CA, RP, BT, BT, BE, BT, BT, BT, BS, BT, BT, BT, GO, CO, BT, GO, // **
  MO, CO, CA, RP, CO, BC, BE, CO, CO, CO, CO, CO, GO, GO, GO, CO, CP, GO, // //
  MO, BT, BS, BS, BT, BT, BT, BT, BT, BT, BS, KO, BT, BT, BT, PC, BT, BT, // )
  MO, EV, CA, EV, EV, EV, EV, EV, EV, EV, EV, EV, EV, EV, EV, CO, EV, GO  // %
};

// Notes:
//        |-      -- start/terminal operator
//        [       -- function/array bracket
//        eqv     -- .EQV.,  .NEQV.
//        rel     -- .LT., .GT., .EQ., .NE., .LE., .GE.
//
//        operator1 is in row, operator2 is in column  ( e.g. op1 * op2 )

//
// LegalOprs -- specifies which operators are legal given the operand
//
// --------------------------------------------------------------------+
//   15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0    |
//                                                                     |
//   //  **   /   *   -   +   r   .   P   =   %   n   a   o   n   e    |
//                            e       H           o   n   r   e   q    |
//                            l       I           t   d       q   v    |
//                                                            v        |
// --------------------------------------------------------------------+
// NOTES : PHI -- not used
//       : rel -- relational operators

#define CT      0x8000          // //
#define RL      0x0200          // .EQ. .NE. .GT. .LT. .GE. .LE.
#define LG      0x000F          // .AND. .OR. .EQV. .NEQV. .XOR.
#define AR      0x7C40          // +, -, *, /, **, =
#define EQ      0x0040          // =
#define NONE    0x0000          // none allowed
#define NOT     0x0010          // .NOT.
#define PLMIN   0x0C00          // +, -
#define IPLMIN  0x0C10          // integer +, -, .NOT.
#define CHAR    (CT+RL+EQ)      // legal character-character ops
#define LOG     (LG+EQ)         // legal logical-logical ops
#define NUMOP   (AR+RL)         // legal number-number ops
#define INUMOP  (LG+AR+RL)      // legal integer-integer ops
#define FLDOP   0x0120          // field selection
#define FLDEQ   0x0160          // field selection, .EQ.

#define LEGALOPR_TAB_ROWS       13
#define LEGALOPR_TAB_COLS       14
#define LEGALOPR_TAB_SIZE       182

static  const unsigned_16     __FAR LegalOprs[] = {                                            // opnd1 /
//                                                                                             //     /
//null  log1 log4  int1   int2   int4   real   dble   xtnd   cmplx  dcmplx xcmplex char struct //   /
// (unary)                                                                                     // /  opnd2
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                                                                                               //
NOT,   LOG,  LOG,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, FLDOP, // log*1
NOT,   LOG,  LOG,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, FLDOP, // log*4
IPLMIN,NONE, NONE, INUMOP,INUMOP,INUMOP,NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // int*1
IPLMIN,NONE, NONE, INUMOP,INUMOP,INUMOP,NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // int*2
IPLMIN,NONE, NONE, INUMOP,INUMOP,INUMOP,NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // int*4
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // real
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // double
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // extend
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // complex
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // dcomplex
PLMIN, NONE, NONE, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NUMOP, NONE, FLDOP, // xcomplex
NONE,  NONE, NONE, NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  CHAR, FLDOP, // character
NONE,  NONE, NONE, NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, FLDEQ  // structure
};



static  bool    SimpleScript( itnode *op ) {
//==========================================

    if( ( op->opn & OPN_WHERE ) != 0 ) return( FALSE );
    switch( op->opn & OPN_WHAT ) {
    case OPN_NNL:
    case OPN_CON:
    case OPN_PHI:
        return( TRUE );
    }
    return( FALSE );
}


static  int     SameScripts( itnode *op1, itnode *op2 ) {
//=======================================================

    if( !SimpleScript( op1 ) ) return( 0 );
    if( ( op1->opn & OPN_WHAT ) == OPN_PHI ) {
        if( ( op2->opn & OPN_WHAT ) == OPN_CON ) {
            return( ITIntValue( op2 ) );
        } else {
            return( 0 );
        }
    }
    if( ( op1->opn & OPN_WHAT ) != ( op2->opn & OPN_WHAT ) ) {
        return( 0 );
    }
    if( ( op1->opn & OPN_WHAT ) == OPN_NNL ) {
        if( op1->sym_ptr == op2->sym_ptr ) {
            return( 1 );
        }
    } else if( ( op1->opn & OPN_WHAT ) == OPN_CON ) {
        return( ITIntValue( op2 ) - ITIntValue( op1 ) + 1 );
    }
    return( 0 );
}


bool    OptimalChSize( uint size ) {
//==================================

    return( ( size == 1 ) || ( size == 2 ) || ( size == 4 ) );
}


static  void    EvalOpn() {
//=========================

// Evaluate operand.

    if( CITNode->opn == OPN_CON ) {
        AddConst( CITNode );
    }
    PushOpn( CITNode );
}


static  void    ParenExpr() {
//===========================

// Finish off evaluation of a parenthesized expression.

    // don't evaluate constants enclosed in parentheses
    // so that they can be folded. Consider: (3+4)+5
    if( CITNode->opn != OPN_CON ) {
        // Consider: CHARACTER A
        //           IF( ('9') .NE. (A) ) CONTINUE
        // make sure that we can optimize the character operation
        if( CITNode->opn == OPN_NNL ) {
            if( CITNode->typ == TY_CHAR ) {
                int     ch_size;

                ch_size = CITNode->size;
                if( OptimalChSize( ch_size ) ) {
                    CITNode->value.st.ss_size = ch_size;
                    CITNode->opn |= OPN_SS1;
                }
            }
        }
        PushOpn( CITNode );
        GParenExpr();
    }
    BackTrack();
}


static  void    ProcOpn() {
//=========================

// Process operand and then scan backwards.
// Currently called for = -| sequence, and most % opr sequences.

    EvalOpn();
    BackTrack();
}


static  void    RemoveParen() {
//=============================

// Upscan routine when second operator is '('.
//
//  Before:                           |   After:
//                 --------------     |                 --------------
//       CIT ==>   | opr1 | PHI |     |                    released**
//                 --------------     |                 --------------
//                 | (    | opn |     |        CIT ==>  | opr1 | opn |
//                 --------------     |                 --------------
//                 | )    | PHI |     |                    released
//                 --------------     |                 --------------
//                 | opr2 |     |     |                 | opr2 |     |
//                 --------------     |                 --------------
//
//  ** see KillOpnOpr() for case where first node is start-node of expr

    itnode      *cit;

    if( CITNode->opn != OPN_PHI ) {
        AdvError( SX_NO_OPR );
    }
    MoveDown();
    KillOpnOpr();
    cit = CITNode;
    if( CITNode->opn == OPN_PHI ) {
        Error( PC_SURP_PAREN );
    }
    CITNode = CITNode->link;
    if( CITNode->opn != OPN_PHI ) {
        Error( SX_NO_OPR );
    }
    cit->link = CITNode->link;
    FreeOneNode( CITNode );
    CITNode = cit;
}


static  void    GrabColon() {
//===========================

// Upscan routine for handling substring indexing expression.
// The operator sequence is OPR_LBR - OPR_COL.
//
// We expect that when this routine is called, there is an array element
// which has a substring expression after it.
//
// Note: Arith() does not process a dim'n declarator containing a ":"
//       since the operator sequences "comma,colon" and "colon,comma"
//       must be disallowed in OprSeqTab ( by syntax of substring expr )

// Consider PRINT *, '1234'(2:3)
// It is illegal to substring constants. Note: If the constant were a
// variable name, DSName() would be called and the OPR_LBR would be
// changed to OPR_FBR and we won't get here.

    if( BkLink->opn != OPN_PHI ) {
        Error( SS_ONLY_IF_CHAR );
    } else {
        KillOpnOpr();
        // Consider PRINT *,(F:80)TEST
        if( CITNode->opr != OPR_TRM ) {
            CITNode->opr = OPR_COM;   // concatenate substring expression
            LowColon();               // onto end of subscript list
        } else {
            Error( SX_BAD_OPR_SEQ );
        }
    }
}


static  void    LowColon() {
//==========================

    // we are looking at the low bound
    if( CITNode->opn == OPN_PHI ) {
        AddSS( 1 );
    }
    PrepArg();
}


static  void    HighColon() {
//===========================

    // must be high bound
    if( CITNode->opn == OPN_PHI ) {
        // we don't know have access to the symbol table
        // entry so we can't compute the size
        CITNode->opn = OPN_SSR;
        CITNode->typ = TY_INTEGER;
        CITNode->size = TypeSize( TY_INTEGER );
    } else if( SimpleScript( CITNode ) &&
               ( (BkLink->opr == OPR_FBR) || (BkLink->opr == OPR_LBR) ) ) {
        int     ch_size;

        ch_size = SameScripts( BkLink, CITNode );
        if( ch_size > 0 ) { // The size no longer has to be optimal, only constant
            CITNode->value.st.ss_size = ch_size;
            CITNode->opn |= OPN_SS1;
        }
    }
    PrepArg();
}


static  void    MkConst( intstar4 number ) {
//==========================================

    CITNode->value.intstar4 = number;
    CITNode->typ  = TY_INTEGER;
    CITNode->size = TypeSize( TY_INTEGER );
    CITNode->opn  = OPN_CON;
}


static  void    AddSS( int number ) {
//===================================

    MkConst( number );
    AddConst( CITNode );
}


static  void    BadSequence() {
//=============================

// Upscan routine for bad sequence of operators.

    AdvError( SX_BAD_OPR_SEQ );
}


static  void    Missing() {
//=========================

// Upscan routine for missing operator.

    AdvError( SX_NO_OPR );
}


static  void    BadBracket() {
//============================

// Upscan routine for odd/unexpected parenthesis sequence.

    Error( PC_SURP_PAREN );
}


void    BadEqual() {
//==================

// Upscan routine for illegal quantity on left side of equal sign

    Error( EQ_BAD_TARGET );
}


static  void    BadRelOpn() {
//===========================

// Upscan routine for relop with log opnd (relop,relop).

    Error( MD_BAD_REL_OPN );
}


static  void    BadColonOpn() {
//=============================

// Upscan routine for ":" operator with invalid operand(s).

    Error( SX_BAD_OPR_SEQ );
}


sym_id    CkAssignOk() {
//======================

// Check if operand is allowed to be assigned a value.

    sym_id      sym;

    switch( CITNode->opn & OPN_WHAT ) {
    case OPN_NNL:
    case OPN_ASS:
    case OPN_NWL:
    case OPN_ARR:
        if( ClassIs( SY_VARIABLE ) ) {
            if( BitOn( SY_DO_PARM ) ) {
                Error( DO_PARM_REDEFINED );
                return( NULL );
            }
            sym = CITNode->sym_ptr;
            // Consider: READ *, CH(I:J)
            // GFiniSS() sets the symbol table entry in the I.T. node
            // to the temporary SCB so we need to get the actual symbol
            // we are substringing elsewhere
            if( CITNode->opn & OPN_ASY ) {
                sym = CITNode->value.st.ss_id;
            }
            sym->ns.xflags |= SY_DEFINED;
            return( sym );
        } else {
            ClassErr( EQ_CANNOT_ASSIGN, CITNode->sym_ptr );
            return( NULL );
        }
        break;
    default:
        Error( EQ_BAD_TARGET );
        return( NULL );
    }
}


static  void    USCleanUp() {
//===========================

// Clean up text list after expression error has occurred
// releasing all nodes on the way, leaving:
//                                               +------------------+
//                                       CIT ==> | OPR_TRM |        |
//                                               +------------------+
//                                               | OPR_TRM |        |
//                                               +------------------+
// NOTE : CITNode must not be pointing at the end of expression terminal

    itnode      *junk;
    itnode      *first;

    while( CITNode->opr != OPR_TRM ) {
        BackTrack();
    }
    first = CITNode;
    switch( first->opn & OPN_WHAT ) {
    case OPN_NWL:
    case OPN_ASS:
        if( first->list != NULL ) {
            FreeITNodes( first->list );
        }
    }
    CITNode = CITNode->link;
    while( CITNode->opr != OPR_TRM ) {
        junk = CITNode;
        CITNode = CITNode->link;
        FreeOneNode( junk );
    }
    first->link = CITNode;
    CITNode = first;
    CITNode->typ = TY_NO_TYPE;
    CITNode->opn = OPN_PHI;
}


static  bool    DoGenerate( int typ1, int typ2, uint *res_size ) {
//================================================================

    if( CITNode->link->opr == OPR_EQU ) {
        ResultType = typ1;
        *res_size = CITNode->size;
        if( ( ASType & AST_ASF ) || CkAssignOk() ) return( TRUE );
        return( FALSE );
    } else {
        if( ( ( typ1 == TY_DOUBLE ) && ( typ2 == TY_COMPLEX ) ) ||
            ( ( typ2 == TY_DOUBLE ) && ( typ1 == TY_COMPLEX ) ) ) {
            ResultType = TY_DCOMPLEX;
            *res_size = TypeSize( TY_DCOMPLEX );
            Extension( MD_DBLE_WITH_CMPLX );
        } else if( ( ( typ1 == TY_TRUE_EXTENDED ) && ( typ2 == TY_COMPLEX ) )
            ||     ( ( typ2 == TY_TRUE_EXTENDED ) && ( typ1 == TY_COMPLEX ) )
            ||     ( ( typ1 == TY_TRUE_EXTENDED ) && ( typ2 == TY_DCOMPLEX ) )
            ||     ( ( typ2 == TY_TRUE_EXTENDED ) && ( typ1 == TY_DCOMPLEX ) ) ) {
            ResultType = TY_XCOMPLEX;
            *res_size = TypeSize( TY_XCOMPLEX );
            Extension( MD_DBLE_WITH_CMPLX );
        } else if( ( typ2 > typ1 ) || ( typ1 == TY_STRUCTURE ) ) {
            ResultType = typ2;
            *res_size = TypeSize( typ2 );
        } else {
            ResultType = typ1;
            if( _IsTypeInteger( ResultType ) ) {
                *res_size = CITNode->size;
                if( *res_size < CITNode->link->size ) {
                    *res_size = CITNode->link->size;
                }
            } else
                *res_size = TypeSize( typ1 );
        }
        return( TRUE );
    }
}


static  byte    OprNum( byte opr ) {
//==================================

    if( ( opr >= FIRST_RELOP ) && ( opr <= LAST_RELOP ) ) {
        opr = 0x09;
    } else {
        opr &= 0x0F;
    }
    return( opr );
}


static  void    FixFldNode( void ) {
//==================================

// Fix CITNode after a field-op has been generated.

    itnode      *next;

    next = CITNode->link;
    if( CITNode->opn & OPN_FLD ) { // sub-field
        CITNode->sym_ptr = next->sym_ptr;
        CITNode->opn = next->opn;
        if( next->opn & OPN_SS1 ) {
            CITNode->value.st.ss_size = next->value.st.ss_size;
        }
    } else {
        if( ( next->opn & OPN_WHAT ) == OPN_ARR ) {
            CITNode->opn &= ~OPN_WHAT;
            CITNode->opn |= OPN_ARR;
            // pass on structure name
            // Consider:        PRINT *, X.Y
            // where Y is an array. We need the field name of
            // array for i/o.
            CITNode->value.st.field_id = next->sym_ptr;
        } else if( next->typ == TY_STRUCTURE ) {
            // pass on structure name
            // Consider:        PRINT *, X.Y(1)
            // where Y is an array of structures. The structure we
            // want to display is for Y, not X.
            CITNode->value.st.field_id = next->sym_ptr;
        }
        if( next->opn & OPN_SS1 ) {
            CITNode->opn |= OPN_SS1;
            CITNode->value.st.ss_size = next->value.st.ss_size;
        }
        // so we can tell the difference between an array of
        // structures and a field of a structure that is an
        // array
        CITNode->opn |= OPN_FLD;
    }
}


static  void    Generate() {
//==========================

// Generate code.

    int         typ1;
    int         typ2;
    byte        op;
    byte        opr;
    itnode      *next;
    unsigned_16 mask;
    uint        res_size;

    next = CITNode->link;
    if( next->opn == OPN_PHI ) {
        BadSequence();
    } else {
        typ1 = CITNode->typ;
        typ2 = next->typ;
        opr = next->opr;
        if( RecNOpn() ) {
            typ1 = -1;
            CITNode->size = next->size;
            if( (opr != OPR_PLS) && (opr != OPR_MIN) && (opr != OPR_NOT) ) {
                BadSequence();
                return;
            }
        }
        op = OprNum( opr );
        mask = LegalOprs[ typ2 * LEGALOPR_TAB_COLS + typ1 + 1 ];
        if( ( ( mask >> op ) & 1 ) == 0 ) {
            // illegal combination
            MoveDown();
            if( typ1 == -1 ) {
                TypeErr( MD_UNARY_OP, typ2 );
            } else if( typ1 == typ2 ) {
                TypeErr( MD_ILL_OPR, typ1 );
            } else {
                TypeTypeErr( MD_MIXED, typ1, typ2 );
            }
            BackTrack();
        } else if( DoGenerate( typ1, typ2, &res_size ) ) {
            if( ( opr >= FIRST_RELOP ) && ( opr <= LAST_RELOP ) &&
                ( (ResultType == TY_COMPLEX) || (ResultType == TY_DCOMPLEX) ||
                (ResultType == TY_XCOMPLEX) ) &&
                ( opr != OPR_EQ ) && ( opr != OPR_NE ) ) {
                // can only compare complex with .EQ. and .NE.
                Error( MD_RELOP_OPND_COMPLEX );
            } else {
                if( ( next->opn == OPN_CON ) &&
                    ( ( CITNode->opn == OPN_CON ) || ( typ1 == -1 ) ) ) {
                    // we can do some constant folding
                    ConstTable[ op ]( typ1, typ2, op );
                } else {
                    // we have to generate code
                    if( CITNode->opn == OPN_CON ) {
                        AddConst( CITNode );
                    } else if( next->opn == OPN_CON ) {
                        AddConst( next );
                    }
                    GenOprTable[ op ]( typ1, typ2, op );
                }
            }
            switch( opr ) {
            case OPR_EQV:
            case OPR_NEQV:
            case OPR_OR:
            case OPR_AND:
            case OPR_NOT:
                if( _IsTypeInteger( typ1 ) ) {
                    Extension( MD_LOGOPR_INTOPN );
                }
                break;
            case OPR_EQ:        // relational operators
            case OPR_NE:
            case OPR_LT:
            case OPR_GE:
            case OPR_LE:
            case OPR_GT:
                ResultType = TY_LOGICAL;
                res_size = TypeSize( ResultType );
                break;
            case OPR_FLD:
            case OPR_DPT:
                // set result size to size of field
                res_size = next->size;
                FixFldNode();
                break;
            }
            CITNode->size = res_size;
            CITNode->typ = ResultType;
            FixList();
        }
    }
}


void    AddConst( itnode *node ) {
//================================

// Add constant to symbol table.

    cstring     *val_ptr;

    val_ptr = &node->value.cstring;
    if( node->typ != TY_CHAR ) {
        node->sym_ptr = STConst( val_ptr, node->typ, node->size );
    } else {
        if( node->value.cstring.len == 0 ) {
            Error( CN_ZERO_LEN );
        }
        node->sym_ptr = STLit( val_ptr->strptr, val_ptr->len );
    }
}


static  byte    IFPromote( byte typ ) {
//=====================================
// if the promote switch is activated we promote certain integer intrinsics
// arguments

    if( ( Options & OPT_PROMOTE ) && _IsTypeInteger( typ ) ) {
        typ = TY_INTEGER;
    }
    return( typ );
}


static  void    Call() {
//======================

// Upscan routine for either (1) calling a function or subroutine
//                           (2) detaching a subscript list
//                           (3) detaching a substring expression
//

    int         ifunc;

    if( Subscripted() ) {
        DetSubList();
        EvalList();
    } else if( ClassIs( SY_SUBPROGRAM ) ) {
        if( BitOn( SY_INTRINSIC ) ) {
            if( CITNode->link->opn != OPN_PHI ) {  // consider IFIX()
                if( IFSpecific( IFPromote( CITNode->link->typ ) ) == MAGIC ) {
                    DetCallList();
                    IFPrmChk();     // must do before InLineCnvt and ...
                                    // ... after IFSpecific, DetCallList
                    ifunc = CITNode->sym_ptr->ns.si.fi.index;
                    if( ( IsIFMax( ifunc ) || IsIFMin( ifunc ) ) &&
                        ( CITNode->sym_ptr->ns.si.fi.num_args > 2 ) )
                        EvalList();
                    else
                        InlineCnvt();
                } else {
                    DetCallList();
                    IFPrmChk();
                    EvalList();
                }
            } else {
                Error( LI_NO_PARM );
            }
        } else {
            DetCallList();
            EvalList();
        }
    } else {
        DetSStr();
        EvalList();
    }
}


static  void    EvalList() {
//==========================

    if( !AError ) {
        if( RecNextOpr( OPR_EQU ) && !( CITNode->opn & OPN_FLD ) ) {
            SetDefinedStatus();
        }
        ProcList( CITNode );
    }
}


static  void    IFPrmChk() {
//==========================

// Check that argument types agree with those intrinsic func expects.
//     (1) check that argument codes are acceptable ( e.g. ~ array )
//     (2) check correct number of arguments has been passed
//

    sym_id      sym;
    itnode      *oldcit;
    int         ifn;
    byte        prm_cnt;
    byte        typ;
    byte        code;

    sym = CITNode->sym_ptr;
    ifn = sym->ns.si.fi.index;
    typ = IFArgType( ifn );
    oldcit = CITNode;
    CITNode = oldcit->list;
    prm_cnt = 0;
    for(;;) {
        if( RecColon() ) break;         // substring the i.f.
        if( RecCloseParen() ) break;    // end of list
        code = ParmCode( CITNode );
        switch( ifn ) {
        case IF_ALLOCATED: {
            sym_id      sym = CITNode->sym_ptr;

            if( (code == PC_ARRAY_NAME) && _Allocatable( sym ) ) break;
            if( (code == PC_VARIABLE) && (sym->ns.typ == TY_CHAR) &&
                (sym->ns.xt.size == 0) && !(sym->ns.flags & SY_SUB_PARM) ) {
                sym->ns.xflags |= SY_ALLOCATABLE;
                break;
            }
            Error( LI_ARG_ALLOCATED );
            break;
        }
        case IF_ISIZEOF:
            switch( CITNode->opn ) {
            case OPN_NNL:
                MkConst( _SymSize( CITNode->sym_ptr ) );
                break;
            case OPN_CON:
                if( CITNode->typ == TY_CHAR ) {
                    MkConst( CITNode->value.cstring.len );
                } else if( CITNode->typ == TY_STRUCTURE ) {
                    MkConst( CITNode->value.intstar4 );
                } else {
                    MkConst( CITNode->size );
                }
                break;
            case OPN_ARR:
                MkConst( CITNode->sym_ptr->ns.si.va.dim_ext->num_elts *
                         _SymSize( CITNode->sym_ptr ) );
                break;
            default:
                Error( LI_ARG_ISIZEOF );
                break;
            }
            break;
        case IF_LOC:
        case IF_VOLATILE:
            break;
        default:
            if( CITNode->typ != typ ) {
                if( ( (CITNode->opn & OPN_WHAT) == OPN_CON) && TypeIs( typ ) ) {
                    // we don't want an error in the following case:
                    //          INTEGER*2 I
                    //          PRINT *, MOD( I, 3 )
                    // I is INTEGER*2 and 3 is INTEGER*4
                    CnvTo( CITNode, typ, TypeSize( typ ) );
                } else if( ( Options & OPT_PROMOTE ) && ( typ == TY_INTEGER ) &&
                           TypeIs( typ ) ) {
                    // check if we should allow
                    //  INTEGER*1 I
                    //  I = 13
                    //  PRINT *, IABS( I )
                    // if the users turns on the switch we allow it
                    break;
                } else {
                    switch( ifn ) {
                    case IF_INT:
                    case IF_REAL:
                    case IF_DBLE:
                    case IF_QEXT:
                    case IF_CMPLX:
                    case IF_DCMPLX:
                    case IF_QCMPLX:
                        break;
                    case IF_FLOAT:
                    case IF_DFLOAT:
                    case IF_CHAR:
                        if( TypeIs( typ ) ) {
                            break;
                        }
                        // else drop through to error
                    default:
                        // consider:
                        //          I = 5
                        //          PRINT *, ANINT(I)
                        // we don't want to issue 2 error msgs
                        if( !AError ) {
                            TypeTypeErr( LI_PT_MISMATCH, typ, CITNode->typ );
                        }
                        break;
                    }
                }
            }
            switch( code ) {
            case PC_CONST:
            case PC_VARIABLE:
            case PC_ARRAY_ELT:
            case PC_SS_ARRAY:
                break;
            default:
                PrmCodeErr( LI_ILL_PARM_CODE, code );
                break;
            }
            break;
        }
        AdvanceITPtr();
        ++prm_cnt;
    }
    // for intrinsic functions that take variable # of args (e.g. MAX, MIN)
    sym->ns.si.fi.num_args = prm_cnt;
    CITNode = oldcit;
    IFCntPrms( ifn, prm_cnt );
}


static  bool    IFAsOperator() {
//==============================

    if( CITNode->opr != OPR_FBR ) return( FALSE );
    if( ( BkLink->flags & SY_CLASS ) != SY_SUBPROGRAM ) return( FALSE );
    if( !(BkLink->flags & SY_INTRINSIC) ) return( FALSE );
    switch( BkLink->sym_ptr->ns.si.fi.index ) {
    case IF_ISIZEOF:
    case IF_ALLOCATED:
    case IF_VOLATILE:
    case IF_CHAR:
        return( TRUE );
    }
    return( FALSE );
}


static  void    PrepArg() {
//=========================

// Upscan routine to prepare an item in a function or subscript list.

    int         if_index;

    if( ClassIs( SY_SUBPROGRAM ) && BitOn( SY_INTRINSIC ) ) {
        if( CITNode->opn == OPN_NNL ) {
            if_index = CITNode->sym_ptr->ns.si.fi.index;
            if( ( IFFlags[ if_index ] & IF_ARG_OK ) == 0 ) {
                Error( LI_NOT_PARM );
            } else {
                MarkIFUsed( if_index );
                CITNode->sym_ptr->ns.flags |= SY_IF_ARGUMENT;
            }
        }
    }
    if( ( CITNode->opn & OPN_WHAT ) == OPN_STN ) {
        if( !(ASType & AST_ALT) ) {
            PrmCodeErr( SR_ILL_PARM, PC_STATEMENT );
        }
    } else {
        if( !IFAsOperator() ) {
            if( CITNode->opn == OPN_CON ) {
                AddConst( CITNode );
            }
            if( ( CITNode->opn & OPN_SS1 ) == 0 )
            GArg();
        }
    }
    BackTrack();
}


static  void    FixList() {
//=========================

// Remove the second operand itnode after code generation
// and update variable type or result.

    itnode      *junk;

    junk = CITNode->link;
    CITNode->link = junk->link;
    FreeOneNode( junk );
}


static  void    InlineCnvt() {
//============================

// Do conversion routines inline (no function call).

    itnode      *cit;
    int         ifn;
    byte        typ;
    byte        func_type;

    cit = CITNode;
    ifn = CITNode->sym_ptr->ns.si.fi.index;
    func_type = CITNode->typ;
    CITNode = CITNode->list;
    typ = CITNode->typ;
    if( ifn == IF_ALLOCATED ) {
        GAllocated();
    } else if( ifn == IF_LOC ) {
        GLoc();
    } else if( ifn == IF_VOLATILE ) {
        func_type = typ;
        GVolatile();
    } else if( ifn == IF_CHAR ) {
        if( CITNode->opn == OPN_CON ) {
            intstar4    arg;

            arg = ITIntValue( CITNode );
            // see comments in IFCHAR
            if( ( arg < -128 ) || ( arg > 255 ) ) {
                Error( LI_CHAR_BOUND );
                arg = '?';
            }
            cit->value.cstring.data = arg;
            cit->value.cstring.strptr = &cit->value.cstring.data;
            cit->value.cstring.len = 1;
            cit->opn = OPN_CON;
            cit->flags = 0;
        } else {
            GArg();
            GILCnvTo( TY_INTEGER, TypeSize( TY_INTEGER ) );
            ProcList( cit );
            MarkIFUsed( IF_CHAR );
            CITNode = cit;
            return;
        }
    } else if( ifn == IF_ISIZEOF ) {
        cit->value.intstar4 = CITNode->value.intstar4;
        cit->opn = OPN_CON;
        cit->flags = 0;
    } else if( ( typ >= TY_INTEGER_1 ) && ( typ <= TY_XCOMPLEX ) ) {
        // this switch statement replaces a huge if() statement for speed
        switch( ifn ) {
        case IF_CMPLX:
            switch( typ ) {
            case( TY_DOUBLE ):
                ifn = IF_DCMPLX;
                func_type = TY_DCOMPLEX;
                break;
            case( TY_TRUE_EXTENDED ):
                ifn = IF_QCMPLX;
                func_type = TY_XCOMPLEX;
                break;
            }
        case IF_DCMPLX:
        case IF_QCMPLX:
            if( RecNextOpr( OPR_COM ) ) {
                if( CITNode->link->typ == typ ) {
                    if( typ <= TY_EXTENDED ) {
                        if( ifn == IF_CMPLX ) {
                            GMakeCplx();
                        } else if ( ifn == IF_DCMPLX ) {
                            GMakeDCplx();
                        } else {
                            GMakeXCplx();
                        }
                    } else {
                        Error( LI_2_CMPLX, IFNames[ ifn ] );
                        AdvanceITPtr();
                    }
                } else {
                    Error( LI_CMPLX_TYPES, IFNames[ ifn ] );
                    AdvanceITPtr();
                }
            } else {
                GILCnvTo( func_type, TypeSize( func_type ) );
            }
            break;
        case IF_HFIX:
            GILCnvTo( func_type, sizeof( intstar2 ) );
            break;
        case IF_I1MOD:
        case IF_I2MOD:
        case IF_MOD:            GModulus();     break;
        case IF_AIMAG:
        case IF_QIMAG:
        case IF_DIMAG:          GImag();        break;
        case IF_CONJG:
        case IF_QCONJG:
        case IF_DCONJG:         GConjg();       break;
        case IF_DPROD:          GDProd();       break;
        case IF_QPROD:          GXProd();       break;
        case IF_I1BTEST:
        case IF_I2BTEST:
        case IF_BTEST:          GBitTest();     break;
        case IF_I1BSET:
        case IF_I2BSET:
        case IF_IBSET:          GBitSet();      break;
        case IF_I1BCLR:
        case IF_I2BCLR:
        case IF_IBCLR:          GBitClear();    break;
        case IF_I1OR:
        case IF_I2OR:
        case IF_IOR:            GBitOr();       break;
        case IF_I1AND:
        case IF_I2AND:
        case IF_IAND:           GBitAnd();      break;
        case IF_I1NOT:
        case IF_I2NOT:
        case IF_NOT:            GBitNot();      break;
        case IF_I1EOR:
        case IF_I2EOR:
        case IF_IEOR:           GBitExclOr();   break;
        case IF_I1BCHNG:
        case IF_I2BCHNG:
        case IF_IBCHNG:         GBitChange();   break;
        case IF_AMOD:
        case IF_QMOD:
        case IF_DMOD:           GMod();         break;
        case IF_ABS:
        case IF_IABS:
        case IF_I1ABS:
        case IF_I2ABS:
        case IF_QABS:
        case IF_DABS:           GAbs();         break;
        case IF_ASIN:
        case IF_QASIN:
        case IF_DASIN:          GASin();        break;
        case IF_ACOS:
        case IF_QACOS:
        case IF_DACOS:          GACos();        break;
        case IF_ATAN:
        case IF_QATAN:
        case IF_DATAN:          GATan();        break;
        case IF_ATAN2:
        case IF_QATAN2:
        case IF_DATAN2:         GATan2();       break;
        case IF_ALOG:
        case IF_QLOG:
        case IF_DLOG:           GLog();         break;
        case IF_ALOG10:
        case IF_QLOG10:
        case IF_DLOG10:         GLog10();       break;
        case IF_COS:
        case IF_QCOS:
        case IF_DCOS:           GCos();         break;
        case IF_SIN:
        case IF_QSIN:
        case IF_DSIN:           GSin();         break;
        case IF_TAN:
        case IF_QTAN:
        case IF_DTAN:           GTan();         break;
        case IF_SINH:
        case IF_QSINH:
        case IF_DSINH:          GSinh();        break;
        case IF_COSH:
        case IF_QCOSH:
        case IF_DCOSH:          GCosh();        break;
        case IF_TANH:
        case IF_QTANH:
        case IF_DTANH:          GTanh();        break;
        case IF_SQRT:
        case IF_QSQRT:
        case IF_DSQRT:          GSqrt();        break;
        case IF_EXP:
        case IF_QEXP:
        case IF_DEXP:           GExp();         break;
        case IF_SIGN:
        case IF_DSIGN:
        case IF_ISIGN:
        case IF_I1SIGN:
        case IF_I2SIGN:
        case IF_QSIGN:          GSign();        break;
        case IF_I1LSHIFT:
        case IF_I2LSHIFT:
        case IF_LSHIFT:         GBitLShift();   break;
        case IF_I1RSHIFT:
        case IF_I2RSHIFT:
        case IF_RSHIFT:         GBitRShift();   break;
        // IsIFMax() stuff... be sure to keep this up to date with iflookup.c
        case IF_AMAX0:
        case IF_AMAX1:
        case IF_DMAX1:
        case IF_QMAX1:
        case IF_I1MAX0:
        case IF_I2MAX0:
        case IF_MAX0:
        case IF_MAX1:           GMax( func_type );      break;
        // IsIfMin() stuff... be sure to keep this up to date with iflookup.c
        case IF_AMIN0:
        case IF_AMIN1:
        case IF_DMIN1:
        case IF_QMIN1:
        case IF_I1MIN0:
        case IF_I2MIN0:
        case IF_MIN0:
        case IF_MIN1:           GMin( func_type );      break;
        case IF_REAL:           // Make sure that D<-REAL(Z) && X<-REAL(Q)
            switch( typ ) {
            case( TY_DCOMPLEX ):
                func_type = TY_DOUBLE;
                break;
            case( TY_TRUE_XCOMPLEX ):
                func_type = TY_EXTENDED;
                break;
            break;
            }                   // Fall through to default:
        default:
            GILCnvTo( func_type, TypeSize( func_type ) );
            break;
        }
    } else if( IFIsGeneric( ifn ) ) {
        TypeErr( LI_EXP_CNV_TYPE, typ );      // typ is what we got
    } else if( ifn == IF_LEN ) {
        GCharLen();
    } else { // ichar
        GIChar();
    }
    AdvanceITPtr();
    ReqCloseParen();
    CITNode = cit;
    cit = CITNode->list;
    CITNode->typ = func_type;
    CITNode->size = TypeSize( func_type );
    SetOpn( CITNode, cit->opn & OPN_WHERE );
    CITNode->list = NULL;
    UpdateNode( CITNode, cit );
    FreeITNodes( cit );
}


static  const void (* const __FAR RtnTable[])() = {
         &BadBracket,           // for odd parenthesis sequences
         &BadColonOpn,          // colon operator expects integer operands
         &BadEqual,             // illegal quantity on left side of =
         &BadRelOpn,            // relational operator has logical operand
         &BadSequence,          // bad sequence of operators
         &Call,                 // detach ss list, substr list, call subprog
         &EndExpr,              // opr sequence is start,terminate
         &GrabColon,            // substring indexing expression using :
         &Generate,             // go and generate some code
         &PrepArg,              // prepare item in function or subscript list
         &RemoveParen,          // remove parenthesis
         &FiniCat,              // finish concatenation
         &Missing,              // missing operator
         &BackTrack,            // scan backwards
         &CatBack,              // maybe scan backwards on = // sequence
         &CatAxeParens,         // remove parenthesis set on ( // sequence
         &CatOpn,               // concatenation operand
         &CatParen,             // check if ) is for a substring operand
         &ProcOpn,              // process operand and then scan backwards
         &HighColon,            // check for OPN_PHI on : ) sequence
         &LowColon,             // handle [ : and called from GrabColon
         &ParenCat,             // check if ) is for a substring operand
         &ParenExpr,            // done evaluating parenthesized expression
         &ChkCatOpn             // check if concatenation operand
};


void    UpScan() {
//================

// Upscan phase of expression processor.
// On entry, CITNode is OPR_TRM at end of expression
// Action routines move CITNode appropriately each time through loop.

    int         index;

    BackTrack();
    for(;;) {
        if( AError ) {
            USCleanUp();
            break;
        }
        if( ( CITNode->opr == OPR_TRM ) &&
            ( CITNode->link->opr == OPR_TRM ) ) break;
        index = OprIndex[ CITNode->opr ];
        index = index * OPR_SEQ_MAT_COLS + OprIndex[ CITNode->link->opr ];
        RtnTable[ OprSeqMat[ index ] ]();
    }
    EndExpr();
}
