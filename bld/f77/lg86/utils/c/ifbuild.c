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
//  IFBUILD     : produces fciftbl and lg*.asm files
//

#include "ftnstd.h"
#include "ifargs.h"
#include "ifflags.h"
#include "ifnames.h"
#include "symflgs.h"
#include "parmtype.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern  byte            IFArgType(byte);

extern  const char      *IFNames[];
extern  const byte __FAR IFArgCt[];
extern  const unsigned_16 __FAR IFFlags[];
extern  const byte __FAR IFSelect[];

#define IF_IND  0x10            // if indicator
#define SEG_CS          0x2e
#if _TARGET == _8086
#define CALL    0x9a            // "seg CS, far call"
#else
#define CALL    0xe8            // "seg CS, near call"
#endif

#define STATIC

#define MAKE_TABENT( num )      \
    ( ( ( (IFFlags[(num)]&IF_GENERIC) == 0 || IFArgType(num)!=IF_NO_TYPE ) && \
      ( IFSelect[(num)] != MAGIC ) ) || (num) == IF_CHAR )

// get text names for all the ufns
#define UFN( x )        #x ,
static  char    *UFNames[] = {
#include <ufns.inc>
};
#undef UFN
#define NUM_UFNS        ( sizeof( UFNames ) / sizeof( char * ) )

#define X_PREFIX        "X"


STATIC void writeAnExtern( FILE *out, int if_num, int is_386 ) {
//==============================================================

    fprintf( out, "\textrn\t%s\@%s%s : %s\n", "IF",
        ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
        IFNames[ if_num ], ( is_386 ? "near" : "far" ) );
}


STATIC void writeTabent( FILE *out, int if_num, int is_386 ) {
//============================================================

    if( MAKE_TABENT( if_num ) ) {
        // generate a reference to the data above
        fprintf( out, "\tdb\t%d\n\tcall\tExt_to_F77\n\t%s\toffset%s _%s_\n",
            SEG_CS, is_386 ? "dd" : "dw",
            is_386 ? "32" : "", IFNames[ if_num ] );
    } else {
        // generate a null table entry
        fprintf( out, "\tdb\t%d,%d\n\tdd\t0\n\t%s\t0\n", SEG_CS, CALL,
            is_386 ? "dd" : "dw" );
    }
}


STATIC void writeFnData( FILE *out, int if_num, int is_386 ) {
//============================================================

    byte        typ;
    byte        arg_ct;

    fprintf( out, "_%s_ label byte\n", IFNames[ if_num ] );
    switch( IFFlags[ if_num ] & IF_TYPE ) {
    case IFT_LOGICAL_1: typ = PT_LOG_1;         break;
    case IFT_LOGICAL:   typ = PT_LOG_4;         break;
    case IFT_INTEGER_1: typ = PT_INT_1;         break;
    case IFT_INTEGER_2: typ = PT_INT_2;         break;
    case IFT_INTEGER:   typ = PT_INT_4;         break;
    case IFT_REAL:      typ = PT_REAL_4;        break;
    case IFT_DOUBLE:    typ = PT_REAL_8;        break;
    case IFT_TRUE_EXTENDED: typ = PT_REAL_16;   break;
    case IFT_COMPLEX:   typ = PT_CPLX_8;        break;
    case IFT_DCOMPLEX:  typ = PT_CPLX_16;       break;
    case IFT_TRUE_XCOMPLEX: typ = PT_CPLX_32;   break;
    case IFT_CHAR:      typ = PT_CHAR;          break;
    case IFT_NOTYPE:    typ = PT_NOTYPE;        break;
    }
    fprintf( out, "\tdb\t%d", typ + IF_IND );
    switch( IFArgType( if_num ) ) {
    case TY_LOGICAL_1:  typ = PT_LOG_1;         break;
    case TY_LOGICAL:    typ = PT_LOG_4;         break;
    case TY_INTEGER_1:  typ = PT_INT_1;         break;
    case TY_INTEGER_2:  typ = PT_INT_2;         break;
    case TY_INTEGER:    typ = PT_INT_4;         break;
    case TY_REAL:       typ = PT_REAL_4;        break;
    case TY_DOUBLE:     typ = PT_REAL_8;        break;
    case TY_TRUE_EXTENDED: typ = PT_REAL_16;    break;
    case TY_COMPLEX:    typ = PT_CPLX_8;        break;
    case TY_DCOMPLEX:   typ = PT_CPLX_16;       break;
    case TY_TRUE_XCOMPLEX: typ = PT_CPLX_32;    break;
    case TY_CHAR:       typ = PT_CHAR;          break;
    case IF_NO_TYPE:    typ = PT_NOTYPE;        break;
    }
    switch( IFArgCt[ if_num ] & IF_COUNT_MASK ) {
    case ONE_ARG:       arg_ct = 1;             break;
    case TWO_ARG:       arg_ct = 2;             break;
    case TWO_OR_MORE:   arg_ct = 0;             break;
    }
    fprintf( out, ",%d\n", ( typ * 4 ) + arg_ct );
    if( is_386 ) {
        fprintf( out, "\tdd\toffset32 IF@%s%s\n",
            ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE)
                    ? X_PREFIX : "", IFNames[ if_num ] );
    } else {
        fprintf( out, "\tdd\tIF@%s%s\n",
            ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE)
                    ? X_PREFIX : "", IFNames[ if_num ] );
    }
}


