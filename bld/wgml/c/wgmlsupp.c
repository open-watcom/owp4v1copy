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
* Description:  Defines functions moved out of wgml.c so they can be linked
*               into the research programs without including main() from wgml.c
*                   free_inc_fp()
*                   free_resources()
*                   free_some_mem()
*                   get_line()
*                   get_macro_line()
*                   g_banner()
*                   inc_inc_level()
*                   my_exit()
*                   reopen_inc_fp()
*                   show_include_stack()
****************************************************************************/


#include <errno.h>
#include "wgml.h"
#include "banner.h"


#define mystr(x)            # x
#define xmystr(s)           mystr(s)

#define CRLF            "\n"


/***************************************************************************/
/*  Output Banner if wanted and not yet done                               */
/***************************************************************************/

void g_banner( void )
{
    if( !(GlobalFlags.bannerprinted | GlobalFlags.quiet) ) {
        out_msg( banner1w( "Script/GML", _WGML_VERSION_ ) CRLF );
        out_msg( banner2a() CRLF );
        out_msg( banner3 CRLF );
        out_msg( banner3a CRLF );
        out_msg( "Compiled with WATCOMC "xmystr(__WATCOMC__)
                 " "__DATE__" "__TIME__ CRLF);
        mem_banner();
        GlobalFlags.bannerprinted = 1;
    }
}


/***************************************************************************/
/*  increment include level                                                */
/***************************************************************************/

void    inc_inc_level( void )
{
    inc_level++;                        // start new level
    if( inc_level > max_inc_level ) {
        max_inc_level = inc_level;      // record highest level
    }
}


/***************************************************************************/
/*  Program end                                                            */
/***************************************************************************/

void my_exit( int rc )
{
    exit( rc );
}


/***************************************************************************/
/*  Try to close an opened include file                                    */
/***************************************************************************/

static  bool    free_inc_fp( void )
{
    inputcb *   ip;
    filecb  *   cb;
    int         rc;

    ip = input_cbs;
    while( ip != NULL ) {              // as long as input stack is not empty
        if( ip->fmflags & II_file ) {   // if file (not macro)
            if( (cb = ip->s.f) != NULL ) {
                if( (cb->flags & FF_open) ) {   // and file is open
                    rc = fgetpos( cb->fp, &cb->pos );
                    if( rc != 0 ) {
                        strerror_s( buff2, buf_size, errno );
                        g_err( err_file_io, buff2, cb->filename );
                        err_count++;
                        g_suicide();
                    }
                    rc = fclose( cb->fp );
                    if( rc != 0 ) {
                        strerror_s( buff2, buf_size, errno );
                        g_err( err_file_io, buff2, cb->filename );
                        err_count++;
                        g_suicide();
                    }
                    cb->flags &= ~FF_open;
                    return( true );
                }
            }
        }
        ip = ip->prev;                  // next higher input level
    }
    return( false );                    // nothing to close
}


/***************************************************************************/
/* reopen a file which was closed due to resource shortage and perhaps     */
/* close another one                                                       */
/***************************************************************************/

static void reopen_inc_fp( filecb *cb )
{
    int         rc;
    errno_t     erc;
    errno_t     erc2;

    if( ! cb->flags & FF_open ) {
        for( ;; ) {
            erc = fopen_s( &cb->fp, cb->filename, "rb" );
            if( erc == 0 ) break;
            erc2 = errno;
            if( errno != ENOMEM && errno != ENFILE && errno != EMFILE ) break;
            if( !free_inc_fp() ) break; // try closing an include file
        }
        if( erc == 0 ) {
            rc = fsetpos( cb->fp, &cb->pos );
            if( rc != 0 ) {
                strerror_s( buff2, buf_size, errno );
                g_err( err_file_io, buff2, cb->filename );
                err_count++;
                g_suicide();
            }
            cb->flags |= FF_open;
        } else {
            strerror_s( buff2, buf_size, erc2 );
            g_err( err_file_io, buff2, cb->filename );
            err_count++;
            g_suicide();
        }
    }
    return;
}

/***************************************************************************/
/*  Report resource exhaustion: may eventually try to correct the problem  */
/***************************************************************************/

bool    free_resources( errno_t in_errno )
{
    if( in_errno == ENOMEM ) g_err( err_no_memory );
    else g_err( err_no_handles );
    err_count++;
    return( false );
}


