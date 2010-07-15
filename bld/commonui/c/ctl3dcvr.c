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
* Description:  CTL3D covers to avoid static linking and DLL dependency.
*
****************************************************************************/


#include <windows.h>
#include "ctl3dcvr.h"

typedef BOOL    ( WINAPI *LPFN_Ctl3dSubclassDlg )( HWND, WORD );
typedef BOOL    ( WINAPI *LPFN_Ctl3dSubclassDlgEx )( HWND, DWORD );
typedef WORD    ( WINAPI *LPFN_Ctl3dGetVer )( void );
typedef BOOL    ( WINAPI *LPFN_Ctl3dEnabled )( void );
typedef HBRUSH  ( WINAPI *LPFN_Ctl3dCtlColor )( HDC, LONG );
typedef HBRUSH  ( WINAPI *LPFN_Ctl3dCtlColorEx )( UINT, WPARAM, LPARAM );
typedef BOOL    ( WINAPI *LPFN_Ctl3dColorChange )( void );
typedef BOOL    ( WINAPI *LPFN_Ctl3dSubclassCtl )( HWND );
typedef LONG    ( WINAPI *LPFN_Ctl3dDlgFramePaint )( HWND, UINT, WPARAM, LPARAM );
typedef BOOL    ( WINAPI *LPFN_Ctl3dAutoSubclass )( HANDLE );
typedef BOOL    ( WINAPI *LPFN_Ctl3dRegister )( HANDLE );
typedef BOOL    ( WINAPI *LPFN_Ctl3dUnregister )( HANDLE );
typedef VOID    ( WINAPI *LPFN_Ctl3dWinIniChange )( void );

static LPFN_Ctl3dSubclassDlg    cvrCtl3dSubclassDlg     = NULL;
static LPFN_Ctl3dSubclassDlgEx  cvrCtl3dSubclassDlgEx   = NULL;
static LPFN_Ctl3dGetVer         cvrCtl3dGetVer          = NULL;
static LPFN_Ctl3dEnabled        cvrCtl3dEnabled         = NULL;
static LPFN_Ctl3dCtlColor       cvrCtl3dCtlColor        = NULL;
static LPFN_Ctl3dCtlColorEx     cvrCtl3dCtlColorEx      = NULL;
static LPFN_Ctl3dColorChange    cvrCtl3dColorChange     = NULL;
static LPFN_Ctl3dSubclassCtl    cvrCtl3dSubclassCtl     = NULL;
static LPFN_Ctl3dDlgFramePaint  cvrCtl3dDlgFramePaint   = NULL;
static LPFN_Ctl3dAutoSubclass   cvrCtl3dAutoSubclass    = NULL;
static LPFN_Ctl3dRegister       cvrCtl3dRegister        = NULL;
static LPFN_Ctl3dUnregister     cvrCtl3dUnregister      = NULL;
static LPFN_Ctl3dWinIniChange   cvrCtl3dWinIniChange    = NULL;

static HINSTANCE                ctlDLLLib = NULL;

void CvrCtl3DDLLFini( void )
{
    cvrCtl3dSubclassDlg     = NULL;
    cvrCtl3dSubclassDlgEx   = NULL;
    cvrCtl3dGetVer          = NULL;
    cvrCtl3dEnabled         = NULL;
    cvrCtl3dCtlColor        = NULL;
    cvrCtl3dCtlColorEx      = NULL;
    cvrCtl3dColorChange     = NULL;
    cvrCtl3dSubclassCtl     = NULL;
    cvrCtl3dDlgFramePaint   = NULL;
    cvrCtl3dAutoSubclass    = NULL;
    cvrCtl3dRegister        = NULL;
    cvrCtl3dUnregister      = NULL;
    cvrCtl3dWinIniChange    = NULL;
    if( ctlDLLLib != (HINSTANCE)NULL ) {
        FreeLibrary( ctlDLLLib );
    }
}

