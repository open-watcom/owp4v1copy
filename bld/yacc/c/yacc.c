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
* Description:  yacc - Yet Another Compiler Compiler - main and close dependents
*
****************************************************************************/


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h> // For isatty
#include "yacc.h"

#ifdef UNIX
#include <alloca.h>
#endif

FILE *yaccin, *actout, *tokout;
char *loadpath, *srcname;

char lineflag;
char bigflag;
char fastflag;
char denseflag;
char default_shiftflag;
char compactflag;
char eliminateunitflag;
char showflag;
char translateflag;
char defaultwarnflag = 1;

index_t RR_conflicts;
index_t SR_conflicts;
index_t nstate_1_reduce;

static FILE *opena( char const *filename, char const *access );
static void closec( char const *filename, FILE **fpp );
static void txfile( char const *filename, FILE *file, int stop );
static void setoptions( char *p );
static void checkwarn( void );

static int warnings;
static int proflag;
static FILE *stdout_copy;   // stdout may or may not be an lvalue or a constant

int main( int argc, char **argv )
{
    unsigned i;
    FILE *skeleton, *temp;
    char *skelname;

    stdout_copy = stdout;
    for( i = 1; i < argc && *argv[i] == '-'; ++i ) {
        setoptions( &argv[i][1] );
    }
    if( i != argc - 1 && i != argc - 2 ) {
        puts( "usage: yacc [-bcdflpsuw] <grammar> [<driver>]\n" );
        puts( "options:" );
        puts( "    -b    output bigger (less optimal) tables" );
        puts( "    -c    output compact tables (slower to execute)" );
        puts( "    -d    use dense tokens (no '+' style tokens allowed)" );
        puts( "    -ds   use 'default shift' optimization" );
        puts( "    -f    output fast tables (larger size)" );
        puts( "    -l    output #line directives" );
        puts( "    -p    dump all productions" );
        puts( "    -s    dump full state tables" );
        puts( "    -t    translate 'keyword' to Y_KEYWORD, '++' to Y_PLUS_PLUS, etc." );
        puts( "    -u    eliminate useless unit production reduction" );
        puts( "    -w    disable default action type checking" );
        exit( 1 );
    }
    if( i == argc - 2 ) {
        skelname = argv[argc - 1];
        skeleton = opena( skelname, "rb" );
        if( !skeleton ) {
            msg( "could not open driver source code '%s'\n", argv[ argc - 1 ] );
        }
    } else {
        skelname = "yydriver.c";
        skeleton = fpopen( loadpath, skelname );
        if( !skeleton ) {
            msg( "Can't find yacc skeleton yydriver.c\n" );
        }
    }
    loadpath = argv[0];
    *getname( loadpath ) = '\0';
    srcname = argv[i];
    if( !strrchr( srcname, '.' ) ) {
        srcname = alloca( strlen( argv[i] )+3 );
        srcname = strcat( strcpy( srcname, argv[i] ), ".y" );
    }
    yaccin = opena( srcname,  "rb" );
    tokout = opena( "ytab.h", "w" );
    defs();                                 // #define lines -> ytab.h
    if( !(temp = tmpfile()) ) {
        msg( "Can't create temporary file\n" );
    }
    actout = temp;
    rules();                                // "case n: ... break;" -> tmpfiles()
    (void)fseek( actout, 0L, SEEK_SET );    // rewind preserving error indicator
    buildpro();
    CalcMinSentence();
    if( proflag || showflag ) {
        showpro();
    }
    lr0();
    lalr1();
    SetupStateTable();
    /* apply state filters */
    FindUnused();
    if( eliminateunitflag ) {
        EliminateUnitReductions();
    }
    if( default_shiftflag ) {
        if( ! keyword_id_low ) {
            msg( "No %%keyword_id <low> <high> specified." );
        } else {
            MarkDefaultShifts();
        }
    }
    MarkNoUnitRuleOptimizationStates();
    RemoveDeadStates();
    MarkDefaultReductions();
    if( showflag ) {
        showstates();
    }
    checkwarn();
    parsestats();
    dumpstatistic( "parser states", nstate );
    dumpstatistic( "# states (1 reduce only)", nstate_1_reduce );
    dumpstatistic( "reduce/reduce conflicts", RR_conflicts );
    dumpstatistic( "shift/reduce conflicts", SR_conflicts );
    show_unused();
    actout = opena( "ytab.c", "w" );
    txfile( skelname,    skeleton, '\f' );               // skeleton 1st page -> ytab.c
    genobj();                               // tables            -> ytab.c
    txfile( skelname,    skeleton, '\f' );               // skeleton 2nd page -> ytab.c
    txfile( "tmpfile()", temp,     EOF );                    // case n: ... break -> ytab.c
    txfile( skelname,    skeleton, EOF );                // skeleton rest     -> ytab.c
    tail();                                 // any utility stuff -> ytab.c
    closec( skelname,    &skeleton    );
    closec( srcname,     &yaccin      );
    closec( "ytab.c",    &actout      );
    closec( "ytab.h",    &tokout      );
    closec( "tmpfile()", &temp        );
    closec( "stdout",    &stdout_copy );
    // No stderr check! Where would report go?
    checkwarn();
    return( 0 );
}

