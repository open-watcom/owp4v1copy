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
* Description:  WGML tag  :PB trial implementation
*                         PB is not documented, so this is guessing
****************************************************************************/


#include    "wgml.h"


/***************************************************************************/
/*  :PB.                                                                   */
/*                                                                         */
/*  not documented in Reference Manual                                     */
/*  Probable full name: Paragraph Break                                    */
/*  when used after P, PB, PC, or LP: passess g_post_skip on to next       */
/*    document element                                                     */
/*  otherwise, uses the g_post_skip as a pre_skip                          */
/***************************************************************************/

extern  void    gml_pb( const gmltag * entry )
{
    bool        in_para = ProcFlags.para_has_text;
    char    *   p;

    scan_err = false;
    p = scan_start;

    start_doc_sect();                   // if not already done
    scr_process_break();                // clears ProcFlags.para_has_text

    if( in_para ) {
        ProcFlags.skips_valid = true;   // keep existing skips inside paragraph
        ProcFlags.para_has_text = true; // reset flag, still in paragraph
    } else {
        ProcFlags.skips_valid = false;  // convert g_post_skip to g_subs_skip
    }

    SkipDot( p );                       // over '.'
    post_space = 0;
    if( *p != '\0' ) {
        process_text( p, g_curr_font );
    }

    scan_start = scan_stop + 1;
    return;
}

