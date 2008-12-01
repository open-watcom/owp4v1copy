/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
*               TBD restructure
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

inputcb *   cb;

/***************************************************************************/
/*  .im   processing  IMBED                                                */
/*  format .im filename                                                    */
/***************************************************************************/

static  void    scr_im( void )
{
    char        *   fnstart;
    char        *   p;
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

    ProcFlags.newLevelFile = 1;
    line_from = LINEFROM_DEFAULT;
    line_to   = LINETO_DEFAULT;


    return;
}

/***************************************************************************/
/*  .ap processing APPEND                                                  */
/*  format  .ap filename                                                   */
/***************************************************************************/

static  void    scr_ap( void )
{
    input_cbs->s.f->flags |= FF_eof;    // simulate EOF for .append
    input_cbs->fmflags   |= II_eof;     // simulate EOF for .append
    scr_im();                           // do .imbed processing
}


static  void    free_mac_lines( inp_line * line )
{
    inp_line    *wk;
    inp_line    *wk1;

    wk = line;
    while( wk != NULL ) {
         wk1 = wk->next;
         mem_free( wk );
         wk = wk1;
    }
    return;
}

/* .dm processing define macro
 *
 *
 *
 */
static  void    scr_dm( void )
{
    char        *   nmstart;
    char        *   p;
    char        *   pn;
    char            save;
    int             len;
    int             compbegin;
    int             compend;
    char            macname[ MAC_NAME_LENGTH + 1 ];
    inp_line    *   head;
    inp_line    *   last;
    inp_line    *   work;
    ulong           lineno_start;
    condcode        cc;

    cb = input_cbs;

    garginit();

    cc = getarg();

    if( cc == omit ) {
        err_count++;
        out_msg("ERR_MACRO_NAME_MISSING line %d of file '%s'\n",
                cb->s.f->lineno,
                cb->s.f->filename );
        return;
    }

    p = err_start;

    pn      = macname;
    len     = 0;

    /*  truncate name if too long WITHOUT error msg
     *  this is wgml 4.0 behaviour
     *
     */
    while( *p && *p != ' ' ) {
        if( len < MAC_NAME_LENGTH ) {
            *pn++ = *p++;               // copy macroname
            *pn   = '\0';
        } else {
            break;
        }
        len++;
    }
    macname[ MAC_NAME_LENGTH ] = '\0';

    cc = getarg();
    if( cc == omit ) {                  // nothing found
        err_count++;
        // SC--048 A control word parameter is missing
        out_msg("ERR_MACRO_DEFINITON '%s'"
                " expecting BEGIN END /macro/lines/\n"
                "\t\t\tLine %d of file '%s'\n",
                macname,
                cb->s.f->lineno,
                cb->s.f->filename );
        return;
    }

    p = arg_start;
    head = NULL;
    last = NULL;
    save = *p;
    *p   = '\0';

    compend   = !stricmp( err_start, "end" );
    compbegin = !stricmp( err_start, "begin" );
    if( !(compbegin | compend) ) { // only .dm macname /line1/line2/ possible
        char    sepchar;

        if( ProcFlags.in_macro_define ) {
            err_count++;
            out_msg("ERR_NESTED_MACRO_DEFINE '%s' expecting END\n"
                    "\t\t\tline %d of file '%s'\n",
                    err_start,
                    cb->s.f->lineno,
                    cb->s.f->filename );
            return;
        }
        ProcFlags.in_macro_define = 1;

        *p   = save;
        lineno_start = cb->s.f->lineno;


        p = err_start;
        sepchar = *p++;
        nmstart = p;
        while( *p ) {
            while( *p && *p != sepchar ) {  // look for seperator
                ++p;
            }
            len = p - nmstart;
            *p = '\0';
            work = mem_alloc( sizeof( inp_line ) + len );
            work->next = NULL;
            strcpy_s( work->value, len + 1, nmstart );
            if( last != NULL ) {
                last->next = work;
            }
            last = work;
            if( head == NULL ) {
                head = work;
            }
            nmstart = ++p;
        }
        compend = 1;                    // so the end processing will happen
    }                                   // BEGIN or END not found

    if( compend && !(ProcFlags.in_macro_define) ) {
        err_count++;
        // SC--003: A macro is not being defined
        out_msg("ERR_MACRO_DEFINE END without BEGIN '%s'\n"
                "\t\t\tLine %d of file '%s'\n",
                macname,
                cb->s.f->lineno,
                cb->s.f->filename );
        return;
    }
    if( compbegin && (ProcFlags.in_macro_define) ) {
        err_count++;
        // SC--002 The control word parameter '%s' is invalid
        out_msg("ERR_NESTED_MACRO_DEFINE '%s' expecting END\n"
                "\t\t\tline %d of file '%s'\n",
                macname,
                cb->s.f->lineno,
                cb->s.f->filename );
    }
    *p   = save;
    if( compbegin ) {                   // start new macro define

        ProcFlags.in_macro_define = 1;
        lineno_start = cb->s.f->lineno;

        while( !(cb->s.f->flags & FF_eof) ) {  // process all macro lines

            get_line();

            if( cb->s.f->flags & (FF_eof | FF_err) ) {
                break;
            }
            p = buff2;
            if( *p == SCR_char ) {      // possible macro end
                if( tolower( *(p + 1) ) == 'd' &&
                    tolower( *(p + 2) ) == 'm' &&
                    (*(p + 3) == ' ' || *(p + 3) == '\0') ) {

                    garginit();

                    cc = getarg();
                    if( cc == omit ) {  // only .dm  means macro end
                        compend = 1;
                        break;
                    }
                    p = arg_start;
                    save = *p;
                    *p = '\0';
                    if( strncmp( macname, err_start, MAC_NAME_LENGTH ) ) {
                        // macroname from begin different from end
                        err_count++;
                        // SC--005 Macro '%s' is not being defined
                        out_msg( "ERR_MACRO_DEF Macro '%s' is not being defined\n"
                                "\t\t\tLine %d of file '%s'\n",
                                err_start,
                                cb->s.f->lineno,
                                cb->s.f->filename );
                        *p = save;
                        free_mac_lines( head );
                        return;
                    }
                    *p = save;
                    cc = getarg();
                    if( cc == omit ) {
                        err_count++;
                        // SC--048 A control word parameter is missing
                        out_msg(
                            "ERR_PARM_MISSING A control word parameter is missing\n"
                                "\t\t\tLine %d of file '%s'\n",
                                cb->s.f->lineno,
                                cb->s.f->filename );
                        free_mac_lines( head );
                        return;
                    }
                    p = arg_start;
                    save = *p;
                    *p = '\0';
                    if( strcmp( err_start, "end") ) {
                        err_count++;
                        // SC--002 The control word parameter '%s' is invalid
                        out_msg( "ERR_PARMINVALID "
                                 "The control word parameter '%s' is invalid\n"
                                "\t\t\tLine %d of file '%s'\n",
                                cb->s.f->lineno,
                                cb->s.f->filename );
                        free_mac_lines( head );
                        return;
                    }
                    compend = 1;
                    break;              // out of read loop
                }
            }
            work = mem_alloc( sizeof( inp_line ) + cb->s.f->usedlen );
            work->next = NULL;
            strcpy_s( work->value, cb->s.f->usedlen + 1, buff2 );
            if( last != NULL ) {
                last->next = work;
            }
            last = work;
            if( head == NULL ) {
                head = work;
            }
        }
        if( cb->s.f->flags & (FF_eof | FF_err) ) {
            err_count++;
            // error SC--004 End of file reached
            // macro '%s' is still being defined
            out_msg("ERR_MACRO_DEFINE End of file reached"
                    " line %d of file '%s'\n"
                    "\t\t\tmacro '%s' is still being defined\n",
                    cb->s.f->lineno,
                    cb->s.f->filename,
                    macname );
            free_mac_lines( head );
            return;
        }
    }                                   // end compbegin

    if( compend ) {                     // macro END definition processing
        mac_entry   *   me;
        mac_entry   *   dict;

        ProcFlags.in_macro_define = 0;
        len = strlen( cb->s.f->filename );
        me  = mem_alloc( len + sizeof( mac_entry ) );
        me->next = NULL;
        strcpy( me->name, macname );
        me->macline = head;
        me->lineno = lineno_start;
        strcpy( me->mac_file_name, cb->s.f->filename );

        if( macro_dict == NULL ) {
            macro_dict = me;
        } else {
            dict = macro_dict;
            while( dict->next != NULL ) {
                dict = dict->next;
            }
            dict->next = me;
        }
    } else {
        err_count++;
        out_msg("ERR_MACRO_DEFINE_logic error '%s'\n", macname );
        free_mac_lines( head );
        return;
    }
    return;
}

