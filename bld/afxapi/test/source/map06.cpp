#include "stdafx.h"

int main()
{
    CMapStringToOb map;

    if( !map.IsEmpty() ) _fail;
    if( map.GetCount() != 0 ) _fail;
    if( map.GetSize() != 0 ) _fail;

    map.SetAt( _T( "0" ), (CObject *)0 );
    map.SetAt( _T( "1" ), (CObject *)1 );
    map.SetAt( _T( "2" ), (CObject *)2 );

    CObject *value;
    if( !map.Lookup( _T( "0" ), value ) ) _fail;
    if( value != (CObject *)0 ) _fail;
    if( !map.Lookup( _T( "1" ), value ) ) _fail;
    if( value != (CObject *)1 ) _fail;
    if( !map.Lookup( _T( "2" ), value ) ) _fail;
    if( value != (CObject *)2 ) _fail;
    if( map.Lookup( _T( "3" ), value ) ) _fail;

    POSITION    position = map.GetStartPosition();
    CString     key;
    BOOL        bFound0 = FALSE;
    BOOL        bFound1 = FALSE;
    BOOL        bFound2 = FALSE;
    while( position != NULL ) {
        map.GetNextAssoc( position, key, value );
        if( key == _T( "0" ) ) {
            if( bFound0 ) _fail;
            if( value != (CObject *)0 ) _fail;
            bFound0 = TRUE;
        } else if( key == _T( "1" ) ) {
            if( bFound1 ) _fail;
            if( value != (CObject *)1 ) _fail;
            bFound1 = TRUE;
        } else if( key == _T( "2" ) ) {
            if( bFound2 ) _fail;
            if( value != (CObject *)2 ) _fail;
            bFound2 = TRUE;
        } else {
            _fail;
        }
    }
    if( !bFound0 ) _fail;
    if( !bFound1 ) _fail;
    if( !bFound2 ) _fail;

    map.RemoveKey( _T( "0" ) );
    if( map.Lookup( _T( "0" ), value ) ) _fail;

    _PASS;
}
