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


/* Include new common control styles that require recent versions of Windows. */
#ifdef __NT__
    #undef _WIN32_IE
    #define _WIN32_IE   0x0600
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "spy.h"
#include <commdlg.h>
#ifdef __NT__
    #include <commctrl.h>
#endif

/*                                                               */
/* not included in MS WIN32 header files                         */
/* It looks like as MS remove support for this on WIN32 platform */
/*                                                               */
#ifndef CS_KEYCVTWINDOW
#define CS_KEYCVTWINDOW 0x0004
#endif
#ifndef CS_NOKEYCVT
#define CS_NOKEYCVT 0x0100
#endif

/*
 * Define common controls if they aren't already defined.
 */
#ifdef __NT__
    #ifndef MCS_NOTRAILINGDATES
        #define MCS_NOTRAILINGDATES         0x0040L
    #endif
    #ifndef MCS_SHORTDAYSOFWEEK
        #define MCS_SHORTDAYSOFWEEK         0x0080L
    #endif
    #ifndef MCS_NOSELCHANGEONNAV
        #define MCS_NOSELCHANGEONNAV        0x0100L
    #endif
    #ifndef PBS_MARQUEE
        #define PBS_MARQUEE                 0x0008L
    #endif
    #ifndef PBS_SMOOTHREVERSE
        #define PBS_SMOOTHREVERSE           0x0010L
    #endif
    #ifndef SBARS_TOOLTIPS
        #define SBARS_TOOLTIPS              0x0800L
    #endif
    #ifndef TTS_USEVISUALSTYLE
        #define TTS_USEVISUALSTYLE          0x0100L
    #endif
    #ifndef TBS_NOTIFYBEFOREMOVE
        #define TBS_NOTIFYBEFOREMOVE        0x0800L
    #endif
    #ifndef TBS_TRANSPARENTBKGND
        #define TBS_TRANSPARENTBKGND        0x1000L
    #endif
    #ifndef HDS_FLAT
        #define HDS_FLAT                    0x0200L
    #endif
    #ifndef HDS_CHECKBOXES
        #define HDS_CHECKBOXES              0x0400L
    #endif
    #ifndef HDS_NOSIZING
        #define HDS_NOSIZING                0x0800L
    #endif
    #ifndef HDS_OVERFLOW
        #define HDS_OVERFLOW                0x1000L
    #endif
#endif

/*
 * GetHexStr - convert a number to a hex string, padded out with 0's
 */
void GetHexStr( LPSTR res, DWORD num, int padlen )
{
    char        tmp[10];
    int         i;
    int         j,k;

    ultoa( num, tmp, 16 );
    i = strlen( tmp );
    k = 0;
    for( j=i;j<padlen;j++ ) {
        res[k++] = '0';
    }
    for( j=0;j<i;j++ ) {
        res[k++] = tmp[j];
    }

} /* GetHexStr */

/*
 * IsMyWindow - check if a specific window belongs to our task
 */
BOOL IsMyWindow( HWND hwnd )
{

    if( hwnd == NULL ) {
        return( FALSE );
    }
    if( GetWindowTask( hwnd ) == MyTask ) {
        if( hwnd == GetDesktopWindow() ) {
            return( FALSE );
        }
        return( TRUE );
    }
    return( FALSE );

} /* IsMyWindow */

/*
 * GetWindowName - set up a window name string
 */
void GetWindowName( HWND hwnd, char *str )
{
    int         len;
    char        name[64];

    if( hwnd == NULL ) {
        strcpy( str, GetRCString( STR_NONE ) );
        return;
    }
    len = GetWindowText( hwnd, name, sizeof( name ) );
    name[ len ] = 0;
    if( len == 0 ) {
        GetHexStr( str, (UINT) hwnd, 4 );
        str[4] = 0;
    } else {
        sprintf( str,"%0*x: %s", UINT_STR_LEN, (UINT) hwnd, name );
    }

} /* GetWindowName */

/*
 * GetWindowStyleString - get string corrosponding to window style bits
 */
