#include "stdafx.h"

int main()
{
    for( int i = 1; i <= 5; i++ ) {
        CObArray arr1;
        CObArray arr2;
        CObArray arr3;

        arr1.SetSize( 0, i );
        arr2.SetSize( 0, i );
        arr3.SetSize( 0, i );

        if( !arr1.IsEmpty() ) _fail;

        arr1.Add( (CObject *)0 );
        arr1.Add( (CObject *)1 );
        arr1.Add( (CObject *)2 );
        arr2.Add( (CObject *)3 );
        arr2.Add( (CObject *)4 );
        arr2.Add( (CObject *)5 );
        arr1.Append( arr2 );
        arr3.Copy( arr1 );
        arr3.SetAt( 0, (CObject *)1 );
        arr3.RemoveAt( 1 );

        if( arr3.GetAt( 0 ) != (CObject *)1 ) _fail;
        if( arr3.GetAt( 1 ) != (CObject *)2 ) _fail;
        if( arr3.GetAt( 2 ) != (CObject *)3 ) _fail;
        if( arr3.GetAt( 3 ) != (CObject *)4 ) _fail;
        if( arr3.GetAt( 4 ) != (CObject *)5 ) _fail;
        if( arr3.GetSize() != 5 ) _fail;
        if( arr3.GetUpperBound() != 4 ) _fail;

        arr1.Copy( arr2 );
        arr1.InsertAt( 1, (CObject *)1, 2 );
        if( arr1.GetAt( 0 ) != (CObject *)3 ) _fail;
        if( arr1.GetAt( 1 ) != (CObject *)1 ) _fail;
        if( arr1.GetAt( 2 ) != (CObject *)1 ) _fail;
        if( arr1.GetAt( 3 ) != (CObject *)4 ) _fail;
        if( arr1.GetAt( 4 ) != (CObject *)5 ) _fail;
        if( arr1.GetSize() != 5 ) _fail;
        if( arr1.GetUpperBound() != 4 ) _fail;

        arr1.Copy( arr2 );
        arr1.InsertAt( 1, &arr2 );
        arr1.SetAtGrow( 7, (CObject *)6 );
        if( arr1.GetAt( 0 ) != (CObject *)3 ) _fail;
        if( arr1.GetAt( 1 ) != (CObject *)3 ) _fail;
        if( arr1.GetAt( 2 ) != (CObject *)4 ) _fail;
        if( arr1.GetAt( 3 ) != (CObject *)5 ) _fail;
        if( arr1.GetAt( 4 ) != (CObject *)4 ) _fail;
        if( arr1.GetAt( 5 ) != (CObject *)5 ) _fail;
        if( arr1.GetAt( 6 ) != NULL ) _fail;
        if( arr1.GetAt( 7 ) != (CObject *)6 ) _fail;
        if( arr1.GetSize() != 8 ) _fail;
        if( arr1.GetUpperBound() != 7 ) _fail;

        arr1.SetSize( 10 );
        if( arr1.GetSize() != 10 ) _fail;
        if( arr1.GetUpperBound() != 9 ) _fail;
        if( arr1.GetAt( 8 ) != NULL ) _fail;
        if( arr1.GetAt( 9 ) != NULL ) _fail;
    }

    _PASS;
}
