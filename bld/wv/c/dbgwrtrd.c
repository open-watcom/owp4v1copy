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
* Description:  Non-blocking debug window
*
****************************************************************************/


#include "dbgdefn.h"
#include "dbginfo.h"
#include "dbgwind.h"
#include "dbgreg.h"
#include <string.h>
#include <stdlib.h>

extern bool             RemoteGetRunThreadInfo( int row, char *infotype, int *width, char *header, int maxsize );

extern bool             IsThdCurr( thread_state *thd );
extern void             MakeThdCurr( thread_state * );
extern void             RemoteThdName( dtid_t, char * );
extern void             SetUnderLine( a_window *, wnd_line_piece * );
extern void             DbgUpdate( update_list );

extern thread_state     *HeadThd;
extern char             *TxtBuff;

#include "menudef.h"
static gui_menu_struct TrdMenu[] = {
    #include "menurtrd.h"
};

#define TITLE_SIZE      2

#define MAX_PIECE_COUNT     4
#define MAX_HEADER_SIZE     80

static int      PieceCount = 0;
static char     Indents[MAX_PIECE_COUNT];
static char     InfoType[MAX_PIECE_COUNT];
static char     HeaderArr[MAX_PIECE_COUNT][MAX_HEADER_SIZE + 1];

void InitRunThreadWnd()
{
    int     Width;
    bool    ok;

    PieceCount = 0;
    Indents[0] = 0;
    
    for(i  = 0; i < MAX_PIECE_COUNT; i++ ) {
        ok = RemoteGetRunThreadInfo( i, &Info[PieceCount], &Width, HeaderArr[PieceCount], MAX_HEADER_SIZE );
        if( ok ) {
            Indents[PieceCount + 1] = Indents[PieceCount] + (char)Width;
            PieceCount++;
        } else
            break;
    }
}
