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
* Description:  OS/2 Base DOS include file for 16-bit development.
*
****************************************************************************/

#ifdef INCL_DOS
    #define INCL_DOSDATETIME
    #define INCL_DOSDEVICES
    #define INCL_DOSFILEMGR
    #define INCL_DOSINFOSEG
    #define INCL_DOSMEMMGR
    #define INCL_DOSMISC
    #define INCL_DOSMODULEMGR
    #define INCL_DOSMONITORS
    #define INCL_DOSNLS
    #define INCL_DOSNMPIPES
    #define INCL_DOSPROCESS
    #define INCL_DOSQUEUES
    #define INCL_DOSRESOURCES
    #define INCL_DOSSEMAPHORES
    #define INCL_DOSSESMGR
    #define INCL_DOSSIGNALS
#endif

#define CCHMAXPATH          260
#define CCHMAXPATHCOMP      256

#ifdef INCL_DOSPROCESS

typedef struct _RESULTCODES {
    USHORT codeTerminate;
    USHORT codeResult;
} RESULTCODES, FAR *PRESULTCODES;

typedef struct _PIDINFO {
    PID pid;
    TID tid;
    PID pidParent;
} PIDINFO, FAR *PPIDINFO;

typedef VOID (FAR *PFNTHREAD)(VOID);
typedef VOID (PASCAL FAR *PFNEXITLIST)(USHORT);

USHORT APIENTRY DosBeep(USHORT Frequency, USHORT Duration);
USHORT APIENTRY DosCreateThread(PFNTHREAD PgmAddress, PTID ThreadIDWord, PBYTE NewThreadStack);
USHORT APIENTRY DosCwait(USHORT ActionCode, USHORT WaitOption, PRESULTCODES ReturnCodes,
                         PPID ProcessIDWord, PID ProcessID);
USHORT APIENTRY DosEnterCritSec(VOID);
USHORT APIENTRY DosExecPgm(PCHAR ObjNameBuf, SHORT ObjNameBufL, USHORT ExecFlags, PSZ ArgPointer,
                           PSZ EnvPointer, PRESULTCODES ReturnCodes, PSZ PgmPointer);
VOID   APIENTRY DosExit(USHORT ActionCode, USHORT ResultCode);
USHORT APIENTRY DosExitCritSec(VOID);
USHORT APIENTRY DosExitList(USHORT FcnCode_Order, PFNEXITLIST RtnAddress);
USHORT APIENTRY DosGetPID(PPIDINFO ProcessIDsArea);
USHORT APIENTRY DosGetPPID(USHORT PID, PUSHORT PPID);
USHORT APIENTRY DosGetPrty(USHORT Scope, PUSHORT Priority, USHORT ID);
USHORT APIENTRY DosKillProcess(USHORT ActionCode, PID ProcessID);
USHORT APIENTRY DosResumeThread(TID ThreadID);
USHORT APIENTRY DosSetPrty(USHORT Scope, USHORT PriorityClass, SHORT PriorityDelta, USHORT ID);
USHORT APIENTRY DosSleep(ULONG TimeInterval);
USHORT APIENTRY DosSuspendThread(TID ThreadID);

#endif

#ifdef INCL_DOSFILEMGR

typedef SHANDLE     HDIR;
typedef HDIR FAR    *PHDIR;

typedef struct _DENA1 {
    UCHAR  reserved;
    UCHAR  cbName;
    USHORT cbValue;
    UCHAR  szName[1];
} DENA1;

typedef struct _FDATE {
    unsigned day   : 5;
    unsigned month : 4;
    unsigned year  : 7;
} FDATE;

typedef struct _FTIME {
    unsigned twosecs : 5;
    unsigned minutes : 6;
    unsigned hours   : 5;
} FTIME;

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
} FILEFINDBUF, FAR *PFILEFINDBUF;

typedef struct _FILEFINDBUF2 {
    FDATE  fdateCreation;
    FTIME  ftimeCreation;
    FDATE  fdateLastAccess;
    FTIME  ftimeLastAccess;
    FDATE  fdateLastWrite;
    FTIME  ftimeLastWrite;
    ULONG  cbFile;
    ULONG  cbFileAlloc;
    USHORT attrFile;
    ULONG  cbList;
    UCHAR  cchName;
    CHAR   achName[CCHMAXPATHCOMP];
} FILEFINDBUF2, FAR *PFILEFINDBUF2;

