#ifndef _LMUSEFLG_H
#define _LMUSEFLG_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#define USE_NOFORCE 0
#define USE_FORCE 1
#define USE_LOTS_OF_FORCE 2
#endif 

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

