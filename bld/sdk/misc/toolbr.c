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
* Description:  Toolbar class for Windows.
*
****************************************************************************/


#include <windows.h>
#include <string.h>
#include <assert.h>
#ifdef __NT__
    #ifndef _WIN32_IE
        #define _WIN32_IE   0x0400
    #endif
    #include <commctrl.h>
#endif
#include "mem.h"
#include "toolbr.h"
#include "win1632.h"

typedef struct tool {
    struct tool *next;
    union {
        HBITMAP bitmap;
        WORD    blank_space;
    };
    HBITMAP     depressed;
    UINT        id;
    UINT        flags;
    WORD        state;
    RECT        area;
} tool;

typedef struct toolbar {
    HWND        hwnd;
    HWND        owner;
    toolhook    hook;
    helphook    helphook;
    POINT       button_size;
    POINT       border;
    HBITMAP     background;
    HBRUSH      foreground;
    HBRUSH      bgbrush;
    int         border_width;
    tool        *tool_list;
    char        is_fixed:1;
    char        spare:7;
#ifdef __NT__
    HWND        container;
    WNDPROC     old_wndproc;
    HWND        tooltips;
#endif
} toolbar;

#define HNULL                   0

#define BORDER_WIDTH( bar )     1

#ifdef __WINDOWS_386__
    #define __FAR__     __far
    #define MAKEPTR( a ) ((void far *)MK_FP32( (void *) a ))
#else
    #define __FAR__
    #define MAKEPTR( a ) ((LPVOID) a)
#endif

#if defined(__WINDOWS_386__)
    #define WINEXP FAR PASCAL
#elif defined(__NT__)
    #define WINEXP __export __stdcall
#else
    #define WINEXP __export FAR PASCAL
#endif

#define BLANK_SIZE( w ) ((w) / 3)

#define GET_INFO( w )   ((toolbar *)GetWindowLong( w, 0 ))
#define SET_INFO( w,i ) (SetWindowLong( w, 0, (LONG)(LPSTR)i))

static char     *className = "WTool";
#ifdef __NT__
static char     *containerClassName = "WToolContainer";
#endif

static char         gdiObjectsCreated;
static HPEN         blackPen;
static HPEN         btnShadowPen;
static HPEN         btnHighlightPen;
static HPEN         btnFacePen;
static HBRUSH       blackBrush;
static HBRUSH       btnFaceBrush;
static COLORREF     crButtonFace;
#ifdef __NT__
static HINSTANCE    hInstCommCtrl;
#endif

static tool *currTool;
static char currIsDown;
static WORD lastID = -1;
static BOOL mouse_captured = FALSE;
static BOOL ignore_mousemove = FALSE; // release_capture generates
                                      // a WM_MOUSEMOVE msg

#if defined(__NT__) || defined(__WINDOWS__)
void    TB_TransparentBlt( HDC, UINT, UINT, UINT, UINT, HDC, COLORREF );
#endif
#ifdef __NT__
HBITMAP TB_CreateTransparentBitmap( HBITMAP, int, int );
#endif

LONG WINEXP     ToolBarWndProc( HWND, unsigned, UINT, LONG );
#ifdef __NT__
LRESULT WINAPI  WinToolWndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT WINAPI  ToolContainerWndProc( HWND, UINT, WPARAM, LPARAM );
#endif

#ifdef __NT__
typedef VOID    (WINAPI *PFNICC)( VOID );

static PFNICC   pfnInitCommonControls;
#endif

/*
 * findTool - find tool item based on id
 */
static tool *findTool( tool *list, WORD id )
{
    while( list != NULL ) {
        if( list->id == id ) break;
        list = list->next;
    }
    return( list );

} /* findTool */

/*
 * addTool - add an item to the tool bar list
 */
static void addTool( tool **list, tool *t )
{
    tool    *curr, **last;

    last = list;
    for( curr = *list; curr != NULL; curr = curr->next ) {
        last = &curr->next;
    }
    *last = t;
    t->next = NULL;

} /* addTool */

/*
 * deleteTool - delete an item from the tool bar list
 */
static void deleteTool( tool **list, tool *t )
{
    tool    *curr, **last;

    last = list;
    for( curr = *list; curr != NULL; curr = curr->next ) {
        if( curr == t ) {
            *last = t->next;
            t->next = NULL;
            break;
        }
        last = &curr->next;
    }

} /* deleteTool */

/*
 * buttonPosition - get position of a button on the tool bar
 */
static BOOL buttonPosition( HWND hwnd, toolbar *bar, tool *top, POINT *p )
{
    RECT    rect;
    int     width, height;
    tool    *curr;
    POINT   pos;

    GetClientRect( hwnd, &rect );
    width = rect.right - rect.left - 2 * bar->border.x;
    height = rect.bottom - rect.top - 2 * bar->border.y;
    curr = bar->tool_list;
    pos.y = 0;
    while( pos.y + bar->button_size.y <= height ) {
        pos.x = 0;
        while( pos.x + bar->button_size.x <= width ) {
            // we assert curr because top MUST be in the list - the only
            // way curr can be NULL is if top is NULL (bad) or not in the
            // list (also bad).
            assert( curr != NULL );
            if( curr == top ) {
                p->x = pos.x + bar->border.x;
                p->y = pos.y + bar->border.y;
                return( TRUE );
            }
            if( curr->flags & ITEM_BLANK ) {
                pos.x += curr->blank_space;
            } else {
                pos.x += bar->button_size.x-1;
            }
            curr = curr->next;
        }
        pos.y += bar->button_size.y-1;
    }
    return( FALSE );

} /* buttonPosition */

/*
 * createButtonList - create all buttons on a tool bar
 */
static void createButtonList( HWND hwnd, toolbar *bar, tool *top )
{
    POINT       pos;

    // top must be an element in the tool_list hanging off of bar
    // we are going to create buttons for all the tools from top
    // to the end of the list
    while( top != NULL ) {
        if( !buttonPosition( hwnd, bar, top, &pos ) ) {
            // no more buttons will fit
            break;
        }
        top->area.top = pos.y;
        top->area.left = pos.x;
        top->area.bottom = pos.y + bar->button_size.y;
        top->area.right = pos.x + bar->button_size.x;
        top = top->next;
    }

} /* createButtonList */

