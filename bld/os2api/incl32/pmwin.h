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
* Description:  OS/2 Presentation Manager window manager include file
*               for 32-bit development.
*
****************************************************************************/


#define INCL_WININCLUDED

#ifdef INCL_WIN
    #define INCL_WINBUTTONS
    #define INCL_WINCURSORS
    #define INCL_WINDIALOGS
    #define INCL_WINENTRYFIELDS
    #define INCL_WINERRORS
    #define INCL_WINFRAMEMGR
    #define INCL_WININPUT
    #define INCL_WINSCROLLBARS
    #define INCL_WINSTATICS
    #define INCL_WINWINDOWMGR
#endif

#define MPFROMP(p)          ((MPARAM)(VOID *)(p))
#define MPFROMHWND(hwnd)    ((MPARAM)(HWND)(hwnd))
#define MPFROMCHAR(ch)      ((MPARAM)(USHORT)(ch))
#define MPFROMSHORT(s)      ((MPARAM)(USHORT)(s))
#define MPFROM2SHORT(s1, s2)((MPARAM)MAKELONG(s1, s2))
#define MPFROMSH2CH(s, uch1, uch2) \
    ((MPARAM)MAKELONG(s, MAKESHORT(uch1, uch2)))
#define MPFROMLONG(l)       ((MPARAM)(ULONG)(l))
#define PVOIDFROMMP(mp)     ((VOID *)(mp))
#define HWNDFROMMP(mp)      ((HWND)(mp))
#define CHAR1FROMMP(mp)     ((UCHAR)(mp))
#define CHAR2FROMMP(mp)     ((UCHAR)((ULONG)mp >> 8))
#define CHAR3FROMMP(mp)     ((UCHAR)((ULONG)mp >> 16))
#define CHAR4FROMMP(mp)     ((UCHAR)((ULONG)mp >> 24))
#define SHORT1FROMMP(mp)    ((USHORT)(ULONG)(mp))
#define SHORT2FROMMP(mp)    ((USHORT)((ULONG)mp >> 16))
#define LONGFROMMP(mp)      ((ULONG)(mp))
#define MRFROMP(p)          ((MRESULT)(VOID *)(p))
#define MRFROMSHORT(s)      ((MRESULT)(USHORT)(s))
#define MRFROM2SHORT(s1, s2)((MRESULT)MAKELONG(s1, s2))
#define MRFROMLONG(l)       ((MRESULT)(ULONG)(l))
#define PVOIDFROMMR(mr)     ((VOID *)(mr))
#define SHORT1FROMMR(mr)    ((USHORT)((ULONG)mr))
#define SHORT2FROMMR(mr)    ((USHORT)((ULONG)mr >> 16))
#define LONGFROMMR(mr)      ((ULONG)(mr))

#define HWND_DESKTOP       (HWND)1
#define HWND_OBJECT        (HWND)2
#define HWND_TOP           (HWND)3
#define HWND_BOTTOM        (HWND)4
#define HWND_THREADCAPTURE (HWND)5

#define WC_FRAME           ((PSZ)0xffff0001)
#define WC_COMBOBOX        ((PSZ)0xffff0002)
#define WC_BUTTON          ((PSZ)0xffff0003)
#define WC_MENU            ((PSZ)0xffff0004)
#define WC_STATIC          ((PSZ)0xffff0005)
#define WC_ENTRYFIELD      ((PSZ)0xffff0006)
#define WC_LISTBOX         ((PSZ)0xffff0007)
#define WC_SCROLLBAR       ((PSZ)0xffff0008)
#define WC_TITLEBAR        ((PSZ)0xffff0009)
#define WC_MLE             ((PSZ)0xffff000A)
#define WC_APPSTAT         ((PSZ)0xffff0010)
#define WC_KBDSTAT         ((PSZ)0xffff0011)
#define WC_PECIC           ((PSZ)0xffff0012)
#define WC_DBE_KKPOPUP     ((PSZ)0xffff0013)
#define WC_SPINBUTTON      ((PSZ)0xffff0020)
#define WC_CONTAINER       ((PSZ)0xffff0025)
#define WC_SLIDER          ((PSZ)0xffff0026)
#define WC_VALUESET        ((PSZ)0xffff0027)
#define WC_NOTEBOOK        ((PSZ)0xffff0028)
#define WC_PENFIRST        ((PSZ)0xffff0029)
#define WC_PENLAST         ((PSZ)0xffff002C)
#define WC_MMPMFIRST       ((PSZ)0xffff0040)
#define WC_CIRCULARSLIDER  ((PSZ)0xffff0041)
#define WC_MMPMLAST        ((PSZ)0xffff004f)
#define WC_PRISTDDLGFIRST  ((PSZ)0xffff0050)
#define WC_PRISTDDLGLAST   ((PSZ)0xffff0057)
#define WC_PUBSTDDLGFIRST  ((PSZ)0xffff0058)
#define WC_PUBSTDDLGLAST   ((PSZ)0xffff005f)