STATIC void buildIfTable( int is_386, const char *filename ) {
//============================================================

    FILE        *out;
    int         if_num;
    char        fname[ _MAX_FNAME ];

    out = fopen( filename, "w" );
    if( out == NULL ) {
        fprintf( stderr, "error opening %s for write: %s\n", filename,
                    strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    fprintf( out, "; this file generated by \\f77\\lg86\\utils\\c\\ifbuild t%c %s\n",
        is_386 ? '3' : '2', filename );
    if( is_386 ) {
        fprintf( out, ".386\n" );
    }
    fprintf( out, "include mdef.inc\n" );
    fprintf( out, "\txref\tExt_to_F77\n" );

    // do the extrn's
    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            writeAnExtern( out, if_num, is_386 );
        }
    }

    _splitpath( filename, NULL, NULL, fname, NULL );
    fprintf( out, "\tmodstart\t%s\n", fname );

    // generate the data
    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            writeFnData( out, if_num, is_386 );
        }
    }

    // generate the table
    fprintf( out, "\txdefp\tLG@IF_TABLE\ndefn\tLG@IF_TABLE\n" );
    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        writeTabent( out, if_num, is_386 );
    }
    fprintf( out, "endproc\tLG@IF_TABLE\n" );
    fprintf( out, "\tendmod\nend\n" );
    fclose( out );
}


STATIC  void    buildLG3Files( char *dir ) {
//=========================================

    int         if_num;
    FILE        *out;
    char        fname[ _MAX_FNAME ];
    char        path[ _MAX_PATH ];

    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            sprintf( fname, "lg%.6s", IFNames[ if_num ] );
            _makepath( path, NULL, dir, fname, ".asm" );
            out = fopen( path, "w" );
            if( out == NULL ) {
                fprintf( stderr, "error opening %s for write: %s\n", path,
                            strerror( errno ) );
                exit( EXIT_FAILURE );
            }
            fprintf( out,
            "; this file generated by \\f77\\lg86\\utils\\c\\ifbuild f3 %s\n",
                dir );
            fprintf( out, ".386\ninclude mdef.inc\n" );
            fprintf( out, "\txref\tExt_to_F77\n" );
            writeAnExtern( out, if_num, TRUE );
            fprintf( out, "\tmodstart\t%s\n", fname );
            writeFnData( out, if_num, TRUE );
            fprintf( out, "\txdefp\tLG@%s%s\n",
                ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
                IFNames[ if_num ] );
            fprintf( out, "LG@%s%s label byte\n",
                ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
                IFNames[ if_num ] );
            writeTabent( out, if_num, TRUE );
            fprintf( out, "\tendmod\nend\n" );
            fclose( out );
        }
    }
}

STATIC  void    buildLG2Files( char *dir ) {
//=========================================

    int         if_num;
    FILE        *out;
    char        fname[ _MAX_FNAME ];
    char        path[ _MAX_PATH ];

    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            sprintf( fname, "lg%.6s", IFNames[ if_num ] );
            _makepath( path, NULL, dir, fname, ".asm" );
            out = fopen( path, "w" );
            if( out == NULL ) {
                fprintf( stderr, "error opening %s for write: %s\n", path,
                            strerror( errno ) );
                exit( EXIT_FAILURE );
            }
            fprintf( out,
            "; this file generated by \\f77\\lg86\\utils\\c\\ifbuild f2 %s\n",
                dir );
            fprintf( out, "include mdef.inc\n" );
            fprintf( out, "\txref\tExt_to_F77\n" );
            writeAnExtern( out, if_num, FALSE );
            fprintf( out, "\tmodstart\t%s\n", fname );
            fprintf( out, "\txdefp\tLG@%s%s\n",
                ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
                IFNames[ if_num ] );
            fprintf( out, "LG@%s%s label byte\n",
                ((IFArgCt[if_num] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
                IFNames[ if_num ] );
            writeTabent( out, if_num, FALSE );
            writeFnData( out, if_num, FALSE );
            fprintf( out, "\tendmod\nend\n" );
            fclose( out );
        }
    }
}

