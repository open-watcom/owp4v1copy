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
// MAINLINE  : WATFOR-77 main line
//

#include "ftnstd.h"
#include "global.h"
#include "cpopt.h"

extern  int             CompMain(char *);
extern  void            FMemInit(void);
extern  void            InitCompMain(void);
extern  void            FiniCompMain(void);
extern  void            ShowUsage(void);
extern  void            ProcOpts(char **);
extern  void            InitPredefinedMacros(void);
extern  bool            MainCmdLine(char **,char**,char **,char *);
extern  void            SaveInitialOpts(void);
#if _EDITOR == _ON
extern  void            EditAndCompile();
#endif
#if _8087 == _ON
extern  bool            Chk8087(void);
#endif

bool                    CmdLineOption;


int     MainLine( char *cmd ) {
//=============================

// WATFOR-77 main line.

    int         ret_code;
    char        *opts[MAX_OPTIONS+1];

    ret_code = 0;
#if _8087 == _ON
    if( !Chk8087() ) return( ret_code );
#endif
    InitCompMain();
    if( MainCmdLine( &SrcName, &CmdPtr, &opts, cmd ) ) {
        CmdLineOption = TRUE; // "/EDIT" option allowed only on command line
        ProcOpts( &opts );
        InitPredefinedMacros();
        CmdLineOption = FALSE;
        SaveInitialOpts();
#if _EDITOR == _ON
        if( Options & OPT_EDIT ) {
            EditAndCompile();
        } else {
#endif
            ret_code = CompMain( cmd );
#if _EDITOR == _ON
        }
#endif
    } else {
        ShowUsage();
    }
    FiniCompMain();
    return( ret_code );
}