#define WS_VISIBLE         0x80000000
#define WS_DISABLED        0x40000000
#define WS_CLIPCHILDREN    0x20000000
#define WS_CLIPSIBLINGS    0x10000000
#define WS_PARENTCLIP      0x08000000
#define WS_SAVEBITS        0x04000000
#define WS_SYNCPAINT       0x02000000
#define WS_MINIMIZED       0x01000000
#define WS_MAXIMIZED       0x00800000
#define WS_ANIMATE         0x00400000

#define WS_GROUP           0x00010000
#define WS_TABSTOP         0x00020000
#define WS_MULTISELECT     0x00040000

#define CS_MOVENOTIFY      0x00000001
#define CS_SIZEREDRAW      0x00000004
#define CS_HITTEST         0x00000008
#define CS_PUBLIC          0x00000010
#define CS_FRAME           0x00000020
#define CS_SYNCPAINT       0x02000000
#define CS_SAVEBITS        0x04000000
#define CS_PARENTCLIP      0x08000000
#define CS_CLIPSIBLINGS    0x10000000
#define CS_CLIPCHILDREN    0x20000000

typedef LHANDLE HWND;
typedef LHANDLE HAB;

typedef LONG    COLOR;
typedef COLOR   *PCOLOR;
typedef LHANDLE HBITMAP;
typedef HBITMAP *PHBITMAP;
typedef LHANDLE HDC;
typedef HDC     *PHDC;
typedef LHANDLE HMF;
typedef HMF     *PHMF;
typedef LHANDLE HPAL;
typedef HPAL    *PHPAL;
typedef LHANDLE HPS;
typedef HPS     *PHPS;
typedef LHANDLE HRGN;
typedef HRGN    *PHRGN;

typedef VOID    *MPARAM;
typedef MPARAM  *PMPARAM;
typedef VOID    *MRESULT;
typedef MRESULT *PMRESULT;

typedef MRESULT (APIENTRY *PFNWP)(HWND, USHORT, MPARAM, MPARAM);

typedef struct _POINTL {
    LONG  x;
    LONG  y;
} POINTL, *PPOINTL;

typedef struct _POINTS {
    SHORT x;
    SHORT y;
} POINTS, *PPOINTS;

typedef struct _RECTL {
    LONG xLeft;
    LONG yBottom;
    LONG xRight;
    LONG yTop;
} RECTL, *PRECTL;

#if defined(INCL_WINBUTTONS)

#define BS_PUSHBUTTON          0
#define BS_CHECKBOX            1
#define BS_AUTOCHECKBOX        2
#define BS_RADIOBUTTON         3
#define BS_AUTORADIOBUTTON     4
#define BS_3STATE              5
#define BS_AUTO3STATE          6
#define BS_USERBUTTON          7
#define BS_NOTEBOOKBUTTON      8
#define BS_PRIMARYSTYLES  0x000F

#define BS_TEXT           0x0010
#define BS_MINIICON       0x0020
#define BS_BITMAP         0x0040
#define BS_ICON           0x0080
#define BS_HELP           0x0100
#define BS_SYSCOMMAND     0x0200
#define BS_DEFAULT        0x0400
#define BS_NOPOINTERFOCUS 0x0800
#define BS_NOBORDER       0x1000
#define BS_NOCURSORSELECT 0x2000
#define BS_AUTOSIZE       0x4000

#define BN_CLICKED    1
#define BN_DBLCLICKED 2
#define BN_PAINT      3

#define BDS_HILITED  0x0100
#define BDS_DISABLED 0x0200
#define BDS_DEFAULT  0x0400

#define BM_CLICK           0x0120
#define BM_QUERYCHECKINDEX 0x0121
#define BM_QUERYHILITE     0x0122
#define BM_SETHILITE       0x0123
#define BM_QUERYCHECK      0x0124
#define BM_SETCHECK        0x0125
#define BM_SETDEFAULT      0x0126
#define BM_AUTOSIZE        0x0128

#pragma pack(2)

