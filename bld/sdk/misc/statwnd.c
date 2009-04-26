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
* Description:  Status window for Windows.
*
****************************************************************************/


#include <windows.h>
#include <string.h>
#ifdef __NT__
    #include <commctrl.h>
#endif
#include "statwnd.h"

extern LPVOID   MemAlloc( unsigned );
extern void     MemFree( LPVOID );

#ifndef MAX_SECTIONS
#define MAX_SECTIONS    20
#endif

static char                     *className = "StatusWnd";
static int                      numSections = 0;
static status_block_desc        sectionDesc[MAX_SECTIONS];
static LPSTR                    sectionData[MAX_SECTIONS + 1];
static UINT                     sectionDataFlags[MAX_SECTIONS + 1];
static HFONT                    sectionDataFont;
#if defined( __NT__ )
static HFONT                    systemDataFont;
#endif
static HPEN                     penLight;
static HPEN                     penShade;
static HBRUSH                   brushButtonFace;
static COLORREF                 colorButtonFace;
static COLORREF                 colorTextFace;
static statushook               statusWndHookFunc;
static RECT                     statusRect;
static BOOL                     hasGDIObjects = FALSE;
static HWND                     stat = NULL;
#ifdef __NT__
static HINSTANCE                hInstCommCtrl;

typedef VOID    (WINAPI *PFNICC)( VOID );

static PFNICC   pfnInitCommonControls;
#endif

#if defined(__WINDOWS_386__)
#define CB      FAR PASCAL
#elif defined(__WINDOWS__)
#define CB      __export FAR PASCAL
#elif defined(__NT__)
#define CB      __export __stdcall
#endif

/*
 * getRect - get a rectangle
 */
static void getRect( RECT *r, int i )
{
    WORD        pos;
    WORD        width;

    *r = statusRect;
    width = statusRect.right - statusRect.left;
    if( i > 0 ) {
        if( sectionDesc[i - 1].width_is_percent ) {
            pos = (WORD) (((DWORD) width * (DWORD) sectionDesc[i].width) / 100L);
        } else {
            pos = sectionDesc[i - 1].width;
        }
        r->left = pos + sectionDesc[i - 1].separator_width;
    }
    if( i == numSections ) {
        pos = statusRect.right;
    } else if( sectionDesc[i].width_is_percent ) {
        pos = (WORD) (((DWORD) (statusRect.right - statusRect.left)
                    * (DWORD) sectionDesc[i].width) / 100L);
    } else {
        pos = sectionDesc[i].width;
    }
    r->right = pos;

} /* getRect */

static HFONT    oldFont;
static HBRUSH   oldBrush;
static COLORREF oldBkColor;
static COLORREF oldTextColor;

/*
 * initHDC - initialize our HDC for drawing text
 */
static char initHDC( HDC hdc )
{
    if( sectionDataFont == NULL ) {
        return( FALSE );
    }
#if defined (__NT__)
    oldFont = SelectObject( hdc, systemDataFont );
#else
    oldFont = SelectObject( hdc, sectionDataFont );
#endif
    oldBrush = SelectObject( hdc, brushButtonFace );
    oldBkColor = GetBkColor( hdc );
    oldTextColor = GetTextColor( hdc );
    SetBkColor( hdc, colorButtonFace );
    SetTextColor( hdc, colorTextFace );
    return( TRUE );

} /* initHDC */

/*
 * finiHDC - finished with our HDC
 */
static void finiHDC( HDC hdc )
{
    SelectObject( hdc, oldBrush );
    SelectObject( hdc, oldFont );
    SetBkColor( hdc, oldBkColor );
    SetTextColor( hdc, oldTextColor );

} /* finiHDC */

/*
 * makeInsideRect - make a rectangle the inside of a rectangle
 */
static void makeInsideRect( RECT *r )
{
    r->left += BORDER_SIZE;
    r->right -= BORDER_SIZE;
    r->top += BORDER_SIZE;
    r->bottom -= BORDER_SIZE;

} /* makeInsideRect */

/*
 * outlineRect - draw the outline of a rectangle
 */
static void outlineRect( HDC hdc, RECT *r )
{
    MoveToEx( hdc, r->left, r->bottom - 1, NULL );
    LineTo( hdc, r->right - 1, r->bottom - 1 );
    LineTo( hdc, r->right - 1, r->top );
    SelectObject( hdc, penShade );
    LineTo( hdc, r->left, r->top );
    LineTo( hdc, r->left, r->bottom - 1 );
    SelectObject( hdc, penLight );

} /* outlineRect */

/*
 * StatusWndCallback - handle messages for
 */
