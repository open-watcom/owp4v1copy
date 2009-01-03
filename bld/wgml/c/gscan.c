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


/***************************************************************************/
/*  add info about macro   to LIFO input list                              */
/***************************************************************************/

static  void    add_macro_cb_entry( mac_entry *me )
{
    macrocb *   new;
    inputcb *   nip;

    new = mem_alloc( sizeof( macrocb ) );

    nip = mem_alloc( sizeof( inputcb ) );
    nip->hidden_head = NULL;
    nip->hidden_tail = NULL;
    init_dict( &nip->local_dict );

    nip->fmflags = II_macro;
    nip->s.m     = new;

    new->lineno  = 0;
    new->macline = me->macline;
    new->mac     = me;
    new->flags   = FF_macro;

    nip->prev = input_cbs;
    input_cbs = nip;
    return;
}


/***************************************************************************/
/*  .im   processing  IMBED                                                */
/*  format .im filename                                                    */
/*         .im nn        -> SYSUSR0n.GML                                   */
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

    if( (cc == pos) && (gn.result < 10) ) {  // include SYSUSR0x.GML
        sprintf_s( token_buf, buf_size, "SYSUSR0%d.GML", gn.result );
    } else {
        p = gn.argstart;

        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = ' ';                // error??
        }
        fnstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';
        strcpy_s( token_buf, buf_size, fnstart );
    }

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


/*
 * free storage for the macro lines
 *
 */

void    free_lines( inp_line * line )
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

/*
 * .dm processing define macro
 */
static  void    scr_dm( void )
{
    char        *   nmstart;
    char        *   p;
    char        *   pn;
    char            save;
    int             len;
    int             macro_line_count;
    int             compbegin;
    int             compend;
    char            macname[ MAC_NAME_LENGTH + 1 ];
    inp_line    *   head;
    inp_line    *   last;
    inp_line    *   work;
    ulong           lineno_start;
    condcode        cc;
    inputcb     *   cb;

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

    p = tok_start;

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

    p = scan_start;
    head = NULL;
    last = NULL;
    save = *p;             // save char so we can make null terminated string
    *p   = '\0';
    macro_line_count = 0;

    compend   = !stricmp( tok_start, "end" );
    compbegin = !stricmp( tok_start, "begin" );
    if( !(compbegin | compend) ) { // only .dm macname /line1/line2/ possible
        char    sepchar;

        if( ProcFlags.in_macro_define ) {
            err_count++;
            out_msg("ERR_NESTED_MACRO_DEFINE '%s' expecting END\n"
                    "\t\t\tline %d of file '%s'\n",
                    tok_start,
                    cb->s.f->lineno,
                    cb->s.f->filename );
            return;
        }
        ProcFlags.in_macro_define = 1;

        *p   = save;
        lineno_start = cb->s.f->lineno;


        p = tok_start;
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
            macro_line_count++;
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
                break;                  // out of read loop
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
                        break;          // out of read loop
                    }
                    p = scan_start;
                    save = *p;
                    *p = '\0';
                    if( strncmp( macname, tok_start, MAC_NAME_LENGTH ) ) {
                        // macroname from begin different from end
                        err_count++;
                        // SC--005 Macro '%s' is not being defined
                        out_msg( "ERR_MACRO_DEF Macro '%s' is not being defined\n"
                                "\t\t\tLine %d of file '%s'\n",
                                tok_start,
                                cb->s.f->lineno,
                                cb->s.f->filename );
                        *p = save;
                        free_lines( head );
                        return;
                    }
                    *p = save;
                    cc = getarg();
                    if( cc == omit ) {
                        err_count++;
                        // SC--048 A control word parameter is missing
                        out_msg( "ERR_PARM_MISSING "
                                 "A control word parameter is missing\n"
                                 "\t\t\tLine %d of file '%s'\n",
                                 cb->s.f->lineno,
                                 cb->s.f->filename );
                        free_lines( head );
                        return;
                    }
                    p = scan_start;
                    save = *p;
                    *p = '\0';
                    if( strcmp( tok_start, "end") ) {
                        err_count++;
                        // SC--002 The control word parameter '%s' is invalid
                        out_msg( "ERR_PARMINVALID "
                                 "The control word parameter '%s' is invalid\n"
                                 "\t\t\tLine %d of file '%s'\n",
                                 cb->s.f->lineno,
                                 cb->s.f->filename );
                        free_lines( head );
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
            macro_line_count++;
        }                               // end read loop
        if( cb->s.f->flags & (FF_eof | FF_err) ) {
            err_count++;
            // error SC--004 End of file reached
            // macro '%s' is still being defined
            out_msg( "ERR_MACRO_DEFINE End of file reached"
                     " line %d of file '%s'\n"
                     "\t\t\tmacro '%s' is still being defined\n",
                     cb->s.f->lineno,
                     cb->s.f->filename,
                     macname );
            free_lines( head );
            return;
        }
    }                                   // end compbegin

    if( compend ) {                     // macro END definition processing
        mac_entry   *   me;

        me = find_macro( macro_dict, macname );
        if( me != NULL ) {              // delete macro with same name
            free_macro_entry( &macro_dict, me );
        }

        ProcFlags.in_macro_define = 0;

        len = strlen( cb->s.f->filename );
        me  = mem_alloc( len + sizeof( mac_entry ) );
        me->next = NULL;
        strcpy( me->name, macname );
        me->macline = head;
        me->lineno = lineno_start;
        strcpy( me->mac_file_name, cb->s.f->filename );

        add_macro_entry( &macro_dict, me );

        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            out_msg( "INF_MACRO '%s' defined with %d lines\n", macname,
                     macro_line_count );
        }
    } else {
        err_count++;
        out_msg( "ERR_MACRO_DEFINE_logic error '%s'\n", macname );
        free_lines( head );
        show_include_stack();
        return;
    }
    return;
}