typedef struct _BTNCDATA {
    USHORT  cb;
    USHORT  fsCheckState;
    USHORT  fsHiliteState;
    LHANDLE hImage;
} BTNCDATA, *PBTNCDATA;

#pragma pack()

typedef struct _USERBUTTON {
    HWND  hwnd;
    HPS   hps;
    ULONG fsState;
    ULONG fsStateOld;
} USERBUTTON, *PUSERBUTTON;

#endif

#if defined(INCL_WINCURSORS)

BOOL  APIENTRY  WinShowCursor(HWND hwnd, BOOL fShow);

#endif

#if defined(INCL_WINWINDOWMGR)

#define QW_NEXT       0
#define QW_PREV       1
#define QW_TOP        2
#define QW_BOTTOM     3
#define QW_OWNER      4
#define QW_PARENT     5
#define QW_NEXTTOP    6
#define QW_PREVTOP    7
#define QW_FRAMEOWNER 8

#define AWP_MINIMIZED  0x00010000
#define AWP_MAXIMIZED  0x00020000
#define AWP_RESTORED   0x00040000
#define AWP_ACTIVATE   0x00080000
#define AWP_DEACTIVATE 0x00100000

#define SWP_SIZE            0x0001
#define SWP_MOVE            0x0002
#define SWP_ZORDER          0x0004
#define SWP_SHOW            0x0008
#define SWP_HIDE            0x0010
#define SWP_NOREDRAW        0x0020
#define SWP_NOADJUST        0x0040
#define SWP_ACTIVATE        0x0080
#define SWP_DEACTIVATE      0x0100
#define SWP_EXTSTATECHANGE  0x0200
#define SWP_MINIMIZE        0x0400
#define SWP_MAXIMIZE        0x0800
#define SWP_RESTORE         0x1000
#define SWP_FOCUSACTIVATE   0x2000
#define SWP_FOCUSDEACTIVATE 0x4000
#define SWP_NOAUTOCLOSE     0x8000

#define DT_LEFT            0x00000000
#define DT_QUERYEXTENT     0x00000002
#define DT_UNDERSCORE      0x00000010
#define DT_STRIKEOUT       0x00000020
#define DT_TEXTATTRS       0x00000040
#define DT_EXTERNALLEADING 0x00000080
#define DT_CENTER          0x00000100
#define DT_RIGHT           0x00000200
#define DT_TOP             0x00000000
#define DT_VCENTER         0x00000400
#define DT_BOTTOM          0x00000800
#define DT_HALFTONE        0x00001000
#define DT_MNEMONIC        0x00002000
#define DT_WORDBREAK       0x00004000
#define DT_ERASERECT       0x00008000

#define WM_NULL                 0x0000
#define WM_CREATE               0x0001
#define WM_DESTROY              0x0002
#define WM_ENABLE               0x0004
#define WM_SHOW                 0x0005
#define WM_MOVE                 0x0006
#define WM_SIZE                 0x0007
#define WM_ADJUSTWINDOWPOS      0x0008
#define WM_CALCVALIDRECTS       0x0009
#define WM_SETWINDOWPARAMS      0x000a
#define WM_QUERYWINDOWPARAMS    0x000b
#define WM_HITTEST              0x000c
#define WM_ACTIVATE             0x000d
#define WM_SETFOCUS             0x000f
#define WM_SETSELECTION         0x0010
#define WM_PPAINT               0x0011
#define WM_PSETFOCUS            0x0012
#define WM_PSYSCOLORCHANGE      0x0013
#define WM_PSIZE                0x0014
#define WM_PACTIVATE            0x0015
#define WM_PCONTROL             0x0016
#define WM_COMMAND              0x0020
#define WM_SYSCOMMAND           0x0021
#define WM_HELP                 0x0022
#define WM_PAINT                0x0023
#define WM_TIMER                0x0024
#define WM_SEM1                 0x0025
#define WM_SEM2                 0x0026
#define WM_SEM3                 0x0027
#define WM_SEM4                 0x0028
#define WM_CLOSE                0x0029
#define WM_QUIT                 0x002a
#define WM_SYSCOLORCHANGE       0x002b
#define WM_SYSVALUECHANGED      0x002d
#define WM_APPTERMINATENOTIFY   0x002e
#define WM_PRESPARAMCHANGED     0x002f
#define WM_CONTROL              0x0030
#define WM_VSCROLL              0x0031
#define WM_HSCROLL              0x0032
#define WM_INITMENU             0x0033
#define WM_MENUSELECT           0x0034
#define WM_MENUEND              0x0035
#define WM_DRAWITEM             0x0036
#define WM_MEASUREITEM          0x0037
#define WM_CONTROLPOINTER       0x0038
#define WM_QUERYDLGCODE         0x003a
#define WM_INITDLG              0x003b
#define WM_SUBSTITUTESTRING     0x003c
#define WM_MATCHMNEMONIC        0x003d
#define WM_SAVEAPPLICATION      0x003e
#define WM_SEMANTICEVENT        0x0490
#define WM_HELPBASE             0x0F00
#define WM_HELPTOP              0x0FFF
#define WM_USER                 0x1000

