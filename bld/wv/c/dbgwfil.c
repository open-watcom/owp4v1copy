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


#include <limits.h>
#include <string.h>
#include "dbgdefn.h"
#include "dbgio.h"
#include <stdio.h> // Should follow "dbgio.h" as that has SEEK_CUR as an enum value
#include "dbgwind.h"
#include "dbgadget.h"
#include "dbginfo.h"
#include "dbgtoken.h"
#include "dbgitem.h"
#include "dbgbreak.h"
#include "dbgstk.h"

extern bool             ScanItem( bool, char **, unsigned int * );
extern void             ReqEOC( void );
extern bool             ScanSelectedExpr( char * );
extern void             BreakOnSelected( char *item );

extern char             *StrCopy( const char *from, char * to );
extern a_window         *WndFileInspect( char *file, bool binary );
extern void             *OpenSrcFile( cue_handle * );
extern int              FReadLine( void *, int, int, char*, int );
extern void             FDoneSource( void * );
extern char             *ScanPos( void );
extern unsigned int     ScanCmd( const char * );
extern void             Scan( void );
extern char             *ReScan( char * );
extern unsigned long    FSize( void* );
extern unsigned long    FLastOffset( void* );
extern brk              *FindBreakByLine( mod_handle, cue_file_id, unsigned );
extern void             WndFuncInspect( mod_handle mod );
extern void             WndInspect( char* );
extern int              DlgSearch( a_window*, void* );
extern bool             DlgLongExpr( char*title, long*value );
extern void             GoToAddr( address addr );
extern void             ToggleBreak( address );
extern void             SetCodeDot( address );
extern address          GetCodeDot( void );
extern void             WndVarInspect( char * );
extern void             AsmMoveDot( a_window *, address );
extern void             AsmJoinSrc( a_window *, a_window * );
extern void             AsmFreeSrc( a_window * );
extern void             AsmNewSrcNotify( a_window *, mod_handle, bool track );
extern void             StepIntoFunction( char * );
extern int              FCurrLine( struct browser *hndl );
extern int              FileIsRemote( struct browser *hndl );
extern unsigned         NewCurrRadix( unsigned int );
extern bool             DbgWndSearch( a_window *, bool, int );
extern char             *DupStr( char* );
extern char             *Format( char *, char *, ... );
extern bool             DlgCodeAddr( char *title, address *value );
extern void             WndSrcInspect( address addr );
extern bool             DlgModName( char *title, mod_handle *mod );
extern void             WndModInspect( mod_handle mod );
extern a_window         *AsmWndFind( a_window* asm, address addr, bool track );
extern a_window         *DoWndSrcOpen( cue_handle *, bool track );
extern char             *ModImageName( mod_handle handle );
extern char             *FGetName( void *viewhndl );
extern a_window         *WndNewSrcInspect( address addr );
extern int              AddrComp( address a, address b );
extern void             GoHome( void );
extern void             DbgUpdate( update_list );

extern char             *TxtBuff;
extern tokens           CurrToken;
extern debug_level      ActiveWindowLevel;
extern address          NilAddr;
extern mod_handle       ContextMod;
extern void             *SrchHistory;

#define MAX_LINE_LEN    255 // must not wrap a gui_ord

#include "menudef.h"

static gui_menu_struct FileShowMenu[] = {
    #define pick( a, b, c, d, e, f, g ) MENU_ITEM( a, b )
    #include "mfilshow.h"
};

static gui_menu_struct FileMenu[] = {
    #include "menufile.h"
};

typedef struct {
    void                *viewhndl;
    unsigned long       size;
    unsigned            active;
    mod_handle          mod;
    cue_file_id         file_id;
    wnd_row             rows;
    wnd_row             rows_offset;
    address             dotaddr;
    char                *name;
    a_window            *asm;
    unsigned            eof;
    unsigned            track : 1;
    unsigned            erase : 1;
    unsigned            toggled_break : 1;
} file_window;
#define WndFile( wnd ) ( (file_window *) WndExtra( wnd ) )

