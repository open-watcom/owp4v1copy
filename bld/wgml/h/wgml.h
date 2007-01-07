/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML global include header.
*
****************************************************************************/

#ifndef WGML_H_INCLUDED
#define WGML_H_INCLUDED

#ifndef __STDC_WANT_LIB_EXT1__
    #define __STDC_WANT_LIB_EXT1__  1  /* use safer C library              */
#endif

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <process.h>

#include "gtype.h"


#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode */
#endif

//================= Function Prototypes ========================

/* nwgml.c                              */
extern  void    GBanner( void );
extern  char   *GetFilenameFullPath( char *buff, char const *name, size_t max );
extern  void    MyExit( int );

extern  int     TryOpen( char *prefix, char *separator, char *filename, char *suffix );
extern  int     SearchFileinDirs( char * filename, char * defext, char * altext, DIRSEQ sequence );





extern  void    GMemFree( void *p );
extern  void   *GMemAlloc( size_t size );





/* gdata.c                              */
extern  void    InitGlobalVars( void );
extern  void    GetEnvVars( void );
extern  char   *GMLGetEnv( char *name );


/* goptions.c                           */
extern  void    ProcOptions( char *cmdline );

/* gerror.c                             */
extern  void    OutMsg( char *fmt, ... );

/* -------------------------------- TBD
extern  void    GErr(int,...);
extern  void    GWarn(int, ...);
extern  void    GInfoMsg(int,...);
----------------------------------*/

extern  void    GSuicide(void);


#ifdef  __cplusplus
}   /* End of "C" linkage for C++ */
#endif

#endif  /* WGML_H_INCLUDED */
