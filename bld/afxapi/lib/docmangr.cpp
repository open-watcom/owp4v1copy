/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Implementation of CDocManager.
*
****************************************************************************/


#include "stdafx.h"
#include "filenew.h"

IMPLEMENT_DYNAMIC( CDocManager, CObject )

CDocManager::CDocManager()
/************************/
{
}

void CDocManager::AddDocTemplate( CDocTemplate *pTemplate )
/*********************************************************/
{
    ASSERT( pTemplate != NULL );
    pTemplate->LoadTemplate();
    m_templateList.AddTail( pTemplate );
}

void CDocManager::CloseAllDocuments( BOOL bEndSession )
/*****************************************************/
{
    POSITION position = m_templateList.GetHeadPosition();
    while( position != NULL ) {
        CDocTemplate *pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
        ASSERT( pTemplate != NULL );
        pTemplate->CloseAllDocuments( bEndSession );
    }
}

BOOL CDocManager::DoPromptFileName( CString &fileName, UINT nIDSTitle, DWORD lFlags,
                                    BOOL bOpenFileDialog, CDocTemplate *pTemplate )
/*********************************************************************************/
{
    CString strTitle;
    strTitle.LoadString( nIDSTitle );

    CString strFilter;
    CString strDocString;
    if( pTemplate != NULL ) {
        if( pTemplate->GetDocString( strDocString, CDocTemplate::filterName ) ) {
            strFilter += strDocString;
            strFilter.AppendChar( _T( '\0' ) );
            strFilter.AppendChar( _T( '*' ) );
            pTemplate->GetDocString( strDocString, CDocTemplate::filterExt );
            strFilter += strDocString;
            strFilter.AppendChar( _T( '\0' ) );
        }
    } else {
        POSITION position = m_templateList.GetHeadPosition();
        while( position != NULL ) {
            pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
            ASSERT( pTemplate != NULL );
            if( pTemplate->GetDocString( strDocString, CDocTemplate::filterName ) ) {
                strFilter += strDocString;
                strFilter.AppendChar( _T( '\0' ) );
                strFilter.AppendChar( _T( '*' ) );
                pTemplate->GetDocString( strDocString, CDocTemplate::filterExt );
                strFilter += strDocString;
                strFilter.AppendChar( _T( '\0' ) );
            }
        }
    }
    strDocString.LoadString( AFX_IDS_ALLFILTER );
    strFilter += strDocString;
    strFilter.AppendChar( _T( '\0' ) );
    strFilter += _T( "*.*" );
    strFilter.AppendChar( _T( '\0' ) );

    CFileDialog dlg( bOpenFileDialog, NULL, NULL,
                     lFlags | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter,
                     AfxGetMainWnd() );
    dlg.m_ofn.lpstrTitle = strTitle;
    if( dlg.DoModal() != IDOK ) {
        return( FALSE );
    }
    fileName = dlg.GetFileName();
    return( TRUE );
}

POSITION CDocManager::GetFirstDocTemplatePosition() const
/*******************************************************/
{
    return( m_templateList.GetHeadPosition() );
}

CDocTemplate *CDocManager::GetNextDocTemplate( POSITION &pos ) const
/******************************************************************/
{
    return( (CDocTemplate *)m_templateList.GetNext( pos ) );
}

void CDocManager::OnFileNew()
/***************************/
{
    POSITION position = GetFirstDocTemplatePosition();
    if( position != NULL ) {
        CDocTemplate *pTemplate = GetNextDocTemplate( position );
        ASSERT( pTemplate != NULL );
        if( position == NULL ) {
            pTemplate->OpenDocumentFile( NULL );
        } else {
            CFileNewDialog dlg( this );
            if( dlg.DoModal() == IDOK ) {
                ASSERT( dlg.m_pDocTemplate != NULL );
                dlg.m_pDocTemplate->OpenDocumentFile( NULL );
            }
        }
    }
}

void CDocManager::OnFileOpen()
/****************************/
{
    CString strFileName;
    if( DoPromptFileName( strFileName, AFX_IDS_OPENFILE,
                          OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, TRUE, NULL ) ) {
        if( OpenDocumentFile( strFileName ) == NULL ) {
            AfxMessageBox( AFX_IDP_FAILED_TO_OPEN_DOC );
        }
    }
}

