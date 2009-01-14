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
    }
    return document->getNextToken();
}
/***************************************************************************/
void Ctrl::build( Controls* ctrls)
{
    ControlGroup grp( ctrlid );
    ctrls->addGroup( grp );
    if( page )
        ;//FIXME: don't know what to do with this
    if( coverpage )
        ctrls->setCover( grp.index() );
    if( !controls.empty() ) {
        std::wstring::size_type p1( 0 );
        while( p1 < std::wstring::npos ) {
            std::wstring::size_type p2( controls.find( L' ', p1 ) );
            std::wstring temp( controls.substr( p1, p2 - p1 ) );
            ControlButton* btn( ctrls->getButtonById( temp ) ); //check if button is present
            if( btn == 0 ) {                       //if not, add specified button
                if( temp == L"Previous" ) {
                    ControlButton btn( L"Previous", 1, L"Pr~evious" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Search" ) {
                    ControlButton btn( L"Search", 2, L"~Search" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Print" ) {
                    ControlButton btn( L"Print", 3, L"~Print" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Index" ) {
                    ControlButton btn( L"Index", 4, L"~Index" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Contents" ) {
                    ControlButton btn( L"Contents", 5, L"Con~tents" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Back" ) {
                    ControlButton btn( L"Back", 6, L"~Back" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else if( temp == L"Forward" ) {
                    ControlButton btn( L"Forward", 7, L"~Forward" );
                    ctrls->addButton( btn );
                    ctrls->group()->addButtonIndex( btn.index() );
                }
                else
                    document->printError( ERR3_NOBUTTON );
            }
            p1 = p2 == std::wstring::npos ? std::wstring::npos : p2 + 1;
        }
    }
}

