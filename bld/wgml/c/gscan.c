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

    ProcFlags.newLevel = true;
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
/*    SCR keywords                                                         */
/***************************************************************************/
static tag     scr_tags[] = {
    { NULL, "ap",       2,  sc_ap },
//  { NULL, "dm",       2,  sc_dm },
    { NULL, "im",       2,  sc_im },
    { NULL, "se",       2,  sc_se },
    { NULL, "  ",       0,  NULL  } };

#define SCR_TAGMAX  (sizeof( scr_tags ) / sizeof( scr_tags[ 0 ] ) - 1)

#define MAX_SCRTAGNAME_LENGTH   (sizeof( scr_tags->tagname ) - 1)




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
                ProcFlags.CW_sep_ignore = true;
            } else {
                ProcFlags.CW_sep_ignore = false;
            }

            if( *p == SCR_char ) {
                p++;
                ProcFlags.macro_ignore = true;
            } else {
                ProcFlags.macro_ignore = false;
            }
        }
        token_buf[ 0 ] = '\0';
        pt = token_buf;
        while( isalnum( *p ) && *p != SCR_char ) {  // end of keyword
           *pt++ = *p++;                // copy to TokenBuf
        }

        *pt = '\0';
        toklen = pt - token_buf;
        if( toklen > MAX_SCRTAGNAME_LENGTH ) {
            *(token_buf + MAX_SCRTAGNAME_LENGTH) = '\0';
        }
        if( toklen ) {
            if( GlobalFlags.firstpass && GlobalFlags.research ) {
                printf_research("L%d    %s found %s(%d)\n", inc_level,
                               token_buf, cb->filename, cb->lineno );
                add_SCR_tag_research( token_buf );
            }
        }

        for( k = 0; k < SCR_TAGMAX; ++k ) {
            if( toklen == scr_tags[ k ].min_abbrev ) {
                if( !stricmp( scr_tags[ k ].tagname, token_buf ) ) {
                    scan_start = p;
                    scr_tags[ k ].tagproc();// tag found, process
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
                if( *(p + 3) == '.' ||
                    *(p + 3) == ' ' ||
                    *(p + 3) == '\0' ) {
                    return;             // ignore comment line
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
            if( toklen > MAX_SCRTAGNAME_LENGTH ) {
                *(scan_start + MAX_SCRTAGNAME_LENGTH) = '\0';
            }

            if( GlobalFlags.firstpass && GlobalFlags.research ) {
                printf_research("L%d    %s found %s(%d)\n", inc_level,
                       scan_start, cb->filename, cb->lineno );
                add_GML_tag_research( scan_start );
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
                    ProcFlags.newLevel = true;
                    return;
                }
            }

            /***************************************************************/
            /*  :set name="varname" value="abc".                           */
            /*                            delete.                          */
            /***************************************************************/

            if( !stricmp( ":set", scan_start ) ) {
                p++;
                while( *p == ' ' ) {
                    p++;
                }
                if( !strnicmp( "name=", p, 5 ) ) {
                    char    quote;
                    char    *nstart;

                    p += 5;
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
                        quote = '.';    // error?? filename without quotes
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

