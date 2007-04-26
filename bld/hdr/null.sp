#ifndef NULL
#if defined( __SMALL__ ) || defined( __MEDIUM__ ) || defined( __386__ ) || defined( __AXP__ ) || defined( __PPC__ )
#define                         NULL                    0
#else
#define                         NULL                    0L
#endif
#endif
:: New code to be used after version 1.7
::
::#ifndef NULL
::  #ifdef __cplusplus
::    #define NULL 0
::  #else
::    #define NULL ((void *)0)
::  #endif
::#endif
