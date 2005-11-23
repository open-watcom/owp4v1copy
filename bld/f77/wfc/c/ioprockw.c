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
// IOPROCKW  : processing for the i/o keyword list
//

#include "ftnstd.h"
#include "iodefn.h"
#include "errcod.h"
#include "opr.h"
#include "opn.h"
#include "rtconst.h"
#include "units.h"
#include "global.h"
#include "recog.h"
#include "types.h"
#include "ferror.h"
#include "insert.h"
#include "utility.h"

extern  int             RecIOKW(void);
extern  bool            Permission(int);
extern  void            CheckList(void);
extern  void            KWRememb(int);
extern  sym_id          LkUpStmtNo(void);
extern  sym_id          LkUpFormat(void);
extern  void            GFmtVarSet(void);
extern  void            GFmtArrSet(void);
extern  void            GFmtExprSet(void);
extern  void            GPassStmtNo(sym_id,uint);
extern  void            GPassValue(uint);
extern  void            GPassAddr(uint);
extern  void            GPassLabel(label_id,uint);
extern  void            GArrIntlSet(void);
extern  void            GIntlSet(void);
extern  sym_id          CkAssignOk(void);
extern  void            AddConst(itnode *);
extern  label_id        NextLabel(void);
extern  void            FScan(int,char *,cs_label);
extern  void            ScanExpr(void);
extern  void            FreeLabel(label_id);
extern  sym_id          NameListFind(void);
extern  void            GSetNameList(uint);
extern  void            CkVarRef(void);
extern  void            BIOutNameList(sym_id);


static bool FormatIdentifier() {
//==============================

    if( RecComma() && RecNextOpr( OPR_EQU ) ) return FALSE;
    if( RecCloseParen() ) return FALSE;
    if( RecTrmOpr() ) return FALSE;
    ReqComma();
    return TRUE;
}


void    KeywordList() {
//=====================

    bool        morelist;

    if( RecNextOpr( OPR_EQU ) ) {
        morelist = TRUE;
    } else {
        if( Permission( IO_UNIT ) ) {
            Unit();
            AdvanceITPtr();
        } else {
            NextComma();
        }
        if( FormatIdentifier() ) {
            if( Permission( IO_FMT ) ) {
                FormatIdd();
                AdvanceITPtr();
            } else {
                NextComma();
            }
        }
        morelist = RecComma();
    }
    if( morelist ) {
        for(;;) {
            GetItem();
            if( !RecComma() ) break;
        }
    }
    CheckList();               // check that list had necessities ( unit )
                               // also remember if end= is specified
}


static  void    GetItem() {
//=========================

    int         kw;

    kw = RecIOKW();
    if( Permission( kw ) ) {
        AdvanceITPtr();
        ReqEquSign();
        switch( kw ) {
          case IO_ACCESS:
              CharItem( RT_SET_ACC );
              break;
          case IO_ACTION:
              CharItem( RT_SET_ACTION );
              break;
          case IO_BLANK:
              CharItem( RT_SET_BLNK );
              break;
          case IO_BLOCKSIZE:
              BlockSize();
              break;
          case IO_CARRIAGECONTROL:
              CharItem( RT_SET_CCTRL );
              break;
          case IO_DIRECT:
              CharItem( RT_SET_DIR );
              break;
          case IO_END:
              LabelItem( RT_SET_END );
              break;
          case IO_ERR:
              LabelItem( RT_SET_ERR );
              break;
          case IO_EXIST:
              BoolInq( RT_SET_EXST );
              break;
          case IO_FILE:
              FileItem();
              break;
          case IO_FMT:
              FormatIdd();
              break;
          case IO_FORM:
              CharItem( RT_SET_FORM );
              break;
          case IO_FMTTED:
              CharItem( RT_SET_FMTD );
              break;
          case IO_IOSTAT:
              IntInq( RT_SET_IOS );
              break;
          case IO_NAME:
              CharItem( RT_SET_NAME );
              break;
          case IO_NAMED:
              BoolInq( RT_SET_NMD );
              break;
          case IO_NXTREC:
              IntInq( RT_SET_NREC );
              break;
          case IO_NUMBER:
              IntInq( RT_SET_NUMB );
              break;
          case IO_OPENED:
              BoolInq( RT_SET_OPEN );
              break;
          case IO_REC:
              IntItem( RT_SET_REC );
              break;
          case IO_RECL:
              Recl();
              break;
          case IO_RECORDTYPE:
              CharItem( RT_SET_RECTYPE );
              break;
          case IO_SEQ:
              CharItem( RT_SET_SEQ );
              break;
          case IO_SHARE:
              CharItem( RT_SET_SHARE );
              break;
          case IO_STATUS:
              CharItem( RT_SET_STAT );
              break;
          case IO_UNFMTD:
              CharItem( RT_SET_UFMTD );
              break;
          case IO_UNIT:
              Unit();
              break;
        }
        AdvanceITPtr();
    } else {
        NextComma();
    }
}


static  void    NextComma() {
//===========================

    int         level;

    AdvanceITPtr();
    level = 0;
    for(;;) {
        if( RecOpenParen() ) {
            level++;
        } else if( RecCloseParen() ) {
            level--;
        }
        if( level < 0 ) break;
        if( RecEOS() ) break;
        if( RecComma() && ( level == 0 ) ) break;
        AdvanceITPtr();
    }
}