typedef struct _GEA {
    BYTE cbName;
    CHAR szName[1];
} GEA, FAR *PGEA;

typedef struct _GEALIST {
    ULONG  cbList;
    GEA list[1];
} GEALIST, FAR *PGEALIST;

typedef struct _FEA {
    BYTE fEA;
    BYTE cbName;
    USHORT cbValue;
} FEA, FAR *PFEA;

typedef struct _FEALIST {
    ULONG  cbList;
    FEA list[1];
} FEALIST, FAR *PFEALIST;

typedef struct _EAOP {
    PGEALIST fpGEAList;
    PFEALIST fpFEAList;
    ULONG    oError;
} EAOP, FAR *PEAOP;

USHORT APIENTRY DosBufReset(HFILE FileHandle);
USHORT APIENTRY DosChDir(PSZ DirName, ULONG Reserved);
USHORT APIENTRY DosChgFilePtr(HFILE FileHandle, LONG Distance, USHORT MoveType, PULONG NewPointer);
USHORT APIENTRY DosClose(HFILE FileHandle);
USHORT APIENTRY DosCopy(PSZ SourceName, PSZ TargetName, USHORT OpMode, ULONG Reserved);
USHORT APIENTRY DosDelete(PSZ FileName, ULONG Reserved);
USHORT APIENTRY DosDupHandle(HFILE OldFileHandle, PHFILE NewFileHandle);
USHORT APIENTRY DosEditName(USHORT EditLevel, PSZ SourceString, PSZ EditString, PBYTE TargetBuf,
                        USHORT TargetBufLen);
USHORT APIENTRY DosEnumAttribute(USHORT RefType, PVOID FileRef, ULONG EntryNum, PVOID EnumBuf,
                        ULONG EnumBufSize, PULONG EnumCnt, ULONG InfoLevel, ULONG Reserved);
USHORT APIENTRY DosFileIO(HFILE FileHandle, PBYTE CommandList, USHORT CommandListLen,
                        PLONG ErrorOffset);
USHORT APIENTRY DosFileLocks(HFILE FileHandle, PLONG UnLockRange, PLONG LockRange);
USHORT APIENTRY DosFindClose(HDIR DirHandle);
USHORT APIENTRY DosFindFirst(PSZ FileName, PHDIR DirHandle, USHORT Attribute, PFILEFINDBUF ResultBuf,
                        USHORT ResultBufLen, PUSHORT SearchCount, ULONG Reserved);
USHORT APIENTRY DosFindFirst2(PSZ FileName, PHDIR DirHandle, USHORT Attribute, PVOID ResultBuf,
                        USHORT ResultBufLen, PUSHORT SearchCount, USHORT FileInfoLevel, ULONG Reserved);
USHORT APIENTRY DosFindNext(HDIR DirHandle, PFILEFINDBUF ResultBuf, USHORT ResultBufLen, PUSHORT SearchCount);
USHORT APIENTRY DosFSAttach(PSZ DeviceName, PSZ FSDName, PBYTE DataBuffer,
                        USHORT DataBufferLen, USHORT OpFlag, ULONG Reserved);
USHORT APIENTRY DosFSCtl(PBYTE DataArea, USHORT DataLengthMax, PUSHORT DataLength, PBYTE ParmList,
                        USHORT ParmLengthMax, PUSHORT ParmLength, USHORT FunctionCode, PSZ RouteName,
                        HFILE FileHandle, USHORT RouteMethod, ULONG Reserved);
USHORT APIENTRY DosMkDir(PSZ DirName, ULONG Reserved);
USHORT APIENTRY DosMkDir2(PSZ DirName, PEAOP EABuf, ULONG Reserved);
USHORT APIENTRY DosMove(PSZ OldPathName, PSZ NewPathName, ULONG Reserved);
USHORT APIENTRY DosNewSize(HFILE FileHandle, ULONG FileSize);
USHORT APIENTRY DosOpen(PSZ FileName, PHFILE FileHandle, PUSHORT ActionTaken, ULONG FileSize,
                        USHORT FileAttribute, USHORT OpenFlag, USHORT OpenMode, ULONG Reserved);
