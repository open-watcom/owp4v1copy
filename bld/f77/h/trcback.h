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


// Define traceback information:
// ==============================

// This is the basic traceback structure that must be common to optimizing
// and load 'n go compilers. If the line field is TB_LG it indicates that the
// traceback is a load 'n go traceback

typedef struct traceback {
    void PGM            *link;          // pointer to previous traceback
    uint                line;           // current line
    char                *name;          // current module
} traceback;

#define TB_LG           (~(uint)0)
#define TB_LG_DB        (~(uint)0-1)

#if _OPT_CG == _ON

#if defined( __MT__ )

    #include "fthread.h"

    #define     ExCurr          (*(traceback **)&(__FTHREADDATAPTR->__ExCurr))

#endif

#else

typedef struct lg_traceback {
    void PGM            *link;          // pointer to previous traceback
    uint                line;           // has the value TB_LG
    obj_ptr             name;           // offset to entry name
    obj_ptr             lineptr;        // offset to previous line
#if _TARGET == _8086 || _TARGET == _80386
    obj_ptr             epilog_seq;     // offset to epilogue sequence
#endif
} lg_traceback;

typedef struct db_traceback {
    void PGM            *link;          // pointer to previous traceback
    uint                line;           // has the value TB_LG_DB
    obj_ptr             name;           // offset to entry name
    obj_ptr             lineptr;        // offset to previous line
#if _TARGET == _8086 || _TARGET == _80386
    obj_ptr             epilog_seq;     // offset to epilogue sequence
    obj_ptr             data_area;      // offset to data area
    obj_ptr             fc_call;        // for calling F-Codes in debugger
#else
    byte                *first_isn;     // pointer to first source line
#endif
} db_traceback;

#if _TARGET == _8086 || _TARGET == _80386

typedef struct epilog_sequence {
    unsigned_16         epi_fcode;      // epilogue F-Code
    obj_ptr             darg_list;      // offset to dummy argument list
    void                PGM *ret_addr;  // return address
#if _TARGET == _80386
    obj_ptr             ret_data_base;  // where to save data offset of caller
    obj_ptr             my_data_base;   // data offset of sp data area
#endif
    char                func_val;       // return value
} epilog_sequence;

#endif

typedef struct sf_trace {
    obj_ptr             ret;            // return address
    struct sf_trace     PGM *link;      // pointer to previous traceback
    uint                line;           // TB_LG
    char                *name;          // pointer to entry name
    obj_ptr             line_ptr;       // pointer to previous line
} sf_trace;

#if defined( __MT__ )

    #include "fthread.h"

    #define     ExCurr          (*(lg_traceback **)&(__FTHREADDATAPTR->__ExCurr))

#endif

#endif
