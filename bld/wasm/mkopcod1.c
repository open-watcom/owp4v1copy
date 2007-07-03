#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watcom.h"

char Chars[ 32000 ];

struct words {
    char    *word;
    int     index;
};

int str_compare( const void *p1, const void *p2 )
{
    return( strcmp( ((const struct words *)p1)->word,
                    ((const struct words *)p2)->word ) );
}

int main( int argc, char *argv[] )
{
    FILE            *in;
    FILE            *out;
    char            *out_name;
    unsigned int    i;
    unsigned int    index;
    unsigned int    j;
    unsigned int    count;
    unsigned int    idx;
    struct words    *Words;
    char            buf[ 80 ];
    char            *prefix;
    char            *suffix;
    char            *ptr;

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
        for( ; ; ) {
            if( fgets( buf, 80, in ) == NULL )
                break;
            count++;
        }
        fclose( in );
    }
    Words = malloc( (count+1) * sizeof( struct words ) );
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
        for( ; ; ) {
            if( fgets( buf, 80, in ) == NULL )
                break;
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
    qsort( Words, count, sizeof( struct words ), str_compare );

    out = fopen( out_name, "w" );
    if( out == NULL ) {
        printf( "Unable to open '%s'\n", out_name );
        exit( 1 );
    }
    fprintf( out, "  enum mkopcode {\n" );
    for( i = 0; i < count; i++ ) {
        strcpy( buf, Words[ i ].word );
        strupr( buf );
        ptr = buf;
        if( *ptr == '.' ) {
            ptr++;
            prefix = "T_DOT_";
        } else {
            prefix = "T_";
        }
        j = strlen(buf);
        if( *(buf + j - 1) == '?' ) {
        // append suffix _UN if ? on end
            suffix = "_UN";
            *(buf + j - 1) = 0;
        } else {
            suffix = "";
        }
        fprintf( out, "\t%s%s%s,\n", prefix, ptr, suffix );
    }
    fprintf( out, "};\n\n" );
    fclose( out );
    return( 0 );
}
