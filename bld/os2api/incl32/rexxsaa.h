/*
    OS/2 REXX programming interface include file.
*/

#ifndef __REXXSAA_H__
#define __REXXSAA_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OS2_INCLUDED
#include <os2.h>
#endif

#if (defined(__IBMC__) || defined(__WATCOMC__)) && !defined(REXX_NODEFLIB)
#pragma library("rexx.lib")
#endif

#define RXCOMMAND    0
#define RXSUBROUTINE 1
#define RXFUNCTION   2

#define RXAUTOBUFLEN 256

#define RXFUNC_DYNALINK  1
#define RXFUNC_CALLENTRY 2

#define RXFUNC_OK         0
#define RXFUNC_DEFINED   10
#define RXFUNC_NOMEM     20
#define RXFUNC_NOTREG    30
#define RXFUNC_MODNOTFND 40
#define RXFUNC_ENTNOTFND 50
#define RXFUNC_NOTINIT   60
#define RXFUNC_BADTYPE   70

#define RXSUBCOM_DROPPABLE 0
#define RXSUBCOM_NONDROP   1

#define RXSUBCOM_OK          0
#define RXSUBCOM_ISREG       1
#define RXSUBCOM_ERROR       1
#define RXSUBCOM_FAILURE     2
#define RXSUBCOM_DUP        10
#define RXSUBCOM_MAXREG     20
#define RXSUBCOM_NOTREG     30
#define RXSUBCOM_NOCANDROP  40
#define RXSUBCOM_LOADERR    50
#define RXSUBCOM_NOPROC    127
#define RXSUBCOM_BADENTRY 1001
#define RXSUBCOM_NOEMEM   1002
#define RXSUBCOM_BADTYPE  1003
#define RXSUBCOM_NOTINIT  1004

#define RXENDLST 0
#define RXFNC    2
#define RXFNCCAL 1
#define RXCMD    3
#define RXCMDHST 1
#define RXMSQ    4
#define RXMSQPLL 1
#define RXMSQPSH 2
#define RXMSQSIZ 3
#define RXMSQNAM 20
#define RXSIO    5
#define RXSIOSAY 1
#define RXSIOTRC 2
#define RXSIOTRD 3
#define RXSIODTR 4
#define RXSIOTLL 5
#define RXHLT    7
#define RXHLTCLR 1
#define RXHLTTST 2
#define RXTRC    8
#define RXTRCTST 1
#define RXINI    9
#define RXINIEXT 1
#define RXTER    10
#define RXTEREXT 1
#define RXNOOFEXITS 11

#define RXSHV_SET   0
#define RXSHV_FETCH 1
#define RXSHV_DROPV 2
#define RXSHV_SYSET 3
#define RXSHV_SYFET 4
#define RXSHV_SYDRO 5
#define RXSHV_NEXTV 6
#define RXSHV_PRIV  7
#define RXSHV_EXIT  8
#define RXSHV_NOAVL 144

#define RXSHV_OK    0x00
#define RXSHV_NEWV  0x01
#define RXSHV_LVAR  0x02
#define RXSHV_TRUNC 0x04
#define RXSHV_BADN  0x08
#define RXSHV_MEMFL 0x10
#define RXSHV_BADF  0x80

#define MAKERXSTRING(r,p,l) {(r).strptr=(PCH)p;(r).strlength=(ULONG)l;}
#define RXNULLSTRING(r)     (!(r).strptr)
#define RXSTRLEN(r)         (RXNULLSTRING(r)?0L:(r).strlength)
#define RXSTRPTR(r)         (r).strptr
#define RXVALIDSTRING(r)    ((r).strptr && (r).strlength)
#define RXZEROLENSTRING(r)  ((r).strptr && !(r).strlength)

#define RXEXIT_HANDLED       0
#define RXEXIT_NOT_HANDLED   1
#define RXEXIT_RAISE_ERROR (-1)

#define RXEXIT_ISREG         1
#define RXEXIT_ERROR         1
#define RXEXIT_FAILURE       2
#define RXEXIT_BADENTRY   1001
#define RXEXIT_NOEMEM     1002
#define RXEXIT_BADTYPE    1003
#define RXEXIT_NOTINIT    1004

#define RXEXIT_OK            0
#define RXEXIT_DUP          10
#define RXEXIT_MAXREG       20
#define RXEXIT_NOTREG       30
#define RXEXIT_NOCANDROP    40
#define RXEXIT_LOADERR      50
#define RXEXIT_NOPROC      127

typedef PUCHAR PEXIT;

typedef struct {
    ULONG strlength;
    PCH   strptr;
} RXSTRING, *PRXSTRING;

typedef struct _SHVBLOCK {
    struct _SHVBLOCK *shvnext;
    RXSTRING         shvname;
    RXSTRING         shvvalue;
    ULONG            shvnamelen;
    ULONG            shvvaluelen;
    UCHAR            shvcode;
    UCHAR            shvret;
} SHVBLOCK, *PSHVBLOCK;

typedef struct _RXSYSEXIT {
    PSZ  sysexit_name;
    LONG sysexit_code;
} RXSYSEXIT, *PRXSYSEXIT;

typedef struct _RXSIOSAY_PARM {
    RXSTRING    rxsio_string;
} RXSIOSAY_PARM;

typedef struct _RXSIOTRC_PARM {
    RXSTRING    rxsio_string;
} RXSIOTRC_PARM;

typedef struct _RXSIOTRD_PARM {
    RXSTRING    rxsiotrd_retc;
} RXSIOTRD_PARM;

typedef struct _RXSIODTR_PARM {
    RXSTRING    rxsiodtr_retc;
} RXSIODTR_PARM;

typedef ULONG APIENTRY RexxSubcomHandler(PRXSTRING,PUSHORT,PRXSTRING);
typedef ULONG APIENTRY RexxFunctionHandler(PUCHAR,ULONG,PRXSTRING,PSZ,PRXSTRING);

APIRET APIENTRY RexxDeregisterExit(PCSZ,PCSZ);
APIRET APIENTRY RexxDeregisterFunction(PCSZ);
APIRET APIENTRY RexxDeregisterSubcom(PCSZ,PCSZ);
APIRET APIENTRY RexxQueryFunction(PCSZ);
APIRET APIENTRY RexxQuerySubcom(PCSZ,PCSZ,PUSHORT,PUCHAR);
APIRET APIENTRY RexxRegisterExitExe(PCSZ,PFN,PUCHAR);
APIRET APIENTRY RexxRegisterFunctionDll(PCSZ,PCSZ,PCSZ);
APIRET APIENTRY RexxRegisterFunctionExe(PCSZ,PFN);

APIRET APIENTRY RexxRegisterSubcomDll(PCSZ,PCSZ,PCSZ,PUCHAR,ULONG);
APIRET APIENTRY RexxRegisterSubcomExe(PCSZ,PFN,PUCHAR);
APIRET APIENTRY RexxResetTrace(PID,TID);
APIRET APIENTRY RexxSetHalt(PID,TID);
APIRET APIENTRY RexxSetTrace(PID,TID);

LONG   APIENTRY RexxStart(LONG,PRXSTRING,PCSZ,PRXSTRING,PCSZ,LONG,PRXSYSEXIT,PSHORT,PRXSTRING);
APIRET APIENTRY RexxVariablePool(PSHVBLOCK);

#ifdef __cplusplus
};
#endif

#endif