typedef struct _SWP {
    ULONG fl;
    LONG  cy;
    LONG  cx;
    LONG  y;
    LONG  x;
    HWND  hwndInsertBehind;
    HWND  hwnd;
    ULONG ulReserved1;
    ULONG ulReserved2;
} SWP, *PSWP;

HAB     APIENTRY WinInitialize(ULONG fsOptions);

HWND    APIENTRY WinQueryWindow(HWND hwnd, LONG lCode);
HDC     APIENTRY WinQueryWindowDC(HWND hwnd);
BOOL    APIENTRY WinQueryWindowPos(HWND hwnd, PSWP pswp);
BOOL    APIENTRY WinQueryWindowProcess(HWND hwnd, PPID ppid, PTID ptid);
PVOID   APIENTRY WinQueryWindowPtr(HWND hwnd, LONG index);
BOOL    APIENTRY WinQueryWindowRect(HWND hwnd, PRECTL prclDest);
LONG    APIENTRY WinQueryWindowText(HWND hwnd, LONG lLength, PCH pun);
LONG    APIENTRY WinQueryWindowTextLength(HWND hwnd);
ULONG   APIENTRY WinQueryWindowULong(HWND hwnd, LONG index);
USHORT  APIENTRY WinQueryWindowUShort(HWND hwnd, LONG index);

BOOL    APIENTRY WinTerminate(HAB hab);

MRESULT APIENTRY WinSendMsg(HWND hwnd, ULONG ulMsgid, MPARAM mpParam1, MPARAM mpParam2);

HWND    APIENTRY WinWindowFromDC(HDC hdc);
HWND    APIENTRY WinWindowFromID(HWND hwndParent, ULONG id);
HWND    APIENTRY WinWindowFromPoint(HWND hwndParent, PPOINTL pptlPoint, BOOL fEnumChildren);

#endif

#if defined(INCL_WINMESSAGEMGR)

typedef struct _QMSG {
    HWND  hwnd;
    USHORT msg;
    MPARAM mp1;
    MPARAM mp2;
    ULONG  time;
    POINTL ptl;
} QMSG, *PQMSG;

typedef LHANDLE     HMQ;

HMQ  APIENTRY WinCreateMsgQueue(HAB hab, SHORT sQueuesize);
BOOL APIENTRY WinDestroyMsgQueue(HMQ hmq);

#endif

#if defined(INCL_WINDIALOGS)

#define DID_OK     1
#define DID_CANCEL 2
#define DID_ERROR  0xffff

#define WA_WARNING    0
#define WA_NOTE       1
#define WA_ERROR      2
#define WA_CWINALARMS 3

#define MB_OK               0x0000
#define MB_OKCANCEL         0x0001
#define MB_RETRYCANCEL      0x0002
#define MB_ABORTRETRYIGNORE 0x0003
#define MB_YESNO            0x0004
#define MB_YESNOCANCEL      0x0005
#define MB_CANCEL           0x0006
#define MB_ENTER            0x0007
#define MB_ENTERCANCEL      0x0008

#define MB_NOICON           0x0000
#define MB_CUANOTIFICATION  0x0000
#define MB_ICONQUESTION     0x0010
#define MB_ICONEXCLAMATION  0x0020
#define MB_CUAWARNING       0x0020
#define MB_ICONASTERISK     0x0030
#define MB_ICONHAND         0x0040
#define MB_CUACRITICAL      0x0040
#define MB_QUERY            MB_ICONQUESTION
#define MB_WARNING          MB_CUAWARNING
#define MB_INFORMATION      MB_ICONASTERISK
#define MB_CRITICAL         MB_CUACRITICAL
#define MB_ERROR            MB_CRITICAL

#define MB_DEFBUTTON1       0x0000
#define MB_DEFBUTTON2       0x0100
#define MB_DEFBUTTON3       0x0200

#define MB_APPLMODAL        0x0000
#define MB_SYSTEMMODAL      0x1000
#define MB_HELP             0x2000
#define MB_MOVEABLE         0x4000

