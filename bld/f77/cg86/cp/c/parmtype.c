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
// PARMTYPE     : determine argument type and code
//

#include <stdio.h>
#include "ftnstd.h"
#include "parmtype.h"
#include "opn.h"
#include "global.h"
#include "errcod.h"

extern  int             TypeSize(uint);
extern  void            Error( int, ... );

static int pt_table[] = {
  PT_LOG_1,             // LOGICAL*1
  PT_LOG_4,             // LOGICAL*4
  PT_INT_1,             // INTEGER*1
  PT_INT_2,             // INTEGER*2
  PT_INT_4,             // INTEGER*4
  PT_REAL_4,            // REAL*4
  PT_REAL_8,            // REAL*8
  PT_REAL_16,           // REAL*16
  PT_CPLX_8,            // COMPLEX*8
  PT_CPLX_16,           // COMPLEX*16
  PT_CPLX_32,           // COMPLEX*32
  PT_CHAR,              // CHARACTER
  PT_STRUCT,            // STRUCTURE
};

int     ParmType( int typ, int size ) {
//=====================================

    if( typ == TY_NO_TYPE ) return( PT_NOTYPE );
    size = size;
    return( pt_table[ typ ] );
}
