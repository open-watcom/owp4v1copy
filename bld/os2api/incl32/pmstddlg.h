/*
    OS/2 Presentation Manager standard dialogs include file.
*/


#define STDDLG_INCLUDED

#ifdef INCL_WINSTDDLGS
    #define INCL_WINSTDFILE
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

HWND   APIENTRY WinFileDlg(HWND hwndP, HWND hwndO, PFILEDLG pfild);
BOOL   APIENTRY WinFreeFileDlgList(PAPSZ papszFQFilename);
#endif

