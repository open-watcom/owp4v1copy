#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watcom.h"

#include "hash.h"
#include "mkopcode.gh"

#define ins(tok,op1,byte1_info,op2,op3,op_dir,rm_info,opcode,rm_byte,cpu,prefix) tok,

#if defined( _STANDALONE_ )
    #define insa(tok,op1,byte1_info,op2,op3,op_dir,rm_info,opcode,rm_byte,cpu,prefix) tok,
#else
    #define insa(tok,op1,byte1_info,op2,op3,op_dir,rm_info,opcode,rm_byte,cpu,prefix)
#endif

const unsigned short AsmOpTable[] = {
#include "asminsd.h"
};


char Chars[ 32000 ];

static unsigned short inst_table[ HASH_TABLE_SIZE ] = { 0 };
static unsigned short *index_table;
static unsigned short *pos_table;

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
    if( len1 < len2 )
        return( 1 );
    if( len1 > len2 )
        return( -1 );
    return( strcmp( p1->word, p2->word ) );
}

int str_compare( const void *p1, const void *p2 )
{
    return( strcmp( ((const struct words *)p1)->word,
                    ((const struct words *)p2)->word ) );
}

void make_inst_hash_tables( unsigned int count, struct words *Words )
/*******************************************************************/
{
    char            *name;
    unsigned short  i;
    unsigned short  *p;
    int             pos;
    int             size = sizeof( AsmOpTable ) / sizeof( AsmOpTable[ 0 ] );

    index_table = calloc( count, sizeof( unsigned short ) );
    pos_table = calloc( count, sizeof( unsigned short ) );
    for( pos = 0, i = 0; i < count; i++ ) {
        // create indexes for hash item lists
        name = Words[ i ].word;
        for( p = &inst_table[ hashpjw( name ) ]; *p; p = &index_table[ *p - 1 ] ) {
            if( stricmp( name, Words[ *p - 1 ].word ) == 0 ) {
                break;
            }
        }
        if( *p == 0 ) {
            index_table[ i ] = 0;
            *p = i + 1;
        }
        // create index for position in AsmOpTable
        while ( AsmOpTable[ pos ] < i && pos < size )
            pos++;
        if( AsmOpTable[ pos ] != i || pos >= size ) {
            printf( "Wrong data in asminsd.h\n" );
            exit( 1 );
        }
        pos_table[ i ] = pos;
    }
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
    unsigned int    len;
    unsigned int    idx;
    struct words    *Words;
    char            *word;
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
    qsort( Words, count, sizeof( struct words ), len_compare );
    index = 0;
    Chars[ 0 ] = '\0';
    for( i = 0; i < count; i++ ) {
        word = strstr( Chars, Words[ i ].word );
        if( word == NULL ) {
            word = &Chars[ index ];
            len = strlen( Words[ i ].word ) - 1;
            if( index < len )
                len = index;
            for( ; ; ) {
                if( len == 0 )
                    break;
                if( memcmp( word - len, Words[ i ].word, len ) == 0 ) {
                    word -= len;
                    index -= len;
                    break;
                }
                len--;
            }
            strcpy( word, Words[ i ].word );
            index += strlen( word );
        }
        Words[ i ].index = word - Chars;
    }
    qsort( Words, count, sizeof( struct words ), str_compare );

    make_inst_hash_tables( count, Words );

    out = fopen( out_name, "w" );
    if( out == NULL ) {
        printf( "Unable to open '%s'\n", out_name );
        exit( 1 );
    }
    fprintf( out, "\n#ifndef DEFINE_ASMOPS\n" );
    fprintf( out, "  #undef asm_op\n" );
    fprintf( out, "  #define asm_op(token,pos,len,index,next) token\n" );
    fprintf( out, "  enum asm_token {\n" );
    fprintf( out, "#else\n" );
    fprintf( out, "  extern const char AsmChars[] = {\n" );
    for( i = 0; i < index; i++ ) {
        if( i % 10 == 0 )
            fprintf( out, "/*%4d*/ ", i );
        fprintf( out, "'%c',", Chars[ i ] );
        if( i % 10 == 9 ) {
            fprintf( out, "\n" );
        }
    }
    fprintf( out, "'\\0'\n};\n\n" );
    fprintf( out, "static const unsigned short inst_table[ HASH_TABLE_SIZE ] = {\n" );
    for( i = 0; i < HASH_TABLE_SIZE; i++ )
        fprintf( out, "\t%d,\n", inst_table[ i ] );
    fprintf( out, "};\n\n" );
    fprintf( out, "  #undef asm_op\n" );
    fprintf( out, "  #define asm_op(token,pos,len,index,next) {pos,len,index,next}\n" );
    fprintf( out, "  extern const struct AsmCodeName AsmOpcode[] = {\n" );
    fprintf( out, "#endif\n\n" );
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
        fprintf( out, "asm_op( %s%s%s,\t", prefix, ptr, suffix );
        j = 9 + strlen( prefix ) + strlen( ptr ) + strlen( suffix );
        while ( j < 24) {
            fprintf( out, "\t" );
            j += 8;
        }
        j = strlen(Words[ i ].word);
        fprintf( out, "%d,\t%d,\t%d,\t%d\t),\n", pos_table[ i ], j, Words[ i ].index, index_table[ i ] );
    }
    fprintf( out, "asm_op( T_NULL,\t\t\t0,\t0,\t0,\t0\t)\n" );
    fprintf( out, "};\n\n" );
    fclose( out );
    return( 0 );
}