#define MBID_OK             1
#define MBID_CANCEL         2
#define MBID_ABORT          3
#define MBID_RETRY          4
#define MBID_IGNORE         5
#define MBID_YES            6
#define MBID_NO             7
#define MBID_HELP           8
#define MBID_ENTER          9
#define MBID_ERROR          (-1)

#pragma pack(2)

typedef struct _DLGTITEM {
    USHORT fsItemStatus;
    USHORT cChildren;
    USHORT cchClassName;
    USHORT offClassName;
    USHORT cchText;
    USHORT offText;
    ULONG  flStyle;
    SHORT  x;
    SHORT  y;
    SHORT  cx;
    SHORT  cy;
    USHORT id;
    USHORT offPresParams;
    USHORT offCtlData;
} DLGTITEM, *PDLGITEM;

typedef struct _DLGTEMPLATE {
    USHORT   cbTemplate;
    USHORT   type;
    USHORT   codepage;
    USHORT   offadlgti;
    USHORT   fsTemplateStatus;
    USHORT   iItemFocus;
    USHORT   coffPresParams;
    DLGTITEM adlgti[1];
} DLGTEMPLATE, *PDLGTEMPLATE;

#pragma pack()

USHORT APIENTRY WinMessageBox(HWND hwndParent, HWND hwndOwner, PSZ pszText, PSZ pszTitle,
                    USHORT usWindow, USHORT fsStyle);

#endif

#if defined(INCL_WINENTRYFIELDS)

#define ES_LEFT       0x00000000
#define ES_CENTER     0x00000001
#define ES_RIGHT      0x00000002
#define ES_AUTOSCROLL 0x00000004
#define ES_MARGIN     0x00000008
#define ES_AUTOTAB    0x00000010
#define ES_READONLY   0x00000020
#define ES_COMMAND    0x00000040
#define ES_UNREADABLE 0x00000080
#define ES_AUTOSIZE   0x00000200

#define ES_ANY        0x00000000
#define ES_SBCS       0x00001000
#define ES_DBCS       0x00002000
#define ES_MIXED      0x00003000

#define CBM_SHOWLIST      0x0170
#define CBM_HILITE        0x0171
#define CBM_ISLISTSHOWING 0x0172

#define CBID_LIST 0x029A
#define CBID_EDIT 0x029B

#define CBN_EFCHANGE 1
#define CBN_EFSCROLL 2
#define CBN_MEMERROR 3
#define CBN_LBSELECT 4
#define CBN_LBSCROLL 5
#define CBN_SHOWLIST 6
#define CBN_ENTER    7

#define CBS_SIMPLE       1
#define CBS_DROPDOWN     2
#define CBS_DROPDOWNLIST 4
#define CBS_COMPATIBLE   8

#define EN_SETFOCUS         0x0001
#define EN_KILLFOCUS        0x0002
#define EN_CHANGE           0x0004
#define EN_SCROLL           0x0008
#define EN_MEMERROR         0x0010
#define EN_OVERFLOW         0x0020
#define EN_INSERTMODETOGGLE 0x0040

#define EM_QUERYCHANGED   0x0140
#define EM_QUERYSEL       0x0141
#define EM_SETSEL         0x0142
#define EM_SETTEXTLIMIT   0x0143
#define EM_CUT            0x0144
#define EM_COPY           0x0145
#define EM_CLEAR          0x0146
#define EM_PASTE          0x0147
#define EM_QUERYFIRSTCHAR 0x0148
#define EM_SETFIRSTCHAR   0x0149
#define EM_QUERYREADONLY  0x014a
#define EM_SETREADONLY    0x014b
#define EM_SETINSERTMODE  0x014c

#pragma pack(2)

typedef struct _COMBOCDATA {
    ULONG cbSize;
    ULONG reserved;
    PVOID pHWXCtlData;
} COMBOCDATA, *PCOMBOCDATA;

typedef struct _ENTRYFDATA {
    USHORT cb;
    USHORT cchEditLimit;
    USHORT ichMinSel;
    USHORT ichMaxSel;
    PVOID  pHWXCtlData;
} ENTRYFDATA, *PENTRYFDATA;

#pragma pack()

#endif

#if defined(INCL_WINERRORS)

#include <pmerr.h>

typedef ULONG   ERRORID, *PERRORID;