USHORT APIENTRY DosOpen2(PSZ FileName, PHFILE FileHandle, PUSHORT ActionTaken, ULONG FileSize,
                        USHORT FileAttribute, USHORT OpenFlag, USHORT OpenMode, PEAOP EABuf,
                        ULONG Reserved);
USHORT APIENTRY DosQCurDir(USHORT DriveNumber, PBYTE DirPath, PUSHORT DirPathLen);
USHORT APIENTRY DosQCurDisk(PUSHORT DriveNumber, PULONG LogicalDriveMap);
USHORT APIENTRY DosQFHandState(HFILE FileHandle, PUSHORT FileHandleState);
USHORT APIENTRY DosQFileInfo(HFILE FileHandle, USHORT FileInfoLevel, PBYTE FileInfoBuf,
                        USHORT FileInfoBufSize);
USHORT APIENTRY DosQFileMode(PSZ FilePathName, PUSHORT CurrentAttribute, ULONG Reserved);
USHORT APIENTRY DosQFSAttach(PSZ DeviceName, USHORT Ordinal, USHORT FSAInfoLevel, PBYTE DataBuffer,
                        PUSHORT DataBufferLen, ULONG Reserved);
USHORT APIENTRY DosQFSInfo(USHORT DriveNumber, USHORT FSInfoLevel, PBYTE FSInfoBuf, USHORT FSInfoBufSize);
USHORT APIENTRY DosQHandType(HFILE FileHandle, PUSHORT HandType, PUSHORT FlagWord);
USHORT APIENTRY DosQPathInfo(PSZ PathName, USHORT PathInfoLevel, PBYTE PathInfoBuf,
                        USHORT PathInfoBufSize, ULONG Reserved);
USHORT APIENTRY DosQVerify(PUSHORT VerifySetting);
USHORT APIENTRY DosRead(HFILE FileHandle, PVOID BufferArea, USHORT BufferLength, PUSHORT BytesRead);
USHORT APIENTRY DosReadAsync(HFILE FileHandle, PULONG RamSemaphore, PUSHORT ReturnCode, PVOID BufferArea,
                        USHORT BufferLength, PUSHORT BytesRead);
USHORT APIENTRY DosRmDir(PSZ DirName, ULONG Reserved);
USHORT APIENTRY DosSelectDisk(USHORT DriveNumber);
USHORT APIENTRY DosSetFHandState(HFILE FileHandle, USHORT FileHandleState);
USHORT APIENTRY DosSetFileInfo(HFILE FileHandle, USHORT FileInfoLevel, PBYTE FileInfoBuf,
                        USHORT FileInfoBufSize);
USHORT APIENTRY DosSetFileMode(PSZ FileName, USHORT NewAttribute, ULONG Reserved);
USHORT APIENTRY DosSetFSInfo(USHORT DriveNumber, USHORT FSInfoLevel, PBYTE FSInfoBuf,
                        USHORT FSInfoBufSize);
USHORT APIENTRY DosSetMaxFH(USHORT NumberHandles);
USHORT APIENTRY DosSetPathInfo(PSZ PathName, USHORT PathInfoLevel, PBYTE PathInfoBuf,
                        USHORT PathInfoBufSize, USHORT PathInfoFlags, ULONG Reserved);
USHORT APIENTRY DosSetVerify(USHORT VerifySetting);
USHORT APIENTRY DosShutdown(ULONG Reserved);
USHORT APIENTRY DosWrite(HFILE FileHandle, PVOID BufferArea, USHORT BufferLength, PUSHORT BytesWritten);
USHORT APIENTRY DosWriteAsync(HFILE FileHandle, PULONG RamSemaphore, PUSHORT ReturnCode,
                        PVOID BufferArea, USHORT BufferLength, PUSHORT BytesWritten);

#endif

#ifdef INCL_DOSMEMMGR

#define SEG_GIVEABLE     1
#define SEG_SHAREABLE    2
#define SEG_DISCARDABLE  4

