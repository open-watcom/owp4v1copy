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
// Target dependent data structures.
//


#if _TARGET == _8086
    #include <i86.h>
    typedef struct {
        unsigned_16     offset;
        unsigned_16     seg;
    } targ_addr;
    #define _NULLTargAddr( ta ) (ta).seg = 0; (ta).offset = 0;
    #define MK_PGM( ta )        MK_FP( ta.seg, ta.offset )
#else
    typedef struct {
        unsigned_32     offset;
    } targ_addr;
    #define _NULLTargAddr( ta ) (ta).offset = 0;
    #define MK_PGM( ta )        (void PGM *)(ta.offset)
#endif

typedef uint            obj_ptr;
typedef unsigned_32     pgm_ptr;

typedef int             file_attr;      // file attributes
typedef void            *file_handle;   // file handle
typedef void            *lib_handle;    // library handle

#include "lgadv.h"

// used for stand alone object files (make sure it is a power of 2 in size)
typedef struct {
    char PGM    *origin;                // origin of common block
    unsigned_32 size;                   // size of common block
} cb_defn;

#if _TARGET == _80386
// macro to do 'esi'-relative code relocations (see CodeReloc)
#define CODE_REL( __p ) ( (__p) + (obj_ptr)&(__p) + sizeof( obj_ptr ) )
#endif