/* the following table and the processing routines will be split into
 * different modules later
 *
 *
 */

/***************************************************************************/
/*    SCR keywords                                                         */
/***************************************************************************/
static scrtag  scr_tags[] = {
    { "ap", scr_ap, 0 },
    { "dm", scr_dm, 0 },
    { "im", scr_im, 0 },
    { "se", scr_se, 0 },
    { "  ", NULL  , 0 }  };

#define SCR_TAGMAX  (sizeof( scr_tags ) / sizeof( scr_tags[ 0 ] ) - 1)

#define MAX_SCRTAGNAME_LENGTH   (sizeof( scr_tags->tagname ) - 1)


/*
 * Process script control line
 *
 */

static void     scan_script( void)
{
    char    *   p;
    char    *   pt;
    int         toklen;
    int         k;
    char        c;

    cb = input_cbs;
    p = ++scan_start;

    if( *p == '*' ) {
        return;                         // .*   +++ ignore comment line
    }
    if( *p == SCR_char && *(p+1) == SCR_char ) {
        return;                         // ...  +++ ignore label line for now
    }

    if( *p == '\'' ) {                  // .'
        p++;
        ProcFlags.CW_sep_ignore = 1;
    } else {
        ProcFlags.CW_sep_ignore = 0;

        if( *p == SCR_char ) {          // ..
            p++;
            ProcFlags.macro_ignore = 1;
        } else {
            ProcFlags.macro_ignore = 0;
        }
    }

    if( !ProcFlags.CW_sep_ignore ) { // scan line for CW_sep_char
        char    *   pchar;

        pchar = strchr( buff2, CW_sep_char );// look for controlword separator
        if( pchar != NULL ) {
            split_input( buff2, pchar + 1 );// ignore CW_sep_char
            *pchar= '\0';               // delete CW_sep_char
            buff2_lg = strlen( buff2 ); // new length of first part
        }
    }

    /***************************************************/
    /*  skip over ".  .  .cw"  indentation             */
    /***************************************************/
    if( p == scan_start ) {
        while( *p == SCR_char && *(p+1) == ' ' ) {
            while( *++p == ' ' ) /* empty */ ;  // skip blanks
        }
    }
    scan_start = p;
    c = *scan_start;

    token_buf[ 0 ] = '\0';
    pt = token_buf;
    while( isalnum( *p ) && *p != SCR_char ) {  // end of controlword
       *pt++ = *p++;                    // copy to TokenBuf
    }

    *pt = '\0';
    toklen = pt - token_buf;
    if( toklen >= TAG_NAME_LENGTH ) {
        *(token_buf + TAG_NAME_LENGTH) = '\0';
    }
    if( toklen ) {
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            printf_research("L%d    %c%s found %s(%d)\n\n", inc_level,
                            SCR_char,token_buf, cb->s.f->filename, cb->s.f->lineno );
            add_SCR_tag_research( token_buf );
        }

        for( k = 0; k < SCR_TAGMAX; ++k ) {
            if( toklen == SCR_KW_LENGTH ) {
                if( !stricmp( scr_tags[ k ].tagname, token_buf ) ) {
                    scan_start = p;     // script controlword found, process
                    scr_tags[ k ].tagproc();
                    break;
                }
            }
        }
    } else {
        out_msg("WNG_SCR_KEYWORD_MISSING\n" );  // only SCR_char in input
        wng_count++;
    }
}




