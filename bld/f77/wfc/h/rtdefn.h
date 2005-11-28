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
* Description:  define data constants for run-time support routines
*
****************************************************************************/


//      id                rt_name         ptr   proc          typ
pick( RT_PAUSE,           "Pause",        NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_STOP,            "Stop",         NULL, &RtStopInfo, TY_NO_TYPE      )
pick( RT_SET_FMT,         "SetFmt",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_UNIT,        "SetUnit",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_INTL,        "SetIntl",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_ERR,         "SetErr",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_END,         "SetEnd",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_REC,         "SetRec",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_IOS,         "SetIOS",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_ACC,         "SetAcc",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_BLNK,        "SetBlnk",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_FILE,        "SetFile",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_FORM,        "SetForm",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_LEN,         "SetLen",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_STAT,        "SetStat",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_DIR,         "SetDir",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_CCTRL,       "SetCCtrl",     NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_FMTD,        "SetFmtd",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NAME,        "SetName",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_SEQ,         "SetSeq",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_UFMTD,       "SetUnFmtd",    NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_EXST,        "SetExst",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NMD,         "SetNmd",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NREC,        "SetNRec",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NUMB,        "SetNumb",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_OPEN,        "SetOpen",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_RECL,        "SetRecl",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NOFMT,       "SetNoFmt",     NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_RECTYPE,     "SetRecType",   NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_ACTION,      "SetAction",    NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_NML,         "SetNml",       NULL, &RtVarInfo,  TY_NO_TYPE      )
pick( RT_SET_BLOCKSIZE,   "SetBlkSize",   NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_INQ_BLOCKSIZE,   "InqBlkSize",   NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_SHARE,       "SetShare",     NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_EX_READ,         "IORead",       NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_WRITE,        "IOWrite",      NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_OPEN,         "IOOpen",       NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_CLOSE,        "IOClose",      NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_BACK,         "IOBack",       NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_ENDF,         "IOEndf",       NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_REW,          "IORew",        NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_EX_INQ,          "IOInq",        NULL, &CoRtnInfo,  TY_INTEGER_TARG )
pick( RT_OUT_LOG1,        "OutLOG1",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_LOG4,        "OutLOG4",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_INT1,        "OutINT1",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_INT2,        "OutINT2",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_INT4,        "OutINT4",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_REAL,        "OutREAL",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_DBLE,        "OutDBLE",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_XTND,        "OutXTND",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_CPLX,        "OutCPLX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_DBCX,        "OutDBCX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_XTCX,        "OutXTCX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_CHAR,        "OutCHAR",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_OUT_STR,         "OutSTR",       NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_LOG1,        "InpLOG1",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_LOG4,        "InpLOG4",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_INT1,        "InpINT1",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_INT2,        "InpINT2",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_INT4,        "InpINT4",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_REAL,        "InpREAL",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_DBLE,        "InpDBLE",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_XTND,        "InpXTND",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_CPLX,        "InpCPLX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_DBCX,        "InpDBCX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_XTCX,        "InpXTCX",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_CHAR,        "InpCHAR",      NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_STR,         "InpSTR",       NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_ENDIO,           "EndIO",        NULL, &CoRtnInfo,  TY_NO_TYPE      )
pick( RT_ALLOCATE,        "Alloc",        NULL, &RtVarInfo,  TY_NO_TYPE      )
pick( RT_DEALLOCATE,      "DeAlloc",      NULL, &RtVarInfo,  TY_NO_TYPE      )
pick( RT_FMT_SCAN,        "FmtScan",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_FMT_ARR_SCAN,    "FmtAScan",     NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_CAT,             "Cat",          NULL, &RtVarInfo,  TY_NO_TYPE      )
pick( RT_SUBSTRING,       "Substring",    NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SUBSCRIPT,       "Subscript",    NULL, &RtVarInfo,  TY_INTEGER      )
pick( RT_SET_LINE,        "SetLine",      NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_SET_MODULE,      "SetModule",    NULL, &RtRtnInfo,  TY_NO_TYPE      )

// Define run-time support routines for complex arithmetic:

pick( RT_C8MUL,           "C8Mul",        NULL, &IFInfo,     TY_COMPLEX      )
pick( RT_C8DIV,           "C8Div",        NULL, &IFInfo,     TY_COMPLEX      )
pick( RT_C8POW,           "C8Pow",        NULL, &IFInfo,     TY_COMPLEX      )
pick( RT_C8POWI,          "C8PowI",       NULL, &IFInfo,     TY_COMPLEX      )
pick( RT_C16MUL,          "C16Mul",       NULL, &IFInfo,     TY_DCOMPLEX     )
pick( RT_C16DIV,          "C16Div",       NULL, &IFInfo,     TY_DCOMPLEX     )
pick( RT_C16POW,          "C16Pow",       NULL, &IFInfo,     TY_DCOMPLEX     )
pick( RT_C16POWI,         "C16PowI",      NULL, &IFInfo,     TY_DCOMPLEX     )
pick( RT_C32MUL,          "C32Mul",       NULL, &IFInfo,     TY_XCOMPLEX     )
pick( RT_C32DIV,          "C32Div",       NULL, &IFInfo,     TY_XCOMPLEX     )
pick( RT_C32POW,          "C32Pow",       NULL, &IFInfo,     TY_XCOMPLEX     )
pick( RT_C32POWI,         "C32PowI",      NULL, &IFInfo,     TY_XCOMPLEX     )

// Define run-time support routine for character comparision:

pick( RT_LEXCMP,          "LexCmp",       NULL, &RtRtnInfo,  TY_INTEGER_TARG )

// Define array i/o run-time routines:

pick( RT_PRT_ARRAY,       "PrtArr",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_PRT_CHAR_ARRAY,  "PrtChArr",     NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_ARRAY,       "InpArr",       NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_INP_CHAR_ARRAY,  "InpChArr",     NULL, &RtRtnInfo,  TY_NO_TYPE      )

// Define run-time character move (a = b):

pick( RT_MOVE,            "Move",         NULL, &RtRtnInfo,  TY_NO_TYPE      )

// Define run-time concatenation into temporary:

pick( RT_TCAT,            "TCat",         NULL, &RtVarInfo,  TY_NO_TYPE      )

// ADV run-time checking routines:

pick( RT_ADV_FILL_HI,     "ADVFillHi",    NULL, &RtRtnInfo,  TY_NO_TYPE      )
pick( RT_ADV_FILL_HI_LO1, "ADVFillHiLo1", NULL, &RtRtnInfo,  TY_NO_TYPE      )