#define NOT_ACTIVE ( (unsigned) -1 )
enum {
    PIECE_BREAK,
    PIECE_SOURCE
};

extern void SrcJoinAsm( const a_window *wnd, a_window *asm );

void SrcNewAsmNotify( a_window *asm, mod_handle mod, bool track )
{
    file_window const  *file;
    a_window           *wnd;

    for( wnd = WndNext( NULL ); wnd != NULL; wnd = WndNext( wnd ) ) {
        if( WndClass( wnd ) != WND_SOURCE )
            continue;
        file = WndFile( wnd );
        if( track != file->track )
            continue;
        if( mod != file->mod )
            continue;
        if( file->asm != NULL )
            continue;
        AsmJoinSrc( asm, wnd );
        SrcJoinAsm( wnd, asm );
        break;
    }
}


void SrcJoinAsm( const a_window *wnd, a_window *asm )
{
    WndFile( wnd )->asm = asm;
}

void SrcFreeAsm( const a_window *wnd )
{
    if( wnd != NULL )
        WndFile( wnd )->asm = NULL;
}

#ifdef DEADCODE
bool SrcIsTracking( a_window *wnd )
{
    return( WndFile( wnd )->track );
}
#endif

static address GetRowAddr( const file_window *file, wnd_row row, bool exact )
{
    DIPHDL( cue, const ch );

    if( file->mod == NO_MOD || row < 0 )
        return( NilAddr );
    switch( LineCue( file->mod, file->file_id, (unsigned long) row + 1, 0, ch ) ) {
    case SR_NONE:
        return( NilAddr );
    case SR_CLOSEST:
        if( exact )
            return( NilAddr );
        break;
    }
    return( CueAddr( ch ) );
}


static void Centre( a_window *wnd, wnd_row line )
{
    WndZapped( wnd );
    WndScroll( wnd, line - ( ( WndRows( wnd ) / 2 ) + WndTop( wnd ) ) );
}


static void GotoLine( a_window *wnd )
{
    long                line;
    wnd_row             row;
    int                 piece;
    unsigned const      old = NewCurrRadix( 10 );

    WndGetCurrent( wnd, &row, &piece );
    line = 1 + ( ( row < 0 || row == WND_NO_ROW ) ? WndTop( wnd ) : row );
    if( DlgLongExpr( LIT( New_Line ), &line ) ) {
        WndDirtyCurr( wnd );
        Centre( wnd, --line );
        WndNewCurrent( wnd, line, PIECE_SOURCE );
    }
    NewCurrRadix( old );
}


