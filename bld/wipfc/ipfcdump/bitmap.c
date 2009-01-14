// Dump bitmap information

#include <ctype.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "ipfcdump.h"

#define FILEBITMAP
#ifdef FILEBITMAP
static size_t   expand( uint8_t *, uint8_t *, uint8_t *, size_t);
static uint16_t getCode( uint8_t *, size_t *, size_t);
static uint8_t *decodeString( uint8_t *, uint16_t, char *);
static void     blockToHex( uint8_t *, size_t, FILE *);
#endif

void readBitMaps( FILE *in, FILE *out )
{
    fputs( "\nBitmap data\n", out );
    if( Hdr.imageOffset ) {
        BitMap bm;
        BitMapBlock bmb;
        RGB rgb[ 256] ;
        uint32_t size;
        uint32_t nextMap = Hdr.imageOffset;
        unsigned int count1 = 0;
        uint16_t blockSize;
#ifdef FILEBITMAP
        uint8_t *compressed = ( uint8_t * )malloc( 65504 * sizeof( uint8_t ) );
        uint8_t *expanded = ( uint8_t * )malloc( 65504 * sizeof( uint8_t ) );
        FILE *btmp;
        FILE *lzw;
        int32_t bytesLeft;
        char name[ 13 ];
#endif
        fseek( in, Hdr.imageOffset, SEEK_SET );
        fread( &bm, sizeof( BitMap ), 1, in );
        while( ( bm.hdr.type[0] == 'b' && bm.hdr.type[1] == 'M' ) ||
               ( bm.hdr.type[0] == 'm' && bm.hdr.type[1] == 'F' ) ) {
            uint32_t totalSize = 0;
            unsigned int count2 = 0;
            fprintf( out, "  Bitmap #%u at offset %8.8x\n", count1, nextMap );
            fprintf( out, "    Type:                 %c%c\n", bm.hdr.type[0], bm.hdr.type[1] );
            fprintf( out, "    Size:           %8.8x (%lu)\n", bm.hdr.size, bm.hdr.size );
            fprintf( out, "    xHotspot:       %8.4x (%hu)\n", bm.hdr.xHotspot, bm.hdr.xHotspot );
            fprintf( out, "    yHotspot:       %8.4x (%hu)\n", bm.hdr.yHotspot, bm.hdr.yHotspot );
            fprintf( out, "    Bits offset:    %8.8x (%lu)\n", bm.hdr.bitsOffset, bm.hdr.bitsOffset );
            fprintf( out, "    Size:           %8.8x (%lu)\n", bm.hdr.info.size, bm.hdr.info.size );
            fprintf( out, "    Width:          %8.4x (%hu)\n", bm.hdr.info.width, bm.hdr.info.width );
            fprintf( out, "    Height:         %8.4x (%hu)\n", bm.hdr.info.height, bm.hdr.info.height );
            fprintf( out, "    Planes:         %8.4x (%hu)\n", bm.hdr.info.planes, bm.hdr.info.planes );
            fprintf( out, "    Bits per Pixel: %8.4x (%hu)\n", bm.hdr.info.bitsPerPixel, bm.hdr.info.bitsPerPixel );
#ifdef FILEBITMAP
            snprintf( name, sizeof(name) / sizeof(char), "btmp%4.4u.bmp", count1 );
            btmp = fopen( name, "wb" );
            snprintf( name, sizeof(name) / sizeof(char), "btmp%4.4u.lzw", count1 );
            lzw = fopen( name, "w" );
            if( bm.hdr.size )
                bytesLeft = bm.hdr.size - bm.hdr.bitsOffset /* sizeof(BitMap) */;
            else {
                bm.hdr.size = sizeof( BitMap );
                if (bm.hdr.info.bitsPerPixel == 1 ) {
                    bytesLeft = bm.hdr.info.width / 8;
                    bytesLeft = bytesLeft & 3 ? ( bytesLeft & ~3 ) + 4 : bytesLeft;
                    bytesLeft *= bm.hdr.info.height;
                }
                else if( bm.hdr.info.bitsPerPixel == 4 ) {
                    bytesLeft = bm.hdr.info.width / 2;
                    bytesLeft = bytesLeft & 3 ? ( bytesLeft & ~3 ) + 4 : bytesLeft;
                    bytesLeft *= bm.hdr.info.height;
                    bm.hdr.size += bytesLeft + 16 * sizeof( RGB );
                }
                else if( bm.hdr.info.bitsPerPixel == 8 ) {
                    bytesLeft = bm.hdr.info.width & 3 ? ( bm.hdr.info.width & ~3 ) + 4 : bm.hdr.info.width;
                    bytesLeft *= bm.hdr.info.height;
                    bm.hdr.size += bytesLeft + 256 * sizeof( RGB );
                }
                else if( bm.hdr.info.bitsPerPixel == 15 || bm.hdr.info.bitsPerPixel == 16 ) {
                    bytesLeft = bm.hdr.info.width * 2;
                    bytesLeft = bytesLeft & 3 ? ( bytesLeft & ~3 ) + 4 : bytesLeft;
                    bytesLeft *= bm.hdr.info.height;
                    bm.hdr.size += bytesLeft;
                }
                else if( bm.hdr.info.bitsPerPixel == 24 ) {
                    bytesLeft = bm.hdr.info.width * 3;
                    bytesLeft = bytesLeft & 3 ? ( bytesLeft & ~3 ) + 4 : bytesLeft;
                    bytesLeft *= bm.hdr.info.height;
                    bm.hdr.size += bytesLeft;
                }
                else if( bm.hdr.info.bitsPerPixel == 32 ) {
                    bytesLeft = bm.hdr.info.width * 4 * bm.hdr.info.height;
                    bm.hdr.size += bytesLeft;
                }
            }
            bm.hdr.type[ 0 ] = toupper( bm.hdr.type[ 0 ] );
            bm.hdr.bitsOffset = sizeof( BitMap );
            if( bm.hdr.info.bitsPerPixel == 4 )
                bm.hdr.bitsOffset += 16 * sizeof( RGB );
            else if( bm.hdr.info.bitsPerPixel == 8 )
                bm.hdr.bitsOffset += 256 * sizeof( RGB );
            bm.hdr.size = bytesLeft + bm.hdr.bitsOffset;
            fwrite( &bm, sizeof( BitMap ), 1, btmp );
            fprintf( lzw, "Bitmap #%u at offset %8.8x\n", count1, nextMap );
            fprintf( lzw, "  Type:                 %c%c\n", bm.hdr.type[0], bm.hdr.type[1] );
            fprintf( lzw, "  Size:           %8.8x (%lu)\n", bm.hdr.size, bm.hdr.size );
            fprintf( lzw, "  xHotspot:       %8.4x (%hu)\n", bm.hdr.xHotspot, bm.hdr.xHotspot );
            fprintf( lzw, "  yHotspot:       %8.4x (%hu)\n", bm.hdr.yHotspot, bm.hdr.yHotspot );
            fprintf( lzw, "  Bits offset:    %8.8x (%lu)\n", bm.hdr.bitsOffset, bm.hdr.bitsOffset );
            fprintf( lzw, "  Size:           %8.8x (%lu)\n", bm.hdr.info.size, bm.hdr.info.size );
            fprintf( lzw, "  Width:          %8.4x (%hu)\n", bm.hdr.info.width, bm.hdr.info.width );
            fprintf( lzw, "  Height:         %8.4x (%hu)\n", bm.hdr.info.height, bm.hdr.info.height );
            fprintf( lzw, "  Planes:         %8.4x (%hu)\n", bm.hdr.info.planes, bm.hdr.info.planes );
            fprintf( lzw, "  Bits per Pixel: %8.4x (%hu)\n", bm.hdr.info.bitsPerPixel, bm.hdr.info.bitsPerPixel );
#endif
            memset( &rgb, 0, 256 * sizeof( RGB ) );
            if( bm.hdr.info.bitsPerPixel == 4 ) {
#ifdef COLOR_PAL
                unsigned int count;
                fread( rgb, sizeof( RGB ), 16, in );
                fputs( "    Color Palette\n", out );
                for( count = 0; count < 16; count++ )
                    fprintf( out, "      %2d [%2.2x %2.2x %2.2x]\n", count, rgb[count].blue, rgb[count].green, rgb[count].red );
#else
                fread( rgb, sizeof( RGB ), 16, in );
                fputs( "    Skipping color palette...\n", out );
#endif
#ifdef FILEBITMAP
                fwrite( rgb, sizeof(RGB), 16, btmp );
                fputs( "Color Palette\n", lzw );
                blockToHex( ( unsigned char * )&rgb[ 0 ], 16 * sizeof( RGB ), lzw );
#endif
            }
            else if( bm.hdr.info.bitsPerPixel == 8 ) {
#ifdef COLOR_PAL
                unsigned int count;
                fread( rgb, sizeof( RGB ), 256, in );
                fputs( "    Color Palette\n", out );
                for  (count = 0; count < 256; count++ )
                    fprintf(out, "      %3d [%2.2x %2.2x %2.2x]\n", count, rgb[count].blue, rgb[count].green, rgb[count].red );
#else
                fread( rgb, sizeof( RGB ), 256, in );
                fputs( "    Skipping color palette...\n", out );
#endif
#ifdef FILEBITMAP
                fwrite(rgb, sizeof(RGB), 256, btmp);
                fputs("Color Palette\n", lzw);
                blockToHex((unsigned char*)&rgb[0], 256 * sizeof(RGB), lzw);
#endif
            }
            else
                fputs("    No color palette for this bit depth\n", out);
            fread( &size, sizeof( uint32_t ), 1, in );
            nextMap = ftell( in ) + size;
            fread( &blockSize, sizeof( uint16_t ), 1, in );
            fprintf( out, "    Data size:      %8.8x (%lu)\n", size, size );
            fprintf( out, "    Block size:     %8.4x (%hu)\n", blockSize, blockSize );
            fread( &bmb, sizeof( BitMapBlock ), 1, in );
#ifdef FILEBITMAP
            fprintf( lzw, "  Data size:      %8.8x (%lu)\n", size, size );
            fprintf( lzw, "  Block size:     %8.4x (%hu)\n", blockSize, blockSize );
#endif
            totalSize += bmb.size;
            while( totalSize < size ) {
                fprintf(out, "    Bitmap #%u Block #%u\n", count1, count2);
                fprintf(out, "      Size:             %4.4x (%hu)\n", bmb.size, bmb.size);
                fprintf(out, "      Compression type: %4.2x (%u)\n", bmb.type, bmb.type);
#ifdef FILEBITMAP
                fprintf(lzw, "  Block #%u\n", count2);
                fprintf(lzw, "    Size:             %4.4x (%hu)\n", bmb.size, bmb.size);
                fprintf(lzw, "    Compression type: %4.2x (%u)\n", bmb.type, bmb.type);
                fread( compressed, sizeof( uint8_t ), bmb.size - 1, in );
                blockToHex( compressed, sizeof( uint8_t ) * ( bmb.size - 1 ), lzw );
                if( bmb.type ) {
                    uint8_t filler;
                    size_t bytes = expand( compressed, expanded, &filler, bmb.size - 1 );
                    if( bytes < blockSize ) {
                        memset(expanded + bytes, filler, blockSize - bytes);
                        //fprintf(out, "Adding %u bytes of filler %2.2x\n", blockSize - bytes, filler);
                    }
                    fwrite( expanded, sizeof( uint8_t ), min( bytesLeft, blockSize ), btmp );
                    bytesLeft -= min( bytesLeft, blockSize );
                }
                else
                    fwrite( compressed, sizeof( uint8_t ), bmb.size - 1, btmp );
#else
                fseek( in, bmb.size - 1, SEEK_CUR );
#endif
                fread( &bmb, sizeof( BitMapBlock ), 1, in );
                totalSize += bmb.size;
                ++count2;
            }
#ifdef FILEBITMAP
            fclose(btmp);
            fclose(lzw);
#endif
            ++count1;
            fseek(in, nextMap, SEEK_SET);
            fread(&bm, sizeof(BitMap), 1, in);
        }
#ifdef FILEBITMAP
        free(compressed);
        free(expanded);
#endif
    }
    else
        fputs("  There are no bitmaps\n", out);
}
/*******************************************************************************************/
/* Based on code in 2 articls from DDJ:
    1. Mark R. Nelson, October 1989
    2. Shawn M. Regan, January 1990
*/
#ifdef FILEBITMAP

