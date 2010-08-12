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
* Description:  Basic screen initialization for RDOS
*
****************************************************************************/


#include <rdos.h>
#include <stdlib.h>
#include <conio.h>
#include "uidef.h"
#include <stdio.h>

#define __B000  0x263
#define __B800  0x26B

static MONITOR ui_data = {
    25,
    80,
    M_VGA,
    NULL,
    NULL,
    NULL,
    NULL,
    4,
    1
};

bool global uiset80col( void )
{
    return( TRUE );
}

int intern initbios( void )
{
    if( UIData == NULL ) {
        UIData = &ui_data;
    }

    UIData->colour = M_VGA;

    UIData->screen.origin = malloc( UIData->width * UIData->height * sizeof( PIXEL ) );
    UIData->screen.increment = UIData->width;
    uiinitcursor();
    initkeyboard();
    UIData->mouse_acc_delay = 250;
    UIData->mouse_rpt_delay = 100;
    UIData->mouse_clk_delay = 250;
    UIData->tick_delay = 500;
    UIData->mouse_speed = 8;

    return( TRUE );
}

unsigned global uiclockdelay( unsigned milli )
{
    return( 1192 * milli);
}

void intern finibios( void )
{
    uifinicursor();
    finikeyboard();
}

void intern physupdate( SAREA *area )
{
    __segment screen = __B800;
    short int __based( void ) * scrptr;
    int i;
    int j;
    int pos;
    short int ach;
    short int *bufptr = (short int *)UIData->screen.origin;

    for( i = 0; i < area->height; i++ ) {
        pos = UIData->width * (i + area->row) + area->col;
        scrptr = (short int __based( void ) *)(2 * pos);
        bufptr = (short int *)UIData->screen.origin + pos;

        for( j = 0; j < area->width; j++ ) {
            ach = *bufptr;
            *(screen:>scrptr) = ach;
            bufptr++;
            scrptr++;
        }
    }    
}

int global uiisdbcs( void )
{
    return( FALSE );
}

int global uicharlen( int ch )
{
    return( 1 );
}