static  WNDMENU FileMenuItem;
static void     FileMenuItem( a_window *wnd, unsigned id, int row, int piece )
{
    file_window const * const   file = WndFile( wnd );
    address                     addr = GetRowAddr( file, row, (int) ( id != MENU_FILE_ASSEMBLY ) );

    piece = piece;
    switch( id ) {
    case MENU_INITIALIZE:
        {
            bool const          has_addr = !IS_NIL_ADDR( addr );
            bool                has_popitem = ( *WndPopItem( wnd ) != '\0' );

            if( has_popitem && !ScanSelectedExpr( WndPopItem( wnd ) ) ) {
                has_popitem = FALSE;
            }
            WndMenuEnable( wnd, MENU_FILE_SHOW, TRUE );
            WndMenuEnable( wnd, MENU_FILE_SHOW_ADDRESS, TRUE );
            WndMenuEnable( wnd, MENU_FILE_SHOW_MODULE, TRUE );
            WndMenuEnable( wnd, MENU_FILE_FUNCTIONS, (int) ( file->mod != NO_MOD ) );
            WndMenuEnable( wnd, MENU_FILE_HOME, TRUE );
            addr = GetRowAddr( file, row, FALSE );
            WndMenuEnable( wnd, MENU_FILE_ASSEMBLY, (int) ( !IS_NIL_ADDR( addr ) ) );
            WndMenuEnable( wnd, MENU_FILE_WATCH, has_popitem );
            WndMenuEnable( wnd, MENU_FILE_INSPECT, has_popitem );
            WndMenuEnable( wnd, MENU_FILE_STEP_INTO, (int) ( file->mod != NO_MOD && has_popitem ) );
            WndMenuEnable( wnd, MENU_FILE_BREAK, has_popitem );
            WndMenuEnable( wnd, MENU_FILE_RUN, has_addr );
        }
        break;
    case MENU_FILE_RUN:
        GoToAddr( addr );
        break;
    case MENU_FILE_BREAK:
        BreakOnSelected( WndPopItem( wnd ) );
        break;
    case MENU_FILE_HOME:
        GoHome();
        break;
    case MENU_FILE_SHOW_MODULE:
        {
            mod_handle          mod = file->mod;

            if( DlgModName( LIT( New_Module ), &mod ) ) {
                WndModInspect( mod );
            }
        }
        break;
    case MENU_FILE_SHOW_ADDRESS:
        if( DlgCodeAddr( LIT( New_Addr ), &addr ) ) {
            WndSrcInspect( addr );
        }
        break;
    case MENU_FILE_STEP_INTO:
        StepIntoFunction( WndPopItem( wnd ) );
        break;
    case MENU_FILE_INSPECT:
        WndInspect( WndPopItem( wnd ) );
        break;
    case MENU_FILE_WATCH:
        WndVarInspect( WndPopItem( wnd ) );
        break;
    case MENU_FILE_SEARCH:
        WndSaveToHistory( SrchHistory, WndPopItem( wnd ) );
        DbgWndSearch( wnd, FALSE, DlgSearch( wnd, SrchHistory ) );
        break;
    case MENU_FILE_ASSEMBLY:
        AsmWndFind( file->asm, addr, file->track );
        break;
    case MENU_FILE_LINE:
        GotoLine( wnd );
        break;
    case MENU_FILE_FUNCTIONS:
        WndFuncInspect( file->mod );
        break;
    }
}


static void FilePosInit( a_window *wnd )
{
    file_window * const file = WndFile( wnd );

    file->size = ( file->viewhndl == NULL ) ? 0 : FSize( file->viewhndl );
    WndSetThumbPercent( wnd, 0 );
}


static void FilePos( a_window *wnd, int pos )
{
    wnd_row             range;
    file_window * const file = WndFile( wnd );

    if( pos < 0 )
        pos = 0;
    if( file->viewhndl == NULL ) {
        if( (unsigned) ( pos + WndRows( wnd ) ) <= file->eof )
            WndSetTop( wnd, pos );
        return;
    }
    if( FReadLine( file->viewhndl, pos + WndRows( wnd ), 0, TxtBuff, TXT_LEN ) < 0 ) {
        pos = FCurrLine( file->viewhndl ) - ( WndRows( wnd ) + 1 );
        if( pos < 0 ) {
            pos = 0;
        }
    }
    WndSetTop( wnd, pos );
    if( pos >= file->rows ) {
        file->rows = pos + 1;
        file->rows_offset = (wnd_row) FLastOffset( file->viewhndl );
    }
    if( file->rows == 0 )
        file->rows = 1;
    range = (wnd_row) file->size * ( ( file->rows_offset == 0 ) ? 1 : ( file->rows ) / file->rows_offset );
    WndSetVScrollRange( wnd, range );
    WndSetThumbPos( wnd, pos );
}


static WNDSCROLL FileScroll;
static int       FileScroll( a_window *wnd, int lines )
{
    int const   old_top = WndTop( wnd );

    FilePos( wnd, old_top + lines );
    return( WndTop( wnd ) - old_top );
}