void GetWindowStyleString( HWND hwnd, char *str, char *sstr )
{
    UINT        id;
    DWORD       style;
    WORD        wstyle;
    char        tmp[40];
    int         len;
    char        *rcstr;

    style = GetWindowLong( hwnd, GWL_STYLE );
    wstyle = (WORD) style;

    GetHexStr( str, style, 8 );
    str[8] = 0;
    sstr[0] = 0;

    if( style & WS_POPUP ) {
        strcat( sstr, "WS_POPUP " );
    }
    if( style & WS_CHILD ) {
        strcat( sstr, "WS_CHILD " );
        id = GET_ID( hwnd );
        rcstr = GetRCString( STR_CHILD_ID );
        sprintf( tmp, rcstr, id, UINT_STR_LEN, id );
        strcat( str, tmp );
    }
    if( !(style & (WS_POPUP|WS_CHILD) ) ) {
        strcat( sstr, "WS_OVERLAPPED " );
    }

    if( style & WS_BORDER ) {
        strcat( sstr, "WS_BORDER " );
    }
    if( style & WS_CAPTION ) {
        strcat( sstr, "WS_CAPTION " );
    }
    if( style & WS_SYSMENU ) {
        strcat( sstr, "WS_SYSMENU " );
    }
    if( style & WS_THICKFRAME ) {
        strcat( sstr, "WS_THICKFRAME " );
    }
    if( style & WS_MINIMIZEBOX ) {
        strcat( sstr, "WS_MINIMIZEBOX " );
    }
    if( style & WS_MAXIMIZEBOX ) {
        strcat( sstr, "WS_MAXIMIZEBOX " );
    }
    if( style & WS_MINIMIZE ) {
        strcat( sstr, "WS_MINIMIZE " );
    }
    if( style & WS_VISIBLE ) {
        strcat( sstr, "WS_VISIBLE " );
    }
    if( style & WS_DISABLED ) {
        strcat( sstr, "WS_DISABLED " );
    }
    if( style & WS_CLIPSIBLINGS ) {
        strcat( sstr, "WS_CLIPSIBLINGS " );
    }
    if( style & WS_CLIPCHILDREN ) {
        strcat( sstr, "WS_CLIPCHILDREN " );
    }
    if( style & WS_MAXIMIZE ) {
        strcat( sstr, "WS_MAXIMIZE " );
    }
    if( style & WS_DLGFRAME ) {
        strcat( sstr, "WS_DLGFRAME " );
    }
    if( style & WS_VSCROLL ) {
        strcat( sstr, "WS_VSCROLL " );
    }
    if( style & WS_HSCROLL ) {
        strcat( sstr, "WS_HSCROLL " );
    }
    if( style & WS_GROUP ) {
        strcat( sstr, "WS_GROUP " );
    }
    if( style & WS_TABSTOP ) {
        strcat( sstr, "WS_TABSTOP " );
    }

    len = GetClassName( hwnd, tmp, sizeof( tmp ) );
    tmp[ len ] = 0;
    if( !stricmp( tmp, "button" ) ) {
        if( style & BS_LEFTTEXT ) {
            strcat( sstr, "BS_LEFTTEXT " );
        }
        switch( wstyle & ~BS_LEFTTEXT ) {
        case BS_PUSHBUTTON:
            strcat( sstr, "BS_PUSHBUTTON " );
            break;
        case BS_DEFPUSHBUTTON:
            strcat( sstr, "BS_DEFPUSHBUTTON " );
            break;
        case BS_CHECKBOX:
            strcat( sstr, "BS_CHECKBOX " );
            break;
        case BS_AUTOCHECKBOX:
            strcat( sstr, "BS_AUTOCHECKBOX " );
            break;
        case BS_RADIOBUTTON:
            strcat( sstr, "BS_RADIOBUTTON " );
            break;
        case BS_3STATE:
            strcat( sstr, "BS_3STATE " );
            break;
        case BS_AUTO3STATE:
            strcat( sstr, "BS_AUTO3STATE " );
            break;
        case BS_GROUPBOX:
            strcat( sstr, "BS_GROUPBOX " );
            break;
        case BS_USERBUTTON:
            strcat( sstr, "BS_USERBUTTON " );
            break;
        case BS_AUTORADIOBUTTON:
            strcat( sstr, "BS_AUTORADIOBUTTON " );
            break;
        case BS_OWNERDRAW:
            strcat( sstr, "BS_OWNERDRAW " );
            break;
        }
    } else if( !stricmp( tmp, "edit" ) ) {
        if( style & ES_LEFT ) {
            strcat( sstr, "ES_LEFT " );
        }
        if( style & ES_CENTER ) {
            strcat( sstr, "ES_CENTER " );
        }
        if( style & ES_RIGHT ) {
            strcat( sstr, "ES_RIGHT " );
        }
        if( style & ES_MULTILINE ) {
            strcat( sstr, "ES_MULTILINE " );
        }
        if( style & ES_UPPERCASE ) {
            strcat( sstr, "ES_UPPERCASE " );
        }
        if( style & ES_LOWERCASE ) {
            strcat( sstr, "ES_LOWERCASE " );
        }
        if( style & ES_PASSWORD ) {
            strcat( sstr, "ES_PASSWORD " );
        }
        if( style & ES_AUTOVSCROLL ) {
            strcat( sstr, "ES_AUTOVSCROLL " );
        }
        if( style & ES_AUTOHSCROLL ) {
            strcat( sstr, "ES_AUTOHSCROLL " );
        }
        if( style & ES_NOHIDESEL ) {
            strcat( sstr, "ES_NOHIDESEL " );
        }
        if( style & ES_OEMCONVERT ) {
            strcat( sstr, "ES_OEMCONVERT " );
        }
        if( style & ES_READONLY ) {
            strcat( sstr, "ES_READONLY " );
        }
    } else if( !stricmp( tmp, "static" ) ) {
        if( style & SS_NOPREFIX ) {
            strcat( sstr, "SS_NOPREFIX " );
        }
        switch( wstyle & ~SS_NOPREFIX ) {
        case SS_LEFT:
            strcat( sstr, "SS_LEFT " );
            break;
        case SS_CENTER:
            strcat( sstr, "SS_CENTER " );
            break;
        case SS_RIGHT:
            strcat( sstr, "SS_RIGHT " );
            break;
        case SS_ICON:
            strcat( sstr, "SS_ICON " );
            break;
        case SS_BLACKRECT:
            strcat( sstr, "SS_BLACKRECT " );
            break;
        case SS_GRAYRECT:
            strcat( sstr, "SS_GRAYRECT " );
            break;
        case SS_WHITERECT:
            strcat( sstr, "SS_WHITERECT " );
            break;
        case SS_BLACKFRAME:
            strcat( sstr, "SS_BLACKFRAME " );
            break;
        case SS_GRAYFRAME:
            strcat( sstr, "SS_GRAYFRAME " );
            break;
        case SS_WHITEFRAME:
            strcat( sstr, "SS_WHITEFRAME " );
            break;
//      case SS_USERITEM:
//          strcat( sstr, "SS_USERITEM " );
//          break;
        case SS_SIMPLE:
            strcat( sstr, "SS_SIMPLE " );
            break;
        case SS_LEFTNOWORDWRAP:
            strcat( sstr, "SS_LEFTNOWORDWRAP " );
            break;
        }
    } else if( !stricmp( tmp, "listbox" ) ) {
        if( style & LBS_NOTIFY ) {
            strcat( sstr, "LBS_NOTIFY " );
        }
        if( style & LBS_SORT ) {
            strcat( sstr, "LBS_SORT " );
        }
        if( style & LBS_NOREDRAW ) {
            strcat( sstr, "LBS_NOREDRAW " );
        }
        if( style & LBS_MULTIPLESEL ) {
            strcat( sstr, "LBS_MULTIPLESEL " );
        }
        if( style & LBS_OWNERDRAWFIXED ) {
            strcat( sstr, "LBS_OWNERDRAWFIXED " );
        }
        if( style & LBS_OWNERDRAWVARIABLE ) {
            strcat( sstr, "LBS_OWNERDRAWVARIABLE " );
        }
        if( style & LBS_HASSTRINGS ) {
            strcat( sstr, "LBS_HASSTRINGS " );
        }
        if( style & LBS_USETABSTOPS ) {
            strcat( sstr, "LBS_USETABSTOPS " );
        }
        if( style & LBS_NOINTEGRALHEIGHT ) {
            strcat( sstr, "LBS_NOINTEGRALHEIGHT " );
        }
        if( style & LBS_MULTICOLUMN ) {
            strcat( sstr, "LBS_MULTICOLUMN " );
        }
        if( style & LBS_WANTKEYBOARDINPUT ) {
            strcat( sstr, "LBS_WANTKEYBOARDINPUT " );
        }
        if( style & LBS_EXTENDEDSEL ) {
            strcat( sstr, "LBS_EXTENDEDSEL " );
        }
        if( style & LBS_DISABLENOSCROLL ) {
            strcat( sstr, "LBS_DISABLENOSCROLL " );
        }
    } else if( !stricmp( tmp, "combobox" ) ) {
        if( style & CBS_SIMPLE ) {
            strcat( sstr, "CBS_SIMPLE " );
        }
        if( style & CBS_DROPDOWN ) {
            strcat( sstr, "CBS_DROPDOWN " );
        }
        if( style & CBS_DROPDOWNLIST ) {
            strcat( sstr, "CBS_DROPDOWNLIST " );
        }
        if( style & CBS_OWNERDRAWFIXED ) {
            strcat( sstr, "CBS_OWNERDRAWFIXED " );
        }
        if( style & CBS_OWNERDRAWVARIABLE ) {
            strcat( sstr, "CBS_OWNERDRAWVARIABLE " );
        }
        if( style & CBS_AUTOHSCROLL ) {
            strcat( sstr, "CBS_AUTOHSCROLL " );
        }
        if( style & CBS_OEMCONVERT ) {
            strcat( sstr, "CBS_OEMCONVERT " );
        }
        if( style & CBS_SORT ) {
            strcat( sstr, "CBS_SORT " );
        }
        if( style & CBS_HASSTRINGS ) {
            strcat( sstr, "CBS_HASSTRINGS " );
        }
        if( style & CBS_NOINTEGRALHEIGHT ) {
            strcat( sstr, "CBS_NOINTEGRALHEIGHT " );
        }
        if( style & CBS_DISABLENOSCROLL ) {
            strcat( sstr, "CBS_DISABLENOSCROLL " );
        }
    } else if( !stricmp( tmp, "#32770" ) ) {
        if( style & DS_ABSALIGN ) {
            strcat( sstr, "DS_ABSALIGN " );
        }
        if( style & DS_SYSMODAL ) {
            strcat( sstr, "DS_SYSMODAL " );
        }
        if( style & DS_LOCALEDIT ) {
            strcat( sstr, "DS_LOCALEDIT " );
        }
        if( style & DS_SETFONT ) {
            strcat( sstr, "DS_SETFONT " );
        }
#ifdef __NT__
        if( style & DS_SETFOREGROUND ) {
            strcat( sstr, "DS_SETFOREGROUND " );
        }
        if( style & DS_3DLOOK ) {
            strcat( sstr, "DS_3DLOOK " );
        }
        if( style & DS_FIXEDSYS ) {
            strcat( sstr, "DS_FIXEDSYS " );
        }
        if( style & DS_NOFAILCREATE ) {
            strcat( sstr, "DS_NOFAILCREATE " );
        }
        if( style & DS_CONTROL ) {
            strcat( sstr, "DS_CONTROL " );
        }
        if( style & DS_CENTER ) {
            strcat( sstr, "DS_CENTER " );
        }
        if( style & DS_CENTERMOUSE ) {
            strcat( sstr, "DS_CENTERMOUSE " );
        }
        if( style & DS_CONTEXTHELP ) {
            strcat( sstr, "DS_CONTEXTHELP " );
        }
        if( (style & DS_SHELLFONT) == DS_SHELLFONT ) {
            strcat( sstr, "DS_SHELLFONT " );
        }
    } else if( !stricmp( tmp, ANIMATE_CLASS ) ) {
        if( style & ACS_CENTER ) {
            strcat( sstr, "ACS_CENTER " );
        }
        if( style & ACS_TRANSPARENT ) {
            strcat( sstr, "ACS_TRANSPARENT " );
        }
        if( style & ACS_AUTOPLAY ) {
            strcat( sstr, "ACS_AUTOPLAY " );
        }
        if( style & ACS_TIMER ) {
            strcat( sstr, "ACS_TIMER " );
        }
    } else if( !stricmp( tmp, DATETIMEPICK_CLASS ) ) {
        if( style & DTS_UPDOWN ) {
            strcat( sstr, "DTS_UPDOWN " );
        }
        if( style & DTS_SHOWNONE ) {
            strcat( sstr, "DTS_SHOWNONE " );
        }
        if( (style & DTS_SHORTDATECENTURYFORMAT) == DTS_SHORTDATECENTURYFORMAT ) {
            strcat( sstr, "DTS_SHORTDATECENTURYFORMAT " );
        } else if( style & DTS_LONGDATEFORMAT ) {
            strcat( sstr, "DTS_LONGDATEFORMAT " );
        } else if( (style & DTS_TIMEFORMAT) == DTS_TIMEFORMAT ) {
            strcat( sstr, "DTS_TIMEFORMAT " );
        } else {
            strcat( sstr, "DTS_SHORTDATEFORMAT " );
        }
        if( style & DTS_APPCANPARSE ) {
            strcat( sstr, "DTS_APPCANPARSE " );
        }
        if( style & DTS_RIGHTALIGN ) {
            strcat( sstr, "DTS_RIGHTALIGN " );
        }
    } else if( !stricmp( tmp, MONTHCAL_CLASS ) ) {
        if( style & MCS_DAYSTATE ) {
            strcat( sstr, "MCS_DAYSTATE " );
        }
        if( style & MCS_MULTISELECT ) {
            strcat( sstr, "MCS_MULTISELECT " );
        }
        if( style & MCS_WEEKNUMBERS ) {
            strcat( sstr, "MCS_WEEKNUMBERS " );
        }
        if( style & MCS_NOTODAYCIRCLE ) {
            strcat( sstr, "MCS_NOTODAYCIRCLE " );
        }
        if( style & MCS_NOTODAY ) {
            strcat( sstr, "MCS_NOTODAY " );
        }
        if( style & MCS_NOTRAILINGDATES ) {
            strcat( sstr, "MCS_NOTRAILINGDATES " );
        }
        if( style & MCS_SHORTDAYSOFWEEK ) {
            strcat( sstr, "MCS_SHORTDAYSOFWEEK " );
        }
        if( style & MCS_NOSELCHANGEONNAV ) {
            strcat( sstr, "MCS_NOSELCHANGEONNAV " );
        }
    } else if( !stricmp( tmp, PROGRESS_CLASS ) ) {
        if( style & PBS_SMOOTH ) {
            strcat( sstr, "PBS_SMOOTH " );
        }
        if( style & PBS_VERTICAL ) {
            strcat( sstr, "PBS_VERTICAL " );
        }
        if( style & PBS_MARQUEE ) {
            strcat( sstr, "PBS_MARQUEE " );
        }
        if( style & PBS_SMOOTHREVERSE ) {
            strcat( sstr, "PBS_SMOOTHREVERSE " );
        }
    } else if( !stricmp( tmp, REBARCLASSNAME ) ) {
        if( style & RBS_TOOLTIPS ) {
            strcat( sstr, "RBS_TOOLTIPS " );
        }
        if( style & RBS_VARHEIGHT ) {
            strcat( sstr, "RBS_VARHEIGHT " );
        }
        if( style & RBS_BANDBORDERS ) {
            strcat( sstr, "RBS_BANDBORDERS " );
        }
        if( style & RBS_FIXEDORDER ) {
            strcat( sstr, "RBS_FIXEDORDER " );
        }
        if( style & RBS_REGISTERDROP ) {
            strcat( sstr, "RBS_REGISTERDROP " );
        }
        if( style & RBS_AUTOSIZE ) {
            strcat( sstr, "RBS_AUTOSIZE " );
        }
        if( style & RBS_VERTICALGRIPPER ) {
            strcat( sstr, "RBS_VERTICALGRIPPER " );
        }
        if( style & RBS_DBLCLKTOGGLE ) {
            strcat( sstr, "RBS_DBLCLKTOGGLE " );
        }
    } else if( !stricmp( tmp, STATUSCLASSNAME ) ) {
        if( style & SBARS_SIZEGRIP ) {
            strcat( sstr, "SBARS_SIZEGRIP " );
        }
        if( style & SBARS_TOOLTIPS ) {
            strcat( sstr, "SBARS_TOOLTIPS " );
        }
    } else if( !stricmp( tmp, TOOLBARCLASSNAME ) ) {
        if( style & TBSTYLE_TOOLTIPS ) {
            strcat( sstr, "TBSTYLE_TOOLTIPS " );
        }
        if( style & TBSTYLE_WRAPABLE ) {
            strcat( sstr, "TBSTYLE_WRAPABLE " );
        }
        if( style & TBSTYLE_ALTDRAG ) {
            strcat( sstr, "TBSTYLE_ALTDRAG " );
        }
        if( style & TBSTYLE_FLAT ) {
            strcat( sstr, "TBSTYLE_FLAT " );
        }
        if( style & TBSTYLE_LIST ) {
            strcat( sstr, "TBSTYLE_LIST " );
        }
        if( style & TBSTYLE_CUSTOMERASE ) {
            strcat( sstr, "TBSTYLE_CUSTOMERASE " );
        }
        if( style & TBSTYLE_REGISTERDROP ) {
            strcat( sstr, "TBSTYLE_REGISTERDROP " );
        }
        if( style & TBSTYLE_TRANSPARENT ) {
            strcat( sstr, "TBSTYLE_TRANSPARENT " );
        }
    } else if( !stricmp( tmp, TOOLTIPS_CLASS ) ) {
        if( style & TTS_ALWAYSTIP ) {
            strcat( sstr, "TTS_ALWAYSTIP " );
        }
        if( style & TTS_NOPREFIX ) {
            strcat( sstr, "TTS_NOPREFIX " );
        }
        if( style & TTS_NOANIMATE ) {
            strcat( sstr, "TTS_NOANIMATE " );
        }
        if( style & TTS_NOFADE ) {
            strcat( sstr, "TTS_NOFADE " );
        }
        if( style & TTS_BALLOON ) {
            strcat( sstr, "TTS_BALLOON " );
        }
        if( style & TTS_CLOSE ) {
            strcat( sstr, "TTS_CLOSE " );
        }
        if( style & TTS_USEVISUALSTYLE ) {
            strcat( sstr, "TTS_USEVISUALSTYLE " );
        }
    } else if( !stricmp( tmp, TRACKBAR_CLASS ) ) {
        if( style & TBS_AUTOTICKS ) {
            strcat( sstr, "TBS_AUTOTICKS " );
        }
        if( style & TBS_VERT ) {
            strcat( sstr, "TBS_VERT " );
            if( style & TBS_LEFT ) {
                strcat( sstr, "TBS_LEFT " );
            } else if( !(style & (TBS_BOTH | TBS_NOTICKS)) ) {
                strcat( sstr, "TBS_RIGHT " );
            }
        } else {
            strcat( sstr, "TBS_HORZ " );
            if( style & TBS_TOP ) {
                strcat( sstr, "TBS_TOP " );
            } else if( !(style & (TBS_BOTH | TBS_NOTICKS)) ) {
                strcat( sstr, "TBS_BOTTOM " );
            }
        }
        if( style & TBS_BOTH ) {
            strcat( sstr, "TBS_BOTH " );
        }
        if( style & TBS_NOTICKS ) {
            strcat( sstr, "TBS_NOTICKS " );
        }
        if( style & TBS_ENABLESELRANGE ) {
            strcat( sstr, "TBS_ENABLESELRANGE " );
        }
        if( style & TBS_FIXEDLENGTH ) {
            strcat( sstr, "TBS_FIXEDLENGTH " );
        }
        if( style & TBS_NOTHUMB ) {
            strcat( sstr, "TBS_NOTHUMB " );
        }
        if( style & TBS_TOOLTIPS ) {
            strcat( sstr, "TBS_TOOLTIPS " );
        }
        if( style & TBS_REVERSED ) {
            strcat( sstr, "TBS_REVERSED " );
        }
        if( style & TBS_DOWNISLEFT ) {
            strcat( sstr, "TBS_DOWNISLEFT " );
        }
        if( style & TBS_NOTIFYBEFOREMOVE ) {
            strcat( sstr, "TBS_NOTIFYBEFOREMOVE " );
        }
        if( style & TBS_TRANSPARENTBKGND ) {
            strcat( sstr, "TBS_TRANSPARENTBKGND " );
        }
    } else if( !stricmp( tmp, UPDOWN_CLASS ) ) {
        if( style & UDS_WRAP ) {
            strcat( sstr, "UDS_WRAP " );
        }
        if( style & UDS_SETBUDDYINT ) {
            strcat( sstr, "UDS_SETBUDDYINT " );
        }
        if( style & UDS_ALIGNRIGHT ) {
            strcat( sstr, "UDS_ALIGNRIGHT " );
        }
        if( style & UDS_ALIGNLEFT ) {
            strcat( sstr, "UDS_ALIGNLEFT " );
        }
        if( style & UDS_AUTOBUDDY ) {
            strcat( sstr, "UDS_AUTOBUDDY " );
        }
        if( style & UDS_ARROWKEYS ) {
            strcat( sstr, "UDS_ARROWKEYS " );
        }
        if( style & UDS_HORZ ) {
            strcat( sstr, "UDS_HORZ " );
        }
        if( style & UDS_NOTHOUSANDS ) {
            strcat( sstr, "UDS_NOTHOUSANDS " );
        }
        if( style & UDS_HOTTRACK ) {
            strcat( sstr, "UDS_HOTTRACK " );
        }
    } else if( !stricmp( tmp, WC_HEADER ) ) {
        if( style & HDS_BUTTONS ) {
            strcat( sstr, "HDS_BUTTONS " );
        }
        if( style & HDS_HOTTRACK ) {
            strcat( sstr, "HDS_HOTTRACK " );
        }
        if( style & HDS_HIDDEN ) {
            strcat( sstr, "HDS_HIDDEN " );
        }
        if( style & HDS_DRAGDROP ) {
            strcat( sstr, "HDS_DRAGDROP " );
        }
        if( style & HDS_FULLDRAG ) {
            strcat( sstr, "HDS_FULLDRAG " );
        }
        if( style & HDS_FILTERBAR ) {
            strcat( sstr, "HDS_FILTERBAR " );
        }
        if( style & HDS_FLAT ) {
            strcat( sstr, "HDS_FLAT " );
        }
        if( style & HDS_CHECKBOXES ) {
            strcat( sstr, "HDS_CHECKBOXES " );
        }
        if( style & HDS_NOSIZING ) {
            strcat( sstr, "HDS_NOSIZING " );
        }
        if( style & HDS_OVERFLOW ) {
            strcat( sstr, "HDS_OVERFLOW " );
        }
    } else if( !stricmp( tmp, WC_LISTVIEW ) ) {
        switch( style & LVS_TYPEMASK ) {
        case LVS_ICON:
            strcat( sstr, "LVS_ICON " );
            break;
        case LVS_REPORT:
            strcat( sstr, "LVS_REPORT " );
            break;
        case LVS_SMALLICON:
            strcat( sstr, "LVS_SMALLICON " );
            break;
        case LVS_LIST:
            strcat( sstr, "LVS_LIST " );
            break;
        }
        if( style & LVS_SINGLESEL ) {
            strcat( sstr, "LVS_SINGLESEL" );
        }
        if( style & LVS_SHOWSELALWAYS ) {
            strcat( sstr, "LVS_SHOWSELALWAYS " );
        }
        if( style & LVS_SORTASCENDING ) {
            strcat( sstr, "LVS_SORTASCENDING " );
        }
        if( style & LVS_SORTDESCENDING ) {
            strcat( sstr, "LVS_SORTDESCENDING " );
        }
        if( style & LVS_SHAREIMAGELISTS ) {
            strcat( sstr, "LVS_SHAREIMAGELISTS " );
        }
        if( style & LVS_NOLABELWRAP ) {
            strcat( sstr, "LVS_NOLABELWRAP " );
        }
        if( style & LVS_AUTOARRANGE ) {
            strcat( sstr, "LVS_AUTOARRANGE " );
        }
        if( style & LVS_EDITLABELS ) {
            strcat( sstr, "LVS_EDITLABELS " );
        }
        if( style & LVS_OWNERDRAWFIXED ) {
            strcat( sstr, "LVS_OWNERDRAWFIXED " );
        }
        switch( style & LVS_ALIGNMASK ) {
        case LVS_ALIGNTOP:
            strcat( sstr, "LVS_ALIGNTOP " );
            break;
        case LVS_ALIGNLEFT:
            strcat( sstr, "LVS_ALIGNLEFT " );
            break;
        }
        if( style & LVS_OWNERDATA ) {
            strcat( sstr, "LVS_OWNERDATA " );
        }
        if( style & LVS_NOSCROLL ) {
            strcat( sstr, "LVS_NOSCROLL " );
        }
        if( style & LVS_NOCOLUMNHEADER ) {
            strcat( sstr, "LVS_NOCOLUMNHEADER " );
        }
        if( style & LVS_NOSORTHEADER ) {
            strcat( sstr, "LVS_NOSORTHEADER " );
        }
    } else if( !stricmp( tmp, WC_TABCONTROL ) ) {
        if( style & TCS_SCROLLOPPOSITE ) {
            strcat( sstr, "TCS_SCROLLOPPOSITE " );
        }
        if( style & TCS_BOTTOM ) {
            if( style & TCS_VERTICAL ) {
                strcat( sstr, "TCS_RIGHT " );
            } else {
                strcat( sstr, "TCS_BOTTOM " );
            }
        }
        if( style & TCS_MULTISELECT ) {
            strcat( sstr, "TCS_MULTISELECT " );
        }
        if( style & TCS_FLATBUTTONS ) {
            strcat( sstr, "TCS_FLATBUTTONS " );
        }
        if( style & TCS_FORCEICONLEFT ) {
            strcat( sstr, "TCS_FORCEICONLEFT " );
        }
        if( style & TCS_FORCELABELLEFT ) {
            strcat( sstr, "TCS_FORCELABELLEFT " );
        }
        if( style & TCS_HOTTRACK ) {
            strcat( sstr, "TCS_HOTTRACK " );
        }
        if( style & TCS_VERTICAL ) {
            strcat( sstr, "TCS_VERTICAL " );
        }
        if( style & TCS_BUTTONS ) {
            strcat( sstr, "TCS_BUTTONS " );
        } else {
            strcat( sstr, "TCS_TABS " );
        }
        if( style & TCS_MULTILINE ) {
            strcat( sstr, "TCS_MULTILINE " );
        } else {
            strcat( sstr, "TCS_SINGLELINE " );
        }
        if( !(style & (TCS_FIXEDWIDTH | TCS_RAGGEDRIGHT)) && (style & TCS_MULTILINE) ) {
            strcat( sstr, "TCS_RIGHTJUSTIFY " );
        }
        if( style & TCS_FIXEDWIDTH ) {
            strcat( sstr, "TCS_FIXEDWIDTH " );
        }
        if( style & TCS_RAGGEDRIGHT ) {
            strcat( sstr, "TCS_RAGGEDRIGHT " );
        }
        if( style & TCS_FOCUSONBUTTONDOWN ) {
            strcat( sstr, "TCS_FOCUSONBUTTONDOWN " );
        }
        if( style & TCS_OWNERDRAWFIXED ) {
            strcat( sstr, "TCS_OWNERDRAWFIXED " );
        }
        if( style & TCS_TOOLTIPS ) {
            strcat( sstr, "TCS_TOOLTIPS " );
        }
        if( style & TCS_FOCUSNEVER ) {
            strcat( sstr, "TCS_FOCUSNEVER " );
        }
    } else if( !stricmp( tmp, WC_TREEVIEW ) ) {
        if( style & TVS_HASBUTTONS ) {
            strcat( sstr, "TVS_HASBUTTONS " );
        }
        if( style & TVS_HASLINES ) {
            strcat( sstr, "TVS_HASLINES " );
        }
        if( style & TVS_LINESATROOT ) {
            strcat( sstr, "TVS_LINESATROOT " );
        }
        if( style & TVS_EDITLABELS ) {
            strcat( sstr, "TVS_EDITLABELS " );
        }
        if( style & TVS_DISABLEDRAGDROP ) {
            strcat( sstr, "TVS_DISABLEDRAGDROP " );
        }
        if( style & TVS_SHOWSELALWAYS ) {
            strcat( sstr, "TVS_SHOWSELALWAYS " );
        }
        if( style & TVS_RTLREADING ) {
            strcat( sstr, "TVS_RTLREADING " );
        }
        if( style & TVS_NOTOOLTIPS ) {
            strcat( sstr, "TVS_NOTOOLTIPS " );
        }
        if( style & TVS_CHECKBOXES ) {
            strcat( sstr, "TVS_CHECKBOXES " );
        }
        if( style & TVS_TRACKSELECT ) {
            strcat( sstr, "TVS_TRACKSELECT " );
        }
        if( style & TVS_SINGLEEXPAND ) {
            strcat( sstr, "TVS_SINGLEEXPAND " );
        }
        if( style & TVS_INFOTIP ) {
            strcat( sstr, "TVS_INFOTIP " );
        }
        if( style & TVS_FULLROWSELECT ) {
            strcat( sstr, "TVS_FULLROWSELECT " );
        }
        if( style & TVS_NOSCROLL ) {
            strcat( sstr, "TVS_NOSCROLL " );
        }
        if( style & TVS_NONEVENHEIGHT ) {
            strcat( sstr, "TVS_NONEVENHEIGHT " );
        }
        if( style & TVS_NOHSCROLL ) {
            strcat( sstr, "TVS_NOHSCROLL " );
        }
#endif
    } else {
        if( style & WS_EX_DLGMODALFRAME ) {
            strcat( sstr, "WS_EX_DLGMODALFRAME " );
        }
        if( style & WS_EX_NOPARENTNOTIFY ) {
            strcat( sstr, "WS_EX_NOPARENTNOTIFY " );
        }
        if( style & WS_EX_TOPMOST ) {
            strcat( sstr, "WS_EX_TOPMOST " );
        }
        if( style & WS_EX_ACCEPTFILES ) {
            strcat( sstr, "WS_EX_ACCEPTFILES " );
        }
        if( style & WS_EX_TRANSPARENT ) {
            strcat( sstr, "WS_EX_TRANSPARENT " );
        }
    }

} /* GetWindowStyleString */

