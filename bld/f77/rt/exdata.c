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
// EXDATA       : Execute DATA statement
//

#include "ftnstd.h"
#include "parmtype.h"
#include "errcod.h"
#include "rundat.h"
#include "iotype.h"
#include "rtenv.h"
#include "trcback.h"

extern  obj_ptr         GetDataPtr(void);
extern  unsigned_16     GetDataU16(void);
extern  void            AsnVal(obj_ptr,int,int);
extern  void            PrepErr(void);
extern  intstar4        GetRepCount(obj_ptr);
extern  void            RTErr(int,...);
extern  void            DatTypMis(int,int,int);
extern  int             IOMain(void (*)());
extern  void            ArrayIOType(void);


static  bool    Numeric( byte typ ) {
//===================================

    return( ( typ >= PT_INT_1 ) && ( typ <= PT_CPLX_16 ) );
}


static  void    ExData() {
//========================

    int         var_typ;
    obj_ptr     rep_ptr;
    intstar4    rep_count;
    obj_ptr     cons_ptr;
    int         cons_typ;
    bool        data_done;
    bool        vars_done;

    PrepErr();
    for(;;) {
        for(;;) {
            ArrayIOType();
            var_typ = IOCB->typ;
            if( var_typ == PT_NOTYPE ) break;
            vars_done = ( var_typ == PT_END_DATASET );
            rep_ptr = GetDataPtr();
            data_done = ( rep_ptr == NULL );
            if( vars_done || data_done ) break;
            rep_count = GetRepCount( rep_ptr );
            cons_typ  = GetDataU16();
            cons_ptr  = GetDataPtr();
            for(;;) {
                if( vars_done ) break;
                if( ( cons_typ != PT_NOTYPE ) &&
                    ( ( var_typ > PT_LOG_4 ) || ( cons_typ > PT_LOG_4 ) ) &&
                    ( cons_typ != PT_CHAR ) &&
                    ( !Numeric( var_typ ) || !Numeric( cons_typ ) ) ) {
                    DatTypMis( DA_TYPE_MISMATCH, var_typ, cons_typ );
                }
                AsnVal( cons_ptr, cons_typ, var_typ );
                if( --rep_count <= 0 ) break;
                ArrayIOType();
                var_typ = IOCB->typ;
                vars_done = ( var_typ == PT_END_DATASET );
            }
            if( vars_done ) break;
        }
        if( var_typ == PT_NOTYPE ) break;
        if( data_done != vars_done ) {
            if( data_done ) {
                RTErr( DA_NOT_ENOUGH );
            } else {
                RTErr( DA_TOO_MUCH );
            }
        }
    }
    IOCB->typ = var_typ;
    ExCurr = NULL;
}


void    IOData() {
//================

// Execute DATA statements.

    IOCB->iostmt = IO_DATA;
    IOMain( &ExData );
}
