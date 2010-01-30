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


#include <rdos.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include "uidef.h"
#include "uishift.h"
#include "uimouse.h"

unsigned long global uiclock( void );

#define KEY_NUM_ACTIVE		0x200
#define KEY_CAPS_ACTIVE		0x100
#define KEY_PRINT_PRESSED	0x20
#define KEY_SCROLL_PRESSED	0x10
#define KEY_PAUSE_PRESSED	0x8
#define KEY_CTRL_PRESSED	0x4
#define KEY_ALT_PRESSED		0x2
#define KEY_SHIFT_PRESSED	0x1

#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14
#define VK_ESCAPE         0x1B
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
#define VK_0              '0'
#define VK_1              '1'
#define VK_2              '2'
#define VK_3              '3'
#define VK_4              '4
#define VK_5              '5'
#define VK_6              '6'
#define VK_7              '7'
#define VK_8              '8'
#define VK_9              '9'
#define VK_A              'A'
#define VK_B              'B'
#define VK_C              'C'
#define VK_D              'D'
#define VK_E              'E'
#define VK_F              'F'
#define VK_G              'G'
#define VK_H              'H'
#define VK_I              'I'
#define VK_J              'J'
#define VK_K              'K'
#define VK_L              'L'
#define VK_M              'M'
#define VK_N              'N'
#define VK_O              'O'
#define VK_P              'P'
#define VK_Q              'Q'
#define VK_R              'R'
#define VK_S              'S'
#define VK_T              'T'
#define VK_U              'U'
#define VK_V              'V'
#define VK_W              'W'
#define VK_X              'X'
#define VK_Y              'Y'
#define VK_Z              'Z
#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

int                  WaitHandle;

typedef struct {
    short int vk;
    short int reg;
    short int shift;
    short int ctrl;
    short int alt;
} map;

