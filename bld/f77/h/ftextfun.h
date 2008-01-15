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
* Description:  Declaration of external functions for f77 compiler & lib
*
****************************************************************************/

#ifndef _F77_EXTERNAL_FUNCS_H
#define _F77_EXTERNAL_FUNCS_H 1

#include "fio.h"

extern  int             ExtractText( char *, int );

extern  void            IOOk( b_file * );
extern  int             SysSeek( b_file *, long int, int );
extern  uint            SysRead( b_file *, char *, uint );
extern  long int        CurrFileOffset( b_file * );
extern  void            FSetErr( int, b_file * );
extern  void            FSetSysErr( b_file * );
extern  void            FSetEof( b_file * );
extern  void            FSetTrunc( b_file * );
extern  long int        FGetFilePos( b_file * );
extern  int             FlushBuffer( b_file * );
extern  int             FSetCC( b_file *, char, char ** );
extern  int             SysWrite( b_file *, char *, uint );
extern  int             FlushBuffer( b_file * );
extern  bool            __DevicesCC( void );
extern  uint            readbytes( b_file *, char *, uint );
extern  void            ChopFile( b_file * );

#endif
