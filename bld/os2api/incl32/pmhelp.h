/*
    OS/2 Presentation Manager Help Manager include file
    for 32-bit development.
*/


#ifndef __PMHELP_H__

#define HM_RESOURCEID 0
#define HM_PANELNAME  1

#define HMPANELTYPE_NUMBER 0
#define HMPANELTYPE_NAME   1

#define CMIC_HIDE_PANEL_ID   0
#define CMIC_SHOW_PANEL_ID   1
#define CMIC_TOGGLE_PANEL_ID 2

#define HM_MSG_BASE               0x0220

#define HM_DISMISS_WINDOW             0x0220
#define HM_DISPLAY_HELP               0x0222
#define HM_EXT_HELP                   0x0223
#define HM_GENERAL_HELP               HM_EXT_HELP
#define HM_SET_ACTIVE_WINDOW          0x0224
#define HM_LOAD_HELP_TABLE            0x0225
#define HM_CREATE_HELP_TABLE          0x0226
#define HM_SET_HELP_WINDOW_TITLE      0x0227
#define HM_SET_SHOW_PANEL_ID          0x0228
#define HM_REPLACE_HELP_FOR_HELP      0x0229
#define HM_REPLACE_USING_HELP         HM_REPLACE_HELP_FOR_HELP
#define HM_HELP_INDEX                 0x022a
#define HM_HELP_CONTENTS              0x022b
#define HM_KEYS_HELP                  0x022c
#define HM_SET_HELP_LIBRARY_NAME      0x022d
#define HM_SET_OBJCOM_WINDOW          0x0238
#define HM_UPDATE_OBJCOM_WINDOW_CHAIN 0x0239
#define HM_QUERY_DDF_DATA             0x023a
#define HM_INVALIDATE_DDF_DATA        0x023b
#define HM_QUERY                      0x023c
#define HM_SET_COVERPAGE_SIZE         0x023d

typedef USHORT HELPSUBTABLE, *PHELPSUBTABLE;

#pragma pack(2)

typedef struct _HELPTABLE {
    USHORT        idAppWindow;
    PHELPSUBTABLE phstHelpSubTable;
    USHORT        idExtPanel;
} HELPTABLE, *PHELPTABLE;

typedef struct _HELPINIT {
    ULONG      cb;
    ULONG      ulReturnCode;
    PSZ        pszTutorialName;
    PHELPTABLE phtHelpTable;
    HMODULE    hmodHelpTableModule;
    HMODULE    hmodAccelActionBarModule;
    ULONG      idAccelTable;
    ULONG      idActionBar;
    PSZ        pszHelpWindowTitle;
    ULONG      fShowPanelId;
    PSZ        pszHelpLibraryName;
} HELPINIT, *PHELPINIT;

#pragma pack()

BOOL   APIENTRY WinAssociateHelpInstance(HWND hwndHelpInstance, HWND hwndApp);
HWND   APIENTRY WinCreateHelpInstance(HAB hab, PHELPINIT phinitHMInitStructure);
BOOL   APIENTRY WinCreateHelpTable(HWND hwndHelpInstance, PHELPTABLE phtHelpTable);
BOOL   APIENTRY WinDestroyHelpInstance(HWND hwndHelpInstance);

#endif
