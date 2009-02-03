
/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML support functions  multi letter functions
*               non functional so far
*                    &'upper(
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*    SCR multi letter functions                                           */
/***************************************************************************/

#define pick(name, length, parms, optparms, routine) { name, length, parms, optparms, routine },

static  const   scrfunc scr_functions[] = {

#include "gsfuncs.h"

    { " ", 0, 0, 0, NULL }              // end
};

#define SCR_FUNC_MAX (sizeof( scr_functions ) / sizeof( scr_functions[ 0 ] ) - 1)

#undef pick

#define max_fun_parms   7             // max parmcount found in documentation

typedef struct parm {
    char    *       a;
    char    *       e;
} parm;

static  parm    parms[ max_fun_parms ];


char    *scr_multi_funcs( char * in, char * end, char * * result )
{
    char            *   pchar;
    int                 rc;
    char            *   pval;
    char            *   pfunc;
    size_t              fnlen;
    int                 funcind;
    int                 k;
    int                 m;
    int                 paren_level;
    int                 paren_level_start;
    char                fn[ FUN_NAME_LENGTH + 1 ];
    bool                found;


    end   = end;
    rc = 0;
    pval = end;
    pfunc = end;

    fnlen = 0;
    pchar = in + 2;                     // over &'

    // collect function name
    while( *pchar && pchar <= end && test_function_char( *pchar ) ) {
        fn[ fnlen ] = *pchar++;
        if( fnlen <= FUN_NAME_LENGTH ) {
            fnlen++;
        } else {
            break;
        }
    }
    fn[ fnlen ] = '\0';

    if( *pchar != '(' ) {               // open paren does not follow
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION parms missing no ( found\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION parms missing no ( found\n"
                     "\t\t\tLine %d of file '%s'\n",
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }

    // test if functionname is valid
    found = false;
    for( k = 0; k < SCR_FUNC_MAX; k++ ) {
        if( fnlen == scr_functions[ k ].length
            && !stricmp( fn, scr_functions[ k ].fname ) ) {

            found = true;
            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                out_msg( " Function %s found\n", scr_functions[ k ].fname );
            }
            break;
        }
    }
    if( !found ) {
        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }
    funcind = k;

    // collect the mandatory parm(s)
    paren_level_start = 0;

    for( k = 0; k < scr_functions[ funcind ].parm_cnt; k++ ) {
        parms[ k ].a = ++pchar;         // first over ( then over ,
        paren_level = paren_level_start;

        while( (paren_level > paren_level_start) ||
                ((*pchar != ',') && (*pchar != ')')) ) {
            if( pchar > end ) {
                break;
            }
            paren_level += (*pchar == '(');
            paren_level -= (*pchar == ')');
            pchar++;
        }
        parms[ k ].e = pchar - 1;
#if 0
        if( parms[ k ].e < parms[ k ].a ) {
            parms[ k ].e++;             // omitted parm  set zero length
        }
#endif
        if( *pchar == ')' ) {
            break;                      // end of parms
        }
    }
    m = k;                              // mandatory parm count

    if( m < scr_functions[ funcind ].parm_cnt ) {
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION %d parms missing\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     scr_functions[ funcind ].parm_cnt - m,
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION %d parms missing\n"
                     "\t\t\tLine %d of file '%s'\n",
                     scr_functions[ funcind ].parm_cnt - m,
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }



    // collect the optional parm(s)
    if( *pchar == ')' ) {               // no optional parms
        k = 0;
    } else {
        for( k = 0; k < scr_functions[ funcind ].opt_parm_cnt; k++ ) {
            parms[ m + k ].a = ++pchar;
            paren_level = paren_level_start;

            while( (paren_level > paren_level_start) ||
                    ((*pchar != ',') && (*pchar != ')')) ) {
                if( pchar > end ) {
                    break;
                }
                paren_level += (*pchar == '(');
                paren_level -= (*pchar == ')');
                pchar++;
            }
            parms[ m + k ].e = pchar - 1;
#if 0
            if( parms[ m + k ].e < parms[ m + k ].a ) {
                parms[ m + k ].e++;     // omitted parm  set zero length
            }
#endif
            if( *pchar == ')' ) {
                break;                  // end of parms
            }
        }
    }
    m += k;                             // total parmcount
    parms[ m + 1 ].a = NULL;            // end of parms indicator

    if( *pchar != ')' ) {
        if( input_cbs->fmflags & II_macro ) {
            out_msg( "ERR_FUNCTION missing closing paren\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     input_cbs->s.m->lineno,
                     input_cbs->s.m->mac->name );
        } else {
            out_msg( "ERR_FUNCTION missing closing paren\n"
                     "\t\t\tLine %d of file '%s'\n",
                     input_cbs->s.f->lineno,
                     input_cbs->s.f->filename );
        }
        err_count++;
        show_include_stack();

        **result = '&';                 // result is & to preserve the input
        *result += 1;
        **result = '\0';
        return( in + 1 );               // but avoid endless loop
    }

    return( pchar + 1 );
}

/***************************************************************************/
/*  script string function &'upper(                                        */
/*                                                                         */
/***************************************************************************/


/***************************************************************************/
/*                                                                         */
/* &'upper(string<,n<,length>>):   The  Uppercase  function  returns  the  */
/*    uppercase equivalent of the 'string' operand.   The first character  */
/*    to be uppercased may be specified  by 'n' and the 'length' defaults  */
/*    to the end of the string.    The conversion to uppercase is defined  */
/*    by the Translate Uppercase (.TU) control word.                       */
/*      &'upper(abc) ==> ABC                                               */
/*      &'upper('Now is the time') ==> NOW IS THE TIME                     */
/*      abc&'upper(time flies)xyz ==> abcTIME FLIESxyz                     */
/*      &'upper(abc)...&'upper(xyz) ==> ABC...XYZ                          */
/*      &'upper(abcdefg,3) ==> abCDEFG                                     */
/*      &'upper(abcdefg,3,2) ==> abCDefg                                   */
/*      &'upper(one,two,three) ==> invalid operands                        */
/*                                                                         */
/***************************************************************************/

char    *scr_upper( char * in, const char * end, char * * result,
                    const int parmcount )
{
    char            *   pchar;
    sub_index           var_ind;
    symvar              symvar_entry;
    symsub          *   symsubval;
    int                 rc;
    char            *   pval;

    end   = end;


    pchar = scan_sym( in + 3, &symvar_entry, &var_ind );

    if( symvar_entry.flags & local_var ) {  // lookup var in dict
        rc = find_symvar( &input_cbs->local_dict, symvar_entry.name,
                          var_ind, &symsubval );
    } else {
        rc = find_symvar( &global_dict, symvar_entry.name, var_ind,
                          &symsubval );
    }
    if( rc == 2 ) {
        pval = symsubval->value;        // upper content of variable
    } else {
        pval =  symvar_entry.name;      // upper variable name
    }
    while( *pval ) {
        **result = toupper( *pval++ );
        *result += 1;
    }
    **result = '\0';

    if( *pchar == '.' ) {
        pchar++;                        // skip optional terminating dot
    }
    return( pchar );
}



#if 0
/***************************************************************************/
/*  script functions                                                       */
/*                                                                         */
/***************************************************************************/

char    *scr_single_funcs( char * in, char * end, char * * result )
{
    char            *   pw;

    if( *(in + 2) == '\'' ) {
        switch( *(in + 1) ) {
        case  'e' :             // exist function
            pw = scr_single_func_e( in, end, result );
            break;
        case  'l' :             // length function
            pw = scr_single_func_l( in, end, result );
            break;
        case  's' :             // subscript
        case  'S' :             // superscript
            if( input_cbs->fmflags & II_macro ) {
                out_msg( "WNG_FUNCTION not yet implemented &%c'\n"
                         "\t\t\tLine %d of macro '%s'\n", *(in + 1),
                         input_cbs->s.m->lineno,
                         input_cbs->s.m->mac->name );
            } else {
                out_msg( "WNG_FUNCTION not yet implemented &%c'\n"
                         "\t\t\tLine %d of file '%s'\n", *(in + 1),
                         input_cbs->s.f->lineno,
                         input_cbs->s.f->filename );
            }
            wng_count++;

            // fallthrough treat as upper for now       TDB

        case  'u' :             // upper function
            pw = scr_single_func_u( in, end, result );
            break;
        default:
            pw = scr_single_func_unsupport( in, result );
            wng_count++;
            break;
        }
    } else {
        out_msg( "ERR_Logic error in gsfunelu.c\n" );
        err_count++;
        g_suicide();
    }
    return( pw );
}



+++++++++++++++++++++++++++++++  used functions in OW documentation +++++++++
used single letter functions: others are unused
&e'  exist 0 or 1
&l'  length, if undefined length of name
&s'  subscript   low
&S'  superscript high
&u'  upper

//pick("delstr",        6,  2,  1,  scr_delstr )
//pick("d2c",           3,  1,  1,  scr_d2c    )
//pick("index",         5,  2,  1,  scr_index  )

used other functions:

&'delstr(
 &'delstr(string,n<,length>):  The  Delete String function  deletes the
    part of 'string' starting at character  number 'n'.   The number of
    characters can be specified in 'length' and defaults from character
    'n' to the end of the string.  If 'string' is less than 'n' charac-
    ters long, then nothing is deleted.
      &'delstr('abcdef',3) ==> ab
      &'delstr('abcdef',3,2) ==> abef
      &'delstr('abcdef',10) ==> abcdef
&'d2c(
 &'d2c(number<,n>):  To  convert a  decimal 'number'  to its  character
    representation of length 'n'.  The 'number' can be negative only if
    the length 'n' is specified.
      "&'d2c(129)" ==> "a"
      "&'d2c(129,1)" ==> "a"
      "&'d2c(129,2)" ==> " a"
      "&'d2c(-127,1)" ==> "a"
      "&'d2c(-127,2)" ==> "ÿa"
      "&'d2c(12,0)" ==> ""
&'index(
 &'index(haystack,needle<,start>):   The  Index  function  returns  the
    character position of the string 'needle' in the string 'haystack'.
    If not found, the function returns zero.  The first character posi-
    tion to be searched in 'haystack' may be specified with the 'start'
    number and  it defaults to the  first character position  in 'hays-
    tack'.
      &'index('abcdef','c') ==> 3
      &'index('abcdef','cd') ==> 3
      &'index('abcdef','yz') ==> 0
      &'index('ab','abcdef') ==> 0
      &'index('ababab','ab',2) ==> 3
      &'index('ababab','ab',6) ==> 0
&'insert(
 &'insert(new,target<,<n><,<length><,pad>>>):   To  Insert   the  'new'
    string with  length 'length' into  the 'target' string  after char-
    acter 'n'.   If 'n' is omitted then  'new' is inserted at the start
    of 'target'.   The 'pad' character may  be used to extend the 'tar-
    get' string to length 'n' or the 'new' string to length 'length'.
      "&'insert(' ','abcdef',3)" ==> "abc def"
      "&'insert('123','abc',5,6)" ==> "abc  123   "
      "&'insert('123','abc',5,6,'+')" ==> "abc++123+++"
      "&'insert('123','abc')" ==> "123abc"
      "&'insert('123','abc',5,,'-')" ==> "abc--123"
      "&'insert('123','abc',,,'-')" ==> "123abc"
&'left(
 &'left(string,length<,pad>):   To  generate  a   character  string  of
    'length' characters  with 'string' in  its start.   If  'length' is
    longer than 'string' then 'pad' the result.
      &'left('ABC D',8) ==> "ABC D   "
      &'left('ABC D',8,'.') ==> "ABC D..."
      &'left('ABC  DEF',7) ==> "ABC  DE"
&'length(
 &'length(string):  The Length function returns  the length of 'string'
    in characters.   The  'string' may be null,  in which  case zero is
    returned.
      &'length(abc) ==> 3
      &'length('Time Flies') ==> 10
      abc&'length(time flies)xyz ==> abc10xyz
      +&'length(one)*&'length(two) ==> +3*3
      &'length(one,two,three) ==> too many operands
&'lower(
 &'lower(string<,n<,length>>):   The  Lowercase  function  returns  the
    lowercase equivalent of the 'string' operand.   The first character
    to be lowercased may be specified  by 'n' and the 'length' defaults
    to the  end of the string.    The conversion to  lowercase includes
    only the alphabetic characters.
      &'lower(ABC) ==> abc
      &'lower('Now is the time') ==> now is the time
      ABC&'lower(TIME FLIES)890 ==> ABCtime flies890
      &'lower(ABC)...&'lower(890) ==> abc...890
      &'lower(ABCDEFG,3) ==> ABcdefg
      &'lower(ABCDEFG,3,2) ==> ABcdEFG
      &'lower(ONE,TWO,THREE) ==> invalid operands
&'min(
 &'length(string):  The Length function returns  the length of 'string'
    in characters.   The  'string' may be null,  in which  case zero is
    returned.
      &'length(abc) ==> 3
      &'length('Time Flies') ==> 10
      abc&'length(time flies)xyz ==> abc10xyz
      +&'length(one)*&'length(two) ==> +3*3
      &'length(one,two,three) ==> too many operands
&'pos(
 &'pos(needle,haystack<,start>):   The  Position function  returns  the
    character position  of the first  occurrence of 'needle'  in 'hays-
    tack'.   The  search for a  match starts  at the first  position of
    'haystack' but  may be  overridden by  adding a  'start' column  in
    'haystack'.    If the  'needle' string  is  not found,   a zero  is
    returned.
      &'pos('c','abcde') ==> 3
      &'pos(x,abcde) ==> 0
      &'pos(abcde,abcde) ==> 1
      &'pos(a,aaaaa) ==> 1
      &'pos('a','aaaaa') ==> 1
      &'pos(a,aaaaa,3) ==> 3
      &'pos(12345678,abc) ==> 0
      &'pos(a) ==> error, too few operands
      &'pos(a,abcd,junk) ==> error, 'start' not numeric
      &'pos(a,abcd,3,'.') ==> error, too many operands
&'right(
 &'right(string,length<,pad>):   To  generate  a  character  string  of
    'length'  characters with  'string' at  the end.    If 'length'  is
    longer than 'string' then 'pad' the  result,  if less then truncate
    on the left.
      &'right('ABC D',8) ==> "   ABC D"
      &'right('ABC D',8,'.') ==> "...ABC D"
      &'right('ABC  DEF',7) ==> "BC  DEF"
&'strip(
 &'strip(string<,<type><,char>>):   To remove  leading and/or  trailing
    'char's from a 'string'.  The 'type' specified may be 'L' to remove
    leading characters,  'T' for trailing  characters,  or 'B' for both
    leading and trailing characters.   If omitted the default 'type' is
    'B'.   All leading  or trailing characters matching  'char' will be
    removed, the default 'char' being a blank.
      "&'strip('  the dog  ')" ==> "the dog"
      "&'strip('  the dog  ','L')" ==> "the dog  "
      "&'strip('  the dog  ','t')" ==> "  the dog"
      "&'strip('a-b--',,'-')" ==> "a-b"
      "&'strip(--a-b--,,-)" ==> "a-b"
&'substr(
 &'substr(string,n<,length<,pad>>):  The Substring function returns the
    portion of 'string' starting at  character number 'n'.   The number
    of  characters  to return  may  be  specified  in 'length'  and  it
    defaults from  character 'n' to the  end of the 'string'.    If the
    'length' is  present and it  would exceed  the total length  of the
    'string' then  string is  extended with  blanks or  the user  'pad'
    character.
      "&'substr('123456789*',5)" ==> "56789*"
      "&'substr('123456789*',5,9)" ==> "56789*   "
      "&'substr('123456789*',5,9,'.')" ==> "56789*..."
      "&'substr('123456789*',1,3,':')" ==> "123"
      .se alpha = 'abcdefghijklmnopqrstuvwxyz'
      "&'substr(&alpha,24)" ==> "xyz"
      "&'substr(&alpha,24,1)" ==> "x"
      "&'substr(&alpha,24,5)" ==> "xyz  "
      "&'substr(&alpha,24,5,':')" ==> "xyz::"
      "&'substr(&alpha,30,5,':')" ==> ":::::"
      "&'substr(abcde,0,5)" ==> start column too small
      "&'substr(abcde,1,-1)" ==> length too small
&'subword(
 &'subword(string,n<,length>):  The Subword function  returns the words
    of 'string' starting at word number 'n'.   The value of 'n' must be
    positive.  If 'length' is omitted, it will default to the remainder
    of the string.  The result will include all blanks between selected
    words and all leading and trailing blanks are not included.
      &'subword('The quick brown  fox',2,2) ==> "quick brown"
      &'subword('The quick brown  fox',3) ==> "brown  fox"
      &'subword('The quick brown  fox',5) ==> ""
      &'subword('The quick brown  fox',0) ==> error, number too small
      &'subword('The quick brown  fox') ==> error, missing number
      &'subword('',1) ==> ""
&'translate(
 &'translate(string<,<tableo><,<tablei><,pad>>>):      To     Translate
    'string' with characters  in 'tablei' to the  corresponding charac-
    ters in  'tableo'.   The  'pad' character  will extend  'tableo' to
    equal the length of 'tablei',  if  required.   If both 'tableo' and
    'tablei' are omitted then 'string' is converted to uppercase.
      &'translate('abcdef') ==> ABCDEF
      &'translate('abbc','&','b') ==> a&&c
      &'translate('abcdef','12','ec') ==> ab2d1f
      &'translate('abcdef','12','abcd','.') ==> 12..ef
      &'translate('4321','abcd','1234') ==> dcba
      &'translate('123abc',,,'$') ==> $$$$$$
&'upper(
 &'upper(string<,n<,length>>):   The  Uppercase  function  returns  the
    uppercase equivalent of the 'string' operand.   The first character
    to be uppercased may be specified  by 'n' and the 'length' defaults
    to the end of the string.    The conversion to uppercase is defined
    by the Translate Uppercase (.TU) control word.
      &'upper(abc) ==> ABC
      &'upper('Now is the time') ==> NOW IS THE TIME
      abc&'upper(time flies)xyz ==> abcTIME FLIESxyz
      &'upper(abc)...&'upper(xyz) ==> ABC...XYZ
      &'upper(abcdefg,3) ==> abCDEFG
      &'upper(abcdefg,3,2) ==> abCDefg
      &'upper(one,two,three) ==> invalid operands
&'veclastpos(
 &'veclastpos(needle,haystack<,<start><,case>>):  The Vector Last Posi-
    tion function returns  the subscript number of  the last occurrence
    of 'needle'  in 'haystack'  set symbol.    The search  for a  match
    starts at the first element of  'haystack' but may be overridden by
    adding a 'start' subscript in 'haystack'.   The default case of 'M'
    or 'Mixed' searches for an element which matches exactly, a case of
    'U' or  'Upper' will match the  'needle' and 'haystack'  element in
    uppercase.    If the  'needle'  string is  not  found,   a zero  is
    returned.
&'vecpos(
 &'vecpos(needle,haystack<,<start><,case>>):  The Vector Position func-
    tion returns the subscript number of  the first occurrence of 'nee-
    dle' in 'haystack'  set symbol.   The search for a  match starts at
    the first element  of 'haystack' but may be overridden  by adding a
    'start'  subscript in  'haystack'.    The default  case  of 'M'  or
    'Mixed' searches for  an element which matches exactly,   a case of
    'U' or  'Upper' will match the  'needle' and 'haystack'  element in
    uppercase.    If the  'needle'  string is  not  found,   a zero  is
    returned.
      .se rodent(1) = 'Gerbil'
      .se rodent(2) = 'Hamster'
      .se rodent(3) = 'Mouse'
      .se rodent(4) = 'Rabbit'
      &'vecpos('Hamster',rodent) ==> 2
      &'vecpos(Dog,rodent) ==> 0
      &'vecpos(Hamster,rodent,3) ==> 0
      &'vecpos(HAMSTER,rodent) ==> 0
      &'vecpos(HAMSTER,rodent,1,U) ==> 2
      &'vecpos(Mouse) ==> error, too few operands
&'word(
 &'word(string,n):  The  Word function returns  only the 'n'th  word in
    'string'.   The value of 'n' must be positive.   If there are fewer
    than 'n' blank delimited words in the 'string' then the null string
    is returned.
      &'word('The quick brown fox',3) ==> "brown"
      &'word('The quick brown fox',5) ==> ""
      &'word('The quick brown fox',0) ==> error, too small
      &'word('The quick brown fox') ==> error, missing number
      &'word('',1) ==> ""
&'wordpos(
 &'wordpos(phrase,string<,start>):  The Word  Position function returns
    the word  position of  the words  in 'phrase'  within the  words of
    'string'.   The  search begins with the  first word of  'string' by
    default which may be changed by  specifying a positive 'start' word
    number.   All interword blanks are treated  as a single blank.   If
    the 'phrase' cannot be found the result is zero.
      &'wordpos('quick brown fox','The quick brown fox') ==> 2
      &'wordpos('quick    brown','The quick  brown fox') ==> 2
      &'wordpos('quick  fox ','The quick  brown fox') ==> 0
      &'wordpos('xyz','The quick  brown fox') ==> 0
      &'wordpos('The quick brown fox') ==> error, missing string
&'words(
 &'words(string):  The Words function returns the number of words found
    in 'string'.
      &'words('The quick brown fox') ==> 4
      &'words(' ') ==> 0
      &'words('') ==> 0
      &'words('cat dot',1) ==> too many operands
#endif
