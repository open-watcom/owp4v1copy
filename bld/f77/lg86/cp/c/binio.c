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
// BINIO        : binary i/o
//

#include "ftnstd.h"
#include "fio.h"

extern  a_file                  *Openf(char *,int);
extern  void                    Closef(a_file *);
extern  void                    FSeekAbs(a_file *,unsigned_32);
extern  unsigned_32             FGetFilePos(a_file *);
extern  int                     FGetRec(a_file *,char *,int);
extern  bool                    EOFile(a_file *);
extern  bool                    Errorf(a_file *);
extern  char                    *ErrorMsg(a_file *);

static  file_attr       BinFileAttrs = { RDONLY };


file_handle     BOpen( char *name ) {
//===================================

    return( Openf( name, BinFileAttrs ) );
}


void    BClose( file_handle f ) {
//===============================

    Closef( f );
}


void    BSeek( file_handle f, unsigned_32 offset ) {
//==================================================

    FSeekAbs( f, offset );
}


unsigned_32     BFilePos( file_handle f ) {
//=================================================

    return( FGetFilePos( f ) );
}


int     BRead( file_handle f, char *buff, int len ) {
//===================================================

    return( FGetRec( f, buff, len ) );
}


bool    BErrorf( file_handle f ) {
//================================

    return( Errorf( f ) );
}


bool    BEof( file_handle f ) {
//=============================

    return( EOFile( f ) );
}


char    *BErrorMsg( file_handle f ) {
//===================================

    return( ErrorMsg( f ) );
}