STATIC  void    buildLG2oldFiles( char *dir ) {
//==========================================

    int         if_num;
    FILE        *out;
    char        fname[ _MAX_FNAME ];
    char        path[ _MAX_PATH ];
    int         num_args;
    char        *ret;
    int         arg_type;
    static char *type_names[] = {
        "logstar4",
        "intstar4",
        "single",
        "double",
        "extended",
        "complex",
        "dcomplex",
        "xcomplex",
        "string"
    };

    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            sprintf( fname, "lg%.6s", IFNames[ if_num ] );
            num_args = IFArgCt[ if_num ] & IF_COUNT_MASK;
            if( num_args == TWO_OR_MORE ) {
                _makepath( path, NULL, dir, fname, ".asm" );
                out = fopen( path, "w" );
                if( out == NULL ) {
                    fprintf( stderr, "error opening %s for write: %s\n", path,
                                strerror( errno ) );
                    exit( EXIT_FAILURE );
                }
                fprintf( out,
            "; this file generated by \\f77\\lg86\\utils\\c\\ifbuild f2 %s\n",
                    dir );
                fprintf( out, "\tname\tlg%s\n", IFNames[ if_num ] );
                fprintf( out, "\textrn\t%s : far\n", IFNames[ if_num ] );
                fprintf( out, "_TEXT\tsegment public byte 'CODE'\n" );
                // for 286 compat code
                fprintf( out, "\tASSUME CS:_TEXT\n" );
                fprintf( out, "\tpublic\tIF@%s%s\n", X_PREFIX,
                                IFNames[ if_num ] );
                fprintf( out, "IF@%s%s:\tjmp\t%s\n", X_PREFIX,
                         IFNames[ if_num ], IFNames[ if_num ] );
                fprintf( out, "_TEXT\tends\n\tend\n" );
            } else {
                _makepath( path, NULL, dir, fname, ".c" );
                out = fopen( path, "w" );
                if( out == NULL ) {
                    fprintf( stderr, "error opening %s for write: %s\n", path,
                                strerror( errno ) );
                    exit( EXIT_FAILURE );
                }
                fprintf( out,
            "/* this file generated by \\f77\\lg86\\utils\\c\\ifbuild f2 %s */\n",
                    dir );
                fprintf( out, "#include \"ftnstd.h\"\n" );
                fprintf( out, "#undef _OPT_CG\n" );
                fprintf( out, "#define _OPT_CG _ON\n" );
                fprintf( out, "#include \"ifenv.h\"\n" );
                fprintf( out, "#pragma aux _FUNC \"%s\";\n", IFNames[ if_num ] );
                switch( IFFlags[ if_num ] & IF_TYPE ) {
                case IFT_LOGICAL_1:     ret = "logstar1";       break;
                case IFT_LOGICAL:       ret = "logstar4";       break;
                case IFT_INTEGER_1:     ret = "intstar1";       break;
                case IFT_INTEGER_2:     ret = "intstar1";       break;
                case IFT_INTEGER:       ret = "intstar4";       break;
                case IFT_REAL:          ret = "single";         break;
                case IFT_DOUBLE:        ret = "double";         break;
                case IFT_TRUE_EXTENDED: ret = "extended";       break;
                case IFT_COMPLEX:       ret = "complex";        break;
                case IFT_DCOMPLEX:      ret = "dcomplex";       break;
                case IFT_TRUE_XCOMPLEX: ret = "xcomplex";       break;
                }
                arg_type = IFArgType( if_num );
                if( ( IFFlags[ if_num ] & IF_TYPE ) == IFT_CHAR ) {
                    fprintf( out, "extern void _FUNC( %s PGM *, string PGM * );\n",
                        type_names[ arg_type ] );
                    fprintf( out, "void %s( %s %s a1, string PGM *a2 ) {\n",
                        IFNames[ if_num ],
                        type_names[ arg_type ],
                        ( arg_type == TY_CHAR ) ? "PGM *" : "" );
                    fprintf( out, "\t_FUNC( %sa1, a2 );\n}\n",
                        ( arg_type == TY_CHAR ) ? "" : "&" );
                } else if( num_args == TWO_ARG ) {
                    fprintf( out, "extern %s _FUNC( %s PGM *, %s PGM * );\n",
                        ret,
                        type_names[ arg_type ],
                        type_names[ arg_type ] );
                    fprintf( out, "%s %s( %s %s a1, %s %s a2 ) {\n",
                        ret,
                        IFNames[ if_num ],
                        type_names[ arg_type ],
                        ( arg_type == TY_CHAR ) ? "PGM *" : "",
                        type_names[ arg_type ],
                        ( arg_type == TY_CHAR ) ? "PGM *" : "" );
                    fprintf( out, "\treturn( _FUNC( %sa1, %sa2 ) );\n}\n",
                        ( arg_type == TY_CHAR ) ? "" : "&",
                        ( arg_type == TY_CHAR ) ? "" : "&" );
                } else {
                    fprintf( out, "extern %s _FUNC( %s PGM * );\n",
                        ret,
                        type_names[ arg_type ] );
                    fprintf( out, "%s %s( %s %s a1 ) {\n",
                        ret,
                        IFNames[ if_num ],
                        type_names[ arg_type ],
                        ( arg_type == TY_CHAR ) ? "PGM *" : "" );
                    fprintf( out, "\treturn( _FUNC( %sa1 ) );\n}\n",
                        ( arg_type == TY_CHAR ) ? "" : "&" );
                }
            }
            fclose( out );
        }
    }
}


