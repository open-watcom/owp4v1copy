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
* Description:  utility functions for wgml input line scanning
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "copfiles.h"
#include "outbuff.h"


/***************************************************************************/
/*    GML tags                                                             */
/***************************************************************************/

#define pick(name, length, routine, flags) { name, length, routine, flags },

static  const   gmltag  gml_tags[] = {

#include "gtags.h"
    { "   ", 0, NULL, 0 }               // end

};

#define GML_TAGMAX  (sizeof( gml_tags ) / sizeof( gml_tags[0] ) - 1)

#undef pick


/***************************************************************************/
/*    SCR control words                                                    */
/***************************************************************************/

#define pick(name, routine, flags) { name, routine, flags },

static  const   scrtag  scr_tags[] = {

#include "gscrcws.h"

    { "  ", NULL, 0   }                 // end
};

#define SCR_TAGMAX  (sizeof( scr_tags ) / sizeof( scr_tags[0] ) - 1)

#undef pick



/***************************************************************************/
/*  scan for gml tags                                                      */
/***************************************************************************/

static void scan_gml( void )
{
    inputcb     *   cb;
    char        *   p;
    int             toklen;
    int             k;
    char            csave;
    bool            processed;
    gtentry     *   ge;                 // GML user tag entry
    mac_entry   *   me;                // script macro for processing GML tag
    char            linestr[MAX_L_AS_STR];

    cb = input_cbs;

    p = scan_start +1;
    tok_start = scan_start;
    while( is_id_char( *p ) && p <= scan_stop ) { // search end of TAG
        p++;
    }
    scan_start = p;                      // store argument start address
    toklen = p - tok_start - 1;
    csave = *p;
    *p = '\0';
    if( toklen >= TAG_NAME_LENGTH ) {
        err_count++;
        // SC--009 The tagname is too long
        if( cb->fmflags & II_macro ) {
            utoa( cb->s.m->lineno, linestr, 10 );
            g_err( err_tag_name, tok_start + 1, linestr, "macro",
                   cb->s.m->mac->name );
        } else {
            utoa( cb->s.f->lineno, linestr, 10 );
            g_err( err_tag_name, tok_start + 1, linestr, "file",
                   cb->s.f->filename );
        }
        if( inc_level > 0 ) {
            show_include_stack();
        }
        *p = csave;
        scan_start = tok_start;         // process as text
        return;
    }

    if( GlobalFlags.research && GlobalFlags.firstpass ) {

        if(  stricmp( tok_start + 1, "cmt" ) ) {   // quiet for :cmt.

            if( cb->fmflags & II_macro ) {
                printf_research( "L%d    %c%s tag found in macro %s(%d)\n\n",
                                 inc_level, GML_char, tok_start + 1,
                                 cb->s.m->mac->name, cb->s.m->lineno );
            } else {
                printf_research( "L%d    %c%s tag found in file %s(%d)\n\n",
                                 inc_level, GML_char, tok_start + 1,
                                 cb->s.f->filename, cb->s.f->lineno );
            }
        }
        add_GML_tag_research( tok_start + 1 );
    }

    ge = find_tag( &tag_dict, tok_start + 1 );
    processed = false;
    if( ge != NULL ) {                  // GML user defined Tag found
        *p = csave;
        if( ge->tagflags & tag_off ) {  // tag off, treat as text
            scan_start = tok_start;
            return;
        }
        me = find_macro( macro_dict, ge->macname );
        if( me == NULL ) {
            err_count++;
            // SC--037: The macro 'xxxxxx' for the gml tag 'yyyyy'
            //          is not defined
            if( cb->fmflags & II_macro ) {
                utoa( cb->s.m->lineno, linestr, 10 );
                g_err( err_tag_macro,
                         ge->macname, ge->name,
                         linestr, "macro", cb->s.m->mac->name );
            } else {
                utoa( cb->s.f->lineno, linestr, 10 );
                g_err( err_tag_macro,
                         ge->macname, ge->name,
                         linestr, "file", cb->s.f->filename );
            }
            if( inc_level > 0 ) {
                show_include_stack();
            }
            *p = csave;
            scan_start = tok_start;         // process as text
            return;
        } else {

            processed = process_tag( ge, me );

        }
    } else {
        for( k = 0; k < GML_TAGMAX; ++k ) {
            if( toklen == gml_tags[k].taglen ) {
                if( !stricmp( gml_tags[k].tagname, tok_start + 1 ) ) {
                    *p = csave;
                    gml_tags[k].gmlproc( &gml_tags[k] );
                    processed = true;
                    if( *scan_start == '.' ) {
                        scan_start++;
                    }
                    break;
                }
            }
        }
    }
    *p = csave;
    if( !processed ) {
        scan_start = tok_start;
    }
}