static  void    CharItem( uint routine ) {
//========================================

    CharSubExpr();
    if( !AError ) {
        if( StmtProc == PR_INQ ) {
            CkAssignOk();
        }
        GPassAddr( routine );
    }
}


static  void    FileItem() {
//==========================

    CharSubExpr();
    if( !AError ) {
        GPassAddr( RT_SET_FILE );
    }
}


static  void    LabelItem( uint routine ) {
//=========================================

    GPassStmtNo( LkUpStmtNo(), routine );
}


static  void    BoolInq( uint routine ) {
//=======================================

    sym_id      sym;

    BoolSubExpr();
    if( !AError ) {
        CkSize4();
        sym = CkAssignOk();
        if( sym != NULL ) {
            sym->ns.xflags |= SY_VOLATILE;
        }
        GPassAddr( routine );
    }
}


static  void    IntInq( uint routine ) {
//======================================

    sym_id      sym;

    IntSubExpr();
    if( !AError ) {
        CkSize4();
        sym = CkAssignOk();
        if( sym != NULL ) {
            sym->ns.xflags |= SY_VOLATILE;
        }
        GPassAddr( routine );
    }
}


void    CkSize4() {
//=================

    if( CITNode->size != 4 ) {
        Error( IL_BAD_SIZE );
    }
}


static  void    IntItem( uint routine ) {
//=======================================

    IntSubExpr();
    if( !AError ) {
        GPassValue( routine );
    }
}


static  void    BlockSize() {
//===========================

    if( StmtProc == PR_INQ ) {
        IntInq( RT_INQ_BLOCKSIZE );
    } else {
        IntItem( RT_SET_BLOCKSIZE );
    }
}


static  void    Recl() {
//======================

    if( StmtProc == PR_INQ ) {
        IntInq( RT_SET_LEN );
    } else {
        IntItem( RT_SET_RECL );
    }
}


void    FormatIdd() {
//===================

    cs_label    fmt_label;
    grp_entry   *ge;

    if( RecName() && ( NameListFind() != NULL ) ) {
        BIOutNameList( CITNode->sym_ptr );
        ge = CITNode->sym_ptr->nl.group_list;
        while( ge != NULL ) {
            ge->sym->ns.flags |= SY_REFERENCED;
            ge = ge->link;
        }
        GSetNameList( RT_SET_NML );
        KWRememb( IO_NAMELIST );
    } else if( RecNumber() ) {
        GPassStmtNo( LkUpFormat(), RT_SET_FMT );
    } else if( RecNOpn() && RecNextOpr( OPR_MUL ) ) {
        if( CITNode->link->opn.ds == DSOPN_PHI ) {
            AdvanceITPtr();   // nothing needs to be loaded for default
            KWRememb( IO_LIST_DIR );
        }
    } else if( RecNOpn() && RecNextOpr( OPR_COM ) ) {
        Extension( IL_NO_ASTERISK );
    } else if( RecIntVar() ) {
        CkVarRef();
        StNumbers.var_format = TRUE;
        GFmtVarSet();
    } else {
        ProcIOExpr();           // will allow for array name alone
        if( !AError ) {
            if( RecArrName() ) {
                if( CITNode->typ != TY_CHAR ) {
                    Extension( IL_NON_CHARACTER );
                }
                ChkAssumed();
                GFmtArrSet();
            } else if( CITNode->typ != TY_CHAR ) {
                Error( IL_BAD_FMT_SPEC );
            } else if( ( CITNode->opn.us == USOPN_CON ) ) {
                AddConst( CITNode ); // in case single constant
                fmt_label.g_label = NextLabel();
                FScan( CITNode->sym_ptr->lt.length,
                       &CITNode->sym_ptr->lt.value, fmt_label );
                GPassLabel( fmt_label.g_label, RT_SET_FMT );
            } else {
                GFmtExprSet();
            }
        }
    }
}


void    Unit( ) {
//===============

    if( RecNOpn() && RecNextOpr( OPR_MUL ) ) {
        if( ( StmtProc != PR_READ ) && ( StmtProc != PR_WRITE ) ) {
            StmtErr( IL_STAR_NOT_ALLOWED );
        }
        AdvanceITPtr();
        ReqNOpn();
    } else {
        ProcIOExpr();
        if( !AError ) {
            if( _IsTypeInteger( CITNode->typ ) && !RecArrName() ) {
                GPassValue( RT_SET_UNIT );
            } else if( CITNode->typ == TY_CHAR ) {
                KWRememb( IO_INTERNAL );
                CkAssignOk();
                if( RecArrName() ) {
                    ChkAssumed();
                    GArrIntlSet();
                } else {
                    GIntlSet();
                }
                if( ( StmtProc != PR_READ ) &&
                    ( StmtProc != PR_WRITE ) &&
                    ( StmtProc != PR_PRINT ) ) {
                    Error( IL_BAD_INTL );
                }
            } else {
                Error( IL_NO_UNIT_ID );
            }
        }
    }
}


void    ChkAssumed() {
//====================

// Check for illegal use of assumed size array.

    sym_id      sym;

    if( CITNode->opn.us & USOPN_FLD ) return;
    sym = CITNode->sym_ptr;
    if( sym->ns.si.va.dim_ext->dim_flags & DIM_ASSUMED ) {
        NameErr( SV_CANT_USE_ASSUMED, sym );
    }
    sym->ns.si.va.dim_ext->dim_flags |= DIM_USED_IN_IO;
}