#ifndef __NT__
 #define STYLE_TYPE             WORD
 #define STYLE_HEX_LEN          4
#else
 #define STYLE_TYPE             DWORD
 #define STYLE_HEX_LEN          8
#endif

/*
 * GetClassStyleString - get a string corrosponding to class style bits
 */
void GetClassStyleString( HWND hwnd, char *str, char *sstr )
{
    STYLE_TYPE          style;

    style = GET_CLASS_STYLE( hwnd );

    GetHexStr( str, style, STYLE_HEX_LEN  );
    str[ STYLE_HEX_LEN ] = 0;
    sstr[0] = 0;

    if( style & CS_VREDRAW ) {
        strcat( sstr, "CS_VREDRAW " );
    }
    if( style & CS_HREDRAW ) {
        strcat( sstr, "CS_HREDRAW " );
    }
    if( style & CS_KEYCVTWINDOW ) {
        strcat( sstr, "CS_KEYCVTWINDOW " );
    }
    if( style & CS_DBLCLKS ) {
        strcat( sstr, "CS_DBLCLKS " );
    }
    if( style & CS_OWNDC ) {
        strcat( sstr, "CS_OWNDC " );
    }
    if( style & CS_CLASSDC ) {
        strcat( sstr, "CS_CLASSDC " );
    }
    if( style & CS_PARENTDC ) {
        strcat( sstr, "CS_PARENTDC " );
    }
    if( style & CS_NOKEYCVT ) {
        strcat( sstr, "CS_NOKEYCVT " );
    }
    if( style & CS_NOCLOSE ) {
        strcat( sstr, "CS_NOCLOSE " );
    }
    if( style & CS_SAVEBITS ) {
        strcat( sstr, "CS_SAVEBITS " );
    }
    if( style & CS_BYTEALIGNCLIENT ) {
        strcat( sstr, "CS_BYTEALIGNCLIENT " );
    }
    if( style & CS_BYTEALIGNWINDOW ) {
        strcat( sstr, "CS_BYTEALIGNWINDOW " );
    }
    if( style & CS_GLOBALCLASS ) {
        strcat( sstr, "CS_GLOBALCLASS " );
    }

} /* GetClassStyleString */

