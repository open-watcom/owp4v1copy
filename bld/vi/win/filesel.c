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
* Description:  File selection dialog (open/save).
*
****************************************************************************/


#define INCLUDE_COMMDLG_H
#include "vi.h"
#include <commdlg.h>
#include <dlgs.h>
#include <cderr.h>
#include <assert.h>

static char *filterList = "C Files (*.c;*.h)\0*.c;*.h\0"
                          "C++ Files (*.cpp;*.hpp)\0*.cpp;*.hpp\0"
                          "Fortran Files (*.for;*.fi;*.f;*.inc)\0*.for;*.fi;*.f;*.inc\0"
                          "Java(Script) Files (*.java;*.js)\0*.java;*.js\0"
                          "SQL Files (*.sql)\0*.sql\0"
                          "Batch Files (*.bat;*.cmd)\0*.bat;*.cmd\0"
                          "Basic (*.bas;*.frm;*.cls)\0*.bas;*.frm;*.cls\0"
                          "Perl Files (*.pl;*.cgi)\0*.pl;*.cgi\0"
                          "HTML Files (*.htm;*.html;*.xhtml)\0*.htm;*.html;*.xhtml\0"
                          "WML Files (*.wml)\0*.wml\0"
                          "GML Files (*.gml)\0*.gml\0"
                          "DBTest (*.tst)\0*.tst\0"
                          "Makefiles (makefile;*.mk;*.mif;*.mak)\0makefile;*.mk;*.mif;*.mak\0"
                          "Asm Files (*.asm;*.inc)\0*.asm;*.inc\0"
                          "All Files (*.*)\0*.*\0"
                          "\0";
static char *FileNameList;

typedef UINT (WINEXP * OPENHOOKTYPE)( HWND, UINT, WPARAM, LPARAM );


BOOL WINEXP OpenHook( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    int                         len;
    static OPENFILENAME __FAR__ *of;

    wparam = wparam;
    lparam = lparam;
    hwnd = hwnd;

    switch( msg ) {
    case WM_INITDIALOG:
        /* danger - not sure that lparam is guaranteed to be the of. struct */
        of = (OPENFILENAME __FAR__ *)MAKEPTR( (LPVOID)lparam );
        // return( FALSE );
        return( TRUE );
    case WM_COMMAND:
        switch( LOWORD( wparam ) ) {
        case IDOK:
            len = SendDlgItemMessage( hwnd, edt1, WM_GETTEXTLENGTH, 0, 0 );
            if( len >= of->nMaxFile ) {
                FileNameList = MemAlloc( len + 1 );
                len = SendDlgItemMessage( hwnd, edt1, WM_GETTEXT, len + 1,
                                          (LPARAM)FileNameList );
            }
        }
        break;
    }
    return( FALSE );
}

/*
 * SelectFileOpen - use common dialog file open to pick a file to edit
 */
