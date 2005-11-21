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
* Description:  Arithmetic downscan.
*
****************************************************************************/


#include "ftnstd.h"
#include "opr.h"
#include "opn.h"
#include "errcod.h"
#include "astype.h"
#include "global.h"
#include "intcnv.h"
#include "fltcnv.h"
#include "types.h"
#include "cpopt.h"
#include "recog.h"
#include "types.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

extern  void            Error(int,...);
extern  void            Warning(int,...);
extern  void            Extension(int,...);
extern  void            OpndErr(int);
extern  void            MoveDown(void);
extern  void            DSName(void);
extern  void            FreeITNodes(itnode *);
extern  void            FreeOneNode(itnode *);
extern  void            AdvanceITPtr(void);
extern  sym_id          LkUpStmtNo(void);
extern  int             FmtS2I(char *,int,bool,intstar4 *,bool,int *);
extern  int             FmtS2F(char *,int,int,bool,int,int,reallong *,bool,int *,bool);


static  void    LitC() {
//======================

    CITNode->value.cstring.strptr = CITNode->opnd;
    CITNode->value.cstring.len = CITNode->opnd_size;
    CITNode->typ = TY_CHAR;
    CITNode->size = CITNode->opnd_size;
    CITNode->opn.us = USOPN_CON;
}


static  void    LogC() {
//======================

    CITNode->value.logstar1 = *CITNode->opnd == 'T';
    CITNode->typ = TY_LOGICAL;
    CITNode->size = TypeSize( TY_LOGICAL );
    CITNode->opn.us = USOPN_CON;
}


static  void    IntC() {
//======================

    if( FmtS2I( CITNode->opnd, CITNode->opnd_size, FALSE, &CITNode->value.intstar4, FALSE, NULL ) != INT_OK ) {
        // don't issue an overflow for -2147483648
        // but we need to be careful since we do want an
        // overflow for I - 2147483648
        if( !(((BkLink == NULL) || (BkLink->opn.ds == DSOPN_PHI)) &&
              (CITNode->opr == OPR_MIN) &&
              (CITNode->value.intstar4 == LONG_MIN)) ) {
            Warning( KO_IOVERFLOW );
        }
    }
    CITNode->typ = TY_INTEGER;
    CITNode->size = TypeSize( TY_INTEGER );
    CITNode->opn.us = USOPN_CON;
}


static  bool    CnvFloat( itnode *cit, int prec ) {
//=================================================

    bool        ext;

    ext = ( Options & OPT_EXTEND_REAL ) != 0;
    if( FmtS2F( cit->opnd, cit->opnd_size, 0, FALSE, 0, prec,
                &cit->value.dble, FALSE, NULL, ext ) != FLT_OK ) {
        OpndErr( CN_FLOAT );
        return( FALSE );
    }
    return( TRUE );
}


static  void    RealC() {
//=======================

    if( CnvFloat( CITNode, PRECISION_SINGLE ) ) {
        CITNode->value.single = CITNode->value.extended;
    }
    CITNode->typ = TY_REAL;
    CITNode->size = TypeSize( TY_REAL );
    CITNode->opn.us = USOPN_CON;
}


static  void    DoubleC() {
//=========================

    if( CnvFloat( CITNode, PRECISION_DOUBLE ) ) {
        CITNode->value.dble = CITNode->value.extended;
    }
    CITNode->typ = TY_DOUBLE;
    CITNode->size = TypeSize( TY_DOUBLE );
    CITNode->opn.us = USOPN_CON;
}


static  void    ExtendedC() {
//===========================

    CnvFloat( CITNode, PRECISION_EXTENDED );
    CITNode->typ = TY_EXTENDED;
    CITNode->size = TypeSize( TY_EXTENDED );
    CITNode->opn.us = USOPN_CON;
}


static  void    OctalC() {
//========================

    ConstBase( 8 );
    CITNode->opn.us = USOPN_CON;
}


static  void    HexC() {
//======================

    ConstBase( 16 );
    CITNode->opn.us = USOPN_CON;
}


