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
* Description:  Process table/etable tags
*
*   :table / :etable
*       cols='' (space separated list of widths in characters)
*       rules=both|horiz|vert|none (default: both)
*       frame=rules|box|none (default: box)
*   Supplying more column data than columns is an error
*   Text is formatted at compile time
*
****************************************************************************/

#include <cstdlib>
#include <numeric>
#include <string>
#include "table.hpp"
#include "brcmd.hpp"
#include "cell.hpp"
#include "color.hpp"
#include "document.hpp"
#include "entity.hpp"
#include "font.hpp"
#include "hpn.hpp"
#include "ipfbuffer.hpp"
#include "link.hpp"
#include "punct.hpp"
#include "util.hpp"
#include "whtspc.hpp"
#include "word.hpp"

Lexer::Token Table::parse( Lexer* lexer )
{
    Lexer::Token tok( parseAttributes( lexer ) );
    unsigned int rowCount( 0 );
    tbBorder( false );
    while( tok != Lexer::END && !( tok == Lexer::TAG && lexer->tagId() == Lexer::EUSERDOC ) ) {
        //allowed: '\n', command, :row.
        if( tok == Lexer::WHITESPACE && lexer->text()[ 0 ] == L'\n' )
            tok = document->getNextToken();     //ignore \n's
        else if( tok == Lexer::COMMAND )
            tok = document->processCommand( lexer, this );
        else if( tok == Lexer::TAG ) {
            if( lexer->tagId() == Lexer::ETABLE )
                break;
            else if( lexer->tagId() == Lexer::TROW ) {
                if( rowCount )
                    rowRule();
                Element* elt( new TableRow( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), colWidth, rules, frame ) );
                appendChild( elt );
                tok = elt->parse( lexer );
                ++rowCount;
            }
            else {
                document->printError( ERR1_TAGCONTEXT );
                parseCleanup( tok );
            }
        }
        else {
            document->printError( ERR1_TABLETEXT );
            tok = document->getNextToken();
        }
    }
    tbBorder( true );
    return tok;
}
/***************************************************************************/
Lexer::Token Table::parseAttributes( Lexer* lexer )
{
    Lexer::Token tok( document->getNextToken() );
    while( tok != Lexer::TAGEND ) {
        if( tok == Lexer::ATTRIBUTE ) {
            std::wstring key;
            std::wstring value;
            splitAttribute( lexer->text(), key, value );
            if( key == L"cols" ) {
                std::wstring::size_type idx1( 0 );
                std::wstring::size_type idx2( value.find( L' ' ) );
                while( idx1 != std::wstring::npos ) { //split value on ' '
                    colWidth.push_back(
                        static_cast< unsigned char >(
                            _wtoi( value.substr( idx1, idx2 - idx1 ).c_str() ) ) );
                    idx1 = idx2 == std::wstring::npos ? std::wstring::npos : idx2 + 1;
                    idx2 = value.find( L' ', idx1 );
                }
            }
            else if( key == L"rules" ) {
                if( value == L"none" )
                    rules = NO_RULES;
                else if( value == L"horiz" )
                    rules = HORIZONTAL;
                else if( value == L"vert" )
                    rules = VERTICAL;
                else if( value == L"both" )
                    rules = BOTH;
                else
                    document->printError( ERR2_VALUE );
            }
            else if( key == L"frame" ) {
                if( value == L"none" )
                    frame = NO_FRAME;
                else if( value == L"rules" )
                    frame = RULES;
                else if( value == L"box" )
                    frame = BOX;
                else
                    document->printError( ERR2_VALUE );
            }
            else
                document->printError( ERR1_ATTRNOTDEF );
        }
        else if( tok == Lexer::FLAG )
            document->printError( ERR1_ATTRNOTDEF );
        else if( tok == Lexer::ERROR_TAG )
            throw FatalError( ERR_SYNTAX );
        else if( tok == Lexer::END )
            throw FatalError( ERR_EOF );
        else
            document->printError( ERR1_TAGSYNTAX );
        tok = document->getNextToken();
    }
    if( colWidth.empty() )
        document->printError( ERR1_NOCOLS );
#if 0
    unsigned int total( 0 );
    for( ConstColWidthIter itr = colWidth.begin(); itr != colWidth.end(); ++itr )
        total += *itr;
    if( total > 250 )
        document->printError( ERR1_TABLEWIDTH );
#else
    //this could overflow
    if( std::accumulate( colWidth.begin(), colWidth.end(), 0 ) > 250 )
        document->printError( ERR1_TABLEWIDTH );
#endif
    return document->getNextToken();    //consume TAGEND
}
/***************************************************************************/
void Table::tbBorder( bool bottom )
{
    if( frame ) {
        std::wstring txt1;
        std::wstring txt2;
        std::wstring txt3;
        unsigned int count1( 0 );
        wchar_t      ch( 0 );
        try {
            ch = document->entity( L"&bxh." );
            if( bottom ) {
                txt1.push_back( document->entity( L"&bxll." ) );
                if( rules & VERTICAL )
                    txt2.push_back( document->entity( L"&bxas." ) );
                else
                    txt2.push_back( ch );
                txt3.push_back( document->entity( L"&bxlr." ) );
            }
            else {
                txt1.push_back( document->entity( L"&bxul." ) );
                if( rules & VERTICAL )
                    txt2.push_back( document->entity( L"&bxde." ) );
                else
                    txt2.push_back( ch );
                txt3.push_back( document->entity( L"&bxur." ) );
            }
        }
        catch( Class2Error& e ) {
            printError( e.code );
        }
        if( frame == BOX ) {
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt1) );
        }
        if( !colWidth.empty() ) {
            for( count1 = 0; count1 < colWidth.size() - 1; ++count1 ) {
                std::wstring txt( colWidth[ count1 ], ch );
                appendChild( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt) );
                appendChild( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt2) );
            }
            std::wstring txt( colWidth[ count1 ], ch );
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt) );
        }
        if( frame == BOX ) {
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt3) );
        }
        appendChild( new BrCmd( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol() ) );
    }
}
/***************************************************************************/
void Table::rowRule()
{
    if( rules & HORIZONTAL ) {
        std::wstring txt1( 1, document->entity( L"&bxlj." ) );
        std::wstring txt2;
        std::wstring txt3( 1, document->entity( L"&bxrj." ) );
        unsigned int count1( 0 );
        wchar_t      ch( document->entity( L"&bxh." ) );
        if( rules == NO_RULES )
            txt2.push_back( L' ' );
        else if( rules == HORIZONTAL )
            txt2.push_back( ch );
        else
            txt2.push_back( document->entity( L"&bxcr." ) );
        if( frame == BOX ) {
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt1) );
        }
        if( !colWidth.empty() ) {
            for( count1 = 0; count1 < colWidth.size() - 1; ++count1 ) {
                std::wstring txt( colWidth[ count1 ], ch );
                appendChild( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt) );
                appendChild( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt2) );
            }
            std::wstring txt( colWidth[ count1 ], ch );
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt) );
        }
        if( frame == BOX ) {
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt3) );
        }
        appendChild( new BrCmd( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol() ) );
    }
}
/***************************************************************************/
void Table::buildText( Cell* cell )
{
    cell->addByte( 0xFF );  //esc
    cell->addByte( 0x02 );  //size
    cell->addByte( 0x0B );  //begin monospaced
    if( cell->textFull() )
        printError( ERR1_LARGEPAGE );
}
/*****************************************************************************/
void ETable::buildText( Cell* cell )
{
    cell->addByte( 0xFF );  //esc
    cell->addByte( 0x02 );  //size
    cell->addByte( 0x0C );  //end monospaced
    if( cell->textFull() )
        printError( ERR1_LARGEPAGE );
}
/*****************************************************************************/
TableRow::~TableRow()
{
    for( ColIter itr = columns.begin(); itr != columns.end(); ++itr )
        delete *itr;
}
/*****************************************************************************/
Lexer::Token TableRow::parse( Lexer* lexer )
{
    Lexer::Token tok( parseAttributes( lexer ) );
    unsigned int colCount( 0 );
    unsigned int rows( 0 );
    std::wstring txt1( 1, document->entity( L"&bxv." ) );
    std::wstring txt2( 1, rules & Table::VERTICAL ? document->entity( L"&bxv." ) : L' ' );
    while( tok != Lexer::END && !( tok == Lexer::TAG && lexer->tagId() == Lexer::EUSERDOC ) ) {
        //allowed: '\n', command, :c.
        if( tok == Lexer::WHITESPACE && lexer->text()[ 0 ] == L'\n' )
            tok = document->getNextToken();     //ignore \n's
        else if( tok == Lexer::COMMAND )
            tok = document->processCommand( lexer, this );
        else if( tok == Lexer::TAG ) {
            if( lexer->tagId() == Lexer::ETABLE || lexer->tagId() == Lexer::TROW )
                break;
            else if( lexer->tagId() == Lexer::TCOL ) {
                unsigned char width( 0 );
                if( colCount < colWidth.size() )
                    width = colWidth[ colCount ];
                else
                    document->printError( ERR1_TABLECELLCOUNTHIGH );
                TableCol* col( new TableCol( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), width ) );
                columns.push_back( col );
                ++colCount;
                tok = col->parse( lexer );
                if( col->rows() > rows )
                    rows = col->rows();
            }
            else {
                document->printError( ERR1_TAGCONTEXT );
                parseCleanup( tok );
            }
        }
        else {
            document->printError( ERR1_TABLETEXT );
            tok = document->getNextToken();
        }
    }
    if( colCount < colWidth.size() ) {    //check for unspecified columns
        document->printError( ERR1_TABLECELLCOUNTLOW );
        while( colCount != colWidth.size() ) {
            unsigned char width( colWidth[ colCount ] );
            TableCol* col( new TableCol( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), width ) );
            columns.push_back( col );
            ++colCount;
        }
    }
    for( unsigned int count1 = 0; count1 < rows; ++count1 ) {
        if( frame == Table::BOX )
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt1) );
        for( unsigned int count2 = 0; count2 < colWidth.size(); ++count2 ) {
            if( count1 < columns[ count2 ]->rows() &&
                columns[ count2 ]->rowData( count1 ).size() ) {
                std::list< Element* >& data = columns[ count2 ]->rowData( count1 );
                for( ChildrenIter itr = data.begin(); itr != data.end(); ++itr )
                    appendChild( *itr );
            }
            else {  //it's blank
                std::wstring blank( colWidth[ count2 ], L' ' );
                appendChild( new WhiteSpace( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), blank, whiteSpace ) );
            }
            if( count2 < colWidth.size() - 1 )
                appendChild( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt2 ) );
        }
        if( frame == Table::BOX )
            appendChild( new Word( document, this, document->dataName(),
                document->lexerLine(), document->lexerCol(), txt1) );
        appendChild( new BrCmd( document, this, document->dataName(),
            document->lexerLine(), document->lexerCol() ) );
    }
    return tok;
}
/*****************************************************************************/
Lexer::Token TableCol::parse( Lexer* lexer )
{
    Lexer::Token tok( parseAttributes( lexer ) );
    bool doneF( false );
    bool inLink( false );
    int spaces( colWidth );
    unsigned int cellLine( 0 );
    unsigned int currentLine = document->dataLine();
    while( !doneF && tok != Lexer::END ) {
        if( tok == Lexer::WORD ) {
            if( currentLine < document->dataLine() ) {
                currentLine = document->dataLine();
                std::list< Element* > lst;
                data.push_back( lst );
                ++cellLine;
                spaces = colWidth;
            }
            if( spaces > 0 ) {
                std::wstring txt( lexer->text() );
                if( txt.size() > spaces ) {
                    txt.erase( spaces );    //trim text
                    document->printError( ERR1_TABLECELLTEXTWIDTH );
                }
                Word* word( new Word( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt ) );
                appendData( cellLine, word );
                spaces -= txt.size();
            }
            else
                document->printError( ERR1_TABLECELLTEXTWIDTH );
            tok = document->getNextToken();
        }
        else if( tok == Lexer::ENTITY ) {
            const std::wstring* txt( document->nameit( lexer->text() ) ); //lookup nameit
            if( txt ) {
                std::wstring* name( document->prepNameitName( lexer->text() ) );
                IpfBuffer* buffer( new IpfBuffer( name, document->dataLine(), document->dataCol(), *txt ) );
                document->pushInput( buffer );
                tok = document->getNextToken();
            }
            else {
                wchar_t ch( 0 );
                try {
                    ch = document->entity( lexer->text() ); //lookup entity
                }
                catch( Class2Error& e ) {
                    document->printError( e.code );
                }
                if( currentLine < document->dataLine() ) {
                    currentLine = document->dataLine();
                    std::list< Element* > lst;
                    data.push_back( lst );
                    ++cellLine;
                    spaces = colWidth;
                }
                if( spaces > 0 && ch ) {
                    std::wstring txt( 1, ch );
                    Entity* entity( new Entity( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), txt ) );
                    appendData( cellLine, entity );
                    --spaces;
                }
                else
                    document->printError( ERR1_TABLECELLTEXTWIDTH );
                tok = document->getNextToken();
            }
        }
        else if( tok == Lexer::PUNCTUATION ) {
            if( currentLine < document->dataLine() ) {
                currentLine = document->dataLine();
                std::list< Element* > lst;
                data.push_back( lst );
                ++cellLine;
                spaces = colWidth;
            }
            if( spaces > 0 ) {
                Punctuation* punct( new Punctuation( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), lexer->text(), false ) );
                appendData( cellLine, punct );
                --spaces;
            }
            else
                document->printError( ERR1_TABLECELLTEXTWIDTH );
            tok = document->getNextToken();
        }
        else if( tok == Lexer::WHITESPACE ) {
            if( lexer->text()[0] == L'\n' )     //ignore \n's
                tok = document->getNextToken(); 
            else if( spaces > 0 ) {
                if( currentLine < document->dataLine() ) {
                    currentLine = document->dataLine();
                    std::list< Element* > lst;
                    data.push_back( lst );
                    ++cellLine;
                    spaces = colWidth;
                }
                std::wstring txt( lexer->text() );
                if( txt.size() > spaces ) {
                    txt.erase( spaces );        //trim text
                    document->printError( ERR1_TABLECELLTEXTWIDTH );
                }
                WhiteSpace* ws( new WhiteSpace( document, this, document->dataName(),
                    document->lexerLine(), document->lexerCol(), txt, whiteSpace ) );
                appendData( cellLine, ws );
                spaces -= txt.size();
                tok = document->getNextToken();
            }
            else {
                document->printError( ERR1_TABLECELLTEXTWIDTH );
                tok = document->getNextToken();
            }
        }
        else if( tok == Lexer::COMMAND )
            tok = document->processCommand( lexer, this );
        else if( tok == Lexer::TAG ) {
            switch( lexer->tagId() ) {
            case Lexer::COLOR:
                {
                    Element* elt( new Color( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol() ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::FONT:
                {
                    Element* elt( new Font( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol() ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP1:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 1 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP2:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 2 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP3:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 3 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP4:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 4 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP5:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 5 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP6:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 6 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP7:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 7 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP8:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 8 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::HP9:
                {
                    Element* elt( new Hpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 9 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP1:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 1 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP2:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 2 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP3:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 3 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP4:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 4 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP5:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 5 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP6:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 6 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP7:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 7 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP8:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 8 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::EHP9:
                {
                    Element* elt( new EHpn( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), 9 ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                }
                break;
            case Lexer::LINK:
                {
                    Link* link( new Link( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol() ) );
                    appendData( cellLine,  link );
                    link->setNoEndTag();
                    inLink = true;
                    tok = link->parse( lexer );
                }
                break;
            case Lexer::ELINK:
                {
                    Element* elt( new ELink( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol() ) );
                    appendData( cellLine,  elt );
                    tok = elt->parse( lexer );
                    inLink = false;
                }
                break;
            case Lexer::TROW:
            case Lexer::TCOL:
            case Lexer::ETABLE:
            case Lexer::EUSERDOC:
                doneF = true;
                if( inLink ) {
                    Element* elt( new ELink( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol() ) );
                    appendData( cellLine,  elt );
                    inLink = false;
                    printError( ERR1_TABLEELINK );
                }
                if( spaces ) {
                    std::wstring txt( spaces, L' ' );
                    WhiteSpace* ws( new WhiteSpace( document, this, document->dataName(),
                        document->lexerLine(), document->lexerCol(), txt, whiteSpace ) );
                    appendData( cellLine, ws );
                }
                break;
            default:
                parseCleanup( tok );
                break;
            }
        }
        else if( tok == Lexer::ERROR_TAG ) {
            document->printError( ERR1_TAGNOTDEF );
            tok = document->getNextToken();
        }
        else if( tok == Lexer::ERROR_ENTITY ) {
            document->printError( ERR1_TAGNOTDEF );
            tok = document->getNextToken();
        }
    }    
    return tok;
}