vi_rc SelectFileOpen( char *dir, char **result, char *mask, bool want_all_dirs )
{
    OPENFILENAME        of;
    BOOL                rc;
    static long         filemask = 1;

#ifdef __NT__
    /* added to get around chicago crashing in the fileopen dlg */
    /* -------------------------------------------------------- */
    DWORD ver;
    bool is_chicago = FALSE;

    ver = GetVersion();
    if( LOBYTE( LOWORD( GetVersion() ) ) >= 4 ) {
        is_chicago = TRUE;
    }
    /* -------------------------------------------------------- */
#endif

    mask = mask;
    want_all_dirs = want_all_dirs;
    *result[0] = 0;
    memset( &of, 0, sizeof( OPENFILENAME ) );
    of.lStructSize = sizeof( OPENFILENAME );
    of.hwndOwner = Root;
    of.lpstrFilter = (LPSTR) filterList;
    of.lpstrDefExt = NULL;
    of.nFilterIndex = filemask;
    of.lpstrFile = *result;
    of.nMaxFile = FILENAME_MAX;
    of.lpstrTitle = NULL;
    of.lpstrInitialDir = dir;
#ifdef __NT__
    if( is_chicago ) {
        of.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                   OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    } else {
        of.Flags = OFN_PATHMUSTEXIST | OFN_ENABLEHOOK |
                   OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER;
#else
        of.Flags = OFN_PATHMUSTEXIST | OFN_ENABLEHOOK |
                   OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY;
#endif
        of.lpfnHook = (LPOFNHOOKPROC) MakeProcInstance( (FARPROC) OpenHook,
                                                        InstanceHandle );
#ifdef __NT__
    }
#endif
    rc = GetOpenFileName( &of );
    filemask = of.nFilterIndex;
#ifdef __NT__
    if( is_chicago ) {
#endif
#ifndef __NT__
        FreeProcInstance( (FARPROC) of.lpfnHook );
#endif
#ifdef __NT__
    }
#endif
    if( rc == FALSE && CommDlgExtendedError() == FNERR_BUFFERTOOSMALL ) {
#ifdef __NT__
        if( !is_chicago ) {
#endif
            MemFree( (char*)(of.lpstrFile) );
            *result = FileNameList;
#ifdef __NT__
        }
#endif
#if 0
        MyBeep();
        Message1( "Please open files in smaller groups" );
#endif
    }
    UpdateCurrentDirectory();
    return( ERR_NO_ERR );

} /* SelectFileOpen */

/*
 * SelectFileSave - use common dialog file open to pickname to save under
 */
vi_rc SelectFileSave( char *result )
{
    OPENFILENAME        of;
    int                 doit;

    assert( CurrentFile != NULL );

    strcpy( result, CurrentFile->name );
    memset( &of, 0, sizeof( OPENFILENAME ) );
    of.lStructSize = sizeof( OPENFILENAME );
    of.hwndOwner = Root;
    of.lpstrFilter = (LPSTR) filterList;
    of.lpstrDefExt = NULL;
    of.nFilterIndex = 1L;
    of.lpstrFile = result;
    of.nMaxFile = FILENAME_MAX;
    of.lpstrTitle = NULL;
    of.lpstrInitialDir = CurrentFile->home;
#ifdef __NT__
    if( LOBYTE( LOWORD( GetVersion() ) ) >= 4 ) {
        of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT |
                   OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_EXPLORER;
    } else {
        of.Flags = OFN_PATHMUSTEXIST | OFN_ENABLEHOOK | OFN_OVERWRITEPROMPT |
                   OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_EXPLORER;
    }
#else
    of.Flags = OFN_PATHMUSTEXIST | OFN_ENABLEHOOK | OFN_OVERWRITEPROMPT |
               OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
#endif
    of.lpfnHook = (LPOFNHOOKPROC) MakeProcInstance( (FARPROC) OpenHook,
                                                    InstanceHandle );
    doit = GetSaveFileName( &of );
#ifndef __NT__
    FreeProcInstance( (FARPROC) of.lpfnHook );
#endif

    if( doit != 0 ) {
        UpdateCurrentDirectory();
        return( ERR_NO_ERR );
    } else {
        return( ERR_SAVE_CANCELED );
    }

} /* SelectFileSave */

/*
 * GetInitialFileName - prompt the user for an initial file
 */
char *GetInitialFileName( void )
{
    char        *path = MemAlloc( FILENAME_MAX );
    char        *ptr;
    vi_rc       rc;

    CloseStartupDialog();
    path[0] = 0;
    rc = SelectFileOpen( "", &path, NULL, FALSE );
    ShowStartupDialog();
    if( rc == ERR_NO_ERR && path[0] != 0 ) {
        AddString( &ptr, path );
    } else {
        ptr = NULL;
    }
    MemFree( path );
    return( ptr );

} /* GetInitialFileName */

vi_rc SelectLineInFile( selflinedata *sfd )
{
    sfd = sfd;
    return( ERR_NO_ERR );
}