/*
 * search for (control word) separator in string outside of quotes
 *       returns ptr to sep char or NULL if not found
 *      quotes are single or double quotes
 */
static char *   search_separator( char * str, char sep )
{
    bool        instring = false;
    char        quote = '\0';

    while( *str != '\0' ) {
        if( instring ) {
            if( *str == quote ) {
                instring = false;
            }
        } else {
            if( (*str == '\"') || (*str == '\'') ) {
                instring = true;
                quote = *str;
            } else {
                if( *str == sep ) {
                    break;
                }
            }
        }
        str++;
    }
    if( *str == sep ) {
        return( str );
    } else {
        return( NULL );
    }

}


/*
 * Scan line with script control word
 *
 */

static void     scan_script( void)
{
    inputcb     *   cb;
    mac_entry   *   me;
    char        *   p;
    char        *   pt;
    int             toklen;
    int             k;
    bool            cwfound;

    cb = input_cbs;
    p = scan_start + 1;
    scan_restart = scan_start;

    if( *p == '*' ) {
        scan_start = scan_stop + 1;
        return;                         // .*   +++ ignore comment up to EOL
    }

    if( *p == SCR_char && *(p+1) == SCR_char ) {
            pt = token_buf;
            *pt++ = SCR_char;               // special for ...label
            *pt++ = SCR_char;
            *pt   = '\0';
            me = NULL;
            scan_start = p + 2;
            toklen = 2;
    } else {

        if( *p == '\'' ) {                  // .'
            p++;
            ProcFlags.CW_sep_ignore = 1;
        } else {
            ProcFlags.CW_sep_ignore = 0;

            if( *p == SCR_char ) {          // ..
                p++;
                ProcFlags.macro_ignore = 1;
                me = NULL;
            } else {
                ProcFlags.macro_ignore = 0;
            }
        }
        if( ProcFlags.literal ) {
            ProcFlags.CW_sep_ignore = 1;
            ProcFlags.macro_ignore = 1;
        }
        if( !ProcFlags.CW_sep_ignore ) { // scan line for CW_sep_char
            char    *   pchar;

            pchar = search_separator( buff2, CW_sep_char );

            if( pchar != NULL ) {
                split_input( buff2, pchar + 1 );// ignore CW_sep_char
                *pchar= '\0';               // delete CW_sep_char
                buff2_lg = strlen( buff2 ); // new length of first part
            }
        }

        scan_start = p;

        pt = token_buf;
        while( *p && is_macro_char( *p ) ) {  // end of controlword
           *pt++ = tolower( *p++ );     // copy lowercase to TokenBuf
        }
        *pt = '\0';

        toklen = pt - token_buf;

        if( *p && (*p != ' ') || toklen == 0 ) {// no valid script controlword / macro
            if( !ProcFlags.literal ) {
               cw_err();
            }
            scan_start = scan_restart;  // treat as text
            return;
        }

        if( toklen >= MAC_NAME_LENGTH ) {
            *(token_buf + MAC_NAME_LENGTH) = '\0';
        }
        if( !ProcFlags.macro_ignore ) {
            me = find_macro( macro_dict, token_buf );
        } else {
            me = NULL;
        }
    }

    if( me != NULL ) {                  // macro found
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            if( cb->fmflags & II_macro ) {
                printf_research( "L%d    %c%s macro found in macro %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.m->mac->name, cb->s.m->lineno );
            } else {
                printf_research( "L%d    %c%s macro found in file %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.f->filename, cb->s.f->lineno );
            }
            add_SCR_tag_research( token_buf );
        }
        add_macro_cb_entry( me, NULL );
        inc_inc_level();
        add_macro_parms( p );
        scan_restart = scan_stop + 1;
    } else {                            // try script controlword
        cwfound = false;
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            if( cb->fmflags & II_macro ) {
                printf_research( "L%d    %c%s CW found in macro %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.m->mac->name, cb->s.m->lineno );
            } else {
                printf_research( "L%d    %c%s CW found in file %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.f->filename, cb->s.f->lineno );
            }
            add_SCR_tag_research( token_buf );
        }

        if( toklen == SCR_KW_LENGTH ) {
            for( k = 0; k < SCR_TAGMAX; ++k ) {
                if( !stricmp( scr_tags[k].tagname, token_buf ) ) {
                    if( ProcFlags.literal  ) {  // .li active
                        if( !stricmp( token_buf, "li" ) ) {// process only .li
                            scan_start = p;// script controlword found, process
                            scr_tags[k].tagproc();
                        }
                    } else {
                        scan_start = p; // script controlword found, process
                        if( scr_tags[k].cwflags & cw_break ) {
                            scr_process_break();
                        }
                        scr_tags[k].tagproc();
                    }
                    cwfound = true;
                    break;
                }
            }
        }
        if( !cwfound ) {
            cw_err();                   // unrecognized control word
        }
    }
    scan_start = scan_restart;
}

/***************************************************************************/
/*  logic for decision on skipping or processing line depending on         */
/*  the current state of the .if .th .el .do  controlwords encountered     */
/*                                                                         */
/*  The //comments refer to labels in the ASM code  see comment in wgml.c  */
/*  file cbt284.011                                                        */
/***************************************************************************/

static  condcode    mainif( void)
{
    condcode    cc;
    ifcb    *   cb;
    char        linestr[MAX_L_AS_STR];

    cb = input_cbs->if_cb;
    cc = no;
#if 0
    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        out_msg( "ANF mainif L %d t %d, f %d"
                " th(%d)  el(%d)  do(%d) last(%d) cwif,do,te(%d,%d,%d)\n",
                 cb->if_level,
                 cb->if_flags[cb->if_level].iftrue,
                 cb->if_flags[cb->if_level].iffalse,
                 cb->if_flags[cb->if_level].ifthen,
                 cb->if_flags[cb->if_level].ifelse,
                 cb->if_flags[cb->if_level].ifdo,
                 cb->if_flags[cb->if_level].iflast,
                 cb->if_flags[cb->if_level].ifcwif,
                 cb->if_flags[cb->if_level].ifcwdo,
                 cb->if_flags[cb->if_level].ifcwte
              );
    }
#endif
//mainif
    if( cb->if_flags[cb->if_level].iflast   // 1. rec after .if
        && !cb->if_flags[cb->if_level].ifcwte) {// not .th or .el

        cb->if_flags[cb->if_level].iflast = false;  // reset first switch
        cb->if_flags[cb->if_level].ifthen = true;   // treat as then
    }
//mnif01
    if( cb->if_flags[cb->if_level].ifcwif ) {   // .if
//mnif03
        if( cb->if_flags[cb->if_level].ifthen
            || cb->if_flags[cb->if_level].ifelse ) {// object of .th or .el

            cc = pos;
        } else {
//mnif03a
            while( cb->if_level > 0 ) { // pop one level
                cb->if_level--;
                if( cb->if_flags[cb->if_level].ifdo ) {
                    break;
                }
            }
            cc = pos;                   // .do or all popped
        }
#if 0
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            out_msg( "END mainif L %d t %d, f %d"
                    " th(%d)  el(%d)  do(%d) last(%d)\n",
                     cb->if_level,
                     cb->if_flags[cb->if_level].iftrue,
                     cb->if_flags[cb->if_level].iffalse,
                     cb->if_flags[cb->if_level].ifthen,
                     cb->if_flags[cb->if_level].ifelse,
                     cb->if_flags[cb->if_level].ifdo,
                     cb->if_flags[cb->if_level].iflast );
        }
#endif
        return( cc );
    } else {                            // not .if
//mnif01 cont.
        if( cb->if_flags[cb->if_level].ifcwdo ) {   // if  .do
            cc = pos;
#if 0
            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                out_msg( "END mainif L %d t %d, f %d"
                        " th(%d)  el(%d)  do(%d) last(%d)\n",
                         cb->if_level,
                         cb->if_flags[cb->if_level].iftrue,
                         cb->if_flags[cb->if_level].iffalse,
                         cb->if_flags[cb->if_level].ifthen,
                         cb->if_flags[cb->if_level].ifelse,
                         cb->if_flags[cb->if_level].ifdo,
                         cb->if_flags[cb->if_level].iflast );
            }
#endif
            return( cc );
        }
        if( cb->if_flags[cb->if_level].ifthen
            || cb->if_flags[cb->if_level].ifelse ) {// object of .th or .el
//mnif05
            if( cb->if_flags[cb->if_level].ifelse ) {   // object of .el
//mnif06
                if( cb->if_flags[cb->if_level].iftrue ) {// omit if true for .el
//mnif08
                    cc = neg;
                } else {
                    cc = pos;
                }
            } else {
                if( cb->if_flags[cb->if_level].iffalse ) {// omit false for .th
                    cc = neg;
                } else {
                    cc = pos;
                }
            }
        } else {
            if( cb->if_flags[cb->if_level].ifcwte ) {
                cc = pos;
            } else {
//mnif02
                while( cb->if_level > 0 ) {
                    cb->if_level--;
                    if( cb->if_flags[cb->if_level].ifdo ) {
//mnif05
                        if( cb->if_flags[cb->if_level].ifelse ) {// object of .el
//mnif06
                            if( cb->if_flags[cb->if_level].iftrue ) {
//mnif08
                                cc = neg;   // omit if true for .el
                            } else {
                                cc = pos;
                            }
                        } else {
                            if( cb->if_flags[cb->if_level].iffalse ) {
                                cc = neg;   // omit false for .th
                            } else {
                                cc = pos;
                            }
                        }
                        break;
                    }
                }
                if( cc == no ) {        // not set then process record
                    cc = pos;
                }
            }
        }
    }
    if( cc == no ) {
        if( input_cbs->fmflags & II_macro ) {
            utoa( input_cbs->s.m->lineno, linestr, 10 );
            g_err( err_if_intern, linestr, "macro", input_cbs->s.m->mac->name );
        } else {
            utoa( input_cbs->s.f->lineno, linestr, 10 );
            g_err( err_if_intern,
                     linestr, "file", input_cbs->s.f->filename );
        }
        if( inc_level > 1 ) {
            show_include_stack();
        }
        err_count++;
    }
