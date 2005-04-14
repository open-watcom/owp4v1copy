// search all public symbols

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pcobj.h"
#include "misc.h"

typedef unsigned char byte;
typedef byte *data_ptr;

static data_ptr    NamePtr;
static byte        NameLen;
static unsigned_16 RecLen;
static data_ptr    RecBuff;
static data_ptr    RecPtr;
static unsigned_16 RecMaxLen;
static int         isMS386;

static void usage( void )
/***********************/
{
    printf( "Usage: objxdef <list of object or library files>\n" );
    exit( -1 );
}

static int EndRec( void )
/***********************/
{
    return( RecPtr >= (RecBuff + RecLen) );
}

static byte GetByte( void )
/*************************/
{
    byte        ret;

    ret = *RecPtr;
    RecPtr++;
    return( ret );
}

static unsigned_16 GetUInt( void )
/********************************/
{
    unsigned_16 word;

    word = *(unsigned_16 *)RecPtr;
    CONV_LE_16( word );
    RecPtr += 2;
    return( word );
}

static unsigned_32 GetOffset( void )
/**********************************/
{
    if( isMS386 ) {
        unsigned_32 dword;

        dword = *(unsigned_32 *)RecPtr;
        CONV_LE_32( dword );
        RecPtr += 4;
        return( dword );
    } else {
        unsigned_16 word;

        word = *(unsigned_16 *)RecPtr;
        CONV_LE_16( word );
        RecPtr += 2;
        return( word );
    }
}

static unsigned_16 GetIndex( void )
/*********************************/
{
    unsigned_16 index;

    index = GetByte();
    if( index & 0x80 ) {
      index = ( (index & 0x7f) << 8 ) + GetByte();
    }
    return( index );
}

static char *GetName( void )
/**************************/
{
    NameLen = GetByte();
    NamePtr = RecPtr;
    RecPtr += NameLen;
    return( NamePtr );
}

static void ResizeBuff( unsigned_16 reqd_len )
/********************************************/
{
    if( reqd_len > RecMaxLen ) {
        RecMaxLen = reqd_len;
        if( RecBuff != NULL ) {
            free( RecBuff );
        }
        RecBuff = malloc( RecMaxLen );
        if( RecBuff == NULL ) {
            printf( "**FATAL** Out of memory!\n" );
            exit( -1 );
        }
    }
}

static void ProcFile( FILE *fp )
/******************************/
{
    byte        hdr[ 3 ];
    unsigned_16 page_len;
    unsigned_32 offset;

    page_len = 0;
    RecBuff = NULL;
    RecMaxLen = 0;
    for(;;) {
        offset = ftell( fp );
        if( fread( hdr, 1, 3, fp ) != 3 )
            break;
        RecLen = hdr[ 1 ] | ( hdr[ 2 ] << 8 );
        ResizeBuff( RecLen );
        RecPtr = RecBuff;
        if( fread( RecBuff, RecLen, 1, fp ) == 0 )
            break;
        RecLen--;
        isMS386 = hdr[ 0 ] & 1;
        switch( hdr[ 0 ] & ~1 ) {
        case CMD_MODEND:
            if( page_len != 0 ) {
                offset = ftell( fp );
                offset = page_len - offset % page_len;
                if( offset != page_len ) {
                    fseek( fp, offset, SEEK_CUR );
                }
            }
            break;
        case CMD_PUBDEF:
            if( ( GetIndex() | GetIndex() ) == 0 )
                GetUInt();
            while( ! EndRec() ) {
                GetName();
                *RecPtr = 0;
                printf( "%s\n", NamePtr );
                GetOffset();
                GetIndex();
            }
            break;
        case LIB_HEADER_REC:
            if( isMS386 ) {
                fseek( fp, 0L, SEEK_END );
                page_len = 0;
            } else {
                page_len = RecLen + 4;
            }
            break;
        default:
            break;
        }
    }
    free( RecBuff );
}

int process_file_pubdef( char *filename )
/***************************************/
{
    FILE    *fp;

    fp = fopen( filename, "rb" );
    if( fp == NULL ) {
        printf( "Cannot open input file: %s.\n", filename );
        return( 0 );
    }
    ProcFile( fp );
    fclose( fp );
    return( 1 );
}

int main( int argc, char *argv[] )
/********************************/
{
    int     i;
    char    *fn;

    if( argc == 1 )
        usage();
    for( i = 1; i < argc; ++i ) {
        fn = DoWildCard( argv[i] );
        while( fn != NULL ) {
            process_file_pubdef( fn );
            fn = DoWildCard( NULL );
        }
        DoWildCardClose();
    }
    return( 0 );
}
