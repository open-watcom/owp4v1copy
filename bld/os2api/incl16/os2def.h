/*
    OS/2 typedefs and constants for 16-bit development.
*/


#if (defined(M_I86SM) || defined(M_I86MM))
#define NULL   0
#else
#define NULL   0L
#endif

#define FAR     _far
#define NEAR    _near
#define PASCAL  _pascal
#define VOID    void

#define EXPENTRY  PASCAL FAR _loadds
#define APIENTRY  PASCAL FAR

#define CHAR     char
#define SHORT    short
#define LONG     long
#define INT      int

typedef unsigned char   UCHAR;
typedef unsigned char   BYTE;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;

typedef unsigned short  SHANDLE;
typedef void _far       *LHANDLE;

typedef SHANDLE    HFILE;
typedef HFILE FAR  *PHFILE;

typedef unsigned char FAR   *PSZ;
typedef unsigned char NEAR  *NPSZ;

typedef unsigned char FAR   *PCH;
typedef unsigned char NEAR  *NPCH;

typedef unsigned short  BOOL;
typedef BOOL FAR        *PBOOL;

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE  0
#endif

typedef CHAR FAR     *PCHAR;
typedef SHORT FAR    *PSHORT;
typedef INT FAR      *PINT;
typedef LONG FAR     *PLONG;
typedef UCHAR FAR    *PUCHAR;
typedef UCHAR FAR    *PBYTE;
typedef USHORT FAR   *PUSHORT;
typedef UINT FAR     *PUINT;
typedef ULONG FAR    *PULONG;

typedef VOID FAR     *PVOID;

typedef int (PASCAL FAR  *PFN)();
typedef int (PASCAL NEAR *NPFN)();
typedef PFN FAR *PPFN;

typedef USHORT       SEL;
typedef SEL FAR      *PSEL;

typedef USHORT       HMODULE;
typedef HMODULE FAR  *PHMODULE;
typedef USHORT       PID;
typedef PID FAR      *PPID;
typedef USHORT       TID;
typedef TID FAR      *PTID;
typedef VOID FAR     *HSEM;
typedef HSEM FAR     *PHSEM;