typedef struct _ERRINFO {
    USHORT  cbFixedErrInfo;
    ERRORID idError;
    USHORT  cDetailLevel;
    USHORT  offaoffszMsg;
    USHORT  offBinaryData;
} ERRINFO, *PERRINFO;

BOOL     APIENTRY WinFreeErrorInfo(PERRINFO perrinfo);
PERRINFO APIENTRY WinGetErrorInfo(HAB hab);
ERRORID  APIENTRY WinGetLastError(HAB hab);

#endif

#if defined(INCL_WINFRAMEMGR)

#define FCF_TITLEBAR        0x00000001
#define FCF_SYSMENU         0x00000002
#define FCF_MENU            0x00000004
#define FCF_SIZEBORDER      0x00000008
#define FCF_MINBUTTON       0x00000010
#define FCF_MAXBUTTON       0x00000020
#define FCF_MINMAX          0x00000030
#define FCF_VERTSCROLL      0x00000040
#define FCF_HORZSCROLL      0x00000080
#define FCF_DLGBORDER       0x00000100
#define FCF_BORDER          0x00000200
#define FCF_SHELLPOSITION   0x00000400
#define FCF_TASKLIST        0x00000800
#define FCF_NOBYTEALIGN     0x00001000
#define FCF_NOMOVEWITHOWNER 0x00002000
#define FCF_ICON            0x00004000
#define FCF_ACCELTABLE      0x00008000
#define FCF_SYSMODAL        0x00010000
#define FCF_SCREENALIGN     0x00020000
#define FCF_MOUSEALIGN      0x00040000
#define FCF_HIDEBUTTON      0x01000000
#define FCF_HIDEMAX         0x01000020
#define FCF_CLOSEBUTTON     0x04000000
#define FCF_AUTOICON        0x40000000
#define FCF_DBE_APPSTAT     0x80000000

#define FCF_STANDARD        0x0000CC3F

#define FS_ICON            0x00000001
#define FS_ACCELTABLE      0x00000002
#define FS_SHELLPOSITION   0x00000004
#define FS_TASKLIST        0x00000008
#define FS_NOBYTEALIGN     0x00000010
#define FS_NOMOVEWITHOWNER 0x00000020
#define FS_SYSMODAL        0x00000040
#define FS_DLGBORDER       0x00000080
#define FS_BORDER          0x00000100
#define FS_SCREENALIGN     0x00000200
#define FS_MOUSEALIGN      0x00000400
#define FS_SIZEBORDER      0x00000800
#define FS_AUTOICON        0x00001000
#define FS_DBE_APPSTAT     0x00008000

#define FS_STANDARD        0x0000000F

#define FF_FLASHWINDOW     0x0001
#define FF_ACTIVE          0x0002
#define FF_FLASHHILITE     0x0004
#define FF_OWNERHIDDEN     0x0008
#define FF_DLGDISMISSED    0x0010
#define FF_OWNERDISABLED   0x0020
#define FF_SELECTED        0x0040
#define FF_NOACTIVATESWP   0x0080
#define FF_DIALOGBOX       0x0100

#define FI_FRAME           1
#define FI_OWNERHIDE       2
#define FI_ACTIVATEOK      4
#define FI_NOMOVEWITHOWNER 8

#define FID_SYSMENU      0x8002
#define FID_TITLEBAR     0x8003
#define FID_MINMAX       0x8004
#define FID_MENU         0x8005
#define FID_VERTSCROLL   0x8006
#define FID_HORZSCROLL   0x8007
#define FID_CLIENT       0x8008
#define FID_DBE_APPSTAT  0x8010
#define FID_DBE_KBDSTAT  0x8011
#define FID_DBE_PECIC    0x8012
#define FID_DBE_KKPOPUP  0x8013

#define SC_SIZE           0x8000
#define SC_MOVE           0x8001
#define SC_MINIMIZE       0x8002
#define SC_MAXIMIZE       0x8003
#define SC_CLOSE          0x8004
#define SC_NEXT           0x8005
#define SC_APPMENU        0x8006
#define SC_SYSMENU        0x8007
#define SC_RESTORE        0x8008
#define SC_NEXTFRAME      0x8009
#define SC_NEXTWINDOW     0x8010
#define SC_TASKMANAGER    0x8011
#define SC_HELPKEYS       0x8012
#define SC_HELPINDEX      0x8013
#define SC_HELPEXTENDED   0x8014
#define SC_SWITCHPANELIDS 0x8015
#define SC_DBE_FIRST      0x8018
#define SC_DBE_LAST       0x801F
#define SC_BEGINDRAG      0x8020
#define SC_ENDDRAG        0x8021
#define SC_SELECT         0x8022
#define SC_OPEN           0x8023
#define SC_CONTEXTMENU    0x8024
#define SC_CONTEXTHELP    0x8025
#define SC_TEXTEDIT       0x8026
#define SC_BEGINSELECT    0x8027
#define SC_ENDSELECT      0x8028
#define SC_WINDOW         0x8029
#define SC_HIDE           0x802a


