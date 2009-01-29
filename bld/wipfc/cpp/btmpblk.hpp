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

#ifndef BITMAPBLOCK_INCLUDED
#define BITMAPBLOCK_INCLUDED

#include <cstdint>
#include <cstdio>
#include <vector>

// BitMapBlock
class BitmapBlock {
public:
    BitmapBlock() { };
    BitmapBlock( std::uint16_t b, std::uint8_t t );
    void write( std::FILE* out ) const;
    std::uint32_t compress( std::FILE* in );
    unsigned int totalSize() const
    { return sizeof( std::uint16_t ) + ( data.size() + 1 ) * sizeof( std::uint8_t ); };
private:
    enum lzwBits {
        INITBITS = 9,
        MAXBITS = 12
    };
    enum lzwTSize {
        TABLESIZE = 5021
    };
    enum lzwCodes {
        CLEAR = 256,
        TERMINATE,
        FIRST,
        UNDEFINED = 0xFFFF
    };
    std::uint32_t   bitBuffer;      //buffer for variable length codes
    size_t          bitCount;       //bits in the buffer
    size_t          bitsPerCode;    //code size
    size_t          hashingShift;
    size_t          maxCode;
    size_t          checkCount;     //when to check for degradation
    std::uint16_t   blockSize;      //uncompressed data size
    std::uint16_t   size;           //starting with next field
    std::uint8_t    type;           //0 == uncompressed, 2 == LZW compressed
    std::vector< std::uint8_t > data;

    size_t maxVal( unsigned int n ) { return ( 1 << n ) - 1; };
    //check for a matching code
    std::int16_t findMatch( std::vector< std::uint16_t >& code,
        std::vector< std::uint16_t >& prefix,
        std::vector< std::uint8_t >& append,
        std::int16_t hashPrefix, std::uint16_t character ) const;
    std::uint16_t outputCode( std::uint16_t code );
    std::uint16_t flushCode( void );
//#define CHECKCOMP
#ifdef CHECKCOMP
    typedef std::vector< std::uint8_t >::iterator DecodeIter;
    typedef std::vector< std::uint8_t >::iterator OutputIter;
    typedef std::vector< std::uint8_t >::iterator InputIter;
    void expand( std::vector< std::uint8_t >& output );
    std::uint16_t getCode( InputIter& index );
    DecodeIter decodeString( std::vector< std::uint16_t >& prefix,
        std::vector< std::uint8_t >& append, DecodeIter buffer, std::uint16_t code );
#endif //CHECKCOMP
};

#endif //BITMAPBLOCK_INCLUDED
