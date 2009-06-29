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
* Description: implement dummy support for not (yet) implemented
*               script control words and gml tags
*                    gml_dummy()
*                    scr_dummy()
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*  scr_dummy        processing                                            */
/***************************************************************************/

void    scr_dummy( void )
{
    char            cwcurr[4];          // control word string for msg

    cwcurr[0] = SCR_char;
    cwcurr[1] = *(scan_restart + 1);
    cwcurr[2] = *(scan_restart + 2);
    cwcurr[3] = '\0';

    scan_restart = scan_stop + 1;

    g_warn( WNG_UNSUPP_CW, cwcurr );
    wng_count++;
}


/***************************************************************************/
/*  gml_dummy        processing                                            */
/***************************************************************************/

void    gml_dummy( const gmltag * entry )
{

    scan_start = scan_stop + 1;

    g_warn( WNG_UNSUPP_TAG, entry->tagname );
    wng_count++;

}