/***************************************************************************/
/*  :CMT.                                                                  */
/*                                                                         */
/* The information following the comment tag on the input line is treated  */
/* as a comment. Text data and GML tags following the comment tag are not  */
/* processed. Except between tag attributes, this tag may appear at any    */
/* point in the GML sourc                                                  */
/***************************************************************************/

static  void    gml_cmt( const gmltag * entry )
{
    return;
}


/***************************************************************************/
/*  :IMBED   file='abc.gml'                                                */
/*  :INCLUDE file="abc.gml"                                                */
/*                                                                         */
/* :IMBED and :INCLUDE are eqivalent.                                      */
/*                                                                         */
/* The value of the required attribute file is used as the name of the     */
/* file to include.  The content of the included file is processed by      */
/* WATCOM Script/GML as if the data was in the original file.  This tag    */
/* provides the means whereby a document may be specified using a          */
/* collection of separate files.  Entering the source text into separate   */
/* files, such as one file for each chapter, may help in managing the      */
/* document.  if( the specified file does not have a file type, the        */
/* default document file type is used.  For example, if the main document  */
/* file is manual.doc, doc is the default document file type.  If the file */
/* is not found, the alternate extension supplied on the command line is   */
/* used.  If the file is still not found, the file type GML is used.  When */
/* working on a PC/DOS system, the DOS environment symbol GMLINC may be    */
/* set with an include file list.  This symbol is defined in the same way  */
/* as a library definition list, and provides a list of alternate          */
/* directories for file inclusion.  If an included file is not defined in  */
/* the current directory, the directories specified by the include path    */
/* list are searched for the file.  If the file is still not found, the    */
/* directories specified by the DOS environment symbol PATH are searched.  */
/*                                                                         */
/***************************************************************************/

static  void    gml_include( const gmltag * entry )
{
    char    *   p;

    p = scan_start;
    p++;
    while( *p == ' ' ) {
        p++;
    }
    *token_buf = '\0';
    if( !strnicmp( "file=", p, 5 ) ) {
        char    quote;
        char    *fnstart;

        p += 5;
        if( *p == '"' || *p == '\'' ) {
            quote = *p;
            ++p;
        } else {
            quote = '.';                // error?? filename without quotes
        }
        fnstart = p;
        while( *p && *p != quote ) {
            ++p;
        }
        *p = '\0';
        strcpy_s( token_buf, buf_size, fnstart );
        ProcFlags.newLevelFile = 1;     // start new include level
    }
    return;
}