static const map events[] = {
    { VK_BACK, EV_RUB_OUT, EV_RUB_OUT, EV_RUB_OUT, EV_RUB_OUT },
    { VK_TAB, EV_TAB_FORWARD, EV_TAB_BACKWARD, EV_CTRL_TAB, EV_TAB_FORWARD },
    { VK_RETURN, EV_ENTER, EV_ENTER, EV_ENTER, EV_ENTER },
    { VK_ESCAPE, EV_ESCAPE, EV_ESCAPE, EV_ESCAPE, EV_ESCAPE },
    { VK_PRIOR, EV_PAGE_UP, EV_PAGE_UP, EV_CTRL_PGUP, EV_ALT_PGUP },
    { VK_NEXT, EV_PAGE_DOWN, EV_PAGE_DOWN, EV_CTRL_PGDN, EV_ALT_PGDN },
    { VK_END, EV_END, EV_END, EV_CTRL_END, EV_ALT_END },
    { VK_HOME, EV_HOME, EV_HOME, EV_CTRL_HOME, EV_ALT_HOME },
    { VK_LEFT, EV_CURSOR_LEFT, EV_CURSOR_LEFT, EV_CTRL_LEFT, EV_CURSOR_LEFT },
    { VK_UP, EV_CURSOR_UP, EV_CURSOR_UP, EV_CTRL_UP, EV_CURSOR_UP },
    { VK_RIGHT, EV_CURSOR_RIGHT, EV_CURSOR_RIGHT, EV_CTRL_RIGHT, EV_CURSOR_RIGHT },
    { VK_DOWN, EV_CURSOR_DOWN, EV_CURSOR_DOWN, EV_CTRL_DOWN, EV_CURSOR_DOWN },
    { VK_INSERT, EV_INSERT, EV_INSERT, EV_CTRL_INSERT, EV_ALT_INSERT},
    { VK_DELETE, EV_DELETE, EV_DELETE, EV_CTRL_DELETE, EV_ALT_DELETE },
    { 'A', 'a', 'A', 'A'-'A'+1, EV_ALT_A },
    { 'B', 'b', 'B', 'B'-'A'+1, EV_ALT_B },
    { 'C', 'c', 'C', 'C'-'A'+1, EV_ALT_C },
    { 'D', 'd', 'D', 'D'-'A'+1, EV_ALT_D },
    { 'E', 'e', 'E', 'E'-'A'+1, EV_ALT_E },
    { 'F', 'f', 'F', 'F'-'A'+1, EV_ALT_F },
    { 'G', 'g', 'G', 'G'-'A'+1, EV_ALT_G },
    { 'H', 'h', 'H', 'H'-'A'+1, EV_ALT_H },
    { 'I', 'i', 'I', 'I'-'A'+1, EV_ALT_I },
    { 'J', 'j', 'J', 'J'-'A'+1, EV_ALT_J },
    { 'K', 'k', 'K', 'K'-'A'+1, EV_ALT_K },
    { 'L', 'l', 'L', 'L'-'A'+1, EV_ALT_L },
    { 'M', 'm', 'M', 'M'-'A'+1, EV_ALT_M },
    { 'N', 'n', 'N', 'N'-'A'+1, EV_ALT_N },
    { 'O', 'o', 'O', 'O'-'A'+1, EV_ALT_O },
    { 'P', 'p', 'P', 'P'-'A'+1, EV_ALT_P },
    { 'Q', 'q', 'Q', 'Q'-'A'+1, EV_ALT_Q },
    { 'R', 'r', 'R', 'R'-'A'+1, EV_ALT_R },
    { 'S', 's', 'S', 'S'-'A'+1, EV_ALT_S },
    { 'T', 't', 'T', 'T'-'A'+1, EV_ALT_T },
    { 'U', 'u', 'U', 'U'-'A'+1, EV_ALT_U },
    { 'V', 'v', 'V', 'V'-'A'+1, EV_ALT_V },
    { 'W', 'w', 'W', 'W'-'A'+1, EV_ALT_W },
    { 'X', 'x', 'X', 'X'-'A'+1, EV_ALT_X },
    { 'Y', 'y', 'Y', 'Y'-'A'+1, EV_ALT_Y },
    { 'Z', 'z', 'Z', 'Z'-'A'+1, EV_ALT_Z },
    { VK_F1, EV_FUNC( 1 ), EV_SHIFT_FUNC( 1 ), EV_CTRL_FUNC( 1 ), EV_ALT_FUNC( 1 ) },
    { VK_F2, EV_FUNC( 2 ), EV_SHIFT_FUNC( 2 ), EV_CTRL_FUNC( 2 ), EV_ALT_FUNC( 2 ) },
    { VK_F3, EV_FUNC( 3 ), EV_SHIFT_FUNC( 3 ), EV_CTRL_FUNC( 3 ), EV_ALT_FUNC( 3 ) },
    { VK_F4, EV_FUNC( 4 ), EV_SHIFT_FUNC( 4 ), EV_CTRL_FUNC( 4 ), EV_ALT_FUNC( 4 ) },
    { VK_F5, EV_FUNC( 5 ), EV_SHIFT_FUNC( 5 ), EV_CTRL_FUNC( 5 ), EV_ALT_FUNC( 5 ) },
    { VK_F6, EV_FUNC( 6 ), EV_SHIFT_FUNC( 6 ), EV_CTRL_FUNC( 6 ), EV_ALT_FUNC( 6 ) },
    { VK_F7, EV_FUNC( 7 ), EV_SHIFT_FUNC( 7 ), EV_CTRL_FUNC( 7 ), EV_ALT_FUNC( 7 ) },
    { VK_F8, EV_FUNC( 8 ), EV_SHIFT_FUNC( 8 ), EV_CTRL_FUNC( 8 ), EV_ALT_FUNC( 8 ) },
    { VK_F9, EV_FUNC( 9 ), EV_SHIFT_FUNC( 9 ), EV_CTRL_FUNC( 9 ), EV_ALT_FUNC( 9 ) },
    { VK_F10, EV_FUNC( 10 ), EV_SHIFT_FUNC( 10 ), EV_CTRL_FUNC( 10 ), EV_ALT_FUNC( 10 ) },
    { VK_F11, EV_FUNC_11, EV_SHIFT_FUNC_11, EV_CTRL_FUNC_11, EV_ALT_FUNC_11 },
    { VK_F12, EV_FUNC_12, EV_SHIFT_FUNC_12, EV_CTRL_FUNC_12, EV_ALT_FUNC_12 }
};


static unsigned char        ShftState;
static int                  KeyInstalled;

static ORD                  currMouseRow;
static ORD                  currMouseCol;
static ORD                  currMouseStatus;

extern MOUSEORD             MouseRow;
extern MOUSEORD             MouseCol;
extern bool                 MouseOn;
extern bool                 MouseInstalled;
extern unsigned short int   MouseStatus;

static void setshiftstate( bool has_shift, bool has_ctrl, bool has_alt )
{
    ShftState = 0;
    if( has_shift ) {
        ShftState |= S_SHIFT;
    }
    if( has_ctrl ) {
        ShftState |= S_CTRL;
    }
    if( has_alt ) {
        ShftState |= S_ALT;
    }
}

int CompareEvents( const void *p1, const void *p2 )
{
    return( ((map*)p1)->vk - ((map*)p2)->vk );
}

static int IsStdKey( int ExtKey, int VirtualKey )
{
    if (ExtKey & 0x8000)
        return FALSE;

    switch (VirtualKey)
    {
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        case VK_CAPITAL:
        case VK_LWIN:
        case VK_RWIN:
        case VK_LSHIFT:
        case VK_RSHIFT:
        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_LMENU:
        case VK_RMENU:
            return FALSE;
    }
    return TRUE;
}

