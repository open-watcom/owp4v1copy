/*
    OS/2 Base DOS include file for 32-bit development.
*/

#ifndef __BSEDOS_H__
#define __BSEDOS_H__

#ifdef INCL_DOS
    #define INCL_DOSDATETIME
    #define INCL_DOSDEVICES
    #define INCL_DOSEXCEPTIONS
    #define INCL_DOSFILEMGR
    #define INCL_DOSMEMMGR
    #define INCL_DOSMISC
    #define INCL_DOSMODULEMGR
    #define INCL_DOSMONITORS
    #define INCL_DOSMVDM
    #define INCL_DOSNLS
    #define INCL_DOSNMPIPES
    #define INCL_DOSPROCESS
    #define INCL_DOSPROFILE
    #define INCL_DOSQUEUES
    #define INCL_DOSRESOURCES
    #define INCL_DOSSEMAPHORES
    #define INCL_DOSSESMGR
#endif

#define CCHMAXPATH          260
#define CCHMAXPATHCOMP      256

#if (defined(INCL_DOSPROCESS) || !defined(INCL_NOCOMMON))

#define DosCwait          DosWaitChild
#define DosSetPrty        DosSetPriority

#define EXEC_SYNC          0
#define EXEC_ASYNC         1
#define EXEC_ASYNCRESULT   2
#define EXEC_TRACE         3
#define EXEC_BACKGROUND    4
#define EXEC_LOAD          5
#define EXEC_ASYNCRESULTDB 6

#define PRTYC_NOCHANGE         0
#define PRTYC_IDLETIME         1
#define PRTYC_REGULAR          2
#define PRTYC_TIMECRITICAL     3
#define PRTYC_FOREGROUNDSERVER 4

#define PRTYS_PROCESS     0
#define PRTYS_PROCESSTREE 1
#define PRTYS_THREAD      2

#define PRTYD_MINIMUM     (-31)
#define PRTYD_MAXIMUM     31

#define EXIT_THREAD      0
#define EXIT_PROCESS     1

#define EXLST_ADD        1
#define EXLST_REMOVE     2
#define EXLST_EXIT       3

#define DKP_PROCESSTREE  0
#define DKP_PROCESS      1

#define CREATE_READY     0
#define CREATE_SUSPENDED 1
#define STACK_SPARSE     0
#define STACK_COMMITTED  2

#define PT_FULLSCREEN    0
#define PT_REALMODE      1
#define PT_WINDOWABLEVIO 2
#define PT_PM            3
#define PT_DETACHED      4

#define PS_EXITLIST      1

#define TC_EXIT          0
#define TC_HARDERROR     1
#define TC_TRAP          2
#define TC_KILLPROCESS   3
#define TC_EXCEPTION     4

#define DCWA_PROCESS     0
#define DCWA_PROCESSTREE 1

#define DCWW_WAIT        0
#define DCWW_NOWAIT      1

#define AFNTY_THREAD     0
#define AFNTY_SYSTEM     1

typedef VOID APIENTRY FNTHREAD (ULONG);
typedef FNTHREAD *PFNTHREAD;
typedef VOID APIENTRY FNEXITLIST (ULONG);
typedef FNEXITLIST *PFNEXITLIST;

typedef struct _RESULTCODES {
    ULONG codeTerminate;
    ULONG codeResult;
} RESULTCODES, *PRESULTCODES;

typedef struct _THREADCREATE {
    ULONG     cbSize;
    PTID      pTid;
    PFNTHREAD pfnStart;
    ULONG     lParam;
    ULONG     lFlag;
    PBYTE     pStack;
    ULONG     cbStack;
} THREADCREATE, *PTHREADCREATE;

typedef struct _MPAFFINITY {
    ULONG mask[2];
} MPAFFINITY, *PMPAFFINITY;

typedef struct _PIB {
    ULONG pib_ulpid;
    ULONG pib_ulppid;
    ULONG pib_hmte;
    PCHAR pib_pchcmd;
    PCHAR pib_pchenv;
    ULONG pib_flstatus;
    ULONG pib_ultype;
} PIB, *PPIB;

typedef struct _TIB2 {
    ULONG  tib2_ultid;
    ULONG  tib2_ulpri;
    ULONG  tib2_version;
    USHORT tib2_usMCCount;
    USHORT tib2_fMCForceFlag;
} TIB2, *PTIB2;

typedef struct _TIB {
    PVOID tib_pexchain;
    PVOID tib_pstack;
    PVOID tib_pstacklimit;
    PTIB2 tib_ptib2;
    ULONG tib_version;
    ULONG tib_ordinal;
} TIB, *PTIB;

APIRET APIENTRY DosAllocThreadLocalMemory(ULONG cb, PULONG *p);
APIRET APIENTRY DosBeep(ULONG freq, ULONG dur);
APIRET APIENTRY DosCreateThread(PTID ptid, PFNTHREAD pfn, ULONG param, ULONG flag, ULONG cbStack);
APIRET APIENTRY DosCreateThread2(PTHREADCREATE ptc);
APIRET APIENTRY DosDebug(PVOID pdbgbuf);
APIRET APIENTRY DosEnterCritSec(VOID);
APIRET APIENTRY DosExecPgm(PCHAR pObjname, LONG cbObjname, ULONG execFlag,
                   PCSZ pArg, PCSZ pEnv, PRESULTCODES pRes, PCSZ pName);
VOID   APIENTRY DosExit(ULONG action, ULONG result);
APIRET APIENTRY DosExitCritSec(VOID);
APIRET APIENTRY DosExitList(ULONG ordercode, PFNEXITLIST pfn);
APIRET APIENTRY DosFreeThreadLocalMemory(ULONG *p);
APIRET APIENTRY DosGetInfoBlocks(PTIB *pptib, PPIB *pppib);
APIRET APIENTRY DosKillProcess(ULONG action, PID pid);
APIRET APIENTRY DosKillThread(TID tid);
APIRET APIENTRY DosResumeThread(TID tid);
APIRET APIENTRY DosSetPriority(ULONG scope, ULONG ulClass, LONG delta, ULONG PorTid);
APIRET APIENTRY DosSleep(ULONG msec);
APIRET APIENTRY DosSuspendThread(TID tid);
APIRET APIENTRY DosWaitChild(ULONG action, ULONG option, PRESULTCODES pres, PPID ppid, PID pid);
APIRET APIENTRY DosWaitThread(PTID ptid, ULONG option);

#endif

#if defined(INCL_DOSFILEMGR) || !defined(INCL_NOCOMMON)

