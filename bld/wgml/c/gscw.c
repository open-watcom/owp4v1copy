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
* Description: implement .cw script control word  separator
*
*  comments are from script-tso.txt
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"


/***************************************************************************/
/* CONTROL WORD SEPARATOR defines the character  that SCRIPT is to recog-  */
/* nize as  a "logical line  end" so that  multiple control words  may be  */
/* entered on one physical input line.                                     */
/*                                                                         */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커       */
/*      |       |                                                  |       */
/*      |  .CW  |    <;|character>                                 |       */
/*      |       |                                                  |       */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸       */
/*                                                                         */
/* This control word does not cause a break.  All subsequent control-word  */
/* input lines  are examined for the  control word separator.    If found  */
/* then  the physical  input line  is  logically divided  into two  input  */
/* lines,  the second of which starts at the first column after the sepa-  */
/* rator.   The control-word-separator  scanning rule is then  applied to  */
/* this resulting second  input line.   The initial  default control word  */
/* separator character is the semi-colon (;).  Any character or two-digit  */
/* hexadecimal value may be specified.                                     */
/*                                                                         */
/* NOTES                                                                   */
/* (1) The last "logical" input line may be a text line.                   */
/* (2) If substitution (.SU) of Set Symbols is ON and the input line is a  */
/*     control-word line,   the substitution scan  will terminate  at the  */
/*     first control word separator.   The remainder  of the line will be  */
/*     substituted when it is processed later as an input line.            */
/* (3) Scanning for  the control  word separator may  be suspended  on an  */
/*     input control line if a single quote (',  the apostrophe)  immedi-  */
/*     ately  follows  the  control-word  indicator.    This  allows  the  */
/*     control-word separator to be used as operand text.  It also allows  */
/*     an entire input line to be substituted with set symbols.            */
/* (4) If the  operand is omitted,  the  existence of and scanning  for a  */
/*     control-word separator is terminated.  Therefore, multiple control  */
/*     words will not be recognized on subsequent input lines,  nor may a  */
/*     control word be followed by text on the same input line.            */
/* (5) Disabling the separator character is the same as ".CW 00".          */
/*                                                                         */
/* EXAMPLES                                                                */
/* (1) .'bd This is text; it contains a semi-colon.                        */
/*     produces:  This is text; it contains a semi-colon.                  */
/*     The  control word  modifier  (')  suspends  control-word-separator  */
/*     scanning and  allows semi-colons  to be  entered in  the operands.  */
/*     The  same  input line  without  the  control word  modifier  would  */
/*     produce:  This is text                                              */
/*      it contains a semi-colon.                                          */
/* (2) .sr i = &i + 1;The value of I is "&i".                              */
/*     The value of "i"  is incremented by one and the  result is used in  */
/*     the text input line that follows.                                   */
/* (3) .sk;.of 4;(a) This is text in a list.                               */
/* (4) .cw #;.rt top /left;/center;/right;/#.cw ;#.sk;.cm                  */
/*     This example makes the separator "#",  defines a TOP running title  */
/*     containing semi-colons,  redefines  the separator to be  ";",  and  */
/*     then uses it.                                                       */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/


static void CW_sep_err( void )
{
    char    linestr[ MAX_L_AS_STR ];

    err_count++;
    g_err( err_inv_cw_sep );
    if( input_cbs->fmflags & II_macro ) {
        utoa( input_cbs->s.m->lineno, linestr, 10 );
        g_info( inf_mac_line, linestr, input_cbs->s.m->mac->name );
    } else {
        utoa( input_cbs->s.f->lineno, linestr, 10 );
        g_info( inf_file_line, linestr, input_cbs->s.f->filename );
    }
    show_include_stack();
    return;
}


/***************************************************************************/
/*  scr_cw    implement .cw control word                                   */
/***************************************************************************/

void    scr_cw( void )
{
    char        *   pa;
    char        *   p;
    char            c;
    int             len;


    p = scan_start;
    while( *p && *p != ' ' ) {          // over cw
        p++;
    }
    while( *p && *p == ' ' ) {          // next word start
        p++;
    }
    pa = p;

    while( *p && *p != ' ' ) {          // end of word
        p++;
    }
    len = p - pa;
    if( len > 2 ) {
        err_count++;
        CW_sep_err();
        return;
    } else if( len == 2 ) {             // 2 hex characters
        if( isxdigit( *pa ) && isxdigit( *(pa + 1) ) ) {
            c = '\0';
            for( ; len > 0; len-- ) {
                c *= 16;
                if( isdigit( *pa ) ) {
                    c += *pa - '0';
                } else {
                    c += toupper( *pa ) - 'A' + 10;
                }
                pa++;
            }
            CW_sep_char = c;
        } else {
            err_count++;
            CW_sep_err();
        }
    } else if( len == 1 ) {
        CW_sep_char = *pa;
    } else {
        CW_sep_char = '\0';
    }
    return;
}