USHORT APIENTRY DosAllocHuge(USHORT NumSeg, USHORT Size, PSEL Selector, USHORT MaxNumSeg, USHORT AllocFlags);
USHORT APIENTRY DosAllocSeg(USHORT Size, PSEL Selector, USHORT AllocFlags);
USHORT APIENTRY DosAllocShrSeg(USHORT Size, PSZ Name, PSEL Selector);
USHORT APIENTRY DosCreateCSAlias(SEL DataSelector, PSEL CodeSelector);
USHORT APIENTRY DosFreeSeg(SEL Selector);
USHORT APIENTRY DosGetHugeShift(PUSHORT ShiftCount);
USHORT APIENTRY DosGetSeg(SEL Selector);
USHORT APIENTRY DosGetShrSeg(PSZ Name, PSEL Selector);
USHORT APIENTRY DosGiveSeg(SEL CallerSegSelector, PID ProcessID, PSEL RecipientSegSelector);
USHORT APIENTRY DosLockSeg(SEL Selector);
USHORT APIENTRY DosMemAvail(PULONG MemAvailSize);
USHORT APIENTRY DosReallocHuge(USHORT NumSeg, USHORT Size, SEL Selector);
USHORT APIENTRY DosReallocSeg(USHORT Size, SEL Selector);
USHORT APIENTRY DosSizeSeg(SEL Selector, PULONG Size);
USHORT APIENTRY DosSubAlloc(SEL SegSelector, PUSHORT BlockOffset, USHORT Size);
USHORT APIENTRY DosSubFree(SEL SegSelector, USHORT BlockOffset, USHORT Size);
USHORT APIENTRY DosSubSet(SEL SegSelector, USHORT Flags, USHORT Size);
USHORT APIENTRY DosUnlockSeg(SEL Selector);

#endif

#ifdef INCL_DOSMODULEMGR

USHORT APIENTRY DosFreeModule(HMODULE ModuleHandle);
USHORT APIENTRY DosGetModHandle(PSZ ModuleName, PHMODULE ModuleHandle);
USHORT APIENTRY DosGetModName(HMODULE ModuleHandle, USHORT BufferLength, PCHAR Buffer);
USHORT APIENTRY DosGetProcAddr(HMODULE ModuleHandle, PSZ ProcName, PFN FAR *ProcAddress);
USHORT APIENTRY DosLoadModule(PSZ ObjNameBuf, USHORT ObjNameBufL, PSZ ModuleName,
                        PHMODULE ModuleHandle);

#endif

#ifdef INCL_DOSNMPIPES

typedef SHANDLE     HPIPE;
typedef HPIPE FAR   *PHPIPE;

USHORT APIENTRY DosCallNmPipe(PSZ FileName, PBYTE InBuffer, USHORT InBufferLen, PBYTE OutBuffer,
                        USHORT OutBufferLen, PUSHORT BytesOut, ULONG TimeOut);
USHORT APIENTRY DosConnectNmPipe(HPIPE Handle);
USHORT APIENTRY DosDisConnectNmPipe(HPIPE Handle);
USHORT APIENTRY DosMakeNmPipe(PSZ PipeName, PHPIPE PipeHandle, USHORT OpenMode, USHORT PipeMode,
                        USHORT OutBufSize, USHORT InBufSize, ULONG TimeOut);
USHORT APIENTRY DosPeekNmPipe(HPIPE Handle, PBYTE Buffer, USHORT BufferLen, PUSHORT BytesRead,
                        PUSHORT BytesAvail, PUSHORT PipeState);
USHORT APIENTRY DosQNmPHandState(HPIPE Handle, PUSHORT PipeHandleState);
USHORT APIENTRY DosQNmPipeInfo(HPIPE Handle, USHORT InfoLevel, PBYTE InfoBuf, USHORT InfoBufSize);
USHORT APIENTRY DosQNmPipeSemState(HSEM SemHandle, PBYTE InfoBuf, USHORT InfoBufLen);
USHORT APIENTRY DosSetNmPHandState(HPIPE Handle, USHORT PipeHandleState);
USHORT APIENTRY DosSetNmPipeSem(HPIPE Handle, HSEM SemHandle, USHORT KeyHandle);
USHORT APIENTRY DosTransactNmPipe(HPIPE Handle, PBYTE InBuffer, USHORT InBufferLen, PBYTE OutBuffer,
                        USHORT OutBufferLen, PUSHORT BytesOut);
USHORT APIENTRY DosWaitNmPipe(PSZ FileName, ULONG TimeOut);

#endif

#ifdef INCL_DOSQUEUES