/***************************************************************************/
/*   :SET symbol='symbol-name'                                             */
/*        value='character-string'                                         */
/*              delete.                                                    */
/*                                                                         */
/* This tag defines and assigns a value to a symbol name.  The symbol      */
/* attribute must be specified.  The value of this attribute is the name   */
/* of the symbol being defined, and cannot have a length greater than ten  */
/* characters.  The symbol name may only contain letters, numbers, and the */
/* characters @, #, $ and underscore(_).  The value attribute must be      */
/* specified.  The attribute value delete or a valid character string may  */
/* be assigned to the symbol name.  If the attribute value delete is used, */
/* the symbol referred to by the symbol name is deleted.                   */
/***************************************************************************/

static  void    gml_set( const gmltag * entry )
{
    char        *   p;
    char        *   symstart;
    char        *   valstart;
    char            c;
    bool            symbolthere = false;
    bool            valuethere = false;
    symvar          sym;
    sub_index       subscript;
    int             rc;
    symvar      * * working_dict;

    subscript = no_subscript;           // not subscripted
    scan_err = false;

    p = scan_start;
    p++;

    for( ;;) {
        while( *p == ' ' ) {            // over WS to attribute
            p++;
        }

        if( !strnicmp( "symbol", p, 6 ) ) {

            p += 6;
            while( *p == ' ' ) {        // over WS to attribute
                p++;
            }
            if( *p == '=' ) {
                p++;
                while( *p == ' ' ) {    // over WS to attribute
                    p++;
                }
            } else {
                continue;
            }
            symstart = p;


            p = scan_sym( symstart, &sym, &subscript );
            if( scan_err ) {
                return;
            }
            if( *p == '"' || *p == '\'' ) {
                p++;                    // skip terminating quote
            }
            if( sym.flags & local_var ) {
                working_dict = &input_cbs->local_dict;
            } else {
                working_dict = &global_dict;
            }
            symbolthere = true;

            while( *p == ' ' ) {
                p++;
            }
        }

        if( !strnicmp( "value", p, 5 ) ) {
            char    quote;

            p += 5;
            while( *p == ' ' ) {        // over WS to attribute
                p++;
            }
            if( *p == '=' ) {
                p++;
                while( *p == ' ' ) {    // over WS to attribute
                    p++;
                }
            } else {
                continue;
            }
            if( *p == '"' || *p == '\'' ) {
                quote = *p;
                ++p;
            } else {
                quote = ' ';
            }
            valstart = p;
            while( *p && *p != quote ) {
                ++p;
            }
            c = *p;
            *p = '\0';
            strcpy_s( token_buf, buf_size, valstart );
            *p = c;
            if( c == '"' || c == '\'' ) {
                p++;
            }
            valuethere = true;
        }
        if( symbolthere && valuethere ) {   // both attributes

            if( !strnicmp( token_buf, "delete", 6 ) ) {
                sym.flags |= deleted;
            }
            rc = add_symvar( working_dict, sym.name, token_buf, subscript,
                             sym.flags );
            break;                          // tag complete with attributes
        }

        c = *p;
        if( p >= scan_stop ) {
            c = '.';                    // simulate end of tag if EOF

            if( !(input_cbs->fmflags & II_eof) ) {
                if( get_line() ) {      // next line for missing attribute

                    process_line();
                    scan_start = buff2;
                    scan_stop  = buff2 + buff2_lg;
                    if( (*scan_start == SCR_char) ||
                        (*scan_start == GML_char) ) {
                                        //  missing attribute not supplied error

                    } else {
                        p = scan_start;
                        continue;       // scanning
                    }
                }
            }
        }
        if( c == '.' ) {                // end of tag found
            err_count++;
            // AT-001 Required attribute not found
            if( input_cbs->fmflags & II_macro ) {
                out_msg( "ERR_ATT_missing Required attribute not found\n"
                         "\t\t\tLine %d of macro '%s'\n",
                         input_cbs->s.m->lineno,
                         input_cbs->s.m->mac->name );
            } else {
                out_msg( "ERR_ATT_missing Required attribute not found\n"
                         "\t\t\tLine %d of file '%s'\n",
                         input_cbs->s.f->lineno, input_cbs->s.f->filename );
            }
            if( inc_level > 0 ) {
                show_include_stack();
            }
            break;
        }
    }
    return;
}



/* the following tables and the processing routines will be split into
 * different modules later
 *
 *
 */

/***************************************************************************/
/*    GML tags                                                             */
/***************************************************************************/

static  const   gmltag  gml_tags[] = {

#define pick(name, length, routine, flags) { name, length, routine, flags },
#include "gtags.h"
#undef pick
    { "   ", 0, NULL, 0 } // end

};

