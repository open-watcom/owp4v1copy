/*
    OS/2 Presentation Manager print support include file
    for 32-bit development.
*/


#ifndef __PMSPL_H__
#define __PMSPL_H__

#ifdef INCL_ERRORS
    #define INCL_SPLERRORS
#endif

#ifdef INCL_SPLERRORS
  #include <pmerr.h>
#endif

#define SPL_INI_SPOOLER         "PM_SPOOLER"
#define SPL_INI_QUEUE           "PM_SPOOLER_QUEUE"
#define SPL_INI_PRINTER         "PM_SPOOLER_PRINTER"
#define SPL_INI_PRINTERDESCR    "PM_SPOOLER_PRINTER_DESCR"
#define SPL_INI_QUEUEDESCR      "PM_SPOOLER_QUEUE_DESCR"
#define SPL_INI_QUEUEDD         "PM_SPOOLER_QUEUE_DD"
#define SPL_INI_QUEUEDDDATA     "PM_SPOOLER_QUEUE_DDDATA"

#define SPL_ERROR 0
#define SPL_OK    1

#define SPLC_ABORT    1
#define SPLC_PAUSE    2
#define SPLC_CONTINUE 3

#define QP_RAWDATA_BYPASS       1
#define QP_PRINT_SEPARATOR_PAGE 2

#define SPLINFO_QPERROR       0x0001
#define SPLINFO_DDERROR       0x0002
#define SPLINFO_SPLERROR      0x0004
#define SPLINFO_OTHERERROR    0x0080
#define SPLINFO_INFORMATION   0x0100
#define SPLINFO_WARNING       0x0200
#define SPLINFO_ERROR         0x0400
#define SPLINFO_SEVERE        0x0800
#define SPLINFO_USERINTREQD   0x1000

#define SPLDATA_PRINTERJAM    0x0001
#define SPLDATA_FORMCHGREQD   0x0002
#define SPLDATA_CARTCHGREQD   0x0004
#define SPLDATA_PENCHGREQD    0x0008
#define SPLDATA_DATAERROR     0x0010
#define SPLDATA_UNEXPECTERROR 0x0020
#define SPLDATA_OTHER         0x8000

#define QPDAT_ADDRESS     0
#define QPDAT_DRIVER_NAME 1
#define QPDAT_DRIVER_DATA 2
#define QPDAT_DATA_TYPE   3
#define QPDAT_COMMENT     4
#define QPDAT_PROC_PARAMS 5
#define QPDAT_SPL_PARAMS  6
#define QPDAT_NET_PARAMS  7
#define QPDAT_DOC_NAME    8
#define QPDAT_QUEUE_NAME  9
#define QPDAT_TOKEN      10
#define QPDAT_JOBID      11

#define SSQL_ERROR (-1L)

typedef LHANDLE HSPL;
typedef LHANDLE HSTD, *PHSTD;
typedef LHANDLE HPROC;
typedef PSZ     *PQMOPENDATA;
typedef PSZ     *PQPOPENDATA;
typedef ULONG   SPLERR;

typedef struct _SQPOPENDATA {
    PSZ       pszLogAddress;
    PSZ       pszDriverName;
    PDRIVDATA pdriv;
    PSZ       pszDataType;
    PSZ       pszComment;
    PSZ       pszProcParams;
    PSZ       pszSpoolParams;
    PSZ       pszNetworkParams;
    PSZ       pszDocName;
    PSZ       pszQueueName;
    PSZ       pszToken;
    USHORT    idJobId;
} SQPOPENDATA, *PSQPOPENDATA;

BOOL  APIENTRY SplQpClose(HPROC hproc);
BOOL  APIENTRY SplQpControl(HPROC hproc, LONG cmdCode);
BOOL  APIENTRY SplQpInstall(HWND hwnd);
HPROC APIENTRY SplQpOpen(LONG cData, PQPOPENDATA pQPDataIn);
BOOL  APIENTRY SplQpPrint(HPROC hproc, PSZ pszFileName);
BOOL  APIENTRY SplQpPrintSeparator(HPROC hproc, PBYTE pBuf, ULONG cbBuf, ULONG ulFlags);
BOOL  APIENTRY SplQpQueryDt(PLONG pcDataType, PSZ *paszDataTypes);
BOOL  APIENTRY SplQpQueryFlags(PULONG pulFlags);

#if defined(INCL_SPLDOSPRINT)

#define CNLEN 15
#define UNLEN 20
#define QNLEN 12
#define PDLEN  8
#define DTLEN  9

