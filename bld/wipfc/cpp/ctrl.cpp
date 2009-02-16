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
* Description:  Process ctrl tag
*   :ctrl
*       ctrlid=[::alphanum::]+
*       controls='' (Search, Print, Index, Contents, Esc, Back, Forward, id)
*       page
*       coverpage
*   Must be a child of :ctrldef
*   Must follow :pbutton
*
****************************************************************************/

#include "ctrl.hpp"
#include "controls.hpp"
#include "document.hpp"
#include "util.hpp"

Lexer::Token Ctrl::parse( Lexer* lexer )
{
    Lexer::Token tok( document->getNextToken() );
    while( tok != Lexer::TAGEND ) {
        if( tok == Lexer::ATTRIBUTE ) {
            std::wstring key;
            std::wstring value;
            splitAttribute( lexer->text(), key, value );
            if( key == L"ctrlid" )
                ctrlid = value;
            else if( key == L"controls" )
                controls = value;
            else
                document->printError( ERR1_ATTRNOTDEF );
        }
        else if ( tok == Lexer::FLAG ) {
            if( lexer->text() == L"page" )
                page = true;
            else if( lexer->text() == L"coverpage" )
                coverpage = true;
            else
                document->printError( ERR1_ATTRNOTDEF );
        }
        else if( tok == Lexer::ERROR_TAG )
            throw FatalError( ERR_SYNTAX );
        else if( tok == Lexer::END )
            throw FatalError( ERR_EOF );
        else
            document->printError( ERR1_TAGSYNTAX );
        tok = document->getNextToken();
    }
    return document->getNextToken();
}
/***************************************************************************/
void Ctrl::build( Controls* ctrls)
{
    ControlGroup grp( ctrlid );
    if( !controls.empty() ) {
        std::wstring::size_type p1( 0 );
        while( p1 < std::wstring::npos ) {
            std::wstring::size_type p2( controls.find( L' ', p1 ) );
            std::wstring temp( controls.substr( p1, p2 - p1 ) );
            ControlButton* btn( ctrls->getButtonById( temp ) ); //check if button is present
            if( btn )
                grp.addButtonIndex( btn->index() );
            else {
                if( temp == L"Esc" )
                    grp.addButtonIndex( 0 );
                else if( temp == L"Search" )
                    grp.addButtonIndex( 1 );
                else if( temp == L"Print" )
                    grp.addButtonIndex( 2 );
                else if( temp == L"Index" )
                    grp.addButtonIndex( 3 );
                else if( temp == L"Contents" )
                    grp.addButtonIndex( 4 );
                else if( temp == L"Back" )
                    grp.addButtonIndex( 5 );
                else if( temp == L"Forward" )
                    grp.addButtonIndex( 6 );
                else
                    document->printError( ERR3_NOBUTTON );
            }
            p1 = p2 == std::wstring::npos ? std::wstring::npos : p2 + 1;
        }
    ctrls->addGroup( grp );
    if( coverpage )
        ctrls->setCover( ctrls->group()->index() + 1);
    }
}

