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
* Description:  Multiple status window class.
*
****************************************************************************/


#include <windows.h>
#include <string.h>
#ifdef __NT__
    #include <commctrl.h>
#endif
#include "mstatwnd.h"

extern LPVOID   MemAlloc( unsigned );
extern void     MemFree( LPVOID );

static char                     *className = "MStatusWnd";
static HPEN                     penLight;
static HPEN                     penShade;
static HBRUSH                   brushButtonFace;
static BOOL                     hasGDIObjects;
static COLORREF                 colorButtonFace;
static statushook               statusWndHookFunc;
static int                      classWinExtra;

static statwnd                 *cheesy_stat;
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
static void getRect( statwnd *sw, RECT *r, int i )
{
    WORD        pos;
    WORD        width;

    *r = sw->statusRect;
    width = sw->statusRect.right - sw->statusRect.left;
    if( i > 0 ) {
        if( sw->sectionDesc[i-1].width_is_percent ) {
            pos = (WORD) (((DWORD) width * (DWORD) sw->sectionDesc[i].width)/100L);
        } else {
            pos = sw->sectionDesc[i-1].width;
        }
        r->left = pos+sw->sectionDesc[i-1].separator_width;
    }
    if( i == sw->numSections ) {
        pos = sw->statusRect.right;
    } else if( sw->sectionDesc[i].width_is_percent ) {
        pos = (WORD) (((DWORD) (sw->statusRect.right-sw->statusRect.left)
                    * (DWORD) sw->sectionDesc[i].width)/100L);
    } else {
        pos = sw->sectionDesc[i].width;
    }
    r->right = pos;

} /* getRect */

static HFONT    oldFont;
static HBRUSH   oldBrush;
static COLORREF oldBkColor;

/*
 * initHDC - initialize our HDC for drawing text
 */
