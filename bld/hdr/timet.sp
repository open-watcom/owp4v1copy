#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
#define _TIME_T_DEFINED_
:segment QNX | LINUX
typedef signed long             time_t; /* time value */
:elsesegment
typedef unsigned long           time_t; /* time value */
:endsegment
#endif
