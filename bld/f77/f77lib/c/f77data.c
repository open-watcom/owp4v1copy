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
// F77DATA      : DATA statement processor
//

#include "ftnstd.h"
#include "rundat.h"
#include "datastmt.h"
#include "parmtype.h"
#include "errcod.h"
#include "pgmacc.h"
#include "rtenv.h"
#include "trcback.h"

#include <dos.h>

extern  int             IOMain(void);
extern  void            SetIOFlags(bool);
extern  bool            UndefData(char PGM *,int);
extern  void            RTErr(int,...);
#if _TARGET == _80386
extern  void            ExecData(void PGM *,obj_ptr);
#else
extern  void            ExecData(void PGM *);
#endif

extern  const byte __FAR        SizeVars[];

#if _TARGET == _8086
static  unsigned_16     DataSeg;
#elif _TARGET == _80386
static  obj_ptr         DataBase;
#endif
static  obj_ptr         ItemPtr;


void    F77Data( void PGM **data_chain ) {
//========================================

// Start DATA statement processing.

    data_prol   PGM *data_stmt;

    data_stmt = *data_chain;
    while( data_stmt != NULL ) {
#if _TARGET == _8086
        ExCurr = MK_FP( FP_SEG( data_stmt ), data_stmt->tb_info );
        DataSeg = FP_SEG( data_stmt );
#elif _TARGET == _80386
        ExCurr = (void PGM *)CODE_REL( data_stmt->tb_info );
#else
        ExCurr = (void PGM *)data_stmt->tb_info;
#endif
        ExLinePtr = FP_OFF( data_stmt ) + offsetof( data_prol, isn_ptr );
        ItemPtr = data_stmt->const_ptr;
        SetIOFlags( FALSE );
#if _TARGET == _80386
        DataBase =
            ((epilog_sequence PGM *)CODE_REL(ExCurr->epilog_seq))->my_data_base;
        ExecData( &data_stmt->data_code, DataBase );
#else
        ExecData( &data_stmt->data_code );
#endif
        data_stmt = data_stmt->dt_link;
    }
}


unsigned_16 GetDataU16() {
//========================

// Get an unsigned_16 from DATA statement sequence.

    unsigned_16 item;

#if _TARGET == _8086
    item = *(unsigned_16 PGM *)MK_FP( DataSeg, ItemPtr );
#else
    item = *(unsigned_16 PGM *)ItemPtr;
#endif
    ItemPtr += sizeof( unsigned_16 );
    return( item );
}


obj_ptr GetDataPtr() {
//====================

// Get an obj_ptr from DATA statement sequence

    obj_ptr     item;

#if _TARGET == _8086
    item = *(obj_ptr PGM *)MK_FP( DataSeg, ItemPtr );
#else
    item = *(obj_ptr PGM *)ItemPtr;
#if _TARGET == _80386
    if( item != 0 ) {
        item += DataBase;
    }
#endif
#endif
    ItemPtr += sizeof( obj_ptr );
    return( item );
}


intstar4        GetRepCount( obj_ptr curr ) {
//===========================================

// Get repitition count.

#if _TARGET == _8086
    return( *(intstar4 PGM *)MK_FP( DataSeg, curr ) );
#else
    return( *(intstar4 PGM *)curr );
#endif
}


void    PrepErr() {
//=================

// Error reporting setup.

}


static  void    I4toI1( intstar4 PGM *old, intstar1 PGM *to ) {
//=============================================================

// Convert constant old INTEGER*4 to INTEGER*1.

    *to = *old;
}


static  void    R4toI1( single PGM *old, intstar1 PGM *to ) {
//===========================================================

// Convert constant old REAL*4 to INTEGER*1.

    *to = *old;
}


static  void    R8toI1( double PGM *old, intstar1 PGM *to ) {
//===========================================================

// Convert constant old REAL*8 to INTEGER*1.

    *to = *old;
}


static  void    R10toI1( extended PGM *old, intstar1 PGM *to ) {
//===========================================================

// Convert constant old REAL*10 to INTEGER*1.

    *to = *old;
}


static  void    C8toI1( complex PGM *old, intstar1 PGM *to ) {
//============================================================

// Convert constant old COMPLEX*8 to INTSTAR4.

    *to = old->realpart;
}


static  void    C16toI1( dcomplex *old, intstar1 PGM *to ) {
//==========================================================

// Convert constant old COMPLEX*16 to INTEGER*1.

    *to = old->realpart;
}


static  void    C20toI1( xcomplex *old, intstar1 PGM *to ) {
//==========================================================

// Convert constant old COMPLEX*20 to INTEGER*1.

    *to = old->realpart;
}