#define DosBufReset    DosResetBuffer
#define DosChDir       DosSetCurrentDir
#define DosChgFilePtr  DosSetFilePtr
#define DosMkDir       DosCreateDir
#define DosNewSize     DosSetFileSize
#define DosQCurDir     DosQueryCurrentDir
#define DosQCurDisk    DosQueryCurrentDisk
#define DosQFileInfo   DosQueryFileInfo
#define DosQFHandState DosQueryFHState
#define DosQFSInfo     DosQueryFSInfo
#define DosQHandType   DosQueryHType
#define DosQPathInfo   DosQueryPathInfo
#define DosSelectDisk  DosSetDefaultDisk

#define FILE_BEGIN    0
#define FILE_CURRENT  1
#define FILE_END      2

#define HDIR_SYSTEM   1
#define HDIR_CREATE   (-1)

#define DCPY_EXISTING 1
#define DCPY_APPEND   2
#define DCPY_FAILEAS  4

#define FILE_NORMAL    0x0000
#define FILE_READONLY  0x0001
#define FILE_HIDDEN    0x0002
#define FILE_SYSTEM    0x0004
#define FILE_DIRECTORY 0x0010
#define FILE_ARCHIVED  0x0020

#define FILE_EXISTED   1
#define FILE_CREATED   2
#define FILE_TRUNCATED 3

#define FILE_OPEN      0x0001
#define FILE_TRUNCATE  0x0002
#define FILE_CREATE    0x0010

#define OPEN_ACTION_FAIL_IF_EXISTS     0
#define OPEN_ACTION_OPEN_IF_EXISTS     1
#define OPEN_ACTION_REPLACE_IF_EXISTS  2

#define OPEN_ACTION_FAIL_IF_NEW     0x0000
#define OPEN_ACTION_CREATE_IF_NEW   0x0010

#define OPEN_ACCESS_READONLY        0x0000
#define OPEN_ACCESS_WRITEONLY       0x0001
#define OPEN_ACCESS_READWRITE       0x0002
#define OPEN_SHARE_DENYREADWRITE    0x0010
#define OPEN_SHARE_DENYWRITE        0x0020
#define OPEN_SHARE_DENYREAD         0x0030
#define OPEN_SHARE_DENYNONE         0x0040
#define OPEN_FLAGS_NOINHERIT        0x0080
#define OPEN_FLAGS_NO_LOCALITY      0x0000
#define OPEN_FLAGS_SEQUENTIAL       0x0100
#define OPEN_FLAGS_RANDOM           0x0200
#define OPEN_FLAGS_RANDOMSEQUENTIAL 0x0300
#define OPEN_FLAGS_NO_CACHE         0x1000
#define OPEN_FLAGS_FAIL_ON_ERROR    0x2000
#define OPEN_FLAGS_WRITE_THROUGH    0x4000
#define OPEN_FLAGS_DASD             0x8000

#define SEARCH_PATH          0
#define SEARCH_CUR_DIRECTORY 1
#define SEARCH_ENVIRONMENT   2
#define SEARCH_IGNORENETERRS 4

#define FIL_STANDARD           1
#define FIL_QUERYEASIZE        2
#define FIL_QUERYEASFROMLIST   3
#define FIL_QUERYFULLNAME      5
#define FIL_STANDARDL         11
#define FIL_QUERYEASIZEL      12
#define FIL_QUERYEASFROMLISTL 13

#define FIO_LOCK      0
#define FIO_UNLOCK    1
#define FIO_SEEK      2
#define FIO_READ      3
#define FIO_WRITE     4

#define FIO_NOSHARE   0
#define FIO_SHAREREAD 1

#define FSIL_ALLOC    1
#define FSIL_VOLSER   2

#define FHT_DISKFILE  0
#define FHT_CHRDEV    1
#define FHT_PIPE      2

#define FHB_DSKREMOTE    0x8000
#define FHB_CHRDEVREMOTE 0x8000
#define FHB_PIPEREMOTE   0x8000

#define FS_ATTACH      0
#define FS_DETACH      1
#define FS_SPOOLATTACH 2
#define FS_SPOOLDETACH 3

#define FSCTL_HANDLE   1
#define FSCTL_PATHNAME 2
#define FSCTL_FSDNAME  3

#define FSAIL_QUERYNAME 1
#define FSAIL_DEVNUMBER 2
#define FSAIL_DRVNUMBER 3

#define FSAT_CHARDEV    1
#define FSAT_PSEUDODEV  2
#define FSAT_LOCALDRV   3
#define FSAT_REMOTEDRV  4

#define HANDTYPE_FILE       0x0000
#define HANDTYPE_DEVICE     0x0001
#define HANDTYPE_PIPE       0x0002
#define HANDTYPE_PROTECTED  0x4000
#define HANDTYPE_NETWORK    0x8000

#define EAT_APPTYP_PMAPI    0x00
#define EAT_APPTYP_DOS      0x01
#define EAT_APPTYP_PMW      0x02
#define EAT_APPTYP_NOPMW    0x03
#define EAT_APPTYP_EXETYPE  0x03
#define EAT_APPTYP_RESERVED ~(EAT_APPTYP_EXETYPE)

#define EAT_BINARY   0xFFFE
#define EAT_ASCII    0xFFFD
#define EAT_BITMAP   0xFFFB
#define EAT_METAFILE 0xFFFA
#define EAT_ICON     0xFFF9
#define EAT_EA       0xFFEE
#define EAT_MVMT     0xFFDF
#define EAT_MVST     0xFFDE
#define EAT_ASN1     0xFFDD

#define DSPI_WRTTHRU 0x10

typedef LHANDLE HDIR, *PHDIR;
typedef ULONG FHLOCK, *PFHLOCK;

typedef struct _FILELOCK {
    LONG lOffset;
    LONG lRange;
} FILELOCK, *PFILELOCK;

typedef struct _FDATE {
    USHORT day:5;
    USHORT month:4;
    USHORT year:7;
} FDATE, *PFDATE;

typedef struct _FTIME {
    USHORT twosecs:5;
    USHORT minutes:6;
    USHORT hours:5;
} FTIME, *PFTIME;

typedef struct _FILEFINDBUF {
    FDATE  fdateCreation;
    FTIME  ftimeCreation;
    FDATE  fdateLastAccess;
    FTIME  ftimeLastAccess;
    FDATE  fdateLastWrite;
    FTIME  ftimeLastWrite;
    ULONG  cbFile;
    ULONG  cbFileAlloc;
    USHORT attrFile;
    UCHAR  cchName;
    CHAR   achName[CCHMAXPATHCOMP];
} FILEFINDBUF, *PFILEFINDBUF;

