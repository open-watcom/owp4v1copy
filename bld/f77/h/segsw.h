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


#define SG_STMT_PROCESSED       0x0001
#define SG_PROLOG_DONE          0x0002
#define SG_SYMTAB_RESOLVED      0x0004
#define SG_NO_MORE_IMPLICIT     0x0008
#define SG_NO_MORE_SPECS        0x0010
#define SG_NO_MORE_SF           0x0020
#define SG_LITTLE_SAVE          0x0040
#define SG_BIG_SAVE             0x0080
#define SG_FIRST_DB_ISN         0x0100
#define SG_IMPLICIT_NONE        0x0200
#define SG_IMPLICIT_STMT        0x0400
#define SG_DEFINING_STRUCTURE   0x0800
#define SG_DEFINING_UNION       0x1000
#define SG_DEFINING_MAP         0x2000
#define SG_SEEN_DATA            0x4000  // a data stmt has been compiled
