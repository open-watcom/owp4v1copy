#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED_
:segment CNAME
namespace std {
typedef long char wchar_t;
}
:elsesegment
#ifdef __cplusplus
typedef long char wchar_t;
#else
typedef unsigned short wchar_t;
#endif
:endsegment
#endif
