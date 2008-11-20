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

#include <windows.h>
#define MSG_RC_BASE     0
#include "rcstr.gh"
#include "ldstr.h"
#include "hint.h"

#ifdef __NT__
    #define USE_SNAP_WINDOW         1
#endif

#ifdef __NT__
    #define GCW_STYLE              (-26)
    #define UINT_STR_LEN           8
#else
    #define UINT_STR_LEN           4
#endif

#include "win1632.h"
#include "spydll.h"
#include "jdlg.h"

/* PenWindows specific messages */
#ifndef WM_PENWINFIRST
    #define WM_PENWINFIRST                  0x0380
#endif
#ifndef WM_PENWINLAST
    #define WM_PENWINLAST                   0x038F
#endif
#ifndef WM_COALESCE_FIRST
    #define WM_COALESCE_FIRST               0x0390
#endif
#ifndef WM_COALESCE_LAST
    #define WM_COALESCE_LAST                0x039F
#endif
#ifndef WM_CTLCOLOR
    #define WM_CTLCOLOR                     0x0019
#endif
#ifndef WM_GETHOTKEY
    #define WM_GETHOTKEY                    0x0033
#endif
#ifndef WM_SETHOTKEY
    #define WM_SETHOTKEY                    0x0032
#endif
#ifndef WM_PAINTICON
    #define WM_PAINTICON                    0x0026
#endif
#ifndef WM_OTHERWINDOWCREATED
    #define WM_OTHERWINDOWCREATED           0x0042
#endif
#ifndef WM_OTHERWINDOWDESTROYED
    #define WM_OTHERWINDOWDESTROYED         0x0043
#endif

// Messages undocumented for WINDOWS but documented for NT
#ifndef WM_ENTERMENULOOP
    #define WM_ENTERMENULOOP                0x0211
#endif
#ifndef WM_EXITMENULOOP
    #define WM_EXITMENULOOP                 0x0212
#endif

// Windows 95 specific messages
#ifndef WM_NOTIFY
    #define WM_NOTIFY                       0x004E
#endif
#ifndef WM_INPUTLANGCHANGEREQUEST
    #define WM_INPUTLANGCHANGEREQUEST       0x0050
#endif
#ifndef WM_INPUTLANGCHANGE
    #define WM_INPUTLANGCHANGE              0x0051
#endif
#ifndef WM_TCARD
    #define WM_TCARD                        0x0052
#endif
#ifndef WM_HELP
    #define WM_HELP                         0x0053
#endif
#ifndef WM_USERCHANGED
    #define WM_USERCHANGED                  0x0054
#endif
#ifndef WM_CONTEXTMENU
    #define WM_CONTEXTMENU                  0x007B
#endif
#ifndef WM_STYLECHANGING
    #define WM_STYLECHANGING                0x007C
#endif
#ifndef WM_STYLECHANGED
    #define WM_STYLECHANGED                 0x007D
#endif
#ifndef WM_DISPLAYCHANGE
    #define WM_DISPLAYCHANGE                0x007E
#endif
#ifndef WM_GETICON
    #define WM_GETICON                      0x007F
#endif
#ifndef WM_SETICON
    #define WM_SETICON                      0x0080
#endif

// Messages introduced after Windows 95
#ifndef WM_NCXBUTTONDOWN
    #define WM_NCXBUTTONDOWN                0x00AB
#endif
#ifndef WM_NCXBUTTONUP
    #define WM_NCXBUTTONUP                  0x00AC
#endif
#ifndef WM_NCXBUTTONDBLCLK
    #define WM_NCXBUTTONDBLCLK              0x00AD
#endif
#ifndef WM_INPUT_DEVICE_CHANGE
    #define WM_INPUT_DEVICE_CHANGE          0x00FE
#endif
#ifndef WM_INPUT
    #define WM_INPUT                        0x00FF
#endif
#ifndef WM_UNICHAR
    #define WM_UNICHAR                      0x0108
#endif
#ifndef WM_MENURBUTTONUP
    #define WM_MENURBUTTONUP                0x0122
#endif
#ifndef WM_MENUDRAG
    #define WM_MENUDRAG                     0x0123
#endif
#ifndef WM_MENUGETOBJECT
    #define WM_MENUGETOBJECT                0x0124
#endif
#ifndef WM_UNINITMENUPOPUP
    #define WM_UNINITMENUPOPUP              0x0125
