/*
    OS/2 DosDevIOCtl structures and constants for 16-bit development.
*/

#ifdef __cplusplus
extern "C" {
#endif

#define BSEDEV_INCLUDED

#define IOCTL_ASYNC           0x0001
#define IOCTL_SCR_AND_PTRDRAW 0x0003
#define IOCTL_KEYBOARD        0x0004
#define IOCTL_PRINTER         0x0005
#define IOCTL_LIGHTPEN        0x0006
#define IOCTL_POINTINGDEVICE  0x0007
#define IOCTL_DISK            0x0008
#define IOCTL_PHYSICALDISK    0x0009
#define IOCTL_MONITOR         0x000A
#define IOCTL_GENERAL         0x000B

#define ASYNC_SETBAUDRATE    0x0041
#define ASYNC_SETLINECTRL    0x0042
#define ASYNC_TRANSMITIMM    0x0044
#define ASYNC_SETBREAKOFF    0x0045
#define ASYNC_SETMODEMCTRL   0x0046
#define ASYNC_SETBREAKON     0x004B
#define ASYNC_STOPTRANSMIT   0x0047
#define ASYNC_STARTTRANSMIT  0x0048
#define ASYNC_SETDCBINFO     0x0053
#define ASYNC_GETBAUDRATE    0x0061
#define ASYNC_GETLINECTRL    0x0062
#define ASYNC_GETCOMMSTATUS  0x0064
#define ASYNC_GETLINESTATUS  0x0065
#define ASYNC_GETMODEMOUTPUT 0x0066
#define ASYNC_GETMODEMINPUT  0x0067
#define ASYNC_GETINQUECOUNT  0x0068
#define ASYNC_GETOUTQUECOUNT 0x0069
#define ASYNC_GETCOMMERROR   0x006D
#define ASYNC_GETCOMMEVENT   0x0072
#define ASYNC_GETDCBINFO     0x0073

#define SCR_ALLOCLDT          0x0070
#define SCR_DEALLOCLDT        0x0071
#define PTR_GETPTRDRAWADDRESS 0x0072
#define SCR_ALLOCLDTOFF       0x0075

#define KBD_SETTRANSTABLE    0x0050
#define KBD_SETINPUTMODE     0x0051
#define KBD_SETINTERIMFLAG   0x0052
#define KBD_SETSHIFTSTATE    0x0053
#define KBD_SETTYPAMATICRATE 0x0054
#define KBD_SETFGNDSCREENGRP 0x0055
#define KBD_SETSESMGRHOTKEY  0x0056
#define KBD_SETFOCUS         0x0057
#define KBD_SETKCB           0x0058
#define KBD_SETNLS           0x005C
#define KBD_CREATE           0x005D
#define KBD_DESTROY          0x005E
#define KBD_GETINPUTMODE     0x0071
#define KBD_GETINTERIMFLAG   0x0072
#define KBD_GETSHIFTSTATE    0x0073
#define KBD_READCHAR         0x0074
#define KBD_PEEKCHAR         0x0075
#define KBD_GETSESMGRHOTKEY  0x0076
#define KBD_GETKEYBDTYPE     0x0077
#define KBD_GETCODEPAGEID    0x0078
#define KBD_XLATESCAN        0x0079

#define PRT_SETFRAMECTL      0x0042
#define PRT_SETINFINITERETRY 0x0044
#define PRT_INITPRINTER      0x0046
#define PRT_ACTIVATEFONT     0x0048
#define PRT_GETFRAMECTL      0x0062
#define PRT_GETINFINITERETRY 0x0064
#define PRT_GETPRINTERSTATUS 0x0066
#define PRT_QUERYACTIVEFONT  0x0069
#define PRT_VERIFYFONT       0x006A

#define MOU_ALLOWPTRDRAW       0x0050
#define MOU_UPDATEDISPLAYMODE  0x0051
#define MOU_SCREENSWITCH       0x0052
#define MOU_SETSCALEFACTORS    0x0053
#define MOU_SETEVENTMASK       0x0054
#define MOU_SETHOTKEYBUTTON    0x0055
#define MOU_SETPTRSHAPE        0x0056
#define MOU_DRAWPTR            0x0057
#define MOU_REMOVEPTR          0x0058
#define MOU_SETPTRPOS          0x0059
#define MOU_SETPROTDRAWADDRESS 0x005A
#define MOU_SETREALDRAWADDRESS 0x005B
#define MOU_SETMOUSTATUS       0x005C
#define MOU_DISPLAYMODECHANGE  0x005D
#define MOU_GETBUTTONCOUNT     0x0060
#define MOU_GETMICKEYCOUNT     0x0061
#define MOU_GETMOUSTATUS       0x0062
#define MOU_READEVENTQUE       0x0063
#define MOU_GETQUESTATUS       0x0064
#define MOU_GETEVENTMASK       0x0065
#define MOU_GETSCALEFACTORS    0x0066
#define MOU_GETPTRPOS          0x0067
#define MOU_GETPTRSHAPE        0x0068
#define MOU_GETHOTKEYBUTTON    0x0069
#define MOU_VER                0x006A

#define DSK_LOCKDRIVE        0x0000
#define DSK_UNLOCKDRIVE      0x0001
#define DSK_REDETERMINEMEDIA 0x0002
#define DSK_SETLOGICALMAP    0x0003
#define DSK_BLOCKREMOVABLE   0x0020
#define DSK_GETLOGICALMAP    0x0021
#define DSK_SETDEVICEPARAMS  0x0043
#define DSK_WRITETRACK       0x0044
#define DSK_FORMATVERIFY     0x0045
#define DSK_GETDEVICEPARAMS  0x0063
#define DSK_READTRACK        0x0064
#define DSK_VERIFYTRACK      0x0065

#define PDSK_LOCKPHYSDRIVE       0x0000
#define PDSK_UNLOCKPHYSDRIVE     0x0001
#define PDSK_WRITEPHYSTRACK      0x0044
#define PDSK_GETPHYSDEVICEPARAMS 0x0063
#define PDSK_READPHYSTRACK       0x0064
#define PDSK_VERIFYPHYSTRACK     0x0065

#define MON_REGISTERMONITOR 0x0040

#define DEV_FLUSHINPUT      0x0001
#define DEV_FLUSHOUTPUT     0x0002
#define DEV_QUERYMONSUPPORT 0x0060

#define RX_QUE_OVERRUN      0x0001
#define RX_HARDWARE_OVERRUN 0x0002
#define PARITY_ERROR        0x0004
#define FRAMING_ERROR       0x0008

#define CHAR_RECEIVED  0x0001
#define LAST_CHAR_SENT 0x0004
#define CTS_CHANGED    0x0008
#define DSR_CHANGED    0x0010
#define DCD_CHANGED    0x0020
#define BREAK_DETECTED 0x0040
#define ERROR_OCCURRED 0x0080
#define RI_DETECTED    0x0100

#define TX_WAITING_FOR_CTS        0x0001
#define TX_WAITING_FOR_DSR        0x0002
#define TX_WAITING_FOR_DCD        0x0004
#define TX_WAITING_FOR_XON        0x0008
#define TX_WAITING_TO_SEND_XON    0x0010
#define TX_WAITING_WHILE_BREAK_ON 0x0020
#define TX_WAITING_TO_SEND_IMM    0x0040
#define RX_WAITING_FOR_DSR        0x0080

#define WRITE_REQUEST_QUEUED   0x0001
#define DATA_IN_TX_QUE         0x0002
#define HARDWARE_TRANSMITTING  0x0004
#define CHAR_READY_TO_SEND_IMM 0x0008
#define WAITING_TO_SEND_XON    0x0010
#define WAITING_TO_SEND_XOFF   0x0020

#define CTS_ON 0x10
#define DSR_ON 0x20
#define RI_ON  0x40
#define DCD_ON 0x80

#define BUILD_BPB_FROM_MEDIUM  0x00
#define REPLACE_BPB_FOR_DEVICE 0x01
#define REPLACE_BPB_FOR_MEDIUM 0x02

#define ASCII_MODE  0x00
#define BINARY_MODE 0x80

#define CONVERSION_REQUEST 0x20
#define INTERIM_CHAR       0x80

#define HOTKEY_MAX_COUNT     0x0000
#define HOTKEY_CURRENT_COUNT 0x0001

#define KBD_DATA_RECEIVED 0x0001
#define KBD_DATA_BINARY   0x8000

#define KBD_READ_WAIT   0x0000
#define KBD_READ_NOWAIT 0x8000

#define SHIFT_REPORT_MODE 0x01

#define MHK_NO_HOTKEY 0x0000

#define MOUSE_QUEUEBUSY        0x0001
#define MOUSE_BLOCKREAD        0x0002
#define MOUSE_FLUSH            0x0004
#define MOUSE_UNSUPPORTED_MODE 0x0008
#define MOUSE_DISABLED         0x0100
#define MOUSE_MICKEYS          0x0200

#define PRINTER_TIMEOUT      0x0001
#define PRINTER_IO_ERROR     0x0008
#define PRINTER_SELECTED     0x0010
#define PRINTER_OUT_OF_PAPER 0x0020
#define PRINTER_ACKNOWLEDGED 0x0040
#define PRINTER_NOT_BUSY     0x0080

#define MODE_DTR_CONTROL     0x01
#define MODE_DTR_HANDSHAKE   0x02
#define MODE_CTS_HANDSHAKE   0x08
#define MODE_DSR_HANDSHAKE   0x10
#define MODE_DCD_HANDSHAKE   0x20
#define MODE_DSR_SENSITIVITY 0x40

#define MODE_AUTO_TRANSMIT   0x01
#define MODE_AUTO_RECEIVE    0x02
#define MODE_ERROR_CHAR      0x04
#define MODE_NULL_STRIPPING  0x08
#define MODE_BREAK_CHAR      0x10
#define MODE_RTS_CONTROL     0x40
#define MODE_RTS_HANDSHAKE   0x80
#define MODE_TRANSMIT_TOGGLE 0xC0

#define MODE_NO_WRITE_TIMEOUT    0x01
#define MODE_READ_TIMEOUT        0x02
#define MODE_WAIT_READ_TIMEOUT   0x04
#define MODE_NOWAIT_READ_TIMEOUT 0x06

#define DEVTYPE_48TPI   0x0000
#define DEVTYPE_96TPI   0x0001
#define DEVTYPE_35      0x0002
#define DEVTYPE_8SD     0x0003
#define DEVTYPE_8DD     0x0004
#define DEVTYPE_FIXED   0x0005
#define DEVTYPE_TAPE    0x0006
#define DEVTYPE_UNKNOWN 0x0007

#define DTR_ON 0x01
#define RTS_ON 0x02

#define DTR_OFF 0xFE
#define RTS_OFF 0xFD

#define RIGHTSHIFT    0x0001
#define LEFTSHIFT     0x0002
#define CONTROL       0x0004
#define ALT           0x0008
#define SCROLLLOCK_ON 0x0010
#define NUMLOCK_ON    0x0020
#define CAPSLOCK_ON   0x0040
#define INSERT_ON     0x0080
#define LEFTCONTROL   0x0100
#define LEFTALT       0x0200
#define RIGHTCONTROL  0x0400
#define RIGHTALT      0x0800
#define SCROLLLOCK    0x1000
#define NUMLOCK       0x2000
#define CAPSLOCK      0x4000
#define SYSREQ        0x8000

#ifndef INCL_MOU

#define MOUSE_MOTION               0x0001
#define MOUSE_MOTION_WITH_BN1_DOWN 0x0002
#define MOUSE_BN1_DOWN             0x0004
#define MOUSE_MOTION_WITH_BN2_DOWN 0x0008
#define MOUSE_BN2_DOWN             0x0010
#define MOUSE_MOTION_WITH_BN3_DOWN 0x0020
#define MOUSE_BN3_DOWN             0x0040

#define MHK_BUTTON1 0x0001
#define MHK_BUTTON2 0x0002
#define MHK_BUTTON3 0x0004

#define MOU_NOWAIT 0x0000
#define MOU_WAIT   0x0001

#endif

typedef struct _DCBINFO {
    USHORT usWriteTimeout;
    USHORT usReadTimeout;
    BYTE   fbCtlHndShake;
    BYTE   fbFlowReplace;
    BYTE   fbTimeout;
    BYTE   bErrorReplacementChar;
    BYTE   bBreakReplacementChar;
    BYTE   bXONChar;
    BYTE   bXOFFChar;
} DCBINFO, FAR *PDCBINFO;

typedef struct _TRACKLAYOUT {
    BYTE   bCommand;
    USHORT usHead;
    USHORT usCylinder;
    USHORT usFirstSector;
    USHORT cSectors;
    struct {
        USHORT usSectorNumber;
        USHORT usSectorSize;
    } TrackTable[1];
} TRACKLAYOUT, FAR *PTRACKLAYOUT;

typedef struct _BIOSPARAMETERBLOCK {
    USHORT usBytesPerSector;
    BYTE   bSectorsPerCluster;
    USHORT usReservedSectors;
    BYTE   cFATs;
    USHORT cRootEntries;
    USHORT cSectors;
    BYTE   bMedia;
    USHORT usSectorsPerFAT;
    USHORT usSectorsPerTrack;
    USHORT cHeads;
    ULONG  cHiddenSectors;
    ULONG  cLargeSectors;
    BYTE   abReserved[6];
    USHORT cCylinders;
    BYTE   bDeviceType;
    USHORT fsDeviceAttr;
} BIOSPARAMETERBLOCK, FAR *PBIOSPARAMETERBLOCK;

typedef struct _SCREENGROUP {
    USHORT idScreenGrp;
    USHORT fTerminate;
} SCREENGROUP, FAR *PSCREENGROUP;

typedef struct _FRAME {
    BYTE bCharsPerLine;
    BYTE bLinesPerInch;
} FRAME, FAR *PFRAME;

typedef struct _KBDTYPE {
    USHORT usType;
    USHORT reserved1;
    USHORT reserved2;
} KBDTYPE, FAR *PKBDTYPE;

typedef struct _LINECONTROL {
    BYTE bDataBits;
    BYTE bParity;
    BYTE bStopBits;
    BYTE fTransBreak;
} LINECONTROL, FAR *PLINECONTROL;

typedef struct _MODEMSTATUS {
    BYTE fbModemOn;
    BYTE fbModemOff;
} MODEMSTATUS, FAR *PMODEMSTATUS;

typedef struct _TRACKFORMAT {
    BYTE bCommand;
    USHORT usHead;
    USHORT usCylinder;
    USHORT usReserved;
    USHORT cSectors;
    struct {
        BYTE bCylinder;
        BYTE bHead;
        BYTE idSector;
        BYTE bBytesSector;
    } FormatTable[1];
} TRACKFORMAT, FAR *PTRACKFORMAT;

typedef struct _RXQUEUE {
    USHORT cch;
    USHORT cb;
} RXQUEUE, FAR *PRXQUEUE;

typedef struct _DEVICEPARAMETERBLOCK {
    USHORT reserved1;
    USHORT cCylinders;
    USHORT cHeads;
    USHORT cSectorsPerTrack;
    USHORT reserved2;
    USHORT reserved3;
    USHORT reserved4;
    USHORT reserved5;
} DEVICEPARAMETERBLOCK, FAR *PDEVICEPARAMETERBLOCK;

typedef struct _PTRDRAWFUNCTION {
    USHORT usReturnCode;
    PFN pfnDraw;
    PCH pchDataSeg;
} PTRDRAWFUNCTION, FAR *PPTRDRAWFUNCTION;

typedef struct _PTRDRAWADDRESS {
    USHORT reserved;
    PTRDRAWFUNCTION ptrdfnc;
} PTRDRAWADDRESS, FAR *PPTRDRAWADDRESS;

typedef struct _SHIFTSTATE {
    USHORT fsState;
    BYTE   fNLS;
} SHIFTSTATE, FAR *PSHIFTSTATE;

typedef struct _HOTKEY {
    USHORT fsHotKey;
    UCHAR  uchScancodeMake;
    UCHAR  uchScancodeBreak;
    USHORT idHotKey;
} HOTKEY, FAR *PHOTKEY;

typedef struct _MONITORPOSITION {
    USHORT fPosition;
    USHORT index;
    ULONG  pbInBuf;
    USHORT offOutBuf;
} MONITORPOSITION, FAR *PMONITORPOSITION;

typedef struct _RATEDELAY {
    USHORT usDelay;
    USHORT usRate;
} RATEDELAY, FAR *PRATEDELAY;

typedef struct _CODEPAGEINFO {
    PBYTE pbTransTable;
    USHORT idCodePage;
    USHORT idTable;
} CODEPAGEINFO, FAR *PCODEPAGEINFO;

typedef struct _CPID {
    USHORT idCodePage;
    USHORT Reserved;
} CPID, FAR *PCPID;

typedef struct _LDTADDRINFO {
    PULONG pulPhysAddr;
    USHORT cb;
} LDTADDRINFO, FAR *PLDTADDRINFO;

typedef struct _PTRDRAWDATA {
    USHORT cb;
    USHORT usConfig;
    USHORT usFlag;
} PTRDRAWDATA, FAR *PPTRDRAWDATA;

#ifdef __cplusplus
}
#endif
