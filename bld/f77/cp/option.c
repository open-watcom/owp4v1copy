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
// OPTION    : command and source line option processing
//

#include "ftnstd.h"
#include "progsw.h"
#include "errcod.h"
#include "cpopt.h"
#include "optflags.h"
#include "global.h"
#include "bglobal.h"
#include "intcnv.h"
#include "csetinfo.h"
#if (_OPT_CG == _OFF) && ((_TARGET == _8086) || (_TARGET == _80386))
#include "mathopt.h"
#include "fcboth.h"
#include "fcgbls.h"
#endif
#if _OPT_CG == _ON
#include "compcfg.h"
#endif

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern  void            *FMemAlloc(int);
extern  void            FMemFree( void *p );
extern  bool            SetLst(bool);
extern  void            LFNewPage(void);
extern  void            ComPrint(void);
extern  char            *SkipBlanks(char *);
extern  void            Warning(int,...);
extern  void            LFSkip(void);
extern  void            PrtLstNL(char *);
extern  void            PrtLst(char *);
extern  uint            TypeSize(uint);
extern  void            MacroDEFINE(char *,uint);
extern  void            MacroUNDEFINE(char *,uint);
extern  void            MacroIFDEF(char *,uint);
extern  void            MacroIFNDEF(char *,uint);
extern  void            MacroELIFDEF(char *,uint);
extern  void            MacroELIFNDEF(char *,uint);
extern  void            MacroELSE(void);
extern  void            MacroENDIF(void);
extern  void            ProcPragma(char *);
extern  void            __UseChineseCharSet(void);
extern  void            __UseJapaneseCharSet(void);
extern  void            __UseKoreanCharSet(void);
#if _OPT_CG == _OFF
extern  void            RestData(void);
#endif

extern  character_set   CharSetInfo;

#if _OPT_CG == _ON
extern  uint            DataThreshold;
#endif
#if (_OPSYS == _PCDOS) && (_OPT_CG == _OFF) && (_EDITOR == _ON)
extern  bool            CmdLineOption;
#endif

// Compiler directives

#define CD_INCLUDE      1
#define CD_EJECT        2
#define CD_PRAGMA       3
#define CD_IFDEF        4
#define CD_IFNDEF       5
#define CD_ENDIF        6
#define CD_ELSE         7
#define CD_ELIFDEF      8
#define CD_ELIFNDEF     9
#define CD_DEFINE       10
#define CD_UNDEFINE     11
#if _OPT_CG == _OFF
#define CD_DATA         12
#endif

static const char __FAR * const __FAR CompDrctvs[] = {
    "INCLUDE",
    "EJECT",
    "PRAGMA",
    "IFDEF",
    "IFNDEF",
    "ENDIF",
    "ELSE",
    "ELSEIFDEF",
    "ELSEIFNDEF",
    "DEFINE",
    "UNDEFINE",
#if _OPT_CG == _OFF
    "DATA",
#endif
    NULL
};

#define DISK_MASK       (OPT_TYPE|OPT_PRINT)

// Used for matching OZOpts bits
#define _BitsMatched( bits, ptrn )      !((bits & ptrn) ^ ptrn)

#define INCLUDE_SEP     ';'

static  char    *SkipOpt( char *buff ) {
//======================================

// Skip over an option.

    while( isalnum( *buff ) ) {
        buff++;
    }
    return( buff );
}


static  bool    GetValue( opt_entry *optn, char *ptr, char **val ) {
//==================================================================

// Get pointer to option value.

    *val = SkipBlanks( ptr );
#if ( _OPT_CG == _OFF )
  #if (_TARGET == _80386) || (_TARGET == _8086)
    if( optn->value == OPT_LINK ) return( TRUE );
  #endif
#endif
    if( ( **val != '=' ) && ( **val != '#' ) ) {
        Warning( CO_NEED_EQUALS, optn->option );
        return( FALSE );
    } else {
        *val = SkipBlanks( *val + sizeof( char ) );
        return( TRUE);
    }
}