#define WM_FLASHWINDOW        0x0040
#define WM_FORMATFRAME        0x0041
#define WM_UPDATEFRAME        0x0042
#define WM_FOCUSCHANGE        0x0043

#define WM_SETBORDERSIZE      0x0044
#define WM_TRACKFRAME         0x0045
#define WM_MINMAXFRAME        0x0046
#define WM_SETICON            0x0047
#define WM_QUERYICON          0x0048
#define WM_SETACCELTABLE      0x0049
#define WM_QUERYACCELTABLE    0x004a
#define WM_TRANSLATEACCEL     0x004b
#define WM_QUERYTRACKINFO     0x004c
#define WM_QUERYBORDERSIZE    0x004d
#define WM_NEXTMENU           0x004e
#define WM_ERASEBACKGROUND    0x004f
#define WM_QUERYFRAMEINFO     0x0050
#define WM_QUERYFOCUSCHAIN    0x0051
#define WM_OWNERPOSCHANGE     0x0052
#define WM_CALCFRAMERECT      0x0053

#define WM_WINDOWPOSCHANGED   0x0055
#define WM_ADJUSTFRAMEPOS     0x0056
#define WM_QUERYFRAMECTLCOUNT 0x0059

#define WM_QUERYHELPINFO      0x005B
#define WM_SETHELPINFO        0x005C
#define WM_ERROR              0x005D
#define WM_REALIZEPALETTE     0x005E

#pragma pack(2)

typedef struct _FRAMECDATA {
    USHORT cb;
    ULONG  flCreateFlags;
    USHORT hmodResources;
    USHORT idResources;
} FRAMECDATA;

#pragma pack()

#endif

#if defined(INCL_WININPUT)

#define VK_BUTTON1                 0x01
#define VK_BUTTON2                 0x02
#define VK_BUTTON3                 0x03
#define VK_BREAK                   0x04
#define VK_BACKSPACE               0x05
#define VK_TAB                     0x06
#define VK_BACKTAB                 0x07
#define VK_NEWLINE                 0x08
#define VK_SHIFT                   0x09
#define VK_CTRL                    0x0A
#define VK_ALT                     0x0B
#define VK_ALTGRAF                 0x0C
#define VK_PAUSE                   0x0D
#define VK_CAPSLOCK                0x0E
#define VK_ESC                     0x0F
#define VK_SPACE                   0x10
#define VK_PAGEUP                  0x11
#define VK_PAGEDOWN                0x12
#define VK_END                     0x13
#define VK_HOME                    0x14
#define VK_LEFT                    0x15
#define VK_UP                      0x16
#define VK_RIGHT                   0x17
#define VK_DOWN                    0x18
#define VK_PRINTSCRN               0x19
#define VK_INSERT                  0x1A
#define VK_DELETE                  0x1B
#define VK_SCRLLOCK                0x1C
#define VK_NUMLOCK                 0x1D
#define VK_ENTER                   0x1E
#define VK_SYSRQ                   0x1F
#define VK_F1                      0x20
#define VK_F2                      0x21
#define VK_F3                      0x22
#define VK_F4                      0x23
#define VK_F5                      0x24
#define VK_F6                      0x25
#define VK_F7                      0x26
#define VK_F8                      0x27
#define VK_F9                      0x28
#define VK_F10                     0x29
#define VK_F11                     0x2A
#define VK_F12                     0x2B
#define VK_F13                     0x2C
#define VK_F14                     0x2D
#define VK_F15                     0x2E
#define VK_F16                     0x2F
#define VK_F17                     0x30
#define VK_F18                     0x31
#define VK_F19                     0x32
#define VK_F20                     0x33
#define VK_F21                     0x34
#define VK_F22                     0x35
#define VK_F23                     0x36
#define VK_F24                     0x37
#define VK_ENDDRAG                 0x38
#define VK_CLEAR                   0x39
#define VK_EREOF                   0x3A
#define VK_PA1                     0x3B
#define VK_ATTN                    0x3C
#define VK_CRSEL                   0x3D
#define VK_EXSEL                   0x3E
#define VK_COPY                    0x3F
#define VK_BLK1                    0x40
#define VK_BLK2                    0x41