typedef struct _FILEFINDBUF3 {
    ULONG oNextEntryOffset;
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    ULONG attrFile;
    UCHAR cchName;
    CHAR  achName[CCHMAXPATHCOMP];
} FILEFINDBUF3, *PFILEFINDBUF3;

typedef struct _FILEFINDBUF4 {
    ULONG oNextEntryOffset;
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    ULONG attrFile;
    ULONG cbList;
    UCHAR cchName;
    CHAR  achName[CCHMAXPATHCOMP];
} FILEFINDBUF4, *PFILEFINDBUF4;

typedef struct _FILESTATUS {
    FDATE  fdateCreation;
    FTIME  ftimeCreation;
    FDATE  fdateLastAccess;
    FTIME  ftimeLastAccess;
    FDATE  fdateLastWrite;
    FTIME  ftimeLastWrite;
    ULONG  cbFile;
    ULONG  cbFileAlloc;
    USHORT attrFile;
} FILESTATUS, *PFILESTATUS;

typedef struct _FILESTATUS3 {
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    ULONG attrFile;
} FILESTATUS3, *PFILESTATUS3;

typedef struct _FILESTATUS4 {
    FDATE fdateCreation;
    FTIME ftimeCreation;
    FDATE fdateLastAccess;
    FTIME ftimeLastAccess;
    FDATE fdateLastWrite;
    FTIME ftimeLastWrite;
    ULONG cbFile;
    ULONG cbFileAlloc;
    ULONG attrFile;
    ULONG cbList;
} FILESTATUS4, *PFILESTATUS4;

typedef struct _FSALLOCATE {
    ULONG  idFileSystem;
    ULONG  cSectorUnit;
    ULONG  cUnit;
    ULONG  cUnitAvail;
    USHORT cbSector;
} FSALLOCATE, *PFSALLOCATE;

typedef struct _GEA2 {
    ULONG oNextEntryOffset;
    BYTE  cbName;
    CHAR  szName[1];
} GEA2, *PGEA2;

typedef struct _GEA2LIST {
    ULONG cbList;
    GEA2  list[1];
} GEA2LIST, *PGEA2LIST;

typedef struct _FEA2 {
    ULONG  oNextEntryOffset;
    BYTE   fEA;
    BYTE   cbName;
    USHORT cbValue;
    CHAR   szName[1];
} FEA2, *PFEA2;

typedef struct _FEA2LIST {
    ULONG cbList;
    FEA2  list[1];
} FEA2LIST, *PFEA2LIST;

typedef struct _EAOP2 {
    PGEA2LIST fpGEA2List;
    PFEA2LIST fpFEA2List;
    ULONG     oError;
} EAOP2, *PEAOP2;

typedef struct _FSQBUFFER2 {
    USHORT iType;
    USHORT cbName;
    USHORT cbFSDName;
    USHORT cbFSAData;
    UCHAR  szName[1];
    UCHAR  szFSDName[1];
    UCHAR  rgFSAData[1];
} FSQBUFFER2, *PFSQBUFFER2;

typedef struct _VOLUMELABEL {
    BYTE cch;
    CHAR szVolLabel[12];
} VOLUMELABEL, *PVOLUMELABEL;

typedef struct _FSINFO {
    FDATE fdateCreation;
    FTIME ftimeCreation;
    VOLUMELABEL vol;
} FSINFO, *PFSINFO;


APIRET APIENTRY DosCancelLockRequest(HFILE hFile, PFILELOCK pflLock);
APIRET APIENTRY DosClose(HFILE hFile);
APIRET APIENTRY DosCopy(PCSZ pszOld, PCSZ pszNew, ULONG option);
APIRET APIENTRY DosCreateDir(PCSZ pszDirName, PEAOP2 peaop2);
APIRET APIENTRY DosDelete(PCSZ pszFile);
APIRET APIENTRY DosDeleteDir(PCSZ pszDir);
APIRET APIENTRY DosDupHandle(HFILE hFile, PHFILE pHfile);
APIRET APIENTRY DosEditName(ULONG metalevel, PCSZ pszSource, PCSZ pszEdit,
                   PBYTE pszTarget, ULONG cbTarget);
APIRET APIENTRY DosEnumAttribute(ULONG ulRefType, PVOID pvFile, ULONG ulEntry,
                   PVOID pvBuf, ULONG cbBuf, PULONG pulCount, ULONG ulInfoLevel);
APIRET APIENTRY DosFindClose(HDIR hDir);
APIRET APIENTRY DosFindFirst(PCSZ pszFileSpec, PHDIR phdir, ULONG flAttribute,
                   PVOID pfindbuf, ULONG cbBuf, PULONG pcFileNames, ULONG ulInfoLevel);
APIRET APIENTRY DosFindNext(HDIR hDir, PVOID pfindbuf, ULONG cbfindbuf, PULONG pcFilenames);
APIRET APIENTRY DosForceDelete(PCSZ pszFile);
APIRET APIENTRY DosFSAttach(PCSZ pszDevice, PCSZ pszFilesystem, PVOID pData,
                   ULONG cbData, ULONG flag);
APIRET APIENTRY DosFSCtl(PVOID pData, ULONG cbData, PULONG pcbData, PVOID pParms,
                   ULONG cbParms, PULONG pcbParms, ULONG function, PCSZ pszRoute,
                   HFILE hFile, ULONG method);
APIRET APIENTRY DosMove(PCSZ pszOld, PCSZ pszNew);
APIRET APIENTRY DosOpen(PCSZ pszFileName, PHFILE pHf, PULONG pulAction, ULONG cbFile,
                   ULONG ulAttribute, ULONG fsOpenFlags, ULONG fsOpenMode, PEAOP2 peaop2);