static  void    BitOption( opt_entry *optn, bool negated ) {
//==========================================================

// Process an option that has a bit value.

    ftnoption   opt_bit;

    opt_bit = optn->value;
#if ( _OPSYS == _PCDOS ) && ( _OPT_CG == _OFF ) && ( _EDITOR == _ON )
    if( ( opt_bit & OPT_EDIT ) && !CmdLineOption ) {
        Warning( CO_NOT_RECOG, optn->option );
        return;
    }
#endif
    if( !negated ) {
#if _OPT_CG == _OFF
    #if (_TARGET != _80386) && (_TARGET != _8086)
        // OBJECT turns off DEBUG and vice-versa
        if( opt_bit & ( OPT_OBJECT | OPT_DEBUG ) ) {
            NewOptions &= ~( OPT_OBJECT | OPT_DEBUG );
        }
    #endif
        // DEBUG turns on LIST
        if( opt_bit & OPT_DEBUG ) {
            NewOptions &= ~DISK_MASK; // force listing file to disk
            NewOptions |= OPT_LIST;
        }
#endif
        // TYPE turns off PRINT and vice-versa
        if( opt_bit & DISK_MASK ) {
            NewOptions &= ~DISK_MASK;
            NewOptions |= OPT_LIST;
        }
        if( opt_bit & OPT_LIST ) {
            SetLst( TRUE );
        }
#if _OPT_CG == _ON
        // SAVE turns off AUTOMATIC and vice-versa
        if( opt_bit & ( OPT_SAVE | OPT_AUTOMATIC ) ) {
            NewOptions &= ~( OPT_SAVE | OPT_AUTOMATIC );
        }
#endif
        NewOptions |= opt_bit;
    } else if( opt_bit & OPT_NO_NO ) {
        Warning( CO_BAD_NO, optn->option );
    } else {
        NewOptions &= ~opt_bit;
        if( opt_bit & OPT_LIST ) {
            SetLst( FALSE );
        }
    }
}


static  void    XLOption( opt_entry *optn, bool negated ) {
//=========================================================

// Extend source line beyond column 72.

    optn = optn;
    if( negated ) {
        LastColumn = LAST_COL;
    } else {
        LastColumn = SRCLEN;
    }
}


static  char    *SkipToken( char *buff ) {
//========================================

    for(;;) {
        if( *buff == NULLCHAR ) break;
        if( *buff == ' ' ) break;
        if( *buff == '\t' ) break;
        buff++;
    }
    return( buff );
}


static  void    DefOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Define a macro.

    optn = optn;
    MacroDEFINE( ptr, SkipToken( ptr ) - ptr );
}


static  void    PathOption( opt_entry *optn, char *ptr ) {
//========================================================

// Process "INCPATH=" option.

    char        *tmp;
    int         len;

    optn = optn;
    if( !IncludePath ) {
        IncludePath = FMemAlloc( strlen( ptr ) + 1 );
        *IncludePath = '\0';
    } else {
        len = strlen( IncludePath );
        tmp = FMemAlloc( strlen( ptr ) + len + 2 );
        strcpy( tmp, IncludePath );
        FMemFree( IncludePath );
        IncludePath = tmp;
        // We must glue the strings together correctly.
        if( ( tmp[ len - 1 ] != INCLUDE_SEP ) && ( *ptr != INCLUDE_SEP ) ) {
            tmp[ len ] = INCLUDE_SEP;
            tmp[ len + 1 ] = '\0';
        } else if( (tmp[len - 1] == INCLUDE_SEP) && (*ptr == INCLUDE_SEP) ) {
            tmp[ len - 1 ] = '\0';
        }
    }
    strcat( IncludePath, ptr );
}


void            FiniProcCmd(void) {
//=================================

    if( IncludePath ) {
        FMemFree( IncludePath );
    }
}


static  unsigned_32     OptV( opt_entry *optn, char *ptr ) {
//==========================================================

// Process an option that requires a value.

    unsigned_32 number;

    if( !isdigit( *ptr ) ) {
        Warning( CO_WANT_NUMBER, optn->option );
    }
    number = 0;
    for(;;) {
        number = 10 * number + ( *ptr - '0' );
        ptr++;
        if( isdigit( *ptr ) == 0 ) break;
    }
    return( number );
}


#if _OPT_CG == _ON


#define _Excl( excl_bits )  if( opt_bit & ( excl_bits ) ) {       \
                                CGOpts &= ~( excl_bits );         \
                            }


