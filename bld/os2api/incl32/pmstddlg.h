/*
    OS/2 Presentation Manager standard dialogs include file.
*/


#define STDDLG_INCLUDED

#ifdef INCL_WINSTDDLGS
    #define INCL_WINSTDDRAG
    #define INCL_WINSTDFILE
    #define INCL_WINSTDFONT
    #define INCL_WINSTDBOOK
    #define INCL_WINSTDSPIN
#endif

#if defined(INCL_WINSTDSPIN) && !defined(WINSTDSPIN_INCLUDED)
#define WINSTDSPIN_INCLUDED

#define SPBS_ALLCHARACTERS 0x00000000
#define SPBS_NUMERICONLY   0x00000001
#define SPBS_READONLY      0x00000002
#define SPBS_MASTER        0x00000010
#define SPBS_SERVANT       0x00000000
#define SPBS_JUSTDEFAULT   0x00000000
#define SPBS_JUSTLEFT      0x00000008
#define SPBS_JUSTRIGHT     0x00000004
#define SPBS_JUSTCENTER    0x0000000C
#define SPBS_NOBORDER      0x00000020
#define SPBS_FASTSPIN      0x00000100
#define SPBS_PADWITHZEROS  0x00000080

#define SPBN_UPARROW   0x20A
#define SPBN_DOWNARROW 0x20B
#define SPBN_ENDSPIN   0x20C
#define SPBN_CHANGE    0x20D
#define SPBN_SETFOCUS  0x20E
#define SPBN_KILLFOCUS 0x20F

#define SPBM_OVERRIDESETLIMITS 0x200
#define SPBM_QUERYLIMITS       0x201
#define SPBM_SETTEXTLIMIT      0x202
#define SPBM_SPINUP            0x203
#define SPBM_SPINDOWN          0x204
#define SPBM_QUERYVALUE        0x205
#define SPBM_SETARRAY          0x206
#define SPBM_SETLIMITS         0x207
#define SPBM_SETCURRENTVALUE   0x208
#define SPBM_SETMASTER         0x209

#define SPBQ_UPDATEIFVALID 0
#define SPBQ_ALWAYSUPDATE  1
#define SPBQ_DONOTUPDATE   3

typedef struct _SPBCDATA {
    ULONG  cbSize;
    ULONG  ulTextLimit;
    LONG   lLowerLimit;
    LONG   lUpperLimit;
    ULONG  idMasterSpb;
    PVOID  pHWXCtlData;
} SPBCDATA, *PSPBCDATA;

#endif

#if defined(INCL_WINSTDBOOK) && !defined(WINSTDBOOK_INCLUDED)
#define WINSTDBOOK_INCLUDED

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

#if defined(INCL_WINSTDFILE) && !defined(WINSTDFILE_INCLUDED)
#define WINSTDFILE_INCLUDED

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

#define DID_FILE_DIALOG        256
#define DID_FILENAME_TXT       257
#define DID_FILENAME_ED        258
#define DID_DRIVE_TXT          259
#define DID_DRIVE_CB           260
#define DID_FILTER_TXT         261
#define DID_FILTER_CB          262
#define DID_DIRECTORY_TXT      263
#define DID_DIRECTORY_LB       264
#define DID_FILES_TXT          265
#define DID_FILES_LB           266
#define DID_HELP_PB            267
#define DID_APPLY_PB           268
#define DID_READ_ONLY          269
#define DID_DIRECTORY_SELECTED 270
#define DID_OK_PB              DID_OK
#define DID_CANCEL_PB          DID_CANCEL

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
    USHORT  usDlgId;
    SHORT   x;
    SHORT   y;
    SHORT   sEAType;
} FILEDLG, *PFILEDLG;

MRESULT APIENTRY WinDefFileDlgProc(HWND,ULONG,MPARAM,MPARAM);
HWND    APIENTRY WinFileDlg(HWND,HWND,PFILEDLG);
BOOL    APIENTRY WinFreeFileDlgList(PAPSZ);

#endif

#if defined(INCL_WINSTDFONT) && !defined(WINSTDFONT_INCLUDED)
#define WINSTDFONT_INCLUDED

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

MRESULT APIENTRY WinDefFontDlgProc(HWND,ULONG,MPARAM,MPARAM);
HWND    APIENTRY WinFontDlg(HWND,HWND,PFONTDLG);

#endif

#if defined(INCL_WINSTDDRAG) && !defined(STDDRAG_INCLUDED)
#define STDDRAG_INCLUDED

#define PMERR_NOT_DRAGGING     0x1f00
#define PMERR_ALREADY_DRAGGING 0x1f01

#define MSGF_DRAG 0x0010