/*
 *
 *
 */

void    scan_line( void )
{
    char    *   p;
    int         toklen;
    char        c;

    cb = input_cbs;

    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;

    if( *scan_start == SCR_char  ) {    // script control line
        scan_script();
    } else {

        while( *scan_start == ' ' ) {       // skip blanks
             scan_start++;
        }
        c = *scan_start;

        if( c == GML_char ) {

        /*******************************************************/
        /*  process GML Tags                                   */
        /*******************************************************/
            p = scan_start +1;

            if( !strnicmp( "CMT", p, 3 ) ) {
                if( *(p + 3) == '.' ||
                    *(p + 3) == ' ' ||
                    *(p + 3) == '\0' ) {
                    return;             // :CMT ignore comment line
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
            if( toklen >= TAG_NAME_LENGTH ) {
                *(scan_start + TAG_NAME_LENGTH) = '\0';
            }

            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                printf_research("L%d    %s found %s(%d)\n\n", inc_level,
                       scan_start, cb->s.f->filename, cb->s.f->lineno );
                add_GML_tag_research( scan_start + 1 );
            }

            if( !stricmp( ":include", scan_start ) ||
                !stricmp( ":imbed", scan_start ) ) {
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
                        quote = '.';    // error?? filename without quotes
                    }
                    fnstart = p;
                    while( *p && *p != quote ) {
                        ++p;
                    }
                    *p = '\0';
                    strcpy_s( token_buf, buf_size, fnstart );
                    ProcFlags.newLevelFile = 1;
                    return;
                }
            }

            /***************************************************************/
            /*  :set symbol="varname" value="abc".                         */
            /*                              delete.                        */
            /***************************************************************/

            /* to be reworked TBD
             *
             *
             *
             */

            if( !stricmp( ":set", scan_start ) ) {
                p++;
                while( *p == ' ' ) {
                    p++;
                }
                if( !strnicmp( "symbol=", p, 7 ) ) {
                    char    quote;
                    char    *nstart;

                    p += 7;
                    if( *p == '"' || *p == '\'' ) {
                        quote = *p;
                        ++p;
                    } else {
                        quote = '.';    // error?? name without quotes
                    }
                    nstart = p;
                    while( *p && *p != quote ) {
                        ++p;
                    }
                    *p = '\0';
                    strcpy_s( token_buf, buf_size, nstart );
                } else {

                }
                p++;
                while( *p == ' ' ) {
                    p++;
                }
                if( !strnicmp( "value=", p, 6 ) ) {
                    char    quote;
                    char    *fnstart;

                    p += 6;
                    if( *p == '"' || *p == '\'' ) {
                        quote = *p;
                        ++p;
                    } else {
                        quote = '.';
                    }
                    fnstart = p;
                    while( *p && *p != quote ) {
                        ++p;
                    }
                    *p = '\0';
                    strcpy_s( token_buf, buf_size, fnstart );
                }
            }
        }
    }
}

