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
    #define INCL_DOSNLS
    #define INCL_DOSNMPIPES
    #define INCL_DOSPROCESS
    #define INCL_DOSQUEUES
    #define INCL_DOSRESOURCES
    #define INCL_DOSSEMAPHORES
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

#endif

#ifdef INCL_DOSQUEUES

typedef SHANDLE         HQUEUE;
typedef HQUEUE FAR      *PHQUEUE;

USHORT APIENTRY DosCloseQueue(HQUEUE QueueHandle);
USHORT APIENTRY DosCreateQueue(PHQUEUE RWHandle, USHORT QueuePrty, PSZ QueueName);

#endif

#ifdef INCL_DOSSEMAPHORES

typedef LHANDLE         HSYSSEM;
typedef HSYSSEM FAR     *PHSYSSEM;

typedef struct _DOSFSRSEM {
    USHORT cb;
    PID    pid;
    TID    tid;
    USHORT cUsage;
    USHORT client;
    ULONG  sem;
} DOSFSRSEM, FAR *PDOSFSRSEM;

USHORT APIENTRY DosCloseSem(HSEM SemHandle);
USHORT APIENTRY DosCreateSem(USHORT NoExclusive, PHSYSSEM SemHandle, PSZ SemName);
USHORT APIENTRY DosFSRamSemClear(PDOSFSRSEM FSRamSemStructure);
USHORT APIENTRY DosFSRamSemRequest(PDOSFSRSEM FSRamSemStructure, LONG Timeout);

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

#endif

#ifdef INCL_DOSSIGNALS

USHORT APIENTRY DosFlagProcess(PID ProcessID, USHORT ActionCode, USHORT Flagnum, USHORT Flagarg);
USHORT APIENTRY DosHoldSignal(USHORT ActionCode);

#endif

#ifdef INCL_DOSRESOURCES

USHORT APIENTRY DosFreeResource(PBYTE ResAddr);
USHORT APIENTRY DosGetResource(HMODULE ModHandle, USHORT TypeID, USHORT NameID, PSEL Selector);
USHORT APIENTRY DosGetResource2(HMODULE ModHandle, USHORT TypeID, USHORT NameID, PULONG ResAddr);

#endif

#ifdef INCL_DOSDATETIME

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

#endif

#ifdef INCL_DOSDEVICES

VOID   APIENTRY DosCallback(PFN Ring3Routine);
USHORT APIENTRY DosCLIAccess(VOID);
USHORT APIENTRY DosDevConfig(PVOID DeviceInfo, USHORT Item, USHORT Parm);
USHORT APIENTRY DosDevIOCtl(PVOID Data, PVOID ParmList, USHORT Function, USHORT Category, HFILE DevHandle);
USHORT APIENTRY DosDevIOCtl2(PVOID Data, USHORT DataLength, PVOID ParmList, USHORT ParmListLength,
                        USHORT Function, USHORT Category, HFILE DevHandle);

#endif