#endif
#ifndef WM_MENUCOMMAND
    #define WM_MENUCOMMAND                  0x0126
#endif
#ifndef WM_CHANGEUISTATE
    #define WM_CHANGEUISTATE                0x0127
#endif
#ifndef WM_UPDATEUISTATE
    #define WM_UPDATEUISTATE                0x0128
#endif
#ifndef WM_QUERYUISTATE
    #define WM_QUERYUISTATE                 0x0129
#endif
#ifndef WM_MOUSEWHEEL
    #define WM_MOUSEWHEEL                   0x020A
#endif
#ifndef WM_XBUTTONDOWN
    #define WM_XBUTTONDOWN                  0x020B
#endif
#ifndef WM_XBUTTONUP
    #define WM_XBUTTONUP                    0x020C
#endif
#ifndef WM_XBUTTONDBLCLK
    #define WM_XBUTTONDBLCLK                0x020D
#endif
#ifndef WM_MOUSEHWHEEL
    #define WM_MOUSEHWHEEL                  0x020E
#endif
#ifndef WM_IME_REQUEST
    #define WM_IME_REQUEST                  0x0288
#endif
#ifndef WM_MOUSEHOVER
    #define WM_MOUSEHOVER                   0x02A1
#endif
#ifndef WM_MOUSELEAVE
    #define WM_MOUSELEAVE                   0x02A3
#endif
#ifndef WM_NCMOUSEHOVER
    #define WM_NCMOUSEHOVER                 0x02A0
#endif
#ifndef WM_NCMOUSELEAVE
    #define WM_NCMOUSELEAVE                 0x02A2
#endif
#ifndef WM_WTSSESSION_CHANGE
    #define WM_WTSSESSION_CHANGE            0x02B1
#endif
#ifndef WM_APPCOMMAND
    #define WM_APPCOMMAND                   0x0319
#endif
#ifndef WM_THEMECHANGED
    #define WM_THEMECHANGED                 0x031A
#endif
#ifndef WM_CLIPBOARDUPDATE
    #define WM_CLIPBOARDUPDATE              0x031D
#endif
#ifndef WM_DWMCOMPOSITIONCHANGED
    #define WM_DWMCOMPOSITIONCHANGED        0x031E
#endif
#ifndef WM_DWMNCRENDERINGCHANGED
    #define WM_DWMNCRENDERINGCHANGED        0x031F
#endif
#ifndef WM_DWMCOLORIZATIONCOLORCHANGED
    #define WM_DWMCOLORIZATIONCOLORCHANGED  0x0320
#endif
#ifndef WM_DWMWINDOWMAXIMIZEDCHANGE
    #define WM_DWMWINDOWMAXIMIZEDCHANGE     0x0321
#endif
#ifndef WM_GETTITLEBARINFOEX
    #define WM_GETTITLEBARINFOEX            0x033F
#endif

// Control messages
#ifndef EM_SETIMESTATUS
    #define EM_SETIMESTATUS                 0x00D8
#endif
#ifndef EM_GETIMESTATUS
    #define EM_GETIMESTATUS                 0x00D9
#endif
#ifndef BM_SETDONTCLICK
    #define BM_SETDONTCLICK                 0x00F8
#endif
#ifndef LB_GETLISTBOXINFO
    #define LB_GETLISTBOXINFO               0x01B2
#endif
#ifndef CB_GETCOMBOBOXINFO
    #define CB_GETCOMBOBOXINFO              0x0164
#endif
#ifndef SBM_GETSCROLLBARINFO
    #define SBM_GETSCROLLBARINFO            0x00EB
#endif

// Common control messages
#ifndef HDM_GETITEMDROPDOWNRECT
    #define HDM_GETITEMDROPDOWNRECT         (HDM_FIRST + 25)
#endif
#ifndef HDM_GETOVERFLOWRECT
    #define HDM_GETOVERFLOWRECT             (HDM_FIRST + 26)
#endif
#ifndef HDM_GETFOCUSEDITEM
    #define HDM_GETFOCUSEDITEM              (HDM_FIRST + 27)
#endif
#ifndef HDM_SETFOCUSEDITEM
    #define HDM_SETFOCUSEDITEM              (HDM_FIRST + 28)
#endif
#ifndef TB_GETMETRICS
    #define TB_GETMETRICS                   (WM_USER + 101)
#endif
#ifndef TB_SETMETRICS
    #define TB_SETMETRICS                   (WM_USER + 102)