static  void    ConstBase( uint base ) {
//======================================

    char        *end;

    Extension( CN_HEX_OCT );
    errno = 0;
    // Note: CITNode->opnd_size includes NULLCHAR
    if( CITNode->opnd_size <= 3 ) {
        CITNode->value.intstar1 = strtoul( CITNode->opnd, &end, base );
        CITNode->size = sizeof( intstar1 );
        CITNode->typ = TY_INTEGER_1;
    } else if( CITNode->opnd_size <= 5 ) {
        CITNode->value.intstar2 = strtoul( CITNode->opnd, &end, base );
        CITNode->size = sizeof( intstar2 );
        CITNode->typ = TY_INTEGER_2;
    } else {
        CITNode->value.intstar4 = strtoul( CITNode->opnd, &end, base );
        CITNode->size = sizeof( intstar4 );
        CITNode->typ = TY_INTEGER;
    }
    if( ( *end != NULLCHAR ) || ( errno != 0 ) ) {
        Error( CN_BAD_HEX_OCT );
    }
    CITNode->is_unsigned = 1;
}


static  bool    Number() {
//========================

    if( CITNode->opn.ds == DSOPN_PHI ) {
        AdvanceITPtr();
        if( !RecPlus() && !RecMin() ) return( FALSE );
    }
    if( CITNode->opn.ds < DSOPN_INT ) return( FALSE );
    AdvanceITPtr();
    return( TRUE );
}