APIRET APIENTRY DosProtectClose(HFILE hFile, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectEnumAttribute(ULONG ulRefType, PVOID pvFile, ULONG ulEntry, PVOID pvBuf,
                   ULONG cbBuf, PULONG pulCount, ULONG ulInfoLevel, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectOpen(PCSZ pszFileName, PHFILE phf, PULONG pulAction, ULONG cbFile,
                   ULONG ulAttribute, ULONG fsOpenFlags, ULONG fsOpenMode,
                   PEAOP2 peaop2, PFHLOCK pfhFileHandleLockID);
APIRET APIENTRY DosProtectQueryFHState(HFILE hFile, PULONG pMode, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectQueryFileInfo(HFILE hf, ULONG ulInfoLevel,
                   PVOID pInfo, ULONG cbInfoBuf, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectRead(HFILE hFile, PVOID pBuffer, ULONG cbRead,
                   PULONG pcbActual, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectSetFHState(HFILE hFile, ULONG mode, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectSetFileInfo(HFILE hf, ULONG ulInfoLevel, PVOID pInfoBuf,
                   ULONG cbInfoBuf, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectSetFileLocks(HFILE hFile, PFILELOCK pflUnlock, PFILELOCK pflLock,
                   ULONG timeout, ULONG flags, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectSetFilePtr(HFILE hFile, LONG ib, ULONG method,
                   PULONG ibActual, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectSetFileSize(HFILE hFile, ULONG cbSize, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosProtectWrite(HFILE hFile, ULONG pBuffer, ULONG cbWrite,
                   PULONG pcbActual, FHLOCK fhFileHandleLockID);
APIRET APIENTRY DosQueryCurrentDir(ULONG disknum, PBYTE pBuf, PULONG pcbBuf);
APIRET APIENTRY DosQueryCurrentDisk(PULONG pdisknum, PULONG plogical);
APIRET APIENTRY DosQueryFHState(HFILE hFile, PULONG pMode);
APIRET APIENTRY DosQueryFileInfo(HFILE hf, ULONG ulInfoLevel, PVOID pInfo, ULONG cbInfoBuf);
APIRET APIENTRY DosQueryFSAttach(PCSZ pszDeviceName, ULONG ulOrdinal, ULONG ulFSAInfoLevel,
                   PFSQBUFFER2 pfsqb, PULONG pcbBuffLength);
APIRET APIENTRY DosQueryFSInfo(ULONG disknum, ULONG  infolevel, PVOID pBuf, ULONG cbBuf);
APIRET APIENTRY DosQueryHType(HFILE hFile, PULONG pType, PULONG pAttr);
APIRET APIENTRY DosQueryPathInfo(PCSZ pszPathName, ULONG ulInfoLevel,
                   PVOID pInfoBuf, ULONG cbInfoBuf);
APIRET APIENTRY DosQueryVerify(BOOL32 *pBool);
APIRET APIENTRY DosRead(HFILE hFile, PVOID pBuffer, ULONG cbRead, PULONG pcbActual);
APIRET APIENTRY DosResetBuffer(HFILE hFile);
APIRET APIENTRY DosSetCurrentDir(PCSZ pszDir);
APIRET APIENTRY DosSetDefaultDisk(ULONG disknum);
APIRET APIENTRY DosSetFHState(HFILE hFile, ULONG mode);
APIRET APIENTRY DosSetFileInfo(HFILE hf, ULONG ulInfoLevel, PVOID pInfoBuf, ULONG cbInfoBuf);
APIRET APIENTRY DosSetFileLocks(HFILE hFile, PFILELOCK pflUnlock, PFILELOCK pflLock,
                   ULONG timeout, ULONG flags);
APIRET APIENTRY DosSetFilePtr(HFILE hFile, LONG ib, ULONG method, PULONG ibActual);
APIRET APIENTRY DosSetFileSize(HFILE hFile, ULONG cbSize);
APIRET APIENTRY DosSetFSInfo(ULONG disknum, ULONG infolevel, ULONG pBuf, PVOID cbBuf);
APIRET APIENTRY DosSetMaxFH(ULONG cFH);
APIRET APIENTRY DosSetPathInfo(PCSZ pszPathName, ULONG ulInfoLevel, PVOID pInfoBuf,
                   ULONG cbInfoBuf, ULONG flOptions);
APIRET APIENTRY DosSetRelMaxFH(PLONG pcbReqCount, PULONG pcbCurMaxFH);
APIRET APIENTRY DosSetVerify(BOOL32 fVerifySetting);
APIRET APIENTRY DosShutdown(ULONG ulShutDownValue);
APIRET APIENTRY DosWrite(HFILE hFile, PVOID pBuffer, ULONG cbWrite, PULONG pcbActual);

#endif

#if defined(INCL_DOSMEMMGR) || !defined(INCL_NOCOMMON)

#define PAG_READ      0x00000001
#define PAG_WRITE     0x00000002
#define PAG_EXECUTE   0x00000004
#define PAG_GUARD     0x00000008
#define PAG_COMMIT    0x00000010
#define PAG_DECOMMIT  0x00000020
#define PAG_DEFAULT   0x00000400
#define PAG_SHARED    0x00002000
#define PAG_FREE      0x00004000
#define PAG_BASE      0x00010000

#define OBJ_TILE      0x00000040
#define OBJ_PROTECTED 0x00000080
#define OBJ_GETTABLE  0x00000100
#define OBJ_GIVEABLE  0x00000200
#define OBJ_ANY       0x00000400

#define DOSSUB_INIT       1
#define DOSSUB_GROW       2
#define DOSSUB_SPARSE_OBJ 4
#define DOSSUB_SERIALIZE  8

#define PAG_NPOUT       0x00000000
#define PAG_PRESENT     0x00000001
#define PAG_NPIN        0x00000002
#define PAG_PRESMASK    0x00000003
#define PAG_INVALID     0x00000000
#define PAG_RESIDENT    0x00000010
#define PAG_SWAPPABLE   0x00000020
#define PAG_DISCARDABLE 0x00000030
#define PAG_TYPEMASK    0x00000030

#define fPERM         (PAG_EXECUTE | PAG_READ | PAG_WRITE)
#define fSHARE        (OBJ_GETTABLE | OBJ_GIVEABLE)
#define fALLOC        (OBJ_TILE | PAG_COMMIT | fPERM)
#define fALLOCSHR     (OBJ_TILE | PAG_COMMIT | fSHARE | fPERM)

APIRET APIENTRY DosAllocMem(PPVOID ppb, ULONG cb, ULONG flag);
APIRET APIENTRY DosAllocSharedMem(PPVOID ppb, PCSZ pszName, ULONG cb, ULONG flag);
APIRET APIENTRY DosFreeMem(PVOID pb);
APIRET APIENTRY DosGetNamedSharedMem(PPVOID ppb, PCSZ pszName, ULONG flag);
APIRET APIENTRY DosGetSharedMem(PVOID pb, ULONG flag);
APIRET APIENTRY DosGiveSharedMem(PVOID pb, PID pid, ULONG flag);
APIRET APIENTRY DosQueryMem(PVOID pb, PULONG pcb, PULONG pFlag);
APIRET APIENTRY DosSetMem(PVOID pb, ULONG cb, ULONG flag);
APIRET APIENTRY DosSubAllocMem(PVOID pbBase, PPVOID ppb, ULONG cb);
APIRET APIENTRY DosSubFreeMem(PVOID pbBase, PVOID pb, ULONG cb);
APIRET APIENTRY DosSubSetMem(PVOID pbBase, ULONG flag, ULONG cb);
APIRET APIENTRY DosSubUnsetMem(PVOID pbBase);

#endif

#if defined(INCL_DOSMODULEMGR)

#define PT_16BIT 0
#define PT_32BIT 1

#define HW_CFG_MCA             0x00000001
#define HW_CFG_EISA            0x00000002
#define HW_CFG_ABIOS_SUPPORTED 0x00000004
#define HW_CFG_ABIOS_PRESENT   0x00000008
#define HW_CFG_PCI             0x00000010
#define HW_CFG_IBM_ABIOS       0x00000000
#define HW_CFG_OEM_ABIOS       0x00000020
#define HW_CFG_PENTIUM_CPU     0x00000040

APIRET APIENTRY DosFreeModule(HMODULE hmod);
APIRET APIENTRY DosLoadModule(PSZ pszName, ULONG cbName, PCSZ pszModname, PHMODULE phmod);
APIRET APIENTRY DosQueryABIOSSupport(ULONG reserved);
APIRET APIENTRY DosQueryModFromEIP(HMODULE *phMod, ULONG *pObjNum, ULONG BuffLen,
                   PCHAR pBuff, ULONG *pOffset, ULONG Address);
APIRET APIENTRY DosQueryModuleHandle(PCSZ pszModname, PHMODULE phmod);
APIRET APIENTRY DosQueryModuleName(HMODULE hmod, ULONG cbName, PCHAR pch);
APIRET APIENTRY DosQueryProcAddr(HMODULE hmod, ULONG ordinal, PCSZ pszName, PFN *ppfn);
APIRET APIENTRY DosQueryProcType(HMODULE hmod, ULONG ordinal, PCSZ pszName, PULONG pulproctype);
APIRET APIENTRY DosReplaceModule(PCSZ pszOldModule, PCSZ pszNewModule, PCSZ pszBackupModule);

#endif

#if defined(INCL_DOSNMPIPES)

#define NP_STATE_DISCONNECTED 1
#define NP_STATE_LISTENING    2
#define NP_STATE_CONNECTED    3
#define NP_STATE_CLOSING      4

#define NP_ACCESS_INBOUND      0x0000
#define NP_ACCESS_OUTBOUND     0x0001
#define NP_ACCESS_DUPLEX       0x0002
#define NP_INHERIT             0x0000
#define NP_NOINHERIT           0x0080
#define NP_WRITEBEHIND         0x0000
#define NP_NOWRITEBEHIND       0x4000

#define NP_READMODE_BYTE       0x0000
#define NP_READMODE_MESSAGE    0x0100
#define NP_TYPE_BYTE           0x0000
#define NP_TYPE_MESSAGE        0x0400
#define NP_END_CLIENT          0x0000
#define NP_END_SERVER          0x4000
#define NP_WAIT                0x0000
#define NP_NOWAIT              0x8000
#define NP_UNLIMITED_INSTANCES 0x00FF

#define NP_DEFAULT_WAIT    0
#define NP_INDEFINITE_WAIT (-1)

typedef LHANDLE HPIPE, *PHPIPE;

typedef struct _AVAILDATA {
    USHORT cbpipe;
    USHORT cbmessage;
} AVAILDATA, *PAVAILDATA;

typedef struct _PIPEINFO {
   USHORT cbOut;
   USHORT cbIn;
   BYTE   cbMaxInst;
   BYTE   cbCurInst;
   BYTE   cbName;
   CHAR   szName[1];
} PIPEINFO, *PPIPEINFO;

typedef struct _PIPESEMSTATE {
   BYTE   fStatus;
   BYTE   fFlag;
   USHORT usKey;
   USHORT usAvail;
} PIPESEMSTATE, *PPIPESEMSTATE;

APIRET APIENTRY DosCallNPipe(PCSZ pszName, PVOID pInbuf, ULONG cbIn, PVOID pOutbuf,
                   ULONG cbOut, PULONG pcbActual, ULONG msec);
APIRET APIENTRY DosConnectNPipe(HPIPE hpipe);
APIRET APIENTRY DosCreateNPipe(PCSZ pszName, PHPIPE pHpipe, ULONG openmode,
                   ULONG pipemode, ULONG cbOutbuf, ULONG cbInbuf, ULONG msec);
APIRET APIENTRY DosDisConnectNPipe(HPIPE hpipe);
APIRET APIENTRY DosPeekNPipe(HPIPE hpipe, PVOID pBuf, ULONG cbBuf, PULONG pcbActual,
                   PAVAILDATA pAvail, PULONG pState);
APIRET APIENTRY DosQueryNPHState(HPIPE hpipe, PULONG pState);
APIRET APIENTRY DosQueryNPipeInfo(HPIPE hpipe, ULONG infolevel, PVOID pBuf, ULONG cbBuf);
APIRET APIENTRY DosQueryNPipeSemState(HSEM hsem, PPIPESEMSTATE pnpss, ULONG cbBuf);
APIRET APIENTRY DosSetNPHState(HPIPE hpipe, ULONG state);
APIRET APIENTRY DosSetNPipeSem(HPIPE hpipe, HSEM hsem, ULONG key);
APIRET APIENTRY DosTransactNPipe(HPIPE hpipe, PVOID pOutbuf, ULONG cbOut,
                   PVOID pInbuf, ULONG cbIn, PULONG pcbRead);
APIRET APIENTRY DosWaitNPipe(PCSZ pszName, ULONG msec);

#endif

#if defined(INCL_DOSQUEUES)

#define QUE_FIFO              0
#define QUE_LIFO              1
#define QUE_PRIORITY          2
#define QUE_NOCONVERT_ADDRESS 0
#define QUE_CONVERT_ADDRESS   4

typedef LHANDLE HQUEUE, *PHQUEUE;

typedef struct _REQUESTDATA {
    PID   pid;
    ULONG ulData;
} REQUESTDATA, *PREQUESTDATA;

APIRET APIENTRY DosCloseQueue(HQUEUE hq);
APIRET APIENTRY DosCreatePipe(PHFILE phfRead, PHFILE phfWrite, ULONG cb);
APIRET APIENTRY DosCreateQueue(PHQUEUE phq, ULONG priority, PCSZ pszName);
APIRET APIENTRY DosOpenQueue(PPID ppid, PHQUEUE phq, PCSZ pszName);
APIRET APIENTRY DosPeekQueue(HQUEUE hq, PREQUESTDATA pRequest, PULONG pcbData,
                   PPVOID ppbuf, PULONG element, BOOL32 nowait, PBYTE ppriority, HEV hsem);
APIRET APIENTRY DosPurgeQueue(HQUEUE hq);
APIRET APIENTRY DosQueryQueue(HQUEUE hq, PULONG pcbEntries);
APIRET APIENTRY DosReadQueue(HQUEUE hq, PREQUESTDATA pRequest, PULONG pcbData,
                   PPVOID ppbuf, ULONG element, BOOL32 wait, PBYTE ppriority, HEV hsem);
APIRET APIENTRY DosWriteQueue(HQUEUE hq, ULONG request, ULONG cbData, PVOID
                   pbData, ULONG priority);

#endif

#if defined(INCL_DOSSEMAPHORES)

#define DC_SEM_SHARED   1
#define DCMW_WAIT_ANY   2
#define DCMW_WAIT_ALL   4

#define DCE_POSTONE     0x0800
#define DCE_AUTORESET   0x1000

#define SEM_IMMEDIATE_RETURN 0
#define SEM_INDEFINITE_WAIT  (-1)

typedef struct _SEMRECORD {
    HSEM  hsemCur;
    ULONG ulUser;
} SEMRECORD, *PSEMRECORD;

APIRET APIENTRY DosAddMuxWaitSem(HMUX hmux, PSEMRECORD pSemRec);
APIRET APIENTRY DosCloseEventSem(HEV hev);
APIRET APIENTRY DosCloseMutexSem(HMTX hmtx);
APIRET APIENTRY DosCloseMuxWaitSem(HMUX hmux);
APIRET APIENTRY DosCreateEventSem(PCSZ pszName, PHEV phev, ULONG flAttr, BOOL32 fState);
APIRET APIENTRY DosCreateMutexSem(PCSZ pszName, PHMTX phmtx, ULONG flAttr, BOOL32 fState);
APIRET APIENTRY DosCreateMuxWaitSem(PCSZ pszName, PHMUX phmux, ULONG cSemRec,
                   PSEMRECORD pSemRec, ULONG flAttr);
APIRET APIENTRY DosDeleteMuxWaitSem(HMUX hmux, HSEM hSem);
APIRET APIENTRY DosOpenEventSem(PCSZ pszName, PHEV phev);
APIRET APIENTRY DosOpenMutexSem(PCSZ pszName, PHMTX phmtx);
APIRET APIENTRY DosOpenMuxWaitSem(PCSZ pszName, PHMUX phmux);
APIRET APIENTRY DosPostEventSem(HEV hev);
APIRET APIENTRY DosQueryEventSem(HEV hev, PULONG pulPostCt);
APIRET APIENTRY DosQueryMutexSem(HMTX hmtx, PPID ppid, PTID ptid, PULONG pulCount);
APIRET APIENTRY DosQueryMuxWaitSem(HMUX hmux, PULONG pcSemRec, PSEMRECORD pSemRec, PULONG pflAttr);
APIRET APIENTRY DosReleaseMutexSem(HMTX hmtx);
APIRET APIENTRY DosRequestMutexSem(HMTX hmtx, ULONG ulTimeout);
APIRET APIENTRY DosResetEventSem(HEV hev, PULONG pulPostCt);
APIRET APIENTRY DosWaitEventSem(HEV hev, ULONG ulTimeout);
APIRET APIENTRY DosWaitMuxWaitSem(HMUX hmux, ULONG ulTimeout, PULONG pulUser);

#endif

#if defined(INCL_DOSMONITORS)

#define DosMonClose DOS16MONCLOSE
#define DosMonOpen  DOS16MONOPEN
#define DosMonRead  DOS16MONREAD
#define DosMonReg   DOS16MONREG
#define DosMonWrite DOS16MONWRITE

typedef SHANDLE  HMONITOR;
typedef HMONITOR *PHMONITOR;

USHORT APIENTRY16 DosMonClose(HMONITOR Handle);
USHORT APIENTRY16 DosMonOpen(PSZ Devname, PHMONITOR Handle);
USHORT APIENTRY16 DosMonRead(PBYTE BufferI, UCHAR WaitFlag, PBYTE DataBuffer, PUSHORT Bytecnt);
USHORT APIENTRY16 DosMonReg(HMONITOR Handle, PBYTE BufferI, PBYTE BufferO,
                     USHORT Posflag, USHORT Index);
USHORT APIENTRY16 DosMonWrite(PBYTE BufferO, PBYTE DataBuffer, USHORT Bytecnt);

#endif

#if defined(INCL_DOSNLS)

#define DosCaseMap     DosMapCase
#define DosGetCollate  DosQueryCollate
#define DosGetCp       DosQueryCp
#define DosGetCtryInfo DosQueryCtryInfo
#define DosGetDBCSEv   DosQueryDBCSEnv
#define DosSetProcCp   DosSetProcessCp

typedef struct _COUNTRYCODE {
    ULONG  country;
    ULONG  codepage;
} COUNTRYCODE, *PCOUNTRYCODE;

typedef struct _COUNTRYINFO {
    ULONG  country;
    ULONG  codepage;
    ULONG  fsDateFmt;
    CHAR   szCurrency[5];
    CHAR   szThousandsSeparator[2];
    CHAR   szDecimal[2];
    CHAR   szDateSeparator[2];
    CHAR   szTimeSeparator[2];
    UCHAR  fsCurrencyFmt;
    UCHAR  cDecimalPlace;
    UCHAR  fsTimeFmt;
    USHORT abReserved1[2];
    CHAR   szDataSeparator[2];
    USHORT abReserved2[5];
} COUNTRYINFO, *PCOUNTRYINFO;

APIRET APIENTRY DosMapCase(ULONG cb, PCOUNTRYCODE pcc, PCHAR pch);
APIRET APIENTRY DosQueryCollate(ULONG cb, PCOUNTRYCODE pcc, PCHAR pch, PULONG pcch);
APIRET APIENTRY DosQueryCp(ULONG cb, PULONG arCP, PULONG pcCP);
APIRET APIENTRY DosQueryCtryInfo(ULONG cb, PCOUNTRYCODE pcc, PCOUNTRYINFO pci, PULONG pcbActual);
APIRET APIENTRY DosQueryDBCSEnv(ULONG cb, PCOUNTRYCODE pcc, PCHAR pBuf);
APIRET APIENTRY DosSetProcessCp(ULONG cp);

#endif

#if defined(INCL_DOSEXCEPTIONS)

#define SIG_UNSETFOCUS 0
#define SIG_SETFOCUS   1

#include <bsexcpt.h>

APIRET APIENTRY DosAcknowledgeSignalException(ULONG ulSignalNum);
APIRET APIENTRY DosEnterMustComplete(PULONG pulNesting);
APIRET APIENTRY DosExitMustComplete(PULONG pulNesting);
APIRET APIENTRY DosQueryThreadContext(TID tid, ULONG level, PCONTEXTRECORD pcxt);
APIRET APIENTRY DosRaiseException(PEXCEPTIONREPORTRECORD pexcept);
APIRET APIENTRY DosSendSignalException(PID pid, ULONG exception);
APIRET APIENTRY DosSetExceptionHandler(PEXCEPTIONREGISTRATIONRECORD pERegRec);
APIRET APIENTRY DosSetSignalExceptionFocus(BOOL32 flag, PULONG pulTimes);
APIRET APIENTRY DosUnsetExceptionHandler(PEXCEPTIONREGISTRATIONRECORD pERegRec);
APIRET APIENTRY DosUnwindException(PEXCEPTIONREGISTRATIONRECORD phandler,
                   PVOID pTargetIP, PEXCEPTIONREPORTRECORD pERepRec);

#endif

#if defined(INCL_DOSRESOURCES)

#define RT_POINTER      1
#define RT_BITMAP       2
#define RT_MENU         3
#define RT_DIALOG       4
#define RT_STRING       5
#define RT_FONTDIR      6
#define RT_FONT         7
#define RT_ACCELTABLE   8
#define RT_RCDATA       9
#define RT_MESSAGE      10
#define RT_DLGINCLUDE   11
#define RT_VKEYTBL      12
#define RT_KEYTBL       13
#define RT_CHARTBL      14
#define RT_DISPLAYINFO  15
#define RT_FKASHORT     16
#define RT_FKALONG      17
#define RT_HELPTABLE    18
#define RT_HELPSUBTABLE 19
#define RT_FDDIR        20
#define RT_FD           21
#define RT_MAX          22
#define RT_RESNAMES     255

#define RF_ORDINALID 0x80000000

APIRET APIENTRY DosFreeResource(PVOID pb);
APIRET APIENTRY DosGetResource(HMODULE hmod, ULONG idType, ULONG idName, PPVOID ppb);
APIRET APIENTRY DosQueryResourceSize(HMODULE hmod, ULONG idt, ULONG idn, PULONG pulsize);

#endif

#if defined(INCL_DOSDATETIME)

typedef LHANDLE HTIMER;
typedef HTIMER  *PHTIMER;

typedef struct _DATETIME {
    UCHAR   hours;
    UCHAR   minutes;
    UCHAR   seconds;
    UCHAR   hundredths;
    UCHAR   day;
    UCHAR   month;
    USHORT  year;
    SHORT   timezone;
    UCHAR   weekday;
} DATETIME, *PDATETIME;

APIRET APIENTRY DosAsyncTimer(ULONG msec, HSEM hsem, PHTIMER phtimer);
APIRET APIENTRY DosGetDateTime(PDATETIME pdt);
APIRET APIENTRY DosSetDateTime(PDATETIME pdt);
APIRET APIENTRY DosStartTimer(ULONG msec, HSEM hsem, PHTIMER phtimer);
APIRET APIENTRY DosStopTimer(HTIMER htimer);

#endif

#if defined(INCL_DOSMISC)

#define DDP_DISABLEPROCDUMP 0
#define DDP_ENABLEPROCDUMP  1
#define DDP_PERFORMPROCDUMP 2

#define FERR_DISABLEHARDERR   0
#define FERR_ENABLEHARDERR    1
#define FERR_ENABLEEXCEPTION  0
#define FERR_DISABLEEXCEPTION 2

#define QSV_MAX_PATH_LENGTH        1
#define QSV_MAX_TEXT_SESSIONS      2
#define QSV_MAX_PM_SESSIONS        3
#define QSV_MAX_VDM_SESSIONS       4
#define QSV_BOOT_DRIVE             5
#define QSV_DYN_PRI_VARIATION      6
#define QSV_MAX_WAIT               7
#define QSV_MIN_SLICE              8
#define QSV_MAX_SLICE              9
#define QSV_PAGE_SIZE             10
#define QSV_VERSION_MAJOR         11
#define QSV_VERSION_MINOR         12
#define QSV_VERSION_REVISION      13
#define QSV_MS_COUNT              14
#define QSV_TIME_LOW              15
#define QSV_TIME_HIGH             16
#define QSV_TOTPHYSMEM            17
#define QSV_TOTRESMEM             18
#define QSV_TOTAVAILMEM           19
#define QSV_MAXPRMEM              20
#define QSV_MAXSHMEM              21
#define QSV_TIMER_INTERVAL        22
#define QSV_MAX_COMP_LENGTH       23
#define QSV_FOREGROUND_FS_SESSION 24
#define QSV_FOREGROUND_PROCESS    25
#define QSV_NUMPROCESSORS         26
#define QSV_MAXHPRMEM             27
#define QSV_MAXHSHMEM             28
#define QSV_MAXPROCESSES          29
#define QSV_VIRTUALADDRESSLIMIT   30
#define QSV_INT10ENABLED          31
#define QSV_MAX                   QSV_INT10ENABLED

#define DSP_IMPLIEDCUR   1
#define DSP_PATHREF      2
#define DSP_IGNORENETERR 4

#define SPU_DISABLESUPPRESSION 0
#define SPU_ENABLESUPPRESSION  1
#define SPU_NOLOGCHANGE        2

#define BEGIN_LIBPATH 1
#define END_LIBPATH   2
#define LIBPATHSTRICT 3

APIRET APIENTRY DosErrClass(ULONG code, PULONG pClass, PULONG pAction, PULONG pLocus);
APIRET APIENTRY DosError(ULONG error);
APIRET APIENTRY DosGetMessage(PCHAR *pTable, ULONG cTable, PCHAR pBuf,
                   ULONG cbBuf, ULONG msgnumber, PCSZ pszFile, PULONG pcbMsg);
APIRET APIENTRY DosInsertMessage(PCHAR *pTable, ULONG cTable, PCSZ pszMsg,
                   ULONG cbMsg, PCHAR pBuf, ULONG cbBuf, PULONG pcbMsg);
APIRET APIENTRY DosPutMessage(HFILE hfile, ULONG cbMsg, PCHAR pBuf);
APIRET APIENTRY DosQueryExtLIBPATH(PSZ pszExtLIBPATH, ULONG flags);
APIRET APIENTRY DosQueryMessageCP(PCHAR pb, ULONG cb, PCSZ pszFilename, PULONG cbBuf);
APIRET APIENTRY DosQuerySysInfo(ULONG iStart, ULONG iLast, PVOID pBuf, ULONG cbBuf);
APIRET APIENTRY DosScanEnv(PCSZ pszName, PSZ *ppszValue);
APIRET APIENTRY DosSearchPath(ULONG flag, PCSZ pszPathOrName, PCSZ pszFilename,
                   PBYTE pBuf, ULONG cbBuf);
APIRET APIENTRY DosSetExtLIBPATH(PCSZ pszExtLIBPATH, ULONG flags);
APIRET APIENTRY DosSuppressPopUps(ULONG flag, CHAR drive);

#endif

#if defined(INCL_DOSSESMGR) || defined(INCL_DOSFILEMGR)

#define FAPPTYP_NOTSPEC         0x0000
#define FAPPTYP_NOTWINDOWCOMPAT 0x0001
#define FAPPTYP_WINDOWCOMPAT    0x0002
#define FAPPTYP_WINDOWAPI       0x0003
#define FAPPTYP_BOUND           0x0008
#define FAPPTYP_DLL             0x0010
#define FAPPTYP_DOS             0x0020
#define FAPPTYP_PHYSDRV         0x0040
#define FAPPTYP_VIRTDRV         0x0080
#define FAPPTYP_PROTDLL         0x0100
#define FAPPTYP_WINDOWSREAL     0x0200
#define FAPPTYP_WINDOWSPROT     0x0400
#define FAPPTYP_WINDOWSPROT31   0x1000
#define FAPPTYP_32BIT           0x4000
#define FAPPTYP_EXETYPE         0x0003

#endif

#if defined(INCL_DOSSESMGR)

#define SSF_RELATED_INDEPENDENT 0
#define SSF_RELATED_CHILD       1

#define SSF_FGBG_FORE          0
#define SSF_FGBG_BACK          1

#define SSF_TRACEOPT_NONE      0
#define SSF_TRACEOPT_TRACE     1
#define SSF_TRACEOPT_TRACEALL  2

#define SSF_INHERTOPT_SHELL    0
#define SSF_INHERTOPT_PARENT   1

#define SSF_TYPE_DEFAULT       0
#define SSF_TYPE_FULLSCREEN    1
#define SSF_TYPE_WINDOWABLEVIO 2
#define SSF_TYPE_PM            3
#define SSF_TYPE_VDM           4
#define SSF_TYPE_GROUP         5
#define SSF_TYPE_DLL           6
#define SSF_TYPE_WINDOWEDVDM   7
#define SSF_TYPE_PDD           8
#define SSF_TYPE_VDD           9

#define SSF_CONTROL_VISIBLE     0x0000
#define SSF_CONTROL_INVISIBLE   0x0001
#define SSF_CONTROL_MAXIMIZE    0x0002
#define SSF_CONTROL_MINIMIZE    0x0004
#define SSF_CONTROL_NOAUTOCLOSE 0x0008
#define SSF_CONTROL_SETPOS      0x8000

#define SET_SESSION_UNCHANGED      0
#define SET_SESSION_SELECTABLE     1
#define SET_SESSION_NON_SELECTABLE 2
#define SET_SESSION_BOND           1
#define SET_SESSION_NO_BOND        2

#define STOP_SESSION_SPECIFIED     0
#define STOP_SESSION_ALL           1

typedef struct _STATUSDATA {
    USHORT Length;
    USHORT SelectInd;
    USHORT BondInd;
} STATUSDATA, *PSTATUSDATA;

typedef struct _STARTDATA {
    USHORT Length;
    USHORT Related;
    USHORT FgBg;
    USHORT TraceOpt;
    PSZ    PgmTitle;
    PSZ    PgmName;
    PBYTE  PgmInputs;
    PBYTE  TermQ;
    PBYTE  Environment;
    USHORT InheritOpt;
    USHORT SessionType;
    PSZ    IconFile;
    ULONG  PgmHandle;
    USHORT PgmControl;
    USHORT InitXPos;
    USHORT InitYPos;
    USHORT InitXSize;
    USHORT InitYSize;
    USHORT Reserved;
    PSZ    ObjectBuffer;
    ULONG  ObjectBuffLen;
} STARTDATA, *PSTARTDATA;

APIRET APIENTRY DosQueryAppType(PCSZ pszName, PULONG pFlags);
APIRET APIENTRY DosSelectSession(ULONG idSession);
APIRET APIENTRY DosSetSession(ULONG idSession, PSTATUSDATA psd);
APIRET APIENTRY DosStartSession(PSTARTDATA psd, PULONG pidSession, PPID ppid);
APIRET APIENTRY DosStopSession(ULONG scope, ULONG idSession);

#endif

#if defined(INCL_DOSDEVICES)

#define DEVINFO_PRINTER     0
#define DEVINFO_RS232       1
#define DEVINFO_FLOPPY      2
#define DEVINFO_COPROCESSOR 3
#define DEVINFO_SUBMODEL    4
#define DEVINFO_MODEL       5
#define DEVINFO_ADAPTER     6

#define INFO_COUNT_PARTITIONABLE_DISKS 1
#define INFO_GETIOCTLHANDLE            2
#define INFO_FREEIOCTLHANDLE           3

APIRET APIENTRY DosDevConfig(PVOID pdevinfo, ULONG item);
APIRET APIENTRY DosDevIOCtl(HFILE hDevice, ULONG category, ULONG function,
                   PVOID pParams, ULONG cbParmLenMax, PULONG pcbParmLen,
                   PVOID pData, ULONG cbDataLenMax, PULONG pcbDataLen);
APIRET APIENTRY DosPhysicalDisk(ULONG function, PVOID pBuf, ULONG cbBuf,
                   PVOID pParams, ULONG cbParams);

#endif

#if defined(INCL_DOSMVDM)

typedef LHANDLE HVDD, *PHVDD;

APIRET APIENTRY DosCloseVDD(HVDD hvdd);
APIRET APIENTRY DosOpenVDD(PCSZ pszVDD, PHVDD phvdd);
APIRET APIENTRY DosRequestVDD(HVDD hvdd, SGID sgid, ULONG cmd, ULONG cbInput,
                   PVOID pInput, ULONG cbOutput, PVOID pOutput);

#endif

#if defined(INCL_DOSPROFILE)

#define CMD_KI_RDCNT 0x63

typedef struct _CPUUTIL {
    ULONG ulTimeLow;
    ULONG ulTimeHigh;
    ULONG ulIdleLow;
    ULONG ulIdleHigh;
    ULONG ulBusyLow;
    ULONG ulBusyHigh;
    ULONG ulIntrLow;
    ULONG ulIntrHigh;
} CPUUTIL, *PCPUUTIL;

APIRET APIENTRY DosPerfSysCall(ULONG ulCommand, ULONG ulParm1, ULONG ulParm2, ULONG ulParm3);
APIRET APIENTRY DosTmrQueryFreq(PULONG pulTmrFreq);
APIRET APIENTRY DosTmrQueryTime(PQWORD pqwTmrTime);

#endif

#endif
