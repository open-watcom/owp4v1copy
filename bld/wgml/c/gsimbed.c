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
* Description:  SCRIPT  .im  .ap   include file control words
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/*  .im   processing  IMBED                                                */
/*  format .im filename                                                    */
/*         .im nn        -> SYSUSR0n.GML                                   */
/***************************************************************************/

void    scr_im( void )
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

extern  void    scr_ap( void )
{
    input_cbs->s.f->flags |= FF_eof;    // simulate EOF for .append
    input_cbs->fmflags   |= II_eof;     // simulate EOF for .append
    scr_im();                           // do .imbed processing
}