#endif
#ifndef TB_SETPRESSEDIMAGELIST
    #define TB_SETPRESSEDIMAGELIST          (WM_USER + 104)
#endif
#ifndef TB_GETPRESSEDIMAGELIST
    #define TB_GETPRESSEDIMAGELIST          (WM_USER + 105)
#endif
#define RB_GETBANDINFO_IE3                  (WM_USER + 5)
#ifndef RB_GETBANDMARGINS
    #define RB_GETBANDMARGINS               (WM_USER + 40)
#endif
#ifndef RB_SETBANDWIDTH
    #define RB_SETBANDWIDTH                 (WM_USER + 44)
#endif
#ifndef TTM_POPUP
    #define TTM_POPUP                       (WM_USER + 34)
#endif
#ifndef TTM_GETTITLE
    #define TTM_GETTITLE                    (WM_USER + 35)
#endif
#ifndef PBM_SETMARQUEE
    #define PBM_SETMARQUEE                  (WM_USER + 10)
#endif
#ifndef PBM_GETSTEP
    #define PBM_GETSTEP                     (WM_USER + 11)
#endif
#ifndef PBM_GETBKCOLOR
    #define PBM_GETBKCOLOR                  (WM_USER + 12)
#endif
#ifndef PBM_GETBARCOLOR
    #define PBM_GETBARCOLOR                 (WM_USER + 13)
#endif
#ifndef PBM_SETSTATE
    #define PBM_SETSTATE                    (WM_USER + 14)
#endif
#ifndef PBM_GETSTATE
    #define PBM_GETSTATE                    (WM_USER + 15)
#endif
#ifndef LVM_GETGROUPSTATE
    #define LVM_GETGROUPSTATE               (LVM_FIRST + 92)
#endif
#ifndef LVM_GETFOCUSEDGROUP
    #define LVM_GETFOCUSEDGROUP             (LVM_FIRST + 93)
#endif
#ifndef LVM_GETGROUPRECT
    #define LVM_GETGROUPRECT                (LVM_FIRST + 98)
#endif
#ifndef LVM_SETSELECTEDCOLUMN
    #define LVM_SETSELECTEDCOLUMN           (LVM_FIRST + 140)
#endif
#ifndef LVM_SETVIEW
    #define LVM_SETVIEW                     (LVM_FIRST + 142)
#endif
#ifndef LVM_GETVIEW
    #define LVM_GETVIEW                     (LVM_FIRST + 143)
#endif
#ifndef LVM_INSERTGROUP
    #define LVM_INSERTGROUP                 (LVM_FIRST + 145)
#endif
#ifndef LVM_SETGROUPINFO
    #define LVM_SETGROUPINFO                (LVM_FIRST + 147)
#endif
#ifndef LVM_GETGROUPINFO
    #define LVM_GETGROUPINFO                (LVM_FIRST + 149)
#endif
#ifndef LVM_REMOVEGROUP
    #define LVM_REMOVEGROUP                 (LVM_FIRST + 150)
#endif
#ifndef LVM_MOVEGROUP
    #define LVM_MOVEGROUP                   (LVM_FIRST + 151)
#endif
#ifndef LVM_GETGROUPCOUNT
    #define LVM_GETGROUPCOUNT               (LVM_FIRST + 152)
#endif
#ifndef LVM_GETGROUPINFOBYINDEX
    #define LVM_GETGROUPINFOBYINDEX         (LVM_FIRST + 153)
#endif
#ifndef LVM_MOVEITEMTOGROUP
    #define LVM_MOVEITEMTOGROUP             (LVM_FIRST + 154)
#endif
#ifndef LVM_SETGROUPMETRICS
    #define LVM_SETGROUPMETRICS             (LVM_FIRST + 155)
#endif
#ifndef LVM_GETGROUPMETRICS
    #define LVM_GETGROUPMETRICS             (LVM_FIRST + 156)
#endif
#ifndef LVM_ENABLEGROUPVIEW
    #define LVM_ENABLEGROUPVIEW             (LVM_FIRST + 157)
#endif
#ifndef LVM_SORTGROUPS
    #define LVM_SORTGROUPS                  (LVM_FIRST + 158)
#endif
#ifndef LVM_INSERTGROUPSORTED
    #define LVM_INSERTGROUPSORTED           (LVM_FIRST + 159)
