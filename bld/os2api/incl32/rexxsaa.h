/*
    OS/2 REXX programming interface include file.
*/

#ifndef __REXXSAA_H__
#define __REXXSAA_H__

#define RXCOMMAND    0
#define RXSUBROUTINE 1
#define RXFUNCTION   2

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

#define MAKERXSTRING(r,p,l) {(r).strptr=(PCH)p;(r).strlength=(ULONG)l;}
#define RXNULLSTRING(r)     (!(r).strptr)
#define RXSTRLEN(r)         (RXNULLSTRING(r)?0L:(r).strlength)
#define RXSTRPTR(r)         (r).strptr
#define RXVALIDSTRING(r)    ((r).strptr && (r).strlength)
#define RXZEROLENSTRING(r)  ((r).strptr && !(r).strlength)

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

typedef ULONG RexxSubcomHandler(PRXSTRING, PUSHORT, PRXSTRING);
typedef ULONG RexxFunctionHandler(PUCHAR, ULONG, PRXSTRING, PSZ, PRXSTRING);

APIRET APIENTRY RexxDeregisterFunction(PCSZ);
APIRET APIENTRY RexxDeregisterSubcom(PCSZ, PCSZ);
APIRET APIENTRY RexxQueryFunction(PCSZ);
APIRET APIENTRY RexxQuerySubcom(PCSZ, PCSZ, PUSHORT, PUCHAR);
APIRET APIENTRY RexxRegisterFunctionDll(PCSZ, PCSZ, PCSZ);
APIRET APIENTRY RexxRegisterFunctionExe(PCSZ, PFN);

APIRET APIENTRY RexxRegisterSubcomDll(PCSZ, PCSZ, PCSZ, PUCHAR, ULONG);
APIRET APIENTRY RexxRegisterSubcomExe(PCSZ, PFN, PUCHAR);
APIRET APIENTRY RexxResetTrace(PID, TID);
APIRET APIENTRY RexxSetHalt(PID, TID);
APIRET APIENTRY RexxSetTrace(PID, TID);

LONG   APIENTRY RexxStart(LONG, PRXSTRING, PCSZ, PRXSTRING,
                   PCSZ, LONG, PRXSYSEXIT, PSHORT, PRXSTRING);
APIRET APIENTRY RexxVariablePool(PSHVBLOCK);

#endif