#define DM_DROP             0x032f
#define DM_DRAGOVER         0x032e
#define DM_DRAGLEAVE        0x032d
#define DM_DROPHELP         0x032c
#define DM_ENDCONVERSATION  0x032b
#define DM_PRINT            0x032a
#define DM_RENDER           0x0329
#define DM_RENDERCOMPLETE   0x0328
#define DM_RENDERPREPARE    0x0327
#define DM_DRAGFILECOMPLETE 0x0326
#define DM_EMPHASIZETARGET  0x0325
#define DM_DRAGERROR        0x0324
#define DM_FILERENDERED     0x0323
#define DM_RENDERFILE       0x0322
#define DM_DRAGOVERNOTIFY   0x0321
#define DM_PRINTOBJECT      0x0320
#define DM_DISCARDOBJECT    0x031f
#define DM_DROPNOTIFY       0x031e

#define DRT_ASM      "Assembler Code"
#define DRT_BASIC    "BASIC Code"
#define DRT_BINDATA  "Binary Data"
#define DRT_BITMAP   "Bitmap"
#define DRT_C        "C Code"
#define DRT_COBOL    "COBOL Code"
#define DRT_DLL      "Dynamic Link Library"
#define DRT_DOSCMD   "DOS Command File"
#define DRT_EXE      "Executable"
#define DRT_FORTRAN  "FORTRAN Code"
#define DRT_ICON     "Icon"
#define DRT_LIB      "Library"
#define DRT_METAFILE "Metafile"
#define DRT_OS2CMD   "OS/2 Command File"
#define DRT_PASCAL   "Pascal Code"
#define DRT_RESOURCE "Resource File"
#define DRT_TEXT     "Plain Text"
#define DRT_UNKNOWN  "Unknown"

#define DOR_NODROP    0x0000
#define DOR_DROP      0x0001
#define DOR_NODROPOP  0x0002
#define DOR_NEVERDROP 0x0003

#define DO_COPYABLE                0x0001
#define DO_MOVEABLE                0x0002
#define DO_LINKABLE                0x0004
#define DO_CREATEABLE              0x0008
#define DO_CREATEPROGRAMOBJECTABLE 0x0010

#define DC_OPEN            0x0001
#define DC_REF             0x0002
#define DC_GROUP           0x0004
#define DC_CONTAINER       0x0008
#define DC_PREPARE         0x0010
#define DC_REMOVEABLEMEDIA 0x0020

#define DO_DEFAULT             0xBFFE
#define DO_UNKNOWN             0xBFFF
#define DO_COPY                0x0010
#define DO_MOVE                0x0020
#define DO_LINK                0x0018
#define DO_CREATE              0x0040
#define DO_CREATEPROGRAMOBJECT 0x0080

#define DGS_DRAGINPROGRESS     0x0001
#define DGS_LAZYDRAGINPROGRESS 0x0002

#define DMFL_TARGETSUCCESSFUL 0x0001
#define DMFL_TARGETFAIL       0x0002
#define DMFL_NATIVERENDER     0x0004
#define DMFL_RENDERRETRY      0x0008
#define DMFL_RENDEROK         0x0010
#define DMFL_RENDERFAIL       0x0020

#define DRG_ICON        0x00000001
#define DRG_BITMAP      0x00000002
#define DRG_POLYGON     0x00000004
#define DRG_STRETCH     0x00000008
#define DRG_TRANSPARENT 0x00000010
#define DRG_CLOSED      0x00000020
#define DRG_MINIBITMAP  0x00000040

#define DME_IGNOREABORT    1
#define DME_IGNORECONTINUE 2
#define DME_REPLACE        3
#define DME_RETRY          4

#define DF_MOVE       0x0001
#define DF_SOURCE     0x0002
#define DF_SUCCESSFUL 0x0004

#define DRR_SOURCE 1
#define DRR_TARGET 2
#define DRR_ABORT  3

#define DFF_MOVE   1
#define DFF_COPY   2
#define DFF_DELETE 3

#define WM_DRAGFIRST 0x0310
#define WM_DRAGLAST  0x032f

typedef LHANDLE HSTR;

typedef struct _DRAGITEM {
    HWND   hwndItem;
    ULONG  ulItemID;
    HSTR   hstrType;
    HSTR   hstrRMF;
    HSTR   hstrContainerName;
    HSTR   hstrSourceName;
    HSTR   hstrTargetName;
    SHORT  cxOffset;
    SHORT  cyOffset;
    USHORT fsControl;
    USHORT fsSupportedOps;
} DRAGITEM, *PDRAGITEM;

typedef struct _DRAGINFO {
    ULONG  cbDraginfo;
    USHORT cbDragitem;
    USHORT usOperation;
    HWND   hwndSource;
    SHORT  xDrop;
    SHORT  yDrop;
    USHORT cditem;
    USHORT usReserved;
} DRAGINFO, *PDRAGINFO;

typedef struct _DRAGIMAGE {
    USHORT  cb;
    USHORT  cptl;
    LHANDLE hImage;
    SIZEL   sizlStretch;
    ULONG   fl;
    SHORT   cxOffset;
    SHORT   cyOffset;
} DRAGIMAGE, *PDRAGIMAGE;

