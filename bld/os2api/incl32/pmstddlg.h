/*
    OS/2 Presentation Manager standard dialogs include file.
*/


#define STDDLG_INCLUDED

#ifdef INCL_WINSTDDLGS
    #define INCL_WINSTDFILE
    #define INCL_WINSTDFONT
    #define INCL_WINSTDBOOK
#endif

#if defined(INCL_WINSTDBOOK)

#define BKM_CALCPAGERECT         0x0353
#define BKM_DELETEPAGE           0x0354
#define BKM_INSERTPAGE           0x0355
#define BKM_INVALIDATETABS       0x0356
#define BKM_TURNTOPAGE           0x0357
#define BKM_QUERYPAGECOUNT       0x0358
#define BKM_QUERYPAGEID          0x0359
#define BKM_QUERYPAGEDATA        0x035a
#define BKM_QUERYPAGEWINDOWHWND  0x035b
#define BKM_QUERYTABBITMAP       0x035c
#define BKM_QUERYTABTEXT         0x035d
#define BKM_SETDIMENSIONS        0x035e
#define BKM_SETPAGEDATA          0x035f
#define BKM_SETPAGEWINDOWHWND    0x0360
#define BKM_SETSTATUSLINETEXT    0x0361
#define BKM_SETTABBITMAP         0x0362
#define BKM_SETTABTEXT           0x0363
#define BKM_SETNOTEBOOKCOLORS    0x0364
#define BKM_QUERYPAGESTYLE       0x0365
#define BKM_QUERYSTATUSLINETEXT  0x0366
#define BKM_SETPAGEINFO          0x0367
#define BKM_QUERYPAGEINFO        0x0368
#define BKM_SETTABCOLOR          0x0374
#define BKM_SETNOTEBOOKBUTTONS   0x0375

#define BKN_PAGESELECTED        130
#define BKN_NEWPAGESIZE         131
#define BKN_HELP                132
#define BKN_PAGEDELETED         133
#define BKN_PAGESELECTEDPENDING 134

#define BKA_ALL    1
#define BKA_SINGLE 2
#define BKA_TAB    4

#define BKA_LAST  0x0002
#define BKA_FIRST 0x0004
#define BKA_NEXT  0x0008
#define BKA_PREV  0x0010
#define BKA_TOP   0x0020

#define BKA_MAJORTAB   0x0001
#define BKA_MINORTAB   0x0002
#define BKA_PAGEBUTTON 0x0100

#define BKA_STATUSTEXTON 0x0001
#define BKA_MAJOR        0x0040
#define BKA_MINOR        0x0080
#define BKA_AUTOPAGESIZE 0x0100
#define BKA_END          0x0200

#define BKA_TEXT   0x0400
#define BKA_BITMAP 0x0800

#define BKA_AUTOCOLOR   (-1)
#define BKA_MAXBUTTONID 7999

#define BKS_BACKPAGESBR      0x00000001
#define BKS_BACKPAGESBL      0x00000002
#define BKS_BACKPAGESTR      0x00000004
#define BKS_BACKPAGESTL      0x00000008
#define BKS_MAJORTABRIGHT    0x00000010
#define BKS_MAJORTABLEFT     0x00000020
#define BKS_MAJORTABTOP      0x00000040
#define BKS_MAJORTABBOTTOM   0x00000080
#define BKS_SQUARETABS       0x00000000
#define BKS_ROUNDEDTABS      0x00000100
#define BKS_POLYGONTABS      0x00000200
#define BKS_SOLIDBIND        0x00000000
#define BKS_SPIRALBIND       0x00000400
#define BKS_STATUSTEXTLEFT   0x00000000
#define BKS_STATUSTEXTRIGHT  0x00001000
#define BKS_STATUSTEXTCENTER 0x00002000
#define BKS_TABTEXTLEFT      0x00000000
#define BKS_TABTEXTRIGHT     0x00004000
#define BKS_TABTEXTCENTER    0x00008000
#define BKS_TABBEDDIALOG     0x00000800
#define BKS_BUTTONAREA       0x00000200

#define BKA_BACKGROUNDPAGECOLORINDEX  0x0001
#define BKA_BACKGROUNDPAGECOLOR       0x0002
#define BKA_BACKGROUNDMAJORCOLORINDEX 0x0003
#define BKA_BACKGROUNDMAJORCOLOR      0x0004
#define BKA_BACKGROUNDMINORCOLORINDEX 0x0005
#define BKA_BACKGROUNDMINORCOLOR      0x0006
#define BKA_FOREGROUNDMAJORCOLORINDEX 0x0007
#define BKA_FOREGROUNDMAJORCOLOR      0x0008
#define BKA_FOREGROUNDMINORCOLORINDEX 0x0009
#define BKA_FOREGROUNDMINORCOLOR      0x000A