static  void    CGOption( opt_entry *optn, bool negated ) {
//=========================================================

// Process a code generator option.

    unsigned_32 opt_bit;

    opt_bit = optn->value;
    if( !negated ) {
#if _TARGET == _8086
        _Excl( CGOPT_M_MEDIUM | CGOPT_M_LARGE | CGOPT_M_HUGE );
#elif _TARGET == _80386
        _Excl( CGOPT_M_FLAT | CGOPT_M_SMALL | CGOPT_M_COMPACT |
               CGOPT_M_MEDIUM | CGOPT_M_LARGE );
        _Excl( CGOPT_BD | CGOPT_BM );
        _Excl( CGOPT_STACK_CHK | CGOPT_STACK_GROW );
#elif _TARGET == _AXP || _TARGET == _PPC
        _Excl( CGOPT_BD | CGOPT_BM );
#else
  #error Unknown platform
#endif
        _Excl( CGOPT_DB_LINE | CGOPT_DB_LOCALS );
        _Excl( CGOPT_DI_CV | CGOPT_DI_DWARF | CGOPT_DI_WATCOM );
        CGOpts |= opt_bit;
    } else if( !(opt_bit & CGOPT_NO_NO) ) {
        CGOpts &= ~opt_bit;
    } else {
        Warning( CO_BAD_NO, optn->option );
    }
}


#define _OZExcl( excl_bits )  if( opt_bit & ( excl_bits ) ) {       \
                                   OZOpts &= ~( excl_bits );        \
                              }

static  void    OZOption( opt_entry *optn, bool negated ) {
//=========================================================

// Process a optimization option.

    unsigned_32 opt_bit;

    opt_bit = optn->value;
    if( !negated ) {
        _OZExcl( OZOPT_O_SPACE | OZOPT_O_TIME );
        OZOpts |= opt_bit;
    } else if( !(opt_bit & OZOPT_NO_NO) ) {
        OZOpts &= ~opt_bit;
    } else {
        Warning( CO_BAD_NO, optn->option );
    }
}


#define _CPUExcl( excl_bits )  if( opt_bit & ( excl_bits ) ) {       \
                                   CPUOpts &= ~( excl_bits );        \
                               }

#if _TARGET == _8086 || _TARGET == _80386
static  void    CPUOption( opt_entry *optn, bool negated ) {
//==========================================================

// Process a code generator option.

    unsigned_32 opt_bit;

    opt_bit = optn->value;
    if( !negated ) {
#if _TARGET == _8086
        _CPUExcl( CPUOPT_8086 | CPUOPT_80186 | CPUOPT_80286 |
                  CPUOPT_80386 | CPUOPT_80486 | CPUOPT_80586 );
#else
        _CPUExcl( CPUOPT_80386 | CPUOPT_80486 | CPUOPT_80586 );
#endif
        _CPUExcl( CPUOPT_FPC | CPUOPT_FPI | CPUOPT_FPI87 )
        _CPUExcl( CPUOPT_FPC | CPUOPT_FP287 | CPUOPT_FP387 | CPUOPT_FP5 );
        CPUOpts |= opt_bit;
    } else if( !(opt_bit & CPUOPT_NO_NO) ) {
        CPUOpts &= ~opt_bit;
    } else {
        Warning( CO_BAD_NO, optn->option );
    }
}
#endif


static  void    DTOption( opt_entry *optn, char *ptr ) {
//======================================================

// Process "DT=" option.

    DataThreshold = OptV( optn, ptr );
}


static  void    FOOption( opt_entry *optn, char *ptr ) {
//======================================================

// Process "FO=" option.

    optn = optn;
    ObjName = ptr;
}


#else


static  bool    TooBig( unsigned_32 *number, unsigned_32 max ) {
//==============================================================

// Check to see if value has exceeded maximum allowable value.

    bool        too_big;

    if( ( max != 0 ) && ( *number > max ) ) {
        too_big = TRUE;
    } else {
        too_big = FALSE;
        if( *number == 0 ) {
            *number = max;
        }
    }
    return( too_big );
}


static  void    StaOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Process "STATEMENTS=" option.

    unsigned_32 number;

    number = OptV( optn, ptr );
    if( TooBig( &number, MaxStLim ) ) {
        Warning( CO_LIMIT_TOO_BIG, optn->option );
    } else {
        StmtLimit = number;
    }
}


static  void    PagOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Process "PAGES=" option.

    unsigned_32 number;

    number = OptV( optn, ptr );
    if( TooBig( &number, MaxPageLim ) ) {
        Warning( CO_LIMIT_TOO_BIG, optn->option );
    } else {
        PageLimit = number;
    }
}