typedef struct _DRAGTRANSFER {
    ULONG     cb;
    HWND      hwndClient;
    PDRAGITEM pditem;
    HSTR      hstrSelectedRMF;
    HSTR      hstrRenderToName;
    ULONG     ulTargetInfo;
    USHORT    usOperation;
    USHORT    fsReply;
} DRAGTRANSFER, *PDRAGTRANSFER;

typedef struct _RENDERFILE {
    HWND   hwndDragFiles;
    HSTR   hstrSource;
    HSTR   hstrTarget;
    USHORT fMove;
    USHORT usRsvd;
} RENDERFILE, *PRENDERFILE;

BOOL      APIENTRY DrgAcceptDroppedFiles(HWND,PCSZ,PCSZ,ULONG,ULONG);
BOOL      APIENTRY DrgAccessDraginfo(PDRAGINFO);
PDRAGINFO APIENTRY DrgAllocDraginfo(ULONG);
PDRAGTRANSFER APIENTRY DrgAllocDragtransfer(ULONG);
HSTR      APIENTRY DrgAddStrHandle(PCSZ);
BOOL      APIENTRY DrgCancelLazyDrag(VOID);
BOOL      APIENTRY DrgDeleteDraginfoStrHandles(PDRAGINFO);
BOOL      APIENTRY DrgDeleteStrHandle(HSTR);
HWND      APIENTRY DrgDrag(HWND,PDRAGINFO,PDRAGIMAGE,ULONG,LONG,PVOID);
BOOL      APIENTRY DrgDragFiles(HWND,PCSZ*,PCSZ*,PCSZ*,ULONG,HPOINTER,ULONG,BOOL,ULONG);
BOOL      APIENTRY DrgFreeDraginfo(PDRAGINFO);
BOOL      APIENTRY DrgFreeDragtransfer(PDRAGTRANSFER);
HPS       APIENTRY DrgGetPS(HWND);
BOOL      APIENTRY DrgLazyDrag(HWND, PDRAGINFO, PDRAGIMAGE, ULONG, PVOID);
BOOL      APIENTRY DrgLazyDrop(HWND, ULONG, PPOINTL);
BOOL      APIENTRY DrgPostTransferMsg(HWND,ULONG,PDRAGTRANSFER,ULONG,ULONG,BOOL);
BOOL      APIENTRY DrgPushDraginfo(PDRAGINFO, HWND);
PDRAGINFO APIENTRY DrgQueryDraginfoPtr(PDRAGINFO);
PDRAGINFO APIENTRY DrgQueryDraginfoPtrFromHwnd(HWND);
PDRAGINFO APIENTRY DrgQueryDraginfoPtrFromDragitem(PDRAGITEM);
BOOL      APIENTRY DrgQueryDragitem(PDRAGINFO,ULONG,PDRAGITEM,ULONG);
ULONG     APIENTRY DrgQueryDragitemCount(PDRAGINFO);
PDRAGITEM APIENTRY DrgQueryDragitemPtr(PDRAGINFO,ULONG);
ULONG     APIENTRY DrgQueryDragStatus(VOID);
ULONG     APIENTRY DrgQueryFormat(PDRAGITEM, PCSZ, ULONG, PCSZ, ULONG);
BOOL      APIENTRY DrgQueryNativeRMF(PDRAGITEM,ULONG,PCHAR);
ULONG     APIENTRY DrgQueryNativeRMFLen(PDRAGITEM);
ULONG     APIENTRY DrgQueryStrName(HSTR,ULONG,PCSZ);
ULONG     APIENTRY DrgQueryStrNameLen(HSTR);
BOOL      APIENTRY DrgQueryTrueType(PDRAGITEM,ULONG,PCSZ);
ULONG     APIENTRY DrgQueryTrueTypeLen(PDRAGITEM);
PDRAGINFO APIENTRY DrgReallocDraginfo (PDRAGINFO, ULONG);
BOOL      APIENTRY DrgReleasePS(HPS);
MRESULT   APIENTRY DrgSendTransferMsg(HWND,ULONG,MPARAM,MPARAM);
BOOL      APIENTRY DrgSetDragitem(PDRAGINFO,PDRAGITEM,ULONG,ULONG);
BOOL      APIENTRY DrgSetDragImage(PDRAGINFO,PDRAGIMAGE,ULONG,PVOID);
BOOL      APIENTRY DrgSetDragPointer(PDRAGINFO, HPOINTER);
BOOL      APIENTRY DrgVerifyTypeSet(PDRAGITEM,PCSZ,ULONG,PCSZ);
BOOL      APIENTRY DrgVerifyNativeRMF(PDRAGITEM, PCSZ);
BOOL      APIENTRY DrgVerifyRMF(PDRAGITEM, PCSZ, PCSZ);
BOOL      APIENTRY DrgVerifyTrueType(PDRAGITEM, PCSZ);
BOOL      APIENTRY DrgVerifyType(PDRAGITEM, PCSZ);

#endif


