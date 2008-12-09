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
* Description:  Declares everything used by wgml to find and open files:
*                   DIRSEQ
*                   ff_teardown()
*                   get_cop_file()
*                   get_env_vars()
*                   GML_get_env()
*                   search_file_in_dirs()
*                   try_file_name
*
****************************************************************************/

#ifndef FINDFILE_H_INCLUDED
#define FINDFILE_H_INCLUDED

#include <stdio.h>

/* Extern variable declaration. */

/* This allows the same declarations to function as definitions.
 * Just #define global before including this file.
 */

#ifndef global
    #define global  extern
#endif

global  char    *   try_file_name;
global  FILE    *   try_fp;

/* Reset so can be reused with other headers. */

#undef global

/* Extern enum declaration. */

/***************************************************************************/
/*  search sequence for gml, opt or layout files                           */
/***************************************************************************/

typedef enum {
    DS_cur_lib_inc_path = 1,            // curdir, gmllib, gmlinc, path
    DS_cur_inc_lib_path,                // curdir, gmlinc, gmllib, path
    DS_cur_lib_path                     // curdir,         gmllib, path
} DIRSEQ;

/* Function declarations. */

#ifdef  __cplusplus
extern "C" {    /* Use "C" linkage when in C++ mode. */
#endif

extern  void        ff_teardown( void );
extern char     *   get_cop_file( char const * in_name );
extern  void        get_env_vars( void );
extern  char    *   GML_get_env( char * name );
extern  int         search_file_in_dirs( char * filename, char * defext, char * altext, DIRSEQ seq );

#ifdef  __cplusplus
}   /* End of "C" linkage for C++. */
#endif

#endif  /* FINDFILE_H_INCLUDED */
