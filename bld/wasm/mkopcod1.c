#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watcom.h"

#include "mkopcode.h"

int main( int argc, char *argv[] )
{
    FILE            *in;
    FILE            *out;
    char            *out_name;
    unsigned int    i;
    unsigned int    index;
    unsigned int    count;
    unsigned int    idx;
    sword           *Words;
    char            buf[ KEY_MAX_LEN ];

    out_name = argv[ argc - 1 ];
    --argc;

    // Count the words in all the input files
    count = 0;
    for( idx = 1; idx < argc; ++idx ) {
        in = fopen( argv[ idx ], "r" );
        if( in == NULL ) {
            printf( "Unable to open '%s'\n", argv[ idx ] );
            exit( 1 );
        }
        for( ; fgets( buf, KEY_MAX_LEN, in ) != NULL; ) {
            count++;
        }
        fclose( in );
    }
    Words = malloc( (count+1) * sizeof( sword ) );
    if( Words == NULL ) {
        printf( "Unable to allocate Words array\n" );
        exit( 1 );
    }
    Words[ count ].word = NULL;
    index = 0;
    for( idx = 1; idx < argc; ++idx ) {
        in = fopen( argv[ idx ], "r" );
        if( in == NULL ) {
            printf( "Unable to open '%s'\n", argv[ idx ] );
            exit( 1 );
        }
        for( ; fgets( buf, KEY_MAX_LEN, in ) != NULL; ) {
            for( i = 0; buf[ i ] && !isspace( buf[ i ] ); i++ )
                ;
            buf[ i ] = '\0';
            Words[ index ].word = strdup( buf );
            if( Words[ index ].word == NULL ) {
                printf( "Out of memory\n" );
                exit( 1 );
            }
            ++index;
        }
        fclose( in );
    }
    qsort( Words, count, sizeof( sword ), str_compare );

    out = fopen( out_name, "w" );
    if( out == NULL ) {
        printf( "Unable to open '%s'\n", out_name );
        exit( 1 );
    }
    fprintf( out, "enum asm_token {\n" );
    for( i = 0; i < count; i++ ) {
        fprintf( out, "\t%s,\n", get_enum_key( Words[ i ].word ) );
    }
    fprintf( out, "\tT_NULL\n" );
    fprintf( out, "};\n\n" );
    fclose( out );
    return( 0 );
}
