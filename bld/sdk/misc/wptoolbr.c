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
* Description:  Toolbar window class for Windows and OS/2.
*
****************************************************************************/


#ifdef __NT__
    #define _WIN32_IE   0x0300
#endif

#ifdef __OS2_PM__
#define INCL_PM
#define INCL_WINFRAMEMGR
#define INCL_NLS
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#include <os2.h>
#define __FAR
#else
#include <windows.h>
#ifdef __NT__
#include <commctrl.h>
#endif
#endif

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "wpi.h"
#include "mem.h"
#include "wptoolbr.h"
#define TOOLBR_DIM  WPI_RECTDIM

typedef struct tool {
    struct tool *next;
    union {
        HBITMAP bitmap;
        WORD    blank_space;
    } u;                        // For compatability with compilers that
                                // don't support anonymous unions
    HBITMAP     depressed;
    WORD        id;
    UINT        flags;
    WORD        state;
    WPI_RECT    area;
} tool;

typedef struct toolbar {
    HWND        hwnd;
    HWND        owner;          // this is actually the parent
    toolhook    hook;
    helphook    helphook;
    WPI_POINT   button_size;
    WPI_POINT   border;
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
#endif
} toolbar;

#define HNULL                   0

#define BORDER_WIDTH( bar )     1

#ifndef __OS2_PM__
#ifdef __WINDOWS_386__
    #define MAKEPTR( a ) ((void far *)MK_FP32( (void *) a ))
#else
    #define MAKEPTR( a ) ((LPVOID) a)
#endif
#endif

#define BLANK_SIZE( w ) ( (w) / 3 )
#define GET_INFO( w )   ((toolbar *)_wpi_getwindowlong( w, 0 ))
#define SET_INFO( w,i ) (_wpi_setwindowlong( w, 0, (LONG)(LPSTR)i))

static char     toolBarClassRegistered;
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
static COLORREF     btnColour;
static WPI_INST     appInst;
#ifdef __NT__
static HINSTANCE    hInstCommCtrl;
#endif

static tool     *currTool;
static char     currIsDown;
static WORD     lastID = -1;
static BOOL     mouse_captured = FALSE;
static BOOL     ignore_mousemove = FALSE; // release_capture generates a
                                          // WM_MOUSEMOVE msg
#ifndef __OS2_PM__
static BOOL     round_corners = TRUE;     // Platform has rounded buttons?
#else
static BOOL     round_corners = FALSE;    // Platform has rounded buttons?
#endif

MRESULT CALLBACK    ToolBarWndProc( HWND, WPI_MSG, WPI_PARAM1, WPI_PARAM2 );
#ifdef __NT__
LRESULT WINAPI      WinToolWndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT WINAPI      ToolContainerWndProc( HWND, UINT, WPARAM, LPARAM );
#endif

#if defined(__NT__) || defined(__WINDOWS__)
void    WPTB_TransparentBlt (HDC hDC,   UINT x, UINT y, UINT width, UINT height,
                          HDC hDCIn, COLORREF cr);
HBITMAP WPTB_CreateTransparentBitmap( HBITMAP, int, int );
#endif

#ifdef __NT__
typedef VOID    (WINAPI *PFNICC)( VOID );

static PFNICC   pfnInitCommonControls;
#endif

static void toolbardestroywindow( HWND hwnd )
{
    hwnd = _wpi_getframe( hwnd );
    _wpi_destroywindow( hwnd );
}

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
    tool        *curr, **last;

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
    tool        *curr, **last;

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
static BOOL buttonPosition( HWND hwnd, toolbar *bar, tool *top, WPI_POINT *p )
{
    WPI_RECT    rect;
    int         width, height;
    int         wndheight;
    tool        *curr;
    WPI_POINT   pos;

    _wpi_getclientrect( hwnd, &rect );
    width = _wpi_getwidthrect( rect ) - 2 * bar->border.x;
    height = _wpi_getheightrect( rect ) - 2 * bar->border.y;
    wndheight = _wpi_getheightrect( rect );
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
#ifndef __OS2_PM__
                p->y = pos.y + bar->border.y;
#else
                p->y = _wpi_cvth_y(pos.y, wndheight) -
                                        bar->button_size.y - bar->border.y+1;
#endif
                return( TRUE );
            }
            if( curr->flags & ITEM_BLANK ) {
                pos.x += curr->u.blank_space;
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
    WPI_POINT   pos;

    // top must be an element in the tool_list hanging off of bar
    // we are going to create buttons for all the tools from top
    // to the end of the list
    while( top != NULL ) {
        if( !buttonPosition( hwnd, bar, top, &pos ) ) {
            // no more buttons will fit
            break;
        }
        _wpi_setrectvalues( &(top->area), pos.x, pos.y,
                                                pos.x + bar->button_size.x,
                                                pos.y + bar->button_size.y );
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
    COLORREF    clr_btnface;
    COLORREF    clr_btnshadow;
    COLORREF    clr_btnhighlight;
    COLORREF    clr_black;
    toolbar     *bar;
#ifndef __OS2_PM__
        /*
         ****************
         * Windows version of initialization
         ****************
         */
    WNDCLASS    wc;
    HANDLE      instance;

    instance = GET_HINSTANCE( parent );
    appInst = instance;

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
        if( !toolBarClassRegistered ) {
            wc.style = CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wc.lpfnWndProc = (LPVOID) ToolBarWndProc;
            wc.lpszMenuName = NULL;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = sizeof( LPVOID );
            wc.hInstance = instance;
            wc.hIcon = HNULL;
            wc.hCursor = LoadCursor( (HANDLE) HNULL, IDC_ARROW );
            wc.hbrBackground = (HBRUSH) 0; // (COLOR_BTNFACE + 1);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = className;
            RegisterClass( &wc );
            toolBarClassRegistered = TRUE;
        }
        clr_btnshadow = GetSysColor( COLOR_BTNSHADOW );
        clr_btnhighlight = GetSysColor( COLOR_BTNHIGHLIGHT );
        btnColour = GetSysColor( COLOR_BTNFACE );
        clr_btnface = btnColour;
#if defined(__NT__) || defined(__WINDOWS__)
        clr_black = GetSysColor(COLOR_BTNTEXT);
#endif
#if defined(__NT__)
        {
            OSVERSIONINFO os;

            GetVersionEx(&os);
            if ( os.dwMajorVersion == 4 || (os.dwMajorVersion == 5 && os.dwMinorVersion == 0)) {
                // round_corners = FALSE;
                // Later, when drawing code is adapted
            }
        }
#else
        clr_black = RGB(0, 0, 0);
#endif
#ifdef __NT__
    }
#endif
#else
        /*
         ******************
         * PM Version of the initialization
         ******************
         */
    int         rc;
    HAB         hab;

    hab = WinQueryAnchorBlock( parent );
    appInst.hab = hab;
    appInst.mod_handle = (HMODULE)0;

    if( !toolBarClassRegistered ) {
        rc = WinRegisterClass( hab, className,
                                (PFNWP)ToolBarWndProc,
                                CS_MOVENOTIFY | CS_SIZEREDRAW | CS_CLIPSIBLINGS,
                                sizeof(PVOID)  );
        toolBarClassRegistered = TRUE;
    }
    clr_btnshadow = CLR_DARKGRAY;
    clr_btnhighlight = CLR_WHITE;
    clr_btnface = CLR_PALEGRAY;
    clr_black = CLR_BLACK;
    btnColour = CLR_PALEGRAY;
#endif

    bar = (toolbar *)MemAlloc( sizeof( toolbar ) );
    if ( bar ) {
        memset ( bar, 0, sizeof( toolbar ) );
        bar->border_width = 1;
        bar->owner = parent;
    }

#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        if( !gdiObjectsCreated ) {
            blackPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_black );
            btnShadowPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnshadow );
            btnHighlightPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnhighlight );
            btnFacePen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnface );
            blackBrush = _wpi_createsolidbrush( clr_black );
            btnFaceBrush = _wpi_createsolidbrush( clr_btnface );
            gdiObjectsCreated = TRUE;
        }