void ToolBarRedrawButtons( struct toolbar *bar )
{
    if( bar ) {
        createButtonList( bar->hwnd, bar, bar->tool_list );
    }
}

/*
 * ToolBarInit - initialize the tool bar
 */
toolbar *ToolBarInit( HWND parent )
{
    WNDCLASS    wc;
    toolbar     *bar;
    HANDLE      instance;
    
    instance = GET_HINSTANCE( parent );

#ifdef __NT__
    if( (hInstCommCtrl = GetModuleHandle( "COMCTL32.DLL" )) != NULL ) {
        pfnInitCommonControls = (PFNICC)GetProcAddress( hInstCommCtrl,
                                                        "InitCommonControls" );
        pfnInitCommonControls();
        if( !GetClassInfo( instance, containerClassName, &wc ) ) {
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wc.lpfnWndProc = (WNDPROC)ToolContainerWndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = instance;
            wc.hIcon = NULL;
            wc.hCursor = LoadCursor( NULL, IDC_ARROW );
            wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = containerClassName;
            RegisterClass( &wc );
        }
    } else {
#endif
        if( !GetClassInfo( instance, className, &wc ) ) {
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wc.lpfnWndProc = (WNDPROC)ToolBarWndProc;
            wc.lpszMenuName = NULL;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = sizeof( LPVOID );
            wc.hInstance = instance;
            wc.hIcon = HNULL;
            wc.hCursor = LoadCursor( (HANDLE) HNULL, IDC_ARROW );
            wc.hbrBackground = (HBRUSH) 0;
            wc.lpszMenuName = NULL;
            wc.lpszClassName = className;
            RegisterClass( &wc );
        }
        if( !gdiObjectsCreated ) {
            blackPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                  GetSysColor( COLOR_BTNTEXT ) );
            btnShadowPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                      GetSysColor( COLOR_BTNSHADOW ) );
            btnHighlightPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                         GetSysColor( COLOR_BTNHIGHLIGHT ) );
            btnFacePen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                    GetSysColor( COLOR_BTNFACE ) );
            blackBrush = GetStockObject( BLACK_BRUSH );
            btnFaceBrush = CreateSolidBrush( GetSysColor( COLOR_BTNFACE ) );
            gdiObjectsCreated = TRUE;
        }
#ifdef __NT__
    }
#endif
    bar = (toolbar *)MemAlloc( sizeof( toolbar ) );
    if ( bar ) {
        bar->border_width = 1;
        bar->owner = parent;
    }

    return( bar );

} /* ToolBarInit */

/*
 * ToolBarChangeSysColors - fiddle with what ToolBar believes
 *                          are the system colours.
 * new:                     stopped fiddeling!
 */
void ToolBarChangeSysColors( COLORREF tbFace,
                             COLORREF tbHighlight, COLORREF tbShadow )
{
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        if( gdiObjectsCreated ) {
            DeleteObject( blackPen );
            DeleteObject( btnShadowPen );
            DeleteObject( btnHighlightPen );
            DeleteObject( btnFacePen );
            DeleteObject( btnFaceBrush );
        }

        blackPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                              GetSysColor( COLOR_BTNTEXT ) );
        btnShadowPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                  GetSysColor( COLOR_BTNSHADOW ) );
        btnHighlightPen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                     GetSysColor( COLOR_BTNHIGHLIGHT ) );
        btnFacePen = CreatePen( PS_SOLID, BORDER_WIDTH( bar ),
                                GetSysColor( COLOR_BTNFACE ) );
        crButtonFace = GetSysColor( COLOR_BTNFACE );
        btnFaceBrush = CreateSolidBrush( crButtonFace );
        gdiObjectsCreated = TRUE;
#ifdef __NT__
    }
#endif

} /* ToolBarChangeSysColors */

/*
 * ToolBarDestroy - done with the tool bar
 */
void ToolBarDestroy ( toolbar *bar )
{
    tool    *curr, *tmp;

    if ( bar ) {
        if( bar->hwnd != HNULL ) {
            DestroyWindow( bar->hwnd );
        }
#ifdef __NT__
        if( bar->container != NULL ) {
            DestroyWindow( bar->container );
        }
#endif
        curr = bar->tool_list;
        while( curr != NULL ) {
            tmp = curr;
            curr = curr->next;
            MemFree( tmp );
        }
        if( bar->bgbrush != NULL ) {
            DeleteObject( bar->bgbrush );
        }
        MemFree( bar );
    }

} /* ToolBarDestroy */

/*
 * ToolBarFini - done with all tool bars
 */
void ToolBarFini( toolbar *bar )
{
    ToolBarDestroy ( bar );

    if( gdiObjectsCreated ) {
        DeleteObject( blackPen );
        DeleteObject( btnShadowPen );
        DeleteObject( btnHighlightPen );
        DeleteObject( btnFacePen );
        DeleteObject( blackBrush );
        DeleteObject( btnFaceBrush );
        gdiObjectsCreated = FALSE;
    }

} /* ToolBarFini */

/*
 * ToolBarAddItem - add a specific bitmap to the tool bar
 */
