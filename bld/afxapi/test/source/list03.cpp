#include "stdafx.h"

int main()
{
    CStringList list1;
    CStringList list2;

    if( list1.GetHeadPosition() != NULL ) _fail;
    if( list1.GetTailPosition() != NULL ) _fail;

    list1.AddTail( _T( "A" ) );
    list1.AddTail( _T( "B" ) );
    list1.AddHead( _T( " " ) );
    if( list1.GetCount() != 3 ) _fail;
    if( list1.GetSize() != 3 ) _fail;
    if( list1.GetHead() != _T( " " ) ) _fail;
    if( list1.GetTail() != _T( "B" ) ) _fail;

    POSITION position = list1.GetHeadPosition();
    if( position == NULL ) _fail;
    if( list1.GetNext( position ) != _T( " " ) ) _fail;
    if( position == NULL ) _fail;
    if( list1.GetNext( position ) != _T( "A" ) ) _fail;
    if( position == NULL ) _fail;
    if( list1.GetNext( position ) != _T( "B" ) ) _fail;
    if( position != NULL ) _fail;

    position = list1.GetTailPosition();
    if( position == NULL ) _fail;
    if( list1.GetPrev( position ) != _T( "B" ) ) _fail;
    if( position == NULL ) _fail;
    if( list1.GetPrev( position ) != _T( "A" ) ) _fail;
    if( position == NULL ) _fail;
    if( list1.GetPrev( position ) != _T( " " ) ) _fail;
    if( position != NULL ) _fail;

    position = list1.Find( _T( " " ) );
    if( position != list1.GetHeadPosition() ) _fail;
    position = list1.Find( _T( "A" ) );
    if( position == list1.GetHeadPosition() ) _fail;
    if( position == list1.GetTailPosition() ) _fail;
    if( position == NULL ) _fail;
    position = list1.Find( _T( "B" ) );
    if( position != list1.GetTailPosition() ) _fail;
    position = list1.Find( _T( "C" ) );
    if( position != NULL ) _fail;

    POSITION posStart = list1.Find( _T( "A" ) );
    position = list1.Find( _T( " " ), posStart );
    if( position != NULL ) _fail;
    position = list1.Find( _T( "A" ), posStart );
    if( position == list1.GetHeadPosition() ) _fail;
    if( position == list1.GetTailPosition() ) _fail;
    if( position == NULL ) _fail;
    position = list1.Find( _T( "B" ), posStart );
    if( position != list1.GetTailPosition() ) _fail;

    position = list1.FindIndex( 0 );
    if( position != list1.GetHeadPosition() ) _fail;
    if( list1.GetAt( position ) != _T( " " ) ) _fail;
    position = list1.FindIndex( 1 );
    if( position == list1.GetHeadPosition() ) _fail;
    if( position == list1.GetTailPosition() ) _fail;
    if( position == NULL ) _fail;
    if( list1.GetAt( position ) != _T( "A" ) ) _fail;
    position = list1.FindIndex( 2 );
    if( position != list1.GetTailPosition() ) _fail;
    if( list1.GetAt( position ) != _T( "B" ) ) _fail;
    position = list1.FindIndex( 3 );
    if( position != NULL ) _fail;

    list2.AddTail( _T( "C" ) );
    list2.AddHead( &list1 );
    list2.AddTail( &list1 );
    if( list2.GetCount() != 7 ) _fail;
    if( list2.GetSize() != 7 ) _fail;
    position = list2.GetHeadPosition();
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( " " ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "A" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "B" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "C" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( " " ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "A" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "B" ) ) _fail;
    if( position != NULL ) _fail;

    if( list2.RemoveHead() != _T( " " ) ) _fail;
    if( list2.RemoveTail() != _T( "B" ) ) _fail;
    position = list2.Find( _T( " " ) );
    if( position == NULL ) _fail;
    list2.RemoveAt( position );
    if( list2.GetCount() != 4 ) _fail;
    if( list2.GetSize() != 4 ) _fail;
    position = list2.GetHeadPosition();
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "A" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "B" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "C" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetNext( position ) != _T( "A" ) ) _fail;
    if( position != NULL ) _fail;

    position = list2.GetTailPosition();
    list2.SetAt( position, _T( "E" ) );
    list2.InsertBefore( position, _T( "D" ) );
    list2.InsertAfter( position, _T( "F" ) );
    position = list2.GetTailPosition();
    if( position == NULL ) _fail;
    if( list2.GetPrev( position ) != _T( "F" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetPrev( position ) != _T( "E" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetPrev( position ) != _T( "D" ) ) _fail;
    if( position == NULL ) _fail;
    if( list2.GetPrev( position ) != _T( "C" ) ) _fail;

    _PASS;
}