#define BFA_PAGEDATA            0x00000001
#define BFA_PAGEFROMHWND        0x00000002
#define BFA_PAGEFROMDLGTEMPLATE 0x00000004
#define BFA_PAGEFROMDLGRES      0x00000008
#define BFA_STATUSLINE          0x00000010
#define BFA_MAJORTABBITMAP      0x00000020
#define BFA_MINORTABBITMAP      0x00000040
#define BFA_MAJORTABTEXT        0x00000080
#define BFA_MINORTABTEXT        0x00000100
#define BFA_BIDIINFO            0x00000200

typedef struct _BOOKPAGEINFO {
    ULONG        cb;
    ULONG        fl;
    BOOL         bLoadDlg;
    ULONG        ulPageData;
    HWND         hwndPage;
    PFN          pfnPageDlgProc;
    ULONG        idPageDlg;
    HMODULE      hmodPageDlg;
    PVOID        pPageDlgCreateParams;
    PDLGTEMPLATE pdlgtPage;
    ULONG        cbStatusLine;
    PSZ          pszStatusLine;
    HBITMAP      hbmMajorTab;
    HBITMAP      hbmMinorTab;
    ULONG        cbMajorTab;
    PSZ          pszMajorTab;
    ULONG        cbMinorTab;
    PSZ          pszMinorTab;
    PVOID        pBidiInfo;
} BOOKPAGEINFO, *PBOOKPAGEINFO;

typedef struct _BOOKTEXT {
    PSZ   pString;
    ULONG textLen;
} BOOKTEXT, *PBOOKTEXT;

typedef struct _NOTEBOOKBUTTON {
    PSZ     pszText;
    ULONG   idButton;
    LHANDLE hImage;
    LONG    flStyle;
} NOTEBOOKBUTTON, *PNOTEBOOKBUTTON;

typedef struct _DELETENOTIFY {
    HWND    hwndBook;
    HWND    hwndPage;
    ULONG   ulAppPageData;
    HBITMAP hbmTab;
} DELETENOTIFY, *PDELETENOTIFY;

typedef struct _PAGESELECTNOTIFY {
    HWND  hwndBook;
    ULONG ulPageIdCur;
    ULONG ulPageIdNew;
} PAGESELECTNOTIFY, *PPAGESELECTNOTIFY;

#endif

#if defined(INCL_WINSTDFILE)

#define FDS_CENTER            0x00000001
#define FDS_CUSTOM            0x00000002
#define FDS_FILTERUNION       0x00000004
#define FDS_HELPBUTTON        0x00000008
#define FDS_APPLYBUTTON       0x00000010
#define FDS_PRELOAD_VOLINFO   0x00000020
#define FDS_MODELESS          0x00000040
#define FDS_INCLUDE_EAS       0x00000080
#define FDS_OPEN_DIALOG       0x00000100
#define FDS_SAVEAS_DIALOG     0x00000200
#define FDS_MULTIPLESEL       0x00000400
#define FDS_ENABLEFILELB      0x00000800
#define FDS_NATIONAL_LANGUAGE 0x80000000

#define FDS_SUCCESSFUL                 0
#define FDS_ERR_DEALLOCATE_MEMORY      1
#define FDS_ERR_FILTER_TRUNC           2
#define FDS_ERR_INVALID_DIALOG         3
#define FDS_ERR_INVALID_DRIVE          4
#define FDS_ERR_INVALID_FILTER         5
#define FDS_ERR_INVALID_PATHFILE       6
#define FDS_ERR_OUT_OF_MEMORY          7
#define FDS_ERR_PATH_TOO_LONG          8
#define FDS_ERR_TOO_MANY_FILE_TYPES    9
#define FDS_ERR_INVALID_VERSION       10
#define FDS_ERR_INVALID_CUSTOM_HANDLE 11
#define FDS_ERR_DIALOG_LOAD_ERROR     12
#define FDS_ERR_DRIVE_ERROR           13

#define FDS_EFSELECTION 0
#define FDS_LBSELECTION 1

typedef PSZ APSZ[1];
typedef APSZ *PAPSZ;

typedef struct _FILEDLG {
    ULONG   cbSize;
    ULONG   fl;
    ULONG   ulUser;
    LONG    lReturn;
    LONG    lSRC;
    PSZ     pszTitle;
    PSZ     pszOKButton;
    PFNWP   pfnDlgProc;
    PSZ     pszIType;
    PAPSZ   papszITypeList;
    PSZ     pszIDrive;
    PAPSZ   papszIDriveList;
    HMODULE hMod;
    CHAR    szFullFile[CCHMAXPATH];
    PAPSZ   papszFQFilename;
    ULONG   ulFQFCount;
    USHORT  usDlgID;
    SHORT   x;
    SHORT   y;
    SHORT   sEAType;
} FILEDLG, *PFILEDLG;