STATIC void buildNoVector( const char *filename ) {
//=================================================

    int         idx;
    FILE        *out;

    out = fopen( filename, "w" );
    if( out == NULL ) {
        fprintf( stderr, "error opening %s for write: %s\n", filename,
                    strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    fprintf( out,
"# this file generated by \\f77\\lg86\\utils\\c\\ifbuild v %s\n"
"#\n"
"# The intrinsic function table needs to be accessed by the compile-time system;\n"
"# but functions won't be called until run-time.  So we novector all the entries\n"
"# to get the proper results.\n"
"#\n", filename );
    for( idx = 0; idx <= IF_MAX_NAME; ++idx ) {
        if( MAKE_TABENT( idx ) ) {
            fprintf( out, "novector 'IF@%s%s'\n",
                ((IFArgCt[idx] & IF_COUNT_MASK) == TWO_OR_MORE) ? X_PREFIX : "",
                IFNames[ idx ] );
        }
    }
    for( idx = 0; idx < NUM_UFNS; ++idx ) {
        fprintf( out, "novector '%s'\n", UFNames[ idx ] );
    }
    fclose( out );
}


STATIC void buildMIF( const char *filename ) {
//============================================

    int         if_num;
    FILE        *out;

    out = fopen( filename, "w" );
    if( out == NULL ) {
        fprintf( stderr, "error opening %s for write: %s\n", filename,
                    strerror( errno ) );
        exit( EXIT_FAILURE );
    }
    fprintf( out,
        "# this file generated by \\f77\\lg86\\utils\\c\\ifbuild m %s\n",
        filename );
    fprintf( out, "obj_ifns =" );
    for( if_num = 0; if_num <= IF_MAX_NAME; ++if_num ) {
        if( MAKE_TABENT( if_num ) ) {
            fprintf( out, " &\n..\\ifns\\lg%.6s.obj", IFNames[ if_num ] );
        }
    }
    fprintf( out, "\n" );
    fclose( out );
}


#pragma aux usage aborts;
STATIC void usage( void ) {
//=========================

    fputs( "usage:\n", stderr );
    fputs( "    ifbuild t2 fciftbl.asm\t\t-- build 286 fciftbl.asm\n", stderr );
    fputs( "    ifbuild t3 fciftbl.asm\t\t-- build 386 fciftbl.asm\n", stderr );
    fputs( "    ifbuild f2 directory\t\t\t-- build lg*.c files\n", stderr );
    fputs( "    ifbuild f3 directory\t\t\t-- build lg*.asm files\n", stderr );
    fputs( "    ifbuild m ifns.mif\t\t\t-- build ifns.mif file\n", stderr );
    fputs( "    ifbuild v fciftbl.lnk\t\t-- build novector file\n", stderr );
    exit( EXIT_FAILURE );
}


void main( int argc, char **argv ) {
//==================================

    if( argc != 3 ) usage();
    switch( argv[1][0] ) {
    case 't':
        if( argv[1][1] == 0 ) usage();
        buildIfTable( argv[1][1] - '2', argv[2] );
        break;
    case 'f':
        switch( argv[1][1] ) {
        case '3':
            buildLG3Files( argv[2] );
            break;
        case '2':
            buildLG2Files( argv[2] );
            break;
        default:
            usage();
        }
        break;
    case 'm':
        buildMIF( argv[2] );
        break;
    case 'v':
        buildNoVector( argv[2] );
        break;
    default:
        usage();
        break;
    }
    exit( EXIT_SUCCESS );
}