#define QP_DATATYPE_SIZE     15
#define DRIV_DEVICENAME_SIZE 31
#define DRIV_NAME_SIZE        8
#define PRINTERNAME_SIZE     32
#define FORMNAME_SIZE        31
#define MAXCOMMENTSZ         48

#define PRPORT_AUTODETECT   1
#define PRPORT_DISABLE_BIDI 2
#define PRPORT_ENABLE_BIDI  3

#define SPLPORT_VERSION_REGULAR 0
#define SPLPORT_VERSION_VIRTUAL 1

#define PRPO_PORT_DRIVER  1
#define PRPO_PROTOCOL_CNV 2
#define PRPO_MODE         3
#define PRPO_PRIORITY     4

#define PRQ_PRIORITY_PARMNUM         2
#define PRQ_STARTTIME_PARMNUM        3
#define PRQ_UNTILTIME_PARMNUM        4
#define PRQ_SEPARATOR_PARMNUM        5
#define PRQ_PROCESSOR_PARMNUM        6
#define PRQ_DESTINATIONS_PARMNUM     7
#define PRQ_PARMS_PARMNUM            8
#define PRQ_COMMENT_PARMNUM          9
#define PRQ_TYPE_PARMNUM            10
#define PRQ_PRINTERS_PARMNUM        12
#define PRQ_DRIVERNAME_PARMNUM      13
#define PRQ_DRIVERDATA_PARMNUM      14
#define PRQ_REMOTE_COMPUTER_PARMNUM 15
#define PRQ_REMOTE_QUEUE_PARMNUM    16
#define PRQ_MAXPARMNUM              16

#define PRQ_MAX_PRIORITY 1
#define PRQ_DEF_PRIORITY 5
#define PRQ_MIN_PRIORITY 9
#define PRQ_NO_PRIORITY  0

#define PRQ_STATUS_MASK 3
#define PRQ_ACTIVE      0
#define PRQ_PAUSED      1
#define PRQ_ERROR       2
#define PRQ_PENDING     3

#define PRQ3_PAUSED  0x1
#define PRQ3_PENDING 0x2

#define PRQ3_TYPE_RAW         0x0001
#define PRQ3_TYPE_BYPASS      0x0002
#define PRQ3_TYPE_APPDEFAULT  0x0004

#define PRJ_NOTIFYNAME_PARMNUM        3
#define PRJ_DATATYPE_PARMNUM          4
#define PRJ_PARMS_PARMNUM             5
#define PRJ_POSITION_PARMNUM          6
#define PRJ_JOBFILEINUSE_PARMNUM      7
#define PRJ_COMMENT_PARMNUM          11
#define PRJ_DOCUMENT_PARMNUM         12
#define PRJ_STATUSCOMMENT_PARMNUM    13
#define PRJ_PRIORITY_PARMNUM         14
#define PRJ_PROCPARMS_PARMNUM        16
#define PRJ_DRIVERDATA_PARMNUM       18
#define PRJ_SPOOLFILENAME_PARMNUM    19
#define PRJ_PAGESSPOOLED_PARMNUM     20
#define PRJ_PAGESSENT_PARMNUM        21
#define PRJ_PAGESPRINTED_PARMNUM     22
#define PRJ_TIMEPRINTED_PARMNUM      23
#define PRJ_EXTENDSTATUS_PARMNUM     24
#define PRJ_STARTPAGE_PARMNUM        25
#define PRJ_ENDPAGE_PARMNUM          26
#define PRJ_MAXPARMNUM               26

#define PRJ_QSTATUS      0x0003
#define PRJ_DEVSTATUS    0x0ffc
#define PRJ_COMPLETE     0x0004
#define PRJ_INTERV       0x0008
#define PRJ_ERROR        0x0010
#define PRJ_DESTOFFLINE  0x0020
#define PRJ_DESTPAUSED   0x0040
#define PRJ_NOTIFY       0x0080
#define PRJ_DESTNOPAPER  0x0100
#define PRJ_DESTFORMCHG  0x0200
#define PRJ_DESTCRTCHG   0x0400
#define PRJ_DESTPENCHG   0x0800
#define PRJ_JOBFILEINUSE 0x4000
#define PRJ_DELETED      0x8000

#define PRJ4_INPRINTER      0x00000001
#define PRJ4_STACKED        0x00000002
#define PRJ4_HELDINPRINTER  0x00000004
#define PRJ4_JOBSTARTED     0x00000008

#define PRJ_QS_QUEUED   0
#define PRJ_QS_PAUSED   1
#define PRJ_QS_SPOOLING 2
#define PRJ_QS_PRINTING 3

#define PRJ_MAX_PRIORITY 99
#define PRJ_MIN_PRIORITY  1
#define PRJ_NO_PRIORITY   0

