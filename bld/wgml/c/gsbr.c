
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
* Description: implement .br (break)  script control word
*                    and related routines                            TBD
*
*  comments are from script-tso.txt
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"
#include "copfiles.h"



/**************************************************************************/
/* BREAK forces  the current partially-full output  line (if any)   to be */
/* printed without  justification (if on),  and  a new output line  to be */
/* begun.                                                                 */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |  .BR  |    <line>                                        |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* The optional  "line" operand starts one  blank after the  control word */
/* and may be text or another control word.                               */
/*                                                                        */
/* NOTES                                                                  */
/* (1) Many other control words also cause  a Break.   No Break is neces- */
/*     sary  when one  of these  is  present.   The  description of  each */
/*     control word  indicates whether  it causes  a break,   and summary */
/*     lists are contained in other components of this document.          */
/* (2) A blank or tab in column one of  an input line has the effect of a */
/*     Break immediately before the line.    See Leading Blank (.LB)  and */
/*     Leading Tab (.LT) to control this automatic Break.                 */
/* (3) If Concatenate NO is in effect, all lines appear to be followed by */
/*     a Break.                                                           */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) This is a text line                                                */
/*     .br                                                                */
/*     followed by a .BR control word.                                    */
/*     produces:                                                          */
/*     This is a text line                                                */
/*     followed by a .BR control word.                                    */
/*                                                                        */
/*     Without the Break, it would print as:                              */
/*     This is a text line followed by a .BR control word.                */
/**************************************************************************/

void    scr_br( void )
{
    char        *   p;
#if 0
    char            cwcurr[4];          // if errmsg is neccessary

    cwcurr[0] = SCR_char;
    cwcurr[1] = 't';
    cwcurr[2] = 'i';
    cwcurr[3] = '\0';
#endif
    p = scan_start;
    while( *p && *p != ' ' ) {          // over cw
        p++;
    }
    if( *p ) {
        p++;                            // over space
        if( *p ) {
            split_input( scan_start, p );   // line operand
        }
    }
    scr_process_break();                // break processing

    scan_restart = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  output incomplete line if any                                          */
/***************************************************************************/

void  scr_process_break( void )
{
    if( t_line.first != NULL ) {
        process_line_full( &t_line, false );
    }
    return;

}



