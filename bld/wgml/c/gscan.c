/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  utility functions for wgml input line scanning
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

//  #include "gscan.h"

typedef struct tag {
    struct tag  *next;
    char        tagname[ 16 ];
    unsigned    min_abbrev;
    void       (*tagproc)( void );
} tag;

typedef enum {
    selfdef     = 4,
    aritherr    = 8,
    ilorder     = 12,
    illchar     = 16,
    mnyerr      = 20,
    operr       = 24,
    parerr      = 28,
    enderr      = 32
} errcode;

typedef struct getnum_block {
    condcode    cc;
    int         ignore_blanks;          // 1 if blanks are ignored
    char        *argstart;
    char        *argstop;
    char        *errstart;
    char        *first;
    long        length;
    long        res;
    errcode     error;
    char        resc[5];

} getnum_block;



condcode getnum( getnum_block *gn );


static char    *scan_start;
static char    *scan_stop;


/***************************************************************************/
/*  .im   processing  IMBED                                                */
/***************************************************************************/

static  void    sc_im( void )
{
    char            *fnstart;
    char            *p;
    char            quote;
    condcode        cc;
    getnum_block    gn;

    p = scan_start;
    while( *p == ' ' ) {
        p++;
    }

    gn.argstart = p;
    gn.argstop  = scan_stop;
    gn.ignore_blanks = 0;
    cc = getnum( &gn );

    p = gn.argstart;

    if( *p == '"' || *p == '\'' ) {
        quote = *p;
        ++p;
    } else {
        quote = ' ';                    // error??
    }
    fnstart = p;
    while( *p && *p != quote ) {
        ++p;
    }
    *p = '\0';
    strcpy_s( token_buf, buf_size, fnstart );

    ProcFlags.newLevel = TRUE;
    line_from = LINEFROM_DEFAULT;
    line_to   = LINETO_DEFAULT;


    return;
}

/***************************************************************************/
/*  .ap processing APPEND                                                  */
/***************************************************************************/

static  void    sc_ap( void )
{
    file_cbs->flags &= FF_eof;          // simulate EOF for .append
    sc_im();                            // do .imbed processing
}


/***************************************************************************/
/*                                                                         */
/***************************************************************************/

tag     tags[] = {
    { NULL, "im",       2,  sc_im },
    { NULL, "ap",       2,  sc_ap },
//    { NULL, "dm",       2,  sc_dm },
    { NULL, "  ",       0,  NULL  } };

#define TAGMAX  (sizeof( tags ) / sizeof( tags[ 0 ] ) - 1)


#define MAX_TAGNAME_LENGTH    (sizeof( tags->tagname ) - 1)