#define GML_TAGMAX  (sizeof( gml_tags ) / sizeof( gml_tags[ 0 ] ) - 1)



/***************************************************************************/
/*    SCR control words                                                    */
/***************************************************************************/
static  const   scrtag  scr_tags[] = {
    { "ap", scr_ap },
    { "dm", scr_dm },
    { "im", scr_im },
    { "se", scr_se },
    { "  ", NULL   }
};

#define SCR_TAGMAX  (sizeof( scr_tags ) / sizeof( scr_tags[ 0 ] ) - 1)


static void scan_gml( void )
{
    inputcb     *   cb;
    char        *   p;
    int             toklen;
    int             k;
    char            csave;

    cb = input_cbs;

    p = scan_start +1;
    tok_start = scan_start;
    while( *p != ' ' && *p != '.' && p <= scan_stop ) {// search end of keyword
        p++;
    }
    scan_start = p;                      // store argument start address
    toklen = p - tok_start - 1;
    csave = *p;
    *p = '\0';
    if( toklen >= TAG_NAME_LENGTH ) {
        err_count++;
        // SC--074 For the symbol '%s'
        //         The length of a symbol cannot exceed ten characters
        if( cb->fmflags & II_macro ) {
            out_msg( "ERR_SYM_NAME_too_long '%s'\n"
                     "\t\tThe length of a symbol cannot exceed ten characters\n"
                     "\t\t\tLine %d of macro '%s'\n",
                     tok_start + 1,
                     cb->s.m->lineno, cb->s.m->mac->name );
        } else {
            out_msg( "ERR_SYM_NAME_too_long '%s'\n"
                     "\t\tThe length of a symbol cannot exceed ten characters\n"
                     "\t\t\tLine %d of file '%s'\n",
                     tok_start + 1,
                     cb->s.f->lineno, cb->s.f->filename );
        }
        if( inc_level > 0 ) {
            show_include_stack();
        }
        return;
    }

    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        if( cb->fmflags & II_macro ) {
            printf_research( "L%d    %c%s found in macro %s(%d)\n\n",
                             inc_level, GML_char, tok_start + 1,
                             cb->s.m->mac->name, cb->s.m->lineno );
        } else {
            printf_research( "L%d    %c%s found in file %s(%d)\n\n",
                             inc_level, GML_char, tok_start + 1,
                             cb->s.f->filename, cb->s.f->lineno );
        }
        add_GML_tag_research( tok_start + 1 );
    }


    for( k = 0; k < GML_TAGMAX; ++k ) {
        if( toklen == gml_tags[ k].taglen ) {
            if( !stricmp( gml_tags[ k ].tagname, tok_start + 1 ) ) {
                *p = csave;
                gml_tags[ k ].gmlproc( &gml_tags[ k ] );
                break;
            }
        }
    }
    *p = csave;
}


/*
 * add macro parms from input line as local symbolic variables
 * for non quoted parms try to assign symbolic variables
 * i.e.  a b c *var="1.8" d "1 + 2"
 *    will give &* =a b c *var="1.8" d "1 + 2"
 *              &*0=5
 *              &*1=a
 *              &*2=b
 *              &*3=c
 *              &*4=d
 *              &*5=1 + 2
 *       and &*var = 1.8
 *
 *  the variable for &* is named _  This can change if this leads to
 *  conflicts  -> change define MAC_STAR_NAME in gtype.h
 *
 */