CDocument *CDocManager::OpenDocumentFile( LPCTSTR lpszFileName )
/**************************************************************/
{
    POSITION                    position = m_templateList.GetHeadPosition();
    CDocTemplate                *pSelected = NULL;
    CDocTemplate::Confidence    nSelConfidence = CDocTemplate::noAttempt;
    while( position != NULL ) {
        CDocTemplate *pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
        ASSERT( pTemplate != NULL );
        
        CDocument                   *pMatch = NULL;
        CDocTemplate::Confidence    nConfidence = pTemplate->MatchDocType( lpszFileName,
                                                                           pMatch );
        if( nConfidence > nSelConfidence ) {
            nSelConfidence = nConfidence;
            pSelected = pTemplate;
            if( nConfidence == CDocTemplate::yesAlreadyOpen ) {
                ASSERT( pMatch != NULL );
                POSITION viewPos = pMatch->GetFirstViewPosition();
                if( viewPos != NULL ) {
                    CView *pView = pMatch->GetNextView( viewPos );
                    CFrameWnd *pFrame = pView->GetParentFrame();
                    ASSERT( pFrame != NULL );
                    pFrame->ActivateFrame();
                }
                return( pMatch );
            }
        }
    }
    if( pSelected == NULL ) {
        return( NULL );
    }
    return( pSelected->OpenDocumentFile( lpszFileName ) );
}

void CDocManager::RegisterShellFileTypes( BOOL bCompat )
/******************************************************/
{
    POSITION position = m_templateList.GetHeadPosition();
    while( position != NULL ) {
        CDocTemplate *pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
        ASSERT( pTemplate != NULL );

        CString strExtension;
        CString strProgId;
        CString strFriendlyName;
        if( !pTemplate->GetDocString( strExtension, CDocTemplate::filterExt ) ) {
            continue;
        }
        if( !pTemplate->GetDocString( strProgId, CDocTemplate::regFileTypeId ) ) {
            continue;
        }
        if( !pTemplate->GetDocString( strFriendlyName,
                                      CDocTemplate::regFileTypeName ) ) {
            continue;
        }

        HKEY hKey = NULL;
        if( ::RegCreateKey( HKEY_CLASSES_ROOT, strExtension, &hKey ) != ERROR_SUCCESS ) {
            continue;
        }
        ::RegSetValue( hKey, NULL, REG_SZ, strProgId, strProgId.GetLength() );
        ::RegCloseKey( hKey );
        
        if( ::RegCreateKey( HKEY_CLASSES_ROOT, strProgId, &hKey ) != ERROR_SUCCESS ) {
            continue;
        }
        ::RegSetValue( hKey, NULL, REG_SZ, strFriendlyName,
                       strFriendlyName.GetLength() );
        ::RegCloseKey( hKey );

        TCHAR szExeName[256];
        ::GetModuleFileName( AfxGetInstanceHandle(), szExeName, 255 );

        CString strFmt1;
        strFmt1.Format( _T( "%s\\shell\\open\\command" ), (LPCTSTR)strProgId );
        if( ::RegCreateKey( HKEY_CLASSES_ROOT, strFmt1, &hKey ) != ERROR_SUCCESS ) {
            continue;
        }
        ::RegSetValue( hKey, NULL, REG_SZ, szExeName, _tcslen( szExeName ) );
        ::RegCloseKey( hKey );
        
        if( bCompat ) {
            CString strFmt2;
            strFmt1.Format( _T( "%s\\shell\\print\\command" ), (LPCTSTR)strProgId );
            strFmt2.Format( _T( "%s /p" ), szExeName );
            if( ::RegCreateKey( HKEY_CLASSES_ROOT, strFmt1, &hKey ) != ERROR_SUCCESS ) {
                continue;
            }
            ::RegSetValue( hKey, NULL, REG_SZ, strFmt2, strFmt2.GetLength() );
            ::RegCloseKey( hKey );

            strFmt1.Format( _T( "%s\\shell\\printto\\command" ), (LPCTSTR)strProgId );
            strFmt2.Format( _T( "%s /pt" ), szExeName );
            if( ::RegCreateKey( HKEY_CLASSES_ROOT, strFmt1, &hKey ) != ERROR_SUCCESS ) {
                continue;
            }
            ::RegSetValue( hKey, NULL, REG_SZ, strFmt2, strFmt2.GetLength() );
            ::RegCloseKey( hKey );
        }
    }
}

BOOL CDocManager::SaveAllModified()
/*********************************/
{
    POSITION position = m_templateList.GetHeadPosition();
    while( position != NULL ) {
        CDocTemplate *pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
        ASSERT( pTemplate != NULL );
        if( !pTemplate->SaveAllModified() ) {
            return( FALSE );
        }
    }
    return( TRUE );
}

#ifdef _DEBUG

void CDocManager::AssertValid() const
/***********************************/
{
    CObject::AssertValid();

    POSITION position = m_templateList.GetHeadPosition();
    while( position != NULL ) {
        CDocTemplate *pTemplate = (CDocTemplate *)m_templateList.GetNext( position );
        ASSERT_VALID( pTemplate );
    }
}

#endif // _DEBUG
