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
* Description:  OS/2 Base subsystems include file for 16-bit development.
*
****************************************************************************/

#ifdef INCL_SUB
    #define INCL_KBD
    #define INCL_MOU
    #define INCL_VIO
#endif

#ifdef INCL_KBD

#define KR_KBDCHARIN       0x00000001
#define KR_KBDPEEK         0x00000002
#define KR_KBDFLUSHBUFFER  0x00000004
#define KR_KBDGETSTATUS    0x00000008
#define KR_KBDSETSTATUS    0x00000010
#define KR_KBDSTRINGIN     0x00000020
#define KR_KBDOPEN         0x00000040
#define KR_KBDCLOSE        0x00000080
#define KR_KBDGETFOCUS     0x00000100
#define KR_KBDFREEFOCUS    0x00000200
#define KR_KBDGETCP        0x00000400
#define KR_KBDSETCP        0x00000800
#define KR_KBDXLATE        0x00001000
#define KR_KBDSETCUSTXT    0x00002000

#define IO_WAIT            0
#define IO_NOWAIT          1

typedef SHANDLE    HKBD;
typedef HKBD FAR   *PHKBD;

typedef struct _KBDKEYINFO {
   UCHAR    chChar;
   UCHAR    chScan;
   UCHAR    fbStatus;
   UCHAR    bNlsShift;
   USHORT   fsState;
   ULONG    time;
}KBDKEYINFO, FAR *PKBDKEYINFO;

typedef struct _KBDHWID {
   USHORT length;
   USHORT kbd_id;
   USHORT reserved1;
   USHORT reserved2;
}KBDHWID, FAR *PKBDHWID;

typedef struct _KBDINFO {
   USHORT cb;
   USHORT fsMask;
   USHORT chTurnAround;
   USHORT fsInterim;
   USHORT fsState;
}KBDINFO, FAR *PKBDINFO;

typedef struct _STRINGINBUF {
  USHORT cb;
  USHORT cchIn;
}STRINGINBUF, FAR *PSTRINGINBUF;

typedef struct _KBDTRANS {
  UCHAR      chChar;
  UCHAR      chScan;
  UCHAR      fbStatus;
  UCHAR      bNlsShift;
  USHORT     fsState;
  ULONG      time;
  USHORT     fsDD;
  USHORT     fsXlate;
  USHORT     fsShift;
  USHORT     sZero;
}KBDTRANS, FAR *PKBDTRANS;

USHORT APIENTRY KbdCharIn(PKBDKEYINFO CharData, USHORT IOWait, HKBD KbdHandle);
USHORT APIENTRY KbdClose(HKBD KbdHandle);
USHORT APIENTRY KbdDeRegister(VOID);
USHORT APIENTRY KbdFlushBuffer(HKBD KbdHandle);
USHORT APIENTRY KbdFreeFocus(HKBD KbdHandle);
USHORT APIENTRY KbdGetCp(ULONG Reserved, PUSHORT CodePageID, HKBD KbdHandle);
USHORT APIENTRY KbdGetFocus(USHORT IOWait, HKBD KbdHandle);
USHORT APIENTRY KbdGetHWId(PKBDHWID KeyboardID, HKBD KbdHandle);
USHORT APIENTRY KbdGetStatus(PKBDINFO Structure, HKBD KbdHandle);
USHORT APIENTRY KbdOpen(PHKBD KbdHandle);
USHORT APIENTRY KbdPeek(PKBDKEYINFO CharData, HKBD KbdHandle);
USHORT APIENTRY KbdRegister(PSZ ModuleName, PSZ EntryPoint, ULONG FunctionMask);
USHORT APIENTRY KbdSetCp(USHORT Reserved, USHORT CodePageID, HKBD KbdHandle);
USHORT APIENTRY KbdSetCustXt(PUSHORT Xlatetable, HKBD KbdHandle);
USHORT APIENTRY KbdSetFgnd(VOID);
USHORT APIENTRY KbdSetStatus(PKBDINFO Structure, HKBD KbdHandle);
USHORT APIENTRY KbdStringIn(PCH CharBuffer, PSTRINGINBUF Length, USHORT IOWait, HKBD KbdHandle);
USHORT APIENTRY KbdSynch(USHORT IOWait);
USHORT APIENTRY KbdXlate(PKBDTRANS XlateRecord, HKBD KbdHandle);

#endif

#ifdef INCL_VIO

#define VR_VIOGETCURPOS    0x00000001
#define VR_VIOGETCURTYPE   0x00000002
#define VR_VIOGETMODE      0x00000004
#define VR_VIOGETBUF       0x00000008
#define VR_VIOGETPHYSBUF   0x00000010
#define VR_VIOSETCURPOS    0x00000020
#define VR_VIOSETCURTYPE   0x00000040
#define VR_VIOSETMODE      0x00000080
#define VR_VIOSHOWBUF      0x00000100
#define VR_VIOREADCHARSTR  0x00000200
#define VR_VIOREADCELLSTR  0x00000400
#define VR_VIOWRTNCHAR     0x00000800
#define VR_VIOWRTNATTR     0x00001000
#define VR_VIOWRTNCELL     0x00002000
#define VR_VIOWRTTTY       0x00004000
#define VR_VIOWRTCHARSTR   0x00008000