static  void    I4toI2( intstar4 PGM *old, intstar2 PGM *to ) {
//=============================================================

// Convert constant old INTEGER*4 to INTEGER*2.

    *to = *old;
}


static  void    R4toI2( single PGM *old, intstar2 PGM *to ) {
//===========================================================

// Convert constant old REAL*4 to INTEGER*2.

    *to = *old;
}


static  void    R8toI2( double PGM *old, intstar2 PGM *to ) {
//===========================================================

// Convert constant old REAL*8 to INTEGER*2.

    *to = *old;
}


static  void    R10toI2( extended PGM *old, intstar2 PGM *to ) {
//===========================================================

// Convert constant old REAL*10 to INTEGER*2.

    *to = *old;
}


static  void    C8toI2( complex PGM *old, intstar2 PGM *to ) {
//============================================================

// Convert constant old COMPLEX*8 to INTEGER*2.

    *to = old->realpart;
}


static  void    C16toI2( dcomplex PGM *old, intstar2 PGM *to ) {
//==============================================================

// Convert constant old COMPLEX*16 to INTEGER*2.

    *to = old->realpart;
}


static  void    C20toI2( xcomplex PGM *old, intstar2 PGM *to ) {
//==============================================================

// Convert constant old COMPLEX*20 to INTEGER*2.

    *to = old->realpart;
}


static  void    R4toI4( single PGM *old, intstar4 PGM *to ) {
//===========================================================

// Convert constant old REAL*8 to INTEGER*4.

    *to = *old;
}


static  void    R8toI4( double PGM *old, intstar4 PGM *to ) {
//===========================================================

// Convert constant old REAL*8 to INTEGER*4.

    *to = *old;
}


static  void    R10toI4( extended PGM *old, intstar4 PGM *to ) {
//===========================================================

// Convert constant old REAL*10 to INTEGER*4.

    *to = *old;
}


static  void    C8toI4( complex PGM *old, intstar4 PGM *to ) {
//============================================================

// Convert constant old COMPLEX*8 to INTEGER*4.

    *to = old->realpart;
}


static  void    C16toI4( dcomplex PGM *old, intstar4 PGM *to ) {
//==============================================================

// Convert constant old COMPLEX*16 to INTEGER*4.

    *to = old->realpart;
}


static  void    C20toI4( xcomplex PGM *old, intstar4 PGM *to ) {
//==============================================================

// Convert constant old COMPLEX*20 to INTEGER*4.

    *to = old->realpart;
}


static  void    I4toR4( intstar4 PGM *old, single PGM *to ) {
//===========================================================

// Convert constant old INTEGER*4 to REAL*4.

    *to = *old;
}


static  void    R8toR4( double PGM *old, single PGM *to ) {
//=========================================================

// Convert constant old REAL*8 to REAL*4.

    *to = *old;
}


static  void    R10toR4( extended PGM *old, single PGM *to ) {
//=========================================================

// Convert constant old REAL*10 to REAL*4.

    *to = *old;
}


static  void    C8toR4( complex PGM *old, single PGM *to ) {
//==========================================================

// Convert constant old COMPLEX*8 to REAL*4.

    *to = old->realpart;
}


static  void    C16toR4( dcomplex PGM *old, single PGM *to ) {
//============================================================

// Convert constant old COMPLEX*16 to REAL*4.

    *to = old->realpart;
}


static  void    C20toR4( xcomplex PGM *old, single PGM *to ) {
//============================================================

// Convert constant old COMPLEX*20 to REAL*4.

    *to = old->realpart;
}


static  void    I4toR8( intstar4 PGM *old, double PGM *to ) {
//===========================================================

// Convert constant old INTEGER*4 to REAL*8.

    *to = *old;
}


static  void    R4toR8( single PGM *old, double PGM *to ) {
//=========================================================

// Convert constant old REAL*4 to REAL*8.

    *to = *old;
}


static  void    R10toR8( extended PGM *old, double PGM *to ) {
//=========================================================

// Convert constant old REAL*10 to REAL*8.

    *to = *old;
}


static  void    C8toR8( complex PGM *old, double PGM *to ) {
//==========================================================

// Convert constant old COMPLEX*8 to REAL*8.

    *to = old->realpart;
}


static  void    C16toR8( dcomplex PGM *old, double PGM *to ) {
//============================================================

// Convert constant old COMPLEX*16 to REAL*8.

    *to = old->realpart;
}


static  void    C20toR8( xcomplex PGM *old, double PGM *to ) {
//============================================================

// Convert constant old COMPLEX*20 to REAL*8.

    *to = old->realpart;
}


static  void    I4toR10( intstar4 PGM *old, extended PGM *to ) {
//===========================================================

// Convert constant old INTEGER*4 to REAL*10.

    *to = *old;
}