void    scan_line( void )
{
    filecb      *cb;
    char        *p;
    char        *pt;
    int         toklen;
    int         k;
    char        c;

    cb = file_cbs;

    scan_start = cb->scanPtr;
    scan_stop = cb->scanStop;

    while( *scan_start == ' ' ) {       // skip blanks
         scan_start++;
    }
    c = *scan_start;

 /* sp„ter nur wenn kein angefangener tag */

    /***************************************************/
    /*  skip over ".  .  .kw"  indentation             */
    /***************************************************/
    while( c == SCR_char && *(scan_start+1) == ' ' ) {

        while( *++scan_start == ' ' ) /* empty */  ;// skip blanks

        c = *scan_start;
    }
    if( c == SCR_char && *(scan_start+1) == GML_char ) {
        scan_start++;                   // .:tag construct
        c = *scan_start;                // ignore .
    }

    if( c == SCR_char ) {

    /***********************************************************/
    /*  process scr control words                              */
    /***********************************************************/

        p = scan_start + 1;

        while( *p == ' ' ) {            // skip blanks
            scan_start++;
            p++;
        }
        if( *(p-1) == SCR_char ) {
            if( *p == '*' ) {
                return;                 // +++++++++++++ ignore comment line
            }

            if( *p == SCR_char && *(p+1) == SCR_char ) {
                return;                 // +++++++++++++ ignore label line
            }

            if( *p == '\'' ) {
                p++;
                ProcFlags.CW_sep_ignore = TRUE;
            } else {
                ProcFlags.CW_sep_ignore = FALSE;
            }

            if( *p == SCR_char ) {
                p++;
                ProcFlags.macro_ignore = TRUE;
            } else {
                ProcFlags.macro_ignore = FALSE;
            }
        }
        token_buf[ 0 ] = '\0';
        pt = token_buf;
        while( isalnum( *p ) && *p != SCR_char ) {  // end of keyword
           *pt++ = *p++;                // copy to TokenBuf
        }

        *pt = '\0';
        toklen = pt - token_buf;
        if( toklen > MAX_TAGNAME_LENGTH ) {
            *(token_buf + MAX_TAGNAME_LENGTH) = '\0';
        }
        if( toklen ) {
            if( GlobalFlags.firstpass && GlobalFlags.research ) {
                printf_research("L%d    %s found %s(%d)\n", inc_level,
                               token_buf, cb->filename, cb->lineno );
                add_SCR_tag_research( token_buf );
            }
        }

        for( k = 0; k < TAGMAX; ++k ) {
            if( toklen >= tags[ k ].min_abbrev ) {
                if( !stricmp( tags[ k ].tagname, token_buf ) ) {
                    scan_start = p;
                    tags[ k ].tagproc();// tag found, process
                    break;
                }
            }
        }
    } else {

        if( c == GML_char ) {

        /*******************************************************/
        /*  process GML Tags                                   */
        /*******************************************************/
            p = scan_start +1;

        if( !strnicmp( "CMT", p, 3 ) ) {
            if( *(p + 3) == '.' || *(p + 3) == ' ' || *(p + 3) == '\0' ) {
                return;                 // ignore comment line
            }
        }

        /*******************************************************/
        /*  look for end of tagname                            */
        /*******************************************************/
        while( *p > ' ' ) {
            if( *p == '.' ) break;
            p++;
        }
        *p = '\0';
        toklen = p - scan_start;
        if( toklen > MAX_TAGNAME_LENGTH ) {
            *(scan_start + MAX_TAGNAME_LENGTH) = '\0';
        }

        if( GlobalFlags.firstpass && GlobalFlags.research ) {
            printf_research("L%d    %s found %s(%d)\n", inc_level,
                   scan_start, cb->filename, cb->lineno );
            add_GML_tag_research( scan_start );
        }

        if( !stricmp( ":include", scan_start )
            || !stricmp( ":imbed", scan_start ) ) {
            p++;
            while( *p == ' ' ) {
                p++;
            }
            if( !strnicmp( "file=", p, 5 ) ) {
                char    quote;
                char    *fnstart;

                p += 5;
                if( *p == '"' || *p == '\'' ) {
                    quote = *p;
                    ++p;
                } else {
                    quote = '.';           // error?? filename without quotes
                }
                fnstart = p;
                while( *p && *p != quote ) {
                    ++p;
                }
                *p = '\0';
                strcpy_s( token_buf, buf_size, fnstart );
                ProcFlags.newLevel = TRUE;
                return;
            }
        }
        }
    }
}

typedef enum {                          // type of constants
    BIN     = 1,
    DEC     = 3,
    HEX     = 4,
    CHAR    = 8
} selfdef_types;

typedef enum {
    condA   = 0,                        // inital state
    condB   = 1,                        // left paren, Binary + or -
    condC   = 2,                        // unary + or -
    condD   = 3,                        // * or /
    condE   = 4,                        // absolute term or right paren
} conditions;