/*
 * DumpToComboBox - dump a string of space separated items to a combo box
 */
void DumpToComboBox( char *str, HWND cb )
{
    char        tmp[128];
    int         i;

    SendMessage( cb, CB_RESETCONTENT, 0, 0L );
    while( *str != 0 ) {
        i = 0;
        while( *str != ' ' ) {
            tmp[i++] = *str;
            str++;
        }
        tmp[i] = 0;
        if( i != 0 ) {
            str++;
            SendMessage( cb, CB_ADDSTRING, 0, (LONG) (LPSTR) tmp );
        }
    }
    SendMessage( cb, CB_SETCURSEL, 0, 0L );

} /* DumpToComboBox */

/*
 * FormatSpyMessage - make a spy message string
 */
void FormatSpyMessage( char *msg, LPMSG pmsg, char *res )
{

    memset( res,' ', SPYOUT_LENGTH );
    strcpy( res, msg );
    res[ strlen( msg ) ] = ' ';
    GetHexStr( &res[SPYOUT_HWND], (DWORD) pmsg->hwnd, SPYOUT_HWND_LEN );
    GetHexStr( &res[SPYOUT_MSG], pmsg->message, SPYOUT_MSG_LEN );
    GetHexStr( &res[SPYOUT_WPARAM], pmsg->wParam, SPYOUT_WPARAM_LEN );
    GetHexStr( &res[SPYOUT_LPARAM], pmsg->lParam, SPYOUT_LPARAM_LEN );
    res[SPYOUT_LENGTH] = 0;

} /* FormatSpyMessage */