static int CvrCtl3DDLLInit( void )
{
#ifdef __NT__
    ctlDLLLib = LoadLibrary( "CTL3D32.DLL" );
#else
    UINT    uErrMode;

    /* Use SetErrorMode to prevent annoying error popups. */
    uErrMode = SetErrorMode( SEM_NOOPENFILEERRORBOX );
    ctlDLLLib = LoadLibrary( "CTL3DV2.DLL" );
    SetErrorMode( uErrMode );
#endif

    if( ctlDLLLib == (HINSTANCE)NULL ) {
        return( FALSE );
    }

    cvrCtl3dSubclassDlg          = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)2 );
    cvrCtl3dSubclassDlgEx        = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)21 );
    cvrCtl3dGetVer               = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)1 );
    cvrCtl3dEnabled              = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)5 );
    cvrCtl3dCtlColor             = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)4 );
    cvrCtl3dCtlColorEx           = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)18 );
    cvrCtl3dColorChange          = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)6 );
    cvrCtl3dSubclassCtl          = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)3 );
    cvrCtl3dDlgFramePaint        = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)20 );
    cvrCtl3dAutoSubclass         = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)16 );
    cvrCtl3dRegister             = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)12 );
    cvrCtl3dUnregister           = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)13 );
    cvrCtl3dWinIniChange         = (void FAR *)GetProcAddress( ctlDLLLib, (LPCSTR)22 );

    if( ( cvrCtl3dSubclassDlg   == NULL ) ||
        ( cvrCtl3dSubclassDlgEx == NULL ) ||
        ( cvrCtl3dGetVer        == NULL ) ||
        ( cvrCtl3dEnabled       == NULL ) ||
        ( cvrCtl3dCtlColor      == NULL ) ||
        ( cvrCtl3dCtlColorEx    == NULL ) ||
        ( cvrCtl3dColorChange   == NULL ) ||
        ( cvrCtl3dSubclassCtl   == NULL ) ||
        ( cvrCtl3dDlgFramePaint == NULL ) ||
        ( cvrCtl3dAutoSubclass  == NULL ) ||
        ( cvrCtl3dRegister      == NULL ) ||
        ( cvrCtl3dUnregister    == NULL ) ||
        ( cvrCtl3dWinIniChange  == NULL ) ) {
        CvrCtl3DDLLFini();
        return( FALSE );
    }

    return( TRUE );
}

int C3D_EXPORT CvrCtl3DInit( HINSTANCE inst )
{
    DWORD       ver;
    BYTE        vm;

    ver = GetVersion();
    vm = (BYTE)( ver & 0x000000FF );
    if( vm >= 0x04 ) {
        return( TRUE );
    }

    CvrCtl3DDLLInit();

    return( TRUE );
}

void C3D_EXPORT CvrCtl3DFini( HINSTANCE inst )
{
    CvrCtl3DDLLFini();
}

BOOL C3D_EXPORT CvrCtl3dSubclassDlg( HWND hwnd, WORD w )
{
    if( cvrCtl3dSubclassDlg ) {
        return( cvrCtl3dSubclassDlg( hwnd, w ) );
    }
    return( FALSE );
}

BOOL C3D_EXPORT CvrCtl3dSubclassDlgEx( HWND hwnd, DWORD dw )
{
    if( cvrCtl3dSubclassDlgEx ) {
        return( cvrCtl3dSubclassDlgEx( hwnd, dw ) );
    }
    return( FALSE );
}

WORD C3D_EXPORT CvrCtl3dGetVer( void )
{
    if( cvrCtl3dGetVer ) {
        return( cvrCtl3dGetVer() );
    }
    return( 0 );
}

BOOL C3D_EXPORT CvrCtl3dEnabled( void )
{
    if( cvrCtl3dEnabled ) {
        return( cvrCtl3dEnabled() );
    }
    return( FALSE );
}

HBRUSH C3D_EXPORT CvrCtl3dCtlColor( HDC dc, LONG l )
{
    if( cvrCtl3dCtlColor ) {
        return( cvrCtl3dCtlColor( dc, l ) );
    }
    return( (HBRUSH)NULL );
}

HBRUSH C3D_EXPORT CvrCtl3dCtlColorEx( UINT wm, WPARAM wParam, LPARAM lParam )
{
    if( cvrCtl3dCtlColorEx ) {
        return( cvrCtl3dCtlColorEx( wm, wParam, lParam ) );
    }
    return( (HBRUSH)NULL );
}

BOOL C3D_EXPORT CvrCtl3dColorChange( void )
{
    if( cvrCtl3dColorChange ) {
        return( cvrCtl3dColorChange() );
    }
    return( FALSE );
}

BOOL C3D_EXPORT CvrCtl3dSubclassCtl( HWND hwnd )
{
    if( cvrCtl3dSubclassCtl ) {
        return( cvrCtl3dSubclassCtl( hwnd ) );
    }
    return( FALSE );
}

LONG C3D_EXPORT CvrCtl3dDlgFramePaint( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
    if( cvrCtl3dDlgFramePaint ) {
        return( cvrCtl3dDlgFramePaint( hwnd, msg, wp, lp ) );
    }
    return( 0L );
}

BOOL C3D_EXPORT CvrCtl3dAutoSubclass( HANDLE hndl )
{
    if( cvrCtl3dAutoSubclass ) {
        return( cvrCtl3dAutoSubclass( hndl ) );
    }
    return( FALSE );
}

BOOL C3D_EXPORT CvrCtl3dRegister( HANDLE hndl )
{
    if( cvrCtl3dRegister ) {
        return( cvrCtl3dRegister( hndl ) );
    }
    return( FALSE );
}

BOOL C3D_EXPORT CvrCtl3dUnregister( HANDLE inst )
{
    if( cvrCtl3dUnregister ) {
        return( cvrCtl3dUnregister( inst ) );
    }
    return( FALSE );
}

void C3D_EXPORT CvrCtl3dWinIniChange( void )
{
    if( cvrCtl3dWinIniChange ) {
        cvrCtl3dWinIniChange();
    }
}