static WNDMODIFY FileModify;
static void      FileModify( a_window *wnd, int row, int piece )
{
    if( piece == PIECE_BREAK ) {
        file_window * const     file = WndFile( wnd );
        address const           addr = GetRowAddr( file, row, TRUE );

        if( !IS_NIL_ADDR( addr ) ) {
            file->toggled_break = ( ( WndFlags & UP_BREAK_CHANGE ) == 0 );
            ToggleBreak( addr );
            WndRowDirty( wnd, row );
        }
    } else {
        WndFirstMenuItem( wnd, row, piece );
    }
}


static void FileSetDotAddr( const a_window *wnd, address addr );

static WNDNOTIFY FileNotify;
static void      FileNotify( a_window *wnd, wnd_row row, int piece )
{
    file_window const * const   file = WndFile( wnd );
    address const               addr = GetRowAddr( file, row, FALSE );

    piece = piece;
    if( !IS_NIL_ADDR( addr ) )
        FileSetDotAddr( wnd, addr );
}


bool FileOpenGadget( a_window *wnd, wnd_line_piece *line, mod_handle mod )
{
    a_window    *curr;

    for( curr = WndNext( NULL ); curr != NULL; curr = WndNext( curr ) ) {
        if( WndClass( curr ) == WND_SOURCE && mod == WndFile( curr )->mod ) {
            if( line != NULL )
                SetGadgetLine( wnd, line, GADGET_OPEN_SOURCE );
            return( TRUE );
        }
    }
    if( line != NULL )
        SetGadgetLine( wnd, line, GADGET_CLOSED_SOURCE );
    return( FALSE );
}


void FileBreakGadget( a_window *wnd, wnd_line_piece *line, bool curr, const brk *bp )
{
    wnd_gadget_type type;

    if( curr ) {
        type = ( bp == NULL          ) ? GADGET_CURRLINE
	:      ( bp->status.b.active ) ? GADGET_CURRBREAK
	:                                GADGET_CURRDIMBREAK;
    } else {
        type = ( bp == NULL          ) ? GADGET_NOBREAK
	:      ( bp->status.b.active ) ? GADGET_BREAK
	:                                GADGET_DIMBREAK;
    }
    SetGadgetLine( wnd, line, type );
}


static WNDGETLINE FileGetLine;
static bool       FileGetLine( a_window *wnd, int row, int piece, wnd_line_piece *line )
{
    file_window * const file = WndFile( wnd );
    bool const          curr = ( (unsigned) row == file->active && ContextMod == file->mod );

    line->text = LIT( Empty );
    if( file->viewhndl == NULL && ModHasInfo( file->mod, HK_CUE ) != DS_OK ) {
        return( FALSE );
    }
    switch( piece ) {
    case PIECE_BREAK:
        {
            line->tabstop = FALSE;
            if( (unsigned) row >= file->eof )
                return( FALSE );
            if( file->mod == NO_MOD )
                return( TRUE );
	    {
	        // Too expensive to do in a search
                address const   addr = ( WndDoingSearch ) ? NilAddr : GetRowAddr( file, row, TRUE );

                if( !IS_NIL_ADDR( addr ) ) {
                    brk const * const       bp = FindBreakByLine( (mod_handle) file->mod, file->file_id, (unsigned) ( row + 1 ) );
                    FileBreakGadget( wnd, line, curr, bp );
                }
            }
        }
        return( TRUE );
    case PIECE_SOURCE:
        {
            int                 len;
            DIPHDL( cue,  const ch );

            line->text = TxtBuff;
            line->extent = WND_MAX_EXTEND;
            if( curr )
                line->attr = WND_STANDOUT;
            if( file->mod != NO_MOD ) {
                line->indent = MaxGadgetLength;
            }
            if( file->viewhndl == NULL ) {
                Format( TxtBuff, LIT( No_Source_Line ), row + 1 );
                if( LineCue( file->mod, file->file_id, 0, 0, ch ) != SR_NONE ) {
                    if( ( CueAdjust( ch, -1, ch ) & DS_ERR ) ) {
                        file->eof = CueLine( ch );
                    }
                }
                return( TRUE );
            }
            len = FReadLine( file->viewhndl, row + 1, 0, TxtBuff, MAX_LINE_LEN );
            if( len < 0 ) {
                file->eof = (unsigned) row;
                return( FALSE );
            }
            if( len >= MAX_LINE_LEN ) {
                StrCopy( " ...", TxtBuff + MAX_LINE_LEN );
            } else {
                TxtBuff[len] = '\0';
            }
            if( row >= file->rows ) {
                file->rows = row + 1;
                file->rows_offset = (wnd_row) FLastOffset( file->viewhndl );
            }
        }
        return( TRUE );
    default:
        return( FALSE );
    }
}