MRESULT APIENTRY WinDefFileDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
HWND    APIENTRY WinFileDlg(HWND hwndP, HWND hwndO, PFILEDLG pfild);
BOOL    APIENTRY WinFreeFileDlgList(PAPSZ papszFQFilename);

#endif

#if defined(INCL_WINSTDFONT)

#define CLRC_FOREGROUND 1
#define CLRC_BACKGROUND 2

#define FNTF_NOVIEWSCREENFONTS   1
#define FNTF_NOVIEWPRINTERFONTS  2
#define FNTF_SCREENFONTSELECTED  4
#define FNTF_PRINTERFONTSELECTED 8

#define FNTS_CENTER             0x00000001
#define FNTS_CUSTOM             0x00000002
#define FNTS_OWNERDRAWPREVIEW   0x00000004
#define FNTS_HELPBUTTON         0x00000008
#define FNTS_APPLYBUTTON        0x00000010
#define FNTS_RESETBUTTON        0x00000020
#define FNTS_MODELESS           0x00000040
#define FNTS_INITFROMFATTRS     0x00000080
#define FNTS_BITMAPONLY         0x00000100
#define FNTS_VECTORONLY         0x00000200
#define FNTS_FIXEDWIDTHONLY     0x00000400
#define FNTS_PROPORTIONALONLY   0x00000800
#define FNTS_NOSYNTHESIZEDFONTS 0x00001000
#define FNTS_NATIONAL_LANGUAGE  0x80000000

#define FNTS_SUCCESSFUL             0
#define FNTS_ERR_INVALID_DIALOG     3
#define FNTS_ERR_ALLOC_SHARED_MEM   4
#define FNTS_ERR_INVALID_PARM       5
#define FNTS_ERR_OUT_OF_MEMORY      7
#define FNTS_ERR_INVALID_VERSION   10
#define FNTS_ERR_DIALOG_LOAD_ERROR 12

#define FNTI_BITMAPFONT       0x0001
#define FNTI_VECTORFONT       0x0002
#define FNTI_FIXEDWIDTHFONT   0x0004
#define FNTI_PROPORTIONALFONT 0x0008
#define FNTI_SYNTHESIZED      0x0010
#define FNTI_DEFAULTLIST      0x0020
#define FNTI_FAMILYNAME       0x0100
#define FNTI_STYLENAME        0x0200
#define FNTI_POINTSIZE        0x0400

#define FNTM_FACENAMECHANGED  (WM_USER+50)
#define FNTM_POINTSIZECHANGED (WM_USER+51)
#define FNTM_STYLECHANGED     (WM_USER+52)
#define FNTM_COLORCHANGED     (WM_USER+53)
#define FNTM_UPDATEPREVIEW    (WM_USER+54)
#define FNTM_FILTERLIST       (WM_USER+55)

typedef struct _FONTDLG {
    ULONG   cbSize;
    HPS     hpsScreen;
    HPS     hpsPrinter;
    PSZ     pszTitle;
    PSZ     pszPreview;
    PSZ     pszPtSizeList;
    PFNWP   pfnDlgProc;
    PSZ     pszFamilyname;
    FIXED   fxPointSize;
    ULONG   fl;
    ULONG   flFlags;
    ULONG   flType;
    ULONG   flTypeMask;
    ULONG   flStyle;
    ULONG   flStyleMask;
    LONG    clrFore;
    LONG    clrBack;
    ULONG   ulUser;
    LONG    lReturn;
    LONG    lSRC;
    LONG    lEmHeight;
    LONG    lXHeight;
    LONG    lExternalLeading;
    HMODULE hMod;
    FATTRS  fAttrs;
    SHORT   sNominalPointSize;
    USHORT  usWeight;
    USHORT  usWidth;
    SHORT   x;
    SHORT   y;
    USHORT  usDlgId;
    USHORT  usFamilyBufLen;
    USHORT  usReserved;
} FONTDLG, *PFONTDLG;

typedef struct _STYLECHANGE {
    USHORT usWeight;
    USHORT usWeightOld;
    USHORT usWidth;
    USHORT usWidthOld;
    ULONG  flType;
    ULONG  flTypeOld;
    ULONG  flTypeMask;
    ULONG  flTypeMaskOld;
    ULONG  flStyle;
    ULONG  flStyleOld;
    ULONG  flStyleMask;
    ULONG  flStyleMaskOld;
} STYLECHANGE, *PSTYLECHANGE;

MRESULT APIENTRY WinDefFontDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
HWND    APIENTRY WinFontDlg(HWND hwndP, HWND hwndO, PFONTDLG pfntd);

#endif