static FILE *opena( char const *filename, char const *access )
{
    FILE    *file;

    if( !(file = fopen( filename, access )) ) {
        msg( "Can't open %s for \"%s\" access: %d(%s).\n", filename, access,
            errno, strerror( errno ) );
    }
    return( file );
}

static void closec( char const *filename, FILE **fpp )
{
    FILE    *fp = *fpp;
    int     failure;

    *fpp = NULL;
    if( fp ) {
        failure = ferror( fp );
        failure |= fclose( fp );
        if( failure ) {
            warn( "%s I/O bad: %d(%s)\n", filename, errno, strerror( errno ) );
        }
    }
}

static void txfile( char const *filename, FILE *file, int stop )
{
    int ch;

    while( (ch = fgetc( file )) != stop && ch != EOF ) {
        if( ch != '\r' ) {
            fputc( ch, actout );
        }
    }
    if( ch != stop ) {
        msg( "Character value %d missing from %s\n", stop, filename );
    }
}

static void setoptions( char *p )
{
    for( ; *p; ++p ) {
        switch( *p ) {
        case 'b':
            bigflag = 1;
            break;
        case 'c':
            compactflag = 1;
            break;
        case 'd':
            if( p[1] == 's' ) {
                ++p;
                default_shiftflag = 1;
            } else {
                denseflag = 1;
            }
            break;
        case 'f':
            fastflag = 1;
            break;
        case 's':
            showflag = 1;
            break;
        case 'l':
            lineflag = 1;
            break;
        case 'p':
            proflag = 1;
            break;
        case 't':
            translateflag = 1;
            break;
        case 'u':
            eliminateunitflag = 1;
            break;
        case 'w':
            defaultwarnflag = 0;
            break;
        default:
            msg( "Unknown option '%c'\n", *p );
        }
    }
}

static void checkwarn( void )
{
    if( warnings ) {
        char const  *plural = (warnings == 1) ? "" : "s";

        // yacc ... and yacc ... > foo OK but yacc ... > foo 2>&1 duplicated
        fprintf( stderr, "%s: %d warning%s\n", srcname, warnings, plural );
        if( stdout_copy != NULL && !isatty( fileno( stdout ) ) ) {
            fprintf( stdout, "%s: %d warning%s\n", srcname, warnings, plural );
        }
        exit( 1 );
    }
}

static void diag( FILE *fp, char const *text, char const *fmt, va_list arg_ptr )
{
    if( srcname != NULL ) {
        fprintf( fp, "%s(%d): ", srcname, lineno );
    }
    fprintf( fp, "%s! ", text );
    vfprintf( fp, fmt, arg_ptr );
}

static void doublewrite( char const *text, char const *fmt, va_list arg_ptr )
{
    va_list arg_cpy;

    // yacc ... and yacc ... > foo OK but yacc ... > foo 2>&1 duplicated
    va_copy( arg_cpy, arg_ptr );
    diag( stderr, text, fmt, arg_cpy );
    va_end( arg_cpy );
    if( stdout_copy != NULL && !isatty( fileno( stdout ) ) ) {
        diag( stdout, text, fmt, arg_ptr );
    }
}

void warn( char *fmt, ... )
{
    va_list arg_ptr;

    va_start( arg_ptr, fmt );
    doublewrite( "Warning", fmt, arg_ptr );
    va_end( arg_ptr );
    ++warnings;
}

void msg( char *fmt, ... )
{
    va_list arg_ptr;

    va_start( arg_ptr, fmt );
    doublewrite( "Error", fmt, arg_ptr );
    va_end( arg_ptr );
    exit( 1 );
}

void dumpstatistic( char *name, unsigned stat )
{
    size_t len;

    len = strlen( name );
    printf( "%s:", name );
    while( len < 39 ) {
        putchar( ' ' );
        ++len;
    }
    printf( "%6u\n", stat );
}