LONG CB StatusWndCallback( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam  )
{
    PAINTSTRUCT ps;
    RECT        r;
    int         i;

    if( statusWndHookFunc != NULL ) {
        if( statusWndHookFunc( hwnd, msg, wparam, lparam ) ) {
            return( 0 );
        }
    }
    switch( msg ) {
    case WM_SIZE:
        GetClientRect( hwnd, &statusRect );
        InflateRect( &statusRect, -HORZ_BORDER, -VERT_BORDER );
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
    case WM_PAINT:
        BeginPaint( hwnd, &ps );
        StatusWndDraw3DBox( ps.hdc );
        if( initHDC( ps.hdc ) ) {
            for( i = 0; i <= numSections; i++ ) {
                if( sectionData[i] != NULL ) {
                    getRect( &r, i );
                    makeInsideRect( &r );
                    DrawText( ps.hdc, sectionData[i], -1, &r, sectionDataFlags[i] );
                }
            }
            finiHDC( ps.hdc );
        }
        EndPaint( hwnd, &ps );
        break;
#if defined( __NT__ )
    case WM_SYSCOLORCHANGE:
        if( hasGDIObjects ) {
            DeleteObject( penLight );
            DeleteObject( penShade );
            DeleteObject( brushButtonFace );
            hasGDIObjects = FALSE;
        }
        if( !hasGDIObjects ) {
            colorButtonFace = GetSysColor( COLOR_BTNFACE );
            colorTextFace = GetSysColor( COLOR_BTNTEXT );
            brushButtonFace = CreateSolidBrush( colorButtonFace );
            penLight = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNHIGHLIGHT ) );
            penShade = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNSHADOW ) );
            hasGDIObjects = TRUE;
        }
    break;
#endif
    case WM_ERASEBKGND:
#if defined( __NT__ )
        if( colorButtonFace != GetSysColor( COLOR_BTNFACE ) ) {
            /* WM_SYSCOLORCHANGED: not received by this window.
               Have to fake it...  */
            SendMessage( hwnd, WM_SYSCOLORCHANGE, (WPARAM)0, (LPARAM)0 );
        }
#endif
        GetClientRect( hwnd, &r );
        UnrealizeObject( brushButtonFace );
        FillRect( (HDC)wparam, &r, brushButtonFace );
        break;
    default:
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
    }
    return( 0 );

} /* StatusWndCallback */

/*
 * StatusWndInit - initialize for using the status window
 */
int StatusWndInit( HINSTANCE hinstance, statushook hook, int extra,
                   HCURSOR hDefaultCursor )
{
    WNDCLASS    wc;
    int         rc;

#ifdef __NT__
    if( (hInstCommCtrl = GetModuleHandle( "COMCTL32.DLL" )) != NULL ) {
        pfnInitCommonControls = (PFNICC)GetProcAddress( hInstCommCtrl,
                                                        "InitCommonControls" );
        pfnInitCommonControls();
        return( 1 );
    } else {
#endif
        if( !hasGDIObjects ) {
            colorButtonFace = GetSysColor( COLOR_BTNFACE );
            colorTextFace = GetSysColor( COLOR_BTNTEXT );
            brushButtonFace = CreateSolidBrush( colorButtonFace );
            penLight = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNHIGHLIGHT ) );
            penShade = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNSHADOW ) );
            hasGDIObjects = TRUE;
        }

        statusWndHookFunc = hook;

        rc = TRUE;
        if( !GetClassInfo( hinstance, className, &wc ) ) {
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = (WNDPROC)StatusWndCallback;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = extra;
            wc.hInstance = hinstance;
            wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
            wc.hCursor = hDefaultCursor;
            wc.hbrBackground = (HBRUSH) 0;
            wc.lpszMenuName = NULL;
            wc.lpszClassName = className;
            rc = RegisterClass( &wc );
        }
        return( rc );
#ifdef __NT__
    }
#endif
} /* StatusWndInit */

/*
 * StatusWndChangeSysColors - fiddle with what StatusWnd believes
 *                            are the system colours
 */
void StatusWndChangeSysColors( COLORREF btnFace, COLORREF btnText,
                               COLORREF btnHighlight, COLORREF btnShadow )
{
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        if( hasGDIObjects ) {
            DeleteObject( penLight );
            DeleteObject( penShade );
            DeleteObject( brushButtonFace );
        }

        colorButtonFace = btnFace;
        colorTextFace = btnText;
        brushButtonFace = CreateSolidBrush( btnFace );
        penLight = CreatePen( PS_SOLID, 1, btnHighlight );
        penShade = CreatePen( PS_SOLID, 1, btnShadow );
        hasGDIObjects = TRUE;
#ifdef __NT__
    }