#ifdef __NT__
    }
#endif

    return( bar );

} /* ToolBarInit */

/*
 * ToolBarFini - done with the tool bar
 */
void ToolBarDestroy ( toolbar *bar )
{
    tool        *curr, *tmp;

    if ( bar ) {
        if( bar->hwnd != HNULL ) {
            toolbardestroywindow( bar->hwnd );
        }
        curr = bar->tool_list;
        while( curr != NULL ) {
            tmp = curr;
            curr = curr->next;
            MemFree( tmp );
        }
        if( bar->bgbrush != NULLHANDLE ) {
            _wpi_deleteobject( bar->bgbrush );
        }
    }

    MemFree( bar );
}

/*
 * ToolBarFini - done with all tool bars
 */
void ToolBarFini( toolbar *bar )
{
    ToolBarDestroy ( bar );

    if( gdiObjectsCreated ) {
        _wpi_deleteobject( blackPen );
        _wpi_deleteobject( btnShadowPen );
        _wpi_deleteobject( btnHighlightPen );
        _wpi_deleteobject( btnFacePen );
        _wpi_deleteobject( blackBrush );
        _wpi_deleteobject( btnFaceBrush );
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

    if( hInstCommCtrl == NULL ) {
#endif
        t = (tool *)MemAlloc( sizeof( tool ) );
        if( info->flags & ITEM_BLANK ) {
            t->u.blank_space = info->u.blank_space;
        } else {
            t->u.bitmap = info->u.bmp;
        }
        t->id = info->id;
        t->next = NULL;
        t->flags = info->flags;
        t->depressed = info->depressed;
        t->state = BUTTON_UP;
        addTool( &bar->tool_list, t );
        if( !(info->flags & ITEM_BLANK) && info->u.bmp != HNULL ) {
            createButtonList( bar->hwnd, bar, t );
        }
#ifdef __NT__
    } else {
        if( !(info->flags & ITEM_BLANK) ) {
            GetObject( info->u.bmp, sizeof( BITMAP ), &bm );
            SendMessage( bar->hwnd, TB_SETBITMAPSIZE, 0,
                MAKELONG( bm.bmWidth, bm.bmHeight ) );
            tbab.hInst = NULL;
            tbab.nID = (UINT_PTR)WPTB_CreateTransparentBitmap( info->u.bmp, bm.bmWidth,
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
        _wpi_invalidaterect( bar->hwnd, &t->area, FALSE );
#ifdef __NT__
    } else {
        SendMessage( bar->hwnd, TB_CHECKBUTTON, (WPARAM)id,
            MAKELONG( state & BUTTON_DOWN, 0 ) );
    }
#endif

}

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

}

/*
 * ToolBarDeleteItem - delete an item from the tool bar
 */
BOOL ToolBarDeleteItem( toolbar *bar, WORD id )
{
    tool        *t, *next;
#ifdef __NT__
    int         n;

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

#ifdef __OS2_PM__
static  WPI_PROC                oldFrameProc;
WPI_MRESULT CALLBACK FrameProc( HWND hwnd, WPI_MSG msg, WPI_PARAM1 wparam,
                                WPI_PARAM2 lparam )
{
    HWND        client;

    switch( msg ) {
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONDBLCLK:
        client = WinWindowFromID( hwnd, FID_CLIENT );
        if( client != NULLHANDLE ) {
#ifndef __OS2_PM__
            _wpi_sendmessage( client, msg, 0, (WPI_PARAM2)0xfffffff );
#else
            _wpi_sendmessage( client, msg, (WPI_PARAM1)0xfffffff, 0 );
#endif
        }
        break;
    case WM_SYSCOMMAND:
        if( SHORT1FROMMP(wparam) != SC_CLOSE ) {
            client = WinWindowFromID( hwnd, FID_CLIENT );
            if( client != NULLHANDLE ) {
                _wpi_sendmessage( client, msg, wparam, lparam );
            }
            break;
        }
        // fall through!!
    case WM_CLOSE:
        client = WinWindowFromID( hwnd, FID_CLIENT );
        if( client != NULLHANDLE ) {
            _wpi_sendmessage( client, WM_CLOSE, 0, 0 );
        }
        _wpi_destroywindow( hwnd );
        return( NULL );
    }
    return( _wpi_callwindowproc( oldFrameProc, hwnd, msg, wparam, lparam ) );
}
#endif
/*
 * ToolBarDisplay - create and show the tool bar
 */
void ToolBarDisplay( toolbar *bar, TOOLDISPLAYINFO *disp )
{
    int         height, width;

    currTool = NULL;
    currIsDown = FALSE;
    lastID = -1;
    mouse_captured = FALSE;

    if( bar->bgbrush != NULLHANDLE ) {
        _wpi_deleteobject( bar->bgbrush );
        bar->bgbrush = NULLHANDLE;
    }

    if( disp->background != NULLHANDLE ) {
        bar->bgbrush = _wpi_createpatternbrush( disp->background );
    }

    if( bar->hwnd != HNULL ) {
        if( _wpi_getcapture() == bar->hwnd ) {
            _wpi_releasecapture();
        }
        toolbardestroywindow( bar->hwnd );
    }

    bar->button_size = disp->button_size;
    bar->hook = disp->hook;
    bar->helphook = disp->helphook;
    bar->border = disp->border_size;
    bar->background = disp->background;
    bar->foreground = disp->foreground;
    bar->is_fixed = disp->is_fixed;
    width = _wpi_getwidthrect( (disp->area) );
    height = _wpi_getheightrect( (disp->area) );

#ifndef __OS2_PM__
#if defined(__NT__)
    if( hInstCommCtrl != NULL ) {
        if( !bar->is_fixed ) {
            bar->container = CreateWindowEx( WS_EX_TOOLWINDOW, containerClassName, NULL,
                WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, disp->area.left,
                disp->area.top, width, height, bar->owner, NULL,
                GET_HINSTANCE( bar->owner ), NULL );
            bar->hwnd = CreateWindow( TOOLBARCLASSNAME, NULL,
                                      WS_CHILD | WS_VISIBLE | TBSTYLE_WRAPABLE,
                                      0, 0, 0, 0, bar->container, NULL,
                                      GET_HINSTANCE( bar->owner ), NULL );
        } else {
            bar->hwnd = CreateWindow( TOOLBARCLASSNAME, NULL,
                                      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                      0, 0, 0, 0, bar->owner, NULL,
                                      GET_HINSTANCE( bar->owner ), NULL );
        }
        bar->old_wndproc = (WNDPROC)GetWindowLong( bar->hwnd, GWL_WNDPROC );
        SetProp( bar->hwnd, "bar", (LPVOID)bar );
        SetWindowLong( bar->hwnd, GWL_WNDPROC, (DWORD)WinToolWndProc );
        SendMessage( bar->hwnd, TB_BUTTONSTRUCTSIZE, sizeof( TBBUTTON ), 0L );
    } else if ( LOBYTE(LOWORD(GetVersion())) >= 4 && (bar->is_fixed) ) {
        CreateWindow( className, NULL, WS_CHILD, //( disp->style ),
            disp->area.left, disp->area.top, width, height,
            bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
    }
    else {
        if( LOBYTE(LOWORD(GetVersion())) >= 4 && ( !bar->is_fixed ) ) {
            CreateWindowEx( WS_EX_TOOLWINDOW, className, NULL, disp->style,
                disp->area.left, disp->area.top, width, height,
                bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
        } else {
            CreateWindow( className, NULL, disp->style,
                disp->area.left, disp->area.top, width, height,
                bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
        }
    }
#else
    CreateWindow( className, NULL, disp->style,
        disp->area.left, disp->area.top, width, height,
        bar->owner, (HMENU) HNULL, GET_HINSTANCE( bar->owner ), bar );
#endif

    /*
     * Windows ignores the GETMINMAXINFO before the WM_CREATE or
     * something so we kluge it.
     */
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        MoveWindow( bar->hwnd, disp->area.left, disp->area.top,
                    width, height, TRUE );
#ifdef __NT__
    }
#endif

#else  // It is __OS2_PM__
    {
        HWND    frame;
        HWND    parent;

        if ( disp->is_fixed ) {
            parent = bar->owner;
        } else {
            parent = HWND_DESKTOP;
        }
        frame = WinCreateStdWindow( parent, 0L, &(disp->style), NULL, "",
                                                0L, (HMODULE)0, 0, NULL);

        WinSetOwner( frame, bar->owner );
        oldFrameProc = _wpi_subclasswindow( frame, (WPI_PROC)FrameProc );

        WinSetPresParam( frame, PP_BACKGROUNDCOLORINDEX,
                                (ULONG)sizeof(LONG)+1, (PVOID)&btnColour );
        WinCreateWindow( frame, className, "", WS_VISIBLE, 0, 0, 0, 0, frame,
                                    HWND_TOP, FID_CLIENT, (PVOID)bar, NULL);

        WinSetWindowPos(frame, HWND_TOP, disp->area.xLeft,
                        disp->area.yBottom, width, height,
                        SWP_MOVE | SWP_SIZE | SWP_HIDE );
    }
#endif
} /* ToolBarDisplay */

/*
 * ToolBarWindow - return a handle to the toolbar window ... for PM this
 *                 is a handle to the frame window
 */
HWND ToolBarWindow( toolbar *bar )
{
    if( bar == NULL ) {
        return( HNULL );
    }
    return( _wpi_getframe(bar->hwnd) );

} /* ToolBarWindow */

/*
 * UpdateToolBar - update our tool bar
 */
void UpdateToolBar( toolbar *bar )
{
    _wpi_invalidaterect( bar->hwnd, NULL, TRUE );
    _wpi_updatewindow( bar->hwnd );

} /* UpdateToolBar */

/*
 * drawTopLeftCorner - draws the top left corner of a button
 */
static void drawTopLeftCorner( WPI_PRES pres, WPI_POINT size, int border,
                                                                HPEN pen )

{
    HPEN        old_pen;
    WPI_POINT   pt;

    old_pen = _wpi_selectobject( pres, pen );
    _wpi_setpoint( &pt, border, size.y - 2*border );
    _wpi_cvth_pt( &pt, size.y );
    _wpi_movetoex( pres, &pt, NULL );

    _wpi_setpoint( &pt, border, border );
    _wpi_cvth_pt( &pt, size.y );
    _wpi_lineto( pres, &pt );

    _wpi_setpoint( &pt, size.x - border, border );
    _wpi_cvth_pt( &pt, size.y );
    _wpi_lineto( pres, &pt );
    _wpi_selectobject( pres, old_pen );

} /* drawTopLeftCorner */

/*
 * drawTopLeftInsideCorner - draws the top left corner of a button
 */
static void drawTopLeftInsideCorner( WPI_PRES pres, WPI_POINT size,
                                                    int border, HPEN pen )

{
    HPEN        old_pen;
    WPI_POINT   pt;

    old_pen = _wpi_selectobject( pres, pen );
    _wpi_setpoint( &pt, border*2, size.y - 2*border );
    _wpi_cvth_pt( &pt, size.y );
    _wpi_movetoex( pres, &pt, NULL );

    _wpi_setpoint( &pt, border*2, border*2 );
    _wpi_cvth_pt( &pt, size.y );
    _wpi_lineto( pres, &pt );

    pt.x = size.x - border;
    _wpi_lineto( pres, &pt );
    _wpi_selectobject( pres, old_pen );
} /* drawTopLeftCorner */

/*
 * drawBottomRightCorner - draws the bottom right corner of a button
 */
static void drawBottomRightCorner( WPI_PRES pres, WPI_POINT size, int border,
                                                                HPEN pen )
{
    HPEN        old_pen;
    WPI_POINT   pt;
    int         height;

    height = size.y;
    size.x -= 2*border;
    size.y -= 2*border;
    old_pen = _wpi_selectobject( pres, pen );
    _wpi_setpoint( &pt, size.x, border );
    _wpi_cvth_pt( &pt, height );
    _wpi_movetoex( pres, &pt, NULL );

    _wpi_setpoint( &pt, size.x, size.y );
    _wpi_cvth_pt( &pt, height );
    _wpi_lineto( pres, &pt );

    _wpi_setpoint( &pt, border-1, size.y );
    _wpi_cvth_pt( &pt, height );
    _wpi_lineto( pres, &pt );
    _wpi_selectobject( pres, old_pen );

} /* drawBottomRightCorner */

/*
 * drawBottomRightInsideCorner - draw the inside corner of a button
 */
static void drawBottomRightInsideCorner( WPI_PRES pres, WPI_POINT size,
                                                    int border, HPEN pen )
{
    HPEN        old_pen;
    WPI_POINT   pt;
    int         height;

    height = size.y;
    size.x -= 3*border;
    size.y -= 3*border;
    old_pen = _wpi_selectobject( pres, pen );
    _wpi_setpoint( &pt, 2*border, size.y );
    _wpi_cvth_pt( &pt, height );
    _wpi_movetoex( pres, &pt, NULL );

    _wpi_setpoint( &pt, size.x, size.y );
    _wpi_cvth_pt( &pt, height );
    _wpi_lineto( pres, &pt );

    _wpi_setpoint( &pt, size.x, border );
    _wpi_cvth_pt( &pt, height );
    _wpi_lineto( pres, &pt );
    _wpi_selectobject( pres, old_pen );

} /* drawBottomRightInsideCorner */

/*
 * drawBorder - draw to border of a button
 */
static void drawBorder( WPI_PRES pres, WPI_POINT size, int border )
{
    HPEN        old_pen;
    int         x,y;
    WPI_POINT   pt;

    y = _wpi_cvth_y( size.y-1, size.y );
    x = size.x-1;

    old_pen = _wpi_selectobject( pres, blackPen );

    _wpi_setpoint( &pt, 0, _wpi_cvth_y(0, size.y) );
    _wpi_movetoex( pres, &pt, NULL );

    pt.x = x;
    _wpi_lineto( pres, &pt );
    pt.y = y;
    _wpi_lineto( pres, &pt );
    pt.x = 0;
    _wpi_lineto( pres, &pt );
    pt.y = _wpi_cvth_y(0, size.y);
    _wpi_lineto( pres, &pt );

    _wpi_selectobject( pres, btnFacePen );
    _wpi_setpoint( &pt, 0, _wpi_cvth_y(0, size.y) );
    _wpi_movetoex( pres, &pt, NULL );
    pt.x = border;
    if (!round_corners) {
        pt.x -= 1;
    }
    _wpi_lineto( pres, &pt );
    pt.x = x;
    _wpi_movetoex( pres, &pt, NULL );
    pt.y = _wpi_cvth_y( border, size.y );
    if (!round_corners) {
        pt.y += 1;
    }
    _wpi_lineto( pres, &pt );

    pt.y = y;
    _wpi_movetoex( pres, &pt, NULL );
    pt.x = x - border;
    if (!round_corners) {
        pt.x += 1;
    }
    _wpi_lineto( pres, &pt );
    pt.x = 0;
    _wpi_movetoex( pres, &pt, NULL );
    pt.y = _wpi_cvth_y( _wpi_cvth_y(y, size.y) - border, size.y );
    if (!round_corners) {
       pt.y -= 1;
    }
    _wpi_lineto( pres, &pt );

    _wpi_selectobject( pres, old_pen );

} /* drawBorder */

/*
 * toolBarDrawBitmap - draw the bitmap on a button
 */
static void toolBarDrawBitmap( WPI_PRES pres, WPI_POINT dst_size,
                                        WPI_POINT dst_org, HBITMAP bitmap )
{
    WPI_BITMAP          bm;
    WPI_PRES            mempres;
    HDC                 memdc;
    WPI_POINT           src_org, src_size;
    HBRUSH              old_brush;
    HBITMAP             old_bmp;

    DPtoLP( pres, &dst_size, 1 );
    DPtoLP( pres, &dst_org, 1 );

    mempres = _wpi_createcompatiblepres( pres, appInst, &memdc );
    old_bmp = _wpi_selectbitmap( mempres, bitmap );

    _wpi_getbitmapstruct( bitmap, &bm );

    src_size.x = _wpi_bitmapwidth( &bm );
    src_size.y = _wpi_bitmapheight( &bm );
    DPtoLP( pres, &src_size, 1 );

    src_org.x = 0;
    src_org.y = 0;
    DPtoLP( pres, &src_org, 1 );

    #if defined(__NT__) || defined(__WINDOWS__)
        SetStretchBltMode( pres, COLORONCOLOR );
    #else
        SetStretchBltMode( pres, STRETCH_DELETESCANS );
    #endif

    /*
     * If it's a monochrome bitmap try and do the right thing - I can see
     * this pissing off some users, but oh well.
     */
    if( _wpi_bitmapbitcount( &bm ) == 1 && _wpi_bitmapplanes( &bm ) == 1 ) {
        old_brush = _wpi_selectobject( pres, blackBrush );

        _wpi_stretchblt( pres, dst_org.x, dst_org.y, dst_size.x, dst_size.y,
            mempres, src_org.x, src_org.y, src_size.x, src_size.y, 0xb8074a );

        _wpi_getoldbitmap( mempres, old_bmp );
        _wpi_selectobject( pres, old_brush );
    } else {
        _wpi_stretchblt( pres, dst_org.x, dst_org.y, dst_size.x, dst_size.y,
            mempres, src_org.x, src_org.y, src_size.x, src_size.y, SRCCOPY );

        _wpi_getoldbitmap( mempres, old_bmp );
    }
    _wpi_deletecompatiblepres( mempres, memdc );

} /* toolBarDrawBitmap */

/*
 * drawButton - draw a button on the toolbar
 */
static void drawButton( HWND hwnd, tool *tool, BOOL down,
                        WPI_PRES pres, WPI_PRES mempres, HDC mem )
{
    toolbar     *bar;
    HBRUSH      brush;
    HBITMAP     bitmap, oldbmp;
    int         shift;
    BOOL        selected;
    WPI_POINT   dst_size;
    WPI_POINT   dst_org;
    HBITMAP     used_bmp;
    TOOLBR_DIM  left;
    TOOLBR_DIM  right;
    TOOLBR_DIM  top;
    TOOLBR_DIM  bottom;
    BOOL        delete_pres;
    BOOL        delete_mempres;
#if defined(__NT__) || defined(__WINDOWS__)
    HBITMAP     bitmap2, oldbmp2 , bmptmp;
    HDC         mem2;
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
        shift = 2*BORDER_WIDTH( bar );
    }

    delete_pres = FALSE;
    delete_mempres = FALSE;
    if( pres == NULLHANDLE ) {
        pres = _wpi_getpres( hwnd );
        mempres = NULLHANDLE;
        delete_pres = TRUE;
    }
    if( ( mempres == NULLHANDLE ) || ( mem == NULLHANDLE ) ) {
        mempres = _wpi_createcompatiblepres( pres, appInst, &mem );
        delete_mempres = TRUE;
    }
    bitmap = _wpi_createcompatiblebitmap( pres, bar->button_size.x,
                                            bar->button_size.y );
    oldbmp = _wpi_selectbitmap( mempres, bitmap );
#if defined(__NT__) || defined(__WINDOWS__)
    mem2 = CreateCompatibleDC( pres );
    bitmap2 = CreateCompatibleBitmap( pres,
                bar->button_size.x, bar->button_size.y );
    oldbmp2 = SelectObject( mem2, bitmap2 );
#endif

    brush = btnFaceBrush;
    if( selected && bar->bgbrush != HNULL ) {
        brush = bar->bgbrush;
    }
    /* Does this do anything ??? */
    /* _wpi_fillrect( mempres, &tool->area, btnColour, brush ); */

    dst_size = bar->button_size;
    dst_size.x -= 4 * BORDER_WIDTH( bar );
    dst_size.y -= 4 * BORDER_WIDTH( bar );
    dst_org.x = (1 * BORDER_WIDTH( bar ))+shift;
    dst_org.y = (1 * BORDER_WIDTH( bar ))+shift;
#ifdef __OS2_PM__
    dst_org.y = _wpi_cvth_y( dst_org.y, (bar->button_size.y) );
    dst_org.y = dst_org.y - dst_size.y + 1;
#endif
    used_bmp = tool->u.bitmap;
    if( selected ) {
        // if the button is selected and it has the ITEM_DOWNBMP flag
        // then we draw the alternate bitmap instead.
        if( tool->flags & ITEM_DOWNBMP ) {
            used_bmp = tool->depressed;
        }
    }
    toolBarDrawBitmap( mempres, dst_size, dst_org, used_bmp );

#if defined(__NT__) || defined(__WINDOWS__)
    /* New, on WIN32 platforms, use WPTB_TransparentBlt() */
    /* Get background color of button bitmap */
    bmptmp = SelectObject(mem2, used_bmp);
    /* Expects 0,0 pix in original to be in background color */
    cr = GetPixel(mem2, 0, 0);
    bmptmp = SelectObject(mem2, bmptmp);
    /* IMPORTANT: must set required new background color for dest bmp */
    SetBkColor(mem2, GetSysColor(COLOR_BTNFACE));
    WPTB_TransparentBlt( mem2,    dst_org.x,  dst_org.y,
                                  dst_size.x, dst_size.y,
                         mempres, cr);
    if( oldbmp != HNULL ) {
        SelectObject( mempres, oldbmp );
        DeleteObject(bitmap);
    }
    if( delete_mempres ) {
        DeleteDC(mempres);
    }
    /* Switch new bitmap into vars expected by code below */
    mempres = mem2;
    delete_mempres = TRUE;
    bitmap = bitmap2;
    oldbmp = oldbmp2;
#endif

    drawBorder( mempres, bar->button_size, BORDER_WIDTH( bar ) );
    if( selected ) {
        drawTopLeftCorner( mempres, bar->button_size, BORDER_WIDTH( bar ),
                btnShadowPen );
        drawTopLeftInsideCorner( mempres, bar->button_size,
                                        BORDER_WIDTH( bar ), btnFacePen );
    } else {
        drawTopLeftCorner( mempres, bar->button_size, BORDER_WIDTH( bar ),
                btnHighlightPen );
        drawBottomRightCorner( mempres, bar->button_size, BORDER_WIDTH( bar ),
                btnShadowPen );
        drawBottomRightInsideCorner( mempres, bar->button_size,
                                        BORDER_WIDTH( bar ), btnShadowPen );

    }
    _wpi_getrectvalues( (tool->area), &left, &top, &right, &bottom );
    _wpi_bitblt( pres, left, top, bar->button_size.x, bar->button_size.y,
            mempres, 0, 0, SRCCOPY );           /* copy it to screen */
    _wpi_getoldbitmap( mempres, oldbmp );
    if( delete_pres ) {
        _wpi_releasepres( hwnd, pres );
    }
    _wpi_deletebitmap( bitmap );
    if( delete_mempres ) {
        _wpi_deletecompatiblepres( mempres, mem );
    }

} /* drawButton */

/*
 * isPointInToolbar - is the point in the mousemove message in the toolbar
 */
BOOL isPointInToolbar( HWND hwnd, WPI_PARAM1 wparam, WPI_PARAM2 lparam )
{
    WPI_POINT   p;
    WPI_RECT    r;

    lparam = lparam;            // to suppress compiler warnings for PM
    wparam = wparam;            // to suppress compiler warnings for Win

    WPI_MAKEPOINT( wparam, lparam, p );
    _wpi_getclientrect( hwnd, &r );
    if( _wpi_ptinrect( &r, p ) ) {
        return( TRUE );
    }
    return( FALSE );
} /* isPointInToolbar */

/*
 * findToolAtPoint - find a tool at a given point
 */
static tool *findToolAtPoint( toolbar *bar, WPI_PARAM1 wparam, WPI_PARAM2 lparam )
{
    WPI_POINT   p;
    tool        *tool;

    lparam = lparam;            // to suppress compiler warnings for PM
    wparam = wparam;            // to suppress compiler warnings for Win

    WPI_MAKEPOINT( wparam, lparam, p );
    for( tool = bar->tool_list; tool != NULL; tool = tool->next ) {
        if( _wpi_ptinrect( &tool->area, p ) ) {
            if( tool->flags & ITEM_BLANK ) {
                return( NULL );
            } else {
                return( tool );
            }
        }
    }
    return( NULL );

} /* findToolAtPoint */

/*
 * customHitTest - find a tool at a given point for a native toolbar
 */
#ifdef __NT__
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
}
#endif

/*
 * HasToolAtPoint - return TRUE if tool exists at a given point
 */
BOOL HasToolAtPoint( struct toolbar *bar, WPI_PARAM1 wparam, WPI_PARAM2 lparam )
{
#ifdef __NT__
    POINT   pt;
    
    if( hInstCommCtrl == NULL ) {
#endif
        return( findToolAtPoint( bar, wparam, lparam ) != NULL );
#ifdef __NT__
    } else {
        pt.x = LOWORD( lparam );
        pt.y = HIWORD( lparam );
        return( customHitTest( bar, &pt ) >= 0 );
    }
#endif

} /* HasToolAtPoint */

/*
 * ToolBarWndProc - callback routine for the tool bar
 */
MRESULT CALLBACK ToolBarWndProc( HWND hwnd, WPI_MSG msg, WPI_PARAM1 wparam,
                                                        WPI_PARAM2 lparam )
{
    CREATESTRUCT        __FAR   *cs;
    toolbar             *bar;
    tool                *tool;
    WPI_RECT            inter;
    WPI_PRES            pres;
    WPI_PRES            mempres;
    HDC                 memdc;
    PAINTSTRUCT         ps;
    BOOL                posted;

    bar = GET_INFO( hwnd );
    if( msg == WM_CREATE ) {
#ifndef __OS2_PM__
        cs = MAKEPTR( lparam );
        bar = (toolbar *)cs->lpCreateParams;
        #ifdef __WINDOWS_386__
            bar = MapAliasToFlat( (DWORD) bar );
        #endif
#else
        cs = PVOIDFROMMP( wparam );
        bar = (toolbar *)cs;
        WinSetPresParam( hwnd, PP_BACKGROUNDCOLORINDEX,
                                (ULONG)sizeof(LONG)+1, (PVOID)&btnColour );
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
            _wpi_invalidaterect( hwnd, NULL, TRUE );
            _wpi_updatewindow( hwnd );
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        if( bar && bar->tool_list ) {
            currTool = NULL;
            tool = findToolAtPoint( bar, wparam, lparam );
            if( tool ) {
                if( bar->hook != NULL ) {
                    bar->hook( hwnd, WM_USER, MPFROMSHORT(tool->id),
                                                        (WPI_PARAM2)0 );
                }
                currTool = tool;
                drawButton( hwnd, tool, TRUE, NULLHANDLE, NULLHANDLE, NULLHANDLE );
                mouse_captured = TRUE;
                _wpi_setcapture( hwnd );
                currIsDown = TRUE;
                if( bar->helphook != NULL ) {
                    bar->helphook( hwnd, MPFROMSHORT(currTool->id), TRUE );
                }
            }
        }
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        if( bar && bar->tool_list ) {
            tool = findToolAtPoint( bar, wparam, lparam );
            posted = FALSE;
            if( tool != NULL ) {
                if( tool == currTool ) {
                    _wpi_postmessage( bar->owner, WM_COMMAND, tool->id, CMDSRC_MENU );
                    posted = TRUE;
                    drawButton( hwnd, tool, FALSE, NULLHANDLE, NULLHANDLE, NULLHANDLE );
                }
            }
            if( !posted && bar->hook != NULL ) {
                if( currTool != NULL ) {
                    bar->hook( hwnd, WM_USER, MPFROMSHORT(currTool->id),
                                                            (WPI_PARAM2)1 );
                }
            }
            if( currTool != NULL ) {
                mouse_captured = FALSE;
                ignore_mousemove = TRUE; // release_capture generates a
                                         // WM_MOUSEMOVE
                _wpi_releasecapture();
                if( bar->helphook != NULL ) {
                    bar->helphook( hwnd, MPFROMSHORT(currTool->id), FALSE );
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
        tool = findToolAtPoint( bar, wparam, lparam );
        if( currTool ) {
            if( tool == currTool ) {
                if( !currIsDown ) {
                    currIsDown = TRUE;
                    drawButton( hwnd, currTool, TRUE, NULLHANDLE, NULLHANDLE, NULLHANDLE );
                    if( bar->helphook != NULL ) {
                        bar->helphook( hwnd, MPFROMSHORT(currTool->id), TRUE );
                    }
                }
            } else {
                if( currIsDown ) {
                    drawButton( hwnd, currTool, FALSE, NULLHANDLE, NULLHANDLE, NULLHANDLE );
                    currIsDown = FALSE;
                    if( bar->helphook != NULL ) {
                        bar->helphook( hwnd, MPFROMSHORT(currTool->id), FALSE );
                    }
                }
            }
        } else {
            if( bar->helphook != NULL ) {
                if( tool ) {
                    bar->helphook( hwnd, MPFROMSHORT(tool->id), TRUE );
                    lastID = tool->id;
                } else if( lastID != (WORD)-1 ) {
                    bar->helphook( hwnd, MPFROMSHORT(lastID), FALSE );
                    lastID = -1;
                }
            }
        }
        break;
#if defined(__NT__) || defined(__WINDOWS__)
    case WM_SYSCOLORCHANGE: {
        COLORREF    clr_btnface;
        COLORREF    clr_btnshadow;
        COLORREF    clr_btnhighlight;
        COLORREF    clr_black;

        if( gdiObjectsCreated ) {
            _wpi_deleteobject( blackPen );
            _wpi_deleteobject( btnShadowPen );
            _wpi_deleteobject( btnHighlightPen );
            _wpi_deleteobject( btnFacePen );
            _wpi_deleteobject( blackBrush );
            _wpi_deleteobject( btnFaceBrush );
            gdiObjectsCreated = FALSE;
        }
        clr_btnshadow = GetSysColor( COLOR_BTNSHADOW );
        clr_btnhighlight = GetSysColor( COLOR_BTNHIGHLIGHT );
        btnColour = GetSysColor( COLOR_BTNFACE );
        clr_btnface = btnColour;
        clr_black = GetSysColor(COLOR_BTNTEXT);
        if( !gdiObjectsCreated ) {
            blackPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_black );
            btnShadowPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnshadow );
            btnHighlightPen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnhighlight );
            btnFacePen = _wpi_createpen( PS_SOLID, BORDER_WIDTH(bar), clr_btnface );
            blackBrush = _wpi_createsolidbrush( clr_black );
            btnFaceBrush = _wpi_createsolidbrush( clr_btnface );
            gdiObjectsCreated = TRUE;
        }
        }
        break;
#endif
    case WM_PAINT:
        pres = _wpi_beginpaint( hwnd, NULLHANDLE, &ps );
        mempres = _wpi_createcompatiblepres( pres, appInst, &memdc );
#ifdef __OS2_PM__
        WinFillRect( pres, &ps, CLR_PALEGRAY );
        for( tool = bar->tool_list; tool != NULL; tool = tool->next ) {
            if( _wpi_intersectrect( appInst, &inter, &ps, &tool->area ) ) {
#else
        /* First non comment line below inserted as test by RR 2003.10.26 */
        /* Ref PM implementation above, and WM_PAINT: handler in toolbr.c */
        _wpi_fillrect( pres, &ps.rcPaint, clr_btnface, btnFaceBrush );
        for( tool = bar->tool_list; tool != NULL; tool = tool->next ) {
            if( _wpi_intersectrect( appInst, &inter, &ps.rcPaint, &tool->area ) ) {
#endif
                drawButton( hwnd, tool, FALSE, pres, mempres, memdc );
            }
        }
        _wpi_deletecompatiblepres( mempres, memdc );
        _wpi_endpaint( hwnd, NULLHANDLE, &ps );
        break;

#ifndef __OS2_PM__
    case WM_ERASEBKGND:
        InvalidateRect( hwnd, NULL, FALSE );
        /* Resulting WM_PAINT will fill entire area, */
        /* so avoid default handler kicking in. */
        return 1;
#endif

    case WM_DESTROY:
        bar->hwnd = HNULL;
        break;
    }
    return( _wpi_defwindowproc( hwnd, msg, wparam, lparam ) );

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
        pt.x = LOWORD( lparam );
        pt.y = HIWORD( lparam );
        if( (id = customHitTest( bar, &pt )) != -1 ) {
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
    HWND    otherwnd;
    
    switch( msg ) {
    case WM_SIZE:
        otherwnd = GetWindow( hwnd, GW_CHILD );
        SendMessage( otherwnd, WM_SIZE, 0, 0L );
        break;
    case WM_COMMAND:
        otherwnd = GetWindow( hwnd, GW_OWNER );
        SendMessage( otherwnd, WM_COMMAND, wparam, lparam );
    }
    return( DefWindowProc( hwnd, msg, wparam, lparam ) );
} /* ToolContainerWndProc */

#endif

/*
 * ChangeToolButtonBitmap - change a bitmap for a toolbar item
 */
void ChangeToolButtonBitmap( toolbar *bar, WORD id, HBITMAP newbmp )
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
            t->u.bitmap = newbmp;
            _wpi_invalidaterect( bar->hwnd, &t->area, TRUE );
            _wpi_updatewindow( bar->hwnd );
        }
#ifdef __NT__
    } else {
        n = SendMessage( bar->hwnd, TB_COMMANDTOINDEX, (WPARAM)id, 0L );
        if( n >= 0 ) {
            SendMessage( bar->hwnd, TB_GETBUTTON, (WPARAM)n, (LPARAM)&tbb );
            tbab.hInst = NULL;
            tbab.nID = (UINT_PTR)WPTB_CreateTransparentBitmap( newbmp,
                bar->button_size.x - bar->border.x,
                bar->button_size.y - bar->border.y - 2 );
            tbb.iBitmap = (int)SendMessage( bar->hwnd, TB_ADDBITMAP, 1, (LPARAM)&tbab );
            SendMessage( bar->hwnd, TB_DELETEBUTTON, n, 0L );
            SendMessage( bar->hwnd, TB_INSERTBUTTON, n, (LPARAM)&tbab );
        }
    }
#endif

} /* ChangeToolButtonBitmap */


/********** Below - new inserted 2003.10.31 ********************/

#if defined(__NT__) || defined(__WINDOWS__)

/*
 * WPTB_TransparentBlt
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
 *             hDCIn    HDC      source, to copy from
 *             cr       COLORREF to consider as transparent in source.
 *
 * Return Value: None
 */

#define ROP_DSPDxax  0x00E20746

void WPTB_TransparentBlt (HDC hDC,   UINT x, UINT y, UINT width, UINT height,
                          HDC hDCIn, COLORREF cr)
{
   HDC hDCMid, hMemDC;
   HBITMAP hBmpMono, hBmpT;
   HBRUSH hBr, hBrT;
   COLORREF crBack, crText;

   if (NULL == hDCIn)
      return;

   /* Make two intermediate DC's */
   hDCMid = CreateCompatibleDC (hDC);
   hMemDC = CreateCompatibleDC (hDC);

   /* Create a monochrome bitmap for masking */
   hBmpMono = CreateCompatibleBitmap (hDCMid, x + width, y + height);
   SelectObject (hDCMid, hBmpMono);

   /* Create a mid-stage bitmap */
   hBmpT = CreateCompatibleBitmap (hDC, x + width, y + height);
   SelectObject (hMemDC, hBmpT);

   /* Create a monochrome mask where we have 0's in the image, 1's elsewhere. */
   crBack = SetBkColor (hDCIn, cr);
   BitBlt (hDCMid, x, y, width, height, hDCIn, x, y, SRCCOPY);
   SetBkColor (hDCIn, crBack);

   /* Put the unmodified image in the temporary bitmap */
   BitBlt (hMemDC, x, y, width, height, hDCIn, x, y, SRCCOPY);

   /* Create an select a brush of the background color */
   hBr = CreateSolidBrush (GetBkColor (hDC));
   hBrT = SelectObject (hMemDC, hBr);

   /* Force conversion of the monochrome to stay black and white. */
   crText = SetTextColor (hMemDC, 0L);
   crBack = SetBkColor (hMemDC, RGB (255, 255, 255));

   /*
    * Where the monochrome mask is 1, Blt the brush; where the mono
    * mask is 0, leave the destination untouched.  This results in
    * painting around the image with the background brush.  We do this
    * first in the temporary bitmap, then put the whole thing to the
    * screen (avoids flicker).
    */
   BitBlt (hMemDC, x, y, width, height, hDCMid, x, y, ROP_DSPDxax);
   BitBlt (hDC, x, y, width, height, hMemDC, x, y, SRCCOPY);

   SetTextColor (hMemDC, crText);
   SetBkColor (hMemDC, crBack);

   SelectObject (hMemDC, hBrT);
   DeleteObject (hBr);

   DeleteDC (hMemDC);
   DeleteDC (hDCMid);
   DeleteObject (hBmpT);
   DeleteObject (hBmpMono);

}  /* TansparentBlt () */

#endif

#ifdef __NT__

/*
 * WPTB_CreateTransparentBitmap()
 *
 * Purpose: Creates a transparent bitmap for use with Win32 native toolbars.
 *
 * Parameters: hBitmap  HBITMAP original bitmap
 *             width    int     width of the original bitmap
 *             height   int     height of the original bitmap
 *
 * Return Value: Handle to the new transparent bitmap.
 */

HBITMAP WPTB_CreateTransparentBitmap( HBITMAP hBitmap, int width, int height )
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
    WPTB_TransparentBlt( hDC2, 0, 0, width, height, hDC1, GetPixel( hDC1, 0, 0 ) );
    SelectObject( hDC1, hOldBitmap1 );
    SelectObject( hDC2, hOldBitmap2 );
    DeleteDC( hDC1 );
    DeleteDC( hDC2 );

    return( hNewBitmap );
}

#endif