static void SeekToTheEnd( file_window *file )
{
    if( file->viewhndl != NULL && !FileIsRemote( file->viewhndl ) ) {
        FReadLine( file->viewhndl, INT_MAX, 0, TxtBuff, TXT_LEN );
        file->rows = FCurrLine( file->viewhndl ) - 1;
        file->rows_offset = (wnd_row) FLastOffset( file->viewhndl );
    } else {
        file->rows = 0;
        file->rows_offset = 0;
    }
}


static unsigned ActiveLine( void )
{
    DIPHDL( cue, const ch );

    if( DeAliasAddrCue( ContextMod, Context.execution, ch ) == SR_NONE )
        return( 0 );
    return( CueLine( ch ) - 1 );
}


#ifdef DEADCODE
void FileReset( a_window *wnd )
{
    file_window *file = WndFile( wnd );

    file->active = NOT_ACTIVE;
}
#endif


static void FileSetTitle( a_window *wnd, mod_handle mod, unsigned active )
{
    char const          * const image_name = ModImageName( mod );
    file_window const * const   file = WndFile( wnd );
    char                       *p = TxtBuff;

    if( file->track ) {
        p = StrCopy( LIT( WindowSource ), p );
        p = StrCopy( ": ", p );
    }
    if( file->viewhndl != NULL ) {
        char const * const fullname = FGetName( file->viewhndl );

        p += sprintf( p, "%s(%u)", fullname, active + 1 );
    } else if( file->mod != NO_MOD ) {
        p += ModName( file->mod, p, TXT_LEN );
    }
    if( image_name[0] != '\0' ) {
        char const * const trimname = SkipPathInfo( image_name, OP_REMOTE );

        sprintf( p, " (%s)", trimname );
    }
    WndSetTitle( wnd, TxtBuff );
}

static void FileTrack( a_window *wnd, cue_handle *ch )
{
    unsigned            active;
    unsigned            old_active;
    unsigned            end_line;
    int                 slack;
    file_window * const file = WndFile( wnd );
    mod_handle          mod = NO_MOD;
    cue_file_id         id = 0;

    if( ch != NULL ) {
        mod = CueMod( ch );
        id = CueFileId( ch );
    }
    if( file->viewhndl == NULL || file->mod != mod || file->file_id != id ) {
        if( file->viewhndl != NULL )
            FDoneSource( file->viewhndl );
        file->mod = mod;
        file->file_id = id;
        FileSetDotAddr( wnd, GetCodeDot() );
        file->viewhndl = ( file->mod == NO_MOD ) ? NULL : OpenSrcFile( ch );
        SeekToTheEnd( file );
        file->eof = UINT_MAX;
        WndZapped( wnd );
        FilePosInit( wnd );
        file->active = NOT_ACTIVE;
        FilePos( wnd, 0 );
        DbgUpdate( UP_OPEN_CHANGE );
    }
    if( ( active = ActiveLine() ) != file->active ) {
        wnd_row         curr_row;
        int             curr_piece;

        FileSetDotAddr( wnd, GetCodeDot() );
        WndGetCurrent( wnd, &curr_row, &curr_piece );
        WndNoCurrent( wnd );
        if( curr_row != WND_NO_ROW )
            WndRowDirty( wnd, curr_row );
    }
    old_active = file->active;
    file->active = NOT_ACTIVE;
    if( ( slack = WndRows( wnd ) / 4 ) > 2 )
        slack = 2;
    end_line = (unsigned) ( WndTop( wnd ) + WndRows( wnd ) - 1 );
    if( old_active == NOT_ACTIVE || active > end_line ) {
        WndZapped( wnd );
        WndScroll( wnd, (int) ( active - (unsigned) ( slack + WndTop( wnd ) ) ) );
    } else if( active > end_line - (unsigned) slack ) {
        WndRowDirtyImmed( wnd, (wnd_row) ( old_active ) );
        WndScroll( wnd, WndRows( wnd ) - 2 * slack );
    } else if( active < (unsigned) WndTop( wnd ) ) {
        WndRowDirtyImmed( wnd, (wnd_row) old_active );
        WndScroll( wnd, (int) ( active - (unsigned) ( WndTop( wnd ) + slack ) ) );
    } else {
        WndRowDirty( wnd, (wnd_row) old_active );
    }
    WndNewCurrent( wnd, (wnd_row) active, PIECE_SOURCE );
    WndRowDirty( wnd, (wnd_row) active );
    file->active = active;
    FileSetTitle( wnd, mod, active );
}



