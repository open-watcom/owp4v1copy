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
// OBJUTIL   : object memory access routines
//

#include <malloc.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <tinyio.h>

#include "ftnstd.h"
#include "errcod.h"
#include "progsw.h"
#include "global.h"
#include "fcdata.h"
#include "undef.h"
#include "objutil.h"
#include "tdreloc.h"
#include "cpopt.h"
#include "genobj.h"

extern  void            InfoError(int,...);

static  uint            SegLen;

#if _TARGET == _8086

static  uint            AllocSegs;
static  void            *initObjCode;
unsigned_16             ObjCode = { 0 };

#define MIN_ALLOC_SEGS          0x1000
#define MK_OBJPTR( offs )       MK_FP( CurrSeg, offs )


void    InitObj() {
//=================

// Initialize object memory.

    void        *ptr;

    AllocSegs = ObjLimit / 0x10;
    for(;;) {
        initObjCode = halloc( AllocSegs, 0x10 );
        if( initObjCode != NULL ) break;
        if( AllocSegs <= MIN_ALLOC_SEGS ) break;
        AllocSegs -= MIN_ALLOC_SEGS;
    }
    if( initObjCode == NULL ) {
        AllocSegs = 0;
        InfoError( MO_OBJECT_OUT );
    } else {
        ptr = initObjCode;
        ptr = MK_FP( ( FP_SEG( ptr ) + ( FP_OFF( ptr ) >> 4 ) ),
                ( FP_OFF( ptr ) & 0x0f ) );
        ObjCode = FP_SEG( ptr );
        if( FP_OFF( ptr ) ) {
            ObjCode++;
            AllocSegs--;
        }
        CurrSeg = ObjCode;
        ObjPtr = 0;
    }
}


uint_32 AvailMem() {
//==================

// Return amount of unused object memory.

    unsigned long int   mem_left;

    mem_left = ( (unsigned long)ObjCode + AllocSegs ) << 4;
    mem_left -= ( (unsigned long)CurrSeg << 4 ) + ObjPtr;
    return( mem_left );
}


void    RelUnused() {
//===================

// Release unused object memory to system.

}


void    RelMem() {
//================

// Release object memory.

    if( AllocSegs != 0 ) {
        hfree( initObjCode );
        AllocSegs = 0;
    }
}


void    NewSPUnit() {
//===================

// Start a subprogram unit.

    if( ProgSw & PS_ERROR ) return;
    NewGItem();
    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr != 0 ) return;
    ObjPtr = 0x10;
    --CurrSeg;
}


void    NewGItem() {
//==================

// Start a global item.

    unsigned_16 segs_left;

    if( ProgSw & PS_ERROR ) return;
    CurrSeg += ObjPtr >> 4;
    ObjPtr &= 0xf;
    segs_left = AllocSegs - ( CurrSeg - ObjCode );
    if( segs_left >= (unsigned int)0x1000 ) {
        SegLen = 0xffff;
    } else {
        SegLen = ( segs_left << 4 ) - ObjPtr;
    }
}

#else

char    *ObjBuffer;

#define MK_OBJPTR( offs )       (void *)( offs )


void    InitObj() {
//=================

// Initialize object memory.

    ObjBuffer = malloc( ObjLimit );
    if( ObjBuffer == NULL ) {
        InfoError( MO_OBJECT_OUT );
    }
    ObjPtr = (obj_ptr)ObjBuffer;
    SegLen = ObjPtr + ObjLimit;
}


uint_32 AvailMem() {
//==================

// Return amount of unused object memory.

    return( SegLen - ObjPtr );
}


void    RelUnused() {
//===================

// Release unused object memory to system.

}


void    RelMem() {
//================

// Release object memory.

    if( ObjBuffer ) {
        free( ObjBuffer );
        ObjBuffer = NULL;
    }
}


void    NewSPUnit() {
//===================

// Start a subprogram unit.

}


void    NewGItem() {
//==================

// Start a global item.

}
#endif


void    AlignEven( inttarg size ) {
//=================================

// Align ObjPtr on an even boundary.

    if( ProgSw & PS_ERROR ) return;
    if( ( ObjPtr + size ) & 0x0001 ) OutByte( 0 );
}


void    BumpPtr( uint size ) {
//============================

// Increment ObjPtr.

    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr + size < ObjPtr ) {
        InfoError( MO_COMPILER_LIMIT, "code area", ~0U );
    } else if( ObjPtr + size >= SegLen ) {
        InfoError( MO_OBJECT_OUT );
    } else {
        ObjPtr += size;
    }
}


void    OutByte( char value ) {
//=============================

// Put a byte to object memory.

    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr + sizeof( char ) < ObjPtr ) {
        InfoError( MO_COMPILER_LIMIT, "code area", ~0U );
    } else if( ObjPtr + sizeof( char ) >= SegLen ) {
        InfoError( MO_OBJECT_OUT );
    } else {
        *( char PGM * )MK_OBJPTR( ObjPtr ) = value;
        ObjPtr += sizeof( char );
    }
}


void    OutBytes( const char *src, uint len ) {
//=============================================

// Put bytes to object memory.

    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr + len < ObjPtr ) {
        InfoError( MO_COMPILER_LIMIT, "code area", ~0U );
    } else if( ObjPtr + len >= SegLen ) {
        InfoError( MO_OBJECT_OUT );
    } else {
#if _TARGET == _80386
        memcpy( MK_OBJPTR( ObjPtr ), src, len );
#else
        _fmemcpy( MK_OBJPTR( ObjPtr ), src, len );
#endif
        ObjPtr += len;
    }
}