#define PRD_STATUS_MASK 0x0003
#define PRD_DEVSTATUS   0x0ffc

#define PRD_ACTIVE  0
#define PRD_PAUSED  1

#define PRD_DELETE  0
#define PRD_PAUSE   1
#define PRD_CONT    2
#define PRD_RESTART 3

#define PRD_LOGADDR_PARMNUM 3
#define PRD_COMMENT_PARMNUM 7
#define PRD_DRIVERS_PARMNUM 8
#define PRD_TIMEOUT_PARMNUM 10

typedef struct _PRJINFO {
    USHORT uJobId;
    CHAR   szUserName[UNLEN+1];
    CHAR   pad_1;
    CHAR   szNotifyName[CNLEN+1];
    CHAR   szDataType[DTLEN+1];
    PSZ    pszParms;
    USHORT uPosition;
    USHORT fsStatus;
    PSZ    pszStatus;
    ULONG  ulSubmitted;
    ULONG  ulSize;
    PSZ    pszComment;
} PRJINFO, *PPRJINFO;

typedef struct _PRJINFO2 {
    USHORT uJobId;
    USHORT uPriority;
    PSZ    pszUserName;
    USHORT uPosition;
    USHORT fsStatus;
    ULONG  ulSubmitted;
    ULONG  ulSize;
    PSZ    pszComment;
    PSZ    pszDocument;
} PRJINFO2, *PPRJINFO2;

typedef struct _PRJINFO3 {
    USHORT uJobId;
    USHORT uPriority;
    PSZ    pszUserName;
    USHORT uPosition;
    USHORT fsStatus;
    ULONG  ulSubmitted;
    ULONG  ulSize;
    PSZ    pszComment;
    PSZ    pszDocument;
    PSZ    pszNotifyName;
    PSZ    pszDataType;
    PSZ    pszParms;
    PSZ    pszStatus;
    PSZ    pszQueue;
    PSZ    pszQProcName;
    PSZ    pszQProcParms;
    PSZ    pszDriverName;
    PDRIVDATA pDriverData;
    PSZ    pszPrinterName;
} PRJINFO3, *PPRJINFO3;

typedef struct _PRJINFO4 {
    USHORT uJobId;
    USHORT uPriority;
    PSZ    pszUserName;
    USHORT uPosition;
    USHORT fsStatus;
    ULONG  ulSubmitted;
    ULONG  ulSize;
    PSZ    pszComment;
    PSZ    pszDocument;
    PSZ    pszSpoolFileName;
    PSZ    pszPortName;
    PSZ    pszStatus;
    ULONG  ulPagesSpooled;
    ULONG  ulPagesSent;
    ULONG  ulPagesPrinted;
    ULONG  ulTimePrinted;
    ULONG  ulExtendJobStatus;
    ULONG  ulStartPage;
    ULONG  ulEndPage;
} PRJINFO4, *PPRJINFO4;

typedef struct _PRDINFO {
    CHAR   szName[PDLEN+1];
    CHAR   szUserName[UNLEN+1];
    USHORT uJobId;
    USHORT fsStatus;
    PSZ    pszStatus;
    USHORT time;
} PRDINFO, *PPRDINFO;

typedef struct _PRDINFO3 {
    PSZ    pszPrinterName;
    PSZ    pszUserName;
    PSZ    pszLogAddr;
    USHORT uJobId;
    USHORT fsStatus;
    PSZ    pszStatus;
    PSZ    pszComment;
    PSZ    pszDrivers;
    USHORT time;
    USHORT usTimeOut;
} PRDINFO3, *PPRDINFO3;

typedef struct _PRQINFO {
    CHAR   szName[QNLEN+1];
    CHAR   pad_1;
    USHORT uPriority;
    USHORT uStartTime;
    USHORT uUntilTime;
    PSZ    pszSepFile;
    PSZ    pszPrProc;
    PSZ    pszDestinations;
    PSZ    pszParms;
    PSZ    pszComment;
    USHORT fsStatus;
    USHORT cJobs;
} PRQINFO, *PPRQINFO;

typedef struct _PRQINFO3 {
    PSZ    pszName;
    USHORT uPriority;
    USHORT uStartTime;
    USHORT uUntilTime;
    USHORT fsType;
    PSZ    pszSepFile;
    PSZ    pszPrProc;
    PSZ    pszParms;
    PSZ    pszComment;
    USHORT fsStatus;
    USHORT cJobs;
    PSZ    pszPrinters;
    PSZ    pszDriverName;
    PDRIVDATA pDriverData;
} PRQINFO3, *PPRQINFO3;

