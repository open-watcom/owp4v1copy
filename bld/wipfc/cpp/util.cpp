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
* Description:  Some utility functions
*
****************************************************************************/

#include <cstring>
#include <string>

void killQuotes( char * text )
{
    if( *text == '"' || *text == '\'' ) {
        char quote = *text;
        std::memmove( text, text + 1, std::strlen( text ) * sizeof( char ) );
        char *end( std::strrchr( text, quote ) );
        if ( end )
            *end = '\0';
    }
}
/*****************************************************************************/
void killQuotes( wchar_t * text )
{
    if( *text == L'"' || *text == '\'' ) {
        wchar_t quote = *text;
        std::memmove( text, text + 1, std::wcslen( text ) * sizeof( wchar_t ) );
        wchar_t *end( std::wcsrchr( text, quote ) );
        if ( end )
            *end = L'\0';
    }
}
/*****************************************************************************/
void killQuotes( std::string& val )
{
    if ( val[0] == '"' || val[0] == '\'' )
        {
        val.erase( 0, 1 );
        if( val[ val.size() - 1 ] == '"' || val[ val.size() - 1 ] == '\'' )
            val.erase( val.size() - 1, 1 );
        }
}
/*****************************************************************************/
void killQuotes( std::wstring& val )
{
    if ( val[0] == L'"' || val[0] == L'\'' )
        {
        val.erase( 0, 1 );
        if( val[ val.size() - 1 ] == L'"' || val[ val.size() - 1 ] == L'\'' )
            val.erase( val.size() - 1, 1 );
        }
}
/*****************************************************************************/
void splitAttribute( const std::wstring& text, std::wstring& key, std::wstring& value)
{
    std::wstring::size_type index( text.find( '=', 0 ) );
    key = text.substr( 0, index );
    value = text.substr( index + 1 );
    killQuotes( value );
}