static char initHDC( statwnd *sw, HDC hdc )
{
#if !defined (__NT__)
    if( sw->sectionDataFont == NULL ) {
        return( FALSE );
    }
#endif
    oldFont = SelectObject( hdc, sw->sectionDataFont );
    oldBrush = SelectObject( hdc, brushButtonFace );
    oldBkColor = GetBkColor( hdc );
    SetBkColor( hdc, colorButtonFace );
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
LONG CB StatusWndCallback( HWND hwnd, UINT msg, UINT wparam, LONG lparam  )
{
    PAINTSTRUCT  ps;
    RECT         r;
    int          i;
    statwnd     *sw;

    sw = (statwnd *) GetWindowLong( hwnd, classWinExtra );

    if( statusWndHookFunc != NULL ) {
        if( statusWndHookFunc( hwnd, msg, wparam, lparam ) ) {
            return( 0 );
        }
    }

    if ( ( ( msg == WM_SIZE ) || ( msg == WM_PAINT ) ) && !sw ) {
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
    }

    switch( msg ) {
    case WM_CREATE:
        SetWindowLong ( hwnd, classWinExtra, (LONG) cheesy_stat );
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
    case WM_SIZE:
        GetClientRect( hwnd, &sw->statusRect );
        InflateRect( &sw->statusRect, - HORZ_BORDER, - VERT_BORDER );
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
    case WM_PAINT:
        BeginPaint( hwnd, &ps );
        StatusWndDraw3DBox( sw, ps.hdc );
        if( initHDC( sw, ps.hdc ) ) {
#if defined (__NT__)
            if( LOBYTE(LOWORD(GetVersion())) >= 4 ) {
                SelectObject( ps.hdc, (HFONT)GetStockObject(DEFAULT_GUI_FONT) );
            } else {
                SelectObject( ps.hdc, (HFONT)GetStockObject(SYSTEM_FONT) );
            }
#endif
            for( i = 0; i <= sw->numSections; i++ ) {
                if( sw->sectionData[i] != NULL ) {
                    getRect( sw, &r, i );
                    makeInsideRect( &r );
                    DrawText( ps.hdc, sw->sectionData[i], -1, &r, sw->sectionDataFlags[i] );
                }
            }
            finiHDC( ps.hdc );
        }
        EndPaint( hwnd, &ps );
        break;
    case WM_ERASEBKGND:
#if defined (__NT__)
        if( colorButtonFace != GetSysColor( COLOR_BTNFACE ) ) {
            DeleteObject( brushButtonFace );
            DeleteObject( penLight );
            DeleteObject( penShade );
            colorButtonFace = GetSysColor( COLOR_BTNFACE );
            brushButtonFace = CreateSolidBrush( colorButtonFace );
            penLight = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNHIGHLIGHT ) );
            penShade = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNSHADOW ) );
            hasGDIObjects = TRUE;
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
int StatusWndInit( HINSTANCE hinstance, statushook hook, int extra )
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
            brushButtonFace = CreateSolidBrush( colorButtonFace );
            penLight = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNHIGHLIGHT ) );
            penShade = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNSHADOW ) );
            hasGDIObjects = TRUE;
        }

        statusWndHookFunc = hook;

        rc = TRUE;

        if( GetClassInfo( hinstance, className, &wc ) ) {
            classWinExtra = wc.cbWndExtra - sizeof(statwnd *);
        } else {
            classWinExtra = extra;
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = (LPVOID) StatusWndCallback;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = extra + sizeof(statwnd *);
            wc.hInstance = hinstance;
            wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
            wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
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
 * StatusWndStart - start a status window
 */
statwnd *StatusWndStart( void )
{
    statwnd *sw;

    sw = (statwnd *) MemAlloc( sizeof(statwnd) );
    if ( sw ) {
        memset ( sw, 0, sizeof(statwnd) );
    }
    return( sw );
} /* StatusWndStart */

#ifdef __NT__

/*
 * updateParts - update the parts of a native status bar
 */
static void updateParts( statwnd *sw ) {
    int     i;
    RECT    rc;
    int     width;
    int     *parts;

    parts = (int *)MemAlloc( sizeof( int ) * (sw->numSections + 1) );
    GetClientRect( sw->win, &rc );
    width = rc.right - rc.left;
    for( i = 0; i < sw->numSections; i++ ) {
        if( sw->sectionDesc[i].width_is_percent ) {
            parts[i] = sw->sectionDesc[i].width * width / 100;
        } else {
            parts[i] = sw->sectionDesc[i].width;
        }
        if( i > 0 && parts[i] != -1 ) {
            parts[i] += sw->sectionDesc[i - 1].separator_width;
        }
    }
    if( parts[sw->numSections - 1] != -1 ) {
        parts[sw->numSections] = -1;
        SendMessage( sw->win, SB_SETPARTS, sw->numSections + 1, (LPARAM)parts );
    } else {
        SendMessage( sw->win, SB_SETPARTS, sw->numSections, (LPARAM)parts );
    }
    MemFree( parts );

} /* updateParts */

#endif

/*
 * StatusWndCreate - create the status window
 */
int StatusWndCreate( statwnd *sw, HWND parent, RECT *size,
                     HINSTANCE hinstance, LPVOID lpvParam )
{
    if ( !sw ) {
        return ( FALSE );
    }

    cheesy_stat = sw;

#if defined (__NT__)
    if( hInstCommCtrl != NULL ) {
        sw->win = CreateWindow( STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE |
                                WS_CLIPSIBLINGS | SBARS_SIZEGRIP,
                                0, 0, 0, 0, parent, NULL, hinstance, NULL );
        if( sw->numSections > 0 ) {
            updateParts( sw );
        }
    } else if( LOBYTE(LOWORD(GetVersion())) >= 4 ) {
        sw->win = CreateWindow( className, NULL,
                             WS_CHILD | WS_CLIPSIBLINGS,
                             size->left, size->top,
                             size->right - size->left, size->bottom - size->top,
                             parent, (HMENU)NULL, hinstance, lpvParam );
    } else {
        sw->win= CreateWindow( className, NULL,
                             WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
                             size->left, size->top,
                             size->right - size->left, size->bottom - size->top,
                             parent, (HMENU)NULL, hinstance, lpvParam );
    }
#else
    sw->win = CreateWindow( className, NULL,
                              WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
                              size->left, size->top,
                              size->right - size->left,
                              size->bottom - size->top,
                              parent, (HMENU)NULL, hinstance, lpvParam );
#endif
    if( sw->win == (HWND)NULL ) {
        return ( FALSE );
    }

    ShowWindow( sw->win, SW_SHOWNORMAL );
    UpdateWindow( sw->win );

    return( TRUE );

} /* StatusWndCreate */

/*
 * StatusWndDraw3DBox - called by StatusWndDrawLine or externally
 *                      in StatusWndDrawLine is not used.
 */
void StatusWndDraw3DBox( statwnd *sw, HDC hdc )
{
    HPEN        old_pen;
    int         i;
    RECT        r;

    old_pen = SelectObject( hdc, penLight );
    for( i=0;i<=sw->numSections;i++ ) {
        getRect( sw, &r, i );
        outlineRect( hdc, &r );
        makeInsideRect( &r );
        FillRect( hdc, &r, brushButtonFace );
    }
    SelectObject( hdc, old_pen );

} /* StatusWndDraw3DBox */

/*
 * outputText - output a text string
 */
void outputText( statwnd *sw, HDC hdc, char *buff, RECT *r,
                 UINT flags, int curr_block )
{
    RECT        ir;
    int         len;
    int         ext;
    int         width;

    if( sw->sectionData[curr_block] != NULL ) {
        if( !strcmp( buff, sw->sectionData[curr_block] ) ) {
            return;
        }
    }

    len = strlen( buff );
    if( len == 0 ) {
        return;
    }
    MemFree( sw->sectionData[curr_block] );
    sw->sectionData[curr_block] = MemAlloc( len+1 );
    memcpy( sw->sectionData[curr_block], buff, len+1 );
    sw->sectionDataFlags[curr_block] = flags;

#ifndef __NT__
    ext = LOWORD( GetTextExtent( hdc, buff, len ) );
#else
    {
        SIZE    sz;

        if( LOBYTE(LOWORD(GetVersion())) >= 4 ) {
            SelectObject( hdc, (HFONT)GetStockObject(DEFAULT_GUI_FONT) );
        } else {
            SelectObject( hdc, (HFONT)GetStockObject(SYSTEM_FONT) );
        }
        GetTextExtentPoint( hdc, buff, len, &sz );
        ext = sz.cx;
    }
#endif
    ir = *r;
    if( flags & DT_CENTER ) {
        width = (ir.right - ir.left - ext)/2;
        if( width > 0 ) {
            ir.right = ir.left+width;
            FillRect( hdc, &ir, brushButtonFace );
            ir.right = r->right;
            ir.left = r->right-width;
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
void StatusWndDrawLine( statwnd *sw, HDC hdc, HFONT hfont,
                        const char *str, UINT flags )
{
    RECT        rect;
    char        buff[256];
    char        *bptr;
    int         curr_block;

    curr_block = 0;
    sw->sectionDataFont = hfont;
#ifdef __NT__
    if( hInstCommCtrl == NULL ) {
#endif
        initHDC( sw, hdc );
        getRect( sw, &rect, curr_block );
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
                        outputText( sw, hdc, buff, &rect, flags, curr_block );
                        curr_block++;
                        getRect( sw, &rect, curr_block );
                        makeInsideRect( &rect );
                        flags = DT_VCENTER | DT_LEFT;
                        bptr = buff;
                        break;
                    case STATUS_FORMAT_CENTER:
                        flags &= ~(DT_RIGHT|DT_LEFT);
                        flags |= DT_CENTER;
                        break;
                    case STATUS_FORMAT_RIGHT:
                        flags &= ~(DT_CENTER|DT_LEFT);
                        flags |= DT_RIGHT;
                        break;
                    case STATUS_FORMAT_LEFT:
                        flags &= ~(DT_CENTER|DT_RIGHT);
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
        outputText( sw, hdc, bptr, &rect, flags, curr_block );
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
                            SendMessage( sw->win, SB_SETTEXT, curr_block, (LPARAM)buff );
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
            SendMessage( sw->win, SB_SETTEXT, curr_block, (LPARAM)bptr );
        }
    }
#endif

} /* StatusWndDrawLine */

/*
 * StatusWndSetSeparators - set the separator list
 */
void StatusWndSetSeparators( statwnd *sw, int num_items,
                             status_block_desc *list )
{
    int i;

    if( num_items > MAX_SECTIONS ) {
        num_items = MAX_SECTIONS;
    }
    for( i=0;i<num_items;i++ ) {
        sw->sectionDesc[i] = list[i];
    }
    sw->numSections = num_items;

#ifdef __NT__
    if( hInstCommCtrl != NULL && sw->win != NULL ) {
        updateParts( sw );
    }
#endif

} /* StatusWndSetSeparators */

/*
 * StatusWndDestroy - cleans up everything allocated for the status window
 */
void StatusWndDestroy( statwnd *sw )
{
    int i;

    if( sw ) {
        if ( sw->win != (HWND)NULL ) {
            DestroyWindow ( sw->win );
        }
        for( i=0;i<=sw->numSections;i++ ) {
            MemFree( sw->sectionData[i] );
        }
        MemFree ( sw );
    }
} /* StatusWndDestroy */

/*
 * StatusWndFini - cleans up everything allocated for the status window
 */
void StatusWndFini( void )
{
    if( hasGDIObjects ) {
        DeleteObject( penLight );
        DeleteObject( penShade );
        DeleteObject( brushButtonFace );
        hasGDIObjects = FALSE;
    }

} /* StatusWndFini */
