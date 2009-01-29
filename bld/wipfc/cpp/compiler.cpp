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
* Description:  The OS/2 help compiler.
*
****************************************************************************/


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "compiler.hpp"
#include "document.hpp"
#include "errors.hpp"
#include "ipfbuffer.hpp"
#include "ipffile.hpp"
#include "lexer.hpp"

Compiler::Compiler():
    warningLevel( 3 ),
    parseContinuously( true ),
    search( true ),
    printBanner( true ),
    xref( false ),
    lexer( new Lexer() ),
    loc( "en_US" )
{
}
/*****************************************************************************/
Compiler::~Compiler()
{
    while( inFiles.size() )
        popInput();
    for( FileNameIter iter = fileNames.begin(); iter != fileNames.end(); ++iter)
        delete *iter;
}
/*****************************************************************************/
void Compiler::setInputFile( const char *name )
{
    wchar_t fbuffer[ _MAX_PATH ];
    if( std::mbstowcs( fbuffer, name, sizeof( fbuffer ) / sizeof( wchar_t ) ) == -1)
        throw FatalError( ERR_T_CONV );
    std::wstring* wname( new std::wstring( fbuffer ) );
    wname = addFileName( wname );
    inFiles.push_back( new IpfFile( wname ) );
}
/*****************************************************************************/
std::wstring* Compiler::addFileName( std::wstring* name )
{
    std::pair< FileNameIter, bool > status( fileNames.insert( name ) );
    if( !status.second )
        delete name;
    return *status.first;
}
/*****************************************************************************/
int Compiler::compile()
{
    int retval( EXIT_SUCCESS );
    std::auto_ptr< Document > doc( new Document( *this, loc ) );
    doc->setOutputType( outType );
    doc->parse( lexer.get() );
    doc->build();
    std::FILE* out( std::fopen( outFileName.c_str() , "wb" ) );
    if( !out )
        throw FatalIOError( ERR_OPEN, L"for inf or hlp output" );
    try {
        doc->write( out );
    }
    catch( FatalError& e ) {
        retval = EXIT_FAILURE;
        printError( e.code );
    }
    catch( FatalIOError& e ) {
        retval = EXIT_FAILURE;
        printError( e.code, e.fname );
    }
    std::fclose( out );
    return retval;
}
/*****************************************************************************/
//Error message format is <fullfilename:line:col> errnum: text [optional info]
void Compiler::printError( ErrCode c ) const
{
    if( c <= ERR_LAST || warningLevel > 2 ||
       ( c <= ERR1_LAST && warningLevel > 0 ) ||
       ( c <= ERR2_LAST && warningLevel > 1 )) {
/*
           std::cout << '<';
           std::cout << dataName();
           std::cout << ':';
           std::cout << lexerLine();
           std::cout << ':';
           std::cout << lexerCol();
           std::cout << "> " << ( c > ERR_LAST ? "Warning" : "Fatal Error" );
           std::cout << std::setw( 2 ) << std::setfill( '0' );
           std::cout << static_cast< unsigned int >( c );
           std::cout << ': ' << ErrText[ c ] << std::endl;
*/
        std::fprintf( stdout, "<%ls:%u:%u> %s %02u: %s\n",
            inFiles.size() ? dataName()->c_str() : L"(no current file)",
            lexerLine(), lexerCol(),
            c > ERR_LAST ? "Warning" : "Fatal Error",
            static_cast< unsigned int >( c ),
            ErrText[ c ] );
    }
}
/*****************************************************************************/
//Error message format is <fullfilename:line:col> errnum: text [optional info]
void Compiler::printError( ErrCode c, std::wstring& txt ) const
{
    if( c <= ERR_LAST || warningLevel > 2 ||
       ( c <= ERR1_LAST && warningLevel > 0 ) ||
       ( c <= ERR2_LAST && warningLevel > 1 )) {
/*
           std::cout << '<';
           std::cout << dataName();
           std::cout << ':';
           std::cout << lexerLine();
           std::cout << ':';
           std::cout << lexerCol();
           std::cout << "> " << ( c > ERR_LAST ? "Warning" : "Fatal Error" );
           std::cout << std::setw( 2 ) << std::setfill( '0' );
           std::cout << static_cast< unsigned int >( c );
           std::cout << ': ' << ErrText[ c ] << txt << std::endl;
*/
        std::fprintf( stdout, "<%ls:%u:%u> %s %02u: %s %ls\n",
            inFiles.size() ? dataName()->c_str() : L"(no current file)",
            lexerLine(), lexerCol(),
            c > ERR_LAST ? "Warning" : "Fatal Error",
            static_cast< unsigned int >( c ),
            ErrText[ c ], txt.c_str() );
    }
}
/*****************************************************************************/
//Error message format is <fullfilename:line:col> errnum: text [optional info]
void Compiler::printError( ErrCode c, const std::wstring* name, unsigned int row,
    unsigned int col ) const
{
    if( c <= ERR_LAST || warningLevel > 2 ||
       ( c <= ERR1_LAST && warningLevel > 0 ) ||
       ( c <= ERR2_LAST && warningLevel > 1 )) {
/*
           std::cout << '<';
           std::cout << inFiles[inFiles.size() - 1]->name();
           std::cout << ':';
           std::cout << row;
           std::cout << ':';
           std::cout << col;
           std::cout << "> " ( c > ERR_LAST ? "Warning" : "Fatal Error" );
           std::cout << std::setw( 2 ) << std::setfill( '0' );
           std::cout << static_cast< unsigned int >( c );
           std::cout << ' ' << ErrText[ c ] << std::endl;
*/
        std::fprintf( stdout, "<%ls:%u:%u> %s %02u: %s\n",
            name->c_str(), row, col,
            c > ERR_LAST ? "Warning" : "Fatal Error",
            static_cast< unsigned int >( c ),
            ErrText[ c ] );
    }
}
/*****************************************************************************/
Lexer::Token Compiler::getNextToken()
{
    Lexer::Token tok( lexer->lex( inFiles[inFiles.size() - 1] ));
    if( parseContinuously && tok == Lexer::END ) {
        popInput();
        if( !inFiles.empty() )
            tok = lexer->lex( inFiles[inFiles.size() - 1] );
    }
    return tok;
}
/*****************************************************************************/
void Compiler::popInput( )
{
    IpfData* f( inFiles[inFiles.size() - 1] );
    inFiles.pop_back();
    delete f;
}
