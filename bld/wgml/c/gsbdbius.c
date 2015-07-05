/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML implement .us script control word
*                    only line option implemented
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"


/**************************************************************************/
/* UNDERSCORE underscores text in the specified number of input lines.    */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |  .US  |    <1|n|ON|OFF|line>                             |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/* This control  word does not  cause a  break.   The operands  and other */
/* considerations are identical to those for the other control words that */
/* emphasize text.  See .BD for details.                                  */
/*                                                                        */
/* NOTES                                                                  */
/* (1) For output devices  that support multiple fonts,   the output from */
/*     this control word is not  necessarily underscored.   It often will */
/*     display in an italic font, for example.                            */
/* (2) Text may also be emphasized with the &'italic( function.           */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) This is example number                                             */
/*     .us one.                                                           */
/*     produces:  This is example number one.                             */
/* (2) 1.2 &'italic('Chapter Title')                                      */
/*     produces:  1.2 Chapter Title                                       */
/* (3) .us on                                                             */
/*     This is a simple test of underscoring some                         */
/*     input text.                                                        */
/*     .ud on 40                                                          */
/*     And now we will underscore the interword blanks,                   */
/*     but justification can continue.                                    */
/*     Since the period is not underscored there                          */
/*     are still blanks between sentences.                                */
/*     .ud on . ?                                                         */
/*     Now even the sentences will be joined because                      */
/*     the special characters period and question mark                    */
/*     are also underscored.                                              */
/*     How does it look?                                                  */
/*          Note that leading blanks on a line are not                    */
/*     underscored.                                                       */
/*     The end.                                                           */
/*     .us off                                                            */
/*     produces:   This is a simple test of underscoring some input text. */
/*     And now we will underscore the interword blanks, but justification */
/*     can continue.  Since the period is not underscored there are still */
/*     blanks between sentences.   Now even  the sentences will be joined */
/*     because the special  characters period and question  mark are also */
/*     underscored.  How does it look?                                    */
/*          Note that leading blanks on a line are not underscored.   The */
/*     end.                                                               */
/**************************************************************************/

void scr_us( void )
{
    char    *   p = scan_start + 1;

    if( *p ) {
        process_text( p, 1 );           // hilite the remaining text
    }
    scan_restart = scan_stop + 1;
    return;
}