typedef SHANDLE     HQUEUE;
typedef HQUEUE FAR  *PHQUEUE;

USHORT APIENTRY DosCloseQueue(HQUEUE QueueHandle);
USHORT APIENTRY DosCreateQueue(PHQUEUE RWHandle, USHORT QueuePrty, PSZ QueueName);
USHORT APIENTRY DosMakePipe(PHFILE ReadHandle, PHFILE WriteHandle, USHORT PipeSize);
USHORT APIENTRY DosOpenQueue(PUSHORT OwnerPID, PHQUEUE QueueHandle, PSZ QueueName);
USHORT APIENTRY DosPeekQueue(HQUEUE QueueHandle, PULONG Request, PUSHORT DataLength,
                        PULONG DataAddress, PUSHORT ElementCode, UCHAR NoWait,
                        PBYTE ElemPriority, ULONG SemaphoreHandle);
USHORT APIENTRY DosPurgeQueue(HQUEUE QueueHandle);
USHORT APIENTRY DosQueryQueue(HQUEUE QueueHandle, PUSHORT NumberElements);
USHORT APIENTRY DosReadQueue(HQUEUE QueueHandle, PULONG Request, PUSHORT DataLength,
                        PULONG DataAddress, USHORT ElementCode, UCHAR NoWait,
                        PBYTE ElemPriority, HSEM SemaphoreHandle);
USHORT APIENTRY DosWriteQueue(HQUEUE QueueHandle, USHORT Request, USHORT DataLength,
                        PBYTE DataBuffer, UCHAR ElemPriority);

#endif

#ifdef INCL_DOSSEMAPHORES

typedef LHANDLE     HSYSSEM;
typedef HSYSSEM FAR *PHSYSSEM;

typedef struct _DOSFSRSEM {
    USHORT cb;
    PID    pid;
    TID    tid;
    USHORT cUsage;
    USHORT client;
    ULONG  sem;
} DOSFSRSEM, FAR *PDOSFSRSEM;

typedef struct _MUXSEM {
    USHORT zero;
    HSEM   hsem;
} MUXSEM, FAR *PMUXSEM;

typedef struct _MUXSEMLIST {
    USHORT  cmxs;
    MUXSEM  amxs[16];
} MUXSEMLIST, FAR *PMUXSEMLIST;

USHORT APIENTRY DosCloseSem(HSEM SemHandle);
USHORT APIENTRY DosCreateSem(USHORT NoExclusive, PHSYSSEM SemHandle, PSZ SemName);
USHORT APIENTRY DosFSRamSemClear(PDOSFSRSEM FSRamSemStructure);
USHORT APIENTRY DosFSRamSemRequest(PDOSFSRSEM FSRamSemStructure, LONG Timeout);
USHORT APIENTRY DosMuxSemWait(PUSHORT IndexNbr, PVOID ListAddr, LONG Timeout);
USHORT APIENTRY DosOpenSem(PHSEM SemHandle, PSZ SemName);
USHORT APIENTRY DosSemClear(HSEM SemHandle);
USHORT APIENTRY DosSemRequest(HSEM SemHandle, LONG Timeout);
USHORT APIENTRY DosSemSet(HSEM SemHandle);
USHORT APIENTRY DosSemSetWait(HSEM SemHandle, LONG Timeout);
USHORT APIENTRY DosSemWait(HSEM SemHandle, LONG Timeout);

#endif

#ifdef INCL_DOSMONITORS

typedef SHANDLE       HMONITOR;
typedef HMONITOR FAR  *PHMONITOR;

USHORT APIENTRY DosMonClose(HMONITOR Handle);
USHORT APIENTRY DosMonOpen(PSZ Devname, PHMONITOR Handle);
USHORT APIENTRY DosMonRead(PBYTE BufferI, UCHAR WaitFlag, PBYTE DataBuffer, PUSHORT Bytecnt);
USHORT APIENTRY DosMonReg(HMONITOR Handle, PBYTE BufferI, PBYTE BufferO, USHORT Posflag, USHORT Index);
USHORT APIENTRY DosMonWrite(PBYTE BufferO, PBYTE DataBuffer, USHORT Bytecnt);

#endif

#ifdef INCL_DOSINFOSEG