#define CLEAR_TABLE 256                 /* flush the string table */
#define TERMINATOR  257                 /* marks EOF */
#define FIRST_CODE  258                 /* First available code for code_value table */
#define INIT_BITS 9
#define MAX_BITS 12
#define TABLE_SIZE 5021
#define MAXVAL(n) ((1 << ( n )) - 1)    /* max_value formula macro */

static uint16_t    *PrefixCode;         /* This array holds the prefix codes */
static uint8_t     *AppendCharacter;    /* This array holds the appended chars */
static uint8_t     *DecodeStack;        /* This array holds the decoded string */
static uint32_t     BitBuffer = 0L;
static size_t       BitCount = 0;
static uint16_t     NumBits = INIT_BITS;/* Starting with 9 bit codes */

static size_t expand( uint8_t *src, uint8_t *dst, uint8_t *lastCode, size_t size )
{
    uint8_t    *string;
    size_t      bytes = 0;
    size_t      index = 0;
    uint16_t    nextCode = FIRST_CODE;
    uint16_t    newCode;
    uint16_t    oldCode;
    uint16_t    maxCode = MAXVAL(INIT_BITS);
    uint16_t    character;
    char        clearF = 1;
    char        errorF = 0;
    
    PrefixCode = ( uint16_t * )calloc( TABLE_SIZE, sizeof( uint16_t ) );
    AppendCharacter = ( uint8_t * )calloc( TABLE_SIZE, sizeof( uint8_t ) );
    DecodeStack =  ( uint8_t * )calloc( TABLE_SIZE, sizeof( uint8_t ) );
    BitBuffer = 0L;
    BitCount = 0;
    NumBits = INIT_BITS;
    while( ( newCode = getCode( src, &index, size ) ) != TERMINATOR ) {
        if (clearF) {                /* (re-)initialize */
            clearF = 0;
            oldCode = newCode;
            character = newCode;
            dst[ bytes ] = ( uint8_t )newCode;
            ++bytes;
            *lastCode = ( uint8_t )newCode;
            continue;
        }
        if (newCode == CLEAR_TABLE) {   /* Clear string table */
            clearF = 1;
            NumBits = INIT_BITS;
            nextCode = FIRST_CODE;
            maxCode = MAXVAL(INIT_BITS);
            continue;
        }
        if (newCode >= nextCode) {      /* Check for string+char+string */
            DecodeStack[0] = character;
            string = decodeString(&DecodeStack[1], oldCode, &errorF);
        }
        else
            string = decodeString(&DecodeStack[0], newCode, &errorF);
        if (errorF) {
            printf("DecodeStack limit exceeded\n\a");
            break;
        }
        character = *string;            /* Output decoded string in reverse */
        while( string >= &DecodeStack[0] ){
            dst[bytes] = *string;
            ++bytes;
            --string;
            if (bytes >= 65504) {
                errorF = 1;
                break;
            }
        }
        if (errorF) {
            printf("destination limit exceeded\n\a");
            break;
        }
        *lastCode = *( ++string );
        if( nextCode <= maxCode ) {     /* Add to string table if not full */
            PrefixCode[ nextCode ] = oldCode;
            AppendCharacter[ nextCode ] = character;
            ++nextCode;
            if( nextCode == maxCode && NumBits < MAX_BITS )
                maxCode = MAXVAL( ++NumBits );
        }
        oldCode = newCode;
    }
    free( PrefixCode );
    PrefixCode = NULL;
    free( AppendCharacter );
    AppendCharacter = NULL;
    free( DecodeStack );
    DecodeStack = NULL;
    return bytes;
}
/*******************************************************************************************/
static uint16_t getCode( uint8_t *src, size_t *index, size_t size )
{
    uint16_t retval;
    
    while( BitCount <= 24 ) {
        if (*index < size) {
            BitBuffer |= ( ( uint32_t )src[ *index ] << ( 24 - BitCount ) );
            *index += 1;
        }
        else
            BitBuffer |= /*0x00*/ (( uint32_t )EOF << ( 24 - BitCount ) );
        BitCount += 8;
    }
    if (*index < size)
        retval = BitBuffer >> (32 - NumBits);
    else
        retval = TERMINATOR;
    BitBuffer <<= NumBits;
    BitCount -= NumBits;
    return( retval );
}
/*******************************************************************************************/
static uint8_t *decodeString( uint8_t *buffer, uint16_t code, char *errorF )
{
    unsigned int index = 0;
    *errorF = 0;
    while( code > 255 ) {
        *buffer = AppendCharacter[ code ];
        ++buffer;
        code = PrefixCode[ code ];
        if( index >= TABLE_SIZE ) {
            *errorF = 1;
            return( buffer );
        }
        ++index;
    }
    *buffer = code;
    return( buffer );
}
/*******************************************************************************************/
static void blockToHex( uint8_t *data, size_t size, FILE* out )
{
    size_t bytes = 0;
    while( bytes < size ) {
        unsigned int count1;
        for( count1 = 0; count1 < 16; count1++ ) {
            if (bytes >= size)
                break;
            fprintf( out, "%2.2X ", data[ bytes ] );
            ++bytes;
        }
        fputc('\n', out);
    }
}

#endif