void    OutU16( unsigned_16 value ) {
//===================================

// Put a 16-bit value to object memory.

    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr + sizeof( unsigned_16 ) < ObjPtr ) {
        InfoError( MO_COMPILER_LIMIT, "code area", ~0U );
    } else if( ObjPtr + sizeof( unsigned_16 ) >= SegLen ) {
        InfoError( MO_OBJECT_OUT );
    } else {
        *( unsigned_16 PGM * )MK_OBJPTR( ObjPtr ) = value;
        ObjPtr += sizeof( unsigned_16 );
    }
}


void    OutU32( unsigned_32 value ) {
//===================================

// Put a 32-bit value to object memory.

    if( ProgSw & PS_ERROR ) return;
    if( ObjPtr + sizeof( unsigned_32 ) < ObjPtr ) {
        InfoError( MO_COMPILER_LIMIT, "code area", ~0U );
    } else if( ObjPtr + sizeof( unsigned_32 ) >= SegLen ) {
        InfoError( MO_OBJECT_OUT );
    } else {
        *( unsigned_32 PGM * )MK_OBJPTR( ObjPtr ) = value;
        ObjPtr += sizeof( unsigned_32 );
    }
}


void    OutInt( inttarg value ) {
//===============================

// Put an integer value to object memory.

#if _TARGET == _8086
    OutU16( value );
#else
    OutU32( value );
#endif
}


void    OutObjPtr( obj_ptr value ) {
//==================================

// Put an object pointer to object memory.

#if _TARGET == _80386
    if( _GenObjectFile() ) {
        if( ProgSw & PS_ERROR ) return;
        GObjOutPtr( value );
        return;
    }
    OutU32( value );
#else
    OutU16( value );
#endif
}

#if _TARGET == _8086

static  void    OutTargAddr( unsigned_32 value ) {
//================================================

// Put an object pointer to object memory.

    if( _GenObjectFile() ) {
        if( ProgSw & PS_ERROR ) return;
        GObjTargAddr( value );
        return;
    }
    OutU32( value );
}

#endif


void    OutCodePtr( obj_ptr value ) {
//===================================

// Put a object pointer that refers to other code in the current SP

#if _TARGET == _8086
    OutU16( value );
#else
    OutU32( value );
#endif
}


void    OutDataPtr( obj_ptr value ) {
//===================================

// Put a object pointer that refers to the local data seg of the current SP

#if _TARGET == _8086
    OutU16( value );
#else
    OutU32( value );
#endif
}



void    OutLocalRef( obj_ptr value ) {
//====================================

// Output a reference to a local value

#if _TARGET == _80386
    if( _GenObjectFile() ) {

        targ_addr       ta;

        if( ProgSw & PS_ERROR ) return;
        GObjOutPtr( 0 );
        ta.offset = value;
        GObjPutPtr( ObjPtr - sizeof( obj_ptr ), value, GObjFindSeg( ta ) );
        return;
    }
#endif
    OutObjPtr( value );
}



void    OutIIPtr() {
//==================

// Put an indirect pointer to object memory
// that will be relocated at compile time

#if _TARGET == _80386
    OutObjPtr( 0 );
#else
    OutTargAddr( 0 );
#endif
}


void    OutRTPtr() {
//==================

// Put an indirect ptr to object memory that will be set at run time

    OutU32( 0 );
}


void    OutSCB( inttarg len, int cp_reloc, int allocatable ) {
//============================================================

// Put an SCB in object memory.

    if( cp_reloc && !allocatable ) {
        OutIIPtr();
    } else {
        OutRTPtr();
    }
    OutInt( len );
    if( allocatable ) {
        OutU16( 0 );
    }
}


void    OutRCB( sym_id sd, int cp_reloc ) {
//=========================================

// Put an RCB in object memory.

    if( cp_reloc ) {
        OutIIPtr();
    } else {
        OutRTPtr();
    }
    SymRef( sd );
}


void PutU16( obj_ptr ptr, unsigned_16 val ) {
//===========================================

    *(unsigned_16 PGM *)MK_OBJPTR( ptr ) = val;
}


obj_ptr PutPtr( obj_ptr ptr, obj_ptr val ) {
//==========================================

// Exchange pointer in object memory.

    obj_ptr             PGM *optr;
    obj_ptr             old_val;

    optr = MK_OBJPTR( ptr );
    old_val = *optr;
    *optr = val;
    return( old_val );
}


void    Undefined( inttarg size ) {
//=================================

// Fill object memory with undefined pattern.

    void PGM *dst;

    dst = MK_OBJPTR( ObjPtr );
    BumpPtr( size );
    if( ProgSw & PS_ERROR ) return;
#if _TARGET == _80386
    memset( dst, UNDEF_CHAR, size );
#else
    _fmemset( dst, UNDEF_CHAR, size );
#endif
}


void    PutAddr( targ_addr *tloc, targ_addr *tlink ) {
//====================================================

    void PGM * PGM      *ptr;
    void PGM *          *loc;
    void PGM *          *link;

    loc = (void PGM **)tloc;
    link = (void PGM **)tlink;
    ptr = *loc;
    *loc = *ptr;
    *ptr = *link;
}


obj_ptr ObjTell() {
//=================

    return( ObjPtr );
}


int     ObjOffset( obj_ptr prev_obj ) {
//=====================================

    return( ObjPtr - prev_obj );
}
