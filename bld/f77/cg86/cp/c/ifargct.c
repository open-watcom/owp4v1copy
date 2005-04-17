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
// IFARGCT      : intrinsic function argument count table
//

#include "ftnstd.h"
#include "ifargs.h"

const byte __FAR        IFArgCt[] = {
        ONE_ARG         | IF_IN_LINE,                   // ABS
        ONE_ARG         | IF_IN_LINE,                   // ACOS
        ONE_ARG         | IF_IN_LINE,                   // AIMAG
        ONE_ARG,                                        // AINT
        ONE_ARG                         | IF_EXTENSION, // ALGAMA
        ONE_ARG                         | IF_EXTENSION, // ALLOCATED
        ONE_ARG         | IF_IN_LINE,                   // ALOG
        ONE_ARG         | IF_IN_LINE,                   // ALOG10
        TWO_OR_MORE     | IF_IN_LINE,                   // AMAX0
        TWO_OR_MORE     | IF_IN_LINE,                   // AMAX1
        TWO_OR_MORE     | IF_IN_LINE,                   // AMIN0
        TWO_OR_MORE     | IF_IN_LINE,                   // AMIN1
        TWO_ARG         | IF_IN_LINE,                   // AMOD
        ONE_ARG,                                        // ANINT
        ONE_ARG         | IF_IN_LINE,                   // ASIN
        ONE_ARG         | IF_IN_LINE,                   // ATAN
        TWO_ARG         | IF_IN_LINE,                   // ATAN2
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // BTEST
        ONE_ARG,                                        // CABS
        ONE_ARG,                                        // CCOS
        ONE_ARG                         | IF_EXTENSION, // CDABS
        ONE_ARG                         | IF_EXTENSION, // CDCOS
        ONE_ARG                         | IF_EXTENSION, // CDEXP
        ONE_ARG                         | IF_EXTENSION, // CDLOG
        ONE_ARG                         | IF_EXTENSION, // CDSIN
        ONE_ARG                         | IF_EXTENSION, // CDSQRT
        ONE_ARG,                                        // CEXP
        ONE_ARG,                                        // CHAR
        ONE_ARG,                                        // CLOG
        ONE_OR_TWO,                                     // CMPLX
        ONE_ARG         | IF_IN_LINE,                   // CONJG
        ONE_ARG         | IF_IN_LINE,                   // COS
        ONE_ARG         | IF_IN_LINE,                   // COSH
        ONE_ARG                         | IF_EXTENSION, // COTAN
        ONE_ARG                         | IF_EXTENSION, // CQABS
        ONE_ARG                         | IF_EXTENSION, // CQCOS
        ONE_ARG                         | IF_EXTENSION, // CQEXP
        ONE_ARG                         | IF_EXTENSION, // CQLOG
        ONE_ARG                         | IF_EXTENSION, // CQSIN
        ONE_ARG                         | IF_EXTENSION, // CQSQRT
        ONE_ARG,                                        // CSIN
        ONE_ARG,                                        // CSQRT
        ONE_ARG         | IF_IN_LINE,                   // DABS
        ONE_ARG         | IF_IN_LINE,                   // DACOS
        ONE_ARG         | IF_IN_LINE,                   // DASIN
        ONE_ARG         | IF_IN_LINE,                   // DATAN
        TWO_ARG         | IF_IN_LINE,                   // DATAN2
        ONE_ARG,                                        // DBLE
        ONE_OR_TWO                      | IF_EXTENSION, // DCMPLX
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // DCONJG
        ONE_ARG         | IF_IN_LINE,                   // DCOS
        ONE_ARG         | IF_IN_LINE,                   // DCOSH
        ONE_ARG                         | IF_EXTENSION, // DCOTAN
        TWO_ARG,                                        // DDIM
        ONE_ARG                         | IF_EXTENSION, // DERF
        ONE_ARG                         | IF_EXTENSION, // DERFC
        ONE_ARG         | IF_IN_LINE,                   // DEXP
        ONE_ARG                         | IF_EXTENSION, // DFLOAT
        ONE_ARG                         | IF_EXTENSION, // DGAMMA
        TWO_ARG,                                        // DIM
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // DIMAG
        ONE_ARG,                                        // DINT
        ONE_ARG                         | IF_EXTENSION, // DLGAMA
        ONE_ARG         | IF_IN_LINE,                   // DLOG
        ONE_ARG         | IF_IN_LINE,                   // DLOG10
        TWO_OR_MORE     | IF_IN_LINE,                   // DMAX1
        TWO_OR_MORE     | IF_IN_LINE,                   // DMIN1
        TWO_ARG         | IF_IN_LINE,                   // DMOD
        ONE_ARG,                                        // DNINT
        TWO_ARG         | IF_IN_LINE,                   // DPROD
        ONE_ARG                         | IF_EXTENSION, // DREAL
        TWO_ARG         | IF_IN_LINE,                   // DSIGN
        ONE_ARG         | IF_IN_LINE,                   // DSIN
        ONE_ARG         | IF_IN_LINE,                   // DSINH
        ONE_ARG         | IF_IN_LINE,                   // DSQRT
        ONE_ARG         | IF_IN_LINE,                   // DTAN
        ONE_ARG         | IF_IN_LINE,                   // DTANH
        ONE_ARG                         | IF_EXTENSION, // ERF
        ONE_ARG                         | IF_EXTENSION, // ERFC
        ONE_ARG         | IF_IN_LINE,                   // EXP
        ONE_ARG,                                        // FLOAT
        ONE_ARG                         | IF_EXTENSION, // GAMMA
        ONE_ARG                         | IF_EXTENSION, // HFIX
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1ABS
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1AND
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1BCHNG
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1BCLR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1BSET
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1BTEST
        TWO_ARG                         | IF_EXTENSION, // I1DIM
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1EOR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1LSHIFT
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // I1MAX0
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // I1MIN0
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1MOD
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1NOT
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1OR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1RSHIFT
        TWO_ARG                         | IF_EXTENSION, // I1SHA
        TWO_ARG                         | IF_EXTENSION, // I1SHC
        TWO_ARG                         | IF_EXTENSION, // I1SHFT
        TWO_ARG                         | IF_EXTENSION, // I1SHL
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I1SIGN
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2ABS
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2AND
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2BCHNG
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2BCLR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2BSET
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2BTEST
        TWO_ARG                         | IF_EXTENSION, // I2DIM
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2EOR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2LSHIFT
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // I2MAX0
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // I2MIN0
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2MOD
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2NOT
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2OR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2RSHIFT
        TWO_ARG                         | IF_EXTENSION, // I2SHA
        TWO_ARG                         | IF_EXTENSION, // I2SHC
        TWO_ARG                         | IF_EXTENSION, // I2SHFT
        TWO_ARG                         | IF_EXTENSION, // I2SHL
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // I2SIGN
        ONE_ARG         | IF_IN_LINE,                   // IABS
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IAND
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IBCHNG
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IBCLR
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IBSET
        ONE_ARG         | IF_IN_LINE,                   // ICHAR
        TWO_ARG,                                        // IDIM
        ONE_ARG,                                        // IDINT
        ONE_ARG,                                        // IDNINT
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IEOR
        ONE_ARG,                                        // IFIX
        ONE_ARG,                                        // IMAG
        TWO_ARG,                                        // INDEX
        ONE_ARG,                                        // INT
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // IOR
        ONE_ARG                         | IF_EXTENSION, // IQINT
        ONE_ARG                         | IF_EXTENSION, // IQNINT
        TWO_ARG                         | IF_EXTENSION, // ISHA
        TWO_ARG                         | IF_EXTENSION, // ISHC
        TWO_ARG                         | IF_EXTENSION, // ISHFT
        TWO_ARG                         | IF_EXTENSION, // ISHL
        TWO_ARG         | IF_IN_LINE,                   // ISIGN
        ONE_ARG                         | IF_EXTENSION, // ISIZEOF
        ONE_ARG         | IF_IN_LINE,                   // LEN
        ONE_ARG                         | IF_EXTENSION, // LENTRIM
        ONE_ARG                         | IF_EXTENSION, // LGAMMA
        TWO_ARG,                                        // LGE
        TWO_ARG,                                        // LGT
        TWO_ARG,                                        // LLE
        TWO_ARG,                                        // LLT
        ONE_ARG                         | IF_EXTENSION, // LOC
        ONE_ARG,                                        // LOG
        ONE_ARG,                                        // LOG10
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // LSHIFT
        TWO_OR_MORE,                                    // MAX
        TWO_OR_MORE     | IF_IN_LINE,                   // MAX0
        TWO_OR_MORE     | IF_IN_LINE,                   // MAX1
        TWO_OR_MORE,                                    // MIN
        TWO_OR_MORE     | IF_IN_LINE,                   // MIN0
        TWO_OR_MORE     | IF_IN_LINE,                   // MIN1
        TWO_ARG         | IF_IN_LINE,                   // MOD
        ONE_ARG,                                        // NINT
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // NOT
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QABS
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QACOS
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QASIN
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QATAN
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // QATAN2
        ONE_OR_TWO                      | IF_EXTENSION, // QCMPLX
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QCONJG
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QCOS
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QCOSH
        ONE_ARG                         | IF_EXTENSION, // QCOTAN
        TWO_ARG                         | IF_EXTENSION, // QDIM
        ONE_ARG                         | IF_EXTENSION, // QERF
        ONE_ARG                         | IF_EXTENSION, // QERFC
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QEXP
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QEXT
        ONE_ARG                         | IF_EXTENSION, // QGAMMA
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QIMAG
        ONE_ARG                         | IF_EXTENSION, // QINT
        ONE_ARG                         | IF_EXTENSION, // QLGAMA
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QLOG
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QLOG10
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // QMAX1
        TWO_OR_MORE     | IF_IN_LINE    | IF_EXTENSION, // QMIN1
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // QMOD
        ONE_ARG                         | IF_EXTENSION, // QNINT
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // QPROD
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // QSIGN
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QSIN
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QSINH
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QSQRT
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QTAN
        ONE_ARG         | IF_IN_LINE    | IF_EXTENSION, // QTANH
        ONE_ARG,                                        // REAL
        TWO_ARG         | IF_IN_LINE    | IF_EXTENSION, // RSHIFT
        TWO_ARG         | IF_IN_LINE,                   // SIGN
        ONE_ARG         | IF_IN_LINE,                   // SIN
        ONE_ARG         | IF_IN_LINE,                   // SINH
        ONE_ARG,                                        // SNGL
        ONE_ARG         | IF_IN_LINE,                   // SQRT
        ONE_ARG         | IF_IN_LINE,                   // TAN
        ONE_ARG         | IF_IN_LINE,                   // TANH
        ONE_ARG         | IF_IN_LINE,                   // VOLATILE
};