typedef struct _PRQINFO6 {
    PSZ    pszName;
    USHORT uPriority;
    USHORT uStartTime;
    USHORT uUntilTime;
    USHORT fsType;
    PSZ    pszSepFile;
    PSZ    pszPrProc;
    PSZ    pszParms;
    PSZ    pszComment;
    USHORT fsStatus;
    USHORT cJobs;
    PSZ    pszPrinters;
    PSZ    pszDriverName;
    PDRIVDATA pDriverData;
    PSZ    pszRemoteComputerName;
    PSZ    pszRemoteQueueName;
} PRQINFO6, *PPRQINFO6;

typedef struct _PRIDINFO {
    USHORT uJobId;
    CHAR   szComputerName[CNLEN+1];
    CHAR   szQueueName[QNLEN+1];
    CHAR   pad_1;
} PRIDINFO, *PPRIDINFO;

typedef struct _QMJOBINFO {
    ULONG ulJobID;
    PSZ   pszComputerName;
    PSZ   pszQueueName;
} QMJOBINFO, *PQMJOBINFO;

typedef struct _PRDRIVINFO {
    CHAR szDrivName[DRIV_NAME_SIZE+1+DRIV_DEVICENAME_SIZE+1];
} PRDRIVINFO, *PPRDRIVINFO;

typedef struct _PRDRIVINFO2 {
    PSZ    pszPrinterName;
    PSZ    pszDriverName;
    USHORT usFlags;
    USHORT cDriverProps;
} PRDRIVINFO2, *PPRDRIVINFO2;

typedef struct _DRIVPROPS {
    PSZ   pszKeyName;
    ULONG cbBuf;
    PVOID pBuf;
} DRIVPROPS, *PDRIVPROPS;

typedef struct _PRQPROCINFO {
    CHAR szQProcName[QNLEN+1];
} PRQPROCINFO, *PPRQPROCINFO;

typedef struct _PRPORTINFO {
    CHAR szPortName[PDLEN+1];
} PRPORTINFO, *PPRPORTINFO;

typedef struct _PRPORTINFO1 {
    PSZ pszPortName ;
    PSZ pszPortDriverName ;
    PSZ pszPortDriverPathName ;
} PRPORTINFO1, *PPRPORTINFO1;

typedef struct _PRPORTINFO2 {
    PSZ   pszPortName;
    PSZ   pszPortDriver;
    PSZ   pszProtocolConverter;
    ULONG ulReserved;
    ULONG ulMode;
    ULONG ulPriority;
} PRPORTINFO2, *PPRPORTINFO2;

typedef struct _PRINTERINFO {
    ULONG flType;
    PSZ   pszComputerName;
    PSZ   pszPrintDestinationName;
    PSZ   pszDescription;
    PSZ   pszLocalName;
} PRINTERINFO, *PPRINTERINFO;

#define SPL_PR_QUEUE         0x00000001
#define SPL_PR_DIRECT_DEVICE 0x00000002
#define SPL_PR_QUEUED_DEVICE 0x00000004
#define SPL_PR_LOCAL_ONLY    0x00000100

#endif

ULONG  APIENTRY SplMessageBox(PCSZ pszLogAddr, ULONG fErrInfo, ULONG fErrData, PCSZ pszText,
                   PCSZ pszCaption, ULONG idWindow, ULONG fStyle);
BOOL   APIENTRY SplQmAbort(HSPL hspl);
BOOL   APIENTRY SplQmAbortDoc(HSPL hspl);
BOOL   APIENTRY SplQmClose(HSPL hspl);
BOOL   APIENTRY SplQmEndDoc(HSPL hspl);
ULONG  APIENTRY SplQmGetJobID(HSPL hspl, ULONG ulLevel, PVOID pBuf, ULONG cbBuf, PULONG pcbNeeded );
BOOL   APIENTRY SplQmNewPage(HSPL hspl, ULONG ulPageNumber );
HSPL   APIENTRY SplQmOpen(PCSZ pszToken, LONG lCount, PQMOPENDATA pqmdopData);
BOOL   APIENTRY SplQmStartDoc(HSPL hspl, PCSZ pszDocName);
BOOL   APIENTRY SplQmWrite(HSPL hspl, LONG lCount, PVOID pData);

BOOL   APIENTRY SplStdClose(HDC hdc);
BOOL   APIENTRY SplStdDelete(HSTD hMetaFile);
BOOL   APIENTRY SplStdGetBits(HSTD hMetaFile, LONG offData, LONG cbData, PCH pchData);
BOOL   APIENTRY SplStdOpen(HDC hdc);
LONG   APIENTRY SplStdQueryLength(HSTD hMetaFile);
BOOL   APIENTRY SplStdStart(HDC hdc);
HSTD   APIENTRY SplStdStop(HDC hdc);

