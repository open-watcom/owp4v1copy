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
// EDITCOMP     : edit and compile for editor
//

#include <string.h>

#include "ftnstd.h"
#include "global.h"
#include "fcgbls.h"
#include "cpopt.h"
#include "errcod.h"
#include "cioconst.h"

extern  int             InitEditor(void);
extern  void            FiniEditor(void);
extern  void            PutRec(char *,int);
extern  void            InitCompile(void);
extern  void            SetInitialOpts(void);
extern  void            Compile(char *);
extern  bool            DoEdit(char *,int);
extern  bool            ResumeEditor(char *,int);
extern  void            MsgBuffer(uint,char *,...);

static  char            *BuffName;

#define MAX_CMD         127


static  void    InvokeEditor() {
//==============================

    bool        first;
    bool        compile;
    char        cmd[MAX_CMD+1];
    char        *cmd_line;

    first = TRUE;
    for(;;) {
        if( first ) {
            first = FALSE;
            strcpy( cmd, SrcName );
            compile = DoEdit( cmd, 0 );
        } else {
            compile = ResumeEditor( cmd, 0 );
        }
        if( !compile ) break;
        BuffName = cmd;
        cmd_line = cmd;
        for(;;) { // skip buffer name
            if( *cmd_line == NULLCHAR ) break;
            if( *cmd_line == ' ' ) {
                *cmd_line = NULLCHAR; // NULLCHAR at end of buffer name
                ++cmd_line;
                break;
            }
            cmd_line++;
        }
        InitCompile();
        SetInitialOpts();
        Compile( cmd_line );
    }
}


void    EditAndCompile() {
//========================

    char        buff[LIST_BUFF_SIZE+1];

    if( InitEditor() != 0x0300 ) {
        MsgBuffer( MS_NO_EDITOR, buff );
        PutRec( buff, strlen( buff ) );
        return;
    }
    InvokeEditor();
    FiniEditor();
}


char    *EdBuffName() {
//=====================

    return( BuffName );
}
