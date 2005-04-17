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
// FCRTNS   : F-Code processing runtime routines
//

#include "ftnstd.h"
#include "global.h"
#include "cgdefs.h"
#include "cg.h"
#include "wf77aux.h"
#include "fcgbls.h"
#include "cgflags.h"
#include "rtentry.h"
#include "cpopt.h"

#include <string.h>

//=================== Back End Code Generation Routines ====================

extern  cg_name         CGFEName(sym_handle,cg_type);
extern  call_handle     CGInitCall(cg_name,cg_type,sym_handle);
extern  void            BEFreeBack(back_handle);

//=========================================================================

extern  cg_type         F772CGType(sym_id sym);
extern  sym_id          STAdd(char *name,int length);
extern  sym_id          STFree(sym_id sym_ptr);
extern  int             TypeSize(uint typ);
extern  segment_id      AllocImpSegId(void);

extern  aux_info        RtRtnInfo;
extern  aux_info        IFInfo;
extern  aux_info        RtVarInfo;
extern  aux_info        RtStopInfo;
extern  aux_info        CoRtnInfo;
extern  unsigned_8      CGFlags;

rt_rtn  __FAR RtnTab[] = {
        "Pause",        NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "Stop",         NULL,   &RtStopInfo,    TY_NO_TYPE,
        NULL,           NULL,   NULL,           0,      // "SetIOCB"
        "SetFmt",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetUnit",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetIntl",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetErr",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetEnd",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetRec",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetIOS",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetAcc",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetBlnk",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetFile",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetForm",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetLen",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetStat",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetDir",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetCCtrl",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetFmtd",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetName",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetSeq",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetUnFmtd",    NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetExst",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetNmd",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetNRec",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetNumb",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetOpen",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetRecl",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetNoFmt",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetRecType",   NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetAction",    NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetNml",       NULL,   &RtVarInfo,     TY_NO_TYPE,
        "SetBlkSize",   NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "InqBlkSize",   NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetShare",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "IORead",       NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOWrite",      NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOOpen",       NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOClose",      NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOBack",       NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOEndf",       NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IORew",        NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "IOInq",        NULL,   &CoRtnInfo,     TY_INTEGER_TARG,
        "OutLOG1",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutLOG4",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutINT1",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutINT2",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutINT4",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutREAL",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutDBLE",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutXTND",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutCPLX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutDBCX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutXTCX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutCHAR",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "OutSTR",       NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpLOG1",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpLOG4",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpINT1",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpINT2",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpINT4",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpREAL",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpDBLE",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpXTND",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpCPLX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpDBCX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpXTCX",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpCHAR",      NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "InpSTR",       NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "EndIO",        NULL,   &CoRtnInfo,     TY_NO_TYPE,
        "Alloc",        NULL,   &RtVarInfo,     TY_NO_TYPE,
        "DeAlloc",      NULL,   &RtVarInfo,     TY_NO_TYPE,
        "FmtScan",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "FmtAScan",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "Cat",          NULL,   &RtVarInfo,     TY_NO_TYPE,
        "Substring",    NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "Subscript",    NULL,   &RtVarInfo,     TY_INTEGER,
        NULL,           NULL,   NULL,           0,      // "Prologue"
        NULL,           NULL,   NULL,           0,      // "Epilogue"
        NULL,           NULL,   NULL,           0,      // "FmtAssign"
        "SetLine",      NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "SetModule",    NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "C8Mul",        NULL,   &IFInfo,        TY_COMPLEX,
        "C8Div",        NULL,   &IFInfo,        TY_COMPLEX,
        "C8Pow",        NULL,   &IFInfo,        TY_COMPLEX,
        "C8PowI",       NULL,   &IFInfo,        TY_COMPLEX,
        "C16Mul",       NULL,   &IFInfo,        TY_DCOMPLEX,
        "C16Div",       NULL,   &IFInfo,        TY_DCOMPLEX,
        "C16Pow",       NULL,   &IFInfo,        TY_DCOMPLEX,
        "C16PowI",      NULL,   &IFInfo,        TY_DCOMPLEX,
        "C32Mul",       NULL,   &IFInfo,        TY_XCOMPLEX,
        "C32Div",       NULL,   &IFInfo,        TY_XCOMPLEX,
        "C32Pow",       NULL,   &IFInfo,        TY_XCOMPLEX,
        "C32PowI",      NULL,   &IFInfo,        TY_XCOMPLEX,
        "LexCmp",       NULL,   &RtRtnInfo,     TY_INTEGER_TARG,
        "PrtArr",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "PrtChArr",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "InpArr",       NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "InpChArr",     NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "Move",         NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "TCat",         NULL,   &RtVarInfo,     TY_NO_TYPE,
        "ADVFillHi",    NULL,   &RtRtnInfo,     TY_NO_TYPE,
        "ADVFillHiLo1", NULL,   &RtRtnInfo,     TY_NO_TYPE
};

#define MAX_RT_INDEX    ((sizeof( RtnTab ) / sizeof( RtnTab[0] ))-1)


call_handle     InitCall( int rtn_id ) {
//======================================

// Initialize a call to a runtime routine.

    sym_id      sym;
    rt_rtn      __FAR *rt_entry;
    byte        typ;
    int         name_len;
    char        __FAR *ptr;

    rt_entry = &RtnTab[ rtn_id ];
    sym = rt_entry->sym_ptr;
    if( sym == NULL ) {
        name_len = 0;
        ptr = rt_entry->name;
        while( *ptr != NULLCHAR ) {
            SymBuff[ name_len ] = *ptr;
            ++name_len;
            ++ptr;
        }
        sym = STAdd( SymBuff, name_len );
        sym->ns.flags = SY_USAGE | SY_TYPE | SY_SUBPROGRAM | SY_FUNCTION |
                        SY_RT_ROUTINE;
        if( rt_entry->typ == TY_NO_TYPE ) {
            sym->ns.typ = TY_INTEGER_TARG;
        } else {
            sym->ns.typ = rt_entry->typ;
        }
        sym->ns.xt.size = TypeSize( sym->ns.typ );
        sym->ns.address = NULL;
        sym->ns.si.sp.imp_segid = AllocImpSegId();
        rt_entry->sym_ptr = sym;
    }
    typ = F772CGType( sym );
    return( CGInitCall( CGFEName( sym, typ ), typ, rt_entry->aux ) );
}


void    InitRtRtns() {
//====================

// Initialize symbol table entries for run-time routines.

    int         rt_index;

    rt_index = 0;
    while( rt_index <= MAX_RT_INDEX ) {
        RtnTab[ rt_index ].sym_ptr = NULL;
        rt_index++;
    }
}


void    FreeRtRtns() {
//====================

// Free symbol table entries for run-time routines.

    int         rt_index;
    sym_id      sym;

    rt_index = 0;
    while( rt_index <= MAX_RT_INDEX ) {
        sym = RtnTab[ rt_index ].sym_ptr;
        if( sym != NULL ) {
            if( ( CGFlags & CG_FATAL ) == 0 ) {
                if( sym->ns.address != NULL ) {
                    BEFreeBack( sym->ns.address );
                }
            }
            STFree( sym );
            RtnTab[ rt_index ].sym_ptr = NULL;
        }
        rt_index++;
    }
}