/*
 * SetSpyState - set current spy state
 */
void SetSpyState( spystate ss )
{
    char        str[130];
    char        *rcstr;
    int         len;

    SpyState = ss;
    len = GetWindowText( SpyMainWindow, &str[1], 128 );
    switch( ss ) {
    case OFF:
        if( str[1] != '<' ) {
            str[0] = '<';
            str[len+1] = '>';
            str[len+2] = 0;
            SetWindowText( SpyMainWindow, str );
        }
        rcstr = GetRCString( STR_SPY_ON );
        ModifyMenu( SpyMenu, SPY_OFFON, MF_BYCOMMAND | MF_ENABLED | MF_STRING,
                        SPY_OFFON, rcstr );
        break;
    case ON:
        rcstr = GetRCString( STR_SPY_OFF );
        ModifyMenu( SpyMenu, SPY_OFFON, MF_BYCOMMAND | MF_ENABLED | MF_STRING,
                        SPY_OFFON, rcstr );
        if( str[1] == '<' ) {
            str[len] = 0;
            SetWindowText( SpyMainWindow, &str[2] );
        }
        break;
    case NEITHER:
        rcstr = GetRCString( STR_SPY_OFF );
        ModifyMenu( SpyMenu, SPY_OFFON, MF_BYCOMMAND | MF_GRAYED | MF_STRING,
                        SPY_OFFON, rcstr );
        if( str[1] == '<' ) {
            str[len] = 0;
            SetWindowText( SpyMainWindow, &str[2] );
        }
        break;
    }
    SetOnOffTool( ss );

} /* SetSpyState */