typedef enum  {                      // internal character representation for
                                        // easier grouping of chars
   i_0      = 0,
   i_1      = 1,
   i_2      = 2,
   i_3      = 3,
   i_4      = 4,
   i_5      = 5,
   i_6      = 6,
   i_7      = 7,
   i_8      = 8,
   i_9      = 9,
   i_a      = 0x0a,
   i_b      = 0x0b,
   i_c      = 0x0c,
   i_d      = 0x0d,
   i_e      = 0x0e,
   i_f      = 0x0f,
   i_g      = 0x10,
   i_h      = 0x11,
   i_i      = 0x12,
   i_j      = 0x13,
   i_k      = 0x14,
   i_l      = 0x15,
   i_m      = 0x16,
   i_n      = 0x17,
   i_o      = 0x18,
   i_p      = 0x19,
   i_q      = 0x1a,
   i_r      = 0x1b,
   i_s      = 0x1c,
   i_t      = 0x1d,
   i_u      = 0x1e,
   i_v      = 0x1f,
   i_w      = 0x20,
   i_x      = 0x21,
   i_y      = 0x22,
   i_z      = 0x23,
   i_dollar = 0x24,                     // $
   i_pound  = 0x25,                     // #
   i_at     = 0x26,                     // @
                   // up to here alphanumeric
   i_plus   = 0x27,                     // +
   i_minus  = 0x28,                     // -
   i_mult   = 0x29,                     // *
   i_aster  = 0x29,                     // = imult
   i_divid  = 0x2a,                     // /
   i_slash  = 0x2a,                     // = idivid
   i_comma  = 0x2b,                     // ,
   i_equal  = 0x2c,                     // =
   i_ampsd  = 0x2d,                     // &
   i_priod  = 0x2e,                     // .
   i_lparn  = 0x2f,                     // (
   i_rparn  = 0x30,                     // )
   i_quote  = 0x31,                     // '
   i_dquot  = 0x32,                     // "
   i_blank  = 0x33,
   i_alpha  = 0x26                      // = iat
} ichar;

#if 0
static const UCHAR ext2int[256] = {

    /* .0     .1     .2     .3     .4     .5     .6     .7  */
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //0x
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //1x
    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    '\x33','\xFF','\x32','\x25','\x24','\xff','\x17','\x31', //2x
    '\x2f','\x30','\x29','\x27','\x2b','\x28','\x2e','\x2a',

    '\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07', //3x
    '\x08','\x09','\xff','\xff','\xff','\x2c','\xff','\xff',

    '\x26','\x0a','\x0b','\x0c','\x0e','\x0F','\x10','\x11', //4x
    '\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19',

    '\x1a','\x1b','\x1c','\x1d','\x1e','\x1F','\x20','\x21', //5x
    '\x22','\x23','\x24','\xff','\xff','\xff','\xff','\xff',

    '\xff','\xff','\xff','\xff','\xff','\xff','\xff','\xff', //6x
    '\xFF','\xFF','\xff','\xff','\xff','\xff','\xff','\xff',

    '\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF', //7x
    '\xFF','\xff','\xff','\xff','\xff','\xff','\xff','\xff',

    /* .0     .1     .2     .3     .4     .5     .6     .7  */
    '\xFF','\x61','\x62','\x63','\x64','\x65','\x66','\x67',
    '\x68','\x69','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',

    '\xFF','\x6A','\x6B','\x6C','\x6D','\x6E','\x6F','\x70',
    '\x71','\x72','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',

    '\xFF','\xe1','\x73','\x74','\x75','\x76','\x77','\x78',
    '\x79','\x7A','\xFF','\xFF','\xFF','\x5B','\xFF','\xFF',

    '\xFF','\xFF','\xFF','\xFF','\xFF','\x40','\xFF','\xFF',
    '\xFF','\xFF','\xFF','\x7C','\xFF','\x5D','\xFF','\xFF',

    '\x84','\x41','\x42','\x43','\x44','\x45','\x46','\x47',
    '\x48','\x49','\xFF','\xFF','\xDD','\xFF','\xFF','\xFF',

    '\x81','\x4A','\x4B','\x4C','\x4D','\x4E','\x4F','\x50',
    '\x51','\x52','\xFF','\xFF','\x7D','\xFF','\xFF','\xFF',

    '\x99','\xFF','\x53','\x54','\x55','\x56','\x57','\x58',
    '\x59','\x5A','\xFF','\xFF','\x5C','\xFF','\xFF','\xFF',

    '\x30','\x31','\x32','\x33','\x34','\x35','\x36','\x37',
    '\x38','\x39','\xFF','\xFF','\x5D','\xFF','\xFF','\xFF'
        };
#endif




