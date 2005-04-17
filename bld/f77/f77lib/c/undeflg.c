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
// UNDEFLG      : output routines that don't do undefined checking
//

#include "ftnstd.h"
#include "rundat.h"
#include "undefrtn.h"
#include "parmtype.h"
#include "fmtdef.h"

extern  void            OutLogCG(void);
extern  void            OutIntCG(void);
extern  bool            UndefInt(void);
extern  bool            UndefLog(void);
extern  bool            UndefReal(single *);
extern  bool            UndefDouble(double *);
extern  bool            UndefExtended(extended *);
extern  void            FmtUFloat(char *,uint);
extern  void            SendUChr(void);
extern  void            USendStr(char PGM *,int);
extern  void            FmtRealCG(char *,single *);
extern  void            FmtDoubleCG(char *,double *);
extern  void            FmtExtendedCG(char *,extended *);
extern  void            SendNUChr(uint);
extern  bool            GetReal(extended *);
extern  void            UFill(char *,uint);

extern  void            (* __FAR OutRtn[])(void);


void OutLogLG() {
//===============

    if( UndefLog() ) {
        SendUChr();
    } else {
        OutLogCG();
    }
}


void OutIntLG() {
//===============

    if( UndefInt() ) {
        SendUChr();
    } else {
        OutIntCG();
    }
}


void    FmtRealLG( char *buffer, single *value ) {
//================================================

    if( UndefReal( value ) ) {
        FmtUFloat( buffer, 2 * sizeof( single ) );
    } else {
        FmtRealCG( buffer, value );
    }
}


void    FmtDoubleLG( char *buffer, double *value ) {
//==================================================

    if( UndefDouble( value ) ) {
        FmtUFloat( buffer, 2 * sizeof( double ) );
    } else {
        FmtDoubleCG( buffer, value );
    }
}


void    FmtExtendedLG( char *buffer, extended *value ) {
//======================================================

    if( UndefExtended( value ) ) {
        FmtUFloat( buffer, 2 * sizeof( extended ) );
    } else {
        FmtExtendedCG( buffer, value );
    }
}


bool    UndefLogLG() {
//====================

    if( UndefLog() ) {
        SendNUChr( IOCB->fmtptr->fmt1.fld1 );
        return( 1 );
    }
    return( 0 );
}


bool GetRealLG( extended *value, uint width ) {
//=============================================

    ftnfile     *fcb;

    if( GetReal( value ) ) return( 1 );
    fcb = IOCB->fileinfo;
    UFill( fcb->buffer + fcb->col, width );
    return( 0 );
}


bool UndefIntLG( uint width ) {
//=============================

    if( UndefInt() ) {
        SendNUChr( width );
        return( 1 );
    }
    return( 0 );
}


void InitUndefIO() {
//==================

// Patch the vectors to point at undefine'd checking code.
// Note that the vectors are never changed back to their link-time
// values.  This is because all these routines check IOCB->flags & UNDEF_IO.

    OutRtn[ PT_LOG_1 ] = OutLogLG;
    OutRtn[ PT_LOG_4 ] = OutLogLG;
    OutRtn[ PT_INT_1 ] = OutIntLG;
    OutRtn[ PT_INT_2 ] = OutIntLG;
    OutRtn[ PT_INT_4 ] = OutIntLG;
    FmtRealRtn = FmtRealLG;
    FmtDoubleRtn = FmtDoubleLG;
    FmtExtendedRtn = FmtExtendedLG;
    SendStrRtn = USendStr;
    UndefRealRtn = UndefReal;
    UndefDoubleRtn = UndefDouble;
    UndefExtendedRtn = UndefExtended;
    UndefLogRtn = UndefLogLG;
    GetRealRtn = GetRealLG;
    UndefIntRtn = UndefIntLG;
}