#define VR_VIOWRTCHARSTRATT  0x00010000
#define VR_VIOWRTCELLSTR     0x00020000
#define VR_VIOSCROLLUP       0x00040000
#define VR_VIOSCROLLDN       0x00080000
#define VR_VIOSCROLLLF       0x00100000
#define VR_VIOSCROLLRT       0x00200000
#define VR_VIOSETANSI        0x00400000
#define VR_VIOGETANSI        0x00800000
#define VR_VIOPRTSC          0x01000000
#define VR_VIOSCRLOCK        0x02000000
#define VR_VIOSCRUNLOCK      0x04000000
#define VR_VIOSAVREDRAWWAIT  0x08000000
#define VR_VIOSAVREDRAWUNDO  0x10000000
#define VR_VIOPOPUP          0x20000000
#define VR_VIOENDPOPUP       0x40000000
#define VR_VIOPRTSCTOGGLE    0x80000000

#define VGMT_OTHER         1
#define VGMT_GRAPHICS      2
#define VGMT_DISABLEBURST  4


typedef SHANDLE    HVIO;
typedef HVIO FAR   *PHVIO;

typedef struct _VIOCONFIGINFO {
   USHORT  cb;
   USHORT  adapter;
   USHORT  display;
   ULONG   cbMemory;
   USHORT  Configuration;
   USHORT  VDHVersion;
   USHORT  Flags;
   ULONG   HWBufferSize;
   ULONG   FullSaveSize;
   ULONG   PartSaveSize;
   USHORT  EMAdaptersOFF;
   USHORT  EMDisplaysOFF;
}VIOCONFIGINFO, FAR *PVIOCONFIGINFO;

typedef struct _VIOCURSORINFO {
   USHORT   yStart;
   USHORT   cEnd;
   USHORT   cx;
   USHORT   attr;
}VIOCURSORINFO, FAR *PVIOCURSORINFO;

typedef struct _VIOFONTINFO {
   USHORT  cb;
   USHORT  type;
   USHORT  cxCell;
   USHORT  cyCell;
   PVOID   pbData;
   USHORT  cbData;
}VIOFONTINFO, FAR *PVIOFONTINFO;

typedef struct _VIOMODEINFO {
   USHORT cb;
   UCHAR  fbType;
   UCHAR  color;
   USHORT col;
   USHORT row;
   USHORT hres;
   USHORT vres;
   UCHAR  fmt_ID;
   UCHAR  attrib;
   ULONG  buf_addr;
   ULONG  buf_length;
   ULONG  full_length;
   ULONG  partial_length;
   PCH    ext_data_addr;
}VIOMODEINFO, FAR *PVIOMODEINFO;

typedef struct _VIOPHYSBUF {
   PBYTE    pBuf;
   ULONG    cb;
   SEL      asel[1];
}VIOPHYSBUF, FAR *PVIOPHYSBUF;

typedef struct _VIOPALSTATE {
   USHORT  cb;
   USHORT  type;
   USHORT  iFirst;
   USHORT  acolor[1];
}VIOPALSTATE, FAR *PVIOPALSTATE;

typedef struct _VIOOVERSCAN {
   USHORT  cb;
   USHORT  type;
   USHORT  color;
}VIOOVERSCAN, FAR *PVIOOVERSCAN;

typedef struct _VIOINTENSITY {
   USHORT  cb;
   USHORT  type;
   USHORT  fs;
}VIOINTENSITY, FAR *PVIOINTENSITY;

typedef struct _VIOCOLORREG {
   USHORT  cb;
   USHORT  type;
   USHORT  firstcolorreg;
   USHORT  numcolorregs;
   PCH     colorregaddr;
}VIOCOLORREG, FAR *PVIOCOLORREG;

typedef struct _VIOSETULINELOC {
   USHORT  cb;
   USHORT  type;
   USHORT  scanline;
}VIOSETULINELOC, FAR *PVIOSETULINELOC;

typedef struct _VIOSETTARGET {
   USHORT  cb;
   USHORT  type;
   USHORT  defaultalgorithm;
}VIOSETTARGET, FAR *PVIOSETTARGET;

