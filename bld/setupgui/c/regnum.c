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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "setup.h"
#include "setupinf.h"
#include "gendlg.h"
#include "genvbl.h"

#if defined( WSQL )
signed char ConvertLetterToBitPattern( char letter )
/*******************************************/
{
    if( letter < 'A' || letter > 'R'
    || letter == 'I' || letter == 'O' ) return -1;
    if( letter >= 'O' ) letter--;
    if( letter >= 'I' ) letter--;
    return( letter - 'A' );
}


// format of Registration Number: Sxxyy
//   where xx is product code S4, S8, 16, 32, SU imply concurrent use license
//     and yy is encoded 8-bit product specification
// the 8 bits are S C A M R P T 0
//   where S - server can be installed
//   where C - client can be installed
//   where A - standalone can be installed
//   where M - remote can be installed
//   where R - runtime can be installed
//   where P - professional server can be installed
//   where T - professional seat can be installed

extern bool ParseRegNumber( char *number )
/****************************************/
{
    unsigned short      bit_pattern;
    signed char         result;

    if( number == NULL ) return FALSE;
    if( strlen( number ) < 5 ) return FALSE;
    strupr( number );
    if( number[ 0 ] != 'S' ) return FALSE;

    result = ConvertLetterToBitPattern( number[ 3 ] );
    if( result == -1 ) return FALSE;
    bit_pattern = result << 4;
    result = ConvertLetterToBitPattern( number[ 4 ] );
    if( result == -1 ) return FALSE;
    bit_pattern |= result;

    if( ( bit_pattern & 0xf6 ) == 0xf6 ) {
        SetVariableByName( "$CanInstallPro", "1" );
        SetVariableByName( "IsMultiInstall", "1" );     // push script on stack
    } else if( ( bit_pattern & 0x44 ) == 0x44 ) {
        SetVariableByName( "$CanInstallProCS", "1" );
        SetVariableByName( "IsMultiInstall", "1" );
    } else if( ( bit_pattern & 0xf0 ) == 0xf0 ) {
        SetVariableByName( "$CanInstallAll", "1" );
        SetVariableByName( "IsMultiInstall", "1" );     // push script on stack
    } else if( ( bit_pattern & 0xc0 ) == 0xc0 ) {
        SetVariableByName( "$CanInstallCS", "1" );
        SetVariableByName( "IsMultiInstall", "1" );
    } else {
        // can be only a single bit on
        if( ( bit_pattern & 0x80 ) == 0x80 ) {  // server
            SetVariableByName( "DoPickSrv", "1" );
        } else if( ( bit_pattern & 0x40 ) == 0x40 ) { // client
            SetVariableByName( "DoPickClt", "1" );
        } else if( ( bit_pattern & 0x20 ) == 0x20 ) { // standalone
            SetVariableByName( "DoPickStd", "1" );
        } else if( ( bit_pattern & 0x10 ) == 0x10 ) { // remote
            SetVariableByName( "DoPickRem", "1" );
        } else if( ( bit_pattern & 0x08 ) == 0x08 ) { // runtime
            SetVariableByName( "DoPickRun", "1" );
        } else if( ( bit_pattern & 0x04 ) == 0x04 ) { // pro-server
            SetVariableByName( "DoPickPsv", "1" );
        } else if( ( bit_pattern & 0x02 ) == 0x02 ) { // pro-seat
            SetVariableByName( "DoPickPst", "1" );
        } else {
            return( FALSE );
        }
    }

    if( strnicmp( &number[ 1 ], "S4", 2 ) == 0 ||
        strnicmp( &number[ 1 ], "S8", 2 ) == 0 ||
        strnicmp( &number[ 1 ], "16", 2 ) == 0 ||
        strnicmp( &number[ 1 ], "32", 2 ) == 0 ||
        strnicmp( &number[ 1 ], "SU", 2 ) == 0 ||
        ( number[1] == 'S' && isdigit( number[2] ) ) ) {
        SetVariableByName( "$IsLicConCur", "1" );
    } else {
        SetVariableByName( "$IsLicPerSeat", "1" );
    }

    return( TRUE );
}


void EvalRegNumber()
/******************/
{
    a_dialog_header     *dlg;

    // check dialog condition
    dlg = FindDialogByName( "EvalRegNumber" );
    if( dlg == NULL ) {
        return;
    }
    if( dlg->condition != NULL && !EvalCondition( dlg->condition ) ) {
        return;
    }

    if( !ParseRegNumber( GetVariableStrVal( "$RegNumber" ) ) ) {
        SetVariableByName( "$BadRegNum", "1" );
    } else {
        SetVariableByName( "$BadRegNum", "0" );
    }
}
#endif