#endif
}

/*
 * updateParts - update the parts of a native status bar
 */
#ifdef __NT__
static void updateParts() {
    int     i;
    RECT    rc;
    int     width;
    int     *parts;

    parts = (int *)MemAlloc( sizeof( int ) * (numSections + 1) );
    GetClientRect( stat, &rc );
    width = rc.right - rc.left;
    for( i = 0; i < numSections; i++ ) {
        if( sectionDesc[i].width_is_percent ) {
            parts[i] = sectionDesc[i].width * width / 100;
        } else {
            parts[i] = sectionDesc[i].width;
        }
        if( i > 0 && parts[i] != -1 ) {
            parts[i] += sectionDesc[i - 1].separator_width;
        }
    }
    if( parts[numSections - 1] != -1 ) {
        parts[numSections] = -1;
        SendMessage( stat, SB_SETPARTS, numSections + 1, (LPARAM)parts );
    } else {
        SendMessage( stat, SB_SETPARTS, numSections, (LPARAM)parts );
    }
    MemFree( parts );

} /* updateParts */
#endif

/*
 * StatusWndCreate - create the status window
 */
HWND StatusWndCreate( HWND parent, RECT *size, HINSTANCE hinstance,
                      LPVOID lpvParam )
{
#if defined (__NT__)
    if( hInstCommCtrl != NULL ) {
        stat = CreateWindow( STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE |
                             WS_CLIPSIBLINGS | SBARS_SIZEGRIP,
                             0, 0, 0, 0, parent, NULL, hinstance, NULL );
        if( numSections > 0 ) {
            updateParts();
        }
    } else if( LOBYTE(LOWORD(GetVersion())) >= 4 ) {
        stat = CreateWindow( className, NULL, WS_CHILD, size->left, size->top,
                             size->right - size->left, size->bottom - size->top, parent,
                             (HMENU)NULL, hinstance, lpvParam );
    } else {
        stat = CreateWindow( className, NULL, WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
                             size->left, size->top, size->right - size->left,
                             size->bottom - size->top, parent, (HMENU)NULL, hinstance,
                             lpvParam );
    }
#else
    stat = CreateWindow( className, NULL, WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
                         size->left, size->top, size->right - size->left,
                         size->bottom - size->top, parent, (HMENU)NULL, hinstance,
                         lpvParam );
#endif
    if( stat != NULL ) {
#if defined( __NT__ )
       if( LOBYTE( LOWORD( GetVersion() ) ) >= 4 ) {
           /* New shell active, Win95 or later */
           systemDataFont = (HFONT) GetStockObject( DEFAULT_GUI_FONT );
       } else {
           systemDataFont = (HFONT) GetStockObject( SYSTEM_FONT );
       }
#endif
        ShowWindow( stat, SW_SHOWNORMAL );
        UpdateWindow( stat );
    }
    return( stat );

} /* StatusWndCreate */

/*
 * StatusWndDraw3DBox - called by StatusWndDrawLine or externally
 *                      if StatusWndDrawLine is not used.
 */
void StatusWndDraw3DBox( HDC hdc )
{
    HPEN    old_pen;
    int     i;
    RECT    r;

    old_pen = SelectObject( hdc, penLight );
    for( i = 0; i <= numSections; i++ ) {
        getRect( &r, i );
        outlineRect( hdc, &r );
        makeInsideRect( &r );
        FillRect( hdc, &r, brushButtonFace );
    }
    SelectObject( hdc, old_pen );

} /* StatusWndDraw3DBox */

/*
 * outputText - output a text string
 */
void outputText( HDC hdc, char *buff, RECT *r, UINT flags, int curr_block )
{
    RECT    ir;
    int     len;
    int     ext;
    int     width;

    if( sectionData[curr_block] != NULL ) {
        if( !strcmp( buff, sectionData[curr_block] ) &&
            flags == sectionDataFlags[curr_block] ) {
            return;
        }
    }

    len = strlen( buff );
    if( len == 0 ) {
        return;
    }
    MemFree( sectionData[curr_block] );
    sectionData[curr_block] = MemAlloc( len + 1 );
    memcpy( sectionData[curr_block], buff, len + 1 );
    sectionDataFlags[curr_block] = flags;

#ifndef __NT__
    ext = LOWORD( GetTextExtent( hdc, buff, len ) );
#else
    {
        SIZE    sz;

        GetTextExtentPoint( hdc, buff, len, &sz );
        ext = sz.cx;
    }
#endif
    ir = *r;
    if( flags & DT_CENTER ) {
        width = (ir.right - ir.left - ext) / 2;
        if( width > 0 ) {
            ir.right = ir.left + width;
            FillRect( hdc, &ir, brushButtonFace );
            ir.right = r->right;
            ir.left = r->right - width;
            FillRect( hdc, &ir, brushButtonFace );
        }
    } else if( flags & DT_RIGHT ) {
        ir.right -= ext;
        if( ir.right >= ir.left ) {
            FillRect( hdc, &ir, brushButtonFace );
        }
    } else {
        ir.left += ext;
        if( ir.left < ir.right ) {
            FillRect( hdc, &ir, brushButtonFace );
        }
    }
    DrawText( hdc, buff, -1, r, flags );

} /* outputText */

