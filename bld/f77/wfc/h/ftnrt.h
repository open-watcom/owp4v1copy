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


enum {
    RT_PAUSE      = RT_FIRST_FORTRAN,
    RT_STOP,
    RT_SET_IOCB,
    RT_SET_FMT,
    RT_SET_UNIT,
    RT_SET_INTL,
    RT_SET_ERR,
    RT_SET_END,
    RT_SET_REC,
    RT_SET_IOS,
    RT_SET_ACC,
    RT_SET_BLNK,
    RT_SET_FILE,
    RT_SET_FORM,
    RT_SET_LEN,
    RT_SET_STAT,
    RT_SET_DIR,
    RT_SET_CCTRL,
    RT_SET_FMTD,
    RT_SET_NAME,
    RT_SET_SEQ,
    RT_SET_UFMTD,
    RT_SET_EXST,
    RT_SET_NMD,
    RT_SET_NREC,
    RT_SET_NUMB,
    RT_SET_OPEN,
    RT_SET_RECL,
    RT_SET_NOFMT,
    RT_SET_RECTYPE,
    RT_SET_ACTION,
    RT_SET_NML,
    RT_SET_BLOCKSIZE,
    RT_INQ_BLOCKSIZE,
    RT_SET_SHARE,
    RT_IO_STMTS,
    RT_EX_READ = RT_IO_STMTS,
    RT_EX_WRITE,
    RT_EX_OPEN,
    RT_EX_CLOSE,
    RT_EX_BACK,
    RT_EX_ENDF,
    RT_EX_REW,
    RT_EX_INQ,
    RT_OUT_LOG1,
    RT_OUT_LOG4,
    RT_OUT_INT1,
    RT_OUT_INT2,
    RT_OUT_INT4,
    RT_OUT_REAL,
    RT_OUT_DBLE,
    RT_OUT_XTND,
    RT_OUT_CPLX,
    RT_OUT_DBCX,
    RT_OUT_XTCX,
    RT_OUT_CHAR,
    RT_OUT_STR,
    RT_INP_LOG1,
    RT_INP_LOG4,
    RT_INP_INT1,
    RT_INP_INT2,
    RT_INP_INT4,
    RT_INP_REAL,
    RT_INP_DBLE,
    RT_INP_XTND,
    RT_INP_CPLX,
    RT_INP_DBCX,
    RT_INP_XTCX,
    RT_INP_CHAR,
    RT_INP_STR,
    RT_ENDIO,
    RT_ALLOCATE,
    RT_DEALLOCATE,
    RT_FMT_SCAN,
    RT_FMT_ARR_SCAN,
    RT_CAT,
    RT_SUBSTRING,
    RT_SUBSCRIPT,
    RT_PROLOGUE,
    RT_EPILOGUE,
    RT_FMT_ASSIGN,
    RT_SET_LINE,
    RT_SET_MODULE,
    RT_RTN_COUNT,
    RT_LAST_FORTRAN       = RT_RTN_COUNT - 1
};