#define WM_MOUSEFIRST           0x0070
#define WM_MOUSELAST            0x0079
#define WM_BUTTONCLICKFIRST     0x0071
#define WM_BUTTONCLICKLAST      0x0079

#define WM_MOUSEMOVE            0x0070
#define WM_BUTTON1DOWN          0x0071
#define WM_BUTTON1UP            0x0072
#define WM_BUTTON1DBLCLK        0x0073
#define WM_BUTTON2DOWN          0x0074
#define WM_BUTTON2UP            0x0075
#define WM_BUTTON2DBLCLK        0x0076

#define WM_BUTTON3DOWN          0x0077
#define WM_BUTTON3UP            0x0078
#define WM_BUTTON3DBLCLK        0x0079

#define WM_MOUSEMAP             0x007D

#define WM_EXTMOUSEFIRST        0x0410
#define WM_EXTMOUSELAST         0x0419

#define WM_CHORD                0x0410
#define WM_BUTTON1MOTIONSTART   0x0411
#define WM_BUTTON1MOTIONEND     0x0412
#define WM_BUTTON1CLICK         0x0413
#define WM_BUTTON2MOTIONSTART   0x0414
#define WM_BUTTON2MOTIONEND     0x0415
#define WM_BUTTON2CLICK         0x0416
#define WM_BUTTON3MOTIONSTART   0x0417
#define WM_BUTTON3MOTIONEND     0x0418
#define WM_BUTTON3CLICK         0x0419

#define WM_BEGINDRAG            0x0420
#define WM_ENDDRAG              0x0421
#define WM_SINGLESELECT         0x0422
#define WM_OPEN                 0x0423
#define WM_CONTEXTMENU          0x0424
#define WM_CONTEXTHELP          0x0425
#define WM_TEXTEDIT             0x0426
#define WM_BEGINSELECT          0x0427
#define WM_ENDSELECT            0x0428
#define WM_PICKUP               0x0429

#define WM_PENFIRST             0x04C0
#define WM_PENLAST              0x04FF
#define WM_MMPMFIRST            0x0500
#define WM_MMPMLAST             0x05FF
#define WM_STDDLGFIRST          0x0600
#define WM_STDDLGLAST           0x065F

#define  WM_BIDI_FIRST          0x0BD0
#define  WM_BIDI_LAST           0x0BFF

#endif

#if defined(INCL_WINSCROLLBARS)

#define SBS_HORZ      0x0000
#define SBS_VERT      0x0001
#define SBS_THUMBSIZE 0x0002
#define SBS_AUTOTRACK 0x0004
#define SBS_AUTOSIZE  0x2000

#define SB_LINEUP         1
#define SB_LINEDOWN       2
#define SB_LINELEFT       1
#define SB_LINERIGHT      2
#define SB_PAGEUP         3
#define SB_PAGEDOWN       4
#define SB_PAGELEFT       3
#define SB_PAGERIGHT      4
#define SB_SLIDERTRACK    5
#define SB_SLIDERPOSITION 6
#define SB_ENDSCROLL      7

#define SBM_SETSCROLLBAR  0x01a0
#define SBM_SETPOS        0x01a1
#define SBM_QUERYPOS      0x01a2
#define SBM_QUERYRANGE    0x01a3
#define SBM_SETTHUMBSIZE  0x01a6

#pragma pack(2)

typedef struct _SBCDATA {
    USHORT cb;
    USHORT sHilite;
    SHORT  posFirst;
    SHORT  posLast;
    SHORT  posThumb;
    SHORT  cVisible;
    SHORT  cTotal;
} SBCDATA, *PSBCDATA;

#pragma pack()

#endif

#if defined(INCL_WINSTATICS)

#define SS_TEXT          0x0001
#define SS_GROUPBOX      0x0002
#define SS_ICON          0x0003
#define SS_BITMAP        0x0004
#define SS_FGNDRECT      0x0005
#define SS_HALFTONERECT  0x0006
#define SS_BKGNDRECT     0x0007
#define SS_FGNDFRAME     0x0008
#define SS_HALFTONEFRAME 0x0009
#define SS_BKGNDFRAME    0x000A
#define SS_SYSICON       0x000B
#define SS_AUTOSIZE      0x0040

#define SM_SETHANDLE     0x0100
#define SM_QUERYHANDLE   0x0101

#define WM_MSGBOXINIT    0x010E
#define WM_MSGBOXDISMISS 0x010F

#endif
