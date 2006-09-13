/*
    Include file for Windows 3.x development.
*/

#if !defined(_ENABLE_AUTODEPEND)
  #pragma read_only_file;
#endif

#ifdef _WINDOWS_16_
#include <win16.h>
#else
#include <_win386.h>
#endif