static void FileSetDotAddr( const a_window *wnd, address addr )
{
    file_window * const file = WndFile( wnd );

    if( AddrComp( file->dotaddr, addr ) == 0 )
        return;
    file->dotaddr = addr;
    if( !IS_NIL_ADDR( addr ) && wnd == WndFindActive() ) {
        AsmMoveDot( file->asm, addr );
        SetCodeDot( addr );
    }
}


bool SrcMoveDot( a_window *wnd, address addr )
{
    unsigned            line;
    mod_handle          mod;
    file_window const  *file;
    DIPHDL( cue, const  ch );

    if( wnd == NULL )
        return( FALSE );
    file = WndFile( wnd );
    if( file->mod == NO_MOD && !file->track )
        return( FALSE );
    if( IS_NIL_ADDR( addr ) ) {
        WndScrollAbs( wnd, 0 );
        return( FALSE );
    }
    DeAliasAddrMod( addr, &mod );
    if( DeAliasAddrCue( mod, addr, ch ) == SR_NONE ) {
        if( LineCue( mod, 0, 0, 0, ch ) == SR_NONE )
            return( FALSE );
    }
    line = CueLine( ch ) - 1;
    if( mod != file->mod || CueFileId( ch ) != file->file_id ) {
        if( !file->track )
            return( FALSE );
        FileTrack( wnd, ch );
    }
    WndScrollAbs( wnd, (int) line );
    WndMoveCurrent( wnd, (wnd_row) line, PIECE_SOURCE );
    FileSetDotAddr( wnd, addr );
    FileSetTitle( wnd, CueMod( ch ), line );
    return( TRUE );
}

a_window *SrcWndFind( a_window *wnd, address addr, bool track )
{
    a_window    *new;
    mod_handle  mod;
    DIPHDL( cue, ch );

    if( wnd == NULL ) {
        if( DeAliasAddrMod( addr, &mod ) == SR_NONE ) {
            new = NULL;
        } else {
            if( DeAliasAddrCue( mod, addr, ch ) == SR_NONE ) {
                ch = NULL;
            }
            new = DoWndSrcOpen( ch, track );
        }
    } else {
        WndRestoreToFront( wnd );
        new = wnd;
    }
    SrcMoveDot( new, addr );
    return( new );
}

bool SrcHasFileOpen( const a_window *wnd )
{
    return( WndFile( wnd )->viewhndl != NULL );
}

static void FileActive( a_window *wnd, mod_handle mod )
{
    file_window * const file = WndFile( wnd );

    if( file->mod != mod ) {
        if( file->active != NOT_ACTIVE ) {
            file->active = NOT_ACTIVE;
            WndZapped( wnd );
        }
    } else {
        unsigned const  line = ActiveLine();

        if( line != file->active ) {
            WndRowDirty( wnd, (wnd_row) file->active );
            file->active = line;
            WndRowDirty( wnd, (wnd_row) file->active );
        }
    }
    if( file->mod != NO_MOD && ( WndFlags & UP_BREAK_CHANGE ) ) {
        WndNoSelect( wnd );
        WndRepaint( wnd );
    }
    if( file->active != NOT_ACTIVE ) {
        WndNewCurrent( wnd, (wnd_row) file->active, PIECE_SOURCE );
    }
}