typedef struct _GINFOSEG {
    ULONG   time;
    ULONG   msecs;
    UCHAR   hour;
    UCHAR   minutes;
    UCHAR   seconds;
    UCHAR   hundredths;
    USHORT  timezone;
    USHORT  cusecTimerInterval;
    UCHAR   day;
    UCHAR   month;
    USHORT  year;
    UCHAR   weekday;
    UCHAR   uchMajorVersion;
    UCHAR   uchMinorVersion;
    UCHAR   chRevisionLetter;
    UCHAR   sgCurrent;
    UCHAR   sgMax;
    UCHAR   cHugeShift;
    UCHAR   fProtectModeOnly;
    USHORT  pidForeground;
    UCHAR   fDynamicSched;
    UCHAR   csecMaxWait;
    USHORT  cmsecMinSlice;
    USHORT  cmsecMaxSlice;
    USHORT  bootdrive;
    UCHAR   amecRAS[32];
    UCHAR   csgWindowableVioMax;
    UCHAR   csgPMMax;
} GINFOSEG;

typedef struct _LINFOSEG {
    PID     pidCurrent;
    PID     pidParent;
    USHORT  prtyCurrent;
    TID     tidCurrent;
    USHORT  sgCurrent;
    UCHAR   rfProcStatus;
    UCHAR   dummy1;
    BOOL    fForeground;
    UCHAR   typeProcess;
    UCHAR   dummy2;
    SEL     selEnvironment;
    USHORT  offCmdLine;
    USHORT  cbDataSegment;
    USHORT  cbStack;
    USHORT  cbHeap;
    HMODULE hmod;
    SEL     selDS;
} LINFOSEG;

USHORT APIENTRY DosGetInfoSeg(PSEL GlobalSeg, PSEL LocalSeg);

#endif

#ifdef INCL_DOSNLS

typedef struct _COUNTRYCODE {
    USHORT country;
    USHORT codepage;
} COUNTRYCODE, FAR *PCOUNTRYCODE;

typedef struct _COUNTRYINFO {
    USHORT country;
    USHORT codepage;
    USHORT fsDateFmt;
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
} COUNTRYINFO, FAR *PCOUNTRYINFO;

USHORT APIENTRY DosCaseMap(USHORT Length, PCOUNTRYCODE Structure, PCHAR BinaryString);
USHORT APIENTRY DosGetCollate(USHORT Length, PCOUNTRYCODE Structure, PCHAR MemoryBuffer, PUSHORT DataLength);
USHORT APIENTRY DosGetCp(USHORT Length, PUSHORT CodePageList, PUSHORT DataLength);
USHORT APIENTRY DosGetCtryInfo(USHORT Length, PCOUNTRYCODE Structure, PCOUNTRYINFO MemoryBuffer, PUSHORT DataLength);
USHORT APIENTRY DosGetDBCSEv(USHORT Length, PCOUNTRYCODE Structure, PCHAR MemoryBuffer);
USHORT APIENTRY DosSetCp(USHORT CodePage, USHORT Reserved);
USHORT APIENTRY DosSetProcCp(USHORT CodePage, USHORT Reserved);

#endif

#ifdef INCL_DOSSIGNALS

typedef VOID (PASCAL FAR *PFNSIGHANDLER)(USHORT, USHORT);

USHORT APIENTRY DosFlagProcess(PID ProcessID, USHORT ActionCode, USHORT Flagnum, USHORT Flagarg);
USHORT APIENTRY DosHoldSignal(USHORT ActionCode);
USHORT APIENTRY DosSendSignal(USHORT PID, USHORT SigNumber);
USHORT APIENTRY DosSetSigHandler(PFNSIGHANDLER Routine, PFNSIGHANDLER FAR *PrevAddress,
                        PUSHORT PrevAction, USHORT Action, USHORT SigNumber);

#endif

#ifdef INCL_DOSRESOURCES

USHORT APIENTRY DosFreeResource(PBYTE ResAddr);
USHORT APIENTRY DosGetResource(HMODULE ModHandle, USHORT TypeID, USHORT NameID, PSEL Selector);
USHORT APIENTRY DosGetResource2(HMODULE ModHandle, USHORT TypeID, USHORT NameID, PULONG ResAddr);

#endif

#ifdef INCL_DOSDATETIME

