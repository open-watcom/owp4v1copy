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
* Description:  Process acviewport tag
*
*   :acviewport
*       dll=[a-zA-z][a-zA-z0-9]*
*       objectname=[a-zA-z][a-zA-z0-9]*
*       objectinfo=[a-zA-z][a-zA-z0-9]*
*       objectid=[a-zA-z][a-zA-z0-9]*
*       vpx=([0-9]+[c|x|p|%]) | (left|center|right)
*       vpy=([0-9]+[c|x|p|%]) | (top|center|bottom)
*       vpcx=([0-9]+[c|x|p|%])
*       vpcy=([0-9]+[c|x|p|%])
*
****************************************************************************/

#include "acvwport.hpp"
#include "cell.hpp"
#include "document.hpp"
#include "errors.hpp"
#include "util.hpp"

Lexer::Token AcViewport::parse( Lexer* lexer )
{
    document->printError( ERR3_NOTSUPPORTED );
    Lexer::Token tok( parseAttributes( lexer ) );
    return tok;
}
/***************************************************************************/
Lexer::Token AcViewport::parseAttributes( Lexer* lexer )
{
    Lexer::Token tok( document->getNextToken() );
    while( tok != Lexer::TAGEND ) {
        if( tok == Lexer::ATTRIBUTE ) {
            std::wstring key;
            std::wstring value;
            splitAttribute( lexer->text(), key, value );
            if( key == L"dll" ) {
                dll = value;
            }
            else if( key == L"objectname" )
                objectName = value;
            else if( key == L"objectinfo" )
                objectInfo = value;
            else if( key == L"objectid" )
                objectId = value;
            else if( key == L"vpx" ) {
                if( value == L"left" ) {
                    origin.xPosType = ExtTocEntry::DYNAMIC;
                    origin.xpos = ExtTocEntry::DYNAMIC_LEFT;
                }
                else if( value == L"center" ) {
                    origin.xPosType = ExtTocEntry::DYNAMIC;
                    origin.xpos = ExtTocEntry::DYNAMIC_CENTER;
                }
                else if( value == L"right" ) {
                    origin.xPosType = ExtTocEntry::DYNAMIC;
                    origin.xpos = ExtTocEntry::DYNAMIC_RIGHT;
                }
                else if( value == L"top" || value == L"bottom" )
                    document->printError( ERR2_VALUE );
                else {
                    wchar_t *end;
                    unsigned long int x( std::wcstoul( value.c_str(), &end, 10 ) );
                    origin.xpos = static_cast< unsigned short >( x );
                    if( *end == L'c' )
                        origin.xPosType = ExtTocEntry::ABSOLUTE_CHAR;
                    else if( *end == L'%' )
                        origin.xPosType = ExtTocEntry::RELATIVE_PERCENT;
                    else if( *end == L'x' )
                        origin.xPosType = ExtTocEntry::ABSOLUTE_PIXEL;
                    else if( *end == L'p' )
                        origin.xPosType = ExtTocEntry::ABSOLUTE_POINTS;
                    else
                        document->printError( ERR2_VALUE );
                }
                if( origin.xPosType == ExtTocEntry::DYNAMIC && size.widthType != ExtTocEntry::RELATIVE_PERCENT )
                    document->printError( ERR3_MIXEDUNITS );
            }
            else if( key == L"vpy" ) {
                if( value == L"top" ) {
                    origin.yPosType = ExtTocEntry::DYNAMIC;
                    origin.ypos = ExtTocEntry::DYNAMIC_TOP;
                }
                else if( value == L"center" ) {
                    origin.yPosType = ExtTocEntry::DYNAMIC;
                    origin.ypos = ExtTocEntry::DYNAMIC_CENTER;
                }
                else if( value == L"bottom" ) {
                    origin.yPosType = ExtTocEntry::DYNAMIC;
                    origin.ypos = ExtTocEntry::DYNAMIC_BOTTOM;
                }
                else if( value == L"left" || value == L"right" )
                    document->printError( ERR2_VALUE );
                else {
                    wchar_t *end;
                    unsigned long int y( std::wcstoul( value.c_str(), &end, 10 ) );
                    origin.ypos = static_cast< unsigned short >( y );
                    if( *end == L'c' )
                        origin.yPosType = ExtTocEntry::ABSOLUTE_CHAR;
                    else if( *end == L'%' )
                        origin.yPosType = ExtTocEntry::RELATIVE_PERCENT;
                    else if( *end == L'x' )
                        origin.yPosType = ExtTocEntry::ABSOLUTE_PIXEL;
                    else if( *end == L'p' )
                        origin.yPosType = ExtTocEntry::ABSOLUTE_POINTS;
                    else
                        document->printError( ERR2_VALUE );
                }
                if( origin.yPosType == ExtTocEntry::DYNAMIC && size.heightType != ExtTocEntry::RELATIVE_PERCENT )
                    document->printError( ERR3_MIXEDUNITS );
            }
            else if( key == L"vpcx" ) {
                if( value == L"left" ||
                    value == L"center" ||
                    value == L"right" ||
                    value == L"top" ||
                    value == L"bottom" )
                    document->printError( ERR2_VALUE );
                else {
                    wchar_t *end;
                    unsigned long int width = std::wcstoul( value.c_str(), &end, 10 );
                    size.width = static_cast< unsigned short >( width );
                    if( *end == L'c' )
                        size.widthType = ExtTocEntry::ABSOLUTE_CHAR;
                    else if( *end == L'%' )
                        size.widthType = ExtTocEntry::RELATIVE_PERCENT;
                    else if( *end == L'x' )
                        size.widthType = ExtTocEntry::ABSOLUTE_PIXEL;
                    else if( *end == L'p' )
                        size.widthType = ExtTocEntry::ABSOLUTE_POINTS;
                    else
                        document->printError( ERR2_VALUE );
                }
                if( origin.xPosType == ExtTocEntry::DYNAMIC && size.widthType != ExtTocEntry::RELATIVE_PERCENT )
                    document->printError( ERR3_MIXEDUNITS );
            }
            else if( key == L"vpcy" ) {
                if( value == L"left" ||
                    value == L"center" ||
                    value == L"right" ||
                    value == L"top" ||
                    value == L"bottom" )
                    document->printError( ERR2_VALUE );
                else {
                    wchar_t *end;
                    unsigned long int height = std::wcstoul( value.c_str(), &end, 10 );
                    size.height = static_cast< unsigned short >( height );
                    if( *end == L'c' )
                        size.heightType = ExtTocEntry::ABSOLUTE_CHAR;
                    else if( *end == L'%' )
                        size.heightType = ExtTocEntry::RELATIVE_PERCENT;
                    else if( *end == L'x' )
                        size.heightType = ExtTocEntry::ABSOLUTE_PIXEL;
                    else if( *end == L'p' )
                        size.heightType = ExtTocEntry::ABSOLUTE_POINTS;
                    else
                        document->printError( ERR2_VALUE );
                }
                if( origin.yPosType == ExtTocEntry::DYNAMIC && size.heightType != ExtTocEntry::RELATIVE_PERCENT )
                    document->printError( ERR3_MIXEDUNITS );
            }
            else
                document->printError( ERR1_ATTRNOTDEF );
        }
        else if( tok == Lexer::FLAG ) {
            document->printError( ERR1_ATTRNOTDEF );
        }
        else if( tok == Lexer::ERROR_TAG )
            throw FatalError( ERR_SYNTAX );
        else if( tok == Lexer::END )
            throw FatalError( ERR_EOF );
        tok = document->getNextToken();
    }
    return document->getNextToken(); //consume TAGEND;
}
/***************************************************************************/
void AcViewport::buildText( Cell* cell )
{
    //FIXME: how do we encode this?
    cell = cell;
}