static void FileNewIP( a_window *wnd )
{
    if( WndFile( wnd )->track ) {
        DIPHDL( cue, const      ch );
        cue_handle * const pch = ( DeAliasAddrCue( ContextMod, Context.execution, ch ) != SR_NONE ) ? ch : NULL;

        FileTrack( wnd, pch );
    } else {
        FileActive( wnd, ContextMod );
    }
}

static void ClearSrcFile( file_window *file )
{
    if( file->viewhndl != NULL ) {
        FDoneSource( file->viewhndl );
        file->viewhndl = NULL;
    }
}

static WNDREFRESH FileRefresh;
static void FileRefresh( a_window *wnd )
{
    file_window * const file = WndFile( wnd );

    if( WndFlags & UP_SYM_CHANGE ) {
        if( file->mod != NO_MOD )
            ClearSrcFile( file );
        if( DeAliasAddrMod( file->dotaddr, &file->mod ) == SR_NONE ) {
            file->mod = NO_MOD;
        }
        WndZapped( wnd );
    }
    if( WndFlags & ( UP_CSIP_CHANGE | UP_STACKPOS_CHANGE ) ) {
        FileNewIP( wnd );
    }
    if( ( WndFlags & ( UP_NEW_SRC | UP_SYM_CHANGE ) ) && ( file->mod != NO_MOD ) ) {
        DIPHDL( cue, const      ch );

        ClearSrcFile( file );
        if( LineCue( file->mod, file->file_id, 0, 0, ch ) != SR_NONE ) {
            address const       dotaddr = file->dotaddr;

            FileTrack( wnd, ch );
            SrcMoveDot( wnd, dotaddr );
        }
    }
    if( ( WndFlags & UP_BREAK_CHANGE) && ( file->mod != NO_MOD) ) {
        if( file->toggled_break ) {
            file->toggled_break = FALSE;
        } else {
            WndRepaint( wnd );
        }
    }
}


static WNDCALLBACK FileEventProc;
static bool FileEventProc( a_window * wnd, gui_event gui_ev, void *parm )
{
    file_window * const file = WndFile( wnd );

    parm = parm;
    switch( gui_ev ) {
    case GUI_NOW_ACTIVE:
        ActiveWindowLevel = SOURCE;
        if( !IS_NIL_ADDR( file->dotaddr ) ) {
            SetCodeDot( file->dotaddr );
            AsmMoveDot( file->asm, file->dotaddr );
        }
        return( TRUE );
    case GUI_INIT_WINDOW:
        file->active = NOT_ACTIVE;
        file->rows = 0;
        file->rows_offset = 0;
        file->asm = NULL;
        SeekToTheEnd( file );
        FileNewIP( wnd );
        DbgUpdate( UP_OPEN_CHANGE );
        return( TRUE );
    case GUI_DESTROY :
        if( file->viewhndl != NULL ) {
            FDoneSource( file->viewhndl );
        }
        if( file->erase && file->name ) {
            FileRemove( file->name, 0 );
        }
        WndFree( file->name );
        AsmFreeSrc( file->asm );
        WndFree( file );
        DbgUpdate( UP_OPEN_CHANGE );
        return( TRUE );
    }
    return( FALSE );
}

wnd_info FileInfo = {
    FileEventProc,
    FileRefresh,
    FileGetLine,
    FileMenuItem,
    FileScroll,
    NoBegPaint,
    NoEndPaint,
    FileModify,
    NoNumRows,
    NoNextRow,
    FileNotify,
    UP_NEW_SRC | UP_SYM_CHANGE | UP_CSIP_CHANGE | UP_STACKPOS_CHANGE | UP_BREAK_CHANGE,
    DefPopUp( FileMenu )
};

