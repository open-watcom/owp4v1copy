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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


//
//  FCBUILD     : process the fcinfo.dat file to create various tables
//

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE        512

#define HOW_TO_GENERATE \
    "this file generated automatically by " __FILE__

FILE    *inFile;
char    lineBuf[ MAX_LINE ];
char    *inputPtr;
char    *endPtr;
char    saveBuf[ MAX_LINE ];
FILE    *outFile;


enum {
    KEY_EOF,
    KEY_FTNRT,
    KEY_GROUP,
    KEY_DEFINE,
    KEY_FCODE
};


char *getWord( void ) {
//=====================

    char *word;

    while( isspace( *inputPtr ) ) ++inputPtr;
    word = inputPtr;
    while( !isspace( *inputPtr ) && *inputPtr ) ++inputPtr;
    *inputPtr = 0;
    if( inputPtr != endPtr ) ++inputPtr;
    return( word );
}


int getLine( void ) {
//===================

    char        *p;
    char        *tok;

    for(;;) {
        if( fgets( lineBuf, MAX_LINE - 1, inFile ) == NULL ) break;
        p = strchr( lineBuf, '\n' );
        if( p != NULL ) {
            *p = 0;
        }
        p = strchr( lineBuf, '#' );     /* strip comments */
        if( p != NULL ) {
            *p = 0;
        }
        inputPtr = lineBuf;
        endPtr = memchr( lineBuf, 0, ~0 );
        tok = getWord();
        if( tok != endPtr ) {           /* if not an empty line */
            if( strcmp( tok, "ftnrt" ) == 0 ) {
                return( KEY_FTNRT );
            } else if( strcmp( tok, "group" ) == 0 ) {
                return( KEY_GROUP );
            } else if( strcmp( tok, "define" ) == 0 ) {
                return( KEY_DEFINE );
            } else if( strcmp( tok, "fcode" ) == 0 ) {
                return( KEY_FCODE );
            } else {
                fprintf( stderr, "Do not understand command: %s\n", tok );
                exit( EXIT_FAILURE );
            }
        }
    }
    return( KEY_EOF );
}


char *recWord( void ) {
//=====================

    char *tok;

    tok = getWord();
    if( tok == endPtr ) {
        fprintf( stderr, "Expecting a word after %s\n", lineBuf );
        exit( EXIT_FAILURE );
    }
    return( tok );
}


void buildEnum( void ) {
//======================

    int         first_of_group;
    char        *tok;

    fprintf( outFile,
        "/* " HOW_TO_GENERATE " */\n"
        "#define RT_FIRST_FORTRAN 0\n"
        "#include <ftnrt.h>\n"
        "enum {\n"
        "END_OF_PROGRAM = RT_LAST_FORTRAN + 1,\n"
    );
    first_of_group = 0;
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            fprintf( outFile, "LAST_FCODE\n};\n" );
            return;
        case KEY_FTNRT:
            break;
        case KEY_GROUP:
            tok = recWord();
            fprintf( outFile, "%s,\n", tok );
            strcpy( saveBuf, tok );
            first_of_group = 1;
            break;
        case KEY_DEFINE:
            fprintf( outFile, "#define %s\n", inputPtr );
            break;
        case KEY_FCODE:
            tok = recWord();
            if( first_of_group ) {
                fprintf( outFile, "%s = %s,\n", tok, saveBuf );
                first_of_group = 0;
            } else {
                fprintf( outFile, "%s,\n", tok );
            }
            break;
        }
    }
}


void buildAsmEnum( void ) {
//=========================

    int         first_of_group;
    char        *tok;
    int         value;

    fprintf( outFile, "; " HOW_TO_GENERATE "\n" );
    value = 0;
    first_of_group = 0;
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            return;
        case KEY_FTNRT:
            ++value;
            break;
        case KEY_GROUP:
            tok = recWord();
            fprintf( outFile, "FCODE_%s equ %d\n", tok, value );
            ++value;
            strcpy( saveBuf, tok );
            first_of_group = 1;
            break;
        case KEY_DEFINE:
            break;
        case KEY_FCODE:
            tok = recWord();
            if( first_of_group ) {
                fprintf( outFile, "FCODE_%s equ FCODE_%s\n", tok, saveBuf );
                first_of_group = 0;
            } else {
                fprintf( outFile, "FCODE_%s equ %d\n", tok, value );
                ++value;
            }
            break;
        }
    }
}


