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
// SDCIO      : System dependent compiler I/O routines
//

#include <string.h>

#include "ftnstd.h"
#include "omodes.h"
#include "cpopt.h"
#include "global.h"
#include "fio.h"
#include "posio.h"

typedef struct e_file {                 // editor file
    f_attrs     attrs;                  // file attributes
    unsigned_16 ep;                     // editor handle
} e_file;

extern  file_handle             Openf(char *,int);
extern  int                     FGetRec(file_handle,char *,int);
extern  void                    FPutRec(file_handle,char *,int);
extern  void                    FSeekRec(file_handle,long,int);
extern  void                    FRewind(file_handle);
extern  void                    Closef(file_handle);
extern  bool                    EOFile(file_handle);
extern  int                     Errorf(file_handle);
extern  char                    *ErrorMsg(file_handle);
extern  void                    Scratchf(char *);
extern  void                    *FMemAlloc(int);
extern  void                    FMemFree(void *);
extern  unsigned_16             EOpenf(char *);
extern  void                    EClosef(unsigned_16);
extern  int                     EGetRec(unsigned_16,char *,int);
extern  bool                    EEof(unsigned_16);
extern  void                    InitStd(void);

extern  file_handle             FStdOut;

static  const int __FAR         Modes[] = { RDONLY, WRONLY, APPEND, RDWR };

static  file_attr       CurrAttrs = { REC_TEXT };

file_attr       DskAttr = { REC_TEXT | CARRIAGE_CONTROL };
file_attr       PrtAttr = { REC_TEXT | CARRIAGE_CONTROL };
file_attr       TrmAttr = { REC_TEXT | CARRIAGE_CONTROL };
file_attr       ErrAttr = { REC_TEXT };
char            LstExtn[] = { "lst" };
char            ErrExtn[] = { "err" };
char            ForExtn[] = { "for" };
char            BrowseExtn[] = { "mbr" };
#if _OPSYS == _QNX
char            ObjExtn[] = { "o" };
#else
char            ObjExtn[] = { "obj" };
#endif

#if _EDITOR == _ON
#define EDITOR_BUFFER   (REC_FIXED | REC_TEXT | REC_VARIABLE)
#endif


void    SDInitIO() {
//==================

    InitStd();
}


void    SDInitAttr() {
//====================

    CurrAttrs = REC_TEXT;
}


void    SDSetAttr( file_attr attr ) {
//===================================

    CurrAttrs = attr;
}



void    SDScratch( char *name ) {
//===============================

    Scratchf( name );
}



bool    SDIsInternal( e_file *fp ) {
//==================================

#if _EDITOR == _ON
    return( (fp->attrs & EDITOR_BUFFER) == EDITOR_BUFFER );
#else
    fp = fp;
    return( 0 );
#endif
}


file_handle     SDOpen( char *name, int mode ) {
//==============================================

    return( Openf( name, Modes[ mode ] | CurrAttrs ) );
}


#if _EDITOR == _ON

file_handle     EdOpenf( char *name, int mode ) {
//===============================================

    unsigned_16         ep;
    e_file              *fp;

    mode = mode;
    fp = NULL;
    if( Options & OPT_EDIT ) {
        ep = EOpenf( name );
        if( ep != 0 ) {
            fp = FMemAlloc( sizeof( e_file ) );
            fp->attrs = EDITOR_BUFFER;
            fp->ep = ep;
        }
    }
    return( fp );
}

#endif


void    SDClose( e_file *fp ) {
//=============================

#if _EDITOR == _ON
    if( (fp->attrs & EDITOR_BUFFER) == EDITOR_BUFFER ) {
        EClosef( fp->ep );
        FMemFree( fp );
    } else {
        Closef( fp );
    }
#else
    Closef( fp );
#endif
}


uint    SDRead( e_file *fp, byte *buff, uint len ) {
//==================================================

#if _EDITOR == _ON
    if( (fp->attrs & EDITOR_BUFFER) == EDITOR_BUFFER ) {
        return( EGetRec( fp->ep, buff, len ) );
    }
#endif
    return( FGetRec( fp, buff, len ) );
}


void    SDWrite( file_handle fp, byte *buff, int len ) {
//======================================================

    FPutRec( fp, buff, len );
}


void    SDSeek( file_handle fp, int rec_num, int rec_size ) {
//===========================================================

    FSeekRec( fp, rec_num, rec_size );
}


void    SDRewind( file_handle fp ) {
//==================================

    FRewind( fp );
}


bool    SDEof( e_file *fp ) {
//===========================

#if _EDITOR == _ON
    if( (fp->attrs & EDITOR_BUFFER) == EDITOR_BUFFER ) {
        return( EEof( fp->ep ) );
    }
#endif
    return( EOFile( fp ) );
}


bool    SDError( e_file *fp, char *buff ) {
//=========================================

    int         err;

#if _EDITOR == _ON
    if( ( fp != NULL ) &&
        ((fp->attrs & EDITOR_BUFFER) == EDITOR_BUFFER) ) return( FALSE );
#endif
    err = Errorf( fp );
    if( err != IO_OK ) {
        strcpy( buff, ErrorMsg( fp ) );
    }
    return( err != IO_OK );
}
