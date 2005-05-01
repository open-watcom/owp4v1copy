#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watcom.h"

char Chars[32000];
struct words {
        char    *word;
        int     index;
};

int len_compare( const void *pv1, const void *pv2 )
{
    int         len1;
    int         len2;
    const struct words *p1 = pv1;
    const struct words *p2 = pv2;

    len1 = strlen( p1->word );
    len2 = strlen( p2->word );
    if( len1 < len2 )  return( 1 );
    if( len1 > len2 )  return( -1 );
    return( strcmp( p1->word, p2->word ) );
}

int str_compare( const void *p1, const void *p2 )
{
    return( strcmp( ((const struct words *)p1)->word,
                    ((const struct words *)p2)->word ) );
}

int main( int argc, char *argv[] )
{
    FILE        *in;
    FILE        *out;
    char        *out_name;
    unsigned int i;
    unsigned int index;
    unsigned int j;
    unsigned int count;
    unsigned int len;
    unsigned int idx;
    struct words *Words;
    char        *word;
    char        buf[80];
    char        *prefix;
    char        *suffix;
    char        *ptr;

    out_name = argv[argc-1];
    --argc;

    // Count the words in all the input files
    count = 0;
    for( idx = 1; idx < argc; ++idx ) {
        in = fopen( argv[idx], "r" );
        if( in == NULL ) {
            printf( "Unable to open '%s'\n", argv[idx] );
            exit( 1 );
        }
        for(;;) {
            if( fgets( buf, 80, in ) == NULL ) break;
            count++;
        }
        fclose( in );
    }
    Words = malloc( (count+1) * sizeof( struct words ) );
    if( Words == NULL ) {
        printf( "Unable to allocate Words array\n" );
        exit( 1 );
    }
    Words[count].word = NULL;
    index = 0;
    for( idx = 1; idx < argc; ++idx ) {
        in = fopen( argv[idx], "r" );
        if( in == NULL ) {
            printf( "Unable to open '%s'\n", argv[idx] );
            exit( 1 );
        }
        for( ;; ) {
            if( fgets( buf, 80, in ) == NULL ) break;
            for( i = 0; buf[i] && !isspace( buf[i] ); i++ ) {
            }
            buf[i] = '\0';
            Words[index].word = strdup( buf );
            if( Words[index].word == NULL ) {
                printf( "Out of memory\n" );
                exit( 1 );
            }
            ++index;
        }
        fclose( in );
    }
    qsort( Words, count, sizeof( struct words ), len_compare );
    index = 0;
    Chars[0] = '\0';
    for( i = 0; i < count; i++ ) {
        word = strstr( Chars, Words[i].word );
        if( word == NULL ) {
            word = &Chars[index];
            len = strlen( Words[i].word ) - 1;
            if( index < len )  len = index;
            for(;;) {
                if( len == 0 ) break;
                if( memcmp( word - len, Words[i].word, len ) == 0 ) {
                    word -= len;
                    index -= len;
                    break;
                }
                len--;
            }
            strcpy( word, Words[i].word );
            index += strlen( word );
        }
        Words[i].index = word - Chars;
    }
    qsort( Words, count, sizeof( struct words ), str_compare );

    out = fopen( out_name, "w" );
    if( out == NULL ) {
        printf( "Unable to open '%s'\n", out_name );
        exit( 1 );
    }
    fprintf( out, "\n#ifndef DEFINE_ASMOPS\n" );
    fprintf( out, "  #undef asm_op\n" );
    fprintf( out, "  #define asm_op(token,len,index) token\n" );
    fprintf( out, "  enum asm_token {\n" );
    fprintf( out, "#else\n" );
    fprintf( out, "  extern char AsmChars[] = {\n" );
    for( i = 0; i < index; i++ ) {
        if( i % 10 == 0 )  fprintf( out, "/*%4d*/ ", i );
        fprintf( out, "'%c',", Chars[i] );
        if( i % 10 == 9 )  fprintf( out, "\n" );
    }
    fprintf( out, "'\\0'\n};\n\n" );
    fprintf( out, "  #undef asm_op\n" );
    fprintf( out, "  #define asm_op(token,len,index) {0,len,index}\n" );
    fprintf( out, "  extern struct AsmCodeName AsmOpcode[] = {\n" );
    fprintf( out, "#endif\n\n" );
    for( i = 0; i < count; i++ ) {
        strcpy( buf, Words[i].word );
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
        fprintf( out, "asm_op( %s%s%s,\t", prefix, ptr, suffix );
        j = 9 + strlen( prefix ) + strlen( ptr ) + strlen( suffix );
        while ( j < 24) {
            fprintf( out, "\t" );
            j += 8;
        }
        j = strlen(Words[i].word);
        fprintf( out, "%d,\t%d\t),\n", j, Words[i].index );
    }
    fprintf( out, "asm_op( T_NULL,\t\t\t0,\t0\t)\n" );
    fprintf( out, "};\n" );
    fclose( out );
    return( 0 );
}
