/*
    OS/2 Presentation Manager window manager include file
    for 16-bit development.
*/


#define INCL_WININCLUDED

#ifdef INCL_WIN
    #define INCL_WINDIALOGS
    #define INCL_WINDOWMGR
    #define INCL_WINERRORS
#endif

#define MPFROMP(p)          ((MPARAM)(VOID FAR *)(p))
#define MPFROMHWND(hwnd)    ((MPARAM)(HWND)(hwnd))
#define MPFROMCHAR(ch)      ((MPARAM)(USHORT)(ch))
#define MPFROMSHORT(s)      ((MPARAM)(USHORT)(s))
#define MPFROM2SHORT(s1, s2)((MPARAM)MAKELONG(s1, s2))
#define MPFROMSH2CH(s, uch1, uch2) \
    ((MPARAM)MAKELONG(s, MAKESHORT(uch1, uch2)))
#define MPFROMLONG(l)       ((MPARAM)(ULONG)(l))
#define PVOIDFROMMP(mp)     ((VOID FAR *)(mp))
#define HWNDFROMMP(mp)      ((HWND)(mp))
#define CHAR1FROMMP(mp)     ((UCHAR)(mp))
#define CHAR2FROMMP(mp)     ((UCHAR)((ULONG)mp >> 8))
#define CHAR3FROMMP(mp)     ((UCHAR)((ULONG)mp >> 16))
#define CHAR4FROMMP(mp)     ((UCHAR)((ULONG)mp >> 24))
#define SHORT1FROMMP(mp)    ((USHORT)(ULONG)(mp))
#define SHORT2FROMMP(mp)    ((USHORT)((ULONG)mp >> 16))
#define LONGFROMMP(mp)      ((ULONG)(mp))
#define MRFROMP(p)          ((MRESULT)(VOID FAR *)(p))
#define MRFROMSHORT(s)      ((MRESULT)(USHORT)(s))
#define MRFROM2SHORT(s1, s2)((MRESULT)MAKELONG(s1, s2))
#define MRFROMLONG(l)       ((MRESULT)(ULONG)(l))
#define PVOIDFROMMR(mr)     ((VOID FAR *)(mr))
#define SHORT1FROMMR(mr)    ((USHORT)((ULONG)mr))
#define SHORT2FROMMR(mr)    ((USHORT)((ULONG)mr >> 16))
#define LONGFROMMR(mr)      ((ULONG)(mr))

#define HWND_DESKTOP        (HWND)1
#define HWND_OBJECT         (HWND)2
#define HWND_TOP            (HWND)3
#define HWND_BOTTOM         (HWND)4
#define HWND_THREADCAPTURE  (HWND)5

#define WS_VISIBLE          0x80000000L
#define WS_DISABLED         0x40000000L
#define WS_CLIPCHILDREN     0x20000000L
#define WS_CLIPSIBLINGS     0x10000000L
#define WS_PARENTCLIP       0x08000000L
#define WS_SAVEBITS         0x04000000L
#define WS_SYNCPAINT        0x02000000L
#define WS_MINIMIZED        0x01000000L
#define WS_MAXIMIZED        0x00800000L

#define WS_GROUP            0x00010000L
#define WS_TABSTOP          0x00020000L
#define WS_MULTISELECT      0x00040000L

#define CS_MOVENOTIFY       0x00000001L
#define CS_SIZEREDRAW       0x00000004L
#define CS_HITTEST          0x00000008L
#define CS_PUBLIC           0x00000010L
#define CS_FRAME            0x00000020L
#define CS_CLIPCHILDREN     0x20000000L
#define CS_CLIPSIBLINGS     0x10000000L
#define CS_PARENTCLIP       0x08000000L
#define CS_SAVEBITS         0x04000000L
#define CS_SYNCPAINT        0x02000000L

typedef LHANDLE HWND;
typedef LHANDLE HAB;

typedef VOID FAR    *MPARAM;
typedef MPARAM FAR  *PMPARAM;
typedef VOID FAR    *MRESULT;
typedef MRESULT FAR *PMRESULT;

typedef MRESULT (PASCAL FAR *PFNWP)(HWND, USHORT, MPARAM, MPARAM);

typedef struct _POINTL {
    LONG  x;
    LONG  y;
} POINTL, FAR *PPOINTL;

#if (defined(INCL_WINWINDOWMGR) || !defined(INCL_NOCOMMON))

HAB  APIENTRY WinInitialize(USHORT fsOptions);
BOOL APIENTRY WinTerminate(HAB hab);

#endif

#if (defined(INCL_WINMESSAGEMGR) || !defined(INCL_NOCOMMON))

typedef struct _QMSG {
    HWND  hwnd;
    USHORT msg;
    MPARAM mp1;
    MPARAM mp2;
    ULONG  time;
    POINTL ptl;
} QMSG, FAR *PQMSG;

typedef LHANDLE     HMQ;

HMQ  APIENTRY WinCreateMsgQueue(HAB hab, SHORT sQueuesize);
BOOL APIENTRY WinDestroyMsgQueue(HMQ hmq);

#endif

#if (defined(INCL_WINDIALOGS) || !defined(INCL_NOCOMMON))

#define WA_WARNING      0
#define WA_NOTE         1
#define WA_ERROR        2
#define WA_CWINALARMS   3

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

USHORT APIENTRY WinMessageBox(HWND hwndParent, HWND hwndOwner, PSZ pszText, PSZ pszTitle,
                    USHORT usWindow, USHORT fsStyle);

#endif

#ifdef INCL_WINERRORS

#include <pmerr.h>

typedef ULONG   ERRORID;
typedef ERRORID FAR *PERRORID;

typedef struct _ERRINFO {
    USHORT  cbFixedErrInfo;
    ERRORID idError;
    USHORT  cDetailLevel;
    USHORT  offaoffszMsg;
    USHORT  offBinaryData;
} ERRINFO, FAR *PERRINFO;

BOOL     APIENTRY WinFreeErrorInfo(PERRINFO perrinfo);
PERRINFO APIENTRY WinGetErrorInfo(HAB hab);
ERRORID  APIENTRY WinGetLastError(HAB hab);

#endif
