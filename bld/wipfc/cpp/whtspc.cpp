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
* Description:  A WHITESPACE element
*
****************************************************************************/

#include "whtspc.hpp"
#include "cell.hpp"
#include "document.hpp"

WhiteSpace::WhiteSpace( Document* d, Element* p, const std::wstring* f, unsigned int r,
    unsigned int c, const std::wstring& tx, Tag::WsHandling w, bool ts ) :
    whiteSpace( w ), txt( tx ), Text( d, p, f, r, c, ts )
{
    if( w == Tag::SPACES ) {
        GlobalDictionaryWord* word( new GlobalDictionaryWord( tx ) );
        text = document->addWord( word );   //insert into global dictionary
    }
}
/***************************************************************************/
Lexer::Token WhiteSpace::parse( Lexer* lexer )
{
    txt = lexer->text();                    //get text from lexer
    if( whiteSpace == Tag::SPACES && lexer->text()[0] != L'\n') {
        GlobalDictionaryWord* word( new GlobalDictionaryWord( txt ) );
        text = document->addWord( word );   //insert into global dictionary
    }
    if( !document->autoSpacing() ) {
        document->toggleAutoSpacing();
        Lexer::Token t( document->lastToken() );
        if( t == Lexer::WORD || t == Lexer::ENTITY || t == Lexer::PUNCTUATION )
            document->lastText()->setToggleSpacing();
    }
    document->setLastPrintable( Lexer::WHITESPACE, this );
    return document->getNextToken();
}
/***************************************************************************/
void WhiteSpace::buildText( Cell* cell )
{
    if( whiteSpace != Tag::NONE && txt[0] == L'\n' )
        cell->addByte( 0xFD );
    else if( whiteSpace == Tag::SPACES && text ) {
        Text::buildText( cell );
    }
    else if( whiteSpace == Tag::LITERAL ) {
        size_t length = txt.length();
        if( length > 2 ) {
            //toggleSpacing on the first and last, do n - 1
            cell->addByte( 0xFC );
            for( size_t count1 = 0; count1 < length - 1; ++count1 )
                cell->addByte( 0xFE );
            cell->addByte( 0xFC );
            cell->addByte( 0xFE );
        }
        else if( length == 2 ) {
            cell->addByte( 0xFE );
        }
    }
    if( cell->textFull() )
        printError( ERR1_LARGEPAGE );
}
/***************************************************************************/
void LiteralWhiteSpace::buildText( Cell* cell )
{
    if( toggleSpacing )
        cell->addByte( 0xFC );
    cell->addByte( 0xFE );
    if( cell->textFull() )
        printError( ERR1_LARGEPAGE );
}