#endif
#ifndef LVM_REMOVEALLGROUPS
    #define LVM_REMOVEALLGROUPS             (LVM_FIRST + 160)
#endif
#ifndef LVM_HASGROUP
    #define LVM_HASGROUP                    (LVM_FIRST + 161)
#endif
#ifndef LVM_SETTILEVIEWINFO
    #define LVM_SETTILEVIEWINFO             (LVM_FIRST + 162)
#endif
#ifndef LVM_GETTILEVIEWINFO
    #define LVM_GETTILEVIEWINFO             (LVM_FIRST + 163)
#endif
#ifndef LVM_SETTILEINFO
    #define LVM_SETTILEINFO                 (LVM_FIRST + 164)
#endif
#ifndef LVM_GETTILEINFO
    #define LVM_GETTILEINFO                 (LVM_FIRST + 165)
#endif
#ifndef LVM_SETINSERTMARK
    #define LVM_SETINSERTMARK               (LVM_FIRST + 166)
#endif
#ifndef LVM_GETINSERTMARK
    #define LVM_GETINSERTMARK               (LVM_FIRST + 167)
#endif
#ifndef LVM_INSERTMARKHITTEST
    #define LVM_INSERTMARKHITTEST           (LVM_FIRST + 168)
#endif
#ifndef LVM_GETINSERTMARKRECT
    #define LVM_GETINSERTMARKRECT           (LVM_FIRST + 169)
#endif
#ifndef LVM_SETINSERTMARKCOLOR
    #define LVM_SETINSERTMARKCOLOR          (LVM_FIRST + 170)
#endif
#ifndef LVM_GETINSERTMARKCOLOR
    #define LVM_GETINSERTMARKCOLOR          (LVM_FIRST + 171)
#endif
#ifndef LVM_SETINFOTIP
    #define LVM_SETINFOTIP                  (LVM_FIRST + 173)
#endif
#ifndef LVM_GETSELECTEDCOLUMN
    #define LVM_GETSELECTEDCOLUMN           (LVM_FIRST + 174)
#endif
#ifndef LVM_ISGROUPVIEWENABLED
    #define LVM_ISGROUPVIEWENABLED          (LVM_FIRST + 175)
#endif
#ifndef LVM_GETOUTLINECOLOR
    #define LVM_GETOUTLINECOLOR             (LVM_FIRST + 176)
#endif
#ifndef LVM_SETOUTLINECOLOR
    #define LVM_SETOUTLINECOLOR             (LVM_FIRST + 177)
#endif
#ifndef LVM_CANCELEDITLABEL
    #define LVM_CANCELEDITLABEL             (LVM_FIRST + 179)
#endif
#ifndef LVM_MAPINDEXTOID
    #define LVM_MAPINDEXTOID                (LVM_FIRST + 180)
#endif
#ifndef LVM_MAPIDTOINDEX
    #define LVM_MAPIDTOINDEX                (LVM_FIRST + 181)
#endif
#ifndef LVM_ISITEMVISIBLE
    #define LVM_ISITEMVISIBLE               (LVM_FIRST + 182)
#endif
#ifndef LVM_GETEMPTYTEXT
    #define LVM_GETEMPTYTEXT                (LVM_FIRST + 204)
#endif
#ifndef LVM_GETFOOTERRECT
    #define LVM_GETFOOTERRECT               (LVM_FIRST + 205)
#endif
#ifndef LVM_GETFOOTERINFO
    #define LVM_GETFOOTERINFO               (LVM_FIRST + 206)
#endif
#ifndef LVM_GETFOOTERITEMRECT
    #define LVM_GETFOOTERITEMRECT           (LVM_FIRST + 207)
#endif
#ifndef LVM_GETFOOTERITEM
    #define LVM_GETFOOTERITEM               (LVM_FIRST + 208)
#endif
#ifndef LVM_GETITEMINDEXRECT
    #define LVM_GETITEMINDEXRECT            (LVM_FIRST + 209)
#endif
#ifndef LVM_SETITEMINDEXSTATE
    #define LVM_SETITEMINDEXSTATE           (LVM_FIRST + 210)
#endif
#ifndef LVM_GETNEXTITEMINDEX
    #define LVM_GETNEXTITEMINDEX            (LVM_FIRST + 211)
#endif
#ifndef TVM_MAPACCIDTOHTREEITEM
    #define TVM_MAPACCIDTOHTREEITEM         (TV_FIRST + 42)