static  void    TimOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Process "TIME=" option.

    unsigned_32 number;

    number = OptV( optn, ptr );
    if( TooBig( &number, MaxTimeLim ) ) {
        Warning( CO_LIMIT_TOO_BIG, optn->option );
    } else {
        TimeLimit = number;
    }
}


#if (_TARGET == _VAX) || (_OPSYS == _QNX) || (_TARGET == _80386) || (_TARGET == _8086)


static  void    CodOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Process "CODESIZE=" option.

    unsigned_32 number;

    number = OptV( optn, ptr );
    ptr = SkipToken( ptr ) - sizeof( char );
    if( toupper( *ptr ) == 'K' ) {
        number *= 1024;
    }
    ObjLimit = number;
}


#endif


static  void    PSzOption( opt_entry *optn, char *ptr ) {
//=======================================================

// Process "PAGESIZE=" option

    unsigned_32 number;

    number = OptV( optn, ptr );
    ptr = SkipToken( ptr ) - sizeof( char );
    LinesPerPage = number;
}


static  void    MthOption( opt_entry *optn, bool negated ) {
//==========================================================

// Process an option that has a bit value for MathOptions.

    byte        opt_bit;

    opt_bit = optn->value;
    if( !negated ) {
        MathOptions |= opt_bit;
    } else {
        MathOptions &= ~opt_bit;
    }
}


#if ( _TARGET == _80386 ) || (_TARGET == _8086 )

static  void    LnkOption( opt_entry *optn, char *ptr ) {
//=======================================================

    optn = optn;
    NewOptions |= OPT_LINK;
    if( *ptr == '=' || *ptr == '#' ) {
        LinkFile = SkipBlanks( ptr + sizeof( char ) );
    } else {
        LinkFile = NULL;
    }
}

#endif


#endif


static  void    NegOption( opt_entry *optn, bool negated ) {
//==========================================================

// Turn off bit option.

    optn = optn;
    negated = negated;
    NewOptions &= ~optn->value;
    if( optn->value == DISK_MASK ) {
        NewOptions |= OPT_LIST;
    }
}


static  void    ChiOption( opt_entry *optn, bool negated ) {
//==========================================================

    optn = optn; negated = negated;
    __UseChineseCharSet();
    NewOptions &= ~( OPT_CHINESE | OPT_JAPANESE | OPT_KOREAN );
    NewOptions |= OPT_CHINESE;
}


static  void    JapOption( opt_entry *optn, bool negated ) {
//==========================================================

    optn = optn; negated = negated;
    __UseJapaneseCharSet();
    NewOptions &= ~( OPT_CHINESE | OPT_JAPANESE | OPT_KOREAN );
    NewOptions |= OPT_JAPANESE;
}


static  void    KorOption( opt_entry *optn, bool negated ) {
//==========================================================

    optn = optn; negated = negated;
    __UseKoreanCharSet();
    NewOptions &= ~( OPT_CHINESE | OPT_JAPANESE | OPT_KOREAN );
    NewOptions |= OPT_KOREAN;
}


#define opt( name, bit, flags, action, desc ) name, desc, flags, bit, action

#include "optinfo.h"


static  bool    Match( char *buff, const char __FAR *list, bool value ) {
//=======================================================================

// Determine if option matches.

    for(;;) {
        if( *buff == NULLCHAR ) break;
        if( *buff == ' ' ) break;
        if( *buff == '\t' ) break;
        if( value ) {
            if( ( *buff == '=' ) || ( *buff == '#' ) ) break;
        }
        if( isalpha( *buff ) ) {
            if( toupper( *buff ) != toupper( *list ) ) return( FALSE );
        } else {
            if( *buff != *list ) return( FALSE );
        }
        ++list;
        ++buff;
    }
    if( isalpha( *list ) ) {
        if( isupper( *list ) ) return( FALSE );
    } else {
        if( *list != NULLCHAR ) return( FALSE );
    }
    return( TRUE );
}


static  opt_entry       *GetOptn( char *buff, bool *negated ) {
//=============================================================

// Parse an option.

    opt_entry   *optn;

    optn = CompOptns;
    *negated = FALSE;
    if( ( toupper( buff[ 0 ] ) == 'N' ) && ( toupper( buff[ 1 ] ) == 'O' ) ) {
        *negated = TRUE;
        buff += 2 * sizeof( char );
    }
    for( optn = CompOptns; optn->option != NULL; ++optn ) {
        if( optn->flags & CTG ) continue;
        if( Match( buff, optn->option, optn->flags & VAL ) ) {
            return( optn );
        }
    }
    return( NULL );
}


