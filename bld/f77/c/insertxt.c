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
// INSERTXT  : text inserted into compile- and run-time error messages
//

#include "ftnstd.h"

extern  char    MsExpression[];
extern  char    MsSimpVar[];
extern  char    MsArrElt[];
extern  char    MsSubStrAElt[];
extern  char    MsArrName[];
extern  char    MsSpName[];
extern  char    MsAltRet[];
extern  char    MsGeneric[];

char    *PrmCodTab[] = {
    "expression",
    "simple variable",
    "array element",
    "substrung array element",
    "array name",
    "subprogram name",
    "subprogram name",
    "alternate return specifier" };

// This table is used for generated:
//      1. error messages
//      2. names for types used by browser

char    *TypeKW[] = {
    "LOGICAL*1",
    "LOGICAL",
    "INTEGER*1",
    "INTEGER*2",
    "INTEGER",
    "REAL",
    "DOUBLEPRECISION",
    "EXTENDEDPRECISION",
    "COMPLEX",
    "DOUBLECOMPLEX",
    "EXTENDEDCOMPLEX",
    "CHARACTER",
    "STRUCTURE",
    "generic" };