#endif
#ifndef TVM_MAPHTREEITEMTOACCID
    #define TVM_MAPHTREEITEMTOACCID         (TV_FIRST + 43)
#endif
#ifndef TVM_SETEXTENDEDSTYLE
    #define TVM_SETEXTENDEDSTYLE            (TV_FIRST + 44)
#endif
#ifndef TVM_GETEXTENDEDSTYLE
    #define TVM_GETEXTENDEDSTYLE            (TV_FIRST + 45)
#endif
#ifndef TVM_SETAUTOSCROLLINFO
    #define TVM_SETAUTOSCROLLINFO           (TV_FIRST + 59)
#endif
#ifndef TVM_GETSELECTEDCOUNT
    #define TVM_GETSELECTEDCOUNT            (TV_FIRST + 70)
#endif
#ifndef TVM_SHOWINFOTIP
    #define TVM_SHOWINFOTIP                 (TV_FIRST + 71)
#endif
#ifndef TVM_GETITEMPARTRECT
    #define TVM_GETITEMPARTRECT             (TV_FIRST + 72)
#endif
#ifndef MCM_GETCURRENTVIEW
    #define MCM_GETCURRENTVIEW              (MCM_FIRST + 22)
#endif
#ifndef MCM_GETCALENDARCOUNT
    #define MCM_GETCALENDARCOUNT            (MCM_FIRST + 23)
#endif
#ifndef MCM_GETCALENDARGRIDINFO
    #define MCM_GETCALENDARGRIDINFO         (MCM_FIRST + 24)
#endif
#ifndef MCM_GETCALID
    #define MCM_GETCALID                    (MCM_FIRST + 25)
#endif
#ifndef MCM_SETCALID
    #define MCM_SETCALID                    (MCM_FIRST + 26)
#endif
#ifndef MCM_SIZERECTTOMIN
    #define MCM_SIZERECTTOMIN               (MCM_FIRST + 27)
#endif
#ifndef MCM_SETCALENDARBORDER
    #define MCM_SETCALENDARBORDER           (MCM_FIRST + 28)
#endif
#ifndef MCM_GETCALENDARBORDER
    #define MCM_GETCALENDARBORDER           (MCM_FIRST + 29)
#endif
#ifndef MCM_SETCURRENTVIEW
    #define MCM_SETCURRENTVIEW              (MCM_FIRST + 30)
#endif
#ifndef DTM_SETMCSTYLE
    #define DTM_SETMCSTYLE                  (DTM_FIRST + 11)
#endif
#ifndef DTM_GETMCSTYLE
    #define DTM_GETMCSTYLE                  (DTM_FIRST + 12)
#endif
#ifndef DTM_CLOSEMONTHCAL
    #define DTM_CLOSEMONTHCAL               (DTM_FIRST + 13)
#endif
#ifndef DTM_GETDATETIMEPICKERINFO
    #define DTM_GETDATETIMEPICKERINFO       (DTM_FIRST + 14)
#endif
#ifndef DTM_GETIDEALSIZE
    #define DTM_GETIDEALSIZE                (DTM_FIRST + 15)
#endif

#include "font.h"
#include "mem.h"
#include "savelbox.h"
#include "log.h"
#include "spymsgs.h"
#include "msgs.h"
#include "winpick.h"
#include "msgsel.h"
#include "selwin.h"
#include "peekmsg.h"
#include "peekwin.h"
#include "spymenu.h"
#include "ctl3d.h"
#ifdef USE_SNAP_WINDOW
    #include "desknt.h"
    #define SNAP_MODE   TRUE
#else
    #define SNAP_MODE   FALSE
#endif

#define RCSTR_MAX_LEN   128
#ifndef FILE_OPEN
    #define FILE_OPEN   1
#endif
#define FILE_SAVE       2

#define BITMAP_X                (23 + 4)
#define BITMAP_Y                (19 + 4)
#define BORDER_X( x )           ((x) / 4)
#define BORDER_Y( y )           ((y) / 16)
#define GET_TOOLBAR_HEIGHT( y ) ((y) + 2 * BORDER_Y( y ) + 3)
#define TOOLBAR_HEIGHT          GET_TOOLBAR_HEIGHT( BITMAP_Y )

/*
 * offsets in spy messages
 */