#if 0
    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        out_msg( "END mainif L %d t %d, f %d"
                " th(%d)  el(%d)  do(%d) last(%d)\n",
                 cb->if_level,
                 cb->if_flags[cb->if_level].iftrue,
                 cb->if_flags[cb->if_level].iffalse,
                 cb->if_flags[cb->if_level].ifthen,
                 cb->if_flags[cb->if_level].ifelse,
                 cb->if_flags[cb->if_level].ifdo,
                 cb->if_flags[cb->if_level].iflast );
    }
#endif
    return( cc );

}


/***************************************************************************/
/*  first pass at script control words .if .th .el .do                     */
/*                                                                         */
/*  This is needed for routine mainif() above                              */
/*                                                                         */
/***************************************************************************/

static void set_if_then_do( void )
{
    char        cw[3];
    ifcb    *   cb = input_cbs->if_cb;

    if( (*(buff2 + 1) == SCR_char)  ||  // ..CW
        (*(buff2 + 1) == '\'') ) {      // .'CW
        cw[0] = tolower( *(buff2 + 2) );// copy possible controlword
        cw[1] = tolower( *(buff2 + 3) );
    } else {                            // .CW
        cw[0] = tolower( *(buff2 + 1) );// copy possible controlword
        cw[1] = tolower( *(buff2 + 2) );
    }
    cw[2] = '\0';

    if( !strcmp( cw, "if" ) ) {
        cb->if_flags[cb->if_level].ifcwif = true;
    } else if( !strcmp( cw, "do" ) ) {
        cb->if_flags[cb->if_level].ifcwdo = true;
    } else if( !strcmp( cw, "th" ) || !strcmp( cw, "el" ) ) {
        cb->if_flags[cb->if_level].ifcwte = true;
    }
}


