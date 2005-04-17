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
// IFSELECT  : Table for generic function selection.
//             Each entry indicates which function we should look at
//             after having looked at the function indicated by comment.
//

#include "ftnstd.h"
#include "ifnames.h"

const byte __FAR        IFSelect[] = {
    IF_IABS,            // ABS
    IF_DACOS,           // ACOS
    IF_DIMAG,           // AIMAG
    IF_DINT,            // AINT
    IF_NO_MORE,         // ALGAMA
    MAGIC,              // ALLOCATED
    IF_DLOG,            // ALOG
    IF_DLOG10,          // ALOG10
    IF_NO_MORE,         // AMAX0
    IF_DMAX1,           // AMAX1
    IF_NO_MORE,         // AMIN0
    IF_DMIN1,           // AMIN1
    IF_DMOD,            // AMOD
    IF_DNINT,           // ANINT
    IF_DASIN,           // ASIN
    IF_DATAN,           // ATAN
    IF_DATAN2,          // ATAN2
    IF_I2BTEST,         // BTEST
    IF_CDABS,           // CABS
    IF_CDCOS,           // CCOS
    IF_CQABS,           // CDABS
    IF_CQCOS,           // CDCOS
    IF_CQEXP,           // CDEXP
    IF_CQLOG,           // CDLOG
    IF_CQSIN,           // CDSIN
    IF_CQSQRT,          // CDSQRT
    IF_CDEXP,           // CEXP
    MAGIC,              // CHAR
    IF_CDLOG,           // CLOG
    MAGIC,              // CMPLX
    IF_DCONJG,          // CONJG
    IF_DCOS,            // COS
    IF_DCOSH,           // COSH
    IF_DCOTAN,          // COTAN
    IF_NO_MORE,         // CQABS
    IF_NO_MORE,         // CQCOS
    IF_NO_MORE,         // CQEXP
    IF_NO_MORE,         // CQLOG
    IF_NO_MORE,         // CQSIN
    IF_NO_MORE,         // CQSQRT
    IF_CDSIN,           // CSIN
    IF_CDSQRT,          // CSQRT
    IF_QABS,            // DABS
    IF_QACOS,           // DACOS
    IF_QASIN,           // DASIN
    IF_QATAN,           // DATAN
    IF_QATAN2,          // DATAN2
    MAGIC,              // DBLE
    MAGIC,              // DCMPLX
    IF_QCONJG,          // DCONJG
    IF_QCOS,            // DCOS
    IF_QCOSH,           // DCOSH
    IF_QCOTAN,          // DCOTAN
    IF_QDIM,            // DDIM
    IF_QERF,            // DERF
    IF_QERFC,           // DERFC
    IF_QEXP,            // DEXP
    MAGIC,              // DFLOAT
    IF_QGAMMA,          // DGAMMA
    IF_IDIM,            // DIM
    IF_QIMAG,           // DIMAG
    IF_QINT,            // DINT
    IF_QLGAMA,          // DLGAMA
    IF_QLOG,            // DLOG
    IF_QLOG10,          // DLOG10
    IF_QMAX1,           // DMAX1
    IF_QMIN1,           // DMIN1
    IF_QMOD,            // DMOD
    IF_QNINT,           // DNINT
    IF_QPROD,           // DPROD
    MAGIC,              // DREAL
    IF_QSIGN,           // DSIGN
    IF_QSIN,            // DSIN
    IF_QSINH,           // DSINH
    IF_QSQRT,           // DSQRT
    IF_QTAN,            // DTAN
    IF_QTANH,           // DTANH
    IF_DERF,            // ERF
    IF_DERFC,           // ERFC
    IF_DEXP,            // EXP
    MAGIC,              // FLOAT
    IF_DGAMMA,          // GAMMA
    MAGIC,              // HFIX
    IF_DABS,            // I1ABS
    IF_NO_MORE,         // I1AND
    IF_NO_MORE,         // I1BCHNG
    IF_NO_MORE,         // I1BCLR
    IF_NO_MORE,         // I1BSET
    IF_NO_MORE,         // I1BTEST
    IF_DDIM,            // I1DIM
    IF_NO_MORE,         // I1EOR
    IF_NO_MORE,         // I1LSHIFT
    IF_AMAX1,           // I1MAX0
    IF_AMIN1,           // I1MIN0
    IF_AMOD,            // I1MOD
    IF_NO_MORE,         // I1NOT
    IF_NO_MORE,         // I1OR
    IF_NO_MORE,         // IRLSHIFT
    IF_NO_MORE,         // I1SHA
    IF_NO_MORE,         // I1SHC
    IF_NO_MORE,         // I1SHFT
    IF_NO_MORE,         // I1SHL
    IF_NO_MORE,         // I1SIGN
    IF_I1ABS,           // I2ABS
    IF_I1AND,           // I2AND
    IF_I1BCHNG,         // I2BCHNG
    IF_I1BCLR,          // I2BCLR
    IF_I1BSET,          // I2BSET
    IF_I1BTEST,         // I2BTEST
    IF_I1DIM,           // I2DIM
    IF_I1EOR,           // I2EOR
    IF_I1LSHIFT,        // I2LSHIFT
    IF_I1MAX0,          // I2MAX0
    IF_I1MIN0,          // I2MIN0
    IF_I1MOD,           // I2MOD
    IF_I1NOT,           // I2NOT
    IF_I1OR,            // I2OR
    IF_I1RSHIFT,        // I2RSHIFT
    IF_I1SHA,           // I2SHA
    IF_I1SHC,           // I2SHC
    IF_I1SHFT,          // I2SHFT
    IF_I1SHL,           // I2SHL
    IF_I1SIGN,          // I2SIGN
    IF_I2ABS,           // IABS
    IF_I2AND,           // IAND
    IF_I2BCHNG,         // IBCHNG
    IF_I2BCLR,          // IBCLR
    IF_I2BSET,          // IBSET
    IF_NO_MORE,         // ICHAR
    IF_I2DIM,           // IDIM
    MAGIC,              // IDINT
    IF_IQNINT,          // IDNINT
    IF_I2EOR,           // IEOR
    MAGIC,              // IFIX
    IF_AIMAG,           // IMAG
    IF_NO_MORE,         // INDEX
    MAGIC,              // INT
    IF_I2OR,            // IOR
    MAGIC,              // IQINT
    IF_NO_MORE,         // IQNINT
    IF_I2SHA,           // ISHA
    IF_I2SHC,           // ISHC
    IF_I2SHFT,          // ISHFT
    IF_I2SHL,           // ISHL
    IF_DSIGN,           // ISIGN
    MAGIC,              // ISIZEOF
    IF_NO_MORE,         // LEN
    IF_NO_MORE,         // LENTRIM
    IF_DLGAMA,          // LGAMMA
    IF_NO_MORE,         // LGE
    IF_NO_MORE,         // LGT
    IF_NO_MORE,         // LLE
    IF_NO_MORE,         // LLT
    MAGIC,              // LOC
    IF_ALOG,            // LOG
    IF_ALOG10,          // LOG10
    IF_I2LSHIFT,        // LSHIFT
    IF_MAX0,            // MAX
    IF_I2MAX0,          // MAX0
    IF_NO_MORE,         // MAX1
    IF_MIN0,            // MIN
    IF_I2MIN0,          // MIN0
    IF_NO_MORE,         // MIN1
    IF_I2MOD,           // MOD
    IF_IDNINT,          // NINT
    IF_I2NOT,           // NOT
    IF_CABS,            // QABS
    IF_NO_MORE,         // QACOS
    IF_NO_MORE,         // QASIN
    IF_NO_MORE,         // QATAN
    IF_NO_MORE,         // QATAN2
    MAGIC,              // QCMPLX
    IF_NO_MORE,         // QCONJG
    IF_CCOS,            // QCOS
    IF_NO_MORE,         // QCOSH
    IF_NO_MORE,         // QCOTAN
    IF_NO_MORE,         // QDIM
    IF_NO_MORE,         // QERF
    IF_NO_MORE,         // QERFC
    IF_CEXP,            // QEXP
    MAGIC,              // QEXT
    IF_NO_MORE,         // QGAMMA
    IF_NO_MORE,         // QIMAG
    IF_NO_MORE,         // QINT
    IF_ALGAMA,          // QLGAMA
    IF_CLOG,            // QLOG
    IF_NO_MORE,         // QLOG10
    IF_NO_MORE,         // QMAX1
    IF_NO_MORE,         // QMIN1
    IF_NO_MORE,         // QMOD
    IF_NO_MORE,         // QNINT
    IF_NO_MORE,         // QPROD
    IF_I2SIGN,          // QSIGN
    IF_CSIN,            // QSIN
    IF_NO_MORE,         // QSINH
    IF_CSQRT,           // QSQRT
    IF_NO_MORE,         // QTAN
    IF_NO_MORE,         // QTANH
    MAGIC,              // REAL
    IF_I2RSHIFT,        // RSHIFT
    IF_ISIGN,           // SIGN
    IF_DSIN,            // SIN
    IF_DSINH,           // SINH
    MAGIC,              // SNGL
    IF_DSQRT,           // SQRT
    IF_DTAN,            // TAN
    IF_DTANH,           // TANH
    MAGIC               // VOLATILE
};