void buildExtern( void ) {
//========================

    char        *tok;

    fprintf( outFile,
        "/* " HOW_TO_GENERATE " */\n"
        "#include <ftnstd.h>\n"
        "#include <fcenv.h>\n"
    );
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            return;
        case KEY_FTNRT:
            tok = recWord();
            fprintf( outFile, "#pragma aux (f_code) RT_%s;\n", tok );
            fprintf( outFile, "extern void RT_%s(void);\n", tok );
            break;
        case KEY_FCODE:
            tok = recWord();
            fprintf( outFile, "#pragma aux (f_code) %s;\n", tok );
            fprintf( outFile, "extern void %s(void);\n", tok );
            break;
        }
    }
}


void buildTable( void ) {
//=======================

    int         add_a_comma;
    char        *tok;

    fprintf( outFile,
        "#pragma aux FC_TABLE \"LG@*\";\n"
        "__CONST void (__NEAR * __CONST __FAR FC_TABLE[])(void) = {\n"
    );
    add_a_comma = 0;
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            if( add_a_comma ) fprintf( outFile, "\n" );
            fprintf( outFile, "};\n" );
            return;
        case KEY_FTNRT:
            if( add_a_comma ) fprintf( outFile, ",\n" );
            tok = recWord();
            fprintf( outFile, "&RT_%s", tok );
            add_a_comma = 1;
            break;
        case KEY_FCODE:
            if( add_a_comma ) fprintf( outFile, ",\n" );
            tok = recWord();
            fprintf( outFile, "&%s", tok );
            add_a_comma = 1;
            break;
        }
    }
}


void buildNoVector( void ) {
//==========================

    char        *tok;

    fprintf( outFile, "# " HOW_TO_GENERATE "\n" );
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            return;
        case KEY_FTNRT:
            tok = recWord();
            fprintf( outFile, "novector 'FC@RT_%s'\n", tok );
            break;
        case KEY_FCODE:
            tok = recWord();
            fprintf( outFile, "novector 'FC@%s'\n", tok );
            break;
        }
    }
}


void buildNames( void ) {
//========================

    int         add_a_comma;
    char        *tok;

    fprintf( outFile, "/* " HOW_TO_GENERATE " */\n" );
    add_a_comma = 0;
    for(;;) {
        switch( getLine() ) {
        case KEY_EOF:
            return;
        case KEY_FTNRT:
            if( add_a_comma ) fprintf( outFile, ",\n" );
            tok = recWord();
            fprintf( outFile, "\"RT_%s\"", tok );
            add_a_comma = 1;
            break;
        case KEY_FCODE:
            if( add_a_comma ) fprintf( outFile, ",\n" );
            tok = recWord();
            fprintf( outFile, "\"%s\"", tok );
            add_a_comma = 1;
            break;
        }
    }
}


#pragma aux usage aborts;
void usage( void ) {
//==================
    printf(
"usage: fcbuild fcinfo.dat output_file w\n"
"where w is one of:\n"
"e   build fcodes.h\n"
"i   build fcodes.inc\n"
"t   build fctab.c\n"
"v   build novector.lnk\n"
"n   build fcnames.inc\n"
    );
    exit( EXIT_FAILURE );
}


void getInFile( char *fname ) {
//=============================

    inFile = fopen( fname, "r" );
    if( inFile == NULL ) {
        fprintf( stderr, "Unable to open %s for read: %s\n", fname,
            strerror( errno ) );
        exit( EXIT_FAILURE );
    }
}


void getOutFile( char *fname ) {
//==============================

    outFile = fopen( fname, "w" );
    if( outFile == NULL ) {
        fprintf( stderr, "Unable to open %s for write: %s\n", fname,
            strerror( errno ) );
        exit( EXIT_FAILURE );
    }
}


void main( int argc, char **argv ) {
//==================================

    if( argc != 4 ) usage();
    getInFile( argv[ 1 ] );
    getOutFile( argv[ 2 ] );
    switch( argv[3][0] ) {
    case 'e':
        buildEnum();
        break;
    case 'i':
        buildAsmEnum();
        break;
    case 't':
        buildExtern();
        fclose( inFile );
        getInFile( argv[ 1 ] );
        buildTable();
        break;
    case 'v':
        buildNoVector();
        break;
    case 'n':
        buildNames();
        break;
    default:
        usage();
    }
    fclose( inFile );
    exit( EXIT_SUCCESS );
}
