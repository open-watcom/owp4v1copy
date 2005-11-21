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
* Description:  compile-time constants define symbols type
*
****************************************************************************/

#ifndef _SYMTYPES_H_INCLUDED
#define _SYMTYPES_H_INCLUDED

// TYPES
// =====

#ifdef pick
#undef pick
#endif

#define pick(id,text,size,ptype) id,

enum SYM_TYPES {
#include "symdefn.h"
};

typedef enum SYM_TYPES TYPE;

#define TY_FIRST    TY_LOGICAL_1

#define TY_EXTENDED TY_DOUBLE
#define TY_XCOMPLEX TY_DCOMPLEX

#define FIRST_BASE_TYPE TY_LOGICAL_1
#define LAST_BASE_TYPE  TY_TRUE_XCOMPLEX

#if _CPU == 8086
 #define TY_INTEGER_TARG TY_INTEGER_2
#else
 #define TY_INTEGER_TARG TY_INTEGER
#endif

#define _IsTypeLogical( typ )   ((typ >= TY_LOGICAL_1) && (typ <= TY_LOGICAL))
#define _IsTypeInteger( typ )   ((typ >= TY_INTEGER_1) && (typ <= TY_INTEGER))

#endif