void ToolBarAddItem( toolbar *bar, TOOLITEMINFO *info )
{
    tool        *t;
#ifdef __NT__
    TBBUTTON    tbb;
    TBADDBITMAP tbab;
    BITMAP      bm;
    int         n;
    TOOLINFO    ti;

    if( hInstCommCtrl == NULL ) {
#endif
        t = (tool *)MemAlloc( sizeof( tool ) );
        if( info->flags & ITEM_BLANK ) {
            t->blank_space = info->blank_space;
        } else {
            t->bitmap = info->bmp;
        }
        t->id = info->id;
        t->next = NULL;
        t->flags = info->flags;
        t->depressed = info->depressed;
        t->state = BUTTON_UP;
        addTool( &bar->tool_list, t );
        if( !(info->flags & ITEM_BLANK) && info->bmp != HNULL ) {
            createButtonList( bar->hwnd, bar, t );
        }
#ifdef __NT__
    } else {
        if( !(info->flags & ITEM_BLANK) ) {
            GetObject( info->bmp, sizeof( BITMAP ), &bm );
            SendMessage( bar->hwnd, TB_SETBITMAPSIZE, 0,
                         MAKELONG( bm.bmWidth, bm.bmHeight ) );
            tbab.hInst = NULL;
            tbab.nID = (UINT_PTR)TB_CreateTransparentBitmap( info->bmp, bm.bmWidth,
                                                             bm.bmHeight );
            tbb.iBitmap = (int)SendMessage( bar->hwnd, TB_ADDBITMAP, 1, (LPARAM)&tbab );
            tbb.idCommand = info->id;
            tbb.fsState = TBSTATE_ENABLED;
            tbb.fsStyle = TBSTYLE_BUTTON;
            if( info->flags & ITEM_STICKY ) {
                tbb.fsStyle |= TBSTYLE_CHECK;
            }
        } else {
            tbb.iBitmap = -1;
            tbb.idCommand = -1;
            tbb.fsState = 0;
            tbb.fsStyle = TBSTYLE_SEP;
        }
        tbb.iString = 0;
        SendMessage( bar->hwnd, TB_ADDBUTTONS, 1, (LPARAM)&tbb );
        if( bar->tooltips != NULL && !(info->flags & ITEM_BLANK) ) {
            n = SendMessage( bar->hwnd, TB_BUTTONCOUNT, 0, 0L );
            SendMessage( bar->hwnd, TB_GETITEMRECT, n - 1, (LPARAM)&ti.rect );
            ti.cbSize = sizeof( TOOLINFO );
            ti.uFlags = 0;
            ti.hwnd = bar->hwnd;
            ti.uId = info->id;
            ti.hinst = GET_HINSTANCE( bar->owner );
            ti.lpszText = info->tip;
            SendMessage( bar->tooltips, TTM_ADDTOOL, 0, (LPARAM)&ti );
        }
    }
#endif

} /* ToolBarAddItem */

void ToolBarSetState( toolbar *bar, WORD id, WORD state )
{
    tool        *t;
    
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        t = findTool( bar->tool_list, id );
        t->state = state;
        // force the button to be redrawn
        InvalidateRect( bar->hwnd, &t->area, FALSE );
#ifdef __NT__
    } else {
        SendMessage( bar->hwnd, TB_CHECKBUTTON, (WPARAM)id,
                     MAKELONG( state & BUTTON_DOWN, 0 ) );
    }
#endif

} /* ToolBarSetState */

WORD ToolBarGetState( toolbar *bar, WORD id )
{
    tool        *t;
#ifdef __NT__
    int         n;
    TBBUTTON    tbb;

    if( hInstCommCtrl == NULL ) {
#endif
        t = findTool( bar->tool_list, id );
        return( t->state );
#ifdef __NT__
    } else {
        n = (int)SendMessage( bar->hwnd, TB_COMMANDTOINDEX, (WPARAM)id, 0L );
        SendMessage( bar->hwnd, TB_GETBUTTON, n, (LPARAM)&tbb );
        return( (tbb.fsState & TBSTATE_CHECKED) ? BUTTON_DOWN : BUTTON_UP );
    }
#endif

} /* ToolBarGetState */

/*
 * ToolBarDeleteItem - delete an item from the tool bar
 */
BOOL ToolBarDeleteItem( toolbar *bar, WORD id )
{
    tool    *t, *next;
#ifdef __NT__
    int     n;

    if( hInstCommCtrl == NULL ) {
#endif
        t = findTool( bar->tool_list, id );
        if( t != NULL ) {
            // need to destroy the window on the tool bar and recreate
            // all the other tools after this one
            next = t->next;
            deleteTool( &bar->tool_list, t );
            createButtonList( bar->hwnd, bar, next );
            return( TRUE );
        }
#ifdef __NT__
    } else {
        n = (int)SendMessage( bar->hwnd, TB_COMMANDTOINDEX, (WPARAM)id, 0L );
        SendMessage( bar->hwnd, TB_DELETEBUTTON, (WPARAM)n, 0L );
        return( TRUE );
    }
#endif

    return( FALSE );

} /* ToolBarDeleteItem */

/*
 * ToolBarDisplay - create and show the tool bar
 */