void    CmdOption( char *buff ) {
//===============================

// Process an option that can appear on the command line.

    opt_entry   *optn;
    bool        negated;
    char        *value;

    optn = GetOptn( buff, &negated );
    if( optn == NULL ) {
        Warning( CO_NOT_RECOG, buff );
    } else {
        if( optn->flags & VAL ) {
            if( negated ) {
                Warning( CO_BAD_NO, optn->option );
            }
            if( GetValue( optn, SkipOpt( buff ), &value ) ) {
                optn->proc_rtn( optn, value );
            }
        } else {
            optn->proc_rtn( optn, negated );
        }
    }
}


void    SrcOption() {
//===================

// Process an option that can appear only in the source input stream.

    int         directive;
    char        *buff;

    buff = &SrcBuff[ 2 ];
    directive = GetDirective( buff );
    if( directive == CD_INCLUDE ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        CurrFile->flags |= INC_PENDING;
    } else if( directive == CD_EJECT ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        LFNewPage();
#if _OPT_CG == _OFF
    } else if( directive == CD_DATA ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        ComPrint();
        RestData();
#endif
    } else if( directive == CD_PRAGMA ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        ComPrint();
        ProcPragma( SkipOpt( buff ) );
    } else if( directive == CD_DEFINE ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        ComPrint();
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroDEFINE( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_UNDEFINE ) {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        ComPrint();
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroUNDEFINE( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_IFDEF ) {
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroIFDEF( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_ELIFDEF ) {
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroELIFDEF( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_IFNDEF ) {
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroIFNDEF( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_ELIFNDEF ) {
        buff = SkipBlanks( SkipOpt( buff ) );
        MacroELIFNDEF( buff, SkipToken( buff ) - buff );
    } else if( directive == CD_ELSE ) {
        MacroELSE();
    } else if( directive == CD_ENDIF ) {
        MacroENDIF();
    } else {
        if( ProgSw & PS_SKIP_SOURCE ) return;
        ComPrint();
        ScanOpts( buff );
        // consider:
        //      c$warn
        //      c$notime=5
        // CO-04 will not be issued unless /warn or c$warn is done.  But
        // in the above case isn't updated unless we do this.
        Options = NewOptions;
    }
}


static  void    OptWarning( int warn, char *opt ) {
//=================================================

// Issue an option warning message.

    char        *ptr;

    ptr = SkipToken( opt );
    *ptr = NULLCHAR;
    Warning( warn, opt );
}


static  void    ScanOpts( char *buff ) {
//======================================

    opt_entry   *optn;
    bool        negated;
    char        *value;
    bool        first_opt;

    if( strlen( SrcBuff ) > LastColumn ) {
        SrcBuff[ LastColumn ] = NULLCHAR;
    }
    first_opt = TRUE;
    for(;;) {
        buff = SkipBlanks( buff );
        if( *buff == NULLCHAR ) break;
        optn = GetOptn( buff, &negated );
        if( optn == NULL ) {
            if( !first_opt ) {
                OptWarning( CO_NOT_RECOG, buff );
            }
            break;
        }
        first_opt = FALSE;
        if( ( optn->flags & SRC ) == 0 ) {
            Warning( CO_NOT_IN_SOURCE, optn->option );
            buff = SkipOpt( buff );
        } else {
            buff = SkipOpt( buff );
            if( optn->flags & VAL ) {
                if( negated ) {
                    Warning( CO_BAD_NO, optn->option );
                }
                if( !GetValue( optn, buff, &value ) ) continue;
                buff = SkipToken( value );
                optn->proc_rtn( optn, value );
            } else {
                optn->proc_rtn( optn, negated );
            }
        }
    }
}


static  int     GetDirective( char *buff ) {
//==========================================

// Parse a compiler directive.

    int         offset;
    const char  __FAR * const __FAR *drctv;

    drctv = CompDrctvs;
    offset = 0;
    for(;;) {
        if( drctv[ offset ] == NULL ) return( 0 );
        if( Match( buff, drctv[ offset ], FALSE ) ) return( offset + 1 );
        offset++;
    }
}


static  char    *GetOptName( char *buffer, char *opt_name ) {
//===========================================================

// Print option name.

    char        *buff;

    buff = buffer + sizeof( char );
    while( *opt_name != NULLCHAR ) {
        *buff = tolower( *opt_name );
        opt_name++;
        buff++;
    }
    *buff = NULLCHAR;
    return( buff );
}


void    PrtOptions() {
//====================

// Display current options in listing file.

    opt_entry   *optn;
    char        buffer[30];
    char        *buff;
    unsigned_32 number;

    LFSkip();
    PrtLst( "Options:" );
    buffer[ 0 ] = ' ';
    optn = CompOptns;
    for( optn = CompOptns; optn->option != NULL; optn++ ) {
        if( optn->flags & VAL ) {
            // the following check will only work if
            // OPT_DEFINE is a "VAL" option
            if( optn->value == OPT_DEFINE ) continue;
#if _OPT_CG == _OFF
            switch( optn->value ) {
            case OPT_ST_LIM:
                number = StmtLimit;
                break;
            case OPT_PG_LIM:
                number = PageLimit;
                break;
            case OPT_TM_LIM:
                number = TimeLimit;
                break;
#if (_OPSYS == _QNX) || (_TARGET == _VAX) || (_TARGET == _80386) || (_TARGET == _8086)
            case OPT_MM_LIM:
                number = ObjLimit;
                break;
#endif
#if (_TARGET == _80386) || (_TARGET == _8086)
            case OPT_PAGE_SIZE:
                number = LinesPerPage;
                break;
#endif
            }
#else
            if( optn->value == CGOPT_DATA_THRESH ) {
                number = DataThreshold;
            }
#endif
            buff = GetOptName( buffer, optn->option );
            *buff = '=';
            ++buff;
            if( optn->value == OPT_INCPATH ) {
                if( IncludePath == NULL ) continue;
                *buff = NULLCHAR;
                PrtLst( buffer );
                PrtLst( IncludePath );
                buffer[ 0 ] = ',';
                continue;
            }
#if _OPT_CG == _ON
            if( optn->value == CGOPT_OBJ_NAME ) {
                if( ObjName == NULL ) continue;
                *buff = NULLCHAR;
                PrtLst( buffer );
                PrtLst( ObjName );
                buffer[ 0 ] = ',';
                continue;
            }
#elif (_TARGET == _80386) || (_TARGET == _8086)
            if( optn->value == OPT_LINK ) {
                if( ( Options & OPT_LINK ) == 0 ) continue;
                if( LinkFile == NULL ) {
                    --buff;             // remove '='
                }
                *buff = NULLCHAR;
                PrtLst( buffer );
                if( LinkFile != NULL ) {
                    PrtLst( LinkFile );
                }
                buffer[ 0 ] = ',';
                continue;
            }
#endif
            ltoa( number, buff, 10 );
            buff += strlen( buff );
            *buff = NULLCHAR;
            PrtLst( buffer );
            buffer[ 0 ] = ',';
#if (_OPT_CG == _OFF) && ((_TARGET == _80386) || (_TARGET == _8086))
        } else if( optn->flags & MTH ) {
            if( ( MathOptions & optn->value ) == 0 ) continue;
            GetOptName( buffer, optn->option );
            PrtLst( buffer );
            buffer[ 0 ] = ',';
#endif
        } else {
#if _OPT_CG == _ON
            if( optn->flags & CG ) {
                if( optn->flags & NEG ) {
                    if( CGOpts & optn->value ) continue;
                } else {
                    if( ( CGOpts & optn->value ) == 0 ) continue;
                }
            } else if( optn->flags & CPU ) {
                if( optn->flags & NEG ) {
                    if( CPUOpts & optn->value ) continue;
                } else {
                    if( ( CPUOpts & optn->value ) == 0 ) continue;
                }
            } else if( optn->flags & OZ ) {
                if( optn->flags & NEG ) {
                    if( _BitsMatched( OZOpts, optn->value ) ) continue;
                } else {
                    if( !_BitsMatched( OZOpts, optn->value ) ) continue;
                }
            } else
#endif
            if( optn->value == OPT_XLINE ) {
                if( LastColumn == LAST_COL ) continue;
            } else {
                if( optn->flags & NEG ) {
                    if( Options & optn->value ) continue;
                } else {
                    if( ( Options & optn->value ) == 0 ) continue;
                }
            }
            GetOptName( buffer, optn->option );
            PrtLst( buffer );
            buffer[ 0 ] = ',';
        }
    }
    PrtLstNL( "" );
}
