#ifndef _NLSCMPERROR
:segment CNAME
 #include <climits>
:elsesegment
 #include <limits.h>
:endsegment
 #define _NLSCMPERROR INT_MAX
#endif
