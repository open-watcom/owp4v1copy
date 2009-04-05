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
* Description:  screen/keyboard/color helper routines for VI
*
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef __WATCOMC__
#include <conio.h>
#endif
#include "vi.h"
#include "win.h"
#include "dosx.h"
#include "pragmas.h"
#ifdef __CURSES__
#include "curses.h"
#endif

static int saveRow,saveCol;
int PageCnt=0;

/*
 * getCursor - get cursor position
 */
static void getCursor( int *row, int *col )
{
    int x;

    x = BIOSGetCursor( VideoPage );
    *row = (x >> 8);
    *col = x & 0xff;
} /* getCursor */

/*
 * setCursor - set cursor position
 */
static void setCursor( int row, int col )
{
    BIOSSetCursor( VideoPage, row, col );
} /* SetCursorOnScreen */

/*
 * KillCursor - do just that, get rid of it
 */
void KillCursor( void )
{
    getCursor( &saveRow,&saveCol );
    setCursor( WindMaxHeight, 0 );

} /* KillCursor */

#if 0
/*
 * TurnOffCursor - as it sounds
 */
void TurnOffCursor( void )
{
    SetCursorOnScreen( WindMaxHeight,0 );

} /* TurnOffCursor */
#endif

/*
 * RestoreCursor - bring back a killed cursor
 */
void RestoreCursor( void )
{
    setCursor( saveRow, saveCol );

} /* RestoreCursor */

#if !defined(__UNIX__) && !defined(__OS2__) && !defined(__NT__)
    extern void JustAnInt28( void );
    #pragma aux JustAnInt28 = 0xcd 0x28;
#endif

/*
 * KeyboardHit - test for keyboard hit
 */
bool KeyboardHit( void )
{
    bool        rc;

    rc = BIOSKeyboardHit( EditFlags.ExtendedKeyboard + 1 );
#if !defined(__UNIX__) && !defined(__OS2__) && !defined(__NT__)
    if( !rc ) {
        #if defined(__386__)  && !defined(__4G__)
            extern void UpdateDOSClock( void );
            UpdateDOSClock();
        #endif
        JustAnInt28();
    }
#endif
    return( rc );
} /* KeyboardHit */

/*
 * GetKeyboard - get a keyboard char
 */
int GetKeyboard( int *scan )
{
    U_INT       key;

    key = BIOSGetKeyboard( EditFlags.ExtendedKeyboard );
    if( scan != NULL ) {
        *scan = (key>>8);
    }
#ifndef __NT__  /* NT already returns cooked key codes, not scan/key combos */
    key &= 0xff;
    if( key == 0xe0 ) {
        return( 0 );
    }
#endif
    return( key );

} /* GetKeyboard */

int SetFont( char *data )
{
    data = data;
    return( ERR_NO_ERR );
}

/*
 * ClearScreen - clear the screen
 */
void ClearScreen( void )
{
#ifdef __CURSES__
    clear();
#else
    int                 i;
    char_info           what;
    char_info           _FAR *foo;

    if( EditFlags.Quiet ) {
        return;
    }
    foo = (char_info _FAR *) Scrn;
    what.ch = ' ';
    what.attr = ExitAttr;
    for( i=WindMaxWidth*WindMaxHeight-1;i>=0;i-- ) {
        WRITE_SCREEN( *foo, what );
        foo++;
    }
#ifdef __VIO__
    MyVioShowBuf( 0, WindMaxWidth*WindMaxHeight );
#endif
#endif
    setCursor( 0, 0 );
} /* ClearScreen */

/*
 * GetClockStart - get clock start position
 */
void GetClockStart( void )
{
    ClockStart = &Scrn[ sizeof(char_info)*(ClockX+ClockY*WindMaxWidth)];

} /* GetClockStart */

/*
 * GetSpinStart - get spinner start position
 */
void GetSpinStart( void )
{
    SpinLoc = &Scrn[ sizeof(char_info)*(SpinX+SpinY*WindMaxWidth)];

} /* GetSpinStart */

/*
 * SetPosToMessageLine - set cursor position
 */
void SetPosToMessageLine( void )
{
    setCursor( WindMaxHeight - 1, 0 );

} /* SetPosToMessageLine */

/*
 * SetGenericWindowCursor - put cursor in any window at (l,c)
 */
void SetGenericWindowCursor( window_id wn, int l, int c )
{
    wind        *w;
    int         row,col;

    w = Windows[ wn ];

    row = w->y1;
    col = w->x1;

    row += l;
    col += c;
    if( !w->has_border ) {
        row--;
        col--;
    }
    setCursor( row, col );

} /* SetGenericWindowCursor */

void HideCursor( void )
{
    setCursor( -1, -1 );
}