#define SPYOUT_HWND             26
#ifdef __NT__
    #define SPYOUT_HWND_LEN     8
    #define SPYOUT_MSG_LEN      8
    #define SPYOUT_WPARAM_LEN   8
    #define SPYOUT_LPARAM_LEN   8
    #define SPYOUT_WPARAM       SPYOUT_MSG + 1 + SPYOUT_MSG_LEN
    #define SPYOUT_LPARAM       SPYOUT_WPARAM + 1 + SPYOUT_WPARAM_LEN
#else
    #define SPYOUT_HWND_LEN     4
    #define SPYOUT_MSG_LEN      4
    #define SPYOUT_WPARAM_LEN   4
    #define SPYOUT_LPARAM_LEN   8
    #define SPYOUT_WPARAM       SPYOUT_MSG + 3 + SPYOUT_MSG_LEN
    #define SPYOUT_LPARAM       SPYOUT_WPARAM + 2 + SPYOUT_WPARAM_LEN
#endif

#define SPYOUT_MSG      SPYOUT_HWND + 1 + SPYOUT_HWND_LEN
#define SPYOUT_LENGTH   SPYOUT_LPARAM + SPYOUT_LPARAM_LEN


typedef enum {
    ON,
    OFF,
    NEITHER
} spystate;


typedef enum {
    MC_NULL,
    MC_CLIPBRD,
    MC_DDE,
    MC_IME,
    MC_INIT,
    MC_INPUT,
    MC_MDI,
    MC_MOUSE,
    MC_NC_MISC,
    MC_NC_MOUSE,
    MC_OTHER,
    MC_SYSTEM,
    MC_UNKNOWN,
    MC_USER,
    MC_WINDOW,
    MC_CONTROL
} MsgClass;

typedef struct {
    char        flag[2];
    MsgClass    type;
} filter;

typedef struct {
    filter clipboard;
    filter dde;
    filter ime;
    filter init;
    filter input;
    filter mdi;
    filter mouse;
    filter ncmisc;
    filter ncmouse;
    filter other;
    filter system;
    filter unknown;
    filter user;
    filter window;
} _filters;

#define FILTER_ENTRIES  13
typedef union {
    _filters filts;
    filter array[FILTER_ENTRIES];
} filters;
#define M_WATCH         0
#define M_STOPON        1
typedef struct {
    char        bits[2];
    WORD        id;
    char        *str;
    MsgClass    type;
    DWORD       count;
} message;

typedef struct {
    WORD                xsize;
    WORD                ysize;
    int                 xpos;
    int                 ypos;
    BOOL                minimized;
    int                 last_xpos;
    int                 last_ypos;
    BOOL                on_top;
    BOOL                show_hints;
}WndConfigInfo;

/*
 * globals
 */
extern char             *SpyName;
extern char             *TitleBar;
extern char             *TitleBarULine;
extern int              TitleBarLen;
extern spystate         SpyState;
extern char             near SpyPickClass[];
extern HWND             SpyListBox;
extern HWND             SpyListBoxTitle;
extern BOOL             SpyMessagesPaused;
extern HWND             SpyMainWindow;
extern HANDLE           MyTask;
extern HANDLE           Instance;
extern HANDLE           ResInstance;
extern filters          Filters;
extern WORD             MessageArraySize;
extern message          near MessageArray[];
extern WORD             EditMessageArraySize;
extern message          near EditMessageArray[];
extern WORD             ButtonMessageArraySize;
extern message          near ButtonMessageArray[];
extern WORD             StaticMessageArraySize;
extern message          near StaticMessageArray[];
extern WORD             ListBoxMessageArraySize;
extern message          near ListBoxMessageArray[];
extern WORD             ComboBoxMessageArraySize;
extern message          near ComboBoxMessageArray[];
#ifdef __NT__
extern WORD             ScrollBarMessageArraySize;
extern message          near ScrollBarMessageArray[];
extern WORD             HeaderMessageArraySize;
extern message          near HeaderMessageArray[];
extern WORD             ToolbarMessageArraySize;
extern message          near ToolbarMessageArray[];
extern WORD             RebarMessageArraySize;
extern message          near RebarMessageArray[];
extern WORD             ToolTipsMessageArraySize;
extern message          near ToolTipsMessageArray[];
extern WORD             StatusBarMessageArraySize;
extern message          near StatusBarMessageArray[];
extern WORD             TrackBarMessageArraySize;
extern message          near TrackBarMessageArray[];
extern WORD             UpDownMessageArraySize;
extern message          near UpDownMessageArray[];
extern WORD             ProgressBarMessageArraySize;
extern message          near ProgressBarMessageArray[];
extern WORD             HotKeyMessageArraySize;
extern message          near HotKeyMessageArray[];
extern WORD             ListViewMessageArraySize;
extern message          near ListViewMessageArray[];
extern WORD             TreeViewMessageArraySize;
extern message          near TreeViewMessageArray[];
extern WORD             ComboBoxExMessageArraySize;
extern message          near ComboBoxExMessageArray[];
extern WORD             TabControlMessageArraySize;
extern message          near TabControlMessageArray[];
extern WORD             AnimateMessageArraySize;
extern message          near AnimateMessageArray[];
extern WORD             MonthCalMessageArraySize;
extern message          near MonthCalMessageArray[];
extern WORD             DateTimeMessageArraySize;
extern message          near DateTimeMessageArray[];
#endif
extern BOOL             SpyMessagesAutoScroll;
extern BOOL             AutoSaveConfig;
extern WORD             WindowCount;
extern HWND             *WindowList;
extern LPVOID           HandleMessageInst;
extern WndConfigInfo    SpyMainWndInfo;
extern HMENU            SpyMenu;
extern statwnd          *StatusHdl;