static  ichar ext_i( char c )
{
    switch( c ) {
    case   '0':  return( i_0 ); break;
    case   '1':  return( i_1 ); break;
    case   '2':  return( i_2 ); break;
    case   '3':  return( i_3 ); break;
    case   '4':  return( i_4 ); break;
    case   '5':  return( i_5 ); break;
    case   '6':  return( i_6 ); break;
    case   '7':  return( i_7 ); break;
    case   '8':  return( i_8 ); break;
    case   '9':  return( i_9 ); break;
    case   'A':  return( i_a ); break;
    case   'B':  return( i_b ); break;
    case   'C':  return( i_c ); break;
    case   'D':  return( i_d ); break;
    case   'E':  return( i_e ); break;
    case   'F':  return( i_f ); break;
    case   'G':  return( i_g ); break;
    case   'H':  return( i_h ); break;
    case   'I':  return( i_i ); break;
    case   'J':  return( i_j ); break;
    case   'K':  return( i_k ); break;
    case   'L':  return( i_l ); break;
    case   'M':  return( i_m ); break;
    case   'N':  return( i_n ); break;
    case   'O':  return( i_o ); break;
    case   'P':  return( i_p ); break;
    case   'Q':  return( i_q ); break;
    case   'R':  return( i_r ); break;
    case   'S':  return( i_s ); break;
    case   'T':  return( i_t ); break;
    case   'U':  return( i_u ); break;
    case   'V':  return( i_v ); break;
    case   'W':  return( i_w ); break;
    case   'X':  return( i_x ); break;
    case   'Y':  return( i_y ); break;
    case   'Z':  return( i_z ); break;

    case   '$':  return( i_dollar ); break;
    case   '#':  return( i_pound ); break;
    case   '@':  return( i_at ); break;
    case   '+':  return( i_plus ); break;
    case   '-':  return( i_minus ); break;
    case   '*':  return( i_aster ); break;
    case   '/':  return( i_slash ); break;
    case   ',':  return( i_comma ); break;
    case   '=':  return( i_equal ); break;
    case   '&':  return( i_ampsd ); break;
    case   '.':  return( i_priod ); break;
    case   '(':  return( i_lparn ); break;
    case   ')':  return( i_rparn ); break;
    case   '\'':  return( i_quote ); break;
    case   '"':  return( i_dquot ); break;
    case   ' ':  return( i_blank ); break;
    default:
        return( c );
        break;
    }
}
















typedef enum {
    opend       = 0,
    opplus,
    opminus,
    opuplus,
    opuminus,
    opstar,
    opslash
} oper_hier;


#define MAXTERMS    25
#define MAXPARENS   10
#define MAXOPERS    MAXTERMS * 2
static struct  {
    long        terms[ MAXTERMS ];
    int16_t     rlist[ MAXTERMS ];
    int8_t      ntrms[ MAXTERMS ];
    oper_hier   oprns[ MAXOPERS ];

    int         currterm;
    int         currrlist;
    int         currntrms;
    int         curroper;

    int         currparn;

    conditions  cond;
    int         endoe;
    int         newop;
} termswk;


static int      charcnt;
static int      charcntmax;
static long     result;
static char     result_c[ 5 ];


/***************************************************************************/
/*  logic from cbt282.122                                                  */
/*  self defining value convert                                            */
/***************************************************************************/

static  char *sdvcf( selfdef_types typ, int bitcnt, char *s, char *stop )
{
        long    res;
        char    c;
        ichar   ic;
static  int     sdv08[ 8 ] = { 1, 0, 9, 15, 0,0,0, 255 };
        char    resc[ 5 ] = "    ";

    res = 0;
    charcnt = 0;
    sdv08[ 1 ] = typ;

    for( ;; ) {
        if( s > stop ) {
            break;
        }
        c = *s;
        ic = ext_i( c );
        if( typ == DEC || typ == BIN ) {
            if( !isdigit( c ) ) {
                break;
            }
            if( ic > sdv08[ typ - 1 ] ) {
                break;
            }
        } else {
            if( typ == HEX ) {
                if( ic > i_f ) {
                    break;              // no hex digit
                }
            }
        }

        if( typ == CHAR ) {
            if( c == '\'') {
                if( *(s+1) == '\'' ) {
                    s++;
                    continue;
                } else {
                    break;
                }
            }
            resc[ 0 ] = resc[ 1 ];
            resc[ 1 ] = resc[ 2 ];
            resc[ 2 ] = resc[ 3 ];
            resc[ 3 ] = *s;
        } else {
            if( typ == DEC ) {
                if( res >= INT_MAX / 10 ) {
                    charcnt = 10;       // overflow
                } else {
                    res = res * 10 + ic;
                    if( res > INT_MAX - 1 ) {
                        charcnt = 10;
                    }
                }
            } else {
                res <<= bitcnt;
                res += ic;
            }
        }
        charcnt++;
        s++;
    }
    if( typ == CHAR ) {
        strcpy_s( result_c, sizeof( result_c ), resc );
    } else {
        result  = res;
    }
    return( s );
}