/***************************************************************************/
/*  free some buffers                                                      */
/***************************************************************************/

void    free_some_mem( void )
{
    doc_pane    *   sav_pane;
    int             i;

    if( token_buf != NULL ) {
        mem_free( token_buf );
    }
    if( alt_ext != NULL ) {
        mem_free( alt_ext );
    }
    if( def_ext != NULL ) {
        mem_free( def_ext );
    }
    if( master_fname != NULL ) {
        mem_free( master_fname );
    }
    if( master_fname_attr != NULL ) {
        mem_free( master_fname_attr );
    }
    if( dev_name != NULL ) {
        mem_free( dev_name );
    }
    if( lay_files != NULL ) {
        laystack * lwk;

        while( lay_files != NULL ) {
            lwk = lay_files->next;
            mem_free( lay_files );
            lay_files = lwk;
        }
    }
    if( out_file != NULL ) {
        mem_free( out_file );
    }
    if( out_file_attr != NULL ) {
        mem_free( out_file_attr );
    }
    if( global_dict != NULL ) {
        free_dict( &global_dict );
    }
    if( macro_dict != NULL ) {
        free_macro_dict( &macro_dict );
    }
    if( line_buff.text != NULL ) {
        mem_free( line_buff.text );
    }
    if( fig_list != NULL ) {
        free_ffh_list( fig_list );
    }
    if( fn_list != NULL ) {
        free_ffh_list( fn_list );
    }
    if( hd_list != NULL ) {
        free_ffh_list( hd_list );
    }
    if( fig_fwd_refs != NULL ) {
        free_fwd_refs( fig_fwd_refs );
    }
    if( fn_fwd_refs != NULL ) {
        free_fwd_refs( fn_fwd_refs );
    }
    if( hd_fwd_refs != NULL ) {
        free_fwd_refs( hd_fwd_refs );
    }
    if( fig_ref_dict != NULL ) {
        free_ref_dict( &fig_ref_dict );
    }
    if( fn_ref_dict != NULL ) {
        free_ref_dict( &fn_ref_dict );
    }
    if( hd_ref_dict != NULL ) {
        free_ref_dict( &hd_ref_dict );
    }
    if( ix_ref_dict != NULL ) {
        free_ref_dict( &ix_ref_dict );
    }
    if( tag_dict != NULL ) {
        free_tag_dict( &tag_dict );
    }
    if( index_dict != NULL ) {
        free_index_dict( &index_dict );
    }
    if( buff2 != NULL ) {
        mem_free( buff2 );
    }
    if( workbuf != NULL ) {
        mem_free( workbuf );
    }
    if( box_line != NULL ) {
        add_box_col_stack_to_pool( box_line );
    }
    if( cur_line != NULL ) {
        add_box_col_set_to_pool( cur_line );
    }
    if( prev_line != NULL ) {
        add_box_col_set_to_pool( prev_line );
    }
    if( t_line != NULL ) {
        add_text_chars_to_pool( t_line );
        add_text_line_to_pool( t_line );
    }
    if( t_element != NULL ) {
        clear_doc_element( t_element );
    }
    if( t_page.top_ban != NULL ) {
        clear_doc_element( t_page.top_ban );
    }
    while( t_page.panes != NULL ) {
        if( t_page.panes->page_width != NULL ) {
            add_doc_el_to_pool( t_page.panes->page_width );
        }
        for( i = 0; i < MAX_COL; i++ ) {
            if( t_page.panes->cols != NULL ) {
                if( t_page.panes->cols[i].col_width != NULL ) {
                    add_doc_el_to_pool( t_page.panes->cols[i].col_width );
                }
                if( t_page.panes->cols[i].main != NULL ) {
                    add_doc_el_to_pool( t_page.panes->cols[i].main );
                }
                if( t_page.panes->cols[i].bot_fig != NULL ) {
                    add_doc_el_to_pool( t_page.panes->cols[i].bot_fig );
                }
                if( t_page.panes->cols[i].footnote != NULL ) {
                    add_doc_el_to_pool( t_page.panes->cols[i].footnote );
                }
            }
        }
        sav_pane = t_page.panes->next;
        mem_free( t_page.panes );
        t_page.panes = sav_pane;
    }
    if( t_page.bot_ban != NULL ) {
        clear_doc_element( t_page.top_ban );
    }
    if( n_page.page_width != NULL ) {
        add_doc_el_group_to_pool( n_page.page_width );
    }
    if( n_page.col_width != NULL ) {
        add_doc_el_group_to_pool( n_page.col_width );
    }
    if( n_page.col_main != NULL ) {
        add_doc_el_to_pool( n_page.col_main );
    }
    if( n_page.col_bot != NULL ) {
        add_doc_el_group_to_pool( n_page.col_bot );
    }
    if( n_page.col_fn != NULL ) {
        add_doc_el_group_to_pool( n_page.col_fn );
    }

    free_layout();

    free_pool_storage();

}