static EVENT KeyEventProc()
{
    int                 ext;
    int                 keystate;
    int                 vk;
    int                 scan;
    EVENT               ascii;
    bool                has_alt, has_shift, has_ctrl;
    map                 *ev;
    map                 what;

    if( RdosReadKeyEvent( &ext, &keystate, &vk, &scan ) ) {
        ascii = ext & 0x7F;
        has_shift = keystate & KEY_SHIFT_PRESSED;
        has_ctrl = keystate & KEY_CTRL_PRESSED;
        has_alt = keystate & KEY_ALT_PRESSED;
        setshiftstate( has_shift, has_ctrl, has_alt );
        what.vk = (short int)vk;

        if( IsStdKey( ext, vk ) ) {
            ev = bsearch( &what, events, sizeof( events )/sizeof( map ),
                      sizeof( what ), CompareEvents );
            if( ev != NULL ) {
                if( has_shift ) {
                    ascii = ev->shift;
                } else if( has_ctrl ) {
                    ascii = ev->ctrl;
                } else if( has_alt ) {
                    ascii = ev->alt;
                } else {
                    ascii = ev->reg;
                }
            } else if( ascii == 0 ) {
                ascii = EV_NO_EVENT;
            }
        } else
            return( EV_NO_EVENT );

        if( ascii > EV_NO_EVENT ) {
            uihidemouse();
        }
        return( ascii );

    } else
        return( EV_NO_EVENT );
}

static EVENT MouseEventProc()
{
    ORD stat = 0;
    int row;
    int col;
    
    if( RdosGetLeftButton() )
        stat |= MOUSE_PRESS;

    if( RdosGetRightButton() )
        stat |= MOUSE_PRESS_RIGHT;

    RdosGetMousePosition(  &col, &row );

    if( stat != currMouseStatus ) {
        if( !(stat & MOUSE_PRESS) && (currMouseStatus & MOUSE_PRESS) )
            RdosGetLeftButtonReleasePosition( &col, &row );
    
        if( !(stat & MOUSE_PRESS_RIGHT) && (currMouseStatus & MOUSE_PRESS_RIGHT) )
            RdosGetRightButtonReleasePosition( &col, &row );
    
        if( (stat & MOUSE_PRESS) && !(currMouseStatus & MOUSE_PRESS) )
            RdosGetLeftButtonPressPosition( &col, &row );
    
        if( (stat & MOUSE_PRESS_RIGHT) && !(currMouseStatus & MOUSE_PRESS_RIGHT) )
            RdosGetRightButtonPressPosition( &col, &row );
    }
    currMouseRow = row;
    currMouseCol = col;
    currMouseStatus = stat;
    
    return mouseevent();        
}

bool intern initkeyboard( void )
{
    if( !KeyInstalled ) {
        if( WaitHandle == 0 )
            WaitHandle = RdosCreateWait();

        RdosAddWaitForKeyboard( WaitHandle, &KeyEventProc );
    }
    KeyInstalled = TRUE;
    
    return( TRUE );
}

void intern finikeyboard( void )
{
    if( KeyInstalled ) {
        if( !MouseInstalled ) {
            RdosCloseWait( WaitHandle );
            WaitHandle = 0;
        }
    }
    KeyInstalled = FALSE;
}

void intern flushkey( void )
{
    RdosClearKeyboard();
}

void intern kbdspawnstart( void )
{
    finikeyboard();
}

void intern kbdspawnend( void )
{
    initkeyboard();
}

void intern mousespawnstart( void )
{
    uihidemouse();
}

void intern mousespawnend( void )
{
}

void uimousespeed( unsigned speed )
{
    if( (int)speed <= 0 ) {
        speed = 1;
    }
}

bool global initmouse( int install )
{
    unsigned long tmp;

    if( install == 0 ) {
        return( FALSE );
    }
    UIData->mouse_xscale = 8;
    UIData->mouse_yscale = 8;

    if( !MouseInstalled ) {
        if( WaitHandle == 0 )
            WaitHandle = RdosCreateWait();

        RdosAddWaitForMouse( WaitHandle, &MouseEventProc );
        RdosSetMouseWindow( 0, 0, 8 * 80 - 1, 8 * 25 - 1 );
        RdosSetMouseMickey( 8, 8 );
    }

    MouseOn = FALSE;
    MouseInstalled = TRUE;

    UIData->mouse_swapped = FALSE;
    checkmouse( &MouseStatus, &MouseRow, &MouseCol, &tmp );
    return( MouseInstalled );
}

void intern finimouse( void )
{
    if( MouseInstalled ) {
        uioffmouse();

        if( !KeyInstalled ) {
            RdosCloseWait( WaitHandle );
            WaitHandle = 0;
        }
        MouseInstalled = FALSE;
    }
}

void global uisetmouseposn( ORD row, ORD col )
{
    uisetmouse( row, col );
}

void intern checkmouse( unsigned short *pstatus, MOUSEORD *prow,
                        MOUSEORD *pcol, unsigned long *ptime )
{
    *pstatus = currMouseStatus;
    *prow = currMouseRow;
    *pcol = currMouseCol;
    *ptime = uiclock();
    uisetmouse( *prow, *pcol );
}

unsigned char global uicheckshift( void )
/***************************************/
{
    return( ShftState );
}