/*
 * function prototypes
 */

/* spybox.c */
void SpyOut( char *msg, LPMSG pmsg );
void CreateSpyBox( HWND );
void ClearSpyBox( void );
void SpyMessagePauseToggle( void );
void ResizeSpyBox( WORD width, WORD height );
void SaveSpyBox( void );
void ResetSpyListBox( void );
BOOL GetSpyBoxSelection( char *str );

/* spycfg.c */
void LoadSpyConfig( char *fname );
void SaveSpyConfig( char *fname );
void DoSaveSpyConfig( void );
void DoLoadSpyConfig( void );

/* spyfilt.c */
void CALLBACK HandleMessage( LPMSG pmsg );

/* spymdlgs.c */
void DoMessageDialog( HWND hwnd, WORD wparam );
void DoMessageSelDialog( HWND hwnd );

/* spymisc.c */
void GetHexStr( LPSTR res, DWORD num, int padlen );
BOOL IsMyWindow( HWND hwnd );
void GetWindowName( HWND hwnd, char *str );
void GetClassStyleString( HWND hwnd, char *str, char *sstr );
void GetWindowStyleString( HWND hwnd, char *str, char *sstr );
void DumpToComboBox( char *str, HWND cb );
void FormatSpyMessage( char *msg, LPMSG pmsg, char *res );
void SetSpyState( spystate ss );
BOOL GetFileName( char *ext, int type, char *fname );
BOOL InitGblStrings( void );

/* spymsgs.c */
message *GetMessageDataFromID( int msgid, char *class_name );
void ProcessIncomingMessage( int msgid, char *class_name, char *res );
LPSTR GetMessageStructAddr( int msgid );
void InitMessages( void );
void SetFilterMsgs( MsgClass type, BOOL val, int bit );
char *SaveBitState( int x );
void RestoreBitState( char *data, int x );
void ClearMessageCount( void );
char *CloneBitState( char *old );
void FreeBitState( char *data );
void CopyBitState( char *dst, char *src );
void SetFilterSaveBitsMsgs( MsgClass type, BOOL val, char *bits );

/* spypick.c */
void FrameAWindow( HWND hwnd );
void UpdateFramedInfo( HWND dlg, HWND framedhwnd, BOOL ispick  );
HWND DoPickDialog( WORD );

/* spyproc.c */
LONG CALLBACK SpyWindowProc( HWND hwnd, UINT msg, UINT wparam, LONG lparam );
void SetSpyState( spystate ss );

/* spysel.c */
void ClearSelectedWindows( void );
void AddSelectedWindow( HWND hwnd );
void DoShowSelectedDialog( HWND hwnd, BOOL *spyall );
void ShowFramedInfo( HWND hwnd, HWND framed );

/* spytool.c */
void CreateSpyTool( HWND parent );
void DestroySpyTool( void );
void SetOnOffTool( spystate ss );
void ResizeSpyTool( WORD width, WORD height );

/* spylog.c */
void SpyLogTitle( int f );

/* spy.c */
void SpyFini( void );

/* spyzord.c */
HWND GetHwndFromPt( POINT *pt );
void IdentifyWindows( HWND toplevel, HWND topmost );
void RemoveWindow( HWND hwnd );
