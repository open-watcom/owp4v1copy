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
// LIBSUPP      : Library support
//

#include <stdlib.h>
#include <string.h>

#include "linkdata.h"
#include "sdfile.h"
#include "omodes.h"
#include "io.h"

extern  void            BSeek(file_handle,unsigned_32);
extern  int             BRead(file_handle,char *,int);
extern  void            SDClose(file_handle);
extern  file_handle     SDOpen(char *,int);
extern  int             SDRead(file_handle,char *,int);
extern  bool            SDError(file_handle,char *);
extern  bool            SDEof(file_handle);
extern  char            *SDSrcExtn(char *);
extern  int             MakeName(char *,char *,char *);
extern  int             CopyMaxStr(char *,char *,int);
extern  void            *FMemAlloc(int);
extern  void            FMemFree(void *);
extern  obj_entry       *SearchLib(char *,char *);
extern  void            AddSpecNames(void);
extern  bool            ObjPass2(void);
extern  void            FreeObjStruct(void);
extern  void            ProcObjFile(lib_handle);
extern  lib_handle      OpenObj(char *);
extern  lib_handle      Srch4Undef(void);
extern  char            *SkipBlanks(char *);
extern  void            Error(int,...);
extern  void            SrcInclude(char *);
extern  lib_handle      SearchUFNS(char *);

extern  char            ObjExtn[];
extern  char            ForExtn[];
extern  byte            LibStatus;

static  char            LibEnvVar[] = { "LIBRARY" };
static  char            IncEnvVar[] = { "FINCLUDE" };

#define MAX_ENV_STRING  128


void    LibInit() {
//=================

// Library support initialization.

}


bool    LibFini( bool tbreak ) {
//==============================

// Library support finalization.

    return( tbreak );
}


static  bool    ExtractName( char **lib ) {
//=========================================

// Extract a file name from "LIBRARY" specification.

    char        *ptr;
    bool        last;

    ptr = *lib;
    for(;;) {
        if( *ptr == NULLCHAR ) {
            last = TRUE;
            break;
        }
        if( *ptr == ';' ) {
            last = FALSE;
            break;
        }
       ptr++;
    }
    *lib = ptr;
    for(;;) {
        ptr--;
        if( *ptr != ' ' ) break;
    }
    ptr++;
    *ptr = NULLCHAR;
    return( last );
}


static  file_handle     FindSrcFile( char *fname ) {
//==================================================

// Find a source file.

    file_handle lp;

    MakeName( fname, SDSrcExtn( fname ), fname );
    lp = SDOpen( fname, READ_FILE );
    if( lp != NULL ) {
        SrcInclude( fname );
    }
    return( lp );
}


lib_handle      SearchDir( char *path, char *name, bool lib_search ) {
//====================================================================

// Search a directory for an object file, a module in a library, or a source
// file.  Libraries searched are only those that have appeared in a "library"
// pragma.

    char        fn[MAX_FILE+1];

    lib_search = lib_search;
    fn[ Combine( path, name, fn, MAX_FILE ) ] = NULLCHAR;
    return( FindSrcFile( fn ) );
}


static lib_handle       SearchPath( const char *path, char *name, bool libsrch){
//==============================================================================

    bool        last;
    char        *libname;
    lib_handle  lp = NULL;
    char        lib_str[MAX_ENV_STRING+1];
    char        *ptr;


    if( path == NULL ) return( NULL );
    strcpy( lib_str, path );
    ptr = lib_str;

    for(;;) {
        libname = ptr;
        last = ExtractName( &ptr );
        lp = SearchDir( libname, name, libsrch );
        if( lp != NULL ) break;
        if( !last ) {
            ++ptr; // skip the ';'
        } else {
            break;
        }
    }
    return( lp );
}

static  lib_handle      Search( char *name, bool lib_search ) {
//=============================================================

// Search for a library member.

    lib_handle  lp;

    lp = NULL;
    if( lib_search ) {
        lp = SearchPath( getenv( LibEnvVar ), name, lib_search );
        if( lp == NULL ) {
            lp = SearchUFNS( name );
        }
    } else {
        lp = SearchPath( IncludePath, name, lib_search );
        if( lp ) return( lp );
        lp = SearchPath( getenv( IncEnvVar ), name, lib_search );
    }
    return( lp );
}


lib_handle      LibSearch( char *name ) {
//=======================================

// Search for a library member.

    return( Search( name, TRUE ) );
}


lib_handle      IncSearch( char *name ) {
//=======================================

// Search for a source to be included.

    return( Search( name, FALSE ) );
}


int     LibRead( lib_handle lp ) {
//================================

// Read a record from a library member (source only).

    return( SDRead( lp, SrcBuff, SRCLEN ) );
}


bool    LibEof( lib_handle lp ) {
//===============================

// Check for EOF on library read (source only).

    return( SDEof( lp ) );
}


bool    LibError( lib_handle lp, char *buff ) {
//=============================================

// Check for error on library read (source only).

    return( SDError( lp, buff ) );
}


void    IncMemClose( lib_handle lp ) {
//====================================

// Close a library member that was included (source only).

    SDClose( lp );
}


static  int     Combine( char *path, char *name, char *buff, int buff_len ) {
//===========================================================================

// Combine the path and filename.

    int         len;

    len = CopyMaxStr( path, buff, buff_len - sizeof( char ) );
    if( buff[ len - 1 ] != '\\' ) {
        buff[ len ] = '\\';
        ++len;
        buff[ len ] = NULLCHAR;
    }
    len += CopyMaxStr( name, buff + len, buff_len - len );
    return( len );
}