/*
 * StatusWndDrawLine - draws a line in the status bar
 */
void StatusWndDrawLine( HDC hdc, HFONT hfont, const char *str, UINT flags )
{
    RECT    rect;
    char    buff[256];
    char    *bptr;
    int     curr_block;

    curr_block = 0;
#if defined( __NT__ )
    sectionDataFont = systemDataFont;
#else
    sectionDataFont = hfont;
#endif
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        initHDC( hdc );
        getRect( &rect, curr_block );
        makeInsideRect( &rect );
        bptr = (char *)str;
        if( flags == (UINT) -1  ) {
            flags = DT_VCENTER | DT_LEFT;
            bptr = buff;
            while( *str ) {
                if( *str == STATUS_ESC_CHAR ) {
                    str++;
                    switch( *str ) {
                    case STATUS_NEXT_BLOCK:
                        *bptr = 0;
                        outputText( hdc, buff, &rect, flags, curr_block );
                        curr_block++;
                        getRect( &rect, curr_block );
                        makeInsideRect( &rect );
                        flags = DT_VCENTER | DT_LEFT;
                        bptr = buff;
                        break;
                    case STATUS_FORMAT_CENTER:
                        flags &= ~(DT_RIGHT | DT_LEFT);
                        flags |= DT_CENTER;
                        break;
                    case STATUS_FORMAT_RIGHT:
                        flags &= ~(DT_CENTER | DT_LEFT);
                        flags |= DT_RIGHT;
                        break;
                    case STATUS_FORMAT_LEFT:
                        flags &= ~(DT_CENTER | DT_RIGHT);
                        flags |= DT_LEFT;
                        break;
                    }
                } else {
                    *bptr++ = *str;
                }
                str++;
            }
            *bptr = 0;
            bptr = buff;
        }
        outputText( hdc, bptr, &rect, flags, curr_block );
        finiHDC( hdc );
#ifdef __NT__
    } else {
        bptr = (char *)str;
        if( flags == (UINT)-1 ) {
            bptr = buff;
            while( *str ) {
                if( *str == STATUS_ESC_CHAR ) {
                    str++;
                    if( *str == STATUS_NEXT_BLOCK ) {
                        *bptr = 0;
                        if( strlen( buff ) > 0 ) {
                            SendMessage( stat, SB_SETTEXT, curr_block, (LPARAM)buff );
                        }
                        curr_block++;
                        bptr = buff;
                    }
                } else {
                    *bptr++ = *str;
                }
                str++;
            }
            *bptr = 0;
            bptr = buff;
        }
        if( strlen( bptr ) > 0 ) {
            SendMessage( stat, SB_SETTEXT, curr_block, (LPARAM)bptr );
        }
    }
#endif
                    
} /* StatusWndDrawLine */

/*
 * StatusWndSetSeparators - set the separator list
 */
void StatusWndSetSeparators( int num_items, status_block_desc *list )
{
    int     i;

    if( num_items > MAX_SECTIONS ) {
        num_items = MAX_SECTIONS;
    }
    for( i=0; i < num_items; i++ ) {
        sectionDesc[i] = list[i];
    }
    numSections = num_items;

#ifdef __NT__
    if( hInstCommCtrl != NULL && stat != NULL ) {
        updateParts();
    }
#endif

} /* StatusWndSetSeparators */

/*
 * StatusWndFini - cleans up everything allocated for the status window
 */
void StatusWndFini( void )
{
    int i;

    if( hasGDIObjects ) {
        DeleteObject( penLight );
        DeleteObject( penShade );
        DeleteObject( brushButtonFace );
        hasGDIObjects = FALSE;
    }
    for( i = 0; i <= numSections; i++ ) {
        MemFree( sectionData[i] );
        sectionData[i] = NULL;
    }
    numSections = 0;

} /* StatusWndFini */

