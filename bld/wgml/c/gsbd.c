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
* Description:  WGML implement .bd script control word
*                    only line option implemented
*
****************************************************************************/
#include    "wgml.h"
#include    "gvars.h"


/**************************************************************************/
/* BOLD boldfaces the specified number of input text lines.               */
/*                                                                        */
/*      旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커      */
/*      |       |                                                  |      */
/*      |  .BD  |    <1|n|ON|OFF|line>                             |      */
/*      |       |                                                  |      */
/*      읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸      */
/*                                                                        */
/*                                                                        */
/* n:  The next "n" input text lines  are to be made Bold by overstriking */
/*    each character with itself.    If "n" is omitted,  a value  of 1 is */
/*    assumed.                                                            */
/* ON:  All following input text lines are to be made Bold.               */
/* OFF:  Terminates  the boldface  effect,  whether  initiated by  "n" or */
/*    "ON".                                                               */
/* line:  The  boldface effect will be  applied to the  resulting "line". */
/*    If "line"  starts with a control  word indicator,  the  Bold action */
/*    will start one blank after the control word at the start of "line", */
/*    and the result of overstriking must  fit within the maximum allowed */
/*    input line  length.   This restriction does  not apply to  a "line" */
/*    operand of text or text following a numeric or "ON" operand.        */
/*                                                                        */
/* This control word does not cause a break.  BOLD operates independently */
/* of other control words that modify text.    When more than one of .BD, */
/* .BI,  .UC,  .UL,  .UP,  or .US are  in effect,  the result is the best */
/* equivalent of the  sum of the effects.    They may be disabled  in any */
/* order;  the result  will be the best  equivalent of the ones  still in */
/* effect.   The .UD "INCLUDE/IGNORE/SET" characters  may be used to turn */
/* the function on and  off within a "line" or in  subsequent input text. */
/* See .UD for defaults and details.                                      */
/*                                                                        */
/* NOTES                                                                  */
/* (1) For output devices  that support multiple fonts,   the output from */
/*     this  control word  is  not necessarily  bold  but  may merely  be */
/*     selected from a different font.                                    */
/* (2) Text may also be emphasized with the &'bold( function.             */
/* (3) Do not  attempt to use a  numeric expression as a  "line" operand, */
/*     because  it  will be  taken  as  an  "n" operand.    For  example, */
/*     ".BD (1988)" will boldface  the next 1988 input lines  of text and */
/*     not the string  "(1988)".   Use ".BD 1;(1988)" or  ".BD;(1988)" or */
/*     &'bold(1988) instead.                                              */
/*                                                                        */
/* EXAMPLES                                                               */
/* (1) This is                                                            */
/*     .BD Bold text                                                      */
/*     and this                                                           */
/*     .BD ON;is more Bold                                                */
/*     output text.                                                       */
/*     .BD OFF                                                            */
/*     .BD .CE BOLD END                                                   */
/*     produces:  This is Bold text and this is more Bold output text.    */
/*                                  BOLD END                              */
/* (2) Text comes                                                         */
/*     .BD ON;in Boldface                                                 */
/*     .US ON;and Underscored                                             */
/*     .UP ON;and Uppercase/Underscored                                   */
/*     .US OFF;and Uppercase                                              */
/*     .UP OFF;and so                                                     */
/*     .BD OFF;on.                                                        */
/*     produces:     Text  comes   in   Boldface   and  Underscored   AND */
/*     UPPERCASE/UNDERSCORED AND UPPERCASE and so on.                     */
/**************************************************************************/

void scr_bd( void )
{
    char    *   p = scan_start + 1;

    if( *p ) {
        process_text( p, 2 );           // hilite the remaining text
    }
    scan_restart = scan_stop + 1;
    return;
}