static void     add_macro_parms( char * p )
{
    int             len;
    condcode        cc;

    while( *p && *p == ' ' ) {
        ++p;
    }
    len   = strlen( p );
    if( len > 0 ) {
        char    starbuf[ 12 ];
        int     star0;

                                        // the macro parameter line
                                        // the name _ has to change (perhaps)
        add_symvar( &input_cbs->local_dict, MAC_STAR_NAME, p, no_subscript, local_var );

        star0 = 0;
        garginit();
        cc = getarg();
        while( cc > omit ) {            // as long as there are parms
            char        c;
            char    *   scan_save;

            if( cc == pos ) {           // argument not quoted
                           /* look if it is a symbolic variable definition */
                scan_save  = scan_start;
                c          = *scan_save; // prepare value end
                *scan_save = '\0';      // terminate string
                scan_start = tok_start; // rescan for variable
                ProcFlags.suppress_msg = true;  // no errmsg please
                ProcFlags.blanks_allowed = 0;   // no blanks please

                scr_se();               // try to set variable and value

                ProcFlags.suppress_msg = false; // reenable err msg
                ProcFlags.blanks_allowed = 1;   // blanks again
                *scan_save = c;        // restore original char at string end
                scan_start = scan_save; // restore scan address
                if( scan_err ) {        // not valid
                    cc = omit;
                    star0++;
                    sprintf( starbuf, "%d", star0 );
                    p = tok_start + arg_flen ;
                    c = *p;                 // prepare value end
                    *p = '\0';              // terminate string
                    add_symvar( &input_cbs->local_dict, starbuf, tok_start,
                                no_subscript, local_var );
                    *p = c;                // restore original char at string end
                }

            }
            if( cc == quotes ) {        // add argument as local symbolic var
                star0++;
                sprintf( starbuf, "%d", star0 );
                p = tok_start + arg_flen ;
                c = *p;                 // prepare value end
                *p = '\0';              // terminate string
                add_symvar( &input_cbs->local_dict, starbuf, tok_start,
                            no_subscript, local_var );
                *p = c;                // restore original char at string end
            }
            cc = getarg();              // look for next parm
        }
                                        // the positional parameter count
        add_symvar( &input_cbs->local_dict, "0", starbuf,
                    no_subscript, local_var );
    }

    if( GlobalFlags.research && GlobalFlags.firstpass ) {
        print_sym_dict( input_cbs->local_dict );
    }
}



/*
 * search for (control word) separator in string outside of quotes
 *       returns ptr to sep char or NULL if not found
 *      quotes are single or double quotes
 */
char    *   search_separator( char * str, char sep )
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
 * Process script control line
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
    char            c;

    cb = input_cbs;
    p = scan_start + 1;

    if( *p == '*' ) {
        return;                         // .*   +++ ignore comment up to EOL
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
            me = NULL;
        } else {
            ProcFlags.macro_ignore = 0;
        }
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
    while( *p && test_macro_char( *p ) ) {  // end of controlword
       *pt++ = *p++;                    // copy to TokenBuf
    }
    toklen = pt - token_buf;
    if( *p && (*p != ' ') || toklen == 0 ) {// no valid script controlword / macro


//       copy_buff2_to_output();    TBD


       return;
    }
    *pt = '\0';

    if( toklen >= MAC_NAME_LENGTH ) {
        *(token_buf + MAC_NAME_LENGTH) = '\0';
    }
    if( !ProcFlags.macro_ignore ) {
        me = find_macro( macro_dict, token_buf );
    } else {
        me = NULL;
    }
    if( me != NULL ) {                  // macro found
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            if( cb->fmflags & II_macro ) {
                printf_research( "L%d    %c%s found in macro %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.m->mac->name, cb->s.m->lineno );
            } else {
                printf_research( "L%d    %c%s found in file %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.f->filename, cb->s.f->lineno );
            }
            add_SCR_tag_research( token_buf );
        }
        add_macro_cb_entry( me );
        inc_inc_level();
        add_macro_parms( p );
    } else {                            // try script controlword
        if( GlobalFlags.research && GlobalFlags.firstpass ) {
            if( cb->fmflags & II_macro ) {
                printf_research( "L%d    %c%s found in macro %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.m->mac->name, cb->s.m->lineno );
            } else {
                printf_research( "L%d    %c%s found in file %s(%d)\n\n",
                                 inc_level, SCR_char, token_buf,
                                 cb->s.f->filename, cb->s.f->lineno );
            }
            add_SCR_tag_research( token_buf );
        }

        for( k = 0; k < SCR_TAGMAX; ++k ) {
            if( toklen == SCR_KW_LENGTH ) {
                if( !stricmp( scr_tags[ k ].tagname, token_buf ) ) {
                    scan_start = p; // script controlword found, process
                    scr_tags[ k ].tagproc();
                    break;
                }
            }
        }
    }
}




/*
 *  scan_line look whether input is script / gml control line or text
 *
 */

void    scan_line( void )
{

    scan_start = buff2;
    scan_stop  = buff2 + buff2_lg;


    if( *scan_start == SCR_char ) {
        scan_script();                  // script control line
    } else if( *scan_start == GML_char ) {
        scan_gml();                     // gml tags
    } else {
                                        // textline               TBD
    }
}