static  void    R4toR10( single PGM *old, extended PGM *to ) {
//=========================================================

// Convert constant old REAL*4 to REAL*10.

    *to = *old;
}


static  void    R8toR10( double PGM *old, extended PGM *to ) {
//=========================================================

// Convert constant old REAL*8 to REAL*10.

    *to = *old;
}


static  void    C8toR10( complex PGM *old, extended PGM *to ) {
//==========================================================

// Convert constant old COMPLEX*8 to REAL*10.

    *to = old->realpart;
}


static  void    C16toR10( dcomplex PGM *old, extended PGM *to ) {
//============================================================

// Convert constant old COMPLEX*16 to REAL*10.

    *to = old->realpart;
}


static  void    C20toR10( xcomplex PGM *old, extended PGM *to ) {
//============================================================

// Convert constant old COMPLEX*20 to REAL*10.

    *to = old->realpart;
}


static  void    I4toC8( intstar4 PGM *old, complex PGM *to ) {
//============================================================

// Convert constant old INTEGER*4 to COMPLEX*8.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R4toC8( single PGM *old, complex PGM *to ) {
//==========================================================

// Convert constant old REAL*4 to COMPLEX*8.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R8toC8( double PGM *old, complex PGM *to ) {
//==========================================================

// Convert constant old REAL*8 to COMPLEX*8.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R10toC8( extended PGM *old, complex PGM *to ) {
//==========================================================

// Convert constant old REAL*10 to COMPLEX*8.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    C16toC8( dcomplex PGM *old, complex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*16 to COMPLEX*8.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    C20toC8( xcomplex PGM *old, complex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*16 to COMPLEX*8.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    I4toC16( intstar4 PGM *old, dcomplex PGM *to ) {
//==============================================================

// Convert constant old INTEGER*4 to COMPLEX*16.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R4toC16( single PGM *old, dcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*4 to COMPLEX*16.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R8toC16( double PGM *old, dcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*8 to COMPLEX*16.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R10toC16( extended PGM *old, dcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*10 to COMPLEX*16.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    C8toC16( complex PGM *old, dcomplex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*8 to COMPLEX*16.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    C20toC16( xcomplex PGM *old, dcomplex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*20 to COMPLEX*16.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    I4toC20( intstar4 PGM *old, xcomplex PGM *to ) {
//==============================================================

// Convert constant old INTEGER*4 to COMPLEX*20.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R4toC20( single PGM *old, xcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*4 to COMPLEX*20.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R8toC20( double PGM *old, xcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*8 to COMPLEX*20.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    R10toC20( extended PGM *old, xcomplex PGM *to ) {
//============================================================

// Convert constant old REAL*10 to COMPLEX*20.

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    C8toC20( complex PGM *old, xcomplex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*8 to COMPLEX*20.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    C16toC20( xcomplex PGM *old, xcomplex PGM *to ) {
//=============================================================

// Convert constant old COMPLEX*16 to COMPLEX*20.

    to->realpart = old->realpart;
    to->imagpart = old->imagpart;
}


static  void    I1toI2( intstar1 PGM *old, intstar2 PGM *to ) {
//=============================================================

    *to = *old;
}


static  void    I1toI4( intstar1 PGM *old, intstar4 PGM *to ) {
//=============================================================

    *to = *old;
}


static  void    I1toR4( intstar1 PGM *old, single PGM *to ) {
//===========================================================

    *to = *old;
}


static  void    I1toR8( intstar1 PGM *old, double PGM *to ) {
//===========================================================

    *to = *old;
}


static  void    I1toR10( intstar1 PGM *old, extended PGM *to ) {
//===========================================================

    *to = *old;
}


static  void    I1toC8( intstar1 PGM *old, complex PGM *to ) {
//============================================================

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    I1toC16( intstar1 PGM *old, dcomplex PGM *to ) {
//==============================================================

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    I1toC20( intstar1 PGM *old, xcomplex PGM *to ) {
//==============================================================

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    I2toI1( intstar2 PGM *old, intstar1 PGM *to ) {
//=============================================================

    *to = *old;
}


static  void    I2toI4( intstar2 PGM *old, intstar4 PGM *to ) {
//=============================================================

    *to = *old;
}


static  void    I2toR4( intstar2 PGM *old, single PGM *to ) {
//===========================================================

    *to = *old;
}


static  void    I2toR8( intstar2 PGM *old, double PGM *to ) {
//===========================================================

    *to = *old;
}


static  void    I2toR10( intstar2 PGM *old, extended PGM *to ) {
//===========================================================

    *to = *old;
}

static  void    I2toC8( intstar2 PGM *old, complex PGM *to ) {
//============================================================

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    I2toC16( intstar2 PGM *old, dcomplex PGM *to ) {
//==============================================================

    to->realpart = *old;
    to->imagpart = 0;
}


static  void    I2toC20( intstar2 PGM *old, xcomplex PGM *to ) {
//==============================================================

    to->realpart = *old;
    to->imagpart = 0;
}

// This has to be after all the conversion routines... otherwise
// prototypes are req'd for each one.
static  void            (* const __FAR DataCnvTab[])() = {
NULL,     &I2toI1,  &I4toI1,  &R4toI1,  &R8toI1,  &R10toI1,  &C8toI1,  &C16toI1, &C20toI1,
&I1toI2,  NULL,     &I4toI2,  &R4toI2,  &R8toI2,  &R10toI2,  &C8toI2,  &C16toI2, &C20toI2,
&I1toI4,  &I2toI4,  NULL,     &R4toI4,  &R8toI4,  &R10toI4,  &C8toI4,  &C16toI4, &C20toI4,
&I1toR4,  &I2toR4,  &I4toR4,  NULL,     &R8toR4,  &R10toR4,  &C8toR4,  &C16toR4, &C20toR4,
&I1toR8,  &I2toR8,  &I4toR8,  &R4toR8,  NULL,     &R10toR8,  &C8toR8,  &C16toR8, &C20toR8,
&I1toR10, &I2toR10, &I4toR10, &R4toR10, &R8toR10, NULL,      &C8toR10, &C16toR10,&C20toR10,
&I1toC8,  &I2toC8,  &I4toC8,  &R4toC8,  &R8toC8,  &R10toC8,  NULL,     &C16toC8, &C20toC8,
&I1toC16, &I2toC16, &I4toC16, &R4toC16, &R8toC16, &R10toC16, &C8toC16, NULL,     &C20toC16,
&I1toC20, &I2toC20, &I4toC20, &R4toC20, &R8toC20, &R10toC20, &C8toC20, C16toC20, NULL
    };
#define CONST_TYPES     9


void    AsnVal( obj_ptr const_data, int const_type, int var_type ) {
//==================================================================

// Do data initialization.

    int         const_size;
    int         var_size;
    int         size;
    char        PGM *const_ptr;
    char        PGM *var_ptr;
    byte        const_buff[sizeof(ftn_type)];

#if _TARGET == _8086
    const_ptr = MK_FP( DataSeg, const_data );
#else
    const_ptr = (char PGM *)const_data;
#endif
    if( ( const_type == PT_CHAR ) || ( const_type == PT_NOTYPE ) ) {
        const_size = ((string PGM *)const_ptr)->len;
        const_ptr = ((string PGM *)const_ptr)->strptr;
    } else {
        const_size = SizeVars[ const_type ];
    }
    if( var_type == PT_CHAR ) {
        var_ptr = IORslt.string.strptr;
        var_size = IORslt.string.len;
    } else {
        var_ptr = IORslt.pgm_ptr;
        var_size = SizeVars[ var_type ];
    }
    if( !UndefData( var_ptr, var_size ) ) {
        RTErr( DA_DATA_TWICE );
    }
    if( const_type == PT_CHAR ) {
        if( const_size >= var_size ) {
            pgm_memcpy( var_ptr, const_ptr, var_size );
        } else {
            pgm_memcpy( var_ptr, const_ptr, const_size );
            pgm_memset( var_ptr + const_size, ' ', var_size - const_size );
        }
    } else if( const_type == PT_NOTYPE ) {  // initialization with Z-constant
        if( var_type != PT_CHAR ) {
            size = var_size;
            while( size > const_size ) {
                size--;
                const_buff[ size ] = 0;
            }
            while( size > 0 ) {
                size--;
                const_buff[ size ] = *const_ptr;
                const_ptr++;
            }
            const_ptr = &const_buff;
            const_size = var_size;
        }
        if( const_size < var_size ) {
            pgm_memset( var_ptr, 0, var_size - const_size );
            var_ptr += var_size - const_size;
            var_size = const_size;
        } else {
            const_ptr += const_size - var_size;
        }
        pgm_memcpy( var_ptr, const_ptr, var_size );
    } else if( const_type <= PT_LOG_4 ) {
        pgm_memcpy( var_ptr, const_ptr, var_size );
    } else {        // numeric to numeric
        if( const_type != var_type ) {
            DataCnvTab[ ( var_type - PT_INT_1 ) * CONST_TYPES +
                        ( const_type - PT_INT_1 ) ]( const_ptr, var_ptr );
        } else {
            pgm_memcpy( var_ptr, const_ptr, var_size );
        }
    }
}