a_window *DoWndFileOpen( char *name, void *viewhndl, cue_handle *ch,
                                         bool track, bool erase, int class )
{
    static wnd_switches const   switches =
    WSW_LBUTTON_SELECTS | WSW_RBUTTON_SELECTS | WSW_CHAR_CURSOR | WSW_SUBWORD_SELECT;
    file_window * const         file = WndMustAlloc( sizeof( file_window ) );
    a_window                   *wnd;
    wnd_row                     line;

    file->viewhndl = viewhndl;
    if( ch == NULL ) {
        file->mod = NO_MOD;
        file->file_id = 0;
        line = 0;
    } else {
        file->mod = CueMod( ch );
        file->file_id = CueFileId( ch );
        line = (wnd_row) CueLine( ch );
    }
    file->track = 0;
    file->erase = erase;
    file->toggled_break = FALSE;
    file->eof = UINT_MAX;
    file->name = DupStr( name );
    file->dotaddr = NilAddr;
    wnd = DbgWndCreate( LIT( Empty ), &FileInfo, (wnd_class) class, file, &SrcIcon );
    if( ch != NULL ) {
        FileSetDotAddr( wnd, CueAddr( ch ) );
        FileSetTitle( wnd, file->mod, 0 );
    } else {
        FileSetDotAddr( wnd, NilAddr );
        WndSetTitle( wnd, file->name );
    }
    file->track = track;
    if( wnd == NULL )
        return( wnd );
    FileSetTitle( wnd, file->mod, 0 );
    WndSetSwitches( wnd, switches );
    WndClrSwitches( wnd, WSW_HIGHLIGHT_CURRENT );
    if( line != 0 ) {
        WndZapped( wnd );
    }
    FilePosInit( wnd );
    if( line != 0 ) {
        Centre( wnd, --line );
        WndMoveCurrent( wnd, line, PIECE_SOURCE );
    }
    return( wnd );
}

static a_window *SrcFileOpen( cue_handle *ch, bool track, bool erase, mod_handle mod )
{
    void * const        viewhndl = ( ch == NULL ) ? NULL : OpenSrcFile( ch );
    a_window * const    wnd = DoWndFileOpen( LIT( WindowSource ), viewhndl, ch,
                           track, erase, track ? WND_SOURCE : WND_FILE );

    if( wnd != NULL )
        AsmNewSrcNotify( wnd, mod, track = WndFile( wnd )->track );
    return( wnd );
}


a_window *DoWndSrcOpen( cue_handle *ch, bool track )
{
    return( SrcFileOpen( ch, track, FALSE, ch == NULL ? NO_MOD : CueMod( ch ) ) );
}


a_window *WndSrcOpen( void )
{
    mod_handle          mod;
    address             addr;
    DIPHDL( cue,        ch );

    addr = GetCodeDot();
    if( IS_NIL_ADDR( addr ) ) {
        addr = Context.execution;
    }
    if( DeAliasAddrMod( addr, &mod ) == SR_NONE || DeAliasAddrCue( mod, addr, ch ) == SR_NONE ) {
        ch = NULL;
    }
    return( SrcFileOpen( ch, TRUE, FALSE, mod ) );
}


void ProcView( void )
{
    char                *start;
    unsigned            len;
    bool                binary;

    binary = FALSE;
    start = ScanPos();
    if( CurrToken == T_DIV ) {
        Scan();
        if( ScanCmd( "Binary\0" ) != 0 ) {
            binary = TRUE;
        } else {
            ReScan( start );
        }
    }
    ScanItem( TRUE, &start, &len );
    memcpy( TxtBuff, start, len );
    TxtBuff[len] = '\0';
    ReqEOC();
    if( len != 0 ) {
        WndFileInspect( TxtBuff, binary );
    } else {
        WndNewSrcInspect( GetCodeDot() );
    }
}
