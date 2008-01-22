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

#include <stdarg.h>

#include "fio.h"
#include "errrtns.h"

extern bool            __Add( intstar4 *arg1, intstar4 *arg2 );
extern bool            __Sub( intstar4 *arg1, intstar4 *arg2 );

extern bool            AddIOFlo(intstar4 *,intstar4 *);
extern bool            __DevicesCC( void );
extern void            BldErrCode( uint, char * );
extern uint            CarrotType( unsigned int );
extern void            ChopFile( b_file * );
extern long int        CurrFileOffset( b_file * );
extern int             ExtractText( char *, int );
extern void            FEmByte(int);
extern void            FEmChar(char PGM *);
extern void            FEmCode(int);
extern void            FEmNum(int);
extern long int        FGetFilePos( b_file * );
extern void            FSetEof( b_file * );
extern void            FSetErr( int, b_file * );
extern void            FSetSysErr( b_file * );
extern void            FSetTrunc( b_file * );
extern int             FlushBuffer( b_file * );
extern int             FSetCC( b_file *, char, char ** );
extern void            IOOk( b_file * );
extern void            MsgFormat( char *, char *, ... );
extern void            MsgJustErr( uint, ... );
extern void            MsgPrintErr( uint, ... );
extern bool            MulIOFlo(intstar4 *,intstar4 *);
extern uint            readbytes( b_file *, char *, uint );
extern void            R_FError(int);
extern void            R_FExtension(int);
extern bool            RecEOS( void );
extern char            *STGetName( sym_id, char * );
extern void            Substitute( char *, char *, va_list );
extern uint            SysRead( b_file *, char *, uint );
extern int             SysSeek( b_file *, long int, int );
extern int             SysWrite( b_file *, char *, uint );
extern uint            writebytes( b_file *, char *, uint );

#endif
