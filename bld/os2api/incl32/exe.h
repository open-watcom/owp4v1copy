/*
    DOS executable (MZ) format.
*/

#ifndef __EXE__
#define __EXE__

#ifdef __cplusplus
extern "C" {
#endif

#define EXEID   0x5a4d

#ifndef BYTE
typedef unsigned char   BYTE;
#endif

#ifndef WORD
typedef unsigned short  WORD;
#endif

typedef WORD    bbmask;

struct exe {
    WORD    eid;
    WORD    elast;
    WORD    epagsiz;
    WORD    erelcnt;
    WORD    ehdrsiz;
    WORD    eminfre;
    WORD    emaxfre;
    WORD    eiSS;
    WORD    eiSP;
    WORD    enegsum;
    WORD    eiIP;
    WORD    eiCS;
    WORD    ereloff;
    WORD    eovlnum;
    WORD    ever;
    WORD    dumy;
    bbmask  ebb;
    WORD    dumy2[7];
};

#ifdef __cplusplus
}
#endif

#endif