char    num_sign = ' ';                 // unary operator

/***************************************************************************/
/*  logic from cbt282.122                                                  */
/***************************************************************************/

condcode getnum( getnum_block *gn )
{
    char    *a;                         // arg start
    char    *z;                         // arg stop
    char    c;
    ichar   ic;



    memset( &termswk, sizeof( termswk ), '\0' );
/*
    termswk.currterm = 0;
    termswk.currparn = 0;
    termswk.curroper = 0;
    termswk.currrlist = 0;
*/
    a = gn->argstart;
    z = gn->argstop;
    while( a < z && *a == ' ' ) {
        a++;                            // skip leading blanks
    }
    gn->errstart = a;
    gn->first    = a;
    if( a > z ) {
        gn->cc = omit;
        return( omit );                 // nothing there
    }
    c = *a;
    ic = ext_i( c );                    // to internal representation
    if( c == '+' || c == '-' ) {
        num_sign = c;                   // unary sign
    }

//look:
    for( ;; ) {
        if( a > z ) {
            break;
        }

    /***********************************************************************/
    /*  test/expand  pageno symbol ????                                    */
    /***********************************************************************/

//look02:
        if( ic <= i_alpha ) {
            gn->errstart = a;
            if( isdigit( c ) ) {
                a = sdvcf( DEC, 8, a, z );
                termswk.terms[ termswk.currterm ] = result;
                charcntmax = 8;
            } else {
                if( *(a+1) != '\'' ) {
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                }
//nextq:
//first char alphabetic , 2nd is quote
                c = tolower( c );
                ic = ext_i( c );
                switch ( c ) {
                case 'b' :                  // binary selfdefining value
                    a += 2;
                    a = sdvcf( BIN, 32, a, z );
                    termswk.terms[ termswk.currterm ] = result;
                    charcntmax = 32;
                    break;
                case 'c' :                  // char self defining value
                    a += 2;
                    a = sdvcf( CHAR, 4, a, z );
                    termswk.terms[ termswk.currterm ] = (long) result_c;
                    charcntmax = 4;
                    break;
                case 'x' :                  // hex self defining value
                    a += 2;
                    a = sdvcf( HEX, 8, a, z );
                    termswk.terms[ termswk.currterm ] = result;
                    charcntmax = 8;
                    break;
                default:                    // invalid
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                    break;
                }
//ckq:

                if( *a != '\'' ) {          // closing quote?
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                } else {
                    a++;
                }
                if( charcnt <= 0 ) {        // charcnt is set in sdvcf()
                    gn->cc = notnum;
                    gn->error = illchar;
                    return( notnum );
                }
//absd:
                if( charcnt > charcntmax ) {// actual length > maximum
                    gn->cc = notnum;
                    gn->error = selfdef;
                    return( notnum );
                }
                if( termswk.cond > condA ) {
//absck:
                    if( termswk.cond >= condE ) {   // 2 terms in a row
                        gn->cc = notnum;
                        gn->error = ilorder;
                        return( notnum );
                    }
                }
//compt4:
                termswk.cond = condE;       // set absolute term
//  goto compt;

            }
        } else {

//notam:    first char not alfanumeric

            if( ic < i_comma ) {

//ltcom:    first char is one of + - * /
                if( ic >= i_aster ) {       // mult / div
                    if( termswk.cond < condE ) {
                        gn->cc = notnum;
                        gn->error = operr;
                        return( notnum );
                    }
                    termswk.cond = condD;   // set mult / div condition
                    termswk.newop = ic - (i_alpha - 2); // set hierarchy code
                } else {                    // leaves + or -
                    if( termswk.cond == condE ) {
//plmin3:
                        termswk.cond = condB;   // binary + or -
                        termswk.newop = ic - i_alpha;   // set hierarchy code

                    } else {
                        if( termswk.cond != condA ) {
                            termswk.cond = condC;   // unary + or -
//plmin2:
                            termswk.newop = ic - (i_alpha - 2);// set hierarchy code
                        } else {
                            termswk.cond = condE;

                            goto compt; // +++++++++++++++++++++++++++++++++
                        }
                    }
                }
//loop:
                if( termswk.curroper == 0 ) {
                    if( termswk.endoe ) {

                        break;
                    }
//put:
                    termswk.oprns[ termswk.curroper ] = termswk.newop;
                    termswk.curroper++;
                    a++;
                    continue;
                } else {
                    if( termswk.oprns[ termswk.curroper - 1 ] >= termswk.newop ) {
//le:
                        if( termswk.newop == opuminus || termswk.newop == opuplus) {
                            termswk.oprns[ termswk.curroper ] = termswk.newop;
                            termswk.curroper++;
                            a++;
                            continue;
                        } else {
                            if( termswk.oprns[ termswk.curroper - 1 ] ) {
//work:
                      //          int     ncomp = termswk.currrlist;
                        //        long    *firstx1 = &termswk.terms[ termswk.currterm -2 ];



                                switch ( termswk.oprns[ termswk.curroper ] ) {
                                case opplus :
                                    break;
                                case opminus :
                                    break;
                                case opuplus :
                                    break;
                                case  opuminus:
                                    break;
                                case opstar :
                                case opslash :
                                    break;
                                default:
                                    break;
                                }


                  //              goto work;


                            } else {
                                termswk.currparn--;

                                termswk.curroper--;

                                a++;
                                continue;
                            }
                        }
                    } else {
                        if( opslash == termswk.newop &&
                            termswk.oprns[ termswk.curroper - 1 ] == opstar ) {

                    //        goto work;  // +++++++++++++++++++++

                        }
                        termswk.oprns[ termswk.curroper ] = termswk.newop;
                        termswk.curroper++;
                        a++;
                        continue;
                    }
                }
            } else {                        // not less i_comma
                if( ic == i_lparn ) {
                    if( termswk.cond < condE ) {
                        termswk.cond = condB;   // left paren cond
                        if( termswk.currparn >= MAXPARENS - 1 ) {
                            gn->cc = notnum;
                            gn->error = parerr;
                            return( notnum );
                        }
                        termswk.currparn++;
                        termswk.oprns[ termswk.curroper ] = 0;
                        termswk.curroper++;
                        a++;
                    } else {
//lpend:
                    }
                } else {
                    if( ic == i_rparn ) {
//rpar:
                        if( termswk.cond < condE ) {// right paren following term?
                            gn->cc = notnum;
                            gn->error = operr;
                            return( notnum );
                        }
                        termswk.cond = condE;   // right paren cond
                        if( termswk.currparn == 0 ) {   // no paren open?
                            gn->cc = notnum;
                            gn->error = illchar;
                            return( notnum );
                        }
//noend:
                        termswk.newop = opend;

//   b loop:

                    } else {
                        if( ic == i_blank ) {
                            if( gn->ignore_blanks == 0 ) {
                                if( termswk.cond < condE || termswk.currparn > 0) {
                                    gn->cc = notnum;// no term prceceding
                                    gn->error = enderr;
                                    return( notnum );
                                }
//endng:
                                termswk.endoe = TRUE;   // end of expression
//noend:
                                termswk.newop = opend;

//   b loop:


                            } else {
                                a++;    // skip over blank
                                continue;
                            }
                        } else {
                            gn->cc = notnum;
                            gn->error = illchar;
                            return( notnum );
                        }
                    }
                }
            }
        }
compt:
        if( termswk.currterm >= MAXTERMS - 1 ) {
            gn->cc = notnum;
            gn->error = mnyerr;         // too many terms
            return( notnum );
        }
        termswk.ntrms[ termswk.currterm ] = 2;
        termswk.currterm++;
        termswk.currrlist++;
    }                                   // for(;;)


    return( 0 );
}