/***************************************************************************/
/*  get line from current macro                                            */
/***************************************************************************/
static  void    get_macro_line( void )
{
    macrocb *   cb;

    if( input_cbs->fmflags & II_file ) {// current input is file not macro
        g_err( err_logic_mac );
        show_include_stack();
        err_count++;
        g_suicide();
    }
    cb = input_cbs->s.m;

    if( cb->macline == NULL ) {         // no more macrolines
        if( !ProcFlags.concat && (input_cbs->prev->fmflags & II_eol) &&
                ((input_cbs->prev->fmflags & II_file) ||
                 (input_cbs->prev->fmflags & II_macro)) ) {
            scr_process_break();
        }
        input_cbs->fmflags |= II_eof;
        input_cbs->fmflags &= ~(II_sol | II_eol);
        cb->flags          |= FF_eof;
        *buff2              = '\0';
    } else {
        cb->lineno++;
        if( ProcFlags.utc ) { 
            input_cbs->fmflags &= ~II_eol;
        } else if( ProcFlags.ct ) {
            input_cbs->fmflags &= ~II_sol;            
        } else if( (input_cbs->fmflags & II_macro) && input_cbs->fm_symbol
                && !input_cbs->sym_space ) {
            input_cbs->fmflags &= ~(II_sol | II_eol);
        } else {
            input_cbs->fmflags |= (II_sol | II_eol);
        }
        cb->flags          &= ~FF_eof;
        input_cbs->fmflags &= ~II_eof;
        strcpy_s( buff2, buf_size, cb->macline->value );
        cb->macline         = cb->macline->next;
    }
}