static char filterList[] = "File (*.*)" \
                        "\0" \
                        "*.*" \
                        "\0\0";

/*
 * GetFileName - get a file name using common dialog stuff
 */
BOOL GetFileName( char *ext, int type, char *fname )
{
    OPENFILENAME        of;
    BOOL                rc;

    fname[ 0 ] = 0;
    memset( &of, 0, sizeof( OPENFILENAME ) );
    of.lStructSize = sizeof( OPENFILENAME );
    of.hwndOwner = SpyMainWindow;
    of.lpstrFilter = (LPSTR) filterList;
    of.lpstrDefExt = ext;
    of.nFilterIndex = 1L;
    of.lpstrFile = fname;
    of.nMaxFile = _MAX_PATH;
    of.lpstrTitle = NULL;
    of.Flags = OFN_HIDEREADONLY;
    if( type == FILE_OPEN ) {
        rc = GetOpenFileName( &of );
    } else {
        rc = GetSaveFileName( &of );
    }
    return( rc );

} /* GetFileName */

BOOL InitGblStrings( void ) {

    DWORD       heading;
    DWORD       heading_uline;

    SpyName = AllocRCString( STR_APP_NAME );
    if( SpyName == NULL ) return( FALSE );

#ifdef __NT__
    heading = STR_HEADINGS_NT;
    heading_uline = STR_HEADING_UNDERLINE_NT;
#else
    heading = STR_HEADINGS_WIN;
    heading_uline = STR_HEADING_UNDERLINE_WIN;
#endif
    TitleBar = AllocRCString( heading );
    TitleBarLen = strlen( TitleBar + 1 );
    if( TitleBar == NULL ) return( FALSE );

    TitleBarULine = AllocRCString( heading_uline );
    if( TitleBarULine == NULL ) return( FALSE );
    return( TRUE );
}
