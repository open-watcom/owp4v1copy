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
//      DO-loop control block layout
//
typedef struct {
    void PGM    *do_var;        // ptr to DO-loop variable
    obj_ptr     do_addr;        // address of top of loop
    intstar4    do_cnt;         // iteration count
} do_loop_base;

typedef struct {
    do_loop_base        base;   // common information
    intstar4            do_incr;// increment
} do_intstar4;                  // also for int*1 and int*2

typedef struct {
    do_loop_base        base;   // common information
    single              do_incr;// increment
} do_single;

typedef struct {
    do_loop_base        base;   // common information
    double              do_incr;// increment
} do_double;

typedef struct {
    do_loop_base        base;   // common information
    extended            do_incr;// increment
} do_extended;