static  bool    Complex() {
//=========================

    if( Number() && RecComma() && Number() && RecCloseParen() ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}


static  itnode  *CollectNumber( itnode *itptr, int *sign ) {
//==========================================================

    itnode      *save_node;

    save_node = itptr;
    *sign = 1;
    if( itptr->opn.ds == DSOPN_PHI ) {
        itptr = itptr->link;
        if( itptr->opr == OPR_MIN ) {
            *sign = -1;
        } else if( itptr->opr != OPR_PLS ) {
            *sign = 0;
            return( itptr );
        }
    }
    if( itptr->opn.ds >= DSOPN_INT ) {
        if( save_node->opn.ds == DSOPN_PHI ) {
            CITNode->link = itptr;
            itptr->opr = save_node->opr;
            itptr->oprpos = save_node->oprpos;
            FreeOneNode( save_node );
        }
        CITNode = itptr;
        itptr = itptr->link;
    } else {
        *sign = 0;
    }
    return( itptr );
}


#if _CPU == _VAX

static  void    PassBy( OPR opr) {
//================================

    itnode      *itptr;

    if( CITNode->link->opn.ds != DSOPN_PHI ) {
        itptr = CITNode;
        CITNode = CITNode->link;
        itptr->link = NULL;
        FreeITNodes( itptr );
        CITNode->opr = itptr->opr;     // must come before DSTable[]()
        DSTable[ CITNode->opn ]();
    }
    if( opr == OPR_DIV ) {
        CITNode->pass_by |= PASS_BY_VALUE;
    } else if( opr == OPR_AMP ) {
        CITNode->pass_by |= PASS_BY_REF;
    } else {
        CITNode->pass_by |= PASS_BY_DESCR;
    }
}

#endif


static  void    Phi() {
//=====================

// Processing a null operand in an expression.
//
//         1. may be start of a complex constant
//         2. may be start of an alternate return specifier
//         3. may just be two operators in a row
//

    OPR         opr1;
    OPR         opr2;
    itnode      *itptr;
    itnode      *save_cit;
    int         real_sign;
    int         imag_sign;

    opr2 = CITNode->link->opr;
    if( opr2 == OPR_LBR ) {
        itptr = CITNode->link;
        save_cit = CITNode;
        CITNode = itptr;
        if( Complex() ) {
            CITNode = save_cit;
            itptr = CollectNumber( itptr, &real_sign );
            itptr = CollectNumber( itptr, &imag_sign );
            CITNode = save_cit;
            BuildCplx( real_sign, imag_sign );
        } else {
            CITNode = save_cit;
        }
    } else {
        opr1 = CITNode->opr;
        if( ( opr1 == OPR_FBR ) || ( opr1 == OPR_COM ) ) {
            if( opr2 == OPR_MUL ) {
                if( CITNode->link->opn.ds == DSOPN_INT ) {
                    if( ( CITNode->link->link->opr == OPR_COM ) ||
                        ( CITNode->link->link->opr == OPR_RBR ) ) {
                        AltReturn();
                    }
                }
#if _CPU == _VAX
            } else if( ( opr2 == OPR_DIV ) ||     // pass arugment by value
                       ( opr2 == OPR_AMP ) ||     // pass argument by address
                       ( opr2 == OPR_FLD ) ) {    // pass arugment by descriptor
                PassBy( opr2 );
#endif
            }
        }
    }
}


static  void    BuildCplx( int real_sign, int imag_sign ) {
//=========================================================

    itnode      *itptr;
    itnode      *stop;
    ftn_type    *val;

    val = &CITNode->value;
    itptr = CITNode->link;
    if( ( itptr->opn.ds == DSOPN_EXT ) || ( itptr->link->opn.ds == DSOPN_EXT ) ) {
        CITNode->typ = TY_XCOMPLEX;
        if( CnvFloat( itptr, PRECISION_EXTENDED ) ) {
            val->xcomplex.realpart = real_sign * itptr->value.extended;
        }
        itptr = itptr->link;
        if( CnvFloat( itptr, PRECISION_EXTENDED ) ) {
            val->xcomplex.imagpart = imag_sign * itptr->value.extended;
        }
        Extension( CN_DOUBLE_COMPLEX );
    } else if( ( itptr->opn.ds == DSOPN_DBL ) || ( itptr->link->opn.ds == DSOPN_DBL ) ) {
        CITNode->typ = TY_DCOMPLEX;
        if( CnvFloat( itptr, PRECISION_DOUBLE ) ) {
            val->dcomplex.realpart = real_sign * itptr->value.extended;
        }
        itptr = itptr->link;
        if( CnvFloat( itptr, PRECISION_DOUBLE ) ) {
            val->dcomplex.imagpart = imag_sign * itptr->value.extended;
        }
        Extension( CN_DOUBLE_COMPLEX );
    } else {
        CITNode->typ = TY_COMPLEX;
        if( CnvFloat( itptr, PRECISION_SINGLE ) ) {
            val->complex.realpart = real_sign * itptr->value.extended;
        }
        itptr = itptr->link;
        if( CnvFloat( itptr, PRECISION_SINGLE ) ) {
            val->complex.imagpart = imag_sign * itptr->value.extended;
        }
    }
    stop = itptr;
    itptr = itptr->link;
    if( itptr->opn.ds == DSOPN_PHI ) {
        stop = stop->link;
    } else {
        itptr->opr = OPR_PHI;
        itptr->oprpos = itptr->opnpos;
    }
    itptr = CITNode->link;
    CITNode->link = stop->link;
    stop->link = NULL;
    FreeITNodes( itptr );
    CITNode->opn.us = USOPN_CON;
    CITNode->size = TypeSize( CITNode->typ );
}


static  void    AltReturn() {
//===========================

    itnode      *itptr;

    itptr = CITNode;
    CITNode = CITNode->link;
    itptr->sym_ptr = LkUpStmtNo();
    itptr->opn.us = USOPN_STN;
    itptr->link = CITNode->link;
    CITNode->link = NULL;
    FreeITNodes( CITNode );
    CITNode = itptr;
}


static  void    OprEqu() {
//========================

    if( ( ASType & AST_EOK ) == 0 ) {
        Error( EQ_ILL_EQ_SIGN );
        return;
    }
    if( ASType & AST_MEQ ) {
        if( ( ASType & AST_ASF ) || ( StmtProc != PR_ASNMNT ) ) {
            Error( EQ_ILL_EQ_SIGN );
            return;
        }
        if( ( ASType & AST_MSG ) == 0 ) {
            Extension( EQ_MULT_ASSGN );
            ASType |= AST_MSG;
        }
    } else {
        ASType |= AST_MEQ;
    }
}

#ifdef pick
#undef pick
#endif
#define pick(tok_id,dsopn_id,opn_proc) opn_proc,

static void (* const __FAR DSTable[])() = {
#include "tokdsopn.h"
};


void    GetConst() {
//==================

// Constant converting without downscan-upscan process.

    DSTable[ CITNode->opn.ds ]();
    if( CITNode->opn.us != USOPN_CON ) {
        Error( SX_CONST );
    }
}


void    GetIntConst() {
//=====================

    GetConst();
    if( !_IsTypeInteger( CITNode->typ ) ) {
        Error( SX_EXPECT_INT );
    }
}


void    DownScan() {
//==================

    AError = FALSE;
    BkLink = NULL;
    FieldNode = NULL;
    for(;;) {
        if( CITNode->opr == OPR_EQU ) {
            OprEqu();
        }
        DSTable[ CITNode->opn.ds ]();
        MoveDown();
        if( RecTrmOpr() ) break;
    }
}
