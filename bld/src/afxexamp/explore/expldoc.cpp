#include "stdafx.h"
#include "explore.h"
#include "expldoc.h"

IMPLEMENT_DYNCREATE( CExploreDoc, CDocument )

CExploreDoc::CExploreDoc()
{
}

BOOL CExploreDoc::OnNewDocument()
{
    if( !CDocument::OnNewDocument() ) {
        return( FALSE );
    }

    return( TRUE );
}

void CExploreDoc::Serialize( CArchive &ar )
{
    CDocument::Serialize( ar );
}
