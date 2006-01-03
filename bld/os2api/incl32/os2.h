/*
    OS/2 top level include file for 32-bit development.
*/


#ifndef __OS2_H__
#define __OS2_H__

#define OS2_INCLUDED

#ifdef _M_I86
#error This os2.h is for 32-bit development only!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <os2def.h>
#include <bse.h>
#include <pm.h>

#ifdef __cplusplus
}
#endif

#endif