typedef SHANDLE     HTIMER;
typedef HTIMER FAR  *PHTIMER;

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
} DATETIME, FAR *PDATETIME;

USHORT APIENTRY DosGetDateTime(PDATETIME DateTime);
USHORT APIENTRY DosSetDateTime(PDATETIME DateTime);
USHORT APIENTRY DosTimerAsync(ULONG TimeInterval, HSEM SemHandle, PHTIMER Handle);
USHORT APIENTRY DosTimerStart(ULONG TimeInterval, HSEM SemHandle, PHTIMER Handle);
USHORT APIENTRY DosTimerStop(HTIMER Handle);

#endif

#ifdef INCL_DOSMISC

USHORT APIENTRY DosErrClass(USHORT Code, PUSHORT Class, PUSHORT Action, PUSHORT Locus);
USHORT APIENTRY DosError(USHORT Flag);
USHORT APIENTRY DosGetEnv(PUSHORT EnvSegment, PUSHORT CmdOffset);
USHORT APIENTRY DosGetMachineMode(PBYTE MachineMode);
USHORT APIENTRY DosGetMessage(PCHAR FAR *IvTable, USHORT IvCount, PCHAR DataArea, USHORT DataLength,
                        USHORT MsgNumber, PSZ FileName, PUSHORT MsgLength);
USHORT APIENTRY DosGetVersion(PUSHORT VersionWord);
USHORT APIENTRY DosInsMessage(PCHAR FAR *IvTable, USHORT IvCount, PSZ MsgInput, USHORT MsgInLength,
                        PCHAR DataArea, USHORT DataLength, PUSHORT MsgLength);
USHORT APIENTRY DosPutMessage(USHORT FileHandle, USHORT MessageLength, PCHAR MessageBuffer);
USHORT APIENTRY DosPtrace(PBYTE PtraceB);
USHORT APIENTRY DosQSysInfo(USHORT Index, PBYTE DataBuf, USHORT DataBufLen);
USHORT APIENTRY DosScanEnv(PSZ EnvVarName, PSZ FAR *ResultPointer);
USHORT APIENTRY DosSearchPath(USHORT Control, PSZ PathRef, PSZ FileName, PBYTE ResultBuffer,
                        USHORT ResultBufferLen);
USHORT APIENTRY DosSetVec(USHORT VecNum, PFN Routine, PFN PrevAddress);

#endif

#ifdef INCL_DOSSESMGR

typedef struct _STATUSDATA {
  USHORT Length;
  USHORT SelectInd;
  USHORT BondInd;
} STATUSDATA, FAR *PSTATUSDATA;

typedef struct _STARTDATA {
    USHORT cb;
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
} STARTDATA, FAR *PSTARTDATA;

USHORT APIENTRY DosQAppType(PSZ ExecutableFileName, PUSHORT AppType);
USHORT APIENTRY DosSelectSession(USHORT SessID, ULONG Reserved);
USHORT APIENTRY DosSetSession(USHORT SessID, PSTATUSDATA StatusData);
USHORT APIENTRY DosStartSession(PSTARTDATA StartData, PUSHORT SessID, PUSHORT PID);
USHORT APIENTRY DosStopSession(USHORT TargetOption, USHORT SessID, ULONG Reserved);

#endif

#ifdef INCL_DOSDEVICES

VOID   APIENTRY DosCallback(PFN Ring3Routine);
USHORT APIENTRY DosCLIAccess(VOID);
USHORT APIENTRY DosDevConfig(PVOID DeviceInfo, USHORT Item, USHORT Parm);
USHORT APIENTRY DosDevIOCtl(PVOID Data, PVOID ParmList, USHORT Function, USHORT Category, HFILE DevHandle);
USHORT APIENTRY DosDevIOCtl2(PVOID Data, USHORT DataLength, PVOID ParmList, USHORT ParmListLength,
                        USHORT Function, USHORT Category, HFILE DevHandle);
USHORT APIENTRY DosPhysicalDisk(USHORT Function, PBYTE DataPtr, USHORT DataLen, PBYTE ParmPtr, USHORT ParmLen);
USHORT APIENTRY DosPortAccess(USHORT Reserved, USHORT TypeOfAccess, USHORT FirstPort, USHORT LastPort);
USHORT APIENTRY DosR2StackRealloc(USHORT NewSize);

#endif