/*
 *  scan_line look whether input is script / gml control line or text
 *
 */

void    scan_line( void )
{
    condcode        cc;
    ifcb        *   cb;

    cb         = input_cbs->if_cb;
    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;

    cb->if_flags[cb->if_level].ifcwte = false;  // reset
    cb->if_flags[cb->if_level].ifcwdo = false;  // .. current
    cb->if_flags[cb->if_level].ifcwif = false;  // .... if, then, else, do

    if( !ProcFlags.literal && (*scan_start == SCR_char) ) {
        set_if_then_do();
    }
    if( !ProcFlags.literal ) {
        cc = mainif();
    } else {
        cc = pos;
    }
    if( cc == pos ) {
        if( *scan_start == SCR_char ) {
            if( ProcFlags.late_subst ) {
                process_late_subst();   // substitute &gml, &amp, ...
            }
            scan_script();              // script control line
        } else if( *scan_start == GML_char ) {
            if( ProcFlags.late_subst ) {
                process_late_subst();   // substitute &gml, &amp, ...
            }
            scan_gml();                 // gml tags
        } else if( ProcFlags.late_subst ) {
                process_late_subst();   // substitute &gml, &amp, ...
        }
        if( (*scan_start != '\0') && (scan_start <= scan_stop) ) {
            if( GlobalFlags.research && GlobalFlags.firstpass ) {
                g_info( inf_text_line, scan_start );
            }
//          process_text( scan_start, cur_font_num );   TBD
        }
    } else if( GlobalFlags.research && GlobalFlags.firstpass ) {
        g_info( inf_skip_line );
    }
    if( ProcFlags.literal ) {
        if( li_cnt < LONG_MAX ) {   // we decrement, do not wait for .li OFF
            if( li_cnt-- <= 0 ) {
                ProcFlags.literal = false;
            }
        }
    }
}

