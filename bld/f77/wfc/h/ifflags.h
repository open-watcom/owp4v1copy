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
// byte flags for intrinsic function table
//
// Table Bit Map
// +==========+==================+======================+====================+
// |  15  14  |  13  12  11  10  |  09  08  07  06  05  | 04  03  02  01  00 |
// +----------+------------------+----------------------+--------------------+
// | Generic &| Empty            | Function Return type | Parameter Type     |
// | Argument | For future use   | See ifflags.h        | See symflgs.h      |
// | bits     |                  |                      |                    |
// +==========+==================+======================+====================+
// NOTES : arg-ok  <=> name may be passed as an argument to subprogram
//       : generic <=> name is a generic name ( may also be specific )
//       : parameter type and function type correspond to symtab entries

#include "symflgs.h"

#define IF_GENERIC      0x8000
#define IF_NOT_GEN      0x0000
#define IF_ARG_OK       0x4000
#define IF_NOT_ARG      0x0000

#define IF_SHIFT_TYPE   5
#define IF_TYPE         0x1F << IF_SHIFT_TYPE

#define IFT_LOGICAL_1           TY_LOGICAL_1 << IF_SHIFT_TYPE
#define IFT_LOGICAL             TY_LOGICAL   << IF_SHIFT_TYPE
#define IFT_INTEGER_1           TY_INTEGER_1 << IF_SHIFT_TYPE
#define IFT_INTEGER_2           TY_INTEGER_2 << IF_SHIFT_TYPE
#define IFT_INTEGER             TY_INTEGER   << IF_SHIFT_TYPE
#define IFT_REAL                TY_REAL      << IF_SHIFT_TYPE
#define IFT_DOUBLE              TY_DOUBLE    << IF_SHIFT_TYPE
#define IFT_EXTENDED            TY_EXTENDED  << IF_SHIFT_TYPE
// Until the real long doubles are implemented we must use this kludge
// in all case statements
#define IFT_TRUE_EXTENDED       TY_TRUE_EXTENDED  << IF_SHIFT_TYPE
#define IFT_COMPLEX             TY_COMPLEX   << IF_SHIFT_TYPE
#define IFT_DCOMPLEX            TY_DCOMPLEX  << IF_SHIFT_TYPE
#define IFT_XCOMPLEX            TY_XCOMPLEX  << IF_SHIFT_TYPE
// Until the real long doubles are implemented we must use this kludge
// in all case statements
#define IFT_TRUE_XCOMPLEX       TY_TRUE_XCOMPLEX  << IF_SHIFT_TYPE
#define IFT_CHAR                TY_CHAR      << IF_SHIFT_TYPE
#define IFT_NOTYPE              IF_TYPE

#define IF_VAR_TYPE     0x1f
#define IF_NO_TYPE      0x1f