SPLERR APIENTRY SplControlDevice(PCSZ pszComputerName, PCSZ pszPortName, ULONG ulControl);
SPLERR APIENTRY SplCopyJob(PCSZ pszSrcComputerName, PCSZ pszSrcQueueName, ULONG ulSrcJob,
                   PCSZ pszTrgComputerName, PCSZ pszTrgQueueName, PULONG pulTrgJob);
SPLERR APIENTRY SplCreateDevice(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf);
SPLERR APIENTRY SplCreatePort(PCSZ pszComputerName, PCSZ pszPortName, PCSZ pszPortDriver,
                   ULONG ulVersion, PVOID pBuf, ULONG cbBuf);
SPLERR APIENTRY SplCreateQueue(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf);
SPLERR APIENTRY SplDeleteDevice(PCSZ pszComputerName, PCSZ pszPrintDeviceName);
SPLERR APIENTRY SplDeleteJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulJob);
SPLERR APIENTRY SplDeletePort(PCSZ pszComputerName, PCSZ pszPortName);
SPLERR APIENTRY SplDeleteQueue(PCSZ pszComputerName, PCSZ pszQueueName);
SPLERR APIENTRY SplEnumDevice(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf,
                   PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded, PVOID pReserved);
SPLERR APIENTRY SplEnumDriver(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf,
                   PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded, PVOID pReserved);
SPLERR APIENTRY SplEnumJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulLevel, PVOID pBuf,
                   ULONG cbBuf, PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded,
                   PVOID pReserved);
SPLERR APIENTRY SplEnumPort(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf,
                   PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded, PVOID pReserved);
SPLERR APIENTRY SplEnumPrinter(PCSZ pszComputerName, ULONG uLevel, ULONG flType,
                   PVOID pBuf, ULONG cbbuf, PULONG pcReturned, PULONG pcTotal,
                   PULONG pcbNeeded, PVOID pReserved);
SPLERR APIENTRY SplEnumQueue(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf, ULONG cbBuf,
                   PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded, PVOID pReserved);
SPLERR APIENTRY SplEnumQueueProcessor(PCSZ pszComputerName, ULONG ulLevel, PVOID pBuf,
                   ULONG cbBuf, PULONG pcReturned, PULONG pcTotal, PULONG pcbNeeded,
                   PVOID  pReserved);
SPLERR APIENTRY SplHoldJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulJob);
SPLERR APIENTRY SplHoldQueue(PCSZ pszComputerName, PCSZ pszQueueName);
SPLERR APIENTRY SplPurgeQueue(PCSZ pszComputerName, PCSZ pszQueueName);
SPLERR APIENTRY SplQueryDevice(PCSZ pszComputerName, PCSZ pszPrintDeviceName, ULONG ulLevel,
                   PVOID pBuf, ULONG  cbBuf, PULONG pcbNeeded);
SPLERR APIENTRY SplQueryDriver(PCSZ pszComputerName, PCSZ pszDriverName, PCSZ pszPrinterName,
                   ULONG ulLevel, PVOID pBuf, ULONG cbBuf, PULONG pcbNeeded);
SPLERR APIENTRY SplQueryJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulJob, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, PULONG pcbNeeded);
SPLERR APIENTRY SplQueryPort(PCSZ pszComputerName, PCSZ pszPortName, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, PULONG pcbNeeded);
SPLERR APIENTRY SplQueryQueue(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, PULONG pcbNeeded);
SPLERR APIENTRY SplReleaseJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulJob);
SPLERR APIENTRY SplReleaseQueue(PCSZ pszComputerName, PCSZ pszQueueName);
SPLERR APIENTRY SplSetDevice(PCSZ pszComputerName, PCSZ pszPrintDeviceName, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, ULONG ulParmNum);
SPLERR APIENTRY SplSetDriver(PCSZ pszComputerName, PCSZ pszDriverName, PCSZ pszPrinterName,
                   ULONG ulLevel, PVOID pBuf, ULONG cbBuf, ULONG ulParmNum);
SPLERR APIENTRY SplSetJob(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulJob, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, ULONG ulParmNum);
SPLERR APIENTRY SplSetPort(PCSZ pszComputerName, PCSZ pszPortName, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, ULONG ulParmNum);
SPLERR APIENTRY SplSetQueue(PCSZ pszComputerName, PCSZ pszQueueName, ULONG ulLevel,
                   PVOID pBuf, ULONG cbBuf, ULONG ulParmNum);

#endif

