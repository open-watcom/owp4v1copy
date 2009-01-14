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
* Description:  A (possibly compressed) block of bitmap graphics data
*
****************************************************************************/

#include <algorithm>
#include "btmpblk.hpp"
#include "errors.hpp"
#ifdef CHECKCOMP
    #include <iostream>
#endif

BitmapBlock::BitmapBlock( std::uint16_t b, std::uint8_t t ) :
    bitBuffer( 0L ), bitCount( 0 ), bitsPerCode( INITBITS ), hashingShift( MAXBITS - 8 ),
    maxCode( maxVal( INITBITS ) ), checkCount( 100 ), blockSize( b ),
    type( t )
{
}
/***************************************************************************/
void BitmapBlock::write( std::FILE* out ) const
{
    if( std::fwrite( &size, sizeof( std::uint16_t ), 1, out ) != 1 )
        throw FatalError( ERR_WRITE );
    if( std::fputc( type, out ) == EOF )
        throw FatalError( ERR_WRITE );
    if( std::fwrite( &data[0], sizeof( std::uint8_t ), data.size(), out ) != data.size() )
        throw FatalError( ERR_WRITE );
}
/***************************************************************************/
/* This is an LZW/LZC compressor. It is based on code in 2 articles from
   Dr. Dobb's Journal:
    1. Mark R. Nelson, October 1989
    2. Shawn M. Regan, January 1990
*/
std::uint32_t BitmapBlock::compress( std::FILE* in )
{
    size_t block;
    if( type ) {
        std::vector< std::uint8_t > buffer( blockSize );
        block = std::fread( &buffer[0], sizeof( std::uint8_t ), blockSize, in );
        if( !block )
            throw FatalError( ERR_READ );       //should have seen EOF elsewhere
        if( block < blockSize ) {
            blockSize = static_cast< std::uint16_t >( block );
            buffer.resize( blockSize );
        }
        std::vector< std::uint16_t > code( TABLESIZE, UNDEFINED );
        std::vector< std::uint16_t > prefix( TABLESIZE );
        std::vector< std::uint8_t >  append( TABLESIZE );
        std::vector< std::uint8_t >::const_iterator itr( buffer.begin() );
        size_t          bytesIn( 1 );
        size_t          bytesOut( 0 );
        size_t          checkPoint( 0 );
        size_t          oldCompRatio( 100 );
        size_t          newCompRatio;
        std::uint16_t   nextCode( FIRST );
        std::uint16_t   codeIndex;
        std::uint16_t   stringCode( *itr );
        std::uint16_t   character;
        while( ++itr != buffer.end() ) {
            character = static_cast< std::uint16_t >( *itr );
            ++bytesIn;
            codeIndex = findMatch( code, prefix, append, stringCode, character );
            if( code[ codeIndex ] != UNDEFINED )
                stringCode = code[ codeIndex ];
            else {
                if( nextCode <= maxCode ) {
                    code[ codeIndex ] = nextCode++;
                    prefix[ codeIndex ] = stringCode;
                    append[ codeIndex ] = static_cast< std::uint8_t >( character );
                }
                bytesOut += outputCode( stringCode );
                stringCode = character;
                if( nextCode > maxCode ) {                  // if table full
                    if( bitsPerCode < MAXBITS )             // if more bits available
                        maxCode = maxVal( ++bitsPerCode );  // use 'em
                    else if( bytesIn > checkPoint ) {       // else if check time
                        if( bitsPerCode == MAXBITS ) {
                            newCompRatio = bytesOut * 100 / bytesIn;
                            if( newCompRatio > oldCompRatio ) { //check for compression loss
                                outputCode( CLEAR );
                                bitsPerCode = INITBITS;
                                nextCode = FIRST;
                                maxCode = maxVal( INITBITS );
                                bytesIn = 0;
                                bytesOut = 0;
                                oldCompRatio = 100;
                                std::fill( code.begin(), code.end(), UNDEFINED );
                            }
                            else
                                oldCompRatio = newCompRatio;
                        }
                        checkPoint = bytesIn + checkCount;    // Set new checkpoint
                    }
                }
            }
        }
        bytesOut += outputCode( stringCode );
        if( nextCode == maxCode )               // Special case: increment on EOF
            ++bitsPerCode;
        bytesOut += outputCode( TERMINATE );
        if( bitCount > 0 && bitBuffer != 0L )
            bytesOut += outputCode( 0 );            //flush
        //bytesOut += outputCode( 0 );
        //bytesOut += outputCode( 0 );
        size = static_cast< std::uint16_t >( ( data.size() + 1 ) * sizeof( std::uint8_t ) );
#ifdef CHECKCOMP
        std::vector< std::uint8_t> buffer2( blockSize );
        expand( buffer2 );
        if( buffer2.size() != buffer.size() )
            std::cout<< "    Expanded data size is not equal to the original" << std::endl;
        std::cout << "    Index Original Restored" << std::endl;
        size_t index( 0 );
        InputIter in( buffer.begin() );
        OutputIter out( buffer2.begin() );
        while ( in != buffer.end() ) {
            if( *in != *out )
                std::printf( "    %5u %8x %8x\n", index, *in, *out );
            ++in;
            ++out;
            ++index;
        }
#endif
    }
    else {
        data.resize( blockSize );
        block = std::fread( &data[0], sizeof( std::uint8_t ), blockSize, in );
        if( !block )
            throw FatalError( ERR_READ );       //should have seen EOF elsewhere
        if( block < blockSize )
            data.resize( block );
        size = static_cast< std::uint16_t >( block + 1 );
    }
    return block;
}
/***************************************************************************/
std::int16_t BitmapBlock::findMatch( std::vector< std::uint16_t >& code,
    std::vector< std::uint16_t >& prefix, std::vector< std::uint8_t >& append,
    std::int16_t hashPrefix, std::uint16_t character ) const
{
    std::int16_t index( ( character << hashingShift ) ^ hashPrefix );
    std::int16_t offset( ( index == 0 ) ? 1 : TABLESIZE - index );
    while( true ) {
        if( code[ index ] == UNDEFINED )
            return index;
        if( prefix[ index ] == hashPrefix && append[ index ] == character)
            return index;
        index -= offset;
        if( index < 0 )
            index += TABLESIZE;
    }
}
/***************************************************************************/
std::uint16_t BitmapBlock::outputCode( std::uint16_t code )
{
    std::uint16_t bytesOut( 0 );
    bitBuffer |= static_cast< std::uint32_t >( code ) << ( 32 - bitsPerCode - bitCount);
    bitCount += bitsPerCode;
    while( bitCount >= 8 ) {
        //debugging
        //std::uint8_t byte( static_cast< std::uint8_t >( bitBuffer >> 24 ) );
        //size_t bytesWritten( data.size() );
        //data.push_back( byte );
        data.push_back( static_cast< std::uint8_t >( bitBuffer >> 24 ) );
        bitBuffer <<= 8;
        bitCount -= 8;
        ++bytesOut;
    }
    return bytesOut;
}
/***************************************************************************/
#ifdef CHECKCOMP
// Modified from the references above to follow what NewView does
void BitmapBlock::expand( std::vector< std::uint8_t >& output )
{
    std::vector< std::uint16_t > prefix( TABLESIZE );
    std::vector< std::uint8_t >  append( TABLESIZE );
    std::vector< std::uint8_t >  decode( TABLESIZE );
    DecodeIter string;
    OutputIter out( output.begin() );
    //OutputIter outEnd( output.end() );  //debugging
    InputIter in( data.begin() );
    //InputIter inEnd( data.end() );      //debugging
    std::uint16_t nextCode( FIRST );
    std::uint16_t newCode;
    std::uint16_t oldCode;
    std::uint16_t character;
    std::uint8_t  lastCode;
    bool clear( true );
    bitBuffer = 0L;
    bitCount = 0;
    bitsPerCode = INITBITS;
    maxCode = maxVal( INITBITS );
    try {
        while( ( newCode = getCode( in ) ) != TERMINATE ) {
            if( clear ) {               //(re-)initialize
                clear = false;
                oldCode = newCode;
                character = newCode;
                lastCode = static_cast< std::uint8_t >( newCode );//NewView
                *out = static_cast< std::uint8_t >( newCode );
                ++out;
                continue;
            }
            if( newCode == CLEAR ) {    //Clear string table
                clear = true;
                bitsPerCode = INITBITS;
                nextCode = FIRST;
                maxCode = maxVal( INITBITS );
                continue;
            }
            if( newCode >= nextCode) {  //Check for string+char+string
                decode[0] = static_cast< std::uint8_t >( character );
                string = decodeString( prefix, append, decode.begin() + 1, oldCode );
            }
            else
                string = decodeString( prefix, append, decode.begin(), newCode );
            character = *string;        //Output decoded string in reverse
            while( string >= decode.begin() ) {
                if( out == output.end() )
                    throw std::out_of_range( "    Destination limit exceeded" );
                *out = *string;
                ++out;
                --string;
            }
            lastCode = *( ++string );
            if( nextCode <= maxCode ) { //Add to string table if not full
                prefix[ nextCode ] = oldCode;
                append[ nextCode ] = static_cast< std::uint8_t >( character );
                ++nextCode;
                if( nextCode == maxCode && bitsPerCode < MAXBITS )
                    maxCode = maxVal( ++bitsPerCode );
            }
            oldCode = newCode;
        }
        if( out != output.end() )
            std::cout << "    Adding fill bytes..." << std::endl;
        while( out != output.end() ) {
            *out = lastCode;
            ++out;
        }
    }
    catch( std::out_of_range& e ) {
        std::cerr << e.what() << std::endl;
    }
}
/***************************************************************************/
std::uint16_t BitmapBlock::getCode( InputIter& in )
{
    std::uint16_t retval;
    while( bitCount <= 24 ) {
        if( in != data.end() ) {
            bitBuffer |= static_cast< std::uint32_t >( *in ) << ( 24 - bitCount );
            ++in;
        }
        else
            //bitBuffer |= 0;
            bitBuffer |= static_cast< std::uint32_t >( EOF ) << ( 24 - bitCount );
        bitCount += 8;
    }
    if( in != data.end() )
        retval = static_cast< std::uint16_t >( bitBuffer >> ( 32 - bitsPerCode ) );
    else
        retval = TERMINATE;
    //retval = static_cast< std::uint16_t >( bitBuffer >> ( 32 - bitsPerCode ) );
    bitBuffer <<= bitsPerCode;
    bitCount -= bitsPerCode;
    return retval;
}
/*******************************************************************************************/
BitmapBlock::DecodeIter BitmapBlock::decodeString( std::vector< std::uint16_t >& prefix,
    std::vector< std::uint8_t >& append, DecodeIter decode, std::uint16_t code )
{
    size_t index = 0;
    while( code > 255 ) {
        *decode = append[ code ];
        ++decode;
        code = prefix[ code ];
        if( index >= TABLESIZE )
            throw std::out_of_range( "    Decode stack size limit exceeded" );
        ++index;
    }
    *decode = static_cast< std::uint8_t >( code );
    return decode;
}

#endif //CHECKCOMP