void ToolBarDisplay( toolbar *bar, TOOLDISPLAYINFO *disp )
{
    int height, width;

    currTool = NULL;
    currIsDown = FALSE;
    lastID = -1;
    mouse_captured = FALSE;

    if( bar->bgbrush != NULL ) {
        DeleteObject( bar->bgbrush );
        bar->bgbrush = 0;
    }
    if( disp->background != NULL ) {
        bar->bgbrush = CreatePatternBrush( disp->background );
    }

    if( bar->hwnd != HNULL ) {
        DestroyWindow( bar->hwnd );
    }
    bar->button_size = disp->button_size;
    bar->hook = disp->hook;
    bar->helphook = disp->helphook;
    bar->border = disp->border_size;
    bar->background = disp->background;
    bar->foreground = disp->foreground;
    bar->is_fixed = disp->is_fixed;
    width = disp->area.right - disp->area.left;
    height = disp->area.bottom - disp->area.top;
#if defined(__NT__)
    if( hInstCommCtrl != NULL ) {
        if( (disp->style & TOOLBAR_FLOAT_STYLE) == TOOLBAR_FLOAT_STYLE ) {
            bar->container = CreateWindowEx( WS_EX_TOOLWINDOW, containerClassName, NULL,
                                             WS_VISIBLE | WS_CAPTION | WS_SYSMENU |
                                             WS_THICKFRAME, disp->area.left,
                                             disp->area.top, width, height, bar->owner,
                                             NULL, GET_HINSTANCE( bar->owner ), bar );
            bar->hwnd = CreateWindow( TOOLBARCLASSNAME, NULL,
                                      WS_CHILD | WS_VISIBLE | TBSTYLE_WRAPABLE |
                                      TBSTYLE_FLAT, 0, 0, 0, 0, bar->container, NULL,
                                      GET_HINSTANCE( bar->owner ), NULL );
        } else {
            bar->hwnd = CreateWindow( TOOLBARCLASSNAME, NULL,
                                      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
                                      TBSTYLE_FLAT, 0, 0, 0, 0, bar->owner, NULL,
                                      GET_HINSTANCE( bar->owner ), NULL );
            GetWindowRect( bar->hwnd, &disp->area );
            ScreenToClient( bar->owner, (LPPOINT)&disp->area.left );
            ScreenToClient( bar->owner, (LPPOINT)&disp->area.right );
        }
        bar->old_wndproc = (WNDPROC)GetWindowLong( bar->hwnd, GWL_WNDPROC );
        SetProp( bar->hwnd, "bar", (LPVOID)bar );
        SetWindowLong( bar->hwnd, GWL_WNDPROC, (DWORD)WinToolWndProc );
        SendMessage( bar->hwnd, TB_BUTTONSTRUCTSIZE, sizeof( TBBUTTON ), 0L );
        if( disp->use_tips ) {
            bar->tooltips = CreateWindow( TOOLTIPS_CLASS, NULL,
                                          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                          0, 0, 0, 0, bar->hwnd, NULL,
                                          GET_HINSTANCE( bar->owner ), NULL );
            SetWindowPos( bar->tooltips, HWND_TOPMOST, 0, 0, 0, 0,
                          SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
            SendMessage( bar->hwnd, TB_SETTOOLTIPS, (WPARAM)bar->tooltips, 0L );
        } else {
            bar->tooltips = NULL;
        }
    } else if( LOBYTE(LOWORD(GetVersion())) >= 4 &&
         (disp->style & TOOLBAR_FLOAT_STYLE) == TOOLBAR_FLOAT_STYLE ) {
        CreateWindowEx( WS_EX_TOOLWINDOW, className, NULL, disp->style,
                        disp->area.left, disp->area.top, width, height,
                        bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
    }
    else {
        if( LOBYTE(LOWORD(GetVersion())) >= 4 ) {
            CreateWindow( className, NULL, WS_CHILD | WS_CLIPSIBLINGS,
                          disp->area.left, disp->area.top, width, height,
                          bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
        } else {
            CreateWindow( className, NULL, disp->style, disp->area.left, disp->area.top,
                          width, height, bar->owner, (HMENU) HNULL,
                          GET_HINSTANCE( bar->owner ), bar );
        }
    }
#else
    CreateWindow( className, NULL, disp->style, disp->area.left, disp->area.top, width,
                  height, bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
#endif
    /*
     * Windows ignores the GETMINMAXINFO before the WM_CREATE or
     * something so we kluge it.
     */
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        MoveWindow( bar->hwnd, disp->area.left, disp->area.top, width, height, TRUE );
#ifdef __NT__
    }
#endif

} /* ToolBarDisplay */

/*
 * ToolBarWindow - return a handle to the toolbar window
 */
HWND ToolBarWindow( toolbar *bar )
{
    if( bar == NULL ) {
        return( HNULL );
    }
#ifdef __NT__
    if( bar->container != NULL ) {
        return( bar->container );
    }
#endif
    return( bar->hwnd );

} /* ToolBarWindow */

/*
 * UpdateToolBar - update our tool bar
 */
void UpdateToolBar( toolbar *bar )
{
    InvalidateRect( bar->hwnd, NULL, TRUE );
    UpdateWindow( bar->hwnd );

} /* UpdateToolBar */

/*
 * drawTopLeftCorner - draws the top left corner of a button
 */
static void drawTopLeftCorner( HDC hdc, POINT size, int border, HPEN pen )

{
    HPEN        old_pen;

    old_pen = SelectObject( hdc, pen );
    MoveToEx( hdc, border, size.y - 2 * border, NULL );
    LineTo( hdc, border, border );
    LineTo( hdc, size.x - border, border );
    SelectObject( hdc, old_pen );

} /* drawTopLeftCorner */

/*
 * drawTopLeftInsideCorner - draws the top left corner of a button
 */
static void drawTopLeftInsideCorner( HDC hdc, POINT size, int border, HPEN pen )

{
    HPEN        old_pen;

    old_pen = SelectObject( hdc, pen );
    MoveToEx( hdc, border * 2, size.y - 2 * border, NULL );
    LineTo( hdc, border * 2, border * 2 );
    LineTo( hdc, size.x - border, border * 2 );
    SelectObject( hdc, old_pen );

} /* drawTopLeftCorner */

/*
 * drawBottomRightCorner - draws the bottom right corner of a button
 */
static void drawBottomRightCorner( HDC hdc, POINT size, int border, HPEN pen )
{
    HPEN        old_pen;

    size.x -= 2 * border;
    size.y -= 2 * border;
    old_pen = SelectObject( hdc, pen );
    MoveToEx( hdc, size.x, border, NULL );
    LineTo( hdc, size.x, size.y );
    LineTo( hdc, border - 1, size.y );
    SelectObject( hdc, old_pen );

} /* drawBottomRightCorner */

/*
 * drawBottomRightInsideCorner - draw the inside corner of a button
 */
static void drawBottomRightInsideCorner( HDC hdc, POINT size, int border, HPEN pen )
{
    HPEN        old_pen;

    size.x -= 3 * border;
    size.y -= 3 * border;
    old_pen = SelectObject( hdc, pen );
    MoveToEx( hdc, 2 * border, size.y, NULL );
    LineTo( hdc, size.x, size.y );
    LineTo( hdc, size.x, border );
    SelectObject( hdc, old_pen );

} /* drawBottomRightInsideCorner */

/*
 * drawBorder - draw to border of a button
 */
static void drawBorder( HDC hdc, POINT size, int border )
{
    HPEN        old_pen;
    int         y,x;

    y = size.y - 1;
    x = size.x - 1;

    old_pen = SelectObject( hdc, blackPen );

    MoveToEx( hdc, 0, 0, NULL );
    LineTo( hdc, x, 0 );
    LineTo( hdc, x, y );
    LineTo( hdc, 0, y );
    LineTo( hdc, 0, 0 );

    SelectObject( hdc, btnFacePen );
    MoveToEx( hdc, 0, 0, NULL );
    LineTo( hdc, border, 0 );
    MoveToEx( hdc, x, 0, NULL );
    LineTo( hdc, x, border );
    MoveToEx( hdc, x, y, NULL );
    LineTo( hdc, x - border, y );
    MoveToEx( hdc, 0, y, NULL );
    LineTo( hdc, 0, y - border );

    SelectObject( hdc, old_pen );

} /* drawBorder */

/*
 * toolBarDrawBitmap - draw the bitmap on a button
 */
static void toolBarDrawBitmap( HDC hdc, POINT dst_size, POINT dst_org, HBITMAP bitmap )
{
    BITMAP      bm;
    HDC         memdc;
    POINT       src_org, src_size;
    HBRUSH      old_brush;
    HBITMAP     old_bmp;

    DPtoLP( hdc, &dst_size, 1 );
    DPtoLP( hdc, &dst_org, 1 );

    memdc = CreateCompatibleDC( hdc );
    old_bmp = SelectObject( memdc, bitmap );

    GetObject( bitmap, sizeof( BITMAP ), &bm );

    src_size.x = bm.bmWidth;
    src_size.y = bm.bmHeight;
    DPtoLP( hdc, &src_size, 1 );

    src_org.x = 0;
    src_org.y = 0;
    DPtoLP( hdc, &src_org, 1 );

    #if defined( __NT__ ) || defined( __WINDOWS__ )
        SetStretchBltMode( hdc, COLORONCOLOR );
    #else
        SetStretchBltMode( hdc, STRETCH_DELETESCANS );
    #endif

    /*
     * If it's a monochrome bitmap try and do the right thing - I can see
     * this pissing off some users, but oh well.
     */
    if( bm.bmBitsPixel == 1 && bm.bmPlanes == 1 ) {
        old_brush = SelectObject( hdc, blackBrush );

        StretchBlt( hdc, dst_org.x, dst_org.y, dst_size.x, dst_size.y, memdc, src_org.x,
                    src_org.y, src_size.x, src_size.y, 0xb8074a );

        if( old_bmp != HNULL ) {
            SelectObject( memdc, old_bmp );
        }

        SelectObject( hdc, old_brush );
    } else {
        StretchBlt( hdc, dst_org.x, dst_org.y, dst_size.x, dst_size.y, memdc, src_org.x,
                    src_org.y, src_size.x, src_size.y, SRCCOPY );

        if( old_bmp != HNULL ) {
            SelectObject( memdc, old_bmp );
        }
    }
    DeleteDC( memdc );

} /* toolBarDrawBitmap */

/*
 * drawButton - draw a button on the toolbar
 */
static void drawButton( HDC hdc, HWND hwnd, tool *tool, BOOL down )
{
    toolbar     *bar;
    HBRUSH      brush;
    int         shift;
    BOOL        selected;
    BOOL        got_dc;
    POINT       dst_size;
    POINT       dst_org;
    HBITMAP     used_bmp;
    HBITMAP     bitmap, oldbmp;
    HDC         mem;
#if defined( __NT__ ) || defined( __WINDOWS__ )
    HBITMAP     bitmap2, oldbmp2, bmptmp;
    HDC         mem2;
    RECT        fill;
    COLORREF    cr;
#endif

    if( tool->flags & ITEM_BLANK ) {
        return;
    }

    /* draw the button */
    bar = GET_INFO( hwnd );

    if( tool->flags & ITEM_STICKY ) {
        selected = (tool->state == BUTTON_DOWN);
    } else {
        selected = down;
    }
    shift = 0;
    if( selected ) {
        shift = 2 * BORDER_WIDTH( bar );
    }

    got_dc = FALSE;
    if( hdc == (HDC)NULL ) {
        hdc = GetDC( hwnd );
        got_dc = TRUE;
    }

    mem = CreateCompatibleDC( hdc );
    bitmap = CreateCompatibleBitmap( hdc, bar->button_size.x, bar->button_size.y );
    oldbmp = SelectObject( mem, bitmap );
#if defined( __NT__ ) || defined( __WINDOWS__ )
    mem2 = CreateCompatibleDC( hdc );
    bitmap2 = CreateCompatibleBitmap( hdc, bar->button_size.x, bar->button_size.y );
    oldbmp2 = SelectObject( mem2, bitmap2 );
#endif

    brush = btnFaceBrush;
    if( selected && bar->bgbrush != HNULL ) {
        brush = bar->bgbrush;
    }
    FillRect( mem, &tool->area, brush );

    dst_size = bar->button_size;
    dst_size.x -= 4 * BORDER_WIDTH( bar );
    dst_size.y -= 4 * BORDER_WIDTH( bar );
    dst_org.x = (1 * BORDER_WIDTH( bar )) + shift;
    dst_org.y = (1 * BORDER_WIDTH( bar )) + shift;
    used_bmp = tool->bitmap;
    if( selected ) {
        // if the button is selected and it has the ITEM_DOWNBMP flag
        // then we draw the alternate bitmap instead.
        if( tool->flags & ITEM_DOWNBMP ) {
            used_bmp = tool->depressed;
        }
    }
    toolBarDrawBitmap( mem, dst_size, dst_org, used_bmp );

#if defined( __NT__ ) || defined( __WINDOWS__ )
    /* New, on WIN32 platforms, use TB_TransparentBlt() */
    /* Get background color of button bitmap */
    bmptmp = SelectObject( mem2, used_bmp );
    /* Expects 0,0 pix in original to be in background color */
    cr = GetPixel( mem2, 0, 0 );
    bmptmp = SelectObject( mem2, bmptmp );
    /* IMPORTANT: must set required new background color for dest bmp */
    SetBkColor( mem2, GetSysColor( COLOR_BTNFACE ) );
    fill.top = 0;
    fill.left = 0;
    fill.right = bar->button_size.x;
    fill.bottom = bar->button_size.y;
    FillRect( mem2, &fill, brush );
    TB_TransparentBlt( mem2, dst_org.x, dst_org.y, dst_size.x, dst_size.y, mem, cr );
    if( oldbmp != HNULL ) {
        SelectObject( mem, oldbmp );
        DeleteObject( bitmap );
    }
    DeleteDC( mem );
    /* Switch new bitmap into vars expected by code below */
    mem = mem2;
    bitmap = bitmap2;
    oldbmp = oldbmp2;
#endif

    drawBorder( mem, bar->button_size, BORDER_WIDTH( bar ) );
    if( selected ) {
        drawTopLeftCorner( mem, bar->button_size, BORDER_WIDTH( bar ), btnShadowPen );
        drawTopLeftInsideCorner( mem, bar->button_size, BORDER_WIDTH( bar ),
                                 btnFacePen );
    } else {
        drawTopLeftCorner( mem, bar->button_size, BORDER_WIDTH( bar ), btnHighlightPen );
        drawBottomRightCorner( mem, bar->button_size, BORDER_WIDTH( bar ),
                               btnShadowPen );
        drawBottomRightInsideCorner( mem, bar->button_size, BORDER_WIDTH( bar ),
                                     btnShadowPen );

    }
    BitBlt( hdc, tool->area.left, tool->area.top, bar->button_size.x, bar->button_size.y,
            mem, 0, 0, SRCCOPY ); /* copy it to screen */

    if( oldbmp != HNULL ) {
        SelectObject( mem, oldbmp );
    }
    if( got_dc ) {
        ReleaseDC( hwnd, hdc );
    }
    DeleteObject( bitmap );
    DeleteDC( mem );

} /* drawButton */

/*
 * isPointInToolbar - is the point in the mousemove message in the toolbar
 */
BOOL isPointInToolbar( HWND hwnd, WPARAM wparam, LPARAM lparam )
{
    POINT   p;
    RECT    r;

    wparam = wparam;

    MAKE_POINT( p, lparam );
    GetClientRect( hwnd, &r );
    if( PtInRect( &r, p ) ) {
        return( TRUE );
    }
    return( FALSE );

} /* isPointInToolbar */

/*
 * findToolAtPoint - find a tool at a given point
 */
static tool *findToolAtPoint( toolbar *bar, LPARAM lparam )
{
    POINT   p;
    tool    *tool;

    p.x = LOWORD( lparam );
    p.y = HIWORD( lparam );
    for( tool = bar->tool_list; tool != NULL; tool = tool->next ) {
        if( PtInRect( &tool->area, p ) ) {
            if( tool->flags & ITEM_BLANK ) {
                return( NULL );
            } else {
                return( tool );
            }
        }
    }
    return( NULL );

} /* findToolAtPoint */

#ifdef __NT__

/*
 * customHitTest - find a tool at a given point for a native toolbar
 */
static int customHitTest( toolbar *bar, POINT *pt )
{
    int         i;
    int         count;
    TBBUTTON    tbb;
    RECT        rc;

    count = (int)SendMessage( bar->hwnd, TB_BUTTONCOUNT, 0, 0L );
    for( i = 0; i < count; i++ ) {
        SendMessage( bar->hwnd, TB_GETITEMRECT, i, (LPARAM)&rc );
        if( PtInRect( &rc, *pt ) ) {
            SendMessage( bar->hwnd, TB_GETBUTTON, i, (LPARAM)&tbb );
            return( tbb.idCommand );
        }
    }

    return( -1 );

} /* customHitTest */

#endif

/*
 * HasToolAtPoint - return TRUE if tool exists at a given point
 */
BOOL HasToolAtPoint( struct toolbar *bar, LPARAM lparam )
{
#ifdef __NT__
    POINT   pt;
    
    if( hInstCommCtrl == NULL ) {
#endif
        return( findToolAtPoint( bar, lparam ) != NULL );
#ifdef __NT__
    } else {
        pt.x = LOWORD( lparam );
        pt.y = HIWORD( lparam );
        return( customHitTest( bar, &pt ) >= 0 );
    }
#endif

} /* HasToolAtPoint */

/*
 * FindToolIDAtPoint - Find the toolID at a given point, if any.  Returns
 * TRUE if tool exists at a given point.
 */
BOOL FindToolIDAtPoint( struct toolbar *bar, LPARAM lparam, UINT *id )
{
    tool    *ctool;
#ifdef __NT__
    POINT   pt;
    int     ret;
    
    if( hInstCommCtrl == NULL ) {
#endif
        ctool = findToolAtPoint( bar, lparam );
        if( ctool != NULL ) {
            *id = ctool->id;
            return TRUE;
        } else {
            return FALSE;
        }
#ifdef __NT__
    } else {
        pt.x = LOWORD( lparam );
        pt.y = HIWORD( lparam );
        ret = customHitTest( bar, &pt );
        if( ret >= 0 ) {
            *id = ret;
            return( TRUE );
        } else {
            return( FALSE );
        }
    }
#endif

} /* FindToolIDAtPoint */

/*
 * ToolBarWndProc - callback routine for the tool bar
 */
LONG WINEXP ToolBarWndProc( HWND hwnd, unsigned msg, UINT wparam, LONG lparam )
{
    CREATESTRUCT    __FAR__ *cs;
    toolbar         *bar;
    tool            *tool;
    RECT            inter;
    HDC             hdc;
    PAINTSTRUCT     ps;
    BOOL            posted;

    bar = GET_INFO( hwnd );
    if( msg == WM_CREATE ) {
        cs = MAKEPTR( lparam );
        bar = cs->lpCreateParams;
#ifdef __WINDOWS_386__
        bar = MapAliasToFlat( (DWORD) bar );
#endif
        bar->hwnd = hwnd;
        SET_INFO( hwnd, bar );
    }
    if( bar != NULL && bar->hook != NULL ) {
        if( bar->hook( hwnd, msg, wparam, lparam ) && msg != WM_DESTROY ) {
            return( 0L );
        }
    }
    switch( msg ) {
    case WM_SIZE:
        if( bar && bar->tool_list && !bar->is_fixed ) {
            createButtonList( hwnd, bar, bar->tool_list );
            InvalidateRect( hwnd, NULL, TRUE );
            UpdateWindow( hwnd );
        }
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        if( bar && bar->tool_list ) {
            currTool = NULL;
            tool = findToolAtPoint( bar, lparam );
            if( tool ) {
                if( bar->hook != NULL ) {
                    bar->hook( hwnd, WM_USER, tool->id, 0L );
                }
                currTool = tool;
                drawButton( (HDC)NULL, hwnd, tool, TRUE );
                mouse_captured = TRUE;
                SetCapture( hwnd );
                currIsDown = TRUE;
                if( bar->helphook != NULL ) {
                    bar->helphook( hwnd, currTool->id, TRUE );
                }
            }
        }
        break;
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
        if( bar && bar->tool_list ) {
            tool = findToolAtPoint( bar, lparam );
            posted = FALSE;
            if( tool != NULL ) {
                if( tool == currTool ) {
                    PostMessage( bar->owner, WM_COMMAND, tool->id, 0 );
                    posted = TRUE;
                    drawButton( (HDC)NULL, hwnd, tool, FALSE );
                }
            }
            if( !posted && bar->hook != NULL ) {
                if( currTool != NULL ) {
                    bar->hook( hwnd, WM_USER, currTool->id, 1L );
                }
            }
            if( currTool != NULL ) {
                mouse_captured = FALSE;
                ignore_mousemove = TRUE; // release_capture generates a
                                         // WM_MOUSEMOVE
                ReleaseCapture();
                if( bar->helphook != NULL ) {
                    bar->helphook( hwnd, currTool->id, FALSE );
                }
                currTool = NULL;
            }
        }
        break;
    case WM_MOUSEMOVE:
        if( ignore_mousemove ) {
            ignore_mousemove = FALSE;
            break;
        }
        tool = findToolAtPoint( bar, lparam );
        if( currTool ) {
            if( tool == currTool ) {
                if( !currIsDown ) {
                    currIsDown = TRUE;
                    drawButton( (HDC)NULL, hwnd, currTool, TRUE );
                    if( bar->helphook != NULL ) {
                        bar->helphook( hwnd, currTool->id, TRUE );
                    }
                }
            } else {
                if( currIsDown ) {
                    drawButton( (HDC)NULL, hwnd, currTool, FALSE );
                    currIsDown = FALSE;
                    if( bar->helphook != NULL ) {
                        bar->helphook( hwnd, currTool->id, FALSE );
                    }
                }
            }
        } else {
            if( bar->helphook != NULL ) {
                if( tool ) {
                    bar->helphook( hwnd, tool->id, TRUE );
                    lastID = tool->id;
                } else if( lastID != (WORD)-1 ) {
                    bar->helphook( hwnd, lastID, FALSE );
                    lastID = -1;
                }
            }
        }
        break;
#if defined( __NT__ ) || defined( __WINDOWS__ )
    case WM_SYSCOLORCHANGE:
        ToolBarChangeSysColors( (COLORREF)0L, (COLORREF)0L, (COLORREF)0L );
        break;
#endif
    case WM_PAINT:
#if defined( __NT__ ) || defined( __WINDOWS__ )
        if( crButtonFace != GetSysColor( COLOR_BTNFACE ) ) {
            /* WM_SYSCOLORCHANGED: sometimes not received by window.
               Have to fake it...  */
            ToolBarChangeSysColors( (COLORREF)0L, (COLORREF)0L, (COLORREF)0L );
        }
#endif
        hdc = BeginPaint( hwnd, &ps );
        FillRect( hdc, &ps.rcPaint, btnFaceBrush );
        for( tool = bar->tool_list; tool != NULL; tool = tool->next ) {
            if( IntersectRect( &inter, &ps.rcPaint, &tool->area ) ) {
                drawButton( hdc, hwnd, tool, FALSE );
            }
        }
        EndPaint( hwnd, &ps );
        break;
    case WM_DESTROY:
        bar->hwnd = HNULL;
        break;
    }
    return( DefWindowProc( hwnd, msg, wparam, lparam ) );

} /* ToolBarWndPproc */

#ifdef __NT__

LRESULT WINAPI WinToolWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    struct toolbar  *bar;
    UINT            id;
    int             n;
    TBBUTTON        tbb;
    DWORD           pos;
    POINT           pt;
    RECT            rc;
    
    bar = (struct toolbar *)GetProp( hwnd, "bar" );
    switch( msg ) {
    case WM_MOUSEMOVE:
        if( FindToolIDAtPoint( bar, lparam, &id ) ) {
            n = SendMessage( hwnd, TB_COMMANDTOINDEX, id, 0L );
            SendMessage( hwnd, TB_GETBUTTON, n, (LPARAM)&tbb );
            if( (tbb.fsState & TBSTATE_PRESSED) || !(wparam & MK_LBUTTON) ) {
                bar->helphook( hwnd, id, TRUE );
                lastID = id;
                SetTimer( hwnd, 1, 50, NULL );
            } else if( lastID != (WORD)-1 ) {
                bar->helphook( hwnd, lastID, FALSE );
                lastID = -1;
                KillTimer( hwnd, 1 );
            }
        } else if( lastID != (WORD)-1 ) {
            bar->helphook( hwnd, lastID, FALSE );
            lastID = -1;
            KillTimer( hwnd, 1 );
        }
        break;
    case WM_TIMER:
        if( lastID != (WORD)-1 ) {
            pos = GetMessagePos();
            pt.x = LOWORD( pos );
            pt.y = HIWORD( pos );
            ScreenToClient( hwnd, &pt );
            GetClientRect( hwnd, &rc );
            if( !PtInRect( &rc, pt ) ) {
                bar->helphook( hwnd, lastID, FALSE );
                lastID = -1;
                KillTimer( hwnd, 1 );
            }
        };
        break;
    case WM_LBUTTONUP:
        if( lastID != (WORD)-1 ) {
            bar->helphook( hwnd, lastID, FALSE );
            lastID = -1;
            KillTimer( hwnd, 1 );
        }
        break;
    }
    
    return( CallWindowProc( bar->old_wndproc, hwnd, msg, wparam, lparam ) );

} /* WinToolWndProc */

LRESULT WINAPI ToolContainerWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    HWND            otherwnd;
    struct toolbar  *bar;

    if( msg == WM_NCCREATE ) {
        bar = ((CREATESTRUCT *)lparam)->lpCreateParams;
        SetProp( hwnd, "bar", bar );
    } else {
        bar = (struct toolbar *)GetProp( hwnd, "bar" );
    }
    if( bar != NULL ) {
        bar->hook( hwnd, msg, wparam, lparam );
    }
    
    switch( msg ) {
    case WM_SIZE:
        otherwnd = GetWindow( hwnd, GW_CHILD );
        SendMessage( otherwnd, WM_SIZE, 0, 0L );
        break;
    case WM_COMMAND:
        otherwnd = GetWindow( hwnd, GW_OWNER );
        SendMessage( otherwnd, WM_COMMAND, wparam, lparam );
        break;
    }
    return( DefWindowProc( hwnd, msg, wparam, lparam ) );

} /* ToolContainerWndProc */

#endif

/*
 * ChangeToolButtonBitmap - change a bitmap for a toolbar item
 */
void ChangeToolButtonBitmap( toolbar *bar, int id, HBITMAP newbmp )
{
    tool        *t;
#ifdef __NT__
    int         n;
    TBBUTTON    tbb;
    TBADDBITMAP tbab;
    
    if( hInstCommCtrl == NULL ) {
#endif
        t = findTool( bar->tool_list, id );
        if( t != NULL ) {
            t->bitmap = newbmp;
            InvalidateRect( bar->hwnd, &t->area, TRUE );
            UpdateWindow( bar->hwnd );
        }
#ifdef __NT__
    } else {
        n = SendMessage( bar->hwnd, TB_COMMANDTOINDEX, (WPARAM)id, 0L );
        if( n >= 0 ) {
            SendMessage( bar->hwnd, TB_GETBUTTON, (WPARAM)n, (LPARAM)&tbb );
            tbab.hInst = NULL;
            tbab.nID = (UINT_PTR)TB_CreateTransparentBitmap( newbmp,
                bar->button_size.x - bar->border.x,
                bar->button_size.y - bar->border.y - 2 );
            tbb.iBitmap = (int)SendMessage( bar->hwnd, TB_ADDBITMAP, 1, (LPARAM)&tbab );
            SendMessage( bar->hwnd, TB_DELETEBUTTON, n, 0L );
            SendMessage( bar->hwnd, TB_INSERTBUTTON, n, (LPARAM)&tbb );
        }
    }
#endif

} /* ChangeToolButtonBitmap */


#if defined( __NT__ ) || defined( __WINDOWS__ )

/*
 * TB_TransparentBlt
 *
 * Purpose: Given two DC's and a color to assume as transparent in
 * the source, BitBlts the bitmap to the dest DC letting the existing
 * background show in place of the transparent color.
 * Adapted from an old MS SDK sample.
 *
 * NOTE: make sure BkColor is set in dest hDC.
 *
 * Parameters: hDC      HDC      destination, on which to draw.
 *             x, y     UINT     location at which to draw the bitmap
 *             width    UINT     width to draw
 *             height   UINT     height to draw
 *             hDCIn    HDC      source, to draw from
 *             cr       COLORREF to consider as transparent in source.
 *
 * Return Value: None
 */

#define ROP_DSPDxax  0x00E20746

void TB_TransparentBlt( HDC hDC, UINT x, UINT y, UINT width, UINT height,
                        HDC hDCIn, COLORREF cr )
{
   HDC      hDCMid, hMemDC;
   HBITMAP  hBmpMono, hBmpT;
   HBRUSH   hBr, hBrT;
   COLORREF crBack, crText;

   if( NULL == hDCIn )
      return;

   /* Make two intermediate DC's */
   hDCMid = CreateCompatibleDC( hDC );
   hMemDC = CreateCompatibleDC( hDC );

   /* Create a monochrome bitmap for masking */
   hBmpMono = CreateCompatibleBitmap( hDCMid, x + width, y + height );
   SelectObject( hDCMid, hBmpMono );

   /* Create a mid-stage bitmap */
   hBmpT = CreateCompatibleBitmap( hDC, x + width, y + height );
   SelectObject( hMemDC, hBmpT );

   /* Create a monochrome mask where we have 0's in the image, 1's elsewhere. */
   crBack = SetBkColor( hDCIn, cr );
   BitBlt( hDCMid, x, y, width, height, hDCIn, x, y, SRCCOPY );
   SetBkColor( hDCIn, crBack );

   /* Put the unmodified image in the temporary bitmap */
   BitBlt( hMemDC, x, y, width, height, hDCIn, x, y, SRCCOPY );

   /* Create an select a brush of the background color */
   hBr = CreateSolidBrush( GetBkColor( hDC ) );
   hBrT = SelectObject( hMemDC, hBr );

   /* Force conversion of the monochrome to stay black and white. */
   crText = SetTextColor( hMemDC, 0L );
   crBack = SetBkColor( hMemDC, RGB( 255, 255, 255 ) );

   /*
    * Where the monochrome mask is 1, Blt the brush; where the mono
    * mask is 0, leave the destination untouched.  This results in
    * painting around the image with the background brush.  We do this
    * first in the temporary bitmap, then put the whole thing to the
    * screen (avoids flicker).
    */
   BitBlt( hMemDC, x, y, width, height, hDCMid, x, y, ROP_DSPDxax );
   BitBlt( hDC, x, y, width, height, hMemDC, x, y, SRCCOPY );

   SetTextColor( hMemDC, crText );
   SetBkColor( hMemDC, crBack );

   SelectObject( hMemDC, hBrT );
   DeleteObject( hBr );

   DeleteDC( hMemDC );
   DeleteDC( hDCMid );
   DeleteObject( hBmpT );
   DeleteObject( hBmpMono );

}  /* TansparentBlt () */

#endif

#ifdef __NT__

/*
 * TB_CreateTransparentBitmap()
 *
 * Purpose: Creates a transparent bitmap for use with Win32 native toolbars.
 *
 * Parameters: hBitmap  HBITMAP original bitmap
 *             width    int     width of the original bitmap
 *             height   int     height of the original bitmap
 *
 * Return Value: Handle to the new transparent bitmap.
 */

HBITMAP TB_CreateTransparentBitmap( HBITMAP hBitmap, int width, int height )
{
    HDC     hDC1;
    HDC     hDC2;
    HBITMAP hOldBitmap1;
    HBITMAP hOldBitmap2;
    HBITMAP hNewBitmap;
    
    hDC1 = CreateCompatibleDC( NULL );
    hOldBitmap1 = SelectObject( hDC1, hBitmap );
    hDC2 = CreateCompatibleDC( hDC1 );
    hNewBitmap = CreateCompatibleBitmap( hDC1, width, height );
    hOldBitmap2 = SelectObject( hDC2, hNewBitmap );
    SetBkColor( hDC2, GetSysColor( COLOR_3DFACE ) );
    TB_TransparentBlt( hDC2, 0, 0, width, height, hDC1, GetPixel( hDC1, 0, 0 ) );
    SelectObject( hDC1, hOldBitmap1 );
    SelectObject( hDC2, hOldBitmap2 );
    DeleteDC( hDC1 );
    DeleteDC( hDC2 );

    return( hNewBitmap );
}

#endif