USHORT APIENTRY VioDeRegister(VOID);
USHORT APIENTRY VioEndPopUp(HVIO VioHandle);
USHORT APIENTRY VioGetAnsi(PUSHORT Indicator, HVIO VioHandle);
USHORT APIENTRY VioGetBuf(PULONG LVBPtr, PUSHORT Length, HVIO VioHandle);
USHORT APIENTRY VioGetConfig(USHORT ConfigID, PVIOCONFIGINFO ConfigData, HVIO VioHandle);
USHORT APIENTRY VioGetCp(USHORT Reserved, PUSHORT CodePageID, HVIO VioHandle);
USHORT APIENTRY VioGetCurPos(PUSHORT Row, PUSHORT Column, HVIO VioHandle);
USHORT APIENTRY VioGetCurType(PVIOCURSORINFO CursorData, HVIO VioHandle);
USHORT APIENTRY VioGetMode(PVIOMODEINFO ModeData, HVIO VioHandle);
USHORT APIENTRY VioGetPhysBuf(PVIOPHYSBUF Structure, USHORT Reserved);
USHORT APIENTRY VioGetState(PVOID RequestBlock, HVIO VioHandle);
USHORT APIENTRY VioGlobalReg(PSZ ModuleName, PSZ EntryPoint, ULONG FunctionMask1,
                       ULONG FunctionMask2, LONG Reserved);
USHORT APIENTRY VioModeUndo(USHORT OwnerIndic, USHORT KillIndic, USHORT Reserved);
USHORT APIENTRY VioModeWait(USHORT RequestType, PUSHORT NotifyType, USHORT Reserved);
USHORT APIENTRY VioPopUp(PUSHORT Options, HVIO VioHandle);
USHORT APIENTRY VioPrtSc(HVIO VioHandle);
USHORT APIENTRY VioPrtScToggle(HVIO VioHandle);
USHORT APIENTRY VioReadCellStr(PCH CellStr, PUSHORT Length, USHORT Row,
                       USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioReadCharStr(PCH CharStr, PUSHORT Length, USHORT Row,
                       USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioRegister(PSZ ModuleName, PSZ EntryPoint, ULONG FunctionMask1,
                       ULONG FunctionMask2);
USHORT APIENTRY VioSavRedrawUndo(USHORT OwnerIndic, USHORT KillIndic, HVIO VioHandle);
USHORT APIENTRY VioSavRedrawWait(USHORT SavRedrawIndic, PUSHORT NotifyType, HVIO VioHandle);
USHORT APIENTRY VioScrLock(USHORT WaitFlag, PUCHAR Status, HVIO VioHandle);
USHORT APIENTRY VioScrollDn(USHORT TopRow, USHORT LeftCol, USHORT BotRow, USHORT RightCol,
                       USHORT Lines, PBYTE Cell, HVIO VioHandle);
USHORT APIENTRY VioScrollLf(USHORT TopRow, USHORT LeftCol, USHORT BotRow, USHORT RightCol,
                       USHORT Lines, PBYTE Cell, HVIO VioHandle);
USHORT APIENTRY VioScrollRt(USHORT TopRow, USHORT LeftCol, USHORT BotRow, USHORT RightCol,
                       USHORT Lines, PBYTE Cell, HVIO VioHandle);
USHORT APIENTRY VioScrollUp(USHORT TopRow, USHORT LeftCol, USHORT BotRow, USHORT RightCol,
                       USHORT Lines, PBYTE Cell, HVIO VioHandle);
USHORT APIENTRY VioScrUnLock(HVIO VioHandle);
USHORT APIENTRY VioSetAnsi(USHORT Indicator, HVIO VioHandle);
USHORT APIENTRY VioSetCp(USHORT Reserved, USHORT CodePageID, HVIO VioHandle);
USHORT APIENTRY VioSetCurPos(USHORT Row, USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioSetCurType(PVIOCURSORINFO CursorData, HVIO VioHandle);
USHORT APIENTRY VioSetFont(PVIOFONTINFO RequestBlock, HVIO VioHandle);
USHORT APIENTRY VioSetMode(PVIOMODEINFO ModeData, HVIO VioHandle);
USHORT APIENTRY VioSetState(PVOID RequestBlock, HVIO VioHandle);
USHORT APIENTRY VioShowBuf(USHORT Offset, USHORT Length, HVIO VioHandle);
USHORT APIENTRY VioWrtCellStr(PCH CellStr, USHORT Length, USHORT Row, USHORT Column,
                       HVIO VioHandle);
USHORT APIENTRY VioWrtCharStr(PCH CharStr, USHORT Length, USHORT Row, USHORT Column,
                       HVIO VioHandle);
USHORT APIENTRY VioWrtCharStrAtt(PCH CharStr, USHORT Length, USHORT Row, USHORT Column,
                       PBYTE Attr, HVIO VioHandle);
USHORT APIENTRY VioWrtNAttr(PBYTE Attr, USHORT Times, USHORT Row, USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioWrtNCell(PBYTE Cell, USHORT Times, USHORT Row, USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioWrtNChar(PBYTE Cell, USHORT Times, USHORT Row, USHORT Column, HVIO VioHandle);
USHORT APIENTRY VioWrtTTY(PCH CharStr, USHORT Length, HVIO VioHandle);

#endif