/***************************************************************************/
/*  get line from current input ( file )                                   */
/*  skipping lines before the first one to process if neccessary           */
/*                                                                         */
/*  returns  false for EOF                                                 */
/***************************************************************************/
bool    get_line( bool display_line )
{
    filecb      *   cb;
    char        *   p;
    inp_line    *   pline;

    if( ProcFlags.reprocess_line ) {    // there was an unget
        ProcFlags.reprocess_line = false;   // only used for :LAYOUT
        return( !(input_cbs->fmflags & II_eof) );
    }
    if( input_cbs->hidden_head != NULL ) {  // line was previously split,
        strcpy( buff2, input_cbs->hidden_head->value ); // take next part
        pline = input_cbs->hidden_head;
        input_cbs->hidden_head = input_cbs->hidden_head->next;

        input_cbs->fmflags &=  ~(II_sol | II_eol);  // clear current flags
        input_cbs->fmflags |= pline->fmflags;       // use flags from hidden_head
        input_cbs->fm_symbol = pline->fm_symbol;
        input_cbs->sym_space = pline->sym_space;

        mem_free( pline );

        if( input_cbs->hidden_head == NULL ) {  // last part of split line
            input_cbs->hidden_tail = NULL;
        }
    } else {
        if( input_cbs->pe_cb.count > 0 ) {  // .pe perform active
            strcpy( buff2, input_cbs->pe_cb.line );
            input_cbs->pe_cb.count--;
            if( input_cbs->pe_cb.count <= 0 ) {
                reset_pe_cb();
            }
        } else {
            if( input_cbs->fmflags & II_tag_mac ) {
                get_macro_line();       // input from macro line
            } else {
                cb = input_cbs->s.f;    // input from file
                if( !(cb->flags & FF_open) ) {
                    g_info( err_inf_reopen );
                    show_include_stack();
                    reopen_inc_fp( cb );
                }
                do {
                    fgetpos( cb->fp, &cb->pos );// remember position for label
                    p = fgets( buff2, buf_size, cb->fp );
                    if( p != NULL ) {
                        if( cb->lineno >= cb->linemax ) {
                            input_cbs->fmflags |= II_eof;
                            input_cbs->fmflags &= ~(II_sol | II_eol);
                            cb->flags |= FF_eof;
                            *buff2 = '\0';
                            break;
                        }
                        cb->lineno++;
                        input_cbs->fmflags |= (II_sol | II_eol);

                        if( cb->flags & FF_crlf ) {// try to delete CRLF at end
                            p += strlen( p ) - 1;
                            while( (*p == '\r') || (*p == '\n')  ) {
                                *p-- = '\0';
                            }
                        }
#if 1
                        if( ProcFlags.start_section && !ProcFlags.concat &&
                            (*buff2 == '\0') ) {
                            *buff2 = ' ';   // empty line gets 1 blank
                            *(buff2 + 1) = '\0';// requires more testing TBD
                        }
#endif
                    } else {
                        if( feof( cb->fp ) ) {
                            input_cbs->fmflags |= II_eof;
                            input_cbs->fmflags &= ~(II_sol | II_eol);
                            cb->flags |= FF_eof;
                            *buff2 = '\0';
                            break;
                        } else {
                            strerror_s( buff2, buf_size, errno );
                            g_err( err_file_io, buff2, cb->filename );

                            err_count++;
                            g_suicide();
                        }
                    }
                } while( cb->lineno < cb->linemin );
            }
        }
    }

    buff2_lg = strnlen_s( buff2, buf_size );
    *(buff2 + buff2_lg) = '\0';
    *(buff2 + buff2_lg + 1) = '\0';
    if( input_cbs->fmflags & II_file ) {
        input_cbs->s.f->usedlen = buff2_lg;
        if( GlobalFlags.research ) {    // research mode
            if( ProcFlags.researchfile ) {  // for single file
                if( input_cbs->fmflags & II_research ) {// research active
                    if( research_to < input_cbs->s.f->lineno ) {
                        input_cbs->fmflags &= ~II_research;// end of research range
                    }
                } else {                // not (yet) active
                    if( research_from == input_cbs->s.f->lineno ) {
                        if( NULL != strstr( input_cbs->s.f->filename,
                                            research_file_name) ) {
                        input_cbs->fmflags |= II_research;// start of research range
                        }
                    }
                }
            } else {
                input_cbs->fmflags |= II_research;
            }
        }
    }

    if( !(input_cbs->fmflags & II_eof) ) {
        if( display_line && GlobalFlags.firstpass
            && (input_cbs->fmflags & II_research) ) {
            printf( "%s\n", buff2 );
        }
    }
    return( !(input_cbs->fmflags & II_eof) );
}


/***************************************************************************/
/*  output the filenames + lines which were included                       */
/***************************************************************************/

void    show_include_stack( void )
{
    inputcb *   ip;
    char        linestr[MAX_L_AS_STR];
    char        linemac[MAX_L_AS_STR];


    if( input_cbs != NULL ) {
        if( input_cbs->fmflags & II_tag_mac ) {
            ulongtodec( input_cbs->s.m->lineno, linestr );
            ulongtodec( input_cbs->s.m->mac->lineno, linemac );
            g_info( err_inf_mac_def, linestr, input_cbs->s.m->mac->name,
                    linemac, input_cbs->s.m->mac->mac_file_name);
//      } else {
//          ulongtodec( input_cbs->s.f->lineno, linestr );
//          g_info( inf_file_line, linestr, input_cbs->s.f->filename );
        }
        ip = input_cbs->prev;
    } else {
        ip = NULL;
    }
    out_msg( "\n" );
    while( ip != NULL ) {
        switch( ip->fmflags & II_input ) {
        case    II_file:
            ulongtodec( ip->s.f->lineno, linestr );
            g_info( err_inf_line_file, linestr, ip->s.f->filename );
            break;
        case    II_tag :
            g_info( err_inf_tag, ip->s.m->tag->name );
            // fallthrough
        case    II_macro :
            ulongtodec( ip->s.m->lineno, linestr );
            ulongtodec( ip->s.m->mac->lineno, linemac );
            g_info( err_inf_mac_def, linestr, ip->s.m->mac->name,
                    linemac, ip->s.m->mac->mac_file_name);
            break;
        default:
            g_info( err_inc_unknown );
            break;
        }
        ip = ip->prev;
    }
    return;
}

