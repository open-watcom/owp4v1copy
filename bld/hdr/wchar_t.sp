:segment CNAME
#ifndef _STDWCHAR_T_DEFINED
#define _STDWCHAR_T_DEFINED
namespace std {
typedef long char wchar_t;
}
#endif
:elsesegment
#ifdef __cplusplus
#ifndef _STDWCHAR_T_DEFINED
#define _STDWCHAR_T_DEFINED
namespace std {
typedef long char wchar_t;
}
#endif
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED_
using std::wchar_t;
#endif
#else
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED_
typedef unsigned short wchar_t;
#endif
#endif
:endsegment
