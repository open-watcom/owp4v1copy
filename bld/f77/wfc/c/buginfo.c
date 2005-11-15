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
// BUGINFO  : generate extra data-area information for debugger.
//

#include "ftnstd.h"
#include "global.h"
#include "cpopt.h"
#include "progsw.h"
#include "buginfo.h"
#include "sdfile.h"
#include "objutil.h"

#include <string.h>

extern  void            FixTrcBack(sym_id);
extern  void            FixSFTrcBack(sym_id);
extern  void            FixLListHead(void);
extern  int             GetSrcName(char *);

extern  obj_ptr         ObjPtr;


void    StartLList() {
//====================

// Mark the start of the local variable list.

    if( Options & OPT_DEBUG ) {
        FixLListHead();
    }
}


void    EndLList() {
//==================

// Mark the end of the local variable list.

    if( Options & OPT_DEBUG ) {
        OutByte( DB_SPECIAL | DB_END_LIST );
    }
}


static  void    OutVarInfo( sym_id ste_ptr ) {
//============================================

// Output the code for a variable name.

    byte        dbi;
    unsigned_16 flags;

    if( Options & OPT_DEBUG ) {
        flags = ste_ptr->ns.flags;
        dbi = ParmType( ste_ptr->ns.typ, ste_ptr->ns.xt.size ) << DB_CODE_SHIFT;
        if( flags & SY_SUBSCRIPTED ) {
            dbi |= _DimCount( ste_ptr->ns.si.va.dim_ext->dim_flags );
        } else if( ( ste_ptr->ns.typ != TY_CHAR ) &&
                   ( ste_ptr->ns.typ != TY_STRUCTURE ) &&
                   ( flags & ( SY_SUB_PARM | SY_IN_EC ) ) ) {
            dbi |= DB_VAR_INDIRECT;
        }
        OutByte( dbi );
    }
}


static  void    OutEntryNameInfo( sym_id ste_ptr ) {
//==================================================

// Output the code for an entry name.

    byte        code;
    byte        flags;

    if( Options & OPT_DEBUG ) {
        flags = ste_ptr->ns.flags & SY_SUBPROG_TYPE;
        if( flags == SY_STMT_FUNC ) {
            code = DB_STMT_FUNC;
        } else {
            code = DB_ENTRY_NAME;
        }
        if( ( flags != SY_FUNCTION ) && ( flags != SY_STMT_FUNC) ) {
            code |= PT_NOTYPE;
        } else {
            code |= ParmType( ste_ptr->ns.typ, ste_ptr->ns.xt.size );
        }
        OutByte( code );
    }
}


void    OutAddrInfo() {
//=====================

// Output an address.

    if( Options & OPT_DEBUG ) {
        OutByte( DB_SPECIAL | DB_ADDR );
    }
}


void    OutSPName( sym_id ste_ptr ) {
//===================================

// Dump the name of a subprogram.

    OutTrcInfo( &FixTrcBack, ste_ptr );
}


void    OutSFName( sym_id ste_ptr ) {
//===================================

// Dump the name of a statement function.

    OutTrcInfo( &FixSFTrcBack, ste_ptr );
}


static  void    OutTrcInfo( void (*rtn)(), sym_id ste_ptr ) {
//===========================================================

// Dump the name of subprogram/statement function followed by the name of
// the file for traceback information.
//
//   Notes:  1. length needed (a) to scan over item in debugger
//                            (b) for undefined variable check error message
//
//           2. NULLCHAR needed for traceback error message

    int         fn_len;
    int         total;
    int         align;
    byte        typ;
    char        fname[MAX_FILE+1];

    fn_len = strlen( CurrFile->name );
    strcpy( fname, CurrFile->name );
    if( ProgSw & PS_LIBRARY_PROCESS ) {
        typ = TB_LIBRARY;
    } else {
        typ = TB_FILE;
    }
    strlwr( fname );
    total = ste_ptr->ns.name_len + fn_len + 3;
    align = total + 1;
    if( Options & OPT_DEBUG ) {
        align++;
    }
    AlignEven( align );
    OutEntryNameInfo( ste_ptr );
    OutByte( total );
    if( ( ProgSw & PS_ERROR ) == 0 ) {
        rtn( ste_ptr );
    }
    OutBytes( &ste_ptr->ns.name, ste_ptr->ns.name_len );
    OutByte( NULLCHAR );
    OutByte( typ );
    OutBytes( fname, fn_len + 1 ); // include NULLCHAR
}


void    OutSPDumInfo( sym_id ste_ptr ) {
//======================================

// Dump the name of a subprogram dummy argument.

    byte        value_offset;

    value_offset = ste_ptr->ns.name_len + 1;
    if( Options & OPT_DEBUG ) {
        ++value_offset;
    }
    AlignEven( value_offset );
    if( Options & OPT_DEBUG ) {
        OutByte( DB_SPECIAL | DB_DUMMY_SP );
    }
    OutName( ste_ptr );
}


void    OutVarName( sym_id ste_ptr ) {
//====================================

// Dump the name of a variable.

    byte        value_offset;

    value_offset = ste_ptr->ns.name_len + 1;
    if( Options & OPT_DEBUG ) {
        ++value_offset;
    }
    AlignEven( value_offset );
    OutVarInfo( ste_ptr );
    OutName( ste_ptr );
}


static  void    OutName( sym_id ste_ptr ) {
//=========================================

// Output a name.
// Length can be recovered by scanning backward from the end
// of the name to the first byte with value less than 0x20.

    OutByte( ste_ptr->ns.name_len );
    OutBytes( &ste_ptr->ns.name, ste_ptr->ns.name_len );
}


void    OutSrcLine() {
//====================

// Note: Line oriented debugger doesn't need the source record number since
// it looks in the listing file.

     OutInt( SrcRecNum );
}
