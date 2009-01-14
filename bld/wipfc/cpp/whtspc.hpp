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

#ifndef WHITESPACE_INCLUDED
#define WHITESPACE_INCLUDED

#include "text.hpp"
#include "tag.hpp"

class WhiteSpace : public Text {
public:
    WhiteSpace( Document* d, Element* p, const std::wstring* f, unsigned int r,
        unsigned int c, Tag::WsHandling w = Tag::NONE ) :
        whiteSpace( w ), Text( d, p, f, r, c ) { };
    WhiteSpace( Document* d, Element* p, const std::wstring* f, unsigned int r,
        unsigned int c, const std::wstring& tx, Tag::WsHandling w = Tag::NONE,
        bool ts = false );
    ~WhiteSpace() { };
    Lexer::Token parse( Lexer* lexer );
    void buildText( Cell* cell );
private:
    WhiteSpace( const WhiteSpace& rhs );            //no copy
    WhiteSpace& operator=( const WhiteSpace& rhs ); //no assignment
    std::wstring txt;
    Tag::WsHandling whiteSpace;
};

class LiteralWhiteSpace : public Element {
public:
    LiteralWhiteSpace( Document* d, Element* p, const std::wstring* f, unsigned int r,
        unsigned int c, bool ts = false ) : Element( d, p, f, r, c ), toggleSpacing( ts ) { };
    ~LiteralWhiteSpace() { } ;
    Lexer::Token parse( Lexer* lexer ) { lexer = lexer; return Lexer::END; };
    void buildText( Cell* cell );
private:
    LiteralWhiteSpace( const LiteralWhiteSpace& rhs );              //no copy
    LiteralWhiteSpace& operator=( const LiteralWhiteSpace& rhs );   //no assignment
    bool toggleSpacing;
};

#endif //WHITESPACE_INCLUDED
